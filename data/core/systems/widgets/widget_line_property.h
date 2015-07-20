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


#ifndef __UNIGINE_WIDGET_LINE_PROPERTY_H__
#define __UNIGINE_WIDGET_LINE_PROPERTY_H__

#include <core/systems/widgets/widget_line.h>
#include <core/systems/widgets/widget_dialog_property.h>

/*
 */
namespace Unigine::Widgets {
	
	/**************************************************************************\
	*
	* LineKeyProperty
	*
	\**************************************************************************/
	
	/*
	 */
	class LineKeyProperty : LineKey {
		
		int value;
		
		string property;
		
		/*
		 */
		LineKeyProperty(float t,int v,string m = "") : LineKey(t) {
			value = v;
			property = m;
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
		void setProperty(string m) {
			property = m;
		}
		
		string getProperty() {
			return property;
		}
		
		/*
		 */
		string get_text() {
			if(value) return property;
			return "";
		}
		
		int get_direction() {
			if(value) return LINE_KEY_UP;
			return LINE_KEY_DOWN;
		}
		
		vec4 get_color() {
			return LineCurveProperty(curve).getColor();
		}
	};
	
	/**************************************************************************\
	*
	* LineCurveProperty
	*
	\**************************************************************************/
	
	/*
	 */
	class LineCurveProperty : LineCurve {
		
		vec4 color;
		
		/*
		 */
		LineCurveProperty(vec4 c) {
			
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
			if(left != -1) return LineKeyProperty(keys[left]).value;
			
			int right = indices.right(time);
			if(right != -1) return LineKeyProperty(keys[right]).value;
			
			return 0;
		}
		
		/*
		 */
		LineKeyProperty create_key(float time) {
			return new LineKeyProperty(time,0)
		}
		
		LineKeyProperty clone_key(LineKeyProperty key) {
			return new LineKeyProperty(key.getTime(),key.getValue(),key.getProperty())
		}
		
		/*
		 */
		void update_key(LineKeyProperty key) {
			
			// line
			LineCurve curve = key.getCurve();
			Line line = curve.getLine();
			
			// dialog result
			string property = key.getProperty();
			if(dialogProperty("Select property",property)) {
				foreach(LineKey key; line.selected_keys) {
					if(key is LineKeyProperty) {
						LineKeyProperty(key).setValue(1);
						LineKeyProperty(key).setProperty(property);
					}
				}
				line.updateSelectedKeys();
			}
		}
		
		string get_update_func() {
			return "Unigine::Widgets::LineCurveProperty::update_key";
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

#endif /* __UNIGINE_WIDGET_LINE_PROPERTY_H__ */
