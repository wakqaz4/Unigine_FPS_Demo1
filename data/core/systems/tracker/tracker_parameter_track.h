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


#ifndef __UNIGINE_TRACKER_PARAMETER_TRACK_H__
#define __UNIGINE_TRACKER_PARAMETER_TRACK_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	class TrackerParameterTrack : TrackerParameter {
		
		Tracker tracker;			// tracker
		
		/*
		 */
		class Track {
			KeyString keys_n[0];	// name keys
			KeyFloat keys_t[0];		// time keys
			KeyFloat keys_w[0];		// weight keys
			float indices_n[0];		// name indices
			float indices_t[0];		// time indices
			float indices_w[0];		// weight indices
			Tracker tracker;		// tracker
			string name;			// old name
			float time;				// old time
			float weight;			// old weight
		};
		
		/*
		 */
		TrackerParameterTrack(Tracker t,string name) : TrackerParameter(TYPE_TRACK) {
			type_name = name;
			tracker = t;
		}
		
		/**********************************************************************\
		*
		* Compile parameter
		*
		\**********************************************************************/
		
		/*
		 */
		string getSetSource() {
			string name = replace(type_name,"track","Track");
			return "Unigine::Tracker::TrackerParameterTrack::blend" + name + "(track,time,1.0f);\n";
		}
		
		string getGetSource() {
			return "";
		}
		
		string getBlendSource() {
			string name = replace(type_name,"track","Track");
			return "Unigine::Tracker::TrackerParameterTrack::blend" + name + "(track,time,k);\n";
		}
		
		/*
		 */
		string getSaveSource() {
			return "";
		}
		
		string getRestoreSource() {
			return "";
		}
		
		/**********************************************************************\
		*
		* Track
		*
		\**********************************************************************/
		
		/*
		 */
		void getTrackKeys(Track track,float keys[]) {
			keys.copy(track.indices_n);
		}
		
		int getNumTrackKeys(Track track) {
			return track.keys_n.size();
		}
		
		/*
		 */
		float getPrevTrackTime(Track track,float time) {
			return get_prev_track_time(track.indices_n,time);
		}
		
		float getNextTrackTime(Track track,float time) {
			return get_next_track_time(track.indices_n,time);
		}
		
		/*
		 */
		void updateTrack(Track track) {
			
			// update track
			update_track_string(track.keys_n,track.indices_n);
			update_track_float(track.keys_t,track.indices_t);
			update_track_float(track.keys_w,track.indices_w);
			
			// tracker
			track.tracker = tracker;
		}
		
		/*
		 */
		void blendTrack(Track track,float time,float weight) {
			
			// track name
			string track_name = get_track_value_string(track.keys_n,track.indices_n,time);
			if(strlen(track_name) == 0) return;
			
			// load track
			TrackerTrack track_track = track.tracker.loadTrack(track_name);
			if(track_track == NULL) return;
			
			// blend track
			float track_time = get_track_value_float(track.keys_t,track.indices_t,time);
			float track_weight = get_track_value_float(track.keys_w,track.indices_w,time);
			track_track.blend(track_time,track_weight * weight);
		}
		
		/*
		 */
		void blendTrackDiscrete(Track track,float time,float weight) {
			
			// track name
			string track_name = get_track_value_string(track.keys_n,track.indices_n,time);
			if(strlen(track_name) == 0) return;
			
			// load track
			TrackerTrack track_track = track.tracker.loadTrack(track_name);
			if(track_track == NULL) return;
			
			// blend track
			float track_time = get_track_value_float(track.keys_t,track.indices_t,time);
			float track_weight = get_track_value_float(track.keys_w,track.indices_w,time);
			if(track.name != track_name || track.time != track_time || track.weight != track_weight) {
				track_track.blend(track_time,track_weight * weight);
				track.name = track_name;
				track.time = track_time;
				track.weight = track_weight;
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
			clone_track_string(t.keys_n,track.keys_n);
			clone_track_float(t.keys_t,track.keys_t);
			clone_track_float(t.keys_w,track.keys_w);
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
			if(xml.isChild("keys_n") && load_track_string(xml.getChild("keys_n"),track.keys_n) == 0) {
				delete track;
				return 0;
			}
			if(xml.isChild("keys_t") && load_track_float(xml.getChild("keys_t"),track.keys_t) == 0) {
				delete track;
				return 0;
			}
			if(xml.isChild("keys_w") && load_track_float(xml.getChild("keys_w"),track.keys_w) == 0) {
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
			save_track_string(xml.addChild("keys_n"),track.keys_n);
			save_track_float(xml.addChild("keys_t"),track.keys_t);
			save_track_float(xml.addChild("keys_w"),track.keys_w);
			
			return 1;
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_PARAMETER_TRACK_H__ */
