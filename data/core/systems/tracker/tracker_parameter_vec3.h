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


#ifndef __UNIGINE_TRACKER_PARAMETER_VEC3_H__
#define __UNIGINE_TRACKER_PARAMETER_VEC3_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	class TrackerParameterVec3 : TrackerParameter {
		
		vec3 min_value;				// minimum value
		vec3 max_value;				// maximum value
		
		/*
		 */
		class Track {
			KeyFloat keys_x[0];		// x keys
			KeyFloat keys_y[0];		// y keys
			KeyFloat keys_z[0];		// z keys
			float keys_t[0];		// t keys
			float indices_x[0];		// x indices
			float indices_y[0];		// y indices
			float indices_z[0];		// z indices
			float indices_t[0];		// t indices
			int cache;				// cache
		};
		
		/*
		 */
		TrackerParameterVec3() : TrackerParameter(TYPE_VEC3) {
			min_value = -vec3_infinity;
			max_value = vec3_infinity;
		}
		
		/*
		 */
		vec3 getMinValue() {
			return min_value;
		}
		
		vec3 getMaxValue() {
			return max_value;
		}
		
		/**********************************************************************\
		*
		* Load parameter
		*
		\**********************************************************************/
		
		/*
		 */
		int load(Xml xml) {
			
			if(TrackerParameter::load(xml) == 0) return 0;
			
			// value bounds
			if(xml.isArg("min")) min_value = xml.getVec3Arg("min");
			if(xml.isArg("max")) max_value = xml.getVec3Arg("max");
			
			return 1;
		}
		
		/**********************************************************************\
		*
		* Linear time
		*
		\**********************************************************************/
		
		/*
		 */
		float get_linear_time(Track track,float time) {
			
			// create parameterization
			if(track.keys_t.size() == 0) {
				
				// track keys
				float keys[0];
				float indices[];
				foreach(float time; track.indices_x) indices.append(time);
				foreach(float time; track.indices_y) indices.append(time);
				foreach(float time; track.indices_z) indices.append(time);
				foreachkey(float time; indices) keys.append(time);
				if(keys.size() == 0) return time;
				
				// track bounds
				float min_time = keys[0];
				float max_time = keys[keys.size() - 1];
				
				// analyze points
				int num_points = 32;
				float max_ratio = 1.0f;
				forloop(int i = 1; keys.size()) {
					float time_0 = keys[i - 1];
					float time_1 = keys[i];
					float min_distance = INFINITY;
					float  max_distance = -INFINITY;
					Vec3 old_point = getTrackValueScalar(track,time_0);
					forloop(int j = 1; num_points) {
						float time = time_0 + (time_1 - time_0) * j / num_points;
						Vec3 point = getTrackValueScalar(track,time);
						float distance = float(length(point - old_point));
						min_distance = min(min_distance,distance);
						max_distance = max(max_distance,distance);
						old_point = point;
					}
					max_ratio = max(max_ratio,max_distance / min_distance);
				}
				num_points = int(num_points * max_ratio);
				
				// track points
				Vec3 points[0];
				forloop(int i = 1; keys.size()) {
					float time_0 = keys[i - 1];
					float time_1 = keys[i];
					forloop(int j = 0; num_points) {
						float time = time_0 + (time_1 - time_0) * j / num_points;
						points.append(getTrackValueScalar(track,time));
					}
				}
				points.append(getTrackValueScalar(track,keys[keys.size() - 1]));
				
				// track length
				Scalar length = 0.0f;
				forloop(int i = 1; points.size()) {
					length += ::length(points[i] - points[i - 1]);
				}
				
				// empty track
				if(length < EPSILON) {
					track.keys_t.append(min_time);
					track.indices_t.append(min_time);
					track.keys_t.append(max_time);
					track.indices_t.append(max_time);
					return time;
				}
				
				// track parameterization
				Scalar distance = 0.0f;
				Vec3 old_point = points[0];
				forloop(int i = 1, j = 0; keys.size()) {
					float time_0 = keys[i - 1];
					float time_1 = keys[i];
					forloop(int k = 0; num_points) {
						distance += ::length(points[j] - old_point);
						track.keys_t.append(time_0 + (time_1 - time_0) * k / num_points);
						track.indices_t.append(min_time + (max_time - min_time) * float(distance / length));
						old_point = points[j++];
					}
				}
				track.keys_t.append(max_time);
				track.indices_t.append(max_time);
			}
			
			// linearize time
			int index = track.indices_t.left(time);
			if(index >= 0 && index + 1 < track.indices_t.size()) {
				float t0 = track.indices_t[index];
				float t1 = track.indices_t[index + 1];
				if(t0 == t1) return track.keys_t[index];
				float k0 = track.keys_t[index];
				float k1 = track.keys_t[index + 1];
				float k = (time - t0) / (t1 - t0);
				return lerp(k0,k1,k);
			}
			
			return time;
		}
		
		/**********************************************************************\
		*
		* Track
		*
		\**********************************************************************/
		
		/*
		 */
		void getTrackKeys(Track track,float keys[]) {
			keys.clear();
		}
		
		int getNumTrackKeys(Track track) {
			return track.keys_x.size() + track.keys_y.size() + track.keys_z.size();
		}
		
		/*
		 */
		float getPrevTrackTime(Track track,float time) {
			return time;
		}
		
		float getNextTrackTime(Track track,float time) {
			return time;
		}
		
		/*
		 */
		void updateTrack(Track track) {
			
			// update track
			update_track_float(track.keys_x,track.indices_x);
			update_track_float(track.keys_y,track.indices_y);
			update_track_float(track.keys_z,track.indices_z);
			
			// clear parameterization
			track.keys_t.clear();
			track.indices_t.clear();
			
			// unused cache
			track.cache = track.cache;
		}
		
		/*
		 */
		void setTrackValue(Track track,float time,vec3 value) {
			
			vec3 old_value = getTrackValue(track,time);
			
			if(old_value.x != value.x) set_track_value_float(track.keys_x,track.indices_x,time,value.x);
			if(old_value.y != value.y) set_track_value_float(track.keys_y,track.indices_y,time,value.y);
			if(old_value.z != value.z) set_track_value_float(track.keys_z,track.indices_z,time,value.z);
			
			if(old_value != value) updateTrack(track);
		}
		
		vec3 getTrackValue(Track track,float time) {
			
			vec3 value;
			
			value.x = get_track_value_float(track.keys_x,track.indices_x,time);
			value.y = get_track_value_float(track.keys_y,track.indices_y,time);
			value.z = get_track_value_float(track.keys_z,track.indices_z,time);
			
			return value;
		}
		
		dvec3 getTrackValueDouble(Track track,float time) {
			
			dvec3 value;
			
			value.x = get_track_value_double(track.keys_x,track.indices_x,time);
			value.y = get_track_value_double(track.keys_y,track.indices_y,time);
			value.z = get_track_value_double(track.keys_z,track.indices_z,time);
			
			return value;
		}
		
		Vec3 getTrackValueScalar(Track track,float time) {
			
			Vec3 value;
			
			#ifdef USE_DOUBLE
				value.x = get_track_value_double(track.keys_x,track.indices_x,time);
				value.y = get_track_value_double(track.keys_y,track.indices_y,time);
				value.z = get_track_value_double(track.keys_z,track.indices_z,time);
			#else
				value.x = get_track_value_float(track.keys_x,track.indices_x,time);
				value.y = get_track_value_float(track.keys_y,track.indices_y,time);
				value.z = get_track_value_float(track.keys_z,track.indices_z,time);
			#endif
			
			return value;
		}
		
		/*
		 */
		void setTrackValueLinear(Track track,float time,vec3 value) {
			time = get_linear_time(track,time);
			setTrackValue(track,time,value);
		}
		
		vec3 getTrackValueLinear(Track track,float time) {
			time = get_linear_time(track,time);
			return getTrackValue(track,time);
		}
		
		dvec3 getTrackValueLinearDouble(Track track,float time) {
			time = get_linear_time(track,time);
			return getTrackValueDouble(track,time);
		}
		
		Vec3 getTrackValueLinearScalar(Track track,float time) {
			time = get_linear_time(track,time);
			return getTrackValueScalar(track,time);
		}
		
		/*
		 */
		void saveTrackValue(Stream stream,vec3 value) {
			stream.writeVariable(value);
		}
		
		vec3 restoreTrackValue(Stream stream) {
			return stream.readVariable();
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
			clone_track_float(t.keys_x,track.keys_x);
			clone_track_float(t.keys_y,track.keys_y);
			clone_track_float(t.keys_z,track.keys_z);
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
			if(xml.isChild("keys_x") && load_track_float(xml.getChild("keys_x"),track.keys_x) == 0) {
				delete track;
				return 0;
			}
			if(xml.isChild("keys_y") && load_track_float(xml.getChild("keys_y"),track.keys_y) == 0) {
				delete track;
				return 0;
			}
			if(xml.isChild("keys_z") && load_track_float(xml.getChild("keys_z"),track.keys_z) == 0) {
				delete track;
				return 0;
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
			save_track_float(xml.addChild("keys_x"),track.keys_x);
			save_track_float(xml.addChild("keys_y"),track.keys_y);
			save_track_float(xml.addChild("keys_z"),track.keys_z);
			
			return 1;
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_PARAMETER_VEC3_H__ */
