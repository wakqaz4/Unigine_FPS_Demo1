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


#ifndef __UNIGINE_WIDGET_MENUBOX_H__
#define __UNIGINE_WIDGET_MENUBOX_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class MenuBox : Widget {
		
		WidgetMenuBox menubox;
		
		#ifdef HAS_INTERFACE
			int position_x;
			int position_y;
			int update_thread_id;
			InterfaceWindow interface;
			InterfaceWindow window;
		#endif
		
		// constructor
		MenuBox(int x = 0,int y = 0,int flags = 0) {
			if(x is WidgetMenuBox) {
				owner = 0;
				menubox = x;
			} else {
				menubox = new WidgetMenuBox(engine.getGui(),x,y);
			}
			#ifdef HAS_INTERFACE
				interface = new InterfaceWindow(flags);
				if(!menubox.isCallback(SHOW) && !menubox.isCallback(HIDE)) {
					menubox.setCallback(SHOW,functionid(show_callback),this);
					menubox.setCallback(HIDE,functionid(hide_callback),this);
				}
				update_thread_id = -1;
			#endif
			widget = menubox;
		}
		~MenuBox() {
			#ifdef HAS_INTERFACE
				if(menubox != NULL) {
					menubox.setCallback(SHOW,NULL);
					menubox.setCallback(HIDE,NULL);
				}
				delete interface;
			#endif
		}
		
		// interface
		#ifdef HAS_INTERFACE
			InterfaceWindow getInterface() { return interface; }
		#endif
		
		// space size
		void setSpace(int x,int y) { menubox.setSpace(x,y); }
		int getSpaceX() { return menubox.getSpaceX(); }
		int getSpaceY() { return menubox.getSpaceY(); }
		
		// texture
		void setImage(Image image) { menubox.setImage(image); }
		Image getImage() { return menubox.getImage(); }
		void setTexture(string name) { menubox.setTexture(name); }
		string getTexture() { return menubox.getTexture(); }
		
		// clear
		void clear() { menubox.clear(); }
		
		// items
		int addItem(string str,int texture = -1) { return menubox.addItem(str,texture); }
		void removeItem(int item) { menubox.removeItem(item); }
		int getNumItems() { return menubox.getNumItems(); }
		
		// item text
		void setItemText(int item,string str) { menubox.setItemText(item,str); }
		string getItemText(int item) { return menubox.getItemText(item); }
		
		// item data
		void setItemData(int item,string str) { menubox.setItemData(item,str); }
		string getItemData(int item) { return menubox.getItemData(item); }
		
		// item tooltip
		void setItemToolTip(int item,string str) { menubox.setItemToolTip(item,str); }
		string getItemToolTip(int item) { return menubox.getItemToolTip(item); }
		
		// item enabled flag
		void setItemEnabled(int item,int enable) { menubox.setItemEnabled(item,enable); }
		int isItemEnabled(int item) { return menubox.isItemEnabled(item); }
		
		// item texture
		void setItemTexture(int item,int texture) { menubox.setItemTexture(item,texture); }
		int getItemTexture(int item) { return menubox.getItemTexture(item); }
		
		// item separator
		void setItemSeparator(int item,int separator) { menubox.setItemSeparator(item,separator); }
		int isItemSeparator(int item) { return menubox.isItemSeparator(item); }
		
		// item selectable
		void setItemSelectable(int item,int selectable) { menubox.setItemSelectable(item,selectable); }
		int isItemSelectable(int item) { return menubox.isItemSelectable(item); }
		
		// item space
		void setItemSpace(int item,int space) { menubox.setItemSpace(item,space); }
		int getItemSpace(int item) { return menubox.getItemSpace(item); }
		
		// item widget
		void setItemWidget(int item,Widget widget) { menubox.setItemWidget(item,(widget != NULL) ? widget.widget : NULL); }
		Widget getItemWidget(int item) {
			Widget widget = menubox.getItemWidget(item);
			return (widget != NULL) ? new Widget(widget) : NULL;
		}
		
		// current item
		int getCurrentItem() { return menubox.getCurrentItem(); }
		string getCurrentItemText() { return menubox.getCurrentItemText(); }
		string getCurrentItemData() { return menubox.getCurrentItemData(); }
		
		// callbacks
		#ifdef HAS_INTERFACE
			
			// init position
			void init_position() {
				
				// widget position
				position_x = widget.getPositionX();
				position_y = widget.getPositionY();
				::Widget parent = widget.getParent();
				if(parent != NULL) {
					position_x += parent.getScreenPositionX();
					position_y += parent.getScreenPositionY();
				}
				
				// parent window
				int num = engine.interface.findWindow(getParentGui());
				if(num != -1) window = engine.interface.getWindow(num);
				else window = NULL;
			}
			
			// update position
			void update_position() {
				
				// window position
				int x = position_x;
				int y = position_y;
				if(window != NULL) {
					x += window.getPositionX();
					y += window.getPositionY();
				} else {
					x += engine.interface.getPositionX();
					y += engine.interface.getPositionY();
				}
				
				// visible position
				x = engine.interface.getVisiblePositionX(x,widget.getWidth(),widget.getWidth());
				y = engine.interface.getVisiblePositionY(y,widget.getHeight(),widget.getHeight());
				
				// update position
				interface.setPosition(x,y);
			}
			
			// update thread
			void update_thread(MenuBox menubox) {
				while(menubox.update_thread_id != -1) {
					menubox.update_position();
					wait;
				}
			}
			
			// show callback
			void show_callback(MenuBox menubox) {
				
				if(menubox == NULL) return;
				
				InterfaceWindow interface = menubox.getInterface();
				if(interface == NULL) return;
				
				menubox.init_position();
				
				::Widget focus = getFocus();
				int mouse_grab = getMouseGrab();
				
				menubox.setCallbackEnabled(HIDE,0);
				menubox.setCallbackEnabled(SHOW,0);
				interface.setWidget(menubox.widget);
				menubox.update_position();
				interface.setHidden(0);
				menubox.setCallbackEnabled(SHOW,1);
				menubox.setCallbackEnabled(HIDE,1);
				
				setMouseGrab(mouse_grab);
				if(focus != NULL) focus.setFocus();
				
				menubox.update_thread_id = thread(functionid(update_thread),menubox);
			}
			
			// hide callback
			void hide_callback(MenuBox menubox) {
				
				if(menubox == NULL) return;
				
				InterfaceWindow interface = menubox.getInterface();
				if(interface == NULL) return;
				
				menubox.setCallbackEnabled(HIDE,0);
				interface.setWidget(NULL);
				interface.setHidden(1);
				menubox.setCallbackEnabled(HIDE,1);
				
				menubox.update_thread_id = -1;
			}
			
		#endif
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_MENUBOX_H__ */
