/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_objects_volumes.h
 * Desc:    Unigine editor
 * Version: 1.08
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
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
* ObjectVolumeBox
*
\******************************************************************************/

/*
 */
namespace VolumeBox {
	
	/*
	 */
	WidgetVBox main_vb;
	WidgetEditLine size_el[3];
	
	ObjectVolumeBox volume;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("Volume"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
		Nodes::Object::shutdown();
	}
	
	/*
	 */
	void update() {
		
		Nodes::Object::update();
		
		volume = node_cast(node);
		
		setEditLineVec3(size_el,volume.getSize());
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
	}
	
	/*
	 */
	PRESSED_CALLBACK_VEC3(size,volume,Size,volume,modify)
	
} /* namespace VolumeBox */

/******************************************************************************\
*
* ObjectVolumeSphere
*
\******************************************************************************/

/*
 */
namespace VolumeSphere {
	
	/*
	 */
	WidgetVBox main_vb;
	WidgetEditLine radius_el[3];
	
	ObjectVolumeSphere volume;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("Volume"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
		Nodes::Object::shutdown();
	}
	
	/*
	 */
	void update() {
		
		Nodes::Object::update();
		
		volume = node_cast(node);
		
		setEditLineSameVec3(radius_el,volume.getRadius());
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
	}
	
	/*
	 */
	PRESSED_CALLBACK_VEC3_SAME(radius,volume,Radius,volume,modify)
	
	/*
	 */
	void radius_enter(string axis) {
		vec3 radius = volume.getRadius();
		if(axis == "y" && radius_el[1].getText() == "same") radius_el[1].setText(editorFormat(radius.x));
		if(axis == "z" && radius_el[2].getText() == "same") radius_el[2].setText(editorFormat(radius.x));
	}
	
	void radius_leave(string axis) {
		vec3 radius = volume.getRadius();
		if(axis == "y" && radius_el[1].isFocused() == 0 && radius.y == radius.x) radius_el[1].setText("same");
		if(axis == "z" && radius_el[2].isFocused() == 0 && radius.z == radius.x) radius_el[2].setText("same");
	}
	
} /* namespace VolumeSphere */

/******************************************************************************\
*
* ObjectVolumeOmni
*
\******************************************************************************/

/*
 */
namespace VolumeOmni {
	
	/*
	 */
	WidgetVBox main_vb;
	WidgetEditLine width_el;
	WidgetEditLine height_el;
	WidgetEditLine radius_el;
	WidgetEditLine attenuation_el;
	
	ObjectVolumeOmni volume;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("Volume"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
		Nodes::Object::shutdown();
	}
	
	/*
	 */
	void update() {
		
		Nodes::Object::update();
		
		volume = node_cast(node);
		
		width_el.setText(editorFormat(volume.getWidth()));
		height_el.setText(editorFormat(volume.getHeight()));
		radius_el.setText(editorFormat(volume.getRadius()));
		attenuation_el.setText(editorFormat(volume.getAttenuation()));
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
	}
	
	/*
	 */
	PRESSED_CALLBACK_SIZE(width,height,volume,Size,Width,Height,float,editorFormat)
	PRESSED_CALLBACK_FLOAT(radius,volume,Radius,volume,modify)
	PRESSED_CALLBACK_FLOAT(attenuation,volume,Attenuation,volume,modify)
	
} /* namespace VolumeOmni */

/******************************************************************************\
*
* ObjectVolumeProj
*
\******************************************************************************/

/*
 */
namespace VolumeProj {
	
	/*
	 */
	WidgetVBox main_vb;
	WidgetEditLine size_el;
	WidgetEditLine radius_el;
	WidgetEditLine fov_el;
	WidgetSlider fov_sl;
	WidgetEditLine step_el;
	WidgetSlider step_sl;
	WidgetEditLine velocity_el;
	WidgetEditLine rotation_el;
	
	ObjectVolumeProj volume;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("Volume"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
		Nodes::Object::shutdown();
	}
	
	/*
	 */
	void update() {
		
		Nodes::Object::update();
		
		volume = node_cast(node);
		
		size_el.setText(editorFormat(volume.getSize()));
		radius_el.setText(editorFormat(volume.getRadius()));
		fov_el.setText(editorFormat(volume.getFov()));
		step_el.setText(editorFormat(volume.getStep()));
		velocity_el.setText(editorFormat(volume.getVelocity()));
		rotation_el.setText(editorFormat(volume.getRotation()));
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
	}
	
	/*
	 */
	PRESSED_CALLBACK_FLOAT(size,volume,Size,volume,modify)
	PRESSED_CALLBACK_FLOAT(radius,volume,Radius,volume,modify)
	PRESSED_CALLBACK_SLIDER(fov,volume,Fov,volume,modify)
	PRESSED_CALLBACK_SLIDER(step,volume,Step,volume,modify)
	PRESSED_CALLBACK_FLOAT(velocity,volume,Velocity,volume,modify)
	PRESSED_CALLBACK_FLOAT(rotation,volume,Rotation,volume,modify)
	
} /* namespace VolumeProj */
