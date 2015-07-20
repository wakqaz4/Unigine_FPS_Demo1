/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_viewports.h
 * Desc:    Unigine editor
 * Version: 1.00
 * Author:  Vladimir Borovskikh <futurist@unigine.com>
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

#ifndef __EDITOR_VIEWPORTS_H__
#define __EDITOR_VIEWPORTS_H__

#include <editor/widgets/editor_widget_viewport_layout.h>
#include <editor/widgets/editor_widget_viewport_window.h>

/*
 */
namespace Viewports {
	
	/*
	 */
	using Unigine::Widgets;
	using Editor::Widgets;
	
	string window_names[0] = (
		"Viewport 1",
		"Viewport 2",
		"Viewport 3",
		"Viewport 4",
	);
	
	/*
	 */
	WidgetViewportLayout main_viewport;
	WidgetViewportWindow windows[] = (:);
	
	::Widget focused_window;
	
	/*
	 */
	void init() {
		
		main_viewport = new WidgetViewportLayout();
		
		forloop(int i = 0; window_names.size()) {
			create_viewport_window(window_names[i]);
		}
		
		string active_names = configGet("active_viewport_windows","");
		
		string names[0];
		if(strsplit(active_names,",",names)) {
			forloop(int i = 0; names.size()) {
				open_viewport_window(names[i]);
			}
		}
	}
	
	void shutdown() {
		string active_names = "";
		
		forloop(int i = 0; windows.size()) {
			windows[windows.key(i)].setCallbackEnabled(HIDE,0);
			if(!is_window_opened(windows.key(i))) continue;
			active_names += windows.key(i) + ",";
			save_viewport_window(windows.key(i));
		}
		
		configSet("active_viewport_windows",trim(active_names,","));
		
		windows.delete();
		delete main_viewport;
	}
	
	/*
	 */
	void update(int need_reload) {
		
		focused_window = engine.gui.getOverlappedFocus();
		
		#ifdef HAS_INTERFACE
			focused_window = NULL;
			forloop(int i = 0; getNumWindows()) {
				InterfaceWindow window = getWindow(i);
				if(window == NULL) continue;
				if(window.isFocused() == 0) continue;
				
				if(window == tooltipGetWindow()) {
					window = tooltipGetFocusedWindow();
					if(window == NULL) continue;
				}
				
				Gui gui = window.getGui();
				if(gui.getNumChilds()) {
					::Widget child = gui.getChild(0);
					if(child.getType() != WIDGET_ENGINE && child.getType() == WIDGET_WINDOW) focused_window = child;
				}
			}
		#endif
		
		if(focused_window != NULL && (focused_window.getType() == WIDGET_ENGINE || focused_window.getType() != WIDGET_WINDOW)) focused_window = NULL;
		
		main_viewport.update();
		
		WidgetViewportWindow focused = get_focused_viewport();
		controlsSetEnabled(focused == NULL);
	}
	
	/*
	 */
	void save_viewport_window(string name) {
		if(!windows.check(name)) return;
		
		configSet(format("viewport_window_%s_width",name),windows[name].getWidth());
		configSet(format("viewport_window_%s_height",name),windows[name].getHeight());
		configSet(format("viewport_window_%s_position_x",name),windows[name].getPositionX());
		configSet(format("viewport_window_%s_position_y",name),windows[name].getPositionY());
	}
	
	void load_viewport_window(string name) {
		if(!windows.check(name)) return;
		
		windows[name].setWidth(configGet(format("viewport_window_%s_width",name),windows[name].getWidth()));
		windows[name].setHeight(configGet(format("viewport_window_%s_height",name),windows[name].getHeight()));
		int x = configGet(format("viewport_window_%s_position_x",name),windows[name].getPositionX());
		int y = configGet(format("viewport_window_%s_position_y",name),windows[name].getPositionY());
		
		windows[name].setPosition(x,y);
		windows[name].setHidden(0);
	}
	
	void load_viewport_window_size(string name,int &width,int &height) {
		width = configGet(format("viewport_window_%s_width",name),640);
		height = configGet(format("viewport_window_%s_height",name),480);
	}
	
	WidgetViewportWindow create_viewport_window(string name) {
		int width;
		int height;
		load_viewport_window_size(name,width,height);
		WidgetViewportWindow window = new WidgetViewportWindow(name,0,0,width,height);
		window.setCallback(HIDE,functionid(on_window_hide),window);
		window.setHidden(1);
		windows.append(name,window);
	}
	
	/*
	 */
	void open_viewport_window(string name) {
		if(windows.check(name)) {
			addChild(windows[name],ALIGN_OVERLAP | ALIGN_CENTER);
			load_viewport_window(name);
			return;
		}
		
		addChild(create_viewport_window(name),ALIGN_OVERLAP | ALIGN_CENTER);
		load_viewport_window(name);
	}
	
	void close_viewport_window(string name) {
		if(!windows.check(name)) return;
		
		windows[name].runCallback(HIDE);
	}
	
	int is_window_opened(string name) {
		if(!windows.check(name)) return false;
		
		return isChild(windows[name]);
	}
	
	/*
	 */
	WidgetViewportWindow get_focused_viewport() {
		forloop(int i = 0; windows.size()) {
			if(windows[windows.key(i)].isFocused()) return windows[windows.key(i)];
		}
		return NULL;
	}
	
	/*
	 */
	void on_window_hide(WidgetViewportWindow window) {
		string name = windows.find(window,"-1");
		if(name == "-1") return;
		removeChild(windows[name]);
		save_viewport_window(name);
	}
	
	/*
	 */
	::Widget get_focused_window() {
		return focused_window;
	}
}

/*
 */
void viewportsInit() {
	Viewports::init();
}

/*
 */
void viewportsShutdown() {
	Viewports::shutdown();
}

/*
 */
void viewportsUpdate(int need_reload) {
	Viewports::update(need_reload);
}

/*
 */
::Widget viewportsGetFocusedWindow() {
	return Viewports::get_focused_window();
}

/*
 */
int viewportsGetNumWindows() {
	return Viewports::windows.size();
}

Editor::Widgets::WidgetViewportWindow viewportsGetWindow(int num) {
	assert(num >= 0 && num < Viewports::windows.size() && "viewportsGetWindow(): bad index");
	return Viewports::windows[Viewports::windows.key(num)];
}

/*
 */
void viewportsOpenWindow(string name) {
	Viewports::open_viewport_window(name);
}

/*
 */
void viewportsCloseWindow(string name) {
	Viewports::close_viewport_window(name);
}

/*
 */
void viewportsToggleWindow(string name) {
	if(Viewports::is_window_opened(name)) viewportsCloseWindow(name);
	else viewportsOpenWindow(name);
}

/*
 */
Editor::Widgets::WidgetViewportLayout viewportsLayoutGet(string name = NULL) {
	using Editor::Widgets;
	
	if(strlen(name) == 0) return Viewports::main_viewport;
	
	WidgetViewportWindow window = Viewports::windows.check(name,NULL);
	if(window == NULL) return NULL;
	
	return window.getLayout();
}

#endif // __EDITOR_VIEWPORTS_H__
