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


#ifndef __UNIGINE_SCHEMER_EDITOR_PARAMETER_FILE_H__
#define __UNIGINE_SCHEMER_EDITOR_PARAMETER_FILE_H__

/*
 */
namespace Unigine::Schemer {
	
	/*
	 */
	class SchemerEditorParameterFile : SchemerEditorParameter {
		
		using Unigine::Widgets;
		
		Graph graph_w;					// graph
		GraphNode graph_node;			// graph node
		
		Label info_l;					// info label
		EditLine name_el;				// name editline
		
		/*
		 */
		SchemerEditorParameterFile(VBox vbox,Graph g,GraphNode n) {
			
			graph_w = g;
			graph_node = n;
			
			info_l = new Label("file:");
			vbox.addChild(info_l,ALIGN_LEFT);
			
			name_el = new EditLine(graph_node.getData());
			name_el.setToolTip(graph_node.getData());
			name_el.setCallback(PRESSED,functionid(callback_redirector),this,"name_pressed");
			name_el.setCallback(FOCUS_OUT,functionid(callback_redirector),this,"name_pressed");
			name_el.setCallback(DOUBLE_CLICKED,functionid(name_double_clicked),this);
			vbox.addChild(name_el,ALIGN_EXPAND);
		}
		
		~SchemerEditorParameterFile() {
			delete info_l;
			delete name_el;
		}
		
		/*
		 */
		void name_pressed() {
			graph_node.setData(name_el.getText());
			graph_node.setText(basename(name_el.getText()));
			name_el.setToolTip(graph_node.getData());
			graph_w.updateNode(graph_node);
		}
		
		void name_double_clicked(SchemerEditorParameterFile file) {
			string name = "";
			if(is_string(file.graph_node.getData())) name = file.graph_node.getData();
			if(dialogFile("Select file","",name)) {
				file.name_el.setText(name);
				file.name_pressed();
			}
		}
		
		/*
		 */
		void setScript(GraphNode graph_node,SchemerNode node) {
			
			int ret[0];
			int data = 0;
			
			string value = node.getOutputLinkValue("name");
			
			if(re_search(value,"\"(.+)\"$",ret) == 1) {
				data = ret[0];
				graph_node.setText(basename(data));
			}
			else {
				log.error("Unigine::Schemer::SchemerEditorParameterFile::setScript(): unknown node value \"%s\"\n",value);
			}
			
			graph_node.setData(data);
		}
		
		/*
		 */
		void getScript(SchemerNode node,GraphNode graph_node) {
			node.setOutputLinkValue("name",format("\"%s\"",graph_node.getData()));
		}
	};
	
} /* namespace Unigine::Schemer */

#endif /* __UNIGINE_SCHEMER_EDITOR_PARAMETER_FILE_H__ */
