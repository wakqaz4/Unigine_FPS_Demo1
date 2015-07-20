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


#ifndef __UNIGINE_TRACKER_EDITOR_PARAMETER_FLOAT_H__
#define __UNIGINE_TRACKER_EDITOR_PARAMETER_FLOAT_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	class TrackerEditorParameterFloat : TrackerEditorParameter {
		
		using Unigine::Widgets;
		
		TrackCurve track_curve;
		LineCurveTrack line_curve;
		
		TrackerParameterFloat::Track parameter_track;
		
		/*
		 */
		TrackerEditorParameterFloat(TrackerParameterFloat parameter,vec4 color) : TrackerEditorParameter(parameter) {
			
			parameter_track = new TrackerParameterFloat::Track();
			
			table_row = new TableRow(getParameterName(),EDITOR_TABLE_NUM_COLUMNS,1,3);
			table_row.setColor(color);
			table_row.setData(this);
			
			track_curve = new TrackCurve(color,parameter.getMinValue(),parameter.getMaxValue());
			line_curve = new LineCurveTrack(track_curve);
			track_curve.setData(line_curve);
			line_curve.setData(this);
		}
		
		/*
		 */
		void addParameter(Table table,Track track,Line line,TrackerTrack tracker_track) {
			
			add_parameter(tracker_track,parameter_track);
			
			table.addRow(table_row);
			if(table_row.getValue(EDITOR_TABLE_COLUMN_TRACK)) track.addCurve(track_curve);
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
			update_track_float(parameter_track.keys,track_curve);
			parameter.updateTrack(parameter_track);
		}
		
		/*
		 */
		void updateCurve() {
			update_curve_float(track_curve,line_curve,parameter_track.keys);
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_EDITOR_PARAMETER_FLOAT_H__ */
