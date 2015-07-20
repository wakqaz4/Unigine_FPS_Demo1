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


#ifndef __UNIGINE_TRACKER_EDITOR_PARAMETER_COLOR_H__
#define __UNIGINE_TRACKER_EDITOR_PARAMETER_COLOR_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	class TrackerEditorParameterColor : TrackerEditorParameter {
		
		using Unigine::Widgets;
		
		LineCurveColor line_curve;
		
		TrackerParameterColor::Track parameter_track;
		
		/*
		 */
		TrackerEditorParameterColor(TrackerParameterColor parameter) : TrackerEditorParameter(parameter) {
			
			parameter_track = new TrackerParameterColor::Track();
			
			table_row = new TableRow(getParameterName(),EDITOR_TABLE_NUM_COLUMNS,1,1);
			table_row.setData(this);
			
			line_curve = new LineCurveColor();
			line_curve.setData(this);
		}
		
		/*
		 */
		void addParameter(Table table,Track track,Line line,TrackerTrack tracker_track) {
			
			add_parameter(tracker_track,parameter_track);
			
			table.addRow(table_row);
			line.addCurve(line_curve);
		}
		
		/*
		 */
		void setTrack(int track) {
			delete parameter_track;
			parameter_track = parameter.cloneTrack(track);
		}
		
		int getTrack() {
			return parameter_track;
		}
		
		/*
		 */
		void updateTrack() {
			parameter_track.keys.delete();
			foreach(LineKeyColor key; line_curve.keys) {
				TrackerParameterColor::Key track_key = new TrackerParameterColor::Key();
				track_key.time = key.getTime();
				track_key.color = key.getColor();
				parameter_track.keys.append(track_key);
			}
			parameter.updateTrack(parameter_track);
		}
		
		/*
		 */
		void updateCurve() {
			line_curve.clear();
			foreach(TrackerParameterColor::Key key; parameter_track.keys) {
				LineKeyColor line_key = new LineKeyColor(key.time,key.color);
				line_curve.addKey(line_key);
			}
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_EDITOR_PARAMETER_COLOR_H__ */
