/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_creator.h
 * Desc:    Unigine editor
 * Version: 1.24
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
 *          Anna Ishina <anet@unigine.com>
 *          Andrey Viktorov <unclebob@unigine.com>
 *          Mikhail Lyubimov <alterego@unigine.com>
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
namespace Creator {
	
	/*
	 */
	using Unigine::Widgets;
	
	/**************************************************************************\
	*
	* Creator
	*
	\**************************************************************************/
	
	/*
	 */
	class Creator {
		string node;
		string icon;
		string material;
		int function;
		int many_nodes;
		Creator(string n,string i,int f,string m = NULL,int b = 0) {
			node = n;
			icon = i;
			function = f;
			material = m;
			many_nodes = b;
		}
	};
	
	/*
	 */
	string creators[0] = (
		CREATE_NAVIGATION_SECTOR		:	new Creator("navigation_sector.node","navigation_sector.png",functionid(create_node)),
		CREATE_NAVIGATION_MESH			:	new Creator("navigation_mesh.node","navigation_mesh.png",functionid(create_node_mesh)),
		CREATE_OBSTACLE_BOX				:	new Creator("obstacle_box.node","obstacle_box.png",functionid(create_node)),
		CREATE_OBSTACLE_SPHERE			:	new Creator("obstacle_sphere.node","obstacle_sphere.png",functionid(create_node)),
		CREATE_OBSTACLE_CAPSULE			:	new Creator("obstacle_capsule.node","obstacle_capsule.png",functionid(create_node)),
		
		CREATE_SOUND_SOURCE				:	new Creator("sound_source.node","sound_source.png",functionid(load_sound_source)),
		CREATE_SOUND_REVERB				:	new Creator("sound_reverb.node","sound_reverb.png",functionid(create_node)),
		
		CREATE_WORLD_LAYER				:	new Creator(NULL,"world_layer.png",functionid(create_world_layer)),
		CREATE_WORLD_SECTOR				:	new Creator("world_sector.node","world_sector.png",functionid(create_node)),
		CREATE_WORLD_PORTAL				:	new Creator("world_portal.node","world_portal.png",functionid(create_node)),
		CREATE_WORLD_TRIGGER			:	new Creator("world_trigger.node","world_trigger.png",functionid(create_node)),
		CREATE_WORLD_CLUSTER			:	new Creator("world_cluster.node","world_cluster.png",functionid(create_node)),
		CREATE_WORLD_CLUTTER			:	new Creator("world_clutter.node","world_clutter.png",functionid(create_node)),
		CREATE_WORLD_SWITCHER			:	new Creator("world_switcher.node","world_switcher.png",functionid(create_node)),
		CREATE_WORLD_OCCLUDER			:	new Creator("world_occluder.node","world_occluder.png",functionid(create_node)),
		CREATE_WORLD_OCCLUDER_MESH		:	new Creator("world_occluder_mesh.node","world_occluder.png",functionid(create_node_mesh)),
		CREATE_WORLD_OCCLUDER_TERRAIN	:	new Creator("world_occluder_terrain.node","world_occluder.png",functionid(create_node)),
		CREATE_WORLD_TRANSFORM_PATH		:	new Creator("world_transform_path.node","world_transform.png",functionid(create_world_transform_path)),
		CREATE_WORLD_TRANSFORM_BONE		:	new Creator("world_transform_bone.node","world_transform.png",functionid(create_node)),
		CREATE_WORLD_EXPRESSION			:	new Creator("world_expression.node","world_expression.png",functionid(create_node)),
		
		CREATE_DECAL_OBJECT_OMNI		:	new Creator("decal_object_omni.node","decal_object_omni.png",functionid(create_node)),
		CREATE_DECAL_OBJECT_PROJ		:	new Creator("decal_object_proj.node","decal_object_proj.png",functionid(create_node)),
		CREATE_DECAL_OBJECT_ORTHO		:	new Creator("decal_object_ortho.node","decal_object_ortho.png",functionid(create_node)),
		CREATE_DECAL_TERRAIN_PROJ		:	new Creator("decal_terrain_proj.node","decal_terrain_proj.png",functionid(create_node)),
		CREATE_DECAL_TERRAIN_ORTHO		:	new Creator("decal_terrain_ortho.node","decal_terrain_ortho.png",functionid(create_node)),
		CREATE_DECAL_DEFERRED_PROJ		:	new Creator("decal_deferred_proj.node","decal_deferred_proj.png",functionid(create_node)),
		CREATE_DECAL_DEFERRED_ORTHO		:	new Creator("decal_deferred_ortho.node","decal_deferred_ortho.png",functionid(create_node)),
		CREATE_DECAL_DEFERRED_MESH		:	new Creator("decal_deferred_mesh.node","decal_deferred_mesh.png",functionid(create_node_mesh)),
		
		CREATE_LIGHT_PROB				:	new Creator("light_prob.node","light_prob.png",functionid(create_node)),
		CREATE_LIGHT_SPOT				:	new Creator("light_spot.node","light_spot.png",functionid(create_node)),
		CREATE_LIGHT_OMNI				:	new Creator("light_omni.node","light_omni.png",functionid(create_node)),
		CREATE_LIGHT_PROJ				:	new Creator("light_proj.node","light_proj.png",functionid(create_node)),
		CREATE_LIGHT_WORLD				:	new Creator("light_world.node","light_world.png",functionid(create_node)),
		
		CREATE_OBJECT_PARTICLES			:	new Creator("object_particles.node","object_particles.png",functionid(create_node)),
		CREATE_OBJECT_VOLUME_BOX		:	new Creator("object_volume_box.node","object_volume.png",functionid(create_node)),
		CREATE_OBJECT_VOLUME_SPHERE		:	new Creator("object_volume_sphere.node","object_volume.png",functionid(create_node)),
		CREATE_OBJECT_VOLUME_OMNI		:	new Creator("object_volume_omni.node","object_volume.png",functionid(create_node)),
		CREATE_OBJECT_VOLUME_PROJ		:	new Creator("object_volume_proj.node","object_volume.png",functionid(create_node)),
		CREATE_OBJECT_GUI				:	new Creator("object_gui.node","object_gui.png",functionid(create_node)),
		CREATE_OBJECT_GUI_MESH			:	new Creator(NULL,"object_gui.png",functionid(create_object_gui_mesh),"gui_base"),
		CREATE_FIELD_SPACER				:	new Creator("field_spacer.node","field_spacer.png",functionid(create_node)),
		CREATE_FIELD_ANIMATION			:	new Creator("field_animation.node","field_animation.png",functionid(create_node)),
		CREATE_PHYSICAL_WIND			:	new Creator("physical_wind.node","physical_wind.png",functionid(create_node)),
		CREATE_PHYSICAL_FORCE			:	new Creator("physical_force.node","physical_force.png",functionid(create_node)),
		CREATE_PHYSICAL_NOISE			:	new Creator("physical_noise.node","physical_noise.png",functionid(create_node)),
		CREATE_PHYSICAL_WATER			:	new Creator("physical_water.node","physical_water.png",functionid(create_node)),
		CREATE_PHYSICAL_TRIGGER			:	new Creator("physical_trigger.node","physical_trigger.png",functionid(create_node)),
		
		CREATE_OBJECT_MESH_STATIC		:	new Creator(NULL,"object_mesh.png",functionid(create_object_mesh_static),"mesh_pbr_base"),
		CREATE_OBJECT_MESH_SKINNED		:	new Creator(NULL,"object_mesh.png",functionid(create_object_mesh_skinned),"mesh_pbr_base"),
		CREATE_OBJECT_MESH_DYNAMIC		:	new Creator(NULL,"object_mesh_dynamic.png",functionid(create_object_mesh_dynamic),"mesh_pbr_base"),
		CREATE_OBJECT_MESH_CLUSTER		:	new Creator("object_mesh_cluster.node","object_mesh_cluster.png",functionid(create_node_mesh),"mesh_pbr_base"),
		CREATE_OBJECT_MESH_CLUTTER		:	new Creator("object_mesh_clutter.node","object_mesh_clutter.png",functionid(create_node_mesh),"mesh_pbr_base"),
		CREATE_OBJECT_TERRAIN			:	new Creator(NULL,"object_terrain.png",functionid(Terrain::create)),
		CREATE_OBJECT_WATER				:	new Creator("object_water.node","object_water.png",functionid(create_node)),
		CREATE_OBJECT_WATER_MESH		:	new Creator("object_water_mesh.node","object_water.png",functionid(create_node_mesh)),
		CREATE_OBJECT_SKY				:	new Creator("object_sky.node","object_sky.png",functionid(create_node)),
		CREATE_OBJECT_GRASS				:	new Creator("object_grass.node","object_grass.png",functionid(create_node)),
		CREATE_OBJECT_BILLBOARD			:	new Creator("object_billboard.node","object_billboard.png",functionid(create_node)),
		CREATE_OBJECT_BILLBOARDS		:	new Creator("object_billboards.node","object_billboards.png",functionid(create_node)),
		CREATE_OBJECT_DUMMY				:	new Creator("object_dummy.node","object_dummy.png",functionid(create_node)),
		
		CREATE_NODE_DUMMY				:	new Creator("node_dummy.node","node_dummy.png",functionid(create_node)),
		CREATE_NODE_LAYER				:	new Creator(NULL,"node_layer.png",functionid(create_node_layer)),
		CREATE_NODE_PIVOT				:	new Creator("node_pivot.node","node_pivot.png",functionid(create_node)),
		CREATE_NODE_TRIGGER				:	new Creator("node_trigger.node","node_trigger.png",functionid(create_node)),
		CREATE_NODE						:	new Creator(NULL,"nodes_load.png",functionid(load_nodes),NULL,1),
		CREATE_NODE_REFERENCE			:	new Creator(NULL,"node_reference.png",functionid(load_node_reference)),
		CREATE_PLAYER_DUMMY				:	new Creator("player_dummy.node","player_dummy.png",functionid(create_node)),
		CREATE_PLAYER_PERSECUTOR		:	new Creator("player_persecutor.node","player_persecutor.png",functionid(create_node)),
		
		CREATE_PRIMITIVE_BOX			:	new Creator(NULL,"object_mesh_dynamic.png",functionid(PrimitiveBox::create)),
		CREATE_PRIMITIVE_SPHERE			:	new Creator(NULL,"object_mesh_dynamic.png",functionid(PrimitiveSphere::create)),
		CREATE_PRIMITIVE_PLANE			:	new Creator(NULL,"object_mesh_dynamic.png",functionid(PrimitivePlane::create)),
		CREATE_PRIMITIVE_CYLINDER		:	new Creator(NULL,"object_mesh_dynamic.png",functionid(PrimitiveCylinder::create)),
		CREATE_PRIMITIVE_CAPSULE		:	new Creator(NULL,"object_mesh_dynamic.png",functionid(PrimitiveCapsule::create)),
		
		CREATE_FBX						:	new Creator(NULL,"nodes_load.png",functionid(load_fbx)),
	);
	
	Node current_node;
	Node current_nodes[0];
	
	Sprite sprite;
	
	/*
	 */
	void init() {
		
		Primitive::init();
		Terrain::init();
		#ifdef HAS_AUTODESK_IMPORT
			FBX::init();
		#endif
		
		sprite = new Sprite();
		sprite.setTexCoord(vec4(0.0f,0.0f,0.5f,0.5f));
		sprite.setTransform(scale(0.5f,0.5f,1.0f));
	}
	
	void shutdown() {
		
		Primitive::shutdown();
		Terrain::shutdown();
		#ifdef HAS_AUTODESK_IMPORT
			FBX::shutdown();
		#endif
		
		delete sprite;
	}
	
	/*
	 */
	void update(int need_reload) {
		
		if(current_node != NULL) {
			
			// show object node
			if(current_node.isObject()) {
				engine.visualizer.renderObjectSurface(class_cast("Object",current_node),-1,engine.render.getWireframeColor());
			}
			
			// update node tranformation
			nodesUpdateTransform();
			
			// set mouse cursor
			gui.setMouseSprite(sprite.widget);
			
			// remove focus
			::Widget focus = gui.getFocus();
			if(focus != NULL) focus.removeFocus();
		}
	}
	
	/*
	 */
	void set_node(Node n,string texture_name) {
		
		current_node = n;
		
		// save selection
		Node selection[] = ();
		nodesGetNodes(selection);
		
		Node last_selected = Nodes::node;
		Nodes::NodesState_ s = Nodes::create_state_new((current_node),selection,last_selected);
		Nodes::new_state = s;
		
		// create node name
		string name = current_node.getName();
		if(strlen(name) == 0 && toolsGetNodeName()) {
			name = current_node.getTypeName();
			
			current_node = node_cast(current_node);
			switch(current_node.getType()) {
				case NODE_REFERENCE:
				case NODE_LAYER: name = basename(current_node.call("getNodeName")); break;
				case NODE_OBJECT_MESH_STATIC:
				case NODE_OBJECT_MESH_SKINNED: name = basename(current_node.call("getMeshName")); break;
			}
			
			name = extension(name,NULL);
			if(strlen(name)) current_node.setName(name);
		}
		
		current_node.setName(getNewNodeName(name));
		
		// node properties
		if(current_node.isObject()) {
			Object object = node_cast(current_node);
			forloop(int i = 0; object.getNumSurfaces()) {
				if(object.getProperty(i) == NULL) object.setProperty("surface_base",i);
			}
		}
		
		// add node to editor
		engine.editor.addNode(current_node);
		
		// reparent node
		if(toolsGetNodeReparent() && last_selected != NULL) {
			last_selected.addWorldChild(current_node);
		}
		
		// add node to update list
		engine.world.addUpdateNode(current_node);
		
		// update node
		if(!nodesTypeCheck(current_node.getType())) {
			nodesClearTypeFilter();
		}
		
		nodesUpdate(1);
		nodesSelectNew(current_node,0);
		engine.editor.needReload();
		
		// update sprite
		sprite.setTexture(texture_name);
	}
	
	/*
	 */
	void add_node_to_editor(Node node,Node last) {
		// create node name
		string name = node.getName();
		if(strlen(name) == 0 && toolsGetNodeName()) {
			name = node.getTypeName();
			
			node = node_cast(node);
			switch(node.getType()) {
				case NODE_REFERENCE:
				case NODE_LAYER: name = basename(node.call("getNodeName")); break;
				case NODE_OBJECT_MESH_STATIC:
				case NODE_OBJECT_MESH_SKINNED: name = basename(node.call("getMeshName")); break;
			}
			
			name = extension(name,NULL);
			if(strlen(name)) node.setName(name);
		}
		
		node.setName(getNewNodeName(name));
		
		// node properties
		if(node.isObject()) {
			Object object = node_cast(node);
			forloop(int i = 0; object.getNumSurfaces()) {
				if(object.getProperty(i) == NULL) object.setProperty("surface_base",i);
			}
		}
		
		// add node to editor
		engine.editor.addNode(node);
		
		// reparent node
		if(toolsGetNodeReparent() && last != NULL) {
			last.addWorldChild(node);
		}
	}
	
	/*
	 */
	void set_nodes(Node nodes[],string texture_name) {
		
		current_nodes.copy(nodes);
		
		// save selection
		Node selection[] = ();
		nodesGetNodes(selection);
		
		Node last_selected = Nodes::node;
		Nodes::NodesState_ s = Nodes::create_state_new(current_nodes,selection,last_selected);
		Nodes::new_state = s;
		
		forloop(int i = 0; current_nodes.size()) add_node_to_editor(current_nodes[i],last_selected);
		
		// add node to update list
		engine.world.addUpdateNodes(current_nodes);
		
		// update node
		
		foreach(Node n; current_nodes) {
			if(!nodesTypeCheck(n.getType())) {
				nodesClearTypeFilter();
			}
		}
		
		nodesUpdate(1);
		nodesSelectNodes(current_nodes,0);
		engine.editor.needReload();
		
		// update sprite
		sprite.setTexture(texture_name);
		
		current_node = current_nodes[current_nodes.size() - 1];
	}
	
	/*
	 */
	int is_many_nodes() {
		return current_nodes.size();
	}
	
	/*
	 */
	Node get_node() {
		return current_node;
	}
	
	/*
	 */
	void get_nodes(Node nodes[]) {
		nodes.copy(current_nodes);
	}
	
	/*
	 */
	void clear_node(int select_node = 1) {
		// restore editor
		gui.setMouseSprite(NULL);
		
		if(select_node) {
			if(is_many_nodes()) {
				nodesSelectNodes(current_nodes,0);
			} else {
				nodesSelect(current_node,0);
			}
		}
		current_nodes.clear();
		current_node = NULL;
	}
	
	/*
	 */
	void remove_node() {
		
		// remove node with children
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
		
		if(is_many_nodes()) {
			forloop(int i = 0; current_nodes.size()) remove_node(current_nodes[i]);
			current_nodes.clear();
		} else {
			remove_node(current_node);
		}
		
		// restore editor
		gui.setMouseSprite(NULL);
		
		// update node
		nodesUpdate(1);
		nodesSelect(NULL,0);
		engine.editor.needReload();
		current_node = NULL;
	}
	
	/*
	 */
	void create(int type) {
		
		Creator creator = creators[type];
		
		if(!creator.many_nodes) {
			Node node = creator.node != NULL ? call(creator.function,creator.node) : call(creator.function);
			if(node == NULL) return;
			
			if(creator.material != NULL) node_cast(node).call("setMaterial",creator.material,"*");
			
			set_node(node,format("editor/gui/%s",creator.icon));
			
			return;
		}
		
		Node nodes[0];
		Node node = creator.node != NULL ? call(creator.function,creator.node,nodes.id()) : call(creator.function,nodes.id());
		
		if(node == NULL) return;
		
		if(nodes.size() == 1) {
			if(creator.material != NULL) node_cast(node).call("setMaterial",creator.material,"*");
			set_node(node,format("editor/gui/%s",creator.icon));
			
			return;
		}
		
		if(creator.material != NULL) {
			forloop(int i = 0; nodes.size())
				node_cast(nodes[i]).call("setMaterial",creator.material,"*");
		}
		
		set_nodes(nodes,format("editor/gui/%s",creator.icon));
	}
	
	void update_world_libraries(string name) {
		
		template library<TYPE>{
			int check_ ## TYPE(Xml xml) {
				if(xml.getName() != "library") return 0;
				forloop(int j = 0; engine. ## TYPE ## .getNumWorldLibraries()) {
					if(engine. ## TYPE ## .getWorldLibraryName(j) == xml.getData()) return 0;
				}
				return 1;
			}
			
			void load_ ## TYPE(Xml xml) {
				forloop(int i = 0; xml.getNumChilds()) {
					Xml x = xml.getChild(i);
					if(check_ ## TYPE(x) == 0) continue;
					
					string name = basename(x.getData());
					
					if(dialogMessageYesNo(TR("Confirm"),format(TR("Add \"%s\" library to the world?"),name))) {
						engine. ## TYPE ## .addWorldLibrary(x.getData());
					}
				}
			}
		}
		
		library<materials>;
		library<properties>;
		
		Xml xml = new Xml();
		if(xml.load(name) == 0) return;
		
		forloop(int i = 0; xml.getNumChilds()) {
			Xml x = xml.getChild(i);
			if(x.getName() == "materials") load_materials(x);
			if(x.getName() == "properties") load_properties(x);
		}
	}
	
	string dialog_load_mesh() {
		
		// TODO we require multiple dialogs for save path to different resources
		string mesh_name;
		if(dialogFileMesh(TR("Select mesh to load"),mesh_name,DIALOG_FILE_OPEN) == 0) return NULL;
		if(is_file(savePath(mesh_name)) == 0) return NULL;
		
		if(strlen(mesh_name) == 0) return NULL;
		return mesh_name;
	}
	
	/*
	 */
	Node create_node(string node_name) {
		string path = savePath("/editor/resources/nodes/" + node_name);
		Node node = engine.world.loadNode(path,0);
		assert(node != NULL);
		return node_cast(node);
	}
	
	Node create_node_mesh(string name) {
		
		string mesh_name = dialog_load_mesh();
		if(mesh_name == NULL) return NULL;
		
		Node node = create_node(name);
		if(node == NULL) {
			dialogMessageOk(TR("Error"));
			return NULL;
		}
		node.call("loadMesh",mesh_name);
		node.call("setMeshName",mesh_name);
		return node;
	}
	
	/*
	 */
	Node create_node_layer() {
		
		string name = "";
		if(dialogFileNode(TR("Add Layer"),name) == 0) return NULL;
		
		NodeLayer node = node_remove(new NodeLayer(name));
		if(node == NULL) {
			dialogMessageOk(TR("Error"));
			return NULL;
		}
		
		if(is_file(savePath(name))) {
			Node nodes[0];
			if(engine.world.loadNodes(name,nodes)) {
				forloop(int i = 0; nodes.size()) {
					node.addWorldChild(nodes[i]);
				}
			} else {
				dialogMessageOk(TR("Error"));
			}
		}
		
		return node;
	}
	
	/*
	 */
	Node load_sound_source(string name) {
		
		string sound_source_name;
		if(dialogFileSound(TR("Load Sound Source"),sound_source_name,DIALOG_FILE_OPEN) == 0) return NULL;
		if(is_file(savePath(sound_source_name)) == 0) return NULL;
		
		SoundSource source = node_remove(new SoundSource(sound_source_name,false));
		if(source == NULL) {
			dialogMessageOk(TR("Error"));
			return NULL;
		}
		
		source.play();
		return source;
	}
	
	/*
	 */
	Node create_world_layer() {
		
		string name;
		if(dialogFileNode(TR("Select Node"),name,DIALOG_FILE_OPEN) == 0) return NULL;
		
		WorldLayer node = NULL;
		
		if(is_file(savePath(name))) {
			node = node_remove(new WorldLayer(Vec3_one,name));
			node = Nodes::worldLayerResize(node,name);
		}
		
		return node;
	}
	
	/*
	 */
	Node create_world_transform_path(string name) {
		
		string path_name;
		if(dialogFilePath(TR("Load World Transform Path"),path_name,DIALOG_FILE_OPEN) == 0) return NULL;
		if(is_file(savePath(path_name)) == 0) return NULL;
		
		Node node = create_node(name);
		if(node == NULL) {
			dialogMessageOk(TR("Error"));
			return NULL;
		}
		
		WorldTransformPath(node).setPathName(path_name);
		return node;
	}
	
	/*
	 */
	Node load_nodes(Node nodes[]) {
		
		string node_name;
		if(dialogFileNode(TR("Import Node"),node_name,DIALOG_FILE_OPEN) == 0) return NULL;
		if(is_file(savePath(node_name)) == 0) return NULL;
		
		if(!engine.world.loadNodes(node_name,nodes)) {
			dialogMessageOk(TR("Import Error"));
			return NULL;
		}
		
		update_world_libraries(savePath(node_name));
		
		return (nodes.size() > 0 ? nodes[0] : NULL);
	}
	
	/*
	 */
	Node load_node_reference() {
		
		string node_name;
		if(dialogFileNode(TR("Add Node Reference"),node_name,DIALOG_FILE_OPEN) == 0) return NULL;
		if(is_file(savePath(node_name)) == 0) return NULL;
		if(referenceIsEditingNode(engine.filesystem.getFileName(node_name))) {
			dialogMessageOk(TR("Import Error"),format(TR("Can't import %s\nthat references the current reference node in the edit mode."),node_name));
			return NULL;
		}
		
		NodeReference node = node_remove(new NodeReference(node_name));
		if(node == NULL) {
			dialogMessageOk(TR("Import Error"));
			return NULL;
		}
		
		string name = node.getNodeName();
		foreach(Reference::Reference reference; Reference::references) {
			if(strcmp(name,reference.name) == 0) {
				dialogMessageOk(TR("Import Error"),format(TR("Can't import %s\nthat references the current reference node in the edit mode."),name));
				return NULL;
			}
		}
		
		return node;
	}
	
	/*
	 */
	Node load_fbx() {
		#ifndef HAS_AUTODESK_IMPORT
			log.error("Autodesk plugin is not loaded\n");
			dialogMessageOk(TR("Error"));
			
			return NULL;
		#else
			string fbx_name;
			if(dialogFileFbx(TR("Import FBX"),fbx_name,DIALOG_FILE_OPEN) == 0) return NULL;
			if(is_file(savePath(fbx_name)) == 0) return NULL;
			
			Node fbx_root = FBX::create(fbx_name);
			if(fbx_root == NULL) return NULL;
			
			return node_remove(fbx_root);
		#endif
	}
	
	/*
	 */
	template create_object_mesh<NAME> {
		string mesh_name = dialog_load_mesh();
		if(mesh_name == NULL) return NULL;
		
		NAME object = node_remove(new NAME(mesh_name));
		
		if(object == NULL) {
			dialogMessageOk(TR("Error"));
			return NULL;
		}
	}
	
	/*
	 */
	Node create_object_mesh_static() {
		create_object_mesh<ObjectMeshStatic>;
		return object;
	}
	
	/*
	 */
	Node create_object_mesh_skinned () {
		create_object_mesh<ObjectMeshSkinned>;
		return object;
	}
	
	/*
	 */
	Node create_object_mesh_dynamic() {
		create_object_mesh<ObjectMeshDynamic>;
		return object;
	}
	
	/*
	 */
	Node create_object_gui_mesh() {
		create_object_mesh<ObjectGuiMesh>;
		
		object.setCollider(0);
		return object;
	}
	
} /* namespace Creator */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void creatorInit() {
	Creator::init();
}

/*
 */
void creatorShutdown() {
	Creator::shutdown();
}

/*
 */
void creatorUpdate(int need_reload) {
	Creator::update(need_reload);
}

/*
 */
void creatorSetNode(Node node,string name) {
	Creator::set_node(node,name);
}

/*
 */
Node creatorGetNode() {
	return Creator::get_node();
}

/*
 */
void creatorClear(int select_node) {
	Creator::clear_node(select_node);
}

/*
 */
void creatorRemove() {
	Creator::remove_node();
}

/*
 */
int creatorIsManyNodes() {
	return Creator::is_many_nodes();
}

/*
 */
void creatorGetNodes(Node nodes[]) {
	Creator::get_nodes(nodes);
}
