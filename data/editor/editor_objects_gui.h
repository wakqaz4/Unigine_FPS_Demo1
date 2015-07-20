/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_objects_gui.h
 * Desc:    Unigine editor
 * Version: 1.13
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
* Gui
*
\******************************************************************************/

/*
 */
namespace Gui {
	
	/*
	 */
	WidgetVBox main_vb;
	WidgetCheckBox billboard_cb;
	WidgetCheckBox background_cb;
	WidgetCheckBox depth_test_cb;
	WidgetCheckBox mouse_show_cb;
	WidgetEditLine control_distance_el;
	WidgetEditLine polygon_offset_el;
	WidgetEditLine physical_width_el;
	WidgetEditLine physical_height_el;
	WidgetEditLine screen_width_el;
	WidgetEditLine screen_height_el;
	
	ObjectGui gui;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("Gui"));
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
		
		gui = node_cast(node);
		
		setCallbackEnabled(GUI_CLICKED,0,(
			billboard_cb,
			background_cb,
			depth_test_cb,
			mouse_show_cb,
		));
		
		billboard_cb.setChecked(gui.isBillboard());
		background_cb.setChecked(gui.getBackground());
		depth_test_cb.setChecked(gui.getDepthTest());
		mouse_show_cb.setChecked(gui.getMouseShow());
		
		setCallbackEnabled(GUI_CLICKED,1,(
			billboard_cb,
			background_cb,
			depth_test_cb,
			mouse_show_cb,
		));
		
		control_distance_el.setText(editorFormat(gui.getControlDistance()));
		polygon_offset_el.setText(editorFormat(gui.getPolygonOffset()));
		physical_width_el.setText(editorFormat(gui.getPhysicalWidth()));
		physical_height_el.setText(editorFormat(gui.getPhysicalHeight()));
		screen_width_el.setText(string(gui.getScreenWidth()));
		screen_height_el.setText(string(gui.getScreenHeight()));
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
	}
	
	/*
	 */
	CLICKED_CALLBACK(billboard,gui,Billboard,gui)
	CLICKED_CALLBACK(background,gui,Background,gui)
	CLICKED_CALLBACK(depth_test,gui,DepthTest,gui)
	CLICKED_CALLBACK(mouse_show,gui,MouseShow,gui)
	PRESSED_CALLBACK_FLOAT(control_distance,gui,ControlDistance,gui,modify)
	PRESSED_CALLBACK_FLOAT(polygon_offset,gui,PolygonOffset,gui,modify)
	PRESSED_CALLBACK_SIZE(physical_width,physical_height,gui,PhysicalSize,PhysicalWidth,PhysicalHeight,float,editorFormat)
	PRESSED_CALLBACK_SIZE(screen_width,screen_height,gui,ScreenSize,ScreenWidth,ScreenHeight,int,string)
	
} /* namespace Gui */

/******************************************************************************\
*
* GuiMesh
*
\******************************************************************************/

/*
 */
namespace GuiMesh {
	
	/*
	 */
	WidgetVBox main_vb;
	WidgetCheckBox mouse_show_cb;
	WidgetEditLine screen_width_el;
	WidgetEditLine screen_height_el;
	WidgetEditLine control_distance_el;
	
	ObjectGuiMesh gui;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("Gui"));
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
		
		gui = node_cast(node);
		
		setCallbackEnabled(GUI_CLICKED,0,(
			mouse_show_cb,
		));
		
		mouse_show_cb.setChecked(gui.getMouseShow());
		
		setCallbackEnabled(GUI_CLICKED,1,(
			mouse_show_cb,
		));
		
		control_distance_el.setText(editorFormat(gui.getControlDistance()));
		screen_width_el.setText(string(gui.getScreenWidth()));
		screen_height_el.setText(string(gui.getScreenHeight()));
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
	}
	
	/*
	 */
	CLICKED_CALLBACK(mouse_show,gui,MouseShow,gui)
	PRESSED_CALLBACK_FLOAT(control_distance,gui,ControlDistance,gui,modify)
	PRESSED_CALLBACK_SIZE(screen_width,screen_height,gui,ScreenSize,ScreenWidth,ScreenHeight,int,string)
	
} /* namespace GuiMesh */
