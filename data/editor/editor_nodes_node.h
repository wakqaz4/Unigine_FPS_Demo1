/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_nodes_node.h
 * Desc:    Unigine editor
 * Version: 1.01
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
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

/**************************************************************************\
*
* Dummy
*
\**************************************************************************/

/*
 */
namespace Dummy {
	
	/*
	 */
	void init() {
		
	}
	
	void shutdown() {
		
	}
	
	/*
	 */
	void update() {
		
	}
	
	/*
	 */
	void update_dynamic() {
		
	}
}

/**************************************************************************\
*
* Layer
*
\**************************************************************************/

/*
 */
namespace Layer {
	
	/*
	 */
	WidgetVBox main_vb;			// main vbox
	WidgetEditLine name_el;		// layer name
	
	NodeLayer layer;			// layer node
	
	/*
	 */
	void init() {
		parameters_tb.addTab(TR("Layer"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
	}
	
	/*
	 */
	void update() {
		layer = node_cast(node);
		name_el.setText(layer.getNodeName());
	}
	
	/*
	 */
	void update_dynamic() {
		
	}
	
	/*
	 */
	void name_pressed() {
		layer.setNodeName(name_el.getText());
	}
	
	/*
	 */
	void save_clicked() {
		
		if(dialogMessageYesNo(TR("Confirm"),TR("Save layer into the current file?")) == 0) return;
		
		Node nodes[0];
		forloop(int i = 0; layer.getNumChilds()) {
			nodes.append(layer.getChild(i));
		}
		
		log.message("Saving \"%s\" layer\n",layer.getNodeName());
		if(engine.world.saveNodes(layer.getNodeName(),nodes) == 0) {
			dialogMessageOk(TR("Error"));
		}
	}
}

/**************************************************************************\
*
* Pivot
*
\**************************************************************************/

/*
 */
namespace Pivot {
	
	/*
	 */
	WidgetVBox main_vb;						// main vbox
	
	WidgetEditLine pivot_translate_el[3];	// pivot translate
	WidgetEditLine pivot_rotate_el[3];		// pivot rotate
	WidgetEditLine pivot_scale_el[3];		// pivot scale
	
	WidgetEditLine local_translate_el[3];	// local translate
	WidgetEditLine local_rotate_el[3];		// local rotate
	WidgetEditLine local_scale_el[3];		// local scale
	
	WidgetButton edit_pivot_b;				// edit pivot button
	
	NodePivot pivot;						// pivot node
	
	/*
	 */
	WidgetManipulatorTranslator translator_m;
	WidgetManipulatorRotator rotator_m;
	WidgetManipulatorScaler scaler_m;
	
	/*
	 */
	void init() {
		parameters_tb.addTab(TR("Pivot"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
		if(translator_m != NULL) gui.removeChild(translator_m);
		if(rotator_m != NULL) gui.removeChild(rotator_m);
		if(scaler_m != NULL) gui.removeChild(scaler_m);
	}
	
	/*
	 */
	void update() {
		
		pivot = node_cast(node);
		
		update_pivot_transform();
		
		update_local_transform();
		
		edit_pivot_b.setToggled(0);
	}
	
	/*
	 */
	void update_dynamic() {
		
		if(parameters_tb.getCurrentTab() == 1) {
			
			// create manipulators
			if(translator_m == NULL) {
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
			
			WidgetManipulator manipulator = NULL;
			
			// translator
			if(panelPlacementGetTranslator()) {
				updateAuxManipulator(translator_m);
				if(translator_m.isFocused() == 0) {
					manipulator = translator_m;
				}
				if(editorIsChild(translator_m) == 0) {
					gui.addChild(translator_m,GUI_ALIGN_OVERLAP);
					gui.removeChild(rotator_m);
					gui.removeChild(scaler_m);
				}
			}
			// rotator
			else if(panelPlacementGetRotator()) {
				updateAuxManipulator(rotator_m);
				if(rotator_m.isFocused() == 0) {
					manipulator = rotator_m;
				}
				if(editorIsChild(rotator_m) == 0) {
					gui.addChild(rotator_m,GUI_ALIGN_OVERLAP);
					gui.removeChild(translator_m);
					gui.removeChild(scaler_m);
				}
			}
			// scaler
			else if(panelPlacementGetScaler()) {
				updateAuxManipulator(scaler_m);
				if(scaler_m.isFocused() == 0) {
					manipulator = scaler_m;
				}
				if(editorIsChild(scaler_m) == 0) {
					gui.addChild(scaler_m,GUI_ALIGN_OVERLAP);
					gui.removeChild(translator_m);
					gui.removeChild(rotator_m);
				}
			}
			else {
				gui.removeChild(translator_m);
				gui.removeChild(rotator_m);
				gui.removeChild(scaler_m);
			}
			
			if(manipulator != NULL) update_manipulator(manipulator);
			else if(edit_pivot_b.isToggled()) update_pivot_transform();
			else update_local_transform();
		} else {
			if(translator_m != NULL) gui.removeChild(translator_m);
			if(rotator_m != NULL) gui.removeChild(rotator_m);
			if(scaler_m != NULL) gui.removeChild(scaler_m);
		}
	}
	
	/*
	 */
	void update_pivot_transform() {
		vec3 translate,rotate,scale;
		decomposeTransform(pivot.getPivotTransform(),translate,rotate,scale);
		setEditLineVec3(pivot_translate_el,translate);
		setEditLineVec3(pivot_rotate_el,rotate);
		setEditLineVec3(pivot_scale_el,scale);
	}
	
	void update_local_transform() {
		vec3 translate,rotate,scale;
		decomposeTransform(pivot.getLocalTransform(),translate,rotate,scale);
		setEditLineVec3(local_translate_el,translate);
		setEditLineVec3(local_rotate_el,rotate);
		setEditLineVec3(local_scale_el,scale);
	}
	
	/*
	 */
	void update_manipulator(WidgetManipulator manipulator) {
		
		manipulator.setCallbackEnabled(GUI_CHANGED,0);
		
		mat4 pivot_transform = pivot.getWorldTransform() * pivot.getPivotTransform();
		
		int basis = panelPlacementGetBasis();
		
		if(edit_pivot_b.isToggled()) {
			if(basis == PANEL_BASIS_WORLD) manipulator.setBasis(Mat4_identity);
			else if(basis == PANEL_BASIS_PARENT) manipulator.setBasis(pivot.getWorldTransform());
			else if(basis == PANEL_BASIS_LOCAL) manipulator.setBasis(pivot_transform);
			manipulator.setTransform(pivot_transform);
		} else {
			if(basis == PANEL_BASIS_WORLD) manipulator.setBasis(pivot_transform);
			else if(basis == PANEL_BASIS_PARENT) manipulator.setBasis(pivot_transform);
			else if(basis == PANEL_BASIS_LOCAL) manipulator.setBasis(pivot_transform * pivot.getLocalTransform());
			manipulator.setTransform(pivot_transform * pivot.getLocalTransform());
		}
		
		manipulator.setCallbackEnabled(GUI_CHANGED,1);
	}
	
	/*
	 */
	void manipulator_changed(WidgetManipulator manipulator) {
		
		mat4 transform;
		mat4 current_transform;
		
		if(edit_pivot_b.isToggled()) {
			current_transform = pivot.getPivotTransform();
			transform = mat4(pivot.getIWorldTransform() * manipulator.getTransform());
		} else {
			current_transform = pivot.getLocalTransform();
			transform = mat4(inverse(pivot.getWorldTransform() * pivot.getPivotTransform()) * manipulator.getTransform());
		}
		
		if(compare(current_transform,transform,STATE_EPSILON) == 0 && manipulator.isFocused()) {
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,node);
			if(edit_pivot_b.isToggled()) pivot.setPivotTransform(transform);
			else pivot.setLocalTransform(transform);
			save_state(s,manipulator);
		}
	}
	
	/*
	 */
	void pivot_transform_pressed() {
		
		NodesState_ s = new NodesState_();
		
		vec3 translate = getEditLineVec3(pivot_translate_el);
		vec3 rotate = getEditLineVec3(pivot_rotate_el);
		vec3 scale = getEditLineVec3(pivot_scale_el);
		
		mat4 transform = mat4(composeTransform(translate,rotate,scale));
		if(compare(pivot.getPivotTransform(),transform,STATE_EPSILON) == 0) s.init_modify(nodes,node);
		pivot.setPivotTransform(transform);
		
		update_pivot_transform();
		
		update_manipulator(translator_m);
		update_manipulator(rotator_m);
		update_manipulator(scaler_m);
		
		save_state(s);
	}
	
	void local_transform_pressed() {
		
		NodesState_ s = new NodesState_();
		
		vec3 translate = getEditLineVec3(local_translate_el);
		vec3 rotate = getEditLineVec3(local_rotate_el);
		vec3 scale = getEditLineVec3(local_scale_el);
		
		mat4 transform = mat4(composeTransform(translate,rotate,scale));
		if(compare(pivot.getLocalTransform(),transform,STATE_EPSILON) == 0) s.init_modify(nodes,node);
		pivot.setLocalTransform(transform);
		
		update_local_transform();
		
		update_manipulator(translator_m);
		update_manipulator(rotator_m);
		update_manipulator(scaler_m);
		
		save_state(s);
	}
}

/**************************************************************************\
*
* Trigger
*
\**************************************************************************/

/*
 */
namespace Trigger {
	
	/*
	 */
	WidgetVBox main_vb;			// main vbox
	WidgetEditLine enabled_el;	// enabled name
	WidgetEditLine position_el;	// position name
	
	NodeTrigger trigger;		// trigger node
	
	/*
	 */
	void init() {
		parameters_tb.addTab(TR("Trigger"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
	}
	
	/*
	 */
	void update() {
		trigger = node_cast(node);
		enabled_el.setText(trigger.getEnabledCallbackName());
		position_el.setText(trigger.getPositionCallbackName());
	}
	
	/*
	 */
	void update_dynamic() {
		
	}
	
	/*
	 */
	PRESSED_CALLBACK(enabled,trigger,EnabledCallbackName,trigger,string,modify)
	PRESSED_CALLBACK(position,trigger,PositionCallbackName,trigger,string,modify)
}

/**************************************************************************\
*
* Reference
*
\**************************************************************************/

/*
 */
namespace Reference {
	
	/*
	 */
	WidgetVBox main_vb;			// main vbox
	WidgetEditLine name_el;		// reference name
	
	NodeReference reference;	// reference node
	
	/*
	 */
	void init() {
		parameters_tb.addTab(TR("Reference"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
	}
	
	/*
	 */
	void update() {
		reference = node_cast(node);
		name_el.setText(reference.getNodeName());
	}
	
	/*
	 */
	void update_dynamic() {
		
		Node node = reference.getNode();
		if(node == NULL) return;
		
		engine.visualizer.renderNodeBoundBox(node,vec4(0.0f,1.0f,0.0f,1.0f));
		
		// show reference info
		if(helpersGetNodeInfo()) {
			string info = run_function("get_info") + TR("\nNode: ") + reference.getNodeName();
			if(node.isObject()) {
				Object object = node_cast(node);
				int polycount = 0;
				forloop(int i = 0; object.getNumSurfaces()) {
					polycount += object.getNumTriangles(i);
				}
				info += format(TR("\nTriangles: %d"),polycount);
			}
			vec3 center = get_world_bound_center(node);
			float radius = get_bound_radius(node);
			Player player = Unigine::getPlayer();
			vec3 camera_position = (player != NULL) ? player.getPosition() : vec3_zero;
			float distance = length(center - camera_position) - radius;
			info += format(TR("\nCenter: %g %g %g\nRadius: %g\nDistance: %g"),center.x,center.y,center.z,radius,distance);
			engine.visualizer.renderMessage3D(reference.getWorldTransform() * reference.getBoundSphere().getCenter(),vec3_zero,info,vec4_one,1);
			
			render_visualizer(reference);
		}
	}
	
	void render_visualizer(NodeReference n) {
		void show_objects(Node node) {
			forloop(int i = 0; node.getNumChilds()) {
				Node n = node.getChild(i);
				show_objects(n);
			}
			if(node.isObject()) {
				Object object = node_cast(node);
				Object::render_visualizer(object);
			}
		}
		if(n.getNode() != NULL) show_objects(n.getNode());
	}
	
	/*
	 */
	void name_pressed() {
		reference.setNodeName(name_el.getText());
		if(reference.getNode() == NULL) dialogMessageOk(TR("Error"));
	}
	
	void load_clicked() {
		string name = reference.getNodeName();
		if(dialogFileNode(TR("Select node to load"),name,DIALOG_FILE_OPEN)) {
			reference.setNodeName(name);
			name_el.setText(name);
			if(reference.getNode() == NULL) dialogMessageOk(TR("Error"));
		}
	}
	
	void edit_clicked() {
		unredoAddReferenceState();
		referenceEnable(reference);
	}
}

/**************************************************************************\
*
* Extern
*
\**************************************************************************/

/*
 */
namespace Extern {
	
	/*
	 */
	void init() {
		
	}
	
	void shutdown() {
		
	}
	
	/*
	 */
	void update() {
		
	}
	
	/*
	 */
	void update_dynamic() {
		
	}
}
