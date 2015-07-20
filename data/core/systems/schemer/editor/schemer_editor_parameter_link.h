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


#ifndef __UNIGINE_SCHEMER_EDITOR_PARAMETER_LINK_H__
#define __UNIGINE_SCHEMER_EDITOR_PARAMETER_LINK_H__

/*
 */
namespace Unigine::Schemer {
	
	/*
	 */
	class SchemerEditorParameterLink : SchemerEditorParameter {
		
		using Unigine::Widgets;
		
		Graph graph_w;					// graph
		GraphNode graph_node;			// graph node
		GraphAnchor graph_anchor;		// graph anchor
		string label;					// anchor label
		
		Label info_l;					// info label
		EditLine value_el;				// value editline
		
		/*
		 */
		SchemerEditorParameterLink(VBox vbox,Graph g,GraphNode n,GraphAnchor a,string l) {
			
			graph_w = g;
			graph_node = n;
			graph_anchor = a;
			label = l;
			
			info_l = new Label(format("%s:",label));
			vbox.addChild(info_l,ALIGN_LEFT);
			
			value_el = new EditLine(a.getData());
			value_el.setCallback(PRESSED,functionid(callback_redirector),this,"value_pressed");
			value_el.setCallback(FOCUS_OUT,functionid(callback_redirector),this,"value_pressed");
			vbox.addChild(value_el,ALIGN_EXPAND);
		}
		
		~SchemerEditorParameterLink() {
			delete info_l;
			delete value_el;
		}
		
		/*
		 */
		void value_pressed() {
			string value = value_el.getText();
			graph_anchor.setData(value);
			if(strlen(value)) graph_anchor.setText(format("%s (%s)",label,value));
			else graph_anchor.setText(label);
			graph_w.updateNode(graph_node);
		}
	};
	
} /* namespace Unigine::Schemer */

#endif /* __UNIGINE_SCHEMER_EDITOR_PARAMETER_LINK_H__ */
