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


#ifndef __UNIGINE_WIDGET_BUTTON_H__
#define __UNIGINE_WIDGET_BUTTON_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class Button : Widget {
		
		WidgetButton button;
		
		// constructor
		Button(string str = 0) {
			if(str is WidgetButton) {
				owner = 0;
				button = str;
			} else {
				button = new WidgetButton(engine.getGui(),str);
			}
			widget = button;
		}
		
		// background flag
		void setBackground(int background) { button.setBackground(background); }
		int getBackground() { return button.getBackground(); }
		
		// toggleable flag
		void setToggleable(int toggleable) { button.setToggleable(toggleable); }
		int isToggleable() { return button.isToggleable(); }
		
		// toggle status
		void setToggled(int toggled) { button.setToggled(toggled); }
		int isToggled() { return button.isToggled(); }
		
		// texture
		void setImage(Image image) { button.setImage(image); }
		Image getImage() { return button.getImage(); }
		void setTexture(string name) { button.setTexture(name); }
		string getTexture() { return button.getTexture(); }
		
		// text
		void setTextAlign(int align) { button.setTextAlign(align); }
		int getTextAlign() { return button.getTextAlign(); }
		void setText(string str) { button.setText(str); }
		string getText() { return button.getText(); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_BUTTON_H__ */
