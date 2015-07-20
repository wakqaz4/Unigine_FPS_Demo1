/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    randomizer.cpp
 * Desc:    Randomizer plugin
 * Version: 1.00
 * Author:  Andrey Viktorov <unclebob@unigine.com>
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

#include <core/unigine.h>

/******************************************************************************\
*
* Plugin
*
\******************************************************************************/

/*
 */
string getName() {
	return "Randomizer";
}

/*
 */
void init(PluginMeta m) {
	Randomizer::init(m);
}

void shutdown() {
	Randomizer::shutdown();
}

void update(int need_reload) {
	Randomizer::update(need_reload);
}

void save() {
	Randomizer::save();
}

void show() {
	Randomizer::show();
}
/******************************************************************************\
*
* Randomizer
*
\******************************************************************************/

/*
 */
namespace Randomizer {
	
	/*
	 */
	using Unigine::Widgets;
	
	/*
	 */
	UserInterface interface;
	Window window;
	
	enum {
		OFFSET_X = 4,
		OFFSET_Y = 136,
	};
	
	/*
	 */
	PluginMeta meta;
	
	/*
	 */
	void init(PluginMeta m) {
		
		meta = m;
		
		interface = new UserInterface(replace(meta.source,".cpp",".ui"),getName() + "::");
		hotkeysAddUiToolTips(interface);
		
		UI::init();
		
		window.arrange();
		
		pluginsAddWindow(window,meta.title,meta.name);
		hotkeysAddPluginHotkey(getName(),meta.name,meta.title,meta.description,new Hotkeys::HandlerPluginFunction(getName(),"Randomizer::plugin_window_toggle",'r',0,0,Hotkeys::KEY_CTRL));
	}
	
	/*
	 */
	void shutdown() {
		
		hotkeysRemovePlugin(getName());
		
		update(1);
		
		pluginsRemoveWindow(window,meta.name);
		
		UI::shutdown();
		
		hotkeysRemoveUiToolTips(interface);
		delete interface;
	}
	
	/*
	 */
	void update(int need_reload) {
		
	}
	
	/*
	 */
	void plugin_window_toggle() {
		windowToggle(window,meta.name);
	}
	
	/*
	 */
	void save() {
		
	}
	
	/*
	 */
	void show() {
		pluginsShowWindow(window,meta.name);
	}
	
	/**************************************************************************\
	*
	* Random group
	*
	\**************************************************************************/
	
	/*
	 */
	class RandomGroup {
		
		/*
		 */
		enum {
			UI_ICON_SIZE = 24,
			UI_WEIGHT_EDITLINE_WIDTH = 50,
		};
		
		EditLine node_path_el;
		EditLine weight_el;
		
		Icon node_path_b;
		Icon clear_b;
		
		GridBox root;
		
		/*
		 */
		RandomGroup(GridBox r) {
			root = r;
			
			node_path_el = new EditLine("");
			node_path_el.setCallback(DOUBLE_CLICKED,functionid(group_load),this);
			
			node_path_b = new Icon("editor/gui/action_load.png");
			node_path_b.setWidth(UI_ICON_SIZE);
			node_path_b.setHeight(UI_ICON_SIZE);
			node_path_b.setCallback(CLICKED,functionid(group_load),this);
			
			weight_el = new EditLine(string(1));
			weight_el.setWidth(UI_WEIGHT_EDITLINE_WIDTH);
			weight_el.setValidator(VALIDATOR_INT);
			
			clear_b = new Icon("editor/gui/action_clear.png");
			clear_b.setWidth(UI_ICON_SIZE);
			clear_b.setHeight(UI_ICON_SIZE);
			clear_b.setCallback(CLICKED,functionid(group_clear),this);
			
			root.addChild(node_path_el,ALIGN_EXPAND);
			root.addChild(node_path_b);
			root.addChild(weight_el);
			root.addChild(clear_b);
		}
		
		/*
		 */
		~RandomGroup() {
			root.removeChild(node_path_el);
			root.removeChild(node_path_b);
			root.removeChild(weight_el);
			root.removeChild(clear_b);
			
			delete node_path_el;
			delete node_path_b;
			delete weight_el;
			delete clear_b;
			
			root = NULL;
		}
		
		/*
		 */
		void setPath(string path) {
			node_path_el.setText(path);
		}
		
		string getPath() {
			return node_path_el.getText();
		}
		
		/*
		 */
		void setWeight(int weight) {
			weight_el.setText(string(weight));
		}
		
		int getWeight() {
			return int(weight_el.getText());
		}
		
		/*
		 */
		void group_load(RandomGroup group) {
			string path;
			if(!dialogFileNode(TR("Select node"),path)) return;
			
			group.setPath(path);
		}
		
		void group_clear(RandomGroup group) {
			group.setPath("");
			group.setWeight(1);
		}
	};
	
	/**************************************************************************\
	*
	* UI
	*
	\**************************************************************************/
	
	/*
	 */
	namespace UI {
		
		/*
		 */
		GridBox nodes_gb;
		
		EditLine angle_x_el;
		EditLine angle_y_el;
		EditLine angle_z_el;
		
		RandomGroup groups[0];
		
		/*
		 */
		void init() {
			angle_x_el.setText(editorFormat(0.0f));
			angle_y_el.setText(editorFormat(0.0f));
			angle_z_el.setText(editorFormat(0.0f));
			
			group_add();
		}
		
		void shutdown() {
			groups.delete();
		}
		
		/*
		 */
		void window_close_clicked() {
			windowHide(window,meta.name);
		}
		
		/*
		 */
		void group_add() {
			RandomGroup group = new RandomGroup(nodes_gb);
			groups.append(group);
		}
		
		void group_remove() {
			if(groups.size() == 1) return;
			
			int index = groups.size() - 1;
			RandomGroup group = groups[index];
			groups.remove(index);
			
			delete group;
		}
		
		/*
		 */
		void randomize_clicked() {
			Node selection[0];
			nodesGetNodes(selection);
			
			Node last_selected = Nodes::node;
			
			// probabilities
			int summ = 0;
			float probabilities[0];
			string paths[0];
			
			foreach(RandomGroup group; groups) {
				int weight = group.getWeight();
				string path = group.getPath();
				
				summ += weight;
				
				if(path != "") {
					probabilities.append(summ);
					paths.append(path);
				}
			}
			
			float i_summ = 1.0f / summ;
			forloop(int i = 0; probabilities.size()) {
				probabilities[i] *= i_summ;
			}
			
			// replace nodes
			Node nodes_to_remove[0] = ();
			Nodes::NodesState_ states[0] = ();
			
			Node new_selection[0];
			for(int i = selection.size() - 1; i >= 0; i--) {
				Node selected_node = selection[i];
				
				int index = -1;
				float probability = rand();
				
				forloop(int j = 0; paths.size()) {
					if(probability < probabilities[j]) {
						index = j;
						break;
					}
				}
				
				if(index != -1) {
					NodeReference node = node_remove(new NodeReference(paths[index]));
					
					Node parent = selected_node.getParent();
					node.setTransform(selected_node.getTransform());
					node.setName(selected_node.getName());
					
					states.append(Nodes::create_state_new((node),selection,Nodes::node));
					
					nodes_to_remove.append(selected_node);
					
					if(!engine.editor.isNode(node)) engine.editor.addNode(node);
					engine.editor.swapNodes(node,selected_node);
					
					for(int child_index = selected_node.getNumChilds() - 1; child_index >= 0; child_index--) {
						Node child = selected_node.getChild(child_index);
						int pos = Nodes::get_node_position(child);
						states.append(Nodes::create_state_reparent((child),(pos),(selected_node),(node),selection,Nodes::node));
						child.setParent(node);
					}
					
					node.setParent(parent);
					
					randomize_rotation(node);
					
					new_selection.append(node);
					
					states.append(Nodes::create_state_remove((selected_node),selection,Nodes::node));
				} else {
					states.append(Nodes::create_state_modify(selection,selected_node));
					randomize_rotation(selected_node);
				}
			}
			
			// remove old nodes
			forloop(int i = 0; nodes_to_remove.size()) Nodes::remove_node(nodes_to_remove[i]);
			
			nodesReload();
			
			if(new_selection.size() != 0) {
				nodesSelectNew(new_selection[0],0);
				forloop(int i = 1; new_selection.size()) {
					nodesSelectNew(new_selection[i],1);
				}
			}
			
			last_selected = Nodes::node;
			
			Nodes::save_states(states);
		}
		
		/*
		 */
		void randomize_rotation(Node node) {
			float angle_x = float(angle_x_el.getText());
			float angle_y = float(angle_y_el.getText());
			float angle_z = float(angle_z_el.getText());
			
			float random_angle_x = rand(-angle_x,angle_x);
			float random_angle_y = rand(-angle_y,angle_y);
			float random_angle_z = rand(-angle_z,angle_z);
			
			mat4 rotation = mat4(node.getRotation());
			rotation *= rotateX(random_angle_x) * rotateY(random_angle_y) * rotateZ(random_angle_z);
			node.setRotation(quat(rotation));
		}
	}
}
