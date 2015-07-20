/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_objects_billboard.h
 * Desc:    Unigine editor
 * Version: 1.07
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
* ObjectBillboard
*
\******************************************************************************/

/*
 */
namespace Billboard {
	
	/*
	 */
	WidgetVBox main_vb;
	WidgetEditLine width_el;
	WidgetEditLine height_el;
	WidgetEditLine offset_el[3];
	WidgetEditLine texcoord_el[4];
	WidgetCheckBox axis_x_cb;
	WidgetCheckBox axis_z_cb;
	WidgetCheckBox screen_cb;
	
	ObjectBillboard billboard;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("Billboard"));
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
		
		billboard = node_cast(node);
		
		width_el.setText(editorFormat(billboard.getWidth()));
		height_el.setText(editorFormat(billboard.getHeight()));
		setEditLineVec3(offset_el,billboard.getOffset());
		setEditLineVec4(texcoord_el,billboard.getTexCoord());
		
		setCallbackEnabled(GUI_CLICKED,0,(
			axis_x_cb,
			axis_z_cb,
			screen_cb,
		));
		axis_x_cb.setChecked(billboard.getAxisX());
		axis_z_cb.setChecked(billboard.getAxisZ());
		screen_cb.setChecked(billboard.getScreen());
		setCallbackEnabled(GUI_CLICKED,1,(
			axis_x_cb,
			axis_z_cb,
			screen_cb,
		));
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
	}
	
	/*
	 */
	PRESSED_CALLBACK_VEC3(offset,billboard,Offset,billboard,modify)
	CLICKED_CALLBACK(axis_x,billboard,AxisX,billboard)
	CLICKED_CALLBACK(axis_z,billboard,AxisZ,billboard)
	CLICKED_CALLBACK(screen,billboard,Screen,billboard)
	PRESSED_CALLBACK_SIZE(width,height,billboard,Size,Width,Height,float,editorFormat)
	
	/*
	 */
	void texcoord_pressed() {
		NodesState_ s = new NodesState_();
		if(billboard.getTexCoord() != getEditLineVec4(texcoord_el)) s.init_modify(nodes,billboard);
		billboard.setTexCoord(getEditLineVec4(texcoord_el));
		setEditLineVec4(texcoord_el,billboard.getTexCoord());
		save_state(s);
	}
	
} /* namespace Billboard */
