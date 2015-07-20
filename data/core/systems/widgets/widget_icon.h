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


#ifndef __UNIGINE_WIDGET_ICON_H__
#define __UNIGINE_WIDGET_ICON_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class Icon : Widget {
		
		WidgetIcon icon;
		
		// constructor
		Icon(string str = 0,int width = 0,int height = 0) {
			if(str is WidgetIcon) {
				owner = 0;
				icon = str;
			} else {
				icon = new WidgetIcon(engine.getGui(),str,width,height);
			}
			widget = icon;
		}
		
		// toggleable flag
		void setToggleable(int toggleable) { icon.setToggleable(toggleable); }
		int isToggleable() { return icon.isToggleable(); }
		
		// toggle status
		void setToggled(int toggled) { icon.setToggled(toggled); }
		int isToggled() { return icon.isToggled(); }
		
		// texture
		void setImage(Image image) { icon.setImage(image); }
		Image getImage() { return icon.getImage(); }
		void setTexture(string name) { icon.setTexture(name); }
		string getTexture() { return icon.getTexture(); }
		
		// text
		void setTextAlign(int align) { icon.setTextAlign(align); }
		int getTextAlign() { return icon.getTextAlign(); }
		void setText(string str) { icon.setText(str); }
		string getText() { return icon.getText(); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_ICON_H__ */
