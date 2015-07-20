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


#ifndef __UNIGINE_WIDGET_SPINBOX_H__
#define __UNIGINE_WIDGET_SPINBOX_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class SpinBox : Widget {
		
		WidgetSpinBox spinbox;
		
		// constructor
		SpinBox(int min = 0,int max = 100,int value = 0) {
			if(min is WidgetSpinBox) {
				owner = 0;
				spinbox = min;
			} else {
				spinbox = new WidgetSpinBox(engine.getGui(),min,max,value);
			}
			widget = spinbox;
		}
		
		// minimum value
		void setMinValue(int value) { spinbox.setMinValue(value); }
		int getMinValue() { return spinbox.getMinValue(); }
		
		// maximum value
		void setMaxValue(int value) { spinbox.setMaxValue(value); }
		int getMaxValue() { return spinbox.getMaxValue(); }
		
		// value
		void setValue(int value) { spinbox.setValue(value); }
		int getValue() { return spinbox.getValue(); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_SPINBOX_H__ */
