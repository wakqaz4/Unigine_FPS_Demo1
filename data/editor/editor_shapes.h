/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_shapes.h
 * Desc:    Unigine editor
 * Version: 1.17
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
 *          Pavel Kurinnoy <figonet@unigine.com>
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
namespace Shape {
	
	/*
	 */
	WidgetVBox main_vb;							// main vbox
	
	WidgetComboBox shape_cb;					// shape type
	WidgetButton remove_b;						// remove shape
	
	WidgetListBox shapes_lb;					// shapes listbox
	
	WidgetManipulatorTranslator translator_m;	// manipulators
	WidgetManipulatorRotator rotator_m;
	WidgetManipulatorScaler scaler_m;
	
	WidgetVBox parameters_vb;					// shape container
	WidgetLabel type_l;							// shape type
	WidgetCheckBox enabled_cb;					// enabled flag
	WidgetCheckBox continuous_cb;				// continuous flag
	WidgetEditLine name_el;						// shape name
	WidgetEditLine intersection_mask_el;		// intersection mask
	WidgetEditLine collision_mask_el;			// collision mask
	WidgetEditLine exclusion_mask_el;			// exclusion mask
	WidgetEditLine mass_el;						// mass
	WidgetEditLine density_el;					// density
	WidgetEditLine friction_el;					// friction
	WidgetSlider friction_sl;
	WidgetEditLine restitution_el;				// restitution
	WidgetSlider restitution_sl;
	WidgetEditLine translate_el[3];				// translate
	WidgetEditLine rotate_el[3];				// rotate
	WidgetVBox dummy_vb;						// dummy container
	
	Body body;									// base body
	Shape shape;								// base shape
	int type = -1;								// shape type
	int num = -1;								// shape number
	
	int tab = -1;								// tab number
	
	string type_prefix[] = (
		SHAPE_SPHERE	: "Sphere::",
		SHAPE_CAPSULE	: "Capsule::",
		SHAPE_CYLINDER	: "Cylinder::",
		SHAPE_BOX		: "Box::",
		SHAPE_CONVEX	: "Convex::",
	);
	
	/*
	 */
	void run_function(string name) {
		if(type != -1) call(type_prefix[type] + name);
	}
	
	void run_function(string name,int arg) {
		if(type != -1) call(type_prefix[type] + name,arg);
	}
	
	/*
	 */
	void init() {
		
		int old_tab = parameters_tb.getCurrentTab();
		
		// add shapes tab
		tab = parameters_tb.getNumTabs();
		parameters_tb.addTab(TR("Shapes"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
		
		// select last tab
		parameters_tb.setCurrentTab(old_tab);
		
		// manipulators
		translator_m = new WidgetManipulatorTranslator(gui);
		rotator_m = new WidgetManipulatorRotator(gui);
		scaler_m = new WidgetManipulatorScaler(gui);
		translator_m.setCallback(GUI_CHANGED,functionid(manipulator_changed),translator_m);
		translator_m.setCallback(GUI_CLICKED,functionid(slider_clicked));
		rotator_m.setCallback(GUI_CHANGED,functionid(manipulator_changed),rotator_m);
		rotator_m.setCallback(GUI_CLICKED,functionid(slider_clicked));
		scaler_m.setCallback(GUI_CHANGED,functionid(manipulator_changed),scaler_m);
		scaler_m.setCallback(GUI_CLICKED,functionid(slider_clicked));
	}
	
	void shutdown() {
		
		// shutdown last shape
		run_function("shutdown");
		
		// remove manipulators and their callbacks
		if(translator_m != NULL) {
			if(editorIsChild(translator_m)) gui.removeChild(translator_m);
			delete translator_m;
			translator_m = NULL;
		}
		if(rotator_m != NULL) {
			if(editorIsChild(rotator_m)) gui.removeChild(rotator_m);
			delete rotator_m;
			rotator_m = NULL;
		}
		if(scaler_m != NULL) {
			if(editorIsChild(scaler_m)) gui.removeChild(scaler_m);
			delete scaler_m;
			scaler_m = NULL;
		}
		
		// clear shape
		shape = NULL;
		type = -1;
		num = -1;
		
		// remove shapes tab
		if(tab != -1) {
			parameters_tb.removeTab(tab);
			tab = -1;
		}
	}
	
	/*
	 */
	void update_view() {
		
		// disable callbacks
		setCallbackEnabled(GUI_CLICKED,0,(
			enabled_cb,
			continuous_cb,
		));
		
		// clear view
		if(shape == NULL) {
			
			// parameters
			parameters_vb.setEnabled(0);
			
			type_l.setText("");
			enabled_cb.setChecked(0);
			continuous_cb.setChecked(0);
			
			name_el.setText("");
			intersection_mask_el.setText("");
			collision_mask_el.setText("");
			exclusion_mask_el.setText("");
			mass_el.setText("");
			density_el.setText("");
			friction_el.setText("");
			restitution_el.setText("");
			
			forloop(int i = 0; 3) {
				translate_el[i].setText("");
				rotate_el[i].setText("");
			}
		}
		// fill view
		else {
			
			// parameters
			parameters_vb.setEnabled(1);
			
			type_l.setText(shape.getTypeName());
			enabled_cb.setChecked(shape.isEnabled());
			continuous_cb.setChecked(shape.isContinuous());
			
			name_el.setText(shape.getName());
			intersection_mask_el.setText(editorFormatMask(shape.getIntersectionMask()));
			collision_mask_el.setText(editorFormatMask(shape.getCollisionMask()));
			exclusion_mask_el.setText(editorFormatMask(shape.getExclusionMask()));
			mass_el.setText(editorFormat(shape.getMass()));
			density_el.setText(editorFormat(shape.getDensity()));
			friction_el.setText(editorFormat(shape.getFriction()));
			restitution_el.setText(editorFormat(shape.getRestitution()));
			
			vec3 translate,rotate;
			decomposeTransform(body.getShapeTransform(num),translate,rotate);
			
			setEditLineVec3(translate_el,translate);
			setEditLineVec3(rotate_el,rotate);
		}
		
		// enable callbacks
		setCallbackEnabled(GUI_CLICKED,1,(
			enabled_cb,
			continuous_cb,
		));
	}
	
	/*
	 */
	void update() {
		
		// shape
		shape = NULL;
		type = -1;
		num = -1;
		
		// shapes list
		shapes_lb.clear();
		forloop(int i = 0; body.getNumShapes()) {
			Shape shape = body.getShape(i);
			if(shape.getName() == "") shapes_lb.addItem(shape.getTypeName());
			else shapes_lb.addItem(shape.getName());
		}
		shapes_lb.setCurrentItem(-1);
		
		// select first shape
		if(shapes_lb.getNumItems()) {
			remove_b.setEnabled(1);
			shapes_lb.setCurrentItem(0);
		} else {
			remove_b.setEnabled(0);
			update_view();
		}
	}
	
	/*
	 */
	void update_manipulator(WidgetManipulator manipulator,int is_show) {
		if(is_show == 0) {
			gui.removeChild(manipulator);
			return;
		}
		updateManipulator(manipulator);
		if(!manipulator.isFocused()) {
			manipulator.setTransform(body.getTransform() * body.getShapeTransform(num));
		}
		if(editorIsChild(manipulator) == 0) {
			gui.addChild(manipulator,GUI_ALIGN_OVERLAP);
		}
	}
	
	void update_manipulators(int is_show = 1) {
		update_manipulator(translator_m,is_show * panelPlacementGetTranslator());
		update_manipulator(rotator_m,is_show * panelPlacementGetRotator());
		update_manipulator(scaler_m,is_show * panelPlacementGetScaler());
	}
	
	/*
	 */
	void update_transform() {
		if(translator_m.isFocused() || rotator_m.isFocused()) {
			WidgetManipulator m = (translator_m.isFocused()) ? translator_m : rotator_m;
			vec3 translate,rotate;
			decomposeTransform(inverse(body.getTransform()) * m.getTransform(),translate,rotate);
			setEditLineVec3(translate_el,translate);
			setEditLineVec3(rotate_el,rotate);
		}
		if(scaler_m.isFocused()) {
			// update size?
		}
	}
	
	void update_dynamic() {
		
		run_function("update_dynamic");
		
		if(shape != NULL && parameters_tb.getCurrentTab() == tab) {
			shape.renderVisualizer(vec4_one);
			update_manipulators();
			update_transform();
		} else {
			if(translator_m != NULL && rotator_m != NULL && scaler_m != NULL) update_manipulators(0);
		}
	}
	
	/*
	 */
	void add_clicked() {
		
		NodesState_ s = new NodesState_();
		s.init_modify(nodes,object);
		
		// default shape parameters (for ragdoll bones)
		vec3 size = vec3_one;
		vec3 center = object.getBoundSphere().getCenter();
		float radius = 1.0f;
		
		// best shape parameters
		Body parent_body = object.getBody();
		Object object = body.getObject();
		
		if(parent_body == NULL || parent_body.getType() != BODY_RAGDOLL) {
			BoundBox bb = object.getBoundBox();
			BoundSphere bs = object.getBoundSphere();
			size = bb.getMax() - bb.getMin();
			center = bs.getCenter();
			radius = bs.getRadius();
		}
		
		// create shape
		int type = shape_cb.getCurrentItem();
		
		// sphere
		if(type == SHAPE_SPHERE) {
			Shape shape = new ShapeSphere(radius);
			body.addShape(class_remove(shape),translate(center));
		}
		
		// capsule
		else if(type == SHAPE_CAPSULE) {
			radius = length(vec3(size.x,size.y,0.0f)) / 2.0f;
			Shape shape = new ShapeCapsule(radius,size.z - radius * 2.0f);
			body.addShape(class_remove(shape),translate(center));
		}
		
		// cylinder
		else if(type == SHAPE_CYLINDER) {
			radius = length(vec3(size.x,size.y,0.0f)) / 2.0f;
			Shape shape = new ShapeCylinder(radius,size.z);
			body.addShape(class_remove(shape),translate(center));
		}
		
		// box
		else if(type == SHAPE_BOX) {
			Shape shape = new ShapeBox(size);
			body.addShape(class_remove(shape),translate(center));
		}
		
		// convex
		else if(type == SHAPE_CONVEX) {
			if(parent_body == NULL || parent_body.getType() != BODY_RAGDOLL) {
				float error;
				if(dialogConvex(TR("Convex parameters"),error)) {
					ShapeConvex shape = new ShapeConvex();
					shape.setObject(object,-1,error);
					if(shape.getVolume() < EPSILON) {
						dialogMessageOk(TR("Error"),TR("Can't create ShapeConvex"));
						delete shape;
					} else {
						body.addShape(class_remove(shape));
					}
				}
			} else {
				dialogMessageOk(TR("Error"),TR("Can't create ShapeConvex for RagDoll bones"));
			}
		}
		
		// autogenerated convexes
		else if(type == SHAPE_CONVEX + 1) {
			if(parent_body != NULL && parent_body.getType() == BODY_RIGID) {
				int depth;
				float error;
				float threshold;
				if(dialogDecompose(TR("Decompose parameters"),depth,error,threshold)) {
					BodyRigid rigid = body_cast(parent_body);
					rigid.createShapes(depth,error,threshold);
				}
			} else {
				dialogMessageOk(TR("Error"),TR("Can't create ShapeConvex"));
			}
		}
		
		else throw("Nodes::Object::shape::add_clicked(): unknown shape type %d\n",type);
		
		save_state(s);
		
		// shutdown last shape
		run_function("shutdown");
		
		update();
		
		shapes_lb.setCurrentItem(shapes_lb.getNumItems() - 1);
	}
	
	void remove_clicked() {
		
		NodesState_ s = new NodesState_();
		if(dialogMessageYesNo(TR("Confirm"),format(TR("Remove %s shape from the %s?"),shape.getTypeName(),body.getTypeName()))) {
			
			s.init_modify(nodes,object);
			
			// shutdown last shape
			run_function("shutdown");
			
			// delete shape
			body.removeShape(shape);
			delete class_append(shape);
			
			update();
		}
		
		save_state(s);
	}
	
	/*
	 */
	void shapes_changed() {
		
		// shutdown last shape
		run_function("shutdown");
		
		// select shape
		num = shapes_lb.getCurrentItem();
		if(num >= 0 && num < body.getNumShapes()) {
			remove_b.setEnabled(1);
			shape = body.getShape(num);
			type = shape.getType();
			run_function("init");
			run_function("update");
		}
		// null shape
		else {
			remove_b.setEnabled(0);
			shape = NULL;
			type = -1;
			update_view();
		}
	}
	
	/*
	 */
	void clone_clicked() {
		
		if(shape == NULL) return;
		
		NodesState_ s = new NodesState_();
		s.init_modify(nodes,object);
		
		Shape clone = shape_clone(shape);
		string name = (shape.getName() == "") ? shape.getTypeName() : shape.getName();
		clone.setName(name + TR(" clone"));
		body.addShape(clone,body.getShapeTransform(num));
		
		save_state(s);
		
		// shutdown last shape
		run_function("shutdown");
		
		update();
		
		shapes_lb.setCurrentItem(shapes_lb.getNumItems() - 1);
	}
	
	/*
	 */
	void set_manipulator(WidgetManipulator m) {
		updateManipulator(m);
		m.setBasis(body.getTransform());
		m.setTransform(body.getTransform() * body.getShapeTransform(num));
	}
	
	void manipulator_changed(WidgetManipulator m) {
		
		if(node == NULL || num < 0 || num >= body.getNumShapes()) return;
		
		mat4 transform = mat4(inverse(body.getTransform()) * m.getTransform());
		
		if(compare(body.getShapeTransform(num),transform,STATE_EPSILON) == 0 && m.isFocused()) {
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,object);
			save_state(s,m);
			body.setShapeTransform(num,transform);
		}
	}
	
	/*
	 */
	CLICKED_CALLBACK(enabled,shape,Enabled,object)
	CLICKED_CALLBACK(continuous,shape,Continuous,object)
	
	/*
	 */
	void name_pressed() {
		if(num < 0 || num >= body.getNumShapes()) return;
		NodesState_ s = new NodesState_();
		if(name_el.getText() != "") {
			if(shape.getName() == "" || shape.getName() != name_el.getText()) s.init_modify(nodes,object);
			shapes_lb.setItemText(num,name_el.getText());
			shape.setName(name_el.getText());
		} else {
			if(shape.getName() != "") s.init_modify(nodes,object);
			shapes_lb.setItemText(num,shape.getTypeName());
			shape.setName("");
		}
		save_state(s);
	}
	
	void mass_pressed() {
		if(shape == NULL) return;
		NodesState_ s = new NodesState_();
		if(shape.getMass() != float(mass_el.getText())) s.init_modify(nodes,object);
		shape.setMass(float(mass_el.getText()));
		mass_el.setText(editorFormat(shape.getMass()));
		density_el.setText(editorFormat(shape.getDensity()));
		save_state(s);
	}
	
	void density_pressed() {
		if(shape == NULL) return;
		NodesState_ s = new NodesState_();
		if(shape.getDensity() != float(density_el.getText())) s.init_modify(nodes,object);
		shape.setDensity(float(density_el.getText()));
		mass_el.setText(editorFormat(shape.getMass()));
		density_el.setText(editorFormat(shape.getDensity()));
		save_state(s);
	}
	
	/*
	 */
	PRESSED_CALLBACK_MASK(intersection_mask,shape,IntersectionMask,object)
	CLICKED_CALLBACK_MASK(intersection_mask,shape,IntersectionMask,object,TR("Shape intersection mask"),"Nodes::Object::Shape::intersection_mask_changed")
	PRESSED_CALLBACK_MASK(collision_mask,shape,CollisionMask,object)
	CLICKED_CALLBACK_MASK(collision_mask,shape,CollisionMask,object,TR("Shape collision mask"),"Nodes::Object::Shape::collision_mask_changed")
	PRESSED_CALLBACK_MASK(exclusion_mask,shape,ExclusionMask,object)
	CLICKED_CALLBACK_MASK(exclusion_mask,shape,ExclusionMask,object,TR("Shape exclusion mask"),"Nodes::Object::Shape::exclusion_mask_changed")
	PRESSED_CALLBACK_SLIDER(friction,shape,Friction,object,modify)
	PRESSED_CALLBACK_SLIDER(restitution,shape,Restitution,object,modify)
	
	/*
	 */
	void intersection_mask_changed(int mask) {
		if(shape == NULL) return;
		shape.setIntersectionMask(mask);
		intersection_mask_el.setText(editorFormatMask(shape.getIntersectionMask()));
	}
	
	void collision_mask_changed(int mask) {
		if(shape == NULL) return;
		shape.setCollisionMask(mask);
		collision_mask_el.setText(editorFormatMask(shape.getCollisionMask()));
	}
	
	void exclusion_mask_changed(int mask) {
		if(shape == NULL) return;
		shape.setExclusionMask(mask);
		exclusion_mask_el.setText(editorFormatMask(shape.getExclusionMask()));
	}
	
	/*
	 */
	void transform_pressed() {
		
		if(num < 0 || num >= body.getNumShapes()) return;
		
		NodesState_ s = new NodesState_();
		
		mat4 transform = mat4(composeTransform(getEditLineVec3(translate_el),getEditLineVec3(rotate_el)));
		if(compare(body.getShapeTransform(num),transform,STATE_EPSILON) == 0) s.init_modify(nodes,object);
		body.setShapeTransform(num,transform);
		
		vec3 translate,rotate;
		decomposeTransform(transform,translate,rotate);
		
		setEditLineVec3(translate_el,translate);
		setEditLineVec3(rotate_el,rotate);
		
		// update manipulators
		setCallbackEnabled(GUI_CHANGED,0,(
			translator_m,
			rotator_m,
			scaler_m,
		));
		
		transform = body.getTransform() * transform;
		translator_m.setTransform(transform);
		rotator_m.setTransform(transform);
		scaler_m.setTransform(transform);
		
		setCallbackEnabled(GUI_CHANGED,1,(
			translator_m,
			rotator_m,
			scaler_m,
		));
		
		save_state(s);
	}
	
	/**************************************************************************\
	*
	* ShapeSphere
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Sphere {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetEditLine radius_el;
		
		ShapeSphere shape;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_vb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			shape = shape_cast(body.getShape(num));
			
			radius_el.setText(editorFormat(shape.getRadius()));
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		PRESSED_CALLBACK_FLOAT(radius,shape,Radius,object,modify)
		
	} /* namespace Sphere */
	
	/**************************************************************************\
	*
	* ShapeCapsule
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Capsule {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetEditLine radius_el;
		WidgetEditLine height_el;
		
		ShapeCapsule shape;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_vb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			shape = shape_cast(body.getShape(num));
			
			radius_el.setText(editorFormat(shape.getRadius()));
			height_el.setText(editorFormat(shape.getHeight()));
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		PRESSED_CALLBACK_FLOAT(radius,shape,Radius,object,modify)
		PRESSED_CALLBACK_FLOAT(height,shape,Height,object,modify)
		
	} /* namespace Capsule */
	
	/**************************************************************************\
	*
	* ShapeCylinder
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Cylinder {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetEditLine radius_el;
		WidgetEditLine height_el;
		
		ShapeCylinder shape;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_vb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			shape = shape_cast(body.getShape(num));
			
			radius_el.setText(editorFormat(shape.getRadius()));
			height_el.setText(editorFormat(shape.getHeight()));
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		PRESSED_CALLBACK_FLOAT(radius,shape,Radius,object,modify)
		PRESSED_CALLBACK_FLOAT(height,shape,Height,object,modify)
		
	} /* namespace Cylinder */
	
	/**************************************************************************\
	*
	* ShapeBox
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Box {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetEditLine size_el[3];
		
		ShapeBox shape;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_vb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			shape = shape_cast(body.getShape(num));
			
			setEditLineVec3(size_el,shape.getSize());
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(size,shape,Size,object,modify)
		
	} /* namespace Box */
	
	/**************************************************************************\
	*
	* ShapeConvex
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Convex {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		
		ShapeConvex shape;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_vb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			shape = shape_cast(body.getShape(num));
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
	} /* namespace Convex */
	
} /* namespace Shape */
