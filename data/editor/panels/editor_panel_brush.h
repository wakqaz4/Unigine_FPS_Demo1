/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_panel_brush.h
 * Desc:    Unigine editor
 * Version: 1.09
 * Author:  Mikhail Lyubimov <alterego@unigine.com>
            Andrey Viktorov <unclebob@unigine.com>
 *          Alexander Zapryagaev <frustum@unigine.com>
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

#ifndef __EDITOR_PANEL_BRUSH_H__
#define __EDITOR_PANEL_BRUSH_H__

/*
 */
namespace Editor::Panels {
	
	/**************************************************************************\
	*
	* class PanelBrush
	*
	\**************************************************************************/
	
	/*
	 */
	class PanelBrush : PanelBase {
		
		using Unigine::Widgets;
		using Editor::Widgets;
		
		#define FLOAT_VARIABLE(VARIABLE,NAMESPACE) \
		EditLine VARIABLE ## _el; \
		void set_ ## VARIABLE(float value) { \
			VARIABLE ## _el.setText(editorFormat(value)); \
		} \
		void VARIABLE ## _changed() { \
			NAMESPACE ## ::VARIABLE ## _changed(float(VARIABLE ## _el.getText())); \
		} \
		
		enum {
			SLIDER_MULTIPLIER = 1000,
		};
		
		GroupBox main_gb;
		ComboBox mode_cb;
		Label main_l;
		Icon to_external_i;
		Label hotkey_l;
		
		HBox channels_value_h;
		Button channels_value_b[5];
		HBox brushes_h;
		Icon brushes_i[5];
		Icon shapes_i[4];
		
		Image shape_image;
		Sprite sprite_shape_texture;
		
		GridBox slots_gb;
		Icon slots_i[0];
		Icon slots_s[0];
		
		Icon current_slot_i;
		Sprite current_slot_s;
		
		GroupBox limits_g;
		
		FLOAT_VARIABLE(radius,Brushes)
		FLOAT_VARIABLE(power,Brushes)
		FLOAT_VARIABLE(opacity,Brushes)
		FLOAT_VARIABLE(min_angle,Brushes::Terrain)
		FLOAT_VARIABLE(max_angle,Brushes::Terrain)
		FLOAT_VARIABLE(min_height,Brushes::Terrain)
		FLOAT_VARIABLE(max_height,Brushes::Terrain)
		FLOAT_VARIABLE(heights_value,Brushes::Terrain)
		
		Slider min_height_sl;
		Slider max_height_sl;
		
		Label value_l;
		Slider heights_value_sl;
		GroupBox diffuse_value_gb;
		Sprite diffuse_value_s;
		ComboBox mask_value_cb;
		
		WidgetViewportLayout viewport_layout;
		WidgetViewportLayoutPanel panel;
		
		string mode_heights_text[] = ( TR("Replace"), TR("Add"), TR("Subtract"), TR("Smooth"), TR("Dilate"), TR("Erode") );
		string mode_holes_text[] = ( TR("Add"), TR("Remove"));
		string mode_diffuse_text[] = ( TR("Replace"), TR("Add"), TR("Subtract"), TR("Smooth"), TR("Multiply") );
		string mode_mask_text[] = ( TR("Replace"), TR("Add"), TR("Subtract"), TR("Smooth") );
		string mode_material_text[] = ( TR("Replace"), TR("Add"), TR("Subtract"), TR("Smooth") );
		
		PanelBrush(WidgetViewportLayout layout) {
			viewport_layout = layout;
			panel = viewport_layout.getPanel("panel_brush");
			init("editor/panels/editor_panel_brush.ui","Editor::Panels::PanelBrush::");
			panel.setBorder(1);
			panel.setHidden(1);
			shape_image = new Image();
			update_hotkey();
			forloop(int i = 0; 8) add_slot();
			panel.arrange();
		}
		
		/*
		 */
		~PanelBrush() {
		}
		
		/*
		 */
		void enable(int is_terrain) {
			panel.addWidget(main_gb);
			panel.setHidden(0);
			if(is_terrain) {
				main_l.setText(TR("Terrain Editor"));
				panel.setWindowText(main_l.getText());
				brushes_h.setHidden(false);
				limits_g.setHidden(false);
			} else {
				main_l.setText(TR("Mask Editor"));
				panel.setWindowText(main_l.getText());
				brushes_h.setHidden(true);
				limits_g.setHidden(true);
				set_brush(BRUSH_MASK_CHANNELS);
			}
			panel.addAction("close","core/gui/window_close.png");
			panel.setActionCallback("close",CLICKED,functionid(brush_panel_windowed_toggle_redirector),this);
			
			panel.arrange();
			set_panel_position();
		}
		
		void disable() {
			panel.removeWidget(main_gb);
			panel.setHidden(1);
			panel.arrange();
		}
		
		//TODO: drop this and make autolayout
		void set_panel_position() {
			
			int left = panel.panel_box.getPositionX();
			int top = panel.panel_box.getPositionY();
			int right = panel.panel_box.getPositionX() + panel.panel_box.getWidth();
			int bottom = top + panel.panel_box.getHeight();
			
			int offset = 20;
			
			void update_intersected_panels(int left,int top,int right,int bottom,Editor::Widgets::WidgetViewportLayoutPanel panels[]) {
				panel.layout.getIntersectedPanels(left,top,right,bottom,panels);
				for(int i = panels.size() - 1; i >= 0; i--) {
					if(panels[i] == panel) panels.remove(i);
				}
			}
			
			Editor::Widgets::WidgetViewportLayoutPanel panels[0];
			update_intersected_panels(left,top,right,bottom,panels);
			
			while(panels.size() != 0) {
				
				int min_y = panels[0].panel_box.getPositionY();
				
				for(int i = 1; i < panels.size(); i++) {
					int val = panels[i].panel_box.getPositionY();
					if(min_y > val) min_y = val;
				}
				
				bottom = min_y;
				top = min_y - panel.panel_box.getHeight() - panel.layout.padding_top - offset;
				if(top < 0) return;
				
				update_intersected_panels(left,top,right,bottom,panels);
			}
			
			panel.setPosition(left,top);
		}
		
		//
		void set_brush(int brush) {
			
			forloop(int i = 0; brushes_i.size()) {
				brushes_i[i].setCallbackEnabled(GUI_CLICKED,0);
				brushes_i[i].setToggled(i == brush);
				brushes_i[i].setCallbackEnabled(GUI_CLICKED,1);
			}
			
			mode_cb.clear();
			heights_value_el.setHidden(true);
			heights_value_sl.setHidden(true);
			diffuse_value_gb.setHidden(true);
			mask_value_cb.setHidden(true);
			channels_value_h.setHidden(true);
			value_l.setHidden(false);
			
			update_texture_brush(current_slot_i,current_slot_s,brush);
			
			switch(brush) {
				case BRUSH_HEIGHTS:
					foreach(string text; mode_heights_text) {
						mode_cb.addItem(text);
					}
					heights_value_el.setHidden(false);
					heights_value_sl.setHidden(false);
					value_l.setText("Height:");
					break;
				case BRUSH_HOLES:
					foreach(string text; mode_holes_text) {
						mode_cb.addItem(text);
					}
					value_l.setHidden(true);
					break;
				case BRUSH_DIFFUSE:
					foreach(string text; mode_diffuse_text) {
						mode_cb.addItem(text);
					}
					diffuse_value_gb.setHidden(false);
					value_l.setText("Color:");
					break;
				case BRUSH_MASK:
					foreach(string text; mode_mask_text) {
						mode_cb.addItem(text);
					}
					mask_value_cb.setHidden(false);
					value_l.setText("Material");
					break;
				case BRUSH_MATERIAL:
					foreach(string text; mode_material_text) {
						mode_cb.addItem(text);
					}
					diffuse_value_gb.setHidden(false);
					mask_value_cb.setHidden(false);
					value_l.setText("Color:");
					break;
				case BRUSH_MASK_CHANNELS:
					foreach(string text; mode_mask_text) {
						mode_cb.addItem(text);
					}
					channels_value_h.setHidden(false);
					value_l.setText("Channel:");
					break;
			}
		}
		
		void brush_changed_heights() {
			Brushes::brush_changed(BRUSH_HEIGHTS);
		}
		
		void brush_changed_holes() {
			Brushes::brush_changed(BRUSH_HOLES);
		}
		
		void brush_changed_diffuse() {
			Brushes::brush_changed(BRUSH_DIFFUSE);
		}
		
		void brush_changed_mask() {
			Brushes::brush_changed(BRUSH_MASK);
		}
		
		void brush_changed_material() {
			Brushes::brush_changed(BRUSH_MATERIAL);
		}
		
		//
		void set_shape(int shape) {
			forloop(int i = 0; shapes_i.size()) {
				shapes_i[i].setCallbackEnabled(GUI_CLICKED,0);
				shapes_i[i].setToggled(i == shape);
				shapes_i[i].setCallbackEnabled(GUI_CLICKED,1);
			}
		}
		
		void shape_changed_circle() {
			Brushes::shape_changed(SHAPE_CIRCLE);
		}
		
		void shape_changed_square() {
			Brushes::shape_changed(SHAPE_SQUARE);
		}
		
		void shape_changed_diamond() {
			Brushes::shape_changed(SHAPE_DIAMOND);
		}
		
		void shape_changed_texture() {
			Brushes::shape_changed(SHAPE_TEXTURE);
		}
		
		void shape_texture_changed(string texture) {
			shape_image.load(texture);
			
			if(shape_image.isCombinedFormat()) shape_image.decombine();
			if(shape_image.isCompressedFormat()) shape_image.decompress();
			
			shape_image.resize(24,24);
			sprite_shape_texture.setImage(shape_image);
		}
		
		//
		void set_mode(int mode) {
			mode_cb.setCallbackEnabled(GUI_CHANGED,0);
			mode_cb.setCurrentItem(mode);
			mode_cb.setCallbackEnabled(GUI_CHANGED,1);
		}
		
		void mode_changed() {
			Brushes::mode_changed(mode_cb.getCurrentItem());
		}
		
		//
		void set_diffuse_value(vec3 diffuse_value) {
			diffuse_value_s.setColor(diffuse_value);
			current_slot_s.setColor(diffuse_value);
		}
		
		void diffuse_value_changed(WidgetDialogColor dialog) {
			Brushes::Terrain::diffuse_value_changed(dialog.getColor());
		}
		
		void diffuse_value_clicked() {
			dialogColor(TR("Select diffuse color"),functionid(diffuse_value_changed),Brushes::Terrain::get_diffuse_value());
		}
		
		//
		void update_materials(ObjectTerrain terrain) {
			mask_value_cb.clear();
			if(terrain.getNumMaterials() == 0) {
				mask_value_cb.addItem(TR("no materials"));
			} else {
				forloop(int i = 0; terrain.getNumMaterials()) {
					string name = terrain.getMaterialName(i);
					if(strlen(name) == 0) name = format(TR("Material %d"),i);
					mask_value_cb.addItem(name);
				}
			}
		}
		
		void set_mask_value(int mask_value) {
			mask_value_cb.setCurrentItem(mask_value);
		}
		
		void mask_value_changed() {
			Brushes::Terrain::mask_value_changed(mask_value_cb.getCurrentItem());
		}
		
		//
		void texture_shape_button_clicked() {
			string shape_texture = Brushes::getShapeTexture();
			if(dialogFileImageNoHDR(TR("Import custom brush texture"),shape_texture,DIALOG_FILE_OPEN)) {
				Brushes::shape_texture_changed(shape_texture);
			}
		}
		
		//
		void apply_clicked() {
			Brushes::apply();
		}
		
		void cancel_clicked() {
			if(dialogMessageYesNo(TR("Confirm reset of changes"),"Are you sure to cancel changes?") == false) return;
			Brushes::cancel();
		}
		
		//
		void update_slot(int num,int brush,string tooltip,vec4 color = vec4_one) {
			Icon icon = slots_i[num];
			icon.setToolTip(tooltip);
			Sprite sprite = slots_s[num];
			update_texture_brush(icon,sprite,brush);
		}
		
		void update_slot_color(int num,vec4 color) {
			Sprite sprite = slots_s[num];
			sprite.setColor(color);
		}
		
		void update_texture_brush(Icon icon,Sprite sprite,int brush) {
			sprite.setHidden(brush != BRUSH_DIFFUSE && brush != BRUSH_MATERIAL);
			
			if(brush == BRUSH_HEIGHTS) icon.setTexture("editor/gui/terrain_heights_brush.png");
			else if(brush == BRUSH_HOLES) icon.setTexture("editor/gui/terrain_holes_brush.png");
			else if(brush == BRUSH_DIFFUSE) icon.setTexture("editor/gui/terrain_diffuse_brush.png");
			else if(brush == BRUSH_MASK) icon.setTexture("editor/gui/terrain_mask_brush.png");
			else if(brush == BRUSH_MATERIAL) icon.setTexture("editor/gui/terrain_diffuse_mask_brush.png");
			else if(brush == BRUSH_MASK_CHANNELS) icon.setTexture("editor/gui/terrain_diffuse_brush.png");
			else if(brush == BRUSH_EMPTY) icon.setTexture("editor/gui/node_dummy.png");
		}
		
		int add_slot() {
			VBox vbox = new VBox();
			slots_gb.addChild(vbox,GUI_ALIGN_EXPAND);
			
			Icon slot_i = new Icon();
			string num = string(slots_i.size());
			
			slot_i.setData(num);
			slot_i.setCallback(GUI_CLICKED,functionid(slot_clicked),num);
			slot_i.setCallback(GUI_DRAG_DROP,functionid(slot_drag_drop),num);
			
			vbox.addChild(slot_i,GUI_ALIGN_EXPAND);
			slots_i.append(slot_i);
			
			Sprite sprite = new Sprite("editor/gui/terrain_diffuse_brush_color.png");
			vbox.addChild(sprite,GUI_ALIGN_OVERLAP);
			sprite.setWidth(32);
			sprite.setHeight(32);
			sprite.setColor(vec4_one);
			sprite.setHidden(true);
			slots_s.append(sprite);
		}
		
		void slot_clicked(string num) {
			Brushes::slot_set(int(num));
		}
		
		void slot_drag_drop(string num) {
			Widget widget = widgetCast(getFocus());
			
			if(widget.getData() == "") {
				Brushes::slot_get(int(num));
			} else {
				Brushes::slot_swap(int(widget.getData()),int(num));
			}
			
		}
		
		void current_slot_drag_drop() {
			Widget widget = widgetCast(getFocus());
			int num = int(widget.getData());
			Brushes::slot_set(int(num));
		}
		
		//
		string get_mode_text(int mode,int brush) {
			
			if(brush == BRUSH_HEIGHTS) return mode_heights_text[mode];
			if(brush == BRUSH_HOLES) return mode_holes_text[mode];
			if(brush == BRUSH_DIFFUSE) return mode_diffuse_text[mode];
			if(brush == BRUSH_MASK || brush == BRUSH_MASK_CHANNELS) return mode_mask_text[mode];
			if(brush == BRUSH_MATERIAL) return mode_material_text[mode];
		}
		
		//
		void update_hotkey() {
			string hotkey_text = hotkeysGetText("brushes_apply_brush");
			
			hotkey_l.setText(TR(format("Use %s to paint",hotkey_text)));
		}
		
		//
		void set_channels_toggled(int i,int mode) {
				channels_value_b[i].setCallbackEnabled(GUI_CLICKED,0);
				channels_value_b[i].setToggled(mode);
				channels_value_b[i].setCallbackEnabled(GUI_CLICKED,1);
		}
		
		void set_channels_value(vec4 channels_value) {
			set_channels_toggled(0,channels_value.x > 0.0f);
			set_channels_toggled(1,channels_value.y > 0.0f);
			set_channels_toggled(2,channels_value.z > 0.0f);
			set_channels_toggled(3,channels_value.w > 0.0f);
		}
		
		void channels_value_changed() {
			vec4 channels_value;
			channels_value.x = channels_value_b[0].isToggled() ? 1.0f : 0.0f;
			channels_value.y = channels_value_b[1].isToggled() ? 1.0f : 0.0f;
			channels_value.z = channels_value_b[2].isToggled() ? 1.0f : 0.0f;
			channels_value.w = channels_value_b[3].isToggled() ? 1.0f : 0.0f;
			Brushes::Mask::channels_value_changed(channels_value);
		}
		
		void set_channels_enabled(int channel,int mode) {
			channels_value_b[channel].setEnabled(mode);
		}
		
		//
		void set_limits(float fmin,float fmax) {
			int imax = int(fmax * SLIDER_MULTIPLIER);
			int imin = int(fmin * SLIDER_MULTIPLIER);
			
			min_height_sl.setMinValue(min(min_height_sl.getMinValue(),imin));
			min_height_sl.setMaxValue(max(min_height_sl.getMaxValue(),imax));
			
			max_height_sl.setMinValue(min(max_height_sl.getMinValue(),imin));
			max_height_sl.setMaxValue(max(max_height_sl.getMaxValue(),imax));
		}
		
		// panel ui part
		void brush_panel_move() {
			panel.moveStart();
			panel.movePanel();
		}
		
		void brush_panel_move_end() {
			panel.moveEnd();
		}
		
		void brush_panel_windowed_toggle() {
			if(panel.getWindowed()) {
				main_l.setHidden(0);
				to_external_i.setHidden(0);
			} else {
				main_l.setHidden(1);
				to_external_i.setHidden(1);
			}
			panel.setWindowText(main_l.getText());
			panel.setWindowed(!panel.getWindowed());
		}
		
		void brush_panel_windowed_toggle_redirector(PanelBrush self) {
			self.brush_panel_windowed_toggle();
		}
	};
	
} /* namespace Editor::Panels */

#endif /* __EDITOR_PANEL_BRUSH_H__ */
