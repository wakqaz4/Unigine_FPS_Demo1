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


#ifndef __UNIGINE_TRACKER_EDITOR_DIALOG_NODE_H__
#define __UNIGINE_TRACKER_EDITOR_DIALOG_NODE_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	enum {
		
		EDITOR_DIALOG_NODE_WIDTH = 320,		// dialog node width
		EDITOR_DIALOG_NODE_HEIGHT = 480,	// dialog node height
	};
	
	/**************************************************************************\
	*
	* TrackerEditorDialogNode
	*
	\**************************************************************************/
	
	/*
	 */
	class TrackerEditorDialogNode : Unigine::Widgets::Dialog {
		
		EditLine name_el;			// name editline
		TreeBox nodes_tb;			// nodes treebox
		TreeBox references_tb;		// references treebox
		Button ok_b;				// ok button
		
		int nodes[];				// node items
		int references[];			// reference items
		
		// constructor
		TrackerEditorDialogNode(string str) : Dialog(str) {
			
			// ok button
			ok_b = getOkButton();
			
			// grid box
			VBox vbox = new VBox(0,4);
			GridBox gridbox = new GridBox(2,4,4);
			vbox.addChild(gridbox,ALIGN_EXPAND);
			addChild(vbox);
			
			// node name
			name_el = new EditLine();
			name_el.setCallback(CHANGED,functionid(callback_redirector),this,"name_changed");
			name_el.setCallback(PRESSED,functionid(callback_redirector),this,"ok_clicked");
			gridbox.addChild(new Label(TR("Name:")),ALIGN_RIGHT);
			gridbox.addChild(name_el,ALIGN_EXPAND);
			
			// vpaned
			VPaned vpaned = new VPaned();
			vpaned.setValue(32767);
			vpaned.setWidth(EDITOR_DIALOG_NODE_WIDTH);
			vpaned.setHeight(EDITOR_DIALOG_NODE_HEIGHT);
			addChild(vpaned,ALIGN_EXPAND);
			
			// scrollbox
			ScrollBox scrollbox = new ScrollBox();
			vpaned.addChild(scrollbox,ALIGN_EXPAND);
			
			// nodes treebox
			nodes_tb = new TreeBox();
			nodes_tb.setTexture("core/systems/widgets/images/widget_dialog_node.png");
			nodes_tb.setCallback(CHANGED,functionid(callback_redirector),this,"nodes_changed");
			nodes_tb.setCallback(DOUBLE_CLICKED,functionid(callback_redirector),this,"ok_clicked");
			scrollbox.addChild(nodes_tb,ALIGN_EXPAND);
			
			// scrollbox
			scrollbox = new ScrollBox();
			vpaned.addChild(scrollbox,ALIGN_EXPAND);
			
			// references treebox
			references_tb = new TreeBox();
			references_tb.setTexture("core/systems/widgets/images/widget_dialog_node.png");
			references_tb.setCallback(CHANGED,functionid(callback_redirector),this,"references_changed");
			references_tb.setCallback(DOUBLE_CLICKED,functionid(callback_redirector),this,"ok_clicked");
			scrollbox.addChild(references_tb,ALIGN_EXPAND);
		}
		
		/*
		 */
		void setNode(Node target) {
			
			// disable callbacks
			name_el.setCallbackEnabled(CHANGED,0);
			nodes_tb.setCallbackEnabled(CHANGED,0);
			references_tb.setCallbackEnabled(CHANGED,0);
			
			// clear name
			name_el.setText("");
			
			// clear nodes
			nodes.clear();
			nodes_tb.clear();
			
			// clear references
			references.clear();
			references_tb.clear();
			
			// nodes treebox
			forloop(int i = 0; engine.editor.getNumNodes()) {
				Node node = node_cast(engine.editor.getNode(i));
				string name = node.getName();
				if(strlen(name) == 0) name = node.getTypeName();
				int id = nodes_tb.addItem(name,node.getType());
				nodes.append(node,id);
			}
			
			// nodes hierarchy
			forloop(int i = 0; engine.editor.getNumNodes()) {
				Node node = engine.editor.getNode(i);
				Node parent = node.getParent();
				if(parent == NULL || nodes.check(parent) == 0) continue;
				nodes_tb.setItemParent(nodes[node],nodes[parent]);
			}
			
			// target node isn't editor node
			if(engine.editor.isNode(target) == 0) {
				
				// check root node
				int is_root_node(Node node) {
					if(node == target) return 1;
					if(node.getType() == NODE_REFERENCE) {
						Node reference = NodeReference(node).getNode();
						if(reference != NULL) {
							reference = node_cast(reference);
							if(is_root_node(reference)) return 1;
						}
					}
					forloop(int i = 0; node.getNumChilds()) {
						Node child = node_cast(node.getChild(i));
						if(is_root_node(child)) return 1;
					}
					return 0;
				}
				
				// process editor node references
				forloop(int i = 0; engine.editor.getNumNodes()) {
					Node node = node_cast(engine.editor.getNode(i));
					if(node.getType() != NODE_REFERENCE) continue;
					if(is_root_node(node) == 0) continue;
					nodes_tb.setCurrentItem(nodes[node]);
					update_reference(NodeReference(node).getNode());
					name_el.setText(target.getName());
					name_changed();
					break;
				}
			}
			// target node is editor node
			else {
				nodes_tb.setCurrentItem(nodes[target]);
				name_el.setText(target.getName());
			}
			
			// enable callbacks
			name_el.setCallbackEnabled(CHANGED,1);
			nodes_tb.setCallbackEnabled(CHANGED,1);
			references_tb.setCallbackEnabled(CHANGED,1);
		}
		
		Node getNode() {
			if(references.size()) {
				int id = references_tb.getCurrentItem();
				Node node = references.find(id,NULL);
				if(node != NULL) return node;
			}
			int id = nodes_tb.getCurrentItem();
			return nodes.find(id,NULL);
		}
		
		/*
		 */
		void update() {
			Node node = getNode();
			if(node != NULL) {
				if(node.isObject()) engine.visualizer.renderObject(node_cast(node),vec4_one);
				else node.renderVisualizer();
			}
		}
		
		/*
		 */
		void update_reference(Node node) {
			
			// clear references
			references.clear();
			references_tb.clear();
			
			// process child nodes
			void process_node(Node node,int parent) {
				if(node == NULL) return;
				node = node_cast(node);
				string name = node.getName();
				if(strlen(name) == 0) name = node.getTypeName();
				int id = references_tb.addItem(name,node.getType());
				references_tb.setItemParent(id,parent);
				references.append(node,id);
				if(node.getType() == NODE_REFERENCE) {
					process_node(NodeReference(node).getNode(),parent);
				}
				forloop(int i = 0; node.getNumChilds()) {
					Node child = node.getChild(i);
					process_node(child,id);
				}
			}
			
			process_node(node,-1);
		}
		
		/*
		 */
		void name_changed() {
			
			// node reference
			if(references.size()) {
				
				int old_id = references_tb.getCurrentItem();
				
				// select reference by name
				name_el.setCallbackEnabled(CHANGED,0);
				references_tb.setCallbackEnabled(CHANGED,0);
				int length = references_tb.selectByName(name_el.getText());
				name_el.setText(substr(name_el.getText(),0,length));
				name_el.setCallbackEnabled(CHANGED,1);
				references_tb.setCallbackEnabled(CHANGED,1);
				
				// current node
				int id = references_tb.getCurrentItem();
				if(old_id != id) references_changed();
			}
			// editor node
			else {
				
				int old_id = nodes_tb.getCurrentItem();
				
				// select node by name
				name_el.setCallbackEnabled(CHANGED,0);
				nodes_tb.setCallbackEnabled(CHANGED,0);
				int length = nodes_tb.selectByName(name_el.getText());
				name_el.setText(substr(name_el.getText(),0,length));
				name_el.setCallbackEnabled(CHANGED,1);
				nodes_tb.setCallbackEnabled(CHANGED,1);
				
				// current node
				int id = nodes_tb.getCurrentItem();
				if(old_id != id) nodes_changed();
			}
		}
		
		void nodes_changed() {
			
			// clear name
			name_el.setCallbackEnabled(CHANGED,0);
			name_el.setText("");
			name_el.setCallbackEnabled(CHANGED,1);
			ok_b.setEnabled(0);
			
			// selected node
			int id = nodes_tb.getCurrentItem();
			Node node = nodes.find(id,NULL);
			if(node == NULL) return;
			
			// update references
			if(node.getType() == NODE_REFERENCE) {
				update_reference(NodeReference(node).getNode());
			} else {
				references.clear();
				references_tb.clear();
			}
			
			// set node name
			name_el.setCallbackEnabled(CHANGED,0);
			name_el.setText(node.getName());
			name_el.setCallbackEnabled(CHANGED,1);
			
			// ok button
			if(node.getType() == NODE_REFERENCE) {
				ok_b.setEnabled(nodes_tb.getCurrentItemText() != references_tb.getCurrentItemText());
			} else {
				ok_b.setEnabled(1);
			}
		}
		
		void references_changed() {
			
			// clear name
			name_el.setCallbackEnabled(CHANGED,0);
			name_el.setText("");
			name_el.setCallbackEnabled(CHANGED,1);
			ok_b.setEnabled(0);
			
			// selected node
			int id = references_tb.getCurrentItem();
			Node node = references.find(id,NULL);
			if(node == NULL) return;
			if(strlen(node.getName()) == 0) return;
			
			// set node
			name_el.setCallbackEnabled(CHANGED,0);
			name_el.setText(node.getName());
			name_el.setCallbackEnabled(CHANGED,1);
			
			// ok button
			ok_b.setEnabled(nodes_tb.getCurrentItemText() != references_tb.getCurrentItemText());
		}
		
		/*
		 */
		void ok_clicked() {
			ok_b.runCallback(CLICKED);
		}
		
		/*
		 */
		void callback_redirector(TrackerEditorDialogNode dialog,string name) {
			dialog.call(name);
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_EDITOR_DIALOG_NODE_H__ */
