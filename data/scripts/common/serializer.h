/* Copyright (C) 2005-2015, UNIGINE Corp. All rights reserved.
 *
 * This file is a part of the UNIGINE 2.0-RC SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE Corp. at http://unigine.com/
 */


#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__

#include <core/scripts/array.h>

// utility function for argument checking
/// @todo: possible, move this function to library
void checkXmlArg(Xml node,string arg,string caller_function) {
	if(node.isArg(arg) == 0) {
		throw("%s(): Can't find node \"%s\" argument \"%s\"\n",caller_function,node.getName(),arg);
	}
}

// utility function for safe child node search
/// @todo: possible, move this function to library
Xml getXmlChildSafe(Xml node,int id,string caller_function) {
	Xml res = node.getChild(id);
	if(res == NULL) {
		throw("%s(): Can't fing node \"%s\"\n",caller_function,string(id));
	}
	return res;
}



/*
 */ 
namespace Unigine {
	
	/* meta information for FieldMeta class
	 */
	FieldMeta field_meta_metainfo[4] = (
		new FieldMeta("Name","Name"),
		new FieldMeta("Caption","Caption"),
		new FieldMeta("Editor","Editor"),
		new FieldMeta("Category","Category"),
	);
	
	/* class for property serialization
	 */
	class FieldMeta {
		private:
			string name;
			string caption;
			string editor;
			string category;
			
		public:
			
			// constructor
			FieldMeta(string name_,string caption_ = "no name",string editor_ = "",string category_ = "") {
				name = name_;
				caption = caption_;
				editor = editor_;
				category = category_;
			}
			
			// get / set setter function name
			string getName() { return name; }
			void setName(string s) { name = s; }
			
			// get / set propety caption to display
			string getCaption() { return caption; }
			void setCaption(string s) { caption = s; }
			
			// get / set property visual editor type
			string getEditor() { return editor; }
			void setEditor(string s) { editor = s; }
			
			// get / set property category
			string getCategory() { return category; }
			void setCategory(string s) { category = s; }
			
			// return property descriptions count for this class (for serialization / deserialization)
			int getNumFieldMetas() {
				return field_meta_metainfo.size();
			}
			
			// return property description by its index (for serialization / deserialization)
			FieldMeta getFieldMeta(int i) {
				return field_meta_metainfo[i];
			}
			
			// serialize object property
			void serialize(Xml xml,int object) {
				xml.setName(name);
				Unigine::serialize(xml,object.call("get" + name));
			}
			
			// deserialize object property
			void deserialize(Xml xml,int object) {
				object.call("set" + name,Unigine::deserialize(xml));
			}
	};
	
	/* serialize data to xml
	 */
	void serialize(Xml xml,int data) {
		
		string type = typeof(data);
		xml.setArg("type",type);
		
		if(type == "int") {
			xml.setIntData(data);
		} else if(type == "float") {
			xml.setFloatData(data);
		} else if(type == "vec3") {
			xml.setVec3Data(data);
		} else if(type == "vec4") {
			xml.setVec4Data(data);
		} else if(type == "mat4") {
			xml.setMat4Data(data);
		} else if(type == "quat") {
			xml.setQuatData(data);
		} else if(type == "string") {
			xml.setData(data);
		} else if(type == "Unigine::Vector") {
			
			// serialize array
			Unigine::Vector v = data;
			forloop(int i = 0; v.size()) {
				serialize(xml.addChild("element"),v[i]);
			}
			
		} else if(type == "Unigine::Map") {
			
			// serialize map
			Unigine::Map m = data;
			foreachkey(int key; m.data) {
				Xml node = xml.addChild("element");
				serialize(node.addChild("key"),key);
				serialize(node.addChild("data"),m[key]);
			}
			
		} else {
			// serialize class
			if(data != NULL) {
				
				forloop(int i = 0; data.call("getNumFieldMetas")) {
					FieldMeta d = data.call("getFieldMeta",i);
					d.serialize(xml.addChild("property"),data);
				}
				
			} else {
				xml.setArg("type","int");
				xml.setIntData(NULL);
			}
		}
	}
	
	/* deserialize data from xml
	 */
	int deserialize(Xml xml) {
		
		checkXmlArg(xml,"type","PropertyDecscription::deserialize");
		string type = xml.getArg("type");
		
		if(type == "int") return xml.getIntData();
		if(type == "float") return xml.getFloatData();
		if(type == "vec3") return xml.getVec3Data();
		if(type == "vec4") return xml.getVec4Data();
		if(type == "mat4") return xml.getMat4Data();
		if(type == "quat") return xml.getQuatData();
		if(type == "string") return xml.getData();
		
		if(type == "Unigine::Vector") {
			
			// deserialize array
			Unigine::Vector v = new Unigine::Vector();
			forloop(int i = 0; xml.getNumChilds()) {
				v.append(deserialize(xml.getChild(i)));
			}
			return v;
			
		}
		
		if(type == "Unigine::Map") {
			
			// serialize map
			Unigine::Map m = new Unigine::Map();
			forloop(int i = 0; xml.getNumChilds()) {
				Xml node = xml.getChild(i);
				
				int key = deserialize(getXmlChildSafe(node,"key","PropertyDecscription::deserialize"));
				int data = deserialize(getXmlChildSafe(node,"data","PropertyDecscription::deserialize"));
				m.append(key,data);
			}
			return m;
			
		}
		
		// deserialize class
		int data = new(type);
		Unigine::deserialize(xml,data);
		return data;
	}
	
	/* deserialize object from xml in place. Object should be 
	 * serializable user class
	 */
	void deserialize(Xml xml,int object) {
		
		// deserialize each property
		forloop(int i = 0; object.call("getNumFieldMetas")) {
			FieldMeta d = object.call("getFieldMeta",i);
			Xml node = xml.getChild(d.getName());
			if(node != NULL) {
				d.deserialize(node,object);
			}
		}
	}
	
} /* namespace Unigine */

#endif /* __SERIALIZER_H__ */
