/* Copyright (C) 2005-2015, UNIGINE Corp. All rights reserved.
 *
 * This file is a part of the UNIGINE 2.0-RC SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE Corp. at http://unigine.com/
 */


#ifndef __CHARACTER_CONTROLS_H__
#define __CHARACTER_CONTROLS_H__

#include <unigine.h>
#include <scripts/input/control.h>

namespace Unigine {
	
	
	class Character;
	
	/******************************************************************************\
	 *
	 * class CharacterControl
	 *
	\******************************************************************************/
	
	/*
	 */
	class CharacterControls {
		
		Character character;
		
		//
		Character getCharacter() {
			return character;
		}
		
		//
		void setCharacter(Character character_) {
			character = character_;
		}
		
		//
		void init(Character character_) {
			
			setCharacter(character_);
		}
		
		//
		CharacterControls(Character character_) {
			throw("CharacterContorls::CharacterContols(): can not create instance of the abstract class.");
		}
		
		//
		void update() {
		
		}
		
	};
	
	/******************************************************************************\
	 *
	 * class CharacterControlsHuman
	 *
	\******************************************************************************/
	
	/* 
	 */
	class CharacterControlsHuman : CharacterControls {
		
		Unigine::Input::Control control;
		ControlsDummy controls_dummy;
		int config_loaded = 0;
		
		//
		CharacterControlsHuman(Character character_) {
			init(character_);
			control = new Unigine::Input::Control();
			if(character != NULL) {
				controls_dummy = character.getControls();
			}
		}
		
		//
		void update() {
			
			if(character == NULL) return;
			
			control.update(engine.game.getIFps());
			
			Unigine::Input::Action action1;
			Unigine::Input::Action action2;
			
			action1 = control.getAction("move_forward");
			controls_dummy.setState(CONTROLS_STATE_FORWARD,action1.getState() != action1.getNeutral());
			
			action1 = control.getAction("move_backward");
			controls_dummy.setState(CONTROLS_STATE_BACKWARD,action1.getState() != action1.getNeutral());
			
			action1 = control.getAction("strafe_left");
			controls_dummy.setState(CONTROLS_STATE_MOVE_LEFT,action1.getState() != action1.getNeutral());
			
			action1 = control.getAction("strafe_right");
			controls_dummy.setState(CONTROLS_STATE_MOVE_RIGHT,action1.getState() != action1.getNeutral());
			
			action1 = control.getAction("jump");
			controls_dummy.setState(CONTROLS_STATE_JUMP,action1.getState() != action1.getNeutral());
			
			action1 = control.getAction("crouch");
			controls_dummy.setState(CONTROLS_STATE_CROUCH,action1.getState() != action1.getNeutral());
			
			action1 = control.getAction("fire");
			controls_dummy.setState(CONTROLS_STATE_FIRE,action1.getState() != action1.getNeutral());
			
			action1 = control.getAction("run");
			controls_dummy.setState(CONTROLS_STATE_RUN,action1.getState() != action1.getNeutral());
			
			action1 = control.getAction("turn_right");
			action2 = control.getAction("turn_left");
			controls_dummy.setMouseDX(action1.getState() - action2.getState());
			
			action1 = control.getAction("turn_up");
			action2 = control.getAction("turn_down");
			controls_dummy.setMouseDY(action1.getState() - action2.getState());
			
			/*
			action1 = control.getAction("zoom_in");
			action2 = control.getAction("zoom_out");
			character.camera.zoom(action2.getState() - action1.getState());
			*/
		}
		
		//
		void load(string name) {
			config_loaded = control.loadFromFile(name);
		}
		
		//
		void save(string name) {
			if(config_loaded) control.saveToFile(name);
		}
		
		//
		Unigine::Input::Control getControl() {
			return control;
		}
		
		//
		void setCharacter(Character character_) {
			character = character_;
			if(character != NULL) {
				controls_dummy = character.getControls();
			}
		}
		
	};
	
}

#endif
