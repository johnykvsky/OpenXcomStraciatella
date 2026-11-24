/*
 * Copyright 2010-2016 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#include "InteractiveReactionBState.h"
#include "InteractiveReactionState.h"
#include "BattlescapeGame.h"
#include "BattlescapeState.h"
#include "ProjectileFlyBState.h"
#include "Map.h"
#include "Camera.h"
#include "../Engine/Game.h"
#include "../Savegame/SavedGame.h"
#include "../Savegame/SavedBattleGame.h"
#include "../Savegame/BattleUnit.h"
#include "../Savegame/BattleItem.h"
#include "../Mod/Mod.h"
#include "../Mod/RuleItem.h"
#include "../Mod/RuleInventory.h"
#include "../Savegame/HitLog.h"
#include "TileEngine.h"

namespace OpenXcom
{

InteractiveReactionBState::InteractiveReactionBState(BattlescapeGame *parent, BattleAction action, BattleUnit *target)
	: BattleState(parent, action), _target(target), _promptShown(false), _chosenAction(-1),
	  _grenadeItem(nullptr), _grenadeFromBelt(false), _grenadeNeedPrime(false), _grenadeCost(0)
{
	if (_action.actor && _parent->getMod()->isInteractiveReactionEnabled())
	{
		for (auto* bi : *_action.actor->getInventory())
		{
			if (bi->getRules()->isGrenadeOrProxy())
			{
				auto* slot = bi->getSlot();
				if (slot && (slot->isRightHand() || slot->isLeftHand()))
				{
					_grenadeItem = bi;
					_grenadeFromBelt = false;
					break;
				}
			}
		}

		if (!_grenadeItem && _parent->getMod()->isReactionAllowBeltGrenade())
		{
			bool hasFreeHand = (_action.actor->getRightHandWeapon() == nullptr || _action.actor->getLeftHandWeapon() == nullptr);
			if (hasFreeHand)
			{
				for (auto* bi : *_action.actor->getInventory())
				{
					if (bi->getRules()->isGrenadeOrProxy())
					{
						auto* slot = bi->getSlot();
						if (slot && slot->getId() == "STR_BELT")
						{
							_grenadeItem = bi;
							_grenadeFromBelt = true;
							break;
						}
					}
				}
			}
		}

		if (_grenadeItem)
		{
			int cost = 0;
			if (_grenadeFromBelt) cost += 8;
			if (!_grenadeItem->isFuseEnabled() || _grenadeItem->getFuseTimer() < 0)
			{
				_grenadeNeedPrime = true;
				cost += 15;
			}
			BattleAction throwAct;
			throwAct.actor = _action.actor;
			throwAct.weapon = _grenadeItem;
			throwAct.type = BA_THROW;
			throwAct.target = _target->getPosition();
			throwAct.updateTU();
			cost += throwAct.Time;
			_grenadeCost = cost;

			if (_action.actor->getTimeUnits() < _grenadeCost ||
				!_target->getTile() ||
				!ProjectileFlyBState::validThrowRange(&throwAct, _parent->getTileEngine()->getOriginVoxel(throwAct, 0), _target->getTile(), _parent->getSave()->getDepth()))
			{
				_grenadeItem = nullptr;
				_grenadeCost = 0;
			}
		}
	}
}

InteractiveReactionBState::~InteractiveReactionBState()
{
}

void InteractiveReactionBState::init()
{
}

bool InteractiveReactionBState::hasAvailableActions() const
{
	if (!_action.actor || !_target || !_target->getTile()) return false;
	BattleUnit *actor = _action.actor;
	BattleItem *weapon = _action.weapon;
	int distSq = actor->distance3dToPositionSq(_target->getPosition());

	if (weapon && _parent->getSave()->canUseWeapon(weapon, actor, false, BA_SNAPSHOT)
		&& !weapon->getRules()->isOutOfRange(distSq)
		&& weapon->getAmmoForAction(BA_SNAPSHOT)
		&& BattleActionCost(BA_SNAPSHOT, actor, weapon).haveTU()
		&& actor->getTimeUnits() >= BattleActionCost(BA_SNAPSHOT, actor, weapon).Time
		&& BattleActionCost(BA_SNAPSHOT, actor, weapon).Time > 0)
	{
		return true;
	}

	if (weapon && _parent->getSave()->canUseWeapon(weapon, actor, false, BA_AUTOSHOT)
		&& !weapon->getRules()->isOutOfRange(distSq)
		&& weapon->getAmmoForAction(BA_AUTOSHOT)
		&& BattleActionCost(BA_AUTOSHOT, actor, weapon).haveTU()
		&& actor->getTimeUnits() >= BattleActionCost(BA_AUTOSHOT, actor, weapon).Time
		&& BattleActionCost(BA_AUTOSHOT, actor, weapon).Time > 0)
	{
		return true;
	}

	if (weapon && _parent->getSave()->canUseWeapon(weapon, actor, false, BA_AIMEDSHOT)
		&& !weapon->getRules()->isOutOfRange(distSq)
		&& weapon->getAmmoForAction(BA_AIMEDSHOT)
		&& BattleActionCost(BA_AIMEDSHOT, actor, weapon).haveTU()
		&& actor->getTimeUnits() >= BattleActionCost(BA_AIMEDSHOT, actor, weapon).Time
		&& BattleActionCost(BA_AIMEDSHOT, actor, weapon).Time > 0)
	{
		return true;
	}

	if (_grenadeItem && _grenadeCost > 0 && actor->getTimeUnits() >= _grenadeCost)
	{
		return true;
	}

	return false;
}

void InteractiveReactionBState::think()
{
	if (!_target || _target->isOut() || _target->getTile() == nullptr || !_action.actor || _action.actor->isOut())
	{
		_parent->popState();
		return;
	}

	if (!hasAvailableActions())
	{
		// No action available that the unit has enough TUs for
		_parent->popState();
		return;
	}

	if (!_promptShown)
	{
		_promptShown = true;
		_parent->getMap()->getCamera()->centerOnPosition(_action.actor->getPosition());
		_parent->getSave()->getBattleState()->getGame()->pushState(new InteractiveReactionState(_parent, &_action, _target, this));
		return;
	}

	if (_chosenAction == BA_NONE || _chosenAction == -1)
	{
		// Player chose to hold fire
		_parent->popState();
		return;
	}

	if (_chosenAction == BA_THROW && _grenadeItem)
	{
		if (_grenadeFromBelt)
		{
			_action.actor->spendTimeUnits(8);
			const RuleInventory *freeHand = _parent->getMod()->getInventory("STR_RIGHT_HAND");
			if (_action.actor->getItem(freeHand))
			{
				freeHand = _parent->getMod()->getInventory("STR_LEFT_HAND");
			}
			if (freeHand)
			{
				_action.actor->fitItemToInventory(freeHand, _grenadeItem);
			}
		}
		if (_grenadeNeedPrime)
		{
			_action.actor->spendTimeUnits(15);
			_grenadeItem->setFuseTimer(0);
			_grenadeItem->setFuseEnabled(true);
		}

		BattleAction throwAction;
		throwAction.cameraPosition = _parent->getMap()->getCamera()->getMapOffset();
		throwAction.actor = _action.actor;
		throwAction.weapon = _grenadeItem;
		throwAction.type = BA_THROW;
		throwAction.target = _target->getPosition();
		throwAction.targeting = true;
		throwAction.isReaction = true;
		throwAction.reactionThrow = true;
		throwAction.updateTU();

		_parent->getSave()->appendToHitLog(HITLOG_REACTION_FIRE, _action.actor->getFaction());
		_parent->statePushNext(new ProjectileFlyBState(_parent, throwAction));
		_parent->popState();
		return;
	}

	_action.cameraPosition = _parent->getMap()->getCamera()->getMapOffset();
	_action.type = (BattleActionType)_chosenAction;
	_action.target = _target->getPosition();
	_action.updateTU();
	_action.isReaction = true;
	_action.targeting = true;

	if (_action.haveTU() && _action.weapon && _action.weapon->getAmmoForAction(_action.type))
	{
		_parent->getSave()->appendToHitLog(HITLOG_REACTION_FIRE, _action.actor->getFaction());
		_parent->statePushNext(new ProjectileFlyBState(_parent, _action));
	}
	_parent->popState();
}

}
