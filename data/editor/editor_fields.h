/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_fields.h
 * Desc:    Unigine editor
 * Version: 1.02
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

/*
 */
namespace Field {
	
	/*
	 */
	WidgetEditLine field_mask_el;			// field mask
	WidgetEditLine viewport_mask_el;		// viewport mask
	
	Field field;							// base field node
	
	/*
	 */
	void update() {
		
		field = class_cast("Field",node);
		
		if(field_mask_el != NULL) field_mask_el.setText(editorFormatMask(field.getFieldMask()));
		if(viewport_mask_el != NULL) viewport_mask_el.setText(editorFormatMask(field.getViewportMask()));
	}
	
	/*
	 */
	void update_dynamic() {
		
		// show object info
		if(helpersGetNodeInfo()) {
			string info = run_function("get_info");
			engine.visualizer.renderMessage3D(field.getWorldTransform() * vec3_zero,vec3_zero,info,vec4_one,1);
		}
		
		// visualizer
		field.renderVisualizer();
	}
	
	/*
	 */
	PRESSED_CALLBACK_MASK(field_mask,field,FieldMask,field);
	CLICKED_CALLBACK_MASK(field_mask,field,FieldMask,field,"Field mask","Nodes::Field::field_mask_changed")
	PRESSED_CALLBACK_MASK(viewport_mask,field,ViewportMask,field);
	CLICKED_CALLBACK_MASK(viewport_mask,field,ViewportMask,field,"Viewport mask","Nodes::Field::viewport_mask_changed")
	
	/*
	 */
	void field_mask_changed(int mask) {
		field.setFieldMask(mask);
		field_mask_el.setText(editorFormatMask(field.getFieldMask()));
	}
	
	void viewport_mask_changed(int mask) {
		field.setViewportMask(mask);
		viewport_mask_el.setText(editorFormatMask(field.getViewportMask()));
	}
	
	/**************************************************************************\
	*
	* Spacer
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Spacer {
		
		/*
		 */
		WidgetVBox main_vb;
		
		WidgetCheckBox ellipse_cb;
		WidgetEditLine field_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine size_el[3];
		WidgetEditLine attenuation_el;
		
		FieldSpacer field;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Field"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Field::field_mask_el = field_mask_el;
			Nodes::Field::viewport_mask_el = viewport_mask_el;
			Nodes::Field::update();
			
			field = node_cast(node);
			
			ellipse_cb.setCallbackEnabled(GUI_CLICKED,0);
			
			ellipse_cb.setChecked(field.isEllipse());
			setEditLineVec3(size_el,field.getSize());
			attenuation_el.setText(editorFormat(field.getAttenuation()));
			
			ellipse_cb.setCallbackEnabled(GUI_CLICKED,1);
		}
		
		/*
		 */
		void update_dynamic() {
			Nodes::Field::update_dynamic();
		}
		
		/*
		 */
		CLICKED_CALLBACK(ellipse,field,Ellipse,field)
		PRESSED_CALLBACK_VEC3(size,field,Size,field,modify)
		PRESSED_CALLBACK_FLOAT(attenuation,field,Attenuation,field,modify)
		
	} /* namespace Spacer */
	
	/**************************************************************************\
	*
	* Animation
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Animation {
		
		/*
		 */
		WidgetVBox main_vb;
		
		WidgetCheckBox ellipse_cb;
		WidgetEditLine field_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine size_el[3];
		WidgetEditLine attenuation_el;
		WidgetEditLine stem_el;
		WidgetSlider stem_sl;
		WidgetEditLine leaf_el;
		WidgetSlider leaf_sl;
		WidgetEditLine scale_el;
		WidgetSlider scale_sl;
		WidgetEditLine wind_el[3];
		
		FieldAnimation field;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Field"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Field::field_mask_el = field_mask_el;
			Nodes::Field::viewport_mask_el = viewport_mask_el;
			Nodes::Field::update();
			
			field = node_cast(node);
			
			ellipse_cb.setCallbackEnabled(GUI_CLICKED,0);
			
			ellipse_cb.setChecked(field.isEllipse());
			setEditLineVec3(size_el,field.getSize());
			attenuation_el.setText(editorFormat(field.getAttenuation()));
			stem_el.setText(editorFormat(field.getStem()));
			leaf_el.setText(editorFormat(field.getLeaf()));
			scale_el.setText(editorFormat(field.getScale()));
			setEditLineVec3(wind_el,field.getWind());
			
			ellipse_cb.setCallbackEnabled(GUI_CLICKED,1);
		}
		
		/*
		 */
		void update_dynamic() {
			Nodes::Field::update_dynamic();
		}
		
		/*
		 */
		CLICKED_CALLBACK(ellipse,field,Ellipse,field)
		PRESSED_CALLBACK_VEC3(size,field,Size,field,modify)
		PRESSED_CALLBACK_FLOAT(attenuation,field,Attenuation,field,modify)
		PRESSED_CALLBACK_SLIDER(stem,field,Stem,field,modify)
		PRESSED_CALLBACK_SLIDER(leaf,field,Leaf,field,modify)
		PRESSED_CALLBACK_SLIDER(scale,field,Scale,field,modify)
		PRESSED_CALLBACK_VEC3(wind,field,Wind,field,modify)
		
	} /* namespace Animation */
	
} /* namespace Field */
