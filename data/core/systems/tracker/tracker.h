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


#ifndef __UNIGINE_TRACKER_H__
#define __UNIGINE_TRACKER_H__

#include <core/systems/tracker/tracker_parameter.h>
#include <core/systems/tracker/tracker_parameter_track.h>
#include <core/systems/tracker/tracker_parameter_float.h>
#include <core/systems/tracker/tracker_parameter_vec3.h>
#include <core/systems/tracker/tracker_parameter_color.h>
#include <core/systems/tracker/tracker_parameter_environment.h>
#include <core/systems/tracker/tracker_parameter_toggle.h>
#include <core/systems/tracker/tracker_parameter_switch.h>
#include <core/systems/tracker/tracker_parameter_string.h>
#include <core/systems/tracker/tracker_parameter_node.h>
#include <core/systems/tracker/tracker_track.h>

/*
 */
namespace Unigine::Tracker {
	
	/**************************************************************************\
	*
	* Tracker
	*
	\**************************************************************************/
	
	/*
	 */
	class Tracker {
		
		int flags;							// flags
		
		TrackerParameter parameters[0];		// parameters
		TrackerTrack tracks[];				// tracks
		
		Expression expressions[0];			// expressions
		
		/*
		 */
		Tracker(int flags_ = 0) {
			
			// tracker flags
			flags = flags_;
			
			// load node parameters
			if(loadParameters("core/systems/tracker/parameters/node.parameters") == 0) {
				throw("Unigine::Tracker::Tracker::Tracker(): can't load node parameters\n");
			}
			
			// load material parameters
			if(loadParameters("core/systems/tracker/parameters/material.parameters") == 0) {
				throw("Unigine::Tracker::Tracker::Tracker(): can't load material parameters\n");
			}
			
			// load property parameters
			if(loadParameters("core/systems/tracker/parameters/property.parameters") == 0) {
				throw("Unigine::Tracker::Tracker::Tracker(): can't load property parameters\n");
			}
			
			// load render parameters
			if(loadParameters("core/systems/tracker/parameters/render.parameters") == 0) {
				throw("Unigine::Tracker::Tracker::Tracker(): can't load render parameters\n");
			}
			
			// load sound parameters
			if(loadParameters("core/systems/tracker/parameters/sound.parameters") == 0) {
				throw("Unigine::Tracker::Tracker::Tracker(): can't load sound parameters\n");
			}
			
			// load game parameters
			if(loadParameters("core/systems/tracker/parameters/game.parameters") == 0) {
				throw("Unigine::Tracker::Tracker::Tracker(): can't load game parameters\n");
			}
			
			// track parameters
			addParameter(new TrackerParameterTrack(this,"track"));
			addParameter(new TrackerParameterTrack(this,"trackDiscrete"));
		}
		
		~Tracker() {
			
			parameters.delete();
			tracks.delete();
			
			expressions.delete();
		}
		
		/*
		 */
		int getNumParameters() {
			return parameters.size();
		}
		
		TrackerParameter getParameter(int num) {
			return parameters[num];
		}
		
		/*
		 */
		int addParameter(TrackerParameter parameter) {
			if(findParameter(parameter.getTypeName()) != NULL) {
				log.error("Unigine::Tracker::Tracker::addParameter(): \"%s\" parameter is already defined\n",parameter.getTypeName());
				return 0;
			}
			parameters.append(parameter);
			return 1;
		}
		
		TrackerParameter findParameter(string type_name) {
			foreach(TrackerParameter parameter; parameters) {
				if(parameter.getTypeName() == type_name) return parameter;
			}
			return NULL;
		}
		
		/**********************************************************************\
		*
		* Load parameters
		*
		\**********************************************************************/
		
		/*
		 */
		int loadParameters(string name) {
			
			Xml xml = new Xml();
			
			// load parameter
			if(xml.load(name) == 0) {
				log.error("Unigine::Tracker::Tracker::loadParameters(): can't open \"%s\" file\n",name);
				delete xml;
				return 0;
			}
			
			// check parameters version
			if(xml.getName() != "parameters" || xml.isArg("version") == 0 || strlen(xml.getArg("version")) == 0) {
				log.error("Unigine::Tracker::Tracker::loadParameters(): bad parameters file \"%s\"\n",name);
				delete xml;
				return 0;
			}
			
			// load parameters
			forloop(int i = 0; xml.getNumChilds()) {
				Xml x = xml.getChild(i);
				
				// parameter
				if(x.getName() == "parameter") {
					
					// track
					if(x.isArg("track") == 0) {
						log.error("Unigine::Tracker::Tracker::loadParameters(): can't find parameter track\n");
						delete xml;
						return 0;
					}
					string track = x.getArg("track");
					
					// create parameter
					TrackerParameter parameter = NULL;
					if(track == "float") parameter = new TrackerParameterFloat();
					else if(track == "vec3") parameter = new TrackerParameterVec3();
					else if(track == "color") parameter = new TrackerParameterColor();
					else if(track == "environment") parameter = new TrackerParameterEnvironment();
					else if(track == "toggle") parameter = new TrackerParameterToggle();
					else if(track == "switch") parameter = new TrackerParameterSwitch();
					else if(track == "string") parameter = new TrackerParameterString();
					else if(track == "node") parameter = new TrackerParameterNode();
					else {
						log.error("Unigine::Tracker::Tracker::loadParameters(): unknown track \"%s\"\n",track);
						delete xml;
						return 0;
					}
					
					// load parameter
					if(parameter.load(x) == 0) {
						log.error("Unigine::Tracker::Tracker::loadParameters(): can't load parameter from \"%s\" file\n",name);
						delete parameter;
						delete xml;
						return 0;
					}
					
					// add parameter
					if(addParameter(parameter) == 0) {
						log.error("Unigine::Tracker::Tracker::loadParameters(): can't add parameter\n");
						delete parameter;
						delete xml;
						return 0;
					}
				}
				
				else {
					log.error("Unigine::Tracker::Tracker::loadParameters(): unknown node \"%s\" in \"%s\" file\n",x.getName(),name);
					continue;
				}
			}
			
			// print unused data
			xml.printUnusedData(name);
			delete xml;
			
			return 1;
		}
		
		/**********************************************************************\
		*
		* Compile parameters
		*
		\**********************************************************************/
		
		/*
		 */
		int compileParameters(TrackerParameter parameters[]) {
			
			// empty parameters
			int indices[0];
			forloop(int i = 0; parameters.size()) {
				if(parameters[i].set_id != -1) continue;
				indices.append(i);
			}
			if(indices.size() == 0) return 1;
			
			string source = "{\n";
			
			// double types
			#ifdef USE_DOUBLE
				source += "#define Scalar double\n";
				source += "#define Vec3 dvec3\n";
				source += "#define Vec4 dvec4\n";
				source += "#define Mat4 dmat4\n";
			#else
				source += "#define Scalar float\n";
				source += "#define Vec3 vec3\n";
				source += "#define Vec4 vec4\n";
				source += "#define Mat4 mat4\n";
			#endif
			
			// parameter sources
			forloop(int i = 0; indices.size()) {
				source += format("namespace p%d {\n",i);
				source += parameters[indices[i]].getSource(flags);
				source += "}\n";
			}
			
			source += "}\n";
			
			// create expression
			Expression expression = new Expression(source,1);
			if(expression.isCompiled() == NULL) {
				log.error("Unigine::Tracker::Tracker::compileParameters(): can't compile parameters\n");
				delete expression;
				source = 0;
				return 0;
			}
			expressions.append(expression);
			
			// get functions
			forloop(int i = 0; indices.size()) {
				string prefix = format("p%d::",i);
				TrackerParameter parameter = parameters[indices[i]];
				if(expression.isFunction(prefix + "init",3)) parameter.init_id = expression.getFunction(prefix + "init",3);
				parameter.set_id = expression.getFunction(prefix + "set",3);
				parameter.get_id = expression.getFunction(prefix + "get",3);
				parameter.blend_id = expression.getFunction(prefix + "blend",4);
				if(flags & TRACKER_SAVE_RESTORE) {
					parameter.save_id = expression.getFunction(prefix + "save",3);
					parameter.restore_id = expression.getFunction(prefix + "restore",3);
				}
			}
			
			return 1;
		}
		
		/**********************************************************************\
		*
		* Load track
		*
		\**********************************************************************/
		
		/*
		 */
		TrackerTrack loadTrack(string name) {
			TrackerTrack track = tracks.check(name,NULL);
			if(track == NULL) {
				track = new TrackerTrack(this);
				if(track.load(name) == 0) {
					log.error("Unigine::Tracker::Tracker::loadTrack(): can't load \"%s\" track\n",name);
					delete track;
					return NULL;
				}
				tracks.append(name,track);
			}
			return track;
		}
		
		/*
		 */
		void reloadTrack(string name) {
			TrackerTrack track = tracks.check(name,NULL);
			if(track != NULL && track.load(name) == 0) {
				log.error("Unigine::Tracker::Tracker::reloadTrack(): can't reload \"%s\" track\n",name);
			}
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_H__ */
