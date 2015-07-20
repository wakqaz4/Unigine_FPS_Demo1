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


#ifndef __UNIGINE_SCHEMER_EDITOR_PARAMETER_CONSTANT_H__
#define __UNIGINE_SCHEMER_EDITOR_PARAMETER_CONSTANT_H__

/*
 */
namespace Unigine::Schemer {
	
	/*
	 */
	class SchemerEditorParameterConstant : SchemerEditorParameter {
		
		using Unigine::Widgets;
		
		enum {
			TYPE_INT = 0,
			TYPE_FLOAT,
			TYPE_VEC3,
			TYPE_VEC4,
			TYPE_STRING,
		};
		
		Graph graph_w;					// graph
		GraphNode graph_node;			// graph node
		
		ComboBox type_cb;				// type combobox
		EditLine value_el[4];			// value editlines
		
		/*
		 */
		SchemerEditorParameterConstant(VBox vbox,Graph g,GraphNode n) {
			
			graph_w = g;
			graph_node = n;
			
			type_cb = new ComboBox();
			type_cb.addItem("int");
			type_cb.addItem("float");
			type_cb.addItem("vec3");
			type_cb.addItem("vec4");
			type_cb.addItem("string");
			type_cb.setCallback(CHANGED,functionid(callback_redirector),this,"type_changed");
			vbox.addChild(type_cb,ALIGN_EXPAND);
			
			forloop(int i = 0; 4) {
				value_el[i] = new EditLine();
				value_el[i].setCallback(PRESSED,functionid(callback_redirector),this,"value_pressed");
				value_el[i].setCallback(FOCUS_OUT,functionid(callback_redirector),this,"value_pressed");
				vbox.addChild(value_el[i],ALIGN_EXPAND);
			}
			
			update();
		}
		
		~SchemerEditorParameterConstant() {
			delete type_cb;
			value_el.delete();
		}
		
		/*
		 */
		void update() {
			
			// disable callbacks
			type_cb.setCallbackEnabled(CHANGED,0);
			forloop(int i = 0; 4) {
				value_el[i].setCallbackEnabled(CHANGED,0);
				if(i) value_el[i].setHidden(1);
			}
			
			// update node
			int data = graph_node.getData();
			switch(typeid(data)) {
				case typeid(int):
					type_cb.setCurrentItem(TYPE_INT);
					value_el[0].setValidator(VALIDATOR_INT);
					value_el[0].setText(string(data));
					graph_node.setText("int " + data);
					break;
				case typeid(float):
					type_cb.setCurrentItem(TYPE_FLOAT);
					value_el[0].setValidator(VALIDATOR_FLOAT);
					value_el[0].setText(string(data));
					graph_node.setText("float " + data);
					break;
				case typeid(vec3):
					type_cb.setCurrentItem(TYPE_VEC3);
					forloop(int i = 0; 3) {
						value_el[i].setHidden(0);
						value_el[i].setValidator(VALIDATOR_FLOAT);
						value_el[i].setText(string(data[i]));
					}
					graph_node.setText("vec3 " + data);
					break;
				case typeid(vec4):
					type_cb.setCurrentItem(TYPE_VEC4);
					forloop(int i = 0; 4) {
						value_el[i].setHidden(0);
						value_el[i].setValidator(VALIDATOR_FLOAT);
						value_el[i].setText(string(data[i]));
					}
					graph_node.setText("vec4 " + data);
					break;
				case typeid(string):
					type_cb.setCurrentItem(TYPE_STRING);
					value_el[0].setValidator(VALIDATOR_ANY);
					value_el[0].setText(data);
					graph_node.setText("string " + data);
					break;
			}
			
			// enable callbacks
			type_cb.setCallbackEnabled(CHANGED,1);
			forloop(int i = 0; 4) {
				value_el[i].setCallbackEnabled(CHANGED,1);
			}
			
			// update node
			graph_w.updateNode(graph_node);
		}
		
		/*
		 */
		void type_changed() {
			switch(type_cb.getCurrentItem()) {
				case TYPE_INT:		graph_node.setData(0); break;
				case TYPE_FLOAT:	graph_node.setData(0.0f); break;
				case TYPE_VEC3:		graph_node.setData(vec3_zero); break;
				case TYPE_VEC4:		graph_node.setData(vec4_zero); break;
				case TYPE_STRING:	graph_node.setData(""); break;
			}
			update();
		}
		
		void value_pressed() {
			switch(typeid(graph_node.getData())) {
				case typeid(int):		graph_node.setData(int(value_el[0].getText())); break;
				case typeid(float):		graph_node.setData(float(value_el[0].getText())); break;
				case typeid(vec3):		graph_node.setData(vec3(float(value_el[0].getText()),float(value_el[1].getText()),float(value_el[2].getText()))); break;
				case typeid(vec4):		graph_node.setData(vec4(float(value_el[0].getText()),float(value_el[1].getText()),float(value_el[2].getText()),float(value_el[3].getText()))); break;
				case typeid(string):	graph_node.setData(value_el[0].getText()); break;
			}
			update();
		}
		
		/*
		 */
		void setScript(GraphNode graph_node,SchemerNode node) {
			
			int ret[0];
			int data = 0;
			
			string value = node.getOutputLinkValue("value");
			
			if(sscanf(value,"int(%d)",ret) == 1) {
				data = ret[0];
				graph_node.setText("int " + data);
			} else if(sscanf(value,"float(%f)",ret) == 1) {
				data = ret[0];
				graph_node.setText("float " + data);
			} else if(sscanf(value,"vec3(%f,%f,%f)",ret) == 3) {
				data = vec3(ret[0],ret[1],ret[2]);
				graph_node.setText("vec3 " + data);
			} else if(sscanf(value,"vec4(%f,%f,%f,%f)",ret) == 4) {
				data = vec4(ret[0],ret[1],ret[2],ret[3]);
				graph_node.setText("vec4 " + data);
			} else if(re_search(value,"\"(.+)\"$",ret) == 1) {
				data = ret[0];
				graph_node.setText("string " + data);
			} else {
				log.error("Unigine::Schemer::SchemerEditorParameterConstant::setScript(): unknown node value \"%s\"\n",value);
			}
			
			graph_node.setData(data);
		}
		
		/*
		 */
		void getScript(SchemerNode node,GraphNode graph_node) {
			int data = graph_node.getData();
			switch(typeid(data)) {
				case typeid(int):		node.setOutputLinkValue("value",format("int(%d)",data)); break;
				case typeid(float):		node.setOutputLinkValue("value",format("float(%g)",data)); break;
				case typeid(vec3):		node.setOutputLinkValue("value",format("vec3(%g,%g,%g)",data.x,data.y,data.z)); break;
				case typeid(vec4):		node.setOutputLinkValue("value",format("vec4(%g,%g,%g,%g)",data.x,data.y,data.z,data.w)); break;
				case typeid(string):	node.setOutputLinkValue("value",format("\"%s\"",data)); break;
				default:				throw("Unigine::Schemer::SchemerEditorParameterConstant::getScript(): unknown node data %s\n",typeof(data));
			}
		}
	};
	
} /* namespace Unigine::Schemer */

#endif /* __UNIGINE_SCHEMER_EDITOR_PARAMETER_CONSTANT_H__ */
