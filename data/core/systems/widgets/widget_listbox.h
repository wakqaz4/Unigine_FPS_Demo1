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


#ifndef __UNIGINE_WIDGET_LISTBOX_H__
#define __UNIGINE_WIDGET_LISTBOX_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class ListBox : Widget {
		
		WidgetListBox listbox;
		
		// constructor
		ListBox() {
			listbox = new WidgetListBox(engine.getGui());
			widget = listbox;
		}
		
		ListBox(int l) {
			owner = 0;
			listbox = l;
			widget = l;
		}
		
		// multi-selection flag
		void setMultiSelection(int multi_selection) { listbox.setMultiSelection(multi_selection); }
		int isMultiSelection() { return listbox.isMultiSelection(); }
		
		// texture
		void setImage(Image image) { listbox.setImage(image); }
		Image getImage() { return listbox.getImage(); }
		void setTexture(string name) { listbox.setTexture(name); }
		string getTexture() { return listbox.getTexture(); }
		
		// clear
		void clear() { listbox.clear(); }
		
		// items
		int addItem(string str,int texture = -1) { return listbox.addItem(str,texture); }
		void removeItem(int item) { listbox.removeItem(item); }
		int getNumItems() { return listbox.getNumItems(); }
		
		// item text
		void setItemText(int item,string str) { listbox.setItemText(item,str); }
		string getItemText(int item) { return listbox.getItemText(item); }
		
		// item data
		void setItemData(int item,string str) { listbox.setItemData(item,str); }
		string getItemData(int item) { return listbox.getItemData(item); }
		
		// item color
		void setItemColor(int item,vec4 color) { listbox.setItemColor(item,color); }
		vec4 getItemColor(int item) { return listbox.getItemColor(item); }
		
		// item texture
		void setItemTexture(int item,int texture) { listbox.setItemTexture(item,texture); }
		int getItemTexture(int item) { return listbox.getItemTexture(item); }
		
		// item selectable
		void setItemSelectable(int item,int selectable) { listbox.setItemSelectable(item,selectable); }
		int isItemSelectable(int item) { return listbox.isItemSelectable(item); }
		
		// current item
		void setCurrentItem(int item) { listbox.setCurrentItem(item); }
		int getCurrentItem() { return listbox.getCurrentItem(); }
		string getCurrentItemText() { return listbox.getCurrentItemText(); }
		string getCurrentItemData() { return listbox.getCurrentItemData(); }
		
		// selected items
		int getNumSelectedItems() { return listbox.getNumSelectedItems(); }
		int getSelectedItem(int num) { return listbox.getSelectedItem(num); }
		void setItemSelected(int item,int selected) { listbox.setItemSelected(item,selected); }
		int isItemSelected(int item) { return listbox.isItemSelected(item); }
		void clearSelection() { listbox.clearSelection(); }
		
		// show item
		void showItem(int item) { listbox.showItem(item); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_LISTBOX_H__ */
