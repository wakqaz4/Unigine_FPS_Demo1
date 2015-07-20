/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    node_export.cpp
 * Desc:    Node export plugin
 * Version: 1.03
 * Author:  Andrey Viktorov <unclebob@unigine.com>
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

#include <core/unigine.h>

/******************************************************************************\
*
* Plugin
*
\******************************************************************************/

/*
 */
string getName() {
	return "NodeExport";
}

/*
 */
void init(PluginMeta m) {
	NodeExport::init(m);
}

void shutdown() {
	NodeExport::shutdown();
}

void update(int need_reload) {
	NodeExport::update(need_reload);
}

void save() {
	NodeExport::save();
}

void show() {
	NodeExport::show();
}
/******************************************************************************\
*
* NodeExport
*
\******************************************************************************/

/*
 */
namespace NodeExport {
	
	/*
	 */
	using Unigine::Widgets;
	
	/*
	 */
	UserInterface interface;
	Window window;
	
	enum {
		OFFSET_X = 4,
		OFFSET_Y = 172,
	};
	
	PluginMeta meta;
	
	/*
	 */
	void init(PluginMeta m) {
		
		meta = m;
		
		interface = new UserInterface(replace(meta.source,".cpp",".ui"),getName() + "::");
		hotkeysAddUiToolTips(interface);
		
		window.arrange();
		
		UI::load();
		
		pluginsAddWindow(window,meta.title,meta.name);
		
		#ifdef HAS_INTERFACE
			window.getInterface().setWidth(window.getWidth());
			window.getInterface().setHeight(window.getHeight());
		#endif
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
	
	/*
	 */
	void update(int need_reload) {
		
	}
	
	/*
	 */
	void save() {
		
	}
	
	void show() {
		pluginsShowWindow(window,meta.name);
	}
	/**************************************************************************\
	*
	* Node dependencies
	*
	\**************************************************************************/
	
	/*
	 */
	class NodeDependencies {
		
		//
		string nodes[];
		string meshes[];
		
		string properties[0];
		string property_libs[];
		
		string materials[0];
		string material_libs[];
		string textures[];
		
		NodeDependencies() {
			nodes.clear();
			meshes.clear();
			
			properties.clear();
			property_libs.clear();
			
			materials.clear();
			material_libs.clear();
			textures.clear();
		}
		
		~NodeDependencies() {
			nodes.clear();
			meshes.clear();
			
			properties.clear();
			property_libs.clear();
			
			materials.clear();
			material_libs.clear();
			textures.clear();
		}
	};
	
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
		EditLine project_from_data_el;
		
		EditLine project_to_data_el;
		EditLine project_to_name_el;
		
		CheckBox remove_part_cb;
		
		CheckBox custom_paths_cb;
		GroupBox custom_paths_gb;
		
		EditLine nodes_el;
		EditLine meshes_el;
		EditLine materials_el;
		EditLine properties_el;
		EditLine textures_el;
		EditLine terrain_el;
		
		string temp[];
		
		/*
		 */
		void save() {
			configSet("plugin_node_export_project_from",project_from_data_el.getText());
			configSet("plugin_node_export_project_data",project_to_data_el.getText());
			configSet("plugin_node_export_project_name",project_to_name_el.getText());
			
			configSet("plugin_node_export_nodes",nodes_el.getText());
			configSet("plugin_node_export_meshes",meshes_el.getText());
			configSet("plugin_node_export_materials",materials_el.getText());
			configSet("plugin_node_export_properties",properties_el.getText());
			configSet("plugin_node_export_textures",textures_el.getText());
			configSet("plugin_node_export_terrain",terrain_el.getText());
			
			configSet("plugin_node_export_custom_paths",custom_paths_cb.isChecked());
			
			configSet("plugin_node_export_remove_part",remove_part_cb.isChecked());
		}
		
		void load() {
			
			project_to_name_el.setCallbackEnabled(CHANGED,0);
			custom_paths_cb.setCallbackEnabled(CHANGED,0);
			
			project_from_data_el.setText(configGet("plugin_node_export_project_from",""));
			project_to_data_el.setText(configGet("plugin_node_export_project_data",""));
			project_to_name_el.setText(configGet("plugin_node_export_project_name",""));
			
			nodes_el.setText(configGet("plugin_node_export_nodes",""));
			meshes_el.setText(configGet("plugin_node_export_meshes",""));
			materials_el.setText(configGet("plugin_node_export_materials",""));
			properties_el.setText(configGet("plugin_node_export_properties",""));
			textures_el.setText(configGet("plugin_node_export_textures",""));
			terrain_el.setText(configGet("plugin_node_export_terrain",""));
			
			custom_paths_cb.setChecked(configGet("plugin_node_export_custom_paths",0));
			remove_part_cb.setChecked(configGet("plugin_node_export_remove_part",0));
			
			project_to_name_el.setCallbackEnabled(CHANGED,1);
			custom_paths_cb.setCallbackEnabled(CHANGED,1);
			
			custom_paths_gb.setHidden(!custom_paths_cb.isChecked());
		}
		
		/*
		 */
		void window_close_clicked() {
			UI::save();
			windowHide(window,meta.name);
		}
		
		/*
		 */
		void project_changed() {
			string project_name = project_to_name_el.getText() + "/";
			
			nodes_el.setText(join_path(project_name,"nodes/"));
			meshes_el.setText(join_path(project_name,"meshes/"));
			materials_el.setText(join_path(project_name,"materials/"));
			properties_el.setText(join_path(project_name,"properties/"));
			textures_el.setText(join_path(project_name,"textures/"));
			terrain_el.setText(join_path(project_name,"terrains/"));
		}
		
		void custom_paths_changed() {
			custom_paths_gb.setHidden(!custom_paths_cb.isChecked());
			window.arrange();
			#ifdef HAS_INTERFACE
				window.getInterface().setWidth(window.getWidth());
				window.getInterface().setHeight(window.getHeight());
			#endif
		}
		
		/*
		 */
		void export_clicked() {
			save();
			
			thread(functionid(export_nodes));
		}
		
		void export_nodes() {
			string path = join_path(project_to_data_el.getText(),project_to_name_el.getText() + "/");
			mkdir(path,1);
			
			string filename = path;
			if(::dialogFileNode(TR("Save Node"),filename) == 0) return;
			
			path = path + basename(filename);
			path = pathname(systemPath(path)) + basename(path);
			if(::dialogFileConfirm(path) == 0) return;
			
			Node selection[0];
			nodesGetNodes(selection);
			
			for(int i = selection.size() - 1; i >= 0; i--) {
				Node candidate = selection[i];
				int can_skip = 0;
				
				for(int j = 0; j < i; j++) {
					Node parent = selection[j];
					if(parent.isChild(candidate)) {
						can_skip = 1;
						break;
					}
				}
				
				if(can_skip) selection.remove(i);
			}
			
			dialogProgressBegin(TR("Exporting nodes..."));
			
			forloop(int i = 0; selection.size()) {
				dialogProgressUpdate((100 * i) / selection.size());
				Node node = node_cast(selection[i]);
				export_node(node,"");
			}
			
			save_nodes(selection,path);
			
			dialogProgressEnd();
		}
		
		void export_node(Node node,string path) {
			if(node == NULL) return;
			
			NodeDependencies dependencies = new NodeDependencies();
			
			get_node_dependencies(node,dependencies);
			
			mkdirs(dependencies.nodes);
			mkdirs(dependencies.meshes);
			mkdirs(dependencies.textures);
			mkdirs(dependencies.material_libs);
			mkdirs(dependencies.property_libs);
			
			save_meshes(dependencies.meshes);
			save_textures(dependencies.textures);
			
			save_libraries(dependencies.materials,dependencies.material_libs,dependencies.textures,"materials","material");
			save_libraries(dependencies.properties,dependencies.property_libs,temp,"properties","property");
			
			save_node(node,path,dependencies);
			
			foreachkey(string key; dependencies.nodes) {
				Node reference = node_load(key);
				if(reference == NULL) {
					log.error("Can't load \"%s\" node\n",engine.getDataPath() + key);
					continue;
				}
				
				export_node(reference,dependencies.nodes[key]);
				node_delete(reference);
			}
			
			delete dependencies;
		}
		
		/*
		 */
		string join_path(string path1,string path2) {
			string result = path1 + "/" + path2;
			result = replace(result,"\\","/");
			
			return pathname(result) + basename(result);
		}
		
		string replace_path(string old_path,string new_path) {
			
			if(custom_paths_cb.isChecked()) {
				return new_path + basename(old_path);
			}
			
			if(remove_part_cb.isChecked()) {
				string project_path = pathname(project_from_data_el.getText() + "/");
				string local_path = replace(old_path,project_path,"");
				return join_path(project_to_name_el.getText(),local_path);
			}
			
			return join_path(project_to_name_el.getText(),old_path);
		}
		
		int get_path(string name) {
			string path = pathname(systemPath(name));
			
			string dirs[0];
			strsplit(path,"/\\",dirs);
			path = engine.getDataPath();
			
			#ifndef _WIN32
				dirs.append(0,"");
			#endif
			
			path = "";
			forloop(int i = 0; dirs.size()) {
				path += dirs[i] + "/";
				if(is_dir(path) == 0) return "";
			}
			
			dirs.clear();
			
			path = replace(path,engine.getDataPath(),"");
			
			return path + basename(name);
		}
		
		void safe_append(string old_path,string target_path,string collection[]) {
			if(old_path == "") return;
			
			if(collection.check(old_path) != 0) return;
			if(isAbsolute(old_path)) return;
			
			string checked_old_path = get_path(old_path);
			
			/*checked_old_path can be relative (under data) or absolute (outside data)*/
			if(isAbsolute(checked_old_path)) {
				checked_old_path = basename(checked_old_path);
			}
			
			string new_path = replace_path(checked_old_path,target_path);
			new_path = trim(new_path,"/\\");
			new_path = replace(new_path,"\\","/");
			
			collection.append(old_path,new_path);
		}
		
		void safe_append_texture(string old_path,string target_path,string collection[]) {
			safe_append(old_path,target_path,collection);
			
			string texture = extension(basename(old_path),NULL);
			string uncompressed_path = dirname(old_path) + "uncompressed/";
			
			Dir dir = new Dir();
			if(dir.open(engine.getDataPath() + uncompressed_path)) {
				forloop(int i = 0; dir.getNumFiles()) {
					string old_path = replace(dir.getFileName(i),engine.getDataPath(),"");
					if(strstr(old_path,texture) == -1) continue;
					
					safe_append(old_path,target_path + "uncompressed/",collection);
				}
				
				dir.close();
			}
			
			delete dir;
		}
		
		void mkdirs(string collection[]) {
			foreach(string content; collection) {
				string path = pathname(project_to_data_el.getText() + "/" + dirname(content));
				mkdir(path,1);
			}
		}
		
		/*
		 */
		void get_node_dependencies(Node node,NodeDependencies dependencies) {
			get_node_nodes(node,dependencies.nodes);
			get_node_meshes(node,dependencies.meshes);
			get_node_textures(node,dependencies.textures);
			get_node_properties(node,dependencies.properties,dependencies.property_libs);
			get_node_materials(node,dependencies.materials,dependencies.textures,dependencies.material_libs);
			
			forloop(int i = 0; node.getNumChilds()) {
				get_node_dependencies(node_cast(node.getChild(i)),dependencies);
			}
		}
		
		void get_node_nodes(Node node,string nodes[]) {
			if(node is NodeLayer || node is WorldLayer || node is NodeReference) {
				safe_append(node.call("getNodeName"),nodes_el.getText(),nodes);
			}
			
			if(node is WorldCluster || node is WorldClutter) {
				forloop(int i = 0; node.call("getNumReferences")) {
					safe_append(node.call("getReferenceName",i),nodes_el.getText(),nodes);
				}
			}
		}
		
		void get_node_meshes(Node node,string meshes[]) {
			int is_mesh = (node is ObjectMeshStatic || node is ObjectMeshSkinned);
			
			if(is_mesh || node is ObjectMeshCluster || node is ObjectMeshClutter) {
				safe_append(node.call("getMeshName"),meshes_el.getText(),meshes);
			}
			
			if(node is ObjectMeshClutter || node is WorldClutter) {
				safe_append(node.call("getMaskMeshName"),meshes_el.getText(),meshes);
			}
			
			if(node is ObjectTerrain) {
				string terrain_path = systemEditorPath(node.call("getTerrainName"));
				safe_append(terrain_path,terrain_el.getText(),meshes);
				
				Dir dir = new Dir(pathname(systemPath(terrain_path)));
				
				if(dir.isOpened()) {
					string base_name = basename(terrain_path);
					string path_name = pathname(terrain_path);
					
					forloop(int i = 0; dir.getNumFiles()) {
						string file_name = basename(dir.getFileName(i));
						
						if(file_name != base_name) {
							safe_append(path_name + file_name,terrain_el.getText(),meshes);
						}
					}
					dir.close();
				}
				
				delete dir;
			}
		}
		
		void get_node_textures(Node node,string textures[]) {
			if(node is ObjectGrass || node is ObjectMeshClutter || node is WorldClutter) {
				safe_append(node.call("getMaskImageName"),textures_el.getText(),textures);
			}
			
			if(node is ObjectTerrain) {
				ObjectTerrain terrain = node;
				string terrain_name = basename(systemEditorPath(terrain.getTerrainName()));
				
				// coarse
				safe_append(terrain.getDiffuseTextureName(),terrain_el.getText(),textures);
				safe_append(terrain.getNormalTextureName(),terrain_el.getText(),textures);
				safe_append(terrain.getMaskTextureName(),terrain_el.getText(),textures);
				safe_append(terrain.getIndexTextureName(),terrain_el.getText(),textures);
				
				// surfaces
				string path = pathname(terrain_el.getText() + "/" + terrain_name);
				
				forloop(int x = 0; terrain.getSurfacesX()) {
					forloop(int y = 0; terrain.getSurfacesY()) {
						safe_append(terrain.getSurfaceDiffuseTextureName(x,y),path,textures);
						safe_append(terrain.getSurfaceNormalTextureName(x,y),path,textures);
						safe_append(terrain.getSurfaceMaskTextureName(x,y),path,textures);
						safe_append(terrain.getSurfaceIndexTextureName(x,y),path,textures);
					}
				}
				
				// materials
				string material_path = pathname(terrain_el.getText() + "/materials");
				
				forloop(int i = 0; terrain.getNumMaterials()) {
					safe_append_texture(terrain.getMaterialDiffuseTextureName(i),material_path,textures);
					safe_append_texture(terrain.getMaterialNormalTextureName(i),material_path,textures);
					safe_append_texture(terrain.getMaterialSpecularTextureName(i),material_path,textures);
				}
				
				// arrays
				safe_append_texture(terrain.getDiffuseTextureArrayName(),terrain_el.getText(),textures);
				safe_append_texture(terrain.getNormalTextureArrayName(),terrain_el.getText(),textures);
				safe_append_texture(terrain.getSpecularTextureArrayName(),terrain_el.getText(),textures);
			}
		}
		
		void get_node_properties(Node node,string properties[],string libraries[]) {
			
			void add_property(Property property) {
				if(property == NULL) return;
				
				string name = property.getName();
				if(name == "") return;
				
				int id = engine.properties.findPropertyLibrary(name);
				if(id == -1) return;
				
				if(engine.properties.isLibraryEditable(id) == 0) return;
				
				properties.append(name);
				safe_append(engine.properties.getLibraryName(id),properties_el.getText(),libraries);
			}
			
			void traverse_property(Property property) {
				while(property != NULL) {
					add_property(property);
					
					property = property.getParent();
				}
			}
			
			traverse_property(node.getProperty());
			
			if(node.isObject()) {
				Object obj = node;
				
				forloop(int i = 0; obj.getNumSurfaces()) {
					traverse_property(obj.getProperty(i));
				}
			}
		}
		
		void get_node_materials(Node node,string materials[],string textures[],string libraries[]) {
			
			void add_material(Material material) {
				if(material == NULL) return;
				
				string name = material.getName();
				int id = engine.materials.findMaterialLibrary(name);
				
				if(id == -1) return;
				if(engine.materials.isLibraryEditable(id) == 0) return;
				
				materials.append(name);
				safe_append(engine.materials.getLibraryName(id),materials_el.getText(),libraries);
			}
			
			void add_textures(Material material) {
				if(material == NULL) return;
				
				forloop(int i = 0; material.getNumTextures()) {
					if(material.getTextureType(i) != MATERIAL_TEXTURE_IMAGE) continue;
					
					string old_path = material.getImageTextureName(i);
					
					if(old_path == "") continue;
					if(strstr(old_path,"core/") == 0) continue;
					
					safe_append_texture(old_path,textures_el.getText(),textures);
				}
			}
			
			void traverse_material(Material material) {
				while(material != NULL) {
					add_material(material);
					add_textures(material);
					
					material = material.getParent();
				}
			}
			
			if(node.isObject()) {
				Object obj = node;
				forloop(int i = 0; obj.getNumSurfaces()) {
					traverse_material(obj.getMaterial(i));
				}
			}
			
			if(node.isDecal()) {
				Decal decal = node;
				traverse_material(decal.getMaterial());
			}
		}
		
		/*
		 */
		void save_meshes(string meshes[]) {
			if(meshes.size() == 0) return;
			
			foreachkey(string key; meshes) {
				string full_old_path = savePath(engine.filesystem.getFileName(key));
				full_old_path = pathname(full_old_path) + basename(full_old_path);
				if(isAbsolute(full_old_path)) {
					full_old_path = setDiskLetterToLower(full_old_path);
				}
				
				string full_new_path = savePath(join_path(project_to_data_el.getText(),meshes[key]));
				full_new_path = pathname(full_new_path) + basename(full_new_path);
				
				if(full_new_path == full_old_path) continue;
				
				log.message("Copying mesh from \"%s\" to \"%s\"\n",full_old_path,full_new_path);
				if(copy(full_old_path,full_new_path) == 0) {
					log.error("Can't save \"%s\" mesh\n",full_new_path);
				}
			}
		}
		
		void save_textures(string textures[]) {
			if(textures.size() == 0) return;
			
			foreachkey(string key; textures) {
				string full_old_path = savePath(engine.filesystem.getFileName(key));
				full_old_path = pathname(full_old_path) + basename(full_old_path);
				if(isAbsolute(full_old_path)) {
					full_old_path = setDiskLetterToLower(full_old_path);
				}
				
				string full_new_path = savePath(join_path(project_to_data_el.getText(),textures[key]));
				full_new_path = pathname(full_new_path) + basename(full_new_path);
				
				if(full_new_path == full_old_path) continue;
				
				log.message("Copying texture from \"%s\" to \"%s\"\n",full_old_path,full_new_path);
				if(copy(full_old_path,full_new_path) == 0) {
					log.error("Can't save \"%s\" texture\n",full_new_path);
				}
			}
		}
		
		void save_libraries(string data[],string libraries[],string fixes[],string library_type,string data_type) {
			
			int can_skip(string name) {
				forloop(int i = 0; data.size()) {
					if(name == data[i]) return 0;
				}
				
				return 1;
			}
			
			Xml get_merge_candidate(Xml root,string name) {
				forloop(int i = 0; root.getNumChilds()) {
					Xml child = root.getChild(i);
					if(child.getArg("name") == name) return child;
				}
				
				return NULL;
			}
			
			void merge(Xml new_data,Xml old_data) {
				for(int i = new_data.getNumChilds() - 1; i >= 0; i--) {
					Xml child = new_data.getChild(i);
					child.clear();
				}
				
				new_data.parse(old_data.getSubTree());
			}
			
			void fix_paths(Xml root) {
				if(fixes.size() == 0) return;
				
				forloop(int i = 0; root.getNumChilds()) {
					Xml child = root.getChild(i);
					string path = child.getData();
					
					if(fixes.check(path)) child.setData(fixes[path]);
				}
			}
			
			Xml create_copy(Xml node,Xml parent) {
				Xml res = new Xml();
				res.parse(node.getSubTree());
				
				return parent.addChild(res);
			}
			
			if(data.size() == 0) return;
			if(libraries.size() == 0) return;
			
			foreachkey(string key; libraries) {
				string full_old_path = engine.filesystem.getFileName(key);
				string full_new_path = join_path(project_to_data_el.getText(),libraries[key]);
				
				log.message("Copying library from \"%s\" to \"%s\"\n",full_old_path,full_new_path);
				
				Xml old_library = new Xml();
				if(old_library.load(full_old_path) == 0) {
					log.error("Can't save \"%s\" library\n",full_new_path);
					
					delete old_library;
					continue;
				}
				
				if(old_library.getName() != library_type) {
					log.error("Can't save \"%s\" library\n",full_new_path);
					
					delete old_library;
					continue;
				}
				
				Xml new_library = new Xml();
				
				if(is_file(full_new_path)) {
					new_library.load(full_new_path);
				}
				
				if(new_library.getName() != library_type) {
					delete new_library;
					
					new_library = new Xml(library_type);
					new_library.setArg("version",old_library.getArg("version"));
				}
				
				Xml temp_library = new Xml(library_type);
				temp_library.setArg("version",old_library.getArg("version"));
				
				string names_added[];
				names_added.clear();
				
				forloop(int i = 0; old_library.getNumChilds()) {
					Xml old_data = old_library.getChild(i);
					string name = old_data.getArg("name");
					if(can_skip(name)) continue;
					
					Xml new_data = get_merge_candidate(new_library,name);
					if(new_data == NULL) new_data = temp_library.addChild(data_type);
					else new_data = create_copy(new_data,temp_library);
					
					merge(new_data,old_data);
					fix_paths(new_data);
					temp_library.addChild(new_data);
					names_added.append(name);
				}
				
				forloop(int i = 0; new_library.getNumChilds()) {
					Xml new_data = new_library.getChild(i);
					
					if(names_added.check(new_data.getArg("name"))) continue;
					
					create_copy(new_data,temp_library);
				}
				
				delete new_library;
				delete old_library;
				
				if(temp_library.save(full_new_path) == 0) {
					log.error("Can't save \"%s\" library\n",full_new_path);
				}
				
				delete temp_library;
			}
		}
		
		void save_node(Node node,string path,NodeDependencies dependencies) {
			if(path == "") return;
			string full_new_path = join_path(project_to_data_el.getText(),path);
			
			log.message("Saving node to \"%s\"\n",full_new_path);
			
			if(engine.world.saveNode(full_new_path,node) == 0) {
				log.error("Can't save \"%s\" node\n",full_new_path);
				return;
			}
			
			fix_node(full_new_path,dependencies);
		}
		
		void save_nodes(Node nodes[],string path) {
			NodeDependencies dependencies = new NodeDependencies();
			forloop(int i = 0; nodes.size()) {
				Node node = node_cast(nodes[i]);
				get_node_dependencies(node,dependencies);
			}
			
			if(engine.world.saveNodes(path,nodes) == 0) {
				log.error("Can't save \"%s\" node\n",path);
				return;
			}
			
			fix_node(path,dependencies);
			delete dependencies;
		}
		
		/*
		 */
		void fix_node(string path,NodeDependencies dependencies) {
			void fix_paths(Xml root) {
				forloop(int i = 0; root.getNumArgs()) {
					string arg = root.getArgValue(i);
					
					if(dependencies.nodes.check(arg)) root.setArgValue(i,dependencies.nodes[arg]);
					if(dependencies.meshes.check(arg)) root.setArgValue(i,dependencies.meshes[arg]);
					if(dependencies.textures.check(arg)) root.setArgValue(i,dependencies.textures[arg]);
					if(dependencies.material_libs.check(arg)) root.setArgValue(i,dependencies.material_libs[arg]);
					if(dependencies.property_libs.check(arg)) root.setArgValue(i,dependencies.property_libs[arg]);
				}
				
				string data = root.getData();
				
				if(dependencies.nodes.check(data)) root.setData(dependencies.nodes[data]);
				if(dependencies.meshes.check(data)) root.setData(dependencies.meshes[data]);
				if(dependencies.textures.check(data)) root.setData(dependencies.textures[data]);
				if(dependencies.material_libs.check(data)) root.setData(dependencies.material_libs[data]);
				if(dependencies.property_libs.check(data)) root.setData(dependencies.property_libs[data]);
				
				forloop(int i = 0; root.getNumChilds()) {
					fix_paths(root.getChild(i));
				}
			}
			
			Xml root = new Xml();
			if(root.load(path) == 0) {
				log.error("Can't fix \"%s\" node paths\n",path);
				delete root;
				
				return;
			}
			
			fix_paths(root);
			
			if(root.save(path) == 0) {
				log.error("Can't fix \"%s\" node paths\n",path);
			}
			
			delete root;
		}
	}
}
