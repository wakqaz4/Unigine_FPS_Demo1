/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_properties_buffer.h
 * Desc:    Unigine editor
 * Version: 1.05
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
Buffer buffer;

/*
 */
class Buffer {
	
	Property property;	// property
	
	int options;		// common
	
	int states[];		// states
	int parameters[];	// parameters
	
	// constructor
	Buffer(Property p) {
		
		property = p;
		
		// copy common options
		for(int i = 0, j = 1; i < option_names.size(); i++, j *= 2) {
			if(get_option(property,i)) options |= j;
		}
		
		// copy states
		forloop(int i = 0; property.getNumStates()) {
			states.append(property.getStateName(i),property.getState(i));
		}
		
		// copy parameters
		forloop(int i = 0; property.getNumParameters()) {
			int type = property.getParameterType(i);
			string name = property.getParameterName(i);
			if(type == PROPERTY_PARAMETER_INT) parameters.append(name,property.getParameterInt(i));
			else if(type == PROPERTY_PARAMETER_FLOAT) parameters.append(name,property.getParameterFloat(i));
			else if(type == PROPERTY_PARAMETER_TOGGLE) parameters.append(name,property.getParameterToggle(i));
			else if(type == PROPERTY_PARAMETER_SWITCH) parameters.append(name,property.getParameterSwitch(i));
			else if(type == PROPERTY_PARAMETER_STRING) parameters.append(name,property.getParameterString(i));
			else if(type == PROPERTY_PARAMETER_COLOR) parameters.append(name,property.getParameterColor(i));
			else if(type == PROPERTY_PARAMETER_VEC3) parameters.append(name,property.getParameterVec3(i));
			else if(type == PROPERTY_PARAMETER_VEC4) parameters.append(name,property.getParameterVec4(i));
			else if(type == PROPERTY_PARAMETER_MASK) parameters.append(name,property.getParameterMask(i));
		}
	}
	
	// restore
	void restore(Property property) {
		
		// paste common options
		for(int i = 0, j = 1; i < option_names.size(); i++, j *= 2) {
			set_option(property,i,(options & j) != 0);
		}
		
		// paste states
		foreachkey(string name; states) {
			int id = property.findState(name);
			if(id != -1) property.setState(id,states[name]);
		}
		
		// paste parameters
		foreachkey(string name; parameters) {
			int id = property.findParameter(name);
			if(id != -1) {
				int type = property.getParameterType(id);
				if(type == PROPERTY_PARAMETER_INT) property.setParameterInt(id,parameters[name]);
				else if(type == PROPERTY_PARAMETER_FLOAT) property.setParameterFloat(id,parameters[name]);
				else if(type == PROPERTY_PARAMETER_TOGGLE) property.setParameterToggle(id,parameters[name]);
				else if(type == PROPERTY_PARAMETER_SWITCH) property.setParameterSwitch(id,parameters[name]);
				else if(type == PROPERTY_PARAMETER_STRING) property.setParameterString(id,parameters[name]);
				else if(type == PROPERTY_PARAMETER_COLOR) property.setParameterColor(id,parameters[name]);
				else if(type == PROPERTY_PARAMETER_VEC3) property.setParameterVec3(id,parameters[name]);
				else if(type == PROPERTY_PARAMETER_VEC4) property.setParameterVec4(id,parameters[name]);
				else if(type == PROPERTY_PARAMETER_MASK) property.setParameterMask(id,parameters[name]);
			}
		}
	}
};

/*
 */
void copy_clicked() {
	delete buffer;
	buffer = new Buffer(property);
	update_view();
}

void paste_clicked() {
	save_state(create_state(property,Unredo::STATE_MODIFY));
	buffer.restore(property);
	update_view();
}
