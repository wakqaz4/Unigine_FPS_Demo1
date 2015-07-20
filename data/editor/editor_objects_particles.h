/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_objects_particles.h
 * Desc:    Unigine editor
 * Version: 1.28
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

/******************************************************************************\
*
* Callbacks
*
\******************************************************************************/

/*
 */
#define PARTICLES_PRESSED_CALLBACK(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	if(num == -1) return; \
	NodesState_ s = new NodesState_(); \
	if(WIDGET ## _el.isFocused() && particles.get ## FUNC(num) != float(WIDGET ## _el.getText())) { \
		s.init_modify(nodes,particles); \
		particles.set ## FUNC(num,float(WIDGET ## _el.getText())); \
	} \
	WIDGET ## _el.setText(editorFormat(particles.get ## FUNC(num))); \
	save_state(s,engine.gui.getFocus()); \
}

#define PARTICLES_PRESSED_CALLBACK_INT(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	if(num == -1) return; \
	NodesState_ s = new NodesState_(); \
	if(WIDGET ## _el.isFocused() && particles.get ## FUNC(num) != int(WIDGET ## _el.getText())) { \
		s.init_modify(nodes,particles); \
		particles.set ## FUNC(num,int(WIDGET ## _el.getText())); \
	} \
	WIDGET ## _el.setText(string(particles.get ## FUNC(num))); \
	save_state(s,engine.gui.getFocus()); \
}

#define PARTICLES_PRESSED_CALLBACK_VEC3(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	if(num == -1) return; \
	NodesState_ s = new NodesState_(); \
	if(particles.get ## FUNC(num) != getEditLineVec3(WIDGET ## _el)) { \
		s.init_modify(nodes,particles); \
		particles.set ## FUNC(num,getEditLineVec3(WIDGET ## _el)); \
	} \
	setEditLineVec3(WIDGET ## _el,particles.get ## FUNC(num)); \
	save_state(s); \
}

#define PARTICLES_PRESSED_CALLBACK_SLIDER(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	if(num == -1) return; \
	NodesState_ s = new NodesState_(); \
	if((WIDGET ## _el.isFocused() && particles.get ## FUNC(num) != float(WIDGET ## _el.getText())) || WIDGET ## _sl.isFocused()) { \
		s.init_modify(nodes,particles); \
		particles.set ## FUNC(num,float(WIDGET ## _el.getText())); \
	} \
	WIDGET ## _el.setText(editorFormat(particles.get ## FUNC(num))); \
	save_state(s,engine.gui.getFocus()); \
}

#define PARTICLES_MANIPULATOR_CHANGED(MANIPULATOR,FUNC1,FUNC2,TRANSFORM) \
void MANIPULATOR ## _changed() { \
	if(num == -1) return; \
	if(MANIPULATOR ## _m.isFocused() == 0) return; \
	NodesState_ s = new NodesState_(); \
	mat4 transform = MANIPULATOR ## _m.getTransform(); \
	MANIPULATOR ## _m.setBasis(transform); \
	if(particles.FUNC1(num)) transform = particles.getIWorldTransform() * transform; \
	if(compare(particles.FUNC2(num),TRANSFORM,STATE_EPSILON) == 0) s.init_modify(nodes,particles); \
	set_transform(TRANSFORM); \
	save_state(s,MANIPULATOR ## _m); \
}

/******************************************************************************\
*
* Particles
*
\******************************************************************************/

/*
 */
namespace Particles {
	
	/*
	 */
	WidgetVBox main_vb;						// parameters vbox
	WidgetVBox dynamic_vb;					// dynamic vbox
	WidgetVBox forces_vb;					// forces vbox
	WidgetVBox noises_vb;					// noises vbox
	WidgetVBox deflectors_vb;				// deflectors vbox
	
	WidgetLabel num_particles_l;			// number of particles label
	
	WidgetComboBox particles_type_cb;		// parameters
	WidgetCheckBox warming_cb;
	WidgetCheckBox depth_sort_cb;
	WidgetCheckBox variation_x_cb;
	WidgetCheckBox variation_y_cb;
	WidgetCheckBox texture_atlas_cb;
	WidgetCheckBox intersection_cb;
	WidgetCheckBox collision_cb;
	WidgetCheckBox culling_cb;
	
	WidgetEditLine spawn_rate_el;
	WidgetSlider spawn_rate_sl;
	WidgetEditLine spawn_threshold_el;
	WidgetSlider spawn_threshold_sl;
	WidgetEditLine physical_mask_el;
	WidgetEditLine physical_mass_el;
	WidgetSlider physical_mass_sl;
	WidgetEditLine length_stretch_el;
	WidgetSlider length_stretch_sl;
	WidgetEditLine length_flattening_el;
	WidgetSlider length_flattening_sl;
	WidgetEditLine linear_damping_el;
	WidgetSlider linear_damping_sl;
	WidgetEditLine angular_damping_el;
	WidgetSlider angular_damping_sl;
	WidgetEditLine growth_damping_el;
	WidgetSlider growth_damping_sl;
	WidgetEditLine restitution_el;
	WidgetSlider restitution_sl;
	WidgetEditLine roughness_el;
	WidgetSlider roughness_sl;
	
	WidgetComboBox emitter_type_cb;			// emitter
	WidgetEditLine emitter_sequence_el;
	WidgetCheckBox emitter_enabled_cb;
	WidgetCheckBox emitter_based_cb;
	WidgetCheckBox emitter_shift_cb;
	WidgetCheckBox emitter_continuous_cb;
	WidgetCheckBox emitter_sync_cb;
	WidgetEditLine emitter_limit_el;
	WidgetEditLine emitter_size_el[3];
	WidgetEditLine emitter_direction_el[3];
	WidgetEditLine emitter_spread_el[3];
	
	WidgetEditLine gravity_el[3];			// gravity
	
	WidgetEditLine delay_mean_el;			// dynamic
	WidgetEditLine delay_spread_el;
	WidgetEditLine period_mean_el;
	WidgetEditLine period_spread_el;
	WidgetEditLine duration_mean_el;
	WidgetEditLine duration_spread_el;
	WidgetEditLine life_mean_el;
	WidgetEditLine life_spread_el;
	WidgetEditLine velocity_mean_el;
	WidgetEditLine velocity_spread_el;
	WidgetEditLine angle_mean_el;
	WidgetEditLine angle_spread_el;
	WidgetEditLine rotation_mean_el;
	WidgetEditLine rotation_spread_el;
	WidgetEditLine radius_mean_el;
	WidgetEditLine radius_spread_el;
	WidgetEditLine growth_mean_el;
	WidgetEditLine growth_spread_el;
	
	ObjectParticles particles;
	
	/**************************************************************************\
	*
	* Force
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Force {
		
		/*
		 */
		WidgetListBox forces_lb;		// forces
		WidgetButton remove_b;
		WidgetCheckBox attached_cb;
		WidgetEditLine position_el[3];
		WidgetEditLine rotation_el[3];
		WidgetEditLine radius_el;
		WidgetEditLine attenuation_el;
		WidgetEditLine attractor_el;
		WidgetSlider attractor_sl;
		WidgetEditLine rotator_el;
		WidgetSlider rotator_sl;
		
		/*
		 */
		int num = -1;
		WidgetManipulatorTranslator translator_m;
		WidgetManipulatorRotator rotator_m;
		
		/*
		 */
		void shutdown() {
			if(translator_m != NULL) gui.removeChild(translator_m);
			if(rotator_m != NULL) gui.removeChild(rotator_m);
		}
		
		/*
		 */
		void update_transform() {
			
			vec3 position,rotation;
			decomposeTransform(particles.getForceTransform(num),position,rotation);
			
			setEditLineVec3(position_el,position);
			setEditLineVec3(rotation_el,rotation);
		}
		
		void set_transform(mat4 transform) {
			
			particles.setForceTransform(num,transform);
			update_transform();
		}
		
		/*
		 */
		void update_view() {
			
			attached_cb.setCallbackEnabled(GUI_CLICKED,0);
			
			// clear view
			if(num < 0 || num >= particles.getNumForces()) {
				
				setEnabled(0,(
					attached_cb,
					position_el[0],
					position_el[1],
					position_el[2],
					rotation_el[0],
					rotation_el[1],
					rotation_el[2],
					radius_el,
					attenuation_el,
					attractor_el,
					rotator_el,
				));
				
				attached_cb.setChecked(0);
				
				forloop(int i = 0; 3) {
					position_el[i].setText("");
					rotation_el[i].setText("");
				}
				
				radius_el.setText("");
				attenuation_el.setText("");
				attractor_el.setText("");
				rotator_el.setText("");
			}
			// fill view
			else {
				
				setEnabled(1,(
					attached_cb,
					position_el[0],
					position_el[1],
					position_el[2],
					rotation_el[0],
					rotation_el[1],
					rotation_el[2],
					radius_el,
					attenuation_el,
					attractor_el,
					rotator_el,
				));
				
				attached_cb.setChecked(particles.isForceAttached(num));
				update_transform();
				radius_el.setText(editorFormat(particles.getForceRadius(num)));
				attenuation_el.setText(editorFormat(particles.getForceAttenuation(num)));
				attractor_el.setText(editorFormat(particles.getForceAttractor(num)));
				rotator_el.setText(editorFormat(particles.getForceRotator(num)));
			}
			
			attached_cb.setCallbackEnabled(GUI_CLICKED,1);
		}
		
		/*
		 */
		void update() {
			
			num = -1;
			
			forces_lb.clear();
			forloop(int i = 0; particles.getNumForces()) {
				forces_lb.addItem(format("force_%d",i));
			}
			forces_lb.setCurrentItem(-1);
			
			if(particles.getNumForces()) {
				remove_b.setEnabled(1);
				forces_lb.setCurrentItem(0);
			} else {
				remove_b.setEnabled(0);
				update_view();
			}
		}
		
		/*
		 */
		void add_clicked() {
			
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,object);
			
			particles.addForce();
			
			save_state(s);
			
			update();
		}
		
		void remove_clicked() {
			
			if(dialogMessageYesNo(TR("Confirm"),format(TR("Remove force %d from the %s?"),num,node.getTypeName()))) {
				
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,object);
				
				particles.removeForce(num);
				
				save_state(s);
				
				update();
			}
		}
		
		/*
		 */
		void force_changed() {
			
			num = forces_lb.getCurrentItem();
			update_view();
		}
		
		/*
		 */
		void attach_clicked() {
			
			if(num == -1) return;
			if(particles.isForceAttached(num) == attached_cb.isChecked()) return;
			
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,object);
			
			mat4 transform;
			if(particles.isForceAttached(num)) {
				transform = particles.getWorldTransform() * particles.getForceTransform(num);
			} else {
				transform = particles.getIWorldTransform() * particles.getForceTransform(num);
			}
			
			particles.setForceAttached(num,attached_cb.isChecked());
			
			set_transform(transform);
			
			save_state(s);
		}
		
		void transform_pressed() {
			if(num == -1) return;
			NodesState_ s = new NodesState_();
			mat4 transform = composeTransform(getEditLineVec3(position_el),getEditLineVec3(rotation_el));
			if(compare(particles.getForceTransform(num),transform,STATE_EPSILON) == 0) s.init_modify(nodes,particles);
			set_transform(transform);
			save_state(s);
		}
		
		/*
		 */
		PARTICLES_PRESSED_CALLBACK(radius,ForceRadius)
		PARTICLES_PRESSED_CALLBACK(attenuation,ForceAttenuation)
		PARTICLES_PRESSED_CALLBACK_SLIDER(attractor,ForceAttractor)
		PARTICLES_PRESSED_CALLBACK_SLIDER(rotator,ForceRotator)
		PARTICLES_MANIPULATOR_CHANGED(translator,isForceAttached,getForceTransform,transform)
		PARTICLES_MANIPULATOR_CHANGED(rotator,isForceAttached,getForceTransform,transform)
		
		/*
		 */
		void update_dynamic() {
			
			// current force
			if(parameters_tb.getCurrentTab() == 4 && num >= 0 && num < particles.getNumForces()) {
				
				// create manipulators
				if(translator_m == NULL) {
					translator_m = new WidgetManipulatorTranslator(gui);
					rotator_m = new WidgetManipulatorRotator(gui);
					translator_m.setCallback(GUI_CHANGED,functionid(translator_changed));
					translator_m.setCallback(GUI_CLICKED,functionid(slider_clicked));
					rotator_m.setCallback(GUI_CHANGED,functionid(rotator_changed));
					rotator_m.setCallback(GUI_CLICKED,functionid(slider_clicked));
				}
				
				// translator
				if(panelPlacementGetTranslator()) {
					updateAuxManipulator(translator_m);
					if(translator_m.isFocused() == 0) {
						mat4 transform = particles.getForceTransform(num);
						if(particles.isForceAttached(num)) transform = particles.getWorldTransform() * transform;
						translator_m.setCallbackEnabled(GUI_CHANGED,0);
						translator_m.setBasis(transform);
						translator_m.setTransform(transform);
						translator_m.setCallbackEnabled(GUI_CHANGED,1);
					}
					if(editorIsChild(translator_m) == 0) {
						gui.addChild(translator_m,GUI_ALIGN_OVERLAP);
						gui.removeChild(rotator_m);
					}
				}
				// rotator
				else if(panelPlacementGetRotator()) {
					updateAuxManipulator(rotator_m);
					if(rotator_m.isFocused() == 0) {
						mat4 transform = particles.getForceTransform(num);
						if(particles.isForceAttached(num)) transform = particles.getWorldTransform() * transform;
						rotator_m.setCallbackEnabled(GUI_CHANGED,0);
						rotator_m.setBasis(transform);
						rotator_m.setTransform(transform);
						rotator_m.setCallbackEnabled(GUI_CHANGED,1);
					}
					if(editorIsChild(rotator_m) == 0) {
						gui.addChild(rotator_m,GUI_ALIGN_OVERLAP);
						gui.removeChild(translator_m);
					}
				}
				else {
					gui.removeChild(translator_m);
					gui.removeChild(rotator_m);
				}
			}
			else if(translator_m != NULL) {
				gui.removeChild(translator_m);
			}
		}
		
	} /* namespace Force */
	
	/**************************************************************************\
	*
	* Noise
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Noise {
		
		/*
		 */
		WidgetListBox noises_lb;		// noises
		WidgetButton remove_b;
		WidgetCheckBox attached_cb;
		WidgetEditLine position_el[3];
		WidgetEditLine rotation_el[3];
		WidgetEditLine offset_el[3];
		WidgetEditLine step_el[3];
		WidgetEditLine force_el;
		WidgetSlider force_sl;
		WidgetEditLine scale_el;
		WidgetSlider scale_sl;
		WidgetEditLine frequency_el;
		WidgetEditLine size_el;
		
		/*
		 */
		int num = -1;
		WidgetManipulatorTranslator translator_m;
		WidgetManipulatorRotator rotator_m;
		
		/*
		 */
		void shutdown() {
			if(translator_m != NULL) gui.removeChild(translator_m);
			if(rotator_m != NULL) gui.removeChild(rotator_m);
		}
		
		/*
		 */
		void update_transform() {
			
			vec3 position,rotation;
			decomposeTransform(particles.getNoiseTransform(num),position,rotation);
			
			setEditLineVec3(position_el,position);
			setEditLineVec3(rotation_el,rotation);
		}
		
		void set_transform(mat4 transform) {
			
			particles.setNoiseTransform(num,transform);
			update_transform();
		}
		
		/*
		 */
		void update_view() {
			
			attached_cb.setCallbackEnabled(GUI_CLICKED,0);
			
			// clear view
			if(num < 0 || num >= particles.getNumNoises()) {
				
				setEnabled(0,(
					attached_cb,
					position_el[0],
					position_el[1],
					position_el[2],
					rotation_el[0],
					rotation_el[1],
					rotation_el[2],
					offset_el[0],
					offset_el[1],
					offset_el[2],
					step_el[0],
					step_el[1],
					step_el[2],
					force_el,
					scale_sl,
					frequency_el,
					size_el,
				));
				
				attached_cb.setChecked(0);
				
				forloop(int i = 0; 3) {
					position_el[i].setText("");
					rotation_el[i].setText("");
					offset_el[i].setText("");
					step_el[i].setText("");
				}
				
				force_el.setText("");
				scale_el.setText("");
				frequency_el.setText("");
				size_el.setText("");
			}
			// fill view
			else {
				
				setEnabled(1,(
					attached_cb,
					position_el[0],
					position_el[1],
					position_el[2],
					rotation_el[0],
					rotation_el[1],
					rotation_el[2],
					offset_el[0],
					offset_el[1],
					offset_el[2],
					step_el[0],
					step_el[1],
					step_el[2],
					force_el,
					scale_sl,
					frequency_el,
					size_el,
				));
				
				attached_cb.setChecked(particles.isNoiseAttached(num));
				update_transform();
				setEditLineVec3(offset_el,particles.getNoiseOffset(num));
				setEditLineVec3(step_el,particles.getNoiseStep(num));
				force_el.setText(editorFormat(particles.getNoiseForce(num)));
				scale_el.setText(editorFormat(particles.getNoiseScale(num)));
				frequency_el.setText(string(particles.getNoiseFrequency(num)));
				size_el.setText(string(particles.getNoiseSize(num)));
			}
			
			attached_cb.setCallbackEnabled(GUI_CLICKED,1);
		}
		
		/*
		 */
		void update() {
			
			num = -1;
			
			noises_lb.clear();
			forloop(int i = 0; particles.getNumNoises()) {
				noises_lb.addItem(format("noise_%d",i));
			}
			noises_lb.setCurrentItem(-1);
			
			if(particles.getNumNoises()) {
				remove_b.setEnabled(1);
				noises_lb.setCurrentItem(0);
			} else {
				remove_b.setEnabled(0);
				update_view();
			}
		}
		
		/*
		 */
		void add_clicked() {
			
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,object);
			
			particles.addNoise();
			
			save_state(s);
			
			update();
		}
		
		void remove_clicked() {
			
			if(dialogMessageYesNo(TR("Confirm"),format(TR("Remove noise %d from the %s?"),num,node.getTypeName()))) {
				
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,object);
				
				particles.removeNoise(num);
				
				save_state(s);
				
				update();
			}
		}
		
		/*
		 */
		void noise_changed() {
			
			num = noises_lb.getCurrentItem();
			update_view();
		}
		
		/*
		 */
		void attach_clicked() {
			
			if(num == -1) return;
			if(particles.isNoiseAttached(num) == attached_cb.isChecked()) return;
			
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,object);
			
			mat4 transform;
			if(particles.isNoiseAttached(num)) {
				transform = particles.getWorldTransform() * particles.getNoiseTransform(num);
			} else {
				transform = particles.getIWorldTransform() * particles.getNoiseTransform(num);
			}
			
			particles.setNoiseAttached(num,attached_cb.isChecked());
			
			set_transform(transform);
			
			save_state(s);
		}
		
		void transform_pressed() {
			if(num == -1) return;
			NodesState_ s = new NodesState_();
			mat4 transform = composeTransform(getEditLineVec3(position_el),getEditLineVec3(rotation_el));
			if(compare(particles.getNoiseTransform(num),transform,STATE_EPSILON) == 0) s.init_modify(nodes,particles);
			set_transform(transform);
			save_state(s);
		}
		
		/*
		 */
		PARTICLES_PRESSED_CALLBACK_VEC3(offset,NoiseOffset)
		PARTICLES_PRESSED_CALLBACK_VEC3(step,NoiseStep)
		PARTICLES_PRESSED_CALLBACK_SLIDER(force,NoiseForce)
		PARTICLES_PRESSED_CALLBACK_SLIDER(scale,NoiseScale)
		PARTICLES_PRESSED_CALLBACK_INT(frequency,NoiseFrequency)
		PARTICLES_PRESSED_CALLBACK_INT(size,NoiseSize)
		PARTICLES_MANIPULATOR_CHANGED(translator,isNoiseAttached,getNoiseTransform,transform)
		PARTICLES_MANIPULATOR_CHANGED(rotator,isNoiseAttached,getNoiseTransform,transform)
		
		/*
		 */
		void update_dynamic() {
			
			// current noise
			if(parameters_tb.getCurrentTab() == 5 && num >= 0 && num < particles.getNumNoises()) {
				
				// create manipulators
				if(translator_m == NULL) {
					translator_m = new WidgetManipulatorTranslator(gui);
					rotator_m = new WidgetManipulatorRotator(gui);
					translator_m.setCallback(GUI_CHANGED,functionid(translator_changed));
					translator_m.setCallback(GUI_CLICKED,functionid(slider_clicked));
					rotator_m.setCallback(GUI_CHANGED,functionid(rotator_changed));
					rotator_m.setCallback(GUI_CLICKED,functionid(slider_clicked));
				}
				
				// translator
				if(panelPlacementGetTranslator()) {
					updateAuxManipulator(translator_m);
					if(translator_m.isFocused() == 0) {
						mat4 transform = particles.getNoiseTransform(num);
						if(particles.isNoiseAttached(num)) transform = particles.getWorldTransform() * transform;
						translator_m.setCallbackEnabled(GUI_CHANGED,0);
						translator_m.setBasis(transform);
						translator_m.setTransform(transform);
						translator_m.setCallbackEnabled(GUI_CHANGED,1);
					}
					if(editorIsChild(translator_m) == 0) {
						gui.addChild(translator_m,GUI_ALIGN_OVERLAP);
						gui.removeChild(rotator_m);
					}
				}
				// rotator
				else if(panelPlacementGetRotator()) {
					updateAuxManipulator(rotator_m);
					if(rotator_m.isFocused() == 0) {
						mat4 transform = particles.getNoiseTransform(num);
						if(particles.isNoiseAttached(num)) transform = particles.getWorldTransform() * transform;
						rotator_m.setCallbackEnabled(GUI_CHANGED,0);
						rotator_m.setBasis(transform);
						rotator_m.setTransform(transform);
						rotator_m.setCallbackEnabled(GUI_CHANGED,1);
					}
					if(editorIsChild(rotator_m) == 0) {
						gui.addChild(rotator_m,GUI_ALIGN_OVERLAP);
						gui.removeChild(translator_m);
					}
				}
				else {
					gui.removeChild(translator_m);
					gui.removeChild(rotator_m);
				}
			}
			else if(translator_m != NULL) {
				gui.removeChild(translator_m);
			}
		}
		
	} /* namespace Noise */
	
	/**************************************************************************\
	*
	* Deflector
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Deflector {
		
		/*
		 */
		WidgetListBox deflectors_lb;			// deflectors
		WidgetButton remove_b;
		WidgetComboBox type_cb;
		WidgetCheckBox attached_cb;
		WidgetEditLine position_el[3];
		WidgetEditLine rotation_el[3];
		WidgetEditLine size_el[3];
		WidgetEditLine restitution_el;
		WidgetSlider restitution_sl;
		WidgetEditLine roughness_el;
		WidgetSlider roughness_sl;
		
		/*
		 */
		int num = -1;
		WidgetManipulatorTranslator translator_m;
		WidgetManipulatorRotator rotator_m;
		
		/*
		 */
		void shutdown() {
			if(translator_m != NULL) gui.removeChild(translator_m);
			if(rotator_m != NULL) gui.removeChild(rotator_m);
		}
		
		/*
		 */
		void update_transform() {
			
			vec3 position,rotation;
			decomposeTransform(particles.getDeflectorTransform(num),position,rotation);
			
			setEditLineVec3(position_el,position);
			setEditLineVec3(rotation_el,rotation);
		}
		
		void set_transform(mat4 transform) {
			
			particles.setDeflectorTransform(num,transform);
			update_transform();
		}
		
		/*
		 */
		void update_view() {
			
			attached_cb.setCallbackEnabled(GUI_CLICKED,0);
			
			// clear view
			if(num < 0 || num >= particles.getNumDeflectors()) {
				
				setEnabled(0,(
					type_cb,
					attached_cb,
					position_el[0],
					position_el[1],
					position_el[2],
					rotation_el[0],
					rotation_el[1],
					rotation_el[2],
					size_el[0],
					size_el[1],
					restitution_el,
					roughness_el,
				));
				
				type_cb.setCurrentItem(0);
				attached_cb.setChecked(0);
				
				forloop(int i = 0; 3) {
					position_el[i].setText("");
					rotation_el[i].setText("");
					size_el[i].setText("");
				}
				
				restitution_el.setText("");
				roughness_el.setText("");
			}
			// fill view
			else {
				
				setEnabled(1,(
					type_cb,
					attached_cb,
					position_el[0],
					position_el[1],
					position_el[2],
					rotation_el[0],
					rotation_el[1],
					rotation_el[2],
					size_el[0],
					size_el[1],
					restitution_el,
					roughness_el,
				));
				
				type_cb.setCurrentItem(particles.getDeflectorType(num));
				attached_cb.setChecked(particles.isDeflectorAttached(num));
				update_transform();
				setEditLineVec3(size_el,particles.getDeflectorSize(num));
				restitution_el.setText(editorFormat(particles.getDeflectorRestitution(num)));
				roughness_el.setText(editorFormat(particles.getDeflectorRoughness(num)));
			}
			
			attached_cb.setCallbackEnabled(GUI_CLICKED,1);
		}
		
		/*
		 */
		void update() {
			
			num = -1;
			
			deflectors_lb.clear();
			forloop(int i = 0; particles.getNumDeflectors()) {
				deflectors_lb.addItem(format("deflector_%d",i));
			}
			deflectors_lb.setCurrentItem(-1);
			
			if(particles.getNumDeflectors()) {
				remove_b.setEnabled(1);
				deflectors_lb.setCurrentItem(0);
			} else {
				remove_b.setEnabled(0);
				update_view();
			}
		}
		
		/*
		 */
		void add_clicked() {
			
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,object);
			
			particles.addDeflector();
			
			save_state(s);
			
			update();
		}
		
		void remove_clicked() {
			
			if(dialogMessageYesNo(TR("Confirm"),format(TR("Remove deflector %d from the %s?"),num,node.getTypeName()))) {
				
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,object);
				
				particles.removeDeflector(num);
				
				save_state(s);
				
				update();
			}
		}
		
		/*
		 */
		void deflector_changed() {
			
			num = deflectors_lb.getCurrentItem();
			update_view();
		}
		
		/*
		 */
		void type_changed() {
			
			if(num == -1) return;
			
			NodesState_ s = new NodesState_();
			if(particles.getDeflectorType(num) != type_cb.getCurrentItem()) s.init_modify(nodes,particles);
			
			particles.setDeflectorType(num,type_cb.getCurrentItem());
			
			int type = particles.getDeflectorType(num);
			setEnabled(type == OBJECT_PARTICLES_DEFLECTOR_REFLECTOR,(
				restitution_el,
				roughness_el,
			));
			
			save_state(s);
		}
		
		void attach_clicked() {
			
			if(num == -1) return;
			if(particles.isDeflectorAttached(num) == attached_cb.isChecked()) return;
			
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,object);
			
			mat4 transform;
			if(particles.isDeflectorAttached(num)) {
				transform = particles.getWorldTransform() * particles.getDeflectorTransform(num);
			} else {
				transform = particles.getIWorldTransform() * particles.getDeflectorTransform(num);
			}
			
			particles.setDeflectorAttached(num,attached_cb.isChecked());
			
			set_transform(transform);
			
			save_state(s);
		}
		
		void transform_pressed() {
			if(num == -1) return;
			NodesState_ s = new NodesState_();
			mat4 transform = composeTransform(getEditLineVec3(position_el),getEditLineVec3(rotation_el));
			if(compare(particles.getDeflectorTransform(num),transform,STATE_EPSILON) == 0) s.init_modify(nodes,particles);
			set_transform(transform);
			save_state(s);
		}
		
		/*
		 */
		PARTICLES_PRESSED_CALLBACK_VEC3(size,DeflectorSize)
		PARTICLES_PRESSED_CALLBACK_SLIDER(restitution,DeflectorRestitution)
		PARTICLES_PRESSED_CALLBACK_SLIDER(roughness,DeflectorRoughness)
		PARTICLES_MANIPULATOR_CHANGED(translator,isDeflectorAttached,getDeflectorTransform,transform)
		PARTICLES_MANIPULATOR_CHANGED(rotator,isDeflectorAttached,getDeflectorTransform,transform)
		
		/*
		 */
		void update_dynamic() {
			
			// current deflector
			if(parameters_tb.getCurrentTab() == 6 && num >= 0 && num < particles.getNumDeflectors()) {
				
				// create manipulators
				if(translator_m == NULL) {
					translator_m = new WidgetManipulatorTranslator(gui);
					rotator_m = new WidgetManipulatorRotator(gui);
					translator_m.setCallback(GUI_CHANGED,functionid(translator_changed));
					translator_m.setCallback(GUI_CLICKED,functionid(slider_clicked));
					rotator_m.setCallback(GUI_CHANGED,functionid(rotator_changed));
					rotator_m.setCallback(GUI_CLICKED,functionid(slider_clicked));
				}
				
				// translator
				if(panelPlacementGetTranslator()) {
					updateAuxManipulator(translator_m);
					if(translator_m.isFocused() == 0) {
						mat4 transform = particles.getDeflectorTransform(num);
						if(particles.isDeflectorAttached(num)) transform = particles.getWorldTransform() * transform;
						translator_m.setCallbackEnabled(GUI_CHANGED,0);
						translator_m.setBasis(transform);
						translator_m.setTransform(transform);
						translator_m.setCallbackEnabled(GUI_CHANGED,1);
					}
					if(editorIsChild(translator_m) == 0) {
						gui.addChild(translator_m,GUI_ALIGN_OVERLAP);
						gui.removeChild(rotator_m);
					}
				}
				// rotator
				else if(panelPlacementGetRotator()) {
					updateAuxManipulator(rotator_m);
					if(rotator_m.isFocused() == 0) {
						mat4 transform = particles.getDeflectorTransform(num);
						if(particles.isDeflectorAttached(num)) transform = particles.getWorldTransform() * transform;
						rotator_m.setCallbackEnabled(GUI_CHANGED,0);
						rotator_m.setBasis(transform);
						rotator_m.setTransform(transform);
						rotator_m.setCallbackEnabled(GUI_CHANGED,1);
					}
					if(editorIsChild(rotator_m) == 0) {
						gui.addChild(rotator_m,GUI_ALIGN_OVERLAP);
						gui.removeChild(translator_m);
					}
				}
				else {
					gui.removeChild(translator_m);
					gui.removeChild(rotator_m);
				}
			}
			else if(translator_m != NULL) {
				gui.removeChild(translator_m);
				gui.removeChild(rotator_m);
			}
		}
		
	} /* namespace Deflector */
	
	/**************************************************************************\
	*
	* Particles
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("Params"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
		
		parameters_tb.addTab(TR("Dynamic"));
		parameters_tb.addChild(dynamic_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(dynamic_vb);
		
		parameters_tb.addTab(TR("Forces"));
		parameters_tb.addChild(forces_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(forces_vb);
		
		parameters_tb.addTab(TR("Noises"));
		parameters_tb.addChild(noises_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(noises_vb);
		
		parameters_tb.addTab(TR("Deflectors"));
		parameters_tb.addChild(deflectors_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(deflectors_vb);
	}
	
	void shutdown() {
		
		Nodes::Object::shutdown();
		
		Force::shutdown();
		
		Noise::shutdown();
		
		Deflector::shutdown();
	}
	
	/*
	 */
	void update_particles_controls(int type) {
		
		if(type == OBJECT_PARTICLES_TYPE_BILLBOARD || type == OBJECT_PARTICLES_TYPE_FLAT || type == OBJECT_PARTICLES_TYPE_RANDOM || type == OBJECT_PARTICLES_TYPE_ROUTE || type == OBJECT_PARTICLES_TYPE_CHAIN) {
			setEnabled(1,(
				angle_mean_el,
				angle_spread_el,
				rotation_mean_el,
				rotation_spread_el,
			));
			setEnabled(0,(
				length_stretch_el,
				length_flattening_el,
			));
		}
		else if(type == OBJECT_PARTICLES_TYPE_POINT) {
			setEnabled(0,(
				angle_mean_el,
				angle_spread_el,
				rotation_mean_el,
				rotation_spread_el,
			));
			setEnabled(0,(
				length_stretch_el,
				length_flattening_el,
			));
		}
		else if(type == OBJECT_PARTICLES_TYPE_LENGTH) {
			setEnabled(0,(
				angle_mean_el,
				angle_spread_el,
				rotation_mean_el,
				rotation_spread_el,
			));
			setEnabled(1,(
				length_stretch_el,
				length_flattening_el,
			));
		}
		else {
			throw("Nodes::Object::Particles::update_particles_controls(): unknown particles type\n");
		}
	}
	
	/*
	 */
	void update_emitter_controls(int type) {
		
		if(type == OBJECT_PARTICLES_EMITTER_POINT) {
			setEnabled(1,emitter_size_el);
		}
		else if(type == OBJECT_PARTICLES_EMITTER_SPHERE) {
			emitter_size_el[0].setEnabled(1);
			emitter_size_el[1].setEnabled(0);
			emitter_size_el[2].setEnabled(0);
		}
		else if(type == OBJECT_PARTICLES_EMITTER_CYLINDER) {
			emitter_size_el[0].setEnabled(1);
			emitter_size_el[1].setEnabled(1);
			emitter_size_el[2].setEnabled(0);
		}
		else if(type == OBJECT_PARTICLES_EMITTER_BOX) {
			setEnabled(1,emitter_size_el);
		}
		else if(type == OBJECT_PARTICLES_EMITTER_RANDOM) {
			setEnabled(0,emitter_size_el);
		}
		else if(type == OBJECT_PARTICLES_EMITTER_SPARK) {
			setEnabled(0,emitter_size_el);
		}
		else {
			throw("Nodes::Object::Particles::update_emitter_controls(): unknown emitter type\n");
		}
	}
	
	/*
	 */
	void update() {
		
		Nodes::Object::update();
		
		particles = node_cast(node);
		
		setCallbackEnabled(GUI_CLICKED,0,(
			warming_cb,
			depth_sort_cb,
			variation_x_cb,
			variation_y_cb,
			texture_atlas_cb,
			intersection_cb,
			collision_cb,
			culling_cb,
			emitter_enabled_cb,
			emitter_based_cb,
			emitter_shift_cb,
			emitter_continuous_cb,
			emitter_sync_cb,
		));
		
		setCallbackEnabled(GUI_CHANGED,0,(
			particles_type_cb,
			emitter_type_cb,
		));
		
		particles_type_cb.setCurrentItem(particles.getParticlesType());
		update_particles_controls(particles.getParticlesType());
		
		// parameters
		particles_type_cb.runCallback(GUI_CHANGED);
		warming_cb.setChecked(particles.getWarming());
		depth_sort_cb.setChecked(particles.getDepthSort());
		variation_x_cb.setChecked(particles.getVariationX());
		variation_y_cb.setChecked(particles.getVariationY());
		texture_atlas_cb.setChecked(particles.getTextureAtlas());
		intersection_cb.setChecked(particles.getIntersection());
		collision_cb.setChecked(particles.getCollision());
		culling_cb.setChecked(particles.getCulling());
		
		spawn_rate_el.setText(editorFormat(particles.getSpawnRate()));
		spawn_threshold_el.setText(editorFormat(particles.getSpawnThreshold()));
		physical_mask_el.setText(editorFormatMask(particles.getPhysicalMask()));
		physical_mass_el.setText(editorFormat(particles.getPhysicalMass()));
		length_stretch_el.setText(editorFormat(particles.getLengthStretch()));
		length_flattening_el.setText(editorFormat(particles.getLengthFlattening()));
		linear_damping_el.setText(editorFormat(particles.getLinearDamping()));
		angular_damping_el.setText(editorFormat(particles.getAngularDamping()));
		growth_damping_el.setText(editorFormat(particles.getGrowthDamping()));
		restitution_el.setText(editorFormat(particles.getRestitution()));
		roughness_el.setText(editorFormat(particles.getRoughness()));
		
		// emitter
		emitter_type_cb.setCurrentItem(particles.getEmitterType());
		update_emitter_controls(particles.getEmitterType());
		emitter_type_cb.runCallback(GUI_CHANGED);
		emitter_sequence_el.setText(string(particles.getEmitterSequence()));
		emitter_enabled_cb.setChecked(particles.isEmitterEnabled());
		emitter_based_cb.setChecked(particles.isEmitterBased());
		emitter_shift_cb.setChecked(particles.isEmitterShift());
		emitter_continuous_cb.setChecked(particles.isEmitterContinuous());
		emitter_sync_cb.setChecked(particles.getEmitterSync());
		emitter_limit_el.setText(string(particles.getEmitterLimit()));
		setEditLineVec3(emitter_size_el,particles.getEmitterSize());
		setEditLineVec3(emitter_direction_el,particles.getEmitterDirection());
		setEditLineVec3(emitter_spread_el,particles.getEmitterSpread());
		
		// gravity
		setEditLineVec3(gravity_el,particles.getGravity());
		
		// parameters
		delay_mean_el.setText(editorFormat(particles.getDelayMean()));
		delay_spread_el.setText(editorFormat(particles.getDelaySpread()));
		period_mean_el.setText(editorFormat(particles.getPeriodMean()));
		period_spread_el.setText(editorFormat(particles.getPeriodSpread()));
		duration_mean_el.setText(editorFormat(particles.getDurationMean()));
		duration_spread_el.setText(editorFormat(particles.getDurationSpread()));
		life_mean_el.setText(editorFormat(particles.getLifeMean()));
		life_spread_el.setText(editorFormat(particles.getLifeSpread()));
		velocity_mean_el.setText(editorFormat(particles.getVelocityMean()));
		velocity_spread_el.setText(editorFormat(particles.getVelocitySpread()));
		angle_mean_el.setText(editorFormat(particles.getAngleMean()));
		angle_spread_el.setText(editorFormat(particles.getAngleSpread()));
		rotation_mean_el.setText(editorFormat(particles.getRotationMean()));
		rotation_spread_el.setText(editorFormat(particles.getRotationSpread()));
		radius_mean_el.setText(editorFormat(particles.getRadiusMean()));
		radius_spread_el.setText(editorFormat(particles.getRadiusSpread()));
		growth_mean_el.setText(editorFormat(particles.getGrowthMean()));
		growth_spread_el.setText(editorFormat(particles.getGrowthSpread()));
		
		setCallbackEnabled(GUI_CLICKED,1,(
			warming_cb,
			depth_sort_cb,
			variation_x_cb,
			variation_y_cb,
			texture_atlas_cb,
			intersection_cb,
			collision_cb,
			culling_cb,
			emitter_enabled_cb,
			emitter_based_cb,
			emitter_shift_cb,
			emitter_continuous_cb,
			emitter_sync_cb,
		));
		
		setCallbackEnabled(GUI_CHANGED,1,(
			particles_type_cb,
			emitter_type_cb,
		));
		
		// forces
		Force::update();
		
		// noises
		Noise::update();
		
		// deflectors
		Deflector::update();
	}
	
	/*
	 */
	void render_visualizer(ObjectParticles node) {
		if(helpersGetParticlesPhysics() && (Force::num != -1 || Noise::num != -1 || Deflector::num != -1)) node.renderVisualizer();
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
		
		// number of particles
		num_particles_l.setText(string(particles.getNumParticles()));
		
		// forces
		Force::update_dynamic();
		
		// noises
		Noise::update_dynamic();
		
		// deflectors
		Deflector::update_dynamic();
	}
	
	/*
	 */
	void particles_type_changed() {
		NodesState_ s = new NodesState_();
		int type = particles_type_cb.getCurrentItem();
		if(type != particles.getParticlesType()) s.init_modify(nodes,particles);
		foreach(Node n; nodes) {
			if(n.getType() != NODE_OBJECT_PARTICLES) continue;
			ObjectParticles(node_cast(n)).setParticlesType(type);
		}
		update_particles_controls(type);
		save_state(s);
	}
	
	/*
	 */
	CLICKED_CALLBACK(warming,particles,Warming,particles)
	CLICKED_CALLBACK(depth_sort,particles,DepthSort,particles)
	CLICKED_CALLBACK(variation_x,particles,VariationX,particles)
	CLICKED_CALLBACK(variation_y,particles,VariationY,particles)
	CLICKED_CALLBACK(texture_atlas,particles,TextureAtlas,particles)
	CLICKED_CALLBACK(intersection,particles,Intersection,particles)
	CLICKED_CALLBACK(collision,particles,Collision,particles)
	CLICKED_CALLBACK(culling,particles,Culling,particles)
	CLICKED_CALLBACK(emitter_enabled,particles,EmitterEnabled,particles)
	CLICKED_CALLBACK(emitter_based,particles,EmitterBased,particles)
	CLICKED_CALLBACK(emitter_shift,particles,EmitterShift,particles)
	CLICKED_CALLBACK(emitter_continuous,particles,EmitterContinuous,particles)
	CLICKED_CALLBACK(emitter_sync,particles,EmitterSync,particles)
	
	PRESSED_CALLBACK_SLIDER(spawn_rate,particles,SpawnRate,particles,modify)
	PRESSED_CALLBACK_SLIDER(spawn_threshold,particles,SpawnThreshold,particles,modify)
	PRESSED_CALLBACK_MASK(physical_mask,particles,PhysicalMask,particles)
	CLICKED_CALLBACK_MASK(physical_mask,particles,PhysicalMask,particles,TR("Particles physical mask"),"Nodes::Object::Particles::physical_mask_changed")
	PRESSED_CALLBACK_SLIDER(physical_mass,particles,PhysicalMass,particles,modify)
	PRESSED_CALLBACK_SLIDER(length_stretch,particles,LengthStretch,particles,modify)
	PRESSED_CALLBACK_SLIDER(length_flattening,particles,LengthFlattening,particles,modify)
	PRESSED_CALLBACK_SLIDER(linear_damping,particles,LinearDamping,particles,modify)
	PRESSED_CALLBACK_SLIDER(angular_damping,particles,AngularDamping,particles,modify)
	PRESSED_CALLBACK_SLIDER(growth_damping,particles,GrowthDamping,particles,modify)
	PRESSED_CALLBACK_SLIDER(restitution,particles,Restitution,particles,modify)
	PRESSED_CALLBACK_SLIDER(roughness,particles,Roughness,particles,modify)
	
	/*
	 */
	void physical_mask_changed(int mask) {
		particles.setPhysicalMask(mask);
		physical_mask_el.setText(editorFormatMask(particles.getPhysicalMask()));
	}
	
	/*
	 */
	void emitter_type_changed() {
		NodesState_ s = new NodesState_();
		int type = emitter_type_cb.getCurrentItem();
		if(type != particles.getEmitterType()) s.init_modify(nodes,particles);
		foreach(Node n; nodes) {
			if(n.getType() != NODE_OBJECT_PARTICLES) continue;
			ObjectParticles(node_cast(n)).setEmitterType(type);
		}
		update_emitter_controls(type);
		save_state(s);
	}
	
	PRESSED_CALLBACK(emitter_sequence,particles,EmitterSequence,particles,int,modify)
	PRESSED_CALLBACK(emitter_limit,particles,EmitterLimit,particles,int,modify)
	PRESSED_CALLBACK_VEC3(emitter_size,particles,EmitterSize,particles,modify)
	PRESSED_CALLBACK_VEC3(emitter_direction,particles,EmitterDirection,particles,modify)
	PRESSED_CALLBACK_VEC3(emitter_spread,particles,EmitterSpread,particles,modify)
	PRESSED_CALLBACK_VEC3(gravity,particles,Gravity,particles,modify)
	PRESSED_CALLBACK_MEAN_SPREAD(delay,particles,Delay)
	PRESSED_CALLBACK_MEAN_SPREAD(period,particles,Period)
	PRESSED_CALLBACK_MEAN_SPREAD(duration,particles,Duration)
	PRESSED_CALLBACK_MEAN_SPREAD(life,particles,Life)
	PRESSED_CALLBACK_MEAN_SPREAD(velocity,particles,Velocity)
	PRESSED_CALLBACK_MEAN_SPREAD(angle,particles,Angle)
	PRESSED_CALLBACK_MEAN_SPREAD(rotation,particles,Rotation)
	PRESSED_CALLBACK_MEAN_SPREAD(radius,particles,Radius)
	PRESSED_CALLBACK_MEAN_SPREAD(growth,particles,Growth)
}
