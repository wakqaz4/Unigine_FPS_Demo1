/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_widget_manipulator.h
 * Desc:    Unigine object manipulation widgets
 * Version: 1.05
 * Author:  Mikhail Lyubimov <alterego@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

#ifndef __EDITOR_WIDGET_PANEL_H__
#define __EDITOR_WIDGET_PANEL_H__

/*
 */
namespace Editor::Widgets {
	
	enum {
		ANCHOR_LEFT = 0,
		ANCHOR_RIGHT,
		ANCHOR_TOP,
		ANCHOR_BOTTOM,
	};
	
	/**************************************************************************\
	*
	* class WidgetEditorPanel
	*
	\**************************************************************************/
	
	/*
	 */
	class WidgetEditorPanel : Unigine::Widgets::GroupBox {
		
		using Unigine::Widgets;
		
		#ifdef HAS_INTERFACE
			InterfaceWindow interface;
			InterfaceWindow interface_target;
			
			int offset_x;
			int offset_y;
		#endif
		
		Widget target;
		int anchor;
		int update_thread_id; 
		
		int func_call_hide = NULL;
		int args_call_hide[0];
		
		int mouse_state = 0;
		
		WidgetEditorPanel(Widget target_, int anchor_ = ANCHOR_BOTTOM) {
			__GroupBox__();
			target = target_;
			anchor = anchor_;
			setBackground(1);
			
			#ifdef HAS_INTERFACE
				int window = engine.interface.findWindow(target.widget.getParentGui());
				if(window != -1) interface_target = engine.interface.getWindow(window);
				
				interface = new InterfaceWindow(0);
				Gui gui = interface.getGui();
				gui.addChild(groupbox,GUI_ALIGN_EXPAND | GUI_ALIGN_OVERLAP);
				groupbox.setHidden(0);
				
			#else
				Unigine::Widgets::addChild(this,GUI_ALIGN_OVERLAP);
			#endif
			
			setHidden(1);
			
			update_thread_id = thread(functionid(thread_redirector),this,functionid(update));
			
			widget.setOrder(120);
		}
		
		~WidgetEditorPanel() {
			kill_thread(update_thread_id);
			#ifdef HAS_INTERFACE
				delete interface;
			#endif
		}
		
		//
		void setHidden(int hidden) {
			if(isHidden() == hidden) return;
			
			#ifdef HAS_INTERFACE
				interface.setHidden(hidden);
				if(interface_target == NULL) {
					offset_x = abs(Unigine::Widgets::getMouseX() + engine.app.getPositionX() - engine.interface.getMouseX());
					offset_y = abs(Unigine::Widgets::getMouseY() + engine.app.getPositionY() - engine.interface.getMouseY());
				}
			#endif
			
			groupbox.setHidden(hidden);
			
			if(hidden) {
				if(func_call_hide != NULL) call(func_call_hide,args_call_hide);
			}
			
			arrange();
			
			#ifndef HAS_INTERFACE
				if(!hidden) setFocus();
			#endif
		}
		
		//
		void update() {
			
			if(isHidden()) return;
			
			int mouse_click = engine.app.getMouseButtonState(APP_BUTTON_LEFT);
			if(mouse_click != mouse_state) {
				mouse_state = mouse_click;
				if(mouse_state && isMouseOver() == 0) setHidden(1);
			}
			
			update_position();
		}
		
		//
		void update_position() {
			int t_x = target.getScreenPositionX();
			int t_y = target.getScreenPositionY();
			
			#ifdef HAS_INTERFACE
				interface.setWidth(widget.getWidth());
				interface.setHeight(widget.getHeight());
				
				if(interface_target != NULL) {
					t_x += interface_target.getPositionX();
					t_y += interface_target.getPositionY();
				}
				else {
					t_x += engine.app.getPositionX();
					t_y += engine.app.getPositionY();
				}
				t_x += offset_x;
				t_y += offset_y;
			#endif
			
			int t_height = target.getHeight();
			int t_width = target.getWidth();
			
			int x = t_x;
			int y = t_y;
			
			#ifndef HAS_INTERFACE
				int screen_width = Unigine::Widgets::getWidth();
				int screen_height = Unigine::Widgets::getHeight();
			#endif
			
			if(anchor == ANCHOR_BOTTOM) {
				y += t_height;
				#ifndef HAS_INTERFACE
					if(y + getHeight() > screen_height) y = t_y - getHeight();
					if(x + getWidth() > screen_width) x = screen_width - getWidth();
				#endif
			}
			else if(anchor == ANCHOR_TOP) {
				y -= getHeight();
				#ifndef HAS_INTERFACE
					if(y < 0) y = t_y + t_height;
					if(x + getWidth() > screen_width) x = screen_width - getWidth();
				#endif
			}
			else if(anchor == ANCHOR_RIGHT) {
				x += t_width;
				#ifndef HAS_INTERFACE
					if(x + getWidth() > screen_width) x = t_x - getWidth();
					if(y + getHeight() > screen_height) y = screen_height - getHeight();
				#endif
			}
			else if(anchor == ANCHOR_LEFT) {
				x -= getWidth();
				#ifndef HAS_INTERFACE
					if(x < 0) x = t_x + t_width;
					if(y + getHeight() > screen_height) y = screen_height - getHeight();
				#endif
			}
			
			setPosition(x,y);
		}
		
		//
		void thread_redirector(int instance,string method) {
			while(1) {
				instance.call(method);
				wait;
			}
		}
		
		//
		void setCallbackHide(int function,int args[] = ()) {
			func_call_hide = function;
			args_call_hide = args;
		}
		
		//
		int isMouseOver() {
			int x = getMouseX ();
			int y = getMouseY ();
			return x >= 0 && x <= getWidth() && y >= 0 && y <= getHeight();
		}
		
		#ifdef HAS_INTERFACE
			//
			void setPosition(int x,int y) {
				interface.setPosition(x,y);
			}
			
			//
			int isHidden() {
				return interface.isHidden();
			}
			
			//
			InterfaceWindow getInterface() {
				return interface;
			}
		#endif
	};
	
} /* namespace Editor::Widgets */

#endif /* __EDITOR_WIDGET_PANEL_H__ */
