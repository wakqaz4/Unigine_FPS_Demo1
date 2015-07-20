/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_assets_console.h
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

#ifndef __EDITOR_ASSETS_CONSOLE_H__
#define __EDITOR_ASSETS_CONSOLE_H__

/*
 */
namespace Assets::Console {
	
	/**************************************************************************\
	*
	* Data
	*
	\**************************************************************************/
	
	/*
	 */
	enum {
		COMMAND_RELOAD = 0,
		COMMAND_DELETE,
		COMMAND_IMPORT,
		COMMAND_VERBOSE,
	};
	
	string commands[0] = (
		"assets_reload",
		"assets_delete",
		"assets_import",
		"assets_verbose",
	);
	
	string tooltips[0] = (
		// reload
		"scan asset folder and sync data folder",
		
		// delete
		"delete all asset folder\n"
		"\t0 - delete .asset files only\n"
		"\t1 - delete .assets with corresponding runtime assets",
		
		// import
		"import asset from external source to the assets destination folder",
		
		// verbose
		"set log level for asset system\n"
		"\t0 - print only errors and warnings\n"
		"\t1 - print all info",
	);
	
	string callbacks[0] = (
		"Assets::Console::reload_callback",
		"Assets::Console::delete_callback",
		"Assets::Console::import_callback",
		"Assets::Console::verbose_callback",
	);
	
	string usages[0] = (
		// reload
		NULL,
		
		// delete
		"usage: assets_delete delete_runtimes\n\n"
		"delete_runtimes - assets deletion mode:\n"
		"\t0 - delete .asset file, keep generated files inside data folder\n"
		"\t1 - delete both .asset and generated files\n",
		
		// import
		"usage: assets_import create_link source_file <destination_path>\n\n"
		"create_link - asset creation mode:\n"
		"\t0 - copy source file and store it in the assets folder close to corresponding .asset file\n"
		"\t1 - don't copy source file and keep the link to source file inside .asset file\n"
		"source_file - absolute path to source file\n"
		"destination_path - optional, destination folder relative to assets dir, .asset file will have the same name as source file\n",
		
		// verbose
		NULL,
	);
	
	int verbose = 0;
	
	/**************************************************************************\
	*
	* Init / Shutdown
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		forloop(int i = 0; commands.size()) {
			if(engine.console.isCommand(commands[i]) == 0) engine.console.addCommand(commands[i],tooltips[i],callbacks[i]);
		}
		
		verbose = configGet("assets_verbose",0);
	}
	
	void shutdown() {
		forloop(int i = 0; commands.size()) {
			if(engine.console.isCommand(commands[i])) engine.console.removeCommand(commands[i]);
		}
		
		configSet("assets_verbose",verbose);
	}
	
	/**************************************************************************\
	*
	* Callbacks
	*
	\**************************************************************************/
	
	/*
	 */
	void reload_callback() {
		asset_benchmark_start<reload_time>;
		
		// first pass - scan root dir recursively and search for assets
		Asset new_assets[] = (:);
		string new_files[0] = ();
		assetsScan(assetsGetPath(),new_assets,new_files);
		
		// second pass - find links between found files and corresponding .asset files, fix conflicts
		AssetSystem state = Assets::Common::state;
		state.clear();
		
		assetsResolve(state,new_assets,new_files);
		
		new_assets.delete();
		new_files.delete();
		
		// third pass - preprocess intermediate assets
		assetsPreprocess(state);
		
		// fourth pass - generate runtime assets
		assetsGenerate(state);
		
		// save all assets
		foreachkey(string guid; state.asset_instances) {
			Asset asset = state.asset_instances[guid];
			string asset_path = state.asset_paths[guid] + "." + Assets::Common::ASSET_EXTENSION;
			
			assetsSaveAsset(asset,asset_path);
		}
		
		asset_benchmark_end<reload_time>;
	}
	
	/*
	 */
	void delete_callback() {
		log.message(usages[COMMAND_DELETE]);
	}
	
	void delete_callback(string delete_runtimes) {
		asset_benchmark_start<delete_time>;
		
		assetsDelete(Assets::Common::state,int(delete_runtimes));
		
		asset_benchmark_end<delete_time>;
	}
	
	/*
	 */
	void import_callback() {
		log.message(usages[COMMAND_IMPORT]);
	}
	
	void import_callback(string create_link) {
		log.message(usages[COMMAND_IMPORT]);
	}
	
	void import_callback(string create_link,string source_file) {
		asset_benchmark_start<import_time>;
		
		source_file = replace(source_file,"\\","/");
		string destination_file = assetsGetPath() + basename(source_file);
		
		assetsImport(Assets::Common::state,source_file,destination_file,int(create_link));
		
		asset_benchmark_end<import_time>;
	}
	
	void import_callback(string create_link,string source_file,string destination_path) {
		asset_benchmark_start<import_time>;
		
		source_file = replace(source_file,"\\","/");
		destination_path = replace(destination_path,"\\","/");
		destination_path = assetsGetPath() + pathname(destination_path + "/");
		
		if(is_dir(destination_path) == 0 && mkdir(destination_path,1) == 0) {
			log.error("assets_import: can't create \"%s\" folder\n",destination_path);
			return;
		}
		
		string destination_file = destination_path + basename(source_file);
		
		assetsImport(Assets::Common::state,source_file,destination_file,int(create_link));
		
		asset_benchmark_end<import_time>;
	}
	
	/*
	 */
	void verbose_callback() {
		log.message("\tassets_verbose = %d\n",verbose);
	}
	
	void verbose_callback(string value) {
		verbose = int(value);
		log.message("\tassets_verbose = %d\n",verbose);
	}
	
} /* namespace Assets::Console */

/******************************************************************************\
*
* Public API
*
\******************************************************************************/

/*
 */
int assetsVerbose() {
	return Assets::Console::verbose;
}

#endif /* __EDITOR_ASSETS_CONSOLE_H__ */
