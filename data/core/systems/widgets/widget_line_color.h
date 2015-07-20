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


#ifndef __UNIGINE_WIDGET_LINE_COLOR_H__
#define __UNIGINE_WIDGET_LINE_COLOR_H__

#include <core/systems/widgets/widget_line.h>
#include <core/systems/widgets/widget_dialog_color.h>

/*
 */
namespace Unigine::Widgets {
	
	/**************************************************************************\
	*
	* LineKeyColor
	*
	\**************************************************************************/
	
	/*
	 */
	class LineKeyColor : LineKey {
		
		vec4 color;
		
		/*
		 */
		LineKeyColor(float t,vec4 c) : LineKey(t) {
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
		vec4 get_color() {
			return color;
		}
	};
	
	/**************************************************************************\
	*
	* LineCurveColor
	*
	\**************************************************************************/
	
	/*
	 */
	class LineCurveColor : LineCurve {
		
		/*
		 */
		LineCurveColor() {
			
		}
		
		/*
		 */
		vec4 get_color(float time) {
			
			int left = indices.left(time);
			int right = indices.right(time);
			
			if(left != -1 && right != -1) {
				
				LineKeyColor k0 = keys[left];
				LineKeyColor k1 = keys[right];
				float t0 = k0.time;
				float t1 = k1.time;
				
				if(t0 == t1) return k0.color;
				float k = (time - t0) / (t1 - t0);
				k = k * k * (3.0f - 2.0f * k);
				
				return lerp(k0.color,k1.color,k);
			}
			
			if(left != -1) return LineKeyColor(keys[left]).color;
			if(right != -1) return LineKeyColor(keys[right]).color;
			
			return vec4(0.0f,0.0f,0.0f,1.0f);
		}
		
		/*
		 */
		LineKeyColor create_key(float time) {
			return new LineKeyColor(time,get_color(time))
		}
		
		/*
		 */
		LineKeyColor clone_key(LineKeyColor key) {
			return new LineKeyColor(key.getTime(),key.getColor())
		}
		
		/*
		 */
		void update_key(LineKeyColor key) {
			
			// line
			LineCurve curve = key.getCurve();
			Line line = curve.getLine();
			
			// save colors
			vec4 colors[0];
			foreach(LineKey key; line.selected_keys) {
				if(key is LineKeyColor) {
					colors.append(LineKeyColor(key).getColor());
				} else {
					colors.append(vec4_one);
				}
			}
			
			// dialog callback
			CallbackBase callback = MakeCallback(get_function("Unigine::Widgets::LineCurveColor::color_changed",2),line);
			
			// dialog result
			vec4 color = key.getColor();
			if(dialogColor("Select color",color,callback) == 0) {
				forloop(int i = 0; line.getNumSelectedKeys()) {
					LineKey key = line.getSelectedKey(i);
					if(key is LineKeyColor) {
						LineKeyColor(key).setColor(colors[i]);
					}
				}
				line.updateSelectedKeys();
			}
			
			delete callback;
		}
		
		void color_changed(Line line,vec4 color) {
			foreach(LineKey key; line.selected_keys) {
				if(key is LineKeyColor) {
					LineKeyColor(key).setColor(color);
				}
			}
			line.updateSelectedKeys();
		}
		
		string get_update_func() {
			return "Unigine::Widgets::LineCurveColor::update_key";
		}
		
		/*
		 */
		void create_image(int width) {
			image.create2D(width,1,IMAGE_FORMAT_RGBA8);
		}
		
		void update_image(float step,float offset) {
			forloop(int x = 0; image.getWidth()) {
				float time = step * x + offset;
				vec4 color = get_color(time);
				image.set2D(x,0,color);
			}
		}
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_LINE_COLOR_H__ */
