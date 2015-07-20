/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    tracker_tracker.h
 * Desc:    Unigine editor
 * Version: 1.03
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
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

#ifdef EDITOR_TRACKER
	#include <core/systems/tracker/editor/tracker_editor.h>
#endif

/*
 */
namespace Tracker {
	
	/*
	 */
	DECLARE_WINDOW
	
	WidgetVBox container_vb;
	
	#ifdef EDITOR_TRACKER
		string name;
		string title;
		Unigine::Tracker::Tracker tracker;
		Unigine::Tracker::TrackerEditor editor;
	#endif
	
	/*
	 */
	void init() {
		
		editorWindow(window);
		DECLARE_WINDOW_LOAD(tracker)
		
		container_vb = container_vb;
		
		#ifdef EDITOR_TRACKER
			
			int value_h = configGet("tracker_value_h",-32767);
			int value_v = configGet("tracker_value_v",-32767);
			int key_snap = configGet("tracker_key_snap",0);
			name = configGet("tracker_name","");
			
			title = window.getText();
			
			int flags = Unigine::Tracker::TRACKER_SAVE_RESTORE | Unigine::Tracker::TRACKER_CHECK_OBJECTS;
			tracker = new Unigine::Tracker::Tracker(flags);
			
			editor = new Unigine::Tracker::TrackerEditor(tracker);
			Unigine::Widgets::HPaned main_hp = editor.getMainHPaned();
			Unigine::Widgets::VPaned left_vp = editor.getLeftVPaned();
			Unigine::Widgets::VPaned right_vp = editor.getRightVPaned();
			Unigine::Widgets::Track track = editor.getTrackWidget();
			Unigine::Widgets::HBox panel_hbox = track.getPanelHBox();
			
			Unigine::Widgets::Button button = new Unigine::Widgets::Button(TR("Load"));
			button.setCallback(Unigine::Widgets::CLICKED,functionid(load_clicked));
			panel_hbox.addChild(button,Unigine::Widgets::ALIGN_RIGHT);
			
			button = new Unigine::Widgets::Button(TR("Save"));
			button.setCallback(Unigine::Widgets::CLICKED,functionid(save_clicked));
			panel_hbox.addChild(button,Unigine::Widgets::ALIGN_RIGHT);
			
			button = new Unigine::Widgets::Button(TR("Clear"));
			button.setCallback(Unigine::Widgets::CLICKED,functionid(clear_clicked));
			panel_hbox.addChild(button,Unigine::Widgets::ALIGN_RIGHT);
			
			container_vb.addChild(Unigine::Widgets::Widget(editor.getWidget()).widget,GUI_ALIGN_EXPAND);
			
			editor.arrange();
			
			main_hp.setValue(value_h);
			left_vp.setValue(value_v);
			right_vp.setValue(value_v);
			track.setKeySnap(key_snap);
			
		#endif
		
	}
	
	void shutdown() {
		
		DECLARE_WINDOW_SAVE(tracker)
		
		#ifdef EDITOR_TRACKER
			
			configSet("tracker_value_h",editor.main_hp.getValue());
			configSet("tracker_value_v",editor.left_vp.getValue());
			configSet("tracker_key_snap",editor.track_w.getKeySnap());
			configSet("tracker_name",name);
			
			delete editor;
			delete tracker;
			
		#endif
	}
	
	/*
	 */
	void update(int need_reload) {
		
		#ifdef EDITOR_TRACKER
			
			if(window_show) editor.update();
			
		#endif
	}
	
	/*
	 */
	#ifdef EDITOR_TRACKER
		
		void load_clicked() {
			
			using Unigine::Widgets;
			
			if(dialogFileTrack(TR("Select file"),name,DIALOG_FILE_OPEN) == 0) return;
			
			if(editor.loadTrack(name)) {
				window.setText(title + " " + name);
				return;
			}
			
			window.setText(title);
			dialogMessageOk(TR("Error"),TR("Can't load track\n") + engine.getMessage());
		}
		
		void save_clicked() {
			
			using Unigine::Widgets;
			
			if(dialogFileTrack(TR("Select file"),name,DIALOG_FILE_SAVE) == 0) return;
			
			if(editor.saveTrack(name)) {
				window.setText(title + " " + name);
				return;
			}
			
			dialogMessageOk(TR("Error"),TR("Can't save track\n") + engine.getMessage());
		}
		
		void clear_clicked() {
			
			// clear editor
			if(dialogMessageYesNo(TR("Confirm"),TR("Clear tracks?")) == 0) return;
			
			window.setText(title);
			editor.clear();
		}
		
	#endif
	
} /* namespace Tracker */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void trackerInit() {
	Tracker::init();
}

void trackerShutdown() {
	Tracker::shutdown();
}

/*
 */
void trackerToggle() {
	if(Tracker::window_show) trackerHide();
	else trackerShow();
}

void trackerShow() {
	DECLARE_WINDOW_SHOW(Tracker)
}

void trackerHide() {
	DECLARE_WINDOW_HIDE(Tracker)
}

/*
 */
void trackerIsFocused() {
	Widget focus = engine.gui.getFocus();
	return Tracker::window.isChild(focus);
}

/*
 */
void trackerUpdate(int need_reload) {
	Tracker::update(need_reload);
}
