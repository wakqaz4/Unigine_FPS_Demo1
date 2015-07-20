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


#ifndef __UNIGINE_WIDGET_TRACK_H__
#define __UNIGINE_WIDGET_TRACK_H__

#include <core/systems/widgets/widget_vbox.h>
#include <core/systems/widgets/widget_hbox.h>
#include <core/systems/widgets/widget_icon.h>
#include <core/systems/widgets/widget_label.h>
#include <core/systems/widgets/widget_spacer.h>
#include <core/systems/widgets/widget_editline.h>
#include <core/systems/widgets/widget_scrollbox.h>

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	enum {
		
		TRACK_CURVE_CHANGED = 0,		// curve changed callback
		TRACK_KEY_CREATED,				// key created callback
		TRACK_KEY_CHANGED,				// key changed callback
		TRACK_KEY_REMOVED,				// key removed callback
		TRACK_NUM_CALLBACKS,
		
		TRACK_KEY_CONSTANT = 0,			// constant key
		TRACK_KEY_LINEAR,				// linear key
		TRACK_KEY_SMOOTH,				// smooth key
		TRACK_KEY_BREAK,				// break key
		TRACK_KEY_AUTO,					// auto key
		TRACK_NUM_TYPES,
		
		TRACK_SPACE = 6,				// track space
		TRACK_WIDTH = 256,				// track width
		TRACK_HEIGHT = 64,				// track height
		TRACK_THRESHOLD = 8,			// track threshold
		
		TRACK_LINE_DISTANCE = 8.0f,		// line distance
		
		TRACK_TIME_HEIGHT = 2048,		// track time height
		
		TRACK_EDITLINE_WIDTH = 64,		// editline width
		
		TRACK_CANVAS_SPEED = 4,			// canvas speed
		TRACK_CANVAS_WIDTH = 65556,		// canvas width
		
		TRACK_CURVE_POINTS = 4,			// curve points
		TRACK_CURVE_DEPTH = 16,			// curve depth
		TRACK_CURVE_ANGLE = 0.999f,		// curve angle
		
		TRACK_HANDLE_SIZE = 6,			// handle size
		TRACK_HANDLE_SELECTION = 12,	// handle selection
	};
	
	/*
	 */
	vec4 track_time_color = vec4(1.0f,1.0f,1.0f,1.0f);
	vec4 track_range_color = vec4(0.25f,0.25f,0.25f,0.5f);
	vec4 track_smooth_color = vec4(1.0f,1.0f,0.0f,1.0f);
	vec4 track_break_color = vec4(1.0f,0.0f,0.0f,1.0f);
	vec4 track_contour_color = vec4(0.0f,0.0f,0.0f,1.0f);
	vec4 track_selected_color = vec4(1.0f,1.0f,1.0f,1.0f);
	vec4 track_selection_color = vec4(0.5f,0.5f,0.5f,0.25f);
	vec4 track_background_color = vec4(0.15f,0.15f,0.15f,1.0f);
	
	/**************************************************************************\
	*
	* TrackKey
	*
	\**************************************************************************/
	
	/*
	 */
	class TrackKey {
		
		TrackCurve curve;		// curve
		
		float time;				// key time
		float value;			// curve value
		vec4 handles;			// curve handles
		int type;				// curve type
		
		int data;				// key data
		
		int left_line_id;		// left line identifier
		int right_line_id;		// right line identifier
		int key_contour_id;		// key contour identifier
		int left_contour_id;	// left contour identifier
		int right_contour_id;	// right contour identifier
		int key_polygon_id;		// key polygon identifier
		int left_polygon_id;	// left polygon identifier
		int right_polygon_id;	// right polygon identifier
		int key_selection_id;	// key selection identifier
		int left_selection_id;	// left selection identifier
		int right_selection_id;	// right selection identifier
		
		WidgetCanvas canvas;
		
		/*
		 */
		TrackKey(float t,float v) {
			
			time = t;
			value = v;
			type = TRACK_KEY_LINEAR;
			
			left_line_id = -1;
			right_line_id = -1;
			key_contour_id = -1;
			left_contour_id = -1;
			right_contour_id = -1;
			key_polygon_id = -1;
			left_polygon_id = -1;
			right_polygon_id = -1;
			key_selection_id = -1;
			left_selection_id = -1;
			right_selection_id = -1;
		}
		
		~TrackKey() {
			
			if(canvas != NULL) {
				canvas.removeLine(left_line_id);
				canvas.removeLine(right_line_id);
				canvas.removeLine(key_contour_id);
				canvas.removeLine(left_contour_id);
				canvas.removeLine(right_contour_id);
				canvas.removePolygon(key_polygon_id);
				canvas.removePolygon(left_polygon_id);
				canvas.removePolygon(right_polygon_id);
				canvas.removePolygon(key_selection_id);
				canvas.removePolygon(left_selection_id);
				canvas.removePolygon(right_selection_id);
			}
		}
		
		/*
		 */
		void setCurve(TrackCurve c) {
			curve = c;
		}
		
		TrackCurve getCurve() {
			return curve;
		}
		
		/*
		 */
		void setTime(float t) {
			time = t;
		}
		
		float getTime() {
			return time;
		}
		
		/*
		 */
		void setValue(float v) {
			value = v;
		}
		
		float getValue() {
			return value;
		}
		
		/*
		 */
		void setHandles(vec4 h) {
			handles = h;
		}
		
		vec4 getHandles() {
			return handles;
		}
		
		/*
		 */
		void setType(int t) {
			type = t;
		}
		
		float getType() {
			return type;
		}
		
		/*
		 */
		void setData(int i) {
			data = i;
		}
		
		int getData() {
			return data;
		}
		
		/*
		 */
		void setCanvas(WidgetCanvas c) {
			if(canvas != c) {
				canvas = c;
				left_line_id = canvas.addLine(0);
				right_line_id = canvas.addLine(0);
				key_contour_id = canvas.addLine(1);
				left_contour_id = canvas.addLine(1);
				right_contour_id = canvas.addLine(1);
				key_polygon_id = canvas.addPolygon(0);
				left_polygon_id = canvas.addPolygon(0);
				right_polygon_id = canvas.addPolygon(0);
				key_selection_id = canvas.addPolygon(1);
				left_selection_id = canvas.addPolygon(1);
				right_selection_id = canvas.addPolygon(1);
				canvas.setLineColor(key_contour_id,track_contour_color);
				canvas.setLineColor(left_contour_id,track_contour_color);
				canvas.setLineColor(right_contour_id,track_contour_color);
				canvas.setPolygonColor(key_selection_id,vec4_zero);
				canvas.setPolygonColor(left_selection_id,vec4_zero);
				canvas.setPolygonColor(right_selection_id,vec4_zero);
			}
		}
		
		void setOrder(int &order) {
			if(canvas != NULL) {
				canvas.setLineOrder(left_line_id,order);
				canvas.setLineOrder(right_line_id,order++);
				canvas.setPolygonOrder(key_polygon_id,order);
				canvas.setPolygonOrder(left_polygon_id,order);
				canvas.setPolygonOrder(right_polygon_id,order++);
				canvas.setLineOrder(key_contour_id,order);
				canvas.setLineOrder(left_contour_id,order);
				canvas.setLineOrder(right_contour_id,order++);
				canvas.setPolygonOrder(key_selection_id,order);
				canvas.setPolygonOrder(left_selection_id,order);
				canvas.setPolygonOrder(right_selection_id,order++);
			}
		}
		
		/*
		 */
		int isLine(int id) {
			return (left_line_id == id || right_line_id == id);
		}
		
		int isPolygon(int id) {
			return (key_selection_id == id || left_selection_id == id || right_selection_id == id);
		}
		
		int isKeyPolygon(int id) {
			return (key_selection_id == id);
		}
		
		int isLeftPolygon(int id) {
			return (left_selection_id == id);
		}
		
		int isRightPolygon(int id) {
			return (right_selection_id == id);
		}
		
		/*
		 */
		void create(TrackKey prev,TrackKey next,int selected) {
			
			vec4 color = curve.getColor();
			Track track = curve.getTrack();
			mat4 transform = track.get_transform();
			
			vec3 position = floor(transform * vec3(time,value,0.0f));
			vec3 p0 = vec3(-TRACK_HANDLE_SIZE / 2,-TRACK_HANDLE_SIZE / 2,0.0f);
			vec3 p1 = vec3( TRACK_HANDLE_SIZE / 2,-TRACK_HANDLE_SIZE / 2,0.0f);
			vec3 p2 = vec3( TRACK_HANDLE_SIZE / 2, TRACK_HANDLE_SIZE / 2,0.0f);
			vec3 p3 = vec3(-TRACK_HANDLE_SIZE / 2, TRACK_HANDLE_SIZE / 2,0.0f);
			vec3 p4 = vec3(-TRACK_HANDLE_SELECTION / 2,-TRACK_HANDLE_SELECTION / 2,0.0f);
			vec3 p5 = vec3( TRACK_HANDLE_SELECTION / 2,-TRACK_HANDLE_SELECTION / 2,0.0f);
			vec3 p6 = vec3( TRACK_HANDLE_SELECTION / 2, TRACK_HANDLE_SELECTION / 2,0.0f);
			vec3 p7 = vec3(-TRACK_HANDLE_SELECTION / 2, TRACK_HANDLE_SELECTION / 2,0.0f);
			
			// key polygon
			canvas.clearPolygonPoints(key_polygon_id);
			if(selected) canvas.setPolygonColor(key_polygon_id,track_selected_color);
			else canvas.setPolygonColor(key_polygon_id,color);
			canvas.addPolygonPoint(key_polygon_id,position + p0);
			canvas.addPolygonPoint(key_polygon_id,position + p1);
			canvas.addPolygonPoint(key_polygon_id,position + p2);
			canvas.addPolygonPoint(key_polygon_id,position + p3);
			
			// key contour
			canvas.clearLinePoints(key_contour_id);
			canvas.addLinePoint(key_contour_id,position + p0);
			canvas.addLinePoint(key_contour_id,position + p1);
			canvas.addLinePoint(key_contour_id,position + p2);
			canvas.addLinePoint(key_contour_id,position + p3);
			canvas.addLinePoint(key_contour_id,position + p0);
			
			// key selection
			canvas.clearPolygonPoints(key_selection_id);
			canvas.addPolygonPoint(key_selection_id,position + p4);
			canvas.addPolygonPoint(key_selection_id,position + p5);
			canvas.addPolygonPoint(key_selection_id,position + p6);
			canvas.addPolygonPoint(key_selection_id,position + p7);
			
			// left handle
			canvas.clearLinePoints(left_line_id);
			canvas.clearLinePoints(left_contour_id);
			canvas.clearPolygonPoints(left_polygon_id);
			canvas.clearPolygonPoints(left_selection_id);
			if(prev != NULL && type != TRACK_KEY_AUTO && (prev.getType() == TRACK_KEY_SMOOTH || prev.getType() == TRACK_KEY_BREAK)) {
				
				vec3 left_position = floor(transform * vec3(time + handles.x,value + handles.y,0.0f));
				
				// left color
				if(type == TRACK_KEY_SMOOTH) {
					canvas.setLineColor(left_line_id,track_smooth_color);
					canvas.setPolygonColor(left_polygon_id,track_smooth_color);
				} else if(type == TRACK_KEY_BREAK) {
					canvas.setLineColor(left_line_id,track_break_color);
					canvas.setPolygonColor(left_polygon_id,track_break_color);
				}
				
				// left line
				canvas.addLinePoint(left_line_id,position);
				canvas.addLinePoint(left_line_id,left_position);
				
				// left polygon
				canvas.addPolygonPoint(left_polygon_id,left_position + p0);
				canvas.addPolygonPoint(left_polygon_id,left_position + p1);
				canvas.addPolygonPoint(left_polygon_id,left_position + p2);
				canvas.addPolygonPoint(left_polygon_id,left_position + p3);
				
				// left contour
				canvas.clearLinePoints(left_contour_id);
				canvas.addLinePoint(left_contour_id,left_position + p0);
				canvas.addLinePoint(left_contour_id,left_position + p1);
				canvas.addLinePoint(left_contour_id,left_position + p2);
				canvas.addLinePoint(left_contour_id,left_position + p3);
				canvas.addLinePoint(left_contour_id,left_position + p0);
				
				// left selection
				canvas.addPolygonPoint(left_selection_id,left_position + p4);
				canvas.addPolygonPoint(left_selection_id,left_position + p5);
				canvas.addPolygonPoint(left_selection_id,left_position + p6);
				canvas.addPolygonPoint(left_selection_id,left_position + p7);
			}
			
			// right handle
			canvas.clearLinePoints(right_line_id);
			canvas.clearLinePoints(right_contour_id);
			canvas.clearPolygonPoints(right_polygon_id);
			canvas.clearPolygonPoints(right_selection_id);
			if(next != NULL && (type == TRACK_KEY_SMOOTH || type == TRACK_KEY_BREAK)) {
				
				vec3 right_position = floor(transform * vec3(time + handles.z,value + handles.w,0.0f));
				
				// right color
				if(type == TRACK_KEY_SMOOTH) {
					canvas.setLineColor(right_line_id,track_smooth_color);
					canvas.setPolygonColor(right_polygon_id,track_smooth_color);
				} else if(type == TRACK_KEY_BREAK) {
					canvas.setLineColor(right_line_id,track_break_color);
					canvas.setPolygonColor(right_polygon_id,track_break_color);
				}
				
				// right line
				canvas.addLinePoint(right_line_id,position);
				canvas.addLinePoint(right_line_id,right_position);
				
				// right polygon
				canvas.addPolygonPoint(right_polygon_id,right_position + p0);
				canvas.addPolygonPoint(right_polygon_id,right_position + p1);
				canvas.addPolygonPoint(right_polygon_id,right_position + p2);
				canvas.addPolygonPoint(right_polygon_id,right_position + p3);
				
				// right contour
				canvas.clearLinePoints(right_contour_id);
				canvas.addLinePoint(right_contour_id,right_position + p0);
				canvas.addLinePoint(right_contour_id,right_position + p1);
				canvas.addLinePoint(right_contour_id,right_position + p2);
				canvas.addLinePoint(right_contour_id,right_position + p3);
				canvas.addLinePoint(right_contour_id,right_position + p0);
				
				// right selection
				canvas.addPolygonPoint(right_selection_id,right_position + p4);
				canvas.addPolygonPoint(right_selection_id,right_position + p5);
				canvas.addPolygonPoint(right_selection_id,right_position + p6);
				canvas.addPolygonPoint(right_selection_id,right_position + p7);
			}
		}
	};
	
	/**************************************************************************\
	*
	* TrackCurve
	*
	\**************************************************************************/
	
	/*
	 */
	class TrackCurve {
		
		Track track;			// track
		
		vec4 color;				// curve color
		
		float min_value;		// minimum value
		float max_value;		// maximum value
		
		int data;				// curve data
		
		TrackKey keys[0];		// curve keys
		float indices[0];		// key indices
		
		int keys_changed;		// keys changed flag
		
		int line_id;			// line identifier
		
		WidgetCanvas canvas;
		
		/*
		 */
		TrackCurve(vec4 c,float min = -INFINITY,float max = INFINITY) {
			
			color = c;
			
			min_value = min;
			max_value = max;
			
			line_id = -1;
		}
		
		~TrackCurve() {
			
			clear();
			
			if(canvas != NULL) {
				canvas.removeLine(line_id);
			}
		}
		
		/*
		 */
		void setTrack(Track t) {
			track = t;
		}
		
		Track getTrack() {
			return track;
		}
		
		/*
		 */
		void setColor(vec4 c) {
			color = c;
		}
		
		vec4 getColor() {
			return color;
		}
		
		/*
		 */
		void setMinValue(float v) {
			min_value = v;
		}
		
		float getMinValue() {
			return min_value;
		}
		
		/*
		 */
		void setMaxValue(float v) {
			max_value = v;
		}
		
		float getMaxValue() {
			return max_value;
		}
		
		/*
		 */
		void setData(int d) {
			data = d;
		}
		
		int getData() {
			return data;
		}
		
		/*
		 */
		void clear() {
			keys.delete();
			indices.delete();
		}
		
		/*
		 */
		void addKey(TrackKey key) {
			if(keys.find(key) != -1) throw("Unigine::Widgets::TrackCurve::addKey(): key is already defined\n");
			key.setCurve(this);
			key.setCanvas(canvas);
			keys.append(key);
			updateKeys();
		}
		
		void removeKey(TrackKey key) {
			int index = keys.find(key);
			if(index == -1) throw("Unigine::Widgets::TrackCurve::removeKey(): can't find key\n");
			keys.remove(index);
			updateKeys();
			delete key;
		}
		
		/*
		 */
		TrackKey cloneKey(TrackKey key) {
			TrackKey clone = new TrackKey(key.getTime(),key.getValue());
			clone.setHandles(key.getHandles());
			clone.setType(key.getType());
			addKey(clone);
			return clone;
		}
		
		/*
		 */
		TrackKey getPrevKey(TrackKey key) {
			int index = keys.find(key);
			if(index > 0) return keys[index - 1];
			return NULL;
		}
		
		/*
		 */
		void updateKeys() {
			
			// update indices
			indices.resize(keys.size());
			forloop(int i = 0; keys.size()) {
				indices[i] = keys[i].getTime();
			}
			indices.sort(keys);
			
			// create auto handles
			forloop(int i = 0; keys.size()) {
				TrackKey key = keys[i];
				if(key.getType() == TRACK_KEY_AUTO) {
					vec4 handles = vec4_zero;
					if(i > 0) {
						TrackKey prev = keys[i - 1];
						handles.x = (prev.getTime() - key.getTime()) / 3.0f;
						handles.y = (prev.getValue() - key.getValue()) / 3.0f;
					}
					if(i + 1 < keys.size()) {
						TrackKey next = keys[i + 1];
						handles.z = (next.getTime() - key.getTime()) / 3.0f;
						handles.w = (next.getValue() - key.getValue()) / 3.0f;
					}
					handles.x = (handles.x - handles.z) / 2.0f;
					handles.y = (handles.y - handles.w) / 2.0f;
					handles.z = -handles.x;
					handles.w = -handles.y;
					key.setHandles(handles);
				}
			}
			
			keys_changed = 1;
		}
		
		/*
		 */
		void setKeyType(TrackKey key,int type) {
			
			int index = keys.find(key);
			
			// create break handles
			int old_type = key.getType();
			if((old_type == TRACK_KEY_CONSTANT || old_type == TRACK_KEY_LINEAR) && (type == TRACK_KEY_BREAK || type == TRACK_KEY_SMOOTH)) {
				vec4 handles = key.getHandles();
				if(index > 0 && handles.x == 0.0f && handles.y == 0.0f) {
					TrackKey prev = keys[index - 1];
					handles.x = (prev.getTime() - key.getTime()) / 3.0f;
					handles.y = (prev.getValue() - key.getValue()) / 3.0f;
				}
				if(index + 1 < keys.size() && handles.z == 0.0f && handles.w == 0.0f) {
					TrackKey next = keys[index + 1];
					handles.z = (next.getTime() - key.getTime()) / 3.0f;
					handles.w = (next.getValue() - key.getValue()) / 3.0f;
				}
				key.setHandles(handles);
			}
			
			// create smooth handles
			if(type == TRACK_KEY_SMOOTH) {
				vec4 handles = key.getHandles();
				handles.x = (handles.x - handles.z) / 2.0f;
				handles.y = (handles.y - handles.w) / 2.0f;
				handles.z = -handles.x;
				handles.w = -handles.y;
				key.setHandles(handles);
			}
			
			key.setType(type);
			updateKeys();
		}
		
		/*
		 */
		float getValue(float time) {
			
			int left = indices.left(time);
			int right = indices.right(time);
			
			if(left != -1 && right != -1) {
				
				TrackKey k0 = keys[left];
				TrackKey k1 = keys[right];
				float t0 = k0.time;
				float t1 = k1.time;
				
				if(t0 == t1) return k0.value;
				
				switch(k0.type) {
					case TRACK_KEY_CONSTANT:
						return k0.value;
					case TRACK_KEY_LINEAR:
						float k = (time - t0) / (t1 - t0);
						return lerp(k0.value,k1.value,k);
					case TRACK_KEY_SMOOTH:
					case TRACK_KEY_BREAK:
					case TRACK_KEY_AUTO:
						vec4 h0 = k0.handles;
						vec4 h1 = k1.handles;
						vec4 t = vec4(t0,clamp(t0 + h0.z,t0,t1),clamp(t1 + h1.x,t0,t1),t1);
						vec4 v = vec4(k0.value,k0.value + h0.w,k1.value + h1.y,k1.value);
						return bezier(t,v,time);
				}
			}
			
			if(left != -1) return keys[left].value;
			if(right != -1) return keys[right].value;
			
			return 0.0f;
		}
		
		/*
		 */
		int getNumKeys() {
			return keys.size();
		}
		
		TrackKey getKey(int num) {
			return keys[num];
		}
		
		/*
		 */
		void setCanvas(WidgetCanvas c) {
			if(canvas != c) {
				canvas = c;
				line_id = canvas.addLine();
				foreach(TrackKey key; keys) {
					key.setCanvas(canvas);
				}
			}
		}
		
		void setOrder(int &order) {
			canvas.setLineOrder(line_id,order++);
			foreach(TrackKey key; keys) {
				key.setOrder(order);
			}
		}
		
		/*
		 */
		int isLine(int id) {
			return (line_id == id);
		}
		
		int isPolygon(int id) {
			foreach(TrackKey key; keys) {
				if(key.isPolygon(id)) return 1;
			}
			return 0;
		}
		
		/*
		 */
		void create_curve(vec3 p0,vec3 p1,vec3 p2,vec3 p3) {
			
			// get bezier point
			vec3 get_point(float k0) {
				float k1 = 1.0f - k0;
				float k00 = k0 * k0;
				float k11 = k1 * k1;
				return p0 * (k11 * k1) + p1 * (3.0f * k11 * k0) + p2 * (3.0f * k00 * k1) + p3 * (k00 * k0);
			}
			
			// create bezier curve
			int recursion_depth = 0;
			void create_curve(vec3 p0,vec3 p1,float k0,float k1) {
				float k2 = (k0 + k1) * 0.5f;
				vec3 p2 = get_point(k2);
				vec3 p02 = p0 - p2;
				vec3 p12 = p1 - p2;
				float l02 = length(p02);
				float l12 = length(p12);
				if(recursion_depth++ < TRACK_CURVE_DEPTH && l02 > EPSILON && l12 > EPSILON && dot(p02,p12) / (l02 * l12) > -TRACK_CURVE_ANGLE) {
					create_curve(p0,p2,k0,k2);
					create_curve(p2,p1,k2,k1);
				} else {
					canvas.addLinePoint(line_id,floor(p2));
				}
			}
			
			// create curve
			canvas.addLinePoint(line_id,floor(p0));
			float step = 1.0f / TRACK_CURVE_POINTS;
			forloop(int i = 0; TRACK_CURVE_POINTS) {
				float k0 = i * step;
				float k1 = k0 + step;
				vec3 p0 = get_point(k0);
				vec3 p1 = get_point(k1);
				recursion_depth = 0;
				create_curve(p0,p1,k0,k1);
			}
			canvas.addLinePoint(line_id,floor(p3));
		}
		
		/*
		 */
		void create(TrackKey selected_keys[]) {
			
			float min_time = track.getMinTime();
			float max_time = track.getMaxTime();
			mat4 transform = track.get_transform();
			
			// create line
			canvas.clearLinePoints(line_id);
			canvas.setLineColor(line_id,color);
			
			// empty curve
			if(keys.size() == 0) {
				canvas.addLinePoint(line_id,floor(transform * vec3(min_time,0.0f,0.0f)));
				canvas.addLinePoint(line_id,floor(transform * vec3(max_time,0.0f,0.0f)));
				return;
			}
			
			// line begin
			if(keys[0].getTime() >= min_time) {
				canvas.addLinePoint(line_id,floor(transform * vec3(min_time,keys[0].getValue(),0.0f)));
			}
			
			// line keys
			forloop(int i = 0; keys.size() - 1) {
				TrackKey k0 = keys[i + 0];
				TrackKey k1 = keys[i + 1];
				float t0 = k0.time;
				float t1 = k1.time;
				switch(k0.getType()) {
					
					// constant segment
					case TRACK_KEY_CONSTANT:
						canvas.addLinePoint(line_id,floor(transform * vec3(t0,k0.getValue(),0.0f)));
						canvas.addLinePoint(line_id,floor(transform * vec3(t1,k0.getValue(),0.0f)));
						canvas.addLinePoint(line_id,floor(transform * vec3(t1,k1.getValue(),0.0f)));
						break;
					
					// linear segment
					case TRACK_KEY_LINEAR:
						canvas.addLinePoint(line_id,floor(transform * vec3(t0,k0.getValue(),0.0f)));
						canvas.addLinePoint(line_id,floor(transform * vec3(t1,k1.getValue(),0.0f)));
						break;
					
					// bezier segment
					case TRACK_KEY_SMOOTH:
					case TRACK_KEY_BREAK:
					case TRACK_KEY_AUTO:
						vec4 h0 = k0.getHandles();
						vec4 h1 = k1.getHandles();
						vec3 p0 = transform * vec3(t0,k0.getValue(),0.0f);
						vec3 p1 = transform * vec3(clamp(t0 + h0.z,t0,t1),k0.getValue() + h0.w,0.0f);
						vec3 p2 = transform * vec3(clamp(t1 + h1.x,t0,t1),k1.getValue() + h1.y,0.0f);
						vec3 p3 = transform * vec3(t1,k1.getValue(),0.0f);
						create_curve(p0,p1,p2,p3);
						break;
				}
			}
			
			// line end
			if(keys[keys.size() - 1].getTime() <= max_time) {
				canvas.addLinePoint(line_id,floor(transform * vec3(max_time,keys[keys.size() - 1].getValue(),0.0f)));
			}
			
			// create keys
			forloop(int i = 0; keys.size()) {
				TrackKey key = keys[i];
				if(selected_keys.find(key) != -1) {
					TrackKey prev = (i > 0) ? keys[i - 1] : NULL;
					TrackKey next = (i + 1 < keys.size()) ? keys[i + 1] : NULL;
					key.create(prev,next,1);
				} else {
					key.create(NULL,NULL,0);
				}
			}
		}
	};
	
	/**************************************************************************\
	*
	* Track
	*
	\**************************************************************************/
	
	/*
	 */
	class Track : Widget {
		
		int width;							// track width
		int height;							// track  height
		
		int order = 16;						// current order
		int time_line_id;					// time line identifier
		int range_line_id[2];				// range line identifier
		int range_polygon_id[2];			// range polygon identifier
		int selection_line_id;				// selection line identifier
		int selection_polygon_id;			// selection polygon identifier
		
		float min_time;						// minimum time
		float max_time;						// maximum time
		
		float current_time;					// current time
		float range_from;					// range from
		float range_to;						// range to
		
		TrackCurve curves[0];				// curves
		
		int pressed;						// pressed flag
		
		float scale_y;						// scale y
		float offset_y;						// offset y
		mat4 transform;						// transformation
		
		int range_changed;					// range changed flag
		int curves_changed;					// curves changed flag
		
		TrackCurve current_curve;			// current curve
		TrackKey current_key;				// current key
		
		TrackCurve selected_curves[0];		// selected curves
		TrackKey selected_keys[0];			// selected keys
		
		TrackCurve old_selected_curves[0];	// old selected curves
		TrackKey old_selected_keys[0];		// old selected keys
		
		VBox panel_vbox;					// panel vbox
		HBox panel_hbox;					// panel hbox
		
		Icon arrange_x_i;					// arrange x icon
		Icon arrange_y_i;					// arrange y icon
		Icon arrange_xy_i;					// arrange xy icon
		
		Icon key_remove_i;					// key remove icon
		Icon key_snap_i;					// key snap icon
		Icon key_types_i[TRACK_NUM_TYPES];	// key type icons
		
		EditLine time_el;					// time editline
		EditLine value_el;					// value editline
		
		ScrollBox canvas_sb;				// canvas scrollbox
		
		string callbacks[TRACK_NUM_CALLBACKS];
		int callbacks_data_0[TRACK_NUM_CALLBACKS];
		int callbacks_data_1[TRACK_NUM_CALLBACKS];
		int callbacks_enabled[TRACK_NUM_CALLBACKS];
		
		WidgetVBox vbox;
		WidgetCanvas canvas;
		
		/*
		 */
		Track(float min,float max) {
			
			min_time = min;
			max_time = max;
			
			current_time = -INFINITY;
			range_from = min;
			range_to = max;
			
			// create panel
			panel_vbox = new VBox();
			panel_hbox = new HBox(2,2);
			panel_vbox.addChild(panel_hbox,ALIGN_EXPAND);
			
			// arrange icons
			arrange_x_i = new Icon("core/systems/widgets/images/widget_track_arrange_x.png");
			arrange_y_i = new Icon("core/systems/widgets/images/widget_track_arrange_y.png");
			arrange_xy_i = new Icon("core/systems/widgets/images/widget_track_arrange_xy.png");
			arrange_x_i.setCallback(CLICKED,functionid(callback_redirector),this,functionid(arrange_x_clicked));
			arrange_y_i.setCallback(CLICKED,functionid(callback_redirector),this,functionid(arrange_y_clicked));
			arrange_xy_i.setCallback(CLICKED,functionid(callback_redirector),this,functionid(arrange_xy_clicked));
			panel_hbox.addChild(arrange_x_i,ALIGN_LEFT);
			panel_hbox.addChild(arrange_y_i,ALIGN_LEFT);
			panel_hbox.addChild(arrange_xy_i,ALIGN_LEFT);
			
			// spacer
			Spacer spacer = new Spacer();
			spacer.setOrientation(VERTICAL);
			panel_hbox.addChild(spacer,ALIGN_LEFT);
			
			// key remove icon
			key_remove_i = new Icon("core/systems/widgets/images/widget_track_key_remove.png");
			key_remove_i.setCallback(CLICKED,functionid(callback_redirector),this,functionid(key_remove_clicked));
			key_remove_i.setEnabled(0);
			panel_hbox.addChild(key_remove_i,ALIGN_LEFT);
			
			// key snap icon
			key_snap_i = new Icon("core/systems/widgets/images/widget_track_key_snap.png");
			key_snap_i.setToggleable(1);
			panel_hbox.addChild(key_snap_i,ALIGN_LEFT);
			
			// spacer
			spacer = new Spacer();
			spacer.setOrientation(VERTICAL);
			panel_hbox.addChild(spacer,ALIGN_LEFT);
			
			// key type icons
			key_types_i[0] = new Icon("core/systems/widgets/images/widget_track_key_constant.png");
			key_types_i[1] = new Icon("core/systems/widgets/images/widget_track_key_linear.png");
			key_types_i[2] = new Icon("core/systems/widgets/images/widget_track_key_smooth.png");
			key_types_i[3] = new Icon("core/systems/widgets/images/widget_track_key_break.png");
			key_types_i[4] = new Icon("core/systems/widgets/images/widget_track_key_auto.png");
			forloop(int i = 0; TRACK_NUM_TYPES) {
				key_types_i[i].setCallback(CLICKED,functionid(callback_redirector_1),this,functionid(key_type_clicked),i);
				key_types_i[i].setEnabled(0);
				key_types_i[i].setToggleable(1);
				panel_hbox.addChild(key_types_i[i],ALIGN_LEFT);
			}
			
			// spacer
			spacer = new Spacer();
			spacer.setOrientation(VERTICAL);
			panel_hbox.addChild(spacer,ALIGN_LEFT);
			
			// time editline
			time_el = new EditLine();
			time_el.setEnabled(0);
			time_el.setWidth(TRACK_EDITLINE_WIDTH);
			time_el.setValidator(VALIDATOR_FLOAT);
			time_el.setCallback(PRESSED,functionid(callback_redirector),this,functionid(time_pressed));
			panel_hbox.addChild(new Label(TR(" Time: ")),ALIGN_LEFT);
			panel_hbox.addChild(time_el,ALIGN_LEFT);
			
			// value editline
			value_el = new EditLine();
			value_el.setEnabled(0);
			value_el.setWidth(TRACK_EDITLINE_WIDTH);
			value_el.setValidator(VALIDATOR_FLOAT);
			value_el.setCallback(PRESSED,functionid(callback_redirector),this,functionid(value_pressed));
			panel_hbox.addChild(new Label(TR(" Value: ")),ALIGN_LEFT);
			panel_hbox.addChild(value_el,ALIGN_LEFT);
			
			// canvas scrollbox
			canvas_sb = new ScrollBox();
			canvas_sb.setVScrollEnabled(0);
			
			vbox = new WidgetVBox(engine.getGui());
			canvas = new WidgetCanvas(engine.getGui());
			
			vbox.addChild(panel_vbox.widget);
			vbox.addChild(canvas_sb.widget,ALIGN_EXPAND);
			canvas_sb.widget.addChild(canvas,ALIGN_EXPAND);
			
			// canvas parameters
			canvas.setWidth(TRACK_WIDTH);
			canvas.setHeight(TRACK_HEIGHT);
			canvas.setColor(track_background_color);
			
			// create time line
			time_line_id = canvas.addLine();
			canvas.setLineColor(time_line_id,track_time_color);
			
			// create range lines
			range_line_id[0] = canvas.addLine();
			range_line_id[1] = canvas.addLine();
			canvas.setLineColor(range_line_id[0],vec4(track_range_color.xyz,1.0f));
			canvas.setLineColor(range_line_id[1],vec4(track_range_color.xyz,1.0f));
			
			// create range polygons
			range_polygon_id[0] = canvas.addPolygon();
			range_polygon_id[1] = canvas.addPolygon();
			canvas.setPolygonColor(range_polygon_id[0],track_range_color);
			canvas.setPolygonColor(range_polygon_id[1],track_range_color);
			canvas.setPolygonBlendFunc(range_polygon_id[0],BLEND_SRC_ALPHA,BLEND_ONE_MINUS_SRC_ALPHA);
			canvas.setPolygonBlendFunc(range_polygon_id[1],BLEND_SRC_ALPHA,BLEND_ONE_MINUS_SRC_ALPHA);
			
			// create selection line
			selection_line_id = canvas.addLine();
			canvas.setLineColor(selection_line_id,vec4(track_selection_color.xyz,1.0f));
			
			// create selection polygon
			selection_polygon_id = canvas.addPolygon();
			canvas.setPolygonColor(selection_polygon_id,track_selection_color);
			canvas.setPolygonBlendFunc(selection_polygon_id,BLEND_SRC_ALPHA,BLEND_ONE_MINUS_SRC_ALPHA);
			
			// set callbacks
			canvas.setCallback(PRESSED,functionid(callback_redirector),this,functionid(pressed_callback));
			canvas.setCallback(RELEASED,functionid(callback_redirector),this,functionid(released_callback));
			canvas.setCallback(DOUBLE_CLICKED,functionid(callback_redirector),this,functionid(double_clicked_callback));
			
			widget = vbox;
		}
		
		~Track() {
			
			clear();
			
			delete panel_vbox;
			delete panel_hbox;
			
			delete arrange_x_i;
			delete arrange_y_i;
			delete arrange_xy_i;
			delete key_remove_i;
			delete key_snap_i;
			key_types_i.delete();
			
			delete time_el;
			delete value_el;
			
			delete canvas_sb;
			delete canvas;
		}
		
		/*
		 */
		VBox getPanelVBox() {
			return panel_vbox;
		}
		
		HBox getPanelHBox() {
			return panel_hbox;
		}
		
		ScrollBox getScrollBox() {
			return canvas_sb;
		}
		
		Icon getRemoveIcon() {
			return key_remove_i;
		}
		
		EditLine getTimeEditLine() {
			return time_el;
		}
		
		EditLine getValueEditLine() {
			return value_el;
		}
		
		/*
		 */
		int isCanvasFocused() {
			return vbox.isChild(engine.gui.getFocus());
		}
		
		void setCanvasWidth(int width) {
			canvas.setWidth(clamp(width,canvas_sb.getHScrollFrameSize() / canvas_sb.getScrollScale(),TRACK_CANVAS_WIDTH));
		}
		
		int getCanvasWidth() {
			return canvas.getWidth();
		}
		
		void clearCanvas() {
			forloop(int i = 0; canvas.getNumTexts()) {
				canvas.setTextText(canvas.getText(i),"");
			}
			forloop(int i = 0; canvas.getNumLines()) {
				canvas.clearLinePoints(canvas.getLine(i));
			}
			forloop(int i = 0; canvas.getNumPolygons()) {
				canvas.clearPolygonPoints(canvas.getPolygon(i));
			}
		}
		
		/*
		 */
		void setMinTime(float time) {
			min_time = time;
			update_transform();
			curves_changed = 1;
		}
		
		float getMinTime() {
			return min_time;
		}
		
		/*
		 */
		void setMaxTime(float time) {
			max_time = time;
			update_transform();
			curves_changed = 1;
		}
		
		float getMaxTime() {
			return max_time;
		}
		
		/*
		 */
		void setTime(float time) {
			current_time = time;
		}
		
		float getTime() {
			return current_time;
		}
		
		/*
		 */
		void setRangeFrom(float time) {
			range_from = clamp(time,min_time,max_time);
			range_changed = 1;
		}
		
		float getRangeFrom() {
			return range_from;
		}
		
		/*
		 */
		void setRangeTo(float time) {
			range_to = clamp(time,min_time,max_time);
			range_changed = 1;
		}
		
		float getRangeTo() {
			return range_to;
		}
		
		/*
		 */
		void setKeySnap(int snap) {
			key_snap_i.setToggled(snap);
		}
		
		int getKeySnap() {
			return key_snap_i.isToggled();
		}
		
		/*
		 */
		void clear() {
			
			curves.delete();
			
			current_curve = NULL;
			current_key = NULL;
			
			selected_curves.clear();
			selected_keys.clear();
			
			old_selected_curves.clear();
			old_selected_keys.clear();
		}
		
		/**********************************************************************\
		*
		* Curves
		*
		\**********************************************************************/
		
		/*
		 */
		void addCurve(TrackCurve curve) {
			if(curves.find(curve) != -1) throw("Unigine::Widgets::Track::addCurve(): curve is already defined\n");
			curve.setTrack(this);
			curve.setCanvas(canvas);
			curve.setOrder(order);
			curves.append(curve);
		}
		
		void removeCurve(TrackCurve curve) {
			int index = curves.find(curve);
			if(index == -1) throw("Unigine::Widgets::Track::removeCurve(): can't find curve\n");
			curves.remove(index);
			delete curve;
		}
		
		int isCurve(TrackCurve curve) {
			return (curves.find(curve) != -1);
		}
		
		/*
		 */
		int getNumCurves() {
			return curves.size();
		}
		
		TrackCurve getCurve(int num) {
			return curves[num];
		}
		
		/*
		 */
		void setCurrentCurve(TrackCurve curve) {
			if(current_curve != curve) {
				current_curve = curve;
				if(curve != NULL) curve.setOrder(order);
				runCallback(TRACK_CURVE_CHANGED);
			}
		}
		
		TrackCurve getCurrentCurve() {
			return current_curve;
		}
		
		/*
		 */
		void setCurrentKey(TrackKey key) {
			if(current_key != key) {
				if(key != NULL) {
					key.setOrder(order);
					key_remove_i.setEnabled(1);
					forloop(int i = 0; TRACK_NUM_TYPES) {
						key_types_i[i].setEnabled(1);
					}
					time_el.setEnabled(1);
					value_el.setEnabled(1);
				} else {
					key_remove_i.setEnabled(0);
					forloop(int i = 0; TRACK_NUM_TYPES) {
						key_types_i[i].setCallbackEnabled(CLICKED,0);
						key_types_i[i].setToggled(0);
						key_types_i[i].setCallbackEnabled(CLICKED,1);
						key_types_i[i].setEnabled(0);
					}
					time_el.setText("");
					value_el.setText("");
					time_el.setEnabled(0);
					value_el.setEnabled(0);
				}
				current_key = key;
			}
		}
		
		TrackKey getCurrentKey() {
			return current_key;
		}
		
		/*
		 */
		void setSelectedCurves(TrackCurve curves[]) {
			selected_curves.copy(curves);
			curves_changed = 1;
		}
		
		void getNumSelectedCurves() {
			return selected_curves.size();
		}
		
		TrackCurve getSelectedCurve(int num) {
			return selected_curves[num];
		}
		
		/*
		 */
		void setSelectedKeys(TrackKey keys[]) {
			selected_keys.copy(keys);
			curves_changed = 1;
		}
		
		void getNumSelectedKeys() {
			return selected_keys.size();
		}
		
		TrackKey getSelectedKey(int num) {
			return selected_keys[num];
		}
		
		/*
		 */
		void clearSelectedKey(TrackKey key) {
			int index = selected_keys.find(key);
			if(index != -1) {
				selected_curves.remove(index);
				selected_keys.remove(index);
				curves_changed = 1;
			}
		}
		
		/*
		 */
		void moveSelectedKeys(float time,float value) {
			
			// selected bounds
			float min_time = 0.0f;
			float max_time = 0.0f;
			foreach(TrackKey key; selected_keys) {
				min_time = min(min_time,key.getTime() - current_key.getTime());
				max_time = max(max_time,key.getTime() - current_key.getTime());
			}
			
			// clamp values
			time = clamp(time,getMinTime() - min_time,getMaxTime() - max_time);
			
			// move selected keys
			if(current_key.getTime() != time || current_key.getValue() != value) {
				time -= current_key.getTime();
				value -= current_key.getValue();
				forloop(int i = 0; selected_keys.size()) {
					float min_value = selected_curves[i].getMinValue();
					float max_value = selected_curves[i].getMaxValue();
					selected_keys[i].setTime(selected_keys[i].getTime() + time);
					selected_keys[i].setValue(clamp(selected_keys[i].getValue() + value,min_value,max_value));
				}
				updateSelectedKeys();
			}
		}
		
		void removeSelectedKeys() {
			
			// run callback
			runCallback(TRACK_KEY_REMOVED);
			
			// remove selected keys
			forloop(int i = 0; selected_keys.size()) {
				TrackCurve curve = selected_curves[i];
				TrackKey key = selected_keys[i];
				curve.removeKey(key);
			}
			selected_curves.clear();
			selected_keys.clear();
			
			// clear current key
			setCurrentKey(NULL);
			
			// update keys
			updateSelectedKeys();
		}
		
		void updateSelectedKeys() {
			
			// selected curves
			TrackCurve curves[];
			forloop(int i = 0; selected_keys.size()) {
				curves.append(selected_curves[i]);
			}
			foreachkey(TrackCurve curve; curves) {
				curve.updateKeys();
			}
			
			// update key parameters
			if(current_key != NULL) {
				int type = current_key.getType();
				forloop(int i = 0; TRACK_NUM_TYPES) {
					key_types_i[i].setCallbackEnabled(CLICKED,0);
					key_types_i[i].setToggled(type == i);
					key_types_i[i].setCallbackEnabled(CLICKED,1);
				}
				time_el.setText(widgetFormat(current_key.getTime()));
				value_el.setText(widgetFormat(current_key.getValue()));
			}
			
			// run callback
			runCallback(TRACK_KEY_CHANGED);
		}
		
		/*
		 */
		void arrangeX() {
			canvas.setWidth(canvas_sb.getHScrollFrameSize() / canvas_sb.getScrollScale());
		}
		
		void arrangeY() {
			int num_keys = 0;
			float min_value = INFINITY;
			float max_value = -INFINITY;
			foreach(TrackCurve curve; curves) {
				num_keys += curve.keys.size();
				if(curve.keys.size() == 0) {
					min_value = min(min_value,0.0f);
					max_value = max(max_value,0.0f);
				}
				foreach(TrackKey key; curve.keys) {
					float value = key.getValue();
					min_value = min(min_value,value);
					max_value = max(max_value,value);
				}
			}
			if(num_keys == 0) {
				min_value = -1.0f;
				max_value = 1.0f;
			} else if(max_value - min_value < EPSILON) {
				min_value -= 1.0f;
				max_value += 1.0f;
			} else {
				min_value -= EPSILON;
				max_value += EPSILON;
			}
			int height = canvas_sb.getVScrollFrameSize() / canvas_sb.getScrollScale();
			scale_y = -height / (max_value - min_value) * 0.8f;
			offset_y = -max_value - (max_value - min_value) * 0.1f;
			update_transform();
		}
		
		void arrangeXY() {
			arrangeX();
			arrangeY();
		}
		
		/**********************************************************************\
		*
		* Update
		*
		\**********************************************************************/
		
		/*
		 */
		void update_transform() {
			
			// update transformation
			float scale_x = (width - TRACK_SPACE * 2) / (max_time - min_time);
			transform = translate(TRACK_SPACE,0.0f,0.0f) * scale(scale_x,scale_y,1.0f) * translate(-min_time,offset_y,0.0f);
			
			range_changed = 1;
			curves_changed = 1;
		}
		
		mat4 get_transform() {
			return transform;
		}
		
		/*
		 */
		void update() {
			
			// check canvas
			if(canvas == NULL) return;
			
			// canvas changed
			if(width != canvas.getWidth() || height != canvas.getHeight()) {
				
				width = canvas.getWidth();
				height = canvas.getHeight();
				
				update_transform();
			}
			
			// curves changed
			if(curves_changed == 0) {
				foreach(TrackCurve curve; curves) {
					curves_changed |= curve.keys_changed;
					curve.keys_changed = 0;
				}
			}
			
			// curves changed
			if(curves_changed) {
				curves_changed = 0;
				foreach(TrackCurve curve; curves) {
					curve.create(selected_keys);
				}
			}
			
			// current time
			canvas.clearLinePoints(time_line_id);
			canvas.setLineOrder(time_line_id,order + 1);
			int time_x = TRACK_SPACE + int((width - TRACK_SPACE * 2) * (current_time - min_time) / (max_time - min_time));
			canvas.addLinePoint(time_line_id,vec3(time_x - 1,TRACK_TIME_HEIGHT,0.0f));
			canvas.addLinePoint(time_line_id,vec3(time_x - 1,-TRACK_TIME_HEIGHT,0.0f));
			canvas.addLinePoint(time_line_id,vec3(time_x + 1,-TRACK_TIME_HEIGHT,0.0f));
			canvas.addLinePoint(time_line_id,vec3(time_x + 1,TRACK_TIME_HEIGHT,0.0f));
			
			// range changed
			if(range_changed) {
				int range_from_x = TRACK_SPACE + int((width - TRACK_SPACE * 2) * (range_from - min_time) / (max_time - min_time));
				int range_to_x = TRACK_SPACE + int((width - TRACK_SPACE * 2) * (range_to - min_time) / (max_time - min_time));
				canvas.clearLinePoints(range_line_id[0]);
				canvas.clearLinePoints(range_line_id[1]);
				canvas.addLinePoint(range_line_id[0],vec3(range_from_x - 1,TRACK_TIME_HEIGHT,0.0f));
				canvas.addLinePoint(range_line_id[0],vec3(range_from_x - 1,-TRACK_TIME_HEIGHT,0.0f));
				canvas.addLinePoint(range_line_id[1],vec3(range_to_x + 1,TRACK_TIME_HEIGHT,0.0f));
				canvas.addLinePoint(range_line_id[1],vec3(range_to_x + 1,-TRACK_TIME_HEIGHT,0.0f));
				canvas.clearPolygonPoints(range_polygon_id[0]);
				canvas.clearPolygonPoints(range_polygon_id[1]);
				canvas.addPolygonPoint(range_polygon_id[0],vec3(0.0f,TRACK_TIME_HEIGHT,0.0f));
				canvas.addPolygonPoint(range_polygon_id[0],vec3(range_from_x - 1,TRACK_TIME_HEIGHT,0.0f));
				canvas.addPolygonPoint(range_polygon_id[0],vec3(range_from_x - 1,-TRACK_TIME_HEIGHT,0.0f));
				canvas.addPolygonPoint(range_polygon_id[0],vec3(0.0f,-TRACK_TIME_HEIGHT,0.0f));
				canvas.addPolygonPoint(range_polygon_id[1],vec3(width,TRACK_TIME_HEIGHT,0.0f));
				canvas.addPolygonPoint(range_polygon_id[1],vec3(range_to_x + 1,TRACK_TIME_HEIGHT,0.0f));
				canvas.addPolygonPoint(range_polygon_id[1],vec3(range_to_x + 1,-TRACK_TIME_HEIGHT,0.0f));
				canvas.addPolygonPoint(range_polygon_id[1],vec3(width,-TRACK_TIME_HEIGHT,0.0f));
				range_changed = 0;
			}
			
			// canvas is focused
			if(canvas.isFocused()) {
				
				// remove selected keys
				if(clearKeyState(APP_KEY_DELETE)) {
					removeSelectedKeys();
				}
				
				// move selected keys
				if(current_key != NULL && (getKeyState(APP_KEY_LEFT) || getKeyState(APP_KEY_RIGHT))) {
					float time = current_key.getTime();
					float value = current_key.getValue();
					float step = (max_time - min_time) / 100.0f;
					if(clearKeyState(APP_KEY_LEFT)) moveSelectedKeys(time - step,value);
					if(clearKeyState(APP_KEY_RIGHT)) moveSelectedKeys(time + step,value);
				}
			}
			
			// mouse cursor
			int mouse_x = canvas.getMouseX();
			int mouse_y = canvas.getMouseY();
			int id = canvas.getPolygonIntersection(mouse_x,mouse_y);
			if(id != -1) {
				foreach(TrackCurve curve; curves) {
					if(curve.isPolygon(id)) {
						setMouseCursor(CURSOR_MOVE);
						break;
					}
				}
			}
		}
		
		/**********************************************************************\
		*
		* Callbacks
		*
		\**********************************************************************/
		
		/*
		 */
		void setCallback(int callback,string function,int data_0 = 0,int data_1 = 0) {
			callbacks[callback] = function;
			callbacks_data_0[callback] = data_0;
			callbacks_data_1[callback] = data_1;
			callbacks_enabled[callback] = 1;
		}
		
		void runCallback(int callback) {
			string function = callbacks[callback];
			if(function != NULL && callbacks_enabled[callback]) {
				callbacks_enabled[callback] = 0;
				if(is_function(function)) call(function);
				else if(is_function(function,1)) call(function,callbacks_data_0[callback]);
				else if(is_function(function,2)) call(function,callbacks_data_0[callback],callbacks_data_1[callback]);
				else log.error("Unigine::Widgets::Track::runCallback(): can't find \"%s\" callback function\n",function);
				callbacks_enabled[callback] = 1;
			}
		}
		
		void setCallbackEnabled(int callback,int enable) {
			callbacks_enabled[callback] = enable;
		}
		
		/*
		 */
		void arrange_x_clicked() {
			arrangeX();
		}
		
		void arrange_y_clicked() {
			arrangeY();
		}
		
		void arrange_xy_clicked() {
			arrangeXY();
		}
		
		/*
		 */
		void key_remove_clicked() {
			removeSelectedKeys();
		}
		
		/*
		 */
		void key_type_clicked(int type) {
			
			// update icons
			forloop(int i = 0; TRACK_NUM_TYPES) {
				key_types_i[i].setCallbackEnabled(CLICKED,0);
				key_types_i[i].setToggled((i == type));
				key_types_i[i].setCallbackEnabled(CLICKED,1);
			}
			
			// selected keys
			forloop(int i = 0; selected_keys.size()) {
				TrackCurve curve = selected_curves[i];
				TrackKey key = selected_keys[i];
				curve.setKeyType(key,type);
			}
			updateSelectedKeys();
		}
		
		/*
		 */
		void time_pressed() {
			
			// update times
			float time = clamp(float(time_el.getText()),min_time,max_time);
			forloop(int i = 0; selected_keys.size()) {
				selected_keys[i].setTime(time);
			}
			updateSelectedKeys();
		}
		
		void value_pressed() {
			
			// update values
			float value = float(value_el.getText());
			forloop(int i = 0; selected_keys.size()) {
				float min_value = selected_curves[i].getMinValue();
				float max_value = selected_curves[i].getMaxValue();
				selected_keys[i].setValue(clamp(value,min_value,max_value));
			}
			updateSelectedKeys();
		}
		
		/*
		 */
		void pressed_callback() {
			
			/////////////////////////////////
			// left mouse button
			/////////////////////////////////
			
			if(getMouseButton() & APP_BUTTON_LEFT) {
				
				float mouse_time;
				float mouse_value;
				
				float mouse_step_x;
				float mouse_step_y;
				
				int old_mouse_x;
				int old_mouse_y;
				
				int mouse_x = canvas.getMouseX();
				int mouse_y = canvas.getMouseY();
				
				if(pressed == 0) {
					
					pressed = 1;
					
					// save values
					old_mouse_x = mouse_x;
					old_mouse_y = mouse_y;
					
					// polygon intersection
					int id = canvas.getPolygonIntersection(mouse_x,mouse_y);
					if(id != -1) {
						
						// find selected key
						foreach(TrackCurve curve; curves) {
							foreach(TrackKey key; curve.keys) {
								if(key.isPolygon(id)) {
									
									// key is selected
									if(key.isKeyPolygon(id)) {
										mouse_time = key.getTime();
										mouse_value = key.getValue();
										pressed = 2;
									}
									// left handle is selected
									else if(key.isLeftPolygon(id)) {
										vec4 handles = key.getHandles();
										mouse_time = key.getTime() + handles.x;
										mouse_value = key.getValue() + handles.y;
										pressed = 3;
									}
									// right handle is selected
									else if(key.isRightPolygon(id)) {
										vec4 handles = key.getHandles();
										mouse_time = key.getTime() + handles.z;
										mouse_value = key.getValue() + handles.w;
										pressed = 4;
									}
									
									// current key
									setCurrentCurve(curve);
									setCurrentKey(key);
									
									// selected keys
									if(selected_keys.find(key) == -1) {
										if(getKeyState(APP_KEY_CTRL)) {
											selected_curves.append(curve);
											selected_keys.append(key);
										} else {
											setSelectedCurves((curve));
											setSelectedKeys((key));
										}
									}
									
									// clone selected keys
									if(getKeyState(APP_KEY_SHIFT)) {
										forloop(int i = 0; selected_keys.size()) {
											TrackKey key = selected_keys[i];
											TrackCurve curve = selected_curves[i];
											TrackKey clone = curve.cloneKey(key);
											if(current_key == key) setCurrentKey(clone);
											selected_keys[i] = clone;
											clone.setOrder(order);
										}
										runCallback(TRACK_KEY_CREATED);
									}
									
									// update keys
									updateSelectedKeys();
									
									return;
								}
							}
						}
					}
					
					// raise selection
					canvas.setPolygonOrder(selection_polygon_id,order++);
					canvas.setLineOrder(selection_line_id,order++);
					
					// selected keys
					if(getKeyState(APP_KEY_CTRL)) {
						old_selected_curves.copy(selected_curves);
						old_selected_keys.copy(selected_keys);
					} else {
						old_selected_curves.clear();
						old_selected_keys.clear();
						selected_curves.clear();
						selected_keys.clear();
					}
					curves_changed = 1;
				}
				
				else {
					
					int mouse_dx = getMouseDX();
					int mouse_dy = getMouseDY();
					
					float ifps = engine.game.getIFps() * TRACK_CANVAS_SPEED;
					
					// move canvas vertically
					if(mouse_y < 0) mouse_step_y -= ifps * mouse_y;
					else if(mouse_y >= canvas.getHeight()) mouse_step_y += ifps * (canvas.getHeight() - mouse_y);
					else mouse_step_y = 0.0f;
					if(abs(mouse_step_y) > 1.0f) {
						int step = int(mouse_step_y);
						mouse_step_y -= step;
						offset_y += step / scale_y;
						old_mouse_y += step;
						mouse_dy -= step;
						update_transform();
					}
					
					// move canvas horizontally
					int scroll_scale = canvas_sb.getScrollScale();
					int hscroll_value = canvas_sb.getHScrollValue();
					int hscroll_frame_size = canvas_sb.getHScrollFrameSize();
					if(mouse_x < hscroll_value / scroll_scale && hscroll_value > 0) mouse_step_x -= ifps * (mouse_x - hscroll_value / scroll_scale);
					else if(mouse_x >= (hscroll_value + hscroll_frame_size) / scroll_scale && hscroll_value < canvas_sb.getHScrollObjectSize() - hscroll_frame_size) mouse_step_x += ifps * ((hscroll_value + hscroll_frame_size) / scroll_scale - mouse_x);
					else mouse_step_x = 0.0f;
					if(abs(mouse_step_x) > 1.0f) {
						int step = int(mouse_step_x);
						mouse_step_x -= step;
						canvas_sb.setHScrollValue(hscroll_value - step * scroll_scale);
						mouse_dx -= step;
					}
					
					// mouse movement
					vec3 mouse = rotation(inverse(transform)) * vec3(mouse_dx,mouse_dy,0.0f);
					mouse_time += mouse.x;
					mouse_value += mouse.y;
					
					// select keys
					if(pressed == 1) {
						
						vec3 p0 = vec3(min(old_mouse_x,mouse_x),min(old_mouse_y,mouse_y),0.0f);
						vec3 p1 = vec3(max(old_mouse_x,mouse_x),min(old_mouse_y,mouse_y),0.0f);
						vec3 p2 = vec3(max(old_mouse_x,mouse_x),max(old_mouse_y,mouse_y),0.0f);
						vec3 p3 = vec3(min(old_mouse_x,mouse_x),max(old_mouse_y,mouse_y),0.0f);
						
						// selection line
						canvas.clearLinePoints(selection_line_id);
						canvas.addLinePoint(selection_line_id,p0);
						canvas.addLinePoint(selection_line_id,p1);
						canvas.addLinePoint(selection_line_id,p2);
						canvas.addLinePoint(selection_line_id,p3);
						canvas.addLinePoint(selection_line_id,p0);
						
						// selection polygon
						canvas.clearPolygonPoints(selection_polygon_id);
						canvas.addPolygonPoint(selection_polygon_id,p0);
						canvas.addPolygonPoint(selection_polygon_id,p1);
						canvas.addPolygonPoint(selection_polygon_id,p2);
						canvas.addPolygonPoint(selection_polygon_id,p3);
						
						// find selected keys
						selected_curves.clear();
						selected_keys.clear();
						foreach(TrackCurve curve; curves) {
							foreach(TrackKey key; curve.keys) {
								vec3 p = transform * vec3(key.getTime(),key.getValue(),0.0f);
								if(p.x < p0.x || p.y < p0.y) continue;
								if(p.x > p2.x || p.y > p2.y) continue;
								selected_curves.append(curve);
								selected_keys.append(key);
								key.setOrder(order);
							}
						}
						forloop(int i = 0; old_selected_keys.size()) {
							if(selected_keys.find(old_selected_keys[i]) != -1) continue;
							selected_curves.append(old_selected_curves[i]);
							selected_keys.append(old_selected_keys[i]);
						}
						curves_changed = 1;
					}
					// move keys
					else if(pressed == 2) {
						
						// snap time
						float time = mouse_time;
						float threshold = TRACK_THRESHOLD * (max_time - min_time) / (width - TRACK_SPACE * 2);
						if(abs(time - current_time) < threshold && (key_snap_i.isToggled() || getKeyState(APP_KEY_ALT))) {
							time = current_time;
						}
						
						// move keys
						moveSelectedKeys(time,mouse_value);
					}
					// move left handle
					else if(pressed == 3) {
						
						vec4 handles = current_key.getHandles();
						handles.x = mouse_time - current_key.getTime();
						handles.y = mouse_value - current_key.getValue();
						if(current_key.getType() == TRACK_KEY_SMOOTH) {
							handles.z = -handles.x;
							handles.w = -handles.y;
						}
						if(current_key.getHandles() != handles) {
							current_key.setHandles(handles);
							updateSelectedKeys();
						}
					}
					// move right handle
					else if(pressed == 4) {
						
						vec4 handles = current_key.getHandles();
						handles.z = mouse_time - current_key.getTime();
						handles.w = mouse_value - current_key.getValue();
						if(current_key.getType() == TRACK_KEY_SMOOTH) {
							handles.x = -handles.z;
							handles.y = -handles.w;
						}
						if(current_key.getHandles() != handles) {
							current_key.setHandles(handles);
							updateSelectedKeys();
						}
					}
				}
			}
			
			/////////////////////////////////
			// middle mouse button
			/////////////////////////////////
			
			if(getMouseButton() & APP_BUTTON_MIDDLE) {
				
				if(pressed == 0) {
					
					pressed = 5;
				}
				
				// move canvas
				if(pressed == 5) {
					
					int mouse_dx = getMouseDX();
					int mouse_dy = getMouseDY();
					
					// move canvas
					offset_y += mouse_dy / scale_y;
					update_transform();
					
					// move scrollbox
					canvas_sb.setHScrollValue(canvas_sb.getHScrollValue() - mouse_dx * canvas_sb.getScrollScale());
				}
			}
			
			/////////////////////////////////
			// right mouse button
			/////////////////////////////////
			
			if(getMouseButton() & APP_BUTTON_RIGHT) {
				
				float old_scale_y;
				int old_mouse_x;
				int old_mouse_y;
				vec3 old_mouse;
				
				int width;
				int old_width;
				int old_offset_x;
				
				if(pressed == 0) {
					
					pressed = 6;
					
					// save values
					old_scale_y = scale_y;
					old_mouse_x = canvas.getMouseX();
					old_mouse_y = canvas.getMouseY();
					old_mouse = inverse(transform) * vec3(old_mouse_x,old_mouse_y,0.0f);
					
					// save scrollbox values
					width = canvas.getWidth();
					old_width = canvas.getWidth();
					old_offset_x = canvas_sb.getMouseX();
				}
				
				// scale canvas
				else if(pressed == 6) {
					
					int mouse_dy = getMouseDY();
					
					float scale = (1.0f + mouse_dy * 0.01f);
					
					// horizontal scale
					width = clamp(width * scale,canvas_sb.getHScrollFrameSize() / canvas_sb.getScrollScale(),TRACK_CANVAS_WIDTH);
					canvas.setWidth(width);
					
					// vertical scale
					scale_y = old_scale_y * (width / old_width);
					update_transform();
					
					// update offset
					vec3 mouse = inverse(transform) * vec3(old_mouse_x,old_mouse_y,0.0f);
					offset_y += mouse.y - old_mouse.y;
					
					// update scrollbar
					canvas_sb.arrange();
					canvas_sb.setHScrollValue((float(old_mouse_x) * width / old_width - old_offset_x) * canvas_sb.getScrollScale());
				}
			}
		}
		
		/*
		 */
		void released_callback() {
			
			// clear selection
			canvas.clearLinePoints(selection_line_id);
			canvas.clearPolygonPoints(selection_polygon_id);
			
			// selected keys
			if(pressed == 1) {
				if(selected_keys.size()) {
					setCurrentCurve(selected_curves[0]);
					setCurrentKey(selected_keys[0]);
				} else {
					setCurrentCurve(NULL);
					setCurrentKey(NULL);
				}
				updateSelectedKeys();
			}
			
			pressed = 0;
		}
		
		/*
		 */
		void double_clicked_callback() {
			
			int mouse_x = canvas.getMouseX();
			int mouse_y = canvas.getMouseY();
			
			// line intersection
			int id = canvas.getLineIntersection(mouse_x,mouse_y,TRACK_LINE_DISTANCE);
			if(id != -1) {
				
				// find selected curve
				foreach(TrackCurve curve; curves) {
					if(curve.isLine(id)) {
						
						// mouse position
						vec3 mouse = inverse(transform) * vec3(mouse_x,mouse_y,0.0f);
						
						// create key
						TrackKey key = new TrackKey(mouse.x,mouse.y);
						curve.addKey(key);
						
						// set key type
						TrackKey prev = curve.getPrevKey(key);
						if(prev != NULL) curve.setKeyType(key,prev.getType());
						
						// current key
						setCurrentCurve(curve);
						setCurrentKey(key);
						
						// selected keys
						setSelectedCurves((curve));
						setSelectedKeys((key));
						
						// update keys
						updateSelectedKeys();
						
						// run callback
						runCallback(TRACK_KEY_CREATED);
						
						pressed = 7;
						return;
					}
				}
			}
			
			pressed = 0;
		}
		
		/*
		 */
		void callback_redirector(Track track,string name) {
			track.call(name);
		}
		
		void callback_redirector_1(Track track,string name,int data) {
			track.call(name,data);
		}
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_TRACK_H__ */
