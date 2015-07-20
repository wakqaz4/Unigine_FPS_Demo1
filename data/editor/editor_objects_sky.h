/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_objects_sky.h
 * Desc:    Unigine editor
 * Version: 1.03
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

/******************************************************************************\
*
* ObjectSky
*
\******************************************************************************/

/*
 */
namespace Sky {
	
	/*
	 */
	WidgetVBox main_vb;
	
	WidgetCheckBox spherical_cb;
	WidgetCheckBox simulation_cb;
	
	WidgetEditLine size_el[3];
	
	WidgetEditLine min_slices_el;
	WidgetEditLine max_slices_el;
	WidgetEditLine distribute_el;
	WidgetSlider distribute_sl;
	
	WidgetEditLine density_image_el;
	WidgetEditLine density_velocity_el[3];
	WidgetEditLine density_layer_el;
	WidgetSlider density_layer_sl;
	
	WidgetEditLine humidity_el;
	WidgetSlider humidity_sl;
	WidgetEditLine transition_el;
	WidgetSlider transition_sl;
	WidgetEditLine extinction_el;
	WidgetSlider extinction_sl;
	
	ObjectSky sky;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("Parameters"));
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
		
		sky = node_cast(node);
		
		setEnabled(0,(
			spherical_cb,
			simulation_cb,
		));
		
		spherical_cb.setChecked(sky.isSpherical());
		simulation_cb.setChecked(sky.getSimulation());
		
		setEditLineVec3(size_el,sky.getSize());
		
		min_slices_el.setText(string(sky.getMinSlices()));
		max_slices_el.setText(string(sky.getMaxSlices()));
		distribute_el.setText(editorFormat(sky.getDistribute()));
		
		density_image_el.setText(sky.getDensityImageName());
		setEditLineVec3(density_velocity_el,sky.getDensityVelocity());
		density_layer_el.setText(string(sky.getDensityLayer()));
		
		humidity_el.setText(editorFormat(sky.getHumidity()));
		transition_el.setText(editorFormat(sky.getTransition()));
		extinction_el.setText(editorFormat(sky.getExtinction()));
		
		setEnabled(1,(
			spherical_cb,
			simulation_cb,
		));
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
	}
	
	/*
	 */
	CLICKED_CALLBACK(spherical,sky,Spherical,sky)
	CLICKED_CALLBACK(simulation,sky,Simulation,sky)
	PRESSED_CALLBACK_VEC3(size,sky,Size,sky,modify)
	PRESSED_CALLBACK(min_slices,sky,MinSlices,sky,int,modify)
	PRESSED_CALLBACK(max_slices,sky,MaxSlices,sky,int,modify)
	PRESSED_CALLBACK_SLIDER(distribute,sky,Distribute,sky,modify)
	PRESSED_CALLBACK(density_image,sky,DensityImageName,sky,string,modify)
	LOAD_CLICKED_CALLBACK(density_image,sky,DensityImageName,TR("Select density image"))
	VIEW_CLICKED_CALLBACK(density_image,sky,DensityImageName,TR("Clouds density image"))
	CLEAR_CLICKED_CALLBACK(density_image,sky,DensityImageName,TR("Clear density image?"),NULL,"")
	PRESSED_CALLBACK_VEC3(density_velocity,sky,DensityVelocity,sky,modify)
	PRESSED_CALLBACK_SLIDER_INT(density_layer,sky,DensityLayer,sky,modify)
	PRESSED_CALLBACK_SLIDER(humidity,sky,Humidity,sky,modify)
	PRESSED_CALLBACK_SLIDER(transition,sky,Transition,sky,modify)
	PRESSED_CALLBACK_SLIDER(extinction,sky,Extinction,sky,modify)
	
} /* namespace Sky */
