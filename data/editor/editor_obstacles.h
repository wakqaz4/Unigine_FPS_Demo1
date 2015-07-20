/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_obstacles.h
 * Desc:    Unigine editor
 * Version: 1.02
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
* Obstacle
*
\******************************************************************************/

/*
 */
namespace Obstacle {
	
	/*
	 */
	WidgetEditLine obstacle_mask_el;		// obstacle mask
	
	Obstacle obstacle;						// base obstacle node
	
	/*
	 */
	void update() {
		
		obstacle = class_cast("Obstacle",node);
		
		if(obstacle_mask_el != NULL) obstacle_mask_el.setText(editorFormatMask(obstacle.getObstacleMask()));
	}
	
	/*
	 */
	void obstacle_mask_changed(int mask) {
		obstacle.setObstacleMask(mask);
		obstacle_mask_el.setText(editorFormatMask(obstacle.getObstacleMask()));
	}
	
	/*
	 */
	PRESSED_CALLBACK_MASK(obstacle_mask,obstacle,ObstacleMask,obstacle);
	CLICKED_CALLBACK_MASK(obstacle_mask,obstacle,ObstacleMask,obstacle,TR("Obstacle mask"),"Nodes::Obstacle::obstacle_mask_changed")
	
	/**************************************************************************\
	*
	* Box
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Box {
		
		/*
		 */
		WidgetVBox main_vb;
		
		WidgetEditLine obstacle_mask_el;
		WidgetEditLine size_el[3];
		
		ObstacleBox obstacle;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Obstacle"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Obstacle::obstacle_mask_el = obstacle_mask_el;
			Nodes::Obstacle::update();
			
			obstacle = node_cast(node);
			
			setEditLineVec3(size_el,obstacle.getSize());
		}
		
		/*
		 */
		void update_dynamic() {
			
			obstacle.renderVisualizer();
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(size,obstacle,Size,obstacle,modify)
	}
	
	/**************************************************************************\
	*
	* Sphere
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Sphere {
		
		/*
		 */
		WidgetVBox main_vb;
		
		WidgetEditLine obstacle_mask_el;
		WidgetEditLine radius_el;
		
		ObstacleSphere obstacle;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Obstacle"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Obstacle::obstacle_mask_el = obstacle_mask_el;
			Nodes::Obstacle::update();
			
			obstacle = node_cast(node);
			
			radius_el.setText(editorFormat(obstacle.getRadius()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			obstacle.renderVisualizer();
		}
		
		/*
		 */
		PRESSED_CALLBACK_FLOAT(radius,obstacle,Radius,obstacle,modify)
	}
	
	/**************************************************************************\
	*
	* Capsule
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Capsule {
		
		/*
		 */
		WidgetVBox main_vb;
		
		WidgetEditLine obstacle_mask_el;
		WidgetEditLine radius_el;
		WidgetEditLine height_el;
		
		ObstacleCapsule obstacle;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Obstacle"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Obstacle::obstacle_mask_el = obstacle_mask_el;
			Nodes::Obstacle::update();
			
			obstacle = node_cast(node);
			
			radius_el.setText(editorFormat(obstacle.getRadius()));
			height_el.setText(editorFormat(obstacle.getHeight()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			obstacle.renderVisualizer();
		}
		
		/*
		 */
		PRESSED_CALLBACK_FLOAT(radius,obstacle,Radius,obstacle,modify)
		PRESSED_CALLBACK_FLOAT(height,obstacle,Height,obstacle,modify)
	}
}
