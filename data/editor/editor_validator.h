/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    validator_validator.cpp
 * Desc:    Unigine editor
 * Version: 1.02
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
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
namespace Validator {
	
	/*
	 */
	int dialog_show;				// dialog show flag
	WidgetDialog dialog;			// validator dialog
	WidgetListBox messages_lb;		// validator messages
	
	Object objects[0];				// all objects
	string materials[];				// all material names
	string nodes[];					// all node names
	
	/*
	 */
	void add_message(string message) {
		string res[0];
		strsplit(message,"\n",res);
		foreach(string s; res) {
			messages_lb.addItem(replace(s,"\n",""));
		}
	}
	
	/*
	 */
	void get_nodes(Node node) {
		if(node.getType() == NODE_LAYER) {
			NodeLayer layer = node_cast(node);
			nodes.append(layer.getNodeName());
		}
		else if(node.getType() == NODE_REFERENCE) {
			NodeReference reference = node_cast(node);
			nodes.append(reference.getNodeName());
			get_nodes(reference.getNode());
		}
		else if(node.getType() == NODE_WORLD_CLUSTER) {
			WorldCluster cluster = node_cast(node);
			forloop(int i = 0; cluster.getNumReferences()) {
				nodes.append(cluster.getReferenceName(i));
			}
		}
		else if(node.getType() == NODE_WORLD_CLUTTER) {
			WorldClutter clutter = node_cast(node);
			forloop(int i = 0; clutter.getNumReferences()) {
				nodes.append(clutter.getReferenceName(i));
			}
		}
		else if(node.isObject()) {
			Object object = node_cast(node);
			objects.append(object);
			forloop(int i = 0; object.getNumSurfaces()) {
				string name = object.getMaterialName(i);
				if(strlen(name)) materials.append(name);
			}
		}
		else if(node.isDecal()) {
			Decal decal = node_cast(node);
			string name = decal.getMaterialName();
			if(strlen(name)) materials.append(name);
		}
		forloop(int i = 0; node.getNumChilds()) {
			get_nodes(node.getChild(i));
		}
	}
	
	/*
	 */
	int validate_directory(string names[],string path) {
		
		int ret = 1;
		
		string directories[];
		foreachkey(string name; names) {
			if(strstr(name,"core/") != 0) {
				directories.append(pathname(name));
				if(strstr(name,path) != 0) {
					add_message(format(TR("File \"%s\" is not referenced into the \"%s\" directory"),name,path));
					ret = 0;
				}
			}
		}
		
		string files[0];
		Dir dir = new Dir();
		foreachkey(string path; directories) {
			if(dir.open(engine.getDataPath() + path)) {
				files.clear();
				forloop(int i = 0; dir.getNumFiles()) {
					files.append(dir.getFileName(i));
				}
				files.sort();
				forloop(int i = 0; files.size()) {
					string name = replace(files[i],engine.getDataPath(),"");
					if(names.check(name) == 0) {
						add_message(format(TR("Unused file \"%s\" in the \"%s\" directory"),name,path));
						ret = 0;
					}
				}
				dir.close();
			}
		}
		delete dir;
		
		return ret;
	}
	
	/*
	 */
	int validate_materials(string path) {
		
		int ret = 1;
		
		// check unused materials
		forloop(int i = 0; engine.materials.getNumLibraries()) {
			string library = engine.materials.getLibraryName(i);
			if(strstr(library,"core/materials/") == 0) continue;
			forloop(int j = 0; engine.materials.getNumMaterials(i)) {
				Material material = engine.materials.getMaterial(i,j);
				if(material.getNumChilds() != 0) continue;
				string name = engine.materials.getMaterialName(i,j);
				if(materials.check(name) == 0) {
					add_message(format(TR("Unused material \"%s\" in the \"%s\" library"),name,library));
					ret = 0;
				}
			}
		}
		
		// check material libraries
		foreach(Object object; objects) {
			forloop(int i = 0; object.getNumSurfaces()) {
				int find = 0;
				string name = object.getMaterialName(i);
				if(strlen(name) == 0) continue;
				string library = engine.materials.getLibraryName(engine.materials.findMaterialLibrary(name));
				if(strstr(library,"core/") == 0) continue;
				forloop(int j = 0; engine.materials.getNumWorldLibraries()) {
					if(engine.materials.getWorldLibraryName(j) == library) {
						find = 1;
						break;
					}
				}
				if(find == 0) {
					add_message(format(TR("Can't find \"%s\" library for \"%s\" material in the list of preloaded material libraries"),library,name));
					ret = 0;
				}
			}
		}
		
		// material textures
		string textures[];
		forloop(int i = 0; engine.materials.getNumWorldLibraries()) {
			string name = engine.materials.getWorldLibraryName(i);
			int library = engine.materials.findLibrary(name);
			if(library == -1) {
				add_message(format(TR("Can't find \"%s\" library\n"),name));
				ret = 0;
				continue;
			}
			forloop(int j = 0; engine.materials.getNumMaterials(library)) {
				string name = engine.materials.getMaterialName(library,j);
				Material material = engine.materials.findMaterial(name);
				forloop(int k = 0; material.getNumTextures()) {
					if(material.getTextureType(k) == MATERIAL_TEXTURE_IMAGE) {
						textures.append(material.getImageTextureName(k));
					}
				}
			}
		}
		
		// validate directory
		ret &= validate_directory(textures,path);
		
		return ret;
	}
	
	/*
	 */
	int validate_properties(string path) {
		
		return 1;
	}
	
	/*
	 */
	int validate_meshes(string path) {
		
		int ret = 1;
		
		// mesh names
		string meshes[];
		foreach(Object object; objects) {
			if(object.getType() == NODE_OBJECT_MESH_STATIC) {
				string name = ObjectMeshStatic(object).getMeshName();
				meshes.append(name);
			}
			else if(object.getType() == NODE_OBJECT_MESH_CLUSTER) {
				string name = ObjectMeshCluster(object).getMeshName();
				meshes.append(name);
			}
			else if(object.getType() == NODE_OBJECT_MESH_CLUTTER) {
				string name = ObjectMeshClutter(object).getMeshName();
				meshes.append(name);
			}
			else if(object.getType() == NODE_OBJECT_MESH_SKINNED) {
				string name = ObjectMeshSkinned(object).getMeshName();
				meshes.append(name);
			}
		}
		
		// validate directory
		ret &= validate_directory(meshes,path);
		
		return ret;
	}
	
	/*
	 */
	int validate_nodes(string path) {
		
		int ret = 1;
		
		// validate directory
		ret &= validate_directory(nodes,path);
		
		return ret;
	}
	
	/*
	 */
	void dialog_clicked() {
		gui.removeChild(dialog);
	}
	
	/*
	 */
	void init() {
		
		dialog.setCallback(GUI_CLICKED,functionid(dialog_clicked));
		
		editorDialog(dialog);
		
		int width = configGet("validator_width",dialog.getWidth());
		int height = configGet("validator_height",dialog.getHeight());
		int position_x = configGet("validator_position_x",dialog.getPositionX());
		int position_y = configGet("validator_position_y",dialog.getPositionY());
		
		dialog.setWidth(width);
		dialog.setHeight(height);
		dialog.setPosition(position_x,position_y);
	}
	
	void shutdown() {
		
		if(dialog_show) {
			configSet("validator_width",dialog.getWidth());
			configSet("validator_height",dialog.getHeight());
			configSet("validator_position_x",dialog.getPositionX());
			configSet("validator_position_y",dialog.getPositionY());
		}
	}
	
	/*
	 */
	void run() {
		
		string path = getWorldPath();
		if(is_string(path) == 0) {
			dialogMessageOk(TR("Error"),format(TR("Can't find path for \"%s\" world\n"),engine.world.getName()));
			return;
		}
		
		messages_lb.clear();
		
		objects.clear();
		materials.clear();
		nodes.clear();
		
		forloop(int i = 0; engine.editor.getNumNodes()) {
			get_nodes(engine.editor.getNode(i));
		}
		
		int ret = validate_materials(path);
		ret &= validate_properties(path);
		ret &= validate_meshes(path);
		ret &= validate_nodes(path);
		
		if(ret) dialog.setText(FONT_GREEN + TR("Validation complete"));
		else dialog.setText(FONT_RED + TR("Validation failed"));
	}
	
} /* namespace Validator */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void validatorInit() {
	Validator::init();
}

void validatorShutdown() {
	Validator::shutdown();
}

/*
 */
void validatorRun() {
	Validator::run();
	Validator::dialog_show = 1;
	Validator::dialog.arrange();
	gui.addChild(Validator::dialog,GUI_ALIGN_OVERLAP | GUI_ALIGN_CENTER);
	Validator::dialog.setFocus();
}
