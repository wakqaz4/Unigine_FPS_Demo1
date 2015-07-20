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


#ifndef __UNIGINE_WIDGET_TIME_H__
#define __UNIGINE_WIDGET_TIME_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	enum {
		
		TIME_CHANGED = 0,				// time changed callback
		TIME_RANGE_FROM_CHANGED,		// range from changed callback
		TIME_RANGE_TO_CHANGED,			// range to changed callback
		TIME_NUM_CALLBACKS,
		
		TIME_WIDTH = 256,				// time width
		TIME_HEIGHT = 24,				// time height
		TIME_THRESHOLD = 8,				// time threshold
		
		TIME_HANDLE_ARROW = 6,			// handle arrow
		TIME_HANDLE_WIDTH = 12,			// handle width
		TIME_HANDLE_HEIGHT = 8,			// handle height
		
		TIME_BUTTON_WIDTH = 10,			// button width
		TIME_BUTTON_HEIGHT = 7,			// button height
	};
	
	/*
	 */
	vec4 time_contour_color = vec4(0.0f,0.0f,0.0f,1.0f);
	vec4 time_range_color = vec4(0.5f,0.5f,0.5f,0.5f);
	vec4 time_button_color = vec4(0.8f,0.8f,0.8f,1.0f);
	
	/**************************************************************************\
	*
	* Time
	*
	\**************************************************************************/
	
	/*
	 */
	class Time : Widget {
		
		int width;							// time width
		int height;							// time height
		
		float min_time;						// minimum time
		float max_time;						// maximum time
		
		float current_time;					// current time
		float range_from;					// range from
		float range_to;						// range to
		
		float snap_time;					// next time
		
		int pressed;						// pressed flag
		
		int mouse_offset;					// mouse offset
		
		int time_changed;					// time changed flag
		
		int contour_id;						// contour identifier
		int range_contour_id;				// range contour identifier
		int range_polygon_id;				// range polygon identifier
		int range_from_contour_id;			// range from contour identifier
		int range_from_polygon_id;			// range from polygon identifier
		int range_to_contour_id;			// range to contour identifier
		int range_to_polygon_id;			// range to polygon identifier
		int button_contour_id;				// button contour identifier
		int button_polygon_id;				// button polygon identifier
		
		string callbacks[TIME_NUM_CALLBACKS];
		int callbacks_data_0[TIME_NUM_CALLBACKS];
		int callbacks_data_1[TIME_NUM_CALLBACKS];
		int callbacks_enabled[TIME_NUM_CALLBACKS];
		
		WidgetCanvas canvas;
		
		/*
		 */
		Time(float min,float max) {
			
			min_time = min;
			max_time = max;
			
			current_time = min;
			range_from = min;
			range_to = max;
			
			snap_time = INFINITY;
			
			canvas = new WidgetCanvas(engine.getGui());
			
			// canvas parameters
			canvas.setWidth(TIME_WIDTH);
			canvas.setHeight(TIME_HEIGHT);
			
			// create contour
			contour_id = canvas.addLine(0);
			canvas.setLineColor(contour_id,time_contour_color);
			
			// create range
			range_contour_id = canvas.addLine(2);
			range_polygon_id = canvas.addPolygon(1);
			canvas.setLineColor(range_contour_id,time_contour_color);
			canvas.setPolygonColor(range_polygon_id,time_range_color);
			
			// create range from
			range_from_contour_id = canvas.addLine(6);
			range_from_polygon_id = canvas.addPolygon(5);
			canvas.setLineColor(range_from_contour_id,time_contour_color);
			canvas.setPolygonColor(range_from_polygon_id,vec4(time_range_color.xyz,1.0f));
			
			// create range to
			range_to_contour_id = canvas.addLine(8);
			range_to_polygon_id = canvas.addPolygon(7);
			canvas.setLineColor(range_to_contour_id,time_contour_color);
			canvas.setPolygonColor(range_to_polygon_id,vec4(time_range_color.xyz,1.0f));
			
			// create button
			button_contour_id = canvas.addLine(4);
			button_polygon_id = canvas.addPolygon(3);
			canvas.setLineColor(button_contour_id,time_contour_color);
			canvas.setPolygonColor(button_polygon_id,time_button_color);
			
			// set callbacks
			canvas.setCallback(PRESSED,functionid(callback_redirector),this,functionid(pressed_callback));
			canvas.setCallback(RELEASED,functionid(callback_redirector),this,functionid(released_callback));
			
			widget = canvas;
		}
		
		~Time() {
			
			delete canvas;
		}
		
		/*
		 */
		void setMinTime(float time) {
			min_time = time;
			time_changed = 1;
		}
		
		float getMinTime() {
			return min_time;
		}
		
		/*
		 */
		void setMaxTime(float time) {
			max_time = time;
			time_changed = 1;
		}
		
		float getMaxTime() {
			return max_time;
		}
		
		/*
		 */
		void setTime(float time) {
			current_time = clamp(time,range_from,range_to);
			time_changed = 1;
		}
		
		float getTime() {
			return current_time;
		}
		
		/*
		 */
		void setRangeFrom(float time) {
			range_from = clamp(time,min_time,max_time);
			time_changed = 1;
			updateTime();
		}
		
		float getRangeFrom() {
			return range_from;
		}
		
		/*
		 */
		void setRangeTo(float time) {
			range_to = clamp(time,min_time,max_time);
			time_changed = 1;
			updateTime();
		}
		
		float getRangeTo() {
			return range_to;
		}
		
		/*
		 */
		void setSnapTime(float time) {
			snap_time = time;
		}
		
		float getSnapTime() {
			return snap_time;
		}
		
		/**********************************************************************\
		*
		* Update
		*
		\**********************************************************************/
		
		/*
		 */
		void updateTime() {
			if(current_time != clamp(current_time,range_from,range_to)) {
				setTime(clamp(current_time,range_from,range_to));
				runCallback(TIME_CHANGED);
			}
		}
		
		/*
		 */
		void create_handle(int position_x,int contour_id,int polygon_id) {
			
			// handle points
			vec3 p0 = vec3(position_x - TIME_HANDLE_WIDTH / 2,height - TIME_HANDLE_HEIGHT,0.0f);
			vec3 p1 = vec3(position_x,height - TIME_HANDLE_HEIGHT - TIME_HANDLE_ARROW,0.0f);
			vec3 p2 = vec3(position_x + TIME_HANDLE_WIDTH / 2,height - TIME_HANDLE_HEIGHT,0.0f);
			vec3 p3 = vec3(position_x + TIME_HANDLE_WIDTH / 2,height - 1,0.0f);
			vec3 p4 = vec3(position_x - TIME_HANDLE_WIDTH / 2,height - 1,0.0f);
			
			// create line
			canvas.clearLinePoints(contour_id);
			canvas.addLinePoint(contour_id,p0);
			canvas.addLinePoint(contour_id,p1);
			canvas.addLinePoint(contour_id,p2);
			canvas.addLinePoint(contour_id,p3);
			canvas.addLinePoint(contour_id,p4);
			canvas.addLinePoint(contour_id,p0);
			
			// create polygon
			canvas.clearPolygonPoints(polygon_id);
			canvas.addPolygonPoint(polygon_id,p0);
			canvas.addPolygonPoint(polygon_id,p1);
			canvas.addPolygonPoint(polygon_id,p2);
			canvas.addPolygonPoint(polygon_id,p3);
			canvas.addPolygonPoint(polygon_id,p4);
		}
		
		/*
		 */
		void update() {
			
			// check canvas
			if(canvas == NULL) return;
			
			// canvas changed
			if(time_changed || width != canvas.getWidth() || height != canvas.getHeight()) {
				
				width = canvas.getWidth();
				height = canvas.getHeight();
				
				// create contour
				canvas.clearLinePoints(contour_id);
				canvas.addLinePoint(contour_id,vec3(TIME_HANDLE_WIDTH / 2,TIME_HANDLE_HEIGHT,0.0f));
				canvas.addLinePoint(contour_id,vec3(width - TIME_HANDLE_WIDTH / 2,TIME_HANDLE_HEIGHT,0.0f));
				canvas.addLinePoint(contour_id,vec3(width - TIME_HANDLE_WIDTH / 2,height - TIME_HANDLE_HEIGHT,0.0f));
				canvas.addLinePoint(contour_id,vec3(TIME_HANDLE_WIDTH / 2,height - TIME_HANDLE_HEIGHT,0.0f));
				canvas.addLinePoint(contour_id,vec3(TIME_HANDLE_WIDTH / 2,TIME_HANDLE_HEIGHT,0.0f));
				
				// coordinates
				int current_time_x = int(TIME_HANDLE_WIDTH / 2 + (width - TIME_HANDLE_WIDTH) * (current_time - min_time) / (max_time - min_time));
				int range_from_x = int(TIME_HANDLE_WIDTH / 2 + (width - TIME_HANDLE_WIDTH) * (range_from - min_time) / (max_time - min_time));
				int range_to_x = int(TIME_HANDLE_WIDTH / 2 + (width - TIME_HANDLE_WIDTH) * (range_to - min_time) / (max_time - min_time));
				
				// range points
				vec3 p0 = vec3(range_from_x,TIME_HANDLE_HEIGHT,0.0f);
				vec3 p1 = vec3(range_to_x,TIME_HANDLE_HEIGHT,0.0f);
				vec3 p2 = vec3(range_to_x,height - TIME_HANDLE_HEIGHT,0.0f);
				vec3 p3 = vec3(range_from_x,height - TIME_HANDLE_HEIGHT,0.0f);
				
				// create range contour
				canvas.clearLinePoints(range_contour_id);
				canvas.addLinePoint(range_contour_id,p0);
				canvas.addLinePoint(range_contour_id,p1);
				canvas.addLinePoint(range_contour_id,p2);
				canvas.addLinePoint(range_contour_id,p3);
				canvas.addLinePoint(range_contour_id,p0);
				
				// create range polygon
				canvas.clearPolygonPoints(range_polygon_id);
				canvas.addPolygonPoint(range_polygon_id,p0);
				canvas.addPolygonPoint(range_polygon_id,p1);
				canvas.addPolygonPoint(range_polygon_id,p2);
				canvas.addPolygonPoint(range_polygon_id,p3);
				canvas.addPolygonPoint(range_polygon_id,p0);
				
				// create range handles
				create_handle(range_from_x,range_from_contour_id,range_from_polygon_id);
				create_handle(range_to_x,range_to_contour_id,range_to_polygon_id);
				
				// button points
				p0 = vec3(current_time_x - TIME_BUTTON_WIDTH / 2,TIME_HANDLE_HEIGHT - TIME_BUTTON_HEIGHT,0.0f);
				p1 = vec3(current_time_x + TIME_BUTTON_WIDTH / 2,TIME_HANDLE_HEIGHT - TIME_BUTTON_HEIGHT,0.0f);
				p2 = vec3(current_time_x + TIME_BUTTON_WIDTH / 2,height - TIME_HANDLE_HEIGHT + TIME_BUTTON_HEIGHT,0.0f);
				p3 = vec3(current_time_x - TIME_BUTTON_WIDTH / 2,height - TIME_HANDLE_HEIGHT + TIME_BUTTON_HEIGHT,0.0f);
				
				// create button contour
				canvas.clearLinePoints(button_contour_id);
				canvas.addLinePoint(button_contour_id,p0);
				canvas.addLinePoint(button_contour_id,p1);
				canvas.addLinePoint(button_contour_id,p2);
				canvas.addLinePoint(button_contour_id,p3);
				canvas.addLinePoint(button_contour_id,p0);
				
				// create button polygon
				canvas.clearPolygonPoints(button_polygon_id);
				canvas.addPolygonPoint(button_polygon_id,p0);
				canvas.addPolygonPoint(button_polygon_id,p1);
				canvas.addPolygonPoint(button_polygon_id,p2);
				canvas.addPolygonPoint(button_polygon_id,p3);
				canvas.addPolygonPoint(button_polygon_id,p0);
			}
			
			// canvas is focused
			if(canvas.isFocused()) {
				
				// change current time
				if(getKeyState(APP_KEY_LEFT) || getKeyState(APP_KEY_RIGHT)) {
					float step = (max_time - min_time) / 100.0f;
					if(clearKeyState(APP_KEY_LEFT)) setTime(current_time - step);
					if(clearKeyState(APP_KEY_RIGHT)) setTime(current_time + step);
					runCallback(TIME_CHANGED);
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
				if(is_function(function,0)) call(function);
				else if(is_function(function,1)) call(function,callbacks_data_0[callback]);
				else if(is_function(function,2)) call(function,callbacks_data_0[callback],callbacks_data_1[callback]);
				else log.error("Unigine::Widgets::Time::runCallback(): can't find \"%s\" callback function\n",function);
				callbacks_enabled[callback] = 1;
			}
		}
		
		void setCallbackEnabled(int callback,int enable) {
			callbacks_enabled[callback] = enable;
		}
		
		/*
		 */
		void pressed_callback() {
			
			/////////////////////////////////
			// left mouse button
			/////////////////////////////////
			
			if(getMouseButton() & APP_BUTTON_LEFT) {
				
				int mouse_x = canvas.getMouseX();
				int mouse_y = canvas.getMouseY();
				
				if(pressed == 0) {
					
					pressed = 1;
					
					// polygon intersection
					int id = canvas.getPolygonIntersection(mouse_x,mouse_y);
					if(id != -1 && id != range_polygon_id) {
						
						// range from
						if(id == range_from_polygon_id) {
							int range_from_x = int(TIME_HANDLE_WIDTH / 2 + (width - TIME_HANDLE_WIDTH) * (range_from - min_time) / (max_time - min_time));
							mouse_offset = mouse_x - range_from_x;
							pressed = 2;
						}
						// range to
						else if(id == range_to_polygon_id) {
							int range_to_x = int(TIME_HANDLE_WIDTH / 2 + (width - TIME_HANDLE_WIDTH) * (range_to - min_time) / (max_time - min_time));
							mouse_offset = mouse_x - range_to_x;
							pressed = 3;
						}
						// button
						else if(id == button_polygon_id) {
							int current_time_x = int(TIME_HANDLE_WIDTH / 2 + (width - TIME_HANDLE_WIDTH) * (current_time - min_time) / (max_time - min_time));
							mouse_offset = mouse_x - current_time_x;
							pressed = 4;
						}
					}
					else {
						mouse_offset = 0;
						pressed = 4;
					}
				}
				
				// snap time
				float time = clamp(min_time + (max_time - min_time) * (mouse_x - mouse_offset - TIME_HANDLE_WIDTH / 2) / (width - TIME_HANDLE_WIDTH),min_time,max_time);
				float threshold = TIME_THRESHOLD * (max_time - min_time) / width;
				if(abs(time - snap_time) < threshold) time = snap_time;
				
				// range from
				if(pressed == 2) {
					if(range_from != time) {
						setRangeFrom(time);
						runCallback(TIME_RANGE_FROM_CHANGED);
						if(range_to < range_from) {
							setRangeTo(range_from + EPSILON);
							runCallback(TIME_RANGE_TO_CHANGED);
						}
					}
				}
				// range to
				else if(pressed == 3) {
					if(range_to != time) {
						setRangeTo(time);
						runCallback(TIME_RANGE_TO_CHANGED);
						if(range_to < range_from) {
							setRangeFrom(range_to - EPSILON);
							runCallback(TIME_RANGE_FROM_CHANGED);
						}
					}
				}
				// button
				else if(pressed == 4) {
					if(current_time != clamp(time,range_from,range_to)) {
						setTime(clamp(time,range_from,range_to));
						runCallback(TIME_CHANGED);
					}
				}
			}
		}
		
		/*
		 */
		void released_callback() {
			
			pressed = 0;
		}
		
		/*
		 */
		void callback_redirector(Time time,string name) {
			time.call(name);
		}
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_TIME_H__ */
