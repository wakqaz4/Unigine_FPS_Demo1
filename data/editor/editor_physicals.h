/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_physicals.h
 * Desc:    Unigine editor
 * Version: 1.06
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

/*
 */
namespace Physical {
	
	/*
	 */
	WidgetEditLine physical_mask_el;		// physical mask
	
	Physical physical;						// base physical
	
	/*
	 */
	void init() {
		
	}
	
	void shutdown() {
		
		physical = NULL;
	}
	
	/*
	 */
	void update() {
		
		physical = class_cast("Physical",node);
		
		physical_mask_el.setText(editorFormatMask(physical.getPhysicalMask()));
	}
	
	/*
	 */
	void physical_mask_changed(int mask) {
		physical.setPhysicalMask(mask);
		physical_mask_el.setText(editorFormatMask(physical.getPhysicalMask()));
	}
	
	/*
	 */
	PRESSED_CALLBACK_MASK(physical_mask,physical,PhysicalMask,physical)
	CLICKED_CALLBACK_MASK(physical_mask,physical,PhysicalMask,physical,TR("Physical mask"),"Nodes::Physical::physical_mask_changed")
	
	/**************************************************************************\
	*
	* PhysicalWind
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Wind {
		
		/*
		 */
		WidgetVBox main_vb;					// parameters vbox
		
		WidgetEditLine physical_mask_el;	// parameters
		WidgetEditLine size_el[3];
		WidgetEditLine threshold_el[3];
		WidgetEditLine velocity_el[3];
		WidgetEditLine linear_damping_el;
		WidgetEditLine angular_damping_el;
		
		PhysicalWind wind;
		
		/*
		 */
		void init() {
			
			Nodes::Physical::init();
			
			parameters_tb.addTab(TR("Wind"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
			Nodes::Physical::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Physical::physical_mask_el = physical_mask_el;
			
			Nodes::Physical::update();
			
			wind = node_cast(node);
			
			setEditLineVec3(size_el,wind.getSize());
			setEditLineVec3(threshold_el,wind.getThreshold());
			setEditLineVec3(velocity_el,wind.getVelocity());
			linear_damping_el.setText(editorFormat(wind.getLinearDamping()));
			angular_damping_el.setText(editorFormat(wind.getAngularDamping()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			wind.renderVisualizer();
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(size,wind,Size,wind,modify)
		PRESSED_CALLBACK_VEC3(threshold,wind,Threshold,wind,modify)
		PRESSED_CALLBACK_VEC3(velocity,wind,Velocity,wind,modify)
		PRESSED_CALLBACK_FLOAT(linear_damping,wind,LinearDamping,wind,modify)
		PRESSED_CALLBACK_FLOAT(angular_damping,wind,AngularDamping,wind,modify)
		
	} /* namespace Wind */
	
	/**************************************************************************\
	*
	* PhysicalForce
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Force {
		
		/*
		 */
		WidgetVBox main_vb;					// parameters vbox
		
		WidgetEditLine physical_mask_el;	// parameters
		WidgetEditLine radius_el;
		WidgetEditLine attenuation_el;
		WidgetEditLine attractor_el;
		WidgetSlider attractor_sl;
		WidgetEditLine rotator_el;
		WidgetSlider rotator_sl;
		
		PhysicalForce force;
		
		/*
		 */
		void init() {
			
			Nodes::Physical::init();
			
			parameters_tb.addTab(TR("Force"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
			Nodes::Physical::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Physical::physical_mask_el = physical_mask_el;
			
			Nodes::Physical::update();
			
			force = node_cast(node);
			
			radius_el.setText(editorFormat(force.getRadius()));
			attenuation_el.setText(editorFormat(force.getAttenuation()));
			attractor_el.setText(editorFormat(force.getAttractor()));
			rotator_el.setText(editorFormat(force.getRotator()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			force.renderVisualizer();
		}
		
		/*
		 */
		PRESSED_CALLBACK_FLOAT(radius,force,Radius,force,modify)
		PRESSED_CALLBACK_FLOAT(attenuation,force,Attenuation,force,modify)
		PRESSED_CALLBACK_SLIDER(attractor,force,Attractor,force,modify)
		PRESSED_CALLBACK_SLIDER(rotator,force,Rotator,force,modify)
		
	} /* namespace Force */
	
	/**************************************************************************\
	*
	* PhysicalNoise
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Noise {
		
		/*
		 */
		WidgetVBox main_vb;					// parameters vbox
		
		WidgetEditLine physical_mask_el;	// parameters
		WidgetEditLine size_el[3];
		WidgetEditLine threshold_el[3];
		WidgetEditLine offset_el[3];
		WidgetEditLine step_el[3];
		WidgetEditLine force_el;
		WidgetSlider force_sl;
		WidgetEditLine scale_el;
		WidgetSlider scale_sl;
		WidgetEditLine frequency_el;
		WidgetEditLine image_size_el;
		WidgetSprite noise_s;
		
		PhysicalNoise noise;
		
		/*
		 */
		void init() {
			
			Nodes::Physical::init();
			
			parameters_tb.addTab(TR("Noise"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
			Nodes::Physical::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Physical::physical_mask_el = physical_mask_el;
			
			Nodes::Physical::update();
			
			noise = node_cast(node);
			
			setEditLineVec3(size_el,noise.getSize());
			setEditLineVec3(threshold_el,noise.getThreshold());
			setEditLineVec3(offset_el,noise.getOffset());
			setEditLineSameVec3(step_el,noise.getStep());
			force_el.setText(editorFormat(noise.getForce()));
			scale_el.setText(editorFormat(noise.getScale()));
			frequency_el.setText(string(noise.getFrequency()));
			image_size_el.setText(string(noise.getImageSize()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			noise.renderVisualizer();
			
			float scale;
			int size,frequency;
			if(size != noise.getImageSize() || scale != noise.getScale() || frequency != noise.getFrequency()) {
				scale = noise.getScale();
				size = noise.getImageSize();
				frequency = noise.getFrequency();
				
				Image image = noise.getImage();
				if(image != NULL) {
					Image sprite = new Image();
					sprite.create2D(image.getWidth(),image.getHeight(),image.getFormat());
					sprite.copy(image,0,0,0,0,sprite.getWidth(),sprite.getHeight());
					noise_s.setImage(sprite);
					delete sprite;
				}
			}
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(size,noise,Size,noise,modify)
		PRESSED_CALLBACK_VEC3(threshold,noise,Threshold,noise,modify)
		PRESSED_CALLBACK_VEC3(offset,noise,Offset,noise,modify)
		PRESSED_CALLBACK_VEC3_SAME(step,noise,Step,noise,modify)
		PRESSED_CALLBACK_SLIDER(force,noise,Force,noise,modify)
		PRESSED_CALLBACK_SLIDER(scale,noise,Scale,noise,modify)
		PRESSED_CALLBACK(frequency,noise,Frequency,noise,int,modify)
		PRESSED_CALLBACK(image_size,noise,ImageSize,noise,int,modify)
		
	} /* namespace Noise */
	
	/**************************************************************************\
	*
	* PhysicalWater
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Water {
		
		/*
		 */
		WidgetVBox main_vb;					// parameters vbox
		
		WidgetEditLine physical_mask_el;	// parameters
		WidgetEditLine size_el[3];
		WidgetEditLine velocity_el[3];
		WidgetEditLine density_el;
		WidgetEditLine linear_damping_el;
		WidgetEditLine angular_damping_el;
		
		PhysicalWater water;
		
		/*
		 */
		void init() {
			
			Nodes::Physical::init();
			
			parameters_tb.addTab(TR("Water"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
			Nodes::Physical::shutdown();
		}
		
		/*
		 */
		void update() {
			
			Nodes::Physical::physical_mask_el = physical_mask_el;
			
			Nodes::Physical::update();
			
			water = node_cast(node);
			
			setEditLineVec3(size_el,water.getSize());
			setEditLineVec3(velocity_el,water.getVelocity());
			density_el.setText(editorFormat(water.getDensity()));
			linear_damping_el.setText(editorFormat(water.getLinearDamping()));
			angular_damping_el.setText(editorFormat(water.getAngularDamping()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			water.renderVisualizer();
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(size,water,Size,water,modify)
		PRESSED_CALLBACK_VEC3(velocity,water,Velocity,water,modify)
		PRESSED_CALLBACK_FLOAT(density,water,Density,water,modify)
		PRESSED_CALLBACK_FLOAT(linear_damping,water,LinearDamping,water,modify)
		PRESSED_CALLBACK_FLOAT(angular_damping,water,AngularDamping,water,modify)
		
	} /* namespace Water */
	
	/**************************************************************************\
	*
	* PhysicalTrigger
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Trigger {
		
		/*
		 */
		WidgetVBox main_vb;					// parameters vbox
		
		WidgetEditLine physical_mask_el;	// parameters
		WidgetEditLine collision_mask_el;
		WidgetComboBox type_cb;
		WidgetEditLine size_el[3];
		WidgetEditLine enter_el;
		WidgetEditLine leave_el;
		
		PhysicalTrigger trigger;
		
		/*
		 */
		void init() {
			
			Nodes::Physical::init();
			
			parameters_tb.addTab(TR("Trigger"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
			Nodes::Physical::shutdown();
		}
		
		/*
		 */
		void update_size_controls(int type) {
			
			if(type == SHAPE_SPHERE) {
				size_el[1].setEnabled(0);
				size_el[2].setEnabled(0);
			}
			else if(type == SHAPE_CAPSULE || type == SHAPE_CYLINDER) {
				size_el[1].setEnabled(1);
				size_el[2].setEnabled(0);
			}
			else if(type == SHAPE_BOX) {
				size_el[1].setEnabled(1);
				size_el[2].setEnabled(1);
			}
			else {
				throw("Nodes::Physical::Trigger::update_size_controls(): unknown shape type\n");
			}
			
			collision_mask_el.setText(editorFormatMask(trigger.getCollisionMask()));
		}
		
		/*
		 */
		void update() {
			
			Nodes::Physical::physical_mask_el = physical_mask_el;
			
			Nodes::Physical::update();
			
			trigger = node_cast(node);
			
			type_cb.setCallbackEnabled(GUI_CHANGED,0);
			
			type_cb.setCurrentItem(trigger.getShapeType());
			update_size_controls(trigger.getShapeType());
			
			setEditLineVec3(size_el,trigger.getSize());
			enter_el.setText(trigger.getEnterCallbackName());
			leave_el.setText(trigger.getLeaveCallbackName());
			
			type_cb.setCallbackEnabled(GUI_CHANGED,1);
		}
		
		/*
		 */
		void update_dynamic() {
			
			trigger.renderVisualizer();
		}
		
		/*
		 */
		void type_changed() {
			NodesState_ s = new NodesState_();
			int type = type_cb.getCurrentItem();
			if(type != trigger.getShapeType()) s.init_modify(nodes,trigger);
			foreach(Node n; nodes) {
				if(n.getType() != NODE_PHYSICAL_TRIGGER) continue;
				PhysicalTrigger(node_cast(n)).setShapeType(type);
			}
			update_size_controls(type);
			save_state(s);
		}
		
		/*
		 */
		void collision_mask_changed(int mask) {
			trigger.setCollisionMask(mask);
			collision_mask_el.setText(editorFormatMask(trigger.getCollisionMask()));
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(size,trigger,Size,trigger,modify)
		PRESSED_CALLBACK_MASK(collision_mask,trigger,CollisionMask,trigger)
		CLICKED_CALLBACK_MASK(collision_mask,trigger,CollisionMask,trigger,TR("Physical mask"),"Nodes::Physical::Trigger::collision_mask_changed")
		PRESSED_CALLBACK(enter,trigger,EnterCallbackName,trigger,string,modify)
		PRESSED_CALLBACK(leave,trigger,LeaveCallbackName,trigger,string,modify)
		
	} /* namespace Trigger */
	
} /* namespace Physical */
