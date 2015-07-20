/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_nodes_unredo.h
 * Desc:    Unigine editor
 * Version: 1.05
 * Author:  Valia Vaneeva <fattie@unigine.com>
 *          Alexander Zapryagaev <frustum@unigine.com>
 *          Pavel Kurinnoy <figonet@unigine.com>
 *          Vladimir Borovskikh <futurist@unigine.com>
 *          Alexey Zagniy <yingaz@unigine.com>
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

#define STATE_EPSILON	1e-5f

class JointBody {
	int num;
	Object object;
	Joint joint;
	JointBody(Joint j,Body o,int n) {
		num = n;
		object = o;
		joint = j;
	}
};

/*
 */
namespace Object {
	namespace Joint {
		int get_joint_num();
		void set_focus(Widget f,int i);
	}
}

/*
 */
class NodesState_ : Unredo::State {
	
	int state_type;						// state type
	Widget focus;						// focused widget
	
	Node nodes[0];						// editor nodes
	Node clones[0];						// clones
	int enabled[0];						// original 'enabled' states
	Node old_parents[0];				// old parents
	Node new_parents[0];				// new parents
	int positions[0];					// old positions in the list of nodes
	string direction;					// swap directions
	Node selection[0];					// old selection
	Node last_selected;					// last selected node
	int surfaces[0];					// selected surfaces
	int child_numbers[0];				// child numbers
	Joint joint_clones[0];				// joint clones
	Body bodies[0];						// joint bodies
	int joint_num;						// last selected joint
	
	Body body = NULL;
	Node body_node = NULL; // node assigned to body that changed
	int body_node_enabled;
	
	Unigine::Vector ext_joints[];		// external joints in all hierarchies
	
	Blob node_blobs[];					// special blobs for save/restore
	
	NodesState_() {
		super.__State__(Unredo::STATE_NODE);
		state_type = Unredo::STATE_NONE;
		focus = gui.getFocus();
	}
	
	~NodesState_() {
		foreach(Node n; clones) {
			if(n != NULL) node_delete(node_append(n));
		}
		
		foreach(Joint j; joint_clones) {
			if(j != NULL) delete j;
		}
		
		nodes.clear();
		clones.clear();
		joint_clones.clear();
		old_parents.clear();
		new_parents.clear();
		selection.clear();
		bodies.clear();
		ext_joints.clear();
		last_selected = NULL;
		body = NULL;
		surfaces.clear();
		
		node_blobs.delete();
	}
	
	Node create_clone(Node node) {
		if(node.getType() == NODE_OBJECT_BILLBOARDS) {
			Blob blob = new Blob();
			node.saveState(blob);
			blob.seekSet(0);
			node_blobs.append(node,blob);
		}
		return node_clone(node);
	}
	
	void init_select(Node nodes_[],Node last_selected_,int surfaces_[]) {
		state_type = STATE_SELECT;
		is_clear = true;
		
		nodes.copy(nodes_);
		last_selected = last_selected_;
		
		surfaces.copy(surfaces_);
		clones.resize(nodes.size());
	}
	
	void undo_select(Node selection_[],Node &last_selected_,int surfaces_[]) {
		selection_.copy(nodes);
		last_selected_ = last_selected;
		surfaces_.copy(surfaces);
	}
	
	void init_swap(Node nodes_[],Node last_selected_,string direction_) {
		state_type = STATE_SWAP;
		
		nodes.append(last_selected_);
		direction = direction_;
		
		selection.copy(nodes_);
		last_selected = last_selected_;
		
		if(last_selected.isObject()) {
			Object::get_current_surfaces(surfaces);
		}
		
		clones.resize(nodes.size());
	}
	
	void undo_swap(Node selection_[],Node &last_selected_,int surfaces_[]) {
		Node n = get_swap_candidate(direction,nodes[0]);
		
		if(n != NULL) {
			engine.editor.swapNodes(nodes[0],n);
			if(nodes[0].getParent() != NULL) swap_childs(nodes[0],n);
		}
		
		update_nodes();
		select_node(nodes[0],0);
		engine.editor.needReload();
		texturesUpdate(1);
		
		selection_.copy(selection);
		last_selected_ = last_selected;
		
		surfaces_.copy(surfaces);
	}
	
	void init_modify(Node nodes_[],Node last_selected_) {
		state_type = STATE_MODIFY;
		
		// copy selection and nodes first
		nodes.copy(nodes_);
		
		selection.copy(nodes_);
		last_selected = last_selected_;
		
		if(last_selected != NULL && last_selected.isObject()) {
			Object::get_current_surfaces(surfaces);
		}
		
		// leave children out
		remove_childs(nodes);
		
		foreach(Node n; nodes) {
			clones.append(create_clone(n));
			enabled.append(n.isEnabled());
			
			Node p = n.getParent();
			old_parents.append(p);
			if(p != NULL) {
				forloop(int i = 0; p.getNumChilds()) {
					if(p.getChild(i) != n) continue;
					child_numbers.append(i);
					break;
				}
			} else {
				child_numbers.append(-1);
			}
		}
		foreach(Node n; clones) {
			n.setEnabled(0);
			n.setWorldParent(NULL);
		}
	}
	
	void undo_modify(Node selection_[],Node &last_selected_,int surfaces_[]) {
		Node node_map[] = (:);
		forloop(int i = 0; current_nodes.size()) {
			node_map.append(current_nodes[i],current_nodes[i]);
		}
		Node decals_to_update[0] = ();
		forloop(int i = 0; nodes.size()) {
			Node clone = NULL;
			
			Node n = node_map.check(nodes[i],NULL);
			if(n != NULL) {
				clone = node_clone(clones[i]);
				engine.editor.addNode(clone);
				
				clone.setEnabled(enabled[i]);
				add_child(old_parents[i],clone,child_numbers[i]);
				
				// swap nodes
				swap_node_childs(n,clone);
				swap_node(n,clone);
				
				Blob blob = node_blobs.check(n,NULL);
				if(blob != NULL) n.restoreState(blob);
				
				void remove_node(Node node) {
					if(engine.editor.isNode(node)) {
						for(int i = node.getNumChilds() - 1; i >= 0; i--) {
							remove_node(node.getChild(i));
						}
						int ret = 0;
						ret = engine.editor.removeNode(node);
						assert(ret == 1);
					}
				}
				
				if(n.isDecal()) decals_to_update.append(node_cast(n));
				
				remove_node(clone);
			}
		}
		
		update_nodes();
		engine.editor.needReload();
		texturesUpdate(1);
		
		forloop(int i = 0; decals_to_update.size()) {
			update_decals(decals_to_update[i]);
		}
		
		selection_.copy(selection);
		last_selected_ = last_selected;
		
		surfaces_.copy(surfaces);
	}
	
	void init_new(Node nodes_[],Node selection_[],Node last_selected_) {
		state_type = STATE_NEW;
		
		nodes.copy(nodes_);
		remove_childs(nodes);
		
		clones.resize(nodes.size());
		selection.copy(selection_);
		last_selected = last_selected_;
		
		if(last_selected != NULL && last_selected.isObject()) {
			Object::get_current_surfaces(surfaces);
		}
	}
	
	void undo_new(Node selection_[],Node &last_selected_,int surfaces_[]) {
		forloop(int i = 0; nodes.size()) {
			remove_node(nodes[i]);
		}
		
		update_nodes();
		select_node(NULL,0);
		engine.editor.needReload();
		texturesUpdate(1);
		
		selection_.copy(selection);
		last_selected_ = last_selected;
		
		surfaces_.copy(surfaces);
	}
	
	void init_remove(Node nodes_[],Node selection_[],Node last_selected_) {
		state_type = STATE_REMOVE;
		
		// copy selection and nodes first
		nodes.copy(nodes_);
		
		selection.copy(selection_);
		last_selected = last_selected_;
		
		if(last_selected != NULL) {
			if(last_selected.isObject()) {
				Object::get_current_surfaces(surfaces);
			}
			
			selection.append(last_selected);
		}
		
		// leave children out
		remove_childs(nodes);
		
		foreach(Node n; nodes) {
			save_joints(n,ext_joints);
			
			clones.append(n);
			enabled.append(n.isEnabled());
			
			Node p = n.getParent();
			old_parents.append(p);
			if(p != NULL) {
				forloop(int i = 0; p.getNumChilds()) {
					if(p.getChild(i) != n) continue;
					child_numbers.append(i);
					break;
				}
			} else {
				child_numbers.append(-1);
			}
			
			positions.append(get_node_position(n));
		}
		
		if(selection.size() != 0) {
			last_selected = selection[selection.size() - 1];
			selection.remove(selection.size() - 1);
		}
		
		foreach(Node n; clones) {
			n.setEnabled(0);
			n.setWorldParent(NULL);
		}
	}
	
	void undo_remove(Node selection_[],Node &last_selected_,int surfaces_[]) {
		forloop(int i = 0; clones.size()) {
			Node clone = NULL;
			
			clone = clones[i];
			clones[i] = node_clone(clone);
			restore_joints(clone,ext_joints);
			
			clone.setEnabled(enabled[i]);
			engine.editor.addNode(clone);
			
			add_child(old_parents[i],clone,child_numbers[i]);
			
			// move to old position
			move_to_position(clone,positions[i]);
		}
		
		update_nodes();
		engine.editor.needReload();
		texturesUpdate(1);
		
		selection_.copy(selection);
		last_selected_ = last_selected;
		
		surfaces_.copy(surfaces);
	}
	
	void init_reparent(Node nodes_[],int child_numbers_[],Node old_parents_[],Node new_parents_[],Node selection_[],Node last_selected_) {
		state_type = STATE_REPARENT;
		
		assert(nodes_.size() == child_numbers_.size() && nodes_.size() == old_parents_.size() && nodes_.size() == new_parents_.size());
		
		nodes.copy(nodes_);
		old_parents.copy(old_parents_);
		new_parents.copy(new_parents_);
		child_numbers.copy(child_numbers_);
		
		clones.resize(nodes.size());
		selection.copy(selection_);
		last_selected = last_selected_;
		
		if(last_selected != NULL && last_selected.isObject()) {
			Object::get_current_surfaces(surfaces);
		}
	}
	
	void undo_reparent(Node selection_[],Node &last_selected_,int surfaces_[]) {
		forloop(int i = 0; nodes.size()) {
			if(old_parents[i] == NULL) {
				nodes[i].setWorldParent(NULL);
				move_to_position(nodes[i],child_numbers[i]);
			} else {
				add_child(old_parents[i],nodes[i],child_numbers[i]);
			}
		}
		
		update_nodes();
		engine.editor.needReload();
		
		selection_.copy(selection);
		last_selected_ = last_selected;
		
		surfaces_.copy(surfaces);
	}
	
	void init_body(Node nodes_[],Node last_selected_) {
		state_type = STATE_BODY;
		
		Body b = Nodes::Object::Body::get_body();
		if(b != NULL) {
			collect_joints(b.getObject(),ext_joints);
			body = class_append(body_clone(b));
		} else {
			body = NULL;
		}
		
		// selection
		selection.copy(nodes_);
		last_selected = last_selected_;
		
		if(body_node != NULL) {
			delete body_node;
			body_node = NULL;
		}
		
		if(last_selected.isObject()) {
			Object::get_current_surfaces(surfaces);
			
			body_node = class_append(node_clone(last_selected));
			body_node_enabled = last_selected.isEnabled();
			body_node.setEnabled(0);
		}
	}
	
	void undo_body(Node selection_[],Node &last_selected_,int surfaces_[]) {
		
		// revert node
		if(body_node != NULL) {
			last_selected.swap(body_node);
			last_selected.setEnabled(body_node_enabled);
			
			delete body_node;
			body_node = NULL;
		}
		
		Object o = node_cast(last_selected);
		// remove old body
		if(o.getBody() != NULL) class_append(o.getBody());
		o.setBody(NULL);
		
		// assign new body
		if(body != NULL) {
			o.setBody(body_clone(body));
			restore_joints(o,ext_joints);
		}
		
		selection_.copy(selection);
		last_selected_ = last_selected;
		
		surfaces_.copy(surfaces);
	}
	
	void init_joint(Node nodes_[],Node last_selected_) {
		state_type = STATE_JOINT;
		
		joint_num = Object::Joint::get_joint_num();
		clone_joints(last_selected_,joint_clones,bodies,enabled);
		
		selection.copy(nodes_);
		last_selected = last_selected_;
		
		if(last_selected.isObject()) {
			Object::get_current_surfaces(surfaces);
		}
	}
	
	void undo_joint(Node selection_[],Node &last_selected_,int surfaces_[],int &joint_num_) {
		joint_num_ = joint_num;
		replace_joints(last_selected,joint_clones,bodies,enabled);
		
		selection_.copy(selection);
		last_selected_ = last_selected;
		
		surfaces_.copy(surfaces);
	}
	
	void select_nodes(Node selection[],Node last_selected) {
		Nodes::select_node(NULL,0);
		Nodes::select_nodes(selection,1);
		
		Nodes::select_node(last_selected,1);
		if(last_selected != NULL && last_selected.isObject()) {
			Object::surface_select(-1,0);
			if(surfaces.size()) {
				int surface = surfaces[surfaces.size() - 1];
				surfaces.remove(surfaces.size() - 1);
				foreach(int i; surfaces) {
					if(i == surface) continue;
					Object::surface_select(i,1);
				}
				Object::surface_select(surface,1);
			}
		}
	}
	
	void select_nodes() {
		this.select_nodes(selection,last_selected);
	}
	
	NodesState_ undo() {
		NodesState_ result = NULL;
		
		// currently selected nodes
		Node current[0];
		current.copy(nodes);
		Node current_last = node;
		
		// new selection_local
		Node selection_local[0] = ();
		Node last_selected_local = NULL;
		int surfaces_[0] = ();
		int joint = -1;
		
		Widget callbacks_widgets[0];
		disableCallbacks(window,callbacks_widgets);
		Nodes::set_nodes_vbox_callbacks_enabled(0);
		
		if(state_type == Unredo::STATE_SELECT) {
			// save current
			int current_surfaces[0] = ();
			if(current_last != NULL && current_last.isObject()) {
				Object::get_current_surfaces(current_surfaces);
			}
			
			result = create_state_select(current,current_last,current_surfaces);
			
			undo_select(selection_local,last_selected_local,surfaces_);
			
			// tweak focus
			forloop(int i = 0; 3) {
				if(position_el[i].isFocused() || rotation_el[i].isFocused() || scale_el[i].isFocused()) {
					nodes_tb.setFocus();
					break;
				}
			}
		}
		else if(state_type == Unredo::STATE_SWAP) {
			// save current
			string rdir = (direction == "up") ? "down" : "up";
			result = create_state_swap(current,current_last,rdir);
			
			undo_swap(selection_local,last_selected_local,surfaces_);
		}
		else if(state_type == Unredo::STATE_MODIFY) {
			// save current
			result = create_state_modify(current,current_last);
			undo_modify(selection_local,last_selected_local,surfaces_);
		}
		else if(state_type == Unredo::STATE_NEW) {
			// save current
			result = create_state_remove(nodes,current,current_last);
			undo_new(selection_local,last_selected_local,surfaces_);
		}
		else if(state_type == Unredo::STATE_REMOVE) {
			// save current
			undo_remove(selection_local,last_selected_local,surfaces_);
			result = create_state_new(nodes,current,current_last);
		}
		else if(state_type == Unredo::STATE_REPARENT) {
			// save current
			int child_numbers[0] = ();
			forloop(int i = 0; nodes.size()) {
				Node p = new_parents[i];
				if(p != NULL) {
					assert(engine.world.isNode(p));
					int find = 0;
					forloop(int j = 0; p.getNumChilds()) {
						if(p.getChild(j) != nodes[i]) continue;
						child_numbers.append(j);
						find = 1;
						break;
					}
					assert(find == 1);
				} else {
					child_numbers.append(-1);
				}
			}
			
			result = create_state_reparent(nodes,child_numbers,new_parents,old_parents,current,current_last);
			undo_reparent(selection_local,last_selected_local,surfaces_);
		}
		else if(state_type == Unredo::STATE_BODY) {
			// save current
			result = create_state_body(current,current_last);
			undo_body(selection_local,last_selected_local,surfaces_);
		}
		else if(state_type == Unredo::STATE_JOINT) {
			// save current
			result = create_state_joint(current,current_last);
			undo_joint(selection_local,last_selected_local,surfaces_,joint);
		}
		else {
			throw("Nodes::process_states(): unknown state\n");
		}
		
		// enable callbacks
		foreach(Widget w; callbacks_widgets) {
			if(w == NULL) continue;
			for(int i = GUI_SHOW; i <= GUI_DRAG_DROP; i++) {
				w.setCallbackEnabled(i,1);
			}
		}
		
		// set this flag before selection_local and, hence, the fist view update
		auto_focus = 1;
		
		select_nodes(selection_local,last_selected_local);
		
		// select correct joint
		if(state_type == STATE_JOINT) Object::Joint::set_focus(focus,joint);
		
		// set focus
		if(focus != NULL && editorIsChild(focus)) {
			focus.setFocus();
		}
		
		
		Nodes::set_nodes_vbox_callbacks_enabled(1);
		
		return result;
	}
};

class NodesStateMany_ : Unredo::State {
	Unredo::State states[0];
	
	NodesStateMany_() {
		super.__State__(Unredo::STATE_NODE);
	}
	
	~NodesStateMany_() {
		states.delete();
	}
	
	void append(State state) {
		states.append(state);
	}
	
	NodesStateMany_ undo() {
		NodesStateMany_ result = new NodesStateMany_();
		
		for(int i = states.size() - 1; i >= 0; i--) {
			result.append(states[i].undo());
		}
		
		return result;
	}
};

/*
 */
NodesState_ new_state;

void save_new_state() {
	save_state(new_state);
}

void delete_new_state() {
	delete new_state;
}

/*
 */
void remove_childs(Node nodes[]) {
	
	Node nodes_map[] = (:);
	forloop(int i = 0; nodes.size()) {
		nodes_map.append(nodes[i]);
	}
	
	Node new_nodes[] = (:);
	forloop(int i = 0; nodes.size()) {
		if(nodes[i].getParent() == NULL || !nodes_map.check(nodes[i].getParent())) {
			if(new_nodes.check(nodes[i])) continue;
			new_nodes.append(nodes[i]);
		}
	}
	
	nodes.clear();
	forloop(int i = 0; new_nodes.size()) {
		nodes.append(new_nodes.key(i));
	}
}

/*
 */
NodesState_ create_state_select(Node nodes[],Node last_selected,int surfaces[]) {
	NodesState_ s = new NodesState_();
	if(Nodes::last_selected_node != NULL) {
		s.init_select(last_selected_nodes,last_selected_node,last_selected_surfaces);
		last_selected_node = NULL;
	} else s.init_select(nodes,last_selected,surfaces);
	return s;
}

NodesState_ create_state_swap(Node nodes[],Node last_selected,string direction) {
	NodesState_ s = new NodesState_();
	s.init_swap(nodes,last_selected,direction);
	return s;
}

NodesState_ create_state_modify(Node nodes[],Node last_selected) {
	NodesState_ s = new NodesState_();
	s.init_modify(nodes,last_selected);
	return s;
}

NodesState_ create_state_new(Node nodes[],Node selection[],Node last_selected) {
	NodesState_ s = new NodesState_();
	s.init_new(nodes,selection,last_selected);
	return s;
}

NodesState_ create_state_remove(Node nodes[],Node selection[],Node last_selected) {
	NodesState_ s = new NodesState_();
	s.init_remove(nodes,selection,last_selected);
	return s;
}

NodesState_ create_state_reparent(Node nodes[],int child_numbers[],Node old_parents[],Node new_parents[],Node selection[],Node last_selected) {
	NodesState_ s = new NodesState_();
	s.init_reparent(nodes,child_numbers,old_parents,new_parents,selection,last_selected);
	return s;
}

NodesState_ create_state_body(Node nodes[],Node last_selected) {
	NodesState_ s = new NodesState_();
	s.init_body(nodes,last_selected);
	return s;
}

NodesState_ create_state_joint(Node nodes[],Node last_selected) {
	NodesState_ s = new NodesState_();
	s.init_joint(nodes,last_selected);
	return s;
}

NodesStateMany_ create_state_many(NodesState_ states[]) {
	NodesStateMany_ s = new NodesStateMany_();
	foreach(NodesState_ state; states) {
		s.append(state);
	}
	return s;
}

/*
 */
void save_state(NodesState_ state,Widget widget = NULL) {
	
	Widget old_widget;
	
	if(state == NULL) {
		old_widget = NULL;
		return;
	}
	
	if(state.state_type == Unredo::STATE_NONE) {
		delete state;
		return;
	}
	
	if(widget == NULL || old_widget != widget) {
		unredoAddState(state);
		old_widget = widget;
	} else {
		delete state;
	}
}

void save_states(NodesState_ states[]) {
	if(states.size() != 0) {
		NodesStateMany_ state_many = create_state_many(states);
		unredoAddState(state_many);
	}
}

/*
 */
void swap_node(Node n,Node clone) {
	Body clone_body = NULL;
	
	if(clone.isObject()) {
		Object clone_object = node_cast(clone);
		
		if(clone_object.getBody() != NULL) {
			clone_body = body_clone(clone_object.getBody());
		}
		else if(clone.getType() == NODE_OBJECT_TERRAIN && n.getType() == NODE_OBJECT_TERRAIN) {
			ObjectTerrain terrain_clone = node_cast(clone);
			ObjectTerrain terrain = node_cast(n);
			
			if(terrain_clone.getNumMaterials() != terrain.getNumMaterials()) {
				texturesUpdateTerrainDiffuseArray(terrain_clone);
				texturesUpdateTerrainNormalArray(terrain_clone);
				texturesUpdateTerrainSpecularArray(terrain_clone);
			}
			else {
				forloop(int i = 0; terrain.getNumMaterials()) {
					if(terrain.getMaterialDiffuseTextureName(i) == terrain_clone.getMaterialDiffuseTextureName(i)) continue;
					texturesUpdateTerrainDiffuseArray(terrain_clone);
					break;
				}
				forloop(int i = 0; terrain.getNumMaterials()) {
					if(terrain.getMaterialNormalTextureName(i) == terrain_clone.getMaterialNormalTextureName(i)) continue;
					texturesUpdateTerrainNormalArray(terrain_clone);
					break;
				}
				
				forloop(int i = 0; terrain.getNumMaterials()) {
					if(terrain.getMaterialSpecularTextureName(i) == terrain_clone.getMaterialSpecularTextureName(i)) continue;
					texturesUpdateTerrainSpecularArray(terrain_clone);
					break;
				}
			}
		}
	}
	
	n.swap(clone);
	
	if(n.isObject() && clone_body != NULL) {
		Object node_object = node_cast(n);
		
		if(node_object.getBody() != NULL) {
			Body node_body = node_object.getBody();
			
			// save current joints
			Joint joints[0] = ();
			forloop(int i = 0; node_body.getNumJoints()) {
				joints.append(node_body.getJoint(i));
			}
			
			// remove current joints
			while(node_body.getNumJoints() != 0) node_body.removeJoint(node_body.getJoint(0));
			
			// swap bodies
			node_body.setObject(NULL);
			node_body.swap(clone_body);
			node_body.setObject(node_object);
			class_append(clone_body);
			
			// restore joints
			forloop(int i = 0; joints.size()) {
				node_body.addJoint(joints[i]);
			}
		}
	}
}

void swap_node_childs(Node root1,Node root2) {
	assert(root1.getNumChilds() == root2.getNumChilds());
	
	forloop(int i = 0; root1.getNumChilds()) {
		swap_node(root1.getChild(i),root2.getChild(i));
		swap_node_childs(root1.getChild(i),root2.getChild(i));
	}
}

/*
 */
void slider_clicked() {
	save_state(NULL,NULL);
}

/*
 */
void swap_childs(Node n0,Node n1) {
	
	if(n0.getParent() == NULL && n1.getParent() == NULL) return;
	
	assert(n0.getParent() != NULL && n1.getParent() != NULL);
	assert(engine.world.isNode(n0) && engine.world.isNode(n1));
	
	Node childs[] = ();
	Node parent = n0.getParent();
	for(int i = parent.getNumChilds() - 1; i >= 0; i--) {
		Node child = parent.getChild(i);
		parent.removeWorldChild(child);
		childs.append(child);
	}
	
	int i = childs.find(n0);
	int j = childs.find(n1);
	Node node = childs[i];
	childs[i] = childs[j];
	childs[j] = node;
	
	for(int i = childs.size() - 1; i >= 0; i--) {
		parent.addWorldChild(childs[i]);
	}
}

Node get_swap_candidate(string dir,Node n) {
	
	assert(n != NULL && (dir == "up" || dir == "down"));
	
	Node candidate = NULL;
	int item = current_items[n];
	Node parent = n.getParent();
	
	if(dir == "up") {
		while(--item >= 0) {
			Node other_parent = current_nodes[item].getParent();
			if(other_parent == parent) {
				candidate = current_nodes[item];
				break;
			}
		}
	}
	else {
		while(++item < current_nodes.size()) {
			Node other_parent = current_nodes[item].getParent();
			if(other_parent == parent) {
				candidate = current_nodes[item];
				break;
			}
		}
	}
	
	return candidate;
}

/*
 */
void add_child(Node parent,Node node,int pos) {
	
	if(parent == NULL || pos < 0) return;
	
	assert(engine.world.isNode(parent));
	assert(node != NULL || engine.world.isNode(node));
	
	Node childs[] = ();
	for(int i = parent.getNumChilds() - 1; i >= pos; i--) {
		Node child = parent.getChild(i);
		childs.append(child);
		parent.removeWorldChild(child);
	}
	
	parent.addWorldChild(node);
	
	for(int i = childs.size() - 1; i >= 0; i--) {
		parent.addWorldChild(childs[i]);
	}
}

/*
 */
void clone_joints(Node node,Joint clones[],Body bodies[],int enabled[]) {
	
	assert(engine.world.isNode(node));
	
	if(node.isObject() == 0) return;
	
	Object object = node_cast(node);
	Body body = object.getBody();
	if(body == NULL) return;
	
	forloop(int i = 0; body.getNumJoints()) {
		Joint original = body.getJoint(i);
		Joint clone = joint_clone(original);
		clone.setEnabled(0);
		enabled.append(original.isEnabled());
		clones.append(class_append(clone));
		Body b = (body == original.getBody0()) ? original.getBody1() : original.getBody0();
		bodies.append(b);
	}
}

void replace_joints(Node node,Joint clones[],Body bodies[],int enabled[]) {
	
	assert(engine.world.isNode(node));
	
	if(node.isObject() == 0) return;
	
	Object object = node_cast(node);
	Body body0 = object.getBody();
	if(body0 == NULL) return;
	
	Body body1 = NULL;
	
	int reverse[0] = ();
	reverse.resize(clones.size());
	
	for(int i = body0.getNumJoints() - 1; i >= 0; i--) {
		Joint joint = body0.getJoint(i);
		
		if(body0 == joint.getBody1()) {
			reverse[i] = 1;
			body1 = joint.getBody0();
		} else {
			body1 = joint.getBody1();
		}
		body0.removeJoint(joint);
		body1.removeJoint(joint);
		delete class_append(joint);
	}
	
	body1 = NULL;
	
	forloop(int i = 0; clones.size()) {
		Joint clone = joint_clone(clones[i]);
		
		clone.setEnabled(enabled[i]);
		
		body1 = bodies[i];
		
		assert(engine.physics.isBody(body1));
		
		if(reverse[i]) {
			body1.addJoint(clone);
			body0.addJoint(clone);
		} else {
			body0.addJoint(clone);
			body1.addJoint(clone);
		}
	}
}

/*
 */
void save_joints(Node root,Unigine::Vector ext_joints[]) {
	assert(engine.world.isNode(root));
	
	void save_joint(Node node) {
		if(node.isObject()) {
			Object node_object = node_cast(node);
			Body node_body = node_object.getBody();
			if(node_body != NULL) {
				forloop(int i = 0; node_body.getNumJoints()) {
					Joint joint = node_body.getJoint(i);
					
					JointBody bj = NULL;
					
					if(joint.getBody0() == node_body) {
						bj = new JointBody(joint,joint.getBody1().getObject(),1);
						joint.setBody1(NULL);
					}
					if(joint.getBody1() == node_body) {
						bj = new JointBody(joint,joint.getBody0().getObject(),0);
						joint.setBody0(NULL);
					}
					if(bj == NULL) continue;
					
					if(ext_joints.check(node)) {
						ext_joints[node].append(bj);
					} else {
						Unigine::Vector vector = new Unigine::Vector();
						vector.append(bj);
						ext_joints.append(node,vector);
					}
				}
			}
		}
	}
	
	void recur(Node root) {
		forloop(int i = 0; root.getNumChilds()) {
			Node child = root.getChild(i);
			save_joint(child);
			recur(child);
		}
	}
	
	// save own external joints
	save_joint(root);
	
	// save children's external joints
	recur(root);
}

/*
 */
void collect_joints(Node root,Unigine::Vector ext_joints[]) {
	assert(engine.world.isNode(root));
	
	void save_joint(Node node) {
		if(node.isObject()) {
			Object node_object = node_cast(node);
			Body node_body = node_object.getBody();
			if(node_body != NULL) {
				forloop(int i = 0; node_body.getNumJoints()) {
					Joint joint = node_body.getJoint(i);
					
					JointBody bj = NULL;
					
					if(joint.getBody0() == node_body) {
						bj = new JointBody(joint_clone(joint),joint.getBody1().getObject(),1);
						joint.setBody1(NULL);
					}
					if(joint.getBody1() == node_body) {
						bj = new JointBody(joint_clone(joint),joint.getBody0().getObject(),0);
						joint.setBody0(NULL);
					}
					if(bj == NULL) continue;
					
					if(ext_joints.check(node)) {
						ext_joints[node].append(bj);
					} else {
						Unigine::Vector vector = new Unigine::Vector();
						vector.append(bj);
						ext_joints.append(node,vector);
					}
				}
			}
		}
	}
	
	void recur(Node root) {
		forloop(int i = 0; root.getNumChilds()) {
			Node child = root.getChild(i);
			save_joint(child);
			recur(child);
		}
	}
	
	// save own external joints
	save_joint(root);
	
	// save children's external joints
	recur(root);
}

/*
 */
void restore_joints(Node node,Unigine::Vector ext_joints[]) {
	if(node.isObject()) {
		Object node_object = node_cast(node);
		Body node_body = node_object.getBody();
		if(node_body != NULL) {
			
			if(ext_joints.check(node) != 0) {
				
				Unigine::Vector vector = ext_joints[node];
				
				forloop(int i = 0; vector.size()) {
					JointBody bj = vector[i];
					
					Joint joint = bj.joint;
					
					if(bj.num == 0) {
						joint.setBody0(bj.object.getBody());
						if(joint.getBody1() != node_body) joint.setBody1(node_body);
					} else {
						joint.setBody1(bj.object.getBody());
						if(joint.getBody0() != node_body) joint.setBody0(node_body);
					}
				}
			}
		}
	}
	
	forloop(int i = 0; node.getNumChilds()) {
		restore_joints(node.getChild(i),ext_joints);
	}
}

int node_positions_cache_[] = (:);
/*
 */
int get_node_position(Node n) {
	
	int cache_rebuilding_search(Node target_node) {
		
		if(node_positions_cache_.size() > engine.editor.getNumNodes()) node_positions_cache_.clear();
		
		for(int i = engine.editor.getNumNodes() - 1; i >= 0; i--) { // new nodes are usually appended to the end
			Node n = engine.editor.getNode(i);
			node_positions_cache_[n] = i;
			if(n == target_node) return i;
		}
		
		return -1;
	}
	
	if(!engine.editor.isNode(n)) return -1;
	
	if(node_positions_cache_.check(n)) {
		int pos = node_positions_cache_[n];
		if(pos < engine.editor.getNumNodes() && engine.editor.getNode(pos) == n) {
			return pos ;
		}
	}
	
	return cache_rebuilding_search(n);
}

/*
 */
void move_to_position(Node node,int position) {
	
	if(position < 0 || position >= engine.editor.getNumNodes()) return;
	
	int current = get_node_position(node);
	
	while(current != position && current >= 0 && current < engine.editor.getNumNodes()) {
		if(current < position) engine.editor.swapNodes(node,engine.editor.getNode(++current));
		else if(current > position) engine.editor.swapNodes(node,engine.editor.getNode(--current));
	}
}

/******************************************************************************\
*
* States
*
\******************************************************************************/

/*
 */
int undo() {
}

int redo() {
}

int clear() {
	//undo_states.clear();
	//redo_states.clear();
}

/******************************************************************************\
*
* Callbacks
*
\******************************************************************************/

/*
 */
#define PRESSED_CALLBACK(WIDGET,OBJECT,FUNC,NODE,TYPE,ACTION) \
void WIDGET ## _pressed() { \
	if(OBJECT == NULL) return; \
	Node temp = OBJECT; \
	NodesState_ s = new NodesState_(); \
	TYPE value = TYPE (WIDGET ## _el.getText()); \
	if(OBJECT == NODE) { \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			if(OBJECT.get ## FUNC() != value) { \
				if(s.state_type == Unredo::STATE_NONE) s.init_ ## ACTION(nodes,NODE); \
				OBJECT.set ## FUNC(value); \
			} \
		} \
	} else if(NODE.isObject() && Object(NODE).getBody() == OBJECT) { \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.isObject() == 0) continue; \
			Object object = node_cast(n); \
			if(object.getBody() == NULL || Body(object.getBody()).getType() != type) continue; \
			OBJECT = body_cast(object.getBody()); \
			if(OBJECT.get ## FUNC() != value) { \
				if(s.state_type == Unredo::STATE_NONE) s.init_ ## ACTION(nodes,NODE); \
				OBJECT.set ## FUNC(value); \
			} \
		} \
	} else { \
		if(OBJECT.get ## FUNC() != value) { \
			s.init_ ## ACTION(nodes,NODE); \
			OBJECT.set ## FUNC(value);  \
		} \
	} \
	OBJECT = temp; \
	WIDGET ## _el.setText(string(OBJECT.get ## FUNC())); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

/*
 */
#define PRESSED_CALLBACK_FLOAT(WIDGET,OBJECT,FUNC,NODE,ACTION) \
void WIDGET ## _pressed() { \
	if(OBJECT == NULL) return; \
	Node temp = OBJECT; \
	NodesState_ s = new NodesState_(); \
	float value = float(WIDGET ## _el.getText()); \
	if(OBJECT == NODE) { \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			if(OBJECT.get ## FUNC() != value) { \
				if(s.state_type == Unredo::STATE_NONE) s.init_ ## ACTION(nodes,NODE); \
				OBJECT.set ## FUNC(value); \
			} \
		} \
	} else if(NODE.isObject() && Object(NODE).getBody() == OBJECT) { \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.isObject() == 0) continue; \
			Object object = node_cast(n); \
			if(object.getBody() == NULL || Body(object.getBody()).getType() != type) continue; \
			OBJECT = body_cast(object.getBody()); \
			if(OBJECT.get ## FUNC() != value) { \
				if(s.state_type == Unredo::STATE_NONE) s.init_ ## ACTION(nodes,NODE); \
				OBJECT.set ## FUNC(value); \
			} \
		} \
	} else { \
		if(OBJECT.get ## FUNC() != value) { \
			s.init_ ## ACTION(nodes,NODE); \
			OBJECT.set ## FUNC(value);  \
		} \
	} \
	OBJECT = temp; \
	WIDGET ## _el.setText(editorFormat(OBJECT.get ## FUNC())); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

/*
 */
#define PRESSED_CALLBACK_VEC3(WIDGET,OBJECT,FUNC,NODE,ACTION) \
void WIDGET ## _pressed() { \
	if(OBJECT == NULL) return; \
	NodesState_ s = new NodesState_(); \
	if(OBJECT == NODE) { \
		int changed = 0; \
		Node node = OBJECT; \
		int type = OBJECT.getType(); \
		vec3 value = getEditLineVec3(WIDGET ## _el); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			if(OBJECT.get ## FUNC() != value) { \
				changed = 1; \
				break; \
			} \
		} \
		if(changed) { \
			s.init_ ## ACTION(nodes,NODE); \
			foreach(Node n; nodes) { \
				if(n.getType() != type) continue; \
				OBJECT = node_cast(n); \
				OBJECT.set ## FUNC(value); \
			} \
		} \
		OBJECT = node; \
	} else { \
		if(OBJECT.get ## FUNC() != getEditLineVec3(WIDGET ## _el)) { \
			s.init_ ## ACTION(nodes,NODE); \
			OBJECT.set ## FUNC(getEditLineVec3(WIDGET ## _el)); \
		} \
	} \
	setEditLineVec3(WIDGET ## _el,OBJECT.get ## FUNC()); \
	foreach(WidgetEditLine editline; WIDGET ## _el) { \
		if(toolsGetSelection() && editline.isFocused()) { \
			editline.setCursor(0); \
			editline.setSelection(1024); \
		} \
	} \
	save_state(s); \
}

#define PRESSED_CALLBACK_VEC3_SAME(WIDGET,OBJECT,FUNC,NODE,ACTION) \
void WIDGET ## _pressed() { \
	if(OBJECT == NULL) return; \
	NodesState_ s = new NodesState_(); \
	if(OBJECT == NODE) { \
		int changed = 0; \
		Node node = OBJECT; \
		int type = OBJECT.getType(); \
		vec3 value = getEditLineSameVec3(WIDGET ## _el); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			if(OBJECT.get ## FUNC() != value) { \
				changed = 1; \
				break; \
			} \
		} \
		if(changed) { \
			s.init_ ## ACTION(nodes,NODE); \
			foreach(Node n; nodes) { \
				if(n.getType() != type) continue; \
				OBJECT = node_cast(n); \
				OBJECT.set ## FUNC(getEditLineSameVec3(WIDGET ## _el)); \
			} \
		} \
		OBJECT = node; \
	} else { \
		if(OBJECT.get ## FUNC() != getEditLineSameVec3(WIDGET ## _el)) { \
			s.init_ ## ACTION(nodes,NODE); \
			OBJECT.set ## FUNC(getEditLineSameVec3(WIDGET ## _el)); \
		} \
	} \
	setEditLineSameVec3(WIDGET ## _el,OBJECT.get ## FUNC()); \
	foreach(WidgetEditLine editline; WIDGET ## _el) { \
		if(toolsGetSelection() && editline.isFocused()) { \
			editline.setCursor(0); \
			editline.setSelection(1024); \
		} \
	} \
	save_state(s); \
}

/*
 */
#define PRESSED_CALLBACK_VEC4(WIDGET,OBJECT,FUNC,NODE,ACTION) \
void WIDGET ## _pressed() { \
	if(OBJECT == NULL) return; \
	NodesState_ s = new NodesState_(); \
	if(OBJECT == NODE) { \
		int changed = 0; \
		Node node = OBJECT; \
		int type = OBJECT.getType(); \
		vec4 value = getEditLineVec4(WIDGET ## _el); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			if(OBJECT.get ## FUNC() != value) { \
				changed = 1; \
				break; \
			} \
		} \
		if(changed) { \
			s.init_ ## ACTION(nodes,NODE); \
			foreach(Node n; nodes) { \
				if(n.getType() != type) continue; \
				OBJECT = node_cast(n); \
				OBJECT.set ## FUNC(value); \
			} \
		} \
		OBJECT = node; \
	} else { \
		if(OBJECT.get ## FUNC() != getEditLineVec4(WIDGET ## _el)) { \
			s.init_ ## ACTION(nodes,NODE); \
			OBJECT.set ## FUNC(getEditLineVec4(WIDGET ## _el)); \
		} \
	} \
	setEditLineVec4(WIDGET ## _el,OBJECT.get ## FUNC()); \
	foreach(WidgetEditLine editline; WIDGET ## _el) { \
		if(toolsGetSelection() && editline.isFocused()) { \
			editline.setCursor(0); \
			editline.setSelection(1024); \
		} \
	} \
	save_state(s); \
}

/*
 */
#define PRESSED_CALLBACK_MASK(WIDGET,OBJECT,FUNC,NODE) \
void WIDGET ## _pressed() { \
	if(OBJECT == NULL) return; \
	Node temp = OBJECT; \
	int mask = int("0x" + WIDGET ## _el.getText()); \
	NodesState_ s = new NodesState_(); \
	if(OBJECT == NODE) { \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			if(OBJECT.get ## FUNC() != mask) { \
				if(s.state_type == Unredo::STATE_NONE) s.init_modify(nodes,NODE); \
				OBJECT.set ## FUNC(mask); \
			} \
		} \
	} else if(NODE.isObject() && Object(NODE).getBody() == OBJECT) { \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.isObject() == 0) continue; \
			Object object = node_cast(n); \
			if(object.getBody() == NULL || Body(object.getBody()).getType() != type) continue; \
			OBJECT = body_cast(object.getBody()); \
			if(OBJECT.get ## FUNC() != mask) { \
				if(s.state_type == Unredo::STATE_NONE) s.init_modify(nodes,NODE); \
				OBJECT.set ## FUNC(mask); \
			} \
		} \
	} else { \
		if(OBJECT.get ## FUNC() != mask) { \
			s.init_modify(nodes,NODE); \
			OBJECT.set ## FUNC(mask); \
		} \
	} \
	OBJECT = temp; \
	WIDGET ## _el.setText(editorFormatMask(OBJECT.get ## FUNC())); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

/*
 */
#define PRESSED_CALLBACK_MEAN_SPREAD(WIDGET,OBJECT,FUNC) \
void WIDGET ## _pressed() { \
	if(OBJECT == NULL) return; \
	NodesState_ s = new NodesState_(); \
	int changed = 0; \
	Node node = OBJECT; \
	int type = OBJECT.getType(); \
	float mean = float(WIDGET ## _mean_el.getText()); \
	float spread = float(WIDGET ## _spread_el.getText()); \
	foreach(Node n; nodes) { \
		if(n.getType() != type) continue; \
		OBJECT = node_cast(n); \
		if(OBJECT.get ## FUNC ## Mean() != mean || OBJECT.get ## FUNC ## Spread() != spread) { \
			changed = 1; \
			break; \
		} \
	} \
	if(changed) { \
		s.init_modify(nodes,OBJECT); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			OBJECT.set ## FUNC(mean,spread); \
		} \
	} \
	OBJECT = node; \
	WIDGET ## _mean_el.setText(editorFormat(OBJECT.get ## FUNC ## Mean())); \
	WIDGET ## _spread_el.setText(editorFormat(OBJECT.get ## FUNC ## Spread())); \
	if(toolsGetSelection() && WIDGET ## _mean_el.isFocused()) { \
		WIDGET ## _mean_el.setCursor(0); \
		WIDGET ## _mean_el.setSelection(1024); \
	} \
	if(toolsGetSelection() && WIDGET ## _spread_el.isFocused()) { \
		WIDGET ## _spread_el.setCursor(0); \
		WIDGET ## _spread_el.setSelection(1024); \
	} \
	save_state(s); \
}

#define PRESSED_CALLBACK_MEAN_SPREAD_VEC3(WIDGET,OBJECT,FUNC) \
void WIDGET ## _pressed() { \
	if(OBJECT == NULL) return; \
	NodesState_ s = new NodesState_(); \
	int changed = 0; \
	Node node = OBJECT; \
	int type = OBJECT.getType(); \
	vec4 mean = getEditLineVec3(WIDGET ## _mean_el); \
	vec4 spread = getEditLineVec3(WIDGET ## _spread_el); \
	foreach(Node n; nodes) { \
		if(n.getType() != type) continue; \
		OBJECT = node_cast(n); \
		if(OBJECT.get ## FUNC ## Mean() != mean || OBJECT.get ## FUNC ## Spread() != spread) { \
			changed = 1; \
			break; \
		} \
	} \
	if(changed) { \
		s.init_modify(nodes,OBJECT); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			OBJECT.set ## FUNC(mean,spread); \
		} \
	} \
	OBJECT = node; \
	setEditLineVec3(WIDGET ## _mean_el,OBJECT.get ## FUNC ## Mean()); \
	setEditLineVec3(WIDGET ## _spread_el,OBJECT.get ## FUNC ## Spread()); \
	foreach(WidgetEditLine editline; WIDGET ## _mean_el) { \
		if(toolsGetSelection() && editline.isFocused()) { \
			editline.setCursor(0); \
			editline.setSelection(1024); \
		} \
	} \
	foreach(WidgetEditLine editline; WIDGET ## _spread_el) { \
		if(toolsGetSelection() && editline.isFocused()) { \
			editline.setCursor(0); \
			editline.setSelection(1024); \
		} \
	} \
	save_state(s); \
}

#define PRESSED_CALLBACK_MEAN_SPREAD_VEC4_SAME(WIDGET,OBJECT,FUNC) \
void WIDGET ## _pressed() { \
	if(OBJECT == NULL) return; \
	NodesState_ s = new NodesState_(); \
	int changed = 0; \
	Node node = OBJECT; \
	int type = OBJECT.getType(); \
	vec4 mean = getEditLineSameVec4(WIDGET ## _mean_el); \
	vec4 spread = getEditLineSameVec4(WIDGET ## _spread_el); \
	foreach(Node n; nodes) { \
		if(n.getType() != type) continue; \
		OBJECT = node_cast(n); \
		if(OBJECT.get ## FUNC ## Mean() != mean || OBJECT.get ## FUNC ## Spread() != spread) { \
			changed = 1; \
			break; \
		} \
	} \
	if(changed) { \
		s.init_modify(nodes,OBJECT); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			OBJECT.set ## FUNC(mean,spread); \
		} \
	} \
	OBJECT = node; \
	setEditLineSameVec4(WIDGET ## _mean_el,OBJECT.get ## FUNC ## Mean()); \
	setEditLineSameVec4(WIDGET ## _spread_el,OBJECT.get ## FUNC ## Spread()); \
	foreach(WidgetEditLine editline; WIDGET ## _mean_el) { \
		if(toolsGetSelection() && editline.isFocused()) { \
			editline.setCursor(0); \
			editline.setSelection(1024); \
		} \
	} \
	foreach(WidgetEditLine editline; WIDGET ## _spread_el) { \
		if(toolsGetSelection() && editline.isFocused()) { \
			editline.setCursor(0); \
			editline.setSelection(1024); \
		} \
	} \
	save_state(s); \
}

/*
 */
#define PRESSED_CALLBACK_SIZE(WIDGET_X,WIDGET_Y,OBJECT,FUNC,FUNC_X,FUNC_Y,TYPE,FORMATTER) \
void WIDGET_X ## _pressed() { \
	if(OBJECT == NULL) return; \
	NodesState_ s = new NodesState_(); \
	int changed = 0; \
	Node node = OBJECT; \
	int type = OBJECT.getType(); \
	TYPE width = TYPE(WIDGET_X ## _el.getText()); \
	foreach(Node n; nodes) { \
		if(n.getType() != type) continue; \
		OBJECT = node_cast(n); \
		if(OBJECT.get ## FUNC_X() != width) { \
			changed = 1; \
			break; \
		} \
	} \
	if(changed) { \
		s.init_modify(nodes,OBJECT); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			OBJECT.set ## FUNC(width,OBJECT.get ## FUNC_Y()); \
		} \
	} \
	OBJECT = node; \
	WIDGET_X ## _el.setText(FORMATTER(OBJECT.get ## FUNC_X())); \
	if(toolsGetSelection() && WIDGET_X ## _el.isFocused()) { \
		WIDGET_X ## _el.setCursor(0); \
		WIDGET_X ## _el.setSelection(1024); \
	} \
	save_state(s); \
} \
void WIDGET_Y ## _pressed() { \
	if(OBJECT == NULL) return; \
	NodesState_ s = new NodesState_(); \
	int changed = 0; \
	Node node = OBJECT; \
	int type = OBJECT.getType(); \
	TYPE height = TYPE(WIDGET_Y ## _el.getText()); \
	foreach(Node n; nodes) { \
		if(n.getType() != type) continue; \
		OBJECT = node_cast(n); \
		if(OBJECT.get ## FUNC_Y() != height) { \
			changed = 1; \
			break; \
		} \
	} \
	if(changed) { \
		s.init_modify(nodes,OBJECT); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			OBJECT.set ## FUNC(OBJECT.get ## FUNC_X(),height); \
		} \
	} \
	OBJECT = node; \
	WIDGET_Y ## _el.setText(FORMATTER(OBJECT.get ## FUNC_Y())); \
	if(toolsGetSelection() && WIDGET_Y ## _el.isFocused()) { \
		WIDGET_Y ## _el.setCursor(0); \
		WIDGET_Y ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

/*
 */
#define PRESSED_CALLBACK_SLIDER(WIDGET,OBJECT,FUNC,NODE,ACTION) \
void WIDGET ## _pressed() { \
	int previous_focus; \
	if(OBJECT == NULL) return; \
	float value = float(WIDGET ## _el.getText()); \
	int current_focus = (WIDGET ## _sl.isFocused() || WIDGET ## _el.isFocused()); \
	if(!(previous_focus || current_focus) && OBJECT.get ## FUNC() == value) { \
		WIDGET ## _el.setText(editorFormat(value)); \
		return; \
	} \
	previous_focus = current_focus; \
	Node temp = OBJECT; \
	NodesState_ s = new NodesState_(); \
	if(OBJECT == NODE) { \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			if(OBJECT.get ## FUNC() != value) { \
				if(s.state_type == Unredo::STATE_NONE) s.init_ ## ACTION(nodes,NODE); \
				OBJECT.set ## FUNC(value); \
			} \
		} \
	} else if(NODE.isObject() && Object(NODE).getBody() == OBJECT) { \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.isObject() == 0) continue; \
			Object object = node_cast(n); \
			if(object.getBody() == NULL || Body(object.getBody()).getType() != type) continue; \
			OBJECT = body_cast(object.getBody()); \
			if(OBJECT.get ## FUNC() != value) { \
				if(s.state_type == Unredo::STATE_NONE) s.init_ ## ACTION(nodes,NODE); \
				OBJECT.set ## FUNC(value); \
			} \
		} \
	} else { \
		if(OBJECT.get ## FUNC() != value) { \
			s.init_ ## ACTION(nodes,NODE); \
			OBJECT.set ## FUNC(value);  \
		} \
	} \
	OBJECT = temp; \
	WIDGET ## _el.setText(editorFormat(value)); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s,engine.gui.getFocus()); \
}

#define PRESSED_CALLBACK_SLIDER_PREC(WIDGET,OBJECT,FUNC,PREC,NODE,ACTION) \
void WIDGET ## _pressed() { \
	int previous_focus; \
	if(OBJECT == NULL) return; \
	float value = float(WIDGET ## _el.getText()); \
	int current_focus = (WIDGET ## _sl.isFocused() || WIDGET ## _el.isFocused()); \
	if(!(previous_focus || current_focus) && OBJECT.get ## FUNC() == value) { \
		WIDGET ## _el.setText(editorFormat(value,PREC)); \
		return; \
	} \
	previous_focus = current_focus; \
	Node temp = OBJECT; \
	NodesState_ s = new NodesState_(); \
	if(OBJECT == NODE) { \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			if(OBJECT.get ## FUNC() != value) { \
				if(s.state_type == Unredo::STATE_NONE) s.init_ ## ACTION(nodes,NODE); \
				OBJECT.set ## FUNC(value); \
			} \
		} \
	} else { \
		if(OBJECT.get ## FUNC() != value) { \
			s.init_ ## ACTION(nodes,NODE); \
			OBJECT.set ## FUNC(value);  \
		} \
	} \
	OBJECT = temp; \
	WIDGET ## _el.setText(editorFormat(value,PREC)); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s,engine.gui.getFocus()); \
}

#define PRESSED_CALLBACK_SLIDER_INT(WIDGET,OBJECT,FUNC,NODE,ACTION) \
void WIDGET ## _pressed() { \
	if(OBJECT == NULL) return; \
	NodesState_ s = new NodesState_(); \
	if(OBJECT == NODE) { \
		int changed = 0; \
		Node node = OBJECT; \
		int type = OBJECT.getType(); \
		int value = int(WIDGET ## _el.getText()); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			if(OBJECT.get ## FUNC() != value) { \
				changed = 1; \
				break; \
			} \
		} \
		if((WIDGET ## _el.isFocused() && changed) || WIDGET ## _sl.isFocused()) { \
			s.init_ ## ACTION(nodes,NODE); \
			foreach(Node n; nodes) { \
				if(n.getType() != type) continue; \
				OBJECT = node_cast(n); \
				OBJECT.set ## FUNC(int(WIDGET ## _el.getText())); \
			} \
		} \
		OBJECT = node; \
	} else { \
		if(WIDGET ## _el.isFocused() && OBJECT.get ## FUNC() != int(WIDGET ## _el.getText()) || WIDGET ## _sl.isFocused()) { \
			s.init_ ## ACTION(nodes,NODE); \
			OBJECT.set ## FUNC(int(WIDGET ## _el.getText())); \
		} \
	} \
	WIDGET ## _el.setText(string(OBJECT.get ## FUNC())); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s,engine.gui.getFocus()); \
}

/*
 */
#define CLICKED_CALLBACK(WIDGET,OBJECT,FUNC,NODE) \
void WIDGET ## _clicked() { \
	if(OBJECT == NULL) return; \
	Node temp = OBJECT; \
	NodesState_ s = create_state_modify(nodes,node); \
	if(OBJECT == NODE) { \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			OBJECT.set ## FUNC(WIDGET ## _cb.isChecked()); \
		} \
	} else if(NODE.isObject() && Object(NODE).getBody() == OBJECT) { \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.isObject() == 0) continue; \
			Object object = node_cast(n); \
			if(object.getBody() == NULL || Body(object.getBody()).getType() != type) continue; \
			OBJECT = body_cast(object.getBody()); \
			OBJECT.set ## FUNC(WIDGET ## _cb.isChecked()); \
		} \
	} else { \
		OBJECT.set ## FUNC(WIDGET ## _cb.isChecked()); \
	} \
	OBJECT = temp; \
	save_state(s); \
}

/*
 */
#define CLICKED_CALLBACK_MASK(WIDGET,OBJECT,FUNC,NODE,TITLE,FUNC_ID) \
void WIDGET ## _clicked() { \
	NodesState_ s = new NodesState_(); \
	s.init_modify(nodes,NODE); \
	int old_mask = OBJECT.get ## FUNC(); \
	dialogMask(TITLE,FUNC_ID,old_mask); \
	int type = OBJECT.getType(); \
	Node node = OBJECT; \
	int mask = OBJECT.get ## FUNC(); \
	int changed = (old_mask != mask); \
	if(changed == 0) { \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			if(OBJECT.get ## FUNC() != mask) { \
				changed = 1; \
				break; \
			} \
		} \
	} \
	if(changed) { \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			OBJECT.set ## FUNC(mask); \
		} \
		save_state(s); \
	} else { \
		delete s; \
	} \
	OBJECT = node; \
}

/*
 */
#define LOAD_CLICKED_CALLBACK_MESH(WIDGET,OBJECT,FUNC,TITLE) \
void WIDGET ## _load_clicked() { \
	NodesState_ s = new NodesState_(); \
	string name = OBJECT.get ## FUNC(); \
	if(dialogFileMesh(TITLE,name,DIALOG_FILE_OPEN)) { \
		if(OBJECT.get ## FUNC() != name) { \
			s.init_modify(nodes,OBJECT); \
		} \
		Node node = OBJECT; \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			OBJECT.set ## FUNC(name); \
		} \
		OBJECT = node; \
		WIDGET ## _el.setText(OBJECT.get ## FUNC()); \
	} \
	save_state(s); \
}

/*
 */
#define LOAD_CLICKED_CALLBACK(WIDGET,OBJECT,FUNC,TITLE) \
void WIDGET ## _load_clicked() { \
	NodesState_ s = new NodesState_(); \
	string name = OBJECT.get ## FUNC(); \
	if(dialogFileImage(TITLE,name,DIALOG_FILE_OPEN)) { \
		if(OBJECT.get ## FUNC() != name) { \
			s.init_modify(nodes,OBJECT); \
		} \
		Node node = OBJECT; \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			OBJECT.set ## FUNC(name); \
		} \
		OBJECT = node; \
		WIDGET ## _el.setText(OBJECT.get ## FUNC()); \
	} \
	save_state(s); \
}

#define VIEW_CLICKED_CALLBACK(WIDGET,OBJECT,FUNC,TITLE) \
void WIDGET ## _view_clicked() { \
	if(OBJECT.get ## FUNC() != "") { \
		dialogImage(TITLE,OBJECT.get ## FUNC()); \
	} \
}

#define CLEAR_CLICKED_CALLBACK(WIDGET,OBJECT,FUNC,MESSAGE,DEFAULT,DEFAULT_STR) \
void WIDGET ## _clear_clicked() { \
	NodesState_ s = new NodesState_(); \
	if(dialogMessageYesNo(TR("Confirm"),MESSAGE)) { \
		int changed = 0; \
		Node node = OBJECT; \
		int type = OBJECT.getType(); \
		foreach(Node n; nodes) { \
			if(n.getType() != type) continue; \
			OBJECT = node_cast(n); \
			if(OBJECT.get ## FUNC() != DEFAULT_STR) { \
				changed = 1; \
				break; \
			} \
		} \
		if(changed) { \
			s.init_modify(nodes,OBJECT); \
			foreach(Node n; nodes) { \
				if(n.getType() != type) continue; \
				OBJECT = node_cast(n); \
				OBJECT.set ## FUNC(DEFAULT); \
			} \
		} \
		OBJECT = node; \
		WIDGET ## _el.setText(OBJECT.get ## FUNC()); \
	} \
	save_state(s); \
}

#define CLICKED_CALLBACK_NODE(WIDGET,FUNC) \
void WIDGET ## _clicked() { \
	NodesState_ s = create_state_modify(nodes,node); \
	foreach(Node node; nodes) { \
		node.set ## FUNC(WIDGET ## _cb.isChecked()); \
		WIDGET ## _cb.setChecked(node.is ## FUNC()); \
		update_decals(node); \
	} \
	save_state(s); \
}

#define CHANGED_CALLBACK_COMBOBOX(WIDGET,OBJECT,FUNC) \
void WIDGET ## _changed() { \
	NodesState_ s = new NodesState_(); \
	int isInitModify = 0; \
	int value = WIDGET ## _cb.getCurrentItem() - 1; \
	int type = OBJECT.getType(); \
	foreach(Node n; nodes) { \
		if(n.getType() != type) continue; \
		OBJECT = node_cast(n); \
		if(OBJECT.get ## FUNC() != value) { \
			if(isInitModify == 0) { \
				s.init_modify(nodes,OBJECT); \
				isInitModify = 1; \
			} \
			OBJECT.set ## FUNC(value); \
			WIDGET ## _cb.setCurrentItem(OBJECT.get ## FUNC() + 1); \
		} \
	} \
	save_state(s); \
}

