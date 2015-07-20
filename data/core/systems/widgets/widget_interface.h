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


#ifndef __UNIGINE_WIDGET_INTERFACE_H__
#define __UNIGINE_WIDGET_INTERFACE_H__

#include <core/systems/widgets/widget.h>
#include <core/systems/widgets/widget_vbox.h>
#include <core/systems/widgets/widget_hbox.h>
#include <core/systems/widgets/widget_vpaned.h>
#include <core/systems/widgets/widget_hpaned.h>
#include <core/systems/widgets/widget_gridbox.h>
#include <core/systems/widgets/widget_groupbox.h>
#include <core/systems/widgets/widget_tabbox.h>
#include <core/systems/widgets/widget_scrollbox.h>
#include <core/systems/widgets/widget_window.h>
#include <core/systems/widgets/widget_dialog.h>
#include <core/systems/widgets/widget_label.h>
#include <core/systems/widgets/widget_sprite.h>
#include <core/systems/widgets/widget_canvas.h>
#include <core/systems/widgets/widget_icon.h>
#include <core/systems/widgets/widget_button.h>
#include <core/systems/widgets/widget_checkbox.h>
#include <core/systems/widgets/widget_combobox.h>
#include <core/systems/widgets/widget_menubox.h>
#include <core/systems/widgets/widget_menubar.h>
#include <core/systems/widgets/widget_listbox.h>
#include <core/systems/widgets/widget_treebox.h>
#include <core/systems/widgets/widget_spacer.h>
#include <core/systems/widgets/widget_slider.h>
#include <core/systems/widgets/widget_spinbox.h>
#include <core/systems/widgets/widget_editline.h>
#include <core/systems/widgets/widget_edittext.h>

/*
 */
namespace Unigine::Widgets {
	
	/**************************************************************************\
	*
	* UserInterface
	*
	\**************************************************************************/
	
	/*
	 */
	class UserInterface {
		
		string prefix;
		
		::UserInterface ui;
		
		Widget widgets[0];
		
		// constructor
		UserInterface(string name,string p = 0) {
			
			prefix = p;
			
			ui = new ::UserInterface(engine.getGui(),name,prefix);
			
			forloop(int i = 0; ui.getNumWidgets()) {
				::Widget extern_widget = widget_cast(ui.getWidget(i));
				Widget widget = widgetCast(extern_widget);
				widgets.append(widget);
				if(ui.getWidgetExport(i)) {
					string name = ui.getWidgetName(i);
					if(is_variable(name)) set_variable(name,widget);
				}
			}
		}
		
		~UserInterface() {
			widgets.delete();
			delete ui;
		}
		
		// update widgets
		void updateWidgets() {
			ui.updateWidgets();
		}
		
		// widgets
		int getNumWidgets() {
			return widgets.size();
		}
		int findWidget(string name) {
			return ui.findWidget(name);
		}
		Widget getWidget(int num) {
			return widgets[num];
		}
		string getWidgetName(int num) {
			return ui.getWidgetName(num);
		}
		int getWidgetExport(int num) {
			return ui.getWidgetExport(num);
		}
		
		// callbacks
		int getNumCallbacks(int num) {
			return ui.getNumCallbacks(num);
		}
		int getCallback(int num,int callback) {
			return ui.getCallback(num,callback);
		}
		string getCallbackName(int num,int callback) {
			return ui.getCallbackName(num,callback);
		}
		string getCallbackStringData(int num,int callback) {
			return ui.getCallbackStringData(num,callback);
		}
		string getCallbackVariableData(int num,int callback) {
			return ui.getCallbackVariableData(num,callback);
		}
		string getCallbackInstanceData(int num,int callback) {
			return ui.getCallbackInstanceData(num,callback);
		}
	};
	
	/*
	 */
	Widget widgetCast(::Widget widget) {
		widget = widget_cast(widget);
		switch(widget.getType()) {
			case VBOX:		return new VBox(widget);
			case HBOX:		return new HBox(widget);
			case VPANED:	return new VPaned(widget);
			case HPANED:	return new HPaned(widget);
			case GRIDBOX:	return new GridBox(widget);
			case GROUPBOX:	return new GroupBox(widget);
			case TABBOX:	return new TabBox(widget);
			case SCROLLBOX:	return new ScrollBox(widget);
			case WINDOW:	return new Window(widget);
			case DIALOG:	return new Dialog(widget);
			case LABEL:		return new Label(widget);
			case SPRITE:	return new Sprite(widget);
			case CANVAS:	return new Canvas(widget);
			case ICON:		return new Icon(widget);
			case BUTTON:	return new Button(widget);
			case CHECKBOX:	return new CheckBox(widget);
			case COMBOBOX:	return new ComboBox(widget);
			case MENUBAR:	return new MenuBar(widget);
			case MENUBOX:	return new MenuBox(widget);
			case LISTBOX:	return new ListBox(widget);
			case TREEBOX:	return new TreeBox(widget);
			case SPACER:	return new Spacer(widget);
			case SLIDER:	return new Slider(widget);
			case SPINBOX:	return new SpinBox(widget);
			case EDITLINE:	return new EditLine(widget);
			case EDITTEXT:	return new EditText(widget);
			default:		log.error("Unigine::Widgets::widgetCast(): unknown widget type %s\n",widget.getTypeName()); return NULL;
		}
	}
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_INTERFACE_H__ */
