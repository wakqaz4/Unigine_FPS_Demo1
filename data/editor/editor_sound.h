/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_sound.h
 * Desc:    Unigine editor
 * Version: 1.02
 * Author:  Pavel Kurinnoy <figonet@unigine.com>
 *          Alexander Zapryagaev <frustum@unigine.com>
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
#define SOUND_PRESSED_CALLBACK_SLIDER(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	Unredo::State s = NULL; \
	if(skip_state == false && engine.sound.get ## FUNC() != float(WIDGET ## _el.getText())) s = create_state(); \
	engine.sound.set ## FUNC(float(WIDGET ## _el.getText())); \
	WIDGET ## _el.setText(editorFormat(engine.sound.get ## FUNC())); \
	save_state(s); \
}

/*
 */
namespace Sound {
	
	/*
	 */
	DECLARE_WINDOW
	
	WidgetTabBox tabbox;							// sound tabbox
	
	#include <editor/editor_sound_unredo.h>
	
	/**************************************************************************\
	*
	* Common
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Common {
		
		WidgetEditLine volume_el;
		WidgetEditLine doppler_el;
		WidgetEditLine velocity_el;
		WidgetEditLine adaptation_el;
		
		DECLARE_SLIDER_CALLBACKS
		
		/*
		 */
		void update() {
			
			doppler_el.setText(editorFormat(engine.sound.getDoppler()));
			volume_el.setText(editorFormat(engine.sound.getVolume()));
			velocity_el.setText(editorFormat(engine.sound.getVelocity()));
			adaptation_el.setText(editorFormat(engine.sound.getAdaptation()));
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		SOUND_PRESSED_CALLBACK_SLIDER(doppler,Doppler)
		SOUND_PRESSED_CALLBACK_SLIDER(volume,Volume)
		SOUND_PRESSED_CALLBACK_SLIDER(velocity,Velocity)
		SOUND_PRESSED_CALLBACK_SLIDER(adaptation,Adaptation)
		
	} /* namespace Common */
	
	/**************************************************************************\
	*
	* Sources
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Sources {
		
		WidgetGridBox sources_gb;
		
		class Source {
			
			WidgetSlider volume_sl;
			WidgetEditLine volume_el;
			WidgetEditLine limit_el;
			
			Source(int num) {
				
				sources_gb.addChild(new WidgetLabel(gui,format(TR("Volume %d:"),num)));
				
				volume_sl = new WidgetSlider(gui,0,1000,1000);
				volume_el = new WidgetEditLine(gui);
				volume_sl.addAttach(volume_el,"%g",1000);
				
				volume_sl.setCallback(GUI_CLICKED,functionid(slider_clicked));
				volume_sl.setCallback(GUI_CHANGED,functionid(slider_changed));
				
				sources_gb.addChild(volume_sl,GUI_ALIGN_EXPAND);
				sources_gb.addChild(volume_el,GUI_ALIGN_EXPAND);
				
				volume_el.setCallback(GUI_PRESSED,functionid(volume_pressed),num);
				volume_el.setCallback(GUI_FOCUS_OUT,functionid(volume_pressed),num);
				
				sources_gb.addChild(new WidgetLabel(gui,format(TR("Limit:"))));
				
				limit_el = new WidgetEditLine(gui);
				sources_gb.addChild(limit_el,GUI_ALIGN_EXPAND);
				
				limit_el.setCallback(GUI_PRESSED,functionid(limit_pressed),num);
				limit_el.setCallback(GUI_FOCUS_OUT,functionid(limit_pressed),num);
			}
			
			void update(int num) {
				volume_el.setText(editorFormat(engine.sound.getSourceVolume(num)));
				limit_el.setText(string(engine.sound.getSourceLimit(num)));
			}
		};
		
		Source sources[0];
		
		DECLARE_SLIDER_CALLBACKS
		
		/*
		 */
		void init() {
			
			forloop(int i = 0; 32) {
				sources.append(new Source(i));
			}
			
			sources_gb.addChild(new WidgetVBox(gui));
			sources_gb.addChild(new WidgetVBox(gui,64,0));
			sources_gb.addChild(new WidgetVBox(gui,32,0));
			sources_gb.addChild(new WidgetVBox(gui));
			sources_gb.addChild(new WidgetVBox(gui,32,0));
		}
		
		/*
		 */
		void update() {
			
			forloop(int i = 0; 32) {
				sources[i].update(i);
			}
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		void volume_pressed(int num) {
			if(skip_state) return;
			Source source = sources[num];
			
			Unredo::State s = NULL;
			if(engine.sound.getSourceVolume(num) != float(source.volume_el.getText())) s = create_state();
			engine.sound.setSourceVolume(num,float(source.volume_el.getText()));
			source.volume_el.setText(editorFormat(engine.sound.getSourceVolume(num)));
			
			save_state(s);
		}
		
		void limit_pressed(int num) {
			Source source = sources[num];
			
			Unredo::State s = NULL;
			if(engine.sound.getSourceLimit(num) != int(source.limit_el.getText())) s = create_state();
			engine.sound.setSourceLimit(num,int(source.limit_el.getText()));
			source.limit_el.setText(string(engine.sound.getSourceLimit(num)));
			
			save_state(s);
		}
		
	} /* namespace Sources */
	
	/**************************************************************************\
	*
	* Window
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		Sources::init();
		
		Common::update();
		Sources::update();
		
		editorWindow(window);
		DECLARE_WINDOW_LOAD(sound)
		
		tabbox.setCurrentTab(configGet("sound_tab",0));
	}
	
	void shutdown() {
		
		DECLARE_WINDOW_SAVE(sound)
		
		configSet("sound_tab",tabbox.getCurrentTab());
	}
	
	/*
	 */
	void update(int need_reload) {
		
		// reload sound
		if(need_reload) {
			Common::update();
			Sources::update();
		}
		
		int tab = tabbox.getCurrentTab();
		
		// update dynamic
		if(tab == 0) Common::update_dynamic();
		else if(tab == 1) Sources::update_dynamic();
	}
	
	/*
	 */
	void load_clicked() {
		string name;
		Unredo::State s = create_state();
		if(dialogFileSettings(TR("Select sound settings to load"),name,DIALOG_FILE_OPEN)) {
			if(engine.sound.loadSettings(name) == 0) {
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
		if(dialogFileSettings(TR("Select sound settings to save"),name,DIALOG_FILE_SAVE)) {
			name = savePath(name);
			
			if(engine.sound.saveSettings(name,1) == 0) {
				dialogMessageOk(TR("Error"));
			}
		}
	}
	
} /* namespace Sound */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void soundInit() {
	Sound::init();
}

void soundShutdown() {
	Sound::shutdown();
}

/*
 */
void soundToggle() {
	if(Sound::window_show) soundHide();
	else soundShow();
}

void soundShow() {
	DECLARE_WINDOW_SHOW(Sound)
}

void soundHide() {
	DECLARE_WINDOW_HIDE(Sound)
}

/*
 */
void soundUpdate(int need_reload) {
	Sound::update(need_reload);
}
