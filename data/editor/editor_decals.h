/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_decals.h
 * Desc:    Unigine editor
 * Version: 1.09
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
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

/*
 */
#define DECAL_PRESSED_CALLBACK_FLOAT(WIDGET,DECAL,FUNC) \
void WIDGET ## _pressed() { \
	if(DECAL == NULL) return; \
	Node temp = DECAL; \
	NodesState_ s = new NodesState_(); \
	float value = float(WIDGET ## _el.getText()); \
	int type = DECAL.getType(); \
	foreach(Node n; nodes) { \
		if(n.getType() != type) continue; \
		DECAL = node_cast(n); \
		if(DECAL.get ## FUNC() != value) { \
			if(s.state_type == Unredo::STATE_NONE) s.init_modify(nodes,DECAL); \
			DECAL.set ## FUNC(value); \
			update_decal(DECAL); \
		} \
	} \
	DECAL = temp; \
	WIDGET ## _el.setText(editorFormat(DECAL.get ## FUNC())); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

#define DECAL_PRESSED_CALLBACK_STRING(WIDGET,DECAL,FUNC) \
void WIDGET ## _pressed() { \
	if(DECAL == NULL) return; \
	Node temp = DECAL; \
	NodesState_ s = new NodesState_(); \
	string value = WIDGET ## _el.getText(); \
	int type = DECAL.getType(); \
	foreach(Node n; nodes) { \
		if(n.getType() != type) continue; \
		DECAL = node_cast(n); \
		if(DECAL.get ## FUNC() != value) { \
			if(s.state_type == Unredo::STATE_NONE) s.init_modify(nodes,DECAL); \
			DECAL.set ## FUNC(value); \
			update_decal(DECAL); \
		} \
	} \
	DECAL = temp; \
	WIDGET ## _el.setText(DECAL.get ## FUNC()); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

#define DECAL_PRESSED_CALLBACK_VEC4(WIDGET,DECAL,FUNC) \
void WIDGET ## _pressed() { \
	if(DECAL == NULL) return; \
	NodesState_ s = new NodesState_(); \
	int changed = 0; \
	Node node = DECAL; \
	int type = DECAL.getType(); \
	vec4 value = getEditLineVec4(WIDGET ## _el); \
	foreach(Node n; nodes) { \
		if(n.getType() != type) continue; \
		DECAL = node_cast(n); \
		if(DECAL.get ## FUNC() != value) { \
			changed = 1; \
			break; \
		} \
	} \
	if(changed) { \
		s.init_modify(nodes,DECAL); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			DECAL = node_cast(n); \
			DECAL.set ## FUNC(value); \
			update_decal(DECAL); \
		} \
	} \
	DECAL = node; \
	setEditLineVec4(WIDGET ## _el,DECAL.get ## FUNC()); \
	foreach(WidgetEditLine editline; WIDGET ## _el) { \
		if(toolsGetSelection() && editline.isFocused()) { \
			editline.setCursor(0); \
			editline.setSelection(1024); \
		} \
	} \
	save_state(s); \
}

#define DECAL_PRESSED_CALLBACK_SLIDER(WIDGET,DECAL,FUNC) \
void WIDGET ## _pressed() { \
	int previous_focus; \
	if(DECAL == NULL) return; \
	float value = float(WIDGET ## _el.getText()); \
	int current_focus = (WIDGET ## _sl.isFocused() || WIDGET ## _el.isFocused()); \
	if(!(previous_focus || current_focus) && DECAL.get ## FUNC() == value) { \
		WIDGET ## _el.setText(editorFormat(value)); \
		return; \
	} \
	previous_focus = current_focus; \
	Node temp = DECAL; \
	NodesState_ s = new NodesState_(); \
	int type = DECAL.getType(); \
	foreach(Node n; nodes) { \
		if(n.getType() != type) continue; \
		DECAL = node_cast(n); \
		if(DECAL.get ## FUNC() != value) { \
			if(s.state_type == Unredo::STATE_NONE) s.init_modify(nodes,DECAL); \
			DECAL.set ## FUNC(value); \
			update_decal(DECAL); \
		} \
	} \
	DECAL = temp; \
	WIDGET ## _el.setText(editorFormat(value)); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s,engine.gui.getFocus()); \
}

#define DECAL_PRESSED_CALLBACK_MASK(WIDGET,DECAL,FUNC) \
void WIDGET ## _pressed() { \
	if(DECAL == NULL) return; \
	Node temp = DECAL; \
	int mask = int("0x" + WIDGET ## _el.getText()); \
	NodesState_ s = new NodesState_(); \
	int type = DECAL.getType(); \
	foreach(Node n; nodes) { \
		if(n.getType() != type) continue; \
		DECAL = node_cast(n); \
		if(DECAL.get ## FUNC() != mask) { \
			if(s.state_type == Unredo::STATE_NONE) s.init_modify(nodes,DECAL); \
			DECAL.set ## FUNC(mask); \
			update_decal(DECAL); \
		} \
	} \
	DECAL = temp; \
	WIDGET ## _el.setText(editorFormatMask(DECAL.get ## FUNC())); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

/*
 */
#define DECAL_CLICKED_CALLBACK(WIDGET,DECAL,FUNC) \
void WIDGET ## _clicked() { \
	if(DECAL == NULL) return; \
	Node temp = DECAL; \
	NodesState_ s = create_state_modify(nodes,node); \
	int type = DECAL.getType(); \
	foreach(Node n; nodes) { \
		if(n.getType() != type) continue; \
		DECAL = node_cast(n); \
		DECAL.set ## FUNC(WIDGET ## _cb.isChecked()); \
		update_decal(DECAL); \
	} \
	DECAL = temp; \
	save_state(s); \
}

#define DECAL_CLICKED_CALLBACK_MASK(WIDGET,DECAL,FUNC,TITLE,FUNC_ID) \
void WIDGET ## _clicked() { \
	NodesState_ s = new NodesState_(); \
	int old_mask = DECAL.get ## FUNC(); \
	dialogMask(TITLE,FUNC_ID,old_mask); \
	int type = DECAL.getType(); \
	Node node = DECAL; \
	int mask = DECAL.get ## FUNC(); \
	int changed = (old_mask != mask); \
	if(changed == 0) { \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			DECAL = node_cast(n); \
			if(DECAL.get ## FUNC() != mask) { \
				changed = 1; \
				break; \
			} \
		} \
	} \
	if(changed) { \
		s.init_modify(nodes,DECAL); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			DECAL = node_cast(n); \
			DECAL.set ## FUNC(mask); \
			update_decal(DECAL); \
		} \
	} \
	DECAL = node; \
	save_state(s); \
}

/*
 */
namespace Decal {
	
	/*
	 */
	WidgetVBox main_vb;						// main vbox
	
	WidgetEditLine decal_mask_el;			// decal mask
	WidgetEditLine light_mask_el;			// light mask
	WidgetEditLine viewport_mask_el;		// viewport mask
	WidgetEditLine life_time_el;			// life time
	WidgetEditLine fade_time_el;			// fade time
	WidgetEditLine min_visible_distance_el;	// min visible distance
	WidgetEditLine max_visible_distance_el;	// max visible distance
	WidgetEditLine min_fade_distance_el;	// min fade distance
	WidgetEditLine max_fade_distance_el;	// max fade distance
	WidgetEditLine texcoord_el[4];			// texture coordinates
	WidgetEditLine material_el;				// material name
	WidgetVBox dummy_vb;					// dummy container
	
	Decal decal;							// base decal
	
	/*
	 */
	void init() {
		
		// add decal tab
		parameters_tb.addTab(TR("Decal"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
	}
	
	void shutdown() {
		
		decal = NULL;
	}
	
	/*
	 */
	void update() {
		
		decal = class_cast("Decal",node);
		
		decal_mask_el.setText(editorFormatMask(decal.getDecalMask()));
		light_mask_el.setText(editorFormatMask(decal.getLightMask()));
		viewport_mask_el.setText(editorFormatMask(decal.getViewportMask()));
		life_time_el.setText(editorFormat(decal.getLifeTime()));
		fade_time_el.setText(editorFormat(decal.getFadeTime()));
		min_visible_distance_el.setText(editorFormat(decal.getMinVisibleDistance()));
		max_visible_distance_el.setText(editorFormat(decal.getMaxVisibleDistance()));
		min_fade_distance_el.setText(editorFormat(decal.getMinFadeDistance()));
		max_fade_distance_el.setText(editorFormat(decal.getMaxFadeDistance()));
		setEditLineVec4(texcoord_el,decal.getTexCoord());
		
		material_el.setText(decal.getMaterialName());
		
		materialsSelect(decal.getMaterialName());
	}
	
	/*
	 */
	void update_dynamic() {
		decal.renderVisualizer();
	}
	
	/*
	 */
	void update_material_name() {
		if(decal == NULL) return;
		material_el.setText(decal.getMaterialName());
	}
	
	/*
	 */
	void update_decal(Decal decal) {
		if(decal.isDecalObject()) {
			DecalObject decal_object = node_cast(decal);
			decal_object.clear();
			decal_object.create();
		}
	}
	
	/*
	 */
	void set_material(string name) {
		NodesState_ s = new NodesState_();
		int type = decal.getType();
		foreach(Node n; nodes) {
			if(n.getType() != type) continue;
			Decal decal = node_cast(n);
			if(decal.getMaterialName() != name) {
				if(s.state_type == Unredo::STATE_NONE) s.init_modify(nodes,decal);
				decal.setMaterial(name);
				update_decal(decal);
			}
		}
		material_el.setText(decal.getMaterialName());
		save_state(s);
	}
	
	void material_pressed() {
		string material_name = decal.getMaterialName();
		if(material_name == material_el.getText()) return;
		set_material(material_name);
	}
	
	void material_load_clicked() {
		string material_name = decal.getMaterialName();
		if(dialogMaterial(TR("Select material"),material_name)) {
			set_material(material_name);
		}
	}
	
	void material_clear_clicked() {
		string material_name = "decal_base";
		if(decal.getMaterialName() == material_name) return;
		if(dialogMessageYesNo(TR("Confirm"),TR("Clear decal material?"))) {
			set_material(material_name);
		}
	}
	
	/*
	 */
	void decal_mask_changed(int mask) {
		decal.setDecalMask(mask);
		decal_mask_el.setText(editorFormatMask(decal.getDecalMask()));
		update_decal(decal);
	}
	
	void light_mask_changed(int mask) {
		decal.setLightMask(mask);
		light_mask_el.setText(editorFormatMask(decal.getLightMask()));
		update_decal(decal);
	}
	
	void viewport_mask_changed(int mask) {
		decal.setViewportMask(mask);
		viewport_mask_el.setText(editorFormatMask(decal.getViewportMask()));
		update_decal(decal);
	}
	
	/*
	 */
	DECAL_PRESSED_CALLBACK_MASK(decal_mask,decal,DecalMask)
	DECAL_CLICKED_CALLBACK_MASK(decal_mask,decal,DecalMask,TR("Decal mask"),functionid(decal_mask_changed))
	DECAL_PRESSED_CALLBACK_MASK(light_mask,decal,LightMask)
	DECAL_CLICKED_CALLBACK_MASK(light_mask,decal,LightMask,TR("Light mask"),functionid(light_mask_changed))
	DECAL_PRESSED_CALLBACK_MASK(viewport_mask,decal,ViewportMask)
	DECAL_CLICKED_CALLBACK_MASK(viewport_mask,decal,ViewportMask,TR("Viewport mask"),functionid(viewport_mask_changed))
	DECAL_PRESSED_CALLBACK_FLOAT(life_time,decal,LifeTime)
	DECAL_PRESSED_CALLBACK_FLOAT(fade_time,decal,FadeTime)
	DECAL_PRESSED_CALLBACK_FLOAT(min_visible_distance,decal,MinVisibleDistance)
	DECAL_PRESSED_CALLBACK_FLOAT(max_visible_distance,decal,MaxVisibleDistance)
	DECAL_PRESSED_CALLBACK_FLOAT(min_fade_distance,decal,MinFadeDistance)
	DECAL_PRESSED_CALLBACK_FLOAT(max_fade_distance,decal,MaxFadeDistance)
	DECAL_PRESSED_CALLBACK_VEC4(texcoord,decal,TexCoord)
	
	/**************************************************************************\
	*
	* ObjectOmni
	*
	\**************************************************************************/
	
	/*
	 */
	namespace ObjectOmni {
		
		/*
		 */
		WidgetGridBox main_gb;
		
		WidgetCheckBox dynamic_cb;
		WidgetCheckBox two_sided_cb;
		WidgetEditLine decal_mask_el;
		WidgetEditLine light_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine offset_el;
		WidgetEditLine radius_el;
		WidgetEditLine znear_el;
		WidgetEditLine life_time_el;
		WidgetEditLine fade_time_el;
		WidgetEditLine min_visible_distance_el;
		WidgetEditLine max_visible_distance_el;
		WidgetEditLine min_fade_distance_el;
		WidgetEditLine max_fade_distance_el;
		WidgetEditLine texcoord_el[4];
		WidgetEditLine material_el;
		
		/*
		 */
		DecalObjectOmni decal;
		
		/*
		 */
		void init() {
			
			main_vb.replaceChild(main_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_gb);
			
			Nodes::Decal::init();
		}
		
		void shutdown() {
			
			decal = NULL;
			
			main_vb.replaceChild(dummy_vb,main_gb);
			
			Nodes::Decal::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Decal::decal_mask_el = decal_mask_el;
			Nodes::Decal::light_mask_el = light_mask_el;
			Nodes::Decal::viewport_mask_el = viewport_mask_el;
			Nodes::Decal::life_time_el = life_time_el;
			Nodes::Decal::fade_time_el = fade_time_el;
			Nodes::Decal::min_visible_distance_el = min_visible_distance_el;
			Nodes::Decal::max_visible_distance_el = max_visible_distance_el;
			Nodes::Decal::min_fade_distance_el = min_fade_distance_el;
			Nodes::Decal::max_fade_distance_el = max_fade_distance_el;
			Nodes::Decal::texcoord_el = texcoord_el;
			Nodes::Decal::material_el = material_el;
			
			Nodes::Decal::update();
			
			decal = node_cast(node);
			
			setCallbackEnabled(GUI_CLICKED,0,(
				dynamic_cb,
				two_sided_cb,
			));
			
			dynamic_cb.setChecked(decal.isDynamic());
			two_sided_cb.setChecked(decal.getTwoSided());
			
			offset_el.setText(editorFormat(decal.getOffset()));
			radius_el.setText(editorFormat(decal.getRadius()));
			znear_el.setText(editorFormat(decal.getZNear()));
			
			setCallbackEnabled(GUI_CLICKED,1,(
				dynamic_cb,
				two_sided_cb,
			));
		}
		
		/*
		 */
		void update_dynamic() {
			
			Nodes::Decal::update_dynamic();
		}
		
		/*
		 */
		DECAL_CLICKED_CALLBACK(dynamic,decal,Dynamic)
		DECAL_CLICKED_CALLBACK(two_sided,decal,TwoSided)
		DECAL_PRESSED_CALLBACK_FLOAT(offset,decal,Offset)
		DECAL_PRESSED_CALLBACK_FLOAT(radius,decal,Radius)
		DECAL_PRESSED_CALLBACK_FLOAT(znear,decal,ZNear)
		
	} /* namespace ObjectOmni */
	
	/**************************************************************************\
	*
	* ObjectProj
	*
	\**************************************************************************/
	
	/*
	 */
	namespace ObjectProj {
		
		/*
		 */
		WidgetGridBox main_gb;
		
		WidgetCheckBox dynamic_cb;
		WidgetCheckBox two_sided_cb;
		WidgetEditLine decal_mask_el;
		WidgetEditLine light_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine offset_el;
		WidgetEditLine radius_el;
		WidgetEditLine fov_el;
		WidgetSlider fov_sl;
		WidgetEditLine aspect_el;
		WidgetEditLine znear_el;
		WidgetEditLine life_time_el;
		WidgetEditLine fade_time_el;
		WidgetEditLine min_visible_distance_el;
		WidgetEditLine max_visible_distance_el;
		WidgetEditLine min_fade_distance_el;
		WidgetEditLine max_fade_distance_el;
		WidgetEditLine texcoord_el[4];
		WidgetEditLine material_el;
		
		/*
		 */
		DecalObjectProj decal;
		
		/*
		 */
		void init() {
			
			main_vb.replaceChild(main_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_gb);
			
			Nodes::Decal::init();
		}
		
		void shutdown() {
			
			decal = NULL;
			
			main_vb.replaceChild(dummy_vb,main_gb);
			
			Nodes::Decal::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Decal::decal_mask_el = decal_mask_el;
			Nodes::Decal::light_mask_el = light_mask_el;
			Nodes::Decal::viewport_mask_el = viewport_mask_el;
			Nodes::Decal::life_time_el = life_time_el;
			Nodes::Decal::fade_time_el = fade_time_el;
			Nodes::Decal::min_visible_distance_el = min_visible_distance_el;
			Nodes::Decal::max_visible_distance_el = max_visible_distance_el;
			Nodes::Decal::min_fade_distance_el = min_fade_distance_el;
			Nodes::Decal::max_fade_distance_el = max_fade_distance_el;
			Nodes::Decal::texcoord_el = texcoord_el;
			Nodes::Decal::material_el = material_el;
			
			Nodes::Decal::update();
			
			decal = node_cast(node);
			
			setCallbackEnabled(GUI_CLICKED,0,(
				dynamic_cb,
				two_sided_cb,
			));
			
			dynamic_cb.setChecked(decal.isDynamic());
			two_sided_cb.setChecked(decal.getTwoSided());
			
			offset_el.setText(editorFormat(decal.getOffset()));
			radius_el.setText(editorFormat(decal.getRadius()));
			fov_el.setText(editorFormat(decal.getFov()));
			aspect_el.setText(editorFormat(decal.getAspect()));
			znear_el.setText(editorFormat(decal.getZNear()));
			
			setCallbackEnabled(GUI_CLICKED,1,(
				dynamic_cb,
				two_sided_cb,
			));
		}
		
		/*
		 */
		void update_dynamic() {
			
			Nodes::Decal::update_dynamic();
		}
		
		/*
		 */
		DECAL_CLICKED_CALLBACK(dynamic,decal,Dynamic)
		DECAL_CLICKED_CALLBACK(two_sided,decal,TwoSided)
		DECAL_PRESSED_CALLBACK_FLOAT(offset,decal,Offset)
		DECAL_PRESSED_CALLBACK_FLOAT(radius,decal,Radius)
		DECAL_PRESSED_CALLBACK_SLIDER(fov,decal,Fov)
		DECAL_PRESSED_CALLBACK_FLOAT(aspect,decal,Aspect)
		DECAL_PRESSED_CALLBACK_FLOAT(znear,decal,ZNear)
		
	} /* namespace ObjectProj */
	
	/**************************************************************************\
	*
	* ObjectOrtho
	*
	\**************************************************************************/
	
	/*
	 */
	namespace ObjectOrtho {
		
		/*
		 */
		WidgetGridBox main_gb;
		
		WidgetCheckBox dynamic_cb;
		WidgetCheckBox two_sided_cb;
		WidgetEditLine decal_mask_el;
		WidgetEditLine light_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine offset_el;
		WidgetEditLine radius_el;
		WidgetEditLine width_el;
		WidgetEditLine height_el;
		WidgetEditLine znear_el;
		WidgetEditLine life_time_el;
		WidgetEditLine fade_time_el;
		WidgetEditLine min_visible_distance_el;
		WidgetEditLine max_visible_distance_el;
		WidgetEditLine min_fade_distance_el;
		WidgetEditLine max_fade_distance_el;
		WidgetEditLine texcoord_el[4];
		WidgetEditLine material_el;
		
		/*
		 */
		DecalObjectOrtho decal;
		
		/*
		 */
		void init() {
			
			main_vb.replaceChild(main_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_gb);
			
			Nodes::Decal::init();
		}
		
		void shutdown() {
			
			decal = NULL;
			
			main_vb.replaceChild(dummy_vb,main_gb);
			
			Nodes::Decal::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Decal::decal_mask_el = decal_mask_el;
			Nodes::Decal::light_mask_el = light_mask_el;
			Nodes::Decal::viewport_mask_el = viewport_mask_el;
			Nodes::Decal::life_time_el = life_time_el;
			Nodes::Decal::fade_time_el = fade_time_el;
			Nodes::Decal::min_visible_distance_el = min_visible_distance_el;
			Nodes::Decal::max_visible_distance_el = max_visible_distance_el;
			Nodes::Decal::min_fade_distance_el = min_fade_distance_el;
			Nodes::Decal::max_fade_distance_el = max_fade_distance_el;
			Nodes::Decal::texcoord_el = texcoord_el;
			Nodes::Decal::material_el = material_el;
			
			Nodes::Decal::update();
			
			decal = node_cast(node);
			
			setCallbackEnabled(GUI_CLICKED,0,(
				dynamic_cb,
				two_sided_cb,
			));
			
			dynamic_cb.setChecked(decal.isDynamic());
			two_sided_cb.setChecked(decal.getTwoSided());
			
			offset_el.setText(editorFormat(decal.getOffset()));
			radius_el.setText(editorFormat(decal.getRadius()));
			width_el.setText(editorFormat(decal.getWidth()));
			height_el.setText(editorFormat(decal.getHeight()));
			znear_el.setText(editorFormat(decal.getZNear()));
			
			setCallbackEnabled(GUI_CLICKED,1,(
				dynamic_cb,
				two_sided_cb,
			));
		}
		
		/*
		 */
		void update_dynamic() {
			
			Nodes::Decal::update_dynamic();
		}
		
		/*
		 */
		DECAL_CLICKED_CALLBACK(dynamic,decal,Dynamic)
		DECAL_CLICKED_CALLBACK(two_sided,decal,TwoSided)
		DECAL_PRESSED_CALLBACK_FLOAT(offset,decal,Offset)
		DECAL_PRESSED_CALLBACK_FLOAT(radius,decal,Radius)
		DECAL_PRESSED_CALLBACK_FLOAT(width,decal,Width)
		DECAL_PRESSED_CALLBACK_FLOAT(height,decal,Height)
		DECAL_PRESSED_CALLBACK_FLOAT(znear,decal,ZNear)
		
	} /* namespace ObjectProj */
	
	/**************************************************************************\
	*
	* TerrainProj
	*
	\**************************************************************************/
	
	/*
	 */
	namespace TerrainProj {
		
		/*
		 */
		WidgetGridBox main_gb;
		
		WidgetEditLine decal_mask_el;
		WidgetEditLine light_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine radius_el;
		WidgetEditLine fov_el;
		WidgetSlider fov_sl;
		WidgetEditLine aspect_el;
		WidgetEditLine znear_el;
		WidgetEditLine life_time_el;
		WidgetEditLine fade_time_el;
		WidgetEditLine min_visible_distance_el;
		WidgetEditLine max_visible_distance_el;
		WidgetEditLine min_fade_distance_el;
		WidgetEditLine max_fade_distance_el;
		WidgetEditLine texcoord_el[4];
		WidgetEditLine material_el;
		
		/*
		 */
		DecalTerrainProj decal;
		
		/*
		 */
		void init() {
			
			main_vb.replaceChild(main_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_gb);
			
			Nodes::Decal::init();
		}
		
		void shutdown() {
			
			decal = NULL;
			
			main_vb.replaceChild(dummy_vb,main_gb);
			
			Nodes::Decal::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Decal::decal_mask_el = decal_mask_el;
			Nodes::Decal::light_mask_el = light_mask_el;
			Nodes::Decal::viewport_mask_el = viewport_mask_el;
			Nodes::Decal::life_time_el = life_time_el;
			Nodes::Decal::fade_time_el = fade_time_el;
			Nodes::Decal::min_visible_distance_el = min_visible_distance_el;
			Nodes::Decal::max_visible_distance_el = max_visible_distance_el;
			Nodes::Decal::min_fade_distance_el = min_fade_distance_el;
			Nodes::Decal::max_fade_distance_el = max_fade_distance_el;
			Nodes::Decal::texcoord_el = texcoord_el;
			Nodes::Decal::material_el = material_el;
			
			Nodes::Decal::update();
			
			decal = node_cast(node);
			
			radius_el.setText(editorFormat(decal.getRadius()));
			fov_el.setText(editorFormat(decal.getFov()));
			aspect_el.setText(editorFormat(decal.getAspect()));
			znear_el.setText(editorFormat(decal.getZNear()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			Nodes::Decal::update_dynamic();
		}
		
		/*
		 */
		DECAL_PRESSED_CALLBACK_FLOAT(radius,decal,Radius)
		DECAL_PRESSED_CALLBACK_SLIDER(fov,decal,Fov)
		DECAL_PRESSED_CALLBACK_FLOAT(aspect,decal,Aspect)
		DECAL_PRESSED_CALLBACK_FLOAT(znear,decal,ZNear)
		
	} /* namespace TerrainProj */
	
	/**************************************************************************\
	*
	* TerrainOrtho
	*
	\**************************************************************************/
	
	/*
	 */
	namespace TerrainOrtho {
		
		/*
		 */
		WidgetGridBox main_gb;
		
		WidgetEditLine decal_mask_el;
		WidgetEditLine light_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine radius_el;
		WidgetEditLine width_el;
		WidgetEditLine height_el;
		WidgetEditLine znear_el;
		WidgetEditLine life_time_el;
		WidgetEditLine fade_time_el;
		WidgetEditLine min_visible_distance_el;
		WidgetEditLine max_visible_distance_el;
		WidgetEditLine min_fade_distance_el;
		WidgetEditLine max_fade_distance_el;
		WidgetEditLine texcoord_el[4];
		WidgetEditLine material_el;
		
		/*
		 */
		DecalTerrainOrtho decal;
		
		/*
		 */
		void init() {
			
			main_vb.replaceChild(main_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_gb);
			
			Nodes::Decal::init();
		}
		
		void shutdown() {
			
			decal = NULL;
			
			main_vb.replaceChild(dummy_vb,main_gb);
			
			Nodes::Decal::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Decal::decal_mask_el = decal_mask_el;
			Nodes::Decal::light_mask_el = light_mask_el;
			Nodes::Decal::viewport_mask_el = viewport_mask_el;
			Nodes::Decal::life_time_el = life_time_el;
			Nodes::Decal::fade_time_el = fade_time_el;
			Nodes::Decal::min_visible_distance_el = min_visible_distance_el;
			Nodes::Decal::max_visible_distance_el = max_visible_distance_el;
			Nodes::Decal::min_fade_distance_el = min_fade_distance_el;
			Nodes::Decal::max_fade_distance_el = max_fade_distance_el;
			Nodes::Decal::texcoord_el = texcoord_el;
			Nodes::Decal::material_el = material_el;
			
			Nodes::Decal::update();
			
			decal = node_cast(node);
			
			radius_el.setText(editorFormat(decal.getRadius()));
			width_el.setText(editorFormat(decal.getWidth()));
			height_el.setText(editorFormat(decal.getHeight()));
			znear_el.setText(editorFormat(decal.getZNear()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			Nodes::Decal::update_dynamic();
		}
		
		/*
		 */
		DECAL_PRESSED_CALLBACK_FLOAT(radius,decal,Radius)
		DECAL_PRESSED_CALLBACK_FLOAT(width,decal,Width)
		DECAL_PRESSED_CALLBACK_FLOAT(height,decal,Height)
		DECAL_PRESSED_CALLBACK_FLOAT(znear,decal,ZNear)
		
	} /* namespace TerrainProj */
	
	/**************************************************************************\
	*
	* DeferredProj
	*
	\**************************************************************************/
	
	/*
	 */
	namespace DeferredProj {
		
		/*
		 */
		WidgetGridBox main_gb;
		
		WidgetEditLine decal_mask_el;
		WidgetEditLine light_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine radius_el;
		WidgetEditLine fov_el;
		WidgetSlider fov_sl;
		WidgetEditLine aspect_el;
		WidgetEditLine znear_el;
		WidgetEditLine life_time_el;
		WidgetEditLine fade_time_el;
		WidgetEditLine min_visible_distance_el;
		WidgetEditLine max_visible_distance_el;
		WidgetEditLine min_fade_distance_el;
		WidgetEditLine max_fade_distance_el;
		WidgetEditLine texcoord_el[4];
		WidgetEditLine material_el;
		
		/*
		 */
		DecalDeferredProj decal;
		
		/*
		 */
		void init() {
			
			main_vb.replaceChild(main_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_gb);
			
			Nodes::Decal::init();
		}
		
		void shutdown() {
			
			decal = NULL;
			
			main_vb.replaceChild(dummy_vb,main_gb);
			
			Nodes::Decal::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Decal::decal_mask_el = decal_mask_el;
			Nodes::Decal::light_mask_el = light_mask_el;
			Nodes::Decal::viewport_mask_el = viewport_mask_el;
			Nodes::Decal::life_time_el = life_time_el;
			Nodes::Decal::fade_time_el = fade_time_el;
			Nodes::Decal::min_visible_distance_el = min_visible_distance_el;
			Nodes::Decal::max_visible_distance_el = max_visible_distance_el;
			Nodes::Decal::min_fade_distance_el = min_fade_distance_el;
			Nodes::Decal::max_fade_distance_el = max_fade_distance_el;
			Nodes::Decal::texcoord_el = texcoord_el;
			Nodes::Decal::material_el = material_el;
			
			Nodes::Decal::update();
			
			decal = node_cast(node);
			
			radius_el.setText(editorFormat(decal.getRadius()));
			fov_el.setText(editorFormat(decal.getFov()));
			aspect_el.setText(editorFormat(decal.getAspect()));
			znear_el.setText(editorFormat(decal.getZNear()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			Nodes::Decal::update_dynamic();
		}
		
		/*
		 */
		DECAL_PRESSED_CALLBACK_FLOAT(radius,decal,Radius)
		DECAL_PRESSED_CALLBACK_SLIDER(fov,decal,Fov)
		DECAL_PRESSED_CALLBACK_FLOAT(aspect,decal,Aspect)
		DECAL_PRESSED_CALLBACK_FLOAT(znear,decal,ZNear)
		
	} /* namespace DeferredProj */
	
	/**************************************************************************\
	*
	* DeferredOrtho
	*
	\**************************************************************************/
	
	/*
	 */
	namespace DeferredOrtho {
		
		/*
		 */
		WidgetGridBox main_gb;
		
		WidgetEditLine decal_mask_el;
		WidgetEditLine light_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine radius_el;
		WidgetEditLine width_el;
		WidgetEditLine height_el;
		WidgetEditLine znear_el;
		WidgetEditLine life_time_el;
		WidgetEditLine fade_time_el;
		WidgetEditLine min_visible_distance_el;
		WidgetEditLine max_visible_distance_el;
		WidgetEditLine min_fade_distance_el;
		WidgetEditLine max_fade_distance_el;
		WidgetEditLine texcoord_el[4];
		WidgetEditLine material_el;
		
		/*
		 */
		DecalDeferredOrtho decal;
		
		/*
		 */
		void init() {
			
			main_vb.replaceChild(main_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_gb);
			
			Nodes::Decal::init();
		}
		
		void shutdown() {
			
			decal = NULL;
			
			main_vb.replaceChild(dummy_vb,main_gb);
			
			Nodes::Decal::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Decal::decal_mask_el = decal_mask_el;
			Nodes::Decal::light_mask_el = light_mask_el;
			Nodes::Decal::viewport_mask_el = viewport_mask_el;
			Nodes::Decal::life_time_el = life_time_el;
			Nodes::Decal::fade_time_el = fade_time_el;
			Nodes::Decal::min_visible_distance_el = min_visible_distance_el;
			Nodes::Decal::max_visible_distance_el = max_visible_distance_el;
			Nodes::Decal::min_fade_distance_el = min_fade_distance_el;
			Nodes::Decal::max_fade_distance_el = max_fade_distance_el;
			Nodes::Decal::texcoord_el = texcoord_el;
			Nodes::Decal::material_el = material_el;
			
			Nodes::Decal::update();
			
			decal = node_cast(node);
			
			radius_el.setText(editorFormat(decal.getRadius()));
			width_el.setText(editorFormat(decal.getWidth()));
			height_el.setText(editorFormat(decal.getHeight()));
			znear_el.setText(editorFormat(decal.getZNear()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			Nodes::Decal::update_dynamic();
		}
		
		/*
		 */
		DECAL_PRESSED_CALLBACK_FLOAT(radius,decal,Radius)
		DECAL_PRESSED_CALLBACK_FLOAT(width,decal,Width)
		DECAL_PRESSED_CALLBACK_FLOAT(height,decal,Height)
		DECAL_PRESSED_CALLBACK_FLOAT(znear,decal,ZNear)
		
	} /* namespace DeferredOrtho */
	
	/**************************************************************************\
	*
	* DeferredMesh
	*
	\**************************************************************************/
	
	/*
	 */
	namespace DeferredMesh {
		
		/*
		 */
		WidgetGridBox main_gb;
		
		WidgetEditLine decal_mask_el;
		WidgetEditLine light_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine radius_el;
		WidgetEditLine life_time_el;
		WidgetEditLine fade_time_el;
		WidgetEditLine min_visible_distance_el;
		WidgetEditLine max_visible_distance_el;
		WidgetEditLine min_fade_distance_el;
		WidgetEditLine max_fade_distance_el;
		WidgetEditLine texcoord_el[4];
		WidgetEditLine material_el;
		WidgetEditLine mesh_file_el;
		
		/*
		 */
		DecalDeferredMesh decal;
		
		/*
		 */
		void init() {
			
			main_vb.replaceChild(main_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_gb);
			
			Nodes::Decal::init();
		}
		
		void shutdown() {
			
			decal = NULL;
			
			main_vb.replaceChild(dummy_vb,main_gb);
			
			Nodes::Decal::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Decal::decal_mask_el = decal_mask_el;
			Nodes::Decal::light_mask_el = light_mask_el;
			Nodes::Decal::viewport_mask_el = viewport_mask_el;
			Nodes::Decal::life_time_el = life_time_el;
			Nodes::Decal::fade_time_el = fade_time_el;
			Nodes::Decal::min_visible_distance_el = min_visible_distance_el;
			Nodes::Decal::max_visible_distance_el = max_visible_distance_el;
			Nodes::Decal::min_fade_distance_el = min_fade_distance_el;
			Nodes::Decal::max_fade_distance_el = max_fade_distance_el;
			Nodes::Decal::texcoord_el = texcoord_el;
			Nodes::Decal::material_el = material_el;
			
			Nodes::Decal::update();
			
			decal = node_cast(node);
			
			radius_el.setText(editorFormat(decal.getRadius()));
			mesh_file_el.setText(decal.getMeshName());
		}
		
		/*
		 */
		void update_dynamic() {
			
			Nodes::Decal::update_dynamic();
		}
		
		/*
		 */
		void load_clicked() {
			string name;
			if(dialogFileMesh(TR("Select mesh to load"),name,DIALOG_FILE_OPEN)) {
				if(decal.loadMesh(name)) decal.setMeshName(name);
				else dialogMessageOk(TR("Error"));
				
				mesh_file_el.setText(decal.getMeshName());
			}
		}
		
		/*
		 */
		void reload_clicked() {
			
			Node nodes[0];
			nodesGetNodes(nodes);
			
			string invalid_names[0] = ();
			
			int type = decal.getType();
			foreach(Node n; nodes) {
				if(n.getType() != type) continue;
				DecalDeferredMesh current_decal = node_cast(n);
				
				string mesh_name = current_decal.getMeshName();
				if(mesh_name == "") continue;
				
				if(current_decal.loadMesh(mesh_name) == 0) invalid_names.append(mesh_name);
			}
			
			if(invalid_names.size() > 0) {
				
				string warning_string;
				if(invalid_names.size() == 1) {
					warning_string = "Failed reloading mesh: " + invalid_names[0];
				} else {
					warning_string = "Failed reloading meshes:\n";
					foreach(string mesh_name; invalid_names) {
						warning_string += mesh_name + "\n";
					}
				}
				
				dialogMessageOk(TR("Error"),warning_string);
			}
		}
		
		void save_clicked() {
			string name;
			if(dialogFileMesh(TR("Select mesh to save"),name,DIALOG_FILE_SAVE)) {
				name = savePath(name);
				if(decal.saveMesh(name) == 0) dialogMessageOk(TR("Error"));
			}
		}
		
		void project_clicked() {
			
			// find terrain
			ObjectTerrain terrain = NULL;
			Node parent = decal.getParent();
			while(parent != NULL) {
				if(parent.getType() == NODE_OBJECT_TERRAIN) {
					terrain = node_cast(parent);
					break;
				}
				parent = parent.getParent();
			}
			
			// check terrain
			if(terrain == NULL) {
				dialogMessageOk(TR("Error"),TR("Can't find parent ObjectTerrain"));
				return;
			}
			
			// project decal pivot
			Vec3 terrain_pos = terrain.getPoint(decal.getWorldPosition());
			decal.setWorldPosition(terrain_pos);
			
			// get mesh
			Mesh mesh = new Mesh();
			decal.getMesh(mesh);
			
			// project decal
			forloop(int i = 0; mesh.getNumSurfaces()) {
				forloop(int j = 0; mesh.getNumVertex(i)) {
					Vec3 vertex = terrain.getPoint(decal.getWorldTransform() * mesh.getVertex(j,i));
					mesh.setVertex(j,Vec3(decal.getIWorldTransform() * vertex),i);
				}
			}
			
			// smooth normals
			mesh.createTangents();
			decal.setMesh(mesh);
			
			// update decal position
			decal.setWorldPosition(decal.getWorldPosition() + Vec3(0,0,decal.getRadius() / 2));
			
			// update decal
			decal.setMesh(mesh);
			delete mesh;
		}
		
		/*
		 */
		DECAL_PRESSED_CALLBACK_FLOAT(radius,decal,Radius)
		DECAL_PRESSED_CALLBACK_STRING(mesh_file,decal,MeshName)
		
	} /* namespace DeferredMesh */
	
} /* namespace Decal */
