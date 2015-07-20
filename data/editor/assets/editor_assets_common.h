/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_assets_common.h
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

#ifndef __EDITOR_ASSETS_COMMON_H__
#define __EDITOR_ASSETS_COMMON_H__

#include <core/scripts/array.h>

/*
 */
namespace Assets::Common {
	
	/**************************************************************************\
	*
	* Data
	*
	\**************************************************************************/
	
	/*
	 */
	string ASSET_EXTENSION = "asset";
	string ASSET_DIR = "assets/";
	string ASSET_VERSION = "1.0";
	
	/*
	 */
	string importers[] = (
		
		// Textures
		"tif"	: "Assets::Textures::import",
		"tga"	: "Assets::Textures::import",
		"jpg"	: "Assets::Textures::import",
		"png"	: "Assets::Textures::import",
		"dds"	: "Assets::Textures::import",
		"psd"	: "Assets::Textures::import",
		"ppm"	: "Assets::Textures::import",
		"pgm"	: "Assets::Textures::import",
		"hdr"	: "Assets::Textures::import",
		"sgi"	: "Assets::Textures::import",
		"rgb"	: "Assets::Textures::import",
		"rgba"	: "Assets::Textures::import",
		
		// Animation
		// TODO: path, anim
		
		// Nodes
		// TODO: world
		"node"	: "Assets::Nodes::import",
		
		// Meshes
		"mesh"	: "Assets::Meshes::import",
		
		// Libraries
		"mat"	: "Assets::Materials::import",
		// TODO: prop
		
		// Archives
		// do we need to support acrhives at all?
		// TODO: ung, zip
		
		// Fbx
		#ifdef HAS_AUTODESK_IMPORT
			"fbx"	: "Assets::Fbx::import",
		#endif
	);
	
	string assets_path;
	AssetSystem state;
	
	/**************************************************************************\
	*
	* Init / Shutdown
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		log.message("\n---- Assets ----\nVersion: %s\n\n",ASSET_VERSION);
		
		// get asset_path from command line
		forloop(int i = 0; engine.getNumArgs()) {
			if(engine.getArg(i) == "-assets_path" && i + 1 < engine.getNumArgs()) {
				assets_path = engine.getArg(i + 1);
				break;
			}
		}
		
		// use default path near data folder if no -assets_path was passed as a command line argument
		if(strlen(assets_path) == 0) {
			assets_path = pathname(engine.getDataPath() + "../" + ASSET_DIR);
		}
		
		log.message("Assets path: %s\n",assets_path);
		
		if(is_dir(assets_path) == 0) {
			mkdir(assets_path,1);
		}
		
		state = new AssetSystem();
	}
	
	void shutdown() {
		delete state;
	}
	
} /* namespace Assets::Common */

/******************************************************************************\
*
* Public Templates
*
\******************************************************************************/

/*
 */
template asset_benchmark_start<VAR> {
	float start_ ## VAR = clock();
}

template asset_benchmark_end<VAR> {
	float end_ ## VAR = clock();
	log.message("\nTotal process time: %0.2f seconds\n",end_ ## VAR - start_ ## VAR);
}

/******************************************************************************\
*
* Public Classes
*
\******************************************************************************/

/*
 */
class AssetParameter {
	
	//
	string name;
	string type;
	int value;
	
	//
	AssetParameter() {
		name = "";
		type = "";
		value = 0;
	}
	
	AssetParameter(string n,string t,int v) {
		name = n;
		type = t;
		value = v;
	}
	
	//
	int load(Xml xml) {
		
		// args
		if(xml.isArg("type")) type = xml.getArg("type");
		if(xml.isArg("name")) name = xml.getArg("name");
		
		// data
		value = xml.getData();
		
		return 1;
	}
	
	void save(Xml xml) {
		
		// args
		xml.setArg("type",type);
		xml.setArg("name",name);
		
		// data
		xml.setData(value);
	}
};

/*
 */
class AssetRuntime {
	
	//
	string name;
	string id;
	string processor;
	int library_item;
	Xml data;
	AssetParameter parameters[0];
	
	Asset asset;
	
	//
	AssetRuntime() {
		name = "";
		id = "";
		processor = "";
		library_item = 0;
		data = NULL;
		asset = NULL;
	}
	
	AssetRuntime(string n,string i,string f,int l,Xml d) {
		name = n;
		id = i;
		processor = f;
		library_item = l;
		data = NULL;
		asset = NULL;
		
		if(d != NULL) {
			data = new Xml();
			data.copy(d);
		}
	}
	
	~AssetRuntime() {
		delete data;
		asset = NULL;
	}
	
	//
	int load(Xml xml) {
		
		// args
		if(xml.isArg("name")) name = xml.getArg("name");
		if(xml.isArg("id")) id = xml.getArg("id");
		if(xml.isArg("library_item")) library_item = xml.getIntArg("library_item");
		if(xml.isArg("function")) processor = xml.getArg("function");
		
		// parameters
		Xml parameters_root = xml.find("parameters");
		if(parameters_root != NULL) {
			
			parameters.delete();
			forloop(int i = 0; parameters_root.getNumChilds()) {
				Xml parameter_root = parameters_root.getChild(i);
				
				AssetParameter parameter = new AssetParameter();
				if(parameter.load(parameter_root) == 0) {
					log.error("Assets::Asset::load(): can't load runtime parameter\n");
					delete parameter;
					return 0;
				}
				
				parameters.append(parameter);
			}
		}
		
		// data
		Xml data_root = xml.find("data");
		if(data_root != NULL) {
			delete data;
			data = new Xml();
			data.copy(data_root);
		}
		
		return 1;
	}
	
	void save(Xml xml) {
		
		// args
		xml.setArg("name",name);
		xml.setArg("id",id);
		if(library_item) xml.setArg("library_item",library_item);
		if(strlen(processor)) xml.setArg("function",processor);
		
		// parameters
		if(parameters.size() > 0) {
			Xml parameters_root = xml.addChild("parameters");
			forloop(int i = 0; parameters.size()) {
				Xml parameter_root = parameters_root.addChild("parameter");
				parameters[i].save(parameter_root);
			}
		}
		
		// data
		if(data != NULL) {
			Xml child = xml.addChild("data");
			child.copy(data);
		}
	}
	
	void addParameter(string name,string type,int value) {
		AssetParameter parameter = new AssetParameter(name,type,value);
		parameters.append(parameter);
	}
};

/*
 */
class Asset {
	
	//
	string guid;
	string hash;
	string link;
	
	string importer;
	string preprocessor;
	
	AssetParameter parameters[0];
	AssetRuntime runtimes[0];
	
	//
	~Asset() {
		guid = "";
		hash = "";
		link = "";
		
		importer = "";
		preprocessor = "";
		
		parameters.delete();
		runtimes.delete();
	}
	
	/**************************************************************************\
	*
	* Importer
	*
	\**************************************************************************/
	
	//
	AssetParameter addImportParameter(string name,string type,int value) {
		AssetParameter parameter = new AssetParameter(name,type,value);
		parameters.append(parameter);
		
		return parameter;
	}
	
	int getImportParameter(string name,int default_value) {
		forloop(int i = 0; parameters.size()) {
			AssetParameter parameter = parameters[i];
			if(parameter.name == name) return class_cast(parameter.type,parameter.value);
		}
		
		return default_value;
	}
	
	template getImportParameterType<NAME,TYPE> TYPE getImportParameter ## NAME(string name,TYPE default_value) {
		forloop(int i = 0; parameters.size()) {
			AssetParameter parameter = parameters[i];
			if(parameter.name == name && parameter.type == #TYPE) return TYPE(parameter.value);
		}
		
		return default_value;
	}
	
	getImportParameterType<Int,int>;
	getImportParameterType<Float,float>;
	getImportParameterType<String,string>;
	
	void clearImportParameters() {
		parameters.delete();
	}
	
	//
	void setImporter(string i) { importer = i; }
	string getImporter() { return importer; }
	
	/**************************************************************************\
	*
	* Preprocessor
	*
	\**************************************************************************/
	
	//
	void setPreprocessor(string p) { preprocessor = p; }
	string getPreprocessor() { return preprocessor; }
	
	/**************************************************************************\
	*
	* Runtimes
	*
	\**************************************************************************/
	
	//
	AssetRuntime addRuntime(string name,string processor,int library_item = 0,Xml data = NULL) {
		AssetRuntime runtime = new AssetRuntime(name,assetsGenerateGUID(),processor,library_item,data);
		runtime.asset = this;
		
		runtimes.append(runtime);
		
		return runtime;
	}
	
	//
	int getNumRuntimes() { return runtimes.size(); }
	
	AssetRuntime getRuntime(int index) {
		assert(index >= 0 && index < runtimes.size() && "Asset::getRuntime(): bad index");
		return runtimes[index];
	}
	
	AssetRuntime findRuntime(string name) {
		forloop(int i = 0; runtimes.size()) {
			AssetRuntime runtime = runtimes[i];
			if(runtime.name == name) return runtime;
		}
		
		return NULL;
	}
	
	/**************************************************************************\
	*
	* Load / Save
	*
	\**************************************************************************/
	
	//
	int load(Xml xml) {
		
		guid = xml.getChildData("guid","");
		hash = xml.getChildData("hash","");
		link = xml.getChildData("link","");
		
		// import
		Xml import_root = xml.find("import");
		if(import_root != NULL) {
			
			if(import_root.isArg("function")) importer = import_root.getArg("function");
			
			parameters.delete();
			forloop(int i = 0; import_root.getNumChilds()) {
				Xml parameter_root = import_root.getChild(i);
				
				AssetParameter parameter = new AssetParameter();
				if(parameter.load(parameter_root) == 0) {
					log.error("Assets::Asset::load(): can't load import parameter\n");
					delete parameter;
					return 0;
				}
				
				parameters.append(parameter);
			}
		}
		
		// preprocessor
		Xml preprocessor_root = xml.find("preprocessor");
		if(preprocessor_root != NULL) {
			if(preprocessor_root.isArg("function")) preprocessor = preprocessor_root.getArg("function");
		}
		
		// runtimes
		Xml runtimes_root = xml.find("runtimes");
		if(runtimes_root != NULL) {
			
			runtimes.delete();
			forloop(int i = 0; runtimes_root.getNumChilds()) {
				Xml runtime_root = runtimes_root.getChild(i);
				
				AssetRuntime runtime = new AssetRuntime();
				runtime.asset = this;
				
				if(runtime.load(runtime_root) == 0) {
					log.error("Assets::Asset::load(): can't load runtime\n");
					delete runtime;
					return 0;
				}
				
				runtimes.append(runtime);
			}
		}
		
		return 1;
	}
	
	void save(Xml xml) {
		
		// guid
		xml.setChildData("guid",guid);
		
		// hash
		xml.setChildData("hash",hash);
		
		// link
		if(strlen(link)) xml.setChildData("link",link);
		
		// import
		if(strlen(importer)) {
			Xml import_root = xml.addChild("import");
			import_root.setArg("function",importer);
			
			if(parameters.size() > 0) {
				forloop(int i = 0; parameters.size()) {
					Xml parameter_root = import_root.addChild("parameter");
					parameters[i].save(parameter_root);
				}
			}
		}
		
		// preprocessor
		if(strlen(preprocessor)) {
			Xml preprocessor_root = xml.addChild("preprocessor");
			preprocessor_root.setArg("function",preprocessor);
		}
		
		// runtimes
		if(runtimes.size() > 0) {
			Xml runtimes_root = xml.addChild("runtimes");
			
			forloop(int i = 0; runtimes.size()) {
				Xml runtime_root = runtimes_root.addChild("runtime");
				runtimes[i].save(runtime_root);
			}
		}
	}
};

/*
 */
class AssetSystem {
	
	//
	Asset asset_instances[];			// map<guid,Asset>
	string asset_paths[];				// map<guid,path>
	
	AssetRuntime runtime_instances[];	// map<guid,AssetRuntime>
	string runtime_paths[];				// map<guid,path>
	Unigine::Vector runtimes[];			// map<name,list<Runtime>>
	
	//
	~AssetSystem() {
		clear();
	}
	
	//
	void clear() {
		asset_instances.delete();
		asset_paths.delete();
		
		runtime_instances.delete();
		runtime_paths.delete();
		
		runtimes.delete();
	}
};

/******************************************************************************\
*
* Public API
*
\******************************************************************************/

/*
 */
string assetsGenerateHash(string path,int bytes_to_read = 1024 * 1024) {
	File file = new File(path,"rb");
	
	if(file.isOpened()) {
		int size = file.getSize();
		int num_bytes = min(size,bytes_to_read);
		file.seekSet((size - num_bytes) / 2);
		
		Blob blob = new Blob();
		file.readStream(blob,num_bytes);
		file.close();
		delete file;
		
		string hash = blob.getSHA1();
		delete blob;
		
		return hash;
	}
	
	delete file;
	return "no hash";
}

string assetsGenerateHashXml(Xml xml) {
	if(xml == NULL) return "no hash";
	
	Blob blob = new Blob();
	blob.writeString(xml.getFormattedSubTree());
	
	string hash = blob.getSHA1();
	delete blob;
	
	return hash;
}

string assetsGenerateGUID(int num_random_bytes = 20) {
	Blob blob = new Blob();
	
	forloop(int i = 0; num_random_bytes) {
		blob.writeChar(rand(0,256));
	}
	
	string guid = blob.getSHA1();
	delete blob;
	
	return guid;
}

/*
 */
void assetsSetPath(string path) {
	Assets::Common::assets_path = path;
}

string assetsGetPath() {
	return Assets::Common::assets_path;
}

/*
 */
int assetsCreateAsset(Asset asset,string file,string hash) {
	asset.hash = hash;
	asset.guid = assetsGenerateGUID();
	
	if(strlen(asset.importer) == 0) {
		string ext = lower(extension(file));
		asset.importer = assetsGetImporter(ext);
	}
	
	if(is_function(asset.importer,2) == 0) {
		log.error("assetsCreateAsset(): can't find \"%s\" import function with two arguments for \"%s\"\n",asset.importer,file);
		return 0;
	}
	
	return call(asset.importer,asset,file);
}

int assetsDeleteAsset(Asset asset,string file,int delete_runtimes = 1) {
	string asset_file = file + "." + Assets::Common::ASSET_EXTENSION;
	
	if(is_file(file) && remove(file) == 0) return 0;
	if(is_file(asset_file) && remove(asset_file) == 0) return 0;
	if(delete_runtimes && assetsDeleteRuntimes(asset,asset_file) == 0) return 0;
	
	return 1;
}

int assetsLoadAsset(Asset asset,string file) {
	Xml xml = new Xml();
	if(xml.load(file) == 0) {
		log.error("assetsLoadAsset(): can't load \"%s\" asset\n",file);
		delete xml;
		return 0;
	}
	
	if(asset.load(xml) == 0) {
		log.error("assetsLoadAsset(): can't deserialize \"%s\" asset\n",file);
		delete xml;
		return 0;
	}
	
	delete xml;
	return 1;
}

int assetsSaveAsset(Asset asset,string file) {
	Xml xml = new Xml("asset");
	xml.setArg("version",Assets::Common::ASSET_VERSION);
	
	asset.save(xml);
	
	if(xml.save(file) == 0) {
		log.error("assetsSaveAsset(): can't save \"%s\" asset\n",file);
		delete xml;
		return 0;
	}
	
	delete xml;
	return 1;
}

/*
 */
int assetsCreateRuntimes(Asset asset,string file) {
	if(strstr(file,assetsGetPath()) == -1) return 0;
	
	string source_file = (strlen(asset.link)) ? asset.link : extension(file,NULL);
	if(is_file(source_file) == 0) return 0;
	
	string destination_path = engine.getDataPath() + relname(assetsGetPath(),dirname(file));
	mkdir(destination_path,1);
	
	if(is_dir(destination_path) == 0) return 0;
	
	forloop(int i = 0; asset.runtimes.size()) {
		AssetRuntime runtime = asset.runtimes[i];
		string destination_file = destination_path + runtime.name;
		
		if(strlen(runtime.processor) == 0) continue;
		
		if(is_function(runtime.processor,4) == 0) {
			log.error("assetsCreateRuntimes(): can't find \"%s\" processor function with four arguments\n",runtime.processor);
			return 0;
		}
		
		if(call(runtime.processor,asset,runtime,source_file,destination_file) == 0) {
			log.error("assetsCreateRuntimes(): processor function \"%s\" for runtime \"%s\" returned zero\n",runtime.processor,runtime.name);
			return 0;
		}
	}
	
	return 1;
}

int assetsDeleteRuntimes(Asset asset,string file) {
	if(strstr(file,assetsGetPath()) == -1) return 0;
	string path = engine.getDataPath() + relname(assetsGetPath(),dirname(file));
	
	forloop(int i = 0; asset.runtimes.size()) {
		AssetRuntime runtime = asset.runtimes[i];
		string file = path + runtime.name;
		
		// remove file
		if(is_file(file) == 0) continue;
		if(remove(file) == 0) return 0;
		
		// remove empty directories
		Dir dir = new Dir(path);
		int num_entities = 0;
		
		if(dir.isOpened()) {
			num_entities = dir.getNumFiles() + dir.getNumDirs();
			dir.close();
		}
		
		delete dir;
		
		if(num_entities > 0) continue;
		if(rmdir(path) == 0) return 0;
	}
	
	return 1;
}

/*
 */
int assetsPreprocessAsset(Asset asset,string file) {
	if(is_function(asset.preprocessor,2) == 0) {
		log.error("assetsPreprocessAsset(): can't find \"%s\" preprocessor function with two arguments\n",asset.preprocessor);
		return 0;;
	}
	
	return call(asset.preprocessor,asset,file);
}

/*
 */
int assetsAddImporter(string ext,string function) {
	if(is_function(function,2) == 0) {
		log.error("assetsAddImporter(): can't find \"%s\" function with two arguments\n",function);
		return 0;
	}
	
	if(Assets::Common::importers.check(ext)) {
		log.error("assetsAddImporter(): already have \"%s\" function for \"%s\"\n",function,ext);
		return 0;
	}
	
	Assets::Common::importers.append(ext,function);
}

string assetsGetImporter(string ext) {
	return Assets::Common::importers.check(ext,NULL);
}

/*
 */
Asset assetsGetAsset(string guid) {
	return Assets::Common::state.asset_instances.check(guid,NULL);
}

string assetsGetAssetPath(string guid) {
	return Assets::Common::state.asset_paths.check(guid,NULL);
}

AssetRuntime assetsGetRuntime(string guid) {
	return Assets::Common::state.runtime_instances.check(guid,NULL);
}

string assetsGetRuntimePath(string guid) {
	return Assets::Common::state.runtime_paths.check(guid,NULL);
}

Unigine::Vector assetsGetRuntimes(string path) {
	return Assets::Common::state.runtimes.check(path,NULL);
}

#endif /* __EDITOR_ASSETS_COMMON_H__ */
