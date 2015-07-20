/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_about.h
 * Desc:    Unigine editor
 * Version: 1.00
 * Author:  Mikhail Lyubimov <alterego@unigine.com>
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
namespace About {
	
	using Unigine::Widgets;
	
	UserInterface ui;
	Window window;
	
	int is_key_esc_bind = false;
	
	void init() {
		ui = new UserInterface("editor/editor_about.ui");
		hotkeysAddUiToolTips(ui);
		check_key();
	}
	
	void shutdown() {
		
	}
	
	void update() {
		if(is_key_esc_bind || isChild(window) == false) return;
		if(engine.app.getKeyState(APP_KEY_ESC)) hide();
	}
	
	int show() {
		addChild(window,ALIGN_OVERLAP | ALIGN_CENTER);
	}
	
	int hide() {
		removeChild(window);
	}
	
	void check_key() {
		is_key_esc_bind = hotkeysIsKeyBinded(APP_KEY_ESC,("Snap to surface"));
	}
}

void aboutInit() {
	About::init();
}

void aboutShutdown() {
	About::shutdown();
}

void aboutUpdate() {
	About::update();
}

void aboutShow() {
	About::show();
}
void aboutHide() {
	About::hide();
}

void aboutCheckKey() {
	About::check_key();
}
