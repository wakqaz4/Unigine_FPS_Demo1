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


#ifndef __UNIGINE_WIDGET_GRIDBOX_H__
#define __UNIGINE_WIDGET_GRIDBOX_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class GridBox : Widget {
		
		WidgetGridBox gridbox;
		
		// constructor
		GridBox(int num = 2,int x = 0,int y = 0) {
			if(num is WidgetGridBox) {
				owner = 0;
				gridbox = num;
			} else {
				gridbox = new WidgetGridBox(engine.getGui(),num,x,y);
			}
			widget = gridbox;
		}
		
		// space size
		void setSpace(int x,int y) { gridbox.setSpace(x,y); }
		int getSpaceX() { return gridbox.getSpaceX(); }
		int getSpaceY() { return gridbox.getSpaceY(); }
		
		// padding size
		void setPadding(int l,int r,int t,int b) { gridbox.setPadding(l,r,t,b); }
		int getPaddingLeft() { return gridbox.getPaddingLeft(); }
		int getPaddingRight() { return gridbox.getPaddingRight(); }
		int getPaddingTop() { return gridbox.getPaddingTop(); }
		int getPaddingBottom() { return gridbox.getPaddingBottom(); }
		
		// stencil flag
		void setStencil(int stencil) { gridbox.setStencil(stencil); }
		int getStencil() { return gridbox.getStencil(); }
		
		// color multiplier
		void setColor(vec4 color) { gridbox.setColor(color); }
		vec4 getColor() { return gridbox.getColor(); }
		
		// background flag
		void setBackground(int background) { gridbox.setBackground(background); }
		int getBackground() { return gridbox.getBackground(); }
		
		// columns
		void setNumColumns(int num) { gridbox.setNumColumns(num); }
		int getNumColumns() { return gridbox.getNumColumns(); }
		
		// column radio
		void setColumnRatio(int num,int radio) { gridbox.setColumnRatio(num,radio); }
		int getColumnRatio(int num) { return gridbox.getColumnRatio(num); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_GRIDBOX_H__ */
