/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_properties_states.h
 * Desc:    Unigine editor
 * Version: 1.03
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
PState states[0];

/*
 */
enum {
	STATE_GROUP_DEFAULT = 0,
};

string state_titles[];
Unigine::Pair state_names[];
string state_items[];

/*
 */
string get_state_name(string name) {
	Property p = property;
	while(p != NULL) {
		if(state_names.check(p.getName() + "_" + name)) return state_names[p.getName() + "_" + name].first;
		p = p.getParent();
	}
	if(state_names.check(name)) return state_names[name].first;
	return name;
}

int get_state_group(string name) {
	Property p = property;
	while(p != NULL) {
		if(state_names.check(p.getName() + "_" + name)) return state_names[p.getName() + "_" + name].second;
		p = p.getParent();
	}
	if(state_names.check(name)) return state_names[name].second;
	return STATE_GROUP_DEFAULT;
}

string get_state_title(int group) {
	if(state_titles.check(group)) return state_titles[group];
	return "";
}

string get_state_item(string name) {
	Property p = property;
	while(p != NULL) {
		if(state_items.check(p.getName() + "_" + name)) return state_items[p.getName() + "_" + name];
		p = p.getParent();
	}
	if(state_items.check(name)) return state_items[name];
	return name;
}

/*
 */
class PState {
	
	int num;					// state number
	string name;				// state name
	int type;					// state type
	int hidden;					// state hidden
	int group;					// state group
	
	WidgetGroupBox group_gb;	// state group
	WidgetGridBox grid_gb;		// state grid
	
	WidgetLabel name_l;			// name label
	WidgetVBox main_vb;			// main vbox
	
	WidgetIcon clear_i;			// state clear
	
	WidgetCheckBox toggle_cb;	// toggle checkbox
	WidgetComboBox switch_cb;	// switch combobox
	
	// update
	void update() {
		int override = (parent != NULL && parent.findState(name) != -1 && property.getState(num) != parent.getState(parent.findState(name)));
		vec4 color = override ? color_green : vec4_zero;
		name_l.setFontColor(color);
		clear_i.setEnabled(override);
		if(toggle_cb != NULL) toggle_cb.setFontColor(color);
	}
	
	// focus
	Widget get_focus() {
		if(type == PROPERTY_STATE_TOGGLE) return toggle_cb;
		if(type == PROPERTY_STATE_SWITCH) return switch_cb;
		return NULL;
	}
	
	int is_focused() {
		Widget focus = gui.getFocus();
		if(focus == name_l) return 1;
		if(focus == main_vb) return 1;
		if(focus == clear_i) return 1;
		if(focus == toggle_cb) return 1;
		if(focus == switch_cb) return 1;
		return 0;
	}
	
	// constructor
	PState(int n) {
		
		num = n;
		name = property.getStateName(num);
		type = property.getStateType(num);
		hidden = property.isStateHidden(num);
		group = get_state_group(name);
		
		if(hidden) return;
		
		main_vb = new WidgetHBox(gui);
		
		clear_i = new WidgetIcon(gui,"editor/gui/action_clear.png");
		clear_i.setWidth(24);
		clear_i.setHeight(24);
		clear_i.setToolTip(TR("Reset"));
		
		clear_i.setCallback(GUI_CLICKED,functionid(state_clear_clicked),num);
		
		// create group
		PState parent = NULL;
		for(int i = num - 1; i >= 0; i--) {
			if(states[i].hidden) continue;
			if(states[i].group_gb == NULL) continue;
			parent = states[i];
			break;
		}
		if(parent == NULL || parent.group != group) {
			group_gb = new WidgetGroupBox(gui,get_state_title(group),2,2);
			states_vb.addChild(group_gb,GUI_ALIGN_EXPAND);
			grid_gb = new WidgetGridBox(gui,3,2,2);
			group_gb.addChild(grid_gb,GUI_ALIGN_EXPAND);
		} else {
			group_gb = parent.group_gb;
			grid_gb = parent.grid_gb;
		}
		
		// toggle state
		if(type == PROPERTY_STATE_TOGGLE) {
			
			name_l = new WidgetLabel(gui);
			
			toggle_cb = new WidgetCheckBox(gui,get_state_name(name));
			toggle_cb.setChecked(property.getState(num));
			main_vb.addChild(toggle_cb,GUI_ALIGN_LEFT);
			
			toggle_cb.setCallback(GUI_CLICKED,functionid(state_toggle_clicked),num);
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_vb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		// switch state
		else if(type == PROPERTY_STATE_SWITCH) {
			
			name_l = new WidgetLabel(gui,get_state_name(name) + ":");
			
			switch_cb = new WidgetComboBox(gui);
			forloop(int i = 0; property.getStateSwitchNumItems(num)) {
				switch_cb.addItem(get_state_item(property.getStateSwitchItem(num,i)));
			}
			switch_cb.setCurrentItem(property.getState(num));
			main_vb.addChild(switch_cb,GUI_ALIGN_EXPAND);
			
			switch_cb.setCallback(GUI_CHANGED,functionid(state_switch_changed),num);
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_vb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		else {
			throw("Properties::PState::PState(): unknown state type\n");
		}
		
		update();
	}
	
	~PState() {
		
		delete group_gb;
		delete grid_gb;
		
		delete name_l;
		delete main_vb;
		
		delete clear_i;
		
		delete toggle_cb;
		delete switch_cb;
	}
};

/*
 */
void state_toggle_clicked(int num) {
	save_state(create_state(property,Unredo::STATE_MODIFY));
	PState state = states[num];
	state.toggle_cb.setCallbackEnabled(GUI_CLICKED,0);
	property.setState(num,state.toggle_cb.isChecked());
	state.toggle_cb.setChecked(property.getState(num));
	state.toggle_cb.setCallbackEnabled(GUI_CLICKED,1);
	state.update();
	update_parameters();
}

void state_switch_changed(int num) {
	save_state(create_state(property,Unredo::STATE_MODIFY));
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
		if(parent != NULL && parent.findState(state.name) != -1) {
			save_state(create_state(property,Unredo::STATE_MODIFY));
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
	}
	state.update();
	update_parameters();
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
	
	// attach states
	PState last_state = NULL;
	for(int i = states.size() - 1; i >= 0; i--) {
		if(states[i].grid_gb == NULL) continue;
		last_state = states[i];
		break;
	}
	foreach(PState s; states) {
		if(s.grid_gb == NULL) continue;
		if(s.grid_gb == last_state.grid_gb) continue;
		last_state.grid_gb.addAttach(s.grid_gb);
	}
}
