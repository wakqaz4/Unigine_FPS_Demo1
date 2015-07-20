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


#ifndef __UNIGINE_INPUT_CONTROL_SETUP_WINDOW_H__
#define __UNIGINE_INPUT_CONTROL_SETUP_WINDOW_H__

#include <scripts/common/event.h>
#include <scripts/common/dialogs.h>
#include <scripts/input/input_dialog.h>
#include <scripts/input/action.h>
#include <scripts/input/control.h>

namespace Unigine {
	
	namespace Input {
	
		/* Window for editing controls.
		 */
		namespace ControlSetupWindow {
			
			class ActionEditor;
			
			/* Constants.
			 */
			string UI_FILE = "scripts/input/control_setup_window.ui";
			
			/* Paramters.
			 */
			int num_columns = 2;	// number of button columns
			
			/* Gui.
			 */
			Gui gui;
			UserInterface ui;
			WidgetWindow window;
			WidgetGridBox client_area;
			Event on_hide;
			
			/* Edited control.
			 */
			Control control;
			
			/* Editors for control.
			 */
			ActionEditor editors[0];
			
			/*************************************************************************\
			 *
			 * class ActionEditor
			 *
			\*************************************************************************/
			
			/* Class for editing single input action.
			 */
			class ActionEditor {
				
				private:
					
					// editor gui parameters
					int header_width = 120;
					int button_width = 120;
					
					// widgets
					WidgetLabel label;
					WidgetButton input_buttons[0];
					
					// action to be edited
					Action action;
					
					// Return input name or "N/A" if no input or device available.
					string get_input_name(Input input) {
						if(input == NULL) return "N/A";
						return input.getStateName();
					}
					
					// Update input labels.
					void update_gui() {
						
						forloop(int i = 0; input_buttons.size()) {
							
							Input input = (i < action.getNumInputs()) ? action.getInput(i) : NULL;
							
							assert(input_buttons[i] != NULL);
							
							input_buttons[i].setText(get_input_name(input));
							
							// tooltip
							if(input != NULL) input_buttons[i].setToolTip(input.getInputDeviceName());
							else input_buttons[i].setToolTip("N/A");
						}
					}
					
					// On button clicked callback.
					void on_button_clicked(ActionEditor sender,int num) {
						
						assert(sender != NULL);
						
						Action cur_action = sender.action;
						assert(cur_action != NULL);
						
						Input edited_input = NULL;
						if(InputDialog::show("Learn controls",cur_action.getName(),edited_input) == 0) return;
						
						// if input was removed
						if(edited_input == NULL) {
							cur_action.removeInput(num);
							sender.update_gui();
							return;
						}
						
						// if inupt was modified
						assert(control != NULL);
						// check if input already in use
						forloop(int i = 0; control.getNumActions()) {
							
							Action action = control.getActionByIndex(i);
							
							int j = action.find(edited_input);
							if(j < 0) continue;
							
							// input is already in use
							string message = format(
								"Input already in use. Click\nYES to remove this input from \"%s\" action or\nNO to enter another input.",
								action.getName());
							if(DialogMessage::show("Input is already in use",message,DialogMessage::YES | DialogMessage::NO) == DialogMessage::NO) return;
							
							// if user clicked YES remove input from the previously mapped action
							action.removeInput(j);
							
							assert(editors[i] != NULL);
							editors[i].update_gui();
						}
						
						assert(cur_action != NULL);
						
						cur_action.setInput(edited_input,num);
						sender.update_gui();
					}
					
				public:
					
					// Constructor.
					ActionEditor(Action action_) {
						
						action = action_;
						
						// generate gui
						label = new WidgetLabel(gui,action.getName());
						label.setWidth(header_width);
						client_area.addChild(label,GUI_ALIGN_EXPAND);
						
						// init buttons for input editing
						forloop(int i = 0; num_columns) {
							
							// add button
							WidgetButton button = new WidgetButton(gui);
							button.setWidth(button_width);
							
							button.setCallback(
									GUI_CLICKED,
									"Unigine::Input::ControlSetupWindow::ActionEditor::on_button_clicked",
									this,
									i);
							
							client_area.addChild(button,GUI_ALIGN_EXPAND);
							input_buttons.append(button);
						}
						
						update_gui();
					}
					
					// Destructor.
					~ActionEditor() {
						input_buttons.delete();
						delete label;
					}
					
					// Get edited action.
					Action getAction() {
						return action;
					}
					
					// Get/set header width.
					void setHeaderWidth(int width) {
						header_width = width;
					}
					
					int getHeaderWidth() {
						return header_width;
					}
					
					// Get/set button width.
					void setButtonWidth(int width) {
						button_width = width;
					}
					
					int getButtonWidth() {
						return button_width;
					}
			};
			
			/*************************************************************************\
			 *
			 * Unigine::Input::ControlSetupWindow private functions.
			 *
			\*************************************************************************/
			
			/* Initialize widgets for editing.
			 */
			void init_gui() {
				
				editors.clear();
				forloop(int i = 0; control.getNumActions()) {
					editors.append(new ActionEditor(control.getActionByIndex(i)));
				}
			}
			
			/* Release widgets.
			 */
			void release_gui() {
				editors.delete();
			}
			
			/*************************************************************************\
			 *
			 * Unigine::Input::ControlSetupWindow interface functions.
			 *
			\*************************************************************************/
			
			/* Initialize window gui.
			 */
			void init(Gui gui_,int num_columns_ = 2) {
				
				on_hide = new Event();
				
				gui = gui_;
				ui = new UserInterface(gui,UI_FILE);
				window.setTextAlign(GUI_ALIGN_LEFT);
				num_columns = num_columns_;
				client_area.setNumColumns(num_columns + 1);
			}
			
			/* Shutdown window.
			 */
			void shutdown() {
				
				delete ui;
				
				delete on_hide;
			}
			
			/* Update window.
			 */
			void update() {
				if(isVisible() == 0) return;
			}
			
			/* Return whether window is visible.
			 */
			int isVisible() {
				return gui.isChild(window);
			}
			
			/*
			 */
			void save() {
				
			}
			
			/*
			 */
			void restore() {
				// cleanup
				editors.delete();
			}
			
			/* Show window.
			 */
			void show(Control control_) {
				
				if(isVisible() && control == control_) return;
				
				control = control_;
				
				// init gui
				init_gui();
				
				// show window
				gui.addChild(window,GUI_ALIGN_OVERLAP);
				window.setFocus();
			}
			
			/* Hide window.
			 */
			void hide() {
				
				if(isVisible() == 0) return;
				
				on_hide.invoke();
				
				gui.removeChild(window);
				release_gui();
			}
			
			/* Add/remove callback for on_hide event.
			 */
			void addOnHideCallback(string callback,int instance = NULL) {
				on_hide.addCallback(callback,instance);
			}
			
			void removeOnHideCallback(string callback) {
				on_hide.removeCallback(callback);
			}
		}
	}
}

#endif /* __UNIGINE_INPUT_CONTROL_SETUP_WINDOW_H__ */
