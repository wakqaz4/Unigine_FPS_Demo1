/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_objects_water.h
 * Desc:    Unigine editor
 * Version: 1.06
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

/*
 */
void set_wave(WidgetEditLine wave_el[],vec4 wave) {
	
	float height = wave.w;
	float frequency = wave.z;
	float speed = frequency / PI2;
	float length = PI2 / max(0.001f,sqrt(wave.x * wave.x + wave.y * wave.y) * frequency);
	float angle = atan2(wave.x,wave.y) * RAD2DEG;
	
	setEditLineVec4(wave_el,vec4(angle,speed,length,height));
}

vec4 get_wave(WidgetEditLine wave_el[]) {
	
	float angle = float(wave_el[0].getText()) * DEG2RAD;
	float speed = float(wave_el[1].getText());
	float length = float(wave_el[2].getText());
	float height = float(wave_el[3].getText());
	float frequency = PI2 * speed;
	
	vec3 direction = vec3(sin(angle),cos(angle),0.0f);
	
	direction = direction * PI2 / max(0.001f,length * frequency);
	
	return vec4(direction.x,direction.y,frequency,height);
}

/*
 */
#define WATER_PRESSED(NUM) \
void wave_ ## NUM ## _pressed() { \
	NodesState_ s = new NodesState_(); \
	vec4 wave = get_wave(wave_ ## NUM ## _el); \
	if(water.getWave(NUM) != wave) { \
		s.init_modify(nodes,water); \
		water.setWave(NUM,wave); \
	} \
	set_wave(wave_ ## NUM ## _el,wave); \
	save_state(s); \
}

/******************************************************************************\
*
* ObjectWater
*
\******************************************************************************/

/*
 */
namespace Water {
	
	/*
	 */
	WidgetVBox main_vb;
	
	WidgetEditLine field_mask_el;
	
	WidgetEditLine wave_0_el[4];
	WidgetEditLine wave_1_el[4];
	WidgetEditLine wave_2_el[4];
	WidgetEditLine wave_3_el[4];
	
	ObjectWater water;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("Water"));
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
		
		water = node_cast(node);
		
		field_mask_el.setText(editorFormatMask(water.getFieldMask()));
		
		set_wave(wave_0_el,water.getWave(0));
		set_wave(wave_1_el,water.getWave(1));
		set_wave(wave_2_el,water.getWave(2));
		set_wave(wave_3_el,water.getWave(3));
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
	}
	
	/*
	 */
	void field_mask_changed(int mask) {
		water.setFieldMask(mask);
		field_mask_el.setText(editorFormatMask(water.getFieldMask()));
	}
	
	/*
	 */
	PRESSED_CALLBACK_MASK(field_mask,water,FieldMask,water);
	CLICKED_CALLBACK_MASK(field_mask,water,FieldMask,water,TR("Field mask"),"Nodes::Object::Water::field_mask_changed")
	WATER_PRESSED(0)
	WATER_PRESSED(1)
	WATER_PRESSED(2)
	WATER_PRESSED(3)
	
} /* namespace Water */

/******************************************************************************\
*
* ObjectWaterMesh
*
\******************************************************************************/

/*
 */
namespace WaterMesh {
	
	/*
	 */
	WidgetVBox main_vb;
	
	WidgetEditLine mesh_name_el;
	WidgetEditLine field_mask_el;
	
	WidgetEditLine wave_0_el[4];
	WidgetEditLine wave_1_el[4];
	WidgetEditLine wave_2_el[4];
	WidgetEditLine wave_3_el[4];
	
	ObjectWaterMesh water;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("Water"));
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
		
		Nodes::Object::Body::init();
		Nodes::Object::Body::update();
		
		water = node_cast(node);
		
		mesh_name_el.setText(water.getMeshName());
		field_mask_el.setText(editorFormatMask(water.getFieldMask()));
		
		set_wave(wave_0_el,water.getWave(0));
		set_wave(wave_1_el,water.getWave(1));
		set_wave(wave_2_el,water.getWave(2));
		set_wave(wave_3_el,water.getWave(3));
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
		
		Body::update_dynamic();
	}
	
	/*
	 */
	void load_clicked() {
		string name;
		if(dialogFileMesh(TR("Select mesh to load"),name,DIALOG_FILE_OPEN)) {
			if(water.loadMesh(name) == 0) dialogMessageOk(TR("Error"));
		}
	}
	
	void save_clicked() {
		string name;
		if(dialogFileMesh(TR("Select mesh to save"),name,DIALOG_FILE_SAVE)) {
			if(water.saveMesh(name) == 0) dialogMessageOk(TR("Error"));
		}
	}
	
	/*
	 */
	void mesh_name_pressed() {
		water.setMeshName(mesh_name_el.getText());
	}
	
	void mesh_load_clicked() {
		string name = water.getMeshName();
		if(dialogFileMesh(TR("Select mesh to store"),name,DIALOG_FILE_OPEN)) {
			mesh_name_el.setText(name);
			water.setMeshName(name);
		}
	}
	
	/*
	 */
	void field_mask_changed(int mask) {
		water.setFieldMask(mask);
		field_mask_el.setText(editorFormatMask(water.getFieldMask()));
	}
	
	/*
	 */
	PRESSED_CALLBACK_MASK(field_mask,water,FieldMask,water);
	CLICKED_CALLBACK_MASK(field_mask,water,FieldMask,water,TR("Field mask"),"Nodes::Object::WaterMesh::field_mask_changed")
	WATER_PRESSED(0)
	WATER_PRESSED(1)
	WATER_PRESSED(2)
	WATER_PRESSED(3)
	
} /* namespace WaterMesh */
