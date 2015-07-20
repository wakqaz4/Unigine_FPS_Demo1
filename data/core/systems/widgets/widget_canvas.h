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


#ifndef __UNIGINE_WIDGET_CANVAS_H__
#define __UNIGINE_WIDGET_CANVAS_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class Canvas : Widget {
		
		WidgetCanvas canvas;
		
		// constructor
		Canvas() {
			canvas = new WidgetCanvas(engine.getGui());
			widget = canvas;
		}
		
		Canvas(int c) {
			owner = 0;
			canvas = c;
			widget = c;
		}
		
		// texture
		void setImage(Image image) { canvas.setImage(image); }
		Image getImage() { return canvas.getImage(); }
		void setTexture(string name) { canvas.setTexture(name); }
		string getTexture() { return canvas.getTexture(); }
		
		// background color
		void setColor(vec4 color) { canvas.setColor(color); }
		vec4 getColor() { return canvas.getColor(); }
		
		// transformation
		void setTransform(mat4 transform) { canvas.setTransform(transform); }
		mat4 getTransform() { return canvas.getTransform(); }
		
		// clear
		void clear() { canvas.clear(); }
		
		// texts
		int addText(int order = 0) { return canvas.addText(order); }
		void removeText(int text) { canvas.removeText(text); }
		int getNumTexts() { return canvas.getNumTexts(); }
		int getText(int num) { return canvas.getText(num); }
		int getTextIntersection(int x,int y) { return canvas.getTextIntersection(x,y); }
		
		// text parameters
		void setTextOrder(int text,int order) { canvas.setTextOrder(text,order); }
		int getTextOrder(int text) { return canvas.getTextOrder(text); }
		void setTextSize(int text,int size) { canvas.setTextSize(text,size); }
		int getTextSize(int text) { return canvas.getTextSize(text); }
		void setTextColor(int text,vec4 color) { canvas.setTextColor(text,color); }
		vec4 getTextColor(int text) { return canvas.getTextColor(text); }
		void setTextOutline(int text,int outline) { canvas.setTextOutline(text,outline); }
		int getTextOutline(int text) { return canvas.getTextOutline(text); }
		void setTextPosition(int text,vec3 position) { canvas.setTextPosition(text,position); }
		vec3 getTextPosition(int text) { return canvas.getTextPosition(text); }
		void setTextTransform(int text,mat4 transform) { canvas.setTextTransform(text,transform); }
		mat4 getTextTransform(int text) { return canvas.getTextTransform(text); }
		void setTextText(int text,string str) { canvas.setTextText(text,str); }
		string getTextText(int text) { return canvas.getTextText(text); }
		int getTextWidth(int text) { return canvas.getTextWidth(text); }
		int getTextHeight(int text) { return canvas.getTextHeight(text); }
		
		// lines
		int addLine(int order = 0) { return canvas.addLine(order); }
		void removeLine(int line) { canvas.removeLine(line); }
		int getNumLines() { return canvas.getNumLines(); }
		int getLine(int num) { return canvas.getLine(num); }
		int getLineIntersection(int x,int y,float distance) { return canvas.getLineIntersection(x,y,distance); }
		
		// line parameters
		void setLineOrder(int line,int order) { canvas.setLineOrder(line,order); }
		int getLineOrder(int line) { return canvas.getLineOrder(line); }
		void setLineColor(int line,vec4 color) { canvas.setLineColor(line,color); }
		vec4 getLineColor(int line) { return canvas.getLineColor(line); }
		void setLineTransform(int line,mat4 transform) { canvas.setLineTransform(line,transform); }
		mat4 getLineTransform(int line) { return canvas.getLineTransform(line); }
		
		// line points
		void clearLinePoints(int line) { return canvas.clearLinePoints(line); }
		int getNumLinePoints(int line) { return canvas.getNumLinePoints(line); }
		void removeLinePoint(int line,int num) { canvas.removeLinePoint(line,num); }
		int addLinePoint(int line,vec3 point) { return canvas.addLinePoint(line,point); }
		vec3 getLinePoint(int line,int num) { return canvas.getLinePoint(line,num); }
		
		// line indices
		void clearLineIndices(int line) { return canvas.clearLineIndices(line); }
		int getNumLineIndices(int line) { return canvas.getNumLineIndices(line); }
		void removeLineIndex(int line,int num) { canvas.removeLineIndex(line,num); }
		int addLineIndex(int line,int index) { return canvas.addLineIndex(line,index); }
		int getLineIndex(int line,int num) { return canvas.getLineIndex(line,num); }
		
		// polygons
		int addPolygon(int order = 0) { return canvas.addPolygon(order); }
		void removePolygon(int polygon) { canvas.removePolygon(polygon); }
		int getNumPolygons() { return canvas.getNumPolygons(); }
		int getPolygon(int num) { return canvas.getPolygon(num); }
		int getPolygonIntersection(int x,int y) { return canvas.getPolygonIntersection(x,y); }
		
		// polygon parameters
		void setPolygonOrder(int polygon,int order) { canvas.setPolygonOrder(polygon,order); }
		int getPolygonOrder(int polygon) { return canvas.getPolygonOrder(polygon); }
		void setPolygonTwoSided(int polygon,int two_sided) { canvas.setPolygonTwoSided(polygon,two_sided); }
		int getPolygonTwoSided(int polygon) { return canvas.getPolygonTwoSided(polygon); }
		void setPolygonColor(int polygon,vec4 color) { canvas.setPolygonColor(polygon,color); }
		vec4 getPolygonColor(int polygon) { return canvas.getPolygonColor(polygon); }
		void setPolygonWrapRepeat(int polygon,int repeat) { canvas.setPolygonWrapRepeat(polygon,repeat); }
		int getPolygonWrapRepeat(int polygon) { return canvas.getPolygonWrapRepeat(polygon); }
		void setPolygonBlendFunc(int polygon,int src,int dest) { canvas.setPolygonBlendFunc(polygon,src,dest); }
		int getPolygonBlendSrcFunc(int polygon) { return canvas.getPolygonBlendSrcFunc(polygon); }
		int getPolygonBlendDestFunc(int polygon) { return canvas.getPolygonBlendDestFunc(polygon); }
		void setPolygonTransform(int polygon,mat4 transform) { canvas.setPolygonTransform(polygon,transform); }
		mat4 getPolygonTransform(int polygon) { return canvas.getPolygonTransform(polygon); }
		void setPolygonImage(int polygon,Image image) { canvas.setPolygonImage(polygon,image); }
		Image getPolygonImage(int polygon) { return canvas.getPolygonImage(polygon); }
		void setPolygonTexture(int polygon,string name) { canvas.setPolygonTexture(polygon,name); }
		string getPolygonTexture(int polygon) { return canvas.getPolygonTexture(polygon); }
		
		// polygon points
		void clearPolygonPoints(int polygon) { return canvas.clearPolygonPoints(polygon); }
		int getNumPolygonPoints(int polygon) { return canvas.getNumPolygonPoints(polygon); }
		void removePolygonPoint(int polygon,int num) { canvas.removePolygonPoint(polygon,num); }
		int addPolygonPoint(int polygon,vec3 point) { return canvas.addPolygonPoint(polygon,point); }
		vec3 getPolygonPoint(int polygon,int num) { return canvas.getPolygonPoint(polygon,num); }
		void setPolygonTexCoord(int polygon,vec3 texcoord) { canvas.setPolygonTexCoord(polygon,texcoord); }
		vec3 getPolygonTexCoord(int polygon,int num) { return canvas.getPolygonTexCoord(polygon,num); }
		
		// polygon indices
		void clearPolygonIndices(int polygon) { return canvas.clearPolygonIndices(polygon); }
		int getNumPolygonIndices(int polygon) { return canvas.getNumPolygonIndices(polygon); }
		void removePolygonIndex(int polygon,int num) { canvas.removePolygonIndex(polygon,num); }
		int addPolygonIndex(int polygon,int index) { return canvas.addPolygonIndex(polygon,index); }
		int getPolygonIndex(int polygon,int num) { return canvas.getPolygonIndex(polygon,num); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_CANVAS_H__ */
