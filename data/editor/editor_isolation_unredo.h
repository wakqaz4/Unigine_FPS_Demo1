/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_materials_unredo.h
 * Desc:    Unigine editor
 * Version: 1.04
 * Author:  Maxim Belobryukhov <maxi@unigine.com>
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
class IsolationState_ : Unredo::State {
	
	int state_type;		// state type
	
	Blob blob;			// blob
	
	IsolationNode isolations_restore[];
	RootNode root_nodes_restore[];
	
	/*
	 */
	IsolationState_(int t) {
		super.__State__(Unredo::STATE_ISOLATION,1);
		
		blob = new Blob(0);
		
		state_type = t;
		isolations_restore.copy(isolations);
		root_nodes_restore.copy(root_nodes);
	}
	
	/*
	 */
	~IsolationState_() {
		delete blob;
	}
	
	/*
	 */
	void undo_new() {
		isolations.copy(isolations_restore);
		root_nodes.copy(root_nodes_restore);
		
		if(isolationIsEnabled()) Isolation::disable();
	}
	
	/*
	 */
	void undo_remove() {
		if(isolationIsEnabled() == 0) Isolation::enable();
		
		isolations.copy(isolations_restore);
		root_nodes.copy(root_nodes_restore);
		
	}
	
	/*
	 */
	IsolationState_ undo() {
		IsolationState_ result = NULL;
		if(state_type == Unredo::STATE_REMOVE) {
			undo_remove();
			result = create_state(Unredo::STATE_NEW);
		}
		else if(state_type == Unredo::STATE_NEW) {
			result = create_state(Unredo::STATE_REMOVE);
			undo_new();
		}
		
		return result;
	}
};

IsolationState_ create_state(int type) {
	return new IsolationState_(type);
}

void save_state(IsolationState_ s) {
	unredoAddState(s);
}

int undo() {
	
}

int redo() {
	
}
