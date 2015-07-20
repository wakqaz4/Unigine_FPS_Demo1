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


#ifndef __UNIGINE_INPUT_CONTROL_H__
#define __UNIGINE_INPUT_CONTROL_H__

#include <scripts/input/action.h>

namespace Unigine {
	
	namespace Input {
		
		/* Represents control that can be used in the game script.
		 */
		class Control {
			
			private:
				
				// Controls definition filename.
				string def_file_name;
				
				// Actions map: (name: action).
				Action actions[];
				
				// Flags
				int is_action_list_changed = false;
				
				// Load definition file.
				void load_def(string file_name) {
					
					Xml xml = new Xml();
					xml.load(file_name);
					
					if(xml.getName() != "controls_def") {
						throw("Unigine::Input::Control::load_def(): unknown xml node \"%s\"\n",xml.getName());
					}
					
					forloop(int i = 0; xml.getNumChilds()) {
						
						Xml child = xml.getChild(i);
						
						if(child.getName() != "action_def") {
							log.warning("Unigine::Input::Control::load_def(): unknown xml node \"%s\"\n",child.getName());
							continue;
						}
						
						Action action = new Action();
						action.loadDef(child);
						addAction(action);
					}
					
					delete xml;
				}
				
				// Save definition file.
				void save_def(string file_name) {
					if(is_action_list_changed == false) return;
					Xml xml = new Xml("controls_def");
					
					foreach(Action action; actions) {
						Xml child = xml.addChild("action_def");
						action.saveDef(child);
					}
					
					xml.save(file_name);
					
					delete xml;
				}
				
			public:
				
				// Contrructor.
				Control() {
					
				}
				
				// Destructor.
				~Control() {
					actions.delete();
				}
				
				// Get action by its name.
				Action getAction(string name) {
					return actions.check(name,NULL);
				}
				
				// Get action by its index.
				Action getActionByIndex(int index) {
					return actions.get(index);
				}
				
				// Add action.
				void addAction(Action action) {
					assert(action != NULL);
					actions.append(action.getName(),action);
					is_action_list_changed = true;
				}
				
				// Remove action by its name.
				void removeAction(string name) {
					actions.remove(name);
					is_action_list_changed = true;
				}
				
				// Get number of actions.
				int getNumActions() {
					return actions.size();
				}
				
				// Get action state.
				float getActionState(string name) {
					
					Action action = actions.check(name,NULL);
					if(action == NULL) throw("Unigine::Input::Control::getActionState(): action \"%s\" does not exist.\n",name);
					
					return action.getState();
				}
				
				void update() {
					
					throw("Please call Unigine::Input:Control::update(float ifps) instead of Unigine::Input::Control::update()\n");
				}
				
				// Update control.
				void update(float ifps) {
					
					foreach(Action action; actions) {
						action.update(ifps);
						//engine.message("%s: %f",action.getName(),action.getState());
					}
					
				}
				
				// Load control configuration from file.
				int loadFromFile(string file_name) {
					
					Xml xml = new Xml("controls");
					xml.load(file_name);
					
					if(xml.isArg("definition") == 0) {
						throw("Unigine::Input::Control::loadFromFile(): controls definition is not specified.\n");
					}
					
					def_file_name = xml.getArg("definition");
					
					// load action definitions.
					load_def(def_file_name);
					
					forloop(int i = 0; xml.getNumChilds()) {
						
						Xml child = xml.getChild(i);
						
						if(child.getName() != "action") {
							log.warning("Unigine::Input::Control::load(): unknown xml node \"%s\".\n",child.getName());
							continue;
						}
						
						if(child.isArg("name") == 0) {
							throw("Unigine::Input::Control::loadFromFile(): name of the action is not specified.\n");
						}
						
						string action_name = child.getArg("name");
						if(actions.check(action_name) == 0) {
							throw("Unigine::Input::Control::loadFromFile(): action \"%s\" is not defined.\n",action_name);
						}
						
						Action action = getAction(action_name);
						action.load(child);
					}
					
					delete xml;
					
					is_action_list_changed = false;
					return 1;
				}
				
				// Save control configuration to file.
				void saveToFile(string file_name) {
					
					save_def(def_file_name);
					
					Xml xml = new Xml("controls");
					xml.setArg("definition",def_file_name);
					
					foreach(Action action; actions) {
						Xml child = xml.addChild("action");
						child.setArg("name",action.getName());
						action.save(child);
					}
					
					xml.save(file_name);
					
					is_action_list_changed = false;
					delete xml;
				}
		};
	}
}

#endif /* __UNIGINE_INPUT_CONTROL_H__ */
