/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_config.h
 * Desc:    Unigine editor
 * Version: 1.02
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
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
namespace Config {
	
	/*
	 */
	Xml xml;
	string name = "unigine_editor.cfg";
	
	/*
	 */
	void init() {
		
		// parse command line
		forloop(int i = 0; engine.getNumArgs()) {
			if(engine.getArg(i) == "-editor_config" && i + 1 < engine.getNumArgs()) {
				name = engine.getArg(i + 1);
				break;
			}
		}
		
		// load config
		File file = new File();
		if(file.open(engine.getAppPath() + name,"rb")) {
			file.close();
			delete file;
			xml = new Xml();
			if(xml.load(engine.getAppPath() + name) == 0) {
				delete xml;
				xml = new Xml("config","version=\"1.00\"");
			}
		} else {
			xml = new Xml("config","version=\"1.00\"");
		}
	}
	
	void shutdown() {
		
		// save config
		if(xml.save(engine.getAppPath() + name) == 0) {
			log.error("Config::shutdown(): can't save config\n");
		}
		delete xml;
	}
	
	/*
	 */
	void set(string name,int value) {
		Xml x = xml.find(format("item name=\"%s\"",name));
		if(x == NULL) x = xml.addChild("item",format("name=\"%s\"",name));
		if(is_int(value)) x.setIntData(value);
		else if(is_float(value)) x.setFloatData(value);
		else if(is_double(value)) x.setDoubleData(value);
		else if(is_vec3(value)) x.setVec3Data(value);
		else if(is_vec4(value)) x.setVec4Data(value);
		else if(is_dvec3(value)) x.setDVec3Data(value);
		else if(is_dvec4(value)) x.setDVec4Data(value);
		else if(is_mat4(value)) x.setMat4Data(value);
		else if(is_quat(value)) x.setQuatData(value);
		else if(is_string(value)) x.setData(value);
		else throw("Config::set(): unknown value type\n");
	}
	
	int get(string name,int value) {
		Xml x = xml.find(format("item name=\"%s\"",name));
		if(x == NULL) return value;
		if(is_int(value)) return x.getIntData();
		if(is_float(value)) return x.getFloatData();
		if(is_double(value)) return x.getDoubleData();
		if(is_vec3(value)) return Vec3(x.getVec3Data());
		if(is_vec4(value)) return Vec4(x.getVec4Data());
		if(is_dvec3(value)) return Vec3(x.getDVec3Data());
		if(is_dvec4(value)) return Vec4(x.getDVec4Data());
		if(is_mat4(value)) return x.getMat4Data();
		if(is_quat(value)) return x.getQuatData();
		if(is_string(value)) return x.getData();
		throw("Config::get(): unknown value type\n");
	}
	
	int is_item(string name) {
		if(name == "") return false;
		return (xml.find(format("item name=\"%s\"",name)) != NULL);
	}
}

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void configInit() {
	Config::init();
}

void configShutdown() {
	Config::shutdown();
}

/*
 */
void configSet(string name,int value) {
	Config::set(name,value);
}

int configGet(string name,int value) {
	return Config::get(name,value);
}

int configIsItem(string name) {
	return Config::is_item(name);
}
