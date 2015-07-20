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


#ifndef __UNIGINE_WIDGET_DIALOG_NODE_H__
#define __UNIGINE_WIDGET_DIALOG_NODE_H__

#include <core/systems/widgets/widget_vbox.h>
#include <core/systems/widgets/widget_label.h>
#include <core/systems/widgets/widget_gridbox.h>
#include <core/systems/widgets/widget_combobox.h>
#include <core/systems/widgets/widget_editline.h>
#include <core/systems/widgets/widget_scrollbox.h>
#include <core/systems/widgets/widget_treebox.h>
#include <core/systems/widgets/widget_dialog.h>

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	enum {
		
		DIALOG_NODE_WIDTH = 480,	// dialog width
		DIALOG_NODE_HEIGHT = 320,	// dialog height
	};
	
	/*
	 */
	class DialogNode : Dialog {
		
		Button ok_b;				// ok button
		EditLine name_el;			// name editline
		TreeBox nodes_tb;			// nodes treebox
		
		int nodes[];				// node items
		
		// constructor
		DialogNode(string str = 0) : Dialog(str) {
			
			// ok button
			ok_b = getOkButton();
			
			// grid box
			VBox vbox = new VBox(0,4);
			GridBox gridbox = new GridBox(2,4,4);
			vbox.addChild(gridbox,ALIGN_EXPAND);
			addChild(vbox);
			
			// node name
			name_el = new EditLine();
			name_el.setCallback(CHANGED,functionid(callback_redirector),this,functionid(name_changed));
			name_el.setCallback(PRESSED,functionid(callback_redirector),this,functionid(ok_clicked));
			gridbox.addChild(new Label(TR("Name:")),ALIGN_RIGHT);
			gridbox.addChild(name_el,ALIGN_EXPAND);
			
			// scrollbox
			ScrollBox scrollbox = new ScrollBox();
			scrollbox.setWidth(DIALOG_NODE_WIDTH);
			scrollbox.setHeight(DIALOG_NODE_HEIGHT);
			addChild(scrollbox,ALIGN_EXPAND);
			
			// nodes treebox
			nodes_tb = new TreeBox();
			nodes_tb.setTexture("core/systems/widgets/images/widget_dialog_node.png");
			nodes_tb.setCallback(CHANGED,functionid(callback_redirector),this,functionid(nodes_changed));
			nodes_tb.setCallback(DOUBLE_CLICKED,functionid(callback_redirector),this,functionid(ok_clicked));
			scrollbox.addChild(nodes_tb,ALIGN_EXPAND);
		}
		
		/*
		 */
		void setNodeID(int node_id) {
			
			// disable callbacks
			name_el.setCallbackEnabled(CHANGED,0);
			nodes_tb.setCallbackEnabled(CHANGED,0);
			
			// clear name
			name_el.setText("");
			
			// clear nodes
			nodes.clear();
			nodes_tb.clear();
			
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
			
			// get node
			Node node = NULL;
			if(engine.world.isNode(node_id)) {
				node = engine.world.getNode(node_id);
			}
			
			// select node
			if(engine.editor.isNode(node)) {
				name_el.setText(node.getName());
				nodes_tb.setCurrentItem(nodes[node]);
			}
			
			// enable callbacks
			name_el.setCallbackEnabled(CHANGED,1);
			nodes_tb.setCallbackEnabled(CHANGED,1);
		}
		
		int getNodeID() {
			int id = nodes_tb.getCurrentItem();
			Node node = nodes.find(id,NULL);
			if(node != NULL) return node.getID();
			return 0;
		}
		
		/*
		 */
		void update() {
			int id = nodes_tb.getCurrentItem();
			Node node = nodes.find(id,NULL);
			if(node != NULL) {
				if(node.isObject()) engine.visualizer.renderObject(node_cast(node),vec4_one);
				else node.renderVisualizer();
			}
		}
		
		/*
		 */
		void name_changed() {
			
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
			
			// set node name
			name_el.setCallbackEnabled(CHANGED,0);
			name_el.setText(node.getName());
			name_el.setCallbackEnabled(CHANGED,1);
			
			ok_b.setEnabled(1);
		}
		
		/*
		 */
		void ok_clicked() {
			ok_b.runCallback(CLICKED);
		}
		
		/*
		 */
		void callback_redirector(DialogNode dialog,string name) {
			dialog.call(name);
		}
	};
	
	/*
	 */
	int dialogNode(string str,int &id) {
		
		DialogNode dialog;
		
		// create dialog
		if(dialog == NULL) {
			dialog = new DialogNode(str);
			dialog.setSizeable(1);
		}
		
		// dialog node
		dialog.setText(str);
		dialog.setNodeID(id);
		
		// show dialog
		dialog.arrange();
		addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
		dialog.setPermanentFocus();
		
		// wait dialog
		while(dialog.isDone() == 0) {
			dialog.update();
			wait;
		}
		
		// dialog result
		if(dialog.isOkClicked()) id = dialog.getNodeID();
		
		// hide dialog
		removeChild(dialog);
		
		return dialog.isOkClicked();
	}
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_DIALOG_NODE_H__ */
