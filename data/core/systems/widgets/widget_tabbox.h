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


#ifndef __UNIGINE_WIDGET_TABBOX_H__
#define __UNIGINE_WIDGET_TABBOX_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class TabBox : Widget {
		
		WidgetTabBox tabbox;
		
		// constructor
		TabBox(int x = 0,int y = 0) {
			if(x is WidgetTabBox) {
				owner = 0;
				tabbox = x;
			} else {
				tabbox = new WidgetTabBox(engine.getGui(),x,y);
			}
			widget = tabbox;
		}
		
		// space size
		void setSpace(int x,int y) { tabbox.setSpace(x,y); }
		int getSpaceX() { return tabbox.getSpaceX(); }
		int getSpaceY() { return tabbox.getSpaceY(); }
		
		// padding size
		void setPadding(int l,int r,int t,int b) { tabbox.setPadding(l,r,t,b); }
		int getPaddingLeft() { return tabbox.getPaddingLeft(); }
		int getPaddingRight() { return tabbox.getPaddingRight(); }
		int getPaddingTop() { return tabbox.getPaddingTop(); }
		int getPaddingBottom() { return tabbox.getPaddingBottom(); }
		
		// texture
		void setImage(Image image) { tabbox.setImage(image); }
		Image getImage() { return tabbox.getImage(); }
		void setTexture(string name) { tabbox.setTexture(name); }
		string getTexture() { return tabbox.getTexture(); }
		
		// clear
		void clear() { tabbox.clear(); }
		
		// tabs
		int addTab(string str,int texture = -1) { return tabbox.addTab(str,texture); }
		void removeTab(int num) { tabbox.removeTab(num); }
		int getNumTabs() { return tabbox.getNumTabs(); }
		
		// tab hidden
		void setTabHidden(int num,int hidden) { tabbox.setTabHidden(num,hidden); }
		int isTabHidden(int num) { return tabbox.isTabHidden(num); }
		
		// tab text
		void setTabText(int num,string str) { tabbox.setTabText(num,str); }
		string getTabText(int num) { return tabbox.getTabText(num); }
		
		// tab data
		void setTabData(int num,string str) { tabbox.setTabData(num,str); }
		string getTabData(int num) { return tabbox.getTabData(num); }
		
		// tab tooltip
		void setTabToolTip(int num,string str) { tabbox.setTabToolTip(num,str); }
		string getTabToolTip(int num) { return tabbox.getTabToolTip(num); }
		
		// tab texture
		void setTabTexture(int num,int texture) { tabbox.setTabTexture(num,texture); }
		int getTabTexture(int num) { return tabbox.getTabTexture(num); }
		
		// current tab
		void setCurrentTab(int num) { tabbox.setCurrentTab(num); }
		int getCurrentTab() { return tabbox.getCurrentTab(); }
		string getCurrentTabText() { return tabbox.getCurrentTabText(); }
		string getCurrentTabData() { return tabbox.getCurrentTabData(); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_TABBOX_H__ */
