/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_tooltip.h
 * Desc:    Unigine editor
 * Version: 1.01
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Maxim Belobryukhov <maxi@unigine.com>
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

/*
 */
namespace ToolTip {
	
	/*
	 */
	#ifdef HAS_INTERFACE
		
		InterfaceWindow tooltip_window;
		WidgetWindow fake_widget;
		
		InterfaceWindow focused_window;
		
		Widget permanent_focused_widget;
		
		/*
		 */
		void init() {
			
			tooltip_window = new InterfaceWindow(0);
			
			fake_widget = new WidgetWindow(tooltip_window.getGui(),"");
			tooltip_window.setWidget(fake_widget);
			fake_widget.setHidden(1);
			
			permanent_focused_widget = NULL;
		}
		
		void shutdown() {
			
			delete fake_widget;
			delete tooltip_window;
			
			// restore interface window gui tooltip
			forloop(int i = 0; engine.interface.getNumWindows()) {
				InterfaceWindow window = engine.interface.getWindow(i);
				Gui gui = window.getGui();
				gui.setToolTipEnabled(1);
			}
			
			// restore engine gui tooltip
			Gui gui = engine.getGui();
			gui.setToolTipEnabled(1);
		}
		
		/*
		 */
		void update() {
			int tooltip_x = 0;
			int tooltip_y = 0;
			int tooltip_width = 0;
			int tooltip_height = 0;
			string tooltip_text = "";
			
			// get engine gui tooltip
			Gui gui = engine.getGui();
			gui.setToolTipEnabled(0);
			
			if(strlen(gui.getToolTipText()) && (viewportsGetFocusedWindow() == NULL || getIntersectionWindow() == NULL)) {
				tooltip_text = gui.getToolTipText();
				tooltip_x = engine.interface.getPositionX() + gui.getToolTipX();
				tooltip_y = engine.interface.getPositionY() + gui.getToolTipY();
				tooltip_width = gui.getToolTipWidth(tooltip_text);
				tooltip_height = gui.getToolTipHeight(tooltip_text);
			} else {
				// get interface window gui tooltip
				int i;
				int num_windows = getNumWindows();
				forloop(int j = 0; min(ceil(num_windows * engine.app.getIFps()) * 3,num_windows)) {
					
					if(i < num_windows - 1) i++;
					else i = 0;
					
					InterfaceWindow window = getWindow(i);
					
					if(window == NULL || window == tooltip_window) continue;
					
					Gui gui = window.getGui();
					
					gui.setToolTipEnabled(0);
					
					if(strlen(gui.getToolTipText()) == 0) continue;
					
					if(engine.gui.getPermanentFocus() == window.getWidget()) {
						
						#ifdef _LINUX
							gui.setToolTipEnabled(1);
							continue;
						#endif
						
						permanent_focused_widget = window.getWidget();
					}
					
					tooltip_text = gui.getToolTipText();
					tooltip_x = window.getPositionX() + gui.getToolTipX();
					tooltip_y = window.getPositionY() + gui.getToolTipY();
					tooltip_width = gui.getToolTipWidth(tooltip_text);
					tooltip_height = gui.getToolTipHeight(tooltip_text);
					i--;
					
					break;
				}
			}
			
			// show tooltip
			if(strlen(tooltip_text)) {
				Gui gui = tooltip_window.getGui();
				gui.setToolTip(0,0,tooltip_text);
				tooltip_x = engine.interface.getVisiblePositionX(tooltip_x,tooltip_width,tooltip_width);
				tooltip_y = engine.interface.getVisiblePositionY(tooltip_y,tooltip_height,tooltip_height);
				tooltip_window.setWidth(tooltip_width);
				tooltip_window.setHeight(tooltip_height);
				tooltip_window.setPosition(tooltip_x,tooltip_y);
				
				if(tooltip_window.isHidden()) {
					focused_window = findFocusedWindow();
					
					if(permanent_focused_widget != NULL) {
						if(focused_window != NULL) focused_window.setFocus();
						fake_widget.setPermanentFocus();
					}
					
					tooltip_window.setHidden(0);
					
					hotkeysRecoverModifiers();
				}
			}
			// hide tooltip
			else {
				if(!tooltip_window.isHidden()) {
					tooltip_window.setHidden(1);
					
					if(permanent_focused_widget != NULL) permanent_focused_widget = NULL;
					
					if(focused_window != NULL) {
						focused_window.setFocus();
						focused_window = NULL;
					}
					
					hotkeysRecoverModifiers();
				}
			}
			
		}
		
	#else
		
		/*
		 */
		void init() { }
		void shutdown() { }
		
		/*
		 */
		void update() { }
		
	#endif
	
} /* namespace ToolTip */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void tooltipInit() {
	ToolTip::init();
}

void tooltipShutdown() {
	ToolTip::shutdown();
}

/*
 */
void tooltipUpdate(int need_reload) {
	ToolTip::update();
}

#ifdef HAS_INTERFACE
/*
 */
InterfaceWindow tooltipGetWindow() {
	return ToolTip::tooltip_window;
}

/*
 */
InterfaceWindow tooltipGetFocusedWindow() {
	return ToolTip::focused_window;
}

#endif
