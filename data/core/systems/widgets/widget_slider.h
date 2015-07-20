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


#ifndef __UNIGINE_WIDGET_SLIDER_H__
#define __UNIGINE_WIDGET_SLIDER_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class Slider : Widget {
		
		WidgetSlider slider;
		
		// constructor
		Slider(int min = 0,int max = 100,int value = 0) {
			if(min is WidgetSlider) {
				owner = 0;
				slider = min;
			} else {
				slider = new WidgetSlider(engine.getGui(),min,max,value);
			}
			widget = slider;
		}
		
		// orientation
		void setOrientation(int orientation) { slider.setOrientation(orientation); }
		int getOrientation() { return slider.getOrientation(); }
		
		// minimum value
		void setMinValue(int value) { slider.setMinValue(value); }
		int getMinValue() { return slider.getMinValue(); }
		
		// maximum value
		void setMaxValue(int value) { slider.setMaxValue(value); }
		int getMaxValue() { return slider.getMaxValue(); }
		
		// value
		void setValue(int value) { slider.setValue(value); }
		int getValue() { return slider.getValue(); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_SLIDER_H__ */
