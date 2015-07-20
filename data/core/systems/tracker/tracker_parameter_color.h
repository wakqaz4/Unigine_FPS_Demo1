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


#ifndef __UNIGINE_TRACKER_PARAMETER_COLOR_H__
#define __UNIGINE_TRACKER_PARAMETER_COLOR_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	class TrackerParameterColor : TrackerParameter {
		
		/*
		 */
		class Key {
			float time;				// time
			vec4 color;				// color
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
		TrackerParameterColor() : TrackerParameter(TYPE_COLOR) {
			
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
		int getPrevTrackTime(Track track) {
			return track.keys.size();
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
		void setTrackValue(Track track,float time,vec4 color) {
			
			if(getTrackValue(track,time) != color) {
				int index = track.indices.find(time);
				if(index == -1) {
					Key key = new Key();
					key.time = time;
					key.color = color;
					track.keys.append(key);
				} else {
					Key key = track.keys[index];
					key.color = color;
				}
				updateTrack(track);
			}
		}
		
		vec4 getTrackValue(Track track,float time) {
			
			Key keys[] = track.keys;
			float indices[] = track.indices;
			
			int left = indices.left(time);
			int right = indices.right(time);
			
			if(left != -1 && right != -1) {
				
				Key k0 = keys[left];
				Key k1 = keys[right];
				float t0 = k0.time;
				float t1 = k1.time;
				
				if(t0 == t1) return k0.color;
				float k = (time - t0) / (t1 - t0);
				k = k * k * (3.0f - 2.0f * k);
				
				return lerp(k0.color,k1.color,k);
			}
			
			if(left != -1) return keys[left].color;
			if(right != -1) return keys[right].color;
			
			return vec4(0.0f,0.0f,0.0f,1.0f);
		}
		
		/*
		 */
		void saveTrackValue(Stream stream,vec4 color) {
			stream.writeVec4(color);
		}
		
		vec4 restoreTrackValue(Stream stream) {
			return stream.readVec4();
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
				k.color = key.color;
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
			forloop(int i = 0; xml.getNumChilds()) {
				Xml x = xml.getChild(i);
				string name = x.getName();
				
				// key
				if(name == "key") {
					
					// key time
					if(x.isArg("time") == 0) {
						log.error("Unigine::Tracker::TrackerParameterColor::loadTrack(): can't find key time\n");
						delete track;
						return 0;
					}
					
					// create key
					Key key = new Key();
					key.time = x.getFloatArg("time");
					
					// key color
					key.color = x.getVec4Data();
					
					// append key
					track.keys.append(key);
				}
				
				else {
					log.error("Unigine::Tracker::TrackerParameterColor::loadTrack(): unknown node \"%s\"\n",name);
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
			forloop(int i = 0; track.keys.size()) {
				Xml x = xml.addChild("key");
				Key key = track.keys[i];
				
				// key time
				x.setFloatArg("time",key.time);
				
				// key color
				x.setVec4Data(key.color);
			}
			
			return 1;
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_PARAMETER_COLOR_H__ */
