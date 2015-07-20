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


#ifndef __UNIGINE_WIDGET_COMBOBOX_H__
#define __UNIGINE_WIDGET_COMBOBOX_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class ComboBox : Widget {
		
		WidgetComboBox combobox;
		
		// constructor
		ComboBox() {
			combobox = new WidgetComboBox(engine.getGui());
			widget = combobox;
		}
		
		ComboBox(int c) {
			owner = 0;
			combobox = c;
			widget = c;
		}
		
		// texture
		void setImage(Image image) { combobox.setImage(image); }
		Image getImage() { return combobox.getImage(); }
		void setTexture(string name) { combobox.setTexture(name); }
		string getTexture() { return combobox.getTexture(); }
		
		// clear
		void clear() { combobox.clear(); }
		
		// items
		int addItem(string str,int texture = -1) { return combobox.addItem(str,texture); }
		void removeItem(int item) { combobox.removeItem(item); }
		int getNumItems() { return combobox.getNumItems(); }
		
		// item text
		void setItemText(int item,string str) { combobox.setItemText(item,str); }
		string getItemText(int item) { return combobox.getItemText(item); }
		
		// item data
		void setItemData(int item,string str) { combobox.setItemData(item,str); }
		string getItemData(int item) { return combobox.getItemData(item); }
		
		// item texture
		void setItemTexture(int item,int texture) { combobox.setItemTexture(item,texture); }
		int getItemTexture(int item) { return combobox.getItemTexture(item); }
		
		// current item
		void setCurrentItem(int item) { combobox.setCurrentItem(item); }
		int getCurrentItem() { return combobox.getCurrentItem(); }
		string getCurrentItemText() { return combobox.getCurrentItemText(); }
		string getCurrentItemData() { return combobox.getCurrentItemData(); }
		
		// set current item text
		void setCurrentItemText(string str) {
			combobox.setCallbackEnabled(GUI_CHANGED,0);
			forloop(int i = 0; combobox.getNumItems()) {
				if(combobox.getItemText(i) == string(str)) {
					combobox.setCurrentItem(i);
					break;
				}
			}
			combobox.setCallbackEnabled(GUI_CHANGED,1);
		}
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_COMBOBOX_H__ */
