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


#ifndef __UNIGINE_WIDGET_DIALOG_PROPERTY_H__
#define __UNIGINE_WIDGET_DIALOG_PROPERTY_H__

#include <core/systems/widgets/widget_vbox.h>
#include <core/systems/widgets/widget_label.h>
#include <core/systems/widgets/widget_gridbox.h>
#include <core/systems/widgets/widget_combobox.h>
#include <core/systems/widgets/widget_editline.h>
#include <core/systems/widgets/widget_scrollbox.h>
#include <core/systems/widgets/widget_treebox.h>
#include <core/systems/widgets/widget_dialog.h>

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	enum {
		
		DIALOG_PROPERTY_WIDTH = 480,	// dialog width
		DIALOG_PROPERTY_HEIGHT = 320,	// dialog height
	};
	
	/*
	 */
	class DialogProperty : Dialog {
		
		Button ok_b;				// ok button
		ComboBox libraries_cb;		// libraries combobox
		EditLine name_el;			// name editline
		TreeBox properties_tb;		// properties treebox
		
		// constructor
		DialogProperty(string str = 0) : Dialog(str) {
			
			// ok button
			ok_b = getOkButton();
			
			// grid box
			VBox vbox = new VBox(0,4);
			GridBox gridbox = new GridBox(2,4,4);
			vbox.addChild(gridbox,ALIGN_EXPAND);
			addChild(vbox);
			
			// libraries
			libraries_cb = new ComboBox();
			libraries_cb.setCallback(CHANGED,functionid(callback_redirector),this,functionid(libraries_changed));
			gridbox.addChild(new Label(TR("Library:")),ALIGN_RIGHT);
			gridbox.addChild(libraries_cb,ALIGN_EXPAND);
			
			// property name
			name_el = new EditLine();
			name_el.setCallback(CHANGED,functionid(callback_redirector),this,functionid(name_changed));
			name_el.setCallback(PRESSED,functionid(callback_redirector),this,functionid(ok_clicked));
			gridbox.addChild(new Label(TR("Name:")),ALIGN_RIGHT);
			gridbox.addChild(name_el,ALIGN_EXPAND);
			
			// scrollbox
			ScrollBox scrollbox = new ScrollBox();
			scrollbox.setWidth(DIALOG_PROPERTY_WIDTH);
			scrollbox.setHeight(DIALOG_PROPERTY_HEIGHT);
			addChild(scrollbox,ALIGN_EXPAND);
			
			// properties treebox
			properties_tb = new TreeBox();
			properties_tb.setCallback(CHANGED,functionid(callback_redirector),this,functionid(properties_changed));
			properties_tb.setCallback(DOUBLE_CLICKED,functionid(callback_redirector),this,functionid(ok_clicked));
			scrollbox.addChild(properties_tb,ALIGN_EXPAND);
		}
		
		/*
		 */
		void setProperty(string name) {
			
			// update libraries
			libraries_cb.setCallbackEnabled(CHANGED,0);
			update_libraries();
			libraries_cb.setCallbackEnabled(CHANGED,1);
			
			// update properties
			properties_tb.setCallbackEnabled(CHANGED,0);
			update_properties();
			properties_tb.setCallbackEnabled(CHANGED,1);
			
			name_el.setText(name);
		}
		
		string getProperty() {
			return properties_tb.getCurrentItemText();
		}
		
		/*
		 */
		void update_libraries() {
			
			string old_library = libraries_cb.getCurrentItemText();
			
			libraries_cb.clear();
			libraries_cb.addItem(TR("All libraries"));
			forloop(int i = 0; engine.properties.getNumLibraries()) {
				string library = replace(basename(engine.properties.getLibraryName(i)),".prop","");
				if(old_library == library) libraries_cb.setCurrentItem(i + 1);
				libraries_cb.addItem(library);
			}
		}
		
		void update_properties() {
			
			string properties[0];
			if(libraries_cb.getCurrentItem() == 0) {
				forloop(int i = 0; engine.properties.getNumLibraries()) {
					forloop(int j = 0; engine.properties.getNumProperties(i)) {
						string name = engine.properties.getPropertyName(i,j);
						Property property = engine.properties.findProperty(name);
						if(property.isHidden() == 0) properties.append(name);
					}
				}
			} else {
				int library = libraries_cb.getCurrentItem() - 1;
				forloop(int i = 0; engine.properties.getNumProperties(library)) {
					string name = engine.properties.getPropertyName(library,i);
					Property property = engine.properties.findProperty(name);
					if(property.isHidden() == 0) properties.append(name);
				}
			}
			properties.sort();
			
			string items[];
			properties_tb.setCallbackEnabled(CHANGED,0);
			properties_tb.clear();
			foreach(string name; properties) {
				items.append(name,properties_tb.addItem(name));
			}
			properties_tb.setCallbackEnabled(CHANGED,1);
			
			foreach(string name; properties) {
				Property property = engine.properties.findProperty(name);
				Property parent = property.getParent();
				if(parent == NULL || properties.find(parent.getName()) == -1) continue;
				properties_tb.setItemParent(items[name],items[parent.getName()]);
			}
		}
		
		/*
		 */
		void libraries_changed() {
			properties_tb.setCallbackEnabled(CHANGED,0);
			update_properties();
			properties_tb.setCallbackEnabled(CHANGED,1);
			name_el.runCallback(CHANGED);
		}
		
		void name_changed() {
			name_el.setCallbackEnabled(CHANGED,0);
			properties_tb.setCallbackEnabled(CHANGED,0);
			int length = properties_tb.selectByName(name_el.getText());
			name_el.setText(substr(name_el.getText(),0,length));
			name_el.setCallbackEnabled(CHANGED,1);
			properties_tb.setCallbackEnabled(CHANGED,1);
			ok_b.setEnabled((properties_tb.getCurrentItem() != -1));
		}
		
		void properties_changed() {
			name_el.setCallbackEnabled(CHANGED,0);
			name_el.setText(properties_tb.getCurrentItemText());
			name_el.setCallbackEnabled(CHANGED,1);
			ok_b.setEnabled((properties_tb.getCurrentItem() != -1));
		}
		
		/*
		 */
		void ok_clicked() {
			ok_b.runCallback(CLICKED);
		}
		
		/*
		 */
		void callback_redirector(DialogProperty dialog,string name) {
			dialog.call(name);
		}
	};
	
	/*
	 */
	int dialogProperty(string str,string &name) {
		
		DialogProperty dialog;
		
		// create dialog
		if(dialog == NULL) {
			dialog = new DialogProperty(str);
			dialog.setSizeable(1);
		}
		
		// dialog property
		dialog.setText(str);
		dialog.setProperty(name);
		
		// show dialog
		dialog.arrange();
		addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
		dialog.setPermanentFocus();
		
		// wait dialog
		while(dialog.isDone() == 0) wait;
		
		// dialog result
		if(dialog.isOkClicked()) name = dialog.getProperty();
		
		// hide dialog
		removeChild(dialog);
		
		return dialog.isOkClicked();
	}
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_DIALOG_PROPERTY_H__ */
