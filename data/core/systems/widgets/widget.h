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


#ifndef __UNIGINE_WIDGET_H__
#define __UNIGINE_WIDGET_H__

/*
 */
#ifndef TR
	#define TR(TEXT)	TEXT
#endif

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	enum {
		SHOW = GUI_SHOW,
		HIDE = GUI_HIDE,
		FOCUS_IN = GUI_FOCUS_IN,
		FOCUS_OUT = GUI_FOCUS_OUT,
		CHANGED = GUI_CHANGED,
		CLICKED = GUI_CLICKED,
		DOUBLE_CLICKED = GUI_DOUBLE_CLICKED,
		PRESSED = GUI_PRESSED,
		RELEASED = GUI_RELEASED,
		ENTER = GUI_ENTER,
		LEAVE = GUI_LEAVE,
		DRAG_MOVE = GUI_DRAG_MOVE,
		DRAG_DROP = GUI_DRAG_DROP,
	};
	
	/*
	 */
	enum {
		VERTICAL = GUI_VERTICAL,
		HORIZONTAL = GUI_HORIZONTAL,
	};
	
	/*
	 */
	enum {
		VALIDATOR_ANY = GUI_VALIDATOR_ANY,
		VALIDATOR_INT = GUI_VALIDATOR_INT,
		VALIDATOR_UINT = GUI_VALIDATOR_UINT,
		VALIDATOR_FLOAT = GUI_VALIDATOR_FLOAT,
		VALIDATOR_ASCII = GUI_VALIDATOR_ASCII,
	};
	
	/*
	 */
	enum {
		ATTACH_LOG10 = GUI_ATTACH_LOG10,
		ATTACH_MIN_EXPAND = GUI_ATTACH_MIN_EXPAND,
		ATTACH_MAX_EXPAND = GUI_ATTACH_MAX_EXPAND,
	};
	
	/*
	 */
	enum {
		ALIGN_CENTER = GUI_ALIGN_CENTER,
		ALIGN_LEFT = GUI_ALIGN_LEFT,
		ALIGN_RIGHT = GUI_ALIGN_RIGHT,
		ALIGN_TOP = GUI_ALIGN_TOP,
		ALIGN_BOTTOM = GUI_ALIGN_BOTTOM,
		ALIGN_EXPAND = GUI_ALIGN_EXPAND,
		ALIGN_OVERLAP = GUI_ALIGN_OVERLAP,
		ALIGN_BACKGROUND = GUI_ALIGN_BACKGROUND,
		ALIGN_FIXED = GUI_ALIGN_FIXED,
	};
	
	/*
	 */
	enum {
		BLEND_NONE = GUI_BLEND_NONE,
		BLEND_ZERO = GUI_BLEND_ZERO,
		BLEND_ONE = GUI_BLEND_ONE,
		BLEND_SRC_COLOR = GUI_BLEND_SRC_COLOR,
		BLEND_ONE_MINUS_SRC_COLOR = GUI_BLEND_ONE_MINUS_SRC_COLOR,
		BLEND_SRC_ALPHA = GUI_BLEND_SRC_ALPHA,
		BLEND_ONE_MINUS_SRC_ALPHA = GUI_BLEND_ONE_MINUS_SRC_ALPHA,
		BLEND_DEST_COLOR = GUI_BLEND_DEST_COLOR,
		BLEND_ONE_MINUS_DEST_COLOR = GUI_BLEND_ONE_MINUS_DEST_COLOR,
		BLEND_DEST_ALPHA = GUI_BLEND_DEST_ALPHA,
		BLEND_ONE_MINUS_DEST_ALPHA = GUI_BLEND_ONE_MINUS_DEST_ALPHA,
	};
	
	/*
	 */
	enum {
		CURSOR_NONE = GUI_CURSOR_NONE,
		CURSOR_ARROW = GUI_CURSOR_ARROW,
		CURSOR_NA = GUI_CURSOR_NA,
		CURSOR_DND = GUI_CURSOR_DND,
		CURSOR_WIDTH = GUI_CURSOR_WIDTH,
		CURSOR_HEIGHT = GUI_CURSOR_HEIGHT,
		CURSOR_NESW = GUI_CURSOR_NESW,
		CURSOR_NWSE = GUI_CURSOR_NWSE,
		CURSOR_MOVE = GUI_CURSOR_MOVE,
		CURSOR_ROTATE = GUI_CURSOR_ROTATE,
		CURSOR_SCALE = GUI_CURSOR_SCALE,
	};
	
	/*
	 */
	enum {
		VBOX = WIDGET_VBOX,
		HBOX = WIDGET_HBOX,
		VPANED = WIDGET_VPANED,
		HPANED = WIDGET_HPANED,
		GRIDBOX = WIDGET_GRIDBOX,
		GROUPBOX = WIDGET_GROUPBOX,
		TABBOX = WIDGET_TABBOX,
		SCROLLBOX = WIDGET_SCROLLBOX,
		WINDOW = WIDGET_WINDOW,
		DIALOG = WIDGET_DIALOG,
		LABEL = WIDGET_LABEL,
		SPRITE = WIDGET_SPRITE,
		CANVAS = WIDGET_CANVAS,
		ICON = WIDGET_ICON,
		BUTTON = WIDGET_BUTTON,
		CHECKBOX = WIDGET_CHECKBOX,
		COMBOBOX = WIDGET_COMBOBOX,
		MENUBAR = WIDGET_MENUBAR,
		MENUBOX = WIDGET_MENUBOX,
		LISTBOX = WIDGET_LISTBOX,
		TREEBOX = WIDGET_TREEBOX,
		SPACER = WIDGET_SPACER,
		SLIDER = WIDGET_SLIDER,
		SPINBOX = WIDGET_SPINBOX,
		EDITLINE = WIDGET_EDITLINE,
		EDITTEXT = WIDGET_EDITTEXT,
	};
	
	/**************************************************************************\
	*
	* Widget
	*
	\**************************************************************************/
	
	/*
	 */
	class Widget {
		
		int owner;			// owner flag
		
		Widget parent;		// parent
		Widget childs[0];	// childs
		
		::Widget widget;	// widget
		
		// constructor
		Widget() {
			owner = 1;
		}
		
		Widget(int w) {
			owner = 0;
			widget = w;
		}
		
		~Widget() {
			if(owner) delete widget;
			if(parent != NULL) parent.removeChild(this);
			forloop(int i = 0; childs.size()) {
				childs[i].parent = NULL;
			}
		}
		
		// type
		int getType() { return widget.getType(); }
		string getTypeName() { return widget.getTypeName(); }
		
		// gui
		Gui getGui() { return widget.getGui(); }
		Gui getParentGui() { return widget.getParentGui(); }
		
		// interface
		#ifdef HAS_INTERFACE
			InterfaceWindow getInterface() { return NULL; }
		#endif
		
		// hierarchy
		void setParent(Widget w) {
			if(parent != NULL) parent.removeChild(this);
			parent = w;
		}
		Widget getParent() { return parent; }
		
		// flags
		void setFlags(int flags) { widget.setFlags(flags); }
		int getFlags() { return widget.getFlags(); }
		
		// enabled
		void setEnabled(int enable) { widget.setEnabled(enable); }
		int isEnabled() { return widget.isEnabled(); }
		
		// hidden
		void setHidden(int hidden) { widget.setHidden(hidden); }
		int isHidden() { return widget.isHidden(); }
		
		// data
		void setData(string str) { widget.setData(str); }
		string getData() { return widget.getData(); }
		
		// focus
		void setFocus() { widget.setFocus(); }
		void setNextFocus(Widget w) { widget.setNextFocus(w.widget); }
		void setPermanentFocus() {
			widget.setPermanentFocus();
			#ifdef HAS_INTERFACE
				forloop(int i = 0; engine.interface.getNumWindows()) {
					InterfaceWindow interface = engine.interface.getWindow(i);
					Gui gui = interface.getGui();
					if(gui.isChild(widget)) {
						interface.setPermanentFocus();
						break;
					}
				}
			#endif
		}
		void removeFocus() { widget.removeFocus(); }
		int isFocused() { return widget.isFocused(); }
		
		// position
		void setPosition(int x,int y) {
			#ifdef HAS_INTERFACE
				InterfaceWindow interface = getInterface();
				if(interface != NULL) {
					x += engine.app.getPositionX();
					y += engine.app.getPositionY();
					interface.setPosition(x,y);
					return;
				}
			#endif
			widget.setPosition(x,y);
		}
		int getPositionX() {
			#ifdef HAS_INTERFACE
				InterfaceWindow interface = getInterface();
				if(interface != NULL) return interface.getPositionX() - engine.app.getPositionX();
			#endif
			return widget.getPositionX();
		}
		int getPositionY() {
			#ifdef HAS_INTERFACE
				InterfaceWindow interface = getInterface();
				if(interface != NULL) return interface.getPositionY() - engine.app.getPositionY();
			#endif
			return widget.getPositionY();
		}
		
		// screen position
		int getScreenPositionX() { return widget.getScreenPositionX(); }
		int getScreenPositionY() { return widget.getScreenPositionY(); }
		
		// default size
		void setWidth(int width) { widget.setWidth(width); }
		void setHeight(int height) { widget.setHeight(height); }
		
		// current size
		int getWidth() { return widget.getWidth(); }
		int getHeight() { return widget.getHeight(); }
		
		// mouse intersection
		int getIntersection(int x,int y) { return widget.getIntersection(x,y); }
		
		// mouse coordiantes
		int getMouseX() { return widget.getMouseX(); }
		int getMouseY() { return widget.getMouseY(); }
		
		// mouse cursor
		void setMouseCursor(int cursor) { widget.setMouseCursor(cursor); }
		int getMouseCursor() { return widget.getMouseCursor(); }
		
		// font
		void setFont(string name) { widget.setFont(name); }
		void setFontSize(int size) { widget.setFontSize(size); }
		int getFontSize() { return widget.getFontSize(); }
		void setFontColor(vec4 color) { widget.setFontColor(color); }
		vec4 getFontColor() { return widget.getFontColor(); }
		void setFontOutline(int outline) { widget.setFontOutline(outline); }
		int getFontOutline() { return widget.getFontOutline(); }
		void setFontHSpacing(int spacing) { widget.setFontHSpacing(spacing); }
		int getFontHSpacing() { return widget.getFontHSpacing(); }
		void setFontVSpacing(int spacing) { widget.setFontVSpacing(spacing); }
		int getFontVSpacing() { return widget.getFontVSpacing(); }
		void setFontWrap(int wrap) { widget.setFontWrap(wrap); }
		int getFontWrap() { return widget.getFontWrap(); }
		void setFontRich(int rich) { widget.setFontRich(rich); }
		int getFontRich() { return widget.getFontRich(); }
		
		// tooltips
		void setToolTip(string str,int reset = 0) { widget.setToolTip(str,reset); }
		string getToolTip() { return widget.getToolTip(); }
		
		// callbacks
		void setCallback(int callback,string function,int v0 = 0,int v1 = 0,int v2 = 0,int v3 = 0) { widget.setCallback(callback,function,v0,v1,v2,v3); }
		void setCallbackAccel(int callback,int key,int ctrl,int alt,int shift) { widget.setCallbackAccel(callback,key,ctrl,alt,shift); }
		void setCallbackEnabled(int callback,int enable) { widget.setCallbackEnabled(callback,enable); }
		int isCallbackEnabled(int callback) { return widget.isCallbackEnabled(callback); }
		void runCallback(int callback) { widget.runCallback(callback); }
		
		// attaches
		void addAttach(Widget w,string format = 0,int multiplier = 1,int flags = 0) { widget.addAttach(w.widget,format,multiplier,flags); }
		void removeAttach(Widget w) { widget.removeAttach(w.widget); }
		
		// childs
		void addChild(Widget w,int flags = 0) {
			int index = childs.find(w);
			if(index == -1) {
				w.setParent(this);
				widget.addChild(w.widget,flags);
				childs.append(w);
			}
		}
		void removeChild(Widget w) {
			int index = childs.find(w);
			if(index != -1) {
				if(widget != NULL && w.widget != NULL) {
					widget.removeChild(w.widget);
				}
				childs.remove(index);
				w.parent = NULL;
				return;
			}
			forloop(int i = 0; childs.size()) {
				childs[i].removeChild(w);
			}
		}
		int getNumChilds() {
			return childs.size();
		}
		Widget getChild(int num) {
			return childs[num];
		}
		int isChild(Widget w) {
			return (childs.find(w) != -1);
		}
		
		// render
		void raise(Widget w) { widget.raise(w.widget); }
		void arrange() { widget.arrange(); }
	};
	
	/**************************************************************************\
	*
	* Functions
	*
	\**************************************************************************/
	
	/*
	 */
	string widgetFormat(float value) {
		if(abs(value) < 0.001f) return "0.000";
		if(abs(value) < 1000.0f) return format("%.3f",value);
		if(abs(value) < 10000.0f) return format("%.2f",value);
		if(value >= INFINITY) return "inf";
		if(value <= -INFINITY) return "-inf";
		return format("%.1f",value);
	}
	
	/*
	 */
	int getWidth() {
		return engine.app.getWidth();
	}
	
	int getHeight() {
		return engine.app.getHeight();
	}
	
	/*
	 */
	void setMouseCursor(int cursor) {
		engine.gui.setMouseCursor(cursor);
	}
	
	int getMouseCursor() {
		return engine.gui.getMouseCursor();
	}
	
	void setMouseButton(int button) {
		engine.gui.setMouseButton(button);
	}
	
	int getMouseButton() {
		return engine.gui.getMouseButton();
	}
	
	int getMouseX() {
		return engine.gui.getMouseX();
	}
	
	int getMouseY() {
		return engine.gui.getMouseY();
	}
	
	int getMouseDX() {
		return engine.gui.getMouseDX();
	}
	
	int getMouseDY() {
		return engine.gui.getMouseDY();
	}
	
	void setMouseGrab(int grab) {
		engine.gui.setMouseGrab(grab);
	}
	
	int getMouseGrab() {
		return engine.gui.getMouseGrab();
	}
	
	::Widget getFocus() {
		return engine.gui.getFocus();
	}
	
	int getActivity() {
		return engine.gui.getActivity();
	}
	
	/*
	 */
	int getKeyState(int key) {
		return engine.app.getKeyState(key);
	}
	
	int clearKeyState(int key) {
		return engine.app.clearKeyState(key);
	}
	
	/*
	 */
	void addChild(Widget widget,int flags = 0) {
		#ifdef HAS_INTERFACE
			InterfaceWindow interface = widget.getInterface();
			if(interface != NULL) {
				interface.setWidget(widget.widget);
				interface.setCenterPosition();
				interface.setHidden(0);
				return;
			}
		#endif
		engine.gui.addChild(widget.widget,flags);
	}
	
	void removeChild(Widget widget) {
		#ifdef HAS_INTERFACE
			InterfaceWindow interface = widget.getInterface();
			if(interface != NULL) {
				interface.setWidget(NULL);
				interface.setHidden(1);
				return;
			}
		#endif
		engine.gui.removeChild(widget.widget);
	}
	
	int isChild(Widget widget) {
		#ifdef HAS_INTERFACE
			InterfaceWindow interface = widget.getInterface();
			if(interface != NULL) {
				Gui gui = interface.getGui();
				if(gui.isChild(widget.widget)) return 1;
			}
		#endif
		return engine.gui.isChild(widget.widget);
	}
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_H__ */
