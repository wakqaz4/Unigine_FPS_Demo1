/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    hide_interface.cpp
 * Desc:    Hide interface plugin
 * Version: 1.04
 * Author:  Grigory Korpan <griff@unigine.com>
 *          Vladimir Borovskikh <futurist@unigine.com>
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
string getName() {
	return "HideInterface";
}

int show_profiler;
int show_messages;
int show_fps;
int show_visualizer;
int hide;

/*
 */
void hide_interface(int hide) {
	WidgetWindow main_w = class_cast("WidgetWindow", engine.system.get("System::main_w"));
	if(hide) {
		show_profiler = engine.console.getInt("show_profiler");
		show_messages = engine.console.getInt("show_messages");
		show_fps = engine.console.getInt("show_fps");
		show_visualizer = engine.console.getInt("show_visualizer");
		
		engine.console.setInt("show_profiler",0);
		engine.console.setInt("show_visualizer",0);
		engine.console.setInt("show_fps",0);
		engine.console.setInt("show_messages",0);
		engine.system.call("systemHide");
	} else {
		engine.console.setInt("show_profiler",show_profiler);
		engine.console.setInt("show_visualizer",show_messages);
		engine.console.setInt("show_fps",show_fps);
		engine.console.setInt("show_messages",show_visualizer);
	}
	
	forloop(int i = 0; engine.gui.getNumChilds()) {
		Widget child = engine.gui.getChild(i);
		
		if(child.getTypeName() == "WidgetEngine") continue;
		if(child == main_w) continue;
		
		child.setHidden(hide);
	}
}

/*
 */
void toggle_interface_visibility() {
	log.message("HideInterface: %d\n",hide);
	hide = !hide;
	hide_interface(hide);
}

/*
 */
void init(PluginMeta m) {
	hotkeysAddPluginHotkey(getName(),m.name,m.title,m.description,new Hotkeys::HandlerPluginFunction(getName(),"toggle_interface_visibility",'h',0,0,Hotkeys::KEY_CTRL));
}

/*
 */
void shutdown() {
	hotkeysRemovePlugin(getName());
	if(hide) hide_interface(false);
}