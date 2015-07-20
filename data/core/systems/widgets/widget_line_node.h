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


#ifndef __UNIGINE_WIDGET_LINE_NODE_H__
#define __UNIGINE_WIDGET_LINE_NODE_H__

#include <core/systems/widgets/widget_line.h>
#include <core/systems/widgets/widget_dialog_node.h>

/*
 */
namespace Unigine::Widgets {
	
	/**************************************************************************\
	*
	* LineKeyNode
	*
	\**************************************************************************/
	
	/*
	 */
	class LineKeyNode : LineKey {
		
		int value;
		
		int node_id;
		
		/*
		 */
		LineKeyNode(float t,int v,int id = 0) : LineKey(t) {
			value = v;
			node_id = id;
		}
		
		/*
		 */
		void setValue(int v) {
			value = v;
		}
		
		int getValue() {
			return value;
		}
		
		/*
		 */
		void setNodeID(int id) {
			node_id = id;
		}
		
		int getNodeID() {
			return node_id;
		}
		
		/*
		 */
		string get_text() {
			if(value && engine.world.isNode(node_id)) {
				Node node = engine.world.getNode(node_id);
				if(strlen(node.getName())) return node.getName();
				return format("0x%08x",node_id);
			}
			return "";
		}
		
		int get_direction() {
			if(value) return LINE_KEY_UP;
			return LINE_KEY_DOWN;
		}
		
		vec4 get_color() {
			return LineCurveNode(curve).getColor();
		}
	};
	
	/**************************************************************************\
	*
	* LineCurveNode
	*
	\**************************************************************************/
	
	/*
	 */
	class LineCurveNode : LineCurve {
		
		vec4 color;
		
		/*
		 */
		LineCurveNode(vec4 c) {
			
			color = c;
		}
		
		/*
		 */
		void setColor(vec4 c) {
			color = c;
		}
		
		vec4 getColor() {
			return color;
		}
		
		/*
		 */
		int get_value(float time) {
			
			int left = indices.left(time);
			if(left != -1) return LineKeyNode(keys[left]).value;
			
			int right = indices.right(time);
			if(right != -1) return LineKeyNode(keys[right]).value;
			
			return 0;
		}
		
		/*
		 */
		LineKeyNode create_key(float time) {
			return new LineKeyNode(time,0)
		}
		
		LineKeyNode clone_key(LineKeyNode key) {
			return new LineKeyNode(key.getTime(),key.getValue(),key.getNodeID())
		}
		
		/*
		 */
		void update_key(LineKeyNode key) {
			
			// line
			LineCurve curve = key.getCurve();
			Line line = curve.getLine();
			
			// dialog result
			int node_id = key.getNodeID();
			if(dialogNode("Select node",node_id)) {
				foreach(LineKey key; line.selected_keys) {
					if(key is LineKeyNode) {
						LineKeyNode(key).setValue(1);
						LineKeyNode(key).setNodeID(node_id);
					}
				}
				line.updateSelectedKeys();
			}
		}
		
		string get_update_func() {
			return "Unigine::Widgets::LineCurveNode::update_key";
		}
		
		/*
		 */
		void create_image(int width) {
			image.create2D(width,1,IMAGE_FORMAT_RGBA8);
		}
		
		void update_image(float step,float offset) {
			forloop(int x = 0; image.getWidth()) {
				float time = step * x + offset;
				if(get_value(time)) image.set2D(x,0,color);
				else image.set2D(x,0,vec4_zero);
			}
		}
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_LINE_NODE_H__ */
