/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_physics_unredo.h
 * Desc:    Unigine editor
 * Version: 1.01
 * Author:  Valia Vaneeva <fattie@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

class PhysicsState_ : Unredo::State {
	
	Widget focus;
	Blob blob;
	
	float game_scale;
	float sound_scale;
	
	PhysicsState_() {
		super.__State__(Unredo::STATE_PHYSICS);
		focus = gui.getFocus();
		
		blob = new Blob(0);
		engine.physics.saveState(blob);
		blob.seekSet(0);
		
		game_scale = engine.game.getScale();
		sound_scale = engine.sound.getScale();
	}
	
	~PhysicsState_() {
		focus = NULL;
		delete blob;
	}
	
	PhysicsState_ undo() {
		PhysicsState_ result = new PhysicsState_();
		
		engine.physics.restoreState(blob);
		engine.game.setScale(game_scale);
		engine.sound.setScale(sound_scale);
		
		update(1);
		if(focus != NULL && editorIsChild(focus)) focus.setFocus();
		else tabbox.setFocus();
		
		return result;
	}
};

/******************************************************************************\
*
* States
*
\******************************************************************************/

PhysicsState_ create_state() {
	return new PhysicsState_();
}

void save_state(PhysicsState_ state) {
	if(state == NULL) return;
	unredoAddState(state);
}

int undo() {
	
}

int redo() {
	
}
