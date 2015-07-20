/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_panel_helpers.h
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

#ifndef __EDITOR_PANEL_HELPERS_H__
#define __EDITOR_PANEL_HELPERS_H__

#include <editor/panels/editor_panel_base.h>
#include <editor/widgets/editor_widget_viewport_layout.h>

/*
 */
namespace Editor::Panels {
	
	/**************************************************************************\
	*
	* class PanelHelpers
	*
	\**************************************************************************/
	
	
	/*
	 */
	class PanelHelpers : PanelBase {
		
		//
		using Unigine;
		using Unigine::Widgets;
		using Editor::Widgets;
		
		Widget main_widget;
		Button main_b;
		
		WidgetEditorMenu menu;
		Widget menu_vb;
		
		HBox space_hb;
		ScrollBox scroll_box;
		VBox gizmo_vb;
		
		CheckBox node_info_cb;				// node info toggle
		CheckBox dynamic_info_cb;			// dynamic info toggle
		CheckBox object_bounds_cb;			// object bounds toggle
		CheckBox object_wireframe_cb;		// object wireframe toggle
		CheckBox mesh_skinned_bones_cb;		// mesh skinned bones toggle
		CheckBox particles_physics_cb;		// particles physics toggle
		
		CheckBox viewcube_cb;
		Slider size_s;
		
		WidgetCheckBox console_cb[0];
		
		WidgetCheckBox icon_cb[0];
		WidgetCheckBox gizmo_cb[0];
		
		::Widget childs_vb[0];
		
		WidgetCheckBox icon_map_cb[];
		WidgetCheckBox gizmo_map_cb[];
		
		//
		PanelHelpers(WidgetViewportLayout layout) {
			init("editor/panels/editor_panel_helpers.ui","Editor::Panels::PanelHelpers::");
			layout.addWidget("panel_helpers",main_widget);
			
			WidgetCheckBox icon_parent = NULL;
			WidgetCheckBox gizmo_parent = NULL;
			
			int num_parent = 0;
			
			void add_item(::Widget widget) {
				if(widget.getData() == "parent") {
					WidgetIcon icon;
					string name;
					forloop(int i = 0; widget.getNumChilds()) {
						::Widget child = widget.getChild(i);
						
						if(child.getType() == WIDGET_LABEL) {
							if(child.getData() == "text") child.setCallback(GUI_DOUBLE_CLICKED,functionid(icon_toggled),icon);
							else name = replace(child.getData(),"helpers_","");
						}
						else if(child.getType() == WIDGET_ICON) {
							icon = class_cast("WidgetIcon",child);
							icon.setCallback(GUI_CLICKED,functionid(toggled_childs_clicked),this,icon,num_parent);
						}
						else if(child.getData() == "icon") {
							icon_parent = class_cast("WidgetCheckBox",child);
							icon_cb.append(icon_parent);
							icon_map_cb.append(name,icon_parent);
							child.setData("");
						}
						else if(child.getData() == "gizmo") {
							gizmo_parent = class_cast("WidgetCheckBox",child);
							gizmo_cb.append(gizmo_parent);
							gizmo_map_cb.append(name,gizmo_parent);
							child.setData("");
						}
					}
					
					widget.setData("item");
					num_parent++;
					return;
				}
				
				forloop(int i = 0; widget.getNumChilds()) {
					add_item(widget.getChild(i));
				}
				
				if(widget.getData() == "childs") {
					childs_vb.append(widget);
				}
				else if(widget.getType() == WIDGET_CHECKBOX) {
					WidgetCheckBox check_box = class_cast("WidgetCheckBox",widget);
					
					string strings[0];
					strsplit(widget.getData(),",",strings);
					
					if(strings.size() == 0) return;
					
					if(engine.console.isVariable(strings[0])) {
						console_cb.append(check_box);
					}
					else if(strings[0] == "icon") {
						string type = " " + string(getNodeType(strings[1])) + " ";
						widget.setData(type);
						icon_cb.append(check_box);
						
						if(icon_parent != NULL) icon_parent.setData(icon_parent.getData() + type);
					}
					else if(strings[0] == "gizmo") {
						string type = " " + string(getNodeType(strings[1])) + " ";
						widget.setData(type);
						gizmo_cb.append(check_box);
						
						if(gizmo_parent != NULL) gizmo_parent.setData(gizmo_parent.getData() + type);
					}
					
				}
			}
			
			add_item(menu_vb.widget);
			
			menu = new WidgetEditorMenu(main_widget,menu_vb,ANCHOR_BOTTOM);
			menu.setCallbackHide(functionid(panel_hide),(this,main_b));
			
			node_info_cb.setChecked(configGet("helpers_node_info",1));
			dynamic_info_cb.setChecked(configGet("helpers_dynamic_info",1));
			object_bounds_cb.setChecked(configGet("helpers_object_bounds",1));
			object_wireframe_cb.setChecked(configGet("helpers_object_wireframe",0));
			mesh_skinned_bones_cb.setChecked(configGet("helpers_mesh_skinned_bones",1));
			particles_physics_cb.setChecked(configGet("helpers_particles_physics",1));
			

			layout.arrangePanel("panel_helpers");
		}
		
		~PanelHelpers() {
			configSet("helpers_node_info",node_info_cb.isChecked());
			configSet("helpers_dynamic_info",dynamic_info_cb.isChecked());
			configSet("helpers_object_bounds",object_bounds_cb.isChecked());
			configSet("helpers_object_wireframe",object_wireframe_cb.isChecked());
			configSet("helpers_mesh_skinned_bones",mesh_skinned_bones_cb.isChecked());
			configSet("helpers_particles_physics",particles_physics_cb.isChecked());
			
		}
		
		//
		void update(int need_reload) {
			if(menu.isHidden() && scroll_box.isFocused()) scroll_box.removeFocus();
		}
		
		//
		void show_clicked(WidgetButton button) {
			update_items();
			menu.setHidden(!button.isToggled());
			
			update_scroll_box();
		}
		
		//
		void panel_hide(PanelHelpers panel,Button button) {
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
		void set_checked(WidgetCheckBox widget,string cmd) {
			string current_str = " " + engine.console.getString(cmd) + " ";
			
			widget.setCallbackEnabled(GUI_CLICKED,0);
			
			string types[0];
			strsplit(widget.getData()," ",types);
			
			int checked = 1;
			forloop(int i = 0; types.size()) {
				checked = strstr(current_str," " + types[i] + " ") != -1;
				if(!checked) break;
			}
			
			widget.setChecked(checked);
			
			widget.setCallbackEnabled(GUI_CLICKED,1);
		}
		
		//
		void update_items() {
			menu.updateItemsHotkeys();
			
			forloop(int i = 0;console_cb.size()) {
				WidgetCheckBox widget = console_cb[i];
				widget.setCallbackEnabled(GUI_CLICKED,0);
				
				string res[0];
				strsplit(widget.getData(),",",res);
				
				int checked = true;
				forloop(int i = 0; res.size()) {
					if(engine.console.getInt(res[i])) continue;
					checked = false;
					break;
				}
				widget.setChecked(checked);
				
				widget.setCallbackEnabled(GUI_CLICKED,1);
			}
			
			forloop(int i = 0;icon_cb.size()) {
				set_checked(icon_cb[i],"world_show_handler");
			}
			
			forloop(int i = 0;gizmo_cb.size()) {
				set_checked(gizmo_cb[i],"world_show_visualizer");
			}
			
			size_s.setCallbackEnabled(GUI_CHANGED,0);
			size_s.setValue(engine.visualizer.getSize());
			size_s.setCallbackEnabled(GUI_CHANGED,1);
			
			WidgetViewportLayout layout = viewportsLayoutGet();
			viewcube_cb.setCallbackEnabled(GUI_CLICKED,0);
			viewcube_cb.setChecked(!layout.isHidden("panel_viewcube"));
			viewcube_cb.setCallbackEnabled(GUI_CLICKED,1);
		}
		
		//
		void console_clicked(WidgetCheckBox widget,PanelHelpers panel) {
			string strings[0];
			strsplit(widget.getData(),",",strings);
			
			forloop(int i = 0; strings.size()) {
				engine.console.setInt(strings[i],widget.isChecked());
			}
		}
		
		//
		void check_box_clicked(WidgetCheckBox widget,string cmd,PanelHelpers panel) {
			string current_types = " " + engine.console.getString(cmd) + " ";
			string types[0];
			strsplit(widget.getData()," ",types);
			
			forloop(int i = 0; types.size()) {
				current_types = replace(current_types," " + types[i] + " "," ");
			}
			
			if(widget.isChecked()) current_types += " " + widget.getData();
			
			current_types = re_replace(current_types,"[^0-9]+"," ");
			
			engine.console.setString(cmd,current_types);
			
			panel.update_items();
		}
		
		//
		void icon_and_gizmo_clicked(string type,PanelHelpers panel) {
			string type_id = " " + string(getNodeType(type)) + " ";
			
			void type_togled(string cmd) {
				string current_types = " " + engine.console.getString(cmd) + " ";
				if(strstr(current_types,type_id) == -1) {
					current_types += type_id;
				} else {
					current_types = replace(current_types,type_id," ");
				}
				current_types = re_replace(current_types,"[^0-9]+"," ");
				engine.console.setString(cmd,current_types);
			}
			
			type_togled("world_show_visualizer");
			type_togled("world_show_handler");
			
			panel.update_items();
		}
		
		//
		void toggled_childs_clicked(PanelHelpers panel,WidgetIcon icon,int num) {
			
			::Widget childs = panel.childs_vb[num];
			childs.setHidden(icon.isToggled());
			
			panel.update_scroll_box();
		}
		
		//
		void icon_toggled(WidgetIcon icon) {
			icon.setToggled(!icon.isToggled());
		}
		
		//
		void checkbox_toggled(WidgetCheckBox cb) {
			cb.setChecked(!cb.isChecked());
		}
		
		//
		void toggle_icon_gizmo(string name) {
			if(gizmo_map_cb.check(name)) checkbox_toggled(gizmo_map_cb[name]);
			if(icon_map_cb.check(name)) checkbox_toggled(icon_map_cb[name]);
		}
		
		//
		void update_scroll_box() {
			gizmo_vb.arrange();
			int height = gizmo_vb.getHeight();
			
			int width = gizmo_vb.getWidth();
			if(width < 170) gizmo_vb.setWidth(170);
			
			space_hb.setHidden(1);
			if(height > 512) {
				height = 512;
				space_hb.setHidden(0);
			}
			
			scroll_box.setHeight(height);
			
			menu.arrange();
		}
		
		//
		void size_changed() {
			engine.visualizer.setSize(size_s.getValue());
		}
		
		//
		void viewcube_clicked(WidgetCheckBox widget) {
			WidgetViewportLayout layout = viewportsLayoutGet();
			layout.setHidden("panel_viewcube",!widget.isChecked());
		}
		
	};
	
} /* namespace Editor::Panels */

#endif /* __EDITOR_PANEL_HELPERS_H__ */
