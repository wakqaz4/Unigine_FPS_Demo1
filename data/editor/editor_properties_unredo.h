/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_properties_unredo.h
 * Desc:    Unigine editor
 * Version: 1.04
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
class PropertyState_ : Unredo::State {
	
	int state_type;		// state type
	
	int tab;			// current tab
	
	string name;		// property name
	string parent;		// parent property name
	string library;		// library name
	string selected;	// selected property name
	
	Property property;	// property pointer
	
	Blob blob;			// parameters blob
	
	PropertyState_ states[0];	// child states
	
	PropertyState_(Property p,int t) {
		super.__State__(STATE_PROPERTY);
		state_type = t;
		
		tab = property_tb.getCurrentTab();
		
		blob = new Blob(0);
		
		switch(state_type) {
			case Unredo::STATE_MODIFY	: init_modity(p);		break;
			case Unredo::STATE_REMOVE	: init_remove(p);		break;
			case Unredo::STATE_MOVE		: init_move(p);			break;
			case Unredo::STATE_NEW		: init_new(p);			break;
		}
	}
	
	~PropertyState_() {
		property = NULL;
		delete blob;
	}
	
	void init_new(Property p_) {
		name = p_.getName();
		property = p_;
		Property p = property.getParent();
		if(p != NULL) parent = p.getName();
	}
	
	void undo_new() {
		engine.properties.removeProperty(name);
	}
	
	void init_modity(Property p) {
		name = p.getName();
		property = p;
		property.saveState(blob,1);
		blob.seekSet(0);
	}
	
	void undo_modify() {
		property.restoreState(blob);
	}
	
	void init_remove(Property property) {
		name = property.getName();
		Property p = property.getParent();
		if(p != NULL) parent = p.getName();
		library = engine.properties.getLibraryName(engine.properties.findPropertyLibrary(name));
		
		property.saveState(blob,1);
		blob.seekSet(0);
		
		void process(PropertyState_ states[],Property property) {
			PropertyState_ state = new PropertyState_(property,Unredo::STATE_REMOVE);
			states.append(state);
			
			forloop(int i = 0; property.getNumChilds()) {
				process(states,property.getChild(i));
			}
		}
		
		forloop(int i = 0; property.getNumChilds()) {
			process(states,property.getChild(i));
		}
	}
	
	void undo_remove() {
		if(engine.properties.inheritProperty(parent,library,name) == 0) return;
		
		property = engine.properties.findProperty(name);
		if(property == NULL) return;
		
		property.restoreState(blob);
		
		foreach(PropertyState_ state; states) {
			state.undo_remove();
		}
	}
	
	void init_move(Property p) {
		name = p.getName();
		library = engine.properties.getLibraryName(engine.properties.findPropertyLibrary(name));
		property = p;
	}
	
	void undo_move() {
		engine.properties.moveProperty(property.getName(),library,name);
	}
	
	PropertyState_ undo() {
		PropertyState_ result = NULL;
		if(state_type == STATE_MODIFY) {
			if(get_property_name() != name || property_tb.getCurrentTab() != tab) {
				select_property(name);
				property_tb.setCurrentTab(tab);
				if(editorIsChild(property_tb)) property_tb.setFocus();
			}
			
			result = create_state(property,Unredo::STATE_MODIFY);
			undo_modify();
		}
		else if(state_type == STATE_REMOVE) {
			undo_remove();
			result = create_state(property,Unredo::STATE_NEW);
			result.selected = parent;
			update_properties();
			select_property(name);
		}
		else if(state_type == Unredo::STATE_NEW) {
			result = create_state(property,Unredo::STATE_REMOVE);
			undo_new();
			update_properties();
			select_property(selected);
		}
		else if(state_type == STATE_MOVE) {
			result = create_state(property,Unredo::STATE_MOVE);
			undo_move();
			update_properties();
			select_property(name);
		}
		
		window_hp.setFocus();
		
		update_view();
		
		return result;
	}
};

/******************************************************************************\
*
* States
*
\******************************************************************************/

PropertyState_ create_state(Property property,int type) {
	return new PropertyState_(property,type);
}

void save_state(PropertyState_ s) {
	
	if(s == NULL) return;
	
	if(s.property != NULL && s.property.isEditable() == 0) {
		delete s;
		return;
	}
	
	unredoAddState(s);
}

int undo() {
	
}

int redo() {
	
}
