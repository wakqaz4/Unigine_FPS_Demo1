/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_assets_nodes.h
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

#ifndef __EDITOR_ASSETS_NODES_H__
#define __EDITOR_ASSETS_NODES_H__

#include <core/scripts/array.h>

namespace Assets::Nodes {
	
	/*
	 */
	string xml_lookup_nodes[0] = (
		
		// Common
		"texture",
		"mesh_name",
		"library",
		
		// NodeLayer/WorldLayer specific
		"layer",
		
		// NodeReference/WorldClutter/WorldCluster specific
		"reference",
		
		// WorldTransformPath specific
		"path_name"
		
		// ObjectMeshClutter/WorldClutter specific
		"mask_image",
		"mask_mesh",
		
		// ObjectTerrain specific
		"diffuse_texture",
		"normal_texture",
		"specular_texture",
		
		"diffuse_texture_array",
		"normal_texture_array",
		"specular_texture_array",
		
		"index_texture",
		"mask_texture",
		
		// Decal/BodyFracture specific
		"material",
	);
	
	string xml_lookup_args[0] = (
		
		// Object specific
		"material",
		"property",
	);
	
	/*
	 */
	int process(Asset asset,AssetRuntime runtime,string source,string destination) {
		
		if(runtime.data == NULL) return 0;
		
		// TODO: get rid of this copy
		Xml root = new Xml();
		root.copy(runtime.data);
		
		void patch_node(Xml node) {
			forloop(int i = 0; node.getNumArgs()) {
				string arg_name = node.getArgName(i);
				if(xml_lookup_args.find(arg_name) != -1) {
					string new_value = assetsGetRuntimePath(node.getArgValue(i));
					if(strlen(new_value)) node.setArgValue(i,new_value);
				}
			}
			
			string name = node.getName();
			string data = node.getData();
			if(strlen(data) && xml_lookup_nodes.find(name) != -1) {
				string new_value = assetsGetRuntimePath(node.getData());
				if(strlen(new_value)) node.setData(new_value);
			}
		}
		
		foreach_xml_node(root,functionid(patch_node));
		
		if(root.save(destination) == 0) {
			delete root;
			return 0;
		}
		
		delete root;
		return 1;
	}
	
	/*
	 */
	int preprocess(Asset asset,string source) {
		if(!is_file(source)) return 0;
		
		Xml root = new Xml();
		if(root.load(source) == 0) {
			delete root;
			return 0;
		}
		
		void patch_arg(Xml node,string arg_name) {
			int id = node.findArg(arg_name);
			if(id != -1) {
				Unigine::Vector runtimes = assetsGetRuntimes(node.getArgValue(id));
				if(runtimes == NULL) return;
				
				assert(runtimes.size() < 2 && "material names conflict");
				if(runtimes.size()) {
					AssetRuntime choosen_runtime = runtimes[0];
					node.setArgValue(id,choosen_runtime.id);
				}
			}
		}
		
		void patch_node(Xml node) {
			string name = node.getName();
			string data = node.getData();
			if(strlen(data) && xml_lookup_nodes.find(name) != -1) {
				Unigine::Vector runtimes = assetsGetRuntimes(basename(data));
				
				if(runtimes != NULL && runtimes.size()) {
					AssetRuntime choosen_runtime = runtimes[0];
					
					// runtimes ambiguity, choose one with same asset path
					if(runtimes.size() > 1) {
						choosen_runtime = NULL;
						string xml_path = pathname(data);
						
						foreach(AssetRuntime runtime; runtimes.data) {
							string asset_path = pathname(assetsGetAssetPath(runtime.asset.guid));
							string runtime_path = relname(assetsGetPath(),dirname(asset_path));
							if(xml_path == runtime_path) {
								choosen_runtime = runtime;
								break;
							}
						}
						
						// didnt find appropriate runtime, choose first for now
						// TODO: console with choice
						if(choosen_runtime == NULL) {
							log.warning("Runtime collision: can't choose runtime for %s, in file %s\n",data,source);
							choosen_runtime = runtimes[0];
						}
					}
					node.setData(choosen_runtime.id);
				}
			}
			
			forloop(int i = 0; node.getNumArgs()) {
				string arg_name = node.getArgName(i);
				if(xml_lookup_args.find(arg_name) != -1) {
					patch_arg(node,arg_name);
				}
			}
		}
		
		foreach_xml_node(root,functionid(patch_node));
		
		// find runtime to add data
		string runtime_name = basename(source);
		AssetRuntime target_runtime = asset.findRuntime(runtime_name);
		
		if(target_runtime == NULL) {
			log.warning("Runtime %s not found\n",runtime_name);
			
			delete root;
			return 0;
		}
		
		delete target_runtime.data;
		target_runtime.data = new Xml();
		target_runtime.data.copy(root);
		
		delete root;
		return 1;
	}
	
	/*
	 */
	int import(Asset asset,string source) {
		
		asset.setPreprocessor("Assets::Nodes::preprocess");
		asset.addRuntime(basename(source),"Assets::Nodes::process");
		return 1;
	}
}

#endif /* __EDITOR_ASSETS_NODES_H__ */
