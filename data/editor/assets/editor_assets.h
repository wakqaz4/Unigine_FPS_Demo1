/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_assets.h
 * Desc:    Unigine editor
 * Version: 1.00
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

#ifndef __EDITOR_ASSETS_H__
#define __EDITOR_ASSETS_H__

#include <core/scripts/array.h>
#include <editor/assets/editor_assets_common.h>
#include <editor/assets/editor_assets_console.h>
#include <editor/assets/editor_assets_textures.h>
#include <editor/assets/editor_assets_meshes.h>
#include <editor/assets/editor_assets_materials.h>
#include <editor/assets/editor_assets_nodes.h>

#ifdef HAS_AUTODESK_IMPORT
	#include <editor/assets/editor_assets_fbx.h>
#endif

/*
 */
namespace Assets {
	
	/**************************************************************************\
	*
	* Init / Shutdown
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		Common::init();
		Console::init();
	}
	
	void shutdown() {
		Common::shutdown();
		Console::shutdown();
	}
	
	/**************************************************************************\
	*
	* Update
	*
	\**************************************************************************/
	
	/*
	 */
	void update(int need_reload) {
	}
	
	/**************************************************************************\
	*
	* Implementation
	*
	\**************************************************************************/
	
	/*
	 */
	void scan_assets(string path,Asset new_assets[],string new_files[]) {
		dirTraverse(
			path,
			[](string dirname) {
				// skip folders started with dot
				if(strchr(dirname,'.') != -1) return 0;
				
				log.message("Scanning \"%s\"\n",dirname);
				return 1;
			},
			[](string filename) {
				scan_asset(filename,new_assets,new_files);
			}
		);
	}
	
	void scan_asset(string filename,Asset new_assets[],string new_files[]) {
		
		string asset_path = filename;
		string asset_extension = lower(extension(asset_path));
		
		if(asset_extension == Assets::Common::ASSET_EXTENSION) {
			Asset asset = new Asset();
			if(assetsLoadAsset(asset,asset_path)) {
				if(assetsVerbose()) log.message("\tNew asset \"%s\"\n",asset_path);
				new_assets.append(asset_path,asset);
			}
			else {
				delete asset;
				log.error("\tCan't load \"%s\" file\n",asset_path);
			}
		}
		else if(assetsGetImporter(asset_extension) != NULL) {
			new_files.append(asset_path);
			if(assetsVerbose()) log.message("\tNew file \"%s\"\n",asset_path);
		}
	}
	
	/*
	 */
	void resolve_assets(AssetSystem state,Asset new_assets[],string new_files[]) {
		
		log.message("Resolving assets\n");
		
		Asset removed_assets[0];
		
		void clear_assets() {
			forloop(int i = 0; removed_assets.size()) {
				new_assets.remove(removed_assets[i]);
			}
			
			removed_assets.clear();
		}
		
		///////////////////////////////////
		// link .asset with external file
		///////////////////////////////////
		
		if(new_assets.size() > 0) {
			foreachkey(string asset_path; new_assets) {
				Asset asset = new_assets[asset_path];
				if(strlen(asset.link) == 0) continue;
				
				string filename = asset.link;
				string hash = assetsGenerateHash(filename);
				
				if(asset.hash != hash) {
					if(resolve_asset(asset,filename,hash)) {
						log.message("\tAsset \"%s\" was updated from \"%s\" file\n",asset_path,filename);
					}
					else {
						log.error("\tCan't update \"%s\" asset\n",asset_path);
						continue;
					}
				}
				
				if(assetsVerbose()) log.message("\tAsset \"%s\" was linked to \"%s\" external file\n",asset_path,filename);
				
				resolve_state(state,asset,extension(asset_path,NULL));
				
				// take asset list from scanned map, because we added that list to assets map
				removed_assets.append(asset_path);
			}
			
			clear_assets();
		}
		
		///////////////////////////////////
		// link .asset with internal data
		///////////////////////////////////
		
		if(new_assets.size() > 0) {
			foreachkey(string asset_path; new_assets) {
				Asset asset = new_assets[asset_path];
				int should_link = 0;
				
				forloop(int i = 0; asset.runtimes.size()) {
					AssetRuntime runtime = asset.runtimes[i];
					if(runtime.data == NULL) continue;
					
					should_link = 1;
					break;
				}
				
				if(should_link == 0) continue;
				
				if(assetsVerbose()) log.message("\tAsset \"%s\" was linked with its internal data\n",asset_path);
				
				resolve_state(state,asset,extension(asset_path,NULL));
				
				// take asset list from scanned map, because we added that list to assets map
				removed_assets.append(asset_path);
			}
			
			clear_assets();
		}
		
		///////////////////////////////////
		// link .asset near file
		///////////////////////////////////
		
		if(new_assets.size() > 0) {
			for(int i = new_files.size() - 1; i >= 0; i--) {
				string filename = new_files[i];
				
				string asset_path = filename + "." + Assets::Common::ASSET_EXTENSION;
				Asset asset = new_assets.check(asset_path,NULL);
				
				if(asset == NULL) continue;
				
				string hash = assetsGenerateHash(filename);
				
				if(asset.hash != hash) {
					if(resolve_asset(asset,filename,hash)) {
						log.message("\tAsset \"%s\" was updated from \"%s\" file\n",asset_path,filename);
					}
					else {
						log.error("\tCan't update \"%s\" asset\n",asset_path);
						continue;
					}
				}
				
				if(assetsVerbose()) log.message("\tAsset \"%s\" was linked to \"%s\" file\n",asset_path,filename);
				
				resolve_state(state,asset,filename);
				
				// take asset list from scanned map, because we added that list to assets map
				new_files.remove(i);
				new_assets.remove(asset_path);
			}
		}
		
		///////////////////////////////////
		// try to link unknown .asset
		///////////////////////////////////
		
		if(new_assets.size() > 0) {
			for(int i = new_files.size() - 1; i >= 0; i--) {
				string filename = new_files[i];
				string hash = assetsGenerateHash(filename);
				string asset_target_path = filename + "." + Assets::Common::ASSET_EXTENSION;
				
				string asset_path = asset_target_path;
				Asset asset = NULL;
				
				// look for a valid asset in the map
				foreachkey(string path; new_assets) {
					Asset new_asset = new_assets[path];
					
					if(new_asset == NULL) continue;
					if(new_asset.hash != hash) continue;
					
					asset_path = path;
					asset = new_asset;
					break;
				}
				
				if(asset == NULL) continue;
				assert(asset_path != asset_target_path && "Assets::resolve(): found unkown .asset close to file");
				
				// delete all runtimes generated from the .asset file before moving
				if(assetsDeleteRuntimes(asset,asset_path) == 0) {
					log.error("\tCan't delete runtime assets for \"%s\"\n",asset_path);
					continue;
				}
				
				// move .asset to proper place
				if(rename(asset_path,asset_target_path) == 0) {
					log.error("\tCan't move asset from \"%s\" to \"%s\"\n",asset_path,asset_target_path);
					continue;
				}
				
				if(resolve_runtimes(asset,filename,hash) == 0) {
					log.error("\tWrong runtime count of \"%s\" asset\n",asset_target_path);
					continue;
				}
				
				log.warning("\tAsset \"%s\" was moved to \"%s\" and linked to \"%s\" file\n",asset_path,asset_target_path,filename);
				
				resolve_state(state,asset,filename);
				
				// take asset list from scanned map, because we added that list to assets map
				new_files.remove(i);
				new_assets.remove(asset_path);
			}
		}
		
		///////////////////////////////////
		// create new .asset and link with file
		///////////////////////////////////
		
		for(int i = new_files.size() - 1; i >= 0; i--) {
			string filename = new_files[i];
			string hash = assetsGenerateHash(filename);
			
			string asset_path = filename + "." + Assets::Common::ASSET_EXTENSION;
			Asset asset = new Asset();
			
			if(assetsCreateAsset(asset,filename,hash) == 0) {
				delete asset;
				log.error("\tCan't create assets from \"%s\" file\n",filename);
				continue;
			}
			
			if(assetsVerbose()) log.message("\tAsset \"%s\" was created and linked to \"%s\" file\n",asset_path,filename);
			
			resolve_state(state,asset,filename);
			
			// take asset list from scanned map, because we added that list to assets map
			new_files.remove(i);
			new_assets.remove(asset_path);
		}
		
		///////////////////////////////////
		// delete all .asset without links
		///////////////////////////////////
		
		foreachkey(string path; new_assets) {
			log.warning("\tFound asset \"%s\" without link, deleting\n",path);
			assetsDeleteAsset(new_assets[path],path);
		}
		
		new_files.delete();
		new_assets.delete();
	}
	
	int resolve_asset(Asset asset,string file,string hash) {
		
		// create new asset list directly from the file
		Asset new_asset = new Asset();
		if(assetsCreateAsset(new_asset,file,hash) == 0) {
			delete new_asset;
			return 0;
		}
		
		asset.hash = new_asset.hash;
		asset.link = new_asset.link;
		
		// copy import parameters
		if(asset.importer != new_asset.importer) {
			
			asset.parameters.delete();
			forloop(int i = 0; new_asset.parameters.size()) {
				asset.parameters.append(new_asset.parameters[i]);
			}
			
			asset.importer = new_asset.importer;
		}
		
		// merge runtimes
		forloop(int i = 0; new_asset.runtimes.size()) {
			AssetRuntime dst_runtime = new_asset.runtimes[i];
			dst_runtime.asset = asset;
			
			foreach(AssetRuntime runtime; asset.runtimes) {
				if(runtime.name != dst_runtime.name) continue;
				
				dst_runtime.id = runtime.id;
				break;
			}
		}
		
		// copy runtimes
		asset.runtimes.delete();
		forloop(int i = 0; new_asset.runtimes.size()) {
			asset.runtimes.append(new_asset.runtimes[i]);
		}
		
		// cleanup
		new_asset.parameters.clear();
		new_asset.runtimes.clear();
		delete new_asset;
		
		return 1;
	}
	
	int resolve_runtimes(Asset asset,string file,string hash) {
		
		// create new asset list directly from the file
		Asset new_asset = new Asset();
		if(assetsCreateAsset(new_asset,file,hash) == 0) {
			delete new_asset;
			return 0;
		}
		
		if(new_asset.runtimes.size() != asset.runtimes.size()) {
			delete new_asset;
			return 0;
		}
		
		// fix runtime names
		forloop(int i = 0; new_asset.runtimes.size()) {
			AssetRuntime src_runtime = asset.runtimes[i];
			AssetRuntime dst_runtime = new_asset.runtimes[i];
			
			src_runtime.name = dst_runtime.name;
		}
		
		delete new_asset;
		return 1;
	}
	
	void resolve_state(AssetSystem state,Asset asset,string file) {
		
		// add new guid with its instance and path
		string guid = asset.guid;
		
		if(state.asset_instances.check(guid)) {
			log.warning("Asset GUID collision detected\n");
			
			string new_guid = assetsGenerateGUID();
			while(state.asset_instances.check(new_guid)) {
				new_guid = assetsGenerateGUID();
			}
			
			asset.guid = new_guid;
		}
		
		state.asset_instances.append(guid,asset);
		state.asset_paths.append(guid,file);
		
		string runtime_path = relname(assetsGetPath(),dirname(file));
		
		// add all runtime guids with their instances and paths
		foreach(AssetRuntime runtime; asset.runtimes) {
			guid = runtime.id;
			
			if(state.runtime_instances.check(guid)) {
				log.warning("Runtime GUID collision detected\n");
				
				string new_guid = assetsGenerateGUID();
				while(state.runtime_instances.check(new_guid)) {
					new_guid = assetsGenerateGUID();
				}
				
				asset.guid = new_guid;
			}
			
			state.runtime_instances.append(guid,runtime);
			state.runtime_paths.append(guid,(runtime.library_item) ? runtime.name : runtime_path + runtime.name);
			
			Unigine::Vector list = state.runtimes.check(runtime.name,NULL);
			if(list == NULL) {
				list = new Unigine::Vector();
				state.runtimes.append(runtime.name,list);
			}
			
			list.append(runtime);
		}
	}
	
	/*
	 */
	void preprocess_assets(AssetSystem state) {
		
		log.message("Preprocessing intermediate assets\n");
		
		foreachkey(string guid; state.asset_instances) {
			Asset asset = state.asset_instances.check(guid,NULL);
			string path = state.asset_paths.check(guid,NULL);
			
			if(asset == NULL) continue;
			if(path == NULL) continue;
			
			if(strlen(asset.preprocessor) == 0) continue;
			if(assetsVerbose()) log.message("\tPreprocessing \"%s\" file\n",path);
			
			if(assetsPreprocessAsset(asset,path) == 0) {
				log.error("\tCan't preprocess \"%s\" file\n",path);
				continue;
			}
		}
	}
	
	/*
	 */
	void generate_assets(AssetSystem state) {
		
		log.message("Generating runtime assets\n");
		
		foreachkey(string guid; state.asset_instances) {
			Asset asset = state.asset_instances.check(guid,NULL);
			string path = state.asset_paths.check(guid,NULL);
			
			if(asset == NULL) continue;
			if(path == NULL) continue;
			
			string asset_path = path + "." + Assets::Common::ASSET_EXTENSION;
			
			if(assetsVerbose()) log.message("\tGenerating runtime assets for \"%s\"\n",path);
			
			if(assetsCreateRuntimes(asset,asset_path) == 0) {
				log.error("\tCan't create runtime assets for \"%s\"\n",asset_path);
			}
			
			// dump runtime paths
			if(assetsVerbose()) {
				foreach(AssetRuntime runtime; asset.runtimes) {
					string guid = runtime.id;
					log.message("\t\tRuntime \"%s\", guid: \"%s\"\n",state.runtime_paths[guid],guid);
				}
			}
		}
	}
	
	/*
	 */
	void delete_assets(AssetSystem state,int delete_runtimes) {
		
		log.message("Deleting assets\n");
		
		foreachkey(string guid; state.asset_instances) {
			Asset asset = state.asset_instances.check(guid,NULL);
			string path = state.asset_paths.check(guid,NULL);
			
			if(asset == NULL) continue;
			if(path == NULL) continue;
			
			if(assetsDeleteAsset(asset,path,delete_runtimes) == 0) {
				log.error("\tCan't delete \"%s\" asset",path);
			}
		}
		
		state.clear();
	}
	
	/*
	 */
	int import_asset(AssetSystem state,string source,string destination,int create_link) {
		
		log.message("Importing \"%s\" asset\n",source);
		
		string filename = source;
		
		// copy source
		if(create_link == 0) {
			if(copy(source,destination) == 0) {
				log.error("\tCan't copy source file from \"%s\" to \"%s\"\n",source,destination);
				return 0;
			}
			filename = destination;
		}
		
		// create asset
		string hash = assetsGenerateHash(filename);
		string asset_path = destination + "." + Assets::Common::ASSET_EXTENSION;
		Asset asset = new Asset();
		
		if(assetsCreateAsset(asset,filename,hash) == 0) {
			log.error("\tCan't create assets from \"%s\" file\n",filename);
			delete asset;
			return 0;
		}
		
		if(create_link) asset.link = source;
		if(assetsVerbose()) log.message("\tAsset \"%s\" was created and linked to \"%s\" file\n",asset_path,filename);
		
		// create runtimes
		if(strlen(asset.preprocessor) && assetsPreprocessAsset(asset,destination) == 0) {
			log.error("\tCan't preprocess \"%s\" file\n",destination);
			delete asset;
			return 0;
		}
		
		if(assetsCreateRuntimes(asset,asset_path) == 0) {
			log.error("\tCan't create runtime assets for \"%s\" file\n",asset_path);
			delete asset;
			return 0;
		}
		
		// save
		if(assetsSaveAsset(asset,asset_path) == 0) {
			delete asset;
			return 0;
		}
		
		resolve_state(state,asset,extension(asset_path,NULL));
		return 1;
	}
	
} /* namespace Assets */

/******************************************************************************\
*
* Public API
*
\******************************************************************************/

/*
 */
void assetsInit() {
	Assets::init();
}

void assetsShutdown() {
	Assets::shutdown();
}

void assetsUpdate(int need_reload) {
	Assets::update(need_reload);
}

/* Scans directory recursively for assets
 * 
 * [in] path            - root folder to start scan from
 * [out] new_assets     - map<path,Asset>, filled with parsed Asset from found .asset files
 * [out] new_files      - list<path>, filled with valid non-.asset files
 */
void assetsScan(string path,Asset new_assets[],string new_files[]) {
	Assets::scan_assets(path,new_assets,new_files);
}

/* Resolves missing links between .asset and non-.asset files
 * 
 * If no files were found for particular .asset file it'll be automatically deleted
 * These function may rename & move .asset files if that's needed to resolve link issues
 * 
 * [in,out] new_assets  - map<path,Asset>, contains Asset collection from .asset files, will be cleared at the end
 * [in,out] new_files   - list<path>, contains valid non-.asset files, will be cleared at the end
 * [out] state          - AssetSystem, will be filled with asset & runtime instances and corresponding paths
 */
void assetsResolve(AssetSystem state,Asset new_assets[],string new_files[]) {
	Assets::resolve_assets(state,new_assets,new_files);
}

/* Fills all asset runtime instances with additional data
 *
 * Optional step, if asset preprocessor is called it may change or update any of existing asset runtime instances
 * This is useful for nodes/worlds/material libraries, because we're able to look for dependencies and replace them with known GUIDS
 *
 * [in] state           - AssetSystem, contains asset & runtime instances and corresponding paths
 */
void assetsPreprocess(AssetSystem state) {
	Assets::preprocess_assets(state);
}

/* Generates all asset runtimes to data folder
 *
 * Function will take relative path to .asset and use it during generation
 *
 * [in] state           - AssetSystem, contains asset & runtime instances and corresponding paths
 */
void assetsGenerate(AssetSystem state) {
	Assets::generate_assets(state);
}

/* Deletes all .asset files
 *
 * [in,out] state       - AssetSystem, contains asset & runtime instances and corresponding paths, will be cleared after call
 * [in] delete_runtimes - if this flag is set then this function will delete all runtime files also
 */
void assetsDelete(AssetSystem state,int delete_runtimes) {
	Assets::delete_assets(state,delete_runtimes);
}

/* Imports a new asset from source file
 *
 * [in,out] state       - AssetSystem, contains asset & runtime instances and corresponding paths, will contain imported asset after call
 * [in] source          - absolute path to source file
 * [in] destination     - absolute path to destination where the .asset file will be be saved
 * [in] create_link     - flag which allows to create and store link to source file in the .asset without the need of making copy of source file
 */
int assetsImport(AssetSystem state,string source,string destination,int create_link) {
	return Assets::import_asset(state,source,destination,create_link);
}

#endif /* __EDITOR_ASSETS_H__ */
