/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_navigations.h
 * Desc:    Unigine editor
 * Version: 1.03
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in force and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

/******************************************************************************\
*
* Navigation
*
\******************************************************************************/

/*
 */
namespace Navigation {
	
	/*
	 */
	WidgetEditLine navigation_mask_el;		// navigation mask
	WidgetEditLine quality_el;				// quality
	WidgetEditLine velocity_el;				// velocity
	WidgetEditLine dangerous_el;			// dangerous
	
	Navigation navigation;					// base navigation node
	
	/*
	 */
	void update() {
		
		navigation = class_cast("Navigation",node);
		
		if(navigation_mask_el != NULL) navigation_mask_el.setText(editorFormatMask(navigation.getNavigationMask()));
		if(quality_el != NULL) quality_el.setText(string(navigation.getQuality()));
		if(velocity_el != NULL) velocity_el.setText(editorFormat(navigation.getVelocity()));
		if(dangerous_el != NULL) dangerous_el.setText(editorFormat(navigation.getDangerous()));
	}
	
	/*
	 */
	void update_dynamic() {
		
		navigation.renderVisualizer();
		
		forloop(int i = 0; navigation.getNumNavigations()) {
			Navigation n = navigation.getNavigation(i);
			n.renderVisualizer();
		}
	}
	
	/*
	 */
	void navigation_mask_changed(int mask) {
		navigation.setNavigationMask(mask);
		navigation_mask_el.setText(editorFormatMask(navigation.getNavigationMask()));
	}
	
	/*
	 */
	PRESSED_CALLBACK_MASK(navigation_mask,navigation,NavigationMask,navigation);
	CLICKED_CALLBACK_MASK(navigation_mask,navigation,NavigationMask,navigation,TR("Navigation mask"),"Nodes::Navigation::navigation_mask_changed")
	PRESSED_CALLBACK(quality,navigation,Quality,navigation,int,modify)
	PRESSED_CALLBACK_FLOAT(velocity,navigation,Velocity,navigation,modify)
	PRESSED_CALLBACK_FLOAT(dangerous,navigation,Dangerous,navigation,modify)
	
	/**************************************************************************\
	*
	* Sector
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Sector {
		
		/*
		 */
		WidgetVBox main_vb;
		
		WidgetEditLine navigation_mask_el;
		WidgetEditLine quality_el;
		WidgetEditLine velocity_el;
		WidgetEditLine dangerous_el;
		WidgetEditLine size_el[3];
		
		NavigationSector navigation;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Navigation"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Navigation::navigation_mask_el = navigation_mask_el;
			Nodes::Navigation::quality_el = quality_el;
			Nodes::Navigation::velocity_el = velocity_el;
			Nodes::Navigation::dangerous_el = dangerous_el;
			Nodes::Navigation::update();
			
			navigation = node_cast(node);
			
			setEditLineVec3(size_el,navigation.getSize());
		}
		
		/*
		 */
		void update_dynamic() {
			
			Nodes::Navigation::update_dynamic();
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(size,navigation,Size,navigation,modify)
	}
	
	/**************************************************************************\
	*
	* Mesh
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Mesh {
		
		/*
		 */
		WidgetVBox main_vb;
		
		WidgetEditLine navigation_mask_el;
		WidgetEditLine quality_el;
		WidgetEditLine velocity_el;
		WidgetEditLine dangerous_el;
		WidgetEditLine depth_el;
		WidgetEditLine height_el;
		
		NavigationMesh navigation;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Navigation"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Navigation::navigation_mask_el = navigation_mask_el;
			Nodes::Navigation::quality_el = quality_el;
			Nodes::Navigation::velocity_el = velocity_el;
			Nodes::Navigation::dangerous_el = dangerous_el;
			Nodes::Navigation::update();
			
			navigation = node_cast(node);
			
			depth_el.setText(string(navigation.getDepth()));
			height_el.setText(editorFormat(navigation.getHeight()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			Nodes::Navigation::update_dynamic();
		}
		
		/*
		 */
		void load_clicked() {
			string name;
			if(dialogFileMesh(TR("Select mesh to load"),name,DIALOG_FILE_OPEN)) {
				if(navigation.loadMesh(name) == 0) dialogMessageOk(TR("Error"));
			}
		}
		
		void save_clicked() {
			string name;
			if(dialogFileMesh(TR("Select mesh to save"),name,DIALOG_FILE_SAVE)) {
				name = savePath(name);
				if(navigation.saveMesh(name) == 0) dialogMessageOk(TR("Error"));
			}
		}
		
		/*
		 */
		PRESSED_CALLBACK(depth,navigation,Depth,navigation,int,modify)
		PRESSED_CALLBACK_FLOAT(height,navigation,Height,navigation,modify)
	}
}
