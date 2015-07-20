/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_panel_navigation.h
 * Desc:    Unigine editor
 * Version: 1.01
 * Author:  Mikhail Lyubimov <alterego@unigine.com>
 *          Andrey Viktorov <unclebob@unigine.com>
 *          Maxim Belobryukhov <maxi@unigine.com>
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

#ifndef __EDITOR_PANEL_NAVIGATION_H__
#define __EDITOR_PANEL_NAVIGATION_H__

#include <editor/panels/editor_panel_base.h>
#include <editor/widgets/editor_widget_viewport_layout.h>

/*
 */
namespace Editor::Panels {
	
	/**************************************************************************\
	*
	* class PanelNavigation
	*
	\**************************************************************************/
	
	/*
	 */
	class PanelNavigation : PanelBase {
		
		//
		using Unigine::Widgets;
		using Editor::Widgets;
		
		enum {
			PRECISION = 1,
		};
		
		Widget hbox;
		
		Button velocity_index_b[3];
		
		EditLine velocity_el;
		EditLine pos_el[3];
		
		//
		PanelNavigation(WidgetViewportLayout layout) {
			
			init("editor/panels/editor_panel_navigation.ui","Editor::Panels::PanelNavigation::");
			
			set_toggled(camerasGetVelocityIndex());
			layout.addWidget("panel_navigation",hbox);
			
			layout.arrangePanel("panel_navigation");
		}
		
		//
		void update(int need_reload) {
			
			hbox.setEnabled(engine.world.isLoaded());
			
			Vec3 pos = controlsGetPosition();
			forloop(int i = 0; 3) {
				if(pos_el[i].isFocused()) continue;
				pos_el[i].setText(editorFormat(pos[i],PRECISION));
			}
			
			if(!velocity_el.isFocused()) {
				velocity_el.setText(string(camerasGetVelocity()));
			}
		}
		
		//
		void set_toggled_0() {
			set_toggled(0);
		}
		
		void set_toggled_1() {
			set_toggled(1);
		}
		
		void set_toggled_2() {
			set_toggled(2);
		}
		
		void set_toggled(int num) {
			forloop(int i = 0; 3) {
				velocity_index_b[i].setCallbackEnabled(GUI_CLICKED,0);
				velocity_index_b[i].setToggled(i == num);
				velocity_index_b[i].setCallbackEnabled(GUI_CLICKED,1);
			}
			camerasSetVelocityIndex(num);
		}
		
		void switch_speed() {
			int index = camerasGetVelocityIndex();
			index++;
			if(index > velocity_index_b.size()-1) index = 0;
			set_toggled(index);
		}
		
		//
		void transform_pressed() {
			Vec3 pos;
			
			pos.x = Scalar(pos_el[0].getText());
			pos.y = Scalar(pos_el[1].getText());
			pos.z = Scalar(pos_el[2].getText());
			
			controlsSetPosition(pos);
		}
		
		void velocity_pressed() {
			Scalar v = Scalar(velocity_el.getText());
			camerasSetVelocity(max(v,0.01f));
			velocity_el.setText(string(v));
		}
	};
	
} /* namespace Editor::Panels */

#endif /* __EDITOR_PANEL_NAVIGATION_H__ */
