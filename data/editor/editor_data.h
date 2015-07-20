/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_config.h
 * Desc:    Unigine editor
 * Version: 1.02
 * Author:  Alexey Zagniy <yingaz@unigine.com>
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
namespace Data {
	
	/*
	 */
	Xml xml;
	
	/*
	 */
	void init() {
		if(engine.world.isLoaded() == false) return;
		if(xml != NULL) delete xml;
		// load data
		string data = engine.editor.getData();
		
		xml = new Xml();
		if(xml.parse(data)) {
			if(xml.getName() != "settings") delete xml;
		}
		if(xml == NULL) xml = new Xml("settings");
	}
	
	void shutdown() {
		delete xml;
	}
	
	void world_save() {
		if(engine.world.isLoaded() == false) return;
		if(xml == NULL) return;
		engine.editor.setData(xml.getSubTree());
	}
	
	/*
	 */
	void set(string name,int value) {
		if(xml == NULL) return;
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
		else throw(__FUNC__ + ": unknown value type\n");
	}
	
	int get(string name,int value) {
		if(xml == NULL) return value;
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
		throw(__FUNC__ + ": unknown value type\n");
	}
	
	int is_item(string name) {
		if(xml == NULL) return false;
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
void dataInit() {
	Data::init();
}

void dataShutdown() {
	Data::shutdown();
}

void dataWorldSave() {
	Data::world_save();
}

/*
 */
void dataSet(string name,int value) {
	Data::set(name,value);
}

int dataGet(string name,int value) {
	return Data::get(name,value);
}

int dataIsItem(string name) {
	return Data::is_item(name);
}
