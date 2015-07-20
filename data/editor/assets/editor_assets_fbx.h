/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_assets_fbx.h
 * Desc:    Unigine editor
 * Version: 1.00
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

#ifndef __EDITOR_ASSETS_FBX_H__
#define __EDITOR_ASSETS_FBX_H__

/*
 */
#define FILL_FBX_NAMES(TYPE,UPPER,UPPER_MULTIPLE) \
void fill_ ## TYPE ## _names() { \
	int names_map[] = (:); \
	forloop(int i = 0; import.getNum ## UPPER_MULTIPLE()) { \
		string name = import.get ## UPPER ## Name(i); \
		if(names_map.check(name)) { \
			name = getNewName( \
				name + "_0", \
				[](string name) { return names_map.check(name); } \
			); \
		} \
		names_map.append(name); \
		TYPE ## _names.append(name); \
	} \
} \

namespace Assets::Fbx {
	
	/* map: string -> autodesk material import parameter
	 */
	int name_to_import_parameter[] = (
		"emissive"		: AUTODESK_IMPORT_PARAMETER_EMISSIVE,
		"ambient"		: AUTODESK_IMPORT_PARAMETER_AMBIENT,
		"diffuse"		: AUTODESK_IMPORT_PARAMETER_DIFFUSE,
		"specular"		: AUTODESK_IMPORT_PARAMETER_SPECULAR,
		"shininess"		: AUTODESK_IMPORT_PARAMETER_SHININESS,
		"normalmap"		: AUTODESK_IMPORT_PARAMETER_NORMALMAP,
		"bumpmap"		: AUTODESK_IMPORT_PARAMETER_BUMPMAP,
		"reflection"	: AUTODESK_IMPORT_PARAMETER_REFLECTION,
		"transparency"	: AUTODESK_IMPORT_PARAMETER_TRANSPARENCY,
	);
	
	/* map: autodesk material import parameter -> string
	 */
	int import_parameter_to_name[] = (
		AUTODESK_IMPORT_PARAMETER_EMISSIVE		: "emissive",
		AUTODESK_IMPORT_PARAMETER_AMBIENT		: "ambient",
		AUTODESK_IMPORT_PARAMETER_DIFFUSE		: "diffuse",
		AUTODESK_IMPORT_PARAMETER_SPECULAR		: "specular",
		AUTODESK_IMPORT_PARAMETER_SHININESS		: "shininess",
		AUTODESK_IMPORT_PARAMETER_NORMALMAP		: "normalmap",
		AUTODESK_IMPORT_PARAMETER_BUMPMAP		: "bumpmap",
		AUTODESK_IMPORT_PARAMETER_REFLECTION	: "reflection",
		AUTODESK_IMPORT_PARAMETER_TRANSPARENCY	: "transparency",
	);
	
	/*  Wraps names in AutodeskImport
	 */
	class AutodeskWrapper {
		
		AutodeskImport import;
		
		string path;						// file path
		string mesh_names[0];				// unique import mesh names
		string material_names[0];			// unique import material names
		
		FILL_FBX_NAMES(mesh,Mesh,Meshes)
		FILL_FBX_NAMES(material,Material,Materials)
		
		AutodeskWrapper() {
			import = new AutodeskImport();
		}
		
		~AutodeskWrapper() {
			delete import;
		}
		
		int load(string _path) {
			path = _path;
			if(!import.load(path)) return 0;
			
			fill_mesh_names();
			fill_material_names();
			return 1;
		}
	};
	
	namespace FbxCache {
		
		// path : instance
		AutodeskWrapper data[];
		
		// path : hash
		string hashes[];
		
		AutodeskWrapper get(string path) {
			
			log.message("path = %s\n",path);
			if(!is_file(path) || extension(path) != "fbx") return NULL;
			
			string hash = assetsGenerateHash(path);
			
			if(data.check(path)) {
				
				assert(hashes.check(path) && "AutodeskImport get:: no hash for file found");
				
				if(hash == hashes[path]) {
					return data[path];
				} else {
					delete data[path];
					data.remove(path);
					hashes.remove(path);
				}
			}
			
			AutodeskWrapper wrapper = new AutodeskWrapper();
			if(!wrapper.load(path)) {
				delete wrapper;
				return NULL;
			}
			
			data.append(path,wrapper);
			hashes.append(path,hash);
			return wrapper;
		}
	}
	
	/*
	 */
	int process_mesh(Asset asset,AssetRuntime runtime,string source,string destination) {
		
//		if(asset.getImportParameterInt("import_meshes",1) == 0) return 0;
		
//		AutodeskImport import = FbxCache::get(source);
//		if(import == NULL) return 0;
		
		return 1;
	}
	
	/*
	 */
	int process_texture(Asset asset,AssetRuntime runtime,string source,string destination) {
		
		return 1;
	}
	
	/*
	 */
	int process_material_library(Asset asset,AssetRuntime runtime,string source,string destination) {
		
		return 1;
	}
	
	/*
	 */
	int process_animation(Asset asset,AssetRuntime runtime,string source,string destination) {
		
		return 1;
	}
	
	/*
	 */
	int process_node(Asset asset,AssetRuntime runtime,string source,string destination) {
		
		return 1;
	}
	
	/*
	 */
	int import(Asset asset,string source) {
		
		AutodeskWrapper wrapper = FbxCache::get(source);
		if(wrapper == NULL) return 0;
		
		// import parameters
		// TODO: gui with import settings
		int merge_static_meshes = 0;
		int vertex_cache = 1;
		int import_meshes = 1;
		int import_materials = 1;
		int import_textures = 1;
		int import_animations = 1;
		int import_nodes = 0;
		int import_joints = 0;
		int import_lights = 0;
		int import_cameras = 0;
		int import_tangent_space = 0;
		float scale = 1.0f;
		float fps = 60.0f;
		
		asset.clearImportParameters();
		
		asset.addImportParameter("merge_static_meshes","int",merge_static_meshes);
		asset.addImportParameter("vertex_cache","int",vertex_cache);
		asset.addImportParameter("import_meshes","int",import_meshes);
		asset.addImportParameter("import_materials","int",import_materials);
		asset.addImportParameter("import_textures","int",import_textures);
		asset.addImportParameter("import_animations","int",import_animations);
		asset.addImportParameter("import_nodes","int",import_nodes);
		asset.addImportParameter("import_joints","int",import_joints);
		asset.addImportParameter("import_lights","int",import_lights);
		asset.addImportParameter("import_cameras","int",import_cameras);
		asset.addImportParameter("tangent_space","int",import_tangent_space);
		asset.addImportParameter("scale","float",scale);
		asset.addImportParameter("fps","float",fps);
		
		AutodeskImport import = wrapper.import;
		
		// mesh runtimes
		if(import_meshes) {
			for(int i = 0; i < import.getNumMeshes(); i++) {
				string mesh_name = wrapper.mesh_names[i] + ".mesh";
				AssetRuntime runtime = asset.addRuntime(mesh_name,"Assets::Fbx::process_mesh");
				runtime.addParameter("index","int",i);
			}
		}
		
		// material runtimes
		if(import_materials) {
			string library_name = extension(basename(source),"mat");
			asset.addRuntime(library_name,"Assets::Fbx::process_material_library");
			for(int i = 0; i < import.getNumMaterials(); i++) {
				asset.addRuntime(wrapper.material_names[i],"");
			}
		}
		
		// texture runtimes
		if(import_textures) {
			for(int i = 0; i < import.getNumMaterials(); i++) {
				for(int type = 0; type < name_to_import_parameter.size(); type++) {
					string texture_path = import.getMaterialImage(i,type);
					if(strlen(texture_path) == 0) continue;
					
					string name = extension(basename(texture_path),"dds");
					AssetRuntime runtime = asset.addRuntime(name,"Assets::Fbx::process_texture");
					runtime.addParameter("index","int",i);
					runtime.addParameter("type","string",import_parameter_to_name[type]);
				}
			}
		}
		
		// animation runtimes
		if(import_animations && import.getNumAnimations()) {
			// TODO: add meshes to cache
			for(int i = 0; i < import.getNumMeshes(); i++) {
				Mesh mesh = new Mesh();
				int ret = import.getMesh(i,mesh,scale,import_tangent_space);
				
				if(ret) ret = (mesh.getNumBones() != 0);
				delete mesh;
				
				if(ret) {
					string animation_name = wrapper.mesh_names[i] + ".anim";
					asset.addRuntime(animation_name,"Assets::Fbx::process_animation");
				}
			}
		}
		
		// TODO: node runtimes
		
		return 1;
	}
}

#endif /* __EDITOR_ASSETS_FBX_H__ */

