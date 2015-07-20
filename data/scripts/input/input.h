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


#ifndef __UNIGINE_INPUT_INPUT_H__
#define __UNIGINE_INPUT_INPUT_H__

#include <scripts/input/input_device.h>

namespace Unigine {
	
	namespace Input {
		
		/* Represents state of a single key/button/axis/etc.
		 */
		class Input {
			
			private:
				
				InputDeviceBase input_device = NULL;
				
				string input_device_name;
				int state_index = -1;
				
				// state values are always normalized
				// according to input device range (see below)
				float state_value = 0.0f;
				float state_value_old = 0.0f;
				float neutral_state_value = 0.0f;
				
				// input device range
				float min_device_value = 0.0f;
				float max_device_value = 1.0f;
				float neutral_device_value = 0.0f;
				
				float device_change_speed = INFINITY;
				float device_return_speed = INFINITY;
				float device_threshold = default_threshold;
				
				// Normalize value according to device range.
				float normalize_value(float value) {
					return (value - min_device_value) / (max_device_value - min_device_value);
				}
				
				// Transform device state value into input state value.
				float get_input_state(float device_state_value) {
					
					if(getStateType() == STATE_TYPE_BUTTON) return normalize_value(device_state_value);
					
					// STATE_TYPE_AXIS
					if(abs(device_state_value) < device_threshold) return neutral_state_value;
					
					// check if its our axis' range
					if(isPositiveAxis() && (device_state_value < min_device_value || device_state_value > max_device_value)) {
						return neutral_state_value;
					}
					if(isNegativeAxis() && (device_state_value > min_device_value || device_state_value < max_device_value)) {
						return neutral_state_value;
					}
					
					// apply device_threshold
					float input_state_value = sign(device_state_value) * 
						(abs(device_state_value) - device_threshold) / (1.0f - device_threshold);
					
					return normalize_value(input_state_value);
				}
				
			public:
				
				// Consturctor.
				Input() {
					
				}
				
				// Desturctor.
				~Input() {
					
				}
				
				// "Copy contructor".
				void copy(Input input) {
					
					input_device = input.input_device;
					input_device_name = input.input_device_name;
					state_index = input.state_index;
					neutral_state_value = input.neutral_state_value;
					min_device_value = input.min_device_value;
					max_device_value = input.max_device_value;
					neutral_device_value = input.neutral_device_value;
					device_change_speed = input.device_change_speed;
					device_return_speed = input.device_return_speed;
					device_threshold = input.device_threshold;
				}
				
				// Get/set state index.
				int getStateIndex() {
					return state_index;
				}
				
				void setStateIndex(int index) {
					state_index = index;
				}
				
				// Get state name.
				string getStateName() {
					
					if(input_device == NULL || state_index < 0 || state_index >= input_device.getNumStates()) return "N/A";
					
					string state_name = input_device.getStateName(state_index);
					
					if(getStateType() == STATE_TYPE_BUTTON) return state_name;
					
					if(isPositiveAxis()) {
						// positive axis direction
						if(min_device_value != 0.0f) return "increase " + state_name;
						return "positive " + state_name;
					} else {
						// negative axis direction
						if(min_device_value != 0) return "decrease " + state_name;
						return "negative " + state_name;
					}
				}
				
				// Get/set input device.
				void setInputDevice(InputDeviceBase device) {
					assert(device != NULL);
					input_device = device;
					input_device_name = input_device.getName();
				}
				
				InputDeviceBase getInputDevice() {
					return input_device;
				}
				
				// Get input device name.
				string getInputDeviceName() {
					if(input_device == NULL) return "N/A";
					return input_device.getName();
				}
				
				// Return state value (always in [0; 1]).
				float getState() {
					return state_value;
				}
				
				// Neutral state value is always normalized
				float getNeutralStateValue() {
					return neutral_state_value;
				}
				
				// Return state change since previous update.
				float getStateDelta() {
					return state_value - state_value_old;
				}
				
				// Return one of the STATE_TYPE_* constant.
				int getStateType() {
					return input_device.getStateType(state_index);
				}
				
				// Return true if input is axis.
				int isAxis() {
					return getStateType() == STATE_TYPE_AXIS;
				}
				
				int isPositiveAxis() {
					return isAxis() && (min_device_value < max_device_value);
				}
				
				int isNegativeAxis() {
					return isAxis() && (min_device_value > max_device_value);
				}
				
				// Return true if input is virtual axis.
				int isVirtualAxis() {
					return getStateType() == STATE_TYPE_BUTTON && (device_change_speed != INFINITY || device_return_speed != INFINITY);
				}
				
				// Return true if input is button.
				int isButton() {
					return getStateType() == STATE_TYPE_BUTTON && device_change_speed == INFINITY && device_return_speed == INFINITY;
				}
				
				// Get/set device range
				float getMinDeviceValue() {
					return min_device_value;
				}
				
				void setMinDeviceValue(float min) {
					min_device_value = min;
				}
				
				float getMaxDeviceValue() {
					return max_device_value;
				}
				
				void setMaxDeviceValue(float max) {
					max_device_value = max;
				}
				
				void setNeutralDeviceValue(float neutral) {
					neutral_device_value = neutral;
					neutral_state_value = normalize_value(neutral_device_value);
					state_value_old = neutral_state_value;
					state_value = neutral_state_value;
				}
				
				float getNeutralDeviceValue() {
					return neutral_device_value;
				}
				
				// Get/set change speed.
				void setChangeSpeed(float device_change_speed_) {
					device_change_speed = device_change_speed_;
				}
				
				float getChangeSpeed() {
					return device_change_speed;
				}
				
				// Get/set return speed.
				void setReturnSpeed(float device_return_speed_) {
					device_return_speed = device_return_speed_;
				}
				
				float getReturnSpeed() {
					return device_return_speed;
				}
				
				// Get/set device_threshold.
				void setThreshold(float device_threshold_) {
					device_threshold = device_threshold_;
				}
				
				float getThreshold() {
					return device_threshold;
				}
				
				// Update.
				void update(float ifps) {
					
					// configured device can be not plugged in
					if(input_device == NULL) return;
					// using different drivers, the same device can have different number of states
					if(state_index < 0 || state_index > input_device.getNumStates()) return;
					
					// update old state value
					state_value_old = state_value;
					
					float state_value_new = get_input_state(input_device.getState(state_index));
					
					//if(state_value_new > 0.0f) {
					//	engine.message("%s %s %f",input_device.getName(),input_device.getStateName(state_index),state_value_new);
					//}
					
					if(getStateType() == STATE_TYPE_BUTTON) {
						// emulate axis
						if(state_value_new > neutral_state_value) {
							// increase axis
							state_value += ifps * device_change_speed;
						} else {
							// decrease axis
							state_value -= ifps * device_return_speed;
						}
						state_value = clamp(state_value,0.0f,1.0f);
						
					} else {
						// simply update from axis
						state_value = state_value_new;
					}
				}
				
				// Load input parameters from xml node.
				void load(Xml xml) {
					
					assert(xml != NULL);
					
					if(xml.isArg("device") == 0) throw("Unigine::Input::Input::load(): \"device\" attribute not found.\n");
					if(xml.isArg("state") == 0) throw("Unigine::Input::Input::load(): \"state\" attribute not found.\n");
					
					// input device
					input_device_name = xml.getArg("device");
					// state
					state_index = xml.getIntArg("state");
					
					if(xml.isArg("min")) min_device_value = xml.getFloatArg("min");
					if(xml.isArg("max")) max_device_value = xml.getFloatArg("max");
					if(xml.isArg("neutral")) neutral_device_value = xml.getFloatArg("neutral");
					if(xml.isArg("device_change_speed")) device_change_speed = xml.getFloatArg("device_change_speed");
					if(xml.isArg("device_return_speed")) device_return_speed = xml.getFloatArg("device_return_speed");
					if(xml.isArg("device_threshold")) device_threshold = xml.getFloatArg("device_threshold");
					
					neutral_state_value = normalize_value(neutral_device_value);
					state_value = neutral_state_value;
					state_value_old = neutral_state_value;
					
					input_device = DeviceManager::getInputDeviceByName(input_device_name);
					
					if(state_index < 0) {
						throw("Unigine::Input::Input::load(): input device \"%s\" state \"%d\" does not exist.\n",
								input_device_name,state_index);
					}
				}
				
				// Save input parameters into xml node.
				void save(Xml xml) {
					
					xml.setArg("device",input_device_name);
					xml.setIntArg("state",state_index);
					
					if(min_device_value != 0.0f) xml.setFloatArg("min",min_device_value);
					if(max_device_value != 1.0f) xml.setFloatArg("max",max_device_value);
					if(neutral_device_value != 0.0f) xml.setFloatArg("neutral",neutral_device_value);
					if(device_change_speed != INFINITY) xml.setFloatArg("device_change_speed",device_change_speed);
					if(device_return_speed != INFINITY) xml.setFloatArg("device_return_speed",device_return_speed);
					if(device_threshold != default_threshold) xml.setFloatArg("device_threshold",device_threshold);
				}
				
				// Return true if this input is equals to specified input.
				int equals(Input input) {
					
					return (input != NULL &&
						input_device == input.input_device &&
						input_device_name == input.input_device_name &&
						state_index == input.state_index &&
						min_device_value == input.min_device_value &&
						max_device_value == input.max_device_value &&
						neutral_device_value == input.neutral_device_value
					);
				}
		};
	}
}

#endif /* __UNIGINE_INPUT_INPUT_H__ */
