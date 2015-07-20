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


#ifndef __UNIGINE_WIDGET_LINE_H__
#define __UNIGINE_WIDGET_LINE_H__

#include <core/systems/widgets/widget_scrollbox.h>

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	enum {
		
		LINE_CURVE_CHANGED = 0,			// curve changed callback
		LINE_KEY_CREATED,				// key created callback
		LINE_KEY_CHANGED,				// key changed callback
		LINE_KEY_REMOVED,				// key removed callback
		LINE_NUM_CALLBACKS,
		
		LINE_KEY_UP = 0,				// line key up direction
		LINE_KEY_DOWN,					// line key down direction
		
		LINE_WIDTH = 256,				// line width
		LINE_HEIGHT = 22,				// line height
		LINE_THRESHOLD = 8,				// line threshold
		
		LINE_FONT_SIZE = 12,			// line font size
		
		LINE_TIME_HEIGHT = 2048,		// line time hegiht
		
		LINE_CANVAS_SPEED = 4,			// canvas speed
		LINE_CANVAS_WIDTH = 65556,		// canvas width
		LINE_IMAGE_WIDTH = 2048,		// image width
		
		LINE_HANDLE_ARROW = 6,			// key handle arrow
		LINE_HANDLE_WIDTH = 12,			// key handle width
		LINE_HANDLE_HEIGHT = 8,			// key handle height
	};
	
	/*
	 */
	vec4 line_time_color = vec4(1.0f,1.0f,1.0f,1.0f);
	vec4 line_range_color = vec4(0.25f,0.25f,0.25f,0.5f);
	vec4 line_contour_color = vec4(0.0f,0.0f,0.0f,1.0f);
	vec4 line_selected_color = vec4(1.0f,1.0f,1.0f,1.0f);
	vec4 line_selection_color = vec4(0.5f,0.5f,0.5f,0.25f);
	vec4 line_background_color = vec4(0.15f,0.15f,0.15f,1.0f);
	
	/**************************************************************************\
	*
	* LineKey
	*
	\**************************************************************************/
	
	/*
	 */
	class LineKey {
		
		LineCurve curve;		// curve
		
		float time;				// key time
		
		int data;				// key data
		
		int text_id;			// text identifier
		int contour_id;			// contour identifier
		int polygon_id;			// polygon identifier
		int selection_id;		// selection identifier
		
		WidgetCanvas canvas;
		
		/*
		 */
		LineKey(float t) {
			
			time = t;
			
			text_id = -1;
			contour_id = -1;
			polygon_id = -1;
			selection_id = -1;
		}
		
		~LineKey() {
			
			if(canvas != NULL) {
				canvas.removeText(text_id);
				canvas.removeLine(contour_id);
				canvas.removePolygon(polygon_id);
				canvas.removePolygon(selection_id);
			}
		}
		
		/*
		 */
		void setCurve(LineCurve c) {
			curve = c;
		}
		
		LineCurve getCurve() {
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
		void setData(int d) {
			data = d;
		}
		
		int getData() {
			return data;
		}
		
		/*
		 */
		void setCanvas(WidgetCanvas c) {
			if(canvas != c) {
				canvas = c;
				text_id = canvas.addText(3);
				contour_id = canvas.addLine(4);
				polygon_id = canvas.addPolygon(3);
				selection_id = canvas.addPolygon(4);
				canvas.setTextOutline(text_id,1);
				canvas.setLineColor(contour_id,line_contour_color);
				canvas.setPolygonColor(selection_id,vec4_zero);
			}
		}
		
		void setOrder(int &order) {
			if(canvas != NULL) {
				canvas.setTextOrder(text_id,order++);
				canvas.setPolygonOrder(polygon_id,order++);
				canvas.setLineOrder(contour_id,order++);
				canvas.setPolygonOrder(selection_id,order++);
			}
		}
		
		/*
		 */
		int isPolygon(int id) {
			return (selection_id == id);
		}
		
		/*
		 */
		int getPositionX() {
			Line line = curve.getLine();
			int width = canvas.getWidth();
			return LINE_HANDLE_WIDTH / 2 + int((width - LINE_HANDLE_WIDTH) * (time - line.getMinTime()) / (line.getMaxTime() - line.getMinTime()));
		}
		
		/*
		 */
		string get_text() {
			return "";
		}
		
		int get_direction() {
			return LINE_KEY_UP;
		}
		
		vec4 get_color() {
			throw("Unigine::Widgets::LineKey::get_color(): called\n");
		}
		
		/*
		 */
		void create(int position_y,int selected) {
			
			int position_x = getPositionX();
			position_y += LINE_HEIGHT;
			
			// key points
			vec3 p0,p1,p2,p3,p4;
			if(get_direction() == LINE_KEY_UP) {
				p0 = vec3(position_x - LINE_HANDLE_WIDTH / 2,position_y - LINE_HANDLE_HEIGHT,0.0f);
				p1 = vec3(position_x,position_y - LINE_HANDLE_HEIGHT - LINE_HANDLE_ARROW,0.0f);
				p2 = vec3(position_x + LINE_HANDLE_WIDTH / 2,position_y - LINE_HANDLE_HEIGHT,0.0f);
				p3 = vec3(position_x + LINE_HANDLE_WIDTH / 2,position_y - 1,0.0f);
				p4 = vec3(position_x - LINE_HANDLE_WIDTH / 2,position_y - 1,0.0f);
			} else {
				p0 = vec3(position_x - LINE_HANDLE_WIDTH / 2,position_y - LINE_HANDLE_HEIGHT - LINE_HANDLE_ARROW,0.0f);
				p1 = vec3(position_x + LINE_HANDLE_WIDTH / 2,position_y - LINE_HANDLE_HEIGHT - LINE_HANDLE_ARROW,0.0f);
				p2 = vec3(position_x + LINE_HANDLE_WIDTH / 2,position_y - LINE_HANDLE_ARROW - 1,0.0f);
				p3 = vec3(position_x,position_y - 1,0.0f);
				p4 = vec3(position_x - LINE_HANDLE_WIDTH / 2,position_y - LINE_HANDLE_ARROW - 1,0.0f);
			}
			
			// create text
			canvas.setTextText(text_id,get_text());
			int text_offset_y = (LINE_HEIGHT + LINE_HANDLE_HEIGHT + canvas.getTextHeight(text_id) * 3 / 2) / 2;
			canvas.setTextPosition(text_id,vec3(position_x + LINE_HANDLE_WIDTH / 2,position_y - text_offset_y,0.0f));
			
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
			if(selected) canvas.setPolygonColor(polygon_id,line_selected_color);
			else canvas.setPolygonColor(polygon_id,get_color());
			canvas.addPolygonPoint(polygon_id,p0);
			canvas.addPolygonPoint(polygon_id,p1);
			canvas.addPolygonPoint(polygon_id,p2);
			canvas.addPolygonPoint(polygon_id,p3);
			canvas.addPolygonPoint(polygon_id,p4);
			
			// create selection
			canvas.clearPolygonPoints(selection_id);
			canvas.addPolygonPoint(selection_id,vec3(position_x - LINE_HANDLE_WIDTH / 2,position_y - LINE_HEIGHT,0.0f));
			canvas.addPolygonPoint(selection_id,vec3(position_x + LINE_HANDLE_WIDTH / 2,position_y - LINE_HEIGHT,0.0f));
			canvas.addPolygonPoint(selection_id,vec3(position_x + LINE_HANDLE_WIDTH / 2,position_y - 1,0.0f));
			canvas.addPolygonPoint(selection_id,vec3(position_x - LINE_HANDLE_WIDTH / 2,position_y - 1,0.0f));
		}
	};
	
	/**************************************************************************\
	*
	* LineCurve
	*
	\**************************************************************************/
	
	/*
	 */
	class LineCurve {
		
		Line line;				// line
		
		int data;				// curve data
		
		Image image;			// curve image
		
		LineKey keys[0];		// curve keys
		float indices[0];		// key indices
		
		int keys_changed;		// keys changed flag
		
		int contour_id;			// contour identifier
		int polygon_id;			// polygon identifier
		
		WidgetCanvas canvas;
		
		/*
		 */
		LineCurve() {
			
			image = new Image();
			
			contour_id = -1;
			polygon_id = -1;
		}
		
		~LineCurve() {
			
			clear();
			
			delete image;
			
			if(canvas != NULL) {
				canvas.removeLine(contour_id);
				canvas.removePolygon(polygon_id);
			}
		}
		
		/*
		 */
		void setLine(Line l) {
			line = l;
		}
		
		Line getLine() {
			return line;
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
		void addKey(LineKey key) {
			if(keys.find(key) != -1) throw("Unigine::Widgets::LineCurve::addKey(): key is already defined\n");
			key.setCurve(this);
			key.setCanvas(canvas);
			keys.append(key);
			updateKeys();
		}
		
		void removeKey(LineKey key) {
			int index = keys.find(key);
			if(index == -1) throw("Unigine::Widgets::LineCurve::removeKey(): can't find key\n");
			keys.remove(index);
			updateKeys();
			delete key;
		}
		
		int isKey(LineKey key) {
			return (keys.find(key) != -1);
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
			
			keys_changed = 1;
		}
		
		/*
		 */
		int getNumKeys() {
			return keys.size();
		}
		
		LineKey getKey(int num) {
			return keys[num];
		}
		
		/*
		 */
		void setCanvas(WidgetCanvas c) {
			if(canvas != c) {
				canvas = c;
				contour_id = canvas.addLine(2);
				polygon_id = canvas.addPolygon(1);
				canvas.setLineColor(contour_id,line_contour_color);
				foreach(LineKey key; keys) {
					key.setCanvas(canvas);
				}
			}
		}
		
		void setOrder(int &order) {
			
		}
		
		/*
		 */
		int isPolygon(int id) {
			return (polygon_id == id);
		}
		
		/*
		 */
		LineKey create_key(float time) {
			throw("Unigine::Widgets::LineCurve::create_key(): called\n");
		}
		
		LineKey clone_key(LineKey key) {
			throw("Unigine::Widgets::LineCurve::clone_key(): called\n");
		}
		
		/*
		 */
		string get_update_func() {
			throw("Unigine::Widgets::LineCurve::get_update_func(): called\n");
		}
		
		/*
		 */
		void create_image(int width) {
			throw("Unigine::Widgets::LineCurve::create_image(): called\n");
		}
		
		void update_image(float step,float offset) {
			throw("Unigine::Widgets::LineCurve::update_image(): called\n");
		}
		
		/*
		 */
		void create(int position_y,LineKey selected_keys[]) {
			
			int width = canvas.getWidth();
			
			vec3 p0 = vec3(LINE_HANDLE_WIDTH / 2,position_y,0.0f);
			vec3 p1 = vec3(width - LINE_HANDLE_WIDTH / 2,position_y,0.0f);
			vec3 p2 = vec3(width - LINE_HANDLE_WIDTH / 2,position_y + LINE_HEIGHT - LINE_HANDLE_HEIGHT,0.0f);
			vec3 p3 = vec3(LINE_HANDLE_WIDTH / 2,position_y + LINE_HEIGHT - LINE_HANDLE_HEIGHT,0.0f);
			
			// create contour line
			canvas.clearLinePoints(contour_id);
			canvas.addLinePoint(contour_id,p0);
			canvas.addLinePoint(contour_id,p1);
			canvas.addLinePoint(contour_id,p2);
			canvas.addLinePoint(contour_id,p3);
			canvas.addLinePoint(contour_id,p0);
			
			// create image polygon
			canvas.clearPolygonPoints(polygon_id);
			canvas.addPolygonPoint(polygon_id,p0);
			canvas.setPolygonTexCoord(polygon_id,vec3(0.0f,0.0f,0.0f));
			canvas.addPolygonPoint(polygon_id,p1);
			canvas.setPolygonTexCoord(polygon_id,vec3(1.0f,0.0f,0.0f));
			canvas.addPolygonPoint(polygon_id,p2);
			canvas.setPolygonTexCoord(polygon_id,vec3(1.0f,1.0f,0.0f));
			canvas.addPolygonPoint(polygon_id,p3);
			canvas.setPolygonTexCoord(polygon_id,vec3(0.0f,1.0f,0.0f));
			
			// update image
			if(keys_changed) {
				keys_changed = 0;
				if(image.isLoaded()) {
					update_image((line.getMaxTime() - line.getMinTime()) / (image.getWidth() - 1),line.getMinTime());
					canvas.setPolygonImage(polygon_id,image);
				}
			}
			
			// create keys
			foreach(LineKey key; keys) {
				int selected = (selected_keys.find(key) != -1);
				key.create(position_y,selected);
			}
		}
	};
	
	/**************************************************************************\
	*
	* Line
	*
	\**************************************************************************/
	
	/*
	 */
	class Line : Widget {
		
		int width;							// line width
		
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
		
		int key_snap;						// key snap toggle
		
		LineCurve curves[0];				// curves
		
		int pressed;						// pressed flag
		
		int keys_changed;					// keys changed flag
		int range_changed;					// range changed flag
		int curves_changed;					// curves changed flag
		
		LineCurve current_curve;			// current curve
		LineKey current_key;				// current key
		
		LineCurve selected_curves[0];		// selected curves
		LineKey selected_keys[0];			// selected keys
		
		LineCurve old_selected_curves[0];	// selected curves
		LineKey old_selected_keys[0];		// selected keys
		
		ScrollBox canvas_sb;				// canvas scrollbox
		
		string callbacks[LINE_NUM_CALLBACKS];
		int callbacks_data_0[LINE_NUM_CALLBACKS];
		int callbacks_data_1[LINE_NUM_CALLBACKS];
		int callbacks_enabled[LINE_NUM_CALLBACKS];
		
		WidgetVBox vbox;
		WidgetCanvas canvas;
		
		/*
		 */
		Line(float min,float max) {
			
			min_time = min;
			max_time = max;
			
			current_time = -INFINITY;
			range_from = min;
			range_to = max;
			
			// create canvas
			canvas_sb = new ScrollBox();
			vbox = new WidgetVBox(engine.getGui());
			canvas = new WidgetCanvas(engine.getGui());
			canvas.setFontSize(LINE_FONT_SIZE);
			vbox.addChild(canvas_sb.widget,ALIGN_EXPAND);
			canvas_sb.widget.addChild(canvas,ALIGN_EXPAND);
			
			// canvas parameters
			canvas.setWidth(LINE_WIDTH);
			canvas.setHeight(LINE_HEIGHT);
			canvas.setColor(line_background_color);
			
			// create time line
			time_line_id = canvas.addLine();
			canvas.setLineColor(time_line_id,line_time_color);
			
			// create range lines
			range_line_id[0] = canvas.addLine();
			range_line_id[1] = canvas.addLine();
			canvas.setLineColor(range_line_id[0],vec4(line_range_color.xyz,1.0f));
			canvas.setLineColor(range_line_id[1],vec4(line_range_color.xyz,1.0f));
			
			// create range polygons
			range_polygon_id[0] = canvas.addPolygon();
			range_polygon_id[1] = canvas.addPolygon();
			canvas.setPolygonColor(range_polygon_id[0],line_range_color);
			canvas.setPolygonColor(range_polygon_id[1],line_range_color);
			canvas.setPolygonBlendFunc(range_polygon_id[0],BLEND_SRC_ALPHA,BLEND_ONE_MINUS_SRC_ALPHA);
			canvas.setPolygonBlendFunc(range_polygon_id[1],BLEND_SRC_ALPHA,BLEND_ONE_MINUS_SRC_ALPHA);
			
			// create selection line
			selection_line_id = canvas.addLine();
			canvas.setLineColor(selection_line_id,vec4(line_selection_color.xyz,1.0f));
			
			// create selection polygon
			selection_polygon_id = canvas.addPolygon();
			canvas.setPolygonColor(selection_polygon_id,line_selection_color);
			canvas.setPolygonBlendFunc(selection_polygon_id,BLEND_SRC_ALPHA,BLEND_ONE_MINUS_SRC_ALPHA);
			
			// set callbacks
			canvas.setCallback(PRESSED,functionid(callback_redirector),this,functionid(pressed_callback));
			canvas.setCallback(RELEASED,functionid(callback_redirector),this,functionid(released_callback));
			canvas.setCallback(DOUBLE_CLICKED,functionid(double_clicked_callback),this);
			
			widget = vbox;
		}
		
		~Line() {
			
			clear();
			
			delete canvas_sb;
			delete canvas;
		}
		
		/*
		 */
		ScrollBox getScrollBox() {
			return canvas_sb;
		}
		
		/*
		 */
		int isCanvasFocused() {
			return vbox.isChild(engine.gui.getFocus());
		}
		
		void setCanvasWidth(int width) {
			canvas.setWidth(clamp(width,canvas_sb.getHScrollFrameSize() / canvas_sb.getScrollScale(),LINE_CANVAS_WIDTH));
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
			foreach(LineCurve curve; curves) {
				curve.keys_changed = 1;
			}
		}
		
		float getMinTime() {
			return min_time;
		}
		
		/*
		 */
		void setMaxTime(float time) {
			max_time = time;
			foreach(LineCurve curve; curves) {
				curve.keys_changed = 1;
			}
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
		float getPrevTime(float time,int exclude = 0) {
			float prev_time = -INFINITY;
			foreach(LineCurve curve; curves) {
				if(exclude && selected_curves.find(curve) != -1) continue;
				float indices[] = curve.indices;
				int left = indices.left(time);
				if(left == -1) continue;
				if(time == indices[left]) continue;
				if(prev_time < indices[left]) prev_time = indices[left];
			}
			return prev_time;
		}
		
		float getNextTime(float time,int exclude = 0) {
			float next_time = INFINITY;
			foreach(LineCurve curve; curves) {
				if(exclude && selected_curves.find(curve) != -1) continue;
				float indices[] = curve.indices;
				int right = indices.right(time);
				if(right == -1) continue;
				if(time == indices[right] && right + 1 < indices.size()) right++;
				if(time == indices[right]) continue;
				if(next_time > indices[right]) next_time = indices[right];
			}
			return next_time;
		}
		
		float getSnapTime(float time) {
			float error = INFINITY;
			float snap_time = INFINITY;
			foreach(LineCurve curve; curves) {
				float indices[] = curve.indices;
				int left = indices.left(time);
				int right = indices.right(time);
				if(left != -1) {
					float e = abs(indices[left] - time);
					if(error > e) {
						snap_time = indices[left];
						error = e;
					}
				}
				if(right != -1) {
					float e = abs(indices[right] - time);
					if(error > e) {
						snap_time = indices[right];
						error = e;
					}
				}
			}
			return snap_time;
		}
		
		/*
		 */
		void setKeySnap(int snap) {
			key_snap = snap;
		}
		
		int getKeySnap() {
			return key_snap;
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
		void addCurve(LineCurve curve) {
			if(curves.find(curve) != -1) throw("Unigine::Widgets::Line::addCurve(): curve is already defined\n");
			curve.setLine(this);
			curve.setCanvas(canvas);
			curve.setOrder(order);
			curves.append(curve);
			curves_changed = 1;
		}
		
		void removeCurve(LineCurve curve) {
			int index = curves.find(curve);
			if(index == -1) throw("Unigine::Widgets::Line::removeCurve(): can't find curve\n");
			curves.remove(index);
			curves_changed = 1;
			delete curve;
		}
		
		int isCurve(LineCurve curve) {
			return (curves.find(curve) != -1);
		}
		
		/*
		 */
		int getNumCurves() {
			return curves.size();
		}
		
		LineCurve getCurve(int num) {
			return curves[num];
		}
		
		/*
		 */
		void setCurrentCurve(LineCurve curve) {
			if(current_curve != curve) {
				current_curve = curve;
				if(curve != NULL) curve.setOrder(order);
				runCallback(LINE_CURVE_CHANGED);
			}
		}
		
		LineCurve getCurrentCurve() {
			return current_curve;
		}
		
		/*
		 */
		void setCurrentKey(LineKey key) {
			if(current_key != key) {
				if(key != NULL) {
					key.setOrder(order);
				}
				current_key = key;
			}
		}
		
		LineKey getCurrentKey() {
			return current_key;
		}
		
		/*
		 */
		void setSelectedCurves(LineCurve curves[]) {
			selected_curves.copy(curves);
			keys_changed = 1;
		}
		
		void getNumSelectedCurves() {
			return selected_curves.size();
		}
		
		LineCurve getSelectedCurve(int num) {
			return selected_curves[num];
		}
		
		/*
		 */
		void setSelectedKeys(LineKey keys[]) {
			selected_keys.copy(keys);
			keys_changed = 1;
		}
		
		void getNumSelectedKeys() {
			return selected_keys.size();
		}
		
		LineKey getSelectedKey(int num) {
			return selected_keys[num];
		}
		
		/*
		 */
		void clearSelectedKey(LineKey key) {
			int index = selected_keys.find(key);
			if(index != -1) {
				selected_curves.remove(index);
				selected_keys.remove(index);
				keys_changed = 1;
			}
		}
		
		/*
		 */
		void moveSelectedKeys(float time) {
			
			// selected bounds
			float min_time = 0.0f;
			float max_time = 0.0f;
			foreach(LineKey key; selected_keys) {
				min_time = min(min_time,key.getTime() - current_key.getTime());
				max_time = max(max_time,key.getTime() - current_key.getTime());
			}
			
			// clamp values
			time = clamp(time,getMinTime() - min_time,getMaxTime() - max_time);
			
			// move selected keys
			if(current_key.getTime() != time) {
				time -= current_key.getTime();
				foreach(LineKey key; selected_keys) {
					key.setTime(key.getTime() + time);
				}
				updateSelectedKeys();
			}
		}
		
		void removeSelectedKeys() {
			
			// run callback
			runCallback(LINE_KEY_REMOVED);
			
			// remove selected keys
			forloop(int i = 0; selected_keys.size()) {
				LineCurve curve = selected_curves[i];
				LineKey key = selected_keys[i];
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
			LineCurve curves[];
			forloop(int i = 0; selected_keys.size()) {
				curves.append(selected_curves[i]);
			}
			foreachkey(LineCurve curve; curves) {
				curve.updateKeys();
			}
			
			// run callback
			runCallback(LINE_KEY_CHANGED);
		}
		
		/**********************************************************************\
		*
		* Update
		*
		\**********************************************************************/
		
		/*
		 */
		void update() {
			
			// check canvas
			if(canvas == NULL) return;
			
			// canvas changed
			if(curves_changed || width != canvas.getWidth()) {
				
				curves_changed = 0;
				
				width = canvas.getWidth();
				canvas.setHeight(LINE_HEIGHT * (curves.size() + 1));
				
				// create curve images
				int image_width = clamp(npot(width / 2),LINE_WIDTH,LINE_IMAGE_WIDTH);
				foreach(LineCurve curve; curves) {
					if(curve.image.isLoaded() == 0 || curve.image.getWidth() != image_width) {
						curve.create_image(image_width);
						curve.keys_changed = 1;
					}
				}
				
				keys_changed = 1;
				range_changed = 1;
			}
			
			// curves changed
			if(keys_changed == 0) {
				foreach(LineCurve curve; curves) {
					keys_changed |= curve.keys_changed;
				}
			}
			
			// keys changed
			if(keys_changed) {
				forloop(int i = 0; curves.size()) {
					curves[i].create(LINE_HEIGHT * i,selected_keys);
				}
				keys_changed = 0;
			}
			
			// canvas height
			int height = max(LINE_TIME_HEIGHT,canvas.getHeight() * 2);
			
			// current time
			canvas.clearLinePoints(time_line_id);
			canvas.setLineOrder(time_line_id,order);
			int time_x = LINE_HANDLE_WIDTH / 2 + int((width - LINE_HANDLE_WIDTH) * (current_time - min_time) / (max_time - min_time));
			canvas.addLinePoint(time_line_id,vec3(time_x - 1,height,0.0f));
			canvas.addLinePoint(time_line_id,vec3(time_x - 1,-height,0.0f));
			canvas.addLinePoint(time_line_id,vec3(time_x + 1,-height,0.0f));
			canvas.addLinePoint(time_line_id,vec3(time_x + 1,height,0.0f));
			
			// range changed
			if(range_changed) {
				int range_from_x = LINE_HANDLE_WIDTH / 2 + int((width - LINE_HANDLE_WIDTH) * (range_from - min_time) / (max_time - min_time));
				int range_to_x = LINE_HANDLE_WIDTH / 2 + int((width - LINE_HANDLE_WIDTH) * (range_to - min_time) / (max_time - min_time));
				canvas.clearLinePoints(range_line_id[0]);
				canvas.clearLinePoints(range_line_id[1]);
				canvas.addLinePoint(range_line_id[0],vec3(range_from_x - 1,height,0.0f));
				canvas.addLinePoint(range_line_id[0],vec3(range_from_x - 1,-height,0.0f));
				canvas.addLinePoint(range_line_id[1],vec3(range_to_x + 1,height,0.0f));
				canvas.addLinePoint(range_line_id[1],vec3(range_to_x + 1,-height,0.0f));
				canvas.clearPolygonPoints(range_polygon_id[0]);
				canvas.clearPolygonPoints(range_polygon_id[1]);
				canvas.addPolygonPoint(range_polygon_id[0],vec3(0.0f,height,0.0f));
				canvas.addPolygonPoint(range_polygon_id[0],vec3(range_from_x - 1,height,0.0f));
				canvas.addPolygonPoint(range_polygon_id[0],vec3(range_from_x - 1,-height,0.0f));
				canvas.addPolygonPoint(range_polygon_id[0],vec3(0.0f,-height,0.0f));
				canvas.addPolygonPoint(range_polygon_id[1],vec3(width,height,0.0f));
				canvas.addPolygonPoint(range_polygon_id[1],vec3(range_to_x + 1,height,0.0f));
				canvas.addPolygonPoint(range_polygon_id[1],vec3(range_to_x + 1,-height,0.0f));
				canvas.addPolygonPoint(range_polygon_id[1],vec3(width,-height,0.0f));
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
					float step = (max_time - min_time) / 100.0f;
					if(clearKeyState(APP_KEY_LEFT)) moveSelectedKeys(time - step);
					if(clearKeyState(APP_KEY_RIGHT)) moveSelectedKeys(time + step);
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
				else log.error("Unigine::Widgets::Line::runCallback(): can't find \"%s\" callback function\n",function);
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
				
				float mouse_time;
				
				float mouse_step_x;
				
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
						foreach(LineCurve curve; curves) {
							foreach(LineKey key; curve.keys) {
								if(key.isPolygon(id)) {
									
									pressed = 2;
									
									mouse_time = key.getTime();
									
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
											LineKey key = selected_keys[i];
											LineCurve curve = selected_curves[i];
											LineKey clone = curve.clone_key(key);
											curve.addKey(clone);
											if(current_key == key) setCurrentKey(clone);
											selected_keys[i] = clone;
											clone.setOrder(order);
										}
										runCallback(LINE_KEY_CREATED);
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
					keys_changed = 1;
				}
				
				else {
					
					int mouse_dx = getMouseDX();
					
					float ifps = engine.game.getIFps() * LINE_CANVAS_SPEED;
					
					// move canvas
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
					mouse_time += (max_time - min_time) * mouse_dx / (width - LINE_HANDLE_WIDTH);
					
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
						forloop(int i = 0; curves.size()) {
							foreach(LineKey key; curves[i].keys) {
								int position_y = LINE_HEIGHT * i + LINE_HEIGHT;
								if(position_y < p0.y) continue;
								if(position_y - LINE_HANDLE_HEIGHT - LINE_HANDLE_ARROW > p2.y) continue;
								int position_x = key.getPositionX();
								if(position_x + LINE_HANDLE_WIDTH / 2 < p0.x) continue;
								if(position_x - LINE_HANDLE_WIDTH / 2 > p1.x) continue;
								selected_curves.append(curves[i]);
								selected_keys.append(key);
								key.setOrder(order);
							}
						}
						forloop(int i = 0; old_selected_keys.size()) {
							if(selected_keys.find(old_selected_keys[i]) != -1) continue;
							selected_curves.append(old_selected_curves[i]);
							selected_keys.append(old_selected_keys[i]);
						}
						keys_changed = 1;
					}
					// move keys
					else if(pressed == 2) {
						
						// snap time
						float time = mouse_time;
						float prev_time = getPrevTime(time,1);
						float next_time = getNextTime(time,1);
						float threshold = LINE_THRESHOLD * (max_time - min_time) / width;
						if(getKeySnap() || getKeyState(APP_KEY_ALT)) {
							if(abs(time - prev_time) < threshold) time = prev_time;
							if(abs(time - next_time) < threshold) time = next_time;
							if(abs(time - current_time) < threshold) time = current_time;
						}
						
						// move keys
						moveSelectedKeys(time);
						
						// update cursor
						if(mouse_y < 0) setMouseCursor(CURSOR_NA);
					}
				}
			}
			
			/////////////////////////////////
			// middle mouse button
			/////////////////////////////////
			
			if(getMouseButton() & APP_BUTTON_MIDDLE) {
				
				if(pressed == 0) {
					
					pressed = 3;
				}
				
				// move canvas
				if(pressed == 3) {
					
					int mouse_dx = getMouseDX();
					int mouse_dy = getMouseDY();
					
					// move scrollbox
					canvas_sb.setHScrollValue(canvas_sb.getHScrollValue() - mouse_dx * canvas_sb.getScrollScale());
					canvas_sb.setVScrollValue(canvas_sb.getVScrollValue() - mouse_dy * canvas_sb.getScrollScale());
				}
			}
			
			/////////////////////////////////
			// right mouse button
			/////////////////////////////////
			
			if(getMouseButton() & APP_BUTTON_RIGHT) {
				
				int old_mouse_x;
				
				int width;
				int old_width;
				int old_offset_x;
				
				if(pressed == 0) {
					
					pressed = 4;
					
					// save values
					old_mouse_x = canvas.getMouseX();
					
					// save scrollbox values
					width = canvas.getWidth();
					old_width = canvas.getWidth();
					old_offset_x = canvas_sb.getMouseX();
				}
				
				// scale canvas
				else if(pressed == 4) {
					
					int mouse_dy = getMouseDY();
					
					float scale = (1.0f + mouse_dy * 0.01f);
					
					// horizontal scale
					width = clamp(width * scale,canvas_sb.getHScrollFrameSize() / canvas_sb.getScrollScale(),LINE_CANVAS_WIDTH);
					canvas.setWidth(width);
					
					// update scrollbar
					canvas_sb.arrange();
					canvas_sb.setHScrollValue((float(old_mouse_x) * width / old_width - old_offset_x) * canvas_sb.getScrollScale());
				}
			}
		}
		
		/*
		 */
		void released_callback() {
			
			int mouse_y = canvas.getMouseY();
			
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
			// remove selected key
			else if(pressed == 2 && mouse_y < 0) {
				removeSelectedKeys();
			}
			
			pressed = 0;
		}
		
		/*
		 */
		void double_clicked_callback(Line line) {
			
			int mouse_x = line.canvas.getMouseX();
			int mouse_y = line.canvas.getMouseY();
			
			// polygon intersection
			int id = line.canvas.getPolygonIntersection(mouse_x,mouse_y);
			if(id != -1) {
				
				// update current key
				if(line.current_key != NULL && line.current_key.isPolygon(id)) {
					string func = line.current_curve.get_update_func();
					if(strlen(func)) call(func,line.current_key);
					line.pressed = 0;
					return;
				}
				
				// find selected curve
				foreach(LineCurve curve; line.curves) {
					
					// curve is selected
					if(curve.isPolygon(id)) {
						
						// mouse key
						float mouse_key = line.getMinTime() + (line.getMaxTime() - line.getMinTime()) * (mouse_x - LINE_HANDLE_WIDTH / 2) / (line.width - LINE_HANDLE_WIDTH);
						
						// create key
						LineKey key = curve.create_key(mouse_key);
						curve.addKey(key);
						
						// current key
						line.setCurrentCurve(curve);
						line.setCurrentKey(key);
						
						// selected keys
						line.setSelectedCurves((curve));
						line.setSelectedKeys((key));
						
						// update keys
						line.updateSelectedKeys();
						
						// run callback
						line.runCallback(LINE_KEY_CREATED);
						
						line.pressed = 5;
						return;
					}
				}
			}
			
			line.pressed = 0;
		}
		
		/*
		 */
		void callback_redirector(Line line,string name) {
			line.call(name);
		}
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_LINE_H__ */
