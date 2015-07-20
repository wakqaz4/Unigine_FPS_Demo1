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


#ifndef __UNIGINE_TRACKER_PARAMETER_H__
#define __UNIGINE_TRACKER_PARAMETER_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	enum {
		TRACKER_SAVE_RESTORE = 1 << 0,
		TRACKER_CHECK_OBJECTS = 1 << 1,
	};
	
	/*
	 */
	enum {
		KEY_CONSTANT = 0,			// constant key
		KEY_LINEAR,					// linear key
		KEY_SMOOTH,					// smooth key
		KEY_BREAK,					// break key
		KEY_AUTO,					// auto key
	};
	
	enum {
		TYPE_TRACK = 0,				// track parameter
		TYPE_FLOAT,					// float parameter
		TYPE_VEC3,					// vec3 parameter
		TYPE_COLOR,					// color parameter
		TYPE_ENVIRONMENT,			// environment parameter
		TYPE_TOGGLE,				// toggle parameter
		TYPE_SWITCH,				// switch parameter
		TYPE_STRING,				// string parameter
		TYPE_NODE,					// node parameter
	};
	
	/**************************************************************************\
	*
	* TrackerParameter
	*
	\**************************************************************************/
	
	/*
	 */
	class TrackerParameter {
		
		int type_id;				// type id
		
		string type_name;			// type name
		string object_type;			// object type
		string argument_type;		// argument type
		
		string common_source;		// common source
		string init_source;			// init source
		string set_source;			// set source
		string get_source;			// get source
		
		int init_id;				// init function id
		int set_id;					// set function id
		int get_id;					// get function id
		int blend_id;				// blend function id
		int save_id;				// save function id
		int restore_id;				// restore function id
		
		/*
		 */
		class Blend {
			int frame;				// frame number
			float weight;			// current weight
		};
		
		class BlendArguments {
			Blend arguments[];		// argument blending
		};
		
		int current_frame;			// frame number
		float current_weight;		// current weight
		int current_objects[];		// object blending
		
		/*
		 */
		class KeyInt {
			float time;				// time
			int value;				// value
		};
		
		class KeyString {
			float time;				// time
			string value;			// value
		};
		
		class KeyFloat {
			float time;				// time
			float value;			// value
			vec4 handles;			// handles
			int type;				// type
		};
		
		/*
		 */
		TrackerParameter(int id) {
			
			type_id = id;
			
			init_id = -1;
			set_id = -1;
			get_id = -1;
			blend_id = -1;
			save_id = -1;
			restore_id = -1;
			
			current_frame = -1;
			current_weight = 0.0f;
		}
		
		/*
		 */
		int getTypeID() {
			return type_id;
		}
		
		/*
		 */
		string getTypeName() {
			return type_name;
		}
		
		string getObjectType() {
			return object_type;
		}
		
		string getArgumentType() {
			return argument_type;
		}
		
		/**********************************************************************\
		*
		* Load parameter
		*
		\**********************************************************************/
		
		/*
		 */
		int load(Xml xml) {
			
			// type name
			if(xml.isArg("type") == 0) {
				log.error("Unigine::Tracker::TrackerParameter::load(): can't find parameter type\n");
				return 0;
			}
			type_name = xml.getArg("type");
			
			// object type
			if(xml.isArg("object")) object_type = xml.getArg("object");
			
			// argument type
			if(xml.isArg("argument")) argument_type = xml.getArg("argument");
			
			// load parameter
			forloop(int i = 0; xml.getNumChilds()) {
				Xml x = xml.getChild(i);
				string name = x.getName();
				
				// source code
				if(name == "common") common_source = x.getData();
				else if(name == "init") init_source = x.getData();
				else if(name == "set") set_source = x.getData();
				else if(name == "get") get_source = x.getData();
				
				else {
					log.error("Unigine::Tracker::TrackerParameter::load(): unknown node \"%s\"\n",name);
					continue;
				}
			}
			
			return 1;
		}
		
		/**********************************************************************\
		*
		* Compile parameter
		*
		\**********************************************************************/
		
		/*
		 */
		int is_node(string object) {
			if(object == "int") return 0;
			if(strstr(object,"Node") == 0) return 1;
			if(strstr(object,"World") == 0) return 1;
			if(strstr(object,"Field") == 0) return 1;
			if(strstr(object,"Light") == 0) return 1;
			if(strstr(object,"Decal") == 0) return 1;
			if(strstr(object,"Object") == 0) return 1;
			if(strstr(object,"Player") == 0) return 1;
			if(strstr(object,"Physical") == 0) return 1;
			if(strstr(object,"Navigation") == 0) return 1;
			if(strstr(object,"Obstacle") == 0) return 1;
			if(strstr(object,"Sound") == 0) return 1;
			return 0;
		}
		
		/*
		 */
		string getSetSource() {
			if(strstr(set_source,"VALUE") != -1) {
				string source = format("int VALUE = %s::getTrackValue(track,time);\n",typeof(this));
				source += set_source;
				return source;
			}
			return set_source;
		}
		
		string getGetSource() {
			if(strstr(get_source,"VALUE") != -1) {
				string source = "int VALUE;\n";
				source += get_source;
				source += format("%s::setTrackValue(track,time,VALUE);\n",typeof(this));
				return source;
			}
			return get_source;
		}
		
		string getBlendSource() {
			if(strstr(set_source,"VALUE") != -1 && strstr(get_source,"VALUE") != -1) {
				string source = "int VALUE;\n";
				source += get_source;
				source += format("VALUE = lerp(VALUE,%s::getTrackValue(track,time),k);\n",typeof(this));
				source += set_source;
				return source;
			}
			return getSetSource();
		}
		
		/*
		 */
		string getSaveSource() {
			if(strstr(set_source,"VALUE") != -1 && strstr(get_source,"VALUE") != -1) {
				string source = "int VALUE;\n";
				source += get_source;
				source += format("%s::saveTrackValue(stream,VALUE);\n",typeof(this));
				return source;
			}
			return "";
		}
		
		string getRestoreSource() {
			if(strstr(set_source,"VALUE") != -1 && strstr(get_source,"VALUE") != -1) {
				string source = format("int VALUE = %s::restoreTrackValue(stream);\n",typeof(this));
				source += set_source;
				return source;
			}
			return "";
		}
		
		/*
		 */
		string getSource(int flags) {
			
			// object type
			string object = "int";
			if(strlen(object_type)) object = object_type;
			
			// argument type
			string argument = "int";
			if(strlen(argument_type)) argument = argument_type;
			
			// check object source
			string get_check_source(string source) {
				if(flags & TRACKER_CHECK_OBJECTS) {
					if(is_node(object)) return "if(engine.world.isNode(OBJECT)) {\n" + source + "} else { log.error(\"Unigine::Tracker::TrackerParameter(): node is not exist\\n\"); }";
				}
				return source;
			}
			
			// source
			string source = common_source;
			
			if(strlen(init_source)) {
				
				// init source
				source += format("void init(%s OBJECT,int track,%s ARGUMENT) {\n",object,argument);
				source += "int CACHE;\n";
				source += get_check_source(init_source);
				source += format("%s::Track(track).cache = CACHE;\n",typeof(this));
				source += "}\n";
				
				// set source
				string set_source = getSetSource();
				source += format("void set(%s OBJECT,int track,float time) {\n",object);
				if(strstr(set_source,"CACHE") != -1) source += format("int CACHE = %s::Track(track).cache;\n",typeof(this));
				source += get_check_source(set_source);
				source += "}\n";
				
				// get source
				string get_source = getGetSource();
				source += format("void get(%s OBJECT,int track,float time) {\n",object);
				if(strstr(get_source,"CACHE") != -1) source += format("int CACHE = %s::Track(track).cache;\n",typeof(this));
				source += get_check_source(get_source);
				source += "}\n";
				
				// blend source
				string blend_source = getBlendSource();
				source += format("void blend(%s OBJECT,int track,float time,float k) {\n",object);
				if(strstr(blend_source,"CACHE") != -1) source += format("int CACHE = %s::Track(track).cache;\n",typeof(this));
				source += get_check_source(blend_source);
				source += "}\n";
				
				// save/restore source
				if(flags & TRACKER_SAVE_RESTORE) {
					
					// save source
					string save_source = getSaveSource();
					source += format("void save(%s OBJECT,int track,Stream stream) {\n",object);
					if(strstr(save_source,"CACHE") != -1) source += format("int CACHE = %s::Track(track).cache;\n",typeof(this));
					source += get_check_source(save_source);
					source += "}\n";
					
					// restore source
					string restore_source = getRestoreSource();
					source += format("void restore(%s OBJECT,int track,Stream stream) {\n",object);
					if(strstr(restore_source,"CACHE") != -1) source += format("int CACHE = %s::Track(track).cache;\n",typeof(this));
					source += get_check_source(restore_source);
					source += "}\n";
				}
			}
			else {
				
				// set source
				source += format("void set(%s OBJECT,int track,float time) {\n",object);
				source += get_check_source(getSetSource());
				source += "}\n";
				
				// get source
				source += format("void get(%s OBJECT,int track,float time) {\n",object);
				source += get_check_source(getGetSource());
				source += "}\n";
				
				// blend source
				source += format("void blend(%s OBJECT,int track,float time,float k) {\n",object);
				source += get_check_source(getBlendSource());
				source += "}\n";
				
				// save/restore source
				if(flags & TRACKER_SAVE_RESTORE) {
					
					// save source
					source += format("void save(%s OBJECT,int track,Stream stream) {\n",object);
					source += get_check_source(getSaveSource());
					source += "}\n";
					
					// restore source
					source += format("void restore(%s OBJECT,int track,Stream stream) {\n",object);
					source += get_check_source(getRestoreSource());
					source += "}\n";
				}
			}
			
			return source;
		}
		
		/**********************************************************************\
		*
		* Parameter weight
		*
		\**********************************************************************/
		
		/*
		 */
		float getWeight(int object,int argument,int frame,float weight) {
			
			// function weight
			if(strlen(object_type) == 0) {
				if(current_frame != frame) {
					current_frame = frame;
					current_weight = weight;
					return 1.0f;
				}
				current_weight += weight;
				return weight / current_weight;
			}
			
			// objects weight
			if(strlen(argument_type) == 0) {
				Blend blend = current_objects.check(object,NULL);
				if(blend == NULL) {
					blend = new Blend();
					blend.frame = frame;
					blend.weight = weight;
					current_objects.append(object,blend);
					return 1.0f;
				}
				if(blend.frame != frame) {
					blend.frame = frame;
					blend.weight = weight;
					return 1.0f;
				}
				blend.weight += weight;
				return weight / blend.weight;
			}
			
			// arguments weight
			BlendArguments blend_arguments = current_objects.check(object,NULL);
			if(blend_arguments == NULL) {
				blend_arguments = new BlendArguments();
				current_objects.append(object,blend_arguments);
			}
			Blend blend = blend_arguments.arguments.check(argument,NULL);
			if(blend == NULL) {
				blend = new Blend();
				blend.frame = frame;
				blend.weight = weight;
				blend_arguments.arguments.append(argument,blend);
				return 1.0f;
			}
			if(blend.frame != frame) {
				blend.frame = frame;
				blend.weight = weight;
				return 1.0f;
			}
			blend.weight += weight;
			return weight / blend.weight;
		}
		
		/**********************************************************************\
		*
		* Track
		*
		\**********************************************************************/
		
		/*
		 */
		void getTrackKeys(int track,float keys[]) {
			throw("Unigine::Tracker::TrackerParameter::getTrackKeys(): called\n");
		}
		
		int getNumTrackKeys(int track) {
			throw("Unigine::Tracker::TrackerParameter::getNumTrackKeys(): called\n");
		}
		
		/*
		 */
		float getPrevTrackTime(int track,float time) {
			throw("Unigine::Tracker::TrackerParameter::getPrevTrackTime(): called\n");
		}
		
		float getNextTrackTime(int track,float time) {
			throw("Unigine::Tracker::TrackerParameter::getNextTrackTime(): called\n");
		}
		
		/*
		 */
		float get_prev_track_time(float indices[],float time) {
			int index = indices.left(time);
			if(index != -1) return indices[index];
			return time;
		}
		
		float get_next_track_time(float indices[],float time) {
			int index = indices.right(time);
			if(index != -1) return indices[index];
			return time;
		}
		
		/*
		 */
		void updateTrack(int track) {
			throw("Unigine::Tracker::TrackerParameter::updateTrack(): called\n");
		}
		
		/*
		 */
		void update_track_int(KeyInt keys[],float indices[]) {
			indices.clear();
			foreach(KeyInt key; keys) {
				indices.append(key.time);
			}
			indices.sort(keys);
		}
		
		void update_track_string(KeyString keys[],float indices[]) {
			indices.clear();
			foreach(KeyString key; keys) {
				indices.append(key.time);
			}
			indices.sort(keys);
		}
		
		void update_track_float(KeyFloat keys[],float indices[]) {
			indices.clear();
			foreach(KeyFloat key; keys) {
				indices.append(key.time);
			}
			indices.sort(keys);
		}
		
		/*
		 */
		void set_track_value_int(KeyInt keys[],float indices[],float time,int value) {
			int index = indices.find(time);
			if(index == -1) {
				KeyInt key = new KeyInt();
				key.time = time;
				key.value = value;
				keys.append(key);
			} else {
				KeyInt key = keys[index];
				key.value = value;
			}
		}
		
		void set_track_value_string(KeyString keys[],float indices[],float time,string value) {
			int index = indices.find(time);
			if(index == -1) {
				KeyString key = new KeyString();
				key.time = time;
				key.value = value;
				keys.append(key);
			} else {
				KeyString key = keys[index];
				key.value = value;
			}
		}
		
		void set_track_value_float(KeyFloat keys[],float indices[],float time,float value) {
			int index = indices.find(time);
			if(index == -1) {
				KeyFloat key = new KeyFloat();
				key.time = time;
				key.value = value;
				index = indices.left(time);
				if(index == -1) key.type = KEY_LINEAR;
				else key.type = keys[index].type;
				keys.append(key);
			} else {
				KeyFloat key = keys[index];
				key.value = value;
			}
		}
		
		/*
		 */
		int get_track_value_int(KeyInt keys[],float indices[],float time) {
			
			int left = indices.left(time);
			if(left != -1) return keys[left].value;
			
			int right = indices.right(time);
			if(right != -1) return keys[right].value;
			
			return 0;
		}
		
		string get_track_value_string(KeyString keys[],float indices[],float time) {
			
			int left = indices.left(time);
			if(left != -1) return keys[left].value;
			
			int right = indices.right(time);
			if(right != -1) return keys[right].value;
			
			return "";
		}
		
		float get_track_value_float(KeyFloat keys[],float indices[],float time) {
			
			int left = indices.left(time);
			int right = indices.right(time);
			
			if(left != -1 && right != -1) {
				
				KeyFloat k0 = keys[left];
				KeyFloat k1 = keys[right];
				float t0 = k0.time;
				float t1 = k1.time;
				
				if(t0 == t1) return k0.value;
				
				switch(k0.type) {
					case KEY_CONSTANT:
						return k0.value;
					case KEY_LINEAR:
						float k = (time - t0) / (t1 - t0);
						return lerp(k0.value,k1.value,k);
					case KEY_SMOOTH:
					case KEY_BREAK:
					case KEY_AUTO:
						vec4 h0 = k0.handles;
						vec4 h1 = k1.handles;
						vec4 t = vec4(t0,clamp(t0 + h0.z,t0,t1),clamp(t1 + h1.x,t0,t1),t1);
						vec4 v = vec4(k0.value,k0.value + h0.w,k1.value + h1.y,k1.value);
						return bezier(t,v,time);
				}
			}
			
			if(left != -1) return keys[left].value;
			if(right != -1) return keys[right].value;
			
			return 0.0f;
		}
		
		double get_track_value_double(KeyFloat keys[],float indices[],float time) {
			
			int left = indices.left(time);
			int right = indices.right(time);
			
			if(left != -1 && right != -1) {
				
				KeyFloat k0 = keys[left];
				KeyFloat k1 = keys[right];
				float t0 = k0.time;
				float t1 = k1.time;
				
				double v0 = double(k0.value);
				double v1 = double(k1.value);
				
				if(t0 == t1) return v0;
				
				switch(k0.type) {
					case KEY_CONSTANT:
						return v0;
					case KEY_LINEAR:
						float k = (time - t0) / (t1 - t0);
						return lerp(v0,v1,k);
					case KEY_SMOOTH:
					case KEY_BREAK:
					case KEY_AUTO:
						vec4 h0 = k0.handles;
						vec4 h1 = k1.handles;
						vec4 t = vec4(t0,clamp(t0 + h0.z,t0,t1),clamp(t1 + h1.x,t0,t1),t1);
						dvec4 v = dvec4(v0,v0 + h0.w,v1 + h1.y,v1);
						return bezier(t,v,time);
				}
			}
			
			if(left != -1) return double(keys[left].value);
			if(right != -1) return double(keys[right].value);
			
			return 0.0;
		}
		
		/**********************************************************************\
		*
		* Clone track
		*
		\**********************************************************************/
		
		/*
		 */
		int cloneTrack(int track) {
			throw("Unigine::Tracker::TrackerParameter::cloneTrack(): called\n");
		}
		
		/*
		 */
		void clone_track_int(KeyInt dest[],KeyInt src[]) {
			foreach(KeyInt key; src) {
				KeyInt k = new KeyInt();
				k.time = key.time;
				k.value = key.value;
				dest.append(k);
			}
		}
		
		void clone_track_float(KeyFloat dest[],KeyFloat src[]) {
			foreach(KeyFloat key; src) {
				KeyFloat k = new KeyFloat();
				k.time = key.time;
				k.value = key.value;
				k.handles = key.handles;
				k.type = key.type;
				dest.append(k);
			}
		}
		
		void clone_track_string(KeyString dest[],KeyString src[]) {
			foreach(KeyString key; src) {
				KeyString k = new KeyString();
				k.time = key.time;
				k.value = key.value;
				dest.append(k);
			}
		}
		
		/**********************************************************************\
		*
		* Load/save track
		*
		\**********************************************************************/
		
		/*
		 */
		int loadTrack(Xml xml,int &track,int &object,int &argument) {
			
			// object
			if(strlen(object_type)) {
				
				// material
				if(object_type == "Material") {
					
					// material name
					if(xml.isArg("name") == 0) {
						log.error("Unigine::Tracker::TrackerParameter::loadTrack(): can't find material name\n");
						return 0;
					}
					string name = xml.getArg("name");
					
					// find material by name
					Material material = engine.materials.findMaterial(name);
					if(material == NULL) {
						log.error("Unigine::Tracker::TrackerParameter::loadTrack(): can't find \"%s\" material\n",name);
						return 0;
					}
					
					object = material;
				}
				
				// property
				else if(object_type == "Property") {
					
					// property name
					if(xml.isArg("name") == 0) {
						log.error("Unigine::Tracker::TrackerParameter::loadTrack(): can't find property name\n");
						return 0;
					}
					string name = xml.getArg("name");
					
					// find property by name
					Property property = engine.properties.findProperty(name);
					if(property == NULL) {
						log.error("Unigine::Tracker::TrackerParameter::loadTrack(): can't find \"%s\" property\n",name);
						return 0;
					}
					
					object = property;
				}
				
				// node
				else {
					
					Node node = NULL;
					
					// get node by identifier
					if(xml.isArg("id")) {
						int id = xml.getIntArg("id");
						if(engine.world.isNode(id)) {
							node = node_cast(engine.world.getNode(id));
							
							// check node name
							if(xml.isArg("name")) {
								string name = xml.getArg("name");
								if(node.getName() != name) {
									if(node.getType() == NODE_REFERENCE) {
										
										// get child node by name
										Node get_child_node(Node node) {
											if(node.getName() == name) return node;
											if(node.getType() == NODE_REFERENCE) {
												Node reference = NodeReference(node).getNode();
												if(reference != NULL) {
													reference = node_cast(reference);
													Node ret = get_child_node(reference);
													if(ret != NULL) return ret;
												}
											}
											forloop(int i = 0; node.getNumChilds()) {
												Node child = node_cast(node.getChild(i));
												Node ret = get_child_node(child);
												if(ret != NULL) return ret;
											}
											return NULL;
										}
										
										node = get_child_node(node);
									}
									else {
										node = NULL;
									}
								}
							}
						}
					}
					
					// get node by name
					if(node == NULL && xml.isArg("name")) {
						string name = xml.getArg("name");
						node = engine.editor.getNodeByName(name);
						if(node == NULL) {
							log.error("Unigine::Tracker::TrackerParameter::loadTrack(): can't find \"%s\" node\n",name);
							return 0;
						}
						node = node_cast(node);
					}
					
					// check node type
					if(is_base_class(object_type,node) == 0) {
						log.error("Unigine::Tracker::TrackerParameter::loadTrack(): bad node type \"%s\" for \"%s\" parameter\n",node.getTypeName(),object_type);
						return 0;
					}
					
					object = node;
				}
			}
			
			// argument
			if(strlen(argument_type)) {
				
				// check argument
				if(xml.isArg(argument_type) == 0) {
					log.error("Unigine::Tracker::TrackerParameter::loadTrack(): can't find \"%s\" argument\n",argument_type);
					return 0;
				}
				
				// load argument
				if(argument_type == "int") argument = xml.getIntArg("int");
				else if(argument_type == "float") argument = xml.getFloatArg("float");
				else if(argument_type == "vec3") argument = xml.getVec3Arg("vec3");
				else if(argument_type == "vec4") argument = xml.getVec4Arg("vec4");
				else if(argument_type == "mat4") argument = xml.getMat4Arg("mat4");
				else if(argument_type == "quat") argument = xml.getQuatArg("quat");
				else if(argument_type == "string") argument = xml.getArg("string");
				else throw("Unigine::Tracker::TrackerParameter::loadTrack(): unknown type of argument %s\n",argument_type);
			}
			
			return 1;
		}
		
		/*
		 */
		int load_track_float(Xml xml,KeyFloat keys[]) {
			
			forloop(int i = 0; xml.getNumChilds()) {
				Xml x = xml.getChild(i);
				string name = x.getName();
				
				// key
				if(name == "key") {
					
					// key time
					if(x.isArg("time") == 0) {
						log.error("Unigine::Tracker::TrackerParameter::load_track_float(): can't find key time\n");
						return 0;
					}
					
					// create key
					KeyFloat key = new KeyFloat();
					key.time = x.getFloatArg("time");
					
					// key type
					if(x.isArg("type")) key.type = x.getIntArg("type");
					
					// key handles
					if(x.isArg("handles")) key.handles = x.getVec4Arg("handles");
					
					// key value
					key.value = x.getFloatData();
					
					// append key
					keys.append(key);
				}
				
				else {
					log.error("Unigine::Tracker::TrackerParameter::load_track_float(): unknown node \"%s\"\n",name);
					continue;
				}
			}
			
			return 1;
		}
		
		int load_track_string(Xml xml,KeyString keys[]) {
			
			forloop(int i = 0; xml.getNumChilds()) {
				Xml x = xml.getChild(i);
				string name = x.getName();
				
				// key
				if(name == "key") {
					
					// key time
					if(x.isArg("time") == 0) {
						log.error("Unigine::Tracker::TrackerParameter::load_track_string(): can't find key time\n");
						return 0;
					}
					
					// create key
					KeyString key = new KeyString();
					key.time = x.getFloatArg("time");
					
					// key value
					key.value = x.getData();
					
					// append key
					keys.append(key);
				}
				
				else {
					log.error("Unigine::Tracker::TrackerParameter::load_track_string(): unknown node \"%s\"\n",name);
					continue;
				}
			}
			
			return 1;
		}
		
		/*
		 */
		int saveTrack(Xml xml,int track,int object,int argument) {
			
			// object
			if(strlen(object_type)) {
				
				// check object type
				if(is_base_class(object_type,object) == 0) {
					log.error("Unigine::Tracker::TrackerParameter::saveTrack(): bad object type \"%s\" for \"%s\" parameter\n",typeof(object),object_type);
					return 0;
				}
				
				// material
				if(object_type == "Material") {
					Material material = object;
					xml.setArg("name",material.getName());
				}
				// property
				else if(object_type == "Property") {
					Property property = object;
					xml.setArg("name",property.getName());
				}
				// node
				else {
					
					Node node = object;
					
					// target node isn't editor node
					if(engine.editor.isNode(node) == 0) {
						
						// check node childs
						int is_parent_node(Node node) {
							if(node == object) return 1;
							if(node.getType() == NODE_REFERENCE) {
								Node reference = NodeReference(node).getNode();
								if(reference != NULL) {
									reference = node_cast(reference);
									if(is_parent_node(reference)) return 1;
								}
							}
							forloop(int i = 0; node.getNumChilds()) {
								Node child = node_cast(node.getChild(i));
								if(engine.editor.isNode(child)) continue;
								if(is_parent_node(child)) return 1;
							}
							return 0;
						}
						
						// process editor node references
						forloop(int i = 0; engine.editor.getNumNodes()) {
							Node node = node_cast(engine.editor.getNode(i));
							if(node.getType() != NODE_REFERENCE) continue;
							if(is_parent_node(node) == 0) continue;
							xml.setIntArg("id",node.getID());
							break;
						}
					}
					// target node is editor node
					else {
						xml.setIntArg("id",node.getID());
					}
					
					// node name
					if(strlen(node.getName())) xml.setArg("name",node.getName());
				}
			}
			
			// argument
			if(strlen(argument_type)) {
				
				// save argument
				if(argument_type == "int") xml.setIntArg("int",argument);
				else if(argument_type == "float") xml.setFloatArg("float",argument);
				else if(argument_type == "vec3") xml.setVec3Arg("vec3",argument);
				else if(argument_type == "vec4") xml.setVec4Arg("vec4",argument);
				else if(argument_type == "mat4") xml.setMat4Arg("mat4",argument);
				else if(argument_type == "quat") xml.setQuatArg("quat",argument);
				else if(argument_type == "string") xml.setArg("string",argument);
				else throw("Unigine::Tracker::TrackerParameter::saveTrack(): unknown type of argument %s\n",argument_type);
			}
			
			return 1;
		}
		
		/*
		 */
		int save_track_float(Xml xml,KeyFloat keys[]) {
			
			forloop(int i = 0; keys.size()) {
				Xml x = xml.addChild("key");
				KeyFloat key = keys[i];
				
				// key time
				x.setFloatArg("time",key.time);
				
				// key type
				if(key.type != 0) x.setIntArg("type",key.type);
				
				// key handles
				if(key.handles != vec4_zero) x.setVec4Arg("handles",key.handles);
				
				// key value
				x.setFloatData(key.value);
			}
			
			return 1;
		}
		
		int save_track_string(Xml xml,KeyString keys[]) {
			
			forloop(int i = 0; keys.size()) {
				Xml x = xml.addChild("key");
				KeyString key = keys[i];
				
				// key time
				x.setFloatArg("time",key.time);
				
				// key value
				x.setData(key.value);
			}
			
			return 1;
		}
		
		/**********************************************************************\
		*
		* Save/restore state
		*
		\**********************************************************************/
		
		/*
		 */
		int saveState(Stream stream,int object) {
			
			// object
			if(strlen(object_type)) {
				
				// check object type
				if(is_base_class(object_type,object) == 0) {
					throw("Unigine::Tracker::TrackerParameter::saveState(): bad object type \"%s\" for \"%s\" parameter\n",typeof(object),object_type);
				}
				
				// material
				if(object_type == "Material") {
					Material material = object;
					stream.writeString(material.getName());
				}
				// property
				else if(object_type == "Property") {
					Property property = object;
					stream.writeString(property.getName());
				}
			}
		}
		
		/*
		 */
		int restoreState(Stream stream,int &object) {
			
			// object
			if(strlen(object_type)) {
				
				// material
				if(object_type == "Material") {
					string name = stream.readString();
					Material material = engine.materials.findMaterial(name);
					if(material == NULL) {
						throw("Unigine::Tracker::TrackerParameter::restoreState(): can't find \"%s\" material\n",name);
					}
					object = material;
				}
				
				// property
				else if(object_type == "Property") {
					string name = stream.readString();
					Property property = engine.properties.findProperty(name);
					if(property == NULL) {
						throw("Unigine::Tracker::TrackerParameter::restoreState(): can't find \"%s\" property\n",name);
					}
					object = property;
				}
			}
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_PARAMETER_H__ */
