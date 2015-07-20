/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_isolation.h
 * Desc:    Isolation mode
 * Version: 1.04
 * Author:  Valia Vaneeva <fattie@unigine.com>
 *          Alexander Zapryagaev <frustum@unigine.com>
 *          Mikhail Lyubimov <alterego@unigine.com>
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

namespace Isolation {
	
	/*
	 */
	WidgetGroupBox main_gb;
	
	/*
	 */
	class IsolationNode {
		
		Node node;
		int enabled;
		int surfaces[0];
		
		IsolationNode(Node n) {
			node = n;
			enabled = node.isEnabled();
		}
		
		void enable() {
			
			int type = node.getType();
			
			// disable object surfaces
			if(node.isObject()) {
				Object object = node_cast(node);
				surfaces.resize(object.getNumSurfaces());
				forloop(int i = 0; object.getNumSurfaces()) {
					surfaces[i] = object.isEnabled(i);
					object.setEnabled(0,i);
				}
			}
			
			// disable container nodes
			else if(type == NODE_REFERENCE ||
				type == NODE_WORLD_CLUSTER) {
				node.setEnabled(0);
			}
		}
		
		void disable() {
			
			int type = node.getType();
			
			// enable object surfaces
			if(node.isObject()) {
				Object object = node_cast(node);
				forloop(int i = 0; object.getNumSurfaces()) {
					object.setEnabled(surfaces[i],i);
				}
			}
			
			// enable container nodes
			else if(type == NODE_REFERENCE ||
				type == NODE_WORLD_CLUSTER) {
				node.setEnabled(enabled);
			}
		}
	};
	
	/*
	 */
	class RootNode {
		
		Node parent;
		int node_position;
		
		RootNode(Node node) {
			node_position = ::Nodes::get_node_position(node);
			parent = node.getParent();
		}
		
		void restore_parent(Node node) {
			if(parent != node.getParent()) node.setWorldParent(parent);
		}
		
		void restore_position(Node node) {
			if(parent == node.getParent()) ::Nodes::move_to_position(node,node_position);
		}
	};
	
	IsolationNode isolations[];
	RootNode root_nodes[];
	
	#include <editor/editor_isolation_unredo.h>
	
	/*
	 */
	void enable() {
		
		Node nodes[0];
		nodesGetNodes(nodes);
		
		Node hierarchy[];
		void get_hierarchy(Node node) {
			forloop(int i = 0; node.getNumChilds()) {
				Node child = node.getChild(i);
				hierarchy.append(child);
				get_hierarchy(child);
			}
		}
		
		forloop(int i = 0; nodes.size()) {
			Node node = nodes[i];
			hierarchy.append(node);
			if(node.isFolded()) {
				get_hierarchy(node);
			}
			Node parent = node.getParent();
			Node parents[0];
			while(parent != NULL) {
				parents.append(parent);
				if(nodes.find(parent) != -1) {
					foreach(Node n; parents) {
						hierarchy.append(n);
					}
					break;
				}
				parent = parent.getParent();
			}
		}
		
		forloop(int i = 0; engine.editor.getNumNodes()) {
			Node node = engine.editor.getNode(i);
			if(hierarchy.check(node)) continue;
			IsolationNode isolation = new IsolationNode(node);
			isolation.enable();
			isolations.append(node,isolation);
		}
		
		Nodes::update_nodes();
		
		nodesGetCurrentRootNodes(nodes);
		foreach(Node node; nodes) {
			root_nodes.append(node.getID(),new RootNode(node));
		}
		Nodes::reselect_nodes();
		engine.editor.needReload();
		gui.addChild(main_gb,GUI_ALIGN_OVERLAP);
		main_gb.arrange();
		
		set_widget_position(main_gb);
	}
	
	void disable() {
		Node nodes[0];
		nodesGetCurrentRootNodes(nodes);
		
		int num = 0;
		Node parent = NULL;
		foreach(Node node; nodes) {
			if(root_nodes.check(node.getID())) {
				root_nodes[node.getID()].restore_position(node);
				parent = node.getParent();
				num = ::Nodes::get_node_position(node);
			}
		}
		
		foreach(Node node; nodes) {
			if(root_nodes.check(node.getID())) continue;
			node.setWorldParent(parent);
			::Nodes::move_to_position(node,num);
		}
		
		selectCurrentNodes();
		
		foreach(IsolationNode isolation; isolations) {
			isolation.disable();
		}
		
		root_nodes.clear();
		isolations.clear();
		
		Nodes::update_nodes();
		Nodes::reselect_nodes();
		engine.editor.needReload();
		
		gui.removeChild(main_gb);
	}
	
	/*
	 */
	int is_enabled() {
		return (isolations.size() > 0);
	}
	
	int is_node(Node node) {
		foreach(IsolationNode isolation; isolations) {
			if(isolation.node == node) return 1;
		}
		return 0;
	}
	
	int restore_parent_node(Node node) {
		if(root_nodes.check(node.getID())) {
			root_nodes[node.getID()].restore_parent(node);
			return true;
		}
		return false;
	}
	
	void create_unredo_state(int type) {
		IsolationState_ s = create_state(type);
		save_state(s);
	}
	
	/*
	 */
	void reset_clicked() {
		isolationDisable();
	}
	
	/**************************************************************************\
	*
	* Init/Shutdown
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
	}
	
	/*
	 */
	void shutdown() {
		if(is_enabled()) {
			disable();
		}
	}
}

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void isolationInit() {
	Isolation::init();
}

void isolationShutdown() {
	Isolation::shutdown();
}

/*
 */
void isolationEnable() {
	Isolation::enable();
	Isolation::create_unredo_state(Unredo::STATE_NEW);
}

void isolationDisable() {
	Isolation::create_unredo_state(Unredo::STATE_REMOVE);
	Isolation::disable();
}

/*
 */
int isolationIsEnabled() {
	return Isolation::is_enabled();
}

int isolationIsNode(Node node) {
	return Isolation::is_node(node);
}

int isolationRestoreParentNode(Node node) {
	return Isolation::restore_parent_node(node);
}
/*
 */
void isolationSave() {
	if(isolationIsEnabled()) {
		thread(functionid(isolationRestore));
		isolationDisable();
	}
}

void isolationRestore() {
	wait;
	isolationEnable();
}
