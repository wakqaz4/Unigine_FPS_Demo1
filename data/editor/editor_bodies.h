/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_bodies.h
 * Desc:    Unigine editor
 * Version: 1.32
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
namespace Body {
	
	/*
	 */
	WidgetVBox main_vb;					// main vbox
	
	WidgetVBox parameters_vb;			// body container
	WidgetComboBox type_cb;				// body type
	WidgetCheckBox enabled_cb;			// enabled flag
	WidgetCheckBox gravity_cb;			// gravity flag
	WidgetEditLine name_el;				// body name
	WidgetEditLine physical_mask_el;	// physical mask
	WidgetVBox dummy_vb;				// dummy container
	
	Body body;							// base body
	int type = -1;						// body type
	
	string type_prefix[] = (
		BODY_DUMMY		: "Dummy::",
		BODY_RIGID		: "Rigid::",
		BODY_RAGDOLL	: "RagDoll::",
		BODY_FRACTURE	: "Fracture::",
		BODY_ROPE		: "Rope::",
		BODY_CLOTH		: "Cloth::",
		BODY_WATER		: "Water::",
		BODY_PATH		: "Path::",
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
		
		// add body tab
		parameters_tb.addTab(TR("Body"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
	}
	
	void shutdown() {
		
		body = NULL;
		
		// shutdown last body
		run_function("shutdown");
		
		// shutdown shapes and joints in back order
		Nodes::Object::Joint::shutdown();
		Nodes::Object::Shape::shutdown();
	}
	
	/*
	 */
	void update() {
		
		body = object.getBody();
		Nodes::Object::Shape::body = body;
		Nodes::Object::Joint::body = body;
		
		// clear view
		if(body == NULL) {
			
			type = -1;
			
			// body type
			type_cb.setCallbackEnabled(GUI_CHANGED,0);
			type_cb.setCurrentItem(0);
			type_cb.setCallbackEnabled(GUI_CHANGED,1);
			
			// shutdown body
			shutdown();
		}
		// update view
		else {
			
			type = body.getType();
			
			// initialize body
			run_function("init");
			run_function("update");
			
			// initialize shapes and joint
			Nodes::Object::Shape::init();
			Nodes::Object::Joint::init();
			Nodes::Object::Shape::update();
			Nodes::Object::Joint::update();
			
			// body type
			type_cb.setCallbackEnabled(GUI_CHANGED,0);
			type_cb.setCurrentItem(body.getType() + 1);
			type_cb.setCallbackEnabled(GUI_CHANGED,1);
			
			// parameters
			if(enabled_cb != NULL) enabled_cb.setCallbackEnabled(GUI_CLICKED,0);
			if(gravity_cb != NULL) enabled_cb.setCallbackEnabled(GUI_CLICKED,0);
			
			if(enabled_cb != NULL) enabled_cb.setChecked(body.isEnabled());
			if(gravity_cb != NULL) gravity_cb.setChecked(body.isGravity());
			name_el.setText(body.getName());
			
			if(enabled_cb != NULL) enabled_cb.setCallbackEnabled(GUI_CLICKED,1);
			if(gravity_cb != NULL) enabled_cb.setCallbackEnabled(GUI_CLICKED,1);
			
			// physical mask
			if(physical_mask_el != NULL) physical_mask_el.setText(editorFormatMask(body.getPhysicalMask()));
		}
	}
	
	/*
	 */
	void update_dynamic() {
		
		if(body != NULL) {
			
			run_function("update_dynamic");
			Nodes::Object::Shape::update_dynamic();
			Nodes::Object::Joint::update_dynamic();
		}
	}
	
	/*
	 */
	int create() {
		int result;
		
		int type = type_cb.getCurrentItem() - 1;
		
		Body old_body = object.getBody();
		
		// create body
		if(type == -1) body = NULL;
		else if(type == BODY_DUMMY) body = new BodyDummy();
		else if(type == BODY_RIGID) body = new BodyRigid();
		else if(type == BODY_RAGDOLL) body = new BodyRagDoll();
		else if(type == BODY_FRACTURE) body = new BodyFracture();
		else if(type == BODY_ROPE) body = new BodyRope();
		else if(type == BODY_CLOTH) body = new BodyCloth();
		else if(type == BODY_WATER) body = new BodyWater();
		else if(type == BODY_PATH) body = new BodyPath();
		else throw("Nodes::Object::Body::create(): unknown body type %d\n",type);
		
		// set body to object
		object.setBody(NULL);
		if(body != NULL) object.setBody(class_remove(body));
		
		// check body
		if(body != object.getBody()) {
			dialogMessageOk(TR("Error"),format(TR("Can't set %s body to %s object."),body.getTypeName(),object.getTypeName()));
			delete class_append(body);
			body = old_body;
			object.setBody(old_body);
			result = false;
		} else {
			if(old_body != NULL) delete class_append(old_body);
			result = true;
		}
		
		// update
		update();
		
		return result;
	}
	
	/*
	 */
	void type_changed() {
		
		int type = type_cb.getCurrentItem() - 1;
		
		if(body == NULL || (type != body.getType() && dialogMessageYesNo(TR("Confirm"),format(TR("Remove %s from the %s?"),body.getTypeName(),object.getTypeName())))) {
			NodesState_ s = new NodesState_();
			
			s.init_body(nodes,object);
			
			// shutdown body
			shutdown();
			
			if(create()) save_state(s);
		} else {
			type_cb.setCallbackEnabled(GUI_CHANGED,0);
			type_cb.setCurrentItem(body.getType() + 1);
			type_cb.setCallbackEnabled(GUI_CHANGED,1);
		}
	}
	
	Body get_body() {
		return body;
	}
	
	/*
	 */
	CLICKED_CALLBACK(enabled,body,Enabled,object)
	CLICKED_CALLBACK(gravity,body,Gravity,object)
	PRESSED_CALLBACK(name,body,Name,object,string,modify)
	PRESSED_CALLBACK_MASK(physical_mask,body,PhysicalMask,object)
	CLICKED_CALLBACK_MASK(physical_mask,body,PhysicalMask,object,TR("Body physical mask"),functionid(physical_mask_changed))
	
	/*
	 */
	void physical_mask_changed(int mask) {
		body.setPhysicalMask(mask);
		physical_mask_el.setText(editorFormatMask(body.getPhysicalMask()));
	}
	
	/**************************************************************************\
	*
	* BodyDummy
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Dummy {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetCheckBox enabled_cb;
		WidgetCheckBox gravity_cb;
		WidgetEditLine name_el;
		WidgetEditLine physical_mask_el;
		
		BodyDummy body;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			body = NULL;
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			Nodes::Object::Body::enabled_cb = enabled_cb;
			Nodes::Object::Body::gravity_cb = gravity_cb;
			Nodes::Object::Body::name_el = name_el;
			Nodes::Object::Body::physical_mask_el = physical_mask_el;
			
			body = body_cast(object.getBody());
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
	} /* namespace Dummy */
	
	/**************************************************************************\
	*
	* BodyRigid
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Rigid {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetCheckBox enabled_cb;
		WidgetCheckBox gravity_cb;
		WidgetCheckBox freezable_cb;
		WidgetCheckBox shape_based_cb;
		WidgetEditLine name_el;
		WidgetEditLine physical_mask_el;
		WidgetEditLine mass_el;
		WidgetEditLine inertia_0_el[3];
		WidgetEditLine inertia_1_el[3];
		WidgetEditLine inertia_2_el[3];
		WidgetEditLine center_of_mass_el[3];
		WidgetEditLine linear_scale_el[3];
		WidgetEditLine angular_scale_el[3];
		WidgetEditLine linear_damping_el;
		WidgetSlider linear_damping_sl;
		WidgetEditLine angular_damping_el;
		WidgetSlider angular_damping_sl;
		WidgetEditLine max_linear_velocity_el;
		WidgetEditLine max_angular_velocity_el;
		WidgetEditLine frozen_linear_velocity_el;
		WidgetEditLine frozen_angular_velocity_el;
		
		BodyRigid body;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			body = NULL;
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void set_inertia(mat4 inertia) {
			setEditLineVec3(inertia_0_el,inertia.m00m01m02);
			setEditLineVec3(inertia_1_el,inertia.m10m11m12);
			setEditLineVec3(inertia_2_el,inertia.m20m21m22);
		}
		
		mat4 get_inertia() {
			mat4 inertia;
			inertia.m00m01m02 = getEditLineVec3(inertia_0_el);
			inertia.m10m11m12 = getEditLineVec3(inertia_1_el);
			inertia.m20m21m22 = getEditLineVec3(inertia_2_el);
			return inertia;
		}
		
		/*
		 */
		void update() {
			
			Nodes::Object::Body::enabled_cb = enabled_cb;
			Nodes::Object::Body::gravity_cb = gravity_cb;
			Nodes::Object::Body::name_el = name_el;
			Nodes::Object::Body::physical_mask_el = physical_mask_el;
			
			body = body_cast(object.getBody());
			
			setCallbackEnabled(GUI_CLICKED,0,(
				freezable_cb,
				shape_based_cb,
			));
			
			freezable_cb.setChecked(body.isFreezable());
			shape_based_cb.setChecked(body.isShapeBased());
			
			setCallbackEnabled(GUI_CLICKED,1,(
				freezable_cb,
				shape_based_cb,
			));
			
			if(body.isShapeBased()) {
				mass_el.setEnabled(0);
				setEnabled(0,inertia_0_el);
				setEnabled(0,inertia_1_el);
				setEnabled(0,inertia_2_el);
				setEnabled(0,center_of_mass_el);
			} else {
				mass_el.setEnabled(1);
				setEnabled(1,inertia_0_el);
				setEnabled(1,inertia_1_el);
				setEnabled(1,inertia_2_el);
				setEnabled(1,center_of_mass_el);
			}
			
			mass_el.setText(editorFormat(body.getMass()));
			set_inertia(body.getInertia());
			setEditLineVec3(center_of_mass_el,body.getCenterOfMass());
			setEditLineVec3(linear_scale_el,body.getLinearScale());
			setEditLineVec3(angular_scale_el,body.getAngularScale());
			linear_damping_el.setText(editorFormat(body.getLinearDamping()));
			angular_damping_el.setText(editorFormat(body.getAngularDamping()));
			max_linear_velocity_el.setText(editorFormat(body.getMaxLinearVelocity()));
			max_angular_velocity_el.setText(editorFormat(body.getMaxAngularVelocity()));
			frozen_linear_velocity_el.setText(editorFormat(body.getFrozenLinearVelocity()));
			frozen_angular_velocity_el.setText(editorFormat(body.getFrozenAngularVelocity()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			if(body != NULL && body.isShapeBased()) {
				
				mass_el.setText(editorFormat(body.getMass()));
				set_inertia(body.getInertia());
				setEditLineVec3(center_of_mass_el,body.getCenterOfMass());
			}
		}
		
		/*
		 */
		void shape_based_clicked() {
			
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,object);
			
			body.setShapeBased(shape_based_cb.isChecked());
			shape_based_cb.setChecked(body.isShapeBased());
			
			save_state(s);
			
			if(body.isShapeBased()) {
				mass_el.setEnabled(0);
				setEnabled(0,inertia_0_el);
				setEnabled(0,inertia_1_el);
				setEnabled(0,inertia_2_el);
				setEnabled(0,center_of_mass_el);
			} else {
				mass_el.setEnabled(1);
				setEnabled(1,inertia_0_el);
				setEnabled(1,inertia_1_el);
				setEnabled(1,inertia_2_el);
				setEnabled(1,center_of_mass_el);
			}
		}
		
		void mass_pressed() {
			NodesState_ s = new NodesState_();
			if(body.isShapeBased() == 0) {
				float mass = body.getMass();
				if(mass != float(mass_el.getText())) s.init_modify(nodes,object);
				body.setMass(float(mass_el.getText()));
				mass_el.setText(editorFormat(body.getMass()));
				body.setInertia(body.getInertia() * body.getMass() / mass);
				set_inertia(body.getInertia());
			}
			save_state(s);
		}
		
		void inertia_pressed() {
			NodesState_ s = new NodesState_();
			if(body.isShapeBased() == 0) {
				if(body.getInertia() != get_inertia()) s.init_modify(nodes,object);
				body.setInertia(get_inertia());
				set_inertia(body.getInertia());
			}
			save_state(s);
		}
		
		void center_of_mass_pressed() {
			NodesState_ s = new NodesState_();
			if(body.isShapeBased() == 0) {
				if(body.getCenterOfMass() != getEditLineVec3(center_of_mass_el)) s.init_modify(nodes,object);
				body.setCenterOfMass(getEditLineVec3(center_of_mass_el));
				setEditLineVec3(center_of_mass_el,body.getCenterOfMass());
			}
			save_state(s);
		}
		
		/*
		 */
		CLICKED_CALLBACK(freezable,body,Freezable,object)
		PRESSED_CALLBACK_VEC3(linear_scale,body,LinearScale,object,modify)
		PRESSED_CALLBACK_VEC3(angular_scale,body,AngularScale,object,modify)
		PRESSED_CALLBACK_SLIDER(linear_damping,body,LinearDamping,object,modify)
		PRESSED_CALLBACK_SLIDER(angular_damping,body,AngularDamping,object,modify)
		PRESSED_CALLBACK_FLOAT(max_linear_velocity,body,MaxLinearVelocity,object,modify)
		PRESSED_CALLBACK_FLOAT(max_angular_velocity,body,MaxAngularVelocity,object,modify)
		PRESSED_CALLBACK_FLOAT(frozen_linear_velocity,body,FrozenLinearVelocity,object,modify)
		PRESSED_CALLBACK_FLOAT(frozen_angular_velocity,body,FrozenAngularVelocity,object,modify)
		
	} /* namespace Rigid */
	
	/**************************************************************************\
	*
	* BodyRagDoll
	*
	\**************************************************************************/
	
	/*
	 */
	namespace RagDoll {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetCheckBox enabled_cb;
		WidgetCheckBox gravity_cb;
		WidgetCheckBox frame_based_cb;
		WidgetEditLine physical_mask_el;
		WidgetEditLine name_el;
		WidgetEditLine mass_el;
		WidgetEditLine rigidity_el;
		WidgetSlider rigidity_sl;
		WidgetTreeBox bones_tb;
		WidgetLabel bone_type_l;
		WidgetCheckBox bone_enabled_cb;
		WidgetCheckBox bone_frame_based_cb;
		WidgetEditLine bone_physical_mask_el;
		WidgetEditLine linear_damping_el;
		WidgetSlider linear_damping_sl;
		WidgetEditLine angular_damping_el;
		WidgetSlider angular_damping_sl;
		WidgetEditLine max_linear_velocity_el;
		WidgetEditLine max_angular_velocity_el;
		WidgetEditLine frozen_linear_velocity_el;
		WidgetEditLine frozen_angular_velocity_el;
		
		ObjectMeshSkinned object;
		BodyRagDoll body;
		BodyRigid bone;
		int num;
		
		int bones[];
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			object = NULL;
			body = NULL;
			bone = NULL;
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
			
			Nodes::Object::Shape::main_vb.setEnabled(1);
			Nodes::Object::Joint::main_vb.setEnabled(1);
		}
		
		/*
		 */
		void update_bones() {
			
			bone = NULL;
			
			bones_tb.clear();
			bones.clear();
			
			int parent = -1;
			forloop(int i = 0; object.getNumBones()) {
				string name = object.getBoneName(i);
				int num = body.findChild(name);
				name += (num == -1) ? TR(" - free") : TR(" - bound");
				int item = bones_tb.addItem(name);
				bones.append(item,num);
				if(num == 0) parent = item;
			}
			if(parent != -1) {
				forloop(int i = 0; bones_tb.getNumItems()) {
					bones_tb.setItemParent(bones_tb.getItem(i),parent);
				}
			}
			
			bones_tb.setCurrentItem(-1);
			bones_tb.setCurrentItem(parent);
		}
		
		/*
		 */
		void update() {
			
			Nodes::Object::Body::enabled_cb = enabled_cb;
			Nodes::Object::Body::gravity_cb = gravity_cb;
			Nodes::Object::Body::name_el = name_el;
			Nodes::Object::Body::physical_mask_el = physical_mask_el;
			
			object = node_cast(Nodes::Object::object);
			body = body_cast(object.getBody());
			
			frame_based_cb.setCallbackEnabled(GUI_CLICKED,0);
			frame_based_cb.setChecked(body.isFrameBased());
			frame_based_cb.setCallbackEnabled(GUI_CLICKED,1);
			mass_el.setText(editorFormat(body.getMass()));
			rigidity_el.setText(editorFormat(body.getRigidity()));
			
			update_bones();
		}
		
		/*
		 */
		void update_dynamic() {
			
			if(mass_el.isFocused() == 0) {
				mass_el.setText(editorFormat(body.getMass()));
			}
			
			forloop(int i = 0; body.getNumChilds()) {
				Body b = body.getChild(i);
				if(b.isEnabled() == 0) continue;
				vec4 color = vec4(0.5f,0.5f,0.5f,1.0f);
				if(bones_tb.getCurrentItem() != -1) {
					if(bones[bones_tb.getCurrentItem()] == i) color = vec4_one;
				}
				forloop(int j = 0; b.getNumShapes()) {
					Shape shape = b.getShape(j);
					if(shape.isEnabled() == 0) continue;
					shape.renderVisualizer(color);
				}
				forloop(int j = 0; b.getNumJoints()) {
					Joint joint = b.getJoint(j);
					if(joint.isEnabled() == 0) continue;
					joint.renderVisualizer(color);
				}
			}
		}
		
		/*
		 */
		CLICKED_CALLBACK(frame_based,body,FrameBased,object)
		PRESSED_CALLBACK_FLOAT(mass,body,Mass,object,modify)
		PRESSED_CALLBACK_SLIDER(rigidity,body,Rigidity,object,modify)
		
		/*
		 */
		void bones_changed() {
			
			num = -1;
			bone = NULL;
			if(bones_tb.getCurrentItem() != -1) {
				num = bones[bones_tb.getCurrentItem()];
				if(num >= 0 && num < body.getNumChilds()) {
					bone = body_cast(body.getChild(num));
				} else {
					num = -1;
				}
			}
			
			Nodes::Object::Shape::run_function("shutdown");
			Nodes::Object::Joint::run_function("shutdown");
			
			if(bone == NULL) {
				
				Nodes::Object::Shape::body = body;
				Nodes::Object::Joint::body = body;
				
				bone_type_l.setText("");
				bone_enabled_cb.setChecked(0);
				bone_frame_based_cb.setChecked(0);
				bone_physical_mask_el.setText("");
				
				linear_damping_el.setText("");
				angular_damping_el.setText("");
				max_linear_velocity_el.setText("");
				max_angular_velocity_el.setText("");
				frozen_linear_velocity_el.setText("");
				frozen_angular_velocity_el.setText("");
				
				setEnabled(0,(
					Nodes::Object::Shape::main_vb,
					Nodes::Object::Joint::main_vb,
					bone_type_l,
					bone_enabled_cb,
					bone_frame_based_cb,
					linear_damping_el,
					angular_damping_el,
					max_angular_velocity_el,
					max_linear_velocity_el,
					frozen_angular_velocity_el,
					frozen_linear_velocity_el,
				));
				
			} else {
				
				Nodes::Object::Shape::body = bone;
				Nodes::Object::Joint::body = bone;
				
				bone_type_l.setText(bone.getTypeName());
				
				setCallbackEnabled(GUI_CLICKED,0,(
					bone_enabled_cb,
					bone_frame_based_cb,
				));
				
				bone_enabled_cb.setChecked(bone.isEnabled());
				bone_frame_based_cb.setChecked(body.isBoneFrameBased(num));
				
				setCallbackEnabled(GUI_CLICKED,1,(
					bone_enabled_cb,
					bone_frame_based_cb,
				));
				
				if(bone.getType() != BODY_RIGID) {
					
					linear_damping_el.setText("");
					angular_damping_el.setText("");
					max_linear_velocity_el.setText("");
					max_angular_velocity_el.setText("");
					frozen_linear_velocity_el.setText("");
					frozen_angular_velocity_el.setText("");
					
					setEnabled(0,(
						linear_damping_el,
						angular_damping_el,
						max_angular_velocity_el,
						max_linear_velocity_el,
						frozen_angular_velocity_el,
						frozen_linear_velocity_el,
					));
					
				} else {
					
					linear_damping_el.setText(editorFormat(bone.getLinearDamping()));
					angular_damping_el.setText(editorFormat(bone.getAngularDamping()));
					max_linear_velocity_el.setText(editorFormat(bone.getMaxLinearVelocity()));
					max_angular_velocity_el.setText(editorFormat(bone.getMaxAngularVelocity()));
					frozen_linear_velocity_el.setText(editorFormat(bone.getFrozenLinearVelocity()));
					frozen_angular_velocity_el.setText(editorFormat(bone.getFrozenAngularVelocity()));
					
					setEnabled(1,(
						linear_damping_el,
						angular_damping_el,
						max_angular_velocity_el,
						max_linear_velocity_el,
						frozen_angular_velocity_el,
						frozen_linear_velocity_el,
					));
				}
				
				setEnabled(1,(
					Nodes::Object::Shape::main_vb,
					Nodes::Object::Joint::main_vb,
					bone_type_l,
					bone_enabled_cb,
					bone_frame_based_cb,
				));
				
			}
			
			Nodes::Object::Shape::update();
			Nodes::Object::Joint::update();
			
			if(bone != NULL) {
				Nodes::Object::Shape::run_function("init");
				Nodes::Object::Joint::run_function("init");
				Nodes::Object::Shape::run_function("update");
				Nodes::Object::Joint::run_function("update");
			}
		}
		
		void create_clicked() {
			
			NodesState_ s = new NodesState_();
			float mass;
			float error;
			float threshold;
			int capsule;
			if(dialogRagDoll(TR("RagDoll parameters"),mass,error,threshold,capsule)) {
				
				s.init_modify(nodes,object);
				save_state(s);
				body.createBones(error,threshold,capsule);
				body.setMass(mass);
				
				update_bones();
			}
			if(s.state_type == Unredo::STATE_NONE) delete s;
		}
		
		void load_clicked() {
			
			NodesState_ s = new NodesState_();
			string name;
			if(dialogFileNode(TR("Select node to load"),name,DIALOG_FILE_OPEN)) {
				
				Node node = node_load(name);
				if(node == NULL) {
					dialogMessageOk(TR("Error"));
					delete s;
					return;
				}
				
				s.init_modify(nodes,object);
				save_state(s);
				body.setBones(node);
				
				node_delete(node);
			}
			
			update_bones();
			if(s.state_type == Unredo::STATE_NONE) delete s;
		}
		
		void save_clicked() {
			
			string name;
			if(dialogFileNode(TR("Select node to save"),name,DIALOG_FILE_SAVE)) {
				
				Node node = body.getBones();
				if(node == NULL) {
					dialogMessageOk(TR("Error"),TR("Bones is NULL"));
					return;
				}
				
				if(node_save(name,node) == 0) {
					dialogMessageOk(TR("Error"));
				}
				
				node_append(node);
				node_delete(node);
			}
		}
		
		void remove_clicked() {
			
			NodesState_ s = new NodesState_();
			if(dialogMessageYesNo(TR("Confirm"),TR("Remove ragdoll bones?"))) {
				
				s.init_modify(nodes,object);
				save_state(s);
				
				body.setBones(NULL);
				
				update_bones();
			}
			if(s.state_type == Unredo::STATE_NONE) delete s;
		}
		
		/*
		 */
		CLICKED_CALLBACK(bone_enabled,bone,Enabled,object)
		PRESSED_CALLBACK_MASK(bone_physical_mask,bone,PhysicalMask,object)
		CLICKED_CALLBACK_MASK(bone_physical_mask,bone,PhysicalMask,object,TR("Body physical mask"),functionid(bone_physical_mask_changed))
		PRESSED_CALLBACK_SLIDER(linear_damping,bone,LinearDamping,object,modify)
		PRESSED_CALLBACK_SLIDER(angular_damping,bone,AngularDamping,object,modify)
		PRESSED_CALLBACK_FLOAT(max_linear_velocity,bone,MaxLinearVelocity,object,modify)
		PRESSED_CALLBACK_FLOAT(max_angular_velocity,bone,MaxAngularVelocity,object,modify)
		PRESSED_CALLBACK_FLOAT(frozen_linear_velocity,bone,FrozenLinearVelocity,object,modify)
		PRESSED_CALLBACK_FLOAT(frozen_angular_velocity,bone,FrozenAngularVelocity,object,modify)
		
		/*
		 */
		void bone_frame_based_clicked() {
			if(num != -1) {
				NodesState_ s = create_state_modify(nodes,node);
				body.setBoneFrameBased(num,bone_frame_based_cb.isChecked());
				save_state(s);
			}
		}
		
		void bone_physical_mask_changed(int mask) {
			bone.setPhysicalMask(mask);
			bone_physical_mask_el.setText(editorFormatMask(bone.getPhysicalMask()));
		}
		
	} /* namespace RagDoll */
	
	/**************************************************************************\
	*
	* BodyFracture
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Fracture {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetCheckBox enabled_cb;
		WidgetCheckBox gravity_cb;
		WidgetCheckBox broken_cb;
		WidgetEditLine name_el;
		WidgetEditLine physical_mask_el;
		WidgetEditLine collision_mask_el;
		WidgetEditLine exclusion_mask_el;
		WidgetEditLine error_el;
		WidgetEditLine threshold_el;
		WidgetEditLine material_el;
		WidgetIcon material_clear_i;
		WidgetEditLine property_el;
		WidgetIcon property_clear_i;
		WidgetEditLine mass_el;
		WidgetEditLine density_el;
		WidgetEditLine friction_el;
		WidgetSlider friction_sl;
		WidgetEditLine restitution_el;
		WidgetSlider restitution_sl;
		WidgetEditLine linear_damping_el;
		WidgetSlider linear_damping_sl;
		WidgetEditLine angular_damping_el;
		WidgetSlider angular_damping_sl;
		WidgetEditLine max_linear_velocity_el;
		WidgetEditLine max_angular_velocity_el;
		WidgetEditLine frozen_linear_velocity_el;
		WidgetEditLine frozen_angular_velocity_el;
		
		BodyFracture body;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			body = NULL;
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			Nodes::Object::Body::enabled_cb = enabled_cb;
			Nodes::Object::Body::gravity_cb = gravity_cb;
			Nodes::Object::Body::name_el = name_el;
			Nodes::Object::Body::physical_mask_el = physical_mask_el;
			
			body = body_cast(object.getBody());
			broken_cb.setCallbackEnabled(GUI_CLICKED,0);
			broken_cb.setChecked(body.isBroken());
			broken_cb.setCallbackEnabled(GUI_CLICKED,1);
			collision_mask_el.setText(editorFormatMask(body.getCollisionMask()));
			exclusion_mask_el.setText(editorFormatMask(body.getExclusionMask()));
			error_el.setText(editorFormat(body.getError()));
			threshold_el.setText(editorFormat(body.getThreshold()));
			material_el.setText(body.getMaterialName());
			material_clear_i.setEnabled(body.getMaterialName() != "mesh_base");
			property_el.setText(body.getPropertyName());
			property_clear_i.setEnabled(body.getPropertyName() != "surface_base");
			mass_el.setText(editorFormat(body.getMass()));
			density_el.setText(editorFormat(body.getDensity()));
			friction_el.setText(editorFormat(body.getFriction()));
			restitution_el.setText(editorFormat(body.getRestitution()));
			linear_damping_el.setText(editorFormat(body.getLinearDamping()));
			angular_damping_el.setText(editorFormat(body.getAngularDamping()));
			max_linear_velocity_el.setText(editorFormat(body.getMaxLinearVelocity()));
			max_angular_velocity_el.setText(editorFormat(body.getMaxAngularVelocity()));
			frozen_linear_velocity_el.setText(editorFormat(body.getFrozenLinearVelocity()));
			frozen_angular_velocity_el.setText(editorFormat(body.getFrozenAngularVelocity()));
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		void mass_pressed() {
			NodesState_ s = new NodesState_();
			if(body.getMass() != float(mass_el.getText())) s.init_modify(nodes,object);
			body.setMass(float(mass_el.getText()));
			mass_el.setText(editorFormat(body.getMass()));
			density_el.setText(editorFormat(body.getDensity()));
			save_state(s);
		}
		
		void density_pressed() {
			NodesState_ s = new NodesState_();
			if(body.getDensity() != float(density_el.getText())) s.init_modify(nodes,object);
			body.setDensity(float(density_el.getText()));
			mass_el.setText(editorFormat(body.getMass()));
			density_el.setText(editorFormat(body.getDensity()));
			save_state(s);
		}
		
		/*
		 */
		void material_pressed() {
			string material = body.getMaterialName();
			if(material == material_el.getText()) return;
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,node);
			body.setMaterial(material);
			save_state(s);
			update();
		}
		
		/*
		 */
		void material_load_clicked() {
			string material = body.getMaterialName();
			if(dialogMaterial(TR("Select material"),material)) {
				NodesState_ s = new NodesState_();
				if(material != body.getMaterialName()) s.init_modify(nodes,node);
				body.setMaterial(material);
				save_state(s);
				update();
			}
		}
		
		void material_clear_clicked() {
			if(dialogMessageYesNo(TR("Confirm"),TR("Clear body material?"))) {
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,node);
				body.setMaterial("mesh_base");
				save_state(s);
				update();
			}
		}
		
		/*
		 */
		void property_pressed() {
			string property = body.getPropertyName();
			if(property == property_el.getText()) return;
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,node);
			body.setProperty(property);
			save_state(s);
			update();
		}
		
		/*
		 */
		void property_load_clicked() {
			string property = body.getPropertyName();
			if(dialogProperty(TR("Select property"),property)) {
				NodesState_ s = new NodesState_();
				if(property != body.getPropertyName()) s.init_modify(nodes,node);
				body.setProperty(property);
				save_state(s);
				update();
			}
		}
		
		void property_clear_clicked() {
			if(dialogMessageYesNo(TR("Confirm"),TR("Clear body property?"))) {
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,node);
				body.setProperty("mesh_base");
				save_state(s);
				update();
			}
		}
		
		/*
		 */
		void collision_mask_changed(int mask) {
			body.setCollisionMask(mask);
			collision_mask_el.setText(editorFormatMask(body.getCollisionMask()));
		}
		
		/*
		 */
		void exclusion_mask_changed(int mask) {
			body.setExclusionMask(mask);
			exclusion_mask_el.setText(editorFormatMask(body.getExclusionMask()));
		}
		
		/*
		 */
		CLICKED_CALLBACK(broken,body,Broken,object)
		PRESSED_CALLBACK_MASK(collision_mask,body,CollisionMask,object)
		CLICKED_CALLBACK_MASK(collision_mask,body,CollisionMask,object,TR("Body collision mask"),functionid(collision_mask_changed))
		PRESSED_CALLBACK_MASK(exclusion_mask,body,ExclusionMask,object)
		CLICKED_CALLBACK_MASK(exclusion_mask,body,ExclusionMask,object,TR("Body exclusion mask"),functionid(exclusion_mask_changed))
		PRESSED_CALLBACK_FLOAT(error,body,Error,object,modify)
		PRESSED_CALLBACK_FLOAT(threshold,body,Threshold,object,modify)
		PRESSED_CALLBACK_SLIDER(friction,body,Friction,object,modify)
		PRESSED_CALLBACK_SLIDER(restitution,body,Restitution,object,modify)
		PRESSED_CALLBACK_SLIDER(linear_damping,body,LinearDamping,object,modify)
		PRESSED_CALLBACK_SLIDER(angular_damping,body,AngularDamping,object,modify)
		PRESSED_CALLBACK_FLOAT(max_linear_velocity,body,MaxLinearVelocity,object,modify)
		PRESSED_CALLBACK_FLOAT(max_angular_velocity,body,MaxAngularVelocity,object,modify)
		PRESSED_CALLBACK_FLOAT(frozen_linear_velocity,body,FrozenLinearVelocity,object,modify)
		PRESSED_CALLBACK_FLOAT(frozen_angular_velocity,body,FrozenAngularVelocity,object,modify)
		
	} /* namespace Fracture */
	
	/**************************************************************************\
	*
	* BodyRope
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Rope {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetCheckBox enabled_cb;
		WidgetCheckBox gravity_cb;
		WidgetCheckBox collision_cb;
		WidgetEditLine name_el;
		WidgetEditLine physical_mask_el;
		WidgetEditLine collision_mask_el;
		WidgetEditLine num_iterations_el;
		WidgetSlider num_iterations_sl;
		WidgetEditLine distance_el;
		WidgetEditLine mass_el;
		WidgetEditLine radius_el;
		WidgetEditLine rigidity_el;
		WidgetSlider rigidity_sl;
		WidgetEditLine friction_el;
		WidgetSlider friction_sl;
		WidgetEditLine restitution_el;
		WidgetSlider restitution_sl;
		WidgetEditLine linear_damping_el;
		WidgetSlider linear_damping_sl;
		WidgetEditLine linear_stretch_el;
		WidgetSlider linear_stretch_sl;
		WidgetEditLine linear_threshold_el;
		WidgetEditLine angular_threshold_el;
		WidgetEditLine linear_restitution_el;
		WidgetSlider linear_restitution_sl;
		WidgetEditLine angular_restitution_el;
		WidgetSlider angular_restitution_sl;
		
		BodyRope body;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			body = NULL;
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			Nodes::Object::Body::enabled_cb = enabled_cb;
			Nodes::Object::Body::gravity_cb = gravity_cb;
			Nodes::Object::Body::name_el = name_el;
			Nodes::Object::Body::physical_mask_el = physical_mask_el;
			
			body = body_cast(object.getBody());
			
			setCallbackEnabled(GUI_CLICKED,0,(
				collision_cb,
			));
			
			collision_cb.setChecked(body.getCollision());
			
			setCallbackEnabled(GUI_CLICKED,1,(
				collision_cb,
			));
			
			collision_mask_el.setText(editorFormatMask(body.getCollisionMask()));
			
			num_iterations_el.setText(string(body.getNumIterations()));
			
			distance_el.setText(editorFormat(body.getDistance()));
			
			mass_el.setText(editorFormat(body.getMass()));
			radius_el.setText(editorFormat(body.getRadius()));
			rigidity_el.setText(editorFormat(body.getRigidity()));
			friction_el.setText(editorFormat(body.getFriction()));
			restitution_el.setText(editorFormat(body.getRestitution()));
			linear_damping_el.setText(editorFormat(body.getLinearDamping()));
			linear_stretch_el.setText(editorFormat(body.getLinearStretch()));
			
			linear_threshold_el.setText(editorFormat(body.getLinearThreshold()));
			angular_threshold_el.setText(editorFormat(body.getAngularThreshold()));
			
			linear_restitution_el.setText(editorFormat(body.getLinearRestitution()));
			angular_restitution_el.setText(editorFormat(body.getAngularRestitution()));
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		void collision_mask_changed(int mask) {
			body.setCollisionMask(mask);
			collision_mask_el.setText(editorFormatMask(body.getCollisionMask()));
		}
		
		/*
		 */
		CLICKED_CALLBACK(collision,body,Collision,object)
		PRESSED_CALLBACK_MASK(collision_mask,body,CollisionMask,object)
		CLICKED_CALLBACK_MASK(collision_mask,body,CollisionMask,object,TR("Body collision mask"),functionid(collision_mask_changed))
		PRESSED_CALLBACK_SLIDER_INT(num_iterations,body,NumIterations,object,modify)
		PRESSED_CALLBACK_FLOAT(distance,body,Distance,object,modify)
		PRESSED_CALLBACK_FLOAT(mass,body,Mass,object,modify)
		PRESSED_CALLBACK_FLOAT(radius,body,Radius,object,modify)
		PRESSED_CALLBACK_SLIDER(rigidity,body,Rigidity,object,modify)
		PRESSED_CALLBACK_SLIDER(friction,body,Friction,object,modify)
		PRESSED_CALLBACK_SLIDER(restitution,body,Restitution,object,modify)
		PRESSED_CALLBACK_SLIDER(linear_damping,body,LinearDamping,object,modify)
		PRESSED_CALLBACK_SLIDER(linear_stretch,body,LinearStretch,object,modify)
		PRESSED_CALLBACK_FLOAT(linear_threshold,body,LinearThreshold,object,modify)
		PRESSED_CALLBACK_FLOAT(angular_threshold,body,AngularThreshold,object,modify)
		PRESSED_CALLBACK_SLIDER(linear_restitution,body,LinearRestitution,object,modify)
		PRESSED_CALLBACK_SLIDER(angular_restitution,body,AngularRestitution,object,modify)
		
	} /* namespace Rope */
	
	/**************************************************************************\
	*
	* BodyCloth
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Cloth {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetCheckBox enabled_cb;
		WidgetCheckBox gravity_cb;
		WidgetCheckBox collision_cb;
		WidgetCheckBox two_sided_cb;
		WidgetEditLine name_el;
		WidgetEditLine physical_mask_el;
		WidgetEditLine collision_mask_el;
		WidgetEditLine num_iterations_el;
		WidgetSlider num_iterations_sl;
		WidgetEditLine distance_el;
		WidgetEditLine mass_el;
		WidgetEditLine radius_el;
		WidgetEditLine rigidity_el;
		WidgetSlider rigidity_sl;
		WidgetEditLine friction_el;
		WidgetSlider friction_sl;
		WidgetEditLine restitution_el;
		WidgetSlider restitution_sl;
		WidgetEditLine linear_damping_el;
		WidgetSlider linear_damping_sl;
		WidgetEditLine linear_stretch_el;
		WidgetSlider linear_stretch_sl;
		WidgetEditLine linear_threshold_el;
		WidgetEditLine angular_threshold_el;
		WidgetEditLine linear_restitution_el;
		WidgetSlider linear_restitution_sl;
		WidgetEditLine angular_restitution_el;
		WidgetSlider angular_restitution_sl;
		
		BodyCloth body;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			body = NULL;
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			Nodes::Object::Body::enabled_cb = enabled_cb;
			Nodes::Object::Body::gravity_cb = gravity_cb;
			Nodes::Object::Body::name_el = name_el;
			Nodes::Object::Body::physical_mask_el = physical_mask_el;
			
			body = body_cast(object.getBody());
			
			setCallbackEnabled(GUI_CLICKED,0,(
				collision_cb,
				two_sided_cb,
			));
			
			collision_cb.setChecked(body.getCollision());
			two_sided_cb.setChecked(body.getTwoSided());
			
			setCallbackEnabled(GUI_CLICKED,1,(
				collision_cb,
				two_sided_cb,
			));
			
			collision_mask_el.setText(editorFormatMask(body.getCollisionMask()));
			
			num_iterations_el.setText(string(body.getNumIterations()));
			
			distance_el.setText(editorFormat(body.getDistance()));
			
			mass_el.setText(editorFormat(body.getMass()));
			radius_el.setText(editorFormat(body.getRadius()));
			rigidity_el.setText(editorFormat(body.getRigidity()));
			friction_el.setText(editorFormat(body.getFriction()));
			restitution_el.setText(editorFormat(body.getRestitution()));
			linear_damping_el.setText(editorFormat(body.getLinearDamping()));
			linear_stretch_el.setText(editorFormat(body.getLinearStretch()));
			
			linear_threshold_el.setText(editorFormat(body.getLinearThreshold()));
			angular_threshold_el.setText(editorFormat(body.getAngularThreshold()));
			
			linear_restitution_el.setText(editorFormat(body.getLinearRestitution()));
			angular_restitution_el.setText(editorFormat(body.getAngularRestitution()));
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		void collision_mask_changed(int mask) {
			body.setCollisionMask(mask);
			collision_mask_el.setText(editorFormatMask(body.getCollisionMask()));
		}
		
		/*
		 */
		CLICKED_CALLBACK(collision,body,Collision,object)
		CLICKED_CALLBACK(two_sided,body,TwoSided,object)
		PRESSED_CALLBACK_MASK(collision_mask,body,CollisionMask,object)
		CLICKED_CALLBACK_MASK(collision_mask,body,CollisionMask,object,TR("Body collision mask"),functionid(collision_mask_changed))
		PRESSED_CALLBACK_SLIDER_INT(num_iterations,body,NumIterations,object,modify)
		PRESSED_CALLBACK_FLOAT(distance,body,Distance,object,modify)
		PRESSED_CALLBACK_FLOAT(mass,body,Mass,object,modify)
		PRESSED_CALLBACK_FLOAT(radius,body,Radius,object,modify)
		PRESSED_CALLBACK_SLIDER(rigidity,body,Rigidity,object,modify)
		PRESSED_CALLBACK_SLIDER(friction,body,Friction,object,modify)
		PRESSED_CALLBACK_SLIDER(restitution,body,Restitution,object,modify)
		PRESSED_CALLBACK_SLIDER(linear_damping,body,LinearDamping,object,modify)
		PRESSED_CALLBACK_SLIDER(linear_stretch,body,LinearStretch,object,modify)
		PRESSED_CALLBACK_FLOAT(linear_threshold,body,LinearThreshold,object,modify)
		PRESSED_CALLBACK_FLOAT(angular_threshold,body,AngularThreshold,object,modify)
		PRESSED_CALLBACK_SLIDER(linear_restitution,body,LinearRestitution,object,modify)
		PRESSED_CALLBACK_SLIDER(angular_restitution,body,AngularRestitution,object,modify)
		
	} /* namespace Cloth */
	
	/**************************************************************************\
	*
	* BodyWater
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Water {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetCheckBox enabled_cb;
		WidgetCheckBox gravity_cb;
		WidgetCheckBox absorption_cb;
		WidgetCheckBox intersection_cb;
		WidgetEditLine name_el;
		WidgetEditLine physical_mask_el;
		WidgetEditLine distance_el;
		WidgetEditLine depth_el;
		WidgetEditLine density_el;
		WidgetEditLine liquidity_el;
		WidgetEditLine linear_damping_el;
		WidgetEditLine angular_damping_el;
		WidgetEditLine interaction_force_el;
		
		BodyWater body;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			body = NULL;
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			Nodes::Object::Body::enabled_cb = enabled_cb;
			Nodes::Object::Body::gravity_cb = gravity_cb;
			Nodes::Object::Body::name_el = name_el;
			Nodes::Object::Body::physical_mask_el = physical_mask_el;
			
			body = body_cast(object.getBody());
			
			setCallbackEnabled(GUI_CLICKED,0,(
				absorption_cb,
				intersection_cb,
			));
			
			absorption_cb.setChecked(body.getAbsorption());
			intersection_cb.setChecked(body.getIntersection());
			
			setCallbackEnabled(GUI_CLICKED,1,(
				absorption_cb,
				intersection_cb,
			));
			
			distance_el.setText(editorFormat(body.getDistance()));
			
			depth_el.setText(editorFormat(body.getDepth()));
			density_el.setText(editorFormat(body.getDensity()));
			liquidity_el.setText(editorFormat(body.getLiquidity()));
			linear_damping_el.setText(editorFormat(body.getLinearDamping()));
			angular_damping_el.setText(editorFormat(body.getAngularDamping()));
			interaction_force_el.setText(editorFormat(body.getInteractionForce()));
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		CLICKED_CALLBACK(absorption,body,Absorption,object)
		CLICKED_CALLBACK(intersection,body,Intersection,object)
		PRESSED_CALLBACK_FLOAT(distance,body,Distance,object,modify)
		PRESSED_CALLBACK_FLOAT(depth,body,Depth,object,modify)
		PRESSED_CALLBACK_FLOAT(density,body,Density,object,modify)
		PRESSED_CALLBACK_FLOAT(liquidity,body,Liquidity,object,modify)
		PRESSED_CALLBACK_FLOAT(linear_damping,body,LinearDamping,object,modify)
		PRESSED_CALLBACK_FLOAT(angular_damping,body,AngularDamping,object,modify)
		PRESSED_CALLBACK_FLOAT(interaction_force,body,InteractionForce,object,modify)
		
	} /* namespace Water */
	
	/**************************************************************************\
	*
	* BodyPath
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Path {
		
		/*
		 */
		WidgetGridBox parameters_gb;
		WidgetCheckBox enabled_cb;
		WidgetCheckBox gravity_cb;
		WidgetEditLine name_el;
		WidgetEditLine physical_mask_el;
		WidgetEditLine path_el;
		
		BodyPath body;
		
		/*
		 */
		void init() {
			
			parameters_vb.replaceChild(parameters_gb,dummy_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(parameters_gb);
		}
		
		void shutdown() {
			
			body = NULL;
			
			parameters_vb.replaceChild(dummy_vb,parameters_gb);
		}
		
		/*
		 */
		void update() {
			
			Nodes::Object::Body::enabled_cb = enabled_cb;
			Nodes::Object::Body::gravity_cb = gravity_cb;
			Nodes::Object::Body::name_el = name_el;
			Nodes::Object::Body::physical_mask_el = physical_mask_el;
			
			body = body_cast(object.getBody());
			path_el.setText(body.getPathName());
		}
		
		/*
		 */
		void update_dynamic() {
			
			if(body != NULL) {
				
				body.renderVisualizer();
			}
		}
		
		/*
		 */
		void path_pressed() {
			body.setPathName(path_el.getText());
		}
		
		void path_load_clicked() {
			string name = body.getPathName();
			if(dialogFilePath(TR("Select path to load"),name,DIALOG_FILE_OPEN)) {
				body.setPathName(name);
			}
		}
		
	} /* namespace Path */
	
} /* namespace Body */
