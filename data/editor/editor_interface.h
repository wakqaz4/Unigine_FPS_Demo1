/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_interface.h
 * Desc:    Unigine editor
 * Version: 1.05
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

#ifdef HAS_INTERFACE
	
	/*
	 */
	#define DECLARE_DIALOG \
		InterfaceWindow interface;
	
	#define DECLARE_DIALOG_INIT(NAME) \
		interface = new InterfaceWindow(0);
	
	#define DECLARE_DIALOG_SHOW { \
		interface.setWidget(dialog); \
		interface.setCenterPosition(); \
		interface.setHidden(0); \
		interface.setPermanentFocus(); \
		dialog.setPermanentFocus(); \
	}
	
	#define DECLARE_DIALOG_HIDE { \
		interface.setWidget(NULL); \
		interface.setHidden(1); \
	}
	
	/*
	 */
	#define DECLARE_WINDOW \
		int window_show; \
		WidgetWindow window; \
		InterfaceWindow interface;
	
	#define DECLARE_WINDOW_LOAD(NAME) \
		int width = configGet(#NAME "_width",window.getWidth()); \
		int height = configGet(#NAME "_height",window.getHeight()); \
		int position_x = configGet(#NAME "_position_x",window.getPositionX()); \
		int position_y = configGet(#NAME "_position_y",window.getPositionY()); \
		interface = new InterfaceWindow(0); \
		interface.setWidth(width); \
		interface.setHeight(height); \
		interface.setPosition(position_x,position_y); \
		WidgetIcon close_i = new WidgetIcon(gui,"core/gui/window_close.png"); \
		window.addChild(close_i,GUI_ALIGN_OVERLAP | GUI_ALIGN_TOP | GUI_ALIGN_RIGHT); \
		close_i.setCallback(GUI_CLICKED,functionid(#NAME "Hide")); \
		close_i.setPosition(4,-24); \
		if(window.isSizeable()) { \
			window.setWidth(width); \
			window.setHeight(height); \
		} \
		window_show = configGet(#NAME "_show",0); \
		if(window_show) call(functionid(#NAME "Show"));
	
	#define DECLARE_WINDOW_SAVE(NAME) \
		configSet(#NAME "_width",window.getWidth()); \
		configSet(#NAME "_height",window.getHeight()); \
		configSet(#NAME "_position_x",interface.getPositionX()); \
		configSet(#NAME "_position_y",interface.getPositionY()); \
		configSet(#NAME "_show",window_show);
	
	#define DECLARE_WINDOW_SHOW(NAME) { \
		NAME ## ::window_show = 1; \
		NAME ## ::interface.setWidget(NAME ## ::window); \
		NAME ## ::interface.setHidden(0); \
		NAME ## ::window.setFocus(); \
	}
	
	#define DECLARE_WINDOW_HIDE(NAME) { \
		NAME ## ::window_show = 0; \
		NAME ## ::interface.setWidget(NULL); \
		NAME ## ::interface.setHidden(1); \
	}
	
#else
	
	/*
	 */
	#define DECLARE_DIALOG
	
	#define DECLARE_DIALOG_INIT(NAME)
	
	#define DECLARE_DIALOG_SHOW \
		dialog.arrange(); \
		gui.addChild(dialog,GUI_ALIGN_OVERLAP | GUI_ALIGN_CENTER); \
		dialog.setPermanentFocus();
	
	#define DECLARE_DIALOG_HIDE \
		gui.removeChild(dialog); \
		dialog.removeFocus(); \
	
	/*
	 */
	#define DECLARE_WINDOW \
		int window_show; \
		WidgetWindow window;
	
	#define DECLARE_WINDOW_LOAD(NAME) \
		int width = configGet(#NAME "_width",window.getWidth()); \
		int height = configGet(#NAME "_height",window.getHeight()); \
		int position_x = configGet(#NAME "_position_x",window.getPositionX()); \
		int position_y = configGet(#NAME "_position_y",window.getPositionY()); \
		WidgetIcon close_i = new WidgetIcon(gui,"core/gui/window_close.png"); \
		window.addChild(close_i,GUI_ALIGN_OVERLAP | GUI_ALIGN_TOP | GUI_ALIGN_RIGHT); \
		window.setPosition(position_x,position_y); \
		close_i.setCallbackAccel(GUI_CLICKED,APP_KEY_ESC,0,0,0); \
		close_i.setCallback(GUI_CLICKED,functionid(#NAME "Hide")); \
		close_i.setPosition(4,-24); \
		if(window.isSizeable()) { \
			window.setWidth(width); \
			window.setHeight(height); \
		} \
		window_show = configGet(#NAME "_show",0); \
		if(window_show) call(functionid(#NAME "Show"));
	
	#define DECLARE_WINDOW_SAVE(NAME) \
		configSet(#NAME "_width",window.getWidth()); \
		configSet(#NAME "_height",window.getHeight()); \
		configSet(#NAME "_position_x",window.getPositionX()); \
		configSet(#NAME "_position_y",window.getPositionY()); \
		configSet(#NAME "_show",window_show);
	
	#define DECLARE_WINDOW_SHOW(NAME) \
		NAME ## ::window_show = 1; \
		NAME ## ::window.setHidden(0); \
		gui.addChild(NAME ## ::window,GUI_ALIGN_OVERLAP); \
		NAME ## ::window.setFocus();
	
	#define DECLARE_WINDOW_HIDE(NAME) \
		NAME ## ::window_show = 0; \
		gui.removeChild(NAME ## ::window);
	
#endif

