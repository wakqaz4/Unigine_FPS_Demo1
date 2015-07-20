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


#ifndef __COMBINER_H__
#define __COMBINER_H__

#include <unigine.h>
#include <scripts/common/parameter.h>
#include <scripts/character/animation_inputs.h>
#include <scripts/character/animation_events.h>

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * class Combiner
	 *
	\******************************************************************************/
	
	int num_active_combiners = 0;
	float combiners_time = 0;
	
	/*
	 */
	class Combiner {
		
		ObjectMeshSkinned mesh;
		InputManager inputs;
		AnimationEventManager events;
		
		int active;
		int layer;
		float weight;
		
		// TODO: connections_init
		void inputs_init() {
			inputs.clear();
			events.clear();
		}
		
		//
		void combiner_init(ObjectMeshSkinned mesh_) {
			mesh = mesh_;
			layer = mesh.addLayer();
			weight = 0.0f;
			active = 0;
			inputs = new InputManager();
			events = new AnimationEventManager();
			inputs_init();
		}
		
		//
		Combiner(ObjectMeshSkinned mesh_) {
			throw("Combiner::Combiner(): can not create instance of the abstract class.");
		}
		
		//
		void update(float dt) {
			if(active == 0) return;
			events.invoke("update");
			num_active_combiners ++;
		}
		
		//
		void setup() {
			throw("Combiner::setup(): call of the abstract method.");
		}
		
		//
		void save(Xml xml) {
			inputs.save(xml);
			events.save(xml);
		}
		
		//
		void load(Xml xml,ParameterManager parameters = NULL) {
			inputs_init();
			inputs.load(xml,parameters);
			events.load(xml,parameters);
		}
		
		//
		float getWeight() {
			return weight;
		}
		
		//
		void setWeight(float weight_) {
			weight = weight_;
		}
		
		//
		void onActivate() {
			events.invoke("activate");
		}
		
		//
		void onDeactivate() {
			events.invoke("deactivate");
		}
		
		//
		void onBlendInStart() {
			events.invoke("blend_in_start");
		}
		
		//
		void onBlendInFinish() {
			events.invoke("blend_in_finish");
		}
		
		//
		void onBlendOutStart() {
			events.invoke("blend_out_start");
		}
		
		//
		void onBlendOutFinish() {
			events.invoke("blend_out_finish");
		}
		
		//
		int isActive() {
			return active;
		}
		
		//
		void setActive(int active_) {
			
			if(active == active_) return;
			
			if(active == 1 && active_ == 0) {
				onDeactivate();
			} else if(active == 0 && active_ == 1) {
				onActivate();
			}
			
			active = active_;
			
		}
		
	};
	
}

#endif
