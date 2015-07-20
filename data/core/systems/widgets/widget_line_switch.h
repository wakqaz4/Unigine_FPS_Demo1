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


#ifndef __UNIGINE_WIDGET_LINE_SWITCH_H__
#define __UNIGINE_WIDGET_LINE_SWITCH_H__

#include <core/systems/widgets/widget_dialog.h>
#include <core/systems/widgets/widget_gridbox.h>
#include <core/systems/widgets/widget_label.h>
#include <core/systems/widgets/widget_editline.h>
#include <core/systems/widgets/widget_line.h>

/*
 */
namespace Unigine::Widgets {
	
	/**************************************************************************\
	*
	* LineKeySwitch
	*
	\**************************************************************************/
	
	/*
	 */
	class LineKeySwitch : LineKey {
		
		int value;
		
		/*
		 */
		LineKeySwitch(float t,int v) : LineKey(t) {
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
		string get_text() {
			if(value) return string(value);
			return "";
		}
		
		int get_direction() {
			if(value) return LINE_KEY_UP;
			return LINE_KEY_DOWN;
		}
		
		vec4 get_color() {
			return LineCurveSwitch(curve).getColor();
		}
	};
	
	/**************************************************************************\
	*
	* LineCurveSwitch
	*
	\**************************************************************************/
	
	/*
	 */
	class LineCurveSwitch : LineCurve {
		
		vec4 color;
		
		int min_value;
		int max_value;
		
		/*
		 */
		LineCurveSwitch(vec4 c,int min = -1000000,int max = 1000000) {
			
			color = c;
			
			min_value = min;
			max_value = max;
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
		void setMinValue(int v) {
			min_value = v;
		}
		
		int getMinValue() {
			return min_value;
		}
		
		/*
		 */
		void setMaxValue(int v) {
			max_value = v;
		}
		
		int getMaxValue() {
			return max_value;
		}
		
		/*
		 */
		int get_value(float time) {
			
			int left = indices.left(time);
			if(left != -1) return LineKeySwitch(keys[left]).value;
			
			int right = indices.right(time);
			if(right != -1) return LineKeySwitch(keys[right]).value;
			
			return 0;
		}
		
		/*
		 */
		LineKeySwitch create_key(float time) {
			return new LineKeySwitch(time,get_value(time))
		}
		
		LineKeySwitch clone_key(LineKeySwitch key) {
			return new LineKeySwitch(key.getTime(),key.getValue())
		}
		
		/*
		 */
		void update_key(LineKeySwitch key) {
			
			Dialog dialog;
			EditLine time_el;
			EditLine value_el;
			
			// create dialog
			if(dialog == NULL) {
				
				dialog = new Dialog("Key parameters");
				dialog.setSizeable(1);
				
				VBox vbox = new VBox();
				dialog.addChild(vbox);
				
				GridBox gridbox = new GridBox(2,4,4);
				vbox.addChild(gridbox,ALIGN_EXPAND);
				
				gridbox.addChild(new Label(TR("Time:")),ALIGN_RIGHT);
				time_el = new EditLine();
				time_el.setValidator(VALIDATOR_FLOAT);
				gridbox.addChild(time_el,ALIGN_EXPAND);
				
				gridbox.addChild(new Label(TR("Value:")),ALIGN_RIGHT);
				value_el = new EditLine();
				value_el.setValidator(VALIDATOR_INT);
				gridbox.addChild(value_el,ALIGN_EXPAND);
			}
			
			// line
			LineCurveSwitch curve = key.getCurve();
			Line line = curve.getLine();
			
			// save keys
			int has_time = 1;
			int has_value = 1;
			float times[line.getNumSelectedKeys()];
			int values[line.getNumSelectedKeys()];
			forloop(int i = 0; line.getNumSelectedKeys()) {
				LineKey key = line.getSelectedKey(i);
				if(key is LineKeySwitch) {
					values[i] = LineKeySwitch(key).getValue();
					if(i && values[0] != values[i]) has_value = 0;
				}
				times[i] = key.getTime();
				if(i && times[0] != times[i]) has_time = 0;
			}
			
			// key parameters
			time_el.setText((has_time) ? widgetFormat(key.getTime()) : "");
			value_el.setText((has_value) ? string(key.getValue()) : "");
			
			// register callbacks
			time_el.setCallback(PRESSED,functionid(time_pressed),line,time_el);
			value_el.setCallback(PRESSED,functionid(value_pressed),line,value_el);
			
			// show dialog
			dialog.arrange();
			Unigine::Widgets::addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
			dialog.setPermanentFocus();
			
			// wait dialog
			while(dialog.isDone() == 0) wait;
			
			// dialog result
			if(dialog.isOkClicked()) {
				float time = clamp(float(time_el.getText()),line.getMinTime(),line.getMaxTime());
				foreach(LineKey key; line.selected_keys) {
					if(key is LineKeySwitch) {
						LineCurveSwitch curve = key.getCurve();
						if(strlen(value_el.getText())) LineKeySwitch(key).setValue(clamp(int(value_el.getText()),curve.getMinValue(),curve.getMaxValue()));
					}
					if(strlen(time_el.getText())) key.setTime(time);
				}
			} else {
				forloop(int i = 0; line.getNumSelectedKeys()) {
					LineKey key = line.getSelectedKey(i);
					if(key is LineKeySwitch) {
						LineKeySwitch(key).setValue(values[i]);
					}
					key.setTime(times[i]);
				}
			}
			
			// update widgets
			line.updateSelectedKeys();
			
			// hide dialog
			Unigine::Widgets::removeChild(dialog);
		}
		
		void time_pressed(Line line,EditLine time_el) {
			if(strlen(time_el.getText()) == 0) return;
			float time = clamp(float(time_el.getText()),line.getMinTime(),line.getMaxTime());
			foreach(LineKey key; line.selected_keys) {
				key.setTime(time);
			}
			line.updateSelectedKeys();
		}
		
		void value_pressed(Line line,EditLine value_el) {
			if(strlen(value_el.getText()) == 0) return;
			foreach(LineKey key; line.selected_keys) {
				if(key is LineKeySwitch) {
					LineCurveSwitch curve = key.getCurve();
					LineKeySwitch(key).setValue(clamp(int(value_el.getText()),curve.getMinValue(),curve.getMaxValue()));
				}
			}
			line.updateSelectedKeys();
		}
		
		string get_update_func() {
			return "Unigine::Widgets::LineCurveSwitch::update_key";
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

#endif /* __UNIGINE_WIDGET_LINE_SWITCH_H__ */
