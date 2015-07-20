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


#ifndef __UNIGINE_WIDGET_VBOX_H__
#define __UNIGINE_WIDGET_VBOX_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class VBox : Widget {
		
		WidgetVBox vbox;
		
		// constructor
		VBox(int x = 0,int y = 0) {
			if(x is WidgetVBox) {
				owner = 0;
				vbox = x;
			} else {
				vbox = new WidgetVBox(engine.getGui(),x,y);
			}
			widget = vbox;
		}
		
		// space size
		void setSpace(int x,int y) { vbox.setSpace(x,y); }
		int getSpaceX() { return vbox.getSpaceX(); }
		int getSpaceY() { return vbox.getSpaceY(); }
		
		// padding size
		void setPadding(int l,int r,int t,int b) { vbox.setPadding(l,r,t,b); }
		int getPaddingLeft() { return vbox.getPaddingLeft(); }
		int getPaddingRight() { return vbox.getPaddingRight(); }
		int getPaddingTop() { return vbox.getPaddingTop(); }
		int getPaddingBottom() { return vbox.getPaddingBottom(); }
		
		// stencil flag
		void setStencil(int stencil) { vbox.setStencil(stencil); }
		int getStencil() { return vbox.getStencil(); }
		
		// color multiplier
		void setColor(vec4 color) { vbox.setColor(color); }
		vec4 getColor() { return vbox.getColor(); }
		
		// background flag
		void setBackground(int background) { vbox.setBackground(background); }
		int getBackground() { return vbox.getBackground(); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_VBOX_H__ */
