/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_objects_terrain.h
 * Desc:    Unigine editor
 * Version: 1.23
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Andrey Viktorov <unclebob@unigine.com>
 *          Mikhail Lyubimov <alterego@unigine.com>
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

/******************************************************************************\
*
* Callbacks
*
\******************************************************************************/

/*
 */
#define TERRAIN_SURFACE_PRESSED_CALLBACK(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	if(x != -1 && y != -1) { \
		if(terrain.get ## FUNC(x,y) != WIDGET ## _el.getText()) { \
			NodesState_ s = new NodesState_(); \
			s.init_modify(nodes,terrain); \
			terrain.set ## FUNC(x,y,WIDGET ## _el.getText()); \
			save_state(s); \
		} \
		WIDGET ## _el.setText(terrain.get ## FUNC(x,y)); \
		if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
			WIDGET ## _el.setCursor(0); \
			WIDGET ## _el.setSelection(1024); \
		} \
	} \
}

/*
 */
#define TERRAIN_SURFACE_LOAD_CLICKED_CALLBACK(WIDGET,FUNC,TITLE) \
void WIDGET ## _load_clicked() { \
	if(x != -1 && y != -1) { \
		string name = terrain.get ## FUNC(x,y); \
		if(dialogFile(TITLE,".dds",name,DIALOG_FILE_OPEN)) { \
			name = setExtensionImage(name,"dds"); \
			if(terrain.get ## FUNC(x,y) != name) { \
				NodesState_ s = new NodesState_(); \
				s.init_modify(nodes,terrain); \
				terrain.set ## FUNC(x,y,name); \
				WIDGET ## _el.setText(terrain.get ## FUNC(x,y)); \
				save_state(s); \
			} \
		} \
	} \
}

#define TERRAIN_SURFACE_VIEW_CLICKED_CALLBACK(WIDGET,FUNC,TITLE) \
void WIDGET ## _view_clicked() { \
	if(x != -1 && y != -1) { \
		if(terrain.get ## FUNC(x,y) != "") { \
			dialogImage(TITLE,terrain.get ## FUNC(x,y)); \
		} \
	} \
}

#define TERRAIN_SURFACE_CLEAR_CLICKED_CALLBACK(WIDGET,FUNC,TITLE) \
void WIDGET ## _clear_clicked() { \
	if(x != -1 && y != -1) { \
		if(terrain.get ## FUNC(x,y) != "" || surfaces_lb.getNumSelectedItems() > 1) { \
			NodesState_ s = new NodesState_(); \
			if(dialogMessageYesNo(TR("Confirm"),TITLE)) { \
				s.init_modify(nodes,terrain); \
				forloop(int i = 0; surfaces_lb.getNumSelectedItems()) { \
					int item = surfaces_lb.getSelectedItem(i); \
					int x = item % terrain.getSurfacesX(); \
					int y = item / terrain.getSurfacesX(); \
					terrain.set ## FUNC(x,y,NULL); \
				} \
				WIDGET ## _el.setText(terrain.get ## FUNC(x,y)); \
				save_state(s); \
			} \
		} \
	} \
}

/*
 */
#define TERRAIN_ARRAY_PRESSED_CALLBACK(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	if(terrain.get ## FUNC() != WIDGET ## _el.getText()) { \
		NodesState_ s = new NodesState_(); \
		s.init_modify(nodes,terrain); \
		terrain.set ## FUNC(WIDGET ## _el.getText()); \
		save_state(s); \
	} \
	WIDGET ## _el.setText(terrain.get ## FUNC()); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
}

#define TERRAIN_ARRAY_LOAD_CLICKED_CALLBACK(WIDGET,FUNC,TITLE) \
void WIDGET ## _load_clicked() { \
	string name = terrain.get ## FUNC(); \
	if(dialogFile(TITLE,".dds",name,DIALOG_FILE_OPEN)) { \
		name = setExtensionImage(name,"dds"); \
		if(terrain.get ## FUNC() != name) { \
			NodesState_ s = new NodesState_(); \
			s.init_modify(nodes,terrain); \
			terrain.set ## FUNC(name); \
			WIDGET ## _el.setText(terrain.get ## FUNC()); \
			save_state(s); \
		} \
	} \
}

#define TERRAIN_ARRAY_VIEW_CLICKED_CALLBACK(WIDGET,FUNC,TITLE) \
void WIDGET ## _view_clicked() { \
	if(terrain.get ## FUNC() != "") { \
		dialogImage(TITLE,terrain.get ## FUNC()); \
	} \
}

#define TERRAIN_ARRAY_CLEAR_CLICKED_CALLBACK(WIDGET,FUNC,TITLE) \
void WIDGET ## _clear_clicked() { \
	if(terrain.get ## FUNC() != "") { \
		NodesState_ s = new NodesState_(); \
		if(dialogMessageYesNo(TR("Confirm"),TITLE)) { \
			s.init_modify(nodes,terrain); \
			terrain.set ## FUNC(NULL); \
			WIDGET ## _el.setText(terrain.get ## FUNC()); \
			save_state(s); \
		} \
	} \
}

/*
 */
#define TERRAIN_MATERIAL_PRESSED_CALLBACK(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	string path = trim(WIDGET ## _el.getText()); \
	\
	if(terrain.get ## FUNC(num) != path) { \
		\
		Image image = new Image(); \
		string error = ""; \
		\
		if(!image.info(path) && !image.load(path)) { \
			error = format(TR("Failed to get image info: %s"),path); \
		} else if(image.getType() != IMAGE_2D) { \
			error = format(TR("Unsupported texture type (%s). Only 2D textures with unsigned texture format can be used."),image.getTypeName()); \
		} else if(image.isFloatFormat() || image.isHalfFormat()) { \
			error = format(TR("Unsupported texture format (%s). Only 2D textures with unsigned texture format can be used."),image.getFormatName()); \
		} \
		\
		delete image; \
		\
		if(error != "") { \
			WIDGET ## _el.setText(terrain.get ## FUNC(num)); \
			dialogMessageOk(TR("Error"),error); \
			return; \
		} \
		\
		NodesState_ s = new NodesState_(); \
		s.init_modify(nodes,terrain); \
		terrain.set ## FUNC(num,path); \
		WIDGET ## _el.setText(terrain.get ## FUNC(num)); \
		update_textures(WIDGET ## _el);\
		save_state(s); \
		\
	} \
	WIDGET ## _el.setText(terrain.get ## FUNC(num)); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
}

#define TERRAIN_MATERIAL_RESIZE_CALLBACK(WIDGET,NAME) \
void WIDGET ## _array_resize() { \
	NodesState_ s = new NodesState_(); \
	s.init_modify(nodes,terrain); \
	update_textures(WIDGET ## _el); \
	save_state(s); \
}\

#define TERRAIN_MATERIAL_LOAD_CLICKED_CALLBACK(WIDGET,FUNC,TITLE,TYPE) \
void WIDGET ## _load_clicked() { \
	string name = terrain.getMaterial ## FUNC ## TextureName(num); \
	if(texturesDialogImage(TITLE,TYPE,name,false)) { \
		if(terrain.getMaterial ## FUNC ## TextureName(num) != name) { \
			NodesState_ s = new NodesState_(); \
			s.init_modify(nodes,terrain); \
			terrain.setMaterial ## FUNC ## TextureName(num,name); \
			WIDGET ## _el.setText(name); \
			update_textures(WIDGET ## _el);\
			save_state(s); \
		} \
	} \
}

#define TERRAIN_MATERIAL_VIEW_CLICKED_CALLBACK(WIDGET,FUNC,TITLE) \
void WIDGET ## _view_clicked() { \
	if(terrain.get ## FUNC(num) != "") { \
		dialogImage(TITLE,terrain.get ## FUNC(num)); \
	} \
}

#define TERRAIN_MATERIAL_EDIT_CLICKED_CALLBACK(WIDGET,FUNC,TYPE) \
void WIDGET ## _edit_clicked() { \
	if(terrain.get ## FUNC(num) != "") { \
		if(dialogTexture(format(TR("%s texture"),TYPE),TYPE,terrain.get ## FUNC(num),1)) { \
			update_textures(WIDGET ## _el); \
		} \
	} \
}

#define TERRAIN_MATERIAL_PRESSED_CALLBACK_SLIDER(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	int previous_focus; \
	float value = float(WIDGET ## _el.getText()); \
	int current_focus = (WIDGET ## _sl.isFocused() || WIDGET ## _el.isFocused()); \
	if(!(previous_focus || current_focus) && terrain.get ## FUNC(num) == value) { \
		WIDGET ## _el.setText(editorFormat(value)); \
		return; \
	} \
	previous_focus = current_focus; \
	if(terrain.get ## FUNC(num) != value) { \
		NodesState_ s = new NodesState_(); \
		s.init_modify(nodes,terrain); \
		terrain.set ## FUNC(num,value); \
		save_state(s,engine.gui.getFocus()); \
	} \
	WIDGET ## _el.setText(editorFormat(value)); \
}

/******************************************************************************\
*
* Terrain
*
\******************************************************************************/

/*
 */
namespace Terrain {
	
	/*
	 */
	WidgetVBox main_vb;
	WidgetVBox surfaces_vb;
	WidgetVBox materials_vb;
	WidgetVBox grabber_vb;
	WidgetEditLine height_scale_el;
	
	string heights_name;
	string holes_name;
	string diffuse_name;
	string normal_name;
	string index_name;
	string mask_name;
	
	enum {
		TERRAIN_SURFACE_STEP = 256,
	};
	
	ObjectTerrain terrain;
	
	/*
	 */
	void view_image(WidgetEditLine editline) {
		string name = editline.getText();
		if(is_file(savePath(name)) == 0) return;
		dialogImage("View texture",savePath(name));
	}
	
	#include <editor/editor_objects_terrain_parameters.h>
	#include <editor/editor_objects_terrain_import.h>
	
	/**************************************************************************\
	*
	* Surfaces
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Surfaces {
		
		/*
		 */
		WidgetListBox surfaces_lb;
		WidgetCheckBox enabled_cb;
		WidgetCheckBox cast_shadow_cb;
		WidgetEditLine lod_threshold_el;
		WidgetEditLine diffuse_texture_el;
		WidgetIcon diffuse_texture_load_i;
		WidgetIcon diffuse_texture_view_i;
		WidgetIcon diffuse_texture_clear_i;
		WidgetEditLine normal_texture_el;
		WidgetIcon normal_texture_load_i;
		WidgetIcon normal_texture_view_i;
		WidgetIcon normal_texture_clear_i;
		WidgetEditLine index_texture_el;
		WidgetIcon index_texture_load_i;
		WidgetIcon index_texture_view_i;
		WidgetIcon index_texture_clear_i;
		WidgetEditLine mask_texture_el;
		WidgetIcon mask_texture_load_i;
		WidgetIcon mask_texture_view_i;
		WidgetIcon mask_texture_clear_i;
		
		/*
		 */
		int item = -1;
		int x = -1;
		int y = -1;
		
		/*
		 */
		void update_view() {
			
			enabled_cb.setCallbackEnabled(GUI_CLICKED,0);
			cast_shadow_cb.setCallbackEnabled(GUI_CLICKED,0);
			
			// clear view
			if(x == -1 || y == -1) {
				
				setEnabled(0,(
					enabled_cb,
					cast_shadow_cb,
					lod_threshold_el,
					diffuse_texture_el,
					diffuse_texture_load_i,
					diffuse_texture_view_i,
					diffuse_texture_clear_i,
					normal_texture_el,
					normal_texture_load_i,
					normal_texture_view_i,
					normal_texture_clear_i,
					index_texture_el,
					index_texture_load_i,
					index_texture_view_i,
					index_texture_clear_i,
					mask_texture_el,
					mask_texture_load_i,
					mask_texture_view_i,
					mask_texture_clear_i,
				));
				
				enabled_cb.setChecked(0);
				cast_shadow_cb.setChecked(0);
				lod_threshold_el.setText("");
				diffuse_texture_el.setText("");
				normal_texture_el.setText("");
				index_texture_el.setText("");
				mask_texture_el.setText("");
			}
			// multi view
			else if(surfaces_lb.getNumSelectedItems() > 1) {
				
				setEnabled(1,(
					enabled_cb,
					cast_shadow_cb,
					lod_threshold_el,
					diffuse_texture_clear_i,
					normal_texture_clear_i,
					index_texture_clear_i,
					mask_texture_clear_i,
				));
				
				setEnabled(0,(
					diffuse_texture_el,
					diffuse_texture_load_i,
					diffuse_texture_view_i,
					normal_texture_el,
					normal_texture_load_i,
					normal_texture_view_i,
					index_texture_el,
					index_texture_load_i,
					index_texture_view_i,
					mask_texture_el,
					mask_texture_load_i,
					mask_texture_view_i,
				));
				
				enabled_cb.setChecked(terrain.isSurfaceEnabled(x,y));
				cast_shadow_cb.setChecked(terrain.isSurfaceCastShadow(x,y));
				lod_threshold_el.setText(editorFormat(terrain.getSurfaceLodThreshold(x,y)));
				diffuse_texture_el.setText("");
				normal_texture_el.setText("");
				index_texture_el.setText("");
				mask_texture_el.setText("");
			}
			// fill view
			else {
				
				setEnabled(1,(
					enabled_cb,
					cast_shadow_cb,
					lod_threshold_el,
					diffuse_texture_el,
					diffuse_texture_load_i,
					diffuse_texture_view_i,
					diffuse_texture_clear_i,
					normal_texture_el,
					normal_texture_load_i,
					normal_texture_view_i,
					normal_texture_clear_i,
					index_texture_el,
					index_texture_load_i,
					index_texture_view_i,
					index_texture_clear_i,
					mask_texture_el,
					mask_texture_load_i,
					mask_texture_view_i,
					mask_texture_clear_i,
				));
				
				enabled_cb.setChecked(terrain.isSurfaceEnabled(x,y));
				cast_shadow_cb.setChecked(terrain.isSurfaceCastShadow(x,y));
				lod_threshold_el.setText(editorFormat(terrain.getSurfaceLodThreshold(x,y)));
				diffuse_texture_el.setText(terrain.getSurfaceDiffuseTextureName(x,y));
				normal_texture_el.setText(terrain.getSurfaceNormalTextureName(x,y));
				index_texture_el.setText(terrain.getSurfaceIndexTextureName(x,y));
				mask_texture_el.setText(terrain.getSurfaceMaskTextureName(x,y));
			}
			
			enabled_cb.setCallbackEnabled(GUI_CLICKED,1);
			cast_shadow_cb.setCallbackEnabled(GUI_CLICKED,1);
		}
		
		/*
		 */
		void update() {
			
			item = -1;
			x = -1;
			y = -1;
			
			surfaces_lb.clear();
			forloop(int y = 0; terrain.getSurfacesY()) {
				forloop(int x = 0; terrain.getSurfacesX()) {
					surfaces_lb.addItem(get_item_text(x,y));
				}
			}
			
			update_view();
		}
		
		/*
		 */
		void update_dynamic() {
			
			if(parameters_tb.getCurrentTab() == 6) {
				
				// show terrain surfaces
				forloop(int i = 0; surfaces_lb.getNumSelectedItems()) {
					int item = surfaces_lb.getSelectedItem(i);
					int x = item % terrain.getSurfacesX();
					int y = item / terrain.getSurfacesX();
					Vec3 min = terrain.getSurfaceBoundBox(x,y).getMin();
					Vec3 max = terrain.getSurfaceBoundBox(x,y).getMax();
					engine.visualizer.renderSolidBox(max - min,terrain.getWorldTransform() * translate((min + max) * 0.5f),vec4(0.5f));
				}
				
				// current terrain surface
				if(x != -1 && y != -1) {
					if(hotkeysClearState("nodes_focus")) {
						BoundSphere bs = terrain.getSurfaceBoundSphere(x,y);
						controlsShowObject(terrain.getWorldTransform() * bs.getCenter(),bs.getRadius());
					}
				}
				
				int get_selected_surface() {
					vec3 p0,p1;
					Unigine::getPlayerMouseDirection(p0,p1);
					
					WorldIntersectionTexCoord intersection = new WorldIntersectionTexCoord();
					if(engine.world.getIntersection(p0,p1,~0,intersection) == terrain) {
						vec4 texcoord = intersection.getTexCoord();
						int x = clamp(int(texcoord.x * terrain.getSurfacesX()),0,terrain.getSurfacesX() - 1);
						int y = clamp(int(texcoord.y * terrain.getSurfacesY()),0,terrain.getSurfacesY() - 1);
						return terrain.getSurfacesX() * y + x;
					}
					
					return -1;
				}
				
				// select terrain surface
				if(getGuiActivity() == 0 && panelViewCubeFocused() == 0) {
					
					if(hotkeysGetState("select")) {
						hotkeysClearState("select");
						
						int item = get_selected_surface();
						
						if(item >= 0) {
							surfaces_lb.clearSelection();
							surfaces_lb.setItemSelected(item,1);
							surfaces_lb.showItem(item);
						}
					}
					else if(hotkeysGetState("add_selection")) {
						hotkeysClearState("add_selection");
						
						int item = get_selected_surface();
						
						if(item >= 0) {
							surfaces_lb.setItemSelected(item,!surfaces_lb.isItemSelected(item));
							surfaces_lb.showItem(item);
						}
					}
				}
			}
		}
		
		/*
		 */
		string get_item_text(int x,int y) {
			
			string text = format("%02xx%02x",x,y);
			
			if(terrain.isSurfaceEnabled(x,y) == 0) text += TR(" - disabled");
			if(terrain.isSurfaceCastShadow(x,y) != 0) text += TR(" - shadow");
			if(terrain.getSurfaceLodThreshold(x,y) >= 0.0f) text += " - " + editorFormat(terrain.getSurfaceLodThreshold(x,y));
			
			return text;
		}
		
		/*
		 */
		void reload() {
			
			forloop(int y = 0; terrain.getSurfacesY()) {
				forloop(int x = 0; terrain.getSurfacesX()) {
					int item = terrain.getSurfacesX() * y + x;
					surfaces_lb.setItemText(item,get_item_text(x,y));
				}
			}
		}
		
		/*
		 */
		void surface_changed() {
			set_wireframe_mode(1);
			if(surfaces_lb.getNumSelectedItems()) {
				item = surfaces_lb.getSelectedItem(0);
				x = item % terrain.getSurfacesX();
				y = item / terrain.getSurfacesX();
			} else {
				item = -1;
				x = -1;
				y = -1;
			}
			
			update_view();
		}
		
		/*
		 */
		void enabled_clicked() {
			
			forloop(int i = 0; surfaces_lb.getNumSelectedItems()) {
				int item = surfaces_lb.getSelectedItem(i);
				int x = item % terrain.getSurfacesX();
				int y = item / terrain.getSurfacesX();
				terrain.setSurfaceEnabled(x,y,enabled_cb.isChecked());
				surfaces_lb.setItemText(item,get_item_text(x,y));
			}
			
			if(terrain.saveTerrain(systemEditorPath(terrain.getTerrainName())) == 0) {
				dialogMessageOk(TR("Error"));
				return;
			}
		}
		
		void cast_shadow_clicked() {
			
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,terrain);
			
			forloop(int i = 0; surfaces_lb.getNumSelectedItems()) {
				int item = surfaces_lb.getSelectedItem(i);
				int x = item % terrain.getSurfacesX();
				int y = item / terrain.getSurfacesX();
				terrain.setSurfaceCastShadow(x,y,cast_shadow_cb.isChecked());
				surfaces_lb.setItemText(item,get_item_text(x,y));
			}
			
			save_state(s);
		}
		
		void lod_threshold_pressed() {
			
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,terrain);
			
			float threshold = float(lod_threshold_el.getText());
			
			forloop(int i = 0; surfaces_lb.getNumSelectedItems()) {
				int x = item % terrain.getSurfacesX();
				int y = item / terrain.getSurfacesX();
				terrain.setSurfaceLodThreshold(x,y,threshold);
				surfaces_lb.setItemText(item,get_item_text(x,y));
			}
			
			lod_threshold_el.setText(editorFormat(threshold));
			if(toolsGetSelection() && lod_threshold_el.isFocused()) {
				lod_threshold_el.setCursor(0);
				lod_threshold_el.setSelection(1024);
			}
			
			save_state(s);
		}
		
		/*
		 */
		TERRAIN_SURFACE_PRESSED_CALLBACK(diffuse_texture,SurfaceDiffuseTextureName)
		TERRAIN_SURFACE_LOAD_CLICKED_CALLBACK(diffuse_texture,SurfaceDiffuseTextureName,TR("Select surface diffuse texture"))
		TERRAIN_SURFACE_VIEW_CLICKED_CALLBACK(diffuse_texture,SurfaceDiffuseTextureName,TR("Surface diffuse texture"))
		TERRAIN_SURFACE_CLEAR_CLICKED_CALLBACK(diffuse_texture,SurfaceDiffuseTextureName,TR("Clear surface diffuse texture?"))
		
		TERRAIN_SURFACE_PRESSED_CALLBACK(normal_texture,SurfaceNormalTextureName)
		TERRAIN_SURFACE_LOAD_CLICKED_CALLBACK(normal_texture,SurfaceNormalTextureName,TR("Select surface normal texture"))
		TERRAIN_SURFACE_VIEW_CLICKED_CALLBACK(normal_texture,SurfaceNormalTextureName,TR("Surface normal texture"))
		TERRAIN_SURFACE_CLEAR_CLICKED_CALLBACK(normal_texture,SurfaceNormalTextureName,TR("Clear surface normal texture?"))
		
		TERRAIN_SURFACE_PRESSED_CALLBACK(index_texture,SurfaceIndexTextureName)
		TERRAIN_SURFACE_LOAD_CLICKED_CALLBACK(index_texture,SurfaceIndexTextureName,TR("Select surface index texture"))
		TERRAIN_SURFACE_VIEW_CLICKED_CALLBACK(index_texture,SurfaceIndexTextureName,TR("Surface index texture"))
		TERRAIN_SURFACE_CLEAR_CLICKED_CALLBACK(index_texture,SurfaceIndexTextureName,TR("Clear surface index texture?"))
		
		TERRAIN_SURFACE_PRESSED_CALLBACK(mask_texture,SurfaceMaskTextureName)
		TERRAIN_SURFACE_LOAD_CLICKED_CALLBACK(mask_texture,SurfaceMaskTextureName,TR("Select surface mask texture"))
		TERRAIN_SURFACE_VIEW_CLICKED_CALLBACK(mask_texture,SurfaceMaskTextureName,TR("Surface mask texture"))
		TERRAIN_SURFACE_CLEAR_CLICKED_CALLBACK(mask_texture,SurfaceMaskTextureName,TR("Clear surface mask texture?"))
	}
	
	/**************************************************************************\
	*
	* Materials
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Materials {
		
		/*
		 */
		WidgetListBox materials_lb;
		WidgetButton remove_b;
		WidgetButton clone_b;
		WidgetButton up_b;
		WidgetButton down_b;
		WidgetEditLine name_el;
		WidgetEditLine transform_el[4];
		WidgetComboBox parent_cb;
		WidgetCheckBox overlap_cb;
		WidgetCheckBox mask_base_cb;
		WidgetEditLine mask_value_el;
		WidgetSlider mask_value_sl;
		WidgetEditLine mask_threshold_el;
		WidgetSlider mask_threshold_sl;
		WidgetEditLine mask_width_el;
		WidgetSlider mask_width_sl;
		WidgetEditLine diffuse_texture_el;
		WidgetIcon diffuse_texture_load_i;
		WidgetIcon diffuse_texture_view_i;
		WidgetEditLine diffuse_scale_el;
		WidgetSlider diffuse_scale_sl;
		WidgetEditLine normal_texture_el;
		WidgetIcon normal_texture_load_i;
		WidgetIcon normal_texture_view_i;
		WidgetEditLine normal_scale_el;
		WidgetSlider normal_scale_sl;
		WidgetEditLine specular_texture_el;
		WidgetIcon specular_texture_load_i;
		WidgetIcon specular_texture_view_i;
		WidgetEditLine specular_scale_el;
		WidgetSlider specular_scale_sl;
		WidgetComboBox diffuse_texture_array_cb;
		WidgetEditLine diffuse_texture_array_el;
		WidgetComboBox normal_texture_array_cb;
		WidgetEditLine normal_texture_array_el;
		WidgetComboBox specular_texture_array_cb;
		WidgetEditLine specular_texture_array_el;
		
		/*
		 */
		int num = -1;
		
		/*
		 */
		void update_view() {
			
			if(terrain == NULL || num >= terrain.getNumMaterials()) {
				num = -1;
			}
			
			// clear view
			if(num == -1) {
				
				setEnabled(0,(
					name_el,
					parent_cb,
					overlap_cb,
					mask_base_cb,
					mask_value_el,
					mask_width_el,
					mask_threshold_el,
					diffuse_texture_el,
					diffuse_texture_load_i,
					diffuse_texture_view_i,
					diffuse_scale_el,
					normal_texture_el,
					normal_texture_load_i,
					normal_texture_view_i,
					normal_scale_el,
					specular_texture_el,
					specular_texture_load_i,
					specular_texture_view_i,
					specular_scale_el,
				));
				
				setEnabled(0,transform_el);
				
				name_el.setText("");
				
				parent_cb.clear();
				
				overlap_cb.setChecked(0);
				mask_base_cb.setChecked(0);
				
				forloop(int i = 0; 4) {
					transform_el[i].setText("");
				}
				
				mask_value_el.setText("");
				mask_width_el.setText("");
				mask_threshold_el.setText("");
				
				diffuse_texture_el.setText("");
				diffuse_scale_el.setText("");
				normal_texture_el.setText("");
				normal_scale_el.setText("");
				specular_texture_el.setText("");
				specular_scale_el.setText("");
			}
			// fill view
			else {
				
				setEnabled(1,(
					name_el,
					parent_cb,
					overlap_cb,
					mask_base_cb,
					mask_value_el,
					mask_width_el,
					mask_threshold_el,
					diffuse_texture_el,
					diffuse_texture_load_i,
					diffuse_texture_view_i,
					diffuse_scale_el,
					normal_texture_el,
					normal_texture_load_i,
					normal_texture_view_i,
					normal_scale_el,
					specular_texture_el,
					specular_texture_load_i,
					specular_texture_view_i,
					specular_scale_el,
				));
				
				setEnabled(1,transform_el);
				
				name_el.setText(terrain.getMaterialName(num));
				
				parent_cb.setCallbackEnabled(GUI_CHANGED,0);
				parent_cb.clear();
				parent_cb.addItem(TR("None"));
				forloop(int i = 0; num) {
					parent_cb.addItem(terrain.getMaterialName(i));
				}
				parent_cb.setCurrentItem(terrain.getMaterialParent(num) + 1);
				parent_cb.setCallbackEnabled(GUI_CHANGED,1);
				
				overlap_cb.setChecked(terrain.getMaterialOverlap(num));
				
				mask_base_cb.setCallbackEnabled(GUI_CLICKED,0);
				mask_base_cb.setChecked(terrain.getMaterialMaskBase(num));
				mask_base_cb.setCallbackEnabled(GUI_CLICKED,1);
				
				setEditLineVec4(transform_el,terrain.getMaterialTransform(num));
				
				mask_value_el.setText(editorFormat(terrain.getMaterialMaskValue(num)));
				mask_width_el.setText(editorFormat(terrain.getMaterialMaskWidth(num)));
				mask_threshold_el.setText(editorFormat(terrain.getMaterialMaskThreshold(num)));
				
				diffuse_texture_el.setText(terrain.getMaterialDiffuseTextureName(num));
				diffuse_scale_el.setText(editorFormat(terrain.getMaterialDiffuseScale(num)));
				normal_texture_el.setText(terrain.getMaterialNormalTextureName(num));
				normal_scale_el.setText(editorFormat(terrain.getMaterialNormalScale(num)));
				specular_texture_el.setText(terrain.getMaterialSpecularTextureName(num));
				specular_scale_el.setText(editorFormat(terrain.getMaterialSpecularScale(num)));
			}
			
			diffuse_texture_array_el.setText(terrain.getDiffuseTextureArrayName());
			normal_texture_array_el.setText(terrain.getNormalTextureArrayName());
			specular_texture_array_el.setText(terrain.getSpecularTextureArrayName());
		}
		
		/*
		 */
		void update() {
			
			num = -1;
			
			materials_lb.clear();
			forloop(int i = 0; terrain.getNumMaterials()) {
				int item = materials_lb.addItem("");
				update_material(item);
			}
			materials_lb.setCurrentItem(-1);
			
			if(terrain.getNumMaterials()) materials_lb.setCurrentItem(0);
			else update_view();
			
			Editor::Panels::PanelBrush panel_brush = panelsGetPanelBrush();
			panel_brush.update_materials(terrain);
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		string get_material_name(int num) {
			string name = terrain.getMaterialName(num);
			int parent = terrain.getMaterialParent(num);
			if(name == "") name = format(TR("Material %d"),num);
			if(terrain.getMaterialOverlap(num)) name += TR(" - Overlap");
			else name += TR(" - Overlay");
			if(parent >= 0 && parent < terrain.getNumMaterials()) name += " - " + terrain.getMaterialName(parent);
			return name;
		}
		
		void update_material(int num) {
			materials_lb.setItemText(num,get_material_name(num));
			if(terrain.getMaterialOverlap(num)) materials_lb.setItemColor(num,vec4(1.0f,1.0f,0.0f,1.0f));
			else materials_lb.setItemColor(num,vec4(1.0f));
		}
		
		/*
		 */
		void add_clicked() {
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,object);
			int item = terrain.addMaterial(getNewMaterialNameForTerrain(terrain));
			Import::addMask(terrain.getMaterialName(item));
			
			save_state(s);
			update();
			materials_lb.setCurrentItem(item);
			update_textures(NULL);
			material_changed();
		}
		
		void remove_clicked() {
			
			string name = get_material_name(num);
			
			if(dialogMessageYesNo(TR("Confirm"),format(TR("Remove \"%s\" terrain material?"),name))) {
				Import::removeMask(terrain.getMaterialName(num));
				
				int item = num;
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,object);
				terrain.removeMaterial(num);
				
				forloop(int i = 0;terrain.getNumMaterials()) {
					int parent = terrain.getMaterialParent(i);
					
					if(parent == num) terrain.setMaterialParent(i,-1);
					else if(parent > num) terrain.setMaterialParent(i,parent - 1);
				}
				
				save_state(s);
				update();
				if(item > 0) materials_lb.setCurrentItem(item - 1);
				update_textures(NULL);
			}
			material_changed();
		}
		
		void clone_clicked() {
			
			string name = get_material_name(num);
			
			if(dialogMessageYesNo(TR("Confirm"),format(TR("Clone \"%s\" terrain material?"),name))) {
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,object);
				int item = terrain.addMaterial(terrain.getMaterialName(num) + " - clone");
				terrain.setMaterialTransform(item,terrain.getMaterialTransform(num));
				terrain.setMaterialParent(item,terrain.getMaterialParent(num));
				terrain.setMaterialOverlap(item,terrain.getMaterialOverlap(num));
				terrain.setMaterialMaskBase(item,terrain.getMaterialMaskBase(num));
				terrain.setMaterialMaskValue(item,terrain.getMaterialMaskValue(num));
				terrain.setMaterialMaskWidth(item,terrain.getMaterialMaskWidth(num));
				terrain.setMaterialMaskThreshold(item,terrain.getMaterialMaskThreshold(num));
				terrain.setMaterialDiffuseTextureName(item,terrain.getMaterialDiffuseTextureName(num));
				terrain.setMaterialNormalTextureName(item,terrain.getMaterialNormalTextureName(num));
				terrain.setMaterialSpecularTextureName(item,terrain.getMaterialSpecularTextureName(num));
				terrain.setMaterialDiffuseScale(item,terrain.getMaterialDiffuseScale(num));
				terrain.setMaterialNormalScale(item,terrain.getMaterialNormalScale(num));
				terrain.setMaterialSpecularScale(item,terrain.getMaterialSpecularScale(num));
				
				Import::addMask(terrain.getMaterialName(item));
				
				save_state(s);
				update();
				materials_lb.setCurrentItem(item);
				update_textures(NULL);
			}
			material_changed();
		}
		
		void swap(int num_0,int num_1) {
			
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,object);
			
			string name = terrain.getMaterialName(num_0);
			vec4 transform = terrain.getMaterialTransform(num_0);
			int overlap = terrain.getMaterialOverlap(num_0);
			int mask_base = terrain.getMaterialMaskBase(num_0);
			float mask_value = terrain.getMaterialMaskValue(num_0);
			float mask_width = terrain.getMaterialMaskWidth(num_0);
			float mask_threshold = terrain.getMaterialMaskThreshold(num_0);
			string diffuse_name = terrain.getMaterialDiffuseTextureName(num_0);
			string normal_name = terrain.getMaterialNormalTextureName(num_0);
			string specular_name = terrain.getMaterialSpecularTextureName(num_0);
			float diffuse_scale = terrain.getMaterialDiffuseScale(num_0);
			float normal_scale = terrain.getMaterialNormalScale(num_0);
			float specular_scale = terrain.getMaterialSpecularScale(num_0);
			int parent_0 = terrain.getMaterialParent(num_0);
			if(parent_0 == num_1) parent_0 = num_0;
			
			terrain.setMaterialName(num_0,terrain.getMaterialName(num_1));
			terrain.setMaterialTransform(num_0,terrain.getMaterialTransform(num_1));
			terrain.setMaterialOverlap(num_0,terrain.getMaterialOverlap(num_1));
			terrain.setMaterialMaskBase(num_0,terrain.getMaterialMaskBase(num_1));
			terrain.setMaterialMaskValue(num_0,terrain.getMaterialMaskValue(num_1));
			terrain.setMaterialMaskWidth(num_0,terrain.getMaterialMaskWidth(num_1));
			terrain.setMaterialMaskThreshold(num_0,terrain.getMaterialMaskThreshold(num_1));
			terrain.setMaterialDiffuseTextureName(num_0,terrain.getMaterialDiffuseTextureName(num_1));
			terrain.setMaterialNormalTextureName(num_0,terrain.getMaterialNormalTextureName(num_1));
			terrain.setMaterialSpecularTextureName(num_0,terrain.getMaterialSpecularTextureName(num_1));
			terrain.setMaterialDiffuseScale(num_0,terrain.getMaterialDiffuseScale(num_1));
			terrain.setMaterialNormalScale(num_0,terrain.getMaterialNormalScale(num_1));
			terrain.setMaterialSpecularScale(num_0,terrain.getMaterialSpecularScale(num_1));
			int parent_1 = terrain.getMaterialParent(num_1);
			if(parent_1 == num_0) parent_1 = num_1;
			terrain.setMaterialParent(num_0,parent_1);
			
			
			terrain.setMaterialName(num_1,name);
			terrain.setMaterialTransform(num_1,transform);
			terrain.setMaterialOverlap(num_1,overlap);
			terrain.setMaterialMaskBase(num_1,mask_base);
			terrain.setMaterialMaskValue(num_1,mask_value);
			terrain.setMaterialMaskWidth(num_1,mask_width);
			terrain.setMaterialMaskThreshold(num_1,mask_threshold);
			terrain.setMaterialDiffuseTextureName(num_1,diffuse_name);
			terrain.setMaterialNormalTextureName(num_1,normal_name);
			terrain.setMaterialSpecularTextureName(num_1,specular_name);
			terrain.setMaterialDiffuseScale(num_1,diffuse_scale);
			terrain.setMaterialNormalScale(num_1,normal_scale);
			terrain.setMaterialSpecularScale(num_1,specular_scale);
			terrain.setMaterialParent(num_1,parent_0);
			
			forloop(int i = 0;terrain.getNumMaterials()) {
				if(i == num_0 || i == num_1) continue;
				
				if(terrain.getMaterialParent(i) == num_0) {
					terrain.setMaterialParent(i,num_1);
				}
				else if(terrain.getMaterialParent(i) == num_1) {
					terrain.setMaterialParent(i,num_0);
				}
			}
			
			Import::update_materials();
			
			update_textures(NULL);
			
			save_state(s);
			update();
			materials_lb.setCurrentItem(num_0);
			material_changed();
		}
		
		void up_clicked() {
			
			if(num == 0) return;
			
			if(terrain.getMaterialParent(num) == num - 1) return;
			
			swap(num - 1,num);
		}
		
		void down_clicked() {
			
			if(num == terrain.getNumMaterials() - 1) return;
			
			if(terrain.getMaterialParent(num + 1) == num) return;
			
			swap(num + 1,num);
		}
		
		/*
		 */
		void material_changed() {
			num = materials_lb.getCurrentItem();
			remove_b.setEnabled(num != -1);
			clone_b.setEnabled(num != -1);
			up_b.setEnabled(num > 0 && (terrain.getMaterialParent(num) != num - 1));
			down_b.setEnabled(num >= 0 && (num < terrain.getNumMaterials() - 1) && (terrain.getMaterialParent(num + 1) != num));
			update_view();
		}
		
		/*
		 */
		void name_pressed() {
			string old_name = terrain.getMaterialName(num);
			
			if(Import::check_new_name(terrain.getMaterialName(num),name_el.getText())) {
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,terrain);
				terrain.setMaterialName(num,name_el.getText());
				update_material(num);
				save_state(s);
			}
			name_el.setText(terrain.getMaterialName(num));
			
			Import::change_name(old_name,terrain.getMaterialName(num));
		}
		
		void parent_changed() {
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,terrain);
			terrain.setMaterialParent(num,parent_cb.getCurrentItem() - 1);
			update_material(num);
			save_state(s);
			up_b.setEnabled(num > 0 && (terrain.getMaterialParent(num) != num - 1));
			down_b.setEnabled(num >= 0 && (num < terrain.getNumMaterials() - 1) && (terrain.getMaterialParent(num + 1) != num));
			
		}
		
		void overlap_clicked() {
			if(terrain.getMaterialOverlap(num) != overlap_cb.isChecked()) {
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,terrain);
				terrain.setMaterialOverlap(num,overlap_cb.isChecked());
				update_material(num);
				save_state(s);
			}
		}
		
		void mask_base_clicked() {
			if(terrain.getMaterialMaskBase(num) != mask_base_cb.isChecked()) {
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,terrain);
				terrain.setMaterialMaskBase(num,mask_base_cb.isChecked());
				update_material(num);
				save_state(s);
			}
		}
		
		void transform_pressed() {
			if(terrain.getMaterialTransform(num) != getEditLineVec4(transform_el)) {
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,terrain);
				terrain.setMaterialTransform(num,getEditLineVec4(transform_el));
				save_state(s);
			}
			setEditLineVec4(transform_el,terrain.getMaterialTransform(num));
		}
		
		/*
		 */
		void update_textures(Widget widget) {
			
			// check materials
			if(terrain.getNumMaterials() == 0) return;
			
			template save_array<WIDGET,NAME,TITLE> {
				
				if(widget == NULL || widget == WIDGET ## _el) {
					
					string name = terrain.get ## NAME ## TextureArrayName();
					int size = int(WIDGET ## _array_cb.getCurrentItemText());
					
					if(createDummyTextureArray(TITLE,name)) {
						terrain.set ## NAME ## TextureArrayName(name);
						WIDGET ## _array_el.setText(name);
						texturesUpdateTerrain ## NAME ## Array(terrain,size);
					}
				}
			}
			
			save_array<diffuse_texture,Diffuse,TR("Select diffuse texture array to save")>;
			save_array<normal_texture,Normal,TR("Select normal texture array to save")>;
			save_array<specular_texture,Specular,TR("Select specular texture array to save")>;
		}
		
		/*
		 */
		TERRAIN_MATERIAL_PRESSED_CALLBACK_SLIDER(mask_value,MaterialMaskValue)
		TERRAIN_MATERIAL_PRESSED_CALLBACK_SLIDER(mask_width,MaterialMaskWidth)
		TERRAIN_MATERIAL_PRESSED_CALLBACK_SLIDER(mask_threshold,MaterialMaskThreshold)
		
		/*
		 */
		TERRAIN_MATERIAL_PRESSED_CALLBACK(diffuse_texture,MaterialDiffuseTextureName)
		TERRAIN_MATERIAL_LOAD_CLICKED_CALLBACK(diffuse_texture,Diffuse,TR("Select material diffuse texture"),"diffuse")
		TERRAIN_MATERIAL_VIEW_CLICKED_CALLBACK(diffuse_texture,MaterialDiffuseTextureName,TR("Material diffuse texture"))
		TERRAIN_MATERIAL_EDIT_CLICKED_CALLBACK(diffuse_texture,MaterialDiffuseTextureName,"diffuse")
		TERRAIN_MATERIAL_PRESSED_CALLBACK_SLIDER(diffuse_scale,MaterialDiffuseScale)
		
		TERRAIN_MATERIAL_PRESSED_CALLBACK(normal_texture,MaterialNormalTextureName)
		TERRAIN_MATERIAL_LOAD_CLICKED_CALLBACK(normal_texture,Normal,TR("Select material normal texture"),"normal")
		TERRAIN_MATERIAL_VIEW_CLICKED_CALLBACK(normal_texture,MaterialNormalTextureName,TR("Material normal texture"))
		TERRAIN_MATERIAL_EDIT_CLICKED_CALLBACK(normal_texture,MaterialNormalTextureName,"normal")
		TERRAIN_MATERIAL_PRESSED_CALLBACK_SLIDER(normal_scale,MaterialNormalScale)
		
		TERRAIN_MATERIAL_PRESSED_CALLBACK(specular_texture,MaterialSpecularTextureName)
		TERRAIN_MATERIAL_LOAD_CLICKED_CALLBACK(specular_texture,Specular,TR("Select material specular texture"),"specular")
		TERRAIN_MATERIAL_VIEW_CLICKED_CALLBACK(specular_texture,MaterialSpecularTextureName,TR("Material specular texture"))
		TERRAIN_MATERIAL_EDIT_CLICKED_CALLBACK(specular_texture,MaterialSpecularTextureName,"specular")
		TERRAIN_MATERIAL_PRESSED_CALLBACK_SLIDER(specular_scale,MaterialSpecularScale)
		
		TERRAIN_MATERIAL_RESIZE_CALLBACK(diffuse_texture,Diffuse)
		TERRAIN_MATERIAL_RESIZE_CALLBACK(normal_texture,Normal)
		TERRAIN_MATERIAL_RESIZE_CALLBACK(specular_texture,Specular)
		
		TERRAIN_ARRAY_PRESSED_CALLBACK(diffuse_texture_array,DiffuseTextureArrayName)
		TERRAIN_ARRAY_LOAD_CLICKED_CALLBACK(diffuse_texture_array,DiffuseTextureArrayName,TR("Select materials diffuse texture"))
		TERRAIN_ARRAY_VIEW_CLICKED_CALLBACK(diffuse_texture_array,DiffuseTextureArrayName,TR("Materials diffuse texture"))
		TERRAIN_ARRAY_CLEAR_CLICKED_CALLBACK(diffuse_texture_array,DiffuseTextureArrayName,TR("Clear materials diffuse texture?"))
		
		TERRAIN_ARRAY_PRESSED_CALLBACK(normal_texture_array,NormalTextureArrayName)
		TERRAIN_ARRAY_LOAD_CLICKED_CALLBACK(normal_texture_array,NormalTextureArrayName,TR("Select materials normal texture"))
		TERRAIN_ARRAY_VIEW_CLICKED_CALLBACK(normal_texture_array,NormalTextureArrayName,TR("Materials normal texture"))
		TERRAIN_ARRAY_CLEAR_CLICKED_CALLBACK(normal_texture_array,NormalTextureArrayName,TR("Clear materials normal texture?"))
		
		TERRAIN_ARRAY_PRESSED_CALLBACK(specular_texture_array,SpecularTextureArrayName)
		TERRAIN_ARRAY_LOAD_CLICKED_CALLBACK(specular_texture_array,SpecularTextureArrayName,TR("Select materials specular texture"))
		TERRAIN_ARRAY_VIEW_CLICKED_CALLBACK(specular_texture_array,SpecularTextureArrayName,TR("Materials specular texture"))
		TERRAIN_ARRAY_CLEAR_CLICKED_CALLBACK(specular_texture_array,SpecularTextureArrayName,TR("Clear materials specular texture?"))
	}
	
	/**************************************************************************\
	*
	* Grabber
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Grabber {
		
		/*
		 */
		WidgetComboBox size_cb;
		WidgetEditLine height_el;
		string color_name;
		string diffuse_name;
		string normal_name;
		string lightmap_name;
		
		int sizes[] = ( 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192 );
		
		float height;
		
		/*
		 */
		void init() {
			
			size_cb.setCurrentItem(configGet("objects_terrain_grabber_size",0));
			height = configGet("objects_terrain_grabber_height",1000.0f);
			color_name = configGet("objects_terrain_grabber_color_name","");
			diffuse_name = configGet("objects_terrain_grabber_diffuse_name","");
			normal_name = configGet("objects_terrain_grabber_normal_name","");
			lightmap_name = configGet("objects_terrain_grabber_lightmap_name","");
			
			height_el.setText(editorFormat(height));
		}
		
		void shutdown() {
			
			configSet("objects_terrain_grabber_size",size_cb.getCurrentItem());
			configSet("objects_terrain_grabber_height",height);
			configSet("objects_terrain_grabber_color_name",color_name);
			configSet("objects_terrain_grabber_diffuse_name",diffuse_name);
			configSet("objects_terrain_grabber_normal_name",normal_name);
			configSet("objects_terrain_grabber_lightmap_name",lightmap_name);
		}
		
		/*
		 */
		Image grab(string materials) {
			
			Image image = new Image();
			
			int size = sizes[size_cb.getCurrentItem()];
			image.create2D(terrain.getSurfacesX() * size,terrain.getSurfacesY() * size,IMAGE_FORMAT_RGBA8);
			
			// begin progress dialog
			dialogProgressBegin(TR("Grabbing terrain textures..."));
			
			// projection matrix
			float step = terrain.getStep() * TERRAIN_SURFACE_STEP;
			float hstep = step * 0.5f;
			
			mat4 projection = ortho(-hstep,hstep,-hstep,hstep,0.0f,GRABBER_PROJECTION_ZFAR);
			
			Image surface = new Image();
			forloop(int y = 0; terrain.getSurfacesY()) {
				forloop(int x = 0; terrain.getSurfacesX()) {
					
					int frames = 4;
					float X = step * x + hstep;
					float Y = step * y + hstep;
					mat4 modelview = translate(-X,-Y,-height) * terrain.getIWorldTransform();
					engine.render.renderImage2D(projection,modelview,surface,size,size,materials,0);
					
					// additional render frames
					while(frames-- > 0 || engine.filesystem.getNumQueuedResources() || engine.world.getNumQueuedResources()) {
						wait;
						engine.render.renderImage2D(projection,modelview,surface,size,size,materials,0);
					}
					
					surface.flipY();
					image.copy(surface,size * x,size * y,0,0,size,size);
					
					// update progress dialog
					int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
					if(dialogProgressUpdate(progress) == 0) {
						dialogProgressEnd();
						delete surface;
						delete image;
						return NULL;
					}
				}
			}
			delete surface;
			
			image.flipY();
			
			// end progress dialog
			dialogProgressEnd();
			
			return image;
		}
		
		/*
		 */
		void height_pressed() {
			height = max(float(height_el.getText()),0.0f);
			height_el.setText(editorFormat(height));
		}
		
		/*
		 */
		void grab_color_clicked() {
			if(dialogFileImageSave(TR("Grab terrain color"),color_name)) {
				color_name = setExtensionImage(color_name,"dds");
				string save_path = savePath(color_name);
				
				// grab color
				Image image = grab(NULL);
				if(image != NULL) {
					image.convertToFormat(IMAGE_FORMAT_RGB8);
					image.save(save_path);
					engine.filesystem.forceImage(save_path);
					delete image;
				} else {
					log.error("Can't create textures.\n");
				}
			}
		}
		
		void grab_diffuse_clicked() {
			if(dialogFileImageSave(TR("Grab terrain diffuse"),diffuse_name)) {
				diffuse_name = setExtensionImage(diffuse_name,"dds");
				string save_path = savePath(diffuse_name);
				
				// grab diffuse
				if(ambientIsEnabled() == 0) ambientToggle();
				Image image = grab(NULL);
				if(ambientIsEnabled()) ambientToggle();
				if(image != NULL) {
					image.convertToFormat(IMAGE_FORMAT_RGB8);
					if(engine.console.getInt("render_use_srgb")) {
						applyGamma(image);
					}
					image.save(save_path);
					engine.filesystem.forceImage(save_path);
					delete image;
				}
			}
		}
		
		void grab_normal_clicked() {
			if(dialogFileImageSave(TR("Grab terrain normal"),normal_name)) {
				normal_name = setExtensionImage(normal_name,"dds");
				string save_path = savePath(normal_name);
				
				// grab normal
				Image image = grab("post_deferred_normal");
				if(image != NULL) {
					image.convertToFormat(IMAGE_FORMAT_RGB8);
					image.save(save_path);
					engine.filesystem.forceImage(save_path);
					delete image;
				}
			}
		}
		
		void grab_lightmap_clicked() {
			if(dialogFileImageSave(TR("Grab terrain lightmap"),lightmap_name)) {
				lightmap_name = setExtensionImage(lightmap_name,"dds");
				string save_path = savePath(lightmap_name);
				
				// clear specular scale
				Material material = terrain.getMaterialInherit(0);
				int id = material.findParameter("specular_scale");
				float specular_scale = 1.0f;
				if(id != -1) {
					specular_scale = material.getParameterSlider(id);
					material.setParameterSlider(id,0.0f);
				}
				
				// grab lightmap
				Image image = grab(NULL);
				if(image != NULL) {
					image.convertToFormat(IMAGE_FORMAT_RGB8);
					image.save(save_path);
					engine.filesystem.forceImage(save_path);
					delete image;
				}
				
				// restore specular scale
				if(id != -1) material.setParameterSlider(id,specular_scale);
			}
		}
	}
	
	/**************************************************************************\
	*
	* Terrain
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		Grabber::init();
		
		Nodes::Object::init();
		
		Parameters::init();
		
		parameters_tb.addTab(TR("Layers"));
		parameters_tb.addChild(materials_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(materials_vb);
		
		Import::init();
		
		parameters_tb.addTab(TR("Export"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
		
		parameters_tb.addTab(TR("Refined"));
		parameters_tb.addChild(surfaces_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(surfaces_vb);
		
		parameters_tb.addTab(TR("Grabber"));
		parameters_tb.addChild(grabber_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(grabber_vb);
		
		heights_name = configGet("objects_terrain_heights_name","");
		holes_name = configGet("objects_terrain_holes_name","");
		diffuse_name = configGet("objects_terrain_diffuse_name","");
		normal_name = configGet("objects_terrain_normal_name","");
		index_name = configGet("objects_terrain_index_name","");
		mask_name = configGet("objects_terrain_mask_name","");
	}
	
	void shutdown() {
		
		save();
		
		Grabber::shutdown();
		
		Nodes::Object::shutdown();
		
		Parameters::shutdown();
		Import::shutdown();
		
		configSet("objects_terrain_heights_name",heights_name);
		configSet("objects_terrain_holes_name",holes_name);
		configSet("objects_terrain_diffuse_name",diffuse_name);
		configSet("objects_terrain_normal_name",normal_name);
		configSet("objects_terrain_index_name",index_name);
		configSet("objects_terrain_mask_name",mask_name);
		
		terrain = NULL;
	}
	
	/*
	 */
	void update_view() {
		
	}
	
	/*
	 */
	void update() {
		
		Nodes::Object::update();
		
		terrain = node_cast(node);
		
		load();
		
		setEnabled(terrain.isLoaded(),(
			main_vb,
			surfaces_vb,
			materials_vb,
			grabber_vb,
		));
		
		update_view();
		
		// surfaces
		Surfaces::update();
		
		// materials
		Materials::update();
		
		Parameters::update();
		Import::update();
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
		
		// surfaces
		Surfaces::update_dynamic();
		
		// materials
		Materials::update_dynamic();
		
		Import::update_dynamic();
	}
	
	/**************************************************************************\
	*
	* Heights
	*
	\**************************************************************************/
	
	/*
	 */
	void export_heights_clicked() {
		
		if(dialogFileImageSave(TR("Export terrain heights"),heights_name) == 0) return;
		
		Image heights = new Image();
		float scale = float(height_scale_el.getText());
		if(terrain.getHeights(0,0,terrain.getSizeX(),terrain.getSizeY(),heights,IMAGE_FORMAT_R16,scale) == 0) {
			dialogMessageOk(TR("Error"));
			delete heights;
			return;
		}
		
		heights.flipY();
		
		string save_path = savePath(heights_name);
		
		if(heights.save(save_path) == 0) {
			dialogMessageOk(TR("Error"));
			delete heights;
			return;
		}
		log.message("Save terrain heights into the \"%s\" file\n",save_path);
		
		
		delete heights;
		
	}
	
	/**************************************************************************\
	*
	* Holes
	*
	\**************************************************************************/
	
	/*
	 */
	void export_holes_clicked() {
		
		if(dialogFileImageSave(TR("Export terrain holes"),holes_name) == 0) return;
		
		Image holes = new Image();
		if(terrain.getHoles(0,0,terrain.getSizeX(),terrain.getSizeY(),holes) == 0) {
			dialogMessageOk(TR("Error"));
			delete holes;
			return;
		}
		
		holes.flipY();
		
		string save_path = savePath(holes_name);
		
		if(holes.save(save_path) == 0) {
			dialogMessageOk(TR("Error"));
			delete holes;
			return;
		}
		log.message("Save terrain holes into the \"%s\" file\n",save_path);
		delete holes;
	}
	
	/**************************************************************************\
	*
	* Diffuse
	*
	\**************************************************************************/
	
	/*
	 */
	void export_diffuse_clicked() {
		
		// surface size
		int surface_width = 0;
		int surface_height = 0;
		
		// info surface diffuse textures
		Image surface = new Image();
		forloop(int y = 0; terrain.getSurfacesY()) {
			forloop(int x = 0; terrain.getSurfacesX()) {
				
				// surface diffuse texture name
				string surface_name = terrain.getSurfaceDiffuseTextureName(x,y);
				if(strlen(surface_name) == 0) continue;
				
				// load terrain surface diffuse textures
				if(surface.info(surface_name) == 0) {
					if(surface.load(surface_name) == 0) {
						continue;
					}
				}
				
				// get surface diffuse texture size
				surface_width = max(surface_width,surface.getWidth());
				surface_height = max(surface_height,surface.getHeight());
			}
		}
		delete surface;
		
		// single coarse diffuse texture
		if(surface_width == 0 || surface_height == 0) {
			
			// coarse diffuse texture name
			string coarse_name = terrain.getDiffuseTextureName();
			if(strlen(coarse_name) == 0) {
				dialogMessageOk(TR("Error"),TR("No diffuse texture defined"));
				return;
			}
			
			// diffuse texture name
			if(dialogFileImageSave(TR("Export terrain diffuse"),diffuse_name) == 0) return;
			
			string save_path = savePath(diffuse_name);
			
			if(dialogFileConfirm(save_path) == 0) return;
			
			// load terrain coarse diffuse texture
			Image diffuse = new Image();
			if(diffuse.load(coarse_name) == 0) {
				dialogMessageOk(TR("Error"));
				delete diffuse;
				return;
			}
			
			diffuse.flipY();
			
			// save terrain diffuse texture
			if(diffuse.save(save_path) == 0) {
				dialogMessageOk(TR("Error"));
				delete diffuse;
				return;
			}
			delete diffuse;
			
			log.message("Save diffuse texture into the \"%s\" file\n",save_path);
		}
		// multiple surface diffuse textures
		else {
			
			// diffuse texture name
			if(dialogFileImageSave(TR("Export terrain diffuse"),diffuse_name) == 0) return;
			
			string save_path = savePath(diffuse_name);
			
			if(dialogFileConfirm(save_path) == 0) return;
			
			Image diffuse = new Image();
			int width = terrain.getSurfacesX() * surface_width;
			int height = terrain.getSurfacesY() * surface_height;
			
			// coarse diffuse texture name
			string coarse_name = terrain.getDiffuseTextureName();
			if(strlen(coarse_name)) {
				
				// load coarse diffuse texture
				if(diffuse.load(coarse_name) == 0) {
					dialogMessageOk(TR("Error"));
					delete diffuse;
					return;
				}
				
				log.message("Load coarse diffuse texture from the \"%s\" file\n",coarse_name);
				
				// decompress coarse diffuse texture
				if(diffuse.isZCompressedFormat()) diffuse.decompress();
				if(diffuse.isCompressedFormat()) diffuse.decompress();
				
				// resize coarse diffuse texture
				diffuse.resize(width,height);
			}
			
			// begin progress dialog
			dialogProgressBegin(TR("Creating a diffuse texture..."));
			
			// load surface diffuse textures
			Image surface = new Image();
			forloop(int y = 0; terrain.getSurfacesY()) {
				forloop(int x = 0; terrain.getSurfacesX()) {
					
					// surface diffuse texture name
					string surface_name = terrain.getSurfaceDiffuseTextureName(x,y);
					if(strlen(surface_name) == 0) continue;
					
					// load surface diffuse texture
					if(surface.load(surface_name) == 0) continue;
					
					log.message("Load surface diffuse texture from the \"%s\" file\n",surface_name);
					
					// decompress surface diffuse texture
					if(surface.isZCompressedFormat()) surface.decompress();
					if(surface.isCompressedFormat()) surface.decompress();
					
					// resize surface diffuse texture
					surface.resize(surface_width,surface_height);
					
					// copy surface diffuse texture
					if(diffuse.isLoaded() == 0) diffuse.create2D(width,height,surface.getFormat());
					diffuse.copy(surface,surface_width * x,surface_height * y,0,0,surface_width,surface_height);
					
					// update progress dialog
					int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
					if(dialogProgressUpdate(progress) == 0) {
						dialogProgressEnd();
						delete surface;
						delete diffuse;
						return;
					}
				}
			}
			delete surface;
			
			// end progress dialog
			dialogProgressEnd();
			
			diffuse.flipY();
			// save diffuse texture
			if(diffuse.save(save_path) == 0) {
				dialogMessageOk(TR("Error"));
				delete diffuse;
				return;
			}
			delete diffuse;
			
			log.message("Save diffuse texture into the \"%s\" file\n",save_path);
		}
	}
	
	/**************************************************************************\
	*
	* Normal
	*
	\**************************************************************************/
	
	/*
	 */
	void export_normal_clicked() {
		
		// surface size
		int surface_width = 0;
		int surface_height = 0;
		
		// info surface normal textures
		Image surface = new Image();
		forloop(int y = 0; terrain.getSurfacesY()) {
			forloop(int x = 0; terrain.getSurfacesX()) {
				
				// surface normal texture name
				string surface_name = terrain.getSurfaceNormalTextureName(x,y);
				if(strlen(surface_name) == 0) continue;
				
				// load terrain surface normal textures
				if(surface.info(surface_name) == 0) {
					if(surface.load(surface_name) == 0) {
						continue;
					}
				}
				
				// get surface normal texture size
				surface_width = max(surface_width,surface.getWidth());
				surface_height = max(surface_height,surface.getHeight());
			}
		}
		delete surface;
		
		// single coarse normal texture
		if(surface_width == 0 || surface_height == 0) {
			
			// coarse normal texture name
			string coarse_name = terrain.getNormalTextureName();
			if(strlen(coarse_name) == 0) {
				dialogMessageOk(TR("Error"),TR("No normal texture defined"));
				return;
			}
			
			// normal texture name
			if(dialogFileImageSave(TR("Export terrain normal"),normal_name) == 0) return;
			normal_name = setExtensionImage(normal_name,"dds");
			string save_path = savePath(normal_name);
			
			// load terrain coarse normal texture
			Image normal = new Image();
			if(normal.load(coarse_name) == 0) {
				dialogMessageOk(TR("Error"));
				delete normal;
				return;
			}
			
			normal.flipY();
			
			// save terrain normal texture
			if(normal.save(save_path) == 0) {
				dialogMessageOk(TR("Error"));
				delete normal;
				return;
			}
			delete normal;
			
			log.message("Save normal texture into the \"%s\" file\n",save_path);
		}
		// multiple surface normal textures
		else {
			
			// normal texture name
			if(dialogFileImageSave(TR("Export terrain normal"),normal_name) == 0) return;
			normal_name = setExtensionImage(normal_name,"dds");
			string save_path = savePath(normal_name);
			
			Image normal = new Image();
			int width = terrain.getSurfacesX() * surface_width;
			int height = terrain.getSurfacesY() * surface_height;
			
			// coarse normal texture name
			string coarse_name = terrain.getNormalTextureName();
			if(strlen(coarse_name)) {
				
				// load coarse normal texture
				if(normal.load(coarse_name) == 0) {
					dialogMessageOk(TR("Error"));
					delete normal;
					return;
				}
				
				log.message("Load coarse normal texture from the \"%s\" file\n",coarse_name);
				
				// decompress coarse normal texture
				if(normal.isZCompressedFormat()) normal.decompress();
				if(normal.isCompressedFormat()) normal.decompress();
				
				// resize coarse normal texture
				normal.resize(width,height);
			}
			
			// begin progress dialog
			dialogProgressBegin(TR("Creating a normal texture..."));
			
			// load surface normal textures
			Image surface = new Image();
			forloop(int y = 0; terrain.getSurfacesY()) {
				forloop(int x = 0; terrain.getSurfacesX()) {
					
					// surface normal texture name
					string surface_name = terrain.getSurfaceNormalTextureName(x,y);
					if(strlen(surface_name) == 0) continue;
					
					// load surface normal texture
					if(surface.load(surface_name) == 0) continue;
					
					log.message("Load surface normal texture from the \"%s\" file\n",surface_name);
					
					// decompress surface normal texture
					if(surface.isZCompressedFormat()) surface.decompress();
					if(surface.isCompressedFormat()) surface.decompress();
					
					// resize surface normal texture
					surface.resize(surface_width,surface_height);
					
					// copy surface normal texture
					if(normal.isLoaded() == 0) normal.create2D(width,height,surface.getFormat());
					normal.copy(surface,surface_width * x,surface_height * y,0,0,surface_width,surface_height);
					
					// update progress dialog
					int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
					if(dialogProgressUpdate(progress) == 0) {
						dialogProgressEnd();
						delete surface;
						delete normal;
						return;
					}
				}
			}
			delete surface;
			
			// end progress dialog
			dialogProgressEnd();
			
			normal.flipY();
			
			// save normal texture
			if(normal.save(save_path) == 0) {
				dialogMessageOk(TR("Error"));
				delete normal;
				return;
			}
			delete normal;
			
			log.message("Save normal texture into the \"%s\" file\n",save_path);
		}
	}
	
	/**************************************************************************\
	*
	* Mask
	*
	\**************************************************************************/
	
	/*
	 */
	void export_mask_clicked() {
		
		// check terrain materials
		if(terrain.getNumMaterials() == 0) {
			dialogMessageOk(TR("Error"),TR("No materials defined"));
			return;
		}
		
		// surface size
		int surface_width = 0;
		int surface_height = 0;
		
		// info surface mask textures
		Image surface = new Image();
		forloop(int y = 0; terrain.getSurfacesY()) {
			forloop(int x = 0; terrain.getSurfacesX()) {
				
				// surface mask texture name
				string surface_name = terrain.getSurfaceMaskTextureName(x,y);
				if(strlen(surface_name) == 0) continue;
				
				// load surface mask texture
				if(surface.info(surface_name) == 0) {
					if(surface.load(surface_name) == 0) {
						continue;
					}
				}
				
				// get surface mask texture size
				surface_width = max(surface_width,surface.getWidth());
				surface_height = max(surface_height,surface.getHeight());
			}
		}
		delete surface;
		
		// no surface mask textures
		if(surface_width == 0 || surface_height == 0) {
			dialogMessageOk(TR("Error"),TR("No surface mask textures defined"));
			return;
		}
		
		// mask texture name
		if(dialogFileImageSave(TR("Export terrain mask"),mask_name) == 0) return;
		mask_name = setExtensionImage(mask_name,"dds");
		string save_path = savePath(mask_name);
		
		// create mask texture
		Image mask = new Image();
		int width = terrain.getSurfacesX() * surface_width;
		int height = terrain.getSurfacesY() * surface_height;
		mask.create3D(width,height,terrain.getNumMaterials(),IMAGE_FORMAT_R8);
		
		// begin progress dialog
		dialogProgressBegin(TR("Creating a mask texture..."));
		
		// load surface mask textures
		surface = new Image();
		Image index = new Image();
		Image layer = new Image();
		layer.create2D(surface_width,surface_height,IMAGE_FORMAT_R8);
		forloop(int y = 0; terrain.getSurfacesY()) {
			forloop(int x = 0; terrain.getSurfacesX()) {
				
				// surface index texture name
				string index_name = terrain.getSurfaceIndexTextureName(x,y);
				if(strlen(index_name) == 0) index_name = terrain.getIndexTextureName();
				if(strlen(index_name) == 0) continue;
				
				// surface mask texture name
				string surface_name = terrain.getSurfaceMaskTextureName(x,y);
				if(strlen(surface_name) == 0) continue;
				
				// load surface mask texture
				if(index.load(index_name) == 0 || surface.load(surface_name) == 0) continue;
				
				log.message("Load surface mask texture from the \"%s\" file\n",surface_name);
				
				// decompress index index texture
				if(index.isZCompressedFormat()) index.decompress();
				if(index.isCompressedFormat()) index.decompress();
				
				// decompress surface mask texture
				if(surface.isZCompressedFormat()) surface.decompress();
				if(surface.isCompressedFormat()) surface.decompress();
				
				// resize surface mask texture
				surface.resize(surface_width,surface_height);
				
				// create mask texture
				if(engine.editor.createTerrainMask(surface,index,terrain.getNumMaterials()) == 0) continue;
				
				// copy mask texture
				forloop(int i = 0; terrain.getNumMaterials()) {
					if(layer.copy(surface,i) == 0 || mask.copy(layer,surface_width * x,mask.getHeight() * i + surface_height * y,0,0,surface_width,surface_height) == 0) {
						dialogMessageOk(TR("Error"));
						dialogProgressEnd();
						delete surface;
						delete layer;
						delete index;
						delete mask;
						return;
					}
				}
				
				// update progress dialog
				int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
				if(dialogProgressUpdate(progress) == 0) {
					dialogProgressEnd();
					delete surface;
					delete layer;
					delete index;
					delete mask;
					return;
				}
			}
		}
		delete surface;
		delete layer;
		delete index;
		
		// end progress dialog
		dialogProgressEnd();
		
		// save mask texture
		if(mask.save(save_path) == 0) {
			dialogMessageOk(TR("Error"));
			delete mask;
			return;
		}
		delete mask;
		
		log.message("Save mask texture into the \"%s\" file\n",save_path);
	}
	
	/**************************************************************************\
	*
	* Compression
	*
	\**************************************************************************/
	
	/*
	 */
	void compress_image(Async async,string name,float gamma,int zcompress) {
		// check name
		if(strlen(name) == 0) return;
		
		// info image
		Image image = new Image();
		int id = async.run(image,"info",name);
		while(async.isRunning(id)) wait;
		if(async.getResult(id) == 0) {
			delete image;
			return;
		}
		// check image format
		if(image.isCompressedFormat() && image.isZCompressedFormat() == zcompress) {
			delete image;
			return;
		}
		// load image
		id = async.run(image,"load",name);
		
		while(async.isRunning(id)) wait;
		if(async.getResult(id) == 0) {
			delete image;
			return;
		}
		
		log.message("Compress \"%s\" file\n",name);
		
		// compress image
		if(image.isCompressedFormat() == 0) {
			async.run(image,"createMipmaps",IMAGE_FILTER_LINEAR,gamma);
			id = async.run(image,"compress",-1);
			while(async.isRunning(id)) wait;
		}
		
		// zcompress image
		if(zcompress) image.convertToFormat(IMAGE_FORMAT_ZLC2);
		
		// save image
		image.save(name);
		
		delete image;
	}
	
	void compress_clicked() {
		
		// asynchronous processing
		dialogProgressBegin(TR("Compressing textures..."));
		
		panelStatusBarInfoStart("Compressing textures...");
		
		asyncRunNamedThread(functionid(compress_image),(terrain.getDiffuseTextureName(),2.2f,0),"compress");
		asyncRunNamedThread(functionid(compress_image),(terrain.getNormalTextureName(),1.0f,0),"compress");
		asyncRunNamedThread(functionid(compress_image),(terrain.getMaskTextureName(),1.0f,0),"compress");
		
		forloop(int y = 0; terrain.getSurfacesY()) {
			forloop(int x = 0; terrain.getSurfacesX()) {
				
				asyncRunNamedThread(functionid(compress_image),(terrain.getSurfaceDiffuseTextureName(x,y),2.2f,1),"compress");
				asyncRunNamedThread(functionid(compress_image),(terrain.getSurfaceNormalTextureName(x,y),1.0f,1),"compress");
				asyncRunNamedThread(functionid(compress_image),(terrain.getSurfaceMaskTextureName(x,y),1.0f,0),"compress");
				
				asyncWaitThreads("compress");
				
				// update progress dialog
				int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
				if(dialogProgressUpdate(progress) == 0) {
					dialogProgressEnd();
					panelStatusBarInfoEnd("Compressing textures...");
					return;
				}
			}
		}
		
		dialogProgressEnd();
		panelStatusBarInfoEnd("Compressing textures...");
	}
	
	/*
	 */
	void decompress_image(Async async,string name) {
		
		// check name
		if(strlen(name) == 0) return;
		
		// info image
		Image image = new Image();
		int id = async.run(image,"info",name);
		while(async.isRunning(id)) wait;
		if(async.getResult(id) == 0) {
			delete image;
			return;
		}
		
		// check image format
		if(image.isCompressedFormat() == 0) {
			delete image;
			return;
		}
		
		// load image
		id = async.run(image,"load",name);
		while(async.isRunning(id)) wait;
		if(async.getResult(id) == 0) {
			delete image;
			return;
		}
		
		log.message("Decompress \"%s\" file\n",name);
		
		// decompress image
		if(image.isZCompressedFormat()) image.decompress();
		if(image.isCompressedFormat()) image.decompress();
		
		// save image
		image.save(name);
		
		delete image;
	}
	
	void decompress_clicked() {
		
		// asynchronous processing
		dialogProgressBegin(TR("Decompressing textures..."));
		panelStatusBarInfoStart("Decompressing textures...");
		
		asyncRunNamedThread(functionid(decompress_image),(terrain.getDiffuseTextureName()),"decompress");
		asyncRunNamedThread(functionid(decompress_image),(terrain.getNormalTextureName()),"decompress");
		asyncRunNamedThread(functionid(decompress_image),(terrain.getMaskTextureName()),"decompress");
		forloop(int y = 0; terrain.getSurfacesY()) {
			forloop(int x = 0; terrain.getSurfacesX()) {
				
				asyncRunNamedThread(functionid(decompress_image),(terrain.getSurfaceDiffuseTextureName(x,y)),"decompress");
				asyncRunNamedThread(functionid(decompress_image),(terrain.getSurfaceNormalTextureName(x,y)),"decompress");
				asyncRunNamedThread(functionid(decompress_image),(terrain.getSurfaceMaskTextureName(x,y)),"decompress");
				
				asyncWaitThreads("decompress");
				
				// update progress dialog
				int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
				if(dialogProgressUpdate(progress) == 0) {
					panelStatusBarInfoEnd("Decompressing textures...");
					dialogProgressEnd();
					return;
				}
			}
		}
		
		dialogProgressEnd();
		panelStatusBarInfoEnd("Decompressing textures...");
	}
	
	void save() {
		if(terrain == NULL || engine.editor.isNode(terrain) == 0) return;
		Xml xml = new Xml("objects_terrain");
		Import::save(xml.addChild("import"));
		Parameters::save(xml.addChild("parameters"));
		xml.setChildData("height_scale",height_scale_el.getText());
		
		terrain.setData(xml.getSubTree());
		delete xml;
	}
	
	void load() {
		string data = terrain.getData();
		if(data == NULL || is_string(data) == 0 || strlen(data) == 0 || data[0] != '<') return;
		
		Xml xml = new Xml();
		if(xml.parse(data) == 0 || xml.getName() != "objects_terrain") {
			delete xml;
			return;
		}
		
		if(xml.isChild("import")) Import::load(xml.getChild("import"));
		if(xml.isChild("parameters")) Parameters::load(xml.getChild("parameters"));
		height_scale_el.setText(xml.getChildData("height_scale","1"));
		
		delete xml;
	}
} /* namespace Terrain */
