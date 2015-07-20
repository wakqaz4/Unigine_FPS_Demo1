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


#ifndef __UNIGINE_WIDGET_TREEBOX_H__
#define __UNIGINE_WIDGET_TREEBOX_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class TreeBox : Widget {
		
		WidgetTreeBox treebox;
		
		// constructor
		TreeBox() {
			treebox = new WidgetTreeBox(engine.getGui());
			widget = treebox;
		}
		
		TreeBox(int t) {
			owner = 0;
			treebox = t;
			widget = t;
		}
		
		// editable flag
		void setEditable(int editable) { treebox.setEditable(editable); }
		int isEditable() { return treebox.isEditable(); }
		
		// multi-selection flag
		void setMultiSelection(int multi_selection) { treebox.setMultiSelection(multi_selection); }
		int isMultiSelection() { return treebox.isMultiSelection(); }
		
		// texture
		void setImage(Image image) { treebox.setImage(image); }
		Image getImage() { return treebox.getImage(); }
		void setTexture(string name) { treebox.setTexture(name); }
		string getTexture() { return treebox.getTexture(); }
		
		// clear
		void clear() { treebox.clear(); }
		
		// items
		int addItem(string str,int texture = -1) { return treebox.addItem(str,texture); }
		void getItems(int items[]) { treebox.getItems(items); }
		void removeItem(int item) { treebox.removeItem(item); }
		int getNumItems() { return treebox.getNumItems(); }
		int getItem(int num) { return treebox.getItem(num); }
		
		// item text
		void setItemText(int item,string str) { treebox.setItemText(item,str); }
		string getItemText(int item) { return treebox.getItemText(item); }
		
		// item data
		void setItemData(int item,string str) { treebox.setItemData(item,str); }
		string getItemData(int item) { return treebox.getItemData(item); }
		
		// item color
		void setItemColor(int item,vec4 color) { treebox.setItemColor(item,color); }
		vec4 getItemColor(int item) { return treebox.getItemColor(item); }
		
		// item texture
		void setItemTexture(int item,int texture) { treebox.setItemTexture(item,texture); }
		int getItemTexture(int item) { return treebox.getItemTexture(item); }
		
		// item selectable
		void setItemSelectable(int item,int selectable) { treebox.setItemSelectable(item,selectable); }
		int isItemSelectable(int item) { return treebox.isItemSelectable(item); }
		
		// item hidden
		void setItemHidden(int item,int hidden) { treebox.setItemHidden(item,hidden); }
		int isItemHidden(int item) { return treebox.isItemHidden(item); }
		
		// item folded
		void setItemFolded(int item,int folded) { treebox.setItemFolded(item,folded); }
		int isItemFolded(int item) { return treebox.isItemFolded(item); }
		
		// item parent
		void setItemParent(int item,int parent) { treebox.setItemParent(item,parent); }
		int getItemParent(int item) { return treebox.getItemParent(item); }
		
		// items hierarchy
		void addItemChild(int item,int child) { treebox.addItemChild(item,child); }
		void removeItemChild(int item,int child) { treebox.removeItemChild(item,child); }
		int getNumItemChilds(int item) { return treebox.getNumItemChilds(item); }
		int getItemChild(int item,int num) { return treebox.getItemChild(item,num); }
		int isItemChild(int item,int child) { return treebox.isItemChild(item,child); }
		
		// current item
		void setCurrentItem(int item) { treebox.setCurrentItem(item); }
		int getCurrentItem() { return treebox.getCurrentItem(); }
		string getCurrentItemText() { return treebox.getCurrentItemText(); }
		string getCurrentItemData() { return treebox.getCurrentItemData(); }
		
		// selected items
		int getNumSelectedItems() { return treebox.getNumSelectedItems(); }
		int getSelectedItem(int num) { return treebox.getSelectedItem(num); }
		void setItemSelected(int item,int selected) { treebox.setItemSelected(item,selected); }
		int isItemSelected(int item) { return treebox.isItemSelected(item); }
		void clearSelection() { treebox.clearSelection(); }
		
		// show item
		void showItem(int item) { treebox.showItem(item); }
		
		// hierarchy
		void setHierarchy(string names[],string delimiter) {
			string items[];
			string tokens[0];
			foreach(string name; names) {
				string current = "";
				strsplit(name,delimiter,tokens);
				forloop(int i = 0; tokens.size()) {
					string parent = current;
					current += delimiter + tokens[i];
					if(items.check(current) == 0) {
						int id = treebox.addItem(tokens[i]);
						if(tokens.size() > 1) items.append(current,id);
						if(i > 0) treebox.setItemParent(id,items[parent]);
						treebox.setItemFolded(id,1);
					}
				}
			}
		}
		string getHierarchy(string delimiter) {
			string item = "";
			int id = treebox.getCurrentItem();
			while(id != -1) {
				if(strlen(item)) item = treebox.getItemText(id) + delimiter + item;
				else item = treebox.getItemText(id);
				id = treebox.getItemParent(id);
			}
			return item;
		}
		
		// selection
		void selectByName(string name) {
			int item = -1;
			int length = 0;
			forloop(int i = 0; treebox.getNumItems()) {
				int id = treebox.getItem(i);
				string text = treebox.getItemText(id);
				if(text == name) {
					length = strlen(name);
					item = id;
					break;
				}
				forloop(int j = 1; strlen(name) + 1) {
					if(strncmp(text,name,j)) break;
					if(length >= j) continue;
					length = j;
					item = id;
				}
			}
			if(treebox.isMultiSelection()) {
				forloop(int i = 0; treebox.getNumItems()) {
					int id = treebox.getItem(i);
					string text = treebox.getItemText(id);
					treebox.setItemSelected(id,(length && strncmp(text,name,length) == 0));
				}
			} else {
				if(treebox.getCurrentItem() != item) {
					if(item != -1) treebox.showItem(item);
					treebox.setCurrentItem(item);
				}
			}
			return length;
		}
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_TREEBOX_H__ */
