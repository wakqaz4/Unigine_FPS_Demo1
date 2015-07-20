/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_panel_resolution.h
 * Desc:    Unigine editor
 * Version: 1.01
 * Author:  Mikhail Lyubimov <alterego@unigine.com>
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

#ifndef __EDITOR_PANEL_RESOLUTION_H__
#define __EDITOR_PANEL_RESOLUTION_H__

#include <editor/panels/editor_panel_base.h>
#include <editor/widgets/editor_widget_viewport_layout.h>

/*
 */
namespace Editor::Panels {
	
	/**************************************************************************\
	*
	* class PanelResolution
	*
	\**************************************************************************/
	
	/*
	 */
	class PanelResolution : PanelBase {
		
		//
		using Unigine::Widgets;
		using Editor::Widgets
		
		Widget groupbox;
		ComboBox resolution_cb;
		
		WidgetViewportWindow viewport;
		WidgetViewportLayout layout;
		
		int width;
		int height;
		
		/*
		 */
		class Resolution {
			float h;
			float w;
			int mode;
			string text;
			
			Resolution(int mode_,float w_,float h_) {
				h = h_;
				w = w_;
				mode = mode_;
				
				if(mode == 1) text = format("Free %dx%d",w_,h_);
				else text = format("%dx%d",w_,h_);
			}
			
			Resolution() {
				text = "Free";
				mode = 0;
			}
		};
		
		/*
		 */
		enum {
			FREE = 0,
			ASPECT,
			FIXED,
		};
		
		Resolution resolutions[] = (
			new Resolution(),
			
			new Resolution(ASPECT, 5.0f, 4.0f),
			new Resolution(ASPECT, 4.0f, 3.0f),
			new Resolution(ASPECT, 16.0f, 9.0f),
			new Resolution(ASPECT, 16.0f, 10.0f),
			
			new Resolution(FIXED, 640, 360),
			new Resolution(FIXED, 960, 540),
			new Resolution(FIXED, 1024, 576),
			new Resolution(FIXED, 1280, 720),
			new Resolution(FIXED, 1366, 768),
			new Resolution(FIXED, 1600, 900),
			new Resolution(FIXED, 1920, 1080),
			new Resolution(FIXED, 2560, 1440),
			new Resolution(FIXED, 3840, 2160),
			new Resolution(FIXED, 640, 400),
			new Resolution(FIXED, 960, 600),
			new Resolution(FIXED, 1024, 640),
			new Resolution(FIXED, 1280, 800),
			new Resolution(FIXED, 1440, 900),
			new Resolution(FIXED, 1680, 1050),
			new Resolution(FIXED, 1920, 1200),
			new Resolution(FIXED, 2560, 1600),
			new Resolution(FIXED, 640, 480),
			new Resolution(FIXED, 800, 600),
			new Resolution(FIXED, 1024, 768),
			new Resolution(FIXED, 1280, 960),
			new Resolution(FIXED, 1280, 1024),
			new Resolution(FIXED, 1600, 1200),
			new Resolution(FIXED, 2048, 1536),
			new Resolution(FIXED, 2560, 2048),
		);
		
		Resolution resolution;
		
		//
		PanelResolution(WidgetViewportWindow viewport_ = NULL) {
			
			init("editor/panels/editor_panel_resolution.ui","Editor::Panels::PanelResolution::");
			
			viewport = viewport_;
			layout = viewport.getLayout();
			
			layout.addWidget("panel_resolution",groupbox);
			layout.arrangePanel("panel_resolution");
			
			resolution = resolutions[0];
			forloop(int i = 0; resolutions.size()) {
				resolution_cb.addItem(resolutions[i].text);
			}
			
			resolution_cb.setCurrentItem(configGet(layout.getConfigName() + "_panel_resolution_current",0));
		}
		
		void screenshot_clicked(PanelResolution panel) {
			string name = "";
			string title = TR("Select a image to save");
			
			if(dialogFileImageSave(title,name) == 0) return NULL;
			name = setExtensionImage(name,"png");
			
			Image image = new Image();
			panel.viewport.renderImage(image);
			image.flipY();
			image.convertToFormat(IMAGE_FORMAT_RGB8);
			image.save(name);
		}
		
		
		//
		void update(int need_reload) {
			if(viewport.isHidden()) return;
			
			if(resolution.mode == ASPECT) {
				int viewport_h = viewport.getHeight() - 29;
				int viewport_w = viewport.getWidth() - 8;
				
				if(height != viewport_h) {
					height = viewport_h;
					width = int((height * resolution.w) / resolution.h);
					viewport.setWidth(width + 8);
				} else if(width != viewport_w) {
					width = viewport_w;
					height = int((width * resolution.h) / resolution.w);
					viewport.setHeight(height + 29);
				}
				
			} else if(resolution.mode == FIXED) {
				viewport.setHeight(resolution.h - 29);
				viewport.setWidth(resolution.w - 8);
			} else {
				viewport.setMinHeight(360);
				viewport.setMinWidth(360);
			}
		}
		
		//
		void render() {
		}
		
		//
		void worldInit() {
		}
		
		//
		void combobox_changed() {
			configSet(layout.getConfigName() + "_panel_resolution_current",resolution_cb.getCurrentItem());
			Resolution new_resolution = resolutions[resolution_cb.getCurrentItem()];
			if(resolution == new_resolution) return;
			resolution = new_resolution;
			
			viewport.setSizeable(1);
			if(resolution.mode == ASPECT) {
				viewport.setMinHeight(360);
				viewport.setMinWidth((360 * resolution.w) / resolution.h);
				
				height = viewport.getHeight() - 29;
				width = int((height * resolution.w) / resolution.h);
				viewport.setHeight(height + 29);
				viewport.setWidth(width + 8);
			} else if(resolution.mode == FREE) {
				viewport.setMinHeight(360);
				viewport.setMinWidth(360);
			} else if(resolution.mode == FIXED) {
				viewport.setSizeable(0);
				viewport.setHeight(resolution.h - 29);
				viewport.setWidth(resolution.w - 8);
			}
		}
	};
	
} /* namespace Editor::Panels */

#endif /* __EDITOR_PANEL_RESOLUTION_H__ */
