/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_widget_manipulator.h
 * Desc:    Unigine object manipulation widgets
 * Version: 1.05
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Alexey Egorov <evilspirit@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

#ifndef __EDITOR_WIDGET_MANIPULATOR_H__
#define __EDITOR_WIDGET_MANIPULATOR_H__

/*
 */
namespace Editor::Widgets {
	
	/**************************************************************************\
	*
	* class WidgetEditorManipulator
	*
	\**************************************************************************/
	
	/*
	 */
	class WidgetEditorManipulator {
			
		private:
			
			enum {
				INPUT_STATE_RELEASED,
				INPUT_STATE_PRESSED,
				INPUT_STATE_HOLDING,
			};
			
			WidgetCanvas canvas;
			
			// settings
			float line_selection_radius = 8.0f;
			
			// variables
			int mask;					// axis mask
			int size;					// handle size
			float step;					// movement step
			vec4 color;					// color scale
			Mat4 basis;					// basis
			Mat4 ibasis;				// inverse basis
			Vec3 axis_x;				// x axis
			Vec3 axis_y;				// y axis
			Vec3 axis_z;				// z axis
			Vec3 position;				// position
			Mat4 transform;				// transformation
			
			mat4 projection;			// projection matrix
			Mat4 modelview;				// modelview matrix
			Mat4 imodelview;			// inverse modelview matrix
			
			Gui gui;
			
			int callbacks[];
			int callbacks_enable[];
			
			int enabled = 1;
			
			int num_lines = 0;
			int num_polygons = 0;
			int num_texts = 0;
			
			void clear_primitives() {
				forloop(int i = 0; canvas.getNumLines()) {
					int id = canvas.getLine(i);
					canvas.clearLinePoints(id);
				}
				forloop(int i = 0; canvas.getNumPolygons()) {
					int id = canvas.getPolygon(i);
					canvas.clearPolygonPoints(id);
				}
				forloop(int i = 0; canvas.getNumTexts()) {
					int id = canvas.getText(i);
					canvas.setTextText(id,"");
				}
				num_lines = 0;
				num_polygons = 0;
				num_texts = 0;
			}
			
			// manipulator scale
			float get_scale(Vec3 point) {
				Vec4 p0 = projection * Vec4(modelview * point,1.0f);
				Vec4 p1 = projection * Vec4(modelview * (point + modelview.m10m11m12),1.0f);
				p0 /= abs(p0.w);
				p1 /= abs(p1.w);
				Scalar x = (p1.x - p0.x) * 0.5f * gui.getWidth();
				Scalar y = (p1.y - p0.y) * 0.5f * gui.getHeight();
				return float(size / length(Vec3(x,y,0.0f)));
			}
			
			// get projection vector
			Vec3 axis_projection(Vec3 axis,Vec3 v) {
				return axis * dot(axis,v);
			}
			
			// project point to screen space
			vec3 project_point(Vec3 point) {
				Vec4 p = projection * Vec4(modelview * point,1.0f);
				p /= abs(p.w);
				float x = float((0.5f + p.x * 0.5f)) * gui.getWidth();
				float y = float((0.5f - p.y * 0.5f)) * gui.getHeight();
				return vec3(x,y,0.0f);
			}
			
			// unproject srceen space point
			void unproject_point(vec3 point,Vec3 &p0,Vec3 &p1) {
				float x = -(point.x / gui.getWidth() * 2.0f - 1.0f + projection.m02) / projection.m00;
				float y = (point.y / gui.getHeight() * 2.0f - 1.0f + projection.m12) / projection.m11;
				if(compare(projection.m32,-1.0f)) {
					p0 = imodelview.m03m13m23;
					p1 = imodelview * Vec3(x,y,1.0f);
				} else {
					p0 = imodelview * Vec3(-x,-y,-1.0f);
					p1 = imodelview * Vec3(-x,-y,1.0f);
				}
			}
			
			// unproject srceen space point to plane
			Vec3 unproject_point_plane(vec3 point,Vec4 plane) {
				Vec3 p0,p1;
				unproject_point(point,p0,p1);
				Vec3 direction = p1 - p0;
				Scalar k = dot(Vec3(plane),direction); 
				Vec3 result = p0 - direction * dot(plane,p0) / k; 
				return result; 
			}
			
			// unproject srceen space point to plane with check 
			int unproject_point_plane(vec3 point,Vec4 plane,Vec3 &result) {
				Vec3 p0,p1;
				unproject_point(point,p0,p1);
				Vec3 direction = p1 - p0;
				Scalar k = dot(Vec3(plane),direction);
				if(abs(k) < 0.001f) return 0;
				Scalar p = dot(plane,p0) / k;
				if(p < 0.0f) return 0;
				result = p0 - direction * p;
				return 1;
			}
			
			Vec3 unproject_point_sphere(vec3 point,Vec3 center,float radius) {
				Vec3 camera = imodelview.m03m13m23;
				Vec3 normal = normalize(camera - center);
				Vec3 p = unproject_point_plane(point,Vec4(normal,-dot(normal,center)));
				float distance = float(length(p - center));
				if(distance < radius) return p + normal * sqrt(radius * radius - distance * distance);
				return p;
			}
			
			// nearest distance to line
			float get_distance(vec3 p0,vec3 p1,vec3 point) {
				vec3 direction = p1.xy0 - p0.xy0;
				return length(p0.xy0 + direction * clamp(dot(point - p0.xy0,direction) / dot(direction,direction),0.0f,1.0f) - point);
			}
			
			void render_line(Vec3 p0,Vec3 p1,vec4 color,int order = 0) {
				int id;
				if(num_lines >= canvas.getNumLines()) {
					id = canvas.addLine(order);
				} else {
					id = canvas.getLine(num_lines);
				}
				num_lines++;
				canvas.setLineOrder(id,order);
				canvas.addLinePoint(id,p0 - imodelview.col33);
				canvas.addLinePoint(id,p1 - imodelview.col33);
				canvas.setLineColor(id,color);
			}
			
			void render_quad(Vec3 p0,Vec3 p1,Vec3 p2,Vec3 p3,vec4 color,int order = 0) {
				int id;
				if(num_polygons >= canvas.getNumPolygons()) {
					id = canvas.addPolygon(order);
				} else {
					id = canvas.getPolygon(num_polygons);
				}
				num_polygons++;
				canvas.setPolygonOrder(id,order);
				canvas.addPolygonPoint(id,p0 - imodelview.col33);
				canvas.addPolygonPoint(id,p1 - imodelview.col33);
				canvas.addPolygonPoint(id,p2 - imodelview.col33);
				canvas.addPolygonPoint(id,p3 - imodelview.col33);
				canvas.setPolygonColor(id,color);
			}
			
			void render_triangle(Vec3 p0,Vec3 p1,Vec3 p2,vec4 color,int order = 0) {
				int id;
				if(num_polygons >= canvas.getNumPolygons()) {
					id = canvas.addPolygon(order);
				} else {
					id = canvas.getPolygon(num_polygons);
				}
				num_polygons++;
				canvas.setPolygonOrder(id,order);
				canvas.addPolygonPoint(id,p0 - imodelview.col33);
				canvas.addPolygonPoint(id,p1 - imodelview.col33);
				canvas.addPolygonPoint(id,p2 - imodelview.col33);
				canvas.setPolygonColor(id,color);
			}
			
			int render_text(Vec3 pos,string caption,vec4 color,mat4 imodelview_projection = mat4_identity) {
				int id;
				if(num_texts >= canvas.getNumTexts()) {
					id = canvas.addText();
				} else {
					id = canvas.getText(num_texts);
				}
				num_texts++;
				canvas.setTextText(id,caption);
				canvas.setTextPosition(id,project_point(pos));
				canvas.setTextColor(id,color);
				canvas.setTextTransform(id,imodelview_projection);
				return id;
			}
			
		public:
			
			// constructor
			WidgetEditorManipulator() {
				gui = engine.getGui();
				canvas = new WidgetCanvas(gui);
				engine.gui.addChild(canvas,GUI_ALIGN_EXPAND | GUI_ALIGN_OVERLAP | GUI_ALIGN_BACKGROUND);
				
				setMask(MANIPULATOR_MASK_XYZ);
				setSize(128);
				setStep(EPSILON);
				setColor(vec4_one);
				setBasis(Mat4_identity);
				transform = Mat4_identity;
				
				forloop(int i = 0; MANIPULATOR_NUM_CALLBACKS) {
					callbacks_enable[i] = 1;
				}
			}
			
			~WidgetEditorManipulator() {
				delete canvas;
			}
			
			// axis mask
			void setMask(int m) { mask = m & MANIPULATOR_MASK_XYZ; }
			int getMask() { return mask; }
			
			// handle size
			void setSize(int s) { size = max(s,0); }
			int getSize() { return size; }
			
			// movement step
			void setStep(float s) { step = max(s,EPSILON); }
			float getStep() { return step; }
			
			// color scale
			void setColor(vec4 c) { color = saturate(c); }
			vec4 getColor() { return color; }
			
			// parent basis
			void setBasis(Mat4 b) {
				basis = b;
				ibasis = inverse(basis);
				axis_x = normalize(basis.m00m10m20);
				axis_y = normalize(basis.m01m11m21);
				axis_z = normalize(basis.m02m12m22);
				arrange();
			}
			
			Mat4 getBasis() {
				return basis;
			}
			
			// current transformation
			void setTransform(Mat4 t) {
				transform = Mat4(t);
				runCallback(MANIPULATOR_CHANGED);
			}
			
			Mat4 getTransform() {
				return transform;
			}
			
			// projection matrix
			void setProjection(mat4 p) {
				projection = p;
				projection.m00 *= float(gui.getHeight()) / gui.getWidth();
			}
			
			mat4 getProjection() {
				return projection;
			}
			
			// modelview matrix
			void setModelview(Mat4 m) {
				modelview = m;
				imodelview = inverse(modelview);
			}
			
			Mat4 getModelview() {
				return modelview;
			}
			
			//snap value
			Scalar snapValue(Scalar value) {
				Scalar remainder = value % Scalar(step);
				if(remainder > step * 0.5f) return value - remainder + step;
				if(remainder < -step * 0.5f) return value - remainder - step;
				return value - remainder;
			}
			
			Vec3 snapVec3(Vec3 value) {
				return Vec3(snapValue(value.x),snapValue(value.y),snapValue(value.z));
			}
			
			Vec3 snapPoint(Vec3 value) {
				return basis * snapVec3(ibasis * value);
			}
			
			// callback
			void setCallback(int callback,int function) {
				assert(function != 0 && "WidgetEditorManipulator::setCallback(): bad id");
				callbacks[callback] = function;
			}
			
			// callback enable
			void setCallbackEnabled(int callback,int enable) {
				callbacks_enable[callback] = enable;
			}
			
			int isCallbackEnabled(int callback) {
				return callbacks_enable[callback];
			}
			
			// callback run
			void runCallback(int callback) {
				if(callbacks_enable.check(callback,0) == 0) return;
				if(callbacks.check(callback) == 0) return;
				
				call(callbacks[callback]);
			}
			
			// camera front
			int isInCameraFront() {
				vec4 point =  projection * vec4(vec3(modelview * transform.m03m13m23),1.0f); 
				return point.w > EPSILON; 
			}
			
			// focus
			int isFocused() {
				return 0;
			}
			
			void clearFocus() {
				
			}
			
			void arrange() { }
			void render() { }
			void checkCallbacks() { }
			
			int isEnabled() {
				return enabled;
			}
			
			void setEnabled(int state) {
				enabled = state;
			}
	};
	
	/**************************************************************************\
	*
	* class WidgetEditorManipulatorTranslator
	*
	\**************************************************************************/
	
	/*
	 */
	class WidgetEditorManipulatorTranslator : WidgetEditorManipulator {
			
		private:
			
			enum {
				AXIS_UNKNOWN = -1,
				AXIS_X,
				AXIS_Y,
				AXIS_Z,
				AXIS_XX,
				AXIS_YY,
				AXIS_ZZ,
				AXIS_YZ,
				AXIS_ZX,
				AXIS_XY,
				NUM_AXIS,
			};
			
			// settings
			int num_arrow_segments = 16;
			float handle_size = 0.3f;
			float grid_mark_size = 0.04f;
			float axis_grid_size = 10.0f;
			float axis_grid_threshold = 0.1f;
			float plane_grid_size = 3.0f;
			float plane_grid_threshold = 0.1f;
			vec4 grid_color = vec4(0.0f,1.0f,1.0f,0.5f);
			
			// variables
			vec3 center;				// center
			float axis_scale;			// scale
			
			float sign_x;				// x sign
			float sign_y;				// y sign
			float sign_z;				// z sign
			
			vec3 axis[NUM_AXIS];		// axis positions
			int current_axis;			// current axis
			int input_state = INPUT_STATE_RELEASED;		// mouse down state for triggering
			
			Mat4 initial_transform;		// initial transformation
			Vec3 initial_position;		// initial mouse position
			Vec3 transform_delta;
			
			int grid_enabled = 1;
			int grid_show_focused = 0;
			int grid_depth_test = 0;
			
			void select_axis() {
				
				current_axis = AXIS_UNKNOWN;
				
				if(panelViewCubeFocused()) return;
				
				// mouse position
				vec3 mouse = vec3(float(gui.getMouseX()),float(gui.getMouseY()),0.0f);
				
				Vec3 axis2_point[3];
				
				axis2_point[0] = unproject_point_plane(mouse,Vec4(axis_z,-dot(axis_z,position)));
				axis2_point[1] = unproject_point_plane(mouse,Vec4(axis_y,-dot(axis_y,position)));
				axis2_point[2] = unproject_point_plane(mouse,Vec4(axis_x,-dot(axis_x,position)));
				
				// single axis selection
				int axis_id[3] = ( AXIS_X, AXIS_Y, AXIS_Z );
				int axis_mask[3] = ( MANIPULATOR_MASK_X, MANIPULATOR_MASK_Y, MANIPULATOR_MASK_Z );
				float min_distance = -1.0f;
				
				forloop(int i = 0; 3) {
					if((mask & axis_mask[i]) == 0) continue;
					float distance = get_distance(center,project_point(axis[axis_id[i]]),mouse);
					if(distance > line_selection_radius) continue;
					if(min_distance >= 0.0f && distance > min_distance) continue;
					current_axis = axis_id[i];
					min_distance = distance;
				}
				
				// plane axis selection
				int axis2_id[3] = ( AXIS_XY, AXIS_ZX, AXIS_YZ );		// two axis id
				int axis2_mask1[3] = ( MANIPULATOR_MASK_X, MANIPULATOR_MASK_Z, MANIPULATOR_MASK_Y );
				int axis2_mask2[3] = ( MANIPULATOR_MASK_Y, MANIPULATOR_MASK_X, MANIPULATOR_MASK_Z );
				Vec3 axis2_dir1[3] = ( axis_x, axis_z, axis_y );		// moving axis one
				Vec3 axis2_dir2[3] = ( axis_y, axis_x, axis_z );		// moving axis two
				float axis2_sign1[3] = ( sign_x, sign_z, sign_y );
				float axis2_sign2[3] = ( sign_y, sign_x, sign_z );
				
				forloop(int i = 0; 3) {
					if((mask & axis2_mask1[i]) == 0 || (mask & axis2_mask2[i]) == 0) continue;
					float dir1 = float(dot(axis2_dir1[i],axis2_point[i] - position) * axis2_sign1[i] / axis_scale);
					float dir2 = float(dot(axis2_dir2[i],axis2_point[i] - position) * axis2_sign2[i] / axis_scale);
					if(dir1 < 0.0f || dir1 > handle_size || dir2 < 0.0f || dir2 > handle_size) continue;
					current_axis = axis2_id[i];
				}
			}
			
		public:
			
			// constructor
			WidgetEditorManipulatorTranslator() {
				
				sign_x = 1.0f;
				sign_y = 1.0f;
				sign_z = 1.0f;
				current_axis = AXIS_UNKNOWN;
			}
			
			~WidgetEditorManipulatorTranslator() {
				
			}
			
			// events
			void checkCallbacks() {
				
				if(enabled == 0 || isInCameraFront() == 0) {
					input_state = INPUT_STATE_RELEASED;
					current_axis = AXIS_UNKNOWN;
					return;
				}
				
				// arrange
				arrange();
				
				// select axis
				if(gui.getMouseGrab() == 0) select_axis();
				
				if(current_axis == AXIS_UNKNOWN) {
					// camera position
					Vec3 camera = imodelview.m03m13m23;
					Vec3 direction = camera - position;
					
					sign_x = sign(float(dot(axis_x,direction)));
					sign_y = sign(float(dot(axis_y,direction)));
					sign_z = sign(float(dot(axis_z,direction)));
					
					initial_transform = transform;
					
					if(input_state != INPUT_STATE_RELEASED) runCallback(MANIPULATOR_RELEASED);
					input_state = INPUT_STATE_RELEASED;
					
					return;
				}
				
				// currently focused widget
				::Widget focus = gui.getFocus();
				
				// take the root widget of gui which is focused when no other widget under mouse pointer
				::Widget viewport_widget = NULL;
				if(engine.gui.getNumChilds() > 0) {
					::Widget child = engine.gui.getChild(0);
					viewport_widget = child.getParent();
				}
				
				if(gui.getMouseGrab() && viewport_widget != NULL && focus == viewport_widget) {
					
					Mat4 old_transform = transform;
					
					if(gui.getMouseButton() & APP_BUTTON_LEFT) {
						// change cursor
						engine.gui.setMouseCursor(GUI_CURSOR_MOVE);
						
						// initial transformation
						transform = initial_transform;
						arrange();
						
						// mouse position
						vec3 mouse = vec3(float(gui.getMouseX()),float(gui.getMouseY()),0.0f);
						
						// camera position
						Vec3 camera = imodelview.m03m13m23;
						Vec3 direction = Vec3(camera - position);
						
						// move along single axis
						int axis_id[3] = ( AXIS_X, AXIS_Y, AXIS_Z );
						Vec3 axis_dir[3] = ( axis_x, axis_y, axis_z );
						
						forloop(int i = 0; 3) {
							if(current_axis != axis_id[i]) continue;
							
							Vec3 normal = normalize(cross(cross(direction,axis_dir[i]),axis_dir[i]));
							Vec3 point; 
							if(unproject_point_plane(mouse,Vec4(normal,-dot(normal,position)),point)) {
								if(input_state == INPUT_STATE_RELEASED) initial_position = point;
								
								transform_delta = axis_projection(axis_dir[i],point - initial_position);
								transform_delta = axis_projection(axis_dir[i],snapPoint(position + transform_delta) - position);
							}
							transform = translate(transform_delta) * transform;
						}
						
						// move along two axis
						int axis2_id[3] = ( AXIS_XY, AXIS_ZX, AXIS_YZ );			// two axis id
						Vec3 axis2_normal[3] = ( axis_z, axis_y, axis_x );			// moving plane normal
						Vec3 axis2_direction1[3] = ( axis_x, axis_z, axis_y );		// moving axis one
						Vec3 axis2_direction2[3] = ( axis_y, axis_x, axis_z );		// moving axis two
						
						forloop(int i = 0; 3) {
							if(current_axis != axis2_id[i]) continue;
							
							Vec3 normal = axis2_normal[i];
							Vec3 point;
							if(unproject_point_plane(mouse,Vec4(normal,-dot(normal,position)),point)) {
								if(input_state == INPUT_STATE_RELEASED) initial_position = point;
								
								transform_delta = axis_projection(axis2_direction1[i],point - initial_position) + axis_projection(axis2_direction2[i],point - initial_position);
								Vec3 snap_delta = snapPoint(position + transform_delta) - position;
								transform_delta = axis_projection(axis2_direction1[i],snap_delta) + axis_projection(axis2_direction2[i],snap_delta);
							}
							transform = translate(transform_delta) * transform; 
						}
						
						// update axis
						arrange();
						
						if(input_state == INPUT_STATE_RELEASED) input_state = INPUT_STATE_PRESSED;
					}
					
					// run callback
					if(transform != old_transform) {
						runCallback(MANIPULATOR_CHANGED);
					}
					
					if(gui.getMouseButton() & APP_BUTTON_DCLICK && current_axis != AXIS_UNKNOWN) {
						gui.setMouseButton((gui.getMouseButton() & ~APP_BUTTON_DCLICK) | APP_BUTTON_LEFT);
						runCallback(MANIPULATOR_DOUBLE_CLICKED);
					}
					
					if(input_state == INPUT_STATE_PRESSED && current_axis != AXIS_UNKNOWN) {
						runCallback(MANIPULATOR_CLICKED);
						input_state = INPUT_STATE_HOLDING;
					}
				}
				else {
					initial_transform = transform;
					transform_delta = Vec3_zero;
					
					if(input_state != INPUT_STATE_RELEASED) runCallback(MANIPULATOR_RELEASED);
					input_state = INPUT_STATE_RELEASED;
				}
			}
			
			// arrange
			void arrange() {
				
				// position
				position = transform.m03m13m23;
				
				// widget center
				center = project_point(position);
				
				// widget axis_scale
				axis_scale = get_scale(position);
				
				// axis positions
				axis[AXIS_X] = position + axis_x * axis_scale;
				axis[AXIS_Y] = position + axis_y * axis_scale;
				axis[AXIS_Z] = position + axis_z * axis_scale;
				axis[AXIS_XX] = position + axis_x * (sign_x * axis_scale * handle_size);
				axis[AXIS_YY] = position + axis_y * (sign_y * axis_scale * handle_size);
				axis[AXIS_ZZ] = position + axis_z * (sign_z * axis_scale * handle_size);
				axis[AXIS_YZ] = position + (axis_y * sign_y + axis_z * sign_z) * (axis_scale * handle_size);
				axis[AXIS_ZX] = position + (axis_x * sign_x + axis_z * sign_z) * (axis_scale * handle_size);
				axis[AXIS_XY] = position + (axis_x * sign_x + axis_y * sign_y) * (axis_scale * handle_size);
			}
			
			// render
			void render() {
				// clear canvas
				clear_primitives();
				
				if(enabled == 0) return;
				
				// setup modelview & projection for canvas
				int width = gui.getWidth();
				int height = gui.getHeight();
				mat4 modelview_projection = scale(width / 2.0f,height / 2.0f,1.0f) * translate(1.0f,1.0f,0.0f) * scale(1.0f,-1.0f,1.0f) * projection * mat4(rotation(modelview));
				mat4 imodelview_projection = inverse(modelview_projection);
				canvas.setTransform(modelview_projection);
				
				// check visibility
				if(isInCameraFront() == 0) return;
				
				// arrange
				arrange();
				
				// colors
				vec4 tcolor = vec4(1.0f,1.0f,0.0f,1.0f);
				vec4 xcolor = vec4(1.0f,0.0f,0.0f,1.0f);
				vec4 ycolor = vec4(0.0f,1.0f,0.0f,1.0f);
				vec4 zcolor = vec4(0.0f,0.0f,1.0f,1.0f);
				
				vec4 xycolor = vec4(0.5f,0.5f,0.5f,0.25f);
				vec4 yzcolor = vec4(0.5f,0.5f,0.5f,0.25f);
				vec4 zxcolor = vec4(0.5f,0.5f,0.5f,0.25f);
				
				if(current_axis == AXIS_X || current_axis == AXIS_XY || current_axis == AXIS_ZX) xcolor = vec4(1.0f,1.0f,0.0f,1.0f);
				if(current_axis == AXIS_Y || current_axis == AXIS_YZ || current_axis == AXIS_XY) ycolor = vec4(1.0f,1.0f,0.0f,1.0f);
				if(current_axis == AXIS_Z || current_axis == AXIS_ZX || current_axis == AXIS_YZ) zcolor = vec4(1.0f,1.0f,0.0f,1.0f);
				if(current_axis == AXIS_YZ) yzcolor = vec4(1.0f,1.0f,0.0f,0.25f);
				if(current_axis == AXIS_ZX) zxcolor = vec4(1.0f,1.0f,0.0f,0.25f);
				if(current_axis == AXIS_XY) xycolor = vec4(1.0f,1.0f,0.0f,0.25f);
				tcolor *= color;
				xcolor *= color;
				ycolor *= color;
				zcolor *= color;
				xycolor *= color;
				yzcolor *= color;
				zxcolor *= color;
				
				// camera position
				Vec3 camera = imodelview.m03m13m23;
				Vec3 ray = Vec3(camera - position);
				
				// single axis grid
				if(grid_enabled && step > EPSILON && (grid_show_focused == 1 || gui.getMouseGrab())) {
					
					Vec3 grid_axis[3] = ( basis.m00m10m20, basis.m01m11m21, basis.m02m12m22 );
					int axis_id[3] = ( AXIS_X, AXIS_Y, AXIS_Z );
					
					forloop(int i = 0; 3) {
						
						if(current_axis != axis_id[i]) continue;
						
						// calculate snapped to grid position
						Vec3 snap_position = axis_projection(grid_axis[i],snapPoint(position) - position) + position;
						
						// calculate binormal
						Vec3 binormal = normalize(cross(ray,grid_axis[i]));
						
						// grid line length which is not depends from distance
						Scalar line_length = axis_grid_size * axis_scale;
						
						// limit minimum line length by constant number of grid marks
						Scalar min_line_length = (length(grid_axis[i]) * step) * 2;
						line_length = max(line_length,min_line_length);
						
						// calculate number of grid marks
						int num_marks = int(floor(line_length / step / length(grid_axis[i])));
						
						// limit grid mark frequency
						Scalar mark_step = step;
						Scalar step_mark_distance = length(grid_axis[i]) * step / axis_scale;
						int step_ratio = 1.0f;
						while(step_mark_distance < axis_grid_threshold) {
							step_mark_distance *= 2.0f;
							mark_step *= 2.0f;
							step_ratio *= 2.0f;
							num_marks = int(num_marks / 2);
						}
						
						// some magic formula to shift grid lines when frequency limited (to prevent grid marks moving)
						Vec3 local_position = ibasis * snap_position;
						Scalar step_shift = floor((local_position[i] % (step * step_ratio)) / step + 0.5f);
						snap_position -= grid_axis[i] * step * step_shift;
						
						// draw main line
						Vec3 p0 = snap_position - normalize(grid_axis[i]) * line_length;
						Vec3 p1 = snap_position + normalize(grid_axis[i]) * line_length;
						
						if(grid_depth_test) {
							engine.visualizer.renderLine3D(p0,p1,grid_color);
						} else {
							render_line(p0,p1,grid_color);
						}
						
						// draw grid marks
						forloop(int j = -num_marks; num_marks + 1) {
							
							Vec3 pos = snap_position + grid_axis[i] * mark_step * j;
							float mark_scale = get_scale(pos);
							p0 = pos + binormal * mark_scale * grid_mark_size;
							p1 = pos - binormal * mark_scale * grid_mark_size;
							
							if(grid_depth_test) {
								engine.visualizer.renderLine3D(p0,p1,grid_color);
							} else {
								render_line(p0,p1,grid_color);
							}
						}
					}
				}
				
				// two axis grid
				if(grid_enabled && step > EPSILON && (grid_show_focused == 1 || gui.getMouseGrab())) {
					
					Vec3 axis_x = basis.m00m10m20;
					Vec3 axis_y = basis.m01m11m21;
					Vec3 axis_z = basis.m02m12m22;
					
					int axis_id[3] = ( AXIS_XY, AXIS_ZX, AXIS_YZ );			// two axis id
					Vec3 grid_axis1[3] = ( axis_x, axis_z, axis_y );		// moving axis one
					Vec3 grid_axis2[3] = ( axis_y, axis_x, axis_z );		// moving axis two
					Vec3 grid_axis_i1[3] = ( 0, 2, 1 );						// moving axis one index
					Vec3 grid_axis_i2[3] = ( 1, 0, 2 );						// moving axis two index
					
					forloop(int i = 0; 3) {
						
						if(current_axis != axis_id[i]) continue;
						
						// min / max axis
						Vec3 min_grid_axis = grid_axis1[i];
						Vec3 max_grid_axis = grid_axis2[i];
						if(length(min_grid_axis) > length(max_grid_axis)) {
							Vec3 temp = min_grid_axis;
							min_grid_axis = max_grid_axis;
							max_grid_axis = temp;
						}
						
						// snapped to grid position
						Vec3 snap_delta = snapPoint(position) - position;
						Vec3 snap_position = axis_projection(grid_axis1[i],snap_delta) + axis_projection(grid_axis2[i],snap_delta) + position;
						
						// grid line length which is not depends from distance
						float line_length = plane_grid_size * axis_scale;
						
						// limit minimum line length by constant number of grid marks
						float min_line_length = (length(max_grid_axis) * step) * 2.0f;
						line_length = max(line_length,min_line_length);
						
						// calculate number of grid marks
						int num_marks1 = int(floor(line_length / step / length(grid_axis1[i])));
						int num_marks2 = int(floor(line_length / step / length(grid_axis2[i])));
						
						// limit grid mark frequency
						Scalar mark_step = step;
						Scalar step_mark_distance = length(min_grid_axis) * step / axis_scale;
						while(step_mark_distance < plane_grid_threshold) {
							step_mark_distance *= 2.0f;
							mark_step *= 2.0f;
							num_marks1 = int(num_marks1 / 2);
							num_marks2 = int(num_marks2 / 2);
						}
						
						// some magic formula to shift grid lines when frequency limited (to prevent grid marks moving)
						Vec3 local_position = ibasis * snap_position;
						
						Vec3 step_shift1 = floor((local_position[grid_axis_i1[i]] % (mark_step)) / step + 0.5f);
						Vec3 step_shift2 = floor((local_position[grid_axis_i2[i]] % (mark_step)) / step + 0.5f);
						
						forloop(int j = -num_marks1; num_marks1 + 1) {
							Vec3 pos = snap_position + grid_axis1[i] * (mark_step * j - step * step_shift1);
							Vec3 p0 = pos + normalize(grid_axis2[i]) * line_length - grid_axis2[i] * step * step_shift2;
							Vec3 p1 = pos - normalize(grid_axis2[i]) * line_length - grid_axis2[i] * step * step_shift2;
							
							if(grid_depth_test) {
								engine.visualizer.renderLine3D(p0,p1,grid_color);
							} else {
								render_line(p0,p1,grid_color);
							}
						}
						
						forloop(int j = -num_marks2; num_marks2 + 1) {
							Vec3 pos = snap_position + grid_axis2[i] * (mark_step * j - step * step_shift2);
							Vec3 p0 = pos + normalize(grid_axis1[i]) * line_length - grid_axis1[i] * step * step_shift1;
							Vec3 p1 = pos - normalize(grid_axis1[i]) * line_length - grid_axis1[i] * step * step_shift1;
							
							if(grid_depth_test) {
								engine.visualizer.renderLine3D(p0,p1,grid_color);
							} else {
								render_line(p0,p1,grid_color);
							}
						}
					}
				}
				
				// transformation
				render_line(initial_transform.m03m13m23,position,tcolor);
				
				// planes
				if((mask & MANIPULATOR_MASK_X) && (mask & MANIPULATOR_MASK_Y)) render_quad(position,axis[AXIS_XX],axis[AXIS_XY],axis[AXIS_YY],xycolor);
				if((mask & MANIPULATOR_MASK_Y) && (mask & MANIPULATOR_MASK_Z)) render_quad(position,axis[AXIS_YY],axis[AXIS_YZ],axis[AXIS_ZZ],yzcolor);
				if((mask & MANIPULATOR_MASK_Z) && (mask & MANIPULATOR_MASK_X)) render_quad(position,axis[AXIS_ZZ],axis[AXIS_ZX],axis[AXIS_XX],zxcolor);
				
				// axis
				if((mask & MANIPULATOR_MASK_X) && (mask & MANIPULATOR_MASK_Y)) {
					render_line(axis[AXIS_XX],axis[AXIS_XY],xycolor.xyz1);
					render_line(axis[AXIS_YY],axis[AXIS_XY],xycolor.xyz1);
				}
				if((mask & MANIPULATOR_MASK_Y) && (mask & MANIPULATOR_MASK_Z)) {
					render_line(axis[AXIS_YY],axis[AXIS_YZ],yzcolor.xyz1);
					render_line(axis[AXIS_ZZ],axis[AXIS_YZ],yzcolor.xyz1);
				}
				if((mask & MANIPULATOR_MASK_Z) && (mask & MANIPULATOR_MASK_X)) {
					render_line(axis[AXIS_XX],axis[AXIS_ZX],zxcolor.xyz1);
					render_line(axis[AXIS_ZZ],axis[AXIS_ZX],zxcolor.xyz1);
				}
				
				if((mask & MANIPULATOR_MASK_X) && current_axis != AXIS_XY && current_axis != AXIS_ZX) {
					render_line(position,axis[AXIS_XX],zxcolor.xyz1);
				}
				
				if((mask & MANIPULATOR_MASK_Y) && current_axis != AXIS_XY && current_axis != AXIS_YZ) {
					render_line(position,axis[AXIS_YY],xycolor.xyz1);
				}
				
				if((mask & MANIPULATOR_MASK_Z) && current_axis != AXIS_YZ && current_axis != AXIS_ZX) {
					render_line(position,axis[AXIS_ZZ],zxcolor.xyz1);
				}
				
				// main axis
				if(mask & MANIPULATOR_MASK_X) render_line(position,axis[AXIS_X],xcolor,1);
				if(mask & MANIPULATOR_MASK_Y) render_line(position,axis[AXIS_Y],ycolor,1);
				if(mask & MANIPULATOR_MASK_Z) render_line(position,axis[AXIS_Z],zcolor,1);
				
				if(current_axis == AXIS_XY) {
					render_line(position,axis[AXIS_XX],xycolor.xyz1);
					render_line(position,axis[AXIS_YY],xycolor.xyz1);
				}
				if(current_axis == AXIS_YZ) {
					render_line(position,axis[AXIS_YY],yzcolor.xyz1);
					render_line(position,axis[AXIS_ZZ],yzcolor.xyz1);
				}
				if(current_axis == AXIS_ZX) {
					render_line(position,axis[AXIS_XX],zxcolor.xyz1);
					render_line(position,axis[AXIS_ZZ],zxcolor.xyz1);
				}
				
				// arrows
				for(int i = 0; i < num_arrow_segments; i++) {
					float s0 = sin(PI2 * (i + 0) / num_arrow_segments);
					float c0 = cos(PI2 * (i + 0) / num_arrow_segments);
					float s1 = sin(PI2 * (i + 1) / num_arrow_segments);
					float c1 = cos(PI2 * (i + 1) / num_arrow_segments);
					
					s0 *= 0.05f;
					c0 *= 0.05f;
					s1 *= 0.05f;
					c1 *= 0.05f;
					
					Vec3 px0 = position + (axis_x * 0.75f + axis_y * s0 + axis_z * c0) * axis_scale;
					Vec3 px1 = position + (axis_x * 0.75f + axis_y * s1 + axis_z * c1) * axis_scale;
					Vec3 py0 = position + (axis_x * s0 + axis_y * 0.75f + axis_z * c0) * axis_scale;
					Vec3 py1 = position + (axis_x * s1 + axis_y * 0.75f + axis_z * c1) * axis_scale;
					Vec3 pz0 = position + (axis_x * s0 + axis_y * c0 + axis_z * 0.75f) * axis_scale;
					Vec3 pz1 = position + (axis_x * s1 + axis_y * c1 + axis_z * 0.75f) * axis_scale;
					
					if(mask & MANIPULATOR_MASK_X) render_triangle(px0,axis[AXIS_X],px1,xcolor);
					if(mask & MANIPULATOR_MASK_Y) render_triangle(py0,axis[AXIS_Y],py1,ycolor);
					if(mask & MANIPULATOR_MASK_Z) render_triangle(pz0,axis[AXIS_Z],pz1,zcolor);
				}
				
				// values
				if(gui.getMouseGrab()) {
					if(current_axis == AXIS_X || current_axis == AXIS_XY || current_axis == AXIS_ZX) {
						float value = dot(axis_x,position - initial_transform.m03m13m23);
						if(abs(value) < 1e-3f) value = 0.0f;
						string text = format("%.3f",value);
						if(value >= 1e-3f) text = "+" + text;
						int id = render_text(axis[AXIS_X],text,vec4_one,imodelview_projection);
						int w = canvas.getTextWidth(id);
						int h = canvas.getTextHeight(id);
						canvas.setTextPosition(id,project_point(axis[AXIS_X]) + (normalize(project_point(axis[AXIS_X]) - center) * float(w) - vec3(float(w),float(h),0.0f)) * 0.5f);
					}
					if(current_axis == AXIS_Y || current_axis == AXIS_YZ || current_axis == AXIS_XY) {
						float value = dot(axis_y,position - initial_transform.m03m13m23);
						if(abs(value) < 1e-3f) value = 0.0f;
						string text = format("%.3f",value);
						if(value >= 1e-3f) text = "+" + text;
						int id = render_text(axis[AXIS_X],text,vec4_one,imodelview_projection);
						int w = canvas.getTextWidth(id);
						int h = canvas.getTextHeight(id);
						canvas.setTextPosition(id,project_point(axis[AXIS_Y]) + (normalize(project_point(axis[AXIS_Y]) - center) * float(w) - vec3(float(w),float(h),0.0f)) * 0.5f);
					}
					if(current_axis == AXIS_Z || current_axis == AXIS_ZX || current_axis == AXIS_YZ) {
						float value = dot(axis_z,position - initial_transform.m03m13m23);
						if(abs(value) < 1e-3f) value = 0.0f;
						string text = format("%.3f",value);
						if(value >= 1e-3f) text = "+" + text;
						int id = render_text(axis[AXIS_X],text,vec4_one,imodelview_projection);
						int w = canvas.getTextWidth(id);
						int h = canvas.getTextHeight(id);
						canvas.setTextPosition(id,project_point(axis[AXIS_Z]) + (normalize(project_point(axis[AXIS_Z]) - center) * float(w) - vec3(float(w),float(h),0.0f)) * 0.5f);
					}
				}
			}
			
			int isFocused() {
				return current_axis != AXIS_UNKNOWN;
			}
			
			void clearFocus() {
				current_axis = AXIS_UNKNOWN;
			}
			
			// grid enable
			int isGridEnabled() {
				return grid_enabled;
			}
			
			void setGridEnabled(int state) {
				grid_enabled = state;
			}
			
			// grid enable
			int isGridShowFocused() {
				return grid_show_focused;
			}
			
			void setGridShowFocused(int state) {
				grid_show_focused = state;
			}
			
			// grid depth test
			int isGridDepthTest() {
				return grid_depth_test;
			}
			
			void setGridDepthTest(int state) {
				grid_depth_test = state;
			}
	};
	
	/**************************************************************************\
	*
	* class WidgetEditorManipulatorRotator
	*
	\**************************************************************************/
	
	/*
	 */
	class WidgetEditorManipulatorRotator : WidgetEditorManipulator {
			
		private:
			
			enum {
				AXIS_UNKNOWN = -1,
				AXIS_T,
				AXIS_X,
				AXIS_Y,
				AXIS_Z,
			};
			
			// settings
			int num_segments = 64;
			int num_selection_segments = 16;
			float axis_mouse_speed = 0.5f;
			float trackball_mouse_speed = 0.5f;
			float rotator_tangent_size = 0.5f;
			
			// variables
			vec3 center;				// center
			float axis_scale;			// scale
			float radius;				// radius
			
			Vec4 clip_plane;			// clip plane
			int current_axis;			// current axis
			Vec3 rotation_axis;			// rotation axis
			
			Mat4 initial_transform;		// initial transformation
			Vec3 initial_position;		// initial mouse position
			Vec3 current_position;		// current mouse position
			
			Vec3 rotation_tangent;
			Vec3 rotation_normal;
			Scalar rotation_angle;
			
			int input_state = INPUT_STATE_RELEASED;		// mouse down state for triggering
			
			void select_axis() {
				
				current_axis = AXIS_UNKNOWN;
				
				if(panelViewCubeFocused()) return;
				
				// mouse position
				vec3 mouse = vec3(float(gui.getMouseX()),float(gui.getMouseY()),0.0f);
				
				// trackball rotation
				if(mask == MANIPULATOR_MASK_XYZ && length(unproject_point_plane(mouse,clip_plane) - position) < radius) {
					initial_position = unproject_point_sphere(mouse,position,radius);
					current_axis = AXIS_T;
				}
				
				// sin/cos table
				float cos_t[num_selection_segments + 1];
				float sin_t[num_selection_segments + 1];
				forloop(int i = 0; num_selection_segments + 1) {
					cos_t[i] = cos(PI2 * i / num_selection_segments);
					sin_t[i] = sin(PI2 * i / num_selection_segments);
				}
				
				// min distance to axis
				float min_distance = -1.0f;
				
				int axis_id[3] = ( AXIS_X, AXIS_Y, AXIS_Z );
				Vec3 axis_dir[3] = ( axis_x, axis_y, axis_z );
				Vec3 axis_dir0[3] = ( axis_y, axis_x, axis_x );
				Vec3 axis_dir1[3] = ( axis_z, axis_z, axis_y );
				int axis_mask[3] = ( MANIPULATOR_MASK_X, MANIPULATOR_MASK_Y, MANIPULATOR_MASK_Z );
				
				forloop(int j = 0; 3) {
					
					if((mask & axis_mask[j]) == 0) continue;
					
					forloop(int i = 0; num_selection_segments) {
						Vec3 p0 = position + (axis_dir0[j] * sin_t[i] + axis_dir1[j] * cos_t[i]) * axis_scale;
						Vec3 p1 = position + (axis_dir0[j] * sin_t[i + 1] + axis_dir1[j] * cos_t[i + 1]) * axis_scale;
						if(dot(clip_plane,p0) < 0.0f && dot(clip_plane,p1) < 0.0f) continue;
						
						float distance = get_distance(project_point(p0),project_point(p1),mouse);
						if(distance > line_selection_radius || min_distance >= 0.0f && distance > min_distance) continue;
						
						rotation_axis = axis_dir[j];
						initial_position = unproject_point_plane(mouse,Vec4(rotation_axis,-dot(rotation_axis,position)));
						current_axis = axis_id[j];
						min_distance = distance;
					}
				}
			}
			
		public:
			
			// constructor
			WidgetEditorManipulatorRotator() {
				current_axis = AXIS_UNKNOWN;
			}
			
			~WidgetEditorManipulatorRotator() {
				
			}
			
			// events
			void checkCallbacks() {
				
				if(enabled == 0 || isInCameraFront() == 0) {
					input_state = INPUT_STATE_RELEASED;
					current_axis = AXIS_UNKNOWN;
					return;
				}
				
				// arrange
				arrange();
				
				// select axis
				if(gui.getMouseGrab() == 0) select_axis();
				
				if(current_axis == AXIS_UNKNOWN) {
					if(input_state != INPUT_STATE_RELEASED) runCallback(MANIPULATOR_RELEASED);
					input_state = INPUT_STATE_RELEASED;
					
					return;
				}
				
				// currently focused widget
				::Widget focus = gui.getFocus();
				
				// take the root widget of gui which is focused when no other widget under mouse pointer
				::Widget viewport_widget = NULL;
				if(engine.gui.getNumChilds() > 0) {
					::Widget child = engine.gui.getChild(0);
					viewport_widget = child.getParent();
				}
				
				if(gui.getMouseGrab() && viewport_widget != NULL && focus == viewport_widget) {
					
					Mat4 old_transform = transform;
					
					if(gui.getMouseButton() & APP_BUTTON_LEFT) {
						
						// change cursor
						engine.gui.setMouseCursor(GUI_CURSOR_ROTATE);
						
						// mouse position
						vec3 mouse = vec3(float(gui.getMouseX()),float(gui.getMouseY()),0.0f);
						
						// initial transformation
						transform = orthonormalize(transform);
						arrange();
						
						// scale decomposition
						mat4 decompose = mat4(inverse(rotation(transform)) * rotation(initial_transform));
						
						// trackball rotation
						if(current_axis == AXIS_T) {
							
							Vec3 axis0 = modelview.m00m01m02;
							Vec3 axis1 = modelview.m10m11m12;
							
							if(input_state == INPUT_STATE_RELEASED) initial_position = mouse;
							
							float angle_x = snapValue((mouse.x - initial_position.x) * trackball_mouse_speed);
							float angle_y = snapValue((mouse.y - initial_position.y) * trackball_mouse_speed);
							
							Mat4 irtransform = inverse(rotation(transform));
							Mat4 rotation_transform = rotate(irtransform * axis1,angle_x) * rotate(inverse(rotate(irtransform * axis1,angle_x)) * irtransform * axis0,angle_y);
							transform = transform * rotation_transform * decompose;
						}
						
						// rotation around axis
						else {
							
							// current mouse position
							current_position = unproject_point_plane(mouse,Vec4(rotation_axis,-dot(rotation_axis,position)));
							
							if(input_state == INPUT_STATE_RELEASED) {
								rotation_normal = normalize(current_position - position);
								rotation_tangent = normalize(cross(rotation_axis,rotation_normal));
							}
							
							Vec3 p0 = project_point(position + rotation_normal * axis_scale).xy0;
							Vec3 p1 = project_point(position + (rotation_normal + rotation_tangent) * axis_scale).xy0;
							Vec3 projected_tangent = normalize(p1 - p0);
							
							if(input_state == INPUT_STATE_RELEASED) initial_position = mouse;
							
							Scalar delta = dot(projected_tangent,mouse - initial_position);
							rotation_angle = snapValue(delta * axis_mouse_speed);
							rotation_angle -= floor(rotation_angle / 360.0f + 0.5f) * 360.0f;
							
							transform = transform * Mat4(rotate(inverse(rotation(transform)) * Vec3(rotation_axis),Scalar(rotation_angle)) * decompose);
						}
						
						if(input_state == INPUT_STATE_RELEASED) input_state = INPUT_STATE_PRESSED;
					}
					
					// run callback
					if(transform != old_transform) {
						runCallback(MANIPULATOR_CHANGED);
					}
					
					if(input_state == INPUT_STATE_PRESSED) {
						runCallback(MANIPULATOR_CLICKED);
						input_state = INPUT_STATE_HOLDING;
					}
					
					if(gui.getMouseButton() & APP_BUTTON_DCLICK && current_axis != AXIS_UNKNOWN) {
						gui.setMouseButton((gui.getMouseButton() & ~APP_BUTTON_DCLICK) | APP_BUTTON_LEFT);
						runCallback(MANIPULATOR_DOUBLE_CLICKED);
					}
				}
				else {
					initial_transform = transform;
					current_position = initial_position;
					
					if(input_state != INPUT_STATE_RELEASED) runCallback(MANIPULATOR_RELEASED);
					input_state = INPUT_STATE_RELEASED;
				}
			}
			
			// arrange
			void arrange() {
				
				// position
				position = transform.m03m13m23;
				
				// widget center
				center = project_point(position);
				
				// widget scale
				axis_scale = get_scale(position);
				
				// visible radius
				radius = axis_scale;
				
				// clip plane
				Vec3 normal = normalize(imodelview.m03m13m23 - position);
				clip_plane = Vec4(normal,-dot(normal,position));
			}
			
			// render
			void render() {
				
				// clear canvas
				clear_primitives();
				
				if(enabled == 0) return;
				
				// setup modelview & projection for canvas
				int width = gui.getWidth();
				int height = gui.getHeight();
				mat4 modelview_projection = scale(width / 2.0f,height / 2.0f,1.0f) * translate(1.0f,1.0f,0.0f) * scale(1.0f,-1.0f,1.0f) * projection * mat4(rotation(modelview));
				mat4 imodelview_projection = inverse(modelview_projection);
				canvas.setTransform(modelview_projection);
				
				// check visibility
				if(isInCameraFront() == 0) return;
				
				// arrange
				arrange();
				
				// colors
				vec4 scolor = vec4(0.5f,0.5f,0.5f,0.25f);
				vec4 acolor = vec4(1.0f,1.0f,0.0f,0.25f);
				vec4 xcolor = vec4(1.0f,0.0f,0.0f,1.0f);
				vec4 ycolor = vec4(0.0f,1.0f,0.0f,1.0f);
				vec4 zcolor = vec4(0.0f,0.0f,1.0f,1.0f);
				vec4 tcolor = vec4(1.0f,1.0f,0.0f,1.0f);
				if(current_axis == AXIS_X) acolor = vec4(1.0f,0.0f,0.0f,0.25f);
				if(current_axis == AXIS_Y) acolor = vec4(0.0f,1.0f,0.0f,0.25f);
				if(current_axis == AXIS_Z) acolor = vec4(0.0f,0.0f,1.0f,0.25f);
				if(current_axis == AXIS_X) xcolor = vec4(1.0f,1.0f,0.0f,1.0f);
				if(current_axis == AXIS_Y) ycolor = vec4(1.0f,1.0f,0.0f,1.0f);
				if(current_axis == AXIS_Z) zcolor = vec4(1.0f,1.0f,0.0f,1.0f);
				scolor *= color;
				acolor *= color;
				xcolor *= color;
				ycolor *= color;
				zcolor *= color;
				
				// silhouette
				Vec3 direction = imodelview.m03m13m23 - position;
				Vec3 d0 = normalize(cross(direction,modelview.m00m10m20)) * radius;
				Vec3 d1 = normalize(cross(direction,d0)) * radius;
				if(current_axis != AXIS_UNKNOWN) {
					for(int i = 0; i < num_segments; i++) {
						float s0 = sin(PI2 * (i + 0) / num_segments);
						float c0 = cos(PI2 * (i + 0) / num_segments);
						float s1 = sin(PI2 * (i + 1) / num_segments);
						float c1 = cos(PI2 * (i + 1) / num_segments);
						Vec3 p0 = position + d0 * s0 + d1 * c0;
						Vec3 p1 = position + d0 * s1 + d1 * c1;
						render_triangle(position,p0,p1,scolor);
					}
				}
				
				for(int i = 0; i < num_segments; i++) {
					float s0 = sin(PI2 * (i + 0) / num_segments);
					float c0 = cos(PI2 * (i + 0) / num_segments);
					float s1 = sin(PI2 * (i + 1) / num_segments);
					float c1 = cos(PI2 * (i + 1) / num_segments);
					Vec3 p0 = position + d0 * s0 + d1 * c0;
					Vec3 p1 = position + d0 * s1 + d1 * c1;
					render_line(p0,p1,scolor.xyz1);
				}
				
				// rotation tangent
				if(gui.getMouseGrab() && current_axis != AXIS_UNKNOWN && current_axis != AXIS_T) {
					Vec3 p0 = position + (rotation_normal + rotation_tangent * rotator_tangent_size) * axis_scale;
					Vec3 p1 = position + (rotation_normal - rotation_tangent * rotator_tangent_size) * axis_scale;
					render_line(p0,p1,tcolor);
				}
				
				// segment angle
				if(gui.getMouseGrab() && current_axis != AXIS_UNKNOWN && current_axis != AXIS_T) {
					
					// normalize directions
					Vec3 d0 = normalize(rotation_normal);
					Vec3 d1 = normalize(rotate(rotation_axis,rotation_angle) * rotation_normal);
					
					// segment
					for(int i = 0; i < 32; i++) {
						Vec3 v0 = rotate(rotation_axis,rotation_angle * (i + 0) / 32) * d0;
						Vec3 v1 = rotate(rotation_axis,rotation_angle * (i + 1) / 32) * d0;
						render_triangle(position,position + v0 * axis_scale,position + v1 * axis_scale,acolor,1);
					}
					
					// directions
					render_line(position,position + d0 * axis_scale,acolor.xyz1);
					render_line(position,position + d1 * axis_scale,acolor.xyz1);
				}
				
				// direction axis
				if(mask & MANIPULATOR_MASK_X) render_line(position,position + axis_x * axis_scale,xcolor);
				if(mask & MANIPULATOR_MASK_Y) render_line(position,position + axis_y * axis_scale,ycolor);
				if(mask & MANIPULATOR_MASK_Z) render_line(position,position + axis_z * axis_scale,zcolor);
				
				// rotation axis
				for(int i = 0; i < num_segments; i++) {
					float s0 = sin(PI2 * (i + 0) / num_segments);
					float c0 = cos(PI2 * (i + 0) / num_segments);
					float s1 = sin(PI2 * (i + 1) / num_segments);
					float c1 = cos(PI2 * (i + 1) / num_segments);
					
					// x axis
					if(mask & MANIPULATOR_MASK_X) {
						Vec3 p0 = position + (axis_y * s0 + axis_z * c0) * axis_scale;
						Vec3 p1 = position + (axis_y * s1 + axis_z * c1) * axis_scale;
						float k0 = float(dot(clip_plane,p0));
						float k1 = float(dot(clip_plane,p1));
						if(k0 > 0.0f || k1 > 0.0f) {
							if(k0 < 0.0f) p0 = p1 + (p0 - p1) * k1 / (k1 - k0);
							if(k1 < 0.0f) p1 = p0 + (p1 - p0) * k0 / (k0 - k1);
							render_line(p0,p1,xcolor,1);
						}
					}
					
					// y axis
					if(mask & MANIPULATOR_MASK_Y) {
						Vec3 p0 = position + (axis_x * s0 + axis_z * c0) * axis_scale;
						Vec3 p1 = position + (axis_x * s1 + axis_z * c1) * axis_scale;
						float k0 = float(dot(clip_plane,p0));
						float k1 = float(dot(clip_plane,p1));
						if(k0 > 0.0f || k1 > 0.0f) {
							if(k0 < 0.0f) p0 = p1 + (p0 - p1) * k1 / (k1 - k0);
							if(k1 < 0.0f) p1 = p0 + (p1 - p0) * k0 / (k0 - k1);
							render_line(p0,p1,ycolor,1);
						}
					}
					
					// z axis
					if(mask & MANIPULATOR_MASK_Z) {
						Vec3 p0 = position + (axis_x * s0 + axis_y * c0) * axis_scale;
						Vec3 p1 = position + (axis_x * s1 + axis_y * c1) * axis_scale;
						float k0 = float(dot(clip_plane,p0));
						float k1 = float(dot(clip_plane,p1));
						if(k0 > 0.0f || k1 > 0.0f) {
							if(k0 < 0.0f) p0 = p1 + (p0 - p1) * k1 / (k1 - k0);
							if(k1 < 0.0f) p1 = p0 + (p1 - p0) * k0 / (k0 - k1);
							render_line(p0,p1,zcolor,1);
						}
					}
				}
				
				// values
				if(gui.getMouseGrab() && current_axis != AXIS_UNKNOWN) {
					
					// rotation
					vec3 d0 = normalize(vec3(initial_position - position));
					vec3 d1 = normalize(vec3(current_position - position));
					float angle = 0.0f;
					float d = dot(d0,d1);
					if(d < -1.0f + EPSILON) angle = 180.0f;
					else if(d > 1.0f - EPSILON) angle = 0.0f;
					else angle = acos(dot(d0,d1)) * RAD2DEG;
					if(dot(cross(d0,d1),vec3(rotation_axis)) < 0.0f) angle *= -1.0f;
					angle = float(snapValue(angle));
					
					// text
					string text = "";
					if(current_axis == AXIS_X) text = format("%.1f 0.0 0.0",rotation_angle);
					if(current_axis == AXIS_Y) text = format("0.0 %.1f 0.0",rotation_angle);
					if(current_axis == AXIS_Z) text = format("0.0 0.0 %.1f",rotation_angle);
					
					int id = render_text(position,text,vec4_one,imodelview_projection);
					int w = canvas.getTextWidth(id);
					int h = canvas.getTextHeight(id);
					canvas.setTextPosition(id,project_point(position) - vec3(float(w / 2.0f),float(size + h * 3 / 2),0.0f));
				}
			}
			
			int isFocused() {
				return current_axis != AXIS_UNKNOWN;
			}
			
			void clearFocus() {
				current_axis = AXIS_UNKNOWN;
			}
	};
	
	/**************************************************************************\
	*
	* class WidgetEditorManipulatorScaler
	*
	\**************************************************************************/
	
	/*
	 */
	class WidgetEditorManipulatorScaler : WidgetEditorManipulator {
			
		private:
			
			/*
			 */
			float MIN_SCALE = 0.001f;
			float SCALE_FACTOR = 0.1f;
			float XYZ_FADE_ANGLE = 30.0f;
			
			enum {
				AXIS_UNKNOWN = -1,
				AXIS_X,
				AXIS_Y,
				AXIS_Z,
				AXIS_XX,
				AXIS_YY,
				AXIS_ZZ,
				AXIS_XYZ,
				NUM_AXIS,
			};
			
			// settings
			float handle_size = 0.5f;
			
			// variables
			vec3 center;				// center
			float axis_scale;			// scale
			
			vec3 axis[NUM_AXIS];		// axis positions
			int current_axis;			// current axis
			
			Vec3 current_scale;			// current scale
			Vec3 initial_scale;			// initial_scale
			
			vec3 screen_origin;			// initial mouse screen position
			
			int input_state = INPUT_STATE_RELEASED;		// mouse down state for triggering
			
			void select_axis() {
				
				current_axis = AXIS_UNKNOWN;
				
				if(panelViewCubeFocused()) return;
				
				// mouse position
				vec3 mouse = vec3(float(gui.getMouseX()),float(gui.getMouseY()),0.0f);
				Vec3 xpoint = unproject_point_plane(mouse,Vec4(axis_x,-dot(axis_x,position)));
				Vec3 ypoint = unproject_point_plane(mouse,Vec4(axis_y,-dot(axis_y,position)));
				Vec3 zpoint = unproject_point_plane(mouse,Vec4(axis_z,-dot(axis_z,position)));
				
				// camera position
				Vec3 camera = imodelview.m03m13m23;
				
				// x axis
				if((mask & MANIPULATOR_MASK_X) && get_distance(center,project_point(axis[AXIS_X]),mouse) < line_selection_radius) {
					if(current_axis == AXIS_UNKNOWN) {
						
						current_axis = AXIS_X;
					}
				}
				
				// y axis
				if((mask & MANIPULATOR_MASK_Y) && get_distance(center,project_point(axis[AXIS_Y]),mouse) < line_selection_radius) {
					if(current_axis == AXIS_UNKNOWN ||
						(current_axis == AXIS_X && length(zpoint - camera) < length(ypoint - camera))) {
						
						current_axis = AXIS_Y;
					}
				}
				
				// z axis
				if((mask & MANIPULATOR_MASK_Z) && get_distance(center,project_point(axis[AXIS_Z]),mouse) < line_selection_radius) {
					if(current_axis == AXIS_UNKNOWN ||
						(current_axis == AXIS_X && length(xpoint - camera) < length(ypoint - camera)) ||
						(current_axis == AXIS_Y && length(xpoint - camera) < length(zpoint - camera))) {
						
						current_axis = AXIS_Z;
					}
				}
				
				// xyz axis
				if(mask == MANIPULATOR_MASK_XYZ) {
					// yz plane
					float xy = float(dot(axis_y,xpoint - position) / axis_scale);
					float xz = float(dot(axis_z,xpoint - position) / axis_scale);
					if(xy >= 0.0f && xz >= 0.0f && xy + xz <= handle_size) {
						current_axis = AXIS_XYZ;
					}
					
					// xy plane
					float zx = float(dot(axis_x,zpoint - position) / axis_scale);
					float zy = float(dot(axis_y,zpoint - position) / axis_scale);
					if(zx >= 0.0f && zy >= 0.0f && zx + zy <= handle_size) {
						current_axis = AXIS_XYZ;
					}
					
					// zx plane
					float yx = float(dot(axis_x,ypoint - position) / axis_scale);
					float yz = float(dot(axis_z,ypoint - position) / axis_scale);
					if(yx >= 0.0f && yz >= 0.0f && yx + yz <= handle_size) {
						current_axis = AXIS_XYZ;
					}
				}
			}
			
			void init_scaler() {
			}
			
		public:
			
			// constructor
			WidgetEditorManipulatorScaler() {
				current_axis = AXIS_UNKNOWN;
			}
			
			~WidgetEditorManipulatorScaler() {
				
			}
			
			// events
			void checkCallbacks() {
				
				if(enabled == 0 || isInCameraFront() == 0) {
					input_state = INPUT_STATE_RELEASED;
					current_axis = AXIS_UNKNOWN;
					return;
				}
				
				// arrange
				arrange();
				
				// select axis
				if(gui.getMouseGrab() == 0) select_axis();
				
				if(current_axis == AXIS_UNKNOWN) {
					if(input_state != INPUT_STATE_RELEASED) runCallback(MANIPULATOR_RELEASED);
					input_state = INPUT_STATE_RELEASED;
					
					return;
				}
				
				// currently focused widget
				::Widget focus = gui.getFocus();
				
				// take the root widget of gui which is focused when no other widget under mouse pointer
				::Widget viewport_widget = NULL;
				if(engine.gui.getNumChilds() > 0) {
					::Widget child = engine.gui.getChild(0);
					viewport_widget = child.getParent();
				}
				
				if(gui.getMouseGrab() && viewport_widget != NULL && focus == viewport_widget) {
					
					Mat4 old_transform = transform;
					
					if(gui.getMouseButton() & APP_BUTTON_LEFT) {
						
						// mouse position
						vec3 mouse = vec3(float(gui.getMouseX()),float(gui.getMouseY()),0.0f);
						
						// check for grab event
						if(input_state == INPUT_STATE_RELEASED) {
							input_state = INPUT_STATE_PRESSED;
							
							Vec3 t,r;
							decomposeTransform(transform,t,r,initial_scale);
							
							screen_origin = mouse;
						}
						
						// change cursor
						engine.gui.setMouseCursor(GUI_CURSOR_SCALE);
						
						// initial transformation
						Vec3 t,r;
						decomposeTransform(transform,t,r,current_scale);
						
						arrange();
						
						if(current_axis == AXIS_X) {
							vec3 screen_axis_x = normalize(project_point(position + axis_x) - project_point(position));
							float value = dot(mouse - screen_origin,screen_axis_x) / size;
							current_scale.x = max(initial_scale.x * (1.0f + SCALE_FACTOR * value),MIN_SCALE);
						}
						
						if(current_axis == AXIS_Y) {
							vec3 screen_axis_y = normalize(project_point(position + axis_y) - project_point(position));
							float value = dot(mouse - screen_origin,screen_axis_y) / size;
							current_scale.y = max(initial_scale.y * (1.0f + SCALE_FACTOR * value),MIN_SCALE);
						}
						
						if(current_axis == AXIS_Z) {
							vec3 screen_axis_z = normalize(project_point(position + axis_z) - project_point(position));
							float value = dot(mouse - screen_origin,screen_axis_z) / size;
							current_scale.z = max(initial_scale.z * (1.0f + SCALE_FACTOR * value),MIN_SCALE);
						}
						
						if(current_axis == AXIS_XYZ) {
							
							float value = length(mouse - screen_origin) / size;
							
							// determining sign
							if(dot(mouse - screen_origin,vec3(1.0f,-1.0f,0.0f)) < 0.0f) value = -value;
							
							// fading
							float angle = abs(Unigine::getAngle(vec3(1.0f,1.0f,0.0f),mouse - screen_origin));
							if(angle > 90.0f) angle = 180.0f - angle;
							if(angle < XYZ_FADE_ANGLE) value = value * (angle / XYZ_FADE_ANGLE);
							
							// calculating scale
							current_scale = max(initial_scale * (1.0f + SCALE_FACTOR * value),Vec3(MIN_SCALE));
						}
						
						// update axis
						transform = composeTransform(t,r,current_scale);
						arrange();
					}
					
					// run callback
					if(transform != old_transform) {
						runCallback(MANIPULATOR_CHANGED);
					}
					
					if(gui.getMouseButton() & APP_BUTTON_DCLICK && current_axis != AXIS_UNKNOWN) {
						gui.setMouseButton((gui.getMouseButton() & ~APP_BUTTON_DCLICK) | APP_BUTTON_LEFT);
						runCallback(MANIPULATOR_DOUBLE_CLICKED);
					}
					
					if(input_state == INPUT_STATE_PRESSED) {
						runCallback(MANIPULATOR_CLICKED);
						input_state = INPUT_STATE_HOLDING;
					}
				}
				else {
					if(input_state != INPUT_STATE_RELEASED) runCallback(MANIPULATOR_RELEASED);
					input_state = INPUT_STATE_RELEASED;
				}
			}
			
			// arrange
			void arrange() {
				
				// position
				position = transform.m03m13m23;
				
				// camera position
				Vec3 camera = imodelview.m03m13m23;
				Vec3 direction = camera - position;
				if(dot(direction,axis_x) < 0.0f) axis_x = -axis_x;
				if(dot(direction,axis_y) < 0.0f) axis_y = -axis_y;
				if(dot(direction,axis_z) < 0.0f) axis_z = -axis_z;
				
				// widget center
				center = project_point(position);
				
				// widget scale
				axis_scale = get_scale(position);
				
				// axis positions
				axis[AXIS_X] = position + axis_x * axis_scale;
				axis[AXIS_Y] = position + axis_y * axis_scale;
				axis[AXIS_Z] = position + axis_z * axis_scale;
				axis[AXIS_XX] = position + axis_x * (axis_scale * handle_size);
				axis[AXIS_YY] = position + axis_y * (axis_scale * handle_size);
				axis[AXIS_ZZ] = position + axis_z * (axis_scale * handle_size);
				
			}
			
			// render
			void render() {
				
				// clear canvas
				clear_primitives();
				
				if(enabled == 0) return;
				
				// setup modelview & projection for canvas
				int width = gui.getWidth();
				int height = gui.getHeight();
				mat4 modelview_projection = scale(width / 2.0f,height / 2.0f,1.0f) * translate(1.0f,1.0f,0.0f) * scale(1.0f,-1.0f,1.0f) * projection * mat4(rotation(modelview));
				mat4 imodelview_projection = inverse(modelview_projection);
				canvas.setTransform(modelview_projection);
				
				// check visibility
				if(isInCameraFront() == 0) return;
				
				// arrange
				arrange();
				
				// colors
				vec4 xcolor = vec4(1.0f,0.0f,0.0f,1.0f);
				vec4 ycolor = vec4(0.0f,1.0f,0.0f,1.0f);
				vec4 zcolor = vec4(0.0f,0.0f,1.0f,1.0f);
				vec4 xyzcolor = vec4(0.5f,0.5f,0.5f,0.25f);
				if(current_axis == AXIS_X || current_axis == AXIS_XYZ) xcolor = vec4(1.0f,1.0f,0.0f,1.0f);
				if(current_axis == AXIS_Y || current_axis == AXIS_XYZ) ycolor = vec4(1.0f,1.0f,0.0f,1.0f);
				if(current_axis == AXIS_Z || current_axis == AXIS_XYZ) zcolor = vec4(1.0f,1.0f,0.0f,1.0f);
				if(current_axis == AXIS_XYZ) xyzcolor = vec4(1.0f,1.0f,0.0f,0.25f);
				xcolor *= color;
				ycolor *= color;
				zcolor *= color;
				xyzcolor *= color;
				
				// planes
				if(mask == MANIPULATOR_MASK_XYZ) render_triangle(axis[AXIS_XX],axis[AXIS_YY],axis[AXIS_ZZ],xyzcolor);
				
				// axis
				if(mask & MANIPULATOR_MASK_X) render_line(position,axis[AXIS_X],xcolor);
				if(mask & MANIPULATOR_MASK_Y) render_line(position,axis[AXIS_Y],ycolor);
				if(mask & MANIPULATOR_MASK_Z) render_line(position,axis[AXIS_Z],zcolor);
				if(mask == MANIPULATOR_MASK_XYZ) {
					render_line(axis[AXIS_XX],axis[AXIS_YY],xyzcolor.xyz1);
					render_line(axis[AXIS_YY],axis[AXIS_ZZ],xyzcolor.xyz1);
					render_line(axis[AXIS_ZZ],axis[AXIS_XX],xyzcolor.xyz1);
				}
				
				// boxes
				Vec3 p0 = position + axis_x * 0.95f * axis_scale;
				Vec3 p1 = position + axis_y * 0.95f * axis_scale;
				Vec3 p2 = position + axis_z * 0.95f * axis_scale;
				Vec3 v0 = (-axis_x * 0.05f - axis_y * 0.05f - axis_z * 0.05f) * axis_scale;
				Vec3 v1 = ( axis_x * 0.05f - axis_y * 0.05f - axis_z * 0.05f) * axis_scale;
				Vec3 v2 = (-axis_x * 0.05f + axis_y * 0.05f - axis_z * 0.05f) * axis_scale;
				Vec3 v3 = ( axis_x * 0.05f + axis_y * 0.05f - axis_z * 0.05f) * axis_scale;
				Vec3 v4 = (-axis_x * 0.05f - axis_y * 0.05f + axis_z * 0.05f) * axis_scale;
				Vec3 v5 = ( axis_x * 0.05f - axis_y * 0.05f + axis_z * 0.05f) * axis_scale;
				Vec3 v6 = (-axis_x * 0.05f + axis_y * 0.05f + axis_z * 0.05f) * axis_scale;
				Vec3 v7 = ( axis_x * 0.05f + axis_y * 0.05f + axis_z * 0.05f) * axis_scale;
				if(mask & MANIPULATOR_MASK_X) {
					render_quad(p0 + v0,p0 + v1,p0 + v3,p0 + v2,xcolor);
					render_quad(p0 + v4,p0 + v5,p0 + v7,p0 + v6,xcolor);
					render_quad(p0 + v0,p0 + v1,p0 + v5,p0 + v4,xcolor);
					render_quad(p0 + v1,p0 + v3,p0 + v7,p0 + v5,xcolor);
					render_quad(p0 + v3,p0 + v2,p0 + v6,p0 + v7,xcolor);
					render_quad(p0 + v2,p0 + v0,p0 + v4,p0 + v6,xcolor);
				}
				if(mask & MANIPULATOR_MASK_Y) {
					render_quad(p1 + v0,p1 + v1,p1 + v3,p1 + v2,ycolor);
					render_quad(p1 + v4,p1 + v5,p1 + v7,p1 + v6,ycolor);
					render_quad(p1 + v0,p1 + v1,p1 + v5,p1 + v4,ycolor);
					render_quad(p1 + v1,p1 + v3,p1 + v7,p1 + v5,ycolor);
					render_quad(p1 + v3,p1 + v2,p1 + v6,p1 + v7,ycolor);
					render_quad(p1 + v2,p1 + v0,p1 + v4,p1 + v6,ycolor);
				}
				if(mask & MANIPULATOR_MASK_Z) {
					render_quad(p2 + v0,p2 + v1,p2 + v3,p2 + v2,zcolor);
					render_quad(p2 + v4,p2 + v5,p2 + v7,p2 + v6,zcolor);
					render_quad(p2 + v0,p2 + v1,p2 + v5,p2 + v4,zcolor);
					render_quad(p2 + v1,p2 + v3,p2 + v7,p2 + v5,zcolor);
					render_quad(p2 + v3,p2 + v2,p2 + v6,p2 + v7,zcolor);
					render_quad(p2 + v2,p2 + v0,p2 + v4,p2 + v6,zcolor);
				}
				
				// values
				if(gui.getMouseGrab()) {
					
					// scale decomposition
					mat4 decompose = inverse(orthonormalize(rotation(transform))) * rotation(transform);
					
					if(current_axis == AXIS_X || current_axis == AXIS_XYZ) {
						string text = format("%.3f",decompose.m00);
						int id = render_text(axis[AXIS_X],text,vec4_one,imodelview_projection);
						int w = canvas.getTextWidth(id);
						int h = canvas.getTextHeight(id);
						canvas.setTextPosition(id,project_point(axis[AXIS_X]) + (normalize(project_point(axis[AXIS_X]) - center) * float(w) - vec3(float(w),float(h),0.0f)) * 0.5f);
					}
					if(current_axis == AXIS_Y || current_axis == AXIS_XYZ) {
						string text = format("%.3f",decompose.m11);
						int id = render_text(axis[AXIS_X],text,vec4_one,imodelview_projection);
						int w = canvas.getTextWidth(id);
						int h = canvas.getTextHeight(id);
						canvas.setTextPosition(id,project_point(axis[AXIS_Y]) + (normalize(project_point(axis[AXIS_Y]) - center) * float(w) - vec3(float(w),float(h),0.0f)) * 0.5f);
					}
					if(current_axis == AXIS_Z || current_axis == AXIS_XYZ) {
						string text = format("%.3f",decompose.m22);
						int id = render_text(axis[AXIS_X],text,vec4_one,imodelview_projection);
						int w = canvas.getTextWidth(id);
						int h = canvas.getTextHeight(id);
						canvas.setTextPosition(id,project_point(axis[AXIS_Z]) + (normalize(project_point(axis[AXIS_Z]) - center) * float(w) - vec3(float(w),float(h),0.0f)) * 0.5f);
					}
				}
			}
			
			int isFocused() {
				return current_axis != AXIS_UNKNOWN;
			}
			
			void clearFocus() {
				current_axis = AXIS_UNKNOWN;
			}
	};
	
} /* namespace Editor::Widgets */

#endif /* __EDITOR_WIDGET_MANIPULATOR_H__ */
