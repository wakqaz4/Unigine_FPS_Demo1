/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_properties.h
 * Desc:    Unigine editor
 * Version: 1.09
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Maxim Belobryukhov <maxi@unigine.com>
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

/*
 */
namespace Properties {
	
	/*
	 */
	DECLARE_WINDOW
	
	WidgetHPaned window_hp;					// properties hpaned
	
	WidgetIcon libraries_i;					// properties libraries
	WidgetIcon save_i;						// save properties
	WidgetIcon assign_i;					// assign property
	WidgetIcon clone_i;						// clone property
	WidgetIcon inherit_i;					// inherit property
	WidgetIcon move_i;						// move property
	WidgetIcon remove_i;					// remove property
	
	WidgetComboBox libraries_cb;			// libraries combobox
	WidgetEditLine name_el;					// current property name
	WidgetTreeBox properties_tb;			// properties treebox
	int properties_folded[];				// properties hidden flag
	
	Property property;						// current property
	Property parent;						// parent property
	
	WidgetTabBox property_tb;				// property container
	
	WidgetVBox common_vb;					// property common container
	
	WidgetLabel info_library_l;				// library name
	WidgetLabel info_name_l;				// property name
	WidgetLabel info_parent_l;				// parent name
	WidgetLabel info_childs_l;				// number of children
	
	WidgetCheckBox intersection_cb;			// property options
	WidgetCheckBox collision_cb;
	
	WidgetButton copy_b;					// copy settings
	WidgetButton paste_b;					// paste settings
	
	WidgetVBox states_vb;					// property states container
	WidgetVBox parameters_vb;				// property parameters container
	
	Blob blob;
	
	#include <editor/editor_properties_unredo.h>
	//#include <editor/editor_properties_buffer.h>
	#include <editor/editor_properties_parameters.h>
	#include <editor/editor_properties_states.h>
	#include <editor/editor_properties_editor.h>
	
	/**************************************************************************\
	*
	* Property
	*
	\**************************************************************************/
	
	/*
	 */
	void update_view() {
		
		// disable callbacks
		setCallbackEnabled(GUI_CLICKED,0,(
			intersection_cb,
			collision_cb,
		));
		
		// delete resources
		states.delete();
		parameters.delete();
		
		// clear property
		property = NULL;
		if(properties_tb.getCurrentItem() != -1) {
			property = engine.properties.findProperty(properties_tb.getCurrentItemText());
			if(property == NULL) throw("Properties::update_view(): can't find \"%s\" property\n",properties_tb.getCurrentItemText());
		}
		
		// parent property
		parent = (property != NULL) ? property.getParent() : NULL;
		
		// clear view
		if(property == NULL) {
			
			setEnabled(0,(
				common_vb,
				states_vb,
				parameters_vb,
			));
			
			// info
			info_library_l.setText("");
			info_name_l.setText("");
			info_parent_l.setText("");
			info_childs_l.setText("");
			
			// options
			WidgetCheckBox options[] = (
				intersection_cb,
				collision_cb,
			);
			setChecked(0,options);
			setFontColor(vec4_zero,options);
			
			// buttons
			setEnabled(0,(
				assign_i,
				clone_i,
				inherit_i,
				move_i,
				remove_i,
				copy_b,
				paste_b,
			));
		}
		// fill view
		else {
			
			int num_childs = property.getNumChilds();
			int editable = property.isEditable();
			int library = engine.properties.findPropertyLibrary(property.getName());
			int library_editable = engine.properties.isLibraryEditable(library);
			int enabled = (editable && library_editable);
			
			setEnabled(enabled,(
				common_vb,
				states_vb,
				parameters_vb,
			));
			
			// info
			info_library_l.setText(replace(basename(engine.properties.getLibraryName(library)),".prop",""));
			info_name_l.setText(property.getName());
			info_parent_l.setText((parent != NULL) ? parent.getName() : "");
			info_childs_l.setText(string(num_childs));
			
			// options
			intersection_cb.setChecked(property.getIntersection());
			collision_cb.setChecked(property.getCollision());
			
			// update
			update_colors();
			update_states();
			update_parameters();
			
			// buttons
			assign_i.setEnabled(1);
			clone_i.setEnabled(editable);
			inherit_i.setEnabled(1);
			move_i.setEnabled(parent != NULL && editable && library_editable);
			remove_i.setEnabled(parent != NULL && editable && library_editable);
			copy_b.setEnabled(1);
			paste_b.setEnabled(blob != NULL && editable);
		}
		
		// enable callbacks
		setCallbackEnabled(GUI_CLICKED,1,(
			intersection_cb,
			collision_cb,
		));
	}
	
	/*
	 */
	void option_clicked(string option) {
		save_state(create_state(property,Unredo::STATE_MODIFY));
		if(option == "intersection") property.setIntersection(intersection_cb.isChecked());
		else if(option == "collision") property.setCollision(collision_cb.isChecked());
		else throw("Properties::option_clicked(): unknown option %s\n",option);
		update_parameters();
		update_states();
		update_colors();
	}
	
	/*
	 */
	void update_colors() {
		
		parent = property.getParent();
		
		// options
		intersection_cb.setFontColor((parent != NULL && parent.getIntersection() != property.getIntersection()) ? color_green : vec4_zero);
		collision_cb.setFontColor((parent != NULL && parent.getCollision() != property.getCollision()) ? color_green : vec4_zero);
	}
	
	/*
	 */
	void copy_clicked() {
		delete blob;
		
		blob = new Blob(0);
		property.saveState(blob,1);
		blob.seekSet(0);
		
		update_view();
	}
	
	void paste_clicked() {
		save_state(create_state(property,Unredo::STATE_MODIFY));
		property.restoreState(blob,1);
		blob.seekSet(0);
		update_view();
	}
	
	/**************************************************************************\
	*
	* Properties list
	*
	\**************************************************************************/
	
	/*
	 */
	void update_libraries() {
		
		libraries_cb.clear();
		libraries_cb.addItem(TR("All libraries"));
		forloop(int i = 0; engine.properties.getNumLibraries()) {
			libraries_cb.addItem(replace(basename(engine.properties.getLibraryName(i)),".prop",""));
		}
		
		libraries_i.setEnabled(engine.world.isLoaded());
		save_i.setEnabled(engine.world.isLoaded());
	}
	
	void update_properties() {
		
		Property old_property = property;
		property = NULL;
		
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
		
		int hidden[];
		saveTreeBoxFolded(properties_tb,hidden);
		
		string items[];
		properties_tb.setCallbackEnabled(GUI_CHANGED,0);
		properties_tb.clear();
		foreach(string name; properties) {
			items.append(name,properties_tb.addItem(name));
		}
		properties_tb.setCallbackEnabled(GUI_CHANGED,1);
		
		properties_tb.setCurrentItem(-1);
		foreach(string name; properties) {
			Property property = engine.properties.findProperty(name);
			if(property == old_property) properties_tb.setCurrentItem(items[name]);
			
			Property parent = property.getParent();
			if(parent == NULL) properties_tb.setItemColor(items[name],color_green);
			else if(property.getNumChilds()) properties_tb.setItemColor(items[name],color_orange);
			if(parent == NULL || properties.find(parent.getName()) == -1) continue;
			
			properties_tb.setItemParent(items[name],items[parent.getName()]);
		}
		if(properties_tb.getCurrentItem() == -1) update_view();
		
		restoreTreeBoxFolded(properties_tb,hidden);
	}
	
	/*
	 */
	void libraries_changed() {
		update_properties();
		name_el.runCallback(GUI_CHANGED);
	}
	
	void name_changed() {
		name_el.setCallbackEnabled(GUI_CHANGED,0);
		properties_tb.setCallbackEnabled(GUI_CHANGED,0);
		restoreTreeBoxFolded(properties_tb,properties_folded);
		if(selectTreeBoxByName(properties_tb,name_el,0)) update_view();
		name_el.setCallbackEnabled(GUI_CHANGED,1);
		properties_tb.setCallbackEnabled(GUI_CHANGED,1);
	}
	
	void name_pressed() {
		name_el.setCallbackEnabled(GUI_CHANGED,0);
		properties_tb.setCallbackEnabled(GUI_CHANGED,0);
		restoreTreeBoxFolded(properties_tb,properties_folded);
		if(selectTreeBoxByName(properties_tb,name_el,1)) update_view();
		name_el.setCallbackEnabled(GUI_CHANGED,1);
		properties_tb.setCallbackEnabled(GUI_CHANGED,1);
	}
	
	void name_focus_in() {
		saveTreeBoxFolded(properties_tb,properties_folded);
	}
	
	void name_focus_out() {
		properties_folded.clear();
	}
	
	/*
	 */
	void properties_changed() {
		update_view();
	}
	
	void properties_double_clicked() {
		int item = properties_tb.getCurrentItem();
		properties_tb.setItemFolded(item,!properties_tb.isItemFolded(item));
	}
	
	/*
	 */
	string get_property_name() {
		if(property == NULL) return "";
		return property.getName();
	}
	
	/*
	 */
	void select_property(string name) {
		int items_id[0];
		properties_tb.getItems(items_id);
		foreach(int item; items_id) {
			if(properties_tb.getItemText(item) == name) {
				int parent = properties_tb.getItemParent(item);
				while(parent != -1) {
					properties_tb.setItemFolded(parent,0);
					parent = properties_tb.getItemParent(parent);
				}
				properties_tb.setCurrentItem(item);
				properties_tb.showItem(item);
				return;
			}
		}
		properties_tb.setCurrentItem(-1);
	}
	
	/**************************************************************************\
	*
	* Buttons
	*
	\**************************************************************************/
	
	/*
	 */
	void libraries_clicked() {
		dialogPropertyLibraries(TR("Preloaded property libraries"));
		update_libraries();
		update_properties();
	}
	
	/*
	 */
	void save_clicked() {
		if(libraries_cb.getCurrentItem() == 0) {
			forloop(int i = 0; engine.properties.getNumLibraries()) {
				if(engine.properties.isLibraryEditable(i)) {
					engine.properties.save(engine.properties.getLibraryName(i));
				}
			}
		} else {
			int library = libraries_cb.getCurrentItem() - 1;
			if(engine.properties.isLibraryEditable(library)) {
				engine.properties.save(engine.properties.getLibraryName(library));
			}
		}
	}
	
	/*
	 */
	void assign_clicked() {
		Node nodes[0];
		int surfaces[0];
		nodesGetNodes(nodes);
		Node node = nodesGetNode(surfaces);
		Nodes::NodesState_ s = new Nodes::NodesState_();
		s.init_modify(nodes,node);
		foreach(Node n; nodes) {
			if(n == node || n.isObject() == 0) continue;
			Object object = node_cast(n);
			forloop(int i = 0; object.getNumSurfaces()) {
				object.setProperty(property.getName(),i);
			}
		}
		if(node != NULL && node.isObject()) {
			Object object = node_cast(node);
			foreach(int surface; surfaces) {
				object.setProperty(property.getName(),surface);
			}
			nodesUpdateObjectPropertyName();
		}
		Nodes::save_state(s);
	}
	
	/*
	 */
	void clone_clicked() {
		string library = engine.properties.getLibraryName(engine.properties.findPropertyLibrary(property.getName()));
		string name = getNewPropertyName(property.getName());
		if(dialogPropertyName(TR("Clone property to"),library,name)) {
			if(engine.properties.cloneProperty(property.getName(),library,name) == 0) {
				dialogMessageOk(TR("Error"));
				return;
			}
			PropertyState_ s = create_state(engine.properties.findProperty(name),Unredo::STATE_NEW);
			s.selected = property.getName();
			update_properties();
			select_property(name);
			save_state(s);
		}
	}
	
	void inherit_clicked() {
		string library = engine.properties.getLibraryName(engine.properties.findPropertyLibrary(property.getName()));
		string name = getNewPropertyName(property.getName());
		if(dialogPropertyName(TR("Inherit property to"),library,name)) {
			if(engine.properties.inheritProperty(property.getName(),library,name) == 0) {
				dialogMessageOk(TR("Error"));
				return;
			}
			PropertyState_ s = create_state(engine.properties.findProperty(name),Unredo::STATE_NEW);
			s.selected = property.getName();
			update_properties();
			select_property(name);
			save_state(s);
		}
	}
	
	void move_clicked() {
		string library = engine.properties.getLibraryName(engine.properties.findPropertyLibrary(property.getName()));
		string name = property.getName();
		if(dialogPropertyName(TR("Move property to"),library,name)) {
			PropertyState_ s = create_state(property,Unredo::STATE_MOVE);
			if(engine.properties.moveProperty(property.getName(),library,name) == 0) {
				dialogMessageOk(TR("Error"));
				return;
			}
			update_properties();
			select_property(name);
			save_state(s);
		}
	}
	
	void remove_clicked() {
		string message = format(TR("Remove \"%s\" property"),property.getName());
		if(property.getNumChilds()) message += format(TR(" with children"));
		if(dialogMessageYesNo(TR("Confirm"),message + "?")) {
			PropertyState_ s = create_state(property,Unredo::STATE_REMOVE);
			if(engine.properties.removeProperty(property.getName()) == 0) {
				dialogMessageOk(TR("Error"));
				return;
			}
			property = NULL;
			update_properties();
			save_state(s);
		}
	}
	
	/**************************************************************************\
	*
	* Properties
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		// update properties
		update_libraries();
		update_properties();
		
		// default tab
		property_tb.setCurrentTab(0);
		
		// configure window
		editorWindow(window);
		DECLARE_WINDOW_LOAD(properties)
		
		int value = configGet("properties_value",window_hp.getValue());
		window_hp.setValue(value);
		
		blob = NULL;
	}
	
	void shutdown() {
		
		if(blob != NULL) {
			delete blob;
			blob = NULL;
		}
		
		DECLARE_WINDOW_SAVE(properties)
		
		configSet("properties_value",window_hp.getValue());
	}
	
	/*
	 */
	void update(int need_reload) {
		
		// reload properties
		if(need_reload) {
			update_libraries();
			update_properties();
		}
	}
	
} /* namespace Properties */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void propertiesInit() {
	Properties::init();
	Properties::Editor::init();
}

void propertiesShutdown() {
	Properties::shutdown();
	Properties::Editor::shutdown();
}

/*
 */
void propertiesToggle() {
	if(Properties::window_show) propertiesHide();
	else propertiesShow();
}

void propertiesShow() {
	DECLARE_WINDOW_SHOW(Properties)
}

void propertiesHide() {
	DECLARE_WINDOW_HIDE(Properties)
}

/*
 */
void propertiesUpdate(int need_reload) {
	Properties::update(need_reload);
}

void propertiesUpdateView() {
	Properties::update_view();
}

/*
 */
void propertiesSelect(string name) {
	Properties::select_property(name);
}

/*
 */
int propertiesEditor(Property property) {
	return Properties::Editor::run(property);
}
