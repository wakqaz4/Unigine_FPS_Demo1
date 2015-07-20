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


#ifndef __UNIGINE_WIDGET_GRAPH_H__
#define __UNIGINE_WIDGET_GRAPH_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	enum {
		
		GRAPH_ENTRY = 0x80000000,		// anchor entry mask
		
		GRAPH_TRIANGLE = 0,				// anchor triangle shape
		GRAPH_SQUARE,					// anchor square shape
		GRAPH_CIRCLE,					// anchor circle shape
		
		GRAPH_NODE_CHANGED = 0,			// node current callback
		GRAPH_NODE_CREATED,				// node created callback
		GRAPH_NODE_REMOVED,				// node removed callback
		GRAPH_JOINT_CHANGED,			// joint current callback
		GRAPH_JOINT_CREATED,			// joint created callback
		GRAPH_JOINT_REMOVED,			// joint removed callback
		GRAPH_NUM_CALLBACKS,
		
		GRAPH_TEXT_SIZE = 14,			// text size
		GRAPH_NODE_SIZE = 32,			// node texture size
		GRAPH_ANCHOR_STEP = 22,			// anchor node step
		GRAPH_ANCHOR_SIZE = 16,			// anchor texture size
		GRAPH_LINE_POINTS = 24,			// number of line points
		
		GRAPH_LINE_DISTANCE = 8.0f,		// line distance
		
		GRAPH_TEXTURE_WIDTH = 128,		// texture width
		GRAPH_TEXTURE_HEIGHT = 64,		// texture height
	};
	
	/*
	 */
	vec4 graph_background_color = vec4(0.15f,0.15f,0.15f,1.0f);
	string graph_texture = "core/systems/widgets/images/widget_graph.png";
	
	/*
	 */
	vec4 graph_background = vec4(0.0f,0.0f,0.25f,0.5f);
	vec4 graph_center = vec4(0.25f,0.0f,0.5f,0.5f);
	
	/*
	 */
	vec4 graph_triangles[] = ( vec4(0.500f,0.0f,0.625f,0.25f), vec4(0.500f,0.25f,0.625f,0.5f), vec4(0.500f,0.5f,0.625f,0.75f), vec4(0.500f,0.75f,0.625f,1.0f) );
	vec4 graph_squares[] =   ( vec4(0.625f,0.0f,0.750f,0.25f), vec4(0.625f,0.25f,0.750f,0.5f), vec4(0.625f,0.5f,0.750f,0.75f), vec4(0.625f,0.75f,0.750f,1.0f) );
	vec4 graph_circles[] =   ( vec4(0.750f,0.0f,0.875f,0.25f), vec4(0.750f,0.25f,0.875f,0.5f), vec4(0.750f,0.5f,0.875f,0.75f), vec4(0.750f,0.75f,0.875f,1.0f) );
	vec4 graph_arrows[] =    ( vec4(0.875f,0.0f,1.000f,0.25f), vec4(0.875f,0.25f,1.000f,0.5f), vec4(0.875f,0.5f,1.000f,0.75f), vec4(0.875f,0.75f,1.000f,1.0f) );
	
	/*
	 */
	void graph_create_quad(WidgetCanvas canvas,int polygon_id,int x0,int y0,float tx0,float ty0,int x1,int y1,float tx1,float ty1) {
		canvas.clearPolygonPoints(polygon_id);
		canvas.addPolygonPoint(polygon_id,vec3(x0,y0,0.0f));
		canvas.setPolygonTexCoord(polygon_id,vec3(tx0,ty0,0.0f));
		canvas.addPolygonPoint(polygon_id,vec3(x1,y0,0.0f));
		canvas.setPolygonTexCoord(polygon_id,vec3(tx1,ty0,0.0f));
		canvas.addPolygonPoint(polygon_id,vec3(x1,y1,0.0f));
		canvas.setPolygonTexCoord(polygon_id,vec3(tx1,ty1,0.0f));
		canvas.addPolygonPoint(polygon_id,vec3(x0,y1,0.0f));
		canvas.setPolygonTexCoord(polygon_id,vec3(tx0,ty1,0.0f));
	}
	
	/**************************************************************************\
	*
	* GraphAnchor
	*
	\**************************************************************************/
	
	/*
	 */
	class GraphAnchor {
		
		int mask;				// anchor mask
		int align;				// anchor align
		int shape;				// anchor shape
		string name;			// anchor name
		string text;			// anchor text
		vec4 color;				// anchor color
		
		int data;				// anchor data
		
		GraphNode node;			// anchor node
		
		int width;				// anchor text width
		int height;				// anchor text height
		int position_x;			// anchor position x
		int position_y;			// anchor position y
		
		int text_id;			// text identifier
		int polygons_id[2];		// polygon identifiers
		
		WidgetCanvas canvas;
		
		/*
		 */
		GraphAnchor(int m,int a,int s,string n = "",string t = "",vec4 c = vec4_one,int d = 0) {
			
			mask = m;
			align = a;
			shape = s;
			name = n;
			text = t;
			color = c;
			
			data = d;
			
			text_id = -1;
			polygons_id[0] = -1;
			polygons_id[1] = -1;
		}
		
		~GraphAnchor() {
			
			if(canvas != NULL) {
				canvas.removeText(text_id);
				canvas.removePolygon(polygons_id[0]);
				canvas.removePolygon(polygons_id[1]);
			}
		}
		
		/*
		 */
		GraphAnchor clone() {
			return new GraphAnchor(mask,align,shape,name,text,color,data);
		}
		
		/*
		 */
		int getMask() {
			return mask;
		}
		
		int getAlign() {
			return align;
		}
		
		int getShape() {
			return shape;
		}
		
		/*
		 */
		string getName() {
			return name;
		}
		
		/*
		 */
		void setText(string t) {
			text = t;
			if(canvas != NULL) {
				canvas.setTextText(text_id,text);
				width = canvas.getTextWidth(text_id);
				height = canvas.getTextHeight(text_id);
			}
		}
		
		string getText() {
			return text;
		}
		
		/*
		 */
		void setData(int d) {
			data = d;
		}
		
		int getData() {
			return data;
		}
		
		/*
		 */
		GraphNode getNode() {
			return node;
		}
		
		/*
		 */
		void setCanvas(WidgetCanvas c) {
			if(canvas != c) {
				canvas = c;
				text_id = canvas.addText();
				polygons_id[0] = canvas.addPolygon();
				polygons_id[1] = canvas.addPolygon();
				canvas.setTextText(text_id,text);
				canvas.setTextColor(text_id,color);
				canvas.setTextSize(text_id,GRAPH_TEXT_SIZE);
				width = canvas.getTextWidth(text_id);
				height = canvas.getTextHeight(text_id);
				canvas.setPolygonColor(polygons_id[0],color);
				canvas.setPolygonColor(polygons_id[1],vec4_zero);
			}
		}
		
		void setTextOrder(int order) {
			if(canvas != NULL) {
				canvas.setTextOrder(text_id,order);
				canvas.setPolygonOrder(polygons_id[1],order);
			}
		}
		
		void setPolygonOrder(int order) {
			if(canvas != NULL) {
				canvas.setPolygonOrder(polygons_id[0],order);
			}
		}
		
		/*
		 */
		int isPolygon(int id) {
			foreach(int polygon_id; polygons_id) {
				if(polygon_id == id) return 1;
			}
			return 0;
		}
		
		/*
		 */
		void setTransform(mat4 transform) {
			canvas.setTextTransform(text_id,transform);
			canvas.setPolygonTransform(polygons_id[0],transform);
			canvas.setPolygonTransform(polygons_id[1],transform);
		}
		
		void getPosition() {
			mat4 transform = canvas.getPolygonTransform(polygons_id[0]);
			return transform * vec3(position_x,position_y,0.0f);
		}
		
		/*
		 */
		void create(int x,int y,int index) {
			
			// position
			position_x = x;
			position_y = y;
			
			// text position
			switch(align) {
				case ALIGN_LEFT:	canvas.setTextPosition(text_id,vec3(x + GRAPH_NODE_SIZE / 8,y - GRAPH_TEXT_SIZE / 2,0.0f)); break;
				case ALIGN_RIGHT:	canvas.setTextPosition(text_id,vec3(x - width - GRAPH_NODE_SIZE / 8,y - GRAPH_TEXT_SIZE / 2,0.0f)); break;
				case ALIGN_BOTTOM:	canvas.setTextPosition(text_id,vec3(x - width / 2,y - GRAPH_TEXT_SIZE - GRAPH_NODE_SIZE / 4,0.0f)); break;
			}
			
			// texture coordinates
			vec4 texcoord;
			switch(shape) {
				case GRAPH_TRIANGLE:	texcoord = graph_triangles[index]; break;
				case GRAPH_SQUARE:		texcoord = graph_squares[index]; break;
				case GRAPH_CIRCLE:		texcoord = graph_circles[index]; break;
			}
			
			// anchor polygons
			graph_create_quad(canvas,polygons_id[0],x - GRAPH_ANCHOR_SIZE / 2,y - GRAPH_ANCHOR_SIZE / 2,texcoord.x,texcoord.y,x + GRAPH_ANCHOR_SIZE / 2,y + GRAPH_ANCHOR_SIZE / 2,texcoord.z,texcoord.w);
			graph_create_quad(canvas,polygons_id[1],x - GRAPH_ANCHOR_STEP / 2,y - GRAPH_ANCHOR_STEP / 2,texcoord.x,texcoord.y,x + GRAPH_ANCHOR_STEP / 2,y + GRAPH_ANCHOR_STEP / 2,texcoord.z,texcoord.w);
		}
	};
	
	/**************************************************************************\
	*
	* GraphNode
	*
	\**************************************************************************/
	
	/*
	 */
	class GraphNode {
		
		string name;			// node name
		string text;			// node text
		vec4 color;				// node color
		
		int data;				// node data
		
		GraphAnchor anchors[0];	// node anchors
		
		int width;				// node width
		int height;				// node height
		int position_x;			// node position x
		int position_y;			// node position y
		
		int name_id;			// name identifier
		int text_id;			// text identifier
		int polygons_id[18];	// polygon identifiers
		
		WidgetCanvas canvas;
		
		/*
		 */
		GraphNode(string n,string t = "",vec4 c = vec4_one,int d = 0) {
			
			name = n;
			text = t;
			color = c;
			
			data = d;
			
			name_id = -1;
			text_id = -1;
			foreach(int polygon_id; polygons_id) {
				polygon_id = -1;
			}
		}
		
		~GraphNode() {
			
			anchors.delete();
			
			if(canvas != NULL) {
				canvas.removeText(name_id);
				canvas.removeText(text_id);
				foreach(int polygon_id; polygons_id) {
					canvas.removePolygon(polygon_id);
				}
			}
		}
		
		/*
		 */
		GraphNode clone() {
			GraphNode node = new GraphNode(name,text,color,data);
			node.setPosition(position_x,position_y);
			foreach(GraphAnchor anchor; anchors) {
				node.addAnchor(anchor.clone());
			}
			return node;
		}
		
		/*
		 */
		string getName() {
			return name;
		}
		
		/*
		 */
		void setText(string t) {
			text = t;
		}
		
		string getText() {
			return text;
		}
		
		/*
		 */
		void setData(int d) {
			data = d;
		}
		
		int getData() {
			return data;
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
		void addAnchor(GraphAnchor anchor) {
			forloop(int i = 0; anchors.size()) {
				if(anchors[i].name == anchor.name) throw("Unigine::Widgets::GraphNode::addAnchor(): \"%s\" anchor is already defined\n",name);
			}
			anchor.node = this;
			anchors.append(anchor);
		}
		
		GraphAnchor getAnchor(string name) {
			forloop(int i = 0; anchors.size()) {
				if(anchors[i].name == name) return anchors[i];
			}
			throw("Unigine::Widgets::GraphNode::getAnchor(): can't find \"%s\" anchor\n",name);
		}
		
		/*
		 */
		void setCanvas(WidgetCanvas c) {
			if(canvas != c) {
				canvas = c;
				name_id = canvas.addText();
				text_id = canvas.addText();
				canvas.setTextColor(name_id,color);
				canvas.setTextColor(text_id,color);
				canvas.setTextSize(name_id,GRAPH_TEXT_SIZE);
				canvas.setTextSize(text_id,GRAPH_TEXT_SIZE);
				foreach(GraphAnchor anchor; anchors) {
					anchor.setCanvas(canvas);
				}
				foreach(int polygon_id; polygons_id) {
					polygon_id = canvas.addPolygon();
					canvas.setPolygonColor(polygon_id,color);
				}
			}
		}
		
		void setOrder(int &order) {
			if(canvas != NULL) {
				foreach(GraphAnchor anchor; anchors) {
					anchor.setPolygonOrder(order);
				}
				order++;
				forloop(int i = 0; 9) {
					canvas.setPolygonOrder(polygons_id[i],order);
				}
				order++;
				forloop(int i = 9; 18) {
					canvas.setPolygonOrder(polygons_id[i],order);
				}
				order++;
				foreach(GraphAnchor anchor; anchors) {
					anchor.setTextOrder(order);
				}
				canvas.setTextOrder(name_id,order);
				canvas.setTextOrder(text_id,order);
				order++;
			}
		}
		
		/*
		 */
		int isPolygon(int id) {
			foreach(int polygon_id; polygons_id) {
				if(polygon_id == id) return 1;
			}
			return 0;
		}
		
		/*
		 */
		void setPosition(int x,int y) {
			position_x = int(x);
			position_y = int(y);
			if(canvas != NULL) {
				mat4 transform = translate(x,y,0.0f);
				canvas.setTextTransform(name_id,transform);
				canvas.setTextTransform(text_id,transform);
				foreach(int polygon_id; polygons_id) {
					canvas.setPolygonTransform(polygon_id,transform);
				}
				foreach(GraphAnchor anchor; anchors) {
					anchor.setTransform(transform);
				}
			}
		}
		
		int getPositionX() {
			return position_x;
		}
		
		int getPositionY() {
			return position_y;
		}
		
		/*
		 */
		void create_window(int num,int x0,int y0,float tx0,float ty0,int x1,int y1,float tx1,float ty1) {
			int dx = GRAPH_NODE_SIZE / 2;
			int dy = GRAPH_NODE_SIZE / 2;
			float tx01 = (tx0 + tx1) * 0.5f;
			float ty01 = (ty0 + ty1) * 0.5f;
			float dtx = 0.5f / GRAPH_TEXTURE_WIDTH;
			float dty = 0.5f / GRAPH_TEXTURE_HEIGHT;
			graph_create_quad(canvas,polygons_id[num + 0],x0,y0,tx0,ty0,x0 + dx,y0 + dy,tx01 - dtx,ty01 - dty);
			graph_create_quad(canvas,polygons_id[num + 1],x1 - dx,y0,tx01 + dtx,ty0,x1,y0 + dy,tx1,ty01 - dty);
			graph_create_quad(canvas,polygons_id[num + 2],x0,y1 - dy,tx0,ty01 + dty,x0 + dx,y1,tx01 - dtx,ty1);
			graph_create_quad(canvas,polygons_id[num + 3],x1 - dx,y1 - dy,tx01 + dtx,ty01 + dty,x1,y1,tx1,ty1);
			graph_create_quad(canvas,polygons_id[num + 4],x0 + dx,y0,tx01 - dtx,ty0,x1 - dx,y0 + dy,tx01 + dtx,ty01 - dty);
			graph_create_quad(canvas,polygons_id[num + 5],x0,y0 + dy,tx0,ty01 - dtx,x0 + dx,y1 - dy,tx01 - dtx,ty01 + dty);
			graph_create_quad(canvas,polygons_id[num + 6],x1 - dx,y0 + dy,tx01 + dtx,ty01 - dtx,x1,y1 - dy,tx1,ty01 + dty);
			graph_create_quad(canvas,polygons_id[num + 7],x0 + dx,y1 - dy,tx01 - dtx,ty01 + dty,x1 - dx,y1,tx01 + dtx,ty1);
			graph_create_quad(canvas,polygons_id[num + 8],x0 + dx,y0 + dy,tx01 - dtx,ty01 - dty,x1 - dx,y1 - dy,tx01 + dtx,ty01 + dty);
		}
		
		void clear_window(int num) {
			forloop(int i = num; num + 9) {
				int polygon_id = polygons_id[i];
				canvas.clearPolygonPoints(polygon_id);
			}
		}
		
		/*
		 */
		void create() {
			
			// size
			width = 0;
			height = 0;
			
			// texts
			canvas.setTextText(name_id,name);
			canvas.setTextText(text_id,text);
			
			// text size
			if(strlen(text)) {
				width = canvas.getTextWidth(text_id);
				height = canvas.getTextHeight(text_id);
			}
			
			// anchors size
			int num_left = 0;
			int num_right = 0;
			int num_bottom = 0;
			int left_width = 0;
			int right_width = 0;
			int bottom_width = 0;
			foreach(GraphAnchor anchor; anchors) {
				switch(anchor.align) {
					case ALIGN_LEFT:	num_left++; left_width = max(left_width,anchor.width); break;
					case ALIGN_RIGHT:	num_right++; right_width = max(right_width,anchor.width); break;
					case ALIGN_BOTTOM:	num_bottom++; bottom_width = max(bottom_width,anchor.width); break;
				}
			}
			
			// anchors size
			width += left_width + right_width;
			height = max(height,GRAPH_ANCHOR_STEP * (max(num_left,num_right)));
			width = max(width,max(GRAPH_ANCHOR_STEP,bottom_width) * (num_bottom + 1));
			if(num_bottom) height += GRAPH_ANCHOR_STEP / 2;
			
			// name size
			if(strlen(name)) {
				width = max(width,canvas.getTextWidth(name_id));
				height += GRAPH_ANCHOR_STEP / 2;
			}
			
			// texture size
			width += GRAPH_NODE_SIZE;
			height += GRAPH_NODE_SIZE;
			
			// create anchors
			int left_index = 0;
			int right_index = 0;
			int bottom_index = 0;
			int anchors_offset = 0;
			int anchors_height = height;
			if(strlen(name)) anchors_offset += GRAPH_ANCHOR_STEP / 2;
			if(strlen(name)) anchors_height -= GRAPH_ANCHOR_STEP / 2;
			if(num_bottom) anchors_height -= GRAPH_ANCHOR_STEP / 2;
			foreach(GraphAnchor anchor; anchors) {
				switch(anchor.align) {
					case ALIGN_LEFT:	anchor.create(0,anchors_offset + anchors_height * ++left_index / (num_left + 1),0); break;
					case ALIGN_RIGHT:	anchor.create(width,anchors_offset + anchors_height * ++right_index / (num_right + 1),1); break;
					case ALIGN_BOTTOM:	anchor.create(width * ++bottom_index / (num_bottom + 1),height,3); break;
				}
			}
			
			// text positions
			canvas.setTextPosition(name_id,vec3((width - canvas.getTextWidth(name_id)) / 2,GRAPH_NODE_SIZE / 8,0.0f));
			canvas.setTextPosition(text_id,vec3(left_width + GRAPH_NODE_SIZE / 2,GRAPH_NODE_SIZE / 2 + GRAPH_TEXT_SIZE,0.0f));
			
			// window polygons
			create_window(0,0,0,graph_background.x,graph_background.y,width,height,graph_background.z,graph_background.w);
			if(strlen(text)) {
				if(num_bottom) create_window(9,left_width,GRAPH_TEXT_SIZE,graph_center.x,graph_center.y,width - right_width,height - GRAPH_TEXT_SIZE,graph_center.z,graph_center.w);
				else create_window(9,left_width,GRAPH_TEXT_SIZE,graph_center.x,graph_center.y,width - right_width,height,graph_center.z,graph_center.w);
			} else {
				clear_window(9);
			}
			
			// set position
			setPosition(position_x,position_y);
		}
	};
	
	/**************************************************************************\
	*
	* GraphJoint
	*
	\**************************************************************************/
	
	/*
	 */
	class GraphJoint {
		
		int data;				// joint data
		
		GraphAnchor anchor_0;	// from anchor
		GraphAnchor anchor_1;	// to anchor
		
		int line_id;			// line identifier
		int polygon_id;			// polygon identifier
		
		WidgetCanvas canvas;
		
		/*
		 */
		GraphJoint(GraphAnchor a0,GraphAnchor a1) {
			
			anchor_0 = a0;
			anchor_1 = a1;
			
			line_id = -1;
			polygon_id = -1;
		}
		
		~GraphJoint() {
			
			if(canvas != NULL) {
				canvas.removeLine(line_id);
				canvas.removePolygon(polygon_id);
			}
		}
		
		/*
		 */
		void setData(int d) {
			data = d;
		}
		
		int getData() {
			return data;
		}
		
		/*
		 */
		GraphAnchor getAnchor0() {
			return anchor_0;
		}
		
		GraphAnchor getAnchor1() {
			return anchor_1;
		}
		
		/*
		 */
		GraphNode getNode0() {
			return anchor_0.node;
		}
		
		GraphNode getNode1() {
			return anchor_1.node;
		}
		
		/*
		 */
		void setCanvas(WidgetCanvas c) {
			if(canvas != c) {
				canvas = c;
				line_id = canvas.addLine();
				polygon_id = canvas.addPolygon();
				canvas.setLineColor(line_id,anchor_0.color);
				canvas.setPolygonColor(polygon_id,anchor_0.color);
			}
		}
		
		void setOrder(int &order) {
			if(canvas != NULL) {
				canvas.setLineOrder(line_id,order++);
				canvas.setPolygonOrder(polygon_id,order++);
			}
		}
		
		/*
		 */
		int isLine(int id) {
			return (line_id == id);
		}
		
		int isPolygon(int id) {
			return (polygon_id == id);
		}
		
		/*
		 */
		void create() {
			
			// line points
			vec3 p0,p1,p2,p3,p4;
			
			// target points
			p0 = anchor_0.getPosition();
			p4 = anchor_1.getPosition();
			
			// offset distance
			float offset = length(p4 - p0) / 2.0f;
			
			// first control point
			switch(anchor_0.align) {
				case ALIGN_LEFT:	p1 = p0 - vec3(offset,0.0f,0.0f); break;
				case ALIGN_RIGHT:	p1 = p0 + vec3(offset,0.0f,0.0f); break;
				case ALIGN_BOTTOM:	p1 = p0 + vec3(0.0f,offset,0.0f); break;
			}
			
			// second control point
			switch(anchor_1.align) {
				case ALIGN_LEFT:	p3 = p4 - vec3(GRAPH_ANCHOR_SIZE,0.0f,0.0f); p2 = p3 - vec3(offset,0.0f,0.0f); break;
				case ALIGN_RIGHT:	p3 = p4 + vec3(GRAPH_ANCHOR_SIZE,0.0f,0.0f); p2 = p3 + vec3(offset,0.0f,0.0f); break;
				case ALIGN_BOTTOM:	p3 = p4 + vec3(0.0f,GRAPH_ANCHOR_SIZE,0.0f); p2 = p3 + vec3(0.0f,offset,0.0f); break;
			}
			
			// cubic bezier points
			canvas.clearLinePoints(line_id);
			forloop(int i = 0; GRAPH_LINE_POINTS) {
				float k0 = float(i) / GRAPH_LINE_POINTS;
				float k1 = 1.0f - k0;
				float k02 = k0 * k0;
				float k12 = k1 * k1;
				vec3 p = p0 * (k12 * k1) + p1 * (3.0f * k12 * k0) + p2 * (3.0f * k02 * k1) + p3 * (k02 * k0);
				canvas.addLinePoint(line_id,p);
			}
			canvas.addLinePoint(line_id,p4);
			
			// arrow polygon
			vec4 texcoord;
			switch(anchor_1.align) {
				case ALIGN_LEFT:	texcoord = graph_arrows[1]; graph_create_quad(canvas,polygon_id,p3.x,p3.y - GRAPH_ANCHOR_SIZE / 2,texcoord.x,texcoord.y,p3.x + GRAPH_ANCHOR_SIZE,p3.y + GRAPH_ANCHOR_SIZE / 2,texcoord.z,texcoord.w); break;
				case ALIGN_RIGHT:	texcoord = graph_arrows[0]; graph_create_quad(canvas,polygon_id,p3.x - GRAPH_ANCHOR_SIZE,p3.y - GRAPH_ANCHOR_SIZE / 2,texcoord.x,texcoord.y,p3.x,p3.y + GRAPH_ANCHOR_SIZE / 2,texcoord.z,texcoord.w); break;
				case ALIGN_BOTTOM:	texcoord = graph_arrows[2]; graph_create_quad(canvas,polygon_id,p3.x - GRAPH_ANCHOR_SIZE / 2,p3.y - GRAPH_ANCHOR_SIZE,texcoord.x,texcoord.y,p3.x + GRAPH_ANCHOR_SIZE / 2,p3.y,texcoord.z,texcoord.w); break;
			}
		}
	};
	
	/**************************************************************************\
	*
	* Graph
	*
	\**************************************************************************/
	
	/*
	 */
	class Graph : Widget {
		
		int width;						// graph width
		int height;						// graph height
		float scale;					// graph scale
		
		int order;						// current order
		int line_id;					// line identifier
		
		GraphNode nodes[0];				// nodes
		GraphJoint joints[0];			// joints
		
		int pressed;					// pressed flag
		
		GraphNode current_node;			// current node
		GraphAnchor current_anchor;		// current anchor
		GraphJoint current_joint;		// current joint
		
		string callbacks[GRAPH_NUM_CALLBACKS];
		int callbacks_data_0[GRAPH_NUM_CALLBACKS];
		int callbacks_data_1[GRAPH_NUM_CALLBACKS];
		int callbacks_enabled[GRAPH_NUM_CALLBACKS];
		
		WidgetCanvas canvas;
		
		/*
		 */
		Graph(int w,int h) {
			
			width = w;
			height = h;
			scale = 1.0f;
			
			canvas = new WidgetCanvas(engine.getGui());
			
			// canvas parameters
			canvas.setWidth(width);
			canvas.setHeight(height);
			canvas.setColor(graph_background_color);
			canvas.setTexture(graph_texture);
			
			// set callbacks
			canvas.setCallback(PRESSED,functionid(callback_redirector),this,PRESSED);
			canvas.setCallback(RELEASED,functionid(callback_redirector),this,RELEASED);
			canvas.setCallback(DOUBLE_CLICKED,functionid(callback_redirector),this,DOUBLE_CLICKED);
			canvas.setCallback(DRAG_DROP,functionid(callback_redirector),this,DRAG_DROP);
			
			// create line
			line_id = canvas.addLine();
			
			widget = canvas;
		}
		
		~Graph() {
			
			clear();
		}
		
		/*
		 */
		void setScale(float s) {
			s = clamp(s,1.0f / 4.0f,1.0f);
			if(scale != s) {
				scale = s;
				canvas.setWidth(width * scale);
				canvas.setHeight(height * scale);
				canvas.setTransform(::scale(scale,scale,1.0f));
			}
		}
		
		float getScale() {
			return scale;
		}
		
		/*
		 */
		void clear() {
			
			setScale(1.0f);
			
			nodes.delete();
			joints.delete();
		}
		
		/**********************************************************************\
		*
		* Nodes
		*
		\**********************************************************************/
		
		/*
		 */
		void addNode(GraphNode node) {
			if(nodes.find(node) != -1) throw("Unigine::Widgets::Graph::addNode(): node is already defined\n");
			node.setCanvas(canvas);
			node.setOrder(order);
			nodes.append(node);
			node.create();
			current_node = node;
			runCallback(GRAPH_NODE_CHANGED,node);
		}
		
		void removeNode(GraphNode node) {
			int index = nodes.find(node);
			if(index == -1) throw("Unigine::Widgets::Graph::removeNode(): can't find node\n");
			for(int i = joints.size() - 1; i >= 0; i--) {
				GraphJoint joint = joints[i];
				Node node_0 = joint.getNode0();
				Node node_1 = joint.getNode1();
				if(node_0 == node || node_1 == node) {
					joints.remove(i);
					runCallback(GRAPH_JOINT_REMOVED,joint);
					delete joint;
				}
			}
			nodes.remove(index);
			runCallback(GRAPH_NODE_REMOVED,node);
			delete node;
		}
		
		void updateNode(GraphNode node) {
			node.create();
			foreach(GraphJoint joint; joints) {
				Node node_0 = joint.getNode0();
				Node node_1 = joint.getNode1();
				if(node_0 == node || node_1 == node) {
					joint.create();
				}
			}
		}
		
		/*
		 */
		int getNumNodes() {
			return nodes.size();
		}
		
		GraphNode getNode(int num) {
			return nodes[num];
		}
		
		int findNode(GraphNode node) {
			return nodes.find(node);
		}
		
		/*
		 */
		void setCurrentNode(GraphNode node) {
			if(current_node != node) {
				if(node != NULL) {
					node.setOrder(order);
					foreach(GraphJoint joint; joints) {
						Node node_0 = joint.getNode0();
						Node node_1 = joint.getNode1();
						if(node_0 == node || node_1 == node) {
							joint.setOrder(order);
						}
					}
				}
				current_node = node;
				runCallback(GRAPH_NODE_CHANGED,node);
			}
		}
		
		GraphNode getCurrentNode() {
			return current_node;
		}
		
		/**********************************************************************\
		*
		* Joints
		*
		\**********************************************************************/
		
		/*
		 */
		void addJoint(GraphJoint joint) {
			if(joints.find(joint) != -1) throw("Unigine::Widgets::Graph::addJoint(): joint is already defined\n");
			joint.setCanvas(canvas);
			joint.setOrder(order);
			joints.append(joint);
			joint.create();
		}
		
		void removeJoint(GraphJoint joint) {
			int index = joints.find(joint);
			if(index == -1) throw("Unigine::Widgets::Graph::removeJoint(): can't find node\n");
			joints.remove(index);
			runCallback(GRAPH_JOINT_REMOVED,joint);
			delete joint;
		}
		
		/*
		 */
		int getNumJoints() {
			return joints.size();
		}
		
		GraphJoint getJoint(int num) {
			return joints[num];
		}
		
		/*
		 */
		void setCurrentJoint(GraphJoint joint) {
			if(current_joint != joint) {
				current_joint = joint;
				joint.setOrder(order);
				runCallback(GRAPH_JOINT_CHANGED,joint);
			}
		}
		
		GraphJoint getCurrentJoint() {
			return current_joint;
		}
		
		/**********************************************************************\
		*
		* Callbacks
		*
		\**********************************************************************/
		
		/*
		 */
		void setCallback(int callback,string function,int data_0 = 0,int data_1 = 0) {
			callbacks[callback] = function;
			callbacks_data_0[callback] = data_0;
			callbacks_data_1[callback] = data_1;
			callbacks_enabled[callback] = 1;
		}
		
		void runCallback(int callback,int data) {
			string function = callbacks[callback];
			if(function != NULL && callbacks_enabled[callback]) {
				callbacks_enabled[callback] = 0;
				if(is_function(function,1)) call(function,data);
				else if(is_function(function,2)) call(function,data,callbacks_data_0[callback]);
				else if(is_function(function,3)) call(function,data,callbacks_data_0[callback],callbacks_data_1[callback]);
				else log.error("Unigine::Widgets::Graph::runCallback(): can't find \"%s\" callback function\n",function);
				callbacks_enabled[callback] = 1;
			}
		}
		
		void setCallbackEnabled(int callback,int enable) {
			callbacks_enabled[callback] = enable;
		}
		
		/*
		 */
		GraphNode get_intersection_node(int polygon_id) {
			foreach(GraphNode node; nodes) {
				if(node.isPolygon(polygon_id)) return node;
			}
			return NULL;
		}
		
		GraphAnchor get_intersection_anchor(int polygon_id) {
			foreach(GraphNode node; nodes) {
				foreach(GraphAnchor anchor; node.anchors) {
					if(anchor.isPolygon(polygon_id)) return anchor;
				}
			}
			foreach(GraphJoint joint; joints) {
				if(joint.isPolygon(polygon_id)) return joint.anchor_1;
			}
			return NULL;
		}
		
		GraphJoint get_intersection_joint(int line_id,int polygon_id) {
			foreach(GraphJoint joint; joints) {
				if(joint.isLine(line_id)) return joint;
				if(joint.isPolygon(polygon_id)) return joint;
			}
			return NULL;
		}
		
		/*
		 */
		void pressed_callback() {
			
			/////////////////////////////////
			// left mouse button
			/////////////////////////////////
			
			if(getMouseButton() & APP_BUTTON_LEFT) {
				
				int mouse_x = canvas.getMouseX();
				int mouse_y = canvas.getMouseY();
				
				if(pressed == 0) {
					
					pressed = 1;
					
					// clear joint
					current_joint = NULL;
					
					// polygon intersection
					int polygon_id = canvas.getPolygonIntersection(mouse_x,mouse_y);
					if(polygon_id != -1) {
						
						// intersection node
						GraphNode node = get_intersection_node(polygon_id);
						if(node != NULL) {
							if(getKeyState(APP_KEY_SHIFT)) {
								runCallback(GRAPH_NODE_CREATED,node.clone());
								return;
							} else {
								setCurrentNode(node);
								return;
							}
						}
						setCurrentNode(NULL);
						
						// intersection anchor
						GraphAnchor anchor = get_intersection_anchor(polygon_id);
						if(anchor != NULL) {
							current_anchor = anchor;
							return;
						}
						current_anchor = NULL;
						
						// intersection joint
						GraphJoint joint = get_intersection_joint(-1,polygon_id);
						if(joint != NULL) {
							setCurrentJoint(joint);
							return;
						}
					}
					
					// clear node
					current_anchor = NULL;
					setCurrentNode(NULL);
					
					// line intersection
					int line_id = canvas.getLineIntersection(mouse_x,mouse_y,GRAPH_LINE_DISTANCE);
					if(line_id != -1) {
						
						// intersection joint
						GraphJoint joint = get_intersection_joint(line_id,-1);
						if(joint != NULL) {
							setCurrentJoint(joint);
							return;
						}
					}
				}
				else if(pressed == 1) {
					
					int mouse_dx = getMouseDX();
					int mouse_dy = getMouseDY();
					
					// move current node
					if(current_node != NULL) {
						int x = clamp(current_node.position_x + mouse_dx / scale,0,width - current_node.width);
						int y = clamp(current_node.position_y + mouse_dy / scale,0,height - current_node.height);
						current_node.setPosition(x,y);
						setMouseCursor(CURSOR_MOVE);
						updateNode(current_node);
					}
					
					// create joint line
					else if(current_anchor != NULL) {
						
						// find joint
						GraphJoint joint = NULL;
						foreach(GraphJoint j; joints) {
							if(j.anchor_0 != current_anchor && j.anchor_1 != current_anchor) continue;
							joint = j;
							break;
						}
						
						if((current_anchor.mask & GRAPH_ENTRY) == 0 || joint != NULL) {
							canvas.clearLinePoints(line_id);
							canvas.setLineOrder(line_id,order);
							canvas.setLineColor(line_id,current_anchor.color);
							canvas.addLinePoint(line_id,current_anchor.getPosition());
							canvas.addLinePoint(line_id,vec3(mouse_x,mouse_y,0.0f) / scale);
						}
					}
				}
			}
			
			/////////////////////////////////
			// middle mouse button
			/////////////////////////////////
			
			if(getMouseButton() & APP_BUTTON_MIDDLE) {
				
				if(pressed == 0) {
					
					pressed = 2;
				}
				else if(pressed == 2) {
					
					// navigation
					if(getParent() != NULL && getParent().getType() == SCROLLBOX) {
						
						int mouse_dx = getMouseDX();
						int mouse_dy = getMouseDY();
						
						ScrollBox scrollbox = getParent();
						scrollbox.setHScrollValue(scrollbox.getHScrollValue() - mouse_dx * scrollbox.getScrollScale());
						scrollbox.setVScrollValue(scrollbox.getVScrollValue() - mouse_dy * scrollbox.getScrollScale());
						setMouseCursor(CURSOR_MOVE);
					}
				}
			}
			
			/////////////////////////////////
			// right mouse button
			/////////////////////////////////
			
			if(getMouseButton() & APP_BUTTON_RIGHT) {
				
				float old_scale;
				int old_mouse_x;
				int old_mouse_y;
				int old_offset_x;
				int old_offset_y;
				
				if(pressed == 0) {
					
					pressed = 3;
					
					// save values
					old_scale = scale;
					old_mouse_x = canvas.getMouseX();
					old_mouse_y = canvas.getMouseY();
					
					// save scrollbox values
					Widget parent = getParent();
					if(parent != NULL && parent.getType() == SCROLLBOX) {
						ScrollBox scrollbox = parent;
						old_offset_x = scrollbox.getMouseX();
						old_offset_y = scrollbox.getMouseY();
					}
				}
				else if(pressed == 3) {
					
					int mouse_dy = getMouseDY();
					
					// scale graph
					setScale(scale * (1.0f + mouse_dy * 0.01f));
					
					// update scrollbox
					Widget parent = getParent();
					if(parent != NULL && parent.getType() == SCROLLBOX) {
						ScrollBox scrollbox = parent;
						scrollbox.setHScrollValue((old_mouse_x * scale / old_scale - old_offset_x) * scrollbox.getScrollScale());
						scrollbox.setVScrollValue((old_mouse_y * scale / old_scale - old_offset_y) * scrollbox.getScrollScale());
					}
				}
			}
		}
		
		/*
		 */
		void released_callback() {
			
			if(pressed == 1) {
				
				// clear line
				canvas.clearLinePoints(line_id);
				
				// create joint
				if(current_anchor != NULL) {
					
					int mouse_x = canvas.getMouseX();
					int mouse_y = canvas.getMouseY();
					
					// polygon intersection
					int polygon_id = canvas.getPolygonIntersection(mouse_x,mouse_y);
					if(polygon_id != -1) {
						
						// intersection anchor
						GraphAnchor anchor = get_intersection_anchor(polygon_id);
						if(anchor != NULL && (anchor.mask & current_anchor.mask)) {
							
							// find joint
							GraphJoint joint = NULL;
							foreach(GraphJoint j; joints) {
								if(j.anchor_0 != current_anchor && j.anchor_1 != current_anchor) continue;
								if(j.anchor_0 != anchor && j.anchor_1 != anchor) continue;
								joint = j;
								break;
							}
							
							// create joint
							if(joint == NULL && (current_anchor.mask & GRAPH_ENTRY) == 0 && (anchor.mask & GRAPH_ENTRY)) {
								joint = new GraphJoint(current_anchor,anchor);
								addJoint(joint);
								runCallback(GRAPH_JOINT_CREATED,joint);
							}
							
							// remove joint
							if(joint != NULL && (current_anchor.mask & GRAPH_ENTRY) && (anchor.mask & GRAPH_ENTRY) == 0) {
								removeJoint(joint);
							}
						}
					}
				}
			}
			
			pressed = 0;
		}
		
		/*
		 */
		void double_clicked_callback() {
			
			// remove node
			if(current_node != NULL) {
				removeNode(current_node);
			}
			
			// remove joint
			if(current_joint != NULL) {
				removeJoint(current_joint);
			}
		}
		
		/*
		 */
		void drag_drop_callback() {
			runCallback(GRAPH_NODE_CREATED,NULL);
		}
		
		/*
		 */
		void callback_redirector(Graph graph,int callback) {
			switch(callback) {
				case PRESSED: graph.pressed_callback(); return;
				case RELEASED: graph.released_callback(); return;
				case DOUBLE_CLICKED: graph.double_clicked_callback(); return;
				case DRAG_DROP: graph.drag_drop_callback(); return;
			}
			throw("Unigine::Widgets::Graph::callback_redirector(): unknown callback %d\n",callback);
		}
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_GRAPH_H__ */
