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


#ifndef __UNIGINE_WIDGET_MENUBAR_H__
#define __UNIGINE_WIDGET_MENUBAR_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class MenuBar : Widget {
		
		WidgetMenuBar menubar;
		
		// constructor
		MenuBar(int x = 0,int y = 0) {
			if(x is WidgetMenuBar) {
				owner = 0;
				menubar = x;
			} else {
				menubar = new WidgetMenuBar(engine.getGui(),x,y);
			}
			widget = menubar;
		}
		
		// space size
		void setSpace(int x,int y) { menubar.setSpace(x,y); }
		int getSpaceX() { return menubar.getSpaceX(); }
		int getSpaceY() { return menubar.getSpaceY(); }
		
		// clear
		void clear() { menubar.clear(); }
		
		// items
		int addItem(string str,MenuBox menu = NULL) { return menubar.addItem(str,(menu != NULL) ? menu.menubox : NULL); }
		void removeItem(int item) { menubar.removeItem(item); }
		int getNumItems() { return menubar.getNumItems(); }
		
		// item text
		void setItemText(int item,string str) { menubar.setItemText(item,str); }
		string getItemText(int item) { return menubar.getItemText(item); }
		
		// item data
		void setItemData(int item,string str) { menubar.setItemData(item,str); }
		string getItemData(int item) { return menubar.getItemData(item); }
		
		// item enabled flag
		void setItemEnabled(int item,int enable) { menubar.setItemEnabled(item,enable); }
		int isItemEnabled(int item) { return menubar.isItemEnabled(item); }
		
		// item space
		void setItemSpace(int item,int space) { menubar.setItemSpace(item,space); }
		int getItemSpace(int item) { return menubar.getItemSpace(item); }
		
		// item menu
		void setItemMenu(int item,MenuBox menu) { menubar.setItemMenu(item,(menu != NULL) ? menu.menubox : NULL); }
		MenuBox getItemMenu(int item) { return new MenuBox(menubar.getItemMenu(item)); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_MENUBAR_H__ */
