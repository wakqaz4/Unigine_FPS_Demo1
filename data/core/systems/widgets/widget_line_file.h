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


#ifndef __UNIGINE_WIDGET_LINE_FILE_H__
#define __UNIGINE_WIDGET_LINE_FILE_H__

#include <core/systems/widgets/widget_line.h>
#include <core/systems/widgets/widget_dialog_file.h>

/*
 */
namespace Unigine::Widgets {
	
	/**************************************************************************\
	*
	* LineKeyFile
	*
	\**************************************************************************/
	
	/*
	 */
	class LineKeyFile : LineKey {
		
		int value;
		
		string name;
		
		/*
		 */
		LineKeyFile(float t,int v,string n = "") : LineKey(t) {
			value = v;
			name = n;
		}
		
		/*
		 */
		void setValue(int v) {
			value = v;
		}
		
		int getValue() {
			return value;
		}
		
		/*
		 */
		void setName(string n) {
			name = n;
		}
		
		string getName() {
			return name;
		}
		
		/*
		 */
		string get_text() {
			if(value) return basename(name);
			return "";
		}
		
		int get_direction() {
			if(value) return LINE_KEY_UP;
			return LINE_KEY_DOWN;
		}
		
		vec4 get_color() {
			return LineCurveFile(curve).getColor();
		}
	};
	
	/**************************************************************************\
	*
	* LineCurveFile
	*
	\**************************************************************************/
	
	/*
	 */
	class LineCurveFile : LineCurve {
		
		vec4 color;
		
		string filter;
		
		/*
		 */
		LineCurveFile(vec4 c,string f = "") {
			
			color = c;
			filter = f;
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
		void setFilter(string f) {
			filter = f;
		}
		
		string getFilter() {
			return filter;
		}
		
		/*
		 */
		int get_value(float time) {
			
			int left = indices.left(time);
			if(left != -1) return LineKeyFile(keys[left]).value;
			
			int right = indices.right(time);
			if(right != -1) return LineKeyFile(keys[right]).value;
			
			return 0;
		}
		
		/*
		 */
		LineKeyFile create_key(float time) {
			return new LineKeyFile(time,0)
		}
		
		LineKeyFile clone_key(LineKeyFile key) {
			return new LineKeyFile(key.getTime(),key.getValue(),key.getName())
		}
		
		/*
		 */
		void update_key(LineKeyFile key) {
			
			// line
			LineCurveFile curve = key.getCurve();
			Line line = curve.getLine();
			
			// dialog result
			string name = key.getName();
			if(dialogFile("Select file",curve.filter,name)) {
				foreach(LineKey key; line.selected_keys) {
					if(key is LineKeyFile) {
						LineKeyFile(key).setValue(1);
						LineKeyFile(key).setName(name);
					}
				}
				line.updateSelectedKeys();
			}
		}
		
		string get_update_func() {
			return "Unigine::Widgets::LineCurveFile::update_key";
		}
		
		/*
		 */
		void create_image(int width) {
			image.create2D(width,1,IMAGE_FORMAT_RGBA8);
		}
		
		void update_image(float step,float offset) {
			forloop(int x = 0; image.getWidth()) {
				float time = step * x + offset;
				if(get_value(time)) image.set2D(x,0,color);
				else image.set2D(x,0,vec4_zero);
			}
		}
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_LINE_FILE_H__ */
