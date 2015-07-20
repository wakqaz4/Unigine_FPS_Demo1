/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_widget_viewport_layout.h
 * Desc:    Unigine editor
 * Version: 1.00
 * Author:  Vladimir Borovskikh <futurist@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

#ifndef __EDITOR_WIDGET_VIEWPORT_LAYOUT_H__
#define __EDITOR_WIDGET_VIEWPORT_LAYOUT_H__

/*
 */
namespace Editor::Widgets {
	
	using Unigine::Widgets;
	
	/*
	 */
	enum {
		TYPE_HORIZONTAL = 0,
		TYPE_VERTICAL,
		TYPE_CUSTOM,
	};
	
	/*
	 */
	enum {
		ANCHOR_LEFT_TOP = 0,
		ANCHOR_LEFT_BOTTOM,
		ANCHOR_RIGHT_TOP,
		ANCHOR_RIGHT_BOTTOM,
		ANCHOR_TOP_LEFT,
		ANCHOR_TOP_RIGHT,
		ANCHOR_BOTTOM_LEFT,
		ANCHOR_BOTTOM_RIGHT,
		NUM_ANCHORS,
	};
	
	/*
	 */
	string panel_text[0] = (
		"panel_view",
		"panel_camera",
		"panel_resolution",
		"panel_navigation",
		"panel_placement",
		"panel_brush",
		"panel_render",
		"panel_helpers",
		"panel_viewcube",
		"panel_status_bar",
	);
	
	vec3 panel_positions[0] = (
		vec3(1.0f,0.0f,0.0f),
		vec3(0.0f,0.0f,0.0f),
		vec3(1.0f,0.0f,0.0f),
		vec3(1.0f,1.0f,0.0f),
		vec3(0.0f,0.0f,0.0f),
		vec3(1.0f,1.0f,0.0f),
		vec3(0.0f,1.0f,0.0f),
		vec3(0.0f,1.0f,0.0f),
		vec3(1.0f,0.0f,0.0f),
		vec3(1.0f,1.0f,0.0f),
	);
	
	Unigine::Pair default_layout[] = (
		"panel_camera" : new Unigine::Pair("panel_placement",ANCHOR_RIGHT_TOP),
		"panel_render" : new Unigine::Pair("panel_camera",ANCHOR_RIGHT_TOP),
		"panel_helpers" : new Unigine::Pair("panel_render",ANCHOR_RIGHT_TOP),
		"panel_resolution" : new Unigine::Pair("panel_camera",ANCHOR_RIGHT_BOTTOM),
	);
	
	int panel_types[0] = (
		TYPE_HORIZONTAL,
		TYPE_HORIZONTAL,
		TYPE_HORIZONTAL,
		TYPE_HORIZONTAL,
		TYPE_HORIZONTAL,
		TYPE_CUSTOM,
		TYPE_HORIZONTAL,
		TYPE_HORIZONTAL,
		TYPE_CUSTOM,
		TYPE_CUSTOM,
	);
	
	/*
	 */
	enum {
		BOUND_SNAP_NONE = 0,
		BOUND_SNAP_LEFT = 1 << 0,
		BOUND_SNAP_RIGHT = 1 << 1,
		BOUND_SNAP_TOP = 1 << 2,
		BOUND_SNAP_BOTTOM = 1 << 3,
	};
	
	/*
	 */
	enum {
		WINDOW_LEFT_FIX = -12,
		WINDOW_RIGHT_FIX = -8,
		WINDOW_TOP_FIX = -5,
		WINDOW_BOTTOM_FIX = -29,
	};
	
	/**************************************************************************\
	*
	* WidgetViewportLayoutPanel
	*
	\**************************************************************************/
	
	/*
	 */
	class WidgetViewportLayoutPanel {
		
		//
		WidgetViewportLayout layout;
		
		// panel
		VBox panel_box;
		
		// external window
		Window window;
		HBox window_actions;
		Icon window_action_icons[] = (:);
		VBox window_panel_box;
		
		// panel layout
		Widget panel_layout;
		Sprite move_area;
		Widget container;
		
		int x;
		int y;
		int width;
		int height;
		
		int type;
		int need_update;
		int border;
		int windowed;
		int offset_x = 0;
		int offset_y = 0;
		int move_state = 0;
		
		WidgetViewportLayoutPanel parent;
		WidgetViewportLayoutPanel anchors[NUM_ANCHORS];
		int bound_anchor = 0;
		
		//
		WidgetViewportLayoutPanel(WidgetViewportLayout layout_,int type_ = TYPE_HORIZONTAL) {
			layout = layout_;
			type = type_;
			
			#ifdef HAS_INTERFACE
				offset_x = abs(getMouseX() + engine.app.getPositionX() - engine.interface.getMouseX());
				offset_y = abs(getMouseY() + engine.app.getPositionY() - engine.interface.getMouseY());
			#endif
			
			panel_box = new HBox();
			panel_layout = new VBox();
			container = create_container();
			
			#ifdef HAS_INTERFACE
				setOrder(100);
			#endif
			
			window = new Window();
			
			window_actions = new HBox();
			window_actions.setSpace(4,0);
			
			window.addChild(window_actions,ALIGN_OVERLAP | ALIGN_TOP | ALIGN_RIGHT);
			window_actions.setPosition(8,-28);
			window_actions.setHeight(24);
			window_actions.setStencil(1);
			
			window_panel_box = new HBox();
			window.addChild(window_panel_box);
			
			setBorder(0);
			setBackground(1);
			
			create_panel_layout();
			
			panel_box.setHidden(1);
			need_update = 1;
		}
		
		~WidgetViewportLayoutPanel() {
			delete container;
			delete panel_box;
		}
		
		//
		void create_panel_layout() {
			Widget old_panel_layout = panel_layout;
			Widget old_container = container;
			
			// create panel_layout
			VBox new_panel_layout;
			switch(type) {
				case TYPE_VERTICAL: new_panel_layout = new VBox(); break;
				case TYPE_HORIZONTAL: new_panel_layout = new HBox(); break;
				case TYPE_CUSTOM: new_panel_layout = new VBox(); break;
			}
			panel_layout = new_panel_layout;
			
			// create container
			Widget new_container = create_container();
			while(container.getNumChilds() != 0) {
				new_container.addChild(container.getChild(0));
			}
			
			container = new_container;
			
			create_move_area();
			
			// build layout
			if(move_area != NULL) panel_layout.addChild(move_area);
			panel_layout.addChild(container);
			
			if(container.getNumChilds() != 0) panel_box.addChild(panel_layout);
			
			delete old_panel_layout;
			delete old_container;
			
			arrange();
		}
		
		//
		Widget create_container() {
			switch (type) {
				case TYPE_VERTICAL: return new VBox();
				case TYPE_HORIZONTAL: return new HBox();
				case TYPE_CUSTOM: return new VBox();
			}
			
			return NULL;
		}
		
		//
		void create_move_area() {
			delete move_area;
			switch (type) {
				case TYPE_VERTICAL: move_area = new Sprite("editor/gui/panel_move_vertical.png"); break;
				case TYPE_HORIZONTAL: move_area = new Sprite("editor/gui/panel_move_horizontal.png"); break;
				case TYPE_CUSTOM: move_area = NULL; return;
			}
			move_area.setCallback(PRESSED,functionid(pressed_callback),this);
		}
		
		//
		void pressed_callback(WidgetViewportLayoutPanel panel) {
			if(getMouseButton() == APP_BUTTON_LEFT) {
				panel.movePanel();
			}
		}
		
		//
		int getInverseAnchor(int anchor) {
			switch(anchor) {
				case ANCHOR_LEFT_TOP: return ANCHOR_RIGHT_TOP;
				case ANCHOR_LEFT_BOTTOM: return ANCHOR_RIGHT_BOTTOM;
				
				case ANCHOR_RIGHT_TOP: return ANCHOR_LEFT_TOP;
				case ANCHOR_RIGHT_BOTTOM: return ANCHOR_LEFT_BOTTOM;
				
				case ANCHOR_TOP_LEFT: return ANCHOR_BOTTOM_LEFT;
				case ANCHOR_TOP_RIGHT: return ANCHOR_BOTTOM_RIGHT;
				
				case ANCHOR_BOTTOM_LEFT: return ANCHOR_TOP_LEFT;
				case ANCHOR_BOTTOM_RIGHT: return ANCHOR_TOP_RIGHT;
			}
		}
		
		//
		void getAnchorsConstraints(int &left,int &right,int &top,int &bottom) {
			WidgetViewportLayoutPanel panels[0];
			getAttachedPanels(panels);
			left = panel_box.getPositionX() - layout.getPaddingLeft();
			right = panel_box.getPositionX() - layout.getPaddingLeft() + panel_box.getWidth();
			top = panel_box.getPositionY() - layout.getPaddingTop();
			bottom = panel_box.getPositionY() - layout.getPaddingTop() + panel_box.getHeight();
			forloop(int i = 0; panels.size()) {
				if(panels[i].getWidth() == 0 || panels[i].getHeight() == 0) continue;
				
				left = min(left,panels[i].getPositionX());
				right = max(right,panels[i].getPositionX() + panels[i].getWidth());
				top = min(top,panels[i].getPositionY());
				bottom = max(bottom,panels[i].getPositionY() + panels[i].getHeight());
			}
			left = abs(panel_box.getPositionX() - layout.getPaddingLeft() - left);
			right = abs(panel_box.getPositionX() - layout.getPaddingLeft() + panel_box.getWidth() - right);
			top = abs(panel_box.getPositionY() - layout.getPaddingTop() - top);
			bottom = abs(panel_box.getPositionY() - layout.getPaddingTop() + panel_box.getHeight() - bottom);
		}
		
		//
		void arrange() {
			panel_box.arrange();
			
			width = panel_box.getWidth();
			height = panel_box.getHeight();
			
			int left,right,top,bottom;
			getAnchorsConstraints(left,right,top,bottom);
			
			if(parent == NULL && width != 0 && height != 0) {
				if(left + width + right <= layout.getParentWidth()) x = clamp(x,left,layout.getParentWidth() - width - right);
				else x = left;
				if(top + height + bottom <= layout.getParentHeight()) y = clamp(y,top,layout.getParentHeight() - height - bottom);
				else y = top;
			}
			
			panel_box.setPosition(x + layout.getPaddingLeft(),y + layout.getPaddingTop());
			
			window_panel_box.arrange();
			window.setWidth(window_panel_box.getWidth());
			window.setHeight(window_panel_box.getHeight());
			window.arrange();
			
			arrange_anchors();
		}
		
		//
		void arrange_anchors() {
			forloop(int i = 0; anchors.size()) {
				if(anchors[i] != NULL) {
					anchors[i].moveToAnchor(this,i);
					anchors[i].arrange();
				}
			}
		}
		
		//
		int intersect(WidgetViewportLayoutPanel p,int snap_distance = 0) {
			if(p.panel_box.getPositionX() > panel_box.getPositionX() + panel_box.getWidth() + snap_distance) return 0;
			if(p.panel_box.getPositionX() + p.panel_box.getWidth() < panel_box.getPositionX() - snap_distance) return 0;
			if(p.panel_box.getPositionY() > panel_box.getPositionY() + panel_box.getHeight() + snap_distance) return 0;
			if(p.panel_box.getPositionY() + p.panel_box.getHeight() < panel_box.getPositionY() - snap_distance) return 0;
			
			return 1;
		}
		
		//
		int contains(int x,int y) {
			return getLeft() < x && x < getRight() && getTop() < y && y < getBottom();
		}
		
		//
		void moveStart() {
			if(move_state == 0) move_state = 1;
		}
		
		//
		void movePanel() {
			if(!windowed) layout.setMove(this);
			if(windowed && move_state == 2) window.setPosition(window.getPositionX() + getMouseDX(),window.getPositionY() + getMouseDY());
			if(move_state == 1) move_state = 2;
		}
		
		//
		void moveEnd() {
			move_state = 0;
		}
		
		//
		void setWindowed(int w) {
			windowed = w;
			if(windowed) {
				releaseAnchors();
				
				if(layout.getParent() == NULL) removeChild(panel_box);
				else Widget(layout.getParent()).removeChild(panel_box);
				
				window_panel_box.addChild(panel_layout);
				window.addChild(window_panel_box);
				window.setWidth(panel_box.getWidth());
				window.setHeight(panel_box.getHeight());
				
				addChild(window,ALIGN_OVERLAP);
				
				#ifdef HAS_INTERFACE
					offset_x = abs(getMouseX() + engine.app.getPositionX() - engine.interface.getMouseX());
					offset_y = abs(getMouseY() + engine.app.getPositionY() - engine.interface.getMouseY());
				#endif
				
				window.setPosition(panel_box.getPositionX() + offset_x,panel_box.getPositionY() + offset_y);
				
				window.arrange();
				
				arrange();
				
			} else if(window != NULL) {
				
				if(layout.getParent() == NULL) addChild(panel_box,ALIGN_OVERLAP);
				else Widget(layout.getParent()).addChild(panel_box,ALIGN_OVERLAP);
				
				create_panel_layout();
				
				#ifdef HAS_INTERFACE
					offset_x = abs(getMouseX() + engine.app.getPositionX() - engine.interface.getMouseX());
					offset_y = abs(getMouseY() + engine.app.getPositionY() - engine.interface.getMouseY());
				#endif
				
				setPosition(window.getPositionX() - offset_x,window.getPositionY() - layout.getPaddingTop() - offset_y);
				
				removeChild(window);
				
				arrange();
			}
		}
		int getWindowed() {
			return windowed;
		}
		
		//
		void setWindowText(string text) {
			window.setText(text);
		}
		
		string getWindowText() {
			return window.getText();
		}
		
		//
		void addAction(string name,string icon) {
			if(window_action_icons.check(name)) return;
			
			Icon action = new Icon(icon);
			action.setWidth(16);
			action.setHeight(16);
			window_action_icons.append(name,action);
			window_actions.addChild(action);
		}
		
		void setActionCallback(string name,int callback,string function,int v0 = 0,int v1 = 0,int v2 = 0,int v3 = 0) {
			if(!window_action_icons.check(name)) return;
			
			window_action_icons[name].setCallback(callback,function,v0,v1,v2,v3);
		}
		
		void setActionEnabled(string name,int enabled) {
			window_action_icons[name].setEnabled(enabled);
		}
		
		int isActionEnabled(string name) {
			window_action_icons[name].isEnabled();
		}
		
		void removeAction(string name) {
			if(!window_action_icons.check(name)) return;
			
			window_actions.removeChild(window_action_icons[name]);
			delete window_action_icons[name];
			window_action_icons.remove(name);
		}
		
		//
		void setType(int type_) {
			type = type_;
			create_panel_layout();
		}
		
		//
		int getType() {
			return type;
		}
		
		//
		int getWidth() { return width; }
		int getHeight() { return height; }
		
		//
		void setPosition(int x_,int y_) {
			x = x_;
			y = y_;
			need_update = 1;
		}
		
		//
		int getPositionX() { return x; }
		int getPositionY() { return y; }
		
		void setLeft(int left) { x = left; need_update = 1; }
		int getLeft() { return x; }
		
		void setRight(int right) { x = right - width; need_update = 1; }
		int getRight() { return x + width; }
		
		void setTop(int top) { y = top; need_update = 1;}
		int getTop() { return y; }
		
		void setBottom(int bottom) { y = bottom - height; need_update = 1; }
		int getBottom() { return y + height; }
		
		//
		void setEnabled(int enable) { panel_box.setEnabled(enable); }
		int isEnabled() { return panel_box.isEnabled(); }
		
		//
		void setHidden(int hidden) {
			if(!windowed) {
				panel_box.setHidden(hidden);
			} else {
				#ifdef HAS_INTERFACE
				InterfaceWindow interface = window.getInterface();
					if(interface != NULL) interface.setHidden(hidden);
				#endif
				window.setHidden(hidden);
			}
		}
		int isHidden() {
			if(!windowed) return panel_box.isHidden();
			else return window.isHidden();
		}
		
		//
		void setBackground(int mode) { panel_box.call("setBackground",mode); }
		int getBackground() { return panel_box.call("getBackground"); }
		
		//
		void setBorder(int mode) {
			border = mode;
			int order = getOrder();
			int old_backgroud = getBackground();
			
			VBox new_panel_box;
			if(border == 0) new_panel_box = new HBox();
			else new_panel_box = new GroupBox();
			
			if(!windowed) new_panel_box.addChild(panel_layout);
			
			Widget old_panel_box = panel_box;
			panel_box = new_panel_box;
			
			if(layout.getParent() == NULL) addChild(panel_box,ALIGN_OVERLAP);
			else Widget(layout.getParent()).addChild(panel_box,ALIGN_OVERLAP);
			
			delete old_panel_box;
			
			setBackground(old_backgroud);
			
			VBox new_window_panel_box;
			
			if(border == 0) new_window_panel_box = new HBox();
			else new_window_panel_box = new GroupBox();
			
			if(windowed) new_panel_box.addChild(panel_layout);
			
			Widget old_window_panel_box = window_panel_box;
			window_panel_box = new_window_panel_box;
			
			delete old_window_panel_box;
			
			need_update = 1;
			
			setOrder(order);
		}
		
		int getBorder() { return border; }
		
		//
		void setOrder(int order) {
			panel_box.widget.setOrder(order);
		}
		
		int getOrder() {
			return panel_box.widget.getOrder();
		}
		
		//
		void addWidget(Widget widget) {
			container.addChild(widget);
			if(container.getNumChilds() != 0 && panel_box.getNumChilds() == 0 && !windowed) panel_box.addChild(panel_layout);
			need_update = 1;
		}
		
		//
		void removeWidget(Widget widget) {
			container.removeChild(widget);
			if(container.getNumChilds() == 0 && panel_box.getNumChilds() != 0 && windowed) panel_box.removeChild(panel_layout);
			need_update = 1;
		}
		
		//
		int getNumWidgets() { return container.getNumChilds(); }
		
		//
		Widget getWidget(int num) { return container.getChild(num); }
		
		//
		WidgetViewportLayoutPanel setParent(WidgetViewportLayoutPanel p) {
			parent = p;
		}
		WidgetViewportLayoutPanel getParent() {
			return parent;
		}
		
		//
		int findAnchor(WidgetViewportLayoutPanel panel) {
			return anchors.find(panel,-1);
		}
		
		int isAnchorEmpty(int anchor) {
			return anchors[anchor] == NULL;
		}
		
		void releaseAnchor(int anchor) {
			if(anchors[anchor] == NULL) return;
			anchors[anchor].setParent(NULL);
			anchors[anchor] = NULL;
		}
		
		void releaseAnchors() {
			forloop(int i = 0; anchors.size()) {
				releaseAnchor(i);
			}
		}
		
		void releaseHiddenAnchors() {
			forloop(int i = 0; anchors.size()) {
				if(anchors[i] != NULL && anchors[i].isHidden()) releaseAnchor(i);
			}
		}
		
		//
		void attachToPanel(WidgetViewportLayoutPanel panel,int anchor) {
			int id = panel.findAnchor(this);
			if(id != -1) panel.releaseAnchor(id);
			anchors[anchor] = panel;
			anchors[anchor].setParent(this);
			need_update = 1;
		}
		
		//
		void detachFromPanel(WidgetViewportLayoutPanel panel) {
			int id = anchors.find(panel,-1);
			if(id == -1) return;
			releaseAnchor(id);
			need_update = 1;
		}
		
		//
		void getAttachedPanels(WidgetViewportLayoutPanel panels[]) {
			forloop(int i = 0; anchors.size()) {
				if(anchors[i] == NULL) continue;
				panels.append(anchors[i]);
				anchors[i].getAttachedPanels(panels);
			}
		}
		
		//
		int isAttached(WidgetViewportLayoutPanel panel) {
			forloop(int i = 0; anchors.size()) {
				if(anchors[i] == NULL) continue;
				if(anchors[i].isAttached(panel)) return true;
			}
			return anchors.find(panel,-1) != -1;
		}
		
		//
		void moveToAnchor(WidgetViewportLayoutPanel panel,int anchor) {
			switch(anchor) {
				case ANCHOR_LEFT_TOP:
					setRight(panel.getLeft());
					setTop(panel.getTop());
				break;
				case ANCHOR_LEFT_BOTTOM:
					setRight(panel.getLeft());
					setBottom(panel.getBottom());
				break;
				case ANCHOR_RIGHT_TOP:
					setLeft(panel.getRight());
					setTop(panel.getTop());
				break;
				case ANCHOR_RIGHT_BOTTOM:
					setLeft(panel.getRight());
					setBottom(panel.getBottom());
				break;
				case ANCHOR_TOP_LEFT:
					setLeft(panel.getLeft());
					setBottom(panel.getTop());
				break;
				case ANCHOR_TOP_RIGHT:
					setRight(panel.getRight());
					setBottom(panel.getTop());
				break;
				case ANCHOR_BOTTOM_LEFT:
					setLeft(panel.getLeft());
					setTop(panel.getBottom());
				break;
				case ANCHOR_BOTTOM_RIGHT:
					setRight(panel.getRight());
					setTop(panel.getBottom());
				break;
			}
		}
		
		//
		int snapToPanel(WidgetViewportLayoutPanel panel) {
			if(isAttached(panel)) return -1;
			if(intersect(panel,layout.getSnapDistance()) == 0) return -1;
			
			int anchor = -1;
			
			if(abs(panel.getTop() - getBottom()) < layout.getSnapDistanceY()) {
				int left_diff = abs(panel.getLeft() - getLeft());
				int right_diff = abs(panel.getRight() - getRight());
				
				if(min(left_diff,right_diff) < layout.getSnapDistanceX()) {
					if(left_diff < right_diff) anchor = ANCHOR_TOP_LEFT;
					else anchor = ANCHOR_TOP_RIGHT;
				}
			}
			
			if(abs(panel.getBottom() - getTop()) < layout.getSnapDistanceY()) {
				int left_diff = abs(panel.getLeft() - getLeft());
				int right_diff = abs(panel.getRight() - getRight());
				
				if(min(left_diff,right_diff) < layout.getSnapDistanceX()) {
					if(left_diff < right_diff) anchor = ANCHOR_BOTTOM_LEFT;
					else anchor = ANCHOR_BOTTOM_RIGHT;
				}
			}
			
			if(abs(panel.getRight() - getLeft()) < layout.getSnapDistanceX()) {
				int top_diff = abs(panel.getTop() - getTop());
				int bottom_diff = abs(panel.getBottom() - getBottom());
				if(min(top_diff,bottom_diff) < layout.getSnapDistanceY()) {
					if(top_diff < bottom_diff) anchor = ANCHOR_RIGHT_TOP;
					else anchor = ANCHOR_RIGHT_BOTTOM;
				}
			}
			
			if(abs(panel.getLeft() - getRight()) < layout.getSnapDistanceX()) {
				int top_diff = abs(panel.getTop() - getTop());
				int bottom_diff = abs(panel.getBottom() - getBottom());
				if(min(top_diff,bottom_diff) < layout.getSnapDistanceY()) {
					if(top_diff < bottom_diff) anchor = ANCHOR_LEFT_TOP;
					else anchor = ANCHOR_LEFT_BOTTOM;
				}
			}
			
			if(anchor != -1) {
				
				// check intersection with other panels
				int has_intersect_with_attached_panels = false;
				
				// self panels
				WidgetViewportLayoutPanel self_panels[0];
				getAttachedPanels(self_panels);
				self_panels.append(this);
				
				// target panels
				WidgetViewportLayoutPanel target_panels[0];
				panel.getAttachedPanels(target_panels);
				target_panels.append(panel);
				if(panel.parent != NULL) {
					WidgetViewportLayoutPanel current_parent = panel.parent;
					while(current_parent != NULL) {
						target_panels.append(current_parent);
						current_parent = current_parent.parent;
					}
				}
				
				// check intersection with target attached panels
				if(!has_intersect_with_attached_panels) {
					WidgetViewportLayoutPanel panels[0];
					panel.getAttachedPanels(panels);
					forloop(int i = 0; panels.size()) {
						if(panels[i].intersect(this,-1)) {
							has_intersect_with_attached_panels = true;
							break;
						}
					}
				}
				
				// check intersection with self attached panels
				if(!has_intersect_with_attached_panels) {
					WidgetViewportLayoutPanel panels[0];
					getAttachedPanels(panels);
					forloop(int i = 0; panels.size()) {
						if(panels[i].intersect(panel,-1)) {
							has_intersect_with_attached_panels = true;
							break;
						}
					}
				}
				
				// check intersection self attached panels with target attached panels
				if(!has_intersect_with_attached_panels) {
					forloop(int i = 0; self_panels.size()) {
						forloop(int j = 0; target_panels.size()) {
							if(self_panels[i].intersect(target_panels[j],-1)) {
								has_intersect_with_attached_panels = true;
								break;
							}
						}
						if(has_intersect_with_attached_panels) break;
					}
				}
				
				if(!has_intersect_with_attached_panels && isAnchorEmpty(getInverseAnchor(anchor)) && panel.isAnchorEmpty(anchor)) {
					moveToAnchor(panel,anchor);
				}
			}
			
			need_update = 1;
			panel.need_update = 1;
			
			return anchor;
		}
		
		//
		int getBoundAnchor() { return bound_anchor; }
		
		//
		void attachToBound(int bound) {
			if(bound & BOUND_SNAP_LEFT) {
				setPosition(0.0f,getPositionY());
			} else if(bound & BOUND_SNAP_RIGHT) {
				setPosition(layout.getParentWidth(),getPositionY());
			}
			
			if(bound & BOUND_SNAP_TOP) {
				setPosition(getPositionX(),0.0f);
			} else if(bound & BOUND_SNAP_BOTTOM) {
				setPosition(getPositionX(),layout.getParentHeight());
			}
			bound_anchor = bound;
		}
		
		//
		int snapToBounds() {
			int result = 0;
			
			int left,right,top,bottom;
			getAnchorsConstraints(left,right,top,bottom);
			
			// left/right
			if(getLeft() - left < layout.getSnapDistanceX()) {
				result |= BOUND_SNAP_LEFT;
			} else if(layout.getParentWidth() - getRight() - right< layout.getSnapDistanceX()) {
				result |= BOUND_SNAP_RIGHT;
			}
			
			// top/bottom
			if(getTop() - top < layout.getSnapDistanceY()) {
				result |= BOUND_SNAP_TOP;
			} else if(layout.getParentHeight() - getBottom() - bottom < layout.getSnapDistanceY()) {
				result |= BOUND_SNAP_BOTTOM;
			}
			
			return result;
		}
		
		//
		void update() {
			if(need_update) {
				arrange();
				need_update = 0;
			}
		}
	};
	
	/**************************************************************************\
	*
	* WidgetViewportLayout
	*
	\**************************************************************************/
	
	/*
	 */
	class WidgetViewportLayout {
		
		WidgetViewportLayoutPanel panels[] = (:);
		Widget parent;
		
		int first_load;
		
		int padding_top;
		int padding_bottom;
		int padding_left;
		int padding_right;
		
		int parent_width;
		int parent_height;
		
		int scale_positions;
		int snap_distance = 20;
		int snap_distance_x = 20;
		int snap_distance_y = 20;
		int offset_x;
		int offset_y;
		
		string move = NULL;
		int anchor;
		string snap_to;
		
		int focused;
		
		//
		WidgetViewportLayout(Widget parent_ = NULL) {
			first_load = true;
			
			parent = parent_;
			
			parent_width = getParentWidth();
			parent_height = getParentHeight();
			
			if(parent == NULL) setConfigName("main");
			else setConfigName(parent.call("getText"));
			
			if(parent == NULL) {
				setPadding(0,0,menuGetHeight(),-menuGetHeight());
			} else if(parent.getType() == WINDOW) {
				setPadding(WINDOW_LEFT_FIX,WINDOW_RIGHT_FIX,WINDOW_TOP_FIX,WINDOW_BOTTOM_FIX);
			}
			
			
			// default positions
			forloop(int i = 0; panel_text.size()) {
				panels.append(panel_text[i],new WidgetViewportLayoutPanel(this,panel_types[i]));
				setPosition(panel_text[i],int(panel_positions[i].x * getParentWidth()),int(panel_positions[i].y * getParentHeight()));
			}
			
			
			// load positions
			forloop(int i = 0; panels.size()) {
				loadPanelPosition(panels.key(i));
				loadPanelHidden(panels.key(i));
			}
			
			// load layout
			thread([](WidgetViewportLayout self) {
				forloop(int i = 0; self.panels.size()) {
					string name = self.panels.key(i);
					if(self.loadPanelLayout(name)) continue;
					if(!default_layout.check(name)) continue;
					self.attachToPanel(default_layout[name].getFirst(),name,default_layout[name].getSecond());
				}
				
				self.resize();
			},this);
		}
		
		~WidgetViewportLayout() {
			forloop(int i = 0; panels.size()) {
				savePanelPosition(panels.key(i));
				savePanelHidden(panels.key(i));
				savePanelLayout(panels.key(i));
			}
			
			panels.delete();
		}
		
		//
		Widget getParent() { return parent; }
		
		//
		int getParentWidth() {
			if(parent != NULL) return parent.getWidth() + padding_right;
			return getWidth() + padding_right;
		}
		
		int getParentHeight() {
			if(parent != NULL) return parent.getHeight() + padding_bottom;
			return getHeight() + padding_bottom;
		}
		
		//
		int getMouseX() { return Unigine::Widgets::getMouseX(); }
		int getMouseY() { return Unigine::Widgets::getMouseY() - getPaddingTop(); }
		
		//
		string addPanel(string name,int type = TYPE_HORIZONTAL) {
			if(panels.check(name)) return name;
			panels.append(name,new WidgetViewportLayoutPanel(this,type));
			
			loadPanelPosition(name);
			loadPanelHidden(name);
			
			return name;
		}
		
		//
		WidgetViewportLayoutPanel getPanel(string name) {
			assert(panels.check(name) && "WidgetViewportLayout::getPanel(): bad key");
			return panels[name];
		}
		
		string config_name;
		
		void setConfigName(string config_name_) {
			config_name = config_name_;
		}
		
		string getConfigName() {
			return config_name;
		}
		
		//
		void savePanelPosition(string name) {
			configSet(config_name + "_panel_" + name,format("%d %d %d",panels[name].getPositionX(),panels[name].getPositionY(),panels[name].getBoundAnchor()));
		}
		
		//
		void loadPanelPosition(string name) {
			int value[0];
			sscanf(configGet(config_name + "_panel_" + name,format("%d %d %d",
				panels[name].getPositionX(),panels[name].getPositionY(),panels[name].getBoundAnchor())),"%d %d %d",value);
			panels[name].setPosition(value[0],value[1]);
			if(value.size() == 3) panels[name].attachToBound(value[2]);
		}
		
		//
		void savePanelHidden(string name) {
			configSet(config_name + "_panel_" + name + "_hidden",isHidden(name));
		}
		
		//
		void loadPanelHidden(string name) {
			setHidden(name,configGet(config_name + "_panel_" + name + "_hidden",0));
		}
		
		//
		void savePanelLayout(string name) {
			string get_snap(string name) {
				WidgetViewportLayoutPanel parent_panel = panels[name].getParent();
				if(parent_panel == NULL) return NULL;
				string p = findPanel(parent_panel);
				if(p == "-1") return NULL;
				int anchor = parent_panel.findAnchor(panels[name]);
				return format("%s %d",p,anchor);
			}
			configSet(config_name + "_panel_" + name + "_snap_to",get_snap(name));
		}
		
		//
		int loadPanelLayout(string name) {
			string snapped_to = configGet(config_name + "_panel_" + name + "_snap_to","-1");
			if(snapped_to == "-1") return false;
			if(snapped_to == "0") return true;
			
			string snapped[0];
			sscanf(snapped_to,"%s %s",snapped);
			if(snapped[1] == "-1") return false;
			
			panels[snapped[0]].arrange();
			panels[name].arrange();
			attachToPanel(snapped[0],name,int(snapped[1]));
			
			return true;
		}
		
		//
		int getSnapDistance() { return snap_distance; }
		
		//
		int getSnapDistanceX() { return snap_distance_x; }
		int getSnapDistanceY() { return snap_distance_y; }
		
		//
		int getNumPannels() { return panels.size(); }
		
		//
		void setFocused(int focused_) { focused = focused_; }
		int isFocused() { return focused; }
		
		//
		void setPadding(int left,int right,int top,int bottom) {
			padding_left = left;
			padding_right = right;
			padding_top = top;
			padding_bottom = bottom;
		}
		
		//
		int getPaddingLeft() { return padding_left; }
		int getPaddingRight() { return padding_right; }
		int getPaddingTop() { return padding_top; }
		int getPaddingBottom() { return padding_bottom; }
		
		//
		void setEnabled(string name,int enable) {
			assert(panels.check(name) && "WidgetViewportLayout::setEnabled(): bad key");
			panels[name].setEnabled(enable);
		}
		
		int isEnabled(string name) {
			assert(panels.check(name) && "WidgetViewportLayout::isEnabled(): bad key");
			return panels[name].isEnabled();
		}
		
		//
		void setHidden(string name,int hidden) {
			assert(panels.check(name) && "WidgetViewportLayout::setHidden(): bad key");
			panels[name].setHidden(hidden);
		}
		
		int isHidden(string name) {
			assert(panels.check(name) && "WidgetViewportLayout::isHidden(): bad key");
			return panels[name].isHidden();
		}
		
		//
		void setBackground(string name,int mode) {
			assert(panels.check(name) && "WidgetViewportLayout::setBackground(): bad key");
			panels[name].setBackground(mode); 
		}
		
		int getBackground(string name) { 
			assert(panels.check(name) && "WidgetViewportLayout::getBackground(): bad key");
			return panels[name].getBackground();
		}
		
		//
		void setBorder(string name,int mode) {
			assert(panels.check(name) && "WidgetViewportLayout::setBorder(): bad key");
			panels[name].setBorder(mode); 
		}
		
		int getBorder(string name) { 
			assert(panels.check(name) && "WidgetViewportLayout::getBorder(): bad key");
			return panels[name].getBorder();
		}
		
		//
		void setOrder(string name,int order) {
			assert(panels.check(name) && "WidgetViewportLayout::setBorder(): bad key");
			panels[name].setOrder(order); 
		}
		
		int getOrder(string name) {
			assert(panels.check(name) && "WidgetViewportLayout::setBorder(): bad key");
			return panels[name].getOrder(); 
		}
		
		//
		void setPosition(string name,int x,int y) {
			assert(panels.check(name) && "WidgetViewportLayout::setPosition(): bad key");
			panels[name].setPosition(x,y);
			snapPanel(name);
		}
		
		//
		int getPositionX(string name) {
			assert(panels.check(name) && "WidgetViewportLayout::getPositionX(): bad key");
			return panels[name].getPositionX();
		}
		int getPositionY(string name) {
			assert(panels.check(name) && "WidgetViewportLayout::getPositionY(): bad key");
			return panels[name].getPositionY();
		}
		
		//
		void setType(string name,int type) {
			assert(panels.check(name) && "WidgetViewportLayout::setType(): bad key");
			panels[name].setType(type);
		}
		
		int getType(string name) {
			assert(panels.check(name) && "WidgetViewportLayout::getType(): bad key");
			return panels[name].getType();
		}
		
		//
		int getNumWidgets(string name) {
			assert(panels.check(name) && "WidgetViewportLayout::getNumWidgets(): bad key");
			return panels[name].getNumWidgets();
		}
		
		//
		Widget getWidget(string name,int num) {
			assert(panels.check(name) && "WidgetViewportLayout::getWidget(): bad key");
			return panels[name].getWidget(num);
		}
		
		//
		void addWidget(string name,Widget widget) {
			assert(panels.check(name) && "WidgetViewportLayout::addWidget(): bad key");
			panels[name].addWidget(widget);
		}
		
		//
		void removeWidget(string name,Widget widget) {
			assert(panels.check(name) && "WidgetViewportLayout::removeWidget(): bad key");
			panels[name].removeWidget(widget);
		}
		
		//
		void arrangePanel(string name) {
			if(move != NULL) return;
			assert(panels.check(name) && "WidgetViewportLayout::arrangePanel(): bad key");
			panels[name].arrange();
			WidgetViewportLayoutPanel p = panels[name].getParent();
			while(p != NULL) {
				p.arrange();
				p = p.getParent();
			}
		}
		
		//
		void detachPanel(string name) {
			foreachkey(string key; panels) {
				panels[key].releaseHiddenAnchors();
				if(key == name) continue;
				panels[key].detachFromPanel(panels[name]);
			}
		}
		
		//
		void attachToPanel(string parent,string child,int anchor) {
			if(!panels.check(parent) || !panels.check(child)) return;
			if(panels[child].getNumWidgets() == 0) return;
			panels[parent].attachToPanel(panels[child],anchor);
		}
		
		//
		void snapPanel(string name) {
			
			foreachkey(string key; panels) {
				if(name == key) continue;
				if(panels[key].isHidden()) continue;
				WidgetViewportLayoutPanel anchors[0];
				panels[name].getAttachedPanels(anchors);
				if(anchors.find(panels[key]) != -1) continue;
				
				anchor = panels[name].snapToPanel(panels[key]);
				if(anchor != -1) {
					snap_to = key;
					break;
				}
			}
			
			panels[name].attachToBound(panels[name].snapToBounds());
		}
		
		//
		void setMove(WidgetViewportLayoutPanel panel) {
			string key = findPanel(panel);
			if(key == "-1") return;
			
			if(move != key) {
				offset_x = panels[key].getPositionX() - this.getMouseX();
				offset_y = panels[key].getPositionY() - this.getMouseY();
				panels[key].arrange();
				detachPanel(key);
				move = key;
			}
		}
		
		//
		string findPanel(WidgetViewportLayoutPanel panel) {
			return panels.find(panel,"-1");
		}
		
		void resize() {
			int new_parent_width = getParentWidth();
			int new_parent_height = getParentHeight();
			
			WidgetViewportLayoutPanel root_panels[0];
			
			foreach(WidgetViewportLayoutPanel p; panels) {
				if(p.getParent() != NULL) continue;
				root_panels.append(p);
			}
			
			// rescale positions with new viewport size
			forloop(int i = 0; root_panels.size()) {
				
				float scale_x = 1.0f;
				float scale_y = 1.0f;
				
				if(scale_positions) {
					scale_x = float(new_parent_width) / parent_width;
					scale_y = float(new_parent_height) / parent_height;
				}
				
				int left,right,top,bottom;
				root_panels[i].getAnchorsConstraints(left,right,top,bottom);
				
				int scaled_position_x = root_panels[i].getPositionX() * scale_x;
				int scaled_position_y = root_panels[i].getPositionY() * scale_y;
				
				/*if(root_panels[i].bound_anchor & BOUND_SNAP_LEFT) {
					scaled_position_x = left;
				}*/
				if(root_panels[i].bound_anchor & BOUND_SNAP_RIGHT) {
					scaled_position_x = new_parent_width - root_panels[i].getWidth() - right;
				}
				/*if(root_panels[i].bound_anchor & BOUND_SNAP_TOP) {
					scaled_position_y = top;
				}*/
				if(root_panels[i].bound_anchor & BOUND_SNAP_BOTTOM) {
					scaled_position_y = new_parent_height - root_panels[i].getHeight() - bottom;
				}
				
				root_panels[i].setPosition(scaled_position_x,scaled_position_y);
			}
			
			panels.call("arrange");
		}
		
		//
		void update() {
			
			int new_parent_width = getParentWidth();
			int new_parent_height = getParentHeight();
			
			if(parent_width != new_parent_width || parent_height != new_parent_height) {
				
				if(first_load) {
					parent_width = new_parent_width;
					parent_height = new_parent_height;
					first_load = false;
					return;
				}
				
				resize();
				
				parent_width = new_parent_width;
				parent_height = new_parent_height;
			}
			
			int x = this.getMouseX();
			int y = this.getMouseY();
			
			int is_focused = false;
			forloop(int i = 0; panels.size()) {
				if(panels[panels.key(i)].contains(x,y)) {
					is_focused = true;
					break;
				}
			}
			setFocused(is_focused);
			
			if(getMouseButton() == APP_BUTTON_LEFT) {
				if(move != NULL) {
					panels[move].setPosition(x + offset_x,y + offset_y);
					snapPanel(move);
					panels[move].arrange();
				}
			} else {
				if(move != NULL) {
					if(anchor != -1) {
						attachToPanel(snap_to,move,anchor);
					}
					anchor = -1;
					snap_to = NULL;
				}
				move = NULL;
			}
			
			panels.call("update");
		}
		
		// find panels intersected by rectangle
		void getIntersectedPanels(int left,int top,int right,int bottom,WidgetViewportLayoutPanel panels_list[]) {
			panels_list.clear();
			
			foreach(WidgetViewportLayoutPanel panel; panels) {
				int panel_right = panel.panel_box.getPositionX() + panel.panel_box.getWidth();
				int panel_left = panel.panel_box.getPositionX();
				int panel_top = panel.panel_box.getPositionY();
				int panel_bottom = panel.panel_box.getPositionY() + panel.panel_box.getHeight();
				
				if((panel_right > left) && (panel_left < right) &&
				   (panel_bottom > top) && (panel_top < bottom) && panel.panel_box.isHidden() == 0) {
					
					panels_list.append(panel);
				}
			}
		}
		
	};
	
} /* namespace Editor::Widgets */

#endif // __EDITOR_WIDGET_VIEWPORT_LAYOUT_H__
