/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_render_unredo.h
 * Desc:    Unigine editor
 * Version: 1.18
 * Author:  Valia Vaneeva <fattie@unigine.com>
 *          Alexander Zapryagaev <frustum@unigine.com>
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

class RenderState_ : Unredo::State {
	
	Widget focus;
	int tab;
	int hdr_tab;
	
	Blob blob;
	
	float world_budget;
	float world_distance;
	float physics_budget;
	float physics_distance;
	
	RenderState_() {
		super.__State__(Unredo::STATE_RENDER);
		
		focus = gui.getFocus();
		tab = tabbox.getCurrentTab();
		hdr_tab = HDR::get_tabbox_current_tab();
		
		blob = new Blob(0);
		engine.render.saveState(blob);
		blob.seekSet(0);
		
		world_budget = engine.world.getBudget();
		world_distance = engine.world.getDistance();
		physics_budget = engine.physics.getBudget();
		physics_distance = engine.physics.getDistance();
	}
	
	~RenderState_() {
		focus = NULL;
		delete blob;
	}
	
	RenderState_ undo() {
		if(editorIsChild(window) && tabbox.getCurrentTab() != tab) {
			tabbox.setCurrentTab(tab);
			tabbox.setFocus();
		}
		
		if(editorIsChild(window) && HDR::get_tabbox_current_tab() != hdr_tab) HDR::set_tabbox_current_tab(hdr_tab);
		
		RenderState_ result = new RenderState_();
		
		engine.render.restoreState(blob);
		
		engine.world.setBudget(world_budget);
		engine.world.setDistance(world_distance);
		engine.physics.setBudget(physics_budget);
		engine.physics.setDistance(physics_distance);
		
		// update view
		update(1);
		if(focus != NULL && editorIsChild(focus)) focus.setFocus();
		
		return result;
	}
};

/******************************************************************************\
*
* States
*
\******************************************************************************/

void save_state(RenderState_ state) {
	if(state == NULL) return;
	unredoAddState(state);
}

RenderState_ create_state() {
	return new RenderState_();
}

int undo() {
	
}

int redo() {
	
}
