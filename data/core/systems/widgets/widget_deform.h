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


#ifndef __UNIGINE_WIDGET_DEFORM_H__
#define __UNIGINE_WIDGET_DEFORM_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	enum {
		
		DEFORM_CHANGED = 0,				// changed callback
		DEFORM_NUM_CALLBACKS,
		
		DEFORM_SPACE = 16,				// border space
		
		DEFORM_GRID_SIZE = 16,			// grid size
		DEFORM_GRID_POINTS = 256,		// grid points
		
		DEFORM_HANDLE_SIZE = 3,			// handle size
		DEFORM_HANDLE_SELECTION = 6,	// handle selection
	};
	
	/*
	 */
	vec4 deform_grid_color = vec4(1.0f,1.0f,1.0f,1.0f);
	vec4 deform_handle_color = vec4(1.0f,1.0f,0.0f,1.0f);
	vec4 deform_background_color = vec4(0.15f,0.15f,0.15f,1.0f);
	
	/**************************************************************************\
	*
	* Deform
	*
	\**************************************************************************/
	
	/*
	 */
	class Deform : Widget {
		
		int width;						// deform width
		int height;						// deform height
		
		int pressed;					// pressed flag
		int changed;					// changed flag
		
		vec3 points[4];					// points
		vec4 handles[4];				// handles
		
		int points_id[4];				// points id
		int handles_id[8];				// handles id
		
		string callbacks[DEFORM_NUM_CALLBACKS];
		int callbacks_data_0[DEFORM_NUM_CALLBACKS];
		int callbacks_data_1[DEFORM_NUM_CALLBACKS];
		int callbacks_enabled[DEFORM_NUM_CALLBACKS];
		
		WidgetCanvas canvas;
		
		/*
		 */
		Deform(int width,int height) {
			
			// create canvas
			canvas = new WidgetCanvas(engine.getGui());
			canvas.setWidth(width);
			canvas.setHeight(height);
			canvas.setColor(deform_background_color);
			
			// set callbacks
			canvas.setCallback(PRESSED,functionid(callback_redirector),this,functionid(pressed_callback));
			canvas.setCallback(RELEASED,functionid(callback_redirector),this,functionid(released_callback));
			
			clear();
			
			widget = canvas;
		}
		
		~Deform() {
			
			delete canvas;
		}
		
		/*
		 */
		void setPoints(vec3 p[]) {
			points.copy(p);
			changed = 1;
		}
		
		void getPoints(vec3 p[]) {
			p.copy(points);
		}
		
		/*
		 */
		void setHandles(vec4 h[]) {
			handles.copy(h);
			changed = 1;
		}
		
		void getHandles(vec4 h[]) {
			h.copy(handles);
		}
		
		/*
		 */
		void clear() {
			
			changed = 1;
			
			points[0] = vec3(-1.0f,-1.0f,0.0f);
			points[1] = vec3( 1.0f,-1.0f,0.0f);
			points[2] = vec3( 1.0f, 1.0f,0.0f);
			points[3] = vec3(-1.0f, 1.0f,0.0f);
			
			handles[0] = vec4( 0.5f, 0.0f,-0.5f, 0.0f);
			handles[1] = vec4( 0.0f, 0.5f, 0.0f,-0.5f);
			handles[2] = vec4(-0.5f, 0.0f, 0.5f, 0.0f);
			handles[3] = vec4( 0.0f,-0.5f, 0.0f, 0.5f);
		}
		
		/**********************************************************************\
		*
		* Update
		*
		\**********************************************************************/
		
		/*
		 */
		int create_handle(vec3 point,int order) {
			
			// transformation
			mat4 transform = canvas.getTransform();
			mat4 itransform = inverse(transform);
			
			// invese point
			vec3 ipoint = transform * point;
			
			// point contour
			int polygon_id = canvas.addPolygon(2);
			canvas.setPolygonColor(polygon_id,deform_handle_color);
			canvas.addPolygonPoint(polygon_id,ipoint + vec3(-DEFORM_HANDLE_SIZE,-DEFORM_HANDLE_SIZE,0.0f));
			canvas.addPolygonPoint(polygon_id,ipoint + vec3( DEFORM_HANDLE_SIZE,-DEFORM_HANDLE_SIZE,0.0f));
			canvas.addPolygonPoint(polygon_id,ipoint + vec3( DEFORM_HANDLE_SIZE, DEFORM_HANDLE_SIZE,0.0f));
			canvas.addPolygonPoint(polygon_id,ipoint + vec3(-DEFORM_HANDLE_SIZE, DEFORM_HANDLE_SIZE,0.0f));
			canvas.setPolygonTransform(polygon_id,itransform);
			
			// point selection
			int selection_id = canvas.addPolygon(order);
			canvas.setPolygonColor(selection_id,vec4_zero);
			canvas.addPolygonPoint(selection_id,ipoint + vec3(-DEFORM_HANDLE_SELECTION,-DEFORM_HANDLE_SELECTION,0.0f));
			canvas.addPolygonPoint(selection_id,ipoint + vec3( DEFORM_HANDLE_SELECTION,-DEFORM_HANDLE_SELECTION,0.0f));
			canvas.addPolygonPoint(selection_id,ipoint + vec3( DEFORM_HANDLE_SELECTION, DEFORM_HANDLE_SELECTION,0.0f));
			canvas.addPolygonPoint(selection_id,ipoint + vec3(-DEFORM_HANDLE_SELECTION, DEFORM_HANDLE_SELECTION,0.0f));
			canvas.setPolygonTransform(selection_id,itransform);
			return selection_id;
		}
		
		/*
		 */
		void create_handles(vec3 point_0,vec3 point_1,vec4 handle) {
			
			vec3 handle_0 = clamp(point_0 + handle.xy0,-vec3_one,vec3_one);
			vec3 handle_1 = clamp(point_1 + handle.zw0,-vec3_one,vec3_one);
			
			// first line
			int line_id = canvas.addLine(1);
			canvas.setLineColor(line_id,deform_handle_color);
			canvas.addLinePoint(line_id,point_0);
			canvas.addLinePoint(line_id,handle_0);
			
			// second line
			line_id = canvas.addLine(1);
			canvas.setLineColor(line_id,deform_handle_color);
			canvas.addLinePoint(line_id,point_1);
			canvas.addLinePoint(line_id,handle_1);
			
			// handles
			points_id.append(create_handle(point_0,3));
			handles_id.append(create_handle(handle_0,4));
			handles_id.append(create_handle(handle_1,4));
		}
		
		/*
		 */
		void create_points(vec3 p0,vec3 p1,vec3 p2,vec3 p3,vec3 points[]) {
			
			// get bezier point
			vec3 get_point(float k0) {
				float k1 = 1.0f - k0;
				float k00 = k0 * k0;
				float k11 = k1 * k1;
				return p0 * (k11 * k1) + p1 * (3.0f * k11 * k0) + p2 * (3.0f * k00 * k1) + p3 * (k00 * k0);
			}
			
			// curve points
			points.clear();
			forloop(int i = 0; DEFORM_GRID_POINTS) {
				points.append(get_point(float(i) / (DEFORM_GRID_POINTS - 1)));
			}
			
			// curve length
			float lenght = 0.0f;
			forloop(int i = 1; points.size()) {
				lenght += ::length(points[i] - points[i - 1]);
			}
			
			// curve parametrization
			float indices[0];
			float distance = 0.0f;
			indices.append(distance / lenght);
			forloop(int i = 1; points.size()) {
				distance += ::length(points[i] - points[i - 1]);
				indices.append(distance / lenght);
			}
			
			// curve points
			points.clear();
			points.append(p0);
			forloop(int i = 1; DEFORM_GRID_SIZE - 1) {
				float k = float(i) / (DEFORM_GRID_SIZE - 1);
				int index = indices.left(k);
				if(index >= 0 && index + 1 < indices.size()) {
					float t0 = indices[index];
					float t1 = indices[index + 1];
					float k0 = float(index) / indices.size();
					float k1 = float(index + 1) / indices.size();
					k = (k - t0) / (t1 - t0);
					points.append(get_point(lerp(k0,k1,k)));
				}
			}
			points.append(p3);
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
				
				float scale_x = (width - DEFORM_SPACE * 2) / 2.0f;
				float scale_y = -(height - DEFORM_SPACE * 2) / 2.0f;
				mat4 transform = translate(DEFORM_SPACE,DEFORM_SPACE,0.0f) * scale(scale_x,scale_y,1.0f) * translate(1.0f,-1.0f,0.0f);
				canvas.setTransform(transform);
				
				changed = 1;
			}
			
			// update canvas
			if(changed) {
				
				// clear canvas
				canvas.clear();
				
				// create points
				vec3 left_points[0];
				vec3 right_points[0];
				vec3 top_points[0];
				vec3 bottom_points[0];
				create_points(points[0],points[0] + handles[3].zw0,points[3] + handles[3].xy0,points[3],left_points);
				create_points(points[1],points[1] + handles[1].xy0,points[2] + handles[1].zw0,points[2],right_points);
				create_points(points[0],points[0] + handles[0].xy0,points[1] + handles[0].zw0,points[1],top_points);
				create_points(points[3],points[3] + handles[2].zw0,points[2] + handles[2].xy0,points[2],bottom_points);
				
				// create grid
				vec3 point = vec3_zero;
				forloop(int i = 0; DEFORM_GRID_SIZE) {
					int grid_id = canvas.addLine();
					canvas.setLineColor(grid_id,deform_grid_color);
					forloop(int j = 0; DEFORM_GRID_SIZE) {
						point.x = lerp(left_points[i].x,right_points[i].x,float(j) / (DEFORM_GRID_SIZE - 1));
						point.y = lerp(top_points[j].y,bottom_points[j].y,float(i) / (DEFORM_GRID_SIZE - 1));
						canvas.addLinePoint(grid_id,clamp(point,-vec3_one,vec3_one));
					}
					grid_id = canvas.addLine();
					canvas.setLineColor(grid_id,deform_grid_color);
					forloop(int j = 0; DEFORM_GRID_SIZE) {
						point.x = lerp(left_points[j].x,right_points[j].x,float(i) / (DEFORM_GRID_SIZE - 1));
						point.y = lerp(top_points[i].y,bottom_points[i].y,float(j) / (DEFORM_GRID_SIZE - 1));
						canvas.addLinePoint(grid_id,clamp(point,-vec3_one,vec3_one));
					}
				}
				
				// create handles
				points_id.clear();
				handles_id.clear();
				create_handles(points[0],points[1],handles[0]);
				create_handles(points[1],points[2],handles[1]);
				create_handles(points[2],points[3],handles[2]);
				create_handles(points[3],points[0],handles[3]);
				
				changed = 0;
			}
			
			// mouse cursor
			int mouse_x = canvas.getMouseX();
			int mouse_y = canvas.getMouseY();
			int id = canvas.getPolygonIntersection(mouse_x,mouse_y);
			if(id != -1 || pressed >= 2) setMouseCursor(CURSOR_MOVE);
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
				else log.error("Unigine::Widgets::Deform::runCallback(): can't find \"%s\" callback function\n",function);
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
				
				vec3 offset;
				
				// mouse coordinates
				int mouse_x = getMouseX();
				int mouse_y = getMouseY();
				mat4 itransform = inverse(canvas.getTransform());
				vec3 mouse = itransform * vec3(mouse_x,mouse_y,0.0f);
				
				if(pressed == 0) {
					
					pressed = 1;
					
					// polygon intersection
					int id = canvas.getPolygonIntersection(mouse_x,mouse_y);
					if(id != -1) {
						
						// points
						forloop(int i = 0; 4) {
							if(points_id[i] == id) {
								offset = points[i] - mouse;
								pressed = 2 + i;
								break;
							}
						}
						
						// handles
						forloop(int i = 0; 8) {
							if(handles_id[i] == id) {
								if(i == 0) offset = points[0] + handles[0].xy - mouse;
								else if(i == 1) offset = points[1] + handles[0].zw - mouse;
								else if(i == 2) offset = points[1] + handles[1].xy - mouse;
								else if(i == 3) offset = points[2] + handles[1].zw - mouse;
								else if(i == 4) offset = points[2] + handles[2].xy - mouse;
								else if(i == 5) offset = points[3] + handles[2].zw - mouse;
								else if(i == 6) offset = points[3] + handles[3].xy - mouse;
								else if(i == 7) offset = points[0] + handles[3].zw - mouse;
								pressed = 8 + i;
								break;
							}
						}
					}
				}
				else if(pressed != 1) {
					
					mouse = clamp(mouse + offset,-vec3_one,vec3_one);
					
					// points
					if(pressed == 2) points[0] = mouse;
					else if(pressed == 3) points[1] = mouse;
					else if(pressed == 4) points[2] = mouse;
					else if(pressed == 5) points[3] = mouse;
					
					// handles
					else if(pressed == 8) handles[0].xy = mouse - points[0];
					else if(pressed == 9) handles[0].zw = mouse - points[1];
					else if(pressed == 10) handles[1].xy = mouse - points[1];
					else if(pressed == 11) handles[1].zw = mouse - points[2];
					else if(pressed == 12) handles[2].xy = mouse - points[2];
					else if(pressed == 13) handles[2].zw = mouse - points[3];
					else if(pressed == 14) handles[3].xy = mouse - points[3];
					else if(pressed == 15) handles[3].zw = mouse - points[0];
					
					// changed callback
					runCallback(DEFORM_CHANGED);
					
					changed = 1;
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
		void callback_redirector(Deform deform,string name) {
			deform.call(name);
		}
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_DEFORM_H__ */
