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


#ifndef __UNIGINE_TRACKER_TRACK_H__
#define __UNIGINE_TRACKER_TRACK_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	class TrackerTrack {
		
		Tracker tracker;				// tracker
		
		float min_time;					// minimum time
		float max_time;					// maximum time
		float unit_time;				// unit time
		
		TrackerParameter parameters[0];	// parameters
		
		int parameter_tracks[0];		// tracks
		int parameter_objects[0];		// objects
		int parameter_arguments[0];		// arguments
		
		/*
		 */
		TrackerTrack(Tracker t) {
			
			tracker = t;
			
			clear();
		}
		
		~TrackerTrack() {
			
			clear();
		}
		
		/*
		 */
		void clear() {
			
			min_time = 0.0f;
			max_time = 1.0f;
			unit_time = 1.0f;
			
			parameters.clear();
			
			parameter_tracks.delete();
			parameter_objects.clear();
			parameter_arguments.clear();
		}
		
		/*
		 */
		void setMinTime(float time) {
			min_time = time;
		}
		
		float getMinTime() {
			return min_time;
		}
		
		/*
		 */
		void setMaxTime(float time) {
			max_time = time;
		}
		
		float getMaxTime() {
			return max_time;
		}
		
		/*
		 */
		void setUnitTime(float time) {
			unit_time = max(time,EPSILON);
		}
		
		float getUnitTime() {
			return unit_time;
		}
		
		/*
		 */
		void getNumParameters() {
			return parameters.size();
		}
		
		TrackerParameter getParameter(int num) {
			return parameters[num];
		}
		
		/*
		 */
		void setParameterObject(int num,int object) {
			parameter_objects[num] = object;
		}
		
		int getParameterObject(int num) {
			return parameter_objects[num];
		}
		
		/*
		 */
		void setParameterArgument(int num,int argument) {
			parameter_arguments[num] = argument;
		}
		
		int getParameterArgument(int num) {
			return parameter_arguments[num];
		}
		
		/*
		 */
		void getParameterKeys(int num,float keys[]) {
			parameters[num].getTrackKeys(parameter_tracks[num],keys);
		}
		
		int getNumParameterKeys(int num) {
			return parameters[num].getNumTrackKeys(parameter_tracks[num]);
		}
		
		float getPrevParameterTime(int num,float time) {
			return parameters[num].getPrevTrackTime(parameter_tracks[num],time);
		}
		
		float getNextParameterTime(int num,float time) {
			return parameters[num].getNextTrackTime(parameter_tracks[num],time);
		}
		
		/**********************************************************************\
		*
		* Clone track
		*
		\**********************************************************************/
		
		/*
		 */
		TrackerTrack clone() {
			
			TrackerTrack track = new TrackerTrack(tracker);
			
			// copy parameters
			track.setMinTime(getMinTime());
			track.setMaxTime(getMaxTime());
			track.setUnitTime(getUnitTime());
			
			// clone parameters
			forloop(int i = 0; parameters.size()) {
				
				// clone parameter
				TrackerParameter parameter = parameters[i];
				int parameter_track = parameter.cloneTrack(parameter_tracks[i]);
				int parameter_object = parameter_objects[i];
				int parameter_argument = parameter_arguments[i];
				
				// add parameter
				track.parameters.append(parameter);
				track.parameter_tracks.append(parameter_track);
				track.parameter_objects.append(parameter_object);
				track.parameter_arguments.append(parameter_argument);
			}
			
			return track;
		}
		
		/**********************************************************************\
		*
		* Load/save track
		*
		\**********************************************************************/
		
		/*
		 */
		int load(string name) {
			
			clear();
			
			Xml xml = new Xml();
			
			// load script
			if(xml.load(name) == 0) {
				log.error("Unigine::Tracker::TrackerTrack::load(): can't load \"%s\" file\n",name);
				delete xml;
				return 0;
			}
			
			// check script version
			if(xml.getName() != "track" || xml.isArg("version") == 0 || strlen(xml.getArg("version")) == 0) {
				log.error("Unigine::Tracker::TrackerTrack::load(): bad track file \"%s\"\n",name);
				delete xml;
				return 0;
			}
			
			// track parameters
			if(xml.isArg("min_time")) min_time = xml.getFloatArg("min_time");
			if(xml.isArg("max_time")) max_time = xml.getFloatArg("max_time");
			if(xml.isArg("unit_time")) unit_time = xml.getFloatArg("unit_time");
			
			// load track
			forloop(int i = 0; xml.getNumChilds()) {
				Xml x = xml.getChild(i);
				
				// parameter
				if(x.getName() == "parameter") {
					
					// type name
					if(x.isArg("type") == 0) {
						log.error("Unigine::Tracker::TrackerTrack::load(): can't find parameter type\n");
						continue;
					}
					string type_name = x.getArg("type");
					
					// find parameter
					TrackerParameter parameter = tracker.findParameter(type_name);
					if(parameter == NULL) {
						log.error("Unigine::Tracker::TrackerTrack::load(): can't find \"%s\" parameter\n",type_name);
						continue;
					}
					
					// load track
					int parameter_track = NULL;
					int parameter_object = NULL;
					int parameter_argument = NULL;
					if(parameter.loadTrack(x,parameter_track,parameter_object,parameter_argument) == 0) {
						log.error("Unigine::Tracker::TrackerTrack::load(): can't load \"%s\" track from \"%s\" file\n",parameter.getTypeName(),name);
						continue;
					}
					
					// add parameter
					parameters.append(parameter);
					parameter_tracks.append(parameter_track);
					parameter_objects.append(parameter_object);
					parameter_arguments.append(parameter_argument);
				}
				
				else {
					log.error("Unigine::Tracker::TrackerTrack::load(): unknown node \"%s\" in \"%s\" file\n",x.getName(),name);
					continue;
				}
			}
			
			// print unused data
			xml.printUnusedData(name);
			delete xml;
			
			// compile parameters
			tracker.compileParameters(parameters);
			
			init();
			
			return 1;
		}
		
		/*
		 */
		int save(string name) {
			
			Xml xml = new Xml("track","version=\"1.00\"");
			
			// track parameters
			xml.setFloatArg("min_time",min_time);
			xml.setFloatArg("max_time",max_time);
			xml.setFloatArg("unit_time",unit_time);
			
			// parameters
			forloop(int i = 0; parameters.size()) {
				
				TrackerParameter parameter = parameters[i];
				
				// parameter
				Xml x = xml.addChild("parameter");
				x.setArg("type",parameter.getTypeName());
				
				// save track
				int parameter_track = parameter_tracks[i];
				int parameter_object = parameter_objects[i];
				int parameter_argument = parameter_arguments[i];
				if(parameter.saveTrack(x,parameter_track,parameter_object,parameter_argument) == 0) {
					log.error("Unigine::Tracker::TrackerTrack::save(): can't save \"%s\" track\n",parameter.getTypeName());
					delete xml;
					return 0;
				}
			}
			
			// save track
			if(xml.save(name) == 0) {
				log.error("Unigine::Tracker::TrackerTrack::save(): can't save \"%s\" file\n",name);
				delete xml;
				return 0;
			}
			
			delete xml;
			return 1;
		}
		
		/**********************************************************************\
		*
		* Save/restore state
		*
		\**********************************************************************/
		
		/*
		 */
		void __save__(Stream stream) {
			
			// save parameters
			forloop(int i = 0; parameters.size()) {
				TrackerParameter parameter = parameters[i];
				parameter.saveState(stream,parameter_objects[i]);
			}
		}
		
		/*
		 */
		void __restore__(Stream stream) {
			
			// restore parameters
			forloop(int i = 0; parameters.size()) {
				TrackerParameter parameter = parameters[i];
				int parameter_object = parameter_objects[i];
				parameter.restoreState(stream,parameter_object);
				parameter_objects[i] = parameter_object;
			}
		}
		
		/**********************************************************************\
		*
		* Track
		*
		\**********************************************************************/
		
		/*
		 */
		void init() {
			
			// initialize parameters
			forloop(int i = 0; parameters.size()) {
				TrackerParameter parameter = parameters[i];
				if(parameter.init_id == -1) continue;
				int track = parameter_tracks[i];
				int object = parameter_objects[i];
				int argument = parameter_arguments[i];
				call(parameter.init_id,object,track,argument);
			}
		}
		
		/*
		 */
		void set(float time) {
			
			// set parameters
			forloop(int i = 0; parameters.size()) {
				TrackerParameter parameter = parameters[i];
				int track = parameter_tracks[i];
				int object = parameter_objects[i];
				call(parameter.set_id,object,track,time);
			}
		}
		
		/*
		 */
		void get(float time) {
			
			// get parameters
			forloop(int i = 0; parameters.size()) {
				TrackerParameter parameter = parameters[i];
				int track = parameter_tracks[i];
				int object = parameter_objects[i];
				call(parameter.get_id,object,track,time);
			}
		}
		
		/*
		 */
		void blend(float time,float weight) {
			
			// check weight
			if(weight < EPSILON) return;
			
			// blend parameter
			int frame = engine.game.getFrame();
			forloop(int i = 0; parameters.size()) {
				TrackerParameter parameter = parameters[i];
				int track = parameter_tracks[i];
				int object = parameter_objects[i];
				int argument = parameter_arguments[i];
				float k = parameter.getWeight(object,argument,frame,weight);
				call(parameter.blend_id,object,track,time,k);
			}
		}
		
		/**********************************************************************\
		*
		* Save/restore track
		*
		\**********************************************************************/
		
		/*
		 */
		int saveState(Stream stream) {
			
			// save parameters
			forloop(int i = 0; parameters.size()) {
				TrackerParameter parameter = parameters[i];
				int track = parameter_tracks[i];
				int object = parameter_objects[i];
				stream.writeInt(i);
				call(parameter.save_id,object,track,stream);
			}
			
			return 1;
		}
		
		/*
		 */
		int restoreState(Stream stream) {
			
			// restore parameters
			forloop(int i = 0; parameters.size()) {
				TrackerParameter parameter = parameters[i];
				int track = parameter_tracks[i];
				int object = parameter_objects[i];
				if(stream.readInt() != i) return 0;
				call(parameter.restore_id,object,track,stream);
			}
			
			return 1;
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_TRACK_H__ */
