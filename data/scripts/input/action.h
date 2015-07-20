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


#ifndef __UNIGINE_INPUT_ACTION_H__
#define __UNIGINE_INPUT_ACTION_H__

#include <scripts/input/input.h>

namespace Unigine {
	
	namespace Input {
		
		enum {
			ACTION_TYPE_STATE,
			ACTION_TYPE_SWITCH,
		};
		
		/**************************************************************************\
		 *
		 * class ActionDef
		 *
		\**************************************************************************/
		
		/* Container for action paramters loaded from action_defs.
		 */
		class ActionDef {
			
			string name;
			float min = 0.0f;
			float max = 1.0f;
			float neutral = 0.0f;
			float default_change_speed = INFINITY;
			float default_return_speed = INFINITY;
			
			int type = ACTION_TYPE_STATE;
			int lockable = true;
			
			// Load from xml.
			void load(Xml xml) {
				
				name = xml.getArg("name");
				if(xml.isArg("min")) min = xml.getFloatArg("min");
				if(xml.isArg("max")) max = xml.getFloatArg("max");
				if(xml.isArg("neutral")) neutral = xml.getFloatArg("neutral");
				if(xml.isArg("default_change_speed")) default_change_speed = xml.getFloatArg("default_change_speed");
				if(xml.isArg("default_return_speed")) default_return_speed = xml.getFloatArg("default_return_speed");
				if(xml.isArg("lockable")) lockable = xml.getIntArg("lockable");
				if(xml.isArg("type")) {
					if(xml.getArg("type") == "switch") type = ACTION_TYPE_SWITCH;
					else type = ACTION_TYPE_STATE;
				}
			}
			
			// Save to xml.
			void save(Xml xml) {
				
				xml.setArg("name",name);
				xml.setFloatArg("min",min);
				xml.setFloatArg("max",max);
				xml.setFloatArg("neutral",neutral);
				if(default_change_speed != INFINITY) xml.setFloatArg("default_change_speed",default_change_speed);
				if(default_return_speed != INFINITY) xml.setFloatArg("default_return_speed",default_return_speed);
				xml.setIntArg("lockable",lockable);
				xml.setArg("type",type == ACTION_TYPE_SWITCH ? "switch" : "state");
			}
		};
		
		/**************************************************************************\
		 *
		 * class Action
		 *
		\**************************************************************************/
		
		/* Represents an action caused by one of assigned inputs from the user.
		 */
		class Action {
			
			private:
				
				// Action definition.
				ActionDef def;
				
				// List of inputs that generate this action.
				Input inputs[0];
				
				// Get input with maximum state value.
				Input get_max_input_state() {
					
					if(inputs.size() < 1) return NULL;
					
					// find maximum input value
					// note, that inpu state is normalized and always between [0,1]
					Input max_input = NULL;
					float max_state = -1.0f;
					
					foreach(Input input; inputs) {
						
						// filter out inputs without attached input devices & inputs in netural state
						if(input == NULL || input.getInputDevice() == NULL ||
								input.getState() == input.getNeutralStateValue()) continue;
						
						float state = abs(input.getState() - input.getNeutralStateValue());
						if(max_state < 0 || max_state < state) {
							max_state = state;
							max_input = input;
						}
					}
					
					return max_input;
				}
				
				// Get input with maximum delta value.
				Input get_max_input_delta() {
					
					if(inputs.size() < 1) return NULL;
					
					// find maximum input value
					Input max_input = NULL;
					float max_delta = 0.0f;
					
					foreach(Input input; inputs) {
						
						if(input == NULL) continue;
						
						float delta = abs(input.getStateDelta());
						if(max_delta < delta) {
							max_delta = delta;
							max_input = input;
						}
					}
					
					return max_input;
				}
				
			public:
				
				// Constructor.
				Action() {
					def = new ActionDef();
				}
				
				// Desturctor.
				~Action() {
					delete def;
				}
				
				// Get action type.
				int getType() {
					return def.type;
				}
				
				// Get max, min & neutral values for this action.
				float getMax() {
					return def.max;
				}
				
				float getMin() {
					return def.min;
				}
				
				float getNeutral() {
					return def.neutral;
				}
				
				// Get default change & return speed.
				float getDefaultChangeSpeed() {
					return def.default_change_speed;
				}
				
				float getDefaultReturnSpeed() {
					return def.default_return_speed;
				}
				
				// Get number of inputs.
				int getNumInputs() {
					return inputs.size();
				}
				
				// Get input by its index.
				Input getInput(int index) {
					
					assert(index >=0 && index < inputs.size());
					return inputs[index];
				}
				
				// Remove input with specified index.
				void removeInput(int index) {
					
					assert(index >=0 && index < inputs.size());
					inputs.remove(index);
				}
				
				// Modify input with specified index.
				void setInput(Input input,int index) {
					
					if(input == NULL) {
						removeInput(index);
						return;
					}
					
					if(getDefaultChangeSpeed() != INFINITY) input.setChangeSpeed(getDefaultChangeSpeed());
					if(getDefaultReturnSpeed() != INFINITY) input.setReturnSpeed(getDefaultReturnSpeed());
					if(index < inputs.size()) {
						inputs[index] = input;
					} else {
						inputs.append(input);
					}
				}
				
				// Get action state (returns maximum input state).
				float getState() {
					
					if(def.lockable && (engine.gui.getActivity() == 0 || engine.app.getMouseGrab() == 0)) return getNeutral();
					
					if(def.type == ACTION_TYPE_SWITCH) {
						
						Input max_input = get_max_input_delta();
						if(max_input == NULL) return getNeutral();
						float state_delta = max_input.getStateDelta();
						// on key down will return max, on key up -max
						if(state_delta != 0.0f) return sign(state_delta) * getMax();
						return getMin();
					}
					else {
						
						Input max_input = get_max_input_state();
						if(max_input == NULL) return getNeutral();
						
						// convert input state value into action state value range
						return max_input.getState() * (getMax() - getMin()) + getMin();
					}
				}
				
				// Get action state delta (from previous update).
				// Returned value is adjusted to the [def.min; def.max] range.
				float getStateDelta() {
					
					// ??? May be here we should search for maximum delta, not input ???
					
					Input max_input = get_max_input_state();
					if(max_input == NULL) return 0.0f;
					
					return max_input.getStateDelta() * (getMax() - getMin());
				}
				
				// Return action name.
				string getName() {
					return def.name;
				}
				
				// Check if we have such input.
				int check(Input input) {
					foreach(Input i; inputs) if(i.equals(input)) return 1;
					return 0;
				}
				
				// Return index of specified input if exists, -1 otherwise.
				int find(Input input) {
					forloop(int i = 0; inputs.size()) {
						if(inputs[i].equals(input)) {
							return i;
						}
					}
					return -1;
				}
				
				// Update action.
				void update(float ifps) {
					
					foreach(Input input; inputs) {
						if(input == NULL) continue;
						input.update(ifps);
					}
				}
				
				// Load action from xml node.
				void load(Xml xml) {
					
					forloop(int i = 0; xml.getNumChilds()) {
						
						Xml child = xml.getChild(i);
						
						if(child.getName() != "input") {
							log.warning("Unigine::Input::Action::load(): unknown xml node \"%s\".\n",child.getName());
							continue;
						}
						
						Input input = new Input();
						if(getDefaultChangeSpeed() != INFINITY) input.setChangeSpeed(getDefaultChangeSpeed());
						if(getDefaultReturnSpeed() != INFINITY) input.setReturnSpeed(getDefaultReturnSpeed());
						input.load(child);
						
						// to avoid 2 instances of the same input (if we have wrong config)
						if(!check(input)) inputs.append(input);
					}
				}
				
				// Save action into xml node.
				void save(Xml xml) {
					
					foreach(Input input; inputs) {
						input.save(xml.addChild("input"));
					}
				}
				
				// Load action definition.
				void loadDef(Xml xml) {
					def.load(xml);
				}
				
				// Save action definition.
				void saveDef(Xml xml) {
					def.save(xml);
				}
		};
	}
}

#endif /* __UNIGINE_INPUT_ACTION_H__ */
