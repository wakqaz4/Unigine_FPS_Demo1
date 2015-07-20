/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_properties_parameters.h
 * Desc:    Unigine editor
 * Version: 1.08
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
PParameter parameters[0];

/*
 */
enum {
	PARAMETER_GROUP_DEFAULT = 0,
	PARAMETER_GROUP_PHYSICS,
	PARAMETER_GROUP_SOUND,
};

string parameter_titles[] = (
	PARAMETER_GROUP_PHYSICS		: TR("Physics"),
	PARAMETER_GROUP_SOUND		: TR("Sound"),
);

Unigine::Pair parameter_names[] = (
	"friction"			: new Unigine::Pair(TR("Friction"),		PARAMETER_GROUP_PHYSICS),
	"restitution"		: new Unigine::Pair(TR("Restitution"),	PARAMETER_GROUP_PHYSICS),
	"occlusion"			: new Unigine::Pair(TR("Occlusion"),	PARAMETER_GROUP_SOUND),
);

string parameter_items[];


/*
 */
string get_parameter_name(string name) {
	Property p = property;
	while(p != NULL) {
		if(parameter_names.check(p.getName() + "_" + name)) return TR(parameter_names[p.getName() + "_" + name].first);
		p = p.getParent();
	}
	if(parameter_names.check(name)) return TR(parameter_names[name].first);
	return name;
}

int get_parameter_group(string name) {
	Property p = property;
	while(p != NULL) {
		if(parameter_names.check(p.getName() + "_" + name)) return parameter_names[p.getName() + "_" + name].second;
		p = p.getParent();
	}
	if(parameter_names.check(name)) return parameter_names[name].second;
	return PARAMETER_GROUP_DEFAULT;
}

string get_parameter_title(int group) {
	if(parameter_titles.check(group)) return TR(parameter_titles[group]);
	return "";
}

string get_parameter_item(string name) {
	Property p = property;
	while(p != NULL) {
		if(parameter_items.check(p.getName() + "_" + name)) return TR(parameter_items[p.getName() + "_" + name]);
		p = p.getParent();
	}
	if(parameter_items.check(name)) return TR(parameter_items[name]);
	return name;
}

/*
 */
class PParameter {
	
	int num;						// parameter number
	string name;					// parameter name
	int type;						// parameter type
	int hidden;						// parameter hidden
	int group;						// parameter group
	
	WidgetGroupBox group_gb;		// parameter group
	WidgetGridBox grid_gb;			// parameter grid
	
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
	
	WidgetEditLine mask_el;			// mask value
	
	// update
	void update() {
		int override = (parent != NULL && parent.findParameter(name) != -1);
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
		group = get_parameter_group(name);
		
		if(hidden) return;
		
		main_hb = new WidgetHBox(gui);
		
		clear_i = new WidgetIcon(gui,"editor/gui/action_clear.png");
		clear_i.setWidth(24);
		clear_i.setHeight(24);
		clear_i.setToolTip(TR("Reset"));
		
		clear_i.setCallback(GUI_CLICKED,functionid(parameter_clear_clicked),num);
		
		// create group
		PParameter parent = NULL;
		for(int i = num - 1; i >= 0; i--) {
			if(parameters[i].hidden) continue;
			if(parameters[i].group_gb == NULL) continue;
			parent = parameters[i];
			break;
		}
		if(parent == NULL || parent.group != group) {
			group_gb = new WidgetGroupBox(gui,get_parameter_title(group),2,2);
			parameters_vb.addChild(group_gb,GUI_ALIGN_EXPAND);
			grid_gb = new WidgetGridBox(gui,3,2,2);
			group_gb.addChild(grid_gb,GUI_ALIGN_EXPAND);
		} else {
			group_gb = parent.group_gb;
			grid_gb = parent.grid_gb;
		}
		
		// int parameter
		if(type == PROPERTY_PARAMETER_INT) {
			
			name_l = new WidgetLabel(gui,get_parameter_name(name) + ":");
			
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
			slider_sl.setCallback(GUI_CLICKED,functionid(slider_clicked));
			slider_sl.setCallback(GUI_CHANGED,functionid(slider_changed));
			main_hb.addChild(slider_sl,GUI_ALIGN_EXPAND);
			
			slider_el.setText(string(value));
			
			slider_el.setCallback(GUI_PRESSED,functionid(parameter_int_pressed),num);
			slider_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_int_pressed),num);
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		// float parameter
		else if(type == PROPERTY_PARAMETER_FLOAT) {
			
			name_l = new WidgetLabel(gui,get_parameter_name(name) + ":");
			
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
			slider_sl.setCallback(GUI_CLICKED,functionid(slider_clicked));
			slider_sl.setCallback(GUI_CHANGED,functionid(slider_changed));
			main_hb.addChild(slider_sl,GUI_ALIGN_EXPAND);
			
			slider_el.setText(editorFormat(value));
			
			slider_el.setCallback(GUI_PRESSED,functionid(parameter_float_pressed),num);
			slider_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_float_pressed),num);
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		// double parameter
		else if(type == PROPERTY_PARAMETER_DOUBLE) {
			
			name_l = new WidgetLabel(gui,get_parameter_name(name) + ":");
			
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
			slider_sl.setCallback(GUI_CLICKED,functionid(slider_clicked));
			slider_sl.setCallback(GUI_CHANGED,functionid(slider_changed));
			main_hb.addChild(slider_sl,GUI_ALIGN_EXPAND);
			
			slider_el.setText(editorFormat(value));
			
			slider_el.setCallback(GUI_PRESSED,functionid(parameter_double_pressed),num);
			slider_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_double_pressed),num);
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		// toggle parameter
		else if(type == PROPERTY_PARAMETER_TOGGLE) {
			
			name_l = new WidgetLabel(gui);
			
			toggle_cb = new WidgetCheckBox(gui,get_parameter_name(name));
			toggle_cb.setChecked(property.getParameterToggle(num));
			main_hb.addChild(toggle_cb,GUI_ALIGN_LEFT);
			
			toggle_cb.setCallback(GUI_CLICKED,functionid(parameter_toggle_clicked),num);
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		// switch parameter
		else if(type == PROPERTY_PARAMETER_SWITCH) {
			
			name_l = new WidgetLabel(gui,get_parameter_name(name) + ":");
			
			switch_cb = new WidgetComboBox(gui);
			forloop(int i = 0; property.getParameterSwitchNumItems(num)) {
				switch_cb.addItem(get_parameter_item(property.getParameterSwitchItem(num,i)));
			}
			switch_cb.setCurrentItem(property.getParameterSwitch(num));
			main_hb.addChild(switch_cb,GUI_ALIGN_EXPAND);
			
			switch_cb.setCallback(GUI_CHANGED,functionid(parameter_switch_changed),num);
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		// string parameter
		else if(type == PROPERTY_PARAMETER_STRING) {
			
			name_l = new WidgetLabel(gui,get_parameter_name(name) + ":");
			
			string_el = new WidgetEditLine(gui,property.getParameterString(num));
			main_hb.addChild(string_el,GUI_ALIGN_EXPAND);
			
			string_el.setCallback(GUI_PRESSED,functionid(parameter_string_pressed),num);
			string_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_string_pressed),num);
			
			if(property.getParameterStringFile(num)) {
				string_el.setCallback(GUI_DOUBLE_CLICKED,functionid(parameter_string_double_clicked),num);
			}
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		// color parameter
		else if(type == PROPERTY_PARAMETER_COLOR) {
			
			name_l = new WidgetLabel(gui,get_parameter_name(name) + ":");
			
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
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
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
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
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
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		// mask parameter
		else if(type == PROPERTY_PARAMETER_MASK) {
			
			name_l = new WidgetLabel(gui,get_parameter_name(name) + ":");
			
			int value = property.getParameterMask(num);
			
			mask_el = new WidgetEditLine(gui);
			main_hb.addChild(mask_el,GUI_ALIGN_EXPAND);
			
			mask_el.setText(editorFormatMask(value));
			
			mask_el.setCallback(GUI_PRESSED,functionid(parameter_mask_pressed),num);
			mask_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_mask_pressed),num);
			mask_el.setCallback(GUI_DOUBLE_CLICKED,functionid(parameter_mask_double_clicked),num);
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		else {
			throw("Properties::PParameter::PParameter(): unknown parameter type\n");
		}
		
		update();
	}
	
	~PParameter() {
		
		delete group_gb;
		delete grid_gb;
		
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

DECLARE_SLIDER_CALLBACKS

/*
 */
void parameter_int_pressed(int num) {
	PParameter parameter = parameters[num];
	int value = int(parameter.slider_el.getText());
	if(property.getParameterInt(num) != value) {
		if(skip_state == false) save_state(create_state(property,Unredo::STATE_MODIFY));
		property.setParameterInt(num,value);
		parameter.slider_el.setText(string(property.getParameterInt(num)));
		if(toolsGetSelection() && parameter.slider_el.isFocused()) {
			parameter.slider_el.setCursor(0);
			parameter.slider_el.setSelection(1024);
		}
		parameter.update();
	}
}

/*
 */
void parameter_float_pressed(int num) {
	PParameter parameter = parameters[num];
	float value = float(parameter.slider_el.getText());
	if(property.getParameterFloat(num) != value) {
		if(skip_state == false) save_state(create_state(property,Unredo::STATE_MODIFY));
		property.setParameterFloat(num,value);
		parameter.slider_el.setText(editorFormat(property.getParameterFloat(num)));
		if(toolsGetSelection() && parameter.slider_el.isFocused()) {
			parameter.slider_el.setCursor(0);
			parameter.slider_el.setSelection(1024);
		}
		parameter.update();
	}
}

/*
 */
void parameter_double_pressed(int num) {
	PParameter parameter = parameters[num];
	double value = double(parameter.slider_el.getText());
	if(property.getParameterDouble(num) != value) {
		if(skip_state == false) save_state(create_state(property,Unredo::STATE_MODIFY));
		property.setParameterDouble(num,value);
		parameter.slider_el.setText(editorFormat(property.getParameterDouble(num)));
		if(toolsGetSelection() && parameter.slider_el.isFocused()) {
			parameter.slider_el.setCursor(0);
			parameter.slider_el.setSelection(1024);
		}
		parameter.update();
	}
}

/*
 */
void parameter_toggle_clicked(int num) {
	PParameter parameter = parameters[num];
	WidgetCheckBox toggle_cb = parameter.toggle_cb;
	if(property.getParameterToggle(num) != toggle_cb.isChecked()) {
		save_state(create_state(property,Unredo::STATE_MODIFY));
		property.setParameterToggle(num,toggle_cb.isChecked());
		parameter.update();
	}
}

/*
 */
void parameter_switch_changed(int num) {
	PParameter parameter = parameters[num];
	WidgetComboBox switch_cb = parameter.switch_cb;
	if(property.getParameterSwitch(num) != switch_cb.getCurrentItem()) {
		save_state(create_state(property,Unredo::STATE_MODIFY));
		property.setParameterSwitch(num,switch_cb.getCurrentItem());
		parameter.update();
	}
}

/*
 */
void parameter_string_pressed(int num) {
	PParameter parameter = parameters[num];
	WidgetEditLine string_el = parameter.string_el;
	if(property.getParameterString(num) != string_el.getText()) {
		save_state(create_state(property,Unredo::STATE_MODIFY));
		property.setParameterString(num,string_el.getText());
		parameter.update();
	}
}

void parameter_string_double_clicked(int num) {
	PParameter parameter = parameters[num];
	string name = property.getParameterString(num);
	if(dialogFile(format(TR("Select %s file"),get_parameter_name(parameters[num].name)),"",name,DIALOG_FILE_OPEN) == 0) return;
	if(property.getParameterString(num) != name) {
		save_state(create_state(property,Unredo::STATE_MODIFY));
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
	PropertyState_ s = create_state(property,Unredo::STATE_MODIFY);
	vec4 old_color = property.getParameterColor(num);
	parameter_num = num;
	PParameter parameter = parameters[num];
	dialogColor(format(TR("Select %s color"),parameters[num].name),functionid(parameter_color_changed),property.getParameterColor(num));
	if(property.getParameterColor(num) != old_color) save_state(s);
	else delete s;
	parameter.update();
}

/*
 */
void parameter_vec3_pressed(int num) {
	PParameter parameter = parameters[num];
	if(property.getParameterVec3(num) != getEditLineVec3(parameter.vector_el)) {
		save_state(create_state(property,Unredo::STATE_MODIFY));
		property.setParameterVec3(num,getEditLineVec3(parameter.vector_el));
		setEditLineVec3(parameter.vector_el,property.getParameterVec3(num));
		foreach(WidgetEditLine editline; parameter.vector_el) {
			if(toolsGetSelection() && editline.isFocused()) {
				editline.setCursor(0);
				editline.setSelection(1024);
			}		
		}
		parameter.update();
	}
}

void parameter_vec4_pressed(int num) {
	PParameter parameter = parameters[num];
	if(property.getParameterVec4(num) != getEditLineVec4(parameter.vector_el)) {
		save_state(create_state(property,Unredo::STATE_MODIFY));
		property.setParameterVec4(num,getEditLineVec4(parameter.vector_el));
		setEditLineVec4(parameter.vector_el,property.getParameterVec4(num));
		foreach(WidgetEditLine editline; parameter.vector_el) {
			if(toolsGetSelection() && editline.isFocused()) {
				editline.setCursor(0);
				editline.setSelection(1024);
			}		
		}
		parameter.update();
	}
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
	PropertyState_ s = create_state(property,Unredo::STATE_MODIFY);
	int old_mask = property.getParameterMask(num);
	parameter_num = num;
	PParameter parameter = parameters[num];
	dialogMask(format(TR("Select %s mask"),parameters[num].name),functionid(parameter_mask_changed),old_mask);
	if(property.getParameterMask(num) != old_mask) save_state(s);
	else delete s;
	parameter.update();
}

/*
 */
void parameter_clear_clicked(int num) {
	PParameter parameter = parameters[num];
	int type = parameter.type;
	if(dialogMessageYesNo(TR("Confirm"),format(TR("Reset the %s parameter?"),parameter.name))) {
		if(parent != NULL && parent.findParameter(parameter.name) != -1) {
			save_state(create_state(property,Unredo::STATE_MODIFY));
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
	
	// attach parameters
	PParameter last_parameter = NULL;
	for(int i = parameters.size() - 1; i >= 0; i--) {
		if(parameters[i].grid_gb == NULL) continue;
		last_parameter = parameters[i];
		break;
	}
	foreach(PParameter p; parameters) {
		if(p.grid_gb == NULL) continue;
		if(p.grid_gb == last_parameter.grid_gb) continue;
		last_parameter.grid_gb.addAttach(p.grid_gb);
	}
	
	// next focus
	Widget first = NULL;
	Widget previous = NULL;
	foreach(PParameter p; parameters) {
		if(p.hidden) continue;
		Widget widget = NULL;
		if(p.type == PROPERTY_PARAMETER_INT) widget = p.slider_el;
		else if(p.type == PROPERTY_PARAMETER_FLOAT) widget = p.slider_el;
		else if(p.type == PROPERTY_PARAMETER_DOUBLE) widget = p.slider_el;
		else if(p.type == PROPERTY_PARAMETER_TOGGLE) widget = p.toggle_cb;
		else if(p.type == PROPERTY_PARAMETER_SWITCH) widget = p.switch_cb;
		else if(p.type == PROPERTY_PARAMETER_STRING) widget = p.string_el;
		else if(p.type == PROPERTY_PARAMETER_VEC3) widget = p.vector_el[0];
		else if(p.type == PROPERTY_PARAMETER_VEC4) widget = p.vector_el[0];
		if(widget != NULL) {
			if(first == NULL) first = widget;
			if(previous != NULL) previous.setNextFocus(widget);
			previous = widget;
		}
	}
	if(previous != NULL) previous.setNextFocus(first);
}
