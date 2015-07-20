/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_objects.h
 * Desc:    Unigine editor
 * Version: 1.26
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
 *          Pavel Kurinnoy <figonet@unigine.com>
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
#define SURFACE_CLICKED_CALLBACK(WIDGET,FUNC) \
void WIDGET ## _clicked() { \
	if(object == NULL) return; \
	NodesState_ s = new NodesState_(); \
	s.init_modify(nodes,object); \
	int WIDGET = WIDGET ## _cb.isChecked(); \
	forloop(int i = 0; surfaces_tb.getNumSelectedItems()) { \
		int surface_item = surfaces_tb.getSelectedItem(i); \
		int surface_index = treebox_surfaces[surface_item]; \
		object.set ## FUNC(WIDGET,surface_index); \
	} \
	save_state(s); \
}

#define SURFACE_PRESSED_CALLBACK_MASK(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	if(object == NULL) return; \
	NodesState_ s = new NodesState_(); \
	int mask = int("0x" + WIDGET ## _el.getText()); \
	if(gui.getFocus() != surfaces_tb) { \
		forloop(int i = 0; surfaces_tb.getNumSelectedItems()) { \
			int surface_item = surfaces_tb.getSelectedItem(i); \
			int surface_index = treebox_surfaces[surface_item]; \
			if(object.get ## FUNC(surface_index) != mask) { \
				if(s.state_type == Unredo::STATE_NONE) s.init_modify(nodes,object); \
				object.set ## FUNC(mask,surface_index); \
			} \
		} \
	} \
	if(surfaces_tb.getNumSelectedItems() > 0) { \
		int surface = treebox_surfaces[surfaces_tb.getSelectedItem(0)]; \
		WIDGET ## _el.setText(editorFormatMask(object.get ## FUNC(surface))); \
	} \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

#define SURFACE_CHANGED_CALLBACK(WIDGET,FUNC) \
void WIDGET ## _changed(int mask) { \
	if(object == NULL) return; \
	if(gui.getFocus() != surfaces_tb) { \
		forloop(int i = 0; surfaces_tb.getNumSelectedItems()) { \
			int surface_item = surfaces_tb.getSelectedItem(i); \
			int surface_index = treebox_surfaces[surface_item]; \
			object.set ## FUNC(mask,surface_index); \
		} \
	} \
	int surface = treebox_surfaces[surfaces_tb.getSelectedItem(0)]; \
	WIDGET ## _el.setText(editorFormatMask(object.get ## FUNC(surface))); \
}

#define SURFACE_CLICKED_CALLBACK_MASK(WIDGET,FUNC,TITLE) \
void WIDGET ## _clicked() { \
	if(object == NULL) return; \
	NodesState_ s = new NodesState_(); \
	s.init_modify(nodes,object); \
	int surface = treebox_surfaces[surfaces_tb.getSelectedItem(0)]; \
	int mask = object.get ## FUNC(surface); \
	dialogMask(TITLE,functionid(WIDGET ## _changed),object.get ## FUNC(surface)); \
	if(mask != object.get ## FUNC(surface)) save_state(s); \
	else delete s; \
}

#define SURFACE_PRESSED_CALLBACK(WIDGET,FUNC,TYPE,FORMATTER) \
void WIDGET ## _pressed() { \
	if(object == NULL) return; \
	NodesState_ s = new NodesState_(); \
	if(gui.getFocus() != surfaces_tb) { \
		forloop(int i = 0; surfaces_tb.getNumSelectedItems()) { \
			int surface_item = surfaces_tb.getSelectedItem(i); \
			int surface_index = treebox_surfaces[surface_item]; \
			TYPE WIDGET = TYPE(WIDGET ## _el.getText()); \
			if(object.get ## FUNC(surface_index) != WIDGET) { \
				if(s.state_type == Unredo::STATE_NONE) s.init_modify(nodes,object); \
				object.set ## FUNC(WIDGET,surface_index); \
			} \
		} \
	} \
	if(surfaces_tb.getNumSelectedItems() > 0) { \
		int surface = treebox_surfaces[surfaces_tb.getSelectedItem(0)]; \
		WIDGET ## _el.setText(FORMATTER(object.get ## FUNC(surface))); \
	} \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

/*
 */
#define SURFACE_PRESSED_CALLBACK_ATTRIBUTE(WIDGET,TYPE) \
void WIDGET ## _pressed() { \
	NodesState_ s = new NodesState_(); \
	if(gui.getFocus() != surfaces_tb) { \
		if(WIDGET ## _el.getText() != "") { \
			forloop(int i = 0; surfaces_tb.getNumSelectedItems()) { \
				int surface_item = surfaces_tb.getSelectedItem(i); \
				int surface_index = treebox_surfaces[surface_item]; \
				TYPE WIDGET = object.get ## TYPE(surface_index); \
				if(WIDGET == NULL || WIDGET.getName() != WIDGET ## _el.getText()) { \
					if(s.state_type == Unredo::STATE_NONE) s.init_modify(nodes,object); \
					object.set ## TYPE(WIDGET ## _el.getText(),surface_index); \
				} \
				WIDGET = object.get ## TYPE(surface_index); \
				if(WIDGET == NULL) { \
					dialogMessageOk(TR("Error")); \
					break; \
				} \
			} \
		} else { \
			forloop(int i = 0; surfaces_tb.getNumSelectedItems()) { \
				int surface_item = surfaces_tb.getSelectedItem(i); \
				int surface_index = treebox_surfaces[surface_item]; \
				TYPE WIDGET = object.get ## TYPE(surface_index); \
				if(WIDGET != NULL) { \
					if(s.state_type == Unredo::STATE_NONE) s.init_modify(nodes,object); \
					object.set ## TYPE(NULL,surface_index); \
				} \
			} \
		} \
		update_view(); \
	} \
	save_state(s); \
}

#define SURFACE_CLICKED_CALLBACK_ATTRIBUTE_LOAD(WIDGET,TYPE,SELECTOR) \
void WIDGET ## _load_clicked() { \
	NodesState_ s = new NodesState_(); \
	int surface = treebox_surfaces[surfaces_tb.getSelectedItem(0)]; \
	TYPE WIDGET = object.get ## TYPE(surface); \
	string name = (WIDGET != NULL) ? WIDGET.getName() : ""; \
	if(dialog ## TYPE("Select " #WIDGET,name)) { \
		forloop(int i = 0; surfaces_tb.getNumSelectedItems()) { \
			int surface_item = surfaces_tb.getSelectedItem(i); \
			int surface_index = treebox_surfaces[surface_item]; \
			TYPE WIDGET = object.get ## TYPE(surface_index); \
			if(WIDGET == NULL || WIDGET.getName() != name) { \
				if(s.state_type == Unredo::STATE_NONE) s.init_modify(nodes,object); \
				object.set ## TYPE(name,surface_index); \
			} \
			WIDGET = object.get ## TYPE(surface_index); \
			if(WIDGET == NULL) { \
				dialogMessageOk(TR("Error")); \
				break; \
			} \
		} \
		SELECTOR(name); \
		update_view(); \
	} \
	save_state(s); \
}

#define SURFACE_CLICKED_CALLBACK_ATTRIBUTE_EDIT(NAME,TYPE,EDITOR) \
void NAME ## _edit_clicked() { \
	int surface = treebox_surfaces[surfaces_tb.getSelectedItem(0)]; \
	int is_inherit = object.is ## TYPE ## Inherited(surface); \
	TYPE NAME = object.get ## TYPE ## Inherit(surface); \
	if(EDITOR(NAME) == 0 && is_inherit == 0) object.set ## TYPE(object.get ## TYPE ## Name(surface),surface); \
	update_view(); \
}

#define SURFACE_CLICKED_CALLBACK_ATTRIBUTE_CLEAR(WIDGET,FUNC) \
void WIDGET ## _clear_clicked() { \
	NodesState_ s = new NodesState_(); \
	if(dialogMessageYesNo(TR("Confirm"),TR("Clear surface ") + #WIDGET + TR(" parameters?"))) { \
		forloop(int i = 0; surfaces_tb.getNumSelectedItems()) { \
			int surface_item = surfaces_tb.getSelectedItem(i); \
			int surface_index = treebox_surfaces[surface_item]; \
			if(object.get ## FUNC(surface_index) != NULL) { \
				if(s.state_type == Unredo::STATE_NONE) s.init_modify(nodes,object); \
				object.set ## FUNC(object.get ## FUNC ## Name(surface_index),surface_index); \
			} \
		} \
		update_view(); \
	} \
	save_state(s); \
}

/******************************************************************************\
*
* Object
*
\******************************************************************************/

/*
 */
namespace Object {
	
	/*
	 */
	WidgetVBox main_vb;							// main vbox
	WidgetEditLine name_el;						// surface name
	WidgetTreeBox surfaces_tb;					// surfaces treebox
	int surfaces_folded[];						// surfaces hidden flag
	
	WidgetCheckBox enabled_cb;					// enabled flag
	WidgetCheckBox decal_cb;					// decal flag
	WidgetCheckBox cast_shadow_cb;				// cast shadow flag
	WidgetCheckBox receive_shadow_cb;			// receive shadow flag
	WidgetCheckBox cast_world_shadow_cb;		// cast world shadow flag
	WidgetCheckBox receive_world_shadow_cb;		// receive world shadow flag
	WidgetCheckBox intersection_cb;				// intersection flag
	WidgetCheckBox collision_cb;				// collision flag
	WidgetEditLine light_mask_el;				// light mask
	WidgetEditLine decal_mask_el;				// decal mask
	WidgetEditLine viewport_mask_el;			// viewport mask
	WidgetEditLine intersection_mask_el;		// intersection mask
	WidgetEditLine collision_mask_el;			// collision mask
	WidgetEditLine min_parent_el;				// lod min parent
	WidgetEditLine max_parent_el;				// lod max parent
	WidgetEditLine min_visible_distance_el;		// lod min visible distance
	WidgetEditLine max_visible_distance_el;		// lod max visible distance
	WidgetEditLine min_fade_distance_el;		// lod min fade distance
	WidgetEditLine max_fade_distance_el;		// lod max fade distance
	WidgetEditLine material_el;					// material name
	WidgetIcon material_load_i;					// material load
	WidgetIcon material_edit_i;					// material edit
	WidgetIcon material_clear_i;				// material clear
	WidgetEditLine property_el;					// property name
	WidgetIcon property_load_i;					// property load
	WidgetIcon property_edit_i;					// property edit
	WidgetIcon property_clear_i;				// property clear
	
	WidgetButton paste_b;						// paste button
	
	Object object;								// base object node
	
	int current_surfaces[0];					// current surfaces
	
	int wireframe_mode = 0;
	vec4 wireframe_color = vec4(69.0f / 255.0f,251.0f / 255.0f,166.0f / 255.0f,1.0f);
	
	/*
	 */
	#include <editor/editor_shapes.h>
	#include <editor/editor_joints.h>
	#include <editor/editor_bodies.h>
	
	int surface_indices[];		// map<name,surface index>
	int surface_items[];		// map<name,treebox index>
	int treebox_surfaces[];		// map<treebox index,surface index>
	int treebox_items[];		// map<surface index,treebox index>
	
	/*
	 */
	class Buffer {
		
		int enabled;
		int decal;
		int cast_shadow;
		int receive_shadow;
		int cast_world_shadow;
		int receive_world_shadow;
		int intersection;
		int collision;
		
		int light_mask;
		int decal_mask;
		int intersection_mask;
		int collision_mask;
		int viewport_mask;
		
		int min_parent;
		int max_parent;
		float min_visible_distance;
		float max_visible_distance;
		float min_fade_distance;
		float max_fade_distance;
		
		
		string material;
		::Blob material_blob = NULL;
		
		string property;
		::Blob property_blob = NULL;
		
		Buffer(int surface) {
			
			enabled = object.isEnabled(surface);
			decal = object.getDecal(surface);
			cast_shadow = object.getCastShadow(surface);
			receive_shadow = object.getReceiveShadow(surface);
			cast_world_shadow = object.getCastWorldShadow(surface);
			receive_world_shadow = object.getReceiveWorldShadow(surface);
			intersection = object.getIntersection(surface);
			collision = object.getCollision(surface);
			
			light_mask = object.getLightMask(surface);
			decal_mask = object.getDecalMask(surface);
			viewport_mask = object.getViewportMask(surface);
			intersection_mask = object.getIntersectionMask(surface);
			collision_mask = object.getCollisionMask(surface);
			
			min_parent = object.getMinParent(surface);
			max_parent = object.getMaxParent(surface);
			min_visible_distance = object.getMinVisibleDistance(surface);
			max_visible_distance = object.getMaxVisibleDistance(surface);
			min_fade_distance = object.getMinFadeDistance(surface);
			max_fade_distance = object.getMaxFadeDistance(surface);
			
			material = object.getMaterialName(surface);
			if(object.isMaterialInherited(surface)) {
				material_blob = new ::Blob(0);
				Material m = object.getMaterialInherit(surface);
				m.saveState(material_blob,1);
				material_blob.seekSet(0);
			}
			
			property = object.getPropertyName(surface);
			if(object.isPropertyInherited(surface)) {
				property_blob = new ::Blob(0);
				Property p = object.getPropertyInherit(surface);
				p.saveState(property_blob,1);
				property_blob.seekSet(0);
			}
			
		}
		
		void apply(int surface) {
			
			object.setEnabled(enabled,surface);
			object.setDecal(decal,surface);
			object.setCastShadow(cast_shadow,surface);
			object.setReceiveShadow(receive_shadow,surface);
			object.setCastWorldShadow(cast_world_shadow,surface);
			object.setReceiveWorldShadow(receive_world_shadow,surface);
			object.setIntersection(intersection,surface);
			object.setCollision(collision,surface);
			
			object.setLightMask(light_mask,surface);
			object.setDecalMask(decal_mask,surface);
			object.setViewportMask(viewport_mask,surface);
			object.setIntersectionMask(intersection_mask,surface);
			object.setCollisionMask(collision_mask,surface);
			
			object.setMinParent(min_parent,surface);
			object.setMaxParent(max_parent,surface);
			object.setMinVisibleDistance(min_visible_distance,surface);
			object.setMaxVisibleDistance(max_visible_distance,surface);
			object.setMinFadeDistance(min_fade_distance,surface);
			object.setMaxFadeDistance(max_fade_distance,surface);
			
			if(strlen(material)) {
				object.setMaterial(material,surface);
				
				if(material_blob != NULL) {
					Material m = object.getMaterialInherit(surface);
					m.restoreState(material_blob);
				}
				
			} else {
				object.setMaterial(NULL,surface);
			}
			
			if(strlen(property)) {
				object.setProperty(property,surface);
				
				if(property_blob != NULL) {
					Property p = object.getPropertyInherit(surface);
					p.restoreState(property_blob);
				}
				
			} else {
				object.setProperty(NULL,surface);
			}
		}
	};
	
	Buffer buffers[];
	
	/*
	 */
	void init() {
		
		parameters_tb.addTab(TR("Surfaces"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
		// body
		Body::shutdown();
		current_surfaces.clear();
		
		object = NULL;
	}
	
	/*
	 */
	void update_view() {
		
		if(object == NULL) return;
		
		// current surface
		int surface = -1;
		if(surfaces_tb.getNumSelectedItems()) surface = treebox_surfaces[surfaces_tb.getSelectedItem(0)];
		
		setCallbackEnabled(GUI_CLICKED,0,(
			enabled_cb,
			decal_cb,
			cast_shadow_cb,
			receive_shadow_cb,
			cast_world_shadow_cb,
			receive_world_shadow_cb,
			intersection_cb,
			collision_cb,
		));
		
		// clear view
		if(surface == -1) {
			
			setEnabled(0,(
				enabled_cb,
				decal_cb,
				cast_shadow_cb,
				receive_shadow_cb,
				cast_world_shadow_cb,
				receive_world_shadow_cb,
				intersection_cb,
				collision_cb,
				light_mask_el,
				decal_mask_el,
				viewport_mask_el,
				intersection_mask_el,
				collision_mask_el,
				min_parent_el,
				max_parent_el,
				min_visible_distance_el,
				max_visible_distance_el,
				min_fade_distance_el,
				max_fade_distance_el,
				material_el,
				material_load_i,
				material_edit_i,
				material_clear_i,
				property_el,
				property_load_i,
				property_edit_i,
				property_clear_i,
			));
			
			enabled_cb.setChecked(0);
			decal_cb.setChecked(0);
			cast_shadow_cb.setChecked(0);
			receive_shadow_cb.setChecked(0);
			cast_world_shadow_cb.setChecked(0);
			receive_world_shadow_cb.setChecked(0);
			intersection_cb.setChecked(0);
			collision_cb.setChecked(0);
			light_mask_el.setText("");
			decal_mask_el.setText("");
			viewport_mask_el.setText("");
			intersection_mask_el.setText("");
			collision_mask_el.setText("");
			min_parent_el.setText("");
			max_parent_el.setText("");
			min_visible_distance_el.setText("");
			max_visible_distance_el.setText("");
			min_fade_distance_el.setText("");
			max_fade_distance_el.setText("");
			material_el.setText("");
			property_el.setText("");
		}
		// fill view
		else {
			
			setEnabled(1,(
				enabled_cb,
				decal_cb,
				cast_shadow_cb,
				receive_shadow_cb,
				cast_world_shadow_cb,
				receive_world_shadow_cb,
				intersection_cb,
				collision_cb,
				light_mask_el,
				decal_mask_el,
				viewport_mask_el,
				intersection_mask_el,
				collision_mask_el,
				min_parent_el,
				max_parent_el,
				min_visible_distance_el,
				max_visible_distance_el,
				min_fade_distance_el,
				max_fade_distance_el,
				material_el,
				material_load_i,
				material_edit_i,
				material_clear_i,
				property_el,
				property_load_i,
				property_edit_i,
				property_clear_i,
			));
			
			enabled_cb.setChecked(object.isEnabled(surface));
			decal_cb.setChecked(object.getDecal(surface));
			cast_shadow_cb.setChecked(object.getCastShadow(surface));
			receive_shadow_cb.setChecked(object.getReceiveShadow(surface));
			cast_world_shadow_cb.setChecked(object.getCastWorldShadow(surface));
			receive_world_shadow_cb.setChecked(object.getReceiveWorldShadow(surface));
			intersection_cb.setChecked(object.getIntersection(surface));
			collision_cb.setChecked(object.getCollision(surface));
			
			light_mask_el.setText(editorFormatMask(object.getLightMask(surface)));
			decal_mask_el.setText(editorFormatMask(object.getDecalMask(surface)));
			viewport_mask_el.setText(editorFormatMask(object.getViewportMask(surface)));
			intersection_mask_el.setText(editorFormatMask(object.getIntersectionMask(surface)));
			collision_mask_el.setText(editorFormatMask(object.getCollisionMask(surface)));
			
			min_parent_el.setText(string(object.getMinParent(surface)));
			max_parent_el.setText(string(object.getMaxParent(surface)));
			min_visible_distance_el.setText(editorFormat(object.getMinVisibleDistance(surface)));
			max_visible_distance_el.setText(editorFormat(object.getMaxVisibleDistance(surface)));
			min_fade_distance_el.setText(editorFormat(object.getMinFadeDistance(surface)));
			max_fade_distance_el.setText(editorFormat(object.getMaxFadeDistance(surface)));
			
			if(object.getMaterial(surface) != NULL) {
				material_el.setText(object.getMaterialName(surface));
				material_edit_i.setEnabled(object.isMaterialInherited(surface) != 2);
				material_clear_i.setEnabled(object.isMaterialInherited(surface));
			} else {
				material_el.setText("");
				material_edit_i.setEnabled(0);
				material_clear_i.setEnabled(0);
			}
			
			if(object.getProperty(surface) != NULL) {
				property_el.setText(object.getPropertyName(surface));
				property_edit_i.setEnabled(object.isPropertyInherited(surface) != 2);
				property_clear_i.setEnabled(object.isPropertyInherited(surface));
			} else {
				property_el.setText("");
				property_edit_i.setEnabled(0);
				property_clear_i.setEnabled(0);
			}
		}
		
		setCallbackEnabled(GUI_CLICKED,1,(
			enabled_cb,
			decal_cb,
			cast_shadow_cb,
			receive_shadow_cb,
			cast_world_shadow_cb,
			receive_world_shadow_cb,
			intersection_cb,
			collision_cb,
		));
		
		paste_b.setEnabled(buffers.size() > 0);
	}
	
	/*
	 */
	void get_surfaces(int s[]) {
		s.clear();
		forloop(int i = 0; surfaces_tb.getNumSelectedItems()) {
			int item = surfaces_tb.getSelectedItem(i);
			s.append(treebox_surfaces[item]);
		}
	}
	
	void get_current_surfaces() {
		get_surfaces(current_surfaces);
		if(surfaces_tb.getNumSelectedItems()) {
			current_surfaces.append(treebox_surfaces[surfaces_tb.getSelectedItem(0)]);
		}
	}
	
	void get_current_surfaces(int s[]) {
		s.copy(current_surfaces);
	}
	
	/*
	 */
	void update() {
		object = class_cast("Object",node);
		
		// create surfaces
		surface_indices.clear();
		
		forloop(int i = 0; object.getNumSurfaces()) {
			string name = object.getSurfaceName(i);
			surface_indices.append(name,i);
		}
		
		surfaces_tb.setCallbackEnabled(GUI_CHANGED,0);
		
		// surfaces treebox
		treebox_surfaces.clear();
		treebox_items.clear();
		surface_items.clear();
		
		surfaces_tb.clear();
		
		foreachkey(string name; surface_indices) {
			int surface_item = surfaces_tb.addItem(name);
			int surface_index = surface_indices[name];
			
			treebox_surfaces.append(surface_item,surface_index);
			treebox_items.append(surface_index,surface_item);
			surface_items.append(name,surface_item);
			
			if(object.isEnabled(surface_index)) surfaces_tb.setItemColor(surface_item,surfaces_tb.getFontColor());
			else surfaces_tb.setItemColor(surface_item,surfaces_tb.getFontColor() * Vec4(0.5f,0.5f,0.5f,1.0f));
		}
		
		// hierarchy
		foreachkey(string name; surface_indices) {
			int surface_item = surface_items[name];
			int surface_index = surface_indices[name];
			
			int parent = object.getParent(surface_index);
			if(parent != -1) surfaces_tb.setItemParent(surface_item,treebox_items[parent]);
		}
		
		// folded flag
		foreachkey(string name; surface_indices) {
			int surface_item = surface_items[name];
			int surface_index = surface_indices[name];
			
			surfaces_tb.setItemFolded(surface_item,object.isFolded(surface_index));
		}
		
		// select first surface
		forloop(int i = 0; surfaces_tb.getNumItems()) {
			int item = surfaces_tb.getItem(i);
			if(surfaces_tb.getItemParent(item) == -1) {
				surfaces_tb.setItemSelected(item,1);
				Material material = object.getMaterial(treebox_surfaces[item]);
				if(material != NULL) materialsSelect(material.getName());
				Property property = object.getProperty(treebox_surfaces[item]);
				if(property != NULL) propertiesSelect(property.getName());
				break;
			}
		}
		
		surfaces_tb.setCallbackEnabled(GUI_CHANGED,1);
		
		get_current_surfaces();
		
		update_view();
	}
	
	/*
	 */
	void update_dynamic() {
		
		// show object info
		if(helpersGetNodeInfo()) {
			if(surfaces_tb.getNumSelectedItems() > 0) {
				int surface_item = surfaces_tb.getSelectedItem(0);
				int surface_index = treebox_surfaces[surface_item];
				
				WorldBoundSphere bs = object.getWorldBoundSphere(surface_index);
				Vec3 center = bs.getCenter();
				string message = get_dynamic_info(object,surface_index);
				
				engine.visualizer.renderMessage3D(center,vec3_zero,message,vec4_one,1);
			}
		}
		
		render_visualizer(object);
		
		// body
		Body::update_dynamic();
	}
	
	/*
	 */
	string get_dynamic_info(Object object,int surface) {
		
		string node_info = run_function("get_info",object);
		
		// surface parameters
		Material material = object.getMaterial(surface);
		Property property = object.getProperty(surface);
		
		// number of triangles
		int num_triangles = object.getNumTriangles(surface);
		
		// surface world bounds
		WorldBoundSphere bs = object.getWorldBoundSphere(surface);
		Vec3 center = bs.getCenter();
		Scalar radius = bs.getRadius();
		
		// distance to the surface
		Player player = Unigine::getPlayer();
		Vec3 camera_position = (player != NULL) ? player.getPosition() : Vec3_zero;
		float distance = length(center - camera_position) - radius;
		
		string info = format(TR("%s\nSurface: %s\n"),node_info,object.getSurfaceName(surface));
		if(material != NULL) info += format(TR("Material: %s\n"),material.getName());
		if(property != NULL) info += format(TR("Property: %s\n"),property.getName());
		info += format(TR("Triangles: %d\nCenter: %g %g %g\nRadius: %g\nDistance: %g"),num_triangles,center.x,center.y,center.z,radius,distance);
		
		return info;
	}
	
	/*
	 */
	void render_visualizer(Object object) {
		if(object != Nodes::node) {
			if(wireframe_color.a > 0.0f) engine.visualizer.renderObject(object,wireframe_color);
			return;
		}
		
		// show object wireframe
		if(wireframe_color.a > 0.0f) {
			forloop(int i = 0; surfaces_tb.getNumSelectedItems()) {
				int surface_item = surfaces_tb.getSelectedItem(i);
				int surface_index = treebox_surfaces[surface_item];
				
				engine.visualizer.renderObjectSurface(object,surface_index,wireframe_color);
			}
		}
	}
	
	/*
	 */
	void update_material_name() {
		if(object == NULL || surfaces_tb.getNumSelectedItems() == 0) return;
		material_el.setText(object.getMaterialName(treebox_surfaces[surfaces_tb.getSelectedItem(0)]));
	}
	
	void update_property_name() {
		if(object == NULL || surfaces_tb.getNumSelectedItems() == 0) return;
		property_el.setText(object.getPropertyName(treebox_surfaces[surfaces_tb.getSelectedItem(0)]));
	}
	
	void update_surfaces() {
		forloop(int i = 0; surfaces_tb.getNumSelectedItems()) {
			int item = surfaces_tb.getSelectedItem(i);
			int surface = treebox_surfaces[item];
			int parent = surfaces_tb.getItemParent(item);
			
			if(parent == -1 || parent == item) object.setParent(-1,surface);
			else object.setParent(treebox_surfaces[parent],surface);
			
			Material material = object.getMaterial(surface);
			if(material != NULL) materialsSelect(material.getName());
			
			Property property = object.getProperty(surface);
			if(property != NULL) propertiesSelect(property.getName());
		}
		
		// folded flag
		foreachkey(string name; surface_indices) {
			int surface_item = surface_items[name];
			int surface_index = surface_indices[name];
			
			object.setFolded(surfaces_tb.isItemFolded(surface_item),surface_index);
		}
	}
	
	/*
	 */
	void surface_select_all() {
		set_wireframe_mode(1);
		
		surfaces_tb.setCallbackEnabled(GUI_CHANGED,0);
		
		foreach(int surface_item; surface_items) {
			surfaces_tb.setItemSelected(surface_item,1);
		}
		
		get_current_surfaces();
		
		surfaces_tb.setCallbackEnabled(GUI_CHANGED,1);
	}
	
	/*
	 */
	void surface_select(int surface,int toggle) {
		set_wireframe_mode(1);
		
		// multiple selection
		surfaces_tb.setCallbackEnabled(GUI_CHANGED,0);
		
		if(toggle != 0) {
			
			int item = treebox_items[surface];
			surfaces_tb.setItemSelected(item,toggle > 0);
			
			// show new surface
			if(surfaces_tb.isItemSelected(item)) {
				surfaces_tb.showItem(item);
			}
			// select previous surface
			else {
				forloop(int i = 0; surfaces_tb.getNumSelectedItems()) {
					surfaces_tb.showItem(surfaces_tb.getSelectedItem(i));
				}
			}
		}
		// single selection
		else {
			foreachkey(string name; surface_indices) {
				int surface_item = surface_items[name];
				int surface_index = surface_indices[name];
				
				if(surface_index == surface) {
					surfaces_tb.setItemSelected(surface_item,1);
					surfaces_tb.showItem(surface_item);
				} else {
					surfaces_tb.setItemSelected(surface_item,0);
				}
			}
		}
		
		update_surfaces();
		update_view();
		surfaces_tb.setCallbackEnabled(GUI_CHANGED,1);
		
		get_current_surfaces();
	}
	
	/*
	 */
	void name_changed() {
		name_el.setCallbackEnabled(GUI_CHANGED,0);
		surfaces_tb.setCallbackEnabled(GUI_CHANGED,0);
		restoreTreeBoxFolded(surfaces_tb,surfaces_folded);
		selectTreeBoxByName(surfaces_tb,name_el);
		update_view();
		name_el.setCallbackEnabled(GUI_CHANGED,1);
		surfaces_tb.setCallbackEnabled(GUI_CHANGED,1);
	}
	
	void name_focus_in() {
		saveTreeBoxFolded(surfaces_tb,surfaces_folded);
	}
	
	void name_focus_out() {
		surfaces_folded.clear();
	}
	
	/*
	 */
	void surfaces_changed() {
		if(object == NULL) return;
		set_wireframe_mode(1);
		// save old selection
		if(main_vb.isChild(gui.getFocus())) {
			save_state(create_state_select(nodes,node,current_surfaces));
		}
		
		// update surfaces
		update_surfaces();
		
		update_view();
		get_current_surfaces();
	}
	
	void surfaces_double_clicked() {
		set_wireframe_mode(1);
		
		forloop(int i = 0; surfaces_tb.getNumSelectedItems()) {
			int item = surfaces_tb.getSelectedItem(i);
			surfaces_tb.setItemFolded(item,!surfaces_tb.isItemFolded(item));
		}
	}
	
	void enabled_clicked() {
		if(object == NULL) return;
		
		wireframe_mode = 1;
		
		NodesState_ s = new NodesState_();
		s.init_modify(nodes,object);
		int enabled = enabled_cb.isChecked();
		
		forloop(int i = 0; surfaces_tb.getNumSelectedItems()) {
			int surface_item = surfaces_tb.getSelectedItem(i);
			int surface_index = treebox_surfaces[surface_item];
			
			object.setEnabled(enabled,surface_index);
			
			if(enabled) surfaces_tb.setItemColor(surface_item,surfaces_tb.getFontColor());
			else surfaces_tb.setItemColor(surface_item,surfaces_tb.getFontColor() * Vec4(0.5f,0.5f,0.5f,1.0f));
		}
		
		object.setSpatial(object.isSpatial());
		save_state(s);
	}
	
	void update_wireframe() {
		if(helpersGetObjectWireframe()) {
			set_wireframe_mode(1);
			wireframe_color.a = 0.5;
		} else if(wireframe_mode) {
			if(wireframe_color.a >= 0.5f) wireframe_mode = 0;
			else wireframe_color.a += engine.app.getIFps() * 4.0f;
		} else if(wireframe_color.a > 0.0f) {
			wireframe_color.a -= engine.app.getIFps() * 0.75f;
		}
	}
	
	void set_wireframe_mode(int mode) {
		wireframe_mode = mode;
	}
	
	/*
	 */
	SURFACE_CLICKED_CALLBACK(decal,Decal)
	SURFACE_CLICKED_CALLBACK(cast_shadow,CastShadow)
	SURFACE_CLICKED_CALLBACK(receive_shadow,ReceiveShadow)
	SURFACE_CLICKED_CALLBACK(cast_world_shadow,CastWorldShadow)
	SURFACE_CLICKED_CALLBACK(receive_world_shadow,ReceiveWorldShadow)
	SURFACE_CLICKED_CALLBACK(intersection,Intersection)
	SURFACE_CLICKED_CALLBACK(collision,Collision)
	SURFACE_PRESSED_CALLBACK_MASK(light_mask,LightMask)
	SURFACE_CHANGED_CALLBACK(light_mask,LightMask)
	SURFACE_CLICKED_CALLBACK_MASK(light_mask,LightMask,TR("Surface light mask"))
	SURFACE_PRESSED_CALLBACK_MASK(decal_mask,DecalMask)
	SURFACE_CHANGED_CALLBACK(decal_mask,DecalMask)
	SURFACE_CLICKED_CALLBACK_MASK(decal_mask,DecalMask,TR("Surface decal mask"))
	SURFACE_PRESSED_CALLBACK_MASK(viewport_mask,ViewportMask)
	SURFACE_CHANGED_CALLBACK(viewport_mask,ViewportMask)
	SURFACE_CLICKED_CALLBACK_MASK(viewport_mask,ViewportMask,TR("Surface viewport mask"))
	SURFACE_PRESSED_CALLBACK_MASK(intersection_mask,IntersectionMask)
	SURFACE_CHANGED_CALLBACK(intersection_mask,IntersectionMask)
	SURFACE_CLICKED_CALLBACK_MASK(intersection_mask,IntersectionMask,TR("Surface intersection mask"))
	SURFACE_PRESSED_CALLBACK_MASK(collision_mask,CollisionMask)
	SURFACE_CHANGED_CALLBACK(collision_mask,CollisionMask)
	SURFACE_CLICKED_CALLBACK_MASK(collision_mask,CollisionMask,TR("Surface collision mask"))
	SURFACE_PRESSED_CALLBACK(min_parent,MinParent,int,string)
	SURFACE_PRESSED_CALLBACK(max_parent,MaxParent,int,string)
	SURFACE_PRESSED_CALLBACK(min_visible_distance,MinVisibleDistance,float,editorFormat)
	SURFACE_PRESSED_CALLBACK(max_visible_distance,MaxVisibleDistance,float,editorFormat)
	SURFACE_PRESSED_CALLBACK(min_fade_distance,MinFadeDistance,float,editorFormat)
	SURFACE_PRESSED_CALLBACK(max_fade_distance,MaxFadeDistance,float,editorFormat)
	SURFACE_PRESSED_CALLBACK_ATTRIBUTE(material,Material)
	SURFACE_CLICKED_CALLBACK_ATTRIBUTE_LOAD(material,Material,materialsSelect)
	SURFACE_CLICKED_CALLBACK_ATTRIBUTE_EDIT(material,Material,materialsEditor)
	SURFACE_CLICKED_CALLBACK_ATTRIBUTE_CLEAR(material,Material)
	SURFACE_PRESSED_CALLBACK_ATTRIBUTE(property,Property)
	SURFACE_CLICKED_CALLBACK_ATTRIBUTE_LOAD(property,Property,propertiesSelect)
	SURFACE_CLICKED_CALLBACK_ATTRIBUTE_EDIT(property,Property,propertiesEditor)
	SURFACE_CLICKED_CALLBACK_ATTRIBUTE_CLEAR(property,Property)
	
	/*
	 */
	void copy_clicked() {
		
		buffers.delete();
		
		forloop(int i = 0; object.getNumSurfaces()) {
			buffers.append(object.getSurfaceName(i),new Buffer(i));
		}
		
		paste_b.setEnabled(buffers.size() > 0);
	}
	
	void paste_clicked() {
		
		forloop(int i = 0; object.getNumSurfaces()) {
			Buffer buffer = buffers.check(object.getSurfaceName(i),NULL);
			if(buffer != NULL) buffer.apply(i);
		}
		
		update_view();
	}
	
	/**************************************************************************\
	*
	* Dummy
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Dummy {
		
		/*
		 */
		void init() {
			
			Nodes::Object::init();
		}
		
		void shutdown() {
			
			Nodes::Object::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Object::update();
			
			Nodes::Object::Body::init();
			Nodes::Object::Body::update();
		}
		
		/*
		 */
		void show_bodies(Node node) {
			
			if(node.isObject()) {
				Body body = Object(node_cast(node)).getBody();
				if(body != NULL && body.isEnabled()) {
					vec4 color = vec4(0.5f,0.5f,0.5f,1.0f);
					if(object == node) color = vec4_one;
					forloop(int i = 0; body.getNumShapes()) {
						Shape shape = body.getShape(i);
						if(shape.isEnabled() == 0) continue;
						shape.renderVisualizer(color);
					}
					forloop(int i = 0; body.getNumJoints()) {
						Joint joint = body.getJoint(i);
						if(joint.isEnabled() == 0) continue;
						joint.renderVisualizer(color);
					}
				}
			}
			
			forloop(int i = 0; node.getNumChilds()) {
				show_bodies(node.getChild(i));
			}
		}
		
		/*
		 */
		void update_dynamic() {
			
			// show dummy info
			if(helpersGetNodeInfo()) {
				vec3 position = get_world_bound_center(node);
				string message = call("Nodes::get_info") + format(TR("\nChildren: %d"),node.getNumChilds());
				
				if(node.getPropertyName() != "") message += format(TR("\nProperty: %s"),node.getPropertyName());
				
				Player player = Unigine::getPlayer();
				vec3 camera_position = (player != NULL) ? player.getPosition() : vec3_zero;
				vec3 center = node.getIWorldTransform() * position;
				float radius = get_bound_radius(node);
				float distance = length(position - camera_position) - radius;
				message += format(TR("\nCenter: %g %g %g\nRadius: %g\nDistance: %g"),center.x,center.y,center.z,radius,distance);
				
				engine.visualizer.renderMessage3D(position,vec3_zero,message,vec4_one,1);
			}
			
			// show dummy bounds
			if(helpersGetObjectBounds()) {
				vec3 max = get_world_bound_max(node);
				vec3 min = get_world_bound_min(node);
				vec3 position = get_world_bound_center(node);
				engine.visualizer.renderBox(max - min,translate(position),vec4(0,1,0,1));
			}
			
			// show bones
			if(object.getParent() != NULL) show_bodies(object.getParent());
			else show_bodies(object);
			
			// body
			Body::update_dynamic();
		}
		
	} /* namespace Dummy */
	
	/**************************************************************************\
	*
	* Dynamic
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Dynamic {
		
		/*
		 */
		void init() {
			
			Nodes::Object::init();
		}
		
		void shutdown() {
			
			Nodes::Object::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Object::update();
			
			Nodes::Object::Body::init();
			Nodes::Object::Body::update();
		}
		
		/*
		 */
		void update_dynamic() {
			
			// show dummy info
			if(helpersGetNodeInfo()) {
				vec3 position = get_world_bound_center(node);
				string message = call("Nodes::get_info") + format(TR("\nChildren: %d"),node.getNumChilds());
				
				if(node.getPropertyName() != "") message += format(TR("\nProperty: %s"),node.getPropertyName());
				
				Player player = Unigine::getPlayer();
				vec3 camera_position = (player != NULL) ? player.getPosition() : vec3_zero;
				vec3 center = node.getIWorldTransform() * position;
				float radius = get_bound_radius(node);
				float distance = length(position - camera_position) - radius;
				message += format(TR("\nCenter: %g %g %g\nRadius: %g\nDistance: %g"),center.x,center.y,center.z,radius,distance);
				
				engine.visualizer.renderMessage3D(position,vec3_zero,message,vec4_one,1);
			}
			
			// body
			Body::update_dynamic();
		}
		
	} /* namespace Dynamic */
	
	/**************************************************************************\
	*
	* Extern
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Extern {
		
		/*
		 */
		void init() {
			
			Nodes::Object::init();
		}
		
		void shutdown() {
			
			Nodes::Object::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Object::update();
			
			Nodes::Object::Body::init();
			Nodes::Object::Body::update();
		}
		
		/*
		 */
		void update_dynamic() {
			
			// body
			Body::update_dynamic();
		}
		
	} /* namespace Dummy */
	
	/*
	 */
	#include <editor/editor_objects_meshes.h>
	#include <editor/editor_objects_terrain.h>
	#include <editor/editor_objects_grass.h>
	#include <editor/editor_objects_particles.h>
	#include <editor/editor_objects_billboard.h>
	#include <editor/editor_objects_billboards.h>
	#include <editor/editor_objects_volumes.h>
	#include <editor/editor_objects_gui.h>
	#include <editor/editor_objects_water.h>
	#include <editor/editor_objects_sky.h>
	
	void world_save() {
		Terrain::save();
	}
	
} /* namespace Object */
