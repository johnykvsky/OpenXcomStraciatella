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
#include "InteractiveReactionState.h"
#include "InteractiveReactionBState.h"
#include "BattlescapeGame.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextButton.h"
#include "../Engine/Game.h"
#include "../Mod/Mod.h"
#include "../Mod/RuleItem.h"
#include "../Savegame/SavedGame.h"
#include "../Savegame/SavedBattleGame.h"
#include "../Savegame/BattleUnit.h"
#include "../Savegame/BattleItem.h"
#include "../Engine/Options.h"
#include "../Engine/Action.h"
#include "../Engine/LocalizedText.h"
#include <sstream>
#include <cmath>

namespace OpenXcom
{

InteractiveReactionState::InteractiveReactionState(BattlescapeGame *battleGame, BattleAction *action, BattleUnit *target, InteractiveReactionBState *bstate)
	: _battleGame(battleGame), _action(action), _target(target), _bstate(bstate)
{
	_screen = false;

	BattleUnit *actor = _action->actor;
	BattleItem *weapon = _action->weapon;
	int distSq = (_target && actor) ? actor->distance3dToPositionSq(_target->getPosition()) : 0;

	SDLKey keys[5] = { SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5 };
	int optIndex = 0;

	// 1. Snap Shot
	if (weapon && _battleGame->getSave()->canUseWeapon(weapon, actor, false, BA_SNAPSHOT)
		&& !weapon->getRules()->isOutOfRange(distSq)
		&& weapon->getAmmoForAction(BA_SNAPSHOT)
		&& BattleActionCost(BA_SNAPSHOT, actor, weapon).haveTU()
		&& actor->getTimeUnits() >= BattleActionCost(BA_SNAPSHOT, actor, weapon).Time
		&& BattleActionCost(BA_SNAPSHOT, actor, weapon).Time > 0)
	{
		int cost = BattleActionCost(BA_SNAPSHOT, actor, weapon).Time;
		int acc = BattleUnit::getFiringAccuracy(BattleActionAttack::GetBeforeShoot(BA_SNAPSHOT, actor, weapon), _game->getMod());
		std::ostringstream ss;
		ss << (optIndex + 1) << ": SNAP SHOT (" << cost << " TU, " << acc << "%)";
		_options.push_back({ BA_SNAPSHOT, ss.str(), cost, acc, keys[optIndex++] });
	}

	// 2. Auto Shot
	if (weapon && _battleGame->getSave()->canUseWeapon(weapon, actor, false, BA_AUTOSHOT)
		&& !weapon->getRules()->isOutOfRange(distSq)
		&& weapon->getAmmoForAction(BA_AUTOSHOT)
		&& BattleActionCost(BA_AUTOSHOT, actor, weapon).haveTU()
		&& actor->getTimeUnits() >= BattleActionCost(BA_AUTOSHOT, actor, weapon).Time
		&& BattleActionCost(BA_AUTOSHOT, actor, weapon).Time > 0)
	{
		int cost = BattleActionCost(BA_AUTOSHOT, actor, weapon).Time;
		int acc = BattleUnit::getFiringAccuracy(BattleActionAttack::GetBeforeShoot(BA_AUTOSHOT, actor, weapon), _game->getMod());
		std::ostringstream ss;
		ss << (optIndex + 1) << ": AUTO BURST (" << cost << " TU, " << acc << "%)";
		_options.push_back({ BA_AUTOSHOT, ss.str(), cost, acc, keys[optIndex++] });
	}

	// 3. Aimed Shot
	if (weapon && _battleGame->getSave()->canUseWeapon(weapon, actor, false, BA_AIMEDSHOT)
		&& !weapon->getRules()->isOutOfRange(distSq)
		&& weapon->getAmmoForAction(BA_AIMEDSHOT)
		&& BattleActionCost(BA_AIMEDSHOT, actor, weapon).haveTU()
		&& actor->getTimeUnits() >= BattleActionCost(BA_AIMEDSHOT, actor, weapon).Time
		&& BattleActionCost(BA_AIMEDSHOT, actor, weapon).Time > 0)
	{
		int cost = BattleActionCost(BA_AIMEDSHOT, actor, weapon).Time;
		int acc = BattleUnit::getFiringAccuracy(BattleActionAttack::GetBeforeShoot(BA_AIMEDSHOT, actor, weapon), _game->getMod());
		std::ostringstream ss;
		ss << (optIndex + 1) << ": AIMED SHOT (" << cost << " TU, " << acc << "%)";
		_options.push_back({ BA_AIMEDSHOT, ss.str(), cost, acc, keys[optIndex++] });
	}

	// 4. Throw Grenade
	if (_bstate->getGrenadeItem() && _bstate->getGrenadeCost() > 0 && actor->getTimeUnits() >= _bstate->getGrenadeCost())
	{
		int cost = _bstate->getGrenadeCost();
		std::string gname = tr(_bstate->getGrenadeItem()->getRules()->getType());
		std::ostringstream ss;
		ss << (optIndex + 1) << ": THROW " << gname << " (" << cost << " TU)";
		_options.push_back({ BA_THROW, ss.str(), cost, 0, keys[optIndex++] });
	}

	// 5. Hold Fire / Pass
	{
		std::ostringstream ss;
		ss << (optIndex + 1) << ": HOLD FIRE (0 TU)";
		_options.push_back({ BA_NONE, ss.str(), 0, 0, keys[optIndex++] });
	}

	int numOpts = (int)_options.size();
	int winW = 240;
	int winH = 48 + numOpts * 17;

	_window = new Window(this, winW, winH, 40, 20);
	_txtTitle = new Text(winW - 16, 15, 8, 8);
	_txtSoldier = new Text(winW - 16, 11, 8, 22);
	_txtTarget = new Text(winW - 16, 11, 8, 34);

	_battleGame->getSave()->setPaletteByDepth(this);

	add(_window, "messageWindowBorder", "battlescape");
	add(_txtTitle, "messageWindows", "battlescape");
	add(_txtSoldier, "messageWindows", "battlescape");
	add(_txtTarget, "messageWindows", "battlescape");

	_window->setHighContrast(true);
	_window->setBackground(_game->getMod()->getSurface("TAC00.SCR"));

	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setHighContrast(true);
	std::string titleStr = tr("STR_INTERRUPT");
	if (titleStr == "STR_INTERRUPT") titleStr = "REACTION INTERRUPT!";
	_txtTitle->setText(titleStr);

	_txtSoldier->setAlign(ALIGN_CENTER);
	_txtSoldier->setHighContrast(true);
	std::string soldierName = actor->getName(_game->getLanguage());
	std::ostringstream sSoldier;
	sSoldier << soldierName << " (" << actor->getTimeUnits() << " TU)";
	_txtSoldier->setText(sSoldier.str());

	_txtTarget->setAlign(ALIGN_CENTER);
	_txtTarget->setHighContrast(true);
	std::string targetName = target->getName(_game->getLanguage());
	int dist = (int)std::sqrt(actor->distance3dToUnitSq(target));
	std::ostringstream sTarget;
	sTarget << "Target: " << targetName << " (" << dist << " tiles)";
	_txtTarget->setText(sTarget.str());

	for (int i = 0; i < numOpts; ++i)
	{
		TextButton *btn = new TextButton(winW - 20, 15, 10, 48 + i * 17);
		btn->setText(_options[i].label);
		btn->setHighContrast(true);
		btn->onMouseClick((ActionHandler)&InteractiveReactionState::btnOptionClick);
		btn->onKeyboardPress((ActionHandler)&InteractiveReactionState::btnOptionClick, _options[i].hotkey);
		if (_options[i].actionType == BA_NONE)
		{
			btn->onKeyboardPress((ActionHandler)&InteractiveReactionState::btnOptionClick, SDLK_SPACE);
			btn->onKeyboardPress((ActionHandler)&InteractiveReactionState::btnOptionClick, Options::keyCancel);
		}
		add(btn, "messageWindowButtons", "battlescape");
		_buttons.push_back(btn);
	}

	centerAllSurfaces();
}

InteractiveReactionState::~InteractiveReactionState()
{
}

void InteractiveReactionState::init()
{
}

void InteractiveReactionState::handle(Action *action)
{
	State::handle(action);
	if (action->getDetails()->type == SDL_MOUSEBUTTONDOWN && _game->isRightClick(action))
	{
		_bstate->setChosenAction(BA_NONE);
		_game->popState();
	}
}

void InteractiveReactionState::btnOptionClick(Action *action)
{
	for (size_t i = 0; i < _buttons.size(); ++i)
	{
		if (_buttons[i] == action->getSender())
		{
			_bstate->setChosenAction(_options[i].actionType);
			_game->popState();
			return;
		}
	}
	_bstate->setChosenAction(BA_NONE);
	_game->popState();
}

}
