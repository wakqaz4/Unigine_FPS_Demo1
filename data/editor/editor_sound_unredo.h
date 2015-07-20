/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_sound_unredo.h
 * Desc:    Unigine editor
 * Version: 1.02
 * Author:  Pavel Kurinnoy <figonet@unigine.com>
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

class SoundState_ : Unredo::State {
	
	Widget focus;
	
	Blob blob;
	
	SoundState_() {
		super.__State__(Unredo::STATE_SOUND);
		focus = gui.getFocus();
		
		blob = new Blob(0);
		engine.sound.saveState(blob);
		blob.seekSet(0);
	}
	
	~SoundState_() {
		focus = NULL;
		delete blob;
	}
	
	SoundState_ undo() {
		SoundState_ result = new SoundState_();
		
		engine.sound.restoreState(blob);
		
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

SoundState_ create_state() {
	return new SoundState_();
}

void save_state(SoundState_ state) {
	if(state == NULL) return;
	unredoAddState(state);
}

int undo() {
	
}

int redo() {
	
}
