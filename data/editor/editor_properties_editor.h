/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_properties_editor.h
 * Desc:    Unigine editor
 * Version: 1.08
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
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
namespace Editor {
	
	/*
	 */
	WidgetDialog dialog;				// property dialog
	
	DECLARE_DIALOG
	
	Property property;					// current property
	Property parent;					// parent property
	
	WidgetTabBox property_tb;			// property container
	
	WidgetLabel info_library_l;			// library name
	WidgetLabel info_name_l;			// property name
	
	WidgetCheckBox intersection_cb;		// property options
	WidgetCheckBox collision_cb;
	
	WidgetButton paste_b;				// paste settings
	
	WidgetGridBox states_gb;			// property states container
	WidgetGridBox parameters_gb;		// property parameters container
	
	/**************************************************************************\
	*
	* Parameters
	*
	\**************************************************************************/
	
	/*
	 */
	PParameter parameters[0];
	
	/*
	 */
	class PParameter {
		
		int num;						// parameter number
		string name;					// parameter name
		int type;						// parameter type
		int hidden;						// parameter hidden
		
		WidgetLabel name_l;				// name label
		WidgetHBox main_hb;				// main hbox
		
		WidgetIcon clear_i;				// parameter clear
		
		WidgetEditLine slider_el;		// slider editline
		WidgetVBox slider_vb;			// slider vbox
		WidgetSlider slider_sl;			// slider
		int multiplier;					// multiplier
		
		WidgetEditLine string_el;		// string editline
		WidgetCheckBox toggle_cb;		// toggle checkbox
		WidgetComboBox switch_cb;		// switch combobox
		
		WidgetGroupBox color_gb;		// color groupbox
		WidgetHBox color_hb;
		WidgetSprite color_s[2];		// color value
		
		WidgetHBox vector_hb;			// vector value
		WidgetEditLine vector_el[4];
		
		WidgetEditLine mask_el;			// mask editline
		
		// update
		void update() {
			int override = (parent.findParameter(name) != -1);
			if(override) {
				override &= (parent.getParameterType(num) != PROPERTY_PARAMETER_INT || property.getParameterInt(num) != parent.getParameterInt(num));
				override &= (parent.getParameterType(num) != PROPERTY_PARAMETER_FLOAT || property.getParameterFloat(num) != parent.getParameterFloat(num));
				override &= (parent.getParameterType(num) != PROPERTY_PARAMETER_DOUBLE || property.getParameterDouble(num) != parent.getParameterDouble(num));
				override &= (parent.getParameterType(num) != PROPERTY_PARAMETER_TOGGLE || property.getParameterToggle(num) != parent.getParameterToggle(num));
				override &= (parent.getParameterType(num) != PROPERTY_PARAMETER_SWITCH || property.getParameterSwitch(num) != parent.getParameterSwitch(num));
				override &= (parent.getParameterType(num) != PROPERTY_PARAMETER_STRING || property.getParameterString(num) != parent.getParameterString(num));
				override &= (parent.getParameterType(num) != PROPERTY_PARAMETER_COLOR || property.getParameterColor(num) != parent.getParameterColor(num));
				override &= (parent.getParameterType(num) != PROPERTY_PARAMETER_VEC3 || property.getParameterVec3(num) != parent.getParameterVec3(num));
				override &= (parent.getParameterType(num) != PROPERTY_PARAMETER_VEC4 || property.getParameterVec4(num) != parent.getParameterVec4(num));
				override &= (parent.getParameterType(num) != PROPERTY_PARAMETER_MASK || property.getParameterMask(num) != parent.getParameterMask(num));
			}
			name_l.setFontColor(override ? color_green : vec4_zero);
			clear_i.setEnabled(override);
		}
		
		// constructor
		PParameter(int n) {
			
			num = n;
			name = property.getParameterName(num);
			type = property.getParameterType(num);
			hidden = property.isParameterHidden(num);
			
			if(hidden) return;
			
			main_hb = new WidgetHBox(gui);
			
			clear_i = new WidgetIcon(gui,"editor/gui/action_clear.png");
			clear_i.setWidth(24);
			clear_i.setHeight(24);
			clear_i.setToolTip(TR("Reset"));
			
			clear_i.setCallback(GUI_CLICKED,functionid(parameter_clear_clicked),num);
			
			// int parameter
			if(type == PROPERTY_PARAMETER_INT) {
				
				name_l = new WidgetLabel(gui,name + ":");
				
				int value = property.getParameterInt(num);
				
				slider_el = new WidgetEditLine(gui);
				main_hb.addChild(slider_el);
				
				slider_vb = new WidgetVBox(gui,2,2);
				main_hb.addChild(slider_vb);
				
				int flags = 0;
				if(property.getParameterSliderLog10(num)) flags |= GUI_ATTACH_LOG10;
				if(property.getParameterSliderMaxExpand(num)) flags |= GUI_ATTACH_MAX_EXPAND;
				if(property.getParameterSliderMinExpand(num)) flags |= GUI_ATTACH_MIN_EXPAND;
				slider_sl = new WidgetSlider(gui,property.getParameterIntMinValue(num),property.getParameterIntMaxValue(num),value);
				slider_sl.addAttach(slider_el,"%d",1,flags);
				main_hb.addChild(slider_sl,GUI_ALIGN_EXPAND);
				
				slider_el.setText(string(value));
				
				slider_el.setCallback(GUI_PRESSED,functionid(parameter_int_pressed),num);
				slider_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_int_pressed),num);
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			// float parameter
			else if(type == PROPERTY_PARAMETER_FLOAT) {
				
				name_l = new WidgetLabel(gui,name + ":");
				
				float value = property.getParameterFloat(num);
				
				slider_el = new WidgetEditLine(gui);
				main_hb.addChild(slider_el);
				
				slider_vb = new WidgetVBox(gui,2,2);
				main_hb.addChild(slider_vb);
				
				int flags = 0;
				if(property.getParameterSliderLog10(num)) flags |= GUI_ATTACH_LOG10;
				if(property.getParameterSliderMaxExpand(num)) flags |= GUI_ATTACH_MAX_EXPAND;
				if(property.getParameterSliderMinExpand(num)) flags |= GUI_ATTACH_MIN_EXPAND;
				multiplier = (abs(property.getParameterFloatMaxValue(num) - property.getParameterFloatMinValue(num)) > 1000.0f) ? 10 : 1000;
				slider_sl = new WidgetSlider(gui,property.getParameterFloatMinValue(num) * multiplier,property.getParameterFloatMaxValue(num) * multiplier,value * multiplier);
				slider_sl.addAttach(slider_el,"%g",multiplier,flags);
				main_hb.addChild(slider_sl,GUI_ALIGN_EXPAND);
				
				slider_el.setText(editorFormat(value));
				
				slider_el.setCallback(GUI_PRESSED,functionid(parameter_float_pressed),num);
				slider_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_float_pressed),num);
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			// double parameter
			else if(type == PROPERTY_PARAMETER_DOUBLE) {
				
				name_l = new WidgetLabel(gui,name + ":");
				
				double value = property.getParameterDouble(num);
				
				slider_el = new WidgetEditLine(gui);
				main_hb.addChild(slider_el);
				
				slider_vb = new WidgetVBox(gui,2,2);
				main_hb.addChild(slider_vb);
				
				int flags = 0;
				if(property.getParameterSliderLog10(num)) flags |= GUI_ATTACH_LOG10;
				if(property.getParameterSliderMaxExpand(num)) flags |= GUI_ATTACH_MAX_EXPAND;
				if(property.getParameterSliderMinExpand(num)) flags |= GUI_ATTACH_MIN_EXPAND;
				multiplier = (abs(property.getParameterDoubleMaxValue(num) - property.getParameterDoubleMinValue(num)) > 1000.0) ? 10 : 1000;
				slider_sl = new WidgetSlider(gui,property.getParameterDoubleMinValue(num) * multiplier,property.getParameterDoubleMaxValue(num) * multiplier,value * multiplier);
				slider_sl.addAttach(slider_el,"%g",multiplier,flags);
				main_hb.addChild(slider_sl,GUI_ALIGN_EXPAND);
				
				slider_el.setText(editorFormat(value));
				
				slider_el.setCallback(GUI_PRESSED,functionid(parameter_double_pressed),num);
				slider_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_double_pressed),num);
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			// toggle parameter
			else if(type == PROPERTY_PARAMETER_TOGGLE) {
				
				name_l = new WidgetLabel(gui);
				
				toggle_cb = new WidgetCheckBox(gui,get_parameter_name(name));
				toggle_cb.setChecked(property.getParameterToggle(num));
				main_hb.addChild(toggle_cb,GUI_ALIGN_LEFT);
				
				toggle_cb.setCallback(GUI_CLICKED,functionid(parameter_toggle_clicked),num);
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			// switch parameter
			else if(type == PROPERTY_PARAMETER_SWITCH) {
				
				name_l = new WidgetLabel(gui,name + ":");
				
				switch_cb = new WidgetComboBox(gui);
				forloop(int i = 0; property.getParameterSwitchNumItems(num)) {
					switch_cb.addItem(get_parameter_item(property.getParameterSwitchItem(num,i)));
				}
				switch_cb.setCurrentItem(property.getParameterSwitch(num));
				main_hb.addChild(switch_cb,GUI_ALIGN_EXPAND);
				
				switch_cb.setCallback(GUI_CHANGED,functionid(parameter_switch_changed),num);
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			// string parameter
			else if(type == PROPERTY_PARAMETER_STRING) {
				
				name_l = new WidgetLabel(gui,name + ":");
				
				string_el = new WidgetEditLine(gui,property.getParameterString(num));
				main_hb.addChild(string_el,GUI_ALIGN_EXPAND);
				
				string_el.setCallback(GUI_PRESSED,functionid(parameter_string_pressed),num);
				string_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_string_pressed),num);
				
				if(property.getParameterStringFile(num)) {
					string_el.setCallback(GUI_DOUBLE_CLICKED,functionid(parameter_string_double_clicked),num);
				}
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			// color parameter
			else if(type == PROPERTY_PARAMETER_COLOR) {
				
				name_l = new WidgetLabel(gui,name + ":");
				
				color_gb = new WidgetGroupBox(gui);
				main_hb.addChild(color_gb,GUI_ALIGN_EXPAND);
				
				color_hb = new WidgetHBox(gui);
				color_gb.addChild(color_hb,GUI_ALIGN_EXPAND);
				
				color_s[0] = new WidgetSprite(gui,"editor/gui/color_white.png");
				color_s[0].setTransform(scale(8,8,1));
				color_hb.addChild(color_s[0],GUI_ALIGN_EXPAND);
				
				color_s[1] = new WidgetSprite(gui,"editor/gui/color_white.png");
				color_s[1].setTransform(scale(8,8,1));
				color_hb.addChild(color_s[1],GUI_ALIGN_EXPAND);
				
				color_s[0].setCallback(GUI_CLICKED,functionid(parameter_color_clicked),num);
				color_s[1].setCallback(GUI_CLICKED,functionid(parameter_color_clicked),num);
				
				setSpriteColor(color_s,property.getParameterColor(num));
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			// vec3 parameter
			else if(type == PROPERTY_PARAMETER_VEC3) {
				
				name_l = new WidgetLabel(gui,get_parameter_name(name) + ":");
				
				vector_hb = new WidgetHBox(gui,4,0);
				vector_hb.setPadding(-4,-4,0,0);
				main_hb.addChild(vector_hb,GUI_ALIGN_EXPAND);
				
				forloop(int i = 0; 3) {
					vector_el[i] = new WidgetEditLine(gui);
					vector_el[i].setCallback(GUI_PRESSED,functionid(parameter_vec3_pressed),num);
					vector_el[i].setCallback(GUI_FOCUS_OUT,functionid(parameter_vec3_pressed),num);
					vector_hb.addChild(vector_el[i],GUI_ALIGN_EXPAND);
				}
				
				setEditLineVec3(vector_el,property.getParameterVec3(num));
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			// vec4 parameter
			else if(type == PROPERTY_PARAMETER_VEC4) {
				
				name_l = new WidgetLabel(gui,get_parameter_name(name) + ":");
				
				vector_hb = new WidgetHBox(gui,4,0);
				vector_hb.setPadding(-4,-4,0,0);
				main_hb.addChild(vector_hb,GUI_ALIGN_EXPAND);
				
				forloop(int i = 0; 4) {
					vector_el[i] = new WidgetEditLine(gui);
					vector_el[i].setCallback(GUI_PRESSED,functionid(parameter_vec4_pressed),num);
					vector_el[i].setCallback(GUI_FOCUS_OUT,functionid(parameter_vec4_pressed),num);
					vector_hb.addChild(vector_el[i],GUI_ALIGN_EXPAND);
				}
				
				setEditLineVec4(vector_el,property.getParameterVec4(num));
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			// mask parameter
			else if(type == PROPERTY_PARAMETER_MASK) {
				
				name_l = new WidgetLabel(gui,name + ":");
				
				int value = property.getParameterMask(num);
				
				mask_el = new WidgetEditLine(gui);
				main_hb.addChild(mask_el,GUI_ALIGN_EXPAND);
				
				mask_el.setText(editorFormatMask(value));
				
				mask_el.setCallback(GUI_PRESSED,functionid(parameter_mask_pressed),num);
				mask_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_mask_pressed),num);
				mask_el.setCallback(GUI_DOUBLE_CLICKED,functionid(parameter_mask_double_clicked),num);
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			else {
				throw("Properties::PParameter::PParameter(): unknown parameter type\n");
			}
			
			update();
		}
		
		~PParameter() {
			
			delete name_l;
			delete main_hb;
			
			delete clear_i;
			
			delete slider_el;
			delete slider_vb;
			delete slider_sl;
			
			delete string_el;
			delete toggle_cb;
			delete switch_cb;
			
			delete color_gb;
			delete color_hb;
			color_s.delete();
			
			delete vector_hb;
			vector_el.delete();
			
			delete mask_el;
		}
	};
	
	/*
	 */
	int parameter_num;
	
	/*
	 */
	void parameter_int_pressed(int num) {
		PParameter parameter = parameters[num];
		property.setParameterInt(num,int(parameter.slider_el.getText()));
		parameter.slider_el.setText(string(property.getParameterInt(num)));
		parameter.update();
	}
	
	/*
	 */
	void parameter_float_pressed(int num) {
		PParameter parameter = parameters[num];
		property.setParameterFloat(num,float(parameter.slider_el.getText()));
		parameter.slider_el.setText(editorFormat(property.getParameterFloat(num)));
		parameter.update();
	}
	
	/*
	 */
	void parameter_double_pressed(int num) {
		PParameter parameter = parameters[num];
		property.setParameterDouble(num,double(parameter.slider_el.getText()));
		parameter.slider_el.setText(editorFormat(property.getParameterDouble(num)));
		parameter.update();
	}
	
	/*
	 */
	void parameter_toggle_clicked(int num) {
		PParameter parameter = parameters[num];
		property.setParameterToggle(num,parameter.toggle_cb.isChecked());
		parameter.toggle_cb.setChecked(property.getParameterToggle(num));
		parameter.update();
	}
	
	/*
	 */
	void parameter_switch_changed(int num) {
		PParameter parameter = parameters[num];
		property.setParameterSwitch(num,parameter.switch_cb.getCurrentItem());
		parameter.switch_cb.setCurrentItem(property.getParameterSwitch(num));
		parameter.update();
	}
	
	/*
	 */
	void parameter_string_pressed(int num) {
		PParameter parameter = parameters[num];
		property.setParameterString(num,parameter.string_el.getText());
		parameter.update();
	}
	
	void parameter_string_double_clicked(int num) {
		PParameter parameter = parameters[num];
		string name = property.getParameterString(num);
		if(dialogFile(format(TR("Select %s file"),get_parameter_name(parameters[num].name)),"",name,DIALOG_FILE_OPEN) == 0) return;
		if(property.getParameterString(num) != name) {
			property.setParameterString(num,name);
			parameter.string_el.setText(name);
			parameter.update();
		}
	}
	
	/*
	 */
	void parameter_color_changed(WidgetDialogColor dialog) {
		PParameter parameter = parameters[parameter_num];
		property.setParameterColor(parameter_num,dialog.getColor());
		setSpriteColor(parameter.color_s,property.getParameterColor(parameter_num));
	}
	
	void parameter_color_clicked(int num) {
		parameter_num = num;
		PParameter parameter = parameters[num];
		dialogColor(format(TR("Select %s color"),parameters[num].name),functionid(parameter_color_changed),property.getParameterColor(num));
		parameter.update();
	}
	
	/*
	 */
	void parameter_vec3_pressed(int num) {
		PParameter parameter = parameters[num];
		property.setParameterVec3(num,getEditLineVec3(parameter.vector_el));
		setEditLineVec3(parameter.vector_el,property.getParameterVec3(num));
		parameter.update();
	}
	
	void parameter_vec4_pressed(int num) {
		PParameter parameter = parameters[num];
		property.setParameterVec4(num,getEditLineVec4(parameter.vector_el));
		setEditLineVec4(parameter.vector_el,property.getParameterVec4(num));
		parameter.update();
	}
	
	/*
	 */
	void parameter_mask_pressed(int num) {
		PParameter parameter = parameters[num];
		property.setParameterMask(num,int("0x" + parameter.mask_el.getText()));
		parameter.mask_el.setText(editorFormatMask(property.getParameterMask(num)));
		parameter.update();
	}
	
	void parameter_mask_changed(int mask) {
		PParameter parameter = parameters[parameter_num];
		property.setParameterMask(parameter_num,mask);
		parameter.mask_el.setText(editorFormatMask(property.getParameterMask(parameter_num)));
	}
	
	void parameter_mask_double_clicked(int num) {
		parameter_num = num;
		PParameter parameter = parameters[num];
		dialogMask(format(TR("Select %s mask"),parameters[num].name),functionid(parameter_mask_changed),property.getParameterMask(num));
		parameter.update();
	}
	
	/*
	 */
	void parameter_clear_clicked(int num) {
		PParameter parameter = parameters[num];
		int type = parameter.type;
		if(dialogMessageYesNo(TR("Confirm"),format(TR("Reset the %s parameter?"),parameter.name))) {
			if(type == PROPERTY_PARAMETER_INT) {
				int value = parent.getParameterInt(parent.findParameter(parameter.name));
				property.setParameterInt(num,value);
				parameter.slider_el.setText(string(value));
			}
			else if(type == PROPERTY_PARAMETER_FLOAT) {
				float value = parent.getParameterFloat(parent.findParameter(parameter.name));
				property.setParameterFloat(num,value);
				parameter.slider_el.setText(editorFormat(value));
			}
			else if(type == PROPERTY_PARAMETER_DOUBLE) {
				double value = parent.getParameterDouble(parent.findParameter(parameter.name));
				property.setParameterDouble(num,value);
				parameter.slider_el.setText(editorFormat(value));
			}
			else if(type == PROPERTY_PARAMETER_TOGGLE) {
				int value = parent.getParameterToggle(parent.findParameter(parameter.name));
				property.setParameterToggle(num,value);
				parameter.toggle_cb.setChecked(value);
			}
			else if(type == PROPERTY_PARAMETER_SWITCH) {
				int value = parent.getParameterSwitch(parent.findParameter(parameter.name));
				property.setParameterSwitch(num,value);
				parameter.switch_cb.setCurrentItem(value);
			}
			else if(type == PROPERTY_PARAMETER_STRING) {
				string value = parent.getParameterString(parent.findParameter(parameter.name));
				property.setParameterString(num,value);
				parameter.string_el.setText(value);
			}
			else if(type == PROPERTY_PARAMETER_COLOR) {
				vec4 value = parent.getParameterColor(parent.findParameter(parameter.name));
				property.setParameterColor(num,value);
				setSpriteColor(parameter.color_s,value);
			}
			else if(type == PROPERTY_PARAMETER_VEC3) {
				vec3 value = parent.getParameterVec3(parent.findParameter(parameter.name));
				property.setParameterVec3(num,value);
				setEditLineVec3(parameter.vector_el,value);
			}
			else if(type == PROPERTY_PARAMETER_VEC4) {
				vec4 value = parent.getParameterVec4(parent.findParameter(parameter.name));
				property.setParameterVec4(num,value);
				setEditLineVec4(parameter.vector_el,value);
			}
			else if(type == PROPERTY_PARAMETER_MASK) {
				int value = parent.getParameterMask(parent.findParameter(parameter.name));
				property.setParameterMask(num,value);
				parameter.mask_el.setText(editorFormatMask(value));
			}
		}
		parameter.update();
	}
	
	/*
	 */
	void update_parameters() {
		
		// delete parameters
		parameters.delete();
		
		// create parameters
		forloop(int i = 0; property.getNumParameters()) {
			parameters.append(new PParameter(i));
		}
	}
	
	/**************************************************************************\
	*
	* States
	*
	\**************************************************************************/
	
	/*
	 */
	PState states[0];
	
	/*
	 */
	class PState {
		
		int num;					// state number
		string name;				// state name
		int type;					// state type
		int hidden;					// state hidden
		
		WidgetLabel name_l;			// name label
		WidgetVBox main_hb;			// main hbox
		
		WidgetIcon clear_i;			// state clear
		
		WidgetCheckBox toggle_cb;	// toggle checkbox
		WidgetComboBox switch_cb;	// switch combobox
		
		// update
		void update() {
			int override = (parent.findState(name) != -1 && property.getState(num) != parent.getState(parent.findState(name)));
			vec4 color = override ? color_green : vec4_zero;
			name_l.setFontColor(color);
			clear_i.setEnabled(override);
			if(toggle_cb != NULL) toggle_cb.setFontColor(color);
		}
		
		// constructor
		PState(int n) {
			
			num = n;
			name = property.getStateName(num);
			type = property.getStateType(num);
			hidden = property.isStateHidden(num);
			
			if(hidden) return;
			
			main_hb = new WidgetHBox(gui);
			
			clear_i = new WidgetIcon(gui,"editor/gui/action_clear.png");
			clear_i.setWidth(24);
			clear_i.setHeight(24);
			clear_i.setToolTip(TR("Reset"));
			
			clear_i.setCallback(GUI_CLICKED,functionid(state_clear_clicked),num);
			
			// toggle state
			if(type == PROPERTY_STATE_TOGGLE) {
				
				name_l = new WidgetLabel(gui);
				
				toggle_cb = new WidgetCheckBox(gui,name);
				toggle_cb.setChecked(property.getState(num));
				main_hb.addChild(toggle_cb,GUI_ALIGN_LEFT);
				
				toggle_cb.setCallback(GUI_CLICKED,functionid(state_toggle_clicked),num);
				
				states_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				states_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				states_gb.addChild(clear_i);
			}
			// switch state
			else if(type == PROPERTY_STATE_SWITCH) {
				
				name_l = new WidgetLabel(gui,name + ":");
				
				switch_cb = new WidgetComboBox(gui);
				forloop(int i = 0; property.getStateSwitchNumItems(num)) {
					switch_cb.addItem(property.getStateSwitchItem(num,i));
				}
				switch_cb.setCurrentItem(property.getState(num));
				main_hb.addChild(switch_cb,GUI_ALIGN_EXPAND);
				
				switch_cb.setCallback(GUI_CHANGED,functionid(state_switch_changed),num);
				
				states_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				states_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				states_gb.addChild(clear_i);
			}
			else {
				throw("Properties::Editor::PState::PState(): unknown state type\n");
			}
			
			update();
		}
		
		~PState() {
			
			delete name_l;
			delete main_hb;
			
			delete clear_i;
			
			delete toggle_cb;
			delete switch_cb;
		}
	};
	
	/*
	 */
	void state_toggle_clicked(int num) {
		PState state = states[num];
		state.toggle_cb.setCallbackEnabled(GUI_CLICKED,0);
		property.setState(num,state.toggle_cb.isChecked());
		state.toggle_cb.setChecked(property.getState(num));
		state.toggle_cb.setCallbackEnabled(GUI_CLICKED,1);
		state.update();
		update_parameters();
	}
	
	void state_switch_changed(int num) {
		PState state = states[num];
		state.switch_cb.setCallbackEnabled(GUI_CHANGED,0);
		property.setState(num,state.switch_cb.getCurrentItem());
		state.switch_cb.setCurrentItem(property.getState(num));
		state.switch_cb.setCallbackEnabled(GUI_CHANGED,1);
		state.update();
		update_parameters();
	}
	
	/*
	 */
	void state_clear_clicked(int num) {
		PState state = states[num];
		int type = state.type;
		if(dialogMessageYesNo(TR("Confirm"),format(TR("Reset the %s state?"),state.name))) {
			if(type == PROPERTY_STATE_TOGGLE) {
				WidgetCheckBox toggle_cb = state.toggle_cb;
				int value = parent.getState(parent.findState(state.name));
				property.setState(num,value);
				toggle_cb.setCallbackEnabled(GUI_CLICKED,0);
				toggle_cb.setChecked(value);
				toggle_cb.setCallbackEnabled(GUI_CLICKED,1);
			}
			else if(type == PROPERTY_STATE_SWITCH) {
				WidgetComboBox switch_cb = state.switch_cb;
				int value = parent.getState(parent.findState(state.name));
				property.setState(num,value);
				switch_cb.setCallbackEnabled(GUI_CHANGED,0);
				switch_cb.setCurrentItem(value);
				switch_cb.setCallbackEnabled(GUI_CHANGED,1);
			}
		}
		state.update();
	}
	
	/*
	 */
	void update_states() {
		
		// delete states
		states.delete();
		
		// create states
		forloop(int i = 0; property.getNumStates()) {
			states.append(new PState(i));
		}
	}
	
	/**************************************************************************\
	*
	* Property
	*
	\**************************************************************************/
	
	/*
	 */
	void update_view() {
		
		// disable callbacks
		setCallbackEnabled(GUI_CLICKED,0,(
			intersection_cb,
			collision_cb,
		));
		
		// delete resources
		states.delete();
		parameters.delete();
		
		// parent property
		parent = property.getParent();
		
		// info
		int library = engine.properties.findPropertyLibrary(property.getName());
		info_library_l.setText(replace(basename(engine.properties.getLibraryName(library)),".prop",""));
		info_name_l.setText(property.getName());
		
		// options
		intersection_cb.setChecked(property.getIntersection());
		collision_cb.setChecked(property.getCollision());
		
		// update
		update_colors();
		update_states();
		update_parameters();
		
		// buttons
		paste_b.setEnabled(blob != NULL);
		
		// enable callbacks
		setCallbackEnabled(GUI_CLICKED,1,(
			intersection_cb,
			collision_cb,
		));
	}
	
	/*
	 */
	void option_clicked(string option) {
		if(option == "intersection") property.setIntersection(intersection_cb.isChecked());
		else if(option == "collision") property.setCollision(collision_cb.isChecked());
		else throw("Properties::Editor::option_clicked(): unknown option %s\n",option);
		update_parameters();
		update_states();
		update_colors();
	}
	
	/*
	 */
	void update_colors() {
	
		parent = property.getParent();
		
		// options
		intersection_cb.setFontColor((parent != NULL && parent.getIntersection() != property.getIntersection()) ? color_green : vec4_zero);
		collision_cb.setFontColor((parent != NULL && parent.getCollision() != property.getCollision()) ? color_green : vec4_zero);
	}
	
	/*
	 */
	void copy_clicked() {
		delete blob;
		
		blob = new Blob(0);
		property.saveState(blob,1);
		blob.seekSet(0);
		
		update_view();
	}
	
	void paste_clicked() {
		save_state(create_state(property,Unredo::STATE_MODIFY));
		property.restoreState(blob,1);
		blob.seekSet(0);
		update_view();
	}
	
	/**************************************************************************\
	*
	* Dialog
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		property_tb.setCurrentTab(0);
		
		editorDialog(dialog);
		
		DECLARE_DIALOG_INIT(Properties::Editor)
	}
	
	void shutdown() {
		
	}
	
	/*
	 */
	void run(Property p) {
		
		property = p;
		update_view();
		
		blob = new Blob(0);
		property.saveState(blob,1);
		blob.seekSet(0);
		
		DECLARE_DIALOG_SHOW
		while(dialog.isDone() == 0) wait 1;
		DECLARE_DIALOG_HIDE
		
		if(dialog.isCancelClicked()) property.restoreState(blob);
		delete blob;
		
		return dialog.isOkClicked();
	}
	
} /* namespace Editor */
