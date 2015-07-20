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


#ifndef __UNIGINE_WIDGET_WINDOW_H__
#define __UNIGINE_WIDGET_WINDOW_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class Window : Widget {
		
		WidgetWindow window;
		
		#ifdef HAS_INTERFACE
			InterfaceWindow interface;
		#endif
		
		// constructor
		Window(string str = 0,int x = 0,int y = 0,int flags = 0) {
			if(str is WidgetWindow) {
				owner = 0;
				window = str;
			} else {
				window = new WidgetWindow(engine.getGui(),str,x,y);
			}
			#ifdef HAS_INTERFACE
				interface = new InterfaceWindow(flags);
			#endif
			widget = window;
		}
		~Window() {
			#ifdef HAS_INTERFACE
				delete interface;
			#endif
		}
		
		// interface
		#ifdef HAS_INTERFACE
			InterfaceWindow getInterface() { return interface; }
		#endif
		
		// movable flag
		void setMoveable(int moveable) { window.setMoveable(moveable); }
		int isMoveable() { return window.isMoveable(); }
		
		// sizeable flag
		void setSizeable(int sizeable) { window.setSizeable(sizeable); }
		int isSizeable() { return window.isSizeable(); }
		
		// color multiplier
		void setColor(vec4 color) { window.setColor(color); }
		vec4 getColor() { return window.getColor(); }
		
		// space size
		void setSpace(int x,int y) { window.setSpace(x,y); }
		int getSpaceX() { return window.getSpaceX(); }
		int getSpaceY() { return window.getSpaceY(); }
		
		// padding size
		void setPadding(int l,int r,int t,int b) { window.setPadding(l,r,t,b); }
		int getPaddingLeft() { return window.getPaddingLeft(); }
		int getPaddingRight() { return window.getPaddingRight(); }
		int getPaddingTop() { return window.getPaddingTop(); }
		int getPaddingBottom() { return window.getPaddingBottom(); }
		
		// text
		void setText(string str) { window.setText(str); }
		string getText() { return window.getText(); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_WINDOW_H__ */
