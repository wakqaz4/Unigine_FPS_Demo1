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


#ifndef __UNIGINE_SCHEMER_EDITOR_H__
#define __UNIGINE_SCHEMER_EDITOR_H__

#include <core/systems/widgets/widget.h>
#include <core/systems/widgets/widget_vbox.h>
#include <core/systems/widgets/widget_hbox.h>
#include <core/systems/widgets/widget_graph.h>
#include <core/systems/widgets/widget_label.h>
#include <core/systems/widgets/widget_button.h>
#include <core/systems/widgets/widget_treebox.h>
#include <core/systems/widgets/widget_checkbox.h>
#include <core/systems/widgets/widget_combobox.h>
#include <core/systems/widgets/widget_editline.h>
#include <core/systems/widgets/widget_scrollbox.h>
#include <core/systems/widgets/widget_dialog_file.h>

#include <core/systems/schemer/schemer.h>
#include <core/systems/schemer/editor/schemer_editor_parameter.h>
#include <core/systems/schemer/editor/schemer_editor_parameter_name.h>
#include <core/systems/schemer/editor/schemer_editor_parameter_boolean.h>
#include <core/systems/schemer/editor/schemer_editor_parameter_constant.h>
#include <core/systems/schemer/editor/schemer_editor_parameter_file.h>
#include <core/systems/schemer/editor/schemer_editor_parameter_link.h>

/*
 */
namespace Unigine::Schemer {
	
	/*
	 */
	enum {
		
		EDITOR_TYPES_WIDTH = 192,		// types width
		EDITOR_GRAPH_WIDTH = 8192,		// graph width
		EDITOR_GRAPH_HEIGHT = 8192,		// graph height
		EDITOR_PARAMETERS_WIDTH = 128,	// parameters width
	};
	
	/*
	 */
	vec4 path_color = vec4(1.0f,1.0f,1.0f,1.0f);
	vec4 link_color = vec4(0.0f,1.0f,0.0f,1.0f);
	
	/**************************************************************************\
	*
	* SchemerEditor
	*
	\**************************************************************************/
	
	/*
	 */
	class SchemerEditor {
		
		using Unigine::Widgets;
		
		Schemer schemer;						// schemer
		
		HBox main_hbox;							// main hbox
		
		HBox types_hbox;						// types hbox
		VBox types_vbox;						// types vbox
		
		ScrollBox types_sb;						// types scrollbox
		TreeBox types_tb;						// types treebox
		
		VBox graph_vbox;						// graph vbox
		ScrollBox graph_sb;						// graph scrollbox
		Graph graph_w;							// graph widget
		
		HBox parameters_hbox;					// parameters hbox
		VBox parameters_vbox;					// parameters vbox
		
		Label name_el;							// name label
		
		string parameter_names[];				// parameter names
		int parameter_arguments[];				// parameter arguments
		SchemerEditorParameter parameters[0];	// current parameters
		
		vec4 path_colors[];						// path colors
		vec4 link_colors[];						// link colors
		
		/*
		 */
		SchemerEditor(Schemer s) {
			
			schemer = s;
			
			// main hbox
			main_hbox = new HBox();
			
			// types panel
			types_hbox = new HBox();
			types_vbox = new VBox(4,4);
			main_hbox.addChild(types_hbox);
			types_hbox.addChild(types_vbox,ALIGN_EXPAND);
			
			// types treebox
			types_sb = new ScrollBox();
			types_tb = new TreeBox();
			types_sb.setWidth(EDITOR_TYPES_WIDTH);
			types_tb.setCallback(DRAG_DROP,functionid(callback_redirector),this,"types_drag_drop");
			types_vbox.addChild(types_sb,ALIGN_EXPAND);
			types_sb.addChild(types_tb,ALIGN_EXPAND);
			
			// schemer types
			string types[0];
			forloop(int i = 0; schemer.getNumBlocks()) {
				types.append(SchemerBlock(schemer.getBlock(i)).getType());
			}
			types_tb.setHierarchy(types,".");
			
			// graph
			graph_vbox = new VBox();
			graph_sb = new ScrollBox();
			graph_w = new Graph(EDITOR_GRAPH_WIDTH,EDITOR_GRAPH_HEIGHT);
			graph_w.setCallback(GRAPH_NODE_CHANGED,functionid(graph_callback_redirector),this,GRAPH_NODE_CHANGED);
			graph_w.setCallback(GRAPH_NODE_CREATED,functionid(graph_callback_redirector),this,GRAPH_NODE_CREATED);
			graph_w.setCallback(GRAPH_NODE_REMOVED,functionid(graph_callback_redirector),this,GRAPH_NODE_REMOVED);
			graph_w.setCallback(GRAPH_JOINT_CHANGED,functionid(graph_callback_redirector),this,GRAPH_JOINT_CHANGED);
			graph_w.setCallback(GRAPH_JOINT_CREATED,functionid(graph_callback_redirector),this,GRAPH_JOINT_CREATED);
			graph_w.setCallback(GRAPH_JOINT_REMOVED,functionid(graph_callback_redirector),this,GRAPH_JOINT_REMOVED);
			main_hbox.addChild(graph_vbox,ALIGN_EXPAND);
			graph_vbox.addChild(graph_sb,ALIGN_EXPAND);
			graph_sb.addChild(graph_w,ALIGN_EXPAND);
			
			// parameters panel
			parameters_hbox = new HBox();
			parameters_vbox = new VBox(4,4);
			main_hbox.addChild(parameters_hbox);
			parameters_hbox.addChild(parameters_vbox,ALIGN_EXPAND);
			
			// name label
			name_el = new Label();
			name_el.setWidth(EDITOR_PARAMETERS_WIDTH);
			parameters_vbox.addChild(name_el,ALIGN_LEFT);
			
			// register parameters
			parameter_names.append("schemer.boolean","SchemerEditorParameterBoolean");
			parameter_names.append("schemer.constant","SchemerEditorParameterConstant");
			parameter_names.append("schemer.file","SchemerEditorParameterFile");
		}
		
		~SchemerEditor() {
			
			delete main_hbox;
			
			delete types_hbox;
			delete types_vbox;
			
			delete types_sb;
			delete types_tb;
			
			delete graph_vbox;
			delete graph_sb;
			delete graph_w;
			
			delete parameters_hbox;
			delete parameters_vbox;
			
			delete name_el;
			
			parameters.delete();
		}
		
		/*
		 */
		Widget getWidget() {
			return main_hbox;
		}
		
		void arrange() {
			graph_sb.setHScrollValue((graph_sb.getHScrollObjectSize() - graph_sb.getHScrollFrameSize()) / 2);
			graph_sb.setVScrollValue((graph_sb.getVScrollObjectSize() - graph_sb.getVScrollFrameSize()) / 2);
		}
		
		/*
		 */
		void addParameter(string type,string name,int argument) {
			parameter_names.append(type,name);
			parameter_arguments.append(type,argument);
		}
		
		/*
		 */
		void setPathColor(int mask,vec4 color) {
			path_colors.append(mask,color);
		}
		
		void setLinkColor(int mask,vec4 color) {
			link_colors.append(mask,color);
		}
		
		/*
		 */
		void clear() {
			parameters.delete();
			graph_w.clear();
			arrange();
		}
		
		/**********************************************************************\
		*
		* Update
		*
		\**********************************************************************/
		
		/*
		 */
		void update_parameters(GraphNode graph_node) {
			
			name_el.setText("");
			
			parameters.delete();
			
			if(graph_node == NULL) return;
			
			// node type
			string type = graph_node.getName();
			name_el.setText(type);
			
			// node parameter name
			string name = parameter_names.check(type,"SchemerEditorParameterName");
			int argument = parameter_arguments.check(type,0);
			
			// create node parameter
			if(argument != 0) parameters.append(new(name,parameters_vbox,graph_w,graph_node,argument));
			else parameters.append(new(name,parameters_vbox,graph_w,graph_node));
			
			// node block
			SchemerBlock block = schemer.findBlock(type);
			
			// create input link values
			foreach(SchemerAnchor anchor; block.input_links) {
				string name = anchor.getName();
				if(anchor.getEdit()) parameters.append(new SchemerEditorParameterLink(parameters_vbox,graph_w,graph_node,graph_node.getAnchor(name),anchor.getLabel()));
			}
			
			// create output link values
			foreach(SchemerAnchor anchor; block.output_links) {
				string name = anchor.getName();
				if(anchor.getEdit()) parameters.append(new SchemerEditorParameterLink(parameters_vbox,graph_w,graph_node,graph_node.getAnchor(name),anchor.getLabel()));
			}
		}
		
		/**********************************************************************\
		*
		* Load/save script
		*
		\**********************************************************************/
		
		/*
		 */
		int loadScript(string name) {
			
			// load script
			SchemerScript script = new SchemerScript(schemer);
			if(script.load(name) == 0) {
				log.error("Unigine::Schemer::SchemerEditor::loadScript(): can't load \"%s\" script\n",name);
				return 0;
			}
			
			// set script
			if(setScript(script) == 0) {
				log.error("Unigine::Schemer::SchemerEditor::loadScript(): can't set \"%s\" script\n",name);
				delete script;
				return 0;
			}
			
			delete script;
			return 1;
		}
		
		/*
		 */
		int saveScript(string name) {
			
			// get script
			SchemerScript script = getScript();
			if(script == NULL) {
				log.error("Unigine::Schemer::SchemerEditor::saveScript(): can't get \"%s\" script\n",name);
				return 0;
			}
			
			// save script
			if(script.save(name) == 0) {
				log.error("Unigine::Schemer::SchemerEditor::saveScript(): can't save \"%s\" script\n",name);
				delete script;
				return 0;
			}
			
			delete script;
			return 1;
		}
		
		/**********************************************************************\
		*
		* Scripts
		*
		\**********************************************************************/
		
		/*
		 */
		int get_num_nodes(string name) {
			int num = 0;
			forloop(int i = 0; graph_w.getNumNodes()) {
				GraphNode n = graph_w.getNode(i);
				if(n.getName() == name) num++;
			}
			return num;
		}
		
		/*
		 */
		GraphNode create_node(string type,SchemerNode node = NULL) {
			
			SchemerBlock block = schemer.findBlock(type);
			GraphNode graph_node = new GraphNode(type);
			
			GraphAnchor graph_anchors[];
			
			// input path anchors
			foreach(SchemerAnchor anchor; block.input_paths) {
				int mask = anchor.getMask() | GRAPH_ENTRY;
				int align = ALIGN_LEFT;
				if(anchor.getAlign() == "right") align = ALIGN_RIGHT;
				else if(anchor.getAlign() == "bottom") align = ALIGN_BOTTOM;
				vec4 color = path_colors.check(anchor.getMask(),path_color);
				graph_anchors.append(anchor.getID(),new GraphAnchor(mask,align,GRAPH_SQUARE,anchor.getName(),anchor.getLabel(),color));
			}
			
			// output path anchors
			foreach(SchemerAnchor anchor; block.output_paths) {
				int mask = anchor.getMask();
				int align = ALIGN_RIGHT;
				if(anchor.getAlign() == "left") align = ALIGN_LEFT;
				else if(anchor.getAlign() == "bottom") align = ALIGN_BOTTOM;
				vec4 color = path_colors.check(anchor.getMask(),path_color);
				graph_anchors.append(anchor.getID(),new GraphAnchor(mask,align,GRAPH_CIRCLE,anchor.getName(),anchor.getLabel(),color));
			}
			
			// input link anchors
			foreach(SchemerAnchor anchor; block.input_links) {
				int mask = (anchor.getMask() << 8) | GRAPH_ENTRY;
				int align = ALIGN_LEFT;
				if(anchor.getAlign() == "right") align = ALIGN_RIGHT;
				else if(anchor.getAlign() == "bottom") align = ALIGN_BOTTOM;
				vec4 color = link_colors.check(anchor.getMask(),link_color);
				string name = anchor.getName();
				string label = anchor.getLabel();
				string value = anchor.getValue();
				if(node != NULL) value = node.getInputLinkValue(name);
				if(anchor.getEdit() && strlen(value)) label = format("%s (%s)",label,value);
				graph_anchors.append(anchor.getID(),new GraphAnchor(mask,align,GRAPH_TRIANGLE,name,label,color,value));
			}
			
			// output link anchors
			foreach(SchemerAnchor anchor; block.output_links) {
				int mask = anchor.getMask() << 8;
				int align = ALIGN_RIGHT;
				if(anchor.getAlign() == "left") align = ALIGN_LEFT;
				else if(anchor.getAlign() == "bottom") align = ALIGN_BOTTOM;
				vec4 color = link_colors.check(anchor.getMask(),link_color);
				string name = anchor.getName();
				string label = anchor.getLabel();
				string value = anchor.getValue();
				if(node != NULL) value = node.getOutputLinkValue(name);
				if(anchor.getEdit() && strlen(value)) label = format("%s (%s)",label,value);
				graph_anchors.append(anchor.getID(),new GraphAnchor(mask,align,GRAPH_TRIANGLE,name,label,color,value));
			}
			
			// add anchors
			foreach(GraphAnchor graph_anchor; graph_anchors) {
				graph_node.addAnchor(graph_anchor);
			}
			
			// entry node name
			if(type == "schemer.entry") {
				int num = get_num_nodes("schemer.entry");
				if(num == 0) graph_node.setText("main");
				else graph_node.setText(format("entry_%d",num));
			}
			// input node name
			else if(type == "schemer.input") {
				int num = get_num_nodes("schemer.input");
				if(num == 0) graph_node.setText("input");
				else graph_node.setText(format("input_%d",num));
			}
			// output node name
			else if(type == "schemer.output") {
				int num = get_num_nodes("schemer.output");
				if(num == 0) graph_node.setText("output");
				else graph_node.setText(format("output_%d",num));
			}
			
			return graph_node;
		}
		
		/*
		 */
		int setScript(SchemerScript script) {
			
			clear();
			
			// disable node callback
			graph_w.setCallbackEnabled(GRAPH_NODE_CHANGED,0);
			
			// create nodes
			forloop(int i = 0; script.getNumNodes()) {
				SchemerNode node = script.getNode(i);
				
				// create node
				string type = node.getType();
				GraphNode graph_node = create_node(type,node);
				graph_node.setPosition(node.getPositionX(),node.getPositionY());
				
				// set node parameters
				string name = parameter_names.check(type,"SchemerEditorParameterName");
				call(name + "::setScript",graph_node,node);
				
				// add node
				graph_w.addNode(graph_node);
			}
			
			// enable node callback
			graph_w.setCallbackEnabled(GRAPH_NODE_CHANGED,1);
			
			// create path joints
			forloop(int i = 0; script.getNumPathJoints()) {
				SchemerJoint joint = script.getPathJoint(i);
				GraphNode graph_node_0 = graph_w.getNode(script.findNode(joint.getNode0()));
				GraphNode graph_node_1 = graph_w.getNode(script.findNode(joint.getNode1()));
				GraphAnchor graph_anchor_0 = graph_node_0.getAnchor(joint.getAnchor0());
				GraphAnchor graph_anchor_1 = graph_node_1.getAnchor(joint.getAnchor1());
				GraphJoint graph_joint = new GraphJoint(graph_anchor_0,graph_anchor_1);
				graph_w.addJoint(graph_joint);
			}
			
			// create link joints
			forloop(int i = 0; script.getNumLinkJoints()) {
				SchemerJoint joint = script.getLinkJoint(i);
				GraphNode graph_node_0 = graph_w.getNode(script.findNode(joint.getNode0()));
				GraphNode graph_node_1 = graph_w.getNode(script.findNode(joint.getNode1()));
				GraphAnchor graph_anchor_0 = graph_node_0.getAnchor(joint.getAnchor0());
				GraphAnchor graph_anchor_1 = graph_node_1.getAnchor(joint.getAnchor1());
				GraphJoint graph_joint = new GraphJoint(graph_anchor_0,graph_anchor_1);
				graph_w.addJoint(graph_joint);
			}
			
			// clear focus
			graph_w.setCurrentNode(NULL);
			
			return 1;
		}
		
		/*
		 */
		SchemerScript getScript() {
			
			// check graph nodes
			if(graph_w.getNumNodes() == 0) return NULL;
			
			// create script
			SchemerScript script = new SchemerScript(schemer);
			
			// create nodes
			forloop(int i = 0; graph_w.getNumNodes()) {
				GraphNode graph_node = graph_w.getNode(i);
				
				// create node
				string type = graph_node.getName();
				SchemerBlock block = schemer.findBlock(type);
				SchemerNode node = new SchemerNode(block,i);
				node.setPosition(graph_node.getPositionX(),graph_node.getPositionY());
				
				// get input link values
				foreach(SchemerAnchor anchor; block.input_links) {
					string name = anchor.getName();
					GraphAnchor graph_anchor = graph_node.getAnchor(name);
					if(anchor.getValue() == graph_anchor.getData()) continue;
					node.setInputLinkValue(name,graph_anchor.getData());
				}
				
				// get output link values
				foreach(SchemerAnchor anchor; block.output_links) {
					string name = anchor.getName();
					GraphAnchor graph_anchor = graph_node.getAnchor(name);
					if(anchor.getValue() == graph_anchor.getData()) continue;
					node.setOutputLinkValue(name,graph_anchor.getData());
				}
				
				// get node parameters
				string name = parameter_names.check(type,"SchemerEditorParameterName");
				call(name + "::getScript",node,graph_node);
				
				// add node
				if(script.addNode(node) == 0) {
					log.error("Unigine::Schemer::SchemerEditor::getScript(): can't add \"%s\" node\n",type);
					delete script;
					return NULL;
				}
			}
			
			// create joints
			forloop(int i = 0; graph_w.getNumJoints()) {
				GraphJoint graph_joint = graph_w.getJoint(i);
				
				// create joint
				GraphAnchor graph_anchor_0 = graph_joint.getAnchor0();
				GraphAnchor graph_anchor_1 = graph_joint.getAnchor1();
				SchemerNode node_0 = script.getNode(graph_w.findNode(graph_anchor_0.getNode()));
				SchemerNode node_1 = script.getNode(graph_w.findNode(graph_anchor_1.getNode()));
				SchemerJoint joint = new SchemerJoint(node_0,node_1);
				joint.setAnchor0(graph_anchor_0.getName());
				joint.setAnchor1(graph_anchor_1.getName());
				
				// create path joint
				if(graph_anchor_0.getShape() == GRAPH_CIRCLE) {
					if(script.addPathJoint(joint) == 0) {
						log.error("Unigine::Schemer::SchemerEditor::getScript(): can't add path joint\n");
						delete script;
						return NULL;
					}
				}
				// create link joint
				else {
					if(script.addLinkJoint(joint) == 0) {
						log.error("Unigine::Schemer::SchemerEditor::getScript(): can't add link joint\n");
						delete script;
						return NULL;
					}
				}
			}
			
			return script;
		}
		
		/**********************************************************************\
		*
		* Callbacks
		*
		\**********************************************************************/
		
		/*
		 */
		void types_drag_drop() {
			
		}
		
		/*
		 */
		void node_changed(GraphNode node) {
			update_parameters(node);
		}
		
		void node_created(GraphNode node) {
			
			if(node == NULL) {
				int id = types_tb.getCurrentItem();
				if(id == -1) return;
				string type = types_tb.getItemText(id);
				if(types_tb.getNumItemChilds(id)) return;
				id = types_tb.getItemParent(id);
				while(id != -1) {
					type = types_tb.getItemText(id) + "." + type;
					id = types_tb.getItemParent(id);
				}
				node = create_node(type);
				if(node == NULL) return;
				float scale = graph_w.getScale();
				int position_x = int(graph_w.getMouseX() / scale);
				int position_y = int(graph_w.getMouseY() / scale);
				node.setPosition(position_x,position_y);
			}
			
			graph_w.addNode(node);
		}
		
		void node_removed(GraphNode node) {
			update_parameters(NULL);
		}
		
		/*
		 */
		void joint_changed(GraphJoint joint) {
			
		}
		
		void joint_created(GraphJoint joint) {
			
		}
		
		void joint_removed(GraphJoint joint) {
			
		}
		
		/*
		 */
		void callback_redirector(SchemerEditor editor,string name) {
			editor.call(name);
		}
		
		void graph_callback_redirector(int data,SchemerEditor editor,int callback) {
			switch(callback) {
				case GRAPH_NODE_CHANGED:	return editor.node_changed(data);
				case GRAPH_NODE_CREATED:	return editor.node_created(data);
				case GRAPH_NODE_REMOVED:	return editor.node_removed(data);
				case GRAPH_JOINT_CHANGED:	return editor.joint_changed(data);
				case GRAPH_JOINT_CREATED:	return editor.joint_created(data);
				case GRAPH_JOINT_REMOVED:	return editor.joint_removed(data);
			}
			throw("Unigine::Schemer::SchemerEditor::graph_callback_redirector(): unknown callback %d\n",callback);
		}
	};
	
} /* namespace Unigine::Schemer */

#endif /* __UNIGINE_SCHEMER_EDITOR_H__ */
