/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    video_grabber.cpp
 * Desc:    Video grabber editor plugin
 * Version: 1.10
 * Author:  Grigory Korpan <griff@unigine.com>
 *          Alexey Zagniy <yingaz@unigine.com>
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

/******************************************************************************\
*
* VideoGrabber plugin
*
\******************************************************************************/

/*
 */
string getName() { return "VideoGrabber"; }

/*
 */
void init(PluginMeta m) {
	Grabber::init(m);
}

/*
 */
void shutdown() {
	Grabber::shutdown();
}

/*
 */
void update(int need_reload) {
	Grabber::update(need_reload);
}

/*
 */
void show() {
	Grabber::show();
}
/******************************************************************************\
*
* Grabber
*
\******************************************************************************/

/*
 */
namespace Grabber {
	
	/*
	 */
	using Unigine::Widgets;
	using Unigine::Tracker;
	
	/*
	 */
	int is_grabbing;
	int is_load_track_df_shown;
	int is_waiting_to_load;
	
	/*
	 */
	int show_mouse;
	int mouse_cursor;
	int show_messages;
	int show_fps;
	int show_visualizer;
	int app_update;
	
	/*
	 */
	int frame = 0;
	int counter = 0;
	float time = 0.0f;
	float min_time = 0;
	float max_time = 0;
	float unit_time = 1;
	string track_path;
	
	Tracker tracker;
	TrackerTrack track;
	
	/*
	 */
	Window main_w;
	EditLine fps_el;
	EditLine counter_el;
	Button grab_b;
	DialogFile load_track_df;
	EditLine track_path_el;
	EditLine frames_path_el;
	
	/*
	 */
	PluginMeta meta;
	
	/*
	 */
	void init(PluginMeta m) {
		
		meta = m;
		
		tracker = new Tracker();
		
		// interface
		main_w = new Window(TR("Grabber Extended"));
		main_w.setFlags(ALIGN_OVERLAP);
		
		Icon close_i = new Icon("core/gui/window_close.png");
		main_w.addChild(close_i,GUI_ALIGN_OVERLAP | GUI_ALIGN_TOP | GUI_ALIGN_RIGHT);
		close_i.setCallback(CLICKED,functionid(window_close));
		close_i.setPosition(4,-24);
		
		VBox vb = new VBox(4,4);
		main_w.addChild(vb);
		HBox track_hb = new HBox(4,4);
		vb.addChild(track_hb,ALIGN_EXPAND);
		HBox frames_hb = new HBox(4,4);
		vb.addChild(frames_hb,ALIGN_EXPAND);
		HBox grab_hb = new HBox(4,4);
		vb.addChild(grab_hb,ALIGN_EXPAND);
		
		Label track_l = new Label(TR("Track:"));
		track_hb.addChild(track_l,ALIGN_LEFT);
		
		track_path_el = new EditLine();
		track_path_el.setEditable(0);
		track_path_el.setWidth(230);
		track_hb.addChild(track_path_el,ALIGN_LEFT);
		
		Icon load_track_i = new Icon("editor/gui/action_load.png");
		load_track_i.setCallback(CLICKED,functionid(load_track_pressed));
		load_track_i.setToolTip(TR("Load track"));
		load_track_i.setWidth(24);
		load_track_i.setHeight(24);
		track_hb.addChild(load_track_i,ALIGN_RIGHT);
		
		Icon clear_track_i = new Icon("editor/gui/action_clear.png");
		clear_track_i.setCallback(CLICKED,functionid(clear_track_pressed));
		clear_track_i.setToolTip(TR("Clear track"));
		clear_track_i.setWidth(24);
		clear_track_i.setHeight(24);
		track_hb.addChild(clear_track_i,ALIGN_RIGHT);
		
		Label frames_l = new Label(TR("Frames dir /screenshots/:"));
		frames_hb.addChild(frames_l,ALIGN_LEFT);
		
		frames_path_el = new EditLine();
		frames_path_el.setEditable(1);
		frames_hb.addChild(frames_path_el,ALIGN_EXPAND);
		
		Label fps_l = new Label(TR("Frames total:"));
		grab_hb.addChild(fps_l,ALIGN_LEFT);
		
		fps_el = new EditLine("30");
		fps_el.setValidator(GUI_VALIDATOR_UINT);
		grab_hb.addChild(fps_el,ALIGN_LEFT);
		
		Label counter_l = new Label(TR("Warmup frames:"));
		grab_hb.addChild(counter_l,ALIGN_LEFT);
		
		counter_el = new EditLine("1");
		grab_hb.addChild(counter_el,ALIGN_LEFT);
		
		grab_b = new Button(TR("Grab"));
		grab_b.setToggleable(1);
		grab_b.setToolTip(TR("Grab main viewport"));
		grab_b.setCallback(CHANGED,functionid(grab_changed));
		grab_hb.addChild(grab_b,ALIGN_RIGHT);
		
		load_track_df = new DialogFile(TR("Load track"));
		load_track_df.setFilter(".track");
		load_track_df.setCallback(CLICKED,functionid(load_track_df_pressed));
		load_track_df.setFlags(ALIGN_OVERLAP);
		
		main_w.arrange();
		
		loadTrack(configGet("video_grabber_load_track_path",""));
		load_track_df.setPath(configGet("video_grabber_load_track_dir",engine.getDataPath()));
		frames_path_el.setText(configGet("video_grabber_frames_dir",""));
		
		
		pluginsAddWindow(main_w,meta.title,meta.name);
		hotkeysAddPluginHotkey(getName(),meta.name,meta.title,meta.description,new Hotkeys::HandlerPluginFunction(getName(),"Grabber::toggle_grab",'g',0,0,Hotkeys::KEY_CTRL));
	}
	
	/*
	 */
	void shutdown() {
		
		hotkeysRemovePlugin(getName());
		
		configSet("video_grabber_position_x",main_w.getPositionX());
		configSet("video_grabber_position_y",main_w.getPositionY());
		
		pluginsRemoveWindow(main_w,meta.name);
		
		delete tracker;
		delete track;
		
		removeChild(load_track_df);
		removeChild(main_w);
		delete load_track_df;
		delete main_w;
		delete fps_el;
		delete counter_el;
		delete grab_b;
		delete frames_path_el;
	}
	
	/*
	 */
	void update(int need_reload) {
		
		if(need_reload && !is_waiting_to_load) loadTrack(track_path);
		
		if(engine.app.clearKeyState(' ')) stopGrabbing();
		
		if(is_grabbing) {
			
			engine.app.setUpdate(1);
			engine.game.setIFps(1.0f / max(float(fps_el.getText()),EPSILON));
			
			if(track != NULL) {
				
				if(counter == 0) {
					engine.game.setEnabled(1);
					time += engine.game.getIFps() / unit_time;
					track.set(time);
				}
				else {
					engine.game.setEnabled(1);
					engine.game.setFrame(engine.game.getFrame() + 1);
					track.set(time);
					engine.game.setEnabled(0);
				}
				
				if(time >= max_time) {
					stopGrabbing();
				}
			}
			
			if(counter == 0) {
				string extensions[] = ("tga","dds","png","jpg");
				int extension = engine.console.getInt("video_extension");
				engine.console.run(format("video_grab %s/frame_%04d.%s",frames_path_el.getText(),frame++,extensions[extension]));
			}
			
			counter++;
			if(counter >= int(counter_el.getText())) {
				engine.game.setEnabled(1);
				counter = 0;
			} else {
				engine.game.setEnabled(0);
			}
		}
	}
	
	void show() {
		pluginsShowWindow(main_w,meta.name);
	}
	
	/*
	 */
	int loadTrack(string path) {
		
		if(path == "") return;
		
		if(engine.world.isLoaded() == 0) {
			dialogMessage(TR("Error"),TR("World is not loaded"));
			return 0;
		}
		
		tracker.reloadTrack(path);
		TrackerTrack new_track = tracker.loadTrack(path);
		
		string error = engine.getError();
		if(error != "") {
			dialogMessageOk(TR("Error"));
			
			if(new_track == track) {
				track = NULL;
				
				time = 0.0f;
				min_time = 0.0f;
				max_time = 0.0f;
				unit_time = 1.0f;
				
				set_track_path_dir("",1);
			}
			
			return 0;
		}
		
		if(new_track == NULL) return 0;
		
		track = new_track;
		
		time = 0.0f;
		min_time = track.getMinTime();
		max_time = track.getMaxTime();
		unit_time = track.getUnitTime();
		
		set_track_path_dir(path,1,1);
		
		return 1;
	}
	
	/*
	 */
	void startGrabbing() {
		
		if(is_grabbing) return;
		
		if(fps_el.getText() == "") {
			grab_b.setToggled(0);
			return;
		}
		
		string path = "screenshots/" + frames_path_el.getText();
		if(is_dir(path) == 0) {
			mkdir(path,1);
			if(is_dir(path) == 0) {
				dialogMessage(TR("Error"),TR("Bad directory:\n"));
				return;
			}
		}
		
		configSet("video_grabber_frames_dir",frames_path_el.getText());
		frames_path_el.setEditable(0);
		
		is_grabbing = 1;
		
		frame = 0;
		counter = 0;
		time = 0.0f;
		
		// save previous state
		show_mouse = engine.app.getMouseShow();
		mouse_cursor = engine.gui.getMouseCursor();
		show_messages = engine.console.getInt("show_messages");
		show_fps = engine.console.getInt("show_fps");
		show_visualizer = engine.console.getInt("show_visualizer");
		app_update = engine.app.getUpdate();
		
		engine.console.setInt("show_visualizer",0);
		engine.console.setInt("show_fps",0);
		engine.console.setInt("show_messages",0);
		
		engine.app.setMouseShow(0);
		engine.gui.setMouseCursor(GUI_CURSOR_NONE);
		engine.app.setUpdate(1);
		
		grab_b.setCallbackEnabled(CHANGED,0);
		grab_b.setToggled(1);
		grab_b.setCallbackEnabled(CHANGED,1);
		
		hide_interface(1);
	}
	
	/*
	 */
	void stopGrabbing() {
	
		if(is_grabbing == 0) return;
		
		frames_path_el.setEditable(1);
		
		is_grabbing = 0;
		engine.app.setMouseShow(show_mouse);
		engine.gui.setMouseCursor(mouse_cursor);
		
		engine.console.setInt("show_messages",show_messages);
		engine.console.setInt("show_fps",show_fps);
		engine.console.setInt("show_visualizer",show_visualizer);
		
		engine.game.setIFps(-1.0f);
		engine.app.setUpdate(app_update);
		
		grab_b.setCallbackEnabled(CHANGED,0);
		grab_b.setToggled(0);
		grab_b.setCallbackEnabled(CHANGED,1);
		
		hide_interface(0);
	}
	
	/* hide interface
	 */
	void hide_interface(int hidden) {
		forloop(int i = 0; engine.gui.getNumChilds()) {
			::Widget child = engine.gui.getChild(i);
			child.setHidden(hidden);
		}
	}
	
	/*
	 */
	void set_track_path_dir(string path,int save_dir = 0,int save_path = 0) {
		track_path = path;
		track_path_el.setText(basename(track_path));
		
		if(save_dir) configSet("video_grabber_load_track_dir",dirname(track_path));
		if(save_path) configSet("video_grabber_load_track_path",track_path);
	}
	
	/******************************************************************************\
	*
	* Callbacks
	*
	\******************************************************************************/
	
	/*
	 */
	void window_close() {
		windowHide(main_w,meta.name);
	}
	
	/*
	 */
	void grab_changed() {
		if(grab_b.isToggled()) {
			startGrabbing();
		} else {
			stopGrabbing();
		}
	}
	
	/*
	 */
	void toggle_grab() {
		grab_b.setToggled(!grab_b.isToggled());
	}
	
	/*
	 */
	void load_track_pressed() {
		
		addChild(load_track_df,load_track_df.getFlags());
		is_load_track_df_shown = 1;
		
		main_w.setEnabled(0);
		load_track_df.setPermanentFocus();
	}
	
	/*
	 */
	void clear_track_pressed() {
		track = NULL;
		set_track_path_dir("",0,1);
	}
	
	/*
	 */
	void load_track_df_pressed() {
		
		removeChild(load_track_df);
		is_load_track_df_shown = 0;
		main_w.setEnabled(1);
		
		if(load_track_df.isOkClicked()) loadTrack(load_track_df.getFile());
	}
	
} /* namespace Grabber */
