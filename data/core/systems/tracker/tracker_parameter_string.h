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


#ifndef __UNIGINE_TRACKER_PARAMETER_STRING_H__
#define __UNIGINE_TRACKER_PARAMETER_STRING_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	class TrackerParameterString : TrackerParameter {
		
		string filter;				// filter
		
		/*
		 */
		class Track {
			KeyString keys[0];		// keys
			float indices[0];		// indices
			int cache;				// cache
		};
		
		/*
		 */
		TrackerParameterString() : TrackerParameter(TYPE_STRING) {
			
		}
		
		/*
		 */
		string getFilter() {
			return filter;
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
			
			// filter
			if(xml.isArg("filter")) filter = xml.getArg("filter");
			
			return 1;
		}
		
		/**********************************************************************\
		*
		* Compile parameter
		*
		\**********************************************************************/
		
		/*
		 */
		string getSetSource() {
			string source = "string VALUE = Unigine::Tracker::TrackerParameterString::getTrackValue(track,time);\n";
			source += "if(strlen(VALUE) == 0) VALUE = 0;\n";
			source += set_source;
			return source;
		}
		
		string getGetSource() {
			string source = "int VALUE;\n";
			source += get_source;
			source += "Unigine::Tracker::TrackerParameterString::setTrackValue(track,time,VALUE);\n";
			return source;
		}
		
		string getBlendSource() {
			string source = "string VALUE;\n";
			source += get_source;
			source += "if(k > 0.5f) VALUE = Unigine::Tracker::TrackerParameterString::getTrackValue(track,time);\n";
			source += "if(strlen(VALUE) == 0) VALUE = 0;\n";
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
			update_track_string(track.keys,track.indices);
			
			// unused cache
			track.cache = track.cache;
		}
		
		/*
		 */
		void setTrackValue(Track track,float time,string value) {
			if(getTrackValue(track,time) != value) {
				set_track_value_string(track.keys,track.indices,time,value);
				updateTrack(track);
			}
		}
		
		string getTrackValue(Track track,float time) {
			return get_track_value_string(track.keys,track.indices,time);
		}
		
		/*
		 */
		void saveTrackValue(Stream stream,string value) {
			stream.writeString(value);
		}
		
		string restoreTrackValue(Stream stream) {
			return stream.readString();
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
			clone_track_string(t.keys,track.keys);
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
			if(load_track_string(xml,track.keys) == 0) {
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
			save_track_string(xml,track.keys);
			
			return 1;
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_PARAMETER_STRING_H__ */
