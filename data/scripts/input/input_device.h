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


#ifndef __UNIGINE_INPUT_INPUT_DEVICE_H__
#define __UNIGINE_INPUT_INPUT_DEVICE_H__

#include <unigine.h>

namespace Unigine {
	
	namespace Input {
		
		/* Possible state types.
		 */
		enum {
			STATE_TYPE_AXIS,
			STATE_TYPE_BUTTON,
		};
		
		/* Default threshold for input - if we have no value for threshold in input config - use this.
		 */
		float default_threshold = 0.2f;
		
		/**********************************************************************\
		 *
		 * class InputDeviceBase
		 *
		\**********************************************************************/
		
		/* Base class for all input devices.
		 */
		class InputDeviceBase {
			
			// Return name of the input device.
			string getName() { throw("InputDeviceBase::getName(): pure virtual method call\n"); }
			
			// Return number of states (axes and buttons).
			int getNumStates() { throw("InputDeviceBase::getNumStates(): pure virtual method call\n"); }
			
			// Return state value.
			float getState(int i) { throw("InputDeviceBase::getState(): pure virtual method call\n"); }
			
			// Return state index by its name.
			int getStateIndex(string name) {
				
				for(int i = getNumStates() - 1; i >= 0; i--) {
					if(getStateName(i) == name) {
						return i;
					}
				}
				return -1;
			}
			
			// Return state name by its index.
			string getStateName(int i) { throw("InputDeviceBase::getStateName(): pure virtual method call\n"); }
			
			// Return STATE_TYPE_* of state with given index.
			int getStateType(int i) { throw("InputDeviceBase::getStateType(): pure virtual method call\n"); }
			
			// Update input device.
			int update() { throw("InputDeviceBase::update(): pure virtual method call\n"); }
			
			void clear_buttons() { throw ("InputDeviceBase::clear_buttons(): pure virtual method call\n"); }
		};
		
		/**********************************************************************\
		 *
		 * class InputDeviceKeyboard
		 *
		\**********************************************************************/
		
		/* Keyboard input device.
		 */
		class InputDeviceKeyboard : InputDeviceBase {
			
			//
			string getName() {
				return "Keyboard";
			}
			
			//
			int getNumStates() {
				return APP_NUM_KEYS;
			}
			
			//
			float getState(int i) {
				assert(i >=0 && i < getNumStates());
				return float(engine.app.getKeyState(i));
			}
			
			//
			string getStateName(int i) {
				assert(i >=0 && i < getNumStates());
				return engine.app.getKeyName(i);
			}
			
			//
			int getStateType(int i) {
				assert(i >=0 && i < getNumStates());
				return STATE_TYPE_BUTTON;
			}
			
			//
			int update() {
				return 1;
			}
			
			void clear_buttons() {
			}
		};
		
#ifdef HAS_SIXAXIS
		
		/**********************************************************************\
		 *
		 * class InputDeviceSixAxis
		 *
		\**********************************************************************/
		
		/* SixAxis input device.
		 */
		class InputDeviceSixAxis : InputDeviceBase {
			
			string button_names[] = (
				"BUTTON_L1",
				"BUTTON_R1",
				"BUTTON_L2",
				"BUTTON_R2",
				"BUTTON_L3",
				"BUTTON_R3",
				"BUTTON_UP",
				"BUTTON_DOWN",
				"BUTTON_LEFT",
				"BUTTON_RIGHT",
				"BUTTON_TRIANGLE",
				"BUTTON_CIRCLE",
				"BUTTON_CROSS",
				"BUTTON_SQUARE",
				"BUTTON_START",
				"BUTTON_SELECT",
				"LEFT_STICK_X",
				"LEFT_STICK_Y",
				"RIGHT_STICK_X",
				"RIGHT_STICK_Y",
			);
			
			ControlsSixAxis sixaxis;
			
			//
			InputDeviceSixAxis(ControlsSixAxis sixaxis_) {
				if(sixaxis_ == NULL) {
					throw("InputDeviceSixAxis::__InputDeviceSixAxis__(): \"sixaxis_\" param must not be NULL\n");
				}
				sixaxis = sixaxis_;
			}
			
			//
			~InputDeviceSixAxis() {
				delete sixaxis;
			}
			
			//
			string getName() {
				return format("SixAxis%d",sixaxis.getNumber());
			}
			
			//
			int getNumStates() {
				// all buttons + two sticks (x and y coordinates for each stick)
				return CONTROLS_SIXAXIS_NUM_BUTTONS + 4;
			}
			
			//
			float getState(int i) {
				
				assert(i >= 0 && i < getNumStates());
				
				if(sixaxis.isAvailable() == false) return 0.0f;
				
				// all button states as first part of array and then two sticks
				if(i < CONTROLS_SIXAXIS_NUM_BUTTONS) return sixaxis.getButton(i);
				
				i -= CONTROLS_SIXAXIS_NUM_BUTTONS;
				
				// left stick coordinates as second part
				if(i == 0) return sixaxis.getLeftX();
				if(i == 1) return sixaxis.getLeftY();
				
				// right stick coordinates as third part
				if(i == 2) return sixaxis.getRightX();
				if(i == 3) return sixaxis.getRightY();
			}
			
			//
			string getStateName(int i) {
				assert(i >=0 && i < getNumStates());
				return button_names[i];
			}
			
			//
			int getStateType(int i) {
				assert(i >=0 && i < getNumStates());
				
				if(i < CONTROLS_SIXAXIS_NUM_BUTTONS) return STATE_TYPE_BUTTON;
				
				return STATE_TYPE_AXIS;
			}
			
			//
			int update() {
				return sixaxis.updateEvents();
			}
			
			void clear_buttons() {
				forloop(int i = 0; CONTROLS_SIXAXIS_NUM_BUTTONS) {
					sixaxis.clearButton(i);
				}
			}
		};
		
#endif
		
#ifdef HAS_JOYSTICK
		
		/**********************************************************************\
		 *
		 * class InputDeviceJoystick
		 *
		\**********************************************************************/
		
		/* Joystick input device.
		 */
		class InputDeviceJoystick : InputDeviceBase {
			
			ControlsJoystick joystick;
			
			//
			InputDeviceJoystick(ControlsJoystick joystick_) {
				if(joystick_ == NULL) {
					throw("InputDeviceJoystick::__InputDeviceJoystick__(): \"joystick_\" param must not be NULL\n");
				}
				joystick = joystick_;
			}
			
			//
			~InputDeviceJoystick() {
				delete joystick;
			}
			
			//
			string getName() {
				return joystick.getName();
			}
			
			//
			int getNumStates() {
				return joystick.getNumAxes() + joystick.getNumButtons() + joystick.getNumPovs();
			}
			
			//
			float getState(int i) {
				
				if(i < 0 || i >= getNumStates()) log.message("i = %d",i);
				assert(i >= 0 && i < getNumStates());
				
				// axis as a first part of an array
				int num_axes = joystick.getNumAxes();
				if(i >= 0 && i < num_axes) return joystick.getAxis(i);
				
				// buttons as a second part
				i -= num_axes;
				if(i < joystick.getNumButtons()) return joystick.getButton(i);
				
				//povs as third
				i -= joystick.getNumButtons();
				if(i < joystick.getNumPovs()) {
					return joystick.getPov(i);
				}
				
				return 0.0f;
			}
			
			//
			string getStateName(int i) {
				
				assert(i >=0 && i < getNumStates());
				
				int num_axes = joystick.getNumAxes();
				if(i >= 0 && i < num_axes) return joystick.getAxisName(i);
				
				i -= num_axes;
				if(i < joystick.getNumButtons()) return joystick.getButtonName(i);
				
				i -= joystick.getNumButtons();
				if(i < joystick.getNumPovs()) return joystick.getPovName(i);
				
				return format("State%d",i);
			}
			
			//
			int getStateType(int i) {
				assert(i >=0 && i < getNumStates());
				if(i >= 0 && i < joystick.getNumAxes()) return STATE_TYPE_AXIS;
				return STATE_TYPE_BUTTON;
			}
			
			//
			int update() {
				return joystick.updateEvents();
			}
			
			void clear_buttons() {
				forloop(int i = 0; joystick.getNumButtons()) {
					joystick.clearButton(i);
				}
			}
		};
		
#endif
		
#ifdef HAS_XPAD360
		
		/**********************************************************************\
		 *
		 * class InputDeviceXpad360
		 *
		\**********************************************************************/
		
		/* Joystick input device.
		 */
		class InputDeviceXpad360 : InputDeviceBase {
			
			string button_names[] = (
				"BUTTON_A",
				"BUTTON_B",
				"BUTTON_X",
				"BUTTON_Y",
				"BUTTON_BACK",
				"BUTTON_START",
				"BUTTON_DPAD_UP",
				"BUTTON_DPAD_DOWN",
				"BUTTON_DPAD_LEFT",
				"BUTTON_DPAD_RIGHT",
				"BUTTON_THUMB_LEFT",
				"BUTTON_THUMB_RIGHT",
				"BUTTON_SHOULDER_LEFT",
				"BUTTON_SHOULDER_RIGHT",
				"LEFT_STICK_X",
				"LEFT_STICK_Y",
				"RIGHT_STICK_X",
				"RIGHT_STICK_Y",
				"LEFT_TRIGGER",
				"RIGHT_TRIGGER",
			);
			
			ControlsXPad360 xpad;
			
			//
			InputDeviceXpad360(ControlsXPad360 xpad_) {
				if(xpad_ == NULL) {
					throw("InputDeviceXpad360::__InputDeviceXpad360()__: \"xpad_\" param must not be NULL\n");
				}
				xpad = xpad_;
			}
			
			//
			~InputDeviceXpad360() {
				delete xpad;
			}
			
			//
			string getName() {
				return "XBOX360 Controller" + xpad.getNumber();
			}
			
			//
			int getNumStates() {
				// all buttons + 6 axis
				return CONTROLS_XPAD360_NUM_BUTTONS + 6;
			}
			
			//
			float getState(int i) {
				
				if(i < 0 || i >= getNumStates()) log.message("i = %d",i);
				assert(i >= 0 && i < getNumStates());
				
				if(xpad.isAvailable() == false) return 0.0f;
				
				// all button states as first part of array and then axes
				if(i < CONTROLS_XPAD360_NUM_BUTTONS) return xpad.getButton(i);
				
				i -= CONTROLS_XPAD360_NUM_BUTTONS;
				
				// left stick coordinates as second part
				if(i == 0) return xpad.getLeftX();
				if(i == 1) return xpad.getLeftY();
				
				// right stick coordinates as third part
				if(i == 2) return xpad.getRightX();
				if(i == 3) return xpad.getRightY();
				
				// triggers next
				if(i == 4) return xpad.getLeftTrigger();
				if(i == 5) return xpad.getRightTrigger();
			}
			
			//
			string getStateName(int i) {
				
				assert(i >=0 && i < getNumStates());
				return button_names[i];
			}
			
			//
			int getStateType(int i) {
				assert(i >=0 && i < getNumStates());
				
				if(i < CONTROLS_XPAD360_NUM_BUTTONS) return STATE_TYPE_BUTTON;
				
				return STATE_TYPE_AXIS;
			}
			
			//
			int update() {
				return xpad.updateEvents();
			}
			
			void clear_buttons() {
				forloop(int i = 0; CONTROLS_XPAD360_NUM_BUTTONS) {
					xpad.clearButton(i);
				}
			}
		};
		
#endif
		
		/**********************************************************************\
		 *
		 * class InputDeviceMouse
		 *
		\**********************************************************************/
		
		/* Mouse input device.
		 */
		class InputDeviceMouse : InputDeviceBase {
			
			// TODO: do something with button quantity and numbers
			enum {
				NUM_BUTTONS = 10,
				NUM_AXES = 2,
			};
			
			//
			string getName() {
				return "Mouse";
			}
			
			//
			int getNumStates() {
				return NUM_BUTTONS + NUM_AXES;
			}
			
			//
			float getState(int i) {
				
				assert(i >=0 && i < getNumStates());
				
				// return axes first
				if(i == 0) return float(engine.controls.getMouseDX());
				if(i == 1) return float(engine.controls.getMouseDY());
				i -= NUM_AXES;
				
				// buttons as a second part
				if(i < NUM_BUTTONS) {
					// convert state number to APP_BUTTON_*
					return float(engine.app.getMouseButtonState(1 << i));
				}
				
				return 0.0f;
				
			}
			
			//
			string getStateName(int i) {
				
				assert(i >=0 && i < getNumStates());
				
				if(i == 0) return "X Axis";
				if(i == 1) return "Y Axis";
				
				// convert state number to APP_BUTTON_*
				return engine.app.getMouseButtonName(1 << (i - NUM_AXES));
			}
			
			// return STATE_TYPE_* of state with given index
			int getStateType(int i) {
				
				assert(i >=0 && i < getNumStates());
				
				if(i < NUM_AXES) return STATE_TYPE_AXIS;
				return STATE_TYPE_BUTTON;
			}
			
			//
			int update() {
				return 1;
			}
			
			void clear_buttons() {
			}
		};
		
		/**********************************************************************\
		 *
		 * namespace DeviceManager
		 *
		\**********************************************************************/
		
		/* Contains all input devices and utility function for them.
		 */
		namespace DeviceManager {
			
#ifdef HAS_SIXAXIS
			int MAX_SIXAXIS_CONTROLLERS = 4;
#endif
			
#ifdef HAS_JOYSTICK
			int MAX_SUPPORTED_JOYSTICKS = 16;
#endif
			
#ifdef HAS_XPAD360
			int MAX_XPAD360_CONTROLLERS = 4;
#endif
			
			
			// List of available input devices.
			InputDeviceBase input_devices[0];
			
			/* Find and init all available input devices.
			 */
			void init() {
				
				// add keyboard input device
				input_devices.append(new InputDeviceKeyboard());
				
				// add mouse input device
				input_devices.append(new InputDeviceMouse());
				
#ifdef HAS_SIXAXIS
				// find all connected sixaxis controllers
				forloop(int i = 0; MAX_SIXAXIS_CONTROLLERS) {
					
					ControlsSixAxis sixaxis = new ControlsSixAxis(i);
					input_devices.append(new InputDeviceSixAxis(sixaxis));
					
					// for debug
					/*
					log.message("Connected sixaxis controller %d, %savailable\n",sixaxis.getNumber(),sixaxis.isAvailable() ? "" : "not ");
					*/
				}
#endif
				
#ifdef HAS_JOYSTICK
				// find all connected joysticks
				forloop(int i = 0; MAX_SUPPORTED_JOYSTICKS) {
					
					ControlsJoystick joystick = new ControlsJoystick(i);
#ifdef HAS_XPAD360
					if(substr(joystick.getName(),0,8) == "XBOX 360") {
						ControlsXPad360 xpad = new ControlsXPad360(i);
						if(xpad.isAvailable()) {
							input_devices.append(new InputDeviceXpad360(xpad));
							delete joystick;
							continue;
						}
					}
#endif
					
					if(joystick.isAvailable()) {
						input_devices.append(new InputDeviceJoystick(joystick));
					}
				}
#endif
				

			}
			
			/* Shutdown input devices.
			 */
			void shutdown() {
				input_devices.delete();
			}
			
			/* Update all input devices.
			 */
			void update() {
				foreach(InputDeviceBase input_device; input_devices) {
					input_device.update();
				}
			}
			
			/* Return number of registered input devices.
			 */
			int getNumInputDevices() {
				return input_devices.size();
			}
			
			/* Return input device by index.
			 */
			InputDeviceBase getInputDeviceByIndex(int i) {
				if(i >= 0 && i < input_devices.size()) return input_devices[i];
				return NULL;
			}
			
			/* Return input device by given name, and NULL if not found.
			 */
			InputDeviceBase getInputDeviceByName(string name) {
				
				foreach(InputDeviceBase input_device; input_devices) {
					if(input_device.getName() == name) return input_device;
				}
				
				return NULL;
			}
			
		}
		
		/**********************************************************************\
		 *
		 * Public interface.
		 *
		\**********************************************************************/
		
		/* Unigine::Input init
		 */
		void init() {
			DeviceManager::init();
		}
		
		/* Unigine::Input update
		 */
		void update() {
			DeviceManager::update();
		}
		
		/* Unigine::Input shutdown
		 */
		void shutdown() {
			DeviceManager::shutdown();
		}
		
		/* Get/set default threshold.
		 */
		void setDefaultThreshold(float threshold) {
			default_threshold = threshold;
		}
		
		float getDefaultThreshold() {
			return default_threshold;
		}
	}
}

#endif /*__UNIGINE_INPUT_INPUT_DEVICE_H__*/
