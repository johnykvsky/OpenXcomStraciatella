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
#include "../Engine/State.h"
#include <vector>
#include <string>

namespace OpenXcom
{

class Window;
class Text;
class TextButton;
class BattlescapeGame;
class BattleUnit;
class BattleAction;
class InteractiveReactionBState;

struct ReactionOption
{
	int actionType; // BA_SNAPSHOT, BA_AUTOSHOT, BA_AIMEDSHOT, BA_THROW, BA_NONE
	std::string label;
	int tuCost;
	int accuracy;
	SDLKey hotkey;
};

class InteractiveReactionState : public State
{
private:
	BattlescapeGame *_battleGame;
	BattleAction *_action;
	BattleUnit *_target;
	InteractiveReactionBState *_bstate;

	Window *_window;
	Text *_txtTitle;
	Text *_txtSoldier;
	Text *_txtTarget;
	std::vector<TextButton*> _buttons;
	std::vector<ReactionOption> _options;

public:
	InteractiveReactionState(BattlescapeGame *battleGame, BattleAction *action, BattleUnit *target, InteractiveReactionBState *bstate);
	~InteractiveReactionState();

	void init() override;
	void handle(Action *action) override;
	void btnOptionClick(Action *action);
};

}
