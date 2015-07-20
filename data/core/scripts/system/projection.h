/* Copyright (C) 2005-2015, UNIGINE Corp. All rights reserved.
 *
 * This file is a part of the UNIGINE 2.0-RC SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE Corp. at http://unigine.com/
 */


#ifndef __UNIGINE_SYSTEM_PROJECTION_H__
#define __UNIGINE_SYSTEM_PROJECTION_H__

#include <core/systems/widgets/widget.h>
#include <core/systems/widgets/widget_window.h>
#include <core/systems/widgets/widget_dialog_file.h>
#include <core/systems/widgets/widget_dialog_message.h>
#include <core/systems/widgets/widget_vbox.h>
#include <core/systems/widgets/widget_hbox.h>
#include <core/systems/widgets/widget_gridbox.h>
#include <core/systems/widgets/widget_groupbox.h>
#include <core/systems/widgets/widget_label.h>
#include <core/systems/widgets/widget_combobox.h>
#include <core/systems/widgets/widget_editline.h>
#include <core/systems/widgets/widget_spinbox.h>
#include <core/systems/widgets/widget_deform.h>

/*
 */
namespace Projection {
	
	/*
	 */
	using Unigine::Widgets;
	
	/*
	 */
	Window window;						// window
	
	ComboBox grid_mode_cb;				// grid mode combobox
	ComboBox grid_step_cb;				// grid step combobox
	
	EditLine gamma_el;					// gamma editline
	ComboBox angle_cb;					// angle combobox
	ComboBox display_cb;				// display combobox
	Deform deform;						// deform widget
	
	EditLine point_0_el[2];				// top-left point editlines
	EditLine point_1_el[2];				// top-right point editlines
	EditLine point_2_el[2];				// bottom-right point editlines
	EditLine point_3_el[2];				// bottom-left point editlines
	EditLine handle_0_el[4];			// top-left handle editlines
	EditLine handle_1_el[4];			// top-right handle editlines
	EditLine handle_2_el[4];			// bottom-right handle editlines
	EditLine handle_3_el[4];			// bottom-left handle editlines
	
	EditLine border_0_el[3];			// top border editlines
	EditLine border_1_el[3];			// right border editlines
	EditLine border_2_el[3];			// bottom border editlines
	EditLine border_3_el[3];			// left bottom editlines
	
	EditLine linearity_0_el[2];			// top linearity editlines
	EditLine linearity_1_el[2];			// right linearity editlines
	EditLine linearity_2_el[2];			// bottom linearity editlines
	EditLine linearity_3_el[2];			// left bottom editlines
	
	EditLine color_scale_el[4];			// color scale editlines
	EditLine color_bias_el[4];			// color bias editlines
	
	ComboBox auxiliary_cb;				// auxiliary combobox
	
	Button load_b;						// load button
	Button save_b;						// save button
	Button clear_b;						// clear button
	Button apply_b;						// apply button
	Button cancel_b;					// cancel button
	
	int enabled;						// window enabled
	int result;							// window result
	
	int display;						// current display
	Display displays[0];				// displays
	
	Grid grid;							// grid
	
	string projection_name;				// projection name
	
	string changed_callback;			// changed callback
	string changed_world_callback;		// changed world callback
	string changed_editor_callback;		// changed editor callback
	
	/**************************************************************************\
	*
	* Display
	*
	\**************************************************************************/
	
	/*
	 */
	class Display {
		
		int index;						// index
		vec3 points[4];					// points
		vec4 handles[4];				// handles
		vec3 borders[4];				// borders
		vec3 linearity[4];				// linearity
		vec4 color_scale;				// color scale
		vec4 color_bias;				// color bias
		
		/*
		 */
		Display(int i) {
			if(i is int) {
				index = i;
			} else {
				Display display = i;
				index = display.index;
				points.copy(display.points);
				handles.copy(display.handles);
				borders.copy(display.borders);
				linearity.copy(display.linearity);
				color_scale = display.color_scale;
				color_bias = display.color_bias;
			}
		}
		
		/*
		 */
		void load() {
			strsplit(engine.config.getString("projection_points_" + index,"-1 -1 0, 1 -1 0, 1 1 0, -1 1 0"),",",points);
			strsplit(engine.config.getString("projection_handles_" + index,"0.5 0 -0.5 0, 0 0.5 0 -0.5, -0.5 0 0.5 0, 0 -0.5 0 0.5"),",",handles);
			strsplit(engine.config.getString("projection_borders_" + index,"0 0 1, 0 0 1, 0 0 1, 0 0 1"),",",borders);
			strsplit(engine.config.getString("projection_linearity_" + index,"1 1 0, 1 1 0, 1 1 0, 1 1 0"),",",linearity);
			color_scale = vec4(engine.config.getString("projection_color_scale_" + index,"1 1 1 1"));
			color_bias = vec4(engine.config.getString("projection_color_bias_" + index,"0 0 0 0"));
			forloop(int i = 0; 4) {
				points[i] = vec3(points[i]);
				handles[i] = vec4(handles[i]);
				borders[i] = vec3(borders[i]);
				linearity[i] = vec3(linearity[i]);
			}
		}
		void save() {
			string points_str = string(points[0]);
			string handles_str = string(handles[0]);
			string borders_str = string(borders[0]);
			string linearity_str = string(linearity[0]);
			forloop(int i = 1; 4) {
				points_str += ", " + string(points[i]);
				handles_str += ", " + string(handles[i]);
				borders_str += ", " + string(borders[i]);
				linearity_str += ", " + string(linearity[i]);
			}
			engine.config.setString("projection_points_" + index,points_str);
			engine.config.setString("projection_handles_" + index,handles_str);
			engine.config.setString("projection_borders_" + index,borders_str);
			engine.config.setString("projection_linearity_" + index,linearity_str);
			engine.config.setString("projection_color_scale_" + index,string(color_scale));
			engine.config.setString("projection_color_bias_" + index,string(color_bias));
		}
		
		/*
		 */
		void load(Xml xml) {
			forloop(int i = 0; 4) {
				points[i] = xml.getChildData("point_" + i,points[i]);
				handles[i] = xml.getChildData("handle_" + i,handles[i]);
				borders[i] = xml.getChildData("border_" + i,borders[i]);
				linearity[i] = xml.getChildData("linearity_" + i,linearity[i]);
			}
			color_scale = xml.getChildData("color_scale",color_scale);
			color_bias = xml.getChildData("color_bias",color_bias);
		}
		
		void save(Xml xml) {
			forloop(int i = 0; 4) {
				xml.setChildData("point_" + i,points[i]);
				xml.setChildData("handle_" + i,handles[i]);
				xml.setChildData("border_" + i,borders[i]);
				xml.setChildData("linearity_" + i,linearity[i]);
			}
			xml.setChildData("color_scale",color_scale);
			xml.setChildData("color_bias",color_bias);
		}
		
		/*
		 */
		void set() {
			engine.projection.setPoints(index,points);
			engine.projection.setHandles(index,handles);
			engine.projection.setBorders(index,borders);
			engine.projection.setLinearity(index,linearity);
			engine.projection.setColorScale(index,color_scale);
			engine.projection.setColorBias(index,color_bias);
			changed();
		}
		
		void get() {
			engine.projection.getPoints(index,points);
			engine.projection.getHandles(index,handles);
			engine.projection.getBorders(index,borders);
			engine.projection.getLinearity(index,linearity);
			color_scale = engine.projection.getColorScale(index);
			color_bias = engine.projection.getColorBias(index);
		}
		
		/*
		 */
		void setDeform() {
			deform.setPoints(points);
			deform.setHandles(handles);
		}
		void getDeform() {
			deform.getPoints(points);
			deform.getHandles(handles);
			engine.projection.setPoints(index,points);
			engine.projection.setHandles(index,handles);
			setPoints();
			changed();
		}
		
		/*
		 */
		void setPoints() {
			set_vec2(point_0_el,points[0]);
			set_vec2(point_1_el,points[1]);
			set_vec2(point_2_el,points[2]);
			set_vec2(point_3_el,points[3]);
			set_vec4(handle_0_el,handles[0]);
			set_vec4(handle_1_el,handles[1]);
			set_vec4(handle_2_el,handles[2]);
			set_vec4(handle_3_el,handles[3]);
		}
		void getPoints() {
			vec4 vec4_two = vec4(2.0f);
			points[0] = clamp(get_vec2(point_0_el),-vec3_one,vec3_one);
			points[1] = clamp(get_vec2(point_1_el),-vec3_one,vec3_one);
			points[2] = clamp(get_vec2(point_2_el),-vec3_one,vec3_one);
			points[3] = clamp(get_vec2(point_3_el),-vec3_one,vec3_one);
			handles[0] = clamp(get_vec4(handle_0_el),-vec4_two,vec4_two);
			handles[1] = clamp(get_vec4(handle_1_el),-vec4_two,vec4_two);
			handles[2] = clamp(get_vec4(handle_2_el),-vec4_two,vec4_two);
			handles[3] = clamp(get_vec4(handle_3_el),-vec4_two,vec4_two);
			engine.projection.setPoints(index,points);
			engine.projection.setHandles(index,handles);
			setDeform();
			changed();
		}
		
		/*
		 */
		void setBorder() {
			set_vec3(border_0_el,borders[0]);
			set_vec3(border_1_el,borders[1]);
			set_vec3(border_2_el,borders[2]);
			set_vec3(border_3_el,borders[3]);
		}
		void getBorder() {
			borders[0] = get_vec3(border_0_el);
			borders[1] = get_vec3(border_1_el);
			borders[2] = get_vec3(border_2_el);
			borders[3] = get_vec3(border_3_el);
			engine.projection.setBorders(index,borders);
			changed();
		}
		
		/*
		 */
		void setLinearity() {
			set_vec2(linearity_0_el,linearity[0]);
			set_vec2(linearity_1_el,linearity[1]);
			set_vec2(linearity_2_el,linearity[2]);
			set_vec2(linearity_3_el,linearity[3]);
		}
		void getLinearity() {
			linearity[0] = get_vec2(linearity_0_el);
			linearity[1] = get_vec2(linearity_1_el);
			linearity[2] = get_vec2(linearity_2_el);
			linearity[3] = get_vec2(linearity_3_el);
			engine.projection.setLinearity(index,linearity);
			changed();
		}
		
		/*
		 */
		void setColor() {
			set_vec4(color_scale_el,color_scale);
			set_vec4(color_bias_el,color_bias);
		}
		void getColor() {
			color_scale = get_vec4(color_scale_el);
			color_bias = get_vec4(color_bias_el);
			engine.projection.setColorScale(index,color_scale);
			engine.projection.setColorBias(index,color_bias);
			changed();
		}
	};
	
	/**************************************************************************\
	*
	* Grid
	*
	\**************************************************************************/
	
	/*
	 */
	class Grid {
		
		WidgetCanvas canvas[0];
		
		/*
		 */
		Grid() {
			forloop(int i = 0; engine.projection.getWidth()) {
				canvas.append(new WidgetCanvas(engine.getGui()));
				canvas[i].setWidth(engine.app.getWidth());
				canvas[i].setHeight(engine.app.getHeight());
			}
		}
		~Grid() {
			canvas.delete();
		}
		
		/*
		 */
		void clear() {
			forloop(int i = 0; canvas.size()) {
				canvas[i].clear();
			}
		}
		
		/*
		 */
		void set(int mode,int step) {
			forloop(int i = 0; canvas.size()) {
				canvas[i].clear();
				if(mode == 1 || mode == 3) {
					int center_x = engine.app.getWidth() / 2;
					int bezel_x = int(engine.app.getWidth() * wallGetBezelX());
					int step_x = max(step * (engine.app.getWidth() + bezel_x) / engine.app.getWidth(),1);
					forloop(int x = 0; engine.app.getWidth() + bezel_x; step_x) {
						int line_0 = canvas[i].addLine();
						int line_1 = canvas[i].addLine();
						canvas[i].addLinePoint(line_0,vec3(center_x + x,0.0f,0.0f));
						canvas[i].addLinePoint(line_1,vec3(center_x - x,0.0f,0.0f));
						canvas[i].addLinePoint(line_0,vec3(center_x + x,engine.app.getHeight(),0.0f));
						canvas[i].addLinePoint(line_1,vec3(center_x - x,engine.app.getHeight(),0.0f));
					}
				}
				if(mode == 2 || mode == 3) {
					int center_y = engine.app.getHeight() / 2;
					int bezel_y = int(engine.app.getHeight() * wallGetBezelY());
					int step_y = max(step * (engine.app.getHeight() + bezel_y) / engine.app.getHeight(),1);
					forloop(int y = 0; engine.app.getHeight() + bezel_y; step_y) {
						int line_0 = canvas[i].addLine();
						int line_1 = canvas[i].addLine();
						canvas[i].addLinePoint(line_0,vec3(0.0f,center_y + y,0.0f));
						canvas[i].addLinePoint(line_1,vec3(0.0f,center_y - y,0.0f));
						canvas[i].addLinePoint(line_0,vec3(engine.app.getWidth(),center_y + y,0.0f));
						canvas[i].addLinePoint(line_1,vec3(engine.app.getWidth(),center_y - y,0.0f));
					}
				}
				if(mode) {
					Gui gui = engine.projection.getGui(i);
					gui.addChild(canvas[i],ALIGN_OVERLAP | ALIGN_BACKGROUND);
				}
			}
		}
	};
	
	/**************************************************************************\
	*
	* Projection
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		// projection name
		projection_name = engine.config.getString("projection_name",engine.getDataPath());
		
		// create displays
		forloop(int i = 0; engine.projection.getWidth()) {
			displays.append(new Display(i));
			displays[i].load();
			displays[i].set();
		}
		
		// create grid
		grid = new Grid();
		
		// window
		window = new Window("Projection");
		window.setSizeable(1);
		
		// top panel
		VBox vbox = new VBox();
		HBox hbox = new HBox();
		window.addChild(vbox);
		vbox.addChild(hbox,ALIGN_EXPAND);
		
		// grid mode
		grid_mode_cb = new ComboBox();
		grid_mode_cb.addItem("None");
		grid_mode_cb.addItem("Vertical");
		grid_mode_cb.addItem("Horizontal");
		grid_mode_cb.addItem("Vert+Horiz");
		grid_mode_cb.setCurrentItem(engine.config.getInt("projection_grid_mode",0));
		grid_mode_cb.setCallback(CHANGED,"Projection::grid_changed");
		hbox.addChild(new Label("Grid: "),ALIGN_LEFT);
		hbox.addChild(grid_mode_cb,ALIGN_LEFT);
		
		// grid step
		grid_step_cb = new ComboBox();
		grid_step_cb.addItem("16");
		grid_step_cb.addItem("32");
		grid_step_cb.addItem("64");
		grid_step_cb.addItem("128");
		grid_step_cb.addItem("256");
		grid_step_cb.setCurrentItem(engine.config.getInt("projection_grid_step",2));
		grid_step_cb.setCallback(CHANGED,"Projection::grid_changed");
		hbox.addChild(new Label(" Step: "),ALIGN_LEFT);
		hbox.addChild(grid_step_cb,ALIGN_LEFT);
		
		// gamma
		gamma_el = new EditLine();
		gamma_el.setCallback(PRESSED,"Projection::gamma_pressed");
		gamma_el.setCallback(FOCUS_OUT,"Projection::gamma_pressed");
		engine.projection.setGamma((engine.config.getFloat("projection_gamma",2.2f)));
		gamma_el.setText(format(engine.projection.getGamma()));
		hbox.addChild(new Label(" Gamma: "),ALIGN_RIGHT);
		hbox.addChild(gamma_el,ALIGN_RIGHT);
		
		// angle
		angle_cb = new ComboBox();
		angle_cb.addItem("0");
		angle_cb.addItem("90");
		angle_cb.addItem("180");
		angle_cb.addItem("270");
		angle_cb.setCallback(CHANGED,"Projection::angle_changed");
		engine.projection.setAngle(engine.config.getInt("projection_angle",0));
		angle_cb.setCurrentItem(engine.projection.getAngle());
		hbox.addChild(new Label(" Angle: "),ALIGN_RIGHT);
		hbox.addChild(angle_cb,ALIGN_RIGHT);
		
		// display
		display_cb = new ComboBox();
		forloop(int i = 0; engine.projection.getWidth()) {
			display_cb.addItem(string(i));
		}
		display_cb.setCurrentItem(clamp(engine.config.getInt("projection_display",0),0,displays.size() - 1));
		display_cb.setCallback(CHANGED,"Projection::display_changed");
		hbox.addChild(new Label(" Display: "),ALIGN_RIGHT);
		hbox.addChild(display_cb,ALIGN_RIGHT);
		display = display_cb.getCurrentItem();
		
		// deform
		deform = new Deform(320,240);
		deform.setCallback(DEFORM_CHANGED,"Projection::deform_changed");
		window.addChild(new VBox(2,2));
		window.addChild(deform,ALIGN_EXPAND);
		
		/////////////////////////////////
		// points panel
		/////////////////////////////////
		
		GroupBox groupbox = new GroupBox("Points",4,4);
		window.addChild(groupbox);
		GridBox gridbox = new GridBox(7,4,4);
		groupbox.addChild(gridbox,ALIGN_EXPAND);
		
		Button create_point_column_button(string text,int num) {
			Button button = new Button(text);
			button.setBackground(0);
			button.setCallback(CLICKED,"Projection::point_column_clicked",num);
			gridbox.addChild(button);
			return button;
		}
		
		Button create_point_row_button(string text,int num) {
			Button button = new Button(text);
			button.setBackground(0);
			button.setCallback(CLICKED,"Projection::point_row_clicked",num);
			gridbox.addChild(button,ALIGN_RIGHT);
			return button;
		}
		
		EditLine create_point_editline() {
			EditLine editline = new EditLine();
			editline.setCallback(PRESSED,"Projection::point_pressed");
			editline.setCallback(FOCUS_OUT,"Projection::point_pressed");
			gridbox.addChild(editline,ALIGN_EXPAND);
			SpinBox spinbox = new SpinBox(-2000,2000,0);
			editline.addAttach(spinbox,"%.3f",1000);
			gridbox.addChild(spinbox,ALIGN_OVERLAP);
			return editline;
		}
		
		gridbox.addChild(new VBox());
		create_point_column_button("Point X",0);
		create_point_column_button("Point Y",1);
		create_point_column_button("Horiz X",2);
		create_point_column_button("Horiz Y",3);
		create_point_column_button("Vert X",4);
		create_point_column_button("Vert Y",5);
		
		create_point_row_button("Top-Left: ",0);
		point_3_el[0] = create_point_editline();
		point_3_el[1] = create_point_editline();
		handle_2_el[2] = create_point_editline();
		handle_2_el[3] = create_point_editline();
		handle_3_el[0] = create_point_editline();
		handle_3_el[1] = create_point_editline();
		
		create_point_row_button("Top-Right: ",1);
		point_2_el[0] = create_point_editline();
		point_2_el[1] = create_point_editline();
		handle_2_el[0] = create_point_editline();
		handle_2_el[1] = create_point_editline();
		handle_1_el[2] = create_point_editline();
		handle_1_el[3] = create_point_editline();
		
		create_point_row_button("Bottom-Left: ",2);
		point_0_el[0] = create_point_editline();
		point_0_el[1] = create_point_editline();
		handle_0_el[0] = create_point_editline();
		handle_0_el[1] = create_point_editline();
		handle_3_el[2] = create_point_editline();
		handle_3_el[3] = create_point_editline();
		
		create_point_row_button("Bottom-Right: ",3);
		point_1_el[0] = create_point_editline();
		point_1_el[1] = create_point_editline();
		handle_0_el[2] = create_point_editline();
		handle_0_el[3] = create_point_editline();
		handle_1_el[0] = create_point_editline();
		handle_1_el[1] = create_point_editline();
		
		// bottom panel
		vbox = new VBox();
		hbox = new HBox();
		window.addChild(vbox);
		vbox.addChild(hbox,ALIGN_EXPAND);
		
		/////////////////////////////////
		// borders
		/////////////////////////////////
		
		vbox = new VBox();
		hbox.addChild(vbox,ALIGN_EXPAND);
		groupbox = new GroupBox("Border",4,4);
		vbox.addChild(groupbox,ALIGN_TOP);
		gridbox = new GridBox(4,4,4);
		groupbox.addChild(gridbox,ALIGN_EXPAND);
		
		Button create_border_column_button(string text,int num) {
			Button button = new Button(text);
			button.setBackground(0);
			button.setCallback(CLICKED,"Projection::border_column_clicked",num);
			gridbox.addChild(button);
			return button;
		}
		
		Button create_border_row_button(string text,int num) {
			Button button = new Button(text);
			button.setBackground(0);
			button.setCallback(CLICKED,"Projection::border_row_clicked",num);
			gridbox.addChild(button,ALIGN_RIGHT);
			return button;
		}
		
		EditLine create_border_editline() {
			EditLine editline = new EditLine();
			editline.setCallback(PRESSED,"Projection::border_pressed");
			editline.setCallback(FOCUS_OUT,"Projection::border_pressed");
			gridbox.addChild(editline,ALIGN_EXPAND);
			SpinBox spinbox = new SpinBox(-2000,2000,0);
			editline.addAttach(spinbox,"%.3f",1000,ATTACH_MIN_EXPAND | ATTACH_MAX_EXPAND);
			gridbox.addChild(spinbox,ALIGN_OVERLAP);
			return editline;
		}
		
		gridbox.addChild(new VBox());
		create_border_column_button("Offset",0);
		create_border_column_button("Size",1);
		create_border_column_button("Power",2);
		
		create_border_row_button("Left: ",3);
		border_3_el[0] = create_border_editline();
		border_3_el[1] = create_border_editline();
		border_3_el[2] = create_border_editline();
		
		create_border_row_button("Right: ",1);
		border_1_el[0] = create_border_editline();
		border_1_el[1] = create_border_editline();
		border_1_el[2] = create_border_editline();
		
		create_border_row_button("Top: ",0);
		border_0_el[0] = create_border_editline();
		border_0_el[1] = create_border_editline();
		border_0_el[2] = create_border_editline();
		
		create_border_row_button("Bottom: ",2);
		border_2_el[0] = create_border_editline();
		border_2_el[1] = create_border_editline();
		border_2_el[2] = create_border_editline();
		
		hbox.addChild(new VBox(2,2));
		
		/////////////////////////////////
		// linearity
		/////////////////////////////////
		
		vbox = new VBox();
		hbox.addChild(vbox,ALIGN_EXPAND);
		groupbox = new GroupBox("Linearity",4,4);
		vbox.addChild(groupbox,ALIGN_TOP);
		gridbox = new GridBox(3,4,4);
		groupbox.addChild(gridbox,ALIGN_EXPAND);
		
		Button create_linearity_column_button(string text,int num) {
			Button button = new Button(text);
			button.setBackground(0);
			button.setCallback(CLICKED,"Projection::linearity_column_clicked",num);
			gridbox.addChild(button);
			return button;
		}
		
		Button create_linearity_row_button(string text,int num) {
			Button button = new Button(text);
			button.setBackground(0);
			button.setCallback(CLICKED,"Projection::linearity_row_clicked",num);
			gridbox.addChild(button,ALIGN_RIGHT);
			return button;
		}
		
		EditLine create_linearity_editline() {
			EditLine editline = new EditLine();
			editline.setCallback(PRESSED,"Projection::linearity_pressed");
			editline.setCallback(FOCUS_OUT,"Projection::linearity_pressed");
			gridbox.addChild(editline,ALIGN_EXPAND);
			SpinBox spinbox = new SpinBox(-2000,2000,0);
			editline.addAttach(spinbox,"%.3f",1000,ATTACH_MIN_EXPAND | ATTACH_MAX_EXPAND);
			gridbox.addChild(spinbox,ALIGN_OVERLAP);
			return editline;
		}
		
		gridbox.addChild(new VBox());
		create_linearity_column_button("Power",0);
		create_linearity_column_button("Weight",1);
		
		create_linearity_row_button("Left: ",3);
		linearity_3_el[0] = create_linearity_editline();
		linearity_3_el[1] = create_linearity_editline();
		
		create_linearity_row_button("Right: ",1);
		linearity_1_el[0] = create_linearity_editline();
		linearity_1_el[1] = create_linearity_editline();
		
		create_linearity_row_button("Top: ",0);
		linearity_0_el[0] = create_linearity_editline();
		linearity_0_el[1] = create_linearity_editline();
		
		create_linearity_row_button("Bottom: ",2);
		linearity_2_el[0] = create_linearity_editline();
		linearity_2_el[1] = create_linearity_editline();
		
		/////////////////////////////////
		// color
		/////////////////////////////////
		
		groupbox = new GroupBox("Color",4,4);
		window.addChild(groupbox);
		gridbox = new GridBox(5,4,4);
		groupbox.addChild(gridbox,ALIGN_EXPAND);
		
		Button create_color_column_button(string text,int num) {
			Button button = new Button(text);
			button.setBackground(0);
			button.setCallback(CLICKED,"Projection::color_column_clicked",num);
			gridbox.addChild(button);
			return button;
		}
		
		Button create_color_row_button(string text,int num) {
			Button button = new Button(text);
			button.setBackground(0);
			button.setCallback(CLICKED,"Projection::color_row_clicked",num);
			gridbox.addChild(button,ALIGN_RIGHT);
			return button;
		}
		
		EditLine create_color_editline() {
			EditLine editline = new EditLine();
			editline.setCallback(PRESSED,"Projection::color_pressed");
			editline.setCallback(FOCUS_OUT,"Projection::color_pressed");
			gridbox.addChild(editline,ALIGN_EXPAND);
			SpinBox spinbox = new SpinBox(-2000,2000,0);
			editline.addAttach(spinbox,"%.3f",1000,ATTACH_MIN_EXPAND | ATTACH_MAX_EXPAND);
			gridbox.addChild(spinbox,ALIGN_OVERLAP);
			return editline;
		}
		
		gridbox.addChild(new VBox());
		create_color_column_button("Red",0);
		create_color_column_button("Green",1);
		create_color_column_button("Blue",2);
		create_color_column_button("RGB",3);
		
		create_color_row_button("Scale: ",0);
		color_scale_el[0] = create_color_editline();
		color_scale_el[1] = create_color_editline();
		color_scale_el[2] = create_color_editline();
		color_scale_el[3] = create_color_editline();
		
		create_color_row_button("Bias: ",1);
		color_bias_el[0] = create_color_editline();
		color_bias_el[1] = create_color_editline();
		color_bias_el[2] = create_color_editline();
		color_bias_el[3] = create_color_editline();
		
		/////////////////////////////////
		// buttons
		/////////////////////////////////
		
		window.addChild(new VBox(2,2));
		
		vbox = new VBox();
		hbox = new HBox(2,0);
		window.addChild(vbox);
		vbox.addChild(hbox,ALIGN_EXPAND);
		hbox.setPadding(-2,-2,0,0);
		
		auxiliary_cb = new ComboBox();
		hbox.addChild(auxiliary_cb,ALIGN_LEFT);
		auxiliary_cb.setHidden(1);
		
		clear_b = new Button("Clear");
		clear_b.setCallback(CLICKED,"Projection::clear_clicked");
		hbox.addChild(clear_b,ALIGN_RIGHT);
		
		load_b = new Button("Load");
		load_b.setCallback(CLICKED,"Projection::load_clicked");
		hbox.addChild(load_b,ALIGN_RIGHT);
		
		save_b = new Button("Save");
		save_b.setCallback(CLICKED,"Projection::save_clicked");
		hbox.addChild(save_b,ALIGN_RIGHT);
		
		apply_b = new Button("Apply");
		apply_b.setCallback(CLICKED,"Projection::apply_clicked");
		hbox.addChild(apply_b,ALIGN_RIGHT);
		
		cancel_b = new Button("Cancel");
		cancel_b.setCallback(CLICKED,"Projection::cancel_clicked");
		hbox.addChild(cancel_b,ALIGN_RIGHT);
	}
	
	void shutdown() {
		
		// save parameters
		engine.config.setFloat("projection_gamma",engine.projection.getGamma());
		engine.config.setInt("projection_angle",engine.projection.getAngle());
		engine.config.setInt("projection_grid_mode",grid_mode_cb.getCurrentItem());
		engine.config.setInt("projection_grid_step",grid_step_cb.getCurrentItem());
		engine.config.setInt("projection_display",display_cb.getCurrentItem());
		engine.config.setString("projection_name",projection_name);
		
		// save displays
		forloop(int i = 0; displays.size()) {
			displays[i].get();
			displays[i].save();
		}
	}
	
	/*
	 */
	void update() {
		
		// update grid
		if(get_activity()) grid.set(grid_mode_cb.getCurrentItem(),int(grid_step_cb.getCurrentItemText()));
	}
	
	/*
	 */
	void run() {
		
		// check status
		if(get_activity()) return;
		
		enabled = 1;
		
		// old displays
		Display old_displays[0];
		forloop(int i = 0; displays.size()) {
			old_displays.append(new Display(displays[i]));
		}
		
		// set parameters
		float gamma = engine.projection.getGamma();
		int angle = engine.projection.getAngle();
		displays[display].setDeform();
		displays[display].setPoints();
		displays[display].setBorder();
		displays[display].setLinearity();
		displays[display].setColor();
		update();
		
		// show window
		result = 0;
		deform.update();
		window.arrange();
		addChild(window,ALIGN_OVERLAP);
		window.setWidth(engine.config.getInt("projection_width",0));
		window.setHeight(engine.config.getInt("projection_height",0));
		int position_x = engine.config.getInt("projection_position_x",128);
		int position_y = engine.config.getInt("projection_position_y",128);
		window.setPosition(position_x,position_y);
		window.setFocus();
		update();
		while(result == 0) {
			deform.update();
			wait;
		}
		removeChild(window);
		grid.clear();
		
		// restore parameters
		if(result == -1) {
			displays.delete();
			engine.projection.setGamma(gamma);
			engine.projection.setAngle(angle);
			forloop(int i = 0; old_displays.size()) {
				displays.append(new Display(old_displays[i]));
				displays[i].set();
			}
		}
		
		// remove old displays
		old_displays.delete();
		
		// save parameters
		engine.config.setInt("projection_width",window.getWidth());
		engine.config.setInt("projection_height",window.getHeight());
		engine.config.setInt("projection_position_x",window.getPositionX());
		engine.config.setInt("projection_position_y",window.getPositionY());
		
		enabled = 0;
		changed();
	}
	
	/*
	 */
	int get_activity() {
		return enabled;
	}
	
	/*
	 */
	void add_auxiliary_item(string str) {
		auxiliary_cb.addItem(str);
		auxiliary_cb.setHidden(0);
	}
	
	int get_auxiliary_item() {
		return auxiliary_cb.getCurrentItem();
	}
	
	void clear_auxiliary() {
		auxiliary_cb.setHidden(1);
		auxiliary_cb.clear();
	}
	
	/*
	 */
	void load_clicked() {
		if(dialogFile("Select projection to load",".projection",projection_name)) {
			projection_name = extension(projection_name,"projection");
			Xml xml = new Xml();
			if(xml.load(projection_name)) {
				grid_mode_cb.setCurrentItem(xml.getChildData("grid_mode",0));
				grid_step_cb.setCurrentItem(xml.getChildData("grid_step",0));
				engine.projection.setGamma(xml.getChildData("gamma",2.2f));
				engine.projection.setAngle(xml.getChildData("angle",0));
				gamma_el.setText(format(engine.projection.getGamma()));
				angle_cb.setCurrentItem(engine.projection.getAngle());
				forloop(int i = 0; engine.projection.getWidth()) {
					Xml x = xml.find(::format("display index=\"%d\"",i));
					if(x != NULL) displays[i].load(x);
					displays[i].set();
				}
				displays[display].setDeform();
				displays[display].setPoints();
				displays[display].setBorder();
				displays[display].setLinearity();
				displays[display].setColor();
				update();
			} else {
				dialogMessage("Error",::format("Can't load \"%s\" file",projection_name));
			}
			delete xml;
		}
	}
	
	void save_clicked() {
		if(dialogFile("Select projection to save",".projection",projection_name)) {
			projection_name = extension(projection_name,"projection");
			Xml xml = new Xml("projection","version=\"1.00\"");
			xml.setChildData("grid_mode",grid_mode_cb.getCurrentItem());
			xml.setChildData("grid_step",grid_step_cb.getCurrentItem());
			xml.setChildData("gamma",engine.projection.getGamma());
			xml.setChildData("angle",engine.projection.getAngle());
			forloop(int i = 0; engine.projection.getWidth()) {
				Xml x = xml.addChild("display",::format("index=\"%d\"",i));
				displays[i].save(x);
			}
			if(xml.save(projection_name) == 0) {
				dialogMessage("Error",::format("Can't save \"%s\" file",projection_name));
			}
			delete xml;
		}
	}
	
	void clear_clicked() {
		if(dialogMessage("Confirm","Clear projection settings to default?")) {
			engine.projection.setGamma(2.2f);
			engine.projection.setAngle(0);
			engine.projection.clear();
			gamma_el.setText(format(engine.projection.getGamma()));
			angle_cb.setCurrentItem(engine.projection.getAngle());
			forloop(int i = 0; engine.projection.getWidth()) {
				displays[i].get();
			}
			displays[display].setDeform();
			displays[display].setPoints();
			displays[display].setBorder();
			displays[display].setLinearity();
			displays[display].setColor();
			update();
		}
	}
	
	void apply_clicked() {
		result = 1;
	}
	
	void cancel_clicked() {
		result = -1;
	}
	
	/*
	 */
	void changed() {
		if(changed_callback != NULL) call(changed_callback);
		if(changed_world_callback != NULL) engine.world.call(changed_world_callback);
		if(changed_editor_callback != NULL) engine.editor.call(changed_editor_callback);
	}
	
	/*
	 */
	string format(float value) {
		return ::format("%.3f",value);
	}
	
	/*
	 */
	void set_vec2(EditLine editline[],vec3 value) {
		editline[0].setText(format(value.x));
		editline[1].setText(format(value.y));
	}
	
	vec3 get_vec2(EditLine editline[]) {
		vec3 value;
		value.x = float(editline[0].getText());
		value.y = float(editline[1].getText());
		return value;
	}
	
	/*
	 */
	void set_vec3(EditLine editline[],vec3 value) {
		editline[0].setText(format(value.x));
		editline[1].setText(format(value.y));
		editline[2].setText(format(value.z));
	}
	
	vec3 get_vec3(EditLine editline[]) {
		vec3 value;
		value.x = float(editline[0].getText());
		value.y = float(editline[1].getText());
		value.z = float(editline[2].getText());
		return value;
	}
	
	/*
	 */
	void set_vec4(EditLine editline[],vec3 value) {
		editline[0].setText(format(value.x));
		editline[1].setText(format(value.y));
		editline[2].setText(format(value.z));
		editline[3].setText(format(value.w));
	}
	
	vec4 get_vec4(EditLine editline[]) {
		vec4 value;
		value.x = float(editline[0].getText());
		value.y = float(editline[1].getText());
		value.z = float(editline[2].getText());
		value.w = float(editline[3].getText());
		return value;
	}
	
	/*
	 */
	void set_vec4(EditLine editline_0[],EditLine editline_1[],vec4 value) {
		editline_0[0].setText(format(value.x));
		editline_0[1].setText(format(value.y));
		editline_1[2].setText(format(value.z));
		editline_1[3].setText(format(value.w));
	}
	
	/*
	 */
	void grid_changed() {
		grid.set(grid_mode_cb.getCurrentItem(),int(grid_step_cb.getCurrentItemText()));
		changed();
	}
	
	void gamma_pressed() {
		engine.projection.setGamma(clamp(float(gamma_el.getText()),0.1f,10.0f));
		gamma_el.setText(format(engine.projection.getGamma()));
		changed();
	}
	
	void angle_changed() {
		engine.projection.setAngle(angle_cb.getCurrentItem());
		changed();
	}
	
	void display_changed() {
		displays[display].getDeform();
		displays[display].getBorder();
		displays[display].getLinearity();
		displays[display].getColor();
		display = display_cb.getCurrentItem();
		displays[display].setDeform();
		displays[display].setBorder();
		displays[display].setLinearity();
		displays[display].setColor();
	}
	
	/*
	 */
	void deform_changed() {
		displays[display].getDeform();
		displays[display].setDeform();
	}
	
	/*
	 */
	void point_column_clicked(int num) {
		if(num == 0) { point_3_el[0].setText(format(-1.0f)); point_2_el[0].setText(format(1.0f)); point_0_el[0].setText(format(-1.0f)); point_1_el[0].setText(format(1.0f)); }
		if(num == 1) { point_3_el[1].setText(format(1.0f)); point_2_el[1].setText(format(1.0f)); point_0_el[1].setText(format(-1.0f)); point_1_el[1].setText(format(-1.0f)); }
		if(num == 2) { handle_2_el[2].setText(format(0.5f)); handle_2_el[0].setText(format(-0.5f)); handle_0_el[0].setText(format(0.5f)); handle_0_el[2].setText(format(-0.5f)); }
		if(num == 3) { handle_2_el[3].setText(format(0.0f)); handle_2_el[1].setText(format(0.0f)); handle_0_el[1].setText(format(0.0f)); handle_0_el[3].setText(format(0.0f)); }
		if(num == 4) { handle_3_el[0].setText(format(0.0f)); handle_1_el[2].setText(format(0.0f)); handle_3_el[2].setText(format(0.0f)); handle_1_el[0].setText(format(0.0f)); }
		if(num == 5) { handle_3_el[1].setText(format(-0.5f)); handle_1_el[3].setText(format(-0.5f)); handle_3_el[3].setText(format(0.5f)); handle_1_el[1].setText(format(0.5f)); }
		displays[display].getPoints();
		displays[display].setPoints();
	}
	
	void point_row_clicked(int num) {
		if(num == 0) { set_vec2(point_3_el,vec3(-1.0f, 1.0f,0.0f)); set_vec4(handle_3_el,handle_2_el,vec4( 0.0f,-0.5f, 0.5f, 0.0f)); }
		if(num == 1) { set_vec2(point_2_el,vec3( 1.0f, 1.0f,0.0f)); set_vec4(handle_2_el,handle_1_el,vec4(-0.5f, 0.0f, 0.0f,-0.5f)); }
		if(num == 2) { set_vec2(point_0_el,vec3(-1.0f,-1.0f,0.0f)); set_vec4(handle_0_el,handle_3_el,vec4( 0.5f, 0.0f, 0.0f, 0.5f)); }
		if(num == 3) { set_vec2(point_1_el,vec3( 1.0f,-1.0f,0.0f)); set_vec4(handle_1_el,handle_0_el,vec4( 0.0f, 0.5f,-0.5f, 0.0f)); }
		displays[display].getPoints();
		displays[display].setPoints();
	}
	
	void point_pressed() {
		displays[display].getPoints();
		displays[display].setPoints();
	}
	
	/*
	 */
	void border_column_clicked(int num) {
		if(num == 0) { border_3_el[0].setText(format(0.0f)); border_1_el[0].setText(format(0.0f)); border_0_el[0].setText(format(0.0f)); border_2_el[0].setText(format(0.0f)); }
		if(num == 1) { border_3_el[1].setText(format(0.0f)); border_1_el[1].setText(format(0.0f)); border_0_el[1].setText(format(0.0f)); border_2_el[1].setText(format(0.0f)); }
		if(num == 2) { border_3_el[2].setText(format(1.0f)); border_1_el[2].setText(format(1.0f)); border_0_el[2].setText(format(1.0f)); border_2_el[2].setText(format(1.0f)); }
		displays[display].getBorder();
		displays[display].setBorder();
	}
	
	void border_row_clicked(int num) {
		if(num == 0) set_vec3(border_0_el,vec3(0.0f,0.0f,1.0f));
		if(num == 1) set_vec3(border_1_el,vec3(0.0f,0.0f,1.0f));
		if(num == 2) set_vec3(border_2_el,vec3(0.0f,0.0f,1.0f));
		if(num == 3) set_vec3(border_3_el,vec3(0.0f,0.0f,1.0f));
		displays[display].getBorder();
		displays[display].setBorder();
	}
	
	void border_pressed() {
		displays[display].getBorder();
		displays[display].setBorder();
	}
	
	/*
	 */
	void linearity_column_clicked(int num) {
		if(num == 0) { linearity_3_el[0].setText(format(1.0f)); linearity_1_el[0].setText(format(1.0f)); linearity_0_el[0].setText(format(1.0f)); linearity_2_el[0].setText(format(1.0f)); }
		if(num == 1) { linearity_3_el[1].setText(format(1.0f)); linearity_1_el[1].setText(format(1.0f)); linearity_0_el[1].setText(format(1.0f)); linearity_2_el[1].setText(format(1.0f)); }
		displays[display].getLinearity();
		displays[display].setLinearity();
	}
	
	void linearity_row_clicked(int num) {
		if(num == 0) set_vec2(linearity_0_el,vec3(1.0f,1.0f,0.0f));
		if(num == 1) set_vec2(linearity_1_el,vec3(1.0f,1.0f,0.0f));
		if(num == 2) set_vec2(linearity_2_el,vec3(1.0f,1.0f,0.0f));
		if(num == 3) set_vec2(linearity_3_el,vec3(1.0f,1.0f,0.0f));
		displays[display].getLinearity();
		displays[display].setLinearity();
	}
	
	void linearity_pressed() {
		displays[display].getLinearity();
		displays[display].setLinearity();
	}
	
	/*
	 */
	void color_column_clicked(int num) {
		if(num == 0) { color_scale_el[0].setText(format(1.0f)); color_bias_el[0].setText(format(0.0f)); }
		if(num == 1) { color_scale_el[1].setText(format(1.0f)); color_bias_el[1].setText(format(0.0f)); }
		if(num == 2) { color_scale_el[2].setText(format(1.0f)); color_bias_el[2].setText(format(0.0f)); }
		if(num == 3) { color_scale_el[3].setText(format(1.0f)); color_bias_el[3].setText(format(0.0f)); }
		displays[display].getColor();
		displays[display].setColor();
	}
	
	void color_row_clicked(int num) {
		if(num == 0) set_vec4(color_scale_el,vec4(1.0f,1.0f,1.0f,1.0f));
		if(num == 1) set_vec4(color_bias_el,vec4(0.0f,0.0f,0.0f,0.0f));
		displays[display].getColor();
		displays[display].setColor();
	}
	
	void color_pressed() {
		displays[display].getColor();
		displays[display].setColor();
	}
}

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void projectionInit() {
	Projection::init();
}

void projectionShutdown() {
	Projection::shutdown();
}

/*
 */
void projectionUpdate() {
	Projection::update();
}

/*
 */
void projectionRun() {
	thread("Projection::run");
}

/*
 */
int projectionGetActivity() {
	return Projection::get_activity();
}

/*
 */
void projectionAddAuxiliaryItem(string str) {
	Projection::add_auxiliary_item(str);
}

int projectionGetAuxiliaryItem() {
	return Projection::get_auxiliary_item();
}

void projectionClearAuxiliary() {
	Projection::clear_auxiliary();
}

/*
 */
int projectionGetGridMode() {
	return Projection::grid_mode_cb.getCurrentItem();
}

int projectionGetGridStep() {
	return int(Projection::grid_step_cb.getCurrentItemText());
}

void projectionSetGrid(int mode,int step) {
	Projection::grid.set(mode,step);
}

void projectionClearGrid() {
	Projection::grid.clear();
}

/*
 */
void projectionSetChangedCallback(string callback) {
	Projection::changed_callback = callback;
}

string projectionGetChangedCallback() {
	return Projection::changed_callback;
}

void projectionSetChangedWorldCallback(string callback) {
	Projection::changed_world_callback = callback;
}

string projectionGetChangedWorldCallback() {
	return Projection::changed_world_callback;
}

void projectionSetChangedEditorCallback(string callback) {
	Projection::changed_editor_callback = callback;
}

string projectionGetChangedEditorCallback() {
	return Projection::changed_editor_callback;
}

#endif /* __UNIGINE_SYSTEM_PROJECTION_H__ */
