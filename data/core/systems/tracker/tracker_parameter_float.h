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


#ifndef __UNIGINE_TRACKER_PARAMETER_FLOAT_H__
#define __UNIGINE_TRACKER_PARAMETER_FLOAT_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	class TrackerParameterFloat : TrackerParameter {
		
		float min_value;			// minimum value
		float max_value;			// maximum value
		
		/*
		 */
		class Track {
			KeyFloat keys[0];		// keys
			float indices[0];		// indices
			int cache;				// cache
		};
		
		/*
		 */
		TrackerParameterFloat() : TrackerParameter(TYPE_FLOAT) {
			min_value = -INFINITY;
			max_value = INFINITY;
		}
		
		/*
		 */
		float getMinValue() {
			return min_value;
		}
		
		float getMaxValue() {
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
			if(xml.isArg("min")) min_value = xml.getFloatArg("min");
			if(xml.isArg("max")) max_value = xml.getFloatArg("max");
			
			return 1;
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
			update_track_float(track.keys,track.indices);
			
			// unused cache
			track.cache = track.cache;
		}
		
		/*
		 */
		void setTrackValue(Track track,float time,float value) {
			if(getTrackValue(track,time) != value) {
				set_track_value_float(track.keys,track.indices,time,value);
				updateTrack(track);
			}
		}
		
		float getTrackValue(Track track,float time) {
			return get_track_value_float(track.keys,track.indices,time);
		}
		
		/*
		 */
		void saveTrackValue(Stream stream,float value) {
			stream.writeFloat(value);
		}
		
		float restoreTrackValue(Stream stream) {
			return stream.readFloat();
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
			clone_track_float(t.keys,track.keys);
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
			if(load_track_float(xml,track.keys) == 0) {
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
			save_track_float(xml,track.keys);
			
			return 1;
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_PARAMETER_FLOAT_H__ */
