/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_panel_status_bar.h
 * Desc:    Unigine editor
 * Version: 1.0
 * Author:  Alexey Zagniy <yingaz@unigine.com>
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

#ifndef __EDITOR_PANEL_STATUS_BAR_H__
#define __EDITOR_PANEL_STATUS_BAR_H__

#include <editor/panels/editor_panel_base.h>
#include <editor/widgets/editor_widget_viewport_layout.h>

/*
 */
namespace Editor::Panels {
	
	/**************************************************************************\
	*
	* class PanelStatusBar
	*
	\**************************************************************************/
	
	/*
	 */
	class PanelStatusBar : PanelBase {
		
		//
		using Unigine;
		using Unigine::Widgets;
		using Editor::Widgets;
		
		class ThreadInfo {
			string title;
			int stop = false;
			
			ThreadInfo(string t) {
				title = t;
			}
		};
		
		Widget hbox;
		Label label;
		WidgetViewportLayout layout;
		
		ThreadInfo threads[];
		
		int need_arrange;
		
		PanelStatusBar(WidgetViewportLayout l) {
			layout = l;
			init("editor/panels/editor_panel_status_bar.ui","Editor::Panels::PanelStatusBar::");
			layout.addWidget("panel_status_bar",hbox);
			layout.setBackground("panel_status_bar",0);
			
			label.setFontOutline(true);
			need_arrange = true;
		}
		
		~PanelStatusBar() {
			threads.delete();
		}
		
		void update(int need_reload) {
			string text = "";
			
			for(int i = 0; i < threads.size(); i++) {
				ThreadInfo info = threads.get(i);
				if(info.stop) {
					threads.remove(info.title);
					i--;
					continue;
				}
				text += info.title + " \n";
			}
			label.setText(text);
			if(need_arrange) {
				WidgetViewportLayoutPanel p = layout.getPanel("panel_status_bar");
				if(p != NULL) {
					p.setLeft(layout.getParentWidth());
					p.setBottom(layout.getParentHeight() - 24);
				}
				layout.arrangePanel("panel_status_bar");
				need_arrange = false;
			}
		}
		
		void infoStart(string title) {
			ThreadInfo info = threads.check(title,NULL);
			if(info == NULL) {
				info = new ThreadInfo(title);
				threads.append(title,info);
			}
			info.stop = false;
			need_arrange = true;
		}
		
		void infoEnd(string title) {
			ThreadInfo info = threads.check(title,NULL);
			if(info == NULL) return;
			info.stop = true;
			need_arrange = true;
		}
	};
} /* namespace Editor::Panels */

#endif /* __EDITOR_PANEL_STATUS_BAR_H__ */
