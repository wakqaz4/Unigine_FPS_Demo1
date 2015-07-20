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


#ifndef __COMBINER_SWITCH_H__
#define __COMBINER_SWITCH_H__

#include <unigine.h>
#include <scripts/character/combiner.h>

namespace Unigine {
	
	/* magic number (M_PI * 1000000), which is used as 'default' id in cases map
	 */
	enum {
		COMBINER_SWITCH_DEFAULT = 3141592,
	};
	
	/******************************************************************************\
	 *
	 * class CombinerSwitchCase
	 *
	\******************************************************************************/
	
	/*
	 */
	class CombinerSwitchCase {
		
		int case_value;
		float blend_speed = 1.0f;
		Combiner combiner;
		
		//
		void load(Xml xml,ObjectMeshSkinned mesh,ParameterManager parameters = NULL) {
			
			if(xml.getName() == "case") {
				case_value = xml.getIntArg("value");
				if(case_value == COMBINER_SWITCH_DEFAULT) throw("CombinerSwitchCase::load(): can not use \"%d\" value for a case. It is reserved as default case value.",COMBINER_SWITCH_DEFAULT);
			} else {
				case_value = COMBINER_SWITCH_DEFAULT;
			}
			
			if(xml.isArg("blend_speed")) blend_speed = xml.getFloatArg("blend_speed");
			
			Xml child = xml.getChild("combiner");
			combiner = loadCombiner(child,mesh,parameters);
		}
		
		//
		void save(Xml xml) {
			
			if(case_value == COMBINER_SWITCH_DEFAULT) {
				xml.setName("default");
			} else {
				xml.setIntArg("value",case_value);
			}
			xml.setFloatArg("blend_speed",blend_speed);
			
			Xml child = xml.addChild("combiner");
			saveCombiner(child,combiner);
		}
		
	};
	
	/******************************************************************************\
	 *
	 * class CombinerSwitch
	 *
	\******************************************************************************/
	
	/*
	 */
	class CombinerSwitch : Combiner {
		
		CombinerSwitchCase cases[];
		CombinerSwitchCase current_state;
		
		//
		CombinerSwitch(ObjectMeshSkinned mesh_) {
			combiner_init(mesh_);
		}
		
		//
		void update(float dt) {
			
			if(isActive() == 0) return;
			
			Input input_case = inputs.getInput("case");
			int next_state = int(input_case.getValue());
			if(cases.check(next_state) == 0) next_state = COMBINER_SWITCH_DEFAULT;
			if(cases.check(next_state) == 0) {
				throw("CombinerSwitch::update(): default state is not found.");
			} else {
				current_state = cases[next_state];
			}
			
			foreach(CombinerSwitchCase cur_case; cases) {
				
				Combiner operand = cur_case.combiner;
				float new_weight = operand.getWeight();
				
				if(cur_case == current_state) {
					new_weight += dt * current_state.blend_speed;
				} else {
					new_weight -= dt * current_state.blend_speed;
				}
				
				// shit happens
				if(new_weight < 0.0f) {
					new_weight = 0.0f;
					operand.onBlendOutFinish();
				} else if(new_weight > 1.0f) {
					new_weight = 1.0f;
					operand.onBlendInFinish();
				}
				
				if(operand.weight < EPSILON && new_weight > EPSILON) {
					operand.onBlendInStart();
				} else if(operand.weight > 1.0f - EPSILON && new_weight < 1.0f - EPSILON) {
					operand.onBlendOutStart();
				}
				
				if(operand.weight != new_weight) operand.setWeight(new_weight);
				if(operand.weight < EPSILON) {
					if(operand.active == 1) operand.setActive(0);
					continue;
				}
				if(operand.active == 0) operand.setActive(1);
				operand.update(dt);
			}
			
			Combiner::update(dt);
			
		}
		
		//
		void setup() {
			
			if(isActive() == 0) return;
			
			float summ_weight = 0.0f;
			
			mesh.clearLayer(layer);
			foreach(CombinerSwitchCase cur_case; cases) {
				
				Combiner operand = cur_case.combiner;
				if(operand.weight < EPSILON) continue;
				operand.setup();
				
				if(summ_weight < EPSILON) {
					summ_weight = operand.weight;
					mesh.copyLayer(layer,operand.layer);
				} else {
					summ_weight += operand.weight;
					float interpolation = operand.weight / summ_weight;
					mesh.lerpLayer(layer,layer,operand.layer,interpolation);
				}
			}
		}
		
		//
		void save(Xml xml) {
			Combiner::save(xml);
			Xml parent = xml.addChild("switch");
			foreach(CombinerSwitchCase cur_case; cases) {
				Xml child = parent.addChild("case");
				cur_case.save(child);
			}
		}
		
		//
		void load(Xml xml,ParameterManager parameters = NULL) {
			
			Combiner::load(xml,parameters);
			cases.clear();
			Xml parent = xml.getChild("switch");
			forloop(int i = 0; parent.getNumChilds()) {
				Xml child = parent.getChild(i);
				if(child.getName() != "case" && child.getName() != "default") continue;
				CombinerSwitchCase cur_case = new CombinerSwitchCase();
				cur_case.load(child,mesh,parameters);
				cases.append(cur_case.case_value,cur_case);
			}
		}
		
		//
		void onDeactivate() {
			Combiner::onDeactivate();
			foreach(CombinerSwitchCase cur_case; cases) {
				cur_case.combiner.setActive(0);
			}
		}
		
		//
		void inputs_init() {
			Combiner::inputs_init();
			inputs.addInput("case",new InputConst(0));
		}
		
	};
	
}

#endif
