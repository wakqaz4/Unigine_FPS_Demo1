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


#ifndef __UNIGINE_TRACKER_PARAMETER_ENVIRONMENT_H__
#define __UNIGINE_TRACKER_PARAMETER_ENVIRONMENT_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	class TrackerParameterEnvironment : TrackerParameter {
		
		/*
		 */
		class Key {
			float time;				// time
			string name;			// texture name
			vec3 coefficients[9];	// coefficients
		};
		
		/*
		 */
		class Track {
			Key keys[0];			// keys
			float indices[0];		// indices
			int cache;				// cache
		};
		
		/*
		 */
		TrackerParameterEnvironment() : TrackerParameter(TYPE_ENVIRONMENT) {
			
		}
		
		/**********************************************************************\
		*
		* Compile parameter
		*
		\**********************************************************************/
		
		/*
		 */
		string getSetSource() {
			string source = "vec3 VALUE[9];\n";
			source += "Unigine::Tracker::TrackerParameterEnvironment::getTrackValue(track,time,VALUE);\n";
			source += set_source;
			return source;
		}
		
		string getGetSource() {
			string source = "vec3 VALUE[9];\n";
			source += get_source;
			source += "Unigine::Tracker::TrackerParameterEnvironment::setTrackValue(track,time,VALUE);\n";
			return source;
		}
		
		string getBlendSource() {
			string source = "vec3 VALUE[9],coefficients[9];\n";
			source += get_source;
			source += "Unigine::Tracker::TrackerParameterEnvironment::getTrackValue(track,time,coefficients);\n";
			source += "VALUE.lerp(VALUE,coefficients,k);\n";
			source += set_source;
			return source;
		}
		
		/*
		 */
		string getSaveSource() {
			string source = "vec3 VALUE[9];\n";
			source += get_source;
			source += "Unigine::Tracker::TrackerParameterEnvironment::saveTrackValue(stream,VALUE);\n";
			return source;
		}
		
		string getRestoreSource() {
			string source = "vec3 VALUE[9];\n";
			source += "Unigine::Tracker::TrackerParameterEnvironment::restoreTrackValue(stream,VALUE);\n";
			source += set_source;
			return source;
		}
		
		/**********************************************************************\
		*
		* Track
		*
		\**********************************************************************/
		
		/*
		 */
		void getTrackKeys(Track track,float keys[]) {
			keys.copy(track.indices);
		}
		
		int getNumTrackKeys(Track track) {
			return track.keys.size();
		}
		
		/*
		 */
		float getPrevTrackTime(Track track,float time) {
			return get_prev_track_time(track.indices,time);
		}
		
		float getNextTrackTime(Track track,float time) {
			return get_next_track_time(track.indices,time);
		}
		
		/*
		 */
		void updateTrack(Track track) {
			
			// update track
			track.indices.clear();
			foreach(Key key; track.keys) {
				track.indices.append(key.time);
			}
			track.indices.sort(track.keys);
			
			// unused cache
			track.cache = track.cache;
		}
		
		/*
		 */
		void setTrackValue(Track track,float time,vec3 coefficients[]) {
			
			vec3 current_coefficients[9];
			getTrackValue(track,time,current_coefficients);
			
			if(coefficients.compare(current_coefficients) == 0) {
				int index = track.indices.find(time);
				if(index == -1) {
					Key key = new Key();
					key.time = time;
					key.coefficients.copy(coefficients);
					track.keys.append(key);
				} else {
					Key key = track.keys[index];
					key.coefficients.copy(coefficients);
				}
				updateTrack(track);
			}
		}
		
		void getTrackValue(Track track,float time,vec3 coefficients[]) {
			
			Key keys[] = track.keys;
			float indices[] = track.indices;
			
			int left = indices.left(time);
			int right = indices.right(time);
			
			if(left != -1 && right != -1) {
				
				Key k0 = keys[left];
				Key k1 = keys[right];
				float t0 = k0.time;
				float t1 = k1.time;
				
				if(t0 == t1) return coefficients.copy(k0.coefficients);
				float k = (time - t0) / (t1 - t0);
				k = k * k * (3.0f - 2.0f * k);
				
				return coefficients.lerp(k0.coefficients,k1.coefficients,k);
			}
			
			if(left != -1) return coefficients.copy(keys[left].coefficients);
			if(right != -1) return coefficients.copy(keys[right].coefficients);
			
			forloop(int i = 0; coefficients.size()) {
				coefficients[i] = vec3_zero;
			}
		}
		
		/*
		 */
		void saveTrackValue(Stream stream,vec3 coefficients[]) {
			forloop(int i = 0; coefficients.size()) {
				stream.writeVec3(coefficients[i]);
			}
		}
		
		void restoreTrackValue(Stream stream,vec3 coefficients[]) {
			forloop(int i = 0; coefficients.size()) {
				coefficients[i] = stream.readVec3();
			}
		}
		
		/**********************************************************************\
		*
		* Clone track
		*
		\**********************************************************************/
		
		/*
		 */
		Track cloneTrack(Track track) {
			
			Track t = new Track();
			
			// clone track
			foreach(Key key; track.keys) {
				Key k = new Key();
				k.time = key.time;
				k.name = key.name;
				k.coefficients.copy(key.coefficients);
				t.keys.append(k);
			}
			updateTrack(t);
			
			return t;
		}
		
		/**********************************************************************\
		*
		* Load/save track
		*
		\**********************************************************************/
		
		/*
		 */
		int loadTrack(Xml xml,Track &track,int &object,int &argument) {
			
			// load track
			track = new Track();
			float coefficients[27];
			forloop(int i = 0; xml.getNumChilds()) {
				Xml x = xml.getChild(i);
				string name = x.getName();
				
				// key
				if(name == "key") {
					
					// key time
					if(x.isArg("time") == 0) {
						log.error("Unigine::Tracker::TrackerParameterEnvironment::loadTrack(): can't find key time\n");
						delete track;
						return 0;
					}
					
					// create key
					Key key = new Key();
					key.time = x.getFloatArg("time");
					
					// key name
					if(x.isArg("name")) key.name = x.getArg("name");
					
					// key coefficients
					x.getFloatArrayData(coefficients);
					for(int j = 0, k = 0; j < 9; j++, k += 3) {
						key.coefficients[j].x = coefficients[k + 0];
						key.coefficients[j].y = coefficients[k + 1];
						key.coefficients[j].z = coefficients[k + 2];
					}
					
					// append key
					track.keys.append(key);
				}
				
				else {
					log.error("Unigine::Tracker::TrackerParameterEnvironment::loadTrack(): unknown node \"%s\"\n",name);
					continue;
				}
			}
			updateTrack(track);
			
			if(TrackerParameter::loadTrack(xml,track,object,argument) == 0) {
				delete track;
				return 0;
			}
			
			return 1;
		}
		
		/*
		 */
		int saveTrack(Xml xml,Track track,int object,int argument) {
			
			if(TrackerParameter::saveTrack(xml,track,object,argument) == 0) return 0;
			
			// save track
			float coefficients[27];
			forloop(int i = 0; track.keys.size()) {
				Xml x = xml.addChild("key");
				Key key = track.keys[i];
				
				// key time
				x.setFloatArg("time",key.time);
				
				// key name
				if(strlen(key.name)) x.setArg("name",key.name);
				
				// key coefficients
				for(int j = 0, k = 0; j < 9; j++, k += 3) {
					coefficients[k + 0] = key.coefficients[j].x;
					coefficients[k + 1] = key.coefficients[j].y;
					coefficients[k + 2] = key.coefficients[j].z;
				}
				x.setFloatArrayData(coefficients);
			}
			
			return 1;
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_PARAMETER_ENVIRONMENT_H__ */
