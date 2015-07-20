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


#ifndef __UNIGINE_WIDGET_LABEL_H__
#define __UNIGINE_WIDGET_LABEL_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class Label : Widget {
		
		WidgetLabel label;
		
		// constructor
		Label(string str = 0) {
			if(str is WidgetLabel) {
				owner = 0;
				label = str;
			} else {
				label = new WidgetLabel(engine.getGui(),str);
			}
			widget = label;
		}
		
		// text
		void setTextAlign(int align) { label.setTextAlign(align); }
		int getTextAlign() { return label.getTextAlign(); }
		void setText(string str) { label.setText(str); }
		string getText() { return label.getText(); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_LABEL_H__ */
