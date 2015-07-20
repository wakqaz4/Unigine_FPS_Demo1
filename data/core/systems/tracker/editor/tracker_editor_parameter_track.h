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


#ifndef __UNIGINE_TRACKER_EDITOR_PARAMETER_TRACK_H__
#define __UNIGINE_TRACKER_EDITOR_PARAMETER_TRACK_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	class TrackerEditorParameterTrack : TrackerEditorParameter {
		
		using Unigine::Widgets;
		
		LineCurveFile line_curve_n;
		
		TableRow table_row_t;
		TrackCurve track_curve_t;
		LineCurveTrack line_curve_t;
		
		TableRow table_row_w;
		TrackCurve track_curve_w;
		LineCurveTrack line_curve_w;
		
		TrackerParameterTrack::Track parameter_track;
		
		/*
		 */
		TrackerEditorParameterTrack(TrackerParameterTrack parameter,vec4 color) : TrackerEditorParameter(parameter) {
			
			parameter_track = new TrackerParameterTrack::Track();
			
			table_row = new TableRow(getParameterName(),EDITOR_TABLE_NUM_COLUMNS,1,1);
			table_row.setColor(color);
			table_row.setData(this);
			
			line_curve_n = new LineCurveFile(color,".track");
			line_curve_n.setData(this);
			
			table_row_t = new TableRow("",EDITOR_TABLE_NUM_COLUMNS,0,2);
			table_row_t.setOffset(EDITOR_TABLE_OFFSET);
			table_row_t.setSwizzle(TR("Time"));
			table_row_t.setColor(color);
			table_row_t.setData(this);
			
			track_curve_t = new TrackCurve(color,);
			line_curve_t = new LineCurveTrack(track_curve_t);
			track_curve_t.setData(line_curve_t);
			line_curve_t.setData(this);
			
			table_row_w = new TableRow("",EDITOR_TABLE_NUM_COLUMNS,0,2);
			table_row_w.setOffset(EDITOR_TABLE_OFFSET);
			table_row_w.setSwizzle(TR("Weight"));
			table_row_w.setColor(color);
			table_row_w.setData(this);
			
			track_curve_w = new TrackCurve(color,0.0f,INFINITY);
			line_curve_w = new LineCurveTrack(track_curve_w);
			track_curve_w.setData(line_curve_w);
			line_curve_w.setData(this);
		}
		
		/*
		 */
		void addParameter(Table table,Track track,Line line,TrackerTrack tracker_track) {
			
			add_parameter(tracker_track,parameter_track);
			
			table.addRow(table_row);
			line.addCurve(line_curve_n);
			
			table.addRow(table_row_t);
			if(table_row_t.getValue(EDITOR_TABLE_COLUMN_TRACK)) track.addCurve(track_curve_t);
			line.addCurve(line_curve_t);
			
			table.addRow(table_row_w);
			if(table_row_w.getValue(EDITOR_TABLE_COLUMN_TRACK)) track.addCurve(track_curve_w);
			line.addCurve(line_curve_w);
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
			parameter_track.keys_n.delete();
			foreach(LineKeyFile key; line_curve_n.keys) {
				TrackerParameter::KeyString track_key = new TrackerParameter::KeyString();
				track_key.time = key.getTime();
				track_key.value = key.getName();
				parameter_track.keys_n.append(track_key);
			}
			update_track_float(parameter_track.keys_t,track_curve_t);
			update_track_float(parameter_track.keys_w,track_curve_w);
			parameter.updateTrack(parameter_track);
		}
		
		/*
		 */
		void updateCurve() {
			line_curve_n.clear();
			foreach(TrackerParameter::KeyString key; parameter_track.keys_n) {
				LineKeyFile line_key = new LineKeyFile(key.time,(strlen(key.value) > 0),key.value);
				line_curve_n.addKey(line_key);
			}
			update_curve_float(track_curve_t,line_curve_t,parameter_track.keys_t);
			update_curve_float(track_curve_w,line_curve_w,parameter_track.keys_w);
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_EDITOR_PARAMETER_TRACK_H__ */
