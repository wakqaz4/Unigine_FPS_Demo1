/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_unredo.h
 * Desc:    Unigine editor
 * Version: 1.05
 * Author:  Valia Vaneeva <fattie@unigine.com>
 *          Alexander Zapryagaev <frustum@unigine.com>
 *          Pavel Kurinnoy <figonet@unigine.com>
 *          Andrey Viktorov <unclebob@unigine.com>
 *          Mikhail Lyubimov <alterego@unigine.com>
 *          Alexey Zagniy <yingaz@unigine.com>
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
namespace Unredo {
	
	/*
	 */
	class State {
		
		int type;
		int num = 1;
		int skip_dirty = false; // dont revert world dirty on undo/redo
		int is_clear = false; // state doesnt mark world as dirty
		
		State(int t,int n = 1) {
			type = t;
			num = n;
		}
		
		State undo() { return NULL; }
	};
	
	/*
	 */
	State undo_states[0];
	State redo_states[0];
	
	State clear_state;
	int skip_next_state;
	
	int is_dirty;								// is world dirty
	
	/*
	 */
	enum {
		STATE_NODE,
		STATE_MATERIAL,
		STATE_PROPERTY,
		STATE_ISOLATION,
		STATE_REFERENCE,
		STATE_RENDER,
		STATE_SOUND,
		STATE_PHYSICS,
		STATE_BRUSH_EDIT,
		STATE_BRUSH,
	};
	
	enum {
		STATE_NONE = -1,
		STATE_NEW,
		STATE_REMOVE,
		STATE_MODIFY,
		STATE_SELECT,
		STATE_SWAP,
		STATE_REPARENT,
		STATE_BODY,
		STATE_JOINT,
		STATE_MOVE,
	};
	
	/*
	 */
	string type_prefix[] = (
		STATE_NODE				: "Nodes::",
		STATE_MATERIAL			: "Materials::",
		STATE_PROPERTY			: "Properties::",
		STATE_ISOLATION			: "Isolation::",
		STATE_REFERENCE			: "Reference::",
		STATE_RENDER			: "Render::",
		STATE_SOUND				: "Sound::",
		STATE_PHYSICS			: "Physics::",
		STATE_BRUSH_EDIT		: "Brushes::",
		STATE_BRUSH				: "Brushes::",
	);
	
	/*
	 */
	int is_world_dirty() {
		return (engine.world.isLoaded() && is_dirty);
	}
	
	void set_world_dirty(int mode) {
		is_dirty = mode;
		update_title();
	}
	
	int run_function(int type,string name) {
		if(is_function(type_prefix[type] + name)) return call(type_prefix[type] + name);
		else throw("Unredo::run_function(): %s() is not implemented\n",type_prefix[type] + name);
	}
	
	/*
	 */
	void push_into_stack(State stack[],State state) {
		stack.append(state);
		
		if(stack.size() > EDITOR_UNDO_DEPTH) stack.remove(0);
	}
	
	/*
	 */
	void undo() {
		
		if(creatorGetNode() != NULL) return;
		
		if(undo_states.size() == 0) return;
		
		State state = undo_states[undo_states.size() - 1];
		if(state.skip_dirty == false && state.is_clear == false) {
			int is_clear = (state == clear_state);
			set_world_dirty(!is_clear);
		}
		
		forloop(int i = 0; state.num) {
			if(run_function(state.type,"undo")) return;
		}
		
		State redo_state = state.undo();
		delete state;
		if(redo_state != NULL) push_into_stack(redo_states,redo_state);
		
		undo_states.remove(undo_states.size() - 1);
	}
	
	void redo() {
		
		if(creatorGetNode() != NULL) return;
		
		if(redo_states.size() == 0) return;
		
		State state = redo_states[redo_states.size() - 1];
		
		if(state.skip_dirty == false && state.is_clear == false) {
			int is_clear = false;
			if(redo_states.size() > 1) is_clear = (redo_states[redo_states.size() - 2] == clear_state);
			set_world_dirty(!is_clear);
		}
		
		forloop(int i = 0; state.num) {
			if(run_function(state.type,"redo")) return;
		}
		
		State undo_state = state.undo();
		delete state;
		if(undo_state != NULL) push_into_stack(undo_states,undo_state);
		
		redo_states.remove(redo_states.size() - 1);
		if(redo_states.size() == 0 && clear_state == NULL) set_world_dirty(false);
	}
	
	/*
	 */
	void add_state(State state) {
		if(state.type == STATE_NODE) state.skip_dirty = skip_next_state;
		
		int is_dirty = is_world_dirty();
		
		if(state.is_clear == false) {
			
			if((clear_state == NULL && skip_next_state == false) || (clear_state != NULL && skip_next_state == false && is_dirty == false)) {
				clear_state = state;
				set_world_dirty(true);
			}
			
			set_world_dirty(skip_next_state == false || is_dirty);
		}
		
		push_into_stack(undo_states,state);
		redo_states.delete();
		skip_next_state = false;
	}
	
	void reset_state() {
		clear_state = NULL;
		set_world_dirty(false);
		skip_next_state = false;
		if(redo_states.size() > 0) {
			clear_state = redo_states[redo_states.size() - 1];
		}
	}
	
	void remove_states_to_barier(int type,int undo = 0) {
		for(int i = undo_states.size() - 1; i >= 0; i--) {
			State state = undo_states[i];
			undo_states.remove(i);
			
			if(state.type == type) {
				delete state;
				break;
			}
			if(undo == 1) state.undo();
			delete state;
		}
		
		redo_states.delete();
	}
	
	void skip_state() {
		skip_next_state = true;
	}
	
	/**************************************************************************\
	*
	* Unredo
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
	}
	
	/*
	 */
	void shutdown() {
		clear_state = NULL;
		skip_next_state = false;
		undo_states.delete();
		redo_states.delete();
	}
	
	/*
	 */
	void update() {
		
	}
}

/*
 */
void unredoInit() {
	Unredo::init();
}

void unredoShutdown() {
	Unredo::shutdown();
}

void unredoUpdate(int need_reload) {
	Unredo::update();
}

/*
 */
void unredoUndo() {
	Unredo::undo();
}

void unredoRedo() {
	Unredo::redo();
}

void unredoUndoStackSize() {
	return Unredo::undo_states.size();
}

void unredoRedoStackSize() {
	return Unredo::redo_states.size();
}

/*
 */

void unredoAddState(Unredo::State state) {
	Unredo::add_state(state);
}

void unredoAddNodeState(int num = 1) {
	Unredo::add_state(new Unredo::State(Unredo::STATE_NODE,num));
}

void unredoAddMaterialState(int num = 1) {
	Unredo::add_state(new Unredo::State(Unredo::STATE_MATERIAL,num));
}

void unredoAddPropertyState(int num = 1) {
	Unredo::add_state(new Unredo::State(Unredo::STATE_PROPERTY,num));
}

void unredoAddReferenceState(int num = 1) {
	Unredo::add_state(new Unredo::State(Unredo::STATE_REFERENCE,num));
}

void unredoAddBrushEditState(int num = 1) {
	Unredo::add_state(new Unredo::State(Unredo::STATE_BRUSH_EDIT,num));
}

void unredoAddBrushState(int num = 1) {
	Unredo::add_state(new Unredo::State(Unredo::STATE_BRUSH,num));
}

void unredoAddRenderState(int num = 1) {
	Unredo::add_state(new Unredo::State(Unredo::STATE_RENDER,num));
}

void unredoAddSoundState(int num = 1) {
	Unredo::add_state(new Unredo::State(Unredo::STATE_SOUND,num));
}

void unredoAddPhysicsState(int num  = 1) {
	Unredo::add_state(new Unredo::State(Unredo::STATE_PHYSICS,num));
}

void unredoRemoveReferenceStates() {
	Unredo::remove_states_to_barier(Unredo::STATE_REFERENCE);
}

void unredoRemoveBrushEditStates() {
	Unredo::remove_states_to_barier(Unredo::STATE_BRUSH_EDIT,1);
}

/*
 */
void unredoResetState() {
	Unredo::reset_state();
}

void unredoSkipState() {
	Unredo::skip_state();
}

int unredoIsWorldDirty() {
	return Unredo::is_world_dirty();
}

#define DECLARE_SLIDER_CALLBACKS \
	int skip_state = false; \
	int second_change = 0; \
	void slider_clicked() { \
		skip_state = false; \
		second_change = 0;\
	} \
	void slider_changed() { \
		if(second_change) skip_state = true; \
		second_change = 1; \
	} \

#define DECLARE_SPINBOX_CALLBACKS \
	int sb_skip_state = false; \
	int sb_second_change = 0; \
	void spinbox_clicked() { \
		sb_skip_state = false; \
		sb_second_change = 0;\
	} \
	void spinbox_changed() { \
		if(sb_second_change) sb_skip_state = true; \
		sb_second_change = 1; \
	} \
