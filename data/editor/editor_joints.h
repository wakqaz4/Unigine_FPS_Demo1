/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_joints.h
 * Desc:    Unigine editor
 * Version: 1.23
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
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
namespace Joint {
	
	/*
	 */
	WidgetVBox main_vb;						// main vbox
	
	WidgetComboBox joint_cb;				// joint type
	WidgetButton remove_b;					// remove joint
	
	WidgetListBox joints_lb;				// joints listbox
	
	WidgetVBox parameters_vb;				// joint container
	WidgetLabel type_l;						// joint type
	WidgetCheckBox enabled_cb;				// enabled flag
	WidgetCheckBox collision_cb;			// collision flag
	WidgetEditLine name_el;					// joint name
	WidgetEditLine num_iterations_el;		// number of iterations
	WidgetSlider num_iterations_sl;
	WidgetEditLine max_force_el;			// maximum applied force
	WidgetEditLine max_torque_el;			// maximum applied torque
	WidgetEditLine linear_restitution_el;	// joint linear restitution
	WidgetSlider linear_restitution_sl;
	WidgetEditLine angular_restitution_el;	// joint angular restitution
	WidgetSlider angular_restitution_sl;
	WidgetEditLine linear_softness_el;		// joint linear softness
	WidgetSlider linear_softness_sl;
	WidgetEditLine angular_softness_el;		// joint angular softness
	WidgetSlider angular_softness_sl;
	WidgetEditLine anchor_0_el[3];			// local anchors
	WidgetEditLine anchor_1_el[3];
	WidgetButton bind_b;					// bind anchors
	WidgetVBox dummy_vb;					// dummy container
	
	Widget focus;							// focus after undo
	
	Body body;								// base body
	Joint joint;							// base joint
	int type = -1;							// joint type
	int num = -1;							// joint number
	int reverse = 0;						// joint reverse
	
	int tab = -1;							// tab number
	
	string type_prefix[] = (
		JOINT_FIXED			: "Fixed::",
		JOINT_BALL			: "Ball::",
		JOINT_HINGE			: "Hinge::",
		JOINT_PRISMATIC		: "Prismatic::",
		JOINT_CYLINDRICAL	: "Cylindrical::",
		JOINT_SUSPENSION	: "Suspension::",
		JOINT_WHEEL			: "Wheel::",
		JOINT_PARTICLES		: "Particles::",
		JOINT_PATH			: "Path::",
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
	void save_state() {
		NodesState_ s = new NodesState_();
		s.init_joint(nodes,object);
		save_state(s);
	}
	
	/*
	 */
	int get_joint_num() {
		return num;
	}
	
	void set_focus(Widget f,int i) {
		focus = f;
		joints_lb.setCurrentItem(i);
	}
	
	/*
	 */
	void init() {
		
		int old_tab = parameters_tb.getCurrentTab();
		
		// add joints tab
		tab = parameters_tb.getNumTabs();
		parameters_tb.addTab(TR("Joints"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
		
		// select last tab
		parameters_tb.setCurrentTab(old_tab);
	}
	
	void shutdown() {
		
		// shutdown last joint
		run_function("shutdown");
		
		// clear joint
		joint = NULL;
		type = -1;
		num = -1;
		
		// remove joints tab
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
			collision_cb,
		));		
		
		// clear view
		if(joint == NULL) {
			
			// parameters
			parameters_vb.setEnabled(0);
			
			// reverse = 0;
			
			type_l.setText("");
			enabled_cb.setChecked(0);
			collision_cb.setChecked(0);
			
			name_el.setText("");
			
			num_iterations_el.setText("");
			
			max_force_el.setText("");
			max_torque_el.setText("");
			
			linear_restitution_el.setText("");
			angular_restitution_el.setText("");
			
			linear_softness_el.setText("");
			angular_softness_el.setText("");
			
			forloop(int i = 0; 3) {
				anchor_0_el[i].setText("");
				anchor_1_el[i].setText("");
			}
			
			bind_b.setToggled(0);
		}
		// fill view
		else {
			
			// parameters
			parameters_vb.setEnabled(1);
			
			reverse = (joint.getBody1() == body);
			
			type_l.setText(joint.getTypeName());
			enabled_cb.setChecked(joint.isEnabled());
			collision_cb.setChecked(joint.getCollision());
			
			name_el.setText(joint.getName());
			
			num_iterations_el.setText(string(joint.getNumIterations()));
			
			max_force_el.setText(editorFormat(joint.getMaxForce()));
			max_torque_el.setText(editorFormat(joint.getMaxTorque()));
			
			linear_restitution_el.setText(editorFormat(joint.getLinearRestitution()));
			angular_restitution_el.setText(editorFormat(joint.getAngularRestitution()));
			
			linear_softness_el.setText(editorFormat(joint.getLinearSoftness()));
			angular_softness_el.setText(editorFormat(joint.getAngularSoftness()));
			
			if(reverse) {
				setEditLineVec3(anchor_0_el,joint.getAnchor1());
				setEditLineVec3(anchor_1_el,joint.getAnchor0());
			} else {
				setEditLineVec3(anchor_0_el,joint.getAnchor0());
				setEditLineVec3(anchor_1_el,joint.getAnchor1());
			}
			
			bind_b.setToggled(0);
		}
		
		// enable callbacks
		setCallbackEnabled(GUI_CLICKED,1,(
			enabled_cb,
			collision_cb,
		));
	}
	
	/*
	 */
	void update() {
		
		// joint
		joint = NULL;
		type = -1;
		num = -1;
		reverse = 0;
		
		// joints list
		joints_lb.clear();
		forloop(int i = 0; body.getNumJoints()) {
			Joint joint = body.getJoint(i);
			if(joint.getName() == "") joints_lb.addItem(joint.getTypeName());
			else joints_lb.addItem(joint.getName());
		}
		joints_lb.setCurrentItem(-1);
		
		// select first joint
		if(joints_lb.getNumItems()) {
			remove_b.setEnabled(1);
			joints_lb.setCurrentItem(0);
		} else {
			remove_b.setEnabled(0);
			update_view();
		}
	}
	
	/*
	 */
	void update_dynamic() {
		
		run_function("update_dynamic");
		
		if(joint != NULL && parameters_tb.getCurrentTab() == tab) {
			joint.renderVisualizer(vec4_one);
		}
	}
	
	/*
	 */
	void add_clicked() {
		
		Body body_1;
		if(dialogBody(TR("Select body"),body,body_1) == 0) return;
		
		save_state();
		
		// get bodies
		Body b0,b1;
		if(body.getType() == BODY_CLOTH) {
			b0 = body_1;
			b1 = body;
		} else {
			b0 = body;
			b1 = body_1;
		}
		
		// create joint
		int type = joint_cb.getCurrentItem();
		if(type == JOINT_FIXED) class_remove(new JointFixed(b0,b1));
		else if(type == JOINT_BALL) class_remove(new JointBall(b0,b1));
		else if(type == JOINT_HINGE) class_remove(new JointHinge(b0,b1));
		else if(type == JOINT_PRISMATIC) class_remove(new JointPrismatic(b0,b1));
		else if(type == JOINT_CYLINDRICAL) class_remove(new JointCylindrical(b0,b1));
		else if(type == JOINT_SUSPENSION) class_remove(new JointSuspension(b0,b1));
		else if(type == JOINT_WHEEL) class_remove(new JointWheel(b0,b1));
		else if(type == JOINT_PARTICLES) class_remove(new JointParticles(b0,b1));
		else if(type == JOINT_PATH) class_remove(new JointPath(b0,b1));
		else throw("Nodes::Object::Joint::add_clicked(): unknown joint type %d\n",type);
		
		// shutdown last joint
		run_function("shutdown");
		
		update();
		
		joints_lb.setCurrentItem(joints_lb.getNumItems() - 1);
	}
	
	void remove_clicked() {
		
		if(dialogMessageYesNo(TR("Confirm"),format(TR("Remove %s joint from the %s?"),joint.getTypeName(),body.getTypeName()))) {
			
			save_state();
			
			// shutdown last joint
			run_function("shutdown");
			
			// delete joint
			body.removeJoint(joint);
			delete class_append(joint);
			
			update();
		}
	}
	
	/*
	 */
	void joints_changed() {
		
		// shutdown last joint
		run_function("shutdown");
		
		// select joint
		num = joints_lb.getCurrentItem();
		if(body != NULL && num >= 0 && num < body.getNumJoints()) {
			remove_b.setEnabled(1);
			joint = body.getJoint(num);
			type = joint.getType();
			run_function("init");
			run_function("update");
		}
		// null joint
		else {
			remove_b.setEnabled(0);
			joint = NULL;
			type = -1;
			update_view();
		}
		if(focus != NULL) {
			focus.setFocus();
			focus = NULL;
		}
	}
	
	/*
	 */
	CLICKED_CALLBACK(enabled,joint,Enabled,object)
	CLICKED_CALLBACK(collision,joint,Collision,object)
	
	void name_pressed() {
		
		if(joint == NULL) return;
		
		NodesState_ s = new NodesState_();
		if(name_el.getText() != "") {
			if(joint.getName() == "" || joint.getName() != name_el.getText()) s.init_joint(nodes,object);
			joints_lb.setItemText(num,name_el.getText());
			joint.setName(name_el.getText());
		} else {
			if(joint.getName() != "") s.init_joint(nodes,object);
			joints_lb.setItemText(num,joint.getTypeName());
			joint.setName("");
		}
		save_state(s);
	}
	
	/*
	 */
	PRESSED_CALLBACK_SLIDER_INT(num_iterations,joint,NumIterations,object,joint)
	PRESSED_CALLBACK_FLOAT(max_force,joint,MaxForce,object,joint)
	PRESSED_CALLBACK_FLOAT(max_torque,joint,MaxTorque,object,joint)
	PRESSED_CALLBACK_SLIDER(linear_restitution,joint,LinearRestitution,object,joint)
	PRESSED_CALLBACK_SLIDER(angular_restitution,joint,AngularRestitution,object,joint)
	PRESSED_CALLBACK_SLIDER(linear_softness,joint,LinearSoftness,object,joint)
	PRESSED_CALLBACK_SLIDER(angular_softness,joint,AngularSoftness,object,joint)
	
	/*
	 */
	void fix_0_clicked(int skip_state = 0) {
		Body b0 = joint.getBody0();
		Body b1 = joint.getBody1();
		vec3 a0 = b0.getTransform() * joint.getAnchor0();
		vec3 a1 = b1.getTransform() * joint.getAnchor1();
		vec3 old_anchor0 = joint.getAnchor0();
		vec3 old_anchor1 = joint.getAnchor1();
		
		NodesState_ s = new NodesState_();
		skip_state = int(skip_state);
		if(skip_state == 0) s.init_joint(nodes,object);
		
		if(reverse) joint.setAnchor1(inverse(b1.getTransform()) * a0);
		else joint.setAnchor0(inverse(b0.getTransform()) * a1);
		if(reverse) setEditLineVec3(anchor_0_el,joint.getAnchor1());
		else setEditLineVec3(anchor_0_el,joint.getAnchor0());
		
		if(skip_state == 0 && (compare(old_anchor0,joint.getAnchor0(),STATE_EPSILON) == 0 || compare(old_anchor1,joint.getAnchor1(),STATE_EPSILON) == 0)) save_state(s);
		else delete s;
	}
	
	void fix_1_clicked(int skip_state = 0) {
		Body b0 = joint.getBody0();
		Body b1 = joint.getBody1();
		vec3 a0 = b0.getTransform() * joint.getAnchor0();
		vec3 a1 = b1.getTransform() * joint.getAnchor1();
		vec3 old_anchor0 = joint.getAnchor0();
		vec3 old_anchor1 = joint.getAnchor1();
		
		NodesState_ s = new NodesState_();
		skip_state = int(skip_state);
		if(skip_state == 0) s.init_joint(nodes,object);
		
		if(reverse) joint.setAnchor0(inverse(b0.getTransform()) * a1);
		else joint.setAnchor1(inverse(b1.getTransform()) * a0);
		if(reverse) setEditLineVec3(anchor_1_el,joint.getAnchor0());
		else setEditLineVec3(anchor_1_el,joint.getAnchor1());
		
		if(skip_state == 0 && (compare(old_anchor0,joint.getAnchor0(),STATE_EPSILON) == 0 || compare(old_anchor1,joint.getAnchor1(),STATE_EPSILON) == 0)) save_state(s);
		else delete s;
	}
	
	void fix_01_clicked() {
		Body b0 = joint.getBody0();
		Body b1 = joint.getBody1();
		vec3 a0 = b0.getTransform() * joint.getAnchor0();
		vec3 a1 = b1.getTransform() * joint.getAnchor1();
		vec3 a = (a0 + a1) / 2.0f;
		vec3 old_anchor0 = joint.getAnchor0();
		vec3 old_anchor1 = joint.getAnchor1();
		
		NodesState_ s = new NodesState_();
		s.init_joint(nodes,object);
		
		joint.setAnchor1(inverse(b1.getTransform()) * a);
		joint.setAnchor0(inverse(b0.getTransform()) * a);
		if(reverse) {
			setEditLineVec3(anchor_0_el,joint.getAnchor1());
			setEditLineVec3(anchor_1_el,joint.getAnchor0());
		} else {
			setEditLineVec3(anchor_0_el,joint.getAnchor0());
			setEditLineVec3(anchor_1_el,joint.getAnchor1());
		}
		if(compare(old_anchor0,joint.getAnchor0(),STATE_EPSILON) == 0 || compare(old_anchor1,joint.getAnchor1(),STATE_EPSILON) == 0) {
			save_state(s);
		}
	}
	
	void anchor_0_pressed() {
		
		if(joint == NULL) return;
		
		vec3 old_anchor0 = joint.getAnchor0();
		vec3 old_anchor1 = joint.getAnchor1();
		NodesState_ s = new NodesState_();
		s.init_joint(nodes,object);
		if(reverse) {
			joint.setAnchor1(Vec3(getEditLineVec3(anchor_0_el)));
			setEditLineVec3(anchor_0_el,joint.getAnchor1());
		} else {
			joint.setAnchor0(Vec3(getEditLineVec3(anchor_0_el)));
			setEditLineVec3(anchor_0_el,joint.getAnchor0());
		}
		// manage state saving
		int changed = 0;
		if(compare(old_anchor0,joint.getAnchor0(),STATE_EPSILON) == 0 || compare(old_anchor1,joint.getAnchor1(),STATE_EPSILON) == 0) {
			save_state(s);
			changed = 1;
		} else {
			delete s;
		}
		if(bind_b.isToggled()) fix_1_clicked(changed);
	}
	
	void anchor_1_pressed() {
		
		if(joint == NULL) return;
		
		vec3 old_anchor0 = joint.getAnchor0();
		vec3 old_anchor1 = joint.getAnchor1();
		NodesState_ s = new NodesState_();
		s.init_joint(nodes,object);
		if(reverse) {
			joint.setAnchor0(Vec3(getEditLineVec3(anchor_1_el)));
			setEditLineVec3(anchor_1_el,joint.getAnchor0());
		} else {
			joint.setAnchor1(Vec3(getEditLineVec3(anchor_1_el)));
			setEditLineVec3(anchor_1_el,joint.getAnchor1());
		}
		// manage state saving
		int changed = 0;
		if(compare(old_anchor0,joint.getAnchor0(),STATE_EPSILON) == 0 || compare(old_anchor1,joint.getAnchor1(),STATE_EPSILON) == 0) {
			save_state(s);
			changed = 1;
		} else {
			delete s;
		}
		if(bind_b.isToggled()) fix_0_clicked(changed);
	}
	
	/**************************************************************************\
	*
	* JointFixed
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Fixed {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetEditLine rotation_0_el[3];
		WidgetEditLine rotation_1_el[3];
		
		JointFixed joint;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			joint = joint_cast(body.getJoint(num));
			
			if(reverse) {
				setEditLineVec3(rotation_0_el,decomposeRotation(joint.getRotation1()));
				setEditLineVec3(rotation_1_el,decomposeRotation(joint.getRotation0()));
			} else {
				setEditLineVec3(rotation_0_el,decomposeRotation(joint.getRotation0()));
				setEditLineVec3(rotation_1_el,decomposeRotation(joint.getRotation1()));
			}
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		void rotation_0_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			mat4 old_rotation0 = joint.getRotation0();
			mat4 old_rotation1 = joint.getRotation1();
			if(reverse) {
				joint.setRotation1(composeRotation(getEditLineVec3(rotation_0_el)));
				setEditLineVec3(rotation_0_el,decomposeRotation(joint.getRotation1()));
			} else {
				joint.setRotation0(composeRotation(getEditLineVec3(rotation_0_el)));
				setEditLineVec3(rotation_0_el,decomposeRotation(joint.getRotation0()));
			}
			if(compare(old_rotation0,joint.getRotation0(),STATE_EPSILON) == 0 || compare(old_rotation1,joint.getRotation1(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
		void rotation_1_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			mat4 old_rotation0 = joint.getRotation0();
			mat4 old_rotation1 = joint.getRotation1();
			if(reverse) {
				joint.setRotation0(composeRotation(getEditLineVec3(rotation_1_el)));
				setEditLineVec3(rotation_1_el,decomposeRotation(joint.getRotation0()));
			} else {
				joint.setRotation1(composeRotation(getEditLineVec3(rotation_1_el)));
				setEditLineVec3(rotation_1_el,decomposeRotation(joint.getRotation1()));
			}
			if(compare(old_rotation0,joint.getRotation0(),STATE_EPSILON) == 0 || compare(old_rotation1,joint.getRotation1(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
	} /* namespace Fixed */
	
	/**************************************************************************\
	*
	* JointBall
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Ball {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetEditLine axis_0_el[3];
		WidgetEditLine axis_1_el[3];
		WidgetEditLine angular_damping_el;
		WidgetSlider angular_damping_sl;
		WidgetEditLine angular_limit_angle_el;
		WidgetEditLine angular_limit_from_el;
		WidgetEditLine angular_limit_to_el;
		
		JointBall joint;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			joint = joint_cast(body.getJoint(num));
			
			if(reverse) {
				setEditLineVec3(axis_0_el,joint.getAxis1());
				setEditLineVec3(axis_1_el,joint.getAxis0());
			} else {
				setEditLineVec3(axis_0_el,joint.getAxis0());
				setEditLineVec3(axis_1_el,joint.getAxis1());
			}
			
			angular_damping_el.setText(editorFormat(joint.getAngularDamping()));
			angular_limit_angle_el.setText(editorFormat(joint.getAngularLimitAngle()));
			angular_limit_from_el.setText(editorFormat(joint.getAngularLimitFrom()));
			angular_limit_to_el.setText(editorFormat(joint.getAngularLimitTo()));
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		void axis_0_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			vec3 old_axis0 = joint.getAxis0();
			vec3 old_axis1 = joint.getAxis1();
			if(reverse) {
				joint.setAxis1(getEditLineVec3(axis_0_el));
				setEditLineVec3(axis_0_el,joint.getAxis1());
			} else {
				joint.setAxis0(getEditLineVec3(axis_0_el));
				setEditLineVec3(axis_0_el,joint.getAxis0());
			}
			if(compare(old_axis0,joint.getAxis0(),STATE_EPSILON) == 0 || compare(old_axis1,joint.getAxis1(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
		void axis_1_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			vec3 old_axis0 = joint.getAxis0();
			vec3 old_axis1 = joint.getAxis1();
			if(reverse) {
				joint.setAxis0(getEditLineVec3(axis_1_el));
				setEditLineVec3(axis_1_el,joint.getAxis0());
			} else {
				joint.setAxis1(getEditLineVec3(axis_1_el));
				setEditLineVec3(axis_1_el,joint.getAxis1());
			}
			if(compare(old_axis0,joint.getAxis0(),STATE_EPSILON) == 0 || compare(old_axis1,joint.getAxis1(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
		/*
		 */
		PRESSED_CALLBACK_SLIDER(angular_damping,joint,AngularDamping,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_limit_angle,joint,AngularLimitAngle,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_limit_from,joint,AngularLimitFrom,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_limit_to,joint,AngularLimitTo,object,joint)
		
	} /* namespace Ball */
	
	/**************************************************************************\
	*
	* JointHinge
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Hinge {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetEditLine axis_0_el[3];
		WidgetEditLine axis_1_el[3];
		WidgetEditLine angular_damping_el;
		WidgetSlider angular_damping_sl;
		WidgetEditLine angular_limit_from_el;
		WidgetEditLine angular_limit_to_el;
		WidgetEditLine angular_velocity_el;
		WidgetEditLine angular_torque_el;
		WidgetEditLine angular_angle_el;
		WidgetEditLine angular_spring_el;
		WidgetEditLine current_angular_velocity_el;
		WidgetEditLine current_angular_angle_el;
		
		JointHinge joint;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			joint = joint_cast(body.getJoint(num));
			
			if(reverse) {
				setEditLineVec3(axis_0_el,joint.getAxis1());
				setEditLineVec3(axis_1_el,joint.getAxis0());
			} else {
				setEditLineVec3(axis_0_el,joint.getAxis0());
				setEditLineVec3(axis_1_el,joint.getAxis1());
			}
			
			angular_damping_el.setText(editorFormat(joint.getAngularDamping()));
			angular_limit_from_el.setText(editorFormat(joint.getAngularLimitFrom()));
			angular_limit_to_el.setText(editorFormat(joint.getAngularLimitTo()));
			angular_velocity_el.setText(editorFormat(joint.getAngularVelocity()));
			angular_torque_el.setText(editorFormat(joint.getAngularTorque()));
			angular_angle_el.setText(editorFormat(joint.getAngularAngle()));
			angular_spring_el.setText(editorFormat(joint.getAngularSpring()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			current_angular_velocity_el.setText(editorFormat(joint.getCurrentAngularVelocity()));
			current_angular_angle_el.setText(editorFormat(joint.getCurrentAngularAngle()));
		}
		
		/*
		 */
		void axis_0_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			vec3 old_axis0 = joint.getAxis0();
			vec3 old_axis1 = joint.getAxis1();
			if(reverse) {
				joint.setAxis1(getEditLineVec3(axis_0_el));
				setEditLineVec3(axis_0_el,joint.getAxis1());
			} else {
				joint.setAxis0(getEditLineVec3(axis_0_el));
				setEditLineVec3(axis_0_el,joint.getAxis0());
			}
			if(compare(old_axis0,joint.getAxis0(),STATE_EPSILON) == 0 || compare(old_axis1,joint.getAxis1(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
		void axis_1_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			vec3 old_axis0 = joint.getAxis0();
			vec3 old_axis1 = joint.getAxis1();
			if(reverse) {
				joint.setAxis0(getEditLineVec3(axis_1_el));
				setEditLineVec3(axis_1_el,joint.getAxis0());
			} else {
				joint.setAxis1(getEditLineVec3(axis_1_el));
				setEditLineVec3(axis_1_el,joint.getAxis1());
			}
			if(compare(old_axis0,joint.getAxis0(),STATE_EPSILON) == 0 || compare(old_axis1,joint.getAxis1(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
		/*
		 */
		PRESSED_CALLBACK_SLIDER(angular_damping,joint,AngularDamping,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_limit_from,joint,AngularLimitFrom,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_limit_to,joint,AngularLimitTo,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_velocity,joint,AngularVelocity,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_torque,joint,AngularTorque,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_angle,joint,AngularAngle,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_spring,joint,AngularSpring,object,joint)
		
	} /* namespace Hinge */
	
	/**************************************************************************\
	*
	* JointPrismatic
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Prismatic {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetEditLine axis_0_el[3];
		WidgetEditLine rotation_0_el[3];
		WidgetEditLine rotation_1_el[3];
		WidgetEditLine linear_damping_el;
		WidgetSlider linear_damping_sl;
		WidgetEditLine linear_limit_from_el;
		WidgetEditLine linear_limit_to_el;
		WidgetEditLine linear_velocity_el;
		WidgetEditLine linear_force_el;
		WidgetEditLine linear_distance_el;
		WidgetEditLine linear_spring_el;
		WidgetEditLine current_linear_velocity_el;
		WidgetEditLine current_linear_distance_el;
		WidgetLabel axis_0_l;
		
		JointPrismatic joint;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			joint = joint_cast(body.getJoint(num));
			
			if(reverse) {
				axis_0_l.setText(TR("Axis 1:"));
				setEditLineVec3(axis_0_el,joint.getAxis0());
				setEditLineVec3(rotation_0_el,decomposeRotation(joint.getRotation1()));
				setEditLineVec3(rotation_1_el,decomposeRotation(joint.getRotation0()));
			} else {
				axis_0_l.setText(TR("Axis 0:"));
				setEditLineVec3(axis_0_el,joint.getAxis0());
				setEditLineVec3(rotation_0_el,decomposeRotation(joint.getRotation0()));
				setEditLineVec3(rotation_1_el,decomposeRotation(joint.getRotation1()));
			}
			
			linear_damping_el.setText(editorFormat(joint.getLinearDamping()));
			linear_limit_from_el.setText(editorFormat(joint.getLinearLimitFrom()));
			linear_limit_to_el.setText(editorFormat(joint.getLinearLimitTo()));
			linear_velocity_el.setText(editorFormat(joint.getLinearVelocity()));
			linear_force_el.setText(editorFormat(joint.getLinearForce()));
			linear_distance_el.setText(editorFormat(joint.getLinearDistance()));
			linear_spring_el.setText(editorFormat(joint.getLinearSpring()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			current_linear_velocity_el.setText(editorFormat(joint.getCurrentLinearVelocity()));
			current_linear_distance_el.setText(editorFormat(joint.getCurrentLinearDistance()));
		}
		
		/*
		 */
		void rotation_0_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			mat4 old_rotation0 = joint.getRotation0();
			mat4 old_rotation1 = joint.getRotation1();
			if(reverse) {
				joint.setRotation1(composeRotation(getEditLineVec3(rotation_0_el)));
				setEditLineVec3(rotation_0_el,decomposeRotation(joint.getRotation1()));
			} else {
				joint.setRotation0(composeRotation(getEditLineVec3(rotation_0_el)));
				setEditLineVec3(rotation_0_el,decomposeRotation(joint.getRotation0()));
			}
			if(compare(old_rotation0,joint.getRotation0(),STATE_EPSILON) == 0 || compare(old_rotation1,joint.getRotation1(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
		void rotation_1_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			mat4 old_rotation0 = joint.getRotation0();
			mat4 old_rotation1 = joint.getRotation1();
			if(reverse) {
				joint.setRotation0(composeRotation(getEditLineVec3(rotation_1_el)));
				setEditLineVec3(rotation_1_el,decomposeRotation(joint.getRotation0()));
			} else {
				joint.setRotation1(composeRotation(getEditLineVec3(rotation_1_el)));
				setEditLineVec3(rotation_1_el,decomposeRotation(joint.getRotation1()));
			}
			if(compare(old_rotation0,joint.getRotation0(),STATE_EPSILON) == 0 || compare(old_rotation1,joint.getRotation1(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(axis_0,joint,Axis0,object,joint)
		PRESSED_CALLBACK_SLIDER(linear_damping,joint,LinearDamping,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_limit_from,joint,LinearLimitFrom,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_limit_to,joint,LinearLimitTo,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_velocity,joint,LinearVelocity,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_force,joint,LinearForce,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_distance,joint,LinearDistance,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_spring,joint,LinearSpring,object,joint)
		
	} /* namespace Prismatic */
	
	/**************************************************************************\
	*
	* JointCylindrical
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Cylindrical {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetEditLine axis_0_el[3];
		WidgetEditLine axis_1_el[3];
		WidgetEditLine linear_damping_el;
		WidgetSlider linear_damping_sl;
		WidgetEditLine linear_limit_from_el;
		WidgetEditLine linear_limit_to_el;
		WidgetEditLine linear_velocity_el;
		WidgetEditLine linear_force_el;
		WidgetEditLine linear_distance_el;
		WidgetEditLine linear_spring_el;
		WidgetEditLine angular_damping_el;
		WidgetSlider angular_damping_sl;
		WidgetEditLine angular_limit_from_el;
		WidgetEditLine angular_limit_to_el;
		WidgetEditLine angular_velocity_el;
		WidgetEditLine angular_torque_el;
		WidgetEditLine angular_angle_el;
		WidgetEditLine angular_spring_el;
		WidgetEditLine current_linear_velocity_el;
		WidgetEditLine current_linear_distance_el;
		WidgetEditLine current_angular_velocity_el;
		WidgetEditLine current_angular_angle_el;
		
		JointCylindrical joint;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			joint = joint_cast(body.getJoint(num));
			
			if(reverse) {
				setEditLineVec3(axis_0_el,joint.getAxis1());
				setEditLineVec3(axis_1_el,joint.getAxis0());
			} else {
				setEditLineVec3(axis_0_el,joint.getAxis0());
				setEditLineVec3(axis_1_el,joint.getAxis1());
			}
			
			linear_damping_el.setText(editorFormat(joint.getLinearDamping()));
			linear_limit_from_el.setText(editorFormat(joint.getLinearLimitFrom()));
			linear_limit_to_el.setText(editorFormat(joint.getLinearLimitTo()));
			linear_velocity_el.setText(editorFormat(joint.getLinearVelocity()));
			linear_force_el.setText(editorFormat(joint.getLinearForce()));
			linear_distance_el.setText(editorFormat(joint.getLinearDistance()));
			linear_spring_el.setText(editorFormat(joint.getLinearSpring()));
			angular_damping_el.setText(editorFormat(joint.getAngularDamping()));
			angular_limit_from_el.setText(editorFormat(joint.getAngularLimitFrom()));
			angular_limit_to_el.setText(editorFormat(joint.getAngularLimitTo()));
			angular_velocity_el.setText(editorFormat(joint.getAngularVelocity()));
			angular_torque_el.setText(editorFormat(joint.getAngularTorque()));
			angular_angle_el.setText(editorFormat(joint.getAngularAngle()));
			angular_spring_el.setText(editorFormat(joint.getAngularSpring()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			current_linear_velocity_el.setText(editorFormat(joint.getCurrentLinearVelocity()));
			current_linear_distance_el.setText(editorFormat(joint.getCurrentLinearDistance()));
			current_angular_velocity_el.setText(editorFormat(joint.getCurrentAngularVelocity()));
			current_angular_angle_el.setText(editorFormat(joint.getCurrentAngularAngle()));
		}
		
		/*
		 */
		void axis_0_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			vec3 old_axis0 = joint.getAxis0();
			vec3 old_axis1 = joint.getAxis1();
			if(reverse) {
				joint.setAxis1(getEditLineVec3(axis_0_el));
				setEditLineVec3(axis_0_el,joint.getAxis1());
			} else {
				joint.setAxis0(getEditLineVec3(axis_0_el));
				setEditLineVec3(axis_0_el,joint.getAxis0());
			}
			if(compare(old_axis0,joint.getAxis0(),STATE_EPSILON) == 0 || compare(old_axis1,joint.getAxis1(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
		void axis_1_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			vec3 old_axis0 = joint.getAxis0();
			vec3 old_axis1 = joint.getAxis1();
			if(reverse) {
				joint.setAxis0(getEditLineVec3(axis_1_el));
				setEditLineVec3(axis_1_el,joint.getAxis0());
			} else {
				joint.setAxis1(getEditLineVec3(axis_1_el));
				setEditLineVec3(axis_1_el,joint.getAxis1());
			}
			if(compare(old_axis0,joint.getAxis0(),STATE_EPSILON) == 0 || compare(old_axis1,joint.getAxis1(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
		/*
		 */
		PRESSED_CALLBACK_SLIDER(linear_damping,joint,LinearDamping,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_limit_from,joint,LinearLimitFrom,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_limit_to,joint,LinearLimitTo,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_velocity,joint,LinearVelocity,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_force,joint,LinearForce,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_distance,joint,LinearDistance,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_spring,joint,LinearSpring,object,joint)
		PRESSED_CALLBACK_SLIDER(angular_damping,joint,AngularDamping,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_limit_from,joint,AngularLimitFrom,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_limit_to,joint,AngularLimitTo,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_velocity,joint,AngularVelocity,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_torque,joint,AngularTorque,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_angle,joint,AngularAngle,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_spring,joint,AngularSpring,object,joint)
		
	} /* namespace Cylindrical */
	
	/**************************************************************************\
	*
	* JointSuspension
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Suspension {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetEditLine axis_00_el[3];
		WidgetEditLine axis_10_el[3];
		WidgetEditLine axis_11_el[3];
		WidgetEditLine linear_damping_el;
		WidgetSlider linear_damping_sl;
		WidgetEditLine linear_limit_from_el;
		WidgetEditLine linear_limit_to_el;
		WidgetEditLine linear_distance_el;
		WidgetEditLine linear_spring_el;
		WidgetEditLine angular_damping_el;
		WidgetSlider angular_damping_sl;
		WidgetEditLine angular_velocity_el;
		WidgetEditLine angular_torque_el;
		WidgetEditLine current_linear_distance_el;
		WidgetEditLine current_angular_velocity_el;
		WidgetLabel axis_00_l;
		
		JointSuspension joint;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			joint = joint_cast(body.getJoint(num));
			
			if(reverse) {
				axis_00_l.setText(TR("Axis 01:"));
				setEditLineVec3(axis_00_el,joint.getAxis00());
				setEditLineVec3(axis_10_el,joint.getAxis11());
				setEditLineVec3(axis_11_el,joint.getAxis10());
			} else {
				axis_00_l.setText(TR("Axis 00:"));
				setEditLineVec3(axis_00_el,joint.getAxis00());
				setEditLineVec3(axis_10_el,joint.getAxis10());
				setEditLineVec3(axis_11_el,joint.getAxis11());
			}
			
			linear_damping_el.setText(editorFormat(joint.getLinearDamping()));
			linear_limit_from_el.setText(editorFormat(joint.getLinearLimitFrom()));
			linear_limit_to_el.setText(editorFormat(joint.getLinearLimitTo()));
			linear_distance_el.setText(editorFormat(joint.getLinearDistance()));
			linear_spring_el.setText(editorFormat(joint.getLinearSpring()));
			angular_damping_el.setText(editorFormat(joint.getAngularDamping()));
			angular_velocity_el.setText(editorFormat(joint.getAngularVelocity()));
			angular_torque_el.setText(editorFormat(joint.getAngularTorque()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			current_linear_distance_el.setText(editorFormat(joint.getCurrentLinearDistance()));
			current_angular_velocity_el.setText(editorFormat(joint.getCurrentAngularVelocity()));
		}
		
		/*
		 */
		void axis_10_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			vec3 old_axis10 = joint.getAxis10();
			vec3 old_axis11 = joint.getAxis11();
			if(reverse) {
				joint.setAxis11(getEditLineVec3(axis_10_el));
				setEditLineVec3(axis_10_el,joint.getAxis11());
			} else {
				joint.setAxis10(getEditLineVec3(axis_10_el));
				setEditLineVec3(axis_10_el,joint.getAxis10());
			}
			if(compare(old_axis10,joint.getAxis10(),STATE_EPSILON) == 0 || compare(old_axis11,joint.getAxis11(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
		void axis_11_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			vec3 old_axis10 = joint.getAxis10();
			vec3 old_axis11 = joint.getAxis11();
			if(reverse) {
				joint.setAxis10(getEditLineVec3(axis_11_el));
				setEditLineVec3(axis_11_el,joint.getAxis10());
			} else {
				joint.setAxis11(getEditLineVec3(axis_11_el));
				setEditLineVec3(axis_11_el,joint.getAxis11());
			}
			if(compare(old_axis10,joint.getAxis10(),STATE_EPSILON) == 0 || compare(old_axis11,joint.getAxis11(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(axis_00,joint,Axis00,object,joint)
		PRESSED_CALLBACK_SLIDER(linear_damping,joint,LinearDamping,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_limit_from,joint,LinearLimitFrom,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_limit_to,joint,LinearLimitTo,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_distance,joint,LinearDistance,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_spring,joint,LinearSpring,object,joint)
		PRESSED_CALLBACK_SLIDER(angular_damping,joint,AngularDamping,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_velocity,joint,AngularVelocity,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_torque,joint,AngularTorque,object,joint)
		
	} /* namespace Suspension */
	
	/**************************************************************************\
	*
	* JointWheel
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Wheel {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetEditLine axis_00_el[3];
		WidgetEditLine axis_10_el[3];
		WidgetEditLine axis_11_el[3];
		WidgetEditLine linear_damping_el;
		WidgetSlider linear_damping_sl;
		WidgetEditLine linear_limit_from_el;
		WidgetEditLine linear_limit_to_el;
		WidgetEditLine linear_distance_el;
		WidgetEditLine linear_spring_el;
		WidgetEditLine angular_damping_el;
		WidgetSlider angular_damping_sl;
		WidgetEditLine angular_velocity_el;
		WidgetEditLine angular_torque_el;
		WidgetEditLine current_linear_distance_el;
		WidgetEditLine current_angular_velocity_el;
		WidgetEditLine tangent_angle_el;
		WidgetEditLine tangent_friction_el;
		WidgetEditLine binormal_angle_el;
		WidgetEditLine binormal_friction_el;
		WidgetEditLine wheel_mass_el;
		WidgetEditLine wheel_radius_el;
		WidgetEditLine wheel_width_el;
		WidgetLabel axis_00_l;
		
		JointWheel joint;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			joint = joint_cast(body.getJoint(num));
			
			if(reverse) {
				axis_00_l.setText(TR("Axis 01:"));
				setEditLineVec3(axis_00_el,joint.getAxis00());
				setEditLineVec3(axis_10_el,joint.getAxis11());
				setEditLineVec3(axis_11_el,joint.getAxis10());
			} else {
				axis_00_l.setText(TR("Axis 00:"));
				setEditLineVec3(axis_00_el,joint.getAxis00());
				setEditLineVec3(axis_10_el,joint.getAxis10());
				setEditLineVec3(axis_11_el,joint.getAxis11());
			}
			
			linear_damping_el.setText(editorFormat(joint.getLinearDamping()));
			linear_limit_from_el.setText(editorFormat(joint.getLinearLimitFrom()));
			linear_limit_to_el.setText(editorFormat(joint.getLinearLimitTo()));
			linear_distance_el.setText(editorFormat(joint.getLinearDistance()));
			linear_spring_el.setText(editorFormat(joint.getLinearSpring()));
			angular_damping_el.setText(editorFormat(joint.getAngularDamping()));
			angular_velocity_el.setText(editorFormat(joint.getAngularVelocity()));
			angular_torque_el.setText(editorFormat(joint.getAngularTorque()));
			tangent_angle_el.setText(editorFormat(joint.getTangentAngle()));
			tangent_friction_el.setText(editorFormat(joint.getTangentFriction()));
			binormal_angle_el.setText(editorFormat(joint.getBinormalAngle()));
			binormal_friction_el.setText(editorFormat(joint.getBinormalFriction()));
			wheel_mass_el.setText(editorFormat(joint.getWheelMass()));
			wheel_radius_el.setText(editorFormat(joint.getWheelRadius()));
			wheel_width_el.setText(editorFormat(joint.getWheelWidth()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			current_linear_distance_el.setText(editorFormat(joint.getCurrentLinearDistance()));
			current_angular_velocity_el.setText(editorFormat(joint.getCurrentAngularVelocity()));
		}
		
		/*
		 */
		void axis_10_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			vec3 old_axis10 = joint.getAxis10();
			vec3 old_axis11 = joint.getAxis11();
			if(reverse) {
				joint.setAxis11(getEditLineVec3(axis_10_el));
				setEditLineVec3(axis_10_el,joint.getAxis11());
			} else {
				joint.setAxis10(getEditLineVec3(axis_10_el));
				setEditLineVec3(axis_10_el,joint.getAxis10());
			}
			if(compare(old_axis10,joint.getAxis10(),STATE_EPSILON) == 0 || compare(old_axis11,joint.getAxis11(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
		void axis_11_pressed() {
			
			if(joint == NULL) return;
			
			NodesState_ s = new NodesState_();
			s.init_joint(nodes,object);
			vec3 old_axis10 = joint.getAxis10();
			vec3 old_axis11 = joint.getAxis11();
			if(reverse) {
				joint.setAxis10(getEditLineVec3(axis_11_el));
				setEditLineVec3(axis_11_el,joint.getAxis10());
			} else {
				joint.setAxis11(getEditLineVec3(axis_11_el));
				setEditLineVec3(axis_11_el,joint.getAxis11());
			}
			if(compare(old_axis10,joint.getAxis10(),STATE_EPSILON) == 0 || compare(old_axis11,joint.getAxis11(),STATE_EPSILON) == 0) save_state(s);
			else delete s;
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(axis_00,joint,Axis00,object,joint)
		PRESSED_CALLBACK_SLIDER(linear_damping,joint,LinearDamping,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_limit_from,joint,LinearLimitFrom,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_limit_to,joint,LinearLimitTo,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_distance,joint,LinearDistance,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_spring,joint,LinearSpring,object,joint)
		PRESSED_CALLBACK_SLIDER(angular_damping,joint,AngularDamping,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_velocity,joint,AngularVelocity,object,joint)
		PRESSED_CALLBACK_FLOAT(angular_torque,joint,AngularTorque,object,joint)
		PRESSED_CALLBACK_FLOAT(tangent_angle,joint,TangentAngle,object,joint)
		PRESSED_CALLBACK_FLOAT(tangent_friction,joint,TangentFriction,object,joint)
		PRESSED_CALLBACK_FLOAT(binormal_angle,joint,BinormalAngle,object,joint)
		PRESSED_CALLBACK_FLOAT(binormal_friction,joint,BinormalFriction,object,joint)
		PRESSED_CALLBACK_FLOAT(wheel_mass,joint,WheelMass,object,joint)
		PRESSED_CALLBACK_FLOAT(wheel_radius,joint,WheelRadius,object,joint)
		PRESSED_CALLBACK_FLOAT(wheel_width,joint,WheelWidth,object,joint)
		
	} /* namespace Wheel */
	
	/**************************************************************************\
	*
	* JointParticles
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Particles {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetEditLine size_el[3];
		WidgetEditLine threshold_el;
		
		JointParticles joint;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			joint = joint_cast(body.getJoint(num));
			
			setEditLineVec3(size_el,joint.getSize());
			threshold_el.setText(editorFormat(joint.getThreshold()));
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(size,joint,Size,object,joint)
		PRESSED_CALLBACK_FLOAT(threshold,joint,Threshold,object,joint)
		
	} /* namespace Particles */
	
	/**************************************************************************\
	*
	* JointPath
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Path {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetEditLine linear_damping_el;
		WidgetSlider linear_damping_sl;
		WidgetEditLine linear_velocity_el;
		WidgetEditLine linear_force_el;
		WidgetEditLine current_linear_velocity_el;
		
		JointPath joint;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			update_view();
			
			joint = joint_cast(body.getJoint(num));
			
			linear_damping_el.setText(editorFormat(joint.getLinearDamping()));
			linear_velocity_el.setText(editorFormat(joint.getLinearVelocity()));
			linear_force_el.setText(editorFormat(joint.getLinearForce()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			current_linear_velocity_el.setText(editorFormat(joint.getCurrentLinearVelocity()));
		}
		
		/*
		 */
		PRESSED_CALLBACK_SLIDER(linear_damping,joint,LinearDamping,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_velocity,joint,LinearVelocity,object,joint)
		PRESSED_CALLBACK_FLOAT(linear_force,joint,LinearForce,object,joint)
		
	} /* namespace Path */
	
} /* namespace Joint */
