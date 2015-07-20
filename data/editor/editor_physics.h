/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_physics.h
 * Desc:    Unigine editor
 * Version: 1.07
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
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

/*
 */
#define GAME_PRESSED_CALLBACK_SLIDER(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	Unredo::State s = NULL; \
	if(skip_state == false && engine.game.get ## FUNC() != float(WIDGET ## _el.getText())) s = create_state(); \
	engine.game.set ## FUNC(float(WIDGET ## _el.getText())); \
	WIDGET ## _el.setText(editorFormat(engine.game.get ## FUNC())); \
	save_state(s); \
}

/*
 */
#define PHYSICS_PRESSED_CALLBACK_SLIDER(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	Unredo::State s = NULL; \
	if(skip_state == false && engine.physics.get ## FUNC() != float(WIDGET ## _el.getText())) s = create_state(); \
	engine.physics.set ## FUNC(float(WIDGET ## _el.getText())); \
	WIDGET ## _el.setText(editorFormat(engine.physics.get ## FUNC())); \
	save_state(s); \
}

/*
 */
#define PHYSICS_PRESSED_CALLBACK(WIDGET,FUNC,TYPE,FORMATTER) \
void WIDGET ## _pressed() { \
	float WIDGET = TYPE(WIDGET ## _el.getText()); \
	if(skip_state == false && WIDGET != engine.physics.get ## FUNC()) save_state(create_state()); \
	engine.physics.set ## FUNC(WIDGET); \
	WIDGET ## _el.setText(FORMATTER(engine.physics.get ## FUNC())); \
}

/*
 */
namespace Physics {
	
	/*
	 */
	DECLARE_WINDOW
	
	WidgetTabBox tabbox;							// physics tabbox
	
	#include <editor/editor_physics_unredo.h>
	
	/**************************************************************************\
	*
	* Common
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Common {
		
		/*
		 */
		WidgetEditLine fps_el;						// time step
		WidgetEditLine game_scale_el;				// game time scale
		WidgetEditLine sound_scale_el;				// sound time scale
		WidgetEditLine physics_scale_el;			// physics time scale
		WidgetEditLine num_iterations_el;			// number of iterations
		WidgetEditLine linear_damping_el;			// linear damping coefficient
		WidgetEditLine angular_damping_el;			// angular damping coefficient
		WidgetEditLine max_linear_velocity_el;		// maximum linear velocity
		WidgetEditLine max_angular_velocity_el;		// maximum angular velocity
		WidgetEditLine frozen_linear_velocity_el;	// frozen linear velocity
		WidgetEditLine frozen_angular_velocity_el;	// frozen angular velocity
		WidgetEditLine num_frozen_frames_el;		// number of frozen frames
		WidgetEditLine penetration_factor_el;		// penetration factor
		WidgetEditLine penetration_tolerance_el;	// penetration tolerance
		WidgetEditLine gravity_el[3];				// constant gravity
		
		DECLARE_SLIDER_CALLBACKS
		
		/*
		 */
		void update() {
			
			fps_el.setText(editorFormat(1.0f / engine.physics.getIFps()));
			game_scale_el.setText(editorFormat(engine.game.getScale()));
			sound_scale_el.setText(editorFormat(engine.sound.getScale()));
			physics_scale_el.setText(editorFormat(engine.physics.getScale()));
			num_iterations_el.setText(string(engine.physics.getNumIterations()));
			linear_damping_el.setText(editorFormat(engine.physics.getLinearDamping()));
			angular_damping_el.setText(editorFormat(engine.physics.getAngularDamping()));
			max_linear_velocity_el.setText(editorFormat(engine.physics.getMaxLinearVelocity()));
			max_angular_velocity_el.setText(editorFormat(engine.physics.getMaxAngularVelocity()));
			frozen_linear_velocity_el.setText(editorFormat(engine.physics.getFrozenLinearVelocity()));
			frozen_angular_velocity_el.setText(editorFormat(engine.physics.getFrozenAngularVelocity()));
			num_frozen_frames_el.setText(string(engine.physics.getNumFrozenFrames()));
			penetration_factor_el.setText(editorFormat(engine.physics.getPenetrationFactor()));
			penetration_tolerance_el.setText(editorFormat(engine.physics.getPenetrationTolerance()));
			setEditLineVec3(gravity_el,engine.physics.getGravity());
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		void fps_pressed() {
			float ifps = 1.0f / float(fps_el.getText());
			if(ifps != engine.physics.getIFps()) save_state(create_state());
			engine.physics.setIFps(ifps);
			fps_el.setText(editorFormat(1.0f / engine.physics.getIFps()));
		}
		
		void gravity_pressed() {
			vec3 gravity = getEditLineVec3(gravity_el);
			if(compare(gravity,engine.physics.getGravity(),STATE_EPSILON) == 0) save_state(create_state());
			engine.physics.setGravity(gravity);
			setEditLineVec3(gravity_el,engine.physics.getGravity());
		}
		
		/*
		 */
		GAME_PRESSED_CALLBACK_SLIDER(game_scale,Scale)
		SOUND_PRESSED_CALLBACK_SLIDER(sound_scale,Scale)
		PHYSICS_PRESSED_CALLBACK_SLIDER(physics_scale,Scale)
		PHYSICS_PRESSED_CALLBACK(num_iterations,NumIterations,int,string)
		PHYSICS_PRESSED_CALLBACK(linear_damping,LinearDamping,float,editorFormat)
		PHYSICS_PRESSED_CALLBACK(angular_damping,AngularDamping,float,editorFormat)
		PHYSICS_PRESSED_CALLBACK(max_linear_velocity,MaxLinearVelocity,float,editorFormat)
		PHYSICS_PRESSED_CALLBACK(max_angular_velocity,MaxAngularVelocity,float,editorFormat)
		PHYSICS_PRESSED_CALLBACK(frozen_linear_velocity,FrozenLinearVelocity,float,editorFormat)
		PHYSICS_PRESSED_CALLBACK(frozen_angular_velocity,FrozenAngularVelocity,float,editorFormat)
		PHYSICS_PRESSED_CALLBACK(num_frozen_frames,NumFrozenFrames,int,string)
		PHYSICS_PRESSED_CALLBACK_SLIDER(penetration_factor,PenetrationFactor)
		PHYSICS_PRESSED_CALLBACK(penetration_tolerance,PenetrationTolerance,float,editorFormat)
		
	} /* namespace Common */
	
	/**************************************************************************\
	*
	* Window
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		Common::update();
		
		editorWindow(window);
		DECLARE_WINDOW_LOAD(physics)
		
		tabbox.setCurrentTab(configGet("physics_tab",0));
	}
	
	void shutdown() {
		
		configSet("physics_tab",tabbox.getCurrentTab());
		
		DECLARE_WINDOW_SAVE(physics)
	}
	
	/*
	 */
	void update(int need_reload) {
		
		// reload physics
		if(need_reload) {
			Common::update();
		}
		
		int tab = tabbox.getCurrentTab();
		
		// update tabs
		if(tab == 0) Common::update_dynamic();
	}
	
	/*
	 */
	void load_clicked() {
		string name;
		PhysicsState_ s = create_state();
		if(dialogFileSettings(TR("Select physics settings to load"),name,DIALOG_FILE_OPEN)) {
			if(engine.physics.loadSettings(name) == 0) {
				dialogMessageOk(TR("Error"));
				delete s;
			} else {
				save_state(s);
				update(1);
			}
		}
	}
	
	void save_clicked() {
		string name;
		if(dialogFileSettings(TR("Select physics settings to save"),name,DIALOG_FILE_SAVE)) {
			name = savePath(name);
			
			if(engine.physics.saveSettings(name,1) == 0) {
				dialogMessageOk(TR("Error"));
			}
		}
	}
	
} /* namespace Physics */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void physicsInit() {
	Physics::init();
}

void physicsShutdown() {
	Physics::shutdown();
}

/*
 */
void physicsToggle() {
	if(Physics::window_show) physicsHide();
	else physicsShow();
}

void physicsShow() {
	DECLARE_WINDOW_SHOW(Physics)
}

void physicsHide() {
	DECLARE_WINDOW_HIDE(Physics)
}

/*
 */
void physicsUpdate(int need_reload) {
	Physics::update(need_reload);
}
