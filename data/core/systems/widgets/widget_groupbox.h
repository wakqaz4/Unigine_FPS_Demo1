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


#ifndef __UNIGINE_WIDGET_GROUPBOX_H__
#define __UNIGINE_WIDGET_GROUPBOX_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class GroupBox : Widget {
		
		WidgetGroupBox groupbox;
		
		// constructor
		GroupBox(string str = 0,int x = 0,int y = 0) {
			if(str is WidgetGroupBox) {
				owner = 0;
				groupbox = str;
			} else {
				groupbox = new WidgetGroupBox(engine.getGui(),str,x,y);
			}
			widget = groupbox;
		}
		
		// space size
		void setSpace(int x,int y) { groupbox.setSpace(x,y); }
		int getSpaceX() { return groupbox.getSpaceX(); }
		int getSpaceY() { return groupbox.getSpaceY(); }
		
		// padding size
		void setPadding(int l,int r,int t,int b) { groupbox.setPadding(l,r,t,b); }
		int getPaddingLeft() { return groupbox.getPaddingLeft(); }
		int getPaddingRight() { return groupbox.getPaddingRight(); }
		int getPaddingTop() { return groupbox.getPaddingTop(); }
		int getPaddingBottom() { return groupbox.getPaddingBottom(); }
		
		// border flag
		void setBorder(int border) { groupbox.setBorder(border); }
		int getBorder() { return groupbox.getBorder(); }
		
		// stencil flag
		void setStencil(int stencil) { groupbox.setStencil(stencil); }
		int getStencil() { return groupbox.getStencil(); }
		
		// color multiplier
		void setColor(vec4 color) { groupbox.setColor(color); }
		vec4 getColor() { return groupbox.getColor(); }
		
		// background flag
		void setBackground(int background) { groupbox.setBackground(background); }
		int getBackground() { return groupbox.getBackground(); }
		
		// text
		void setText(string str) { groupbox.setText(str); }
		string getText() { return groupbox.getText(); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_GROUPBOX_H__ */
