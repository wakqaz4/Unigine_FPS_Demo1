/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_panel_placement.h
 * Desc:    Unigine editor
 * Version: 1.00
 * Author:  Vladimir Borovskikh <futurist@unigine.com>
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

#ifndef __EDITOR_PANEL_PLACEMENT_H__
#define __EDITOR_PANEL_PLACEMENT_H__

#include <editor/widgets/editor_widget_viewport_layout.h>
#include <editor/panels/editor_panel_base.h>

/*
 */
namespace Editor::Panels {
	
	/**************************************************************************\
	*
	* class PanelPlacement
	*
	\**************************************************************************/
	
	/*
	 */
	class PanelPlacement : PanelBase {
		
		using Unigine::Widgets;
		using Editor::Widgets;
		
		//
		enum {
			PRECISION = 3,
		};
		
		//
		enum {
			SNAP_SETTINGS_WAIT_TIME = 0.5f,
		};
		
		//
		GroupBox manipulators_gb;
		Icon selector_i;
		Icon translator_i;
		Icon rotator_i;
		Icon scaler_i;
		ComboBox pivot_cb;
		ComboBox basis_cb;
		
		//
		Icon snap_by_grid_i;
		Icon snap_by_angle_i;
		Icon snap_to_surface_i;
		Icon drop_down_i;
		
		Icon axis_x_i;
		Icon axis_y_i;
		Icon axis_z_i;
		
		//
		WidgetViewportLayout viewport_layout;
		
		//
		float hide_time = -1.0f;
		
		//
		WidgetEditorPanel snap_by_grid_panel;
		HBox snap_by_grid_hb;
		Slider snap_by_grid_step_s;
		EditLine snap_by_grid_step_e;
		Label snap_by_grid_l;
		
		//
		WidgetEditorPanel snap_by_angle_panel;
		HBox snap_by_angle_hb;
		Slider snap_by_angle_step_s;
		EditLine snap_by_angle_step_e;
		Label snap_by_angle_l;
		
		//
		WidgetEditorPanel snap_to_surface_panel;
		HBox snap_to_surface_hb;
		Label snap_to_surface_offset_l;
		EditLine snap_to_surface_offset_e;
		Label snap_to_surface_relative_l;
		CheckBox snap_to_surface_relative_c;
		
		//
		WidgetEditorPanel drop_down_panel;
		HBox drop_down_hb;
		Label drop_down_offset_l;
		EditLine drop_down_offset_e;
		Label drop_down_normal_l;
		CheckBox drop_down_normal_c;
		Label drop_down_intersection_l;
		EditLine drop_down_intersection_e;
		Icon drop_down_intersection_i;
		
		// placement by grid
		int position_snap;
		int rotation_snap;
		
		// placement by angle
		float position_step;
		float rotation_step;
		
		// placement to surface
		int snap_to_surface_offset;
		int snap_to_surface_relative_transform;
		
		// drop on the ground
		int drop_down_offset;
		int drop_down_normal_mode;
		int drop_down_intersection_mask;
		
		//
		int manipulator_basis;
		
		int manipulator_pivot;
		
		//
		PanelPlacement(WidgetViewportLayout layout) {
			
			init("editor/panels/editor_panel_placement.ui","Editor::Panels::PanelPlacement::");
			
			viewport_layout = layout;
			viewport_layout.addWidget("panel_placement",manipulators_gb);
			
			// manipulator_basis
			manipulator_basis = configGet("panel_placement_manipulator_basis",PANEL_BASIS_WORLD);
			on_basis_changed();
			
			// pivot
			manipulator_pivot = configGet("panel_placement_manipulator_pivot",MANIPULATOR_OBJECT);
			on_pivot_changed();
			
			// manipulator
			string manipulator = configGet("panel_placement_manipulator","selector");
			manipulator_changed(manipulator);
			
			// snapping
			position_step = configGet("panel_placement_position_step",1.0f);
			position_snap = configGet("panel_placement_position_snap",0) && position_step > EPSILON;
			if(position_step <= EPSILON) position_step = 1.0f;
			
			rotation_step = configGet("panel_placement_rotation_step",1.0f);
			rotation_snap = configGet("panel_placement_rotation_snap",0) && rotation_step > EPSILON;
			if(rotation_step <= EPSILON) rotation_step = 1.0f;
			
			// drop on the ground
			drop_down_intersection_mask = configGet("drop_down_intersection_mask",0x1);
			drop_down_normal_mode = configGet("drop_down_normal_mode",DROP_DOWN_IGNORE_NORMAL);
			drop_down_offset = configGet("drop_down_offset",0);
			
			//
			set_toggle(snap_by_grid_i,position_snap);
			set_toggle(snap_by_angle_i,rotation_snap);
			set_toggle(snap_to_surface_i,creatorGetNode() != NULL);
			
			create_snap_panels();
			
			layout.arrangePanel("panel_placement");
		}
		
		//
		~PanelPlacement() {
			configSet("panel_placement_basis",manipulator_basis);
			configSet("panel_placement_position_step",position_step);
			configSet("panel_placement_position_snap",position_snap);
			configSet("panel_placement_rotation_step",rotation_step);
			configSet("panel_placement_rotation_snap",rotation_snap);
			
			configSet("panel_placement_manipulator",get_pressed_manipulator());
			configSet("panel_placement_manipulator_pivot",getManipulatorPivot());
			configSet("panel_placement_manipulator_basis",getManipulatorBasis());
			
			configSet("drop_down_intersection_mask",drop_down_intersection_mask);
			configSet("drop_down_normal_mode",drop_down_normal_mode);
			configSet("drop_down_offset",drop_down_offset);
			
			delete snap_by_grid_panel;
			delete snap_by_grid_hb;
			delete snap_by_grid_step_e;
			delete snap_by_grid_l;
			
			delete snap_by_angle_panel;
			delete snap_by_angle_hb;
			delete snap_by_angle_step_e;
			delete snap_by_angle_l;
			
			delete snap_to_surface_panel;
			delete snap_to_surface_hb;
			delete snap_to_surface_offset_e;
			delete snap_to_surface_relative_c;
			
			delete drop_down_panel;
			delete drop_down_hb;
			delete drop_down_offset_e;
			delete drop_down_normal_c;
			delete drop_down_intersection_e;
			delete drop_down_intersection_i;
		}
		
		//
		void update(int need_reload) {
			
			manipulators_gb.setEnabled(engine.world.isLoaded());
			
			snap_to_surface_i.setEnabled(nodesGetNode() != NULL);
			drop_down_i.setEnabled(nodesGetNode() != NULL);
			
			if(hide_time != -1.0f || snap_to_surface_i.isToggled()) {
				if(hide_time == -1.0f && snap_to_surface_i.isToggled()) {
					show_snap_panels(snap_to_surface_i);
					hide_time = clock();
				}
				
				if(settings_mouse_over() || settings_input_focused() || snap_to_surface_i.isToggled()) {
					hide_time = clock();
				}
				else if(clock() - hide_time > SNAP_SETTINGS_WAIT_TIME) {
					hide_snap_panels();
					hide_time = -1.0f;
				}
			}
		}
		
		//
		HBox snap_by_grid_settings() {
			if(snap_by_grid_hb == NULL) {
				snap_by_grid_hb = new HBox(5,5);
				snap_by_grid_l = new Label(TR("Step"));
				snap_by_grid_hb.addChild(snap_by_grid_l);
				
				snap_by_grid_step_e = new EditLine();
				snap_by_grid_step_e.setValidator(GUI_VALIDATOR_FLOAT);
				snap_by_grid_step_e.setCallback(CHANGED,functionid(callback_redirector),"changed_callback_grid",this);
				snap_by_grid_hb.addChild(snap_by_grid_step_e);
				
				snap_by_grid_step_s = new Slider(1,1000);
				snap_by_grid_step_s.setCallback(CHANGED,functionid(callback_redirector),"changed_callback_grid",this);
				snap_by_grid_step_s.addAttach(snap_by_grid_step_e,"%.3f",10);
				snap_by_grid_hb.addChild(snap_by_grid_step_s);
			}
			
			snap_by_grid_step_e.setText(string(position_step,PRECISION));
			
			return snap_by_grid_hb;
		}
		
		void changed_callback_grid() {
			float value = Scalar(snap_by_grid_step_e.getText());
			
			if(snap_by_grid_step_e.getText() == "-" || value < 0.0f) set_text(snap_by_grid_step_e,trim(snap_by_grid_step_e.getText(),"-"));
			
			panelPlacementSetPositionStep(abs(value));
			
			if(value == 0.0f) setPositionSnap(false);
		}
		
		//
		HBox snap_by_angle_settings() {
			
			if(snap_by_angle_hb == NULL) {
				snap_by_angle_hb = new HBox(5,5);
				snap_by_angle_l = new Label(TR("Angle"));
				snap_by_angle_hb.addChild(snap_by_angle_l);
				
				snap_by_angle_step_e = new EditLine();
				snap_by_angle_step_e.setValidator(GUI_VALIDATOR_FLOAT);
				snap_by_angle_step_e.setCallback(CHANGED,functionid(callback_redirector),"changed_callback_angle",this);
				snap_by_angle_step_e.setCallback(PRESSED,functionid(callback_redirector),"pressed_callback_angle",this);
				snap_by_angle_hb.addChild(snap_by_angle_step_e);
				
				snap_by_angle_step_s = new Slider(1,1800);
				snap_by_angle_step_s.setCallback(CHANGED,functionid(callback_redirector),"changed_callback_angle",this);
				snap_by_angle_step_s.addAttach(snap_by_angle_step_e,"%.3f",10);
				snap_by_angle_hb.addChild(snap_by_angle_step_s);
			}
			
			snap_by_angle_step_e.setText(string(rotation_step,PRECISION));
			
			return snap_by_angle_hb;
		}
		
		void changed_callback_angle() {
			float value = Scalar(snap_by_angle_step_e.getText());
			
			if(snap_by_angle_step_e.getText() == "-" || value < 0.0f) set_text(snap_by_angle_step_e,trim(snap_by_angle_step_e.getText(),"-"));
			
			panelPlacementSetRotationStep(abs(value));
			
			if(value == 0.0f) setRotationSnap(false);
		}
		
		void pressed_callback_angle() {
			float value = Scalar(snap_by_angle_step_e.getText());
			
			if(value > 180.0f) set_text(snap_by_angle_step_e,string(180.0f,PRECISION));
		}
		
		//
		HBox snap_to_surface_settings() {
			
			if(snap_to_surface_hb == NULL) {
				
				snap_to_surface_hb = new HBox(5,5);
				GridBox content = new GridBox(2,5,5);
				snap_to_surface_hb.addChild(content,ALIGN_LEFT | ALIGN_EXPAND);
				
				snap_to_surface_offset_l = new Label(TR("Offset from surface"));
				snap_to_surface_offset_l.setToolTip("Offset from surface");
				snap_to_surface_offset_e = new EditLine();
				snap_to_surface_offset_e.setToolTip("Offset from surface");
				snap_to_surface_offset_e.setValidator(GUI_VALIDATOR_INT);
				snap_to_surface_offset_e.setCallback(CHANGED,functionid(callback_redirector),"changed_callback_offset",this);
				snap_to_surface_offset_e.setCallback(PRESSED,functionid(callback_redirector),"changed_callback_offset",this);
				content.addChild(snap_to_surface_offset_l,ALIGN_LEFT);
				content.addChild(snap_to_surface_offset_e,ALIGN_LEFT);
				
				snap_to_surface_relative_l = new Label(TR("Orient by normal"));
				snap_to_surface_relative_l.setToolTip("Reset offset: ");
				snap_to_surface_relative_c = new CheckBox();
				snap_to_surface_relative_c.setToolTip("Reset offset: ");
				snap_to_surface_relative_c.setCallback(CHANGED,functionid(callback_redirector),"changed_callback_relative",this);
				content.addChild(snap_to_surface_relative_l,ALIGN_LEFT);
				content.addChild(snap_to_surface_relative_c,ALIGN_LEFT);
			}
			// update tooltip
			string tooltip = format("Increase (%s), ",hotkeysGetText("place_node_increase_offset"));
			tooltip += format(TR("Decrease (%s), "),hotkeysGetText("place_node_decrease_offset"));
			tooltip += format(TR("Reset (%s)"),hotkeysGetText("place_node_offset_reset"));
			snap_to_surface_offset_l.setToolTip(tooltip);
			snap_to_surface_offset_e.setToolTip(tooltip);
			
			tooltip = format(TR("Toggle orient by normal (%s)"),hotkeysGetText("place_node_relative_transform"));
			snap_to_surface_relative_l.setToolTip(tooltip);
			snap_to_surface_relative_c.setToolTip(tooltip);
			
			// update data
			snap_to_surface_offset_e.setText(string(snap_to_surface_offset));
			
			snap_to_surface_relative_c.setCallbackEnabled(CHANGED,0);
			snap_to_surface_relative_c.setChecked(snap_to_surface_relative_transform);
			snap_to_surface_relative_c.setCallbackEnabled(CHANGED,1);
			
			return snap_to_surface_hb;
		}
		
		void changed_callback_offset() {
			snap_to_surface_offset = Scalar(snap_to_surface_offset_e.getText());
		}
		
		void changed_callback_relative() {
			snap_to_surface_relative_transform = snap_to_surface_relative_c.isChecked();
		}
		
		//
		HBox drop_down_settings() {
			
			if(drop_down_hb == NULL) {
				
				drop_down_hb = new HBox(5,5);
				GridBox content = new GridBox(3,5,5);
				drop_down_hb.addChild(content,ALIGN_LEFT | ALIGN_EXPAND);
				
				// offset from surface
				drop_down_offset_l = new Label(TR("Offset from surface"));
				drop_down_offset_l.setToolTip("The offset from the node's pivot point to the surface (in units)");
				content.addChild(drop_down_offset_l,ALIGN_LEFT);
				
				drop_down_offset_e = new EditLine();
				drop_down_offset_e.setWidth(80);
				drop_down_offset_e.setValidator(GUI_VALIDATOR_INT);
				drop_down_offset_e.setToolTip("The offset from the node's pivot point to the surface (in units)");
				drop_down_offset_e.setCallback(CHANGED,functionid(callback_redirector),"drop_down_offset_changed",this);
				drop_down_offset_e.setCallback(PRESSED,functionid(callback_redirector),"drop_down_offset_changed",this);
				content.addChild(drop_down_offset_e,ALIGN_LEFT);
				
				content.addChild(new HBox(1,1),ALIGN_LEFT);
				
				// normal orientation mode
				drop_down_normal_l = new Label(TR("Orient by normal"));
				drop_down_normal_l.setToolTip("Orient by the surface normal");
				content.addChild(drop_down_normal_l,ALIGN_LEFT);
				
				drop_down_normal_c = new CheckBox();
				drop_down_normal_c.setToolTip("Orient by the surface normal");
				drop_down_normal_c.setCallback(CHANGED,functionid(callback_redirector),"drop_down_normal_changed",this);
				content.addChild(drop_down_normal_c,ALIGN_LEFT);
				
				content.addChild(new HBox(1,1),ALIGN_LEFT);
				
				// intersection mask
				drop_down_intersection_l = new Label(TR("Intersection mask"));
				drop_down_intersection_l.setToolTip(TR("Intersection mask of objects that will be dropped"));
				content.addChild(drop_down_intersection_l,ALIGN_LEFT);
				
				drop_down_intersection_e = new EditLine();
				drop_down_intersection_e.setWidth(80);
				drop_down_intersection_e.setCapacity(8);
				drop_down_intersection_e.setFont("core/gui/console.ttf");
				drop_down_intersection_e.setToolTip(TR("Intersection mask"));
				drop_down_intersection_e.setCallback(PRESSED,functionid(callback_redirector),"drop_down_intersection_pressed",this);
				drop_down_intersection_e.setCallback(FOCUS_OUT,functionid(callback_redirector),"drop_down_intersection_pressed",this);
				drop_down_intersection_e.setCallback(DOUBLE_CLICKED,functionid(callback_redirector),"drop_down_intersection_clicked",this);
				content.addChild(drop_down_intersection_e,ALIGN_LEFT);
				
				drop_down_intersection_i = new Icon("editor/gui/action_edit.png");
				drop_down_intersection_i.setWidth(24);
				drop_down_intersection_i.setHeight(24);
				drop_down_intersection_i.setToolTip(TR("Edit mask"));
				drop_down_intersection_i.setCallback(CLICKED,functionid(callback_redirector),"drop_down_intersection_clicked",this);
				content.addChild(drop_down_intersection_i,ALIGN_LEFT);
			}
			
			drop_down_offset_e.setText(string(drop_down_offset));
			
			drop_down_normal_c.setCallbackEnabled(CHANGED,0);
			drop_down_normal_c.setChecked(drop_down_normal_mode);
			drop_down_normal_c.setCallbackEnabled(CHANGED,1);
			
			drop_down_intersection_e.setText(editorFormatMask(drop_down_intersection_mask));
			
			return drop_down_hb;
		}
		
		void drop_down_offset_changed() {
			drop_down_offset = Scalar(drop_down_offset_e.getText());
		}
		
		void drop_down_normal_changed() {
			drop_down_normal_mode = drop_down_normal_c.isChecked();
		}
		
		void drop_down_intersection_pressed() {
			drop_down_intersection_mask = int("0x" + drop_down_intersection_e.getText());
			drop_down_intersection_e.setText(editorFormatMask(drop_down_intersection_mask));
			
			if(toolsGetSelection() && drop_down_intersection_e.isFocused()) {
				drop_down_intersection_e.setCursor(0);
				drop_down_intersection_e.setSelection(1024);
			}
		}
		
		void drop_down_intersection_clicked() {
			hide_snap_panels();
			dialogMask(TR("Intersection mask"),functionid(drop_down_intersection_changed),drop_down_intersection_mask,this);
		}
		
		void drop_down_intersection_changed(int mask) {
			drop_down_intersection_mask = mask;
			drop_down_intersection_e.setText(editorFormatMask(drop_down_intersection_mask));
		}
		
		//
		int settings_input_focused() {
			if(snap_by_grid_step_e != NULL && snap_by_grid_step_e.isFocused()) return true;
			if(snap_by_grid_step_s != NULL && snap_by_grid_step_s.isFocused()) return true;
			if(snap_by_angle_step_e != NULL && snap_by_angle_step_e.isFocused()) return true;
			if(snap_by_angle_step_s != NULL && snap_by_angle_step_s.isFocused()) return true;
			if(snap_to_surface_offset_e != NULL && snap_to_surface_offset_e.isFocused()) return true;
			if(snap_to_surface_relative_c != NULL && snap_to_surface_relative_c.isFocused()) return true;
			if(drop_down_offset_e != NULL && drop_down_offset_e.isFocused()) return true;
			if(drop_down_normal_c != NULL && drop_down_normal_c.isFocused()) return true;
			if(drop_down_intersection_e != NULL && drop_down_intersection_e.isFocused()) return true;
			return false;
		}
		
		//
		int settings_mouse_over() {
			return (!snap_by_grid_panel.isHidden() && snap_by_grid_panel.isMouseOver()) ||
				(!snap_by_angle_panel.isHidden() && snap_by_angle_panel.isMouseOver()) ||
				(!snap_to_surface_panel.isHidden() && snap_to_surface_panel.isMouseOver()) ||
				(!drop_down_panel.isHidden() && drop_down_panel.isMouseOver());
		}
		
		//
		void create_snap_panels() {
			snap_by_grid_panel = new WidgetEditorPanel(snap_by_grid_i,ANCHOR_BOTTOM);
			snap_by_grid_panel.setBackground(1);
			snap_by_grid_panel.addChild(snap_by_grid_settings());
			snap_by_grid_panel.arrange();
			snap_by_grid_panel.setHidden(1);
			
			snap_by_angle_panel = new WidgetEditorPanel(snap_by_angle_i,ANCHOR_BOTTOM);
			snap_by_angle_panel.setBackground(1);
			snap_by_angle_panel.addChild(snap_by_angle_settings());
			snap_by_angle_panel.arrange();
			snap_by_angle_panel.setHidden(1);
			
			snap_to_surface_panel = new WidgetEditorPanel(snap_to_surface_i,ANCHOR_BOTTOM);
			snap_to_surface_panel.setBackground(1);
			snap_to_surface_panel.addChild(snap_to_surface_settings());
			snap_to_surface_panel.arrange();
			snap_to_surface_panel.setHidden(1);
			
			drop_down_panel = new WidgetEditorPanel(drop_down_i,ANCHOR_BOTTOM);
			drop_down_panel.setBackground(1);
			drop_down_panel.addChild(drop_down_settings());
			drop_down_panel.arrange();
			drop_down_panel.setHidden(1);
		}
		
		//
		void show_snap_panels(Icon icon) {
			
			#ifdef HAS_INTERFACE
			if(viewportsGetFocusedWindow() != NULL && getIntersectionWindow() != NULL) return;
			#endif
			
			void show_panel(WidgetEditorPanel panel) {
				#ifdef HAS_INTERFACE
				InterfaceWindow focused_window = findFocusedWindow();
				#endif
				
				hide_snap_panels();
				panel.setHidden(0);
				
				#ifdef HAS_INTERFACE
				if(focused_window != NULL) focused_window.setFocus();
				#endif
			}
			
			
			hide_time = -1.0f;
			
			if(icon == snap_by_grid_i && icon.isToggled()) {
				show_panel(snap_by_grid_panel);
			} else if(icon == snap_by_angle_i && icon.isToggled()) {
				show_panel(snap_by_angle_panel);
			} else if(icon == snap_to_surface_i) {
				show_panel(snap_to_surface_panel);
			} else if(icon == drop_down_i) {
				show_panel(drop_down_panel);
			}
		}
		
		//
		void hide_snap_panels() {
			snap_by_grid_panel.setHidden(1);
			snap_by_angle_panel.setHidden(1);
			snap_to_surface_panel.setHidden(1);
			drop_down_panel.setHidden(1);
		}
		
		//
		void show_setting_snap_by_grid() {
			show_snap_panels(snap_by_grid_i);
		}
		
		//
		void show_setting_snap_by_angle() {
			show_snap_panels(snap_by_angle_i);
		}
		
		//
		void show_setting_snap_to_surface() {
			show_snap_panels(snap_to_surface_i);
		}
		
		//
		void show_setting_drop_down() {
			show_snap_panels(drop_down_i);
		}
		
		//
		void on_leave() {
			hide_time = clock();
		}
		
		//
		void snap_by_grid_clicked() {
			panelPlacementTogglePositionSnap();
			show_setting_snap_by_grid();
		}
		
		//
		void snap_by_angle_clicked() {
			panelPlacementToggleRotationSnap();
			show_setting_snap_by_angle();
		}
		
		//
		void snap_to_surface_clicked() {
			set_toggle(snap_to_surface_i,nodesPlaceManually());
			show_setting_snap_to_surface();
		}
		
		//
		void drop_down_clicked() {
			dropDown();
		}
		
		//
		void axis_lock_clicked() {
			int lock_state = 0;
			if(axis_x_i.isToggled()) lock_state |= MANIPULATOR_MASK_X;
			if(axis_y_i.isToggled()) lock_state |= MANIPULATOR_MASK_Y;
			if(axis_z_i.isToggled()) lock_state |= MANIPULATOR_MASK_Z;
			if(lock_state == 0) lock_state = MANIPULATOR_MASK_XYZ;
			nodesSetManipulatorMask(lock_state);
		}
		
		void set_axis_lock(string axis,int value) {
			if(axis == "X") axis_x_i.setToggled(value);
			else if(axis == "Y") axis_y_i.setToggled(value);
			else if(axis == "Z") axis_z_i.setToggled(value);
		}
		
		void on_basis_changed() {
			basis_cb.setCurrentItem(manipulator_basis);
		}
		
		void manipulator_basis_changed() {
			manipulator_basis = basis_cb.getCurrentItem();
		}
		
		void on_pivot_changed() {
			pivot_cb.setCurrentItem(manipulator_pivot);
		}
		
		void manipulator_pivot_changed() {
			manipulator_pivot = pivot_cb.getCurrentItem();
		}
		
		void set_toggle(Icon icon,int toggle) {
			if(icon == NULL) return;
			icon.setCallbackEnabled(CHANGED,0);
			icon.setToggled(toggle);
			icon.setCallbackEnabled(CHANGED,1);
		}
		
		void set_text(EditLine line,string text) {
			if(line == NULL) return;
			line.setCallbackEnabled(CHANGED,0);
			int cursor_pos = line.getCursor();
			line.setText(text);
			line.setCursor((cursor_pos - 1 >= 0) ? cursor_pos - 1 : 0);
			line.setCallbackEnabled(CHANGED,1);
		}
		
		string get_pressed_manipulator() {
			if(selector_i != NULL && selector_i.isToggled()) return "selector";
			if(translator_i != NULL && translator_i.isToggled()) return "translator";
			if(rotator_i != NULL && rotator_i.isToggled()) return "rotator";
			if(scaler_i != NULL && scaler_i.isToggled()) return "scaler";
			return "";
		}
		
		//
		void manipulator_changed_selector() {
			if(selector_i != NULL) set_toggle(selector_i,1);
			if(translator_i != NULL) set_toggle(translator_i,0);
			if(rotator_i != NULL) set_toggle(rotator_i,0);
			if(scaler_i != NULL) set_toggle(scaler_i,0);
		}
		
		void manipulator_changed_translator() {
			
			if(!translator_i.isEnabled()) return;
			
			if(selector_i != NULL) set_toggle(selector_i,0);
			if(translator_i != NULL) set_toggle(translator_i,1);
			if(rotator_i != NULL) set_toggle(rotator_i,0);
			if(scaler_i != NULL) set_toggle(scaler_i,0);
		}
		
		void manipulator_changed_rotator() {
			
			if(!rotator_i.isEnabled()) return;
			
			if(selector_i != NULL) set_toggle(selector_i,0);
			if(translator_i != NULL) set_toggle(translator_i,0);
			if(rotator_i != NULL) set_toggle(rotator_i,1);
			if(scaler_i != NULL) set_toggle(scaler_i,0);
		}
		
		void manipulator_changed_scaler() {
			
			if(!scaler_i.isEnabled()) return;
			
			if(selector_i != NULL) set_toggle(selector_i,0);
			if(translator_i != NULL) set_toggle(translator_i,0);
			if(rotator_i != NULL) set_toggle(rotator_i,0);
			if(scaler_i != NULL) set_toggle(scaler_i,1);
		}
		
		void manipulator_changed(string manipulator) {
			if(manipulator == "selector") manipulator_changed_selector();
			else if(manipulator == "translator") manipulator_changed_translator();
			else if(manipulator == "rotator") manipulator_changed_rotator();
			else if(manipulator == "scaler") manipulator_changed_scaler();
		}
		
		//
		void setManipulator(string name) {
			manipulator_changed(name);
		}
		
		int getSelector() {
			if(selector_i == NULL) return 0;
			return selector_i.isToggled();
		}
		
		int getTranslator() {
			if(PanelPlacement::translator_i == NULL) return 0;
			return translator_i.isToggled();
		}
		
		int getRotator() {
			if(rotator_i == NULL) return 0;
			return rotator_i.isToggled();
		}
		
		int getScaler() {
			if(scaler_i == NULL) return 0;
			return scaler_i.isToggled();
		}
		
		int getManipulatorBasis() {
			return manipulator_basis;
		}
		
		void setTranslatorEnabled(int mode) {
			if(translator_i == NULL) return;
			if(mode == 0 && translator_i.isToggled()) manipulator_changed("selector");
			translator_i.setEnabled(mode);
		}
		
		void setRotatorEnabled(int mode) {
			if(rotator_i == NULL) return;
			if(mode == 0 && rotator_i.isToggled()) manipulator_changed("selector");
			rotator_i.setEnabled(mode);
		}
		
		void setScalerEnabled(int mode) {
			if(scaler_i == NULL) return;
			if(mode == 0 && scaler_i.isToggled()) manipulator_changed("selector");
			scaler_i.setEnabled(mode);
		}
		
		void setSnapToSurface(int mode) {
			set_toggle(snap_to_surface_i,mode);
		}
		
		int getSnapToSurfaceOffset() {
			return snap_to_surface_offset;
		}
		
		void setSnapToSurfaceOffset(int offset) {
			snap_to_surface_offset = offset;
			snap_to_surface_offset_e.setText(string(snap_to_surface_offset));
		}
		
		int getSnapToSurfaceRelativeTransform() {
			return snap_to_surface_relative_transform;
		}
		
		void setSnapToSurfaceRelativeTransform(int value) {
			snap_to_surface_relative_transform = value;
			snap_to_surface_relative_c.setCallbackEnabled(CHANGED,0);
			snap_to_surface_relative_c.setChecked(snap_to_surface_relative_transform);
			snap_to_surface_relative_c.setCallbackEnabled(CHANGED,1);
		}
		
		void setPositionStep(float step) {
			if(position_step < 0.0f) position_step = 0.0f;
			else position_step = step;
		}
		
		float getPositionStep() {
			if (position_snap == 0) return EPSILON;
			return position_step;
		}
		
		int getPositionSnap() {
			return position_snap;
		}
		
		void setPositionSnap(int mode) {
			position_snap = mode;
			set_toggle(snap_by_grid_i,mode);
		}
		
		void togglePositionSnap() {
			position_snap = !position_snap;
			setPositionSnap(position_snap);
		}
		
		void setRotationStep(float step) {
			if(rotation_step < 0.0f) rotation_step = 0.0f;
			else rotation_step = step;
		}
		
		float getRotationStep() {
			if (rotation_snap == 0) return EPSILON;
			return rotation_step;
		}
		
		int getRotationSnap() {
			return rotation_snap;
		}
		
		void setRotationSnap(int mode) {
			rotation_snap = mode;
			set_toggle(snap_by_angle_i,mode);
		}
		
		void toggleRotationSnap() {
			rotation_snap = !rotation_snap;
			setRotationSnap(rotation_snap);
		}
		
		int getManipulatorPivot() {
			return manipulator_pivot;
		}
		
		void toggleManipulatorPivot() {
			manipulator_pivot = !manipulator_pivot;
			on_pivot_changed();
		}
		
		void toggleManipulatorBasis() {
			manipulator_basis++;
			if(manipulator_basis == PANEL_NUM_BASISES) manipulator_basis = PANEL_BASIS_WORLD;
			on_basis_changed();
		}
		
		void dropDown() {
			nodesDropDown(drop_down_intersection_mask,drop_down_normal_mode,drop_down_offset);
		}
	};
	
} /* namespace Editor::Panels */

#endif /* __EDITOR_PANEL_PLACEMENT_H__ */
