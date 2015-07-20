/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_assets_materials.h
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

#ifndef __EDITOR_ASSETS_MATERIALS_H__
#define __EDITOR_ASSETS_MATERIALS_H__

#include <core/scripts/array.h>

namespace Assets::Materials {
	
	/*
	 */
	int process(Asset asset,AssetRuntime runtime,string source,string destination) {
		
		if(runtime.data == NULL) return 0;
		
		// TODO: get rid of this copy
		Xml root = new Xml();
		root.copy(runtime.data);
		
		void patch_arg(Xml node,string arg_name) {
			int id = node.findArg(arg_name);
			if(id != -1) {
				string new_value = assetsGetRuntimePath(node.getArgValue(id));
				if(strlen(new_value)) node.setArgValue(id,new_value);
			}
		}
		
		void patch_node(Xml node) {
			string name = node.getName();
			string data = node.getData();
			if(strlen(data) && name == "texture") {
				AssetRuntime runtime = assetsGetRuntime(data);
				if(runtime != NULL) {
					string new_value = assetsGetRuntimePath(node.getData());
					if(strlen(new_value)) node.setData(new_value);
				}
			} else if(name == "material") {
				patch_arg(node,"name");
				patch_arg(node,"parent");
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
			if(strlen(data) && name == "texture") {
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
			} else if(name == "material") {
				patch_arg(node,"name");
				patch_arg(node,"parent");
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
		if(!is_file(source)) return 0;
		
		Xml root = new Xml();
		if(root.load(source) == 0) {
			delete root;
			return 0;
		}
		
		string runtime_names[0] = ();
		
		foreach_xml_node(root,
			[](Xml node) {
				if(node.getName() == "material") {
					string name = node.getArg("name");
					if(strlen(name)) runtime_names.append(name);
				}
			}
		);
		
		asset.setPreprocessor("Assets::Materials::preprocess");
		asset.addRuntime(basename(source),"Assets::Materials::process");
		foreach(string name; runtime_names) {
			asset.addRuntime(name,"",1);
		}
		
		delete root;
		return 1;
	}
}

#endif /* __EDITOR_ASSETS_MATERIALS_H__ */
