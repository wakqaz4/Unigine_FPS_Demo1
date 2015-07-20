/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_panel_render.h
 * Desc:    Unigine editor
 * Version: 1.0
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

#ifndef __EDITOR_PANEL_RENDER_H__
#define __EDITOR_PANEL_RENDER_H__

#include <editor/panels/editor_panel_base.h>
#include <editor/widgets/editor_widget_viewport_layout.h>

/*
 */
namespace Editor::Panels {
	
	/**************************************************************************\
	*
	* class PanelRender
	*
	\**************************************************************************/
	
	/*
	 */
	class PanelRender : PanelBase {
		
		//
		using Unigine;
		using Unigine::Widgets;
		using Editor::Widgets;
		
		Widget main_widget;
		Button main_b;
		
		WidgetEditorMenu menu;
		Widget menu_vb;
		
		WidgetCheckBox items_cb[0];
		
		CheckBox final_cb;
		CheckBox unlit_cb;
		CheckBox wireframe_cb;
		CheckBox gray_materials_cb;
		CheckBox mipmap_cb;
		
		CheckBox depth_cb;
		CheckBox color_cb;
		CheckBox normal_cb;
		
		//
		PanelRender(WidgetViewportLayout layout) {
			init("editor/panels/editor_panel_render.ui","Editor::Panels::PanelRender::");
			layout.addWidget("panel_render",main_widget);
			
			add_item(menu_vb.widget);
			
			menu = new WidgetEditorMenu(main_widget,menu_vb,ANCHOR_BOTTOM);
			menu.setCallbackHide(functionid(panel_hide),(main_b));
			
			updateItemsHotkeys();
			layout.arrangePanel("panel_render");
		}
		
		//
		void add_item(::Widget widget) {
			forloop(int i = 0; widget.getNumChilds()) {
				add_item(widget.getChild(i));
			}
			
			if(widget.getType() != WIDGET_CHECKBOX ) return;
			
			string res[0];
			strsplit(widget.getData(),",",res);
			
			if(res.size() == 0) return;
			if(engine.console.isVariable(replace(res[0],"!","")) == 0) return;
			
			WidgetCheckBox check_box = class_cast("WidgetCheckBox",widget);
			check_box.setCallback(GUI_CLICKED,functionid(console_clicked),check_box);
			items_cb.append(check_box);
		}
		
		//
		void update(int need_reload) {
		}
		
		//
		void show_clicked(WidgetButton button) {
			update_items();
			menu.setHidden(!button.isToggled());
		}
		
		//
		void panel_hide(Button button) {
			if(engine.app.getMouseButtonState(APP_BUTTON_LEFT)) {
				int x = button.getMouseX ();
				int y = button.getMouseY ();
				if(x >= 0 && x <= button.getWidth() && y >= 0 && y <= button.getHeight()) return;
			}
			button.setCallbackEnabled(GUI_CLICKED,0);
			button.setToggled(0);
			button.setCallbackEnabled(GUI_CLICKED,1);
		}
		
		//
		void update_items() {
			set_callback_enabled(0);
			
			forloop(int i = 0; items_cb.size()) {
				update_cb(items_cb[i],items_cb[i].getData());
			}
			
			final_cb.setChecked(!ambientIsEnabled());
			unlit_cb.setChecked(ambientIsEnabled());
			update_cb(wireframe_cb.widget,"render_show_triangles");
			update_cb(gray_materials_cb.widget,"render_show_normals");
			update_cb(mipmap_cb.widget,"render_show_mipmaps");
			
			depth_cb.setChecked(is_buffer("post_deferred_depth"));
			color_cb.setChecked(is_buffer("post_deferred_color"));
			normal_cb.setChecked(is_buffer("post_deferred_normal"));
			
			set_callback_enabled(1);
		}
		
		//
		void update_cb(WidgetCheckBox widget,string commands) {
			string res[0];
			strsplit(commands,",",res);
			string command = replace(res[0],"!","");
			widget.setChecked(engine.console.getInt(command) == (command == res[0]));
		}
		
		//
		void set_callback_enabled(int enabled) {
			forloop(int i = 0; items_cb.size()) {
				items_cb[i].setCallbackEnabled(GUI_CLICKED,enabled);
			}
			
			final_cb.setCallbackEnabled(GUI_CLICKED,enabled);
			unlit_cb.setCallbackEnabled(GUI_CLICKED,enabled);
			wireframe_cb.setCallbackEnabled(GUI_CLICKED,enabled);
			gray_materials_cb.setCallbackEnabled(GUI_CLICKED,enabled);
			mipmap_cb.setCallbackEnabled(GUI_CLICKED,enabled);
			
			depth_cb.setCallbackEnabled(GUI_CLICKED,enabled);
			color_cb.setCallbackEnabled(GUI_CLICKED,enabled);
			normal_cb.setCallbackEnabled(GUI_CLICKED,enabled);
		}
		
		//
		void console_clicked(WidgetCheckBox widget) {
			string res[0];
			strsplit(widget.getData(),",",res);
			
			forloop(int i = 0; res.size()) {
				string command = replace(res[i],"!","");
				if(engine.console.isVariable(command)) {
					engine.console.setInt(command,widget.isChecked()  == (command == res[i]));
				}
				else if(engine.console.isCommand(command)) {
					engine.console.run(command);
				}
			}
		}
		
		//
		void clear() {
			controlsSetPostMaterials("");
			engine.console.setInt("render_show_triangles",0);
			engine.console.setInt("render_show_normals",0);
			engine.console.setInt("render_show_mipmaps",0);
			if(ambientIsEnabled()) ambientToggle();
		}
		
		//
		void setMode(string mode) {
			clear();
			if(mode == "wireframe") engine.console.setInt("render_show_triangles",1);
			else if(mode == "gray_materials") engine.console.setInt("render_show_normals",1);
			else if(mode == "mipmap") engine.console.setInt("render_show_mipmaps",1);
			else if(mode == "unlit" && ambientIsEnabled() == 0) ambientToggle();
			
			update_items();
		}
		
		//
		void toggleRenderMode() {
			// clear others
			controlsSetPostMaterials("");
			engine.console.setInt("render_show_triangles",0);
			engine.console.setInt("render_show_normals",0);
			engine.console.setInt("render_show_mipmaps",0);
			
			ambientToggle();
			
			update_items();
		}
		
		//
		void mode_clicked(string mode,PanelRender inst) {
			inst.setMode(mode);
		}
		
		//
		void setBuffer(string name) {
			clear();
			controlsSetPostMaterials(name);
			
			update_items();
		}
		
		//
		void buffer_clicked(string name,PanelRender inst) {
			inst.setBuffer(name);
		}
		
		//
		int is_buffer(string name) {
			return name == controlsGetPostMaterials();
		}
		
		//
		void updateItemsHotkeys() {
			menu.updateItemsHotkeys();
		}
	};
	
} /* namespace Editor::Panels */

#endif /* __EDITOR_PANEL_RENDER_H__ */
