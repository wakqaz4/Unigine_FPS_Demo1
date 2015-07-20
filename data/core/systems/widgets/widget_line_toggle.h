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


#ifndef __UNIGINE_WIDGET_LINE_TOGGLE_H__
#define __UNIGINE_WIDGET_LINE_TOGGLE_H__

#include <core/systems/widgets/widget_line.h>

/*
 */
namespace Unigine::Widgets {
	
	/**************************************************************************\
	*
	* LineKeyToggle
	*
	\**************************************************************************/
	
	/*
	 */
	class LineKeyToggle : LineKey {
		
		int value;
		
		/*
		 */
		LineKeyToggle(float t,int v) : LineKey(t) {
			value = v;
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
		int get_direction() {
			if(value) return LINE_KEY_UP;
			return LINE_KEY_DOWN;
		}
		
		vec4 get_color() {
			return LineCurveToggle(curve).getColor();
		}
	};
	
	/**************************************************************************\
	*
	* LineCurveToggle
	*
	\**************************************************************************/
	
	/*
	 */
	class LineCurveToggle : LineCurve {
		
		vec4 color;
		
		/*
		 */
		LineCurveToggle(vec4 c) {
			
			color = c;
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
		int get_value(float time) {
			
			int left = indices.left(time);
			if(left != -1) return LineKeyToggle(keys[left]).value;
			
			int right = indices.right(time);
			if(right != -1) return LineKeyToggle(keys[right]).value;
			
			return 0;
		}
		
		/*
		 */
		LineKeyToggle create_key(float time) {
			return new LineKeyToggle(time,get_value(time))
		}
		
		LineKeyToggle clone_key(LineKeyToggle key) {
			return new LineKeyToggle(key.getTime(),key.getValue())
		}
		
		/*
		 */
		void update_key(LineKeyToggle key) {
			key.setValue(!key.getValue());
			key.curve.updateKeys();
			key.curve.line.runCallback(LINE_KEY_CHANGED);
		}
		
		string get_update_func() {
			return "Unigine::Widgets::LineCurveToggle::update_key";
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

#endif /* __UNIGINE_WIDGET_LINE_TOGGLE_H__ */
