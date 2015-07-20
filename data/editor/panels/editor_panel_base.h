/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_panel_base.h
 * Desc:    Unigine editor
 * Version: 1.00
 * Author:  Andrey Viktorov <unclebob@unigine.com>
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

#ifndef __EDITOR_PANEL_BASE_H__
#define __EDITOR_PANEL_BASE_H__

/*
 */
namespace Editor::Panels {
	
	/**************************************************************************\
	*
	* class PanelBase
	*
	\**************************************************************************/
	
	/*
	 */
	class PanelBase {
		
		using Unigine::Widgets;
		
		//
		int instance;
		UserInterface ui;
		
		//
		PanelBase() {
			instance = this;
		}
		
		~PanelBase() {
			if(ui != NULL) {
				hotkeysRemoveUiToolTips(ui);
				delete ui;
			}
		}
		
		//
		void init(string path,string prefix) {
			ui = new UserInterface(path,prefix);
			hotkeysAddUiToolTips(ui);
		}
		
		//
		void callback_redirector(string func,int panel) {
			panel.call(func);
		}
		
		//
		void callback_redirector_1(Widget widget,string func,int panel) {
			panel.call(func,widget);
		}
		
		//
		virtual void update(int need_reload) { }
		
		//
		virtual void update_nodes() { }
		
		//
		virtual void render() { }
		
		//
		virtual void worldInit() { }
	};
	
} /* namespace Editor::Panels */

#endif /* __EDITOR_PANEL_BASE_H__ */
