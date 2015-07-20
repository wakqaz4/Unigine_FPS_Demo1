/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    city.cpp
 * Desc:    City import plugin
 * Version: 1.15
 * Author:  Andrey Viktorov <unclebob@unigine.com>
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
	return "City";
}

/*
 */
void init(PluginMeta m) {
	MeshUtils::init(m);
}

void shutdown() {
	MeshUtils::shutdown();
}

void update(int need_reload) {
	MeshUtils::update(need_reload);
}

void save() {
	MeshUtils::save();
}

void show() {
	MeshUtils::show();
}
/******************************************************************************\
*
* MeshUtils
*
\******************************************************************************/

/*
 */
namespace MeshUtils {
	
	/*
	 */
	using Unigine::Widgets;
	
	/*
	 */
	UserInterface interface;
	Window window;
	
	int old_nodes;
	
	enum {
		OFFSET_X = 4,
		OFFSET_Y = 100,
		RAYCAST_DEPTH = 5000,
	};
	
	/*
	 */
	PluginMeta meta;
	
	/*
	 */
	void init(PluginMeta m) {
		
		meta = m;
		
		interface = new UserInterface(replace(meta.source,".cpp",".ui"),getName() + "::");
		hotkeysAddUiToolTips(interface);
		
		UI::reload();
		
		window.arrange();
		
		pluginsAddWindow(window,meta.title,meta.name);
		
		old_nodes = engine.editor.getNumNodes();
	}
	
	/*
	 */
	void shutdown() {
		
		update(1);
		
		UI::save();
		
		pluginsRemoveWindow(window,meta.name);
		
		hotkeysRemoveUiToolTips(interface);
		delete interface;
	}
	
	void show() {
		pluginsShowWindow(window,meta.name);
	}
	
	/*
	 */
	void update(int need_reload) {
		int nodes = engine.editor.getNumNodes();
		
		if(need_reload || nodes != old_nodes) {
			old_nodes = nodes;
			UI::reload();
		}
	}
	
	/*
	 */
	void save() {
		UI::save();
	}
	
	/*
	 */
	string get_surfaces_str(string surfaces[]) {
		if(surfaces.size() == 0) return "";
		
		string result = surfaces[0];
		forloop(int i = 1; surfaces.size()) {
			result = result + "," + surfaces[i];
		}
		
		return result;
	}
	
	/*
	 */
	void node_editor_remove(Node node) {
		for(int i = node.getNumChilds() - 1; i >= 0; i--) {
			node_editor_remove(node.getChild(i));
		}
		
		if(engine.editor.isNode(node)) engine.editor.removeNode(node);
	}
	
	/**************************************************************************\
	*
	* Marker
	*
	\**************************************************************************/
	
	/*
	 */
	class Marker {
		
		/*
		 */
		string path;
		string common_surfaces[0];
		
		int num_tiles;
		string tile_names[0];
		string tile_mesh_names[0];
		Vec4 tile_offsets[0];
		
		NodeDummy root;
		
		/*
		 */
		Marker(Xml data) {
			path = data.getChildData("path","");
			common_surfaces.clear();
			
			num_tiles = 0;
			tile_names.clear();
			tile_mesh_names.clear();
			tile_offsets.clear();
		}
		
		/*
		 */
		Marker(string n,string p) {
			path = p;
			common_surfaces.clear();
			
			num_tiles = 0;
			tile_names.clear();
			tile_mesh_names.clear();
			tile_offsets.clear();
			
			root = class_remove(new NodeDummy());
			root.setName(n);
			
			engine.editor.addNode(root);
			nodesReload();
		}
		
		/*
		 */
		~Marker() {
			saveData();
			clear();
			
			root = NULL;
		}
		
		/*
		 */
		void deleteNodes() {
			if(engine.editor.isNode(root)) engine.editor.removeNode(root);
			root = NULL;
			
			nodesReload();
		}
		
		/*
		 */
		void saveData() {
			if(root == NULL) return;
			if(engine.world.isNode(root) == 0) return;
			
			Xml xml = new Xml("marker");
			xml.setChildData("path",path);
			
			root.setData(xml.getSubTree());
			delete xml;
		}
		
		/*
		 */
		int load() {
			clear();
			
			Node tiles = engine.world.loadNode(path);
			if(tiles == NULL) return 0;
			
			tiles = node_append(tiles);
			
			forloop(int i = 0; tiles.getNumChilds()) {
				Node child = tiles.getChild(i);
				if(child.getType() != NODE_OBJECT_MESH_STATIC) continue;
				
				ObjectMeshStatic mesh = node_cast(child);
				
				tile_names.append(mesh.getName());
				tile_mesh_names.append(mesh.getMeshName());
				tile_offsets.append(mesh.getPosition());
				
				forloop(int surface = 0; mesh.getNumSurfaces()) {
					string surface_name = mesh.getSurfaceName(surface);
					
					if(common_surfaces.find(surface_name) != -1) continue;
					common_surfaces.append(surface_name);
				}
				
				num_tiles++;
			}
			
			node_delete(tiles);
			
			return 1;
		}
		
		/*
		 */
		int isValid() {
			return (num_tiles != 0);
		}
		
		/*
		 */
		void clear() {
			common_surfaces.clear();
			
			num_tiles = 0;
			
			tile_names.clear();
			tile_mesh_names.clear();
			tile_offsets.clear();
		}
		
		/*
		 */
		void setRoot(Node node) {
			root = node_cast(node);
		}
		
		Node getRoot() {
			return root;
		}
		
		/*
		 */
		void setName(string n) {
			if(root != NULL) root.setName(n);
		}
		
		string getName() {
			return (root != NULL) ? root.getName() : "";
		}
		
		/*
		 */
		int setPath(string p) {
			string old_path = path;
			path = p;
			
			return (old_path != path);
		}
		
		string getPath() {
			return path;
		}
	};
	
	/**************************************************************************\
	*
	* Layer
	*
	\**************************************************************************/
	
	/*
	 */
	class Layer {
		
		/*
		 */
		string cluster_surfaces[0];
		string cluster_reference;
		float cluster_distance;
		float cluster_shadow;
		int cluster_project;
		int cluster_bake;
		
		string lod_surfaces[0];
		string lod_path;
		int lod_bake;
		
		string decal_surfaces[0];
		string decal_material;
		float decal_radius;
		float decal_distance;
		float decal_fade;
		int decal_bake;
		
		string mesh_surfaces[0];
		string mesh_material;
		string mesh_path;
		float mesh_distance;
		float mesh_fade;
		int mesh_bake;
		
		Marker marker;
		
		NodeDummy root;
		
		ObjectMeshStatic reference_mesh;
		string reference_surfaces[0];
		
		/*
		 */
		Layer(int data,Marker m) {
			marker = m;
			decal_distance = INFINITY;
			mesh_distance = INFINITY;
			cluster_distance = INFINITY;
			
			decal_material = "decal_deferred_base";
			mesh_material = "mesh_base";
			
			if(data is Xml) {
				loadData(data);
			}
			
			if(data is string) {
				root = class_remove(new NodeDummy());
				root.setName(data);
				
				Node marker_root = (marker != NULL) ? marker.getRoot() : NULL;
				
				if(marker_root != NULL) {
					marker_root.addChild(root);
				}
				
				engine.editor.addNode(root);
				nodesReload();
			}
			
			reference_mesh = NULL;
		}
		
		/*
		 */
		~Layer() {
			saveData();
			
			cluster_surfaces.clear();
			lod_surfaces.clear();
			decal_surfaces.clear();
			reference_surfaces.clear();
			
			if(reference_mesh != NULL) {
				node_delete(reference_mesh);
				reference_mesh = NULL;
			}
			
			marker = NULL;
			root = NULL;
		}
		
		/*
		 */
		void deleteNodes() {
			clear();
			
			if(engine.editor.isNode(root)) engine.editor.removeNode(root);
			root = NULL;
			
			nodesReload();
		}
		
		/*
		 */
		void loadData(Xml data) {
			string cluster_surfaces_str = data.getChildData("cluster_surfaces","");
			strsplit(cluster_surfaces_str,",",cluster_surfaces);
			
			cluster_reference = data.getChildData("cluster_reference","");
			cluster_distance = data.getChildData("cluster_distance",INFINITY);
			cluster_shadow = data.getChildData("cluster_shadow",0.0f);
			cluster_project = data.getChildData("cluster_project",0);
			cluster_bake = data.getChildData("cluster_bake",1);
			
			string lod_surfaces_str = data.getChildData("lod_surfaces","");
			strsplit(lod_surfaces_str,",",lod_surfaces);
			
			lod_path = data.getChildData("lod_path","");
			lod_bake = data.getChildData("lod_bake",0);
			
			string decal_surfaces_str = data.getChildData("decal_surfaces","");
			strsplit(decal_surfaces_str,",",decal_surfaces);
			
			decal_material = data.getChildData("decal_material","");
			decal_radius = data.getChildData("decal_radius",0.0f);
			decal_distance = data.getChildData("decal_distance",INFINITY);
			decal_fade = data.getChildData("decal_fade",0.0f);
			decal_bake = data.getChildData("decal_bake",0);
			
			string mesh_surfaces_str = data.getChildData("mesh_surfaces","");
			strsplit(mesh_surfaces_str,",",mesh_surfaces);
			
			mesh_material = data.getChildData("mesh_material","");
			mesh_path = data.getChildData("mesh_path","");
			mesh_distance = data.getChildData("mesh_distance",INFINITY);
			mesh_fade = data.getChildData("mesh_fade",0.0f);
			mesh_bake = data.getChildData("mesh_bake",0);
		}
		
		/*
		 */
		void saveData() {
			if(root == NULL) return;
			if(engine.world.isNode(root) == 0) return;
			
			Xml xml = new Xml("layer");
			xml.setChildData("cluster_surfaces",get_surfaces_str(cluster_surfaces));
			xml.setChildData("cluster_reference",cluster_reference);
			xml.setChildData("cluster_distance",cluster_distance);
			xml.setChildData("cluster_shadow",cluster_shadow);
			xml.setChildData("cluster_project",cluster_project);
			xml.setChildData("cluster_bake",cluster_bake);
			
			xml.setChildData("lod_surfaces",get_surfaces_str(lod_surfaces));
			xml.setChildData("lod_path",lod_path);
			xml.setChildData("lod_bake",lod_bake);
			
			xml.setChildData("decal_surfaces",get_surfaces_str(decal_surfaces));
			xml.setChildData("decal_material",decal_material);
			xml.setChildData("decal_radius",decal_radius);
			xml.setChildData("decal_distance",decal_distance);
			xml.setChildData("decal_fade",decal_fade);
			xml.setChildData("decal_bake",decal_bake);
			
			xml.setChildData("mesh_surfaces",get_surfaces_str(mesh_surfaces));
			xml.setChildData("mesh_material",mesh_material);
			xml.setChildData("mesh_path",mesh_path);
			xml.setChildData("mesh_distance",mesh_distance);
			xml.setChildData("mesh_fade",mesh_fade);
			xml.setChildData("mesh_bake",mesh_bake);
			
			root.setData(xml.getSubTree());
			delete xml;
		}
		
		/*
		 */
		int load() {
			if(marker == NULL) return 0;
			
			if(reference_mesh != NULL) {
				node_delete(reference_mesh);
				reference_mesh = NULL;
			}
			
			Node node = engine.world.loadNode(cluster_reference);
			if(node == NULL) return 0;
			
			node = node_append(node);
			
			if(node.getType() != NODE_OBJECT_MESH_STATIC) {
				node_delete(node);
				return 0;
			}
			
			reference_mesh = node_cast(node);
			reference_mesh.setEnabled(0);
			reference_surfaces.clear();
			
			forloop(int i = 0; reference_mesh.getNumSurfaces()) {
				reference_surfaces.append(reference_mesh.getSurfaceName(i));
			}
			
			return 1;
		}
		
		int isValid() {
			if(cluster_bake) {
				if(reference_mesh == NULL) return 0;
				if(cluster_surfaces.size() == 0) return 0;
				if(cluster_reference == "") return 0;
			}
			
			if(lod_bake) {
				if(reference_mesh == NULL) return 0;
				if(lod_surfaces.size() == 0) return 0;
				if(lod_path == "") return 0;
			}
			
			if(decal_bake) {
				if(decal_surfaces.size() == 0) return 0;
				if(decal_material == "") return 0;
			}
			
			if(mesh_bake) {
				if(mesh_surfaces.size() == 0) return 0;
				if(mesh_material == "") return 0;
				if(mesh_path == "") return 0;
			}
			
			return (cluster_bake || decal_bake || mesh_bake);
		}
		
		/*
		 */
		void collect_marker_transforms(Mesh tile_mesh,Vec3 offset,Mat4 transforms[]) {
			forloop(int surface = 0; tile_mesh.getNumSurfaces()) {
				if(cluster_surfaces.find(tile_mesh.getSurfaceName(surface)) == -1) continue;
				
				int num_indices = tile_mesh.getNumCIndices(surface);
				for(int j = 0; j < num_indices; j += 3) {
					int cindex = tile_mesh.getCIndex(j,surface);
					int tindex = tile_mesh.getTIndex(j,surface);
					
					Vec3 position = Vec3(tile_mesh.getVertex(cindex,surface));
					
					if(cluster_project) {
						Vec3 p0 = position + offset + Vec3(0,0,RAYCAST_DEPTH);
						Vec3 p1 = position + offset - Vec3(0,0,RAYCAST_DEPTH);
						
						Scalar height = 0.0f;
						if(get_intersection(p0,p1,height)) {
							position.z = height - offset.z;
						}
					}
					
					quat rotation = tile_mesh.getTangent(tindex,surface);
					Mat4 reference_transform = Mat4_identity;
					reference_transform.col03 = normalize(rotation.binormal);
					reference_transform.col13 = normalize(cross(rotation.normal,rotation.binormal));
					reference_transform.col23 = normalize(rotation.normal);
					reference_transform.col33 = position;
					
					transforms.append(reference_transform);
				}
			}
		}
		
		/*
		 */
		int get_intersection(Vec3 p0,Vec3 p1,Scalar &height) {
			int ret[0];
			if(engine.world.getIntersectionObjects(p0,p1,ret) == 0) return 0;
			
			int exclude[0];
			exclude.clear();
			
			forloop(int i = 0; ret.size()) {
				Object obj = ret[i];
				if(obj.getType() != NODE_OBJECT_TERRAIN) exclude.append(obj);
			}
			
			ret.clear();
			
			WorldIntersection data = new WorldIntersection();
			if(engine.world.getIntersection(p0,p1,~0,exclude,data) == NULL) {
				delete data;
				return 0;
			}
			
			Vec3 point = data.getPoint();
			height = point.z;
			
			delete data;
			
			return 1;
		}
		
		/*
		 */
		void generate_nodes(string tile_name,ObjectMeshStatic tile_mesh,Vec3 offset,Mat4 transforms[]) {
			
			ObjectMeshCluster cluster = NULL;
			ObjectMeshStatic lod = NULL;
			if(cluster_bake) {
				cluster = Bakery::toMeshCluster(reference_mesh,transforms);
				
				if(lod_bake) {
					string path = pathname(lod_path + "/") + root.getName() + "_" + tile_name + "_lod.mesh";
					lod = Bakery::toMeshFromCluster(cluster,path,lod_surfaces);
				}
			}
			
			DecalDeferredMesh decal = NULL;
			if(decal_bake) {
				decal = Bakery::toDecalDeferredMesh(tile_mesh,decal_radius,decal_material,decal_surfaces);
			}
			
			ObjectMeshStatic mesh = NULL;
			if(mesh_bake) {
				string path = pathname(mesh_path + "/") + root.getName() + "_" + tile_name + ".mesh";
				mesh = Bakery::toObjectMeshStatic(tile_mesh,path,mesh_material,mesh_surfaces);
			}
			
			if(cluster != NULL) {
				engine.editor.addNode(class_remove(cluster));
				
				cluster.setName(tile_name);
				cluster.setVisibleDistance(cluster_distance);
				cluster.setShadowRadius(cluster_shadow);
				
				if(lod != NULL) {
					engine.editor.addNode(class_remove(lod));
					
					lod.setName(tile_name + "_lod");
					lod.setTransform(translate(offset));
					lod.addChild(cluster);
					
					root.addChild(lod);
				}
				else {
					cluster.setTransform(translate(offset));
					root.addChild(cluster);
				}
				
			}
			
			if(decal != NULL) {
				engine.editor.addNode(class_remove(decal));
				
				decal.setName(tile_name + "_decal");
				decal.setTransform(translate(offset.x,offset.y,offset.z + decal_radius * 0.5f));
				decal.setMaxVisibleDistance(decal_distance);
				decal.setMaxFadeDistance(decal_fade);
				
				root.addChild(decal);
			}
			
			if(mesh != NULL) {
				engine.editor.addNode(class_remove(mesh));
				
				mesh.setName(tile_name + "_mesh");
				mesh.setTransform(translate(offset));
				
				forloop(int i = 0; mesh.getNumSurfaces()) {
					mesh.setMaxVisibleDistance(mesh_distance,i);
					mesh.setMaxFadeDistance(mesh_fade,i);
				}
				
				root.addChild(mesh);
			}
		}
		
		/*
		 */
		void generate() {
			if(root == NULL) return;
			if(marker == NULL) return;
			
			clear();
			
			if(lod_bake) {
				mkdir(engine.getDataPath() + pathname(lod_path + "/"),1);
			}
			
			if(mesh_bake) {
				mkdir(engine.getDataPath() + pathname(mesh_path + "/"),1);
			}
			
			string mesh_name = "";
			string tile_name = "";
			Vec4 tile_offset = Vec4_zero;
			Vec4 root_offset = root.getWorldPosition();
			Mat4 marker_transforms[0];
			
			forloop(int tile = 0; marker.num_tiles) {
				tile_name = marker.tile_names[tile];
				mesh_name = marker.tile_mesh_names[tile];
				tile_offset = marker.tile_offsets[tile];
				
				Mesh tile_mesh = new Mesh();
				ObjectMeshStatic tile_node = new ObjectMeshStatic(mesh_name);
				tile_node.getMesh(tile_mesh);
				
				collect_marker_transforms(tile_mesh,tile_offset + root_offset,marker_transforms);
				generate_nodes(tile_name,tile_node,tile_offset,marker_transforms);
				
				node_delete(tile_node);
				delete tile_mesh;
				
				marker_transforms.clear();
			}
			
			nodesReload();
		}
		
		/*
		 */
		void clear() {
			if(root == NULL) return;
			
			for(int i = root.getNumChilds() - 1; i >= 0; i--) {
				node_editor_remove(root.getChild(i));
			}
			
			nodesReload();
		}
		
		/*
		 */
		void setRoot(Node node) {
			clear();
			root = node_cast(node);
		}
		
		Node getRoot() {
			return root;
		}
		
		/*
		 */
		Marker getMarker() {
			return marker;
		}
		
		/*
		 */
		void setName(string n) {
			if(root != NULL) root.setName(n);
		}
		
		string getName() {
			return (root != NULL) ? root.getName() : "";
		}
		
		/*
		 */
		void setClusterSurfaces(string s) {
			cluster_surfaces.clear();
			strsplit(s,",",cluster_surfaces);
		}
		
		string getClusterSurfaces() {
			return get_surfaces_str(cluster_surfaces);
		}
		
		/*
		 */
		int setClusterReference(string r) {
			string old_reference = cluster_reference;
			cluster_reference = r;
			
			return (old_reference != cluster_reference);
		}
		
		string getClusterReference() {
			return cluster_reference;
		}
		
		/*
		 */
		void setClusterDistance(float d) {
			cluster_distance = d;
		}
		
		float getClusterDistance() {
			return cluster_distance;
		}
		
		/*
		 */
		void setClusterShadow(float s) {
			cluster_shadow = s;
		}
		
		float getClusterShadow() {
			return cluster_shadow;
		}
		
		/*
		 */
		int setClusterBake(int value) {
			cluster_bake = value;
		}
		
		void getClusterBake() {
			return cluster_bake;
		}
		
		/*
		 */
		int setClusterProject(int value) {
			cluster_project = value;
		}
		
		void getClusterProject() {
			return cluster_project;
		}
		
		/*
		 */
		void setLodSurfaces(string s) {
			lod_surfaces.clear();
			strsplit(s,",",lod_surfaces);
		}
		
		string getLodSurfaces() {
			return get_surfaces_str(lod_surfaces);
		}
		
		/*
		 */
		void setLodPath(string p) {
			lod_path = p;
		}
		
		string getLodPath() {
			return lod_path;
		}
		
		/*
		 */
		void setLodBake(int value) {
			lod_bake = value;
		}
		
		int getLodBake() {
			return lod_bake;
		}
		
		/*
		 */
		void setDecalSurfaces(string s) {
			decal_surfaces.clear();
			strsplit(s,",",decal_surfaces);
		}
		
		string getDecalSurfaces() {
			return get_surfaces_str(decal_surfaces);
		}
		
		/*
		 */
		void setDecalMaterial(string s) {
			decal_material = s;
		}
		
		string getDecalMaterial() {
			return decal_material;
		}
		
		/*
		 */
		void setDecalRadius(float r) {
			decal_radius = r;
		}
		
		float getDecalRadius() {
			return decal_radius;
		}
		
		/*
		 */
		void setDecalDistance(float d) {
			decal_distance = d;
		}
		
		float getDecalDistance() {
			return decal_distance;
		}
		
		/*
		 */
		void setDecalFade(float f) {
			decal_fade = f;
		}
		
		float getDecalFade() {
			return decal_fade;
		}
		
		/*
		 */
		void setDecalBake(int value) {
			decal_bake = value;
		}
		
		int getDecalBake() {
			return decal_bake;
		}
		
		/*
		 */
		void setMeshSurfaces(string s) {
			mesh_surfaces.clear();
			strsplit(s,",",mesh_surfaces);
		}
		
		string getMeshSurfaces() {
			return get_surfaces_str(mesh_surfaces);
		}
		
		/*
		 */
		void setMeshMaterial(string s) {
			mesh_material = s;
		}
		
		string getMeshMaterial() {
			return mesh_material;
		}
		
		/*
		 */
		void setMeshPath(string p) {
			mesh_path = p;
		}
		
		string getMeshPath() {
			return mesh_path;
		}
		
		/*
		 */
		void setMeshDistance(float d) {
			mesh_distance = d;
		}
		
		float getMeshDistance() {
			return mesh_distance;
		}
		
		/*
		 */
		void setMeshFade(float f) {
			mesh_fade = f;
		}
		
		float getMeshFade() {
			return mesh_fade;
		}
		
		/*
		 */
		void setMeshBake(int value) {
			mesh_bake = value;
		}
		
		int getMeshBake() {
			return mesh_bake;
		}
	};
	
	/**************************************************************************\
	*
	* Surfaces dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Surfaces {
		
		/*
		 */
		Dialog dialog;
		ListBox surfaces_lb;
		
		string selection[0];
		
		/*
		 */
		void update_surfaces(string surfaces[]) {
			surfaces_lb.clear();
			
			forloop(int i = 0; surfaces.size()) {
				surfaces_lb.addItem(surfaces[i]);
			}
		}
		
		/*
		 */
		void surfaces_changed() {
			selection.clear();
			
			forloop(int i = 0; surfaces_lb.getNumSelectedItems()) {
				int item_index = surfaces_lb.getSelectedItem(i);
				selection.append(surfaces_lb.getItemText(item_index));
			}
		}
		
		/*
		 */
		void surfaces_double_clicked() {
			selection.clear();
			
			if(surfaces_lb.getNumSelectedItems() != 0) {
				int item_index = surfaces_lb.getSelectedItem(0);
				selection.append(surfaces_lb.getItemText(item_index));
			}
			
			Button ok_button = dialog.getOkButton();
			ok_button.runCallback(CLICKED);
		}
		
		/*
		 */
		int run(string title,string surfaces[],string &selected_surfaces) {
			dialog.setText(title);
			
			selection.clear();
			surfaces.sort();
			update_surfaces(surfaces);
			
			addChild(dialog,GUI_ALIGN_OVERLAP | GUI_ALIGN_CENTER);
			dialog.setPermanentFocus();
			
			while(dialog.isDone() == 0) wait 1;
			
			removeChild(dialog);
			dialog.removeFocus();
			
			if(dialog.isOkClicked()) {
				selected_surfaces = get_surfaces_str(selection);
				
				return 1;
			}
			
			return 0;
		}
		
	}
	
	/*
	 */
	int dialogSurfaces(string title,string surfaces[],string &selected_surfaces) {
		return Surfaces::run(title,surfaces,selected_surfaces);
	}
	
	/**************************************************************************\
	*
	* Marker create dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace MarkerCreate {
		
		/*
		 */
		Dialog dialog;
		EditLine marker_name_el;
		EditLine marker_path_el;
		
		/*
		 */
		void marker_name_changed() {
			Button ok_button = dialog.getOkButton();
			string text = trim(marker_name_el.getText());
			
			ok_button.setEnabled(strlen(text) > 0);
		}
		
		void marker_path_clicked() {
			string name;
			if(!dialogFileNode(TR("Select City Node"),name)) return;
			
			marker_path_el.setText(name);
		}
		
		/*
		 */
		int run(string &name,string &path) {
			marker_name_el.setCallbackEnabled(CHANGED,0);
			marker_name_el.setText("");
			marker_name_el.setCallbackEnabled(CHANGED,1);
			
			marker_name_changed();
			
			marker_path_el.setText("");
			
			addChild(dialog,GUI_ALIGN_OVERLAP | GUI_ALIGN_CENTER);
			dialog.setPermanentFocus();
			
			while(dialog.isDone() == 0) wait 1;
			
			removeChild(dialog);
			dialog.removeFocus();
			
			if(dialog.isOkClicked()) {
				name = marker_name_el.getText();
				path = marker_path_el.getText();
				
				return 1;
			}
			
			return 0;
		}
		
	}
	
	/*
	 */
	int dialogMarkerCreate(string &name,string &path) {
		return MarkerCreate::run(name,path);
	}
	
	/**************************************************************************\
	*
	* Layer create dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace LayerCreate {
		
		/*
		 */
		Dialog dialog;
		EditLine layer_name_el;
		
		Marker marker;
		
		/*
		 */
		void layer_name_changed() {
			Button ok_button = dialog.getOkButton();
			string text = trim(layer_name_el.getText());
			
			ok_button.setEnabled(strlen(text) > 0);
		}
		
		/*
		 */
		int run(Marker m,string &name) {
			marker = m;
			
			layer_name_el.setCallbackEnabled(CHANGED,0);
			layer_name_el.setText("");
			layer_name_el.setCallbackEnabled(CHANGED,1);
			
			layer_name_changed();
			
			addChild(dialog,GUI_ALIGN_OVERLAP | GUI_ALIGN_CENTER);
			dialog.setPermanentFocus();
			
			while(dialog.isDone() == 0) wait 1;
			
			removeChild(dialog);
			dialog.removeFocus();
			
			if(dialog.isOkClicked()) {
				name = layer_name_el.getText();
				
				return 1;
			}
			
			return 0;
		}
		
	}
	
	/*
	 */
	int dialogLayerCreate(Marker marker,string &name) {
		return LayerCreate::run(marker,name);
	}
	
	/**************************************************************************\
	*
	* Bakery
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Bakery {
		
		/*
		 */
		void copy_surfaces(Object src,Object dst) {
			forloop(int i = 0; dst.getNumSurfaces()) {
				string name = dst.getSurfaceName(i);
				
				int src_index = src.findSurface(name);
				if(src_index == -1) continue;
				
				dst.setEnabled(src.isEnabled(src_index),i);
				
				string material = src.getMaterialName(src_index);
				string property = src.getPropertyName(src_index);
				
				if(material != "") dst.setMaterial(material,i);
				if(property != "") dst.setProperty(property,i);
				
				dst.setMaxFadeDistance(src.getMaxFadeDistance(src_index),i);
				dst.setMaxParent(src.getMaxParent(src_index),i);
				dst.setMaxVisibleDistance(src.getMaxVisibleDistance(src_index),i);
				
				dst.setMinFadeDistance(src.getMinFadeDistance(src_index),i);
				dst.setMinParent(src.getMinParent(src_index),i);
				dst.setMinVisibleDistance(src.getMinVisibleDistance(src_index),i);
				
				dst.setCastShadow(src.getCastShadow(src_index),i);
				dst.setCastWorldShadow(src.getCastWorldShadow(src_index),i);
				dst.setReceiveShadow(src.getReceiveShadow(src_index),i);
				dst.setReceiveWorldShadow(src.getReceiveWorldShadow(src_index),i);
				
				dst.setCollisionMask(src.getCollisionMask(src_index),i);
				dst.setDecalMask(src.getDecalMask(src_index),i);
				dst.setIntersectionMask(src.getIntersectionMask(src_index),i);
				dst.setLightMask(src.getLightMask(src_index),i);
				dst.setViewportMask(src.getViewportMask(src_index),i);
				
				dst.setCollision(src.getCollision(src_index),i);
				dst.setDecal(src.getDecal(src_index),i);
				dst.setIntersection(src.getIntersection(src_index),i);
			}
		}
		
		/*
		 */
		Mesh bake_mesh(ObjectMeshStatic mesh,string surfaces[]) {
			if(mesh == NULL) return NULL;
			if(surfaces.size() == 0) return NULL;
			
			Mesh source = new Mesh();
			mesh.getMesh(source);
			
			Mesh result = new Mesh();
			
			forloop(int i = 0; source.getNumSurfaces()) {
				string name = source.getSurfaceName(i);
				if(surfaces.find(name) == -1) continue;
				
				result.addMeshSurface(name,source,i);
			}
			delete source;
			
			if(result.getNumSurfaces() == 0) {
				delete result;
				return NULL;
			}
			
			result.createBounds();
			return result;
		}
		
		/*
		 */
		Node toDecalDeferredMesh(ObjectMeshStatic mesh,float radius,string material,string surfaces[]) {
			Mesh baked = bake_mesh(mesh,surfaces);
			if(baked == NULL) return NULL;
			
			DecalDeferredMesh decal = new DecalDeferredMesh(baked,radius,material);
			delete baked;
			
			return decal;
		}
		
		/*
		 */
		Node toObjectMeshStatic(ObjectMeshStatic mesh,string path,string material,string surfaces[]) {
			Mesh baked = bake_mesh(mesh,surfaces);
			if(baked == NULL) return NULL;
			
			if(baked.save(path) == 0) {
				delete baked;
				return NULL;
			}
			
			delete baked;
			ObjectMeshStatic result = new ObjectMeshStatic(path);
			
			copy_surfaces(mesh,result);
			
			result.setMaterial(material,"*");
			result.setProperty("surface_base","*");
			result.updateSurfaceBounds();
			
			return result;
		}
		
		/*
		 */
		Node toMeshFromCluster(ObjectMeshCluster cluster,string path,string surfaces[]) {
			if(cluster == NULL) return NULL;
			if(surfaces.size() == 0) return NULL;
			
			Mesh reference = class_manage(new Mesh(cluster.getMeshName()));
			Mesh mesh = class_manage(new Mesh());
			
			int num_meshes = cluster.getNumMeshes();
			if(num_meshes == 0) return NULL;
			
			forloop(int surface = 0; reference.getNumSurfaces()) {
				string name = reference.getSurfaceName(surface);
				if(surfaces.find(name) == -1) continue;
				
				int current_surface = mesh.addMeshSurface(name,reference,surface);
				mesh.setSurfaceTransform(cluster.getMeshTransform(0),current_surface);
				
				forloop(int mesh_index = 1; num_meshes) {
					Mat4 mesh_transform = cluster.getMeshTransform(mesh_index);
					Mesh temp = new Mesh(reference);
					
					temp.setSurfaceTransform(mesh_transform,surface);
					mesh.addMeshSurface(current_surface,temp,surface);
					
					delete temp;
				}
			}
			delete reference;
			
			mesh.createBounds();
			if(mesh.save(path) == 0) {
				delete mesh;
				return NULL;
			}
			
			delete mesh;
			ObjectMeshStatic result = new ObjectMeshStatic(path);
			
			copy_surfaces(cluster,result);
			
			forloop(int i = 0; result.getNumSurfaces()) {
				result.setEnabled(1,i);
			}
			
			result.updateSurfaceBounds();
			
			return result;
		}
		
		/*
		 */
		Node toMeshCluster(ObjectMeshStatic mesh,Mat4 transforms[]) {
			if(mesh == NULL) return NULL;
			if(transforms.size() == 0) return NULL;
			
			ObjectMeshCluster cluster = new ObjectMeshCluster(mesh.getMeshName());
			
			copy_surfaces(mesh,cluster);
			
			cluster.createMeshes(transforms);
			return cluster;
		}
	}
	
	/**************************************************************************\
	*
	* UI
	*
	\**************************************************************************/
	
	/*
	 */
	namespace UI {
		
		/*
		 */
		TreeBox tree_tb;
		
		GroupBox marker_gb;
		GroupBox layer_gb;
		
		GridBox layer_cluster_gb;
		GridBox layer_lod_gb;
		GridBox layer_decal_gb;
		GridBox layer_mesh_gb;
		
		CheckBox layer_cluster_project_cb;
		CheckBox layer_cluster_bake_cb;
		CheckBox layer_lod_bake_cb;
		CheckBox layer_decal_bake_cb;
		CheckBox layer_mesh_bake_cb;
		
		EditLine marker_name_el;
		EditLine marker_path_el;
		EditLine layer_name_el;
		EditLine layer_cluster_surfaces_el;
		EditLine layer_cluster_path_el;
		EditLine layer_cluster_distance_el;
		EditLine layer_cluster_shadow_el;
		EditLine layer_lod_surfaces_el;
		EditLine layer_lod_path_el;
		EditLine layer_decal_surfaces_el;
		EditLine layer_decal_material_el;
		EditLine layer_decal_distance_el;
		EditLine layer_decal_fade_el;
		EditLine layer_decal_radius_el;
		EditLine layer_mesh_surfaces_el;
		EditLine layer_mesh_material_el;
		EditLine layer_mesh_distance_el;
		EditLine layer_mesh_fade_el;
		EditLine layer_mesh_path_el;
		
		Button clear_all_b;
		Button generate_all_b;
		
		Button marker_remove_b;
		
		Button layer_add_b;
		Button layer_remove_b;
		Button layer_clear_b;
		Button layer_generate_b;
		
		string marker_name;
		string marker_path;
		
		string layer_name;
		
		string layer_cluster_path;
		string layer_cluster_surfaces;
		string layer_cluster_distance;
		string layer_cluster_shadow;
		int layer_cluster_bake;
		int layer_cluster_project;
		
		string layer_lod_surfaces;
		string layer_lod_path;
		int layer_lod_bake;
		
		string layer_decal_surfaces;
		string layer_decal_material;
		float layer_decal_distance;
		float layer_decal_fade;
		float layer_decal_radius;
		int layer_decal_bake;
		
		string layer_mesh_surfaces;
		string layer_mesh_material;
		string layer_mesh_distance;
		string layer_mesh_fade;
		string layer_mesh_path;
		int layer_mesh_bake;
		
		int lock_ui;
		int last_id;
		
		Marker markers[];
		Layer layers[];
		
		/**********************************************************************\
		*
		* Save/Reload
		*
		\**********************************************************************/
		
		/*
		 */
		void save() {
			markers.call("saveData");
			layers.call("saveData");
		}
		
		/*
		 */
		void reload() {
			if(lock_ui) return;
			
			marker_name = "";
			marker_path = "";
			
			last_id = -1;
			layer_name = "";
			
			layer_cluster_path = "";
			layer_cluster_surfaces = "";
			layer_cluster_distance = INFINITY;
			layer_cluster_shadow = 0.0f;
			layer_cluster_bake = 0;
			layer_cluster_project = 0;
			
			layer_lod_surfaces = "";
			layer_lod_path = "";
			layer_lod_bake = 0;
			
			layer_decal_surfaces = "";
			layer_decal_material = "decal_deferred_base";
			layer_decal_distance = INFINITY;
			layer_decal_fade = 0.0f;
			layer_decal_radius = 0.0f;
			layer_decal_bake = 0;
			
			layer_mesh_surfaces = "";
			layer_mesh_material = "mesh_base";
			layer_mesh_distance = INFINITY;
			layer_mesh_fade = 0.0f;
			layer_mesh_path = "";
			layer_mesh_bake = 0;
			
			markers.delete();
			layers.delete();
			
			int selection = tree_tb.getCurrentItem();
			tree_tb.clear();
			
			find_data();
			
			tree_tb.setCurrentItem(selection);
			
			update();
		}
		
		/*
		 */
		void find_data() {
			Xml xml = new Xml();
			string data = "";
			
			forloop(int i = 0; engine.editor.getNumNodes()) {
				xml.clear();
				
				Node node = engine.editor.getNode(i);
				data = node.getData();
				
				if(!(data is string)) continue;
				if(strlen(data) == 0) continue;
				
				if(!xml.parse(data)) continue;
				if(xml.getName() != "marker") continue;
				
				Marker marker = new Marker(xml);
				marker.setRoot(node);
				if(!marker.load()) {
					log.error(TR("Failed to load \"%s\" city node\n"),marker.getPath());
				}
				
				int marker_id = tree_tb.addItem(marker.getName());
				markers.append(marker_id,marker);
				marker_ui_update(marker_id);
				
				forloop(int j = 0; node.getNumChilds()) {
					xml.clear();
					
					Node child = node.getChild(j);
					data = child.getData();
					
					if(!(data is string)) continue;
					if(strlen(data) == 0) continue;
					
					if(!xml.parse(data)) continue;
					if(xml.getName() != "layer") continue;
					
					Layer layer = new Layer(xml,marker);
					layer.setRoot(child);
					
					int need_load = layer.getClusterBake() || layer.getLodBake();
					if(need_load && !layer.load()) {
						log.error(TR("Failed to load \"%s\" city layer reference node\n"),layer.getClusterReference());
					}
					
					int layer_id = tree_tb.addItem(layer.getName());
					tree_tb.addItemChild(marker_id,layer_id);
					
					layers.append(layer_id,layer);
					layer_ui_update(layer_id);
				}
			}
			
			delete xml;
		}
		
		/**********************************************************************\
		*
		* Close
		*
		\**********************************************************************/
		
		/*
		 */
		void window_close_clicked() {
			windowHide(window,meta.name); //"city_import_plugin");
		}
		
		/**********************************************************************\
		*
		* Update
		*
		\**********************************************************************/
		
		/*
		 */
		void update() {
			int id = tree_tb.getCurrentItem();
			
			last_id = id;
			common_ui_update();
			marker_ui_update(id);
			layer_ui_update(id);
		}
		
		/*
		 */
		void common_ui_update() {
			marker_gb.setEnabled(0);
			layer_gb.setEnabled(0);
			
			marker_remove_b.setEnabled(1);
			layer_add_b.setEnabled(0);
			layer_remove_b.setEnabled(0);
			
			int has_layers = (layers.size() > 0);
			clear_all_b.setEnabled(has_layers);
			generate_all_b.setEnabled(has_layers);
		}
		
		/*
		 */
		void marker_ui_update(int id) {
			Marker marker = markers.check(id,NULL);
			if(marker == NULL) return;
			
			marker_name = marker.getName();
			marker_path = marker.getPath();
			
			marker_gb.setEnabled(1);
			layer_add_b.setEnabled(1);
			
			if(marker.isValid()) {
				tree_tb.setItemColor(id,vec4_one);
			}
			else {
				tree_tb.setItemColor(id,vec4(1.0f,0.0f,0.0f,1.0f));
			}
			
			marker_name_el.setText(marker_name);
			marker_path_el.setText(marker_path);
			
			tree_tb.setItemText(id,marker_name);
		}
		
		/*
		 */
		void layer_ui_update(int id) {
			Layer layer = layers.check(id,NULL);
			if(layer == NULL) return;
			
			layer_name = layer.getName();
			
			layer_cluster_path = layer.getClusterReference();
			layer_cluster_surfaces = layer.getClusterSurfaces();
			layer_cluster_distance = layer.getClusterDistance();
			layer_cluster_shadow = layer.getClusterShadow();
			layer_cluster_bake = layer.getClusterBake();
			layer_cluster_project = layer.getClusterProject();
			
			layer_lod_surfaces = layer.getLodSurfaces();
			layer_lod_path = layer.getLodPath();
			layer_lod_bake = layer.getLodBake();
			
			layer_decal_surfaces = layer.getDecalSurfaces();
			layer_decal_material = layer.getDecalMaterial();
			layer_decal_distance = layer.getDecalDistance();
			layer_decal_fade = layer.getDecalFade();
			layer_decal_radius = layer.getDecalRadius();
			layer_decal_bake = layer.getDecalBake();
			
			layer_mesh_surfaces = layer.getMeshSurfaces();
			layer_mesh_material = layer.getMeshMaterial();
			layer_mesh_distance = layer.getMeshDistance();
			layer_mesh_fade = layer.getMeshFade();
			layer_mesh_path = layer.getMeshPath();
			layer_mesh_bake = layer.getMeshBake();
			
			marker_remove_b.setEnabled(0);
			
			layer_gb.setEnabled(1);
			layer_add_b.setEnabled(0);
			layer_remove_b.setEnabled(1);
			
			layer_cluster_project_cb.setCallbackEnabled(CLICKED,0);
			layer_cluster_project_cb.setChecked(layer_cluster_project);
			layer_cluster_project_cb.setCallbackEnabled(CLICKED,1);
			
			layer_cluster_bake_cb.setCallbackEnabled(CLICKED,0);
			layer_cluster_bake_cb.setChecked(layer_cluster_bake);
			layer_cluster_gb.setEnabled(layer_cluster_bake);
			layer_cluster_bake_cb.setCallbackEnabled(CLICKED,1);
			
			layer_lod_bake_cb.setCallbackEnabled(CLICKED,0);
			layer_lod_bake_cb.setEnabled(layer_cluster_bake);
			layer_lod_bake_cb.setChecked(layer_lod_bake);
			layer_lod_gb.setEnabled(layer_lod_bake && layer_cluster_bake);
			layer_lod_bake_cb.setCallbackEnabled(CLICKED,1);
			
			layer_decal_bake_cb.setCallbackEnabled(CLICKED,0);
			layer_decal_bake_cb.setChecked(layer_decal_bake);
			layer_decal_gb.setEnabled(layer_decal_bake);
			layer_decal_bake_cb.setCallbackEnabled(CLICKED,1);
			
			layer_mesh_bake_cb.setCallbackEnabled(CLICKED,0);
			layer_mesh_bake_cb.setChecked(layer_mesh_bake);
			layer_mesh_gb.setEnabled(layer_mesh_bake);
			layer_mesh_bake_cb.setCallbackEnabled(CLICKED,1);
			
			layer_clear_b.setEnabled(1);
			
			if(layer.isValid()) {
				layer_generate_b.setEnabled(1);
				tree_tb.setItemColor(id,vec4_one);
			}
			else {
				layer_generate_b.setEnabled(0);
				tree_tb.setItemColor(id,vec4(1.0f,0.0f,0.0f,1.0f));
			}
			
			layer_name_el.setText(layer_name);
			
			layer_cluster_surfaces_el.setText(layer_cluster_surfaces);
			layer_cluster_path_el.setText(layer_cluster_path);
			layer_cluster_distance_el.setText(editorFormat(layer_cluster_distance));
			layer_cluster_shadow_el.setText(editorFormat(layer_cluster_shadow));
			
			layer_lod_surfaces_el.setText(layer_lod_surfaces);
			layer_lod_path_el.setText(layer_lod_path);
			
			layer_decal_surfaces_el.setText(layer_decal_surfaces);
			layer_decal_material_el.setText(layer_decal_material);
			layer_decal_distance_el.setText(editorFormat(layer_decal_distance));
			layer_decal_fade_el.setText(editorFormat(layer_decal_fade));
			layer_decal_radius_el.setText(editorFormat(layer_decal_radius));
			
			layer_mesh_surfaces_el.setText(layer_mesh_surfaces);
			layer_mesh_material_el.setText(layer_mesh_material);
			layer_mesh_distance_el.setText(editorFormat(layer_mesh_distance));
			layer_mesh_fade_el.setText(editorFormat(layer_mesh_fade));
			layer_mesh_path_el.setText(layer_mesh_path);
			
			tree_tb.setItemText(id,layer_name);
		}
		
		/**********************************************************************\
		*
		* Add/Remove
		*
		\**********************************************************************/
		
		/*
		 */
		void marker_add_clicked() {
			if(!dialogMarkerCreate(marker_name,marker_path)) return;
			
			Marker marker = new Marker(marker_name,marker_path);
			if(!marker.load()) {
				dialogMessageOk(TR("Error"),format(TR("Failed to load \"%s\" city node"),marker.getPath()));
			}
			
			int id = tree_tb.addItem(marker.getName());
			markers.append(id,marker);
		}
		
		void marker_remove_clicked() {
			int id = tree_tb.getCurrentItem();
			
			Marker marker = markers.check(id,NULL);
			if(marker == NULL) return;
			
			int num_children = tree_tb.getNumItemChilds(id);
			if(num_children) {
				if(!dialogMessageYesNo(TR("Remove City"),TR("All city layers will be deleted too. Are you sure?"))) return;
				
				for(int i = num_children - 1; i >= 0; i--) {
					int child_id = tree_tb.getItemChild(id,i);
					
					layer_delete(child_id);
				}
			}
			
			tree_tb.removeItem(id);
			
			markers.remove(id);
			marker.deleteNodes();
			delete marker;
		}
		
		/*
		 */
		void layer_add_clicked() {
			int marker_id = tree_tb.getCurrentItem();
			
			Marker marker = markers.check(marker_id,NULL);
			if(marker == NULL) return;
			if(!dialogLayerCreate(marker,layer_name)) return;
			
			Layer layer = new Layer(layer_name,marker);
			int need_load = layer.getClusterBake() || layer.getLodBake();
			
			if(need_load && !layer.load()) {
				dialogMessageOk(TR("Error"),format(TR("Failed to load \"%s\" city layer reference node"),layer.getClusterReference()));
			}
			
			int layer_id = tree_tb.addItem(layer_name);
			tree_tb.addItemChild(marker_id,layer_id);
			
			layers.append(layer_id,layer);
		}
		
		void layer_remove_clicked() {
			layer_delete(tree_tb.getCurrentItem());
		}
		
		/*
		 */
		void layer_delete(int id) {
			Layer layer = layers.check(id,NULL);
			if(layer == NULL) return;
			
			layers.remove(id);
			layer.deleteNodes();
			delete layer;
			
			tree_tb.removeItem(id);
		}
		
		/**********************************************************************\
		*
		* Clear/Generate
		*
		\**********************************************************************/
		
		/*
		 */
		void clear_all_clicked() {
			foreach(Layer layer; layers) {
				layer.clear();
			}
		}
		
		void generate_all_clicked() {
			lock_ui = 1;
			
			dialogProgressBegin(TR("Generating Nodes..."));
			dialogProgressUpdate(1);
			
			foreach(Marker marker; markers) {
				if(!marker.load()) {
					log.error(TR("Failed to load \"%s\" city node\n"),marker.getPath());
				}
			}
			
			int processed_layers = 0;
			foreach(Layer layer; layers) {
				int need_load = layer.getClusterBake() || layer.getLodBake();
				if(need_load && !layer.load()) {
					log.error(TR("Failed to load \"%s\" city layer reference node\n"),layer.getClusterReference());
				}
				
				if(layer.isValid()) layer.generate();
				
				processed_layers++;
				if(dialogProgressUpdate((processed_layers * 100) / layers.size()) == 0) {
					dialogProgressEnd();
					
					clear_all_clicked();
					lock_ui = 0;
					
					return;
				}
			}
			
			dialogProgressEnd();
			lock_ui = 0;
			
			reload();
		}
		
		/**********************************************************************\
		*
		* Marker callbacks
		*
		\**********************************************************************/
		
		/*
		 */
		void marker_update(int id) {
			Marker marker = markers.check(id,NULL);
			if(marker == NULL) return;
			
			marker.setName(marker_name);
			
			int reload = marker.setPath(marker_path);
			if(reload && !marker.load()) {
				dialogMessageOk(TR("Error"),format(TR("Failed to load \"%s\" city node"),marker.getPath()));
			}
			
			marker.saveData();
			marker_ui_update(id);
		}
		
		/*
		 */
		void marker_name_pressed() {
			string new_name = trim(marker_name_el.getText());
			if(strlen(new_name) > 0) marker_name = new_name;
			
			marker_update(last_id);
			nodesUpdate(1);
		}
		
		/*
		 */
		void marker_path_pressed() {
			marker_path = marker_path_el.getText();
			marker_update(last_id);
		}
		
		void marker_path_clicked() {
			if(!dialogFileNode(TR("Select City Node"),marker_path)) return;
			marker_update(last_id);
		}
		
		/**********************************************************************\
		*
		* Layer common callbacks
		*
		\**********************************************************************/
		
		/*
		 */
		void layer_update(int id) {
			Layer layer = layers.check(id,NULL);
			if(layer == NULL) return;
			
			layer.setName(layer_name);
			
			layer.setLodBake(layer_lod_bake);
			layer.setLodPath(layer_lod_path);
			layer.setLodSurfaces(layer_lod_surfaces);
			
			layer.setDecalBake(layer_decal_bake);
			layer.setDecalSurfaces(layer_decal_surfaces);
			layer.setDecalMaterial(layer_decal_material);
			layer.setDecalDistance(layer_decal_distance);
			layer.setDecalFade(layer_decal_fade);
			layer.setDecalRadius(layer_decal_radius);
			
			layer.setMeshBake(layer_mesh_bake);
			layer.setMeshSurfaces(layer_mesh_surfaces);
			layer.setMeshMaterial(layer_mesh_material);
			layer.setMeshDistance(layer_mesh_distance);
			layer.setMeshFade(layer_mesh_fade);
			layer.setMeshPath(layer_mesh_path);
			
			layer.setClusterBake(layer_cluster_bake);
			layer.setClusterProject(layer_cluster_project);
			layer.setClusterSurfaces(layer_cluster_surfaces);
			layer.setClusterDistance(layer_cluster_distance);
			layer.setClusterShadow(layer_cluster_shadow);
			
			int need_reload = layer.setClusterReference(layer_cluster_path);
			int need_load = layer.getClusterBake() || layer.getLodBake();
			
			if(need_load && need_reload && !layer.load()) {
				dialogMessageOk(TR("Error"),format(TR("Failed to load \"%s\" city layer reference node"),layer.getClusterReference()));
			}
			
			layer.saveData();
			layer_ui_update(id);
		}
		
		/*
		 */
		void layer_clear_clicked() {
			Layer layer = layers.check(last_id,NULL);
			if(layer == NULL) return;
			
			layer.clear();
		}
		
		void layer_generate_clicked() {
			Layer layer = layers.check(last_id,NULL);
			if(layer == NULL) return;
			
			Marker marker = layer.getMarker();
			if(marker == NULL) return;
			
			if(!marker.load()) {
				log.error(TR("Failed to load \"%s\" city node\n"),marker.getPath());
				return;
			}
			
			if(marker.isValid() == 0) return;
			
			int need_load = layer.getClusterBake() || layer.getLodBake();
			if(need_load && !layer.load()) {
				log.error(TR("Failed to load \"%s\" city layer reference node\n"),layer.getClusterReference());
			}
			
			if(layer.isValid()) layer.generate();
		}
		
		/*
		 */
		void layer_name_pressed() {
			string new_name = trim(layer_name_el.getText());
			if(strlen(new_name) > 0) layer_name = new_name;
			
			layer_update(last_id);
			nodesUpdate(1);
		}
		
		/**********************************************************************\
		*
		* Layer cluster callbacks
		*
		\**********************************************************************/
		
		/*
		 */
		void layer_cluster_project_checked() {
			layer_cluster_project = layer_cluster_project_cb.isChecked();
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_cluster_bake_checked() {
			layer_cluster_bake = layer_cluster_bake_cb.isChecked();
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_cluster_path_pressed() {
			layer_cluster_path = layer_cluster_path_el.getText();
			layer_update(last_id);
		}
		
		void layer_cluster_path_clicked() {
			if(!dialogFileNode(TR("Select Reference Node"),layer_cluster_path)) return;
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_cluster_surfaces_pressed() {
			layer_cluster_surfaces = layer_cluster_surfaces_el.getText();
			layer_update(last_id);
		}
		
		void layer_cluster_surfaces_clicked() {
			Layer layer = layers.check(last_id,NULL);
			if(layer == NULL) return;
			
			Marker marker = layer.getMarker();
			if(marker == NULL) return;
			
			if(!dialogSurfaces(TR("Select City Surfaces"),marker.common_surfaces,layer_cluster_surfaces)) return;
			
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_cluster_distance_pressed() {
			layer_cluster_distance = float(layer_cluster_distance_el.getText());
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_cluster_shadow_pressed() {
			layer_cluster_shadow = float(layer_cluster_shadow_el.getText());
			layer_update(last_id);
		}
		
		/**********************************************************************\
		*
		* Layer LOD callbacks
		*
		\**********************************************************************/
		
		/*
		 */
		void layer_lod_bake_checked() {
			layer_lod_bake = layer_lod_bake_cb.isChecked();
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_lod_surfaces_pressed() {
			layer_lod_surfaces = layer_lod_surfaces_el.getText();
			layer_update(last_id);
		}
		
		void layer_lod_surfaces_clicked() {
			Layer layer = layers.check(last_id,NULL);
			if(layer == NULL) return;
			
			if(!dialogSurfaces(TR("Select City Layer Surfaces"),layer.reference_surfaces,layer_lod_surfaces)) return;
			
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_lod_path_pressed() {
			layer_lod_path = layer_lod_path_el.getText();
			layer_update(last_id);
		}
		
		/**********************************************************************\
		*
		* Layer decal callbacks
		*
		\**********************************************************************/
		
		/*
		 */
		void layer_decal_bake_checked() {
			layer_decal_bake = layer_decal_bake_cb.isChecked();
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_decal_surfaces_pressed() {
			layer_decal_surfaces = layer_decal_surfaces_el.getText();
			layer_update(last_id);
		}
		
		void layer_decal_surfaces_clicked() {
			Layer layer = layers.check(last_id,NULL);
			if(layer == NULL) return;
			
			Marker marker = layer.getMarker();
			if(marker == NULL) return;
			
			if(!dialogSurfaces(TR("Select City Surfaces"),marker.common_surfaces,layer_decal_surfaces)) return;
			
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_decal_material_pressed() {
			layer_decal_material = layer_decal_material_el.getText();
			layer_update(last_id);
		}
		
		void layer_decal_material_clicked() {
			if(!dialogMaterial(TR("Select Material"),layer_decal_material)) return;
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_decal_radius_pressed() {
			layer_decal_radius = float(layer_decal_radius_el.getText());
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_decal_distance_pressed() {
			layer_decal_distance = float(layer_decal_distance_el.getText());
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_decal_fade_pressed() {
			layer_decal_fade = float(layer_decal_fade_el.getText());
			layer_update(last_id);
		}
		
		/**********************************************************************\
		*
		* Layer mesh callbacks
		*
		\**********************************************************************/
		
		/*
		 */
		void layer_mesh_bake_checked() {
			layer_mesh_bake = layer_mesh_bake_cb.isChecked();
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_mesh_surfaces_pressed() {
			layer_mesh_surfaces = layer_mesh_surfaces_el.getText();
			layer_update(last_id);
		}
		
		void layer_mesh_surfaces_clicked() {
			Layer layer = layers.check(last_id,NULL);
			if(layer == NULL) return;
			
			Marker marker = layer.getMarker();
			if(marker == NULL) return;
			
			if(!dialogSurfaces(TR("Select City Surfaces"),marker.common_surfaces,layer_mesh_surfaces)) return;
			
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_mesh_material_pressed() {
			layer_mesh_material = layer_mesh_material_el.getText();
			layer_update(last_id);
		}
		
		void layer_mesh_material_clicked() {
			if(!dialogMaterial(TR("Select Material"),layer_mesh_material)) return;
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_mesh_path_pressed() {
			layer_mesh_path = layer_mesh_path_el.getText();
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_mesh_distance_pressed() {
			layer_mesh_distance = float(layer_mesh_distance_el.getText());
			layer_update(last_id);
		}
		
		/*
		 */
		void layer_mesh_fade_pressed() {
			layer_mesh_fade = float(layer_mesh_fade_el.getText());
			layer_update(last_id);
		}
	}
}
