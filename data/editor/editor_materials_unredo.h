/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_materials_unredo.h
 * Desc:    Unigine editor
 * Version: 1.04
 * Author:  Valia Vaneeva <fattie@unigine.com>
 *          Alexander Zapryagaev <frustum@unigine.com>
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
class MaterialState_ : Unredo::State {
	
	int state_type;		// state type
	
	int tab;			// current tab
	
	string name;		// material name
	string parent;		// parent material name
	string library;		// library name
	string selected;	// selected material name
	
	Material material;	// material pointer
	
	Blob blob;			// blob
	
	MaterialState_ states[0];	// child states
	
	MaterialState_(Material mat,int t) {
		super.__State__(Unredo::STATE_MATERIAL,1);
		
		tab = material_tb.getCurrentTab();
		
		blob = new Blob(0);
		
		state_type = t;
		switch(state_type) {
			case Unredo::STATE_MODIFY	: init_modity(mat);		break;
			case Unredo::STATE_REMOVE	: init_remove(mat);		break;
			case Unredo::STATE_MOVE		: init_move(mat);		break;
			case Unredo::STATE_NEW		: init_new(mat);		break;
		}
	}
	
	~MaterialState_() {
		material = NULL;
		delete blob;
	}
	
	void init_new(Material m) {
		name = m.getName();
		material = m;
		Material p = material.getParent();
		if(p != NULL) parent = p.getName();
	}
	
	void undo_new() {
		engine.materials.removeMaterial(name);
	}
	
	void init_modity(Material m) {
		name = m.getName();
		material = m;
		material.saveState(blob,1);
		blob.seekSet(0);
	}
	
	void undo_modify() {
		material.restoreState(blob);
	}
	
	void init_remove(Material material) {
		name = material.getName();
		Material p = material.getParent();
		if(p != NULL) parent = p.getName();
		library = engine.materials.getLibraryName(engine.materials.findMaterialLibrary(name));
		
		material.saveState(blob,1);
		blob.seekSet(0);
		
		void process(State states[],Material material) {
			
			MaterialState_ state = new MaterialState_(material,Unredo::STATE_REMOVE);
			states.append(state);
			
			forloop(int i = 0; material.getNumChilds()) {
				process(states,material.getChild(i));
			}
		}
		
		forloop(int i = 0; material.getNumChilds()) {
			process(states,material.getChild(i));
		}
	}
	
	void undo_remove() {
		if(engine.materials.inheritMaterial(parent,library,name) == 0) return;
		
		material = engine.materials.findMaterial(name);
		if(material == NULL) return;
		
		material.restoreState(blob);
		
		foreach(MaterialState_ state; states) {
			state.undo_remove();
		}
	}
	
	void init_move(Material m) {
		name = m.getName();
		library = engine.materials.getLibraryName(engine.materials.findMaterialLibrary(name));
		material = m;
	}
	
	void undo_move() {
		engine.materials.moveMaterial(material.getName(),library,name);
	}
	
	MaterialState_ undo() {
		MaterialState_ result = NULL;
		if(state_type == Unredo::STATE_MODIFY) {
			if(get_material_name() != name || material_tb.getCurrentTab() != tab) {
				select_material(name);
				material_tb.setCurrentTab(tab);
				if(editorIsChild(material_tb)) material_tb.setFocus();
			}
			result = create_state(material,Unredo::STATE_MODIFY);
			undo_modify();
		}
		else if(state_type == Unredo::STATE_REMOVE) {
			undo_remove();
			result = create_state(material,Unredo::STATE_NEW);
			result.selected = parent;
			update_materials();
			select_material(name);
		}
		else if(state_type == Unredo::STATE_NEW) {
			result = create_state(material,Unredo::STATE_REMOVE);
			undo_new();
			update_materials();
			select_material(selected);
		}
		else if(state_type == Unredo::STATE_MOVE) {
			result = create_state(material,Unredo::STATE_MOVE);
			undo_move();
			
			update_materials();
			
			select_material(name);
		}
		
		window_hp.setFocus();
		
		update_view();
		
		return result;
	}
};

/*
 */
string option_names[0] = ( 
	"PostDeferred",
	"PostRefraction",
	"PostScattering",
	"DepthMask",
	"GlowMask",
	"ParallaxMask",
	"AlphaTest",
	"TwoSided",
	"CastShadow",
	"ReceiveShadow",
	"CastWorldShadow",
	"ReceiveWorldShadow",
	"CastTranslucent",
	"ReceiveTranslucent",
);

int get_option(Material material,int option) {
	assert(material != NULL);
	if(option_names.check(option)) return material.call("get" + option_names[option]);
	else throw("Materials::get_option(): unknown material option\n");
}

void set_option(Material material,int option,int value) {
	assert(material != NULL);
	if(option_names.check(option)) material.call("set" + option_names[option],value);
	else throw("Materials::set_option(): unknown material option\n");
}

MaterialState_ create_state(Material material,int type) {
	return new MaterialState_(material,type);
}

void save_state(MaterialState_ s,Widget widget = NULL) {
	
	if(s == NULL) return;
	
	if(s.material != NULL && s.material.isEditable() == 0) {
		delete s;
		return;
	}
	
	unredoAddState(s);
}

int undo() {
	
}

int redo() {
	
}
