/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    mesh_combiner.cpp
 * Desc:    Mesh combiner plugin
 * Version: 1.00
 * Author:  Mikhail Lyubimov <alterego@unigine.com>
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

#include <core/unigine.h>

/******************************************************************************\
*
* Plugin
*
\******************************************************************************/

/*
 */
string getName() {
	return "MeshCombiner";
}

/*
 */
void init(PluginMeta m) {
	MeshCombiner::init(m);
}

void shutdown() {
	MeshCombiner::shutdown();
}

void update(int need_reload) {
	MeshCombiner::update(need_reload);
}

void save() {
	MeshCombiner::save();
}

void show() {
	MeshCombiner::show();
}
/******************************************************************************\
*
* MeshCombiner
*
\******************************************************************************/

/*
 */
namespace MeshCombiner {
	
	/*
	 */
	using Unigine::Widgets;
	
	/*
	 */
	UserInterface interface;
	
	Window window;
	TreeBox treebox;
	EditLine path_el;
	VBox vbox;
	
	EditLine grid_x_el;
	EditLine grid_y_el;
	EditLine grid_z_el;
	
	Button hide_b;
	Button show_b;
	
	int id_nodes[0];
	
	ObjectMeshStatic combined_object;
	Node old_node;
	
	PluginMeta meta;
	
	/*
	 */
	void init(PluginMeta m) {
		
		meta = m;
		
		interface = new UserInterface(replace(meta.source,".cpp",".ui"),getName() + "::");
		hotkeysAddUiToolTips(interface);
		
		window.arrange();
		pluginsAddWindow(window,meta.title,meta.name);
	}
	
	/*
	 */
	void shutdown() {
		pluginsRemoveWindow(window,meta.name);
		
		hotkeysRemoveUiToolTips(interface);
		delete interface;
	}
	
	/*
	 */
	void update(int need_reload) {
		Node node = nodesGetNode();
		
		if(combined_object == NULL || engine.editor.isNode(combined_object) == 0) {
			if(vbox.isEnabled()) {
				vbox.setEnabled(false);
				path_el.setText("");
				id_nodes.clear();
				update_treebox();
			}
		}
		else {
			vbox.setEnabled(true);
			hide_b.setEnabled(combined_object.isEnabled());
			show_b.setEnabled(!combined_object.isEnabled());
		}
		
		if(need_reload == 0 && node == old_node) return;
		old_node = node;
		
		save();
		
		load(node);
	}
	
	/*
	 */
	void combine_clicked() {
		set_enabled(0);
		
		string path = savePath(path_el.getText());
		
		if(isPathValid(path) == 0) {
			if(dialogFileMesh(TR("Select mesh to save"),path,DIALOG_FILE_SAVE) == 0) return;
			path = extension(path,"mesh");
			path_el.setText(editorPath(path));
			path = savePath(path);
		}
		path = extension(path,"mesh");
		
		Mesh mesh = class_manage(new Mesh());
		Mesh reference_mesh = new Mesh();
		
		
		int materials[];
		
		void combine_node(Node node) {
			if(node.isEnabled() == 0) return;
			if(node.getType() != NODE_OBJECT_MESH_STATIC) return;
			
			ObjectMeshStatic object = node_cast(node);
			
			if(object.getMesh(reference_mesh) == 0) return;
			
			Mat4 mesh_transform = mat4(object.getWorldTransform());
			
			forloop(int surface = 0; object.getNumSurfaces()) {
				if(object.isEnabled(surface) == 0) continue;
				
				string material = object.getMaterialName(surface);
				
				if(engine.materials.isMaterial(material) == 0) continue;
				
				WorldBoundSphere bound = object.getWorldBoundSphere(surface);
				Vec3 pos = bound.getCenter();
				int x = pos.x / int(grid_x_el.getText());
				int y = pos.y / int(grid_y_el.getText());
				int z = pos.z / int(grid_z_el.getText());
				string name = format("%s [%03d %03d %03d]",material,x,y,z);
				
				int id_surface = mesh.findSurface(name);
				if(id_surface == -1) {
					id_surface = mesh.addSurface(name);
					materials.append(name,material);
				}
				
				reference_mesh.setSurfaceTransform(mesh_transform,surface);
				mesh.addMeshSurface(id_surface,reference_mesh,surface);
			}
		}
		
		forloop(int i = 0; id_nodes.size()) {
			Node node = engine.world.getNode(id_nodes[i]);
			if(node == NULL) continue;
			
			if(node.getType() == NODE_REFERENCE) {
				void combine_child(Node node) {
					if(node == NULL) return;
					combine_node(node);
					
					forloop(int i = 0; node.getNumChilds()) {
						combine_child(node.getChild(i));
					}
				}
				NodeReference reference = node_cast(node);
				combine_child(reference.getNode());
			}
			else {
				combine_node(node);
			}
		}
		delete reference_mesh;
		
		mesh.createBounds();
		if(mesh.save(path) == 0) {
			delete mesh;
			return;
		}
		
		combined_object.setMesh(mesh);
		combined_object.setMeshName(editorPath(path));
		
		delete mesh;
		
		combined_object.setProperty("surface_base","*");
		
		forloop(int i = 0; combined_object.getNumSurfaces()) {
			combined_object.setMaterial(materials[combined_object.getSurfaceName(i)],i);
			combined_object.setEnabled(1,i);
		}
		materials.clear();
		combined_object.updateSurfaceBounds();
		
		save();
		
		set_enabled(1);
	}
	
	/*
	 */
	void add_node(Node node) {
		if(node == NULL) return;
		if(node == combined_object) return;
		if(node.getType() == NODE_REFERENCE || node.getType() == NODE_OBJECT_MESH_STATIC) {
			int id = node.getID();
			if(id_nodes.find(id) == -1) id_nodes.append(id);
		}
		
		forloop(int i = 0; node.getNumChilds()) {
			add_node(node.getChild(i));
		}
	}
	
	void add_clicked() {
		Node selection[0];
		
		nodesGetNodes(selection);
		
		forloop(int i = 0; selection.size()) {
			add_node(selection[i]);
		}
		
		update_treebox();
	}
	
	/*
	 */
	void remove_node(Node node) {
		if(node == NULL) return;
		if(node.getType() == NODE_REFERENCE || node.getType() == NODE_OBJECT_MESH_STATIC) {
			int id = node.getID();
			int num = id_nodes.find(id);
			if(num != -1) id_nodes.remove(num);
		}
		
		forloop(int i = 0; node.getNumChilds()) {
			remove_node(node.getChild(i));
		}
	}
	
	void remove_clicked() {
		Node selection[0];
		
		nodesGetNodes(selection);
		
		forloop(int i = 0; selection.size()) {
			remove_node(selection[i]);
		}
		
		update_treebox();
	}
	
	/*
	 */
	void show_clicked() {
		set_enabled(1);
	}
	
	/*
	*/
	void hide_clicked() {
		set_enabled(0);
	}
	
	/*
	*/
	void set_enabled(int enabled) {
		if(combined_object == NULL) return;
		if(combined_object.isEnabled() == enabled) return;
		
		
		
		Node node = nodesGetNode();
		
		Node nodes[0];
		nodes.append(combined_object);
		
		forloop(int i = 0; id_nodes.size()) {
			Node n = engine.world.getNode(id_nodes[i]);
			if(n == NULL) continue;
			nodes.append(n);
		}
		
		
		Nodes::NodesState_ s = Nodes::create_state_modify(nodes,node);
		Nodes::save_state(s);
		
		forloop(int i = 1; nodes.size()) {
			nodes[i].setEnabled(!enabled);
		}
		
		combined_object.setEnabled(enabled);
		
		nodes.clear();
	}
	
	/*
	 */
	void update_treebox() {
		treebox.clear();
		forloop(int i = 0; id_nodes.size()) {
			Node n = engine.world.getNode(id_nodes[i]);
			if(n == NULL) continue;
			treebox.addItem(n.getName());
		}
	}
	
	/*
	 */
	void create_clicked() {
		string path = "";
		if(dialogFileMesh(TR("Select mesh to save"),path,DIALOG_FILE_SAVE) == 0) return;
		
		path = extension(path,"mesh");
		path_el.setText(editorPath(path));
		path = savePath(path);
		
		Mesh mesh = new Mesh();
		mesh.save(path);
		delete mesh;
		
		combined_object = new ObjectMeshStatic(path);
		engine.editor.addNode(combined_object);
		combined_object = node_remove(combined_object);
		
		path = editorPath(path);
		combined_object.setMeshName(path);
		path_el.setText(path);
		
		string name = basename(path);
		name = "Combined mesh " + extension(name,"");
		combined_object.setName(name);
		
		// undo/redo
		Node selection[] = ();
		nodesGetNodes(selection);
		
		Node last_selected = Nodes::node;
		Nodes::NodesState_ s = Nodes::create_state_new((combined_object),selection,last_selected);
		Nodes::save_state(s);
		
		id_nodes.clear();
		update_treebox();
		
		save();
	}
	
	/*
	 */
	void save() {
		if(engine.editor.isNode(combined_object) == 0) return;
		
		Xml xml = new Xml("mesh_combiner");
		
		if(id_nodes.size() > 0) {
			Xml xml_nodes = xml.addChild("id_nodes");
			xml_nodes.setIntArrayData(id_nodes);
		}
		
		ivec3 grid;
		grid.x = int(grid_x_el.getText());
		grid.y = int(grid_y_el.getText());
		grid.z = int(grid_z_el.getText());
		
		Xml xml_grid = xml.addChild("grid");
		xml_grid.setIVec3Data(grid);
		
		
		combined_object.setData(xml.getSubTree());
		delete xml;
	}
	
	/*
	 */
	void load(Node node) {
		if(node == NULL) return;
		if(engine.editor.isNode(node) == 0) return;
		
		string data = node.getData();
		if(data == NULL || is_string(data) == 0 || strlen(data) == 0 || data[0] != '<') return;
		
		Xml xml = new Xml();
		if(xml.parse(data) == 0 || xml.getName() != "mesh_combiner") {
			delete xml;
			return;
		}
		
		combined_object = node_cast(node);
		path_el.setText(editorPath(combined_object.getMeshName()));
		
		id_nodes.clear();
		if(xml.isChild("id_nodes")) {
			Xml xml_nodes = xml.getChild("id_nodes");
			xml_nodes.getIntArrayData(id_nodes);
		}
		update_treebox();
		
		ivec3 grid = ivec3_one * 1000;
		
		if(xml.isChild("grid")) {
			Xml xml_grid = xml.getChild("grid");
			grid = xml_grid.getIVec3Data();
		}
		
		grid_x_el.setText(string(grid.x));
		grid_y_el.setText(string(grid.y));
		grid_z_el.setText(string(grid.z));
		
		delete xml;
	}
	
	/*
	 */
	void windowHide() {
		windowHide(window,meta.name);
	}
	
	void show() {
		pluginsShowWindow(window,meta.name);
	}
}
