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

#ifndef __EDITOR_WIDGET_MENU_H__
#define __EDITOR_WIDGET_MENU_H__

/*
 */
namespace Editor::Widgets {
	
	/**************************************************************************\
	*
	* class WidgetEditorMenu
	*
	\**************************************************************************/
	
	/*
	 */
	class WidgetEditorMenu : WidgetEditorPanel {
		
		using Unigine::Widgets;
		
		WidgetEditorMenuItem items[0];
		
		WidgetEditorMenu(Widget target_,Widget menu, int anchor_ = ANCHOR_RIGHT) {
			__WidgetEditorPanel__(target_,anchor_);
			addChild(menu);
			
			void add_item(::Widget widget) {
				if(widget.getData() == "item" && widget.getType() == WIDGET_HBOX) {
					createItem(class_cast("WidgetHBox",widget));
				}
				else if(widget.getData() == "items" && widget.getType() == WIDGET_VBOX) {
					create_items(class_cast("WidgetVBox",widget));
				}
				else if(widget.getData() != "menu") {
					forloop(int i = 0; widget.getNumChilds()) {
						add_item(widget.getChild(i));
					}
				}
			}
			
			forloop(int i = 0; menu.widget.getNumChilds()) {
				add_item(menu.widget.getChild(i));
			}
		}
		
		//
		int addItem(WidgetEditorMenuItem item) {
			items.append(item);
			return getNumItems() - 1;
		}
		
		//
		WidgetEditorMenuItem getItem(int num) {
			return items[num];
		}
		
		//
		int getNumItems() {
			return items.size();
		}
		
		//
		WidgetEditorMenuItem createItem(WidgetHBox hbox) {
			WidgetEditorMenuItem item = new WidgetEditorMenuItem(hbox,this);
			addItem(item);
			
			return item;
		}
		
		//
		WidgetEditorMenuItem create_items(WidgetVBox widget) {
			
			forloop(int i = 0; widget.getNumChilds()) {
				::Widget child = widget.getChild(0);
				
				if(child.getData() == "item" && child.getType() == WIDGET_HBOX) {
					// set child widget to last of child's list
					widget.removeChild(child);
					widget.addChild(child,child.getFlags());
					createItem(class_cast("WidgetHBox",child));
					continue;
				}
				
				
				WidgetHBox container = new WidgetHBox(widget.getGui());
				
				container.setSpace(widget.getSpaceX(),widget.getSpaceY());
				
				container.addChild(child,child.getFlags());
				widget.addChild(container,widget.getFlags());
				
				addItem(new WidgetEditorMenuItem(container,this));
			}
			widget.setSpace(0,0);
		}
		
		//
		void update() {
			if(isHidden()) return;
			
			items.call("update");
			
			forloop(int i = 0; items.size()) {
				if(items[i].isHiddenMenu() == 0) {
					update_position();
					return;
				}
			}
			
			super.update();
		}
		
		//
		void setHidden(int hidden) {
			super.setHidden(hidden);
			if(hidden) items.call("hideMenu");
		}
		
		//
		void updateItemsHotkeys() {
			forloop(int i = 0;items.size()) {
				items[i].updateHotkey();
			}
		}
	};
	
	/**************************************************************************\
	*
	* class WidgetEditorMenuItem
	*
	\**************************************************************************/
	
	/*
	 */
	class WidgetEditorMenuItem : Unigine::Widgets::HBox {
		
		using Unigine::Widgets;
		
		WidgetHBox container;
		WidgetLabel hotkey_l;
		
		WidgetEditorMenu menu;
		WidgetEditorMenu main_menu;
		
		int is_checkbox = 0;
		WidgetCheckBox checkbox;
		
		WidgetEditorMenuItem(WidgetHBox container_,WidgetEditorMenu main_menu_ = NULL,WidgetEditorMenu menu_ = NULL) {
			__HBox__();
			
			container = container_;
			main_menu = main_menu_;
			menu = menu_;
			
			while(container.getNumChilds()) {
				::Widget child = container.getChild(0);
				widget.addChild(child,child.getFlags());
				
				if(child.getData() == "menu" && child.getType() == WIDGET_VBOX) {
					menu = new WidgetEditorMenu(new Widget(container),new Widget(child));
				} else if(child.getType() == WIDGET_CHECKBOX && child.getFlags() == GUI_ALIGN_LEFT) {
					is_checkbox = 1;
					checkbox = class_cast("WidgetCheckBox",child);
				}
			}
			
			add_hotkey_label(widget);
			
			container.addChild(widget,GUI_ALIGN_EXPAND);
			
			container.setBackground(1);
			setBackground(0);
		}
		
		int add_hotkey_label(::Widget widget) {
			
			if(widget.getData() == "menu") return;
			
			forloop(int i = 0; widget.getNumChilds()) {
				add_hotkey_label(widget.getChild(i));
				if(hotkey_l != NULL) return;
			}
			
			if(widget.getType() != WIDGET_LABEL) return;
			if(hotkeysCheckName(widget.getData())== 0) return;
			
			WidgetLabel label = class_cast("WidgetLabel",widget);
			
			if(label.getText() != "hotkey") return;
			
			label.setText("");
			hotkey_l = label;
		}
		
		//
		void update() {
			if(isMouseOver()) {
				set_selection(1);
			} else if(menu == NULL || main_menu == NULL || main_menu.isMouseOver() && menu.isMouseOver() == 0) {
				set_selection(0);
			}
		}
		
		//
		void set_selection(int selection) {
			if(selection) {
				
				container.setColor(vec4(0.5f,0.5f,0.5f,1.0f));
				setColor(vec4_one * 2.0f);
				if(is_checkbox) {
					int old_mouse_state;
					int mouse_state = engine.app.getMouseButtonState(APP_BUTTON_LEFT | APP_BUTTON_DCLICK);
					if(mouse_state && old_mouse_state != mouse_state && !checkboxIsMouseOver()) {
						checkbox_toggled();
					}
					old_mouse_state = mouse_state;
				}
			} else {
				container.setColor(vec4_one);
				setColor(vec4_one);
			}
			
			if(menu != NULL) menu.setHidden(!selection);
		}
		
		//
		int isMouseOver() {
			int x = container.getMouseX();
			int y = container.getMouseY();
			return x > 0 && x < container.getWidth() && y > 0 && y < container.getHeight();
		}
		
		int checkboxIsMouseOver() {
			int x = checkbox.getMouseX();
			int y = checkbox.getMouseY();
			return x >= 0 && x <= checkbox.getWidth() && y >= 0 && y <= checkbox.getHeight();
		}
		
		//
		void isHiddenMenu() {
			if(menu == NULL) return 1;
			return menu.isHidden();
		}
		
		//
		void hideMenu() {
			if(menu == NULL) return;
			menu.setHidden(1);
		}
		
		//
		void updateHotkey() {
			if(hotkey_l != NULL) {
				string text = hotkeysGetText(hotkey_l.getData());
				if(text != "no hotkey") hotkey_l.setText(" " + text);
				else hotkey_l.setText("");
			}
			
			if(menu != NULL) menu.updateItemsHotkeys();
		}
		
		//
		WidgetEditorMenu getMenu() {
			return menu;
		}
		
		//
		WidgetEditorMenu getMainMenu() {
			return main_menu;
		}
		
		//
		void checkbox_toggled() {
			checkbox.setChecked(!checkbox.isChecked());
		}
	};
	
} /* namespace Editor::Widgets */

#endif /* __EDITOR_WIDGET_MENU_H__ */
