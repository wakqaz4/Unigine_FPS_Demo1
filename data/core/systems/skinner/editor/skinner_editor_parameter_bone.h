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


#ifndef __UNIGINE_SKINNER_EDITOR_PARAMETER_BONE_H__
#define __UNIGINE_SKINNER_EDITOR_PARAMETER_BONE_H__

/*
 */
namespace Unigine::Skinner {
	
	/*
	 */
	class SkinnerEditorParameterBone : Unigine::Schemer::SchemerEditorParameter {
		
		using Unigine::Widgets;
		
		Graph graph_w;				// graph widget
		GraphNode graph_node;		// graph node
		
		Label info_l;				// info label
		ComboBox bone_cb;			// bone combobox
		
		/*
		 */
		SkinnerEditorParameterBone(VBox vbox,Graph g,GraphNode n,ObjectMeshSkinned mesh) {
			
			graph_w = g;
			graph_node = n;
			
			info_l = new Label("bone:");
			vbox.addChild(info_l,ALIGN_LEFT);
			
			bone_cb = new ComboBox();
			forloop(int i = 0; mesh.getNumBones()) {
				string name = mesh.getBoneName(i);
				int item = bone_cb.addItem(name);
				if(name == graph_node.getText()) bone_cb.setCurrentItem(item);
			}
			bone_cb.setCallback(CHANGED,functionid(callback_redirector),this,"bone_changed");
			vbox.addChild(bone_cb,ALIGN_EXPAND);
			
			bone_changed();
		}
		
		~SkinnerEditorParameterBone() {
			delete info_l;
			delete bone_cb;
		}
		
		/*
		 */
		void bone_changed() {
			graph_node.setText(bone_cb.getCurrentItemText());
			graph_w.updateNode(graph_node);
		}
		
		/*
		 */
		void setScript(GraphNode graph_node,SchemerNode node) {
			int ret[0];
			string value = node.getOutputLinkValue("name");
			if(re_search(value,"\"(.+)\"$",ret) == 1) graph_node.setText(ret[0]);
			else log.error("Unigine::Skinner::SkinnerEditorParameterBone::setScript(): unknown node value \"%s\"\n",value);
		}
		
		/*
		 */
		void getScript(SchemerNode node,GraphNode graph_node) {
			node.setOutputLinkValue("name",format("\"%s\"",graph_node.getText()));
		}
	};
	
} /* namespace Unigine::Skinner */

#endif /* __UNIGINE_SKINNER_EDITOR_PARAMETER_BONE_H__ */
