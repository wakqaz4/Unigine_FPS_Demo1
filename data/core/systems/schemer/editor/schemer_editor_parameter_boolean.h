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


#ifndef __UNIGINE_SCHEMER_EDITOR_PARAMETER_BOOLEAN_H__
#define __UNIGINE_SCHEMER_EDITOR_PARAMETER_BOOLEAN_H__

/*
 */
namespace Unigine::Schemer {
	
	/*
	 */
	class SchemerEditorParameterBoolean : SchemerEditorParameter {
		
		using Unigine::Widgets;
		
		Graph graph_w;					// graph
		GraphNode graph_node;			// graph node
		
		CheckBox value_cb;				// value checkbox
		
		/*
		 */
		SchemerEditorParameterBoolean(VBox vbox,Graph g,GraphNode n) {
			
			graph_w = g;
			graph_node = n;
			
			value_cb = new CheckBox("Value");
			value_cb.setChecked(graph_node.getData());
			value_cb.setCallback(CHANGED,functionid(callback_redirector),this,"value_changed");
			vbox.addChild(value_cb,ALIGN_LEFT);
			
			update();
		}
		
		~SchemerEditorParameterBoolean() {
			delete value_cb;
		}
		
		/*
		 */
		void update() {
			graph_node.setText((graph_node.getData()) ? "true" : "false");
			graph_w.updateNode(graph_node);
		}
		
		/*
		 */
		void value_changed() {
			graph_node.setData(value_cb.isChecked());
			update();
		}
		
		/*
		 */
		void setScript(GraphNode graph_node,SchemerNode node) {
			graph_node.setData(int(node.getOutputLinkValue("value")));
			graph_node.setText((graph_node.getData()) ? "true" : "false");
		}
		
		/*
		 */
		void getScript(SchemerNode node,GraphNode graph_node) {
			node.setOutputLinkValue("value",string(graph_node.getData()));
		}
	};
	
} /* namespace Unigine::Schemer */

#endif /* __UNIGINE_SCHEMER_EDITOR_PARAMETER_BOOLEAN_H__ */
