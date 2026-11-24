#pragma once
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
#include "BattleState.h"

namespace OpenXcom
{

class BattleUnit;
class BattleItem;

class InteractiveReactionBState : public BattleState
{
private:
	BattleUnit *_target;
	bool _promptShown;
	int _chosenAction; // BA_NONE (pass), BA_SNAPSHOT, BA_AUTOSHOT, BA_AIMEDSHOT, BA_THROW
	BattleItem *_grenadeItem;
	bool _grenadeFromBelt;
	bool _grenadeNeedPrime;
	int _grenadeCost;

public:
	InteractiveReactionBState(BattlescapeGame *parent, BattleAction action, BattleUnit *target);
	~InteractiveReactionBState();

	void init() override;
	void think() override;

	bool hasAvailableActions() const;
	void setChosenAction(int actionType) { _chosenAction = actionType; }
	BattleUnit *getTarget() const { return _target; }
	BattleItem *getGrenadeItem() const { return _grenadeItem; }
	bool isGrenadeFromBelt() const { return _grenadeFromBelt; }
	bool isGrenadeNeedPrime() const { return _grenadeNeedPrime; }
	int getGrenadeCost() const { return _grenadeCost; }
	BattleAction *getAction() { return &_action; }
};

}
