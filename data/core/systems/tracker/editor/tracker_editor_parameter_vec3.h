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


#ifndef __UNIGINE_TRACKER_EDITOR_PARAMETER_VEC3_H__
#define __UNIGINE_TRACKER_EDITOR_PARAMETER_VEC3_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	class TrackerEditorParameterVec3 : TrackerEditorParameter {
		
		using Unigine::Widgets;
		
		TrackCurve track_curve_x;
		LineCurveTrack line_curve_x;
		
		TableRow table_row_y;
		TrackCurve track_curve_y;
		LineCurveTrack line_curve_y;
		
		TableRow table_row_z;
		TrackCurve track_curve_z;
		LineCurveTrack line_curve_z;
		
		TrackerParameterVec3::Track parameter_track;
		
		vec4 color;
		
		float keys[0];
		vec3 values[0];
		
		WidgetManipulatorTranslator keys_m[0];
		
		/*
		 */
		TrackerEditorParameterVec3(TrackerParameterVec3 parameter,vec4 c) : TrackerEditorParameter(parameter) {
			
			vec3 min_value = parameter.getMinValue();
			vec3 max_value = parameter.getMaxValue();
			
			color = c;
			
			parameter_track = new TrackerParameterVec3::Track();
			
			table_row = new TableRow(getParameterName(),EDITOR_TABLE_NUM_COLUMNS,1,3);
			table_row.setSwizzle("X");
			table_row.setColor(color);
			table_row.setData(this);
			
			track_curve_x = new TrackCurve(color * vec4(1.0f,0.0f,0.0f,1.0f),min_value.x,max_value.x);
			line_curve_x = new LineCurveTrack(track_curve_x);
			track_curve_x.setData(line_curve_x);
			line_curve_x.setData(this);
			
			table_row_y = new TableRow("",EDITOR_TABLE_NUM_COLUMNS,0,2);
			table_row_y.setOffset(EDITOR_TABLE_OFFSET);
			table_row_y.setSwizzle("Y");
			table_row_y.setColor(color);
			table_row_y.setData(this);
			
			track_curve_y = new TrackCurve(color * vec4(0.0f,1.0f,0.0f,1.0f),min_value.y,max_value.y);
			line_curve_y = new LineCurveTrack(track_curve_y);
			track_curve_y.setData(line_curve_y);
			line_curve_y.setData(this);
			
			table_row_z = new TableRow("",EDITOR_TABLE_NUM_COLUMNS,0,2);
			table_row_z.setOffset(EDITOR_TABLE_OFFSET);
			table_row_z.setSwizzle("Z");
			table_row_z.setColor(color);
			table_row_z.setData(this);
			
			track_curve_z = new TrackCurve(color * vec4(0.0f,0.0f,1.0f,1.0f),min_value.z,max_value.z);
			line_curve_z = new LineCurveTrack(track_curve_z);
			track_curve_z.setData(line_curve_z);
			line_curve_z.setData(this);
			
			// parameter argument
			string type_name = parameter.getTypeName();
			if(match("node.position*",type_name) || match("node.rotation*",type_name)) {
				parameter_argument = 0.01f;
			}
		}
		
		~TrackerEditorParameterVec3() {
			
			keys_m.delete();
		}
		
		/*
		 */
		void addParameter(Table table,Track track,Line line,TrackerTrack tracker_track) {
			
			add_parameter(tracker_track,parameter_track);
			
			table.addRow(table_row);
			if(table_row.getValue(EDITOR_TABLE_COLUMN_TRACK)) track.addCurve(track_curve_x);
			line.addCurve(line_curve_x);
			
			table.addRow(table_row_y);
			if(table_row_y.getValue(EDITOR_TABLE_COLUMN_TRACK)) track.addCurve(track_curve_y);
			line.addCurve(line_curve_y);
			
			table.addRow(table_row_z);
			if(table_row_z.getValue(EDITOR_TABLE_COLUMN_TRACK)) track.addCurve(track_curve_z);
			line.addCurve(line_curve_z);
		}
		
		/*
		 */
		void setTrack(int track) {
			delete parameter_track;
			parameter_track = parameter.cloneTrack(track);
			keys_m.delete();
			update_keys();
		}
		
		int getTrack() {
			return parameter_track;
		}
		
		/*
		 */
		void updateTrack() {
			update_track_float(parameter_track.keys_x,track_curve_x);
			update_track_float(parameter_track.keys_y,track_curve_y);
			update_track_float(parameter_track.keys_z,track_curve_z);
			parameter.updateTrack(parameter_track);
			keys_m.delete();
			update_keys();
		}
		
		/*
		 */
		void updateCurve() {
			update_curve_float(track_curve_x,line_curve_x,parameter_track.keys_x);
			update_curve_float(track_curve_y,line_curve_y,parameter_track.keys_y);
			update_curve_float(track_curve_z,line_curve_z,parameter_track.keys_z);
			keys_m.delete();
			update_keys();
		}
		
		/*
		 */
		void renderTrack(float time) {
			
			// check type
			string type_name = parameter.getTypeName();
			if(match("node.position*",type_name) == 0) return;
			
			// check track
			int enabled = table_row.getValue(EDITOR_TABLE_COLUMN_TRACK);
			enabled |= table_row_y.getValue(EDITOR_TABLE_COLUMN_TRACK);
			enabled |= table_row_z.getValue(EDITOR_TABLE_COLUMN_TRACK);
			enabled &= is_base_class("Node",parameter_object);
			
			// disabled track
			if(enabled == 0) {
				keys_m.delete();
				return;
			}
			
			// parent transformation
			mat4 transform = mat4_identity;
			Node parent = Node(parameter_object).getParent();
			if(parent != NULL) transform = mat4(parent.getWorldTransform());
			
			// render current point
			vec3 point;
			if(match("node.positionLinear*",type_name)) point = TrackerParameterVec3(parameter).getTrackValueLinear(parameter_track,time);
			else point = TrackerParameterVec3(parameter).getTrackValue(parameter_track,time);
			engine.visualizer.renderPoint3D(Vec3(transform * point),0.1f,color);
			
			// render curve
			forloop(int i = 1; keys.size()) {
				float time_0 = keys[i - 1];
				float time_1 = keys[i];
				forloop(int j = 0; EDITOR_CURVE_POINTS + 1) {
					float time = time_0 + (time_1 - time_0) * j / EDITOR_CURVE_POINTS;
					vec3 point_0,point_1 = transform * TrackerParameterVec3(parameter).getTrackValue(parameter_track,time);
					if(j > 0) engine.visualizer.renderLine3D(Vec3(point_0),Vec3(point_1),color);
					point_0 = point_1;
				}
			}
			
			// update manipulators
			update_manipulators(transform);
		}
		
		/*
		 */
		void update_keys() {
			
			// check type
			string type_name = parameter.getTypeName();
			if(match("node.position*",type_name) == 0) return;
			
			// track keys
			keys.clear();
			float indices[];
			foreach(float time; parameter_track.indices_x) indices.append(time);
			foreach(float time; parameter_track.indices_y) indices.append(time);
			foreach(float time; parameter_track.indices_z) indices.append(time);
			foreachkey(float time; indices) keys.append(time);
			
			// create values
			values.resize(keys.size());
			forloop(int i = 0; keys.size()) {
				values[i] = TrackerParameterVec3(parameter).getTrackValue(parameter_track,keys[i]);
			}
		}
		
		/*
		 */
		void update_manipulators(mat4 transform) {
			
			// get player
			Player player = engine.editor.getPlayer();
			if(player == NULL) player = engine.game.getPlayer();
			if(player == NULL) return;
			
			mat4 projection = player.getProjection();
			Mat4 modelview = player.getIWorldTransform();
			
			// create manipulators
			if(keys_m.size() != keys.size()) {
				keys_m.delete();
				forloop(int i = 0; keys.size()) {
					Gui gui = engine.getGui();
					WidgetManipulatorTranslator manipulator = new WidgetManipulatorTranslator(gui);
					manipulator.setSize(EDITOR_MANIPULATOR_SIZE);
					manipulator.setCallback(CHANGED,functionid(callback_redirector),this,"manipulator_changed",i);
					gui.addChild(manipulator,ALIGN_OVERLAP);
					keys_m.append(manipulator);
				}
			}
			
			// selected keys
			float selected_keys[0];
			Line line = line_curve_x.getLine();
			forloop(int i = 0; line.getNumSelectedKeys()) {
				LineKey key = line.getSelectedKey(i);
				if(line_curve_x.isKey(key) || line_curve_y.isKey(key) || line_curve_z.isKey(key)) {
					selected_keys.append(key.getTime());
				}
			}
			
			// update manipulators
			forloop(int i = 0; keys.size()) {
				float key = keys[i];
				WidgetManipulatorTranslator manipulator = keys_m[i];
				manipulator.setCallbackEnabled(CHANGED,0);
				if(selected_keys.find(key) == -1) manipulator.setColor(vec4(0.5f,0.5f,0.5f,1.0f));
				else manipulator.setColor(vec4_one);
				int mask = 0;
				if(parameter_track.indices_x.find(key) != -1) mask |= WIDGET_MANIPULATOR_MASK_X;
				if(parameter_track.indices_y.find(key) != -1) mask |= WIDGET_MANIPULATOR_MASK_Y;
				if(parameter_track.indices_z.find(key) != -1) mask |= WIDGET_MANIPULATOR_MASK_Z;
				manipulator.setMask(mask);
				manipulator.setBasis(Mat4(transform));
				manipulator.setTransform(Mat4(transform * translate(values[i])));
				manipulator.setProjection(projection);
				manipulator.setModelview(modelview);
				manipulator.setCallbackEnabled(CHANGED,1);
			}
		}
		
		/*
		 */
		void manipulator_changed(int num) {
			
			// parent transformation
			Mat4 itransform = Mat4_identity;
			Node parent = Node(parameter_object).getParent();
			if(parent != NULL) itransform = parent.getIWorldTransform();
			
			// check value
			vec3 value = (itransform * keys_m[num].getTransform()) * vec3_zero;
			if(values[num] == value) return;
			
			// update track
			TrackerParameterVec3(parameter).setTrackValue(parameter_track,keys[num],value);
			
			// update values
			forloop(int i = 0; keys.size()) {
				values[i] = TrackerParameterVec3(parameter).getTrackValue(parameter_track,keys[i]);
			}
			
			// update curve
			update_curve_float(track_curve_x,line_curve_x,parameter_track.keys_x);
			update_curve_float(track_curve_y,line_curve_y,parameter_track.keys_y);
			update_curve_float(track_curve_z,line_curve_z,parameter_track.keys_z);
			
			// update keys
			update_keys();
		}
		
		/*
		 */
		void callback_redirector(TrackerEditorParameterVec3 parameter,string name,int num) {
			parameter.call(name,num);
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_EDITOR_PARAMETER_VEC3_H__ */
