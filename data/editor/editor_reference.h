/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_reference.h
 * Desc:    Reference mode
 * Version: 1.08
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Pavel Kurinnoy <figonet@unigine.com>
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
namespace Reference {
	
	/*
	 */
	WidgetGroupBox main_gb;
	
	Reference references[0];
	
	class Reference {
		
		string name;					// node name
		string filename;				// file name
		string backup;					// backup name
		
		NodeReference reference;		// reference node
		NodeReference references[0];	// cloned reference nodes
		NodeDummy dummy;				// dummy parent
		Node editor[0];					// editor nodes
		
		/*
		 */
		Reference(Node node) {
			
			reference = node_cast(node);
			name = reference.getNodeName();
			filename = engine.filesystem.getFileName(name);
			backup = filename + ".backup";
			
			File file = new File(name,"rb");
			if(file.isOpened() == 0) {
				log.error("Can't load reference file %s",name);
				return;
			}
			
			File backup = new File(backup,"wb");
			if(backup.isOpened() == 0) {
				file.close();
				log.error("Can't create backup reference file %s",name);
				return;
			}
			
			backup.writeStream(file,file.getSize());
			
			file.close();
			backup.close();
			
			delete file;
			delete backup;
		}
		~Reference() {
			delete dummy;
		}
		
		void get_reference(Node node) {
			
			if(node == NULL) return;
			
			if(node.getType() == NODE_REFERENCE) {
				NodeReference reference = node_cast(node);
				
				if(reference.getNodeName() == name) {
					references.append(reference);
				}
				node = reference.getNode();
				if(node == NULL) return;
			}
			forloop(int i = 0; node.getNumChilds()) {
				get_reference(node.getChild(i));
			}
		}
		
		void enable(Node nodes[]) {
			
			// collect nodes
			foreach(Node node; nodes) {
				get_reference(node);
			}
			forloop(int i = 0; engine.editor.getNumNodes()) {
				get_reference(engine.editor.getNode(i));
				editor.append(engine.editor.getNode(i));
			}
			
			// clear editor nodes
			for(int i = editor.size() - 1; i >= 0; i--) {
				engine.editor.releaseNode(editor[i]);
			}
			
			// clone reference node
			Node node = reference.getNode();
			if(node != NULL) {
				
				dummy = new NodeDummy();
				dummy.setWorldTransform(reference.getWorldTransform());
				
				Node child = node_clone(node);
				dummy.addWorldChild(child);
				
				engine.editor.addNode(child);
				reference.setEnabled(0);
			}
		}
		
		void disable(int reset = 0) {
			
			if(reset) {
				engine.world.clearNode(name);
				remove(engine.filesystem.getFileName(name));
				rename(engine.filesystem.getFileName(backup),engine.filesystem.getFileName(name));
			} else {
				Node new_reference = NULL;
				forloop(int i = 0; engine.editor.getNumNodes()) {
					Node node = engine.editor.getNode(i);
					if(node.getParent() == dummy) {
						new_reference = node;
						break;
					}
				}
				if(toolsGetKeepBackups() == 0) remove(engine.filesystem.getFileName(backup));
				new_reference.setWorldTransform(reference.getIWorldTransform() * dummy.getWorldTransform() * new_reference.getWorldTransform());
				engine.world.saveNode(savePath(name),new_reference,toolsGetNodeMaterials());
			}
			
			// reload nodes
			foreach(NodeReference reference; references) {
				reference.setNodeName(name);
			}
			
			// enable reference node
			reference.setEnabled(1);
			
			// update references
			update();
			
			// remove reference nodes
			for(int i = engine.editor.getNumNodes() - 1; i >= 0; i--) {
				engine.editor.removeNode(engine.editor.getNode(i));
			}
			
			// restore editor nodes
			foreach(Node node; editor) {
				if(engine.editor.isNode(node) == 0) engine.editor.addNode(node);
			}
		}
		
		void update() {
			
			// fix hierarchy
			int num_root_nodes = 0;
			int num_free_nodes = 0;
			forloop(int i = 0; engine.editor.getNumNodes()) {
				Node node = engine.editor.getNode(i);
				if(node.getParent() == dummy) num_root_nodes++;
				if(node.getParent() == NULL) num_free_nodes++;
			}
			if(num_root_nodes == 0 && num_free_nodes == 1) {
				forloop(int i = 0; engine.editor.getNumNodes()) {
					Node node = engine.editor.getNode(i);
					if(node.getParent() == NULL) {
						node.setWorldParent(dummy);
						break;
					}
				}
			}
		}
		
		void reload() {
			
			if(engine.editor.getNumNodes() == 0) return;
			
			Node node = engine.editor.getNode(0);
			
			// save node
			node.setWorldTransform(reference.getIWorldTransform() * dummy.getWorldTransform() * node.getWorldTransform());
			engine.world.saveNode(name,node,toolsGetNodeMaterials());
			node.setWorldTransform(reference.getWorldTransform() * dummy.getIWorldTransform() * node.getWorldTransform());
			
			// reload nodes
			foreach(NodeReference reference; references) {
				reference.setNodeName(name);
			}
		}
	};
	
	/*
	 */
	void enable(Node node) {
		
		if(NodeReference(node_cast(node)).getNode() == NULL) {
			log.error("Can't load the %s\n",NodeReference(node_cast(node)).getNodeName());
			return;
		}
		Node nodes[] = ();
		foreach(Reference reference; references) {
			foreach(Node node; reference.editor) {
				nodes.append(node);
			}
		}
		
		Reference reference = new Reference(node);
		references.append(reference);
		reference.enable(nodes);
		
		Nodes::update_nodes();
		if(engine.editor.getNumNodes()) {
			Nodes::select_node(engine.editor.getNode(0),0);
		} else {
			Nodes::select_node(NULL,0);
		}
		engine.editor.needReload();
		
		gui.addChild(main_gb,GUI_ALIGN_OVERLAP);
		main_gb.arrange();
		
		set_widget_position(main_gb);
	}
	
	void disable(int reset = 0) {
		
		Reference reference = references[references.size() - 1];
		references.remove();
		reference.disable(reset);
		delete reference;
		
		Nodes::update_nodes();
		Nodes::select_node(NULL,0);
		engine.editor.needReload();
		
		if(references.size() == 0) {
			gui.removeChild(main_gb);
		}
	}
	
	/*
	 */
	int is_enabled() {
		return (references.size() > 0);
	}
	
	int is_editing_node(string filename) {
		forloop(int i = 0; references.size()) {
			if(savePath(references[i].filename) == savePath(filename)) return 1;
		}
		return 0;
	}
	
	Node get_root() {
		if(references.size()) return references[references.size() - 1].dummy;
		return NULL;
	}
	
	/*
	 */
	void reload() {
		if(references.size() && engine.editor.getNumNodes()) {
			references[references.size() - 1].reload();
		}
	}
	
	/*
	 */
	void apply_clicked() {
		
		// check node reference
		int num_root_nodes = 0;
		int num_free_nodes = 0;
		Reference reference = references[references.size() - 1];
		forloop(int i = 0; engine.editor.getNumNodes()) {
			Node node = engine.editor.getNode(i);
			if(node.getParent() == reference.dummy) num_root_nodes++;
			if(node.getParent() == NULL) num_free_nodes++;
		}
		if(num_root_nodes == 0 && num_free_nodes == 1) {
			forloop(int i = 0; engine.editor.getNumNodes()) {
				Node node = engine.editor.getNode(i);
				if(node.getParent() == NULL) {
					node.setWorldParent(reference.dummy);
					num_root_nodes = 1;
					num_free_nodes = 0;
					break;
				}
			}
		}
		if(num_root_nodes != 1 || num_free_nodes != 0) {
			log.error("Reference::apply_clicked(): root nodes: %d free nodes: %d\n",num_root_nodes,num_free_nodes);
			dialogMessageOk(TR("Error"),TR("Wrong node reference hierarchy"));
			return;
		}
		
		// save node reference
		reference.reload();
		
		// unredo barier
		unredoRemoveReferenceStates();
		
		// disable node editing
		disable();
		
		if(is_enabled()) update();
	}
	
	/*
	 */
	void cancel_clicked() {
		
		if(dialogMessageYesNo(TR("Confirm reset of changes"),"Are you sure to cancel changes?") == false) return;
		
		// unredo barier
		unredoRemoveReferenceStates();
		
		disable(1);
	}
	
	/******************************************************************************\
	*
	* Save references
	*
	\******************************************************************************/
	
	/*
	 */
	void save() {
		
		while(is_enabled()) {
			disable();
		}
	}
	
	/******************************************************************************\
	*
	* Init/Shutdown
	*
	\******************************************************************************/
	
	/*
	 */
	void init() {
		
	}
	
	/*
	 */
	void shutdown() {
		
		while(is_enabled()) {
			disable();
		}
	}
	
	/**************************************************************************\
	*
	* Update
	*
	\**************************************************************************/
	
	/*
	 */
	void update(int need_reload) {
		
		if(references.size() && engine.editor.getNumNodes()) {
			references[references.size() - 1].update();
		}
	}
	
	/**************************************************************************\
	*
	* Undo/redo
	*
	\**************************************************************************/
	
	/*
	 */
	void undo() {
		if(is_enabled()) {
			disable();
		}
	}
	
	void redo() {
	}
}

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void referenceInit() {
	Reference::init();
}

void referenceShutdown() {
	Reference::shutdown();
}

/*
 */
int referenceUpdate(int need_reload) {
	Reference::update(need_reload);
}

/*
 */
int referenceReload() {
	Reference::reload();
}

/*
 */
void referenceEnable(Node node) {
	Reference::enable(node);
}

void referenceDisable() {
	Reference::disable();
}

/*
 */
int referenceIsEnabled() {
	return Reference::is_enabled();
}

int referenceIsEditingNode(string filename) {
	if(!referenceIsEnabled()) return 0;
	return Reference::is_editing_node(filename);
}

Node referenceGetRoot() {
	return Reference::get_root();
}

/*
 */
void referenceSave() {
	Reference::save();
}
