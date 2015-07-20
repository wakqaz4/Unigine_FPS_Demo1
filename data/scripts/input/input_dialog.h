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


#ifndef __UNIGINE_INPUT_INPUT_DIALOG_H__
#define __UNIGINE_INPUT_INPUT_DIALOG_H__

#include <scripts/input/input.h>

namespace Unigine {
	
	namespace Input {
		
		/* Dialog for input editing
		 */
		namespace InputDialog {
			
			/* Input dialog parameters.
			 */
			int spaces = 4;							// spaces between buttons
			int key_cancel = APP_KEY_ESC;
			int key_remove_input = APP_KEY_DELETE;
			
			/* Widgets.
			 */
			Gui gui;
			WidgetWindow window;
			WidgetVBox vbox;
			WidgetLabel label;
			
			int dialog_result;
			
			float timeout = 30.0f; // in seconds
			
			/* All available inputs we support.
			 */
			Input inputs[0];
			
			void init_inputs() {
				
				forloop(int i = 0; DeviceManager::getNumInputDevices()) {
					
					InputDeviceBase input_device = DeviceManager::getInputDeviceByIndex(i);
					
					input_device.clear_buttons();
					
					forloop(int j = 0; input_device.getNumStates()) {
						
						Input input = new Input();
						input.setInputDevice(input_device);
						input.setStateIndex(j);
						
						input.setMinDeviceValue(0.0f);
						input.setMaxDeviceValue(1.0f);
						inputs.append(input);
						
						if(input.isAxis()) {
							Input negative_input = new Input();
							negative_input.setInputDevice(input_device);
							negative_input.setStateIndex(j);
							negative_input.setMinDeviceValue(0.0f);
							negative_input.setMaxDeviceValue(-1.0f);
							inputs.append(negative_input);
						}
					}
				}
				
			}
			
			void shutdown_inputs() {
				inputs.delete();
			}
			
			/* Check if one of inputs changed (if a user pressed any key)
			 */
			void check_input(Input &input) {
				
				// check for gui controls
				if(engine.app.clearKeyState(key_cancel)) {
					// dialog cancelled
					dialog_result =  0;
					return;
				}
				
				if(engine.app.clearKeyState(key_remove_input)) {
					if(input != NULL) delete input;
					input = NULL;
					dialog_result = 1;
					return;
				}
				
				foreach(Input i; inputs) i.update(engine.game.getIFps());
				
				foreach(Input i; inputs) {
					
					if(i.getStateDelta() != 0) {
						input = new Input();
						input.copy(i);
						dialog_result = 1;
						return;
					}
				}
			}
			
			/* Init dialog.
			 */
			void init() {
				
				gui = engine.getGui();
				
				// generate GUI
				window = new WidgetWindow(gui);
				
				vbox = new WidgetVBox(gui,spaces,spaces);
				window.addChild(vbox,GUI_ALIGN_EXPAND);
				
				label = new WidgetLabel(gui);
				label.setFontRich(1);
				vbox.addChild(label,GUI_ALIGN_EXPAND);
				
				init_inputs();
			}
			
			/* Shutdown dialog.
			 */
			void shutdown() {
				shutdown_inputs();
				delete label;
				delete vbox;
				delete window;
			}
			
			/* Show dialog. Return 1 if user finished editing, 0 if cancelled.
			 */
			int show(string title,string action_name,Input &input) {
				
				// init
				init();
				window.setText(title);
				string message = format(
						"Choose the button or axis for action \"%s\".<br/>Press \"%s\" key to cancel editing<br/>or \"%s\" to delete control mapping",
						action_name,
						engine.app.getKeyName(key_cancel),
						engine.app.getKeyName(key_remove_input));
				label.setText(message);
				
				// show
				gui.addChild(window,GUI_ALIGN_CENTER | GUI_ALIGN_OVERLAP);
				window.setPermanentFocus();
				
				// wait for result
				dialog_result = -1;
				
				float begin = clock();
				
				while(dialog_result < 0 && (clock() - begin < timeout)) {
					update();
					check_input(input);
					wait 1;
				}
				
				// hide
				gui.removeChild(window);
				shutdown();
				
				return dialog_result;
			}
			
			/* Get/set spaces for gui.
			 */
			void setSpaces(int spaces_) {
				spaces = spaces_;
			}
			
			int getSpaces() {
				return spaces;
			}
			
			/* Get/set cancel key.
			 */
			void setKeyCancel(int key) {
				key_cancel = key;
			}
			
			int getKeyCancel() {
				return key_cancel;
			}
			
			/* Get/set remove input key.
			 */
			void setKeyRemoveInput(int key) {
				key_remove_input = key;
			}
			
			int getKeyRemoveInput() {
				return key_remove_input;
			}
			
			/* Get/set timeout for input dialog if no key pressed.
			 */
			void setTimeout(float timeout_) {
				timeout = timeout_;
			}
			
			float getTimeout() {
				return timeout;
			}
		}
	}
}
#endif /* __UNIGINE_INPUT_INPUT_DIALOG_H__ */
