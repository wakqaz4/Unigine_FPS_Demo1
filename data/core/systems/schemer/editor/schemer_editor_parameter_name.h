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


#ifndef __UNIGINE_SCHEMER_EDITOR_PARAMETER_NAME_H__
#define __UNIGINE_SCHEMER_EDITOR_PARAMETER_NAME_H__

/*
 */
namespace Unigine::Schemer {
	
	/*
	 */
	class SchemerEditorParameterName : SchemerEditorParameter {
		
		using Unigine::Widgets;
		
		Graph graph_w;					// graph
		GraphNode graph_node;			// graph node
		
		Label info_l;					// info label
		EditLine name_el;				// name editline
		
		/*
		 */
		SchemerEditorParameterName(VBox vbox,Graph g,GraphNode n) {
			
			graph_w = g;
			graph_node = n;
			
			info_l = new Label("name:");
			vbox.addChild(info_l,ALIGN_LEFT);
			
			name_el = new EditLine(graph_node.getText());
			name_el.setCallback(PRESSED,functionid(callback_redirector),this,"name_pressed");
			name_el.setCallback(FOCUS_OUT,functionid(callback_redirector),this,"name_pressed");
			vbox.addChild(name_el,ALIGN_EXPAND);
		}
		
		~SchemerEditorParameterName() {
			delete info_l;
			delete name_el;
		}
		
		/*
		 */
		void name_pressed() {
			graph_node.setText(name_el.getText());
			graph_w.updateNode(graph_node);
		}
		
		/*
		 */
		void setScript(GraphNode graph_node,SchemerNode node) {
			graph_node.setText(node.getName());
		}
		
		/*
		 */
		void getScript(SchemerNode node,GraphNode graph_node) {
			node.setName(graph_node.getText());
		}
	};
	
} /* namespace Unigine::Schemer */

#endif /* __UNIGINE_SCHEMER_EDITOR_PARAMETER_NAME_H__ */
