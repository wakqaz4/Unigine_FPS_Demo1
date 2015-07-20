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


#ifndef __UNIGINE_TRACKER_EDITOR_H__
#define __UNIGINE_TRACKER_EDITOR_H__

#include <core/systems/widgets/widget.h>
#include <core/systems/widgets/widget_hpaned.h>
#include <core/systems/widgets/widget_vpaned.h>
#include <core/systems/widgets/widget_treebox.h>
#include <core/systems/widgets/widget_table.h>
#include <core/systems/widgets/widget_track.h>
#include <core/systems/widgets/widget_time.h>
#include <core/systems/widgets/widget_line_track.h>
#include <core/systems/widgets/widget_line_color.h>
#include <core/systems/widgets/widget_line_environment.h>
#include <core/systems/widgets/widget_line_toggle.h>
#include <core/systems/widgets/widget_line_switch.h>
#include <core/systems/widgets/widget_line_file.h>
#include <core/systems/widgets/widget_line_material.h>
#include <core/systems/widgets/widget_line_property.h>
#include <core/systems/widgets/widget_line_node.h>

#include <core/systems/tracker/tracker.h>
#include <core/systems/tracker/editor/tracker_editor_parameter.h>
#include <core/systems/tracker/editor/tracker_editor_parameter_track.h>
#include <core/systems/tracker/editor/tracker_editor_parameter_float.h>
#include <core/systems/tracker/editor/tracker_editor_parameter_vec3.h>
#include <core/systems/tracker/editor/tracker_editor_parameter_color.h>
#include <core/systems/tracker/editor/tracker_editor_parameter_environment.h>
#include <core/systems/tracker/editor/tracker_editor_parameter_toggle.h>
#include <core/systems/tracker/editor/tracker_editor_parameter_switch.h>
#include <core/systems/tracker/editor/tracker_editor_parameter_file.h>
#include <core/systems/tracker/editor/tracker_editor_parameter_material.h>
#include <core/systems/tracker/editor/tracker_editor_parameter_property.h>
#include <core/systems/tracker/editor/tracker_editor_parameter_node.h>
#include <core/systems/tracker/editor/tracker_editor_dialog_node.h>
#include <core/systems/tracker/editor/tracker_editor_dialog_parameter.h>
#include <core/systems/tracker/editor/tracker_editor_dialog_settings.h>

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	enum {
		
		EDITOR_STATE_STOP = 0,			// editor state top
		EDITOR_STATE_PLAY,				// editor state play
		EDITOR_STATE_LOOP,				// editor state loop
		
		EDITOR_PARAMETER_WIDTH = 256,	// parameter width
	};
	
	/*
	 */
	string editor_track_names[] = ( TR("Continuous"), TR("Discrete") );
	string editor_track_types[] = ( "track", "trackDiscrete" );
	string editor_node_position_names[] = ( TR("Inconst velocity"), TR("Inconst velocity, follow X"), TR("Inconst velocity, follow Y"), TR("Inconst velocity, follow Z"), TR("Const velocity"), TR("Const velocity, follow X"), TR("Const velocity, follow Y"), TR("Const velocity, follow Z") );
	string editor_node_position_types[] = ( "node.position", "node.positionX", "node.positionY", "node.positionZ", "node.positionLinear", "node.positionLinearX", "node.positionLinearY", "node.positionLinearZ" );
	string editor_node_rotation_names[] = ( TR("Inconst velocity, order XYZ"), TR("Inconst velocity, order ZYX"), TR("Const velocity, order XYZ"), TR("Const velocity, order ZYX") );
	string editor_node_rotation_types[] = ( "node.rotationXYZ", "node.rotationZYX", "node.rotationLinearXYZ", "node.rotationLinearZYX" );
	
	/*
	 */
	vec4 editor_scalar_colors[] = (
		vec4(0.678f,0.827f,0.678f,1.0f), vec4(0.831f,0.902f,0.592f,1.0f),
		vec4(0.867f,0.678f,0.627f,1.0f), vec4(0.914f,0.588f,0.718f,1.0f),
		vec4(0.961f,0.773f,0.914f,1.0f), vec4(0.894f,0.886f,1.000f,1.0f),
		vec4(0.553f,0.949f,0.851f,1.0f), vec4(0.525f,0.678f,1.000f,1.0f),
		vec4(0.867f,0.867f,0.867f,1.0f), vec4(0.541f,0.961f,0.553f,1.0f),
		vec4(0.745f,1.000f,0.875f,1.0f), vec4(0.710f,1.000f,0.498f,1.0f),
		vec4(0.929f,1.000f,0.925f,1.0f), vec4(1.000f,1.000f,1.000f,1.0f),
	);
	
	vec4 editor_vector_colors[] = (
		vec4(0.929f,1.000f,0.925f,1.0f), vec4(0.894f,0.886f,1.000f,1.0f),
		vec4(0.867f,0.867f,0.867f,1.0f), vec4(0.745f,1.000f,0.875f,1.0f),
	);
	
	/**************************************************************************\
	*
	* TrackerEditor
	*
	\**************************************************************************/
	
	/*
	 */
	class TrackerEditor {
		
		using Unigine::Widgets;
		
		Tracker tracker;				// tracker
		TrackerTrack tracker_track;		// tracker track
		
		Blob blob;						// blob
		TrackerTrack buffer_track;		// buffer track
		
		VBox main_vbox;					// main vbox
		HPaned main_hp;					// main hpaned
		
		VPaned left_vp;					// left vpaned
		VPaned right_vp;				// right vpaned
		
		VBox left_vbox;					// left vbox
		VBox parameter_vbox;			// parameter vbox
		GridBox parameter_gb;			// parameter gridbox
		
		VBox right_vbox;				// right vbox
		VBox grid_vbox;					// grid vbox
		ScrollBox grid_sb;				// grid scrollbox
		
		Label object_type_l[2];			// object type labels
		Label object_id_l[2];			// object identifier labels
		Label object_name_l;			// object name label
		EditLine object_name_el;		// object name editline
		Label parameter_type_l;			// parameter type label
		ComboBox parameter_type_cb;		// parameter type combobox
		Label argument_value_l;			// argument value label
		EditLine argument_value_el;		// argument value editline
		ComboBox argument_value_cb;		// argument value combobox
		
		VBox line_vbox;					// line vbox
		HBox line_hbox;					// line hbox
		Icon line_add_i;				// line add icon
		Icon line_remove_i;				// line remove icon
		Icon line_clone_i;				// line clone icon
		Icon line_up_i;					// line up icon
		Icon line_down_i;				// line down icon
		Icon line_save_i;				// line save icon
		
		VBox time_vbox;					// time vbox
		HBox time_hbox;					// time hbox
		EditLine time_range_from_el;	// time range from editline
		EditLine time_range_to_el;		// time range to editline
		EditLine time_el;				// time editline
		Icon time_settings_i;			// time settings icon
		Icon time_prev_i;				// time prev icon
		Icon time_play_i;				// time play icon
		Icon time_loop_i;				// time loop icon
		Icon time_next_i;				// time next icon
		
		Table table_w;					// table widget
		Track track_w;					// track widget
		Line line_w;					// line widget
		Time time_w;					// time widget
		Time grid_w;					// grid widget
		
		Icon track_remove_i;			// track remove icon
		EditLine track_time_el;			// track time editline
		EditLine track_value_el;		// track value editline
		
		int state;						// current state
		
		TrackerEditorDialogSettings settings_dialog;	// settings dialog
		TrackerEditorDialogParameter parameter_dialog;	// parameter dialog
		TrackerEditorDialogNode node_dialog;			// node dialog
		
		TrackerEditorParameter parameters[0];			// parameters
		TrackerEditorParameter current_parameter;		// current parameter
		
		/*
		 */
		TrackerEditor(Tracker t) {
			
			tracker = t;
			tracker_track = new TrackerTrack(tracker);
			
			float min_time = tracker_track.getMinTime();
			float max_time = tracker_track.getMaxTime();
			
			// main panel
			main_vbox = new VBox();
			main_hp = new HPaned();
			left_vp = new VPaned();
			right_vp = new VPaned();
			main_hp.setValue(-32767);
			left_vp.setValue(-32767);
			right_vp.setValue(-32767);
			main_hp.setFixed(1);
			left_vp.setFixed(2);
			right_vp.setFixed(2);
			main_vbox.addChild(main_hp,ALIGN_EXPAND);
			main_hp.addChild(left_vp,ALIGN_EXPAND);
			main_hp.addChild(right_vp,ALIGN_EXPAND);
			
			// left vbox
			left_vbox = new VBox(0,0);
			parameter_vbox = new VBox(4,4);
			parameter_gb = new GridBox(2,4,4);
			parameter_gb.setWidth(EDITOR_PARAMETER_WIDTH);
			left_vbox.addChild(parameter_vbox);
			left_vp.addChild(left_vbox,ALIGN_EXPAND);
			parameter_vbox.addChild(parameter_gb,ALIGN_EXPAND);
			
			// right vbox
			right_vbox = new VBox(0,0);
			grid_vbox = new VBox();
			grid_sb = new ScrollBox();
			grid_sb.setVScrollHidden(3);
			grid_sb.setHScrollHidden(3);
			grid_sb.setHeight(TIME_HEIGHT);
			grid_vbox.addChild(grid_sb,ALIGN_EXPAND);
			right_vp.addChild(right_vbox,ALIGN_EXPAND);
			
			// object parameters
			object_type_l[0] = new Label(TR("Type:"));
			object_type_l[1] = new Label();
			object_id_l[0] = new Label(TR("ID:"));
			object_id_l[1] = new Label(TR("Unknown"));
			object_name_l = new Label(TR("Name:"));
			object_name_el = new EditLine();
			parameter_type_l = new Label(TR("Type:"));
			parameter_type_cb = new ComboBox();
			argument_value_l = new Label();
			argument_value_el = new EditLine();
			argument_value_cb = new ComboBox();
			object_name_el.setCallback(PRESSED,functionid(object_name_pressed),this);
			object_name_el.setCallback(DOUBLE_CLICKED,functionid(object_name_double_clicked),this);
			parameter_type_cb.setCallback(CHANGED,functionid(callback_redirector),this,"parameter_type_changed");
			argument_value_el.setCallback(PRESSED,functionid(callback_redirector),this,"argument_value_pressed");
			argument_value_cb.setCallback(CHANGED,functionid(callback_redirector),this,"argument_value_changed");
			parameter_gb.addChild(object_type_l[0],ALIGN_RIGHT);
			parameter_gb.addChild(object_type_l[1],ALIGN_EXPAND);
			parameter_gb.addChild(object_id_l[0],ALIGN_RIGHT);
			parameter_gb.addChild(object_id_l[1],ALIGN_EXPAND);
			parameter_gb.addChild(object_name_l,ALIGN_RIGHT);
			parameter_gb.addChild(object_name_el,ALIGN_EXPAND);
			parameter_gb.addChild(parameter_type_l,ALIGN_RIGHT);
			parameter_gb.addChild(parameter_type_cb,ALIGN_EXPAND);
			parameter_gb.addChild(argument_value_l,ALIGN_RIGHT);
			parameter_gb.addChild(argument_value_el,ALIGN_EXPAND);
			parameter_gb.addChild(argument_value_cb,ALIGN_EXPAND);
			
			// line icons
			line_vbox = new VBox();
			line_hbox = new HBox();
			left_vbox.addChild(line_vbox,ALIGN_BOTTOM);
			line_vbox.addChild(line_hbox,ALIGN_EXPAND);
			line_add_i = new Icon("core/systems/tracker/editor/images/tracker_line_add.png");
			line_remove_i = new Icon("core/systems/tracker/editor/images/tracker_line_remove.png");
			line_clone_i = new Icon("core/systems/tracker/editor/images/tracker_line_clone.png");
			line_up_i = new Icon("core/systems/tracker/editor/images/tracker_line_up.png");
			line_down_i = new Icon("core/systems/tracker/editor/images/tracker_line_down.png");
			line_save_i = new Icon("core/systems/tracker/editor/images/tracker_line_save.png");
			line_add_i.setCallback(CLICKED,functionid(line_add_clicked),this);
			line_remove_i.setCallback(CLICKED,functionid(line_remove_clicked),this);
			line_clone_i.setCallback(CLICKED,functionid(callback_redirector),this,"line_clone_clicked");
			line_up_i.setCallback(CLICKED,functionid(callback_redirector),this,"line_up_clicked");
			line_down_i.setCallback(CLICKED,functionid(callback_redirector),this,"line_down_clicked");
			line_save_i.setCallback(CLICKED,functionid(callback_redirector),this,"line_save_clicked");
			line_hbox.addChild(line_add_i,ALIGN_LEFT);
			line_hbox.addChild(line_remove_i,ALIGN_LEFT);
			line_hbox.addChild(line_clone_i,ALIGN_LEFT);
			line_hbox.addChild(line_up_i,ALIGN_LEFT);
			line_hbox.addChild(line_down_i,ALIGN_LEFT);
			line_hbox.addChild(line_save_i,ALIGN_RIGHT);
			
			// table
			table_w = new Table(("E","T"));
			ScrollBox scrollbox = table_w.getScrollBox();
			table_w.setCallback(TABLE_ROW_CHANGED,functionid(callback_redirector),this,"table_row_changed");
			table_w.setCallback(TABLE_COLUMN_CHANGED,functionid(callback_redirector),this,"table_column_changed");
			left_vp.addChild(table_w,ALIGN_EXPAND);
			
			// track
			track_w = new Track(min_time,max_time);
			scrollbox = track_w.getScrollBox();
			scrollbox.setHScrollHidden(3);
			track_w.setCallback(TRACK_KEY_CREATED,functionid(callback_redirector),this,"track_key_created");
			track_w.setCallback(TRACK_KEY_CHANGED,functionid(callback_redirector),this,"track_key_changed");
			track_w.setCallback(TRACK_KEY_REMOVED,functionid(callback_redirector),this,"track_key_removed");
			right_vbox.addChild(track_w,ALIGN_EXPAND);
			
			// lines
			line_w = new Line(min_time,max_time);
			scrollbox = line_w.getScrollBox();
			scrollbox.setVScrollHidden(3);
			line_w.setCallback(LINE_KEY_CREATED,functionid(callback_redirector),this,"line_key_created");
			line_w.setCallback(LINE_KEY_CHANGED,functionid(callback_redirector),this,"line_key_changed");
			line_w.setCallback(LINE_KEY_REMOVED,functionid(callback_redirector),this,"line_key_removed");
			right_vp.addChild(line_w,ALIGN_EXPAND);
			
			// time
			time_w = new Time(min_time,max_time);
			time_w.setCallback(TIME_CHANGED,functionid(callback_redirector),this,"time_changed");
			time_w.setCallback(TIME_RANGE_FROM_CHANGED,functionid(callback_redirector),this,"time_range_from_changed");
			time_w.setCallback(TIME_RANGE_TO_CHANGED,functionid(callback_redirector),this,"time_range_to_changed");
			
			// grid
			grid_w = new Time(min_time,max_time);
			grid_w.setCallback(TIME_CHANGED,functionid(callback_redirector),this,"grid_changed");
			grid_w.setCallback(TIME_RANGE_FROM_CHANGED,functionid(callback_redirector),this,"grid_range_from_changed");
			grid_w.setCallback(TIME_RANGE_TO_CHANGED,functionid(callback_redirector),this,"grid_range_to_changed");
			grid_sb.addChild(grid_w,ALIGN_EXPAND);
			right_vbox.addChild(grid_vbox);
			
			// track widgets
			track_remove_i = track_w.getRemoveIcon();
			track_time_el = track_w.getTimeEditLine();
			track_value_el = track_w.getValueEditLine();
			track_remove_i.setCallback(CLICKED,functionid(track_remove_clicked),this);
			track_time_el.setCallback(PRESSED,functionid(callback_redirector),this,"track_time_pressed");
			track_value_el.setCallback(PRESSED,functionid(callback_redirector),this,"track_value_pressed");
			
			// track height
			track_w.arrange();
			grid_vbox.arrange();
			left_vbox.setHeight(track_w.getHeight() + grid_vbox.getHeight());
			
			// time widgets
			time_vbox = new VBox();
			time_hbox = new HBox(0,2);
			main_vbox.addChild(time_vbox);
			time_vbox.addChild(time_hbox,ALIGN_EXPAND);
			time_range_from_el = new EditLine();
			time_range_to_el = new EditLine();
			time_el = new EditLine();
			time_settings_i = new Icon("core/systems/tracker/editor/images/tracker_time_settings.png");
			time_prev_i = new Icon("core/systems/tracker/editor/images/tracker_time_prev.png");
			time_play_i = new Icon("core/systems/tracker/editor/images/tracker_time_play.png");
			time_loop_i = new Icon("core/systems/tracker/editor/images/tracker_time_loop.png");
			time_next_i = new Icon("core/systems/tracker/editor/images/tracker_time_next.png");
			time_range_from_el.setValidator(VALIDATOR_FLOAT);
			time_range_to_el.setValidator(VALIDATOR_FLOAT);
			time_el.setValidator(VALIDATOR_FLOAT);
			time_play_i.setToggleable(1);
			time_loop_i.setToggleable(1);
			time_settings_i.setCallback(CLICKED,functionid(time_settings_clicked),this);
			time_prev_i.setCallback(CLICKED,functionid(callback_redirector),this,"time_prev_clicked");
			time_play_i.setCallback(CHANGED,functionid(callback_redirector),this,"time_play_changed");
			time_loop_i.setCallback(CHANGED,functionid(callback_redirector),this,"time_loop_changed");
			time_next_i.setCallback(CLICKED,functionid(callback_redirector),this,"time_next_clicked");
			time_range_from_el.setCallback(PRESSED,functionid(callback_redirector),this,"time_range_from_pressed");
			time_range_from_el.setCallback(FOCUS_OUT,functionid(callback_redirector),this,"time_range_from_pressed");
			time_range_to_el.setCallback(PRESSED,functionid(callback_redirector),this,"time_range_to_pressed");
			time_range_to_el.setCallback(FOCUS_OUT,functionid(callback_redirector),this,"time_range_to_pressed");
			time_el.setCallback(PRESSED,functionid(callback_redirector),this,"time_pressed");
			time_el.setCallback(FOCUS_OUT,functionid(callback_redirector),this,"time_pressed");
			time_hbox.addChild(time_settings_i);
			time_hbox.addChild(new Label(TR(" From: ")));
			time_hbox.addChild(time_range_from_el);
			time_hbox.addChild(new Label(TR(" To: ")));
			time_hbox.addChild(time_range_to_el);
			time_hbox.addChild(new Label(TR(" Time: ")));
			time_hbox.addChild(time_el);
			time_hbox.addChild(time_w,ALIGN_EXPAND);
			time_hbox.addChild(time_prev_i);
			time_hbox.addChild(time_play_i);
			time_hbox.addChild(time_loop_i);
			time_hbox.addChild(time_next_i);
			
			// dialogs
			settings_dialog = new TrackerEditorDialogSettings(tracker,TR("Track settings"));
			parameter_dialog = new TrackerEditorDialogParameter(tracker,TR("Add parameter"));
			node_dialog = new TrackerEditorDialogNode(TR("Select node"));
			settings_dialog.setSizeable(1);
			parameter_dialog.setSizeable(1);
			node_dialog.setSizeable(1);
			
			// clear parameters
			clear();
		}
		
		~TrackerEditor() {
			
			delete tracker_track;
			
			delete main_vbox;
			delete main_hp;
			
			delete left_vp;
			delete right_vp;
			
			delete left_vbox;
			delete parameter_vbox;
			delete parameter_gb;
			
			delete right_vbox;
			delete grid_vbox;
			delete grid_sb;
			
			delete object_type_l[0];
			delete object_type_l[1];
			delete object_id_l[0];
			delete object_id_l[1];
			delete object_name_l;
			delete object_name_el;
			delete parameter_type_l;
			delete parameter_type_cb;
			delete argument_value_l;
			delete argument_value_el;
			delete argument_value_cb;
			
			delete line_vbox;
			delete line_hbox;
			delete line_add_i;
			delete line_remove_i;
			delete line_up_i;
			delete line_down_i;
			delete line_save_i;
			
			delete time_vbox;
			delete time_hbox;
			delete time_range_from_el;
			delete time_range_to_el;
			delete time_el;
			delete time_settings_i;
			delete time_prev_i;
			delete time_play_i;
			delete time_loop_i;
			delete time_next_i;
			
			delete table_w;
			delete track_w;
			delete line_w;
			delete time_w;
			delete grid_w;
			
			delete parameter_dialog;
			delete settings_dialog;
			delete node_dialog;
		}
		
		/*
		 */
		Widget getWidget() {
			return main_vbox;
		}
		
		HPaned getMainHPaned() {
			return main_hp;
		}
		
		VPaned getLeftVPaned() {
			return left_vp;
		}
		
		VPaned getRightVPaned() {
			return right_vp;
		}
		
		Track getTrackWidget() {
			return track_w;
		}
		
		void arrange() {
			main_vbox.arrange();
		}
		
		/*
		 */
		void setMinTime(float time) {
			float old_time = getMinTime();
			tracker_track.setMinTime(time);
			track_w.setMinTime(time);
			line_w.setMinTime(time);
			time_w.setMinTime(time);
			grid_w.setMinTime(time);
			if(time_w.getRangeFrom() == old_time) time_w.setRangeFrom(time);
			else time_w.setRangeFrom(time_w.getRangeFrom());
			time_w.runCallback(TIME_RANGE_FROM_CHANGED);
		}
		
		float getMinTime() {
			return tracker_track.getMinTime();
		}
		
		/*
		 */
		void setMaxTime(float time) {
			float old_time = getMaxTime();
			tracker_track.setMaxTime(time);
			track_w.setMaxTime(time);
			line_w.setMaxTime(time);
			time_w.setMaxTime(time);
			grid_w.setMaxTime(time);
			if(time_w.getRangeTo() == old_time) time_w.setRangeTo(time);
			else time_w.setRangeTo(time_w.getRangeTo());
			time_w.runCallback(TIME_RANGE_TO_CHANGED);
		}
		
		float getMaxTime() {
			return tracker_track.getMaxTime();
		}
		
		/*
		 */
		void setUnitTime(float time) {
			tracker_track.setUnitTime(time);
		}
		
		float getUnitTime() {
			return tracker_track.getUnitTime();
		}
		
		/*
		 */
		void setTime(float time) {
			time_w.setTime(time);
			grid_w.setTime(time);
			time_w.runCallback(TIME_CHANGED);
		}
		
		float getTime() {
			return time_w.getTime();
		}
		
		/*
		 */
		void setState(int state) {
			time_play_i.setToggled((state == EDITOR_STATE_PLAY));
			time_loop_i.setToggled((state == EDITOR_STATE_LOOP));
		}
		
		int getState() {
			return state;
		}
		
		/*
		 */
		void clear() {
			
			// restore buffer track
			if(buffer_track != NULL) {
				blob.seekSet(0);
				buffer_track.restoreState(blob);
				delete buffer_track;
				delete blob;
			}
			
			// clear tracker track
			tracker_track.clear();
			
			// clear widgets
			table_w.clear();
			track_w.clear();
			line_w.clear();
			
			// clear parameters
			parameters.delete();
			current_parameter = NULL;
			
			// clear time
			setMinTime(0.0f);
			setMaxTime(1.0f);
			setTime(0.0f);
			
			// clear state
			state = EDITOR_STATE_STOP;
			time_play_i.setToggled(0);
			time_loop_i.setToggled(0);
			
			// update parameter widgets
			update_parameter(NULL);
			
			// disable line widgets
			line_remove_i.setEnabled(0);
			line_clone_i.setEnabled(0);
			line_up_i.setEnabled(0);
			line_down_i.setEnabled(0);
			line_save_i.setEnabled(0);
		}
		
		/**********************************************************************\
		*
		* Colors
		*
		\**********************************************************************/
		
		/*
		 */
		vec4 get_scalar_color() {
			return saturate(editor_scalar_colors[parameters.size() % editor_scalar_colors.size()]);
		}
		
		vec4 get_vector_color() {
			return saturate(editor_vector_colors[parameters.size() % editor_vector_colors.size()]);
		}
		
		/*
		 */
		vec4 get_track_color() {
			return get_scalar_color();
		}
		
		/*
		 */
		vec4 get_float_color() {
			return get_scalar_color();
		}
		
		vec4 get_vec3_color() {
			return get_vector_color();
		}
		
		/*
		 */
		vec4 get_toggle_color() {
			return get_scalar_color();
		}
		
		vec4 get_switch_color() {
			return get_scalar_color();
		}
		
		/*
		 */
		vec4 get_file_color() {
			return get_scalar_color();
		}
		
		vec4 get_material_color() {
			return get_scalar_color();
		}
		
		vec4 get_property_color() {
			return get_scalar_color();
		}
		
		/*
		 */
		vec4 get_node_color() {
			return get_scalar_color();
		}
		
		/**********************************************************************\
		*
		* Parameters
		*
		\**********************************************************************/
		
		/*
		 */
		TrackerEditorParameter create_parameter(string type_name) {
			
			// find parameter
			TrackerParameter parameter = tracker.findParameter(type_name);
			if(parameter == NULL) {
				if(match("track*",type_name)) type_name = editor_track_types[0];
				if(match("node.position*",type_name)) type_name = editor_node_position_types[0];
				if(match("node.rotation*",type_name)) type_name = editor_node_rotation_types[0];
				parameter = tracker.findParameter(type_name);
				if(parameter == NULL) {
					log.error("Unigine::Tracker::TrackerEditor::create_parameter(): can't find \"%s\" parameter\n",type_name);
					return NULL;
				}
			}
			
			// create parameter
			int type_id = parameter.getTypeID();
			if(type_id == TYPE_TRACK) return new TrackerEditorParameterTrack(parameter,get_track_color());
			if(type_id == TYPE_FLOAT) return new TrackerEditorParameterFloat(parameter,get_float_color());
			if(type_id == TYPE_VEC3) return new TrackerEditorParameterVec3(parameter,get_vec3_color());
			if(type_id == TYPE_COLOR) return new TrackerEditorParameterColor(parameter);
			if(type_id == TYPE_ENVIRONMENT) return new TrackerEditorParameterEnvironment(parameter);
			if(type_id == TYPE_TOGGLE) return new TrackerEditorParameterToggle(parameter,get_toggle_color());
			if(type_id == TYPE_SWITCH) return new TrackerEditorParameterSwitch(parameter,get_switch_color());
			if(type_id == TYPE_NODE) return new TrackerEditorParameterNode(parameter,get_node_color());
			if(type_id == TYPE_STRING) {
				string filter = TrackerParameterString(parameter).getFilter();
				if(filter == "material") return new TrackerEditorParameterMaterial(parameter,get_material_color());
				if(filter == "property") return new TrackerEditorParameterProperty(parameter,get_property_color());
				return new TrackerEditorParameterFile(parameter,get_file_color());
			}
			
			// unknown parameter
			throw("Unigine::Tracker::TrackerEditor::create_parameter(): unknown parameter type \"%s\"\n",type_name);
		}
		
		/*
		 */
		void update_parameter_type(TrackerEditorParameter parameter) {
			
			// hide parameter widgets
			parameter_type_l.setHidden(1);
			parameter_type_cb.setHidden(1);
			
			string type_name = parameter.parameter.getTypeName();
			
			// track
			if(match("track*",type_name)) {
				parameter_type_l.setHidden(0);
				parameter_type_cb.setHidden(0);
				parameter_type_cb.setCallbackEnabled(CHANGED,0);
				parameter_type_cb.clear();
				forloop(int i = 0; editor_track_types.size()) {
					int item = parameter_type_cb.addItem(TR(editor_track_names[i]));
					if(type_name == editor_track_types[i]) parameter_type_cb.setCurrentItem(item);
				}
				parameter_type_cb.setCallbackEnabled(CHANGED,1);
			}
			
			// node position
			else if(match("node.position*",type_name)) {
				parameter_type_l.setHidden(0);
				parameter_type_cb.setHidden(0);
				parameter_type_cb.setCallbackEnabled(CHANGED,0);
				parameter_type_cb.clear();
				forloop(int i = 0; editor_node_position_types.size()) {
					int item = parameter_type_cb.addItem(TR(editor_node_position_names[i]));
					if(type_name == editor_node_position_types[i]) parameter_type_cb.setCurrentItem(item);
				}
				parameter_type_cb.setCallbackEnabled(CHANGED,1);
			}
			
			// node rotation
			else if(match("node.rotation*",type_name)) {
				parameter_type_l.setHidden(0);
				parameter_type_cb.setHidden(0);
				parameter_type_cb.setCallbackEnabled(CHANGED,0);
				parameter_type_cb.clear();
				forloop(int i = 0; editor_node_rotation_types.size()) {
					int item = parameter_type_cb.addItem(TR(editor_node_rotation_names[i]));
					if(type_name == editor_node_rotation_types[i]) parameter_type_cb.setCurrentItem(item);
				}
				parameter_type_cb.setCallbackEnabled(CHANGED,1);
			}
		}
		
		/*
		 */
		void update_parameter_argument(TrackerEditorParameter parameter) {
			
			// update parameter name
			parameter.table_row.setText(parameter.getParameterName());
			
			// hide argument widgets
			argument_value_l.setHidden(1);
			argument_value_el.setHidden(1);
			argument_value_cb.setHidden(1);
			
			// check parameter
			if(parameter.parameter_object == NULL) return;
			if(strlen(parameter.parameter.getArgumentType()) == 0) return;
			
			string name = TR("Arg:");
			string values[0];
			
			string type_name = parameter.parameter.getTypeName();
			string argument_type = parameter.parameter.getArgumentType();
			
			Material material = parameter.parameter_object;
			Property property = parameter.parameter_object;
			Node node = parameter.parameter_object;
			
			// material arguments
			if(type_name == "material.stateToggle") {
				name = TR("State:");
				forloop(int i = 0; material.getNumStates()) {
					if(material.getStateType(i) == MATERIAL_STATE_TOGGLE) values.append(material.getStateName(i));
				}
			} else if(type_name == "material.stateSwitch") {
				name = TR("State:");
				forloop(int i = 0; material.getNumStates()) {
					if(material.getStateType(i) == MATERIAL_STATE_SWITCH) values.append(material.getStateName(i));
				}
			} else if(type_name == "material.textureImage") {
				name = TR("Texture:");
				forloop(int i = 0; material.getNumTextures()) {
					if(material.getTextureType(i) == MATERIAL_TEXTURE_IMAGE) values.append(material.getTextureName(i));
				}
			} else if(type_name == "material.parameterColor") {
				name = TR("Parameter:");
				forloop(int i = 0; material.getNumParameters()) {
					if(material.getParameterType(i) == MATERIAL_PARAMETER_COLOR) values.append(material.getParameterName(i));
				}
			} else if(type_name == "material.parameterSlider") {
				name = TR("Parameter:");
				forloop(int i = 0; material.getNumParameters()) {
					if(material.getParameterType(i) == MATERIAL_PARAMETER_SLIDER) values.append(material.getParameterName(i));
				}
			}
			// property arguments
			else if(type_name == "property.stateToggle") {
				name = TR("State:");
				forloop(int i = 0; property.getNumStates()) {
					if(property.getStateType(i) == PROPERTY_STATE_TOGGLE) values.append(property.getStateName(i));
				}
			} else if(type_name == "property.stateSwitch") {
				name = TR("State:");
				forloop(int i = 0; property.getNumStates()) {
					if(property.getStateType(i) == PROPERTY_STATE_SWITCH) values.append(property.getStateName(i));
				}
			} else if(type_name == "property.parameterFloat") {
				name = TR("Parameter:");
				forloop(int i = 0; property.getNumParameters()) {
					if(property.getParameterType(i) == PROPERTY_PARAMETER_FLOAT) values.append(property.getParameterName(i));
				}
			} else if(type_name == "property.parameterToggle") {
				name = TR("Parameter:");
				forloop(int i = 0; property.getNumParameters()) {
					if(property.getParameterType(i) == PROPERTY_PARAMETER_TOGGLE) values.append(property.getParameterName(i));
				}
			} else if(type_name == "property.parameterSwitch") {
				name = TR("Parameter:");
				forloop(int i = 0; property.getNumParameters()) {
					if(property.getParameterType(i) == PROPERTY_PARAMETER_SWITCH) values.append(property.getParameterName(i));
				}
			} else if(type_name == "property.parameterString") {
				name = TR("Parameter:");
				forloop(int i = 0; property.getNumParameters()) {
					if(property.getParameterType(i) == PROPERTY_PARAMETER_STRING) values.append(property.getParameterName(i));
				}
			} else if(type_name == "property.parameterColor") {
				name = TR("Parameter:");
				forloop(int i = 0; property.getNumParameters()) {
					if(property.getParameterType(i) == PROPERTY_PARAMETER_COLOR) values.append(property.getParameterName(i));
				}
			}
			// node arguments
			else {
				if(match("node.position*",type_name) || match("node.rotation*",type_name)) {
					name = TR("Offset:");
				}
				// object surface arguments
				else if(node.isObject() && (type_name == "node.object.enabled" || type_name == "node.object.material" || type_name == "node.object.property")) {
					name = TR("Surface:");
					Object object = node_cast(node);
					forloop(int i = 0; object.getNumSurfaces()) {
						values.append(object.getSurfaceName(i));
					}
				}
				// object material arguments
				else if(node.isObject() && type_name == "node.object.material.stateToggle") {
					name = TR("State:");
					Object object = node_cast(node);
					forloop(int i = 0; object.getNumSurfaces()) {
						Material material = object.getMaterial(i);
						if(material == NULL) continue;
						forloop(int j = 0; material.getNumStates()) {
							if(material.getStateType(j) == MATERIAL_STATE_TOGGLE) values.append(object.getSurfaceName(i) + ":" + material.getStateName(j));
						}
					}
				} else if(node.isObject() && type_name == "node.object.material.stateSwitch") {
					name = TR("State:");
					Object object = node_cast(node);
					forloop(int i = 0; object.getNumSurfaces()) {
						Material material = object.getMaterial(i);
						if(material == NULL) continue;
						forloop(int j = 0; material.getNumStates()) {
							if(material.getStateType(j) == MATERIAL_STATE_SWITCH) values.append(object.getSurfaceName(i) + ":" + material.getStateName(j));
						}
					}
				} else if(node.isObject() && type_name == "node.object.material.textureImage") {
					name = TR("Texture:");
					Object object = node_cast(node);
					forloop(int i = 0; object.getNumSurfaces()) {
						Material material = object.getMaterial(i);
						if(material == NULL) continue;
						forloop(int j = 0; material.getNumTextures()) {
							if(material.getTextureType(j) == MATERIAL_TEXTURE_IMAGE) values.append(object.getSurfaceName(i) + ":" + material.getTextureName(j));
						}
					}
				} else if(node.isObject() && type_name == "node.object.material.parameterColor") {
					name = TR("Parameter:");
					Object object = node_cast(node);
					forloop(int i = 0; object.getNumSurfaces()) {
						Material material = object.getMaterial(i);
						if(material == NULL) continue;
						forloop(int j = 0; material.getNumParameters()) {
							if(material.getParameterType(j) == MATERIAL_PARAMETER_COLOR) values.append(object.getSurfaceName(i) + ":" + material.getParameterName(j));
						}
					}
				} else if(node.isObject() && type_name == "node.object.material.parameterSlider") {
					name = TR("Parameter:");
					Object object = node_cast(node);
					forloop(int i = 0; object.getNumSurfaces()) {
						Material material = object.getMaterial(i);
						if(material == NULL) continue;
						forloop(int j = 0; material.getNumParameters()) {
							if(material.getParameterType(j) == MATERIAL_PARAMETER_SLIDER) values.append(object.getSurfaceName(i) + ":" + material.getParameterName(j));
						}
					}
				}
				// object property arguments
				else if(node.isObject() && type_name == "node.object.property.stateToggle") {
					name = TR("State:");
					Object object = node_cast(node);
					forloop(int i = 0; object.getNumSurfaces()) {
						Property property = object.getProperty(i);
						if(property == NULL) continue;
						forloop(int j = 0; property.getNumStates()) {
							if(property.getStateType(j) == PROPERTY_STATE_TOGGLE) values.append(object.getSurfaceName(i) + ":" + property.getStateName(j));
						}
					}
				} else if(node.isObject() && type_name == "node.object.property.stateSwitch") {
					name = TR("State:");
					Object object = node_cast(node);
					forloop(int i = 0; object.getNumSurfaces()) {
						Property property = object.getProperty(i);
						if(property == NULL) continue;
						forloop(int j = 0; property.getNumStates()) {
							if(property.getStateType(j) == PROPERTY_STATE_SWITCH) values.append(object.getSurfaceName(i) + ":" + property.getStateName(j));
						}
					}
				} else if(node.isObject() && type_name == "node.object.property.parameterFloat") {
					name = TR("Parameter:");
					Object object = node_cast(node);
					forloop(int i = 0; object.getNumSurfaces()) {
						Property property = object.getProperty(i);
						if(property == NULL) continue;
						forloop(int j = 0; property.getNumParameters()) {
							if(property.getParameterType(j) == PROPERTY_PARAMETER_FLOAT) values.append(object.getSurfaceName(i) + ":" + property.getParameterName(j));
						}
					}
				} else if(node.isObject() && type_name == "node.object.property.parameterColor") {
					name = TR("Parameter:");
					Object object = node_cast(node);
					forloop(int i = 0; object.getNumSurfaces()) {
						Property property = object.getProperty(i);
						if(property == NULL) continue;
						forloop(int j = 0; property.getNumParameters()) {
							if(property.getParameterType(j) == PROPERTY_PARAMETER_COLOR) values.append(object.getSurfaceName(i) + ":" + property.getParameterName(j));
						}
					}
				}
			}
			
			argument_value_l.setText(name);
			argument_value_l.setHidden(0);
			
			// combobox argument
			if(values.size()) {
				argument_value_el.setHidden(1);
				argument_value_cb.setHidden(0);
				argument_value_cb.setCallbackEnabled(CHANGED,0);
				argument_value_cb.clear();
				int current_item = -1;
				forloop(int i = 0; values.size()) {
					argument_value_cb.addItem(values[i]);
					if(parameter.parameter_argument == values[i]) current_item = i;
				}
				argument_value_cb.setCurrentItem(current_item);
				argument_value_cb.setCallbackEnabled(CHANGED,1);
			}
			// editline argument
			else {
				argument_value_el.setHidden(0);
				argument_value_cb.setHidden(1);
				if(argument_type != typeof(parameter.parameter_argument)) {
					if(argument_type == "int") parameter.parameter_argument = 0;
					else if(argument_type == "float") parameter.parameter_argument = 0.0f;
					else if(argument_type == "vec3") parameter.parameter_argument = vec3_zero;
					else if(argument_type == "vec4") parameter.parameter_argument = vec4_zero;
					else if(argument_type == "mat4") parameter.parameter_argument = mat4_identity;
					else if(argument_type == "quat") parameter.parameter_argument = quat_identity;
					else if(argument_type == "string") parameter.parameter_argument = "";
					else throw("Unigine::Tracker::TrackerEditor::update_parameter_argument(): unknown type of argument %s\n",argument_type);
				}
				argument_value_el.setText(string(parameter.parameter_argument));
			}
		}
		
		/*
		 */
		void update_parameter(TrackerEditorParameter parameter) {
			
			// parameter index
			int index = parameters.find(parameter);
			
			// hide parameter widgets
			object_type_l[0].setHidden(1);
			object_type_l[1].setHidden(1);
			object_id_l[0].setHidden(1);
			object_id_l[1].setHidden(1);
			object_name_l.setHidden(1);
			object_name_el.setHidden(1);
			parameter_type_l.setHidden(1);
			parameter_type_cb.setHidden(1);
			argument_value_l.setHidden(1);
			argument_value_el.setHidden(1);
			argument_value_cb.setHidden(1);
			
			// line widgets
			if(index == -1) {
				line_remove_i.setEnabled(0);
				line_clone_i.setEnabled(0);
				line_up_i.setEnabled(0);
				line_down_i.setEnabled(0);
				line_save_i.setEnabled(0);
				return;
			}
			
			// line controls
			line_remove_i.setEnabled(1);
			line_clone_i.setEnabled(1);
			line_up_i.setEnabled((index > 0));
			line_down_i.setEnabled((index < parameters.size() - 1));
			line_save_i.setEnabled((parameter.table_row.getValue(EDITOR_TABLE_COLUMN_ENABLE) == 0));
			
			// object type
			string object_type = parameter.parameter.getObjectType();
			if(strlen(object_type)) {
				
				// object type
				object_type_l[0].setHidden(0);
				object_type_l[1].setHidden(0);
				object_type_l[1].setText(object_type);
				
				// object name
				object_name_l.setHidden(0);
				object_name_el.setHidden(0);
				
				// material object
				if(object_type == "Material") {
					if(is_base_class(object_type,parameter.parameter_object)) {
						Material material = parameter.parameter_object;
						object_name_el.setText(material.getName());
					} else {
						object_name_el.setText("");
					}
				}
				// parameter object
				else if(object_type == "Property") {
					if(is_base_class(object_type,parameter.parameter_object)) {
						Property property = parameter.parameter_object;
						object_name_el.setText(property.getName());
					} else {
						object_name_el.setText("");
					}
				}
				// node object
				else {
					object_id_l[0].setHidden(0);
					object_id_l[1].setHidden(0);
					if(is_base_class(object_type,parameter.parameter_object)) {
						Node node = parameter.parameter_object;
						object_name_el.setText(node.getName());
						object_id_l[1].setText(format("0x%08x",node.getID()));
					} else {
						object_name_el.setText("");
						object_id_l[1].setText("Unknown");
					}
				}
			}
			
			// update parameter type
			update_parameter_type(parameter);
			
			// update parameter argument
			update_parameter_argument(parameter);
		}
		
		/*
		 */
		void check_parameter(TrackerEditorParameter parameter) {
			
			// check object type
			int object = parameter.parameter_object;
			string object_type = parameter.parameter.getObjectType();
			if(strlen(object_type) && (object == NULL || is_base_class(object_type,object) == 0)) return 0;
			if(strlen(object_type) && object_type != "Material" && object_type != "Property" && engine.world.isNode(object) == 0) return 0;
			
			// check argument type
			string argument_type = parameter.parameter.getArgumentType();
			if(strlen(argument_type) && argument_type != typeof(parameter.parameter_argument)) return 0;
			
			return 1;
		}
		
		/*
		 */
		void set_parameter(TrackerEditorParameter parameter) {
			
			// check parameter
			if(check_parameter(parameter) == 0) return;
			
			// check track
			int track = parameter.getTrack();
			if(parameter.parameter.getNumTrackKeys(track) == 0) return;
			if(parameter.table_row.getValue(EDITOR_TABLE_COLUMN_ENABLE) == 0) return;
			
			// set parameter
			int object = parameter.parameter_object;
			call(parameter.parameter.set_id,object,track,getTime());
		}
		
		void get_parameter(TrackerEditorParameter parameter) {
			
			// check parameter
			if(check_parameter(parameter) == 0) return;
			
			// get parameter
			int track = parameter.getTrack();
			int object = parameter.parameter_object;
			int argument = parameter.parameter_argument;
			if(parameter.parameter.init_id != -1) call(parameter.parameter.init_id,object,track,argument);
			call(parameter.parameter.get_id,object,track,getTime());
			parameter.updateCurve();
		}
		
		/*
		 */
		void init_parameter(TrackerEditorParameter parameter) {
			
			// check track
			int track = parameter.getTrack();
			if(parameter.parameter.getNumTrackKeys(track)) return;
			
			// get parameter
			get_parameter(parameter);
			
			// update parameters
			updateParameters();
		}
		
		/*
		 */
		TrackerEditorParameter add_parameter(string type_name) {
			TrackerEditorParameter parameter = create_parameter(type_name);
			tracker.compileParameters((parameter.parameter));
			parameters.append(parameter);
			init_parameter(parameter);
			return parameter;
		}
		
		/*
		 */
		void remove_parameter(TrackerEditorParameter parameter) {
			int index = parameters.find(parameter);
			if(index != -1) parameters.remove(index);
			updateParameters();
			delete parameter;
		}
		
		/*
		 */
		void clone_parameter(TrackerEditorParameter parameter) {
			TrackerEditorParameter new_parameter = create_parameter(parameter.parameter.getTypeName());
			new_parameter.setTrack(parameter.getTrack());
			new_parameter.parameter_object = parameter.parameter_object;
			new_parameter.parameter_argument = parameter.parameter_argument;
			new_parameter.updateCurve();
			parameters.append(new_parameter);
			updateParameters();
		}
		
		/**********************************************************************\
		*
		* Update
		*
		\**********************************************************************/
		
		/*
		 */
		void update() {
			
			// synchronize paned
			if(left_vp.widget != NULL && right_vp.widget != NULL) {
				if(left_vp.isFocused()) {
					right_vp.setValue(left_vp.getValue());
				} else if(right_vp.isFocused()) {
					left_vp.setValue(right_vp.getValue());
				}
			}
			
			// synchronize scrolls
			ScrollBox table_sb = table_w.getScrollBox();
			ScrollBox track_sb = track_w.getScrollBox();
			ScrollBox line_sb = line_w.getScrollBox();
			if(track_sb.widget != NULL && line_sb.widget != NULL) {
				if(table_w.isCanvasFocused()) {
					line_sb.setVScrollValue(table_sb.getVScrollValue());
				} else if(line_w.isCanvasFocused()) {
					table_sb.setVScrollValue(line_sb.getVScrollValue());
				}
				if(track_w.isCanvasFocused()) {
					line_w.setCanvasWidth(track_w.getCanvasWidth());
					line_sb.arrange();
					line_sb.setHScrollValue(track_sb.getHScrollValue());
				} else if(line_w.isCanvasFocused()) {
					track_w.setCanvasWidth(line_w.getCanvasWidth());
					track_sb.arrange();
					track_sb.setHScrollValue(line_sb.getHScrollValue());
				} else {
					track_sb.setHScrollValue(line_sb.getHScrollValue());
				}
				grid_w.setWidth(line_w.getCanvasWidth());
				grid_sb.arrange();
				grid_sb.setHScrollValue(line_sb.getHScrollValue());
			}
			
			// current time
			float time = getTime();
			if(state != EDITOR_STATE_STOP) {
				float new_time = time + engine.game.getIFps() / getUnitTime();
				if(new_time > time_w.getRangeTo()) {
					if(state == EDITOR_STATE_PLAY) {
						time = time_w.getRangeTo();
						time_play_i.setToggled(0);
						state = EDITOR_STATE_STOP;
					} else {
						time = time_w.getRangeFrom() + new_time - time_w.getRangeTo();
					}
				} else {
					time = new_time;
				}
				setTime(time);
			}
			
			// synchronize parameters
			track_w.setRangeFrom(time_w.getRangeFrom());
			track_w.setRangeTo(time_w.getRangeTo());
			line_w.setRangeFrom(time_w.getRangeFrom());
			line_w.setRangeTo(time_w.getRangeTo());
			line_w.setKeySnap(track_w.getKeySnap());
			
			// snap time
			float snap_time = INFINITY;
			if(track_w.getKeySnap() || engine.app.getKeyState(APP_KEY_ALT)) {
				snap_time = line_w.getSnapTime(getTime());
			}
			time_w.setSnapTime(snap_time);
			grid_w.setSnapTime(snap_time);
			
			// widgets time
			track_w.setTime(time);
			line_w.setTime(time);
			
			// update widgets
			table_w.update();
			track_w.update();
			line_w.update();
			time_w.update();
			grid_w.update();
			
			// set parameters
			foreach(TrackerEditorParameter parameter; parameters) {
				set_parameter(parameter);
			}
			
			// render parameters
			if(engine.visualizer.isEnabled()) {
				foreach(TrackerEditorParameter parameter; parameters) {
					parameter.renderTrack(time);
				}
			}
		}
		
		/*
		 */
		void updateParameters() {
			
			// clear tracker track
			tracker_track.parameters.clear();
			tracker_track.parameter_tracks.clear();
			tracker_track.parameter_objects.clear();
			tracker_track.parameter_arguments.clear();
			
			// clear table
			table_w.rows.clear();
			table_w.rows_changed = 1;
			table_w.clearCanvas();
			
			// clear track
			track_w.curves.clear();
			track_w.curves_changed = 1;
			track_w.clearCanvas();
			
			// clear line
			line_w.curves.clear();
			line_w.curves_changed = 1;
			line_w.clearCanvas();
			
			// add parameter
			foreach(TrackerEditorParameter parameter; parameters) {
				parameter.addParameter(table_w,track_w,line_w,tracker_track);
			}
			
			// initialize parameters
			tracker_track.init();
		}
		
		/**********************************************************************\
		*
		* Load/save track
		*
		\**********************************************************************/
		
		/*
		 */
		int loadTrack(string name) {
			
			// load track
			TrackerTrack track = new TrackerTrack(tracker);
			if(track.load(name) == 0) {
				log.error("Unigine::Tracker::TrackerEditor::loadTrack(): can't load \"%s\" track\n",name);
				return 0;
			}
			
			// set track
			if(setTrack(track) == 0) {
				log.error("Unigine::Tracker::TrackerEditor::loadTrack(): can't set \"%s\" track\n",name);
				delete track;
				return 0;
			}
			
			delete track;
			return 1;
		}
		
		/*
		 */
		int saveTrack(string name) {
			
			// get track
			TrackerTrack track = getTrack();
			if(track == NULL) {
				log.error("Unigine::Tracker::TrackerEditor::saveTrack(): can't get \"%s\" track\n",name);
				return 0;
			}
			
			// save track
			if(track.save(name) == 0) {
				log.error("Unigine::Tracker::TrackerEditor::saveTrack(): can't save \"%s\" track\n",name);
				delete track;
				return 0;
			}
			
			// reload track
			tracker.reloadTrack(name);
			
			delete track;
			return 1;
		}
		
		/**********************************************************************\
		*
		* Tracks
		*
		\**********************************************************************/
		
		/*
		 */
		int setTrack(TrackerTrack track) {
			
			clear();
			
			// save state
			delete blob;
			delete buffer_track;
			buffer_track = track.clone();
			buffer_track.init();
			blob = new Blob();
			if(buffer_track.saveState(blob) == 0) {
				delete buffer_track;
				delete blob;
			}
			
			// update range
			setMinTime(track.getMinTime());
			setMaxTime(track.getMaxTime());
			setUnitTime(track.getUnitTime());
			setTime(track.getMinTime());
			
			// create parameters
			forloop(int i = 0; track.parameters.size()) {
				TrackerParameter tracker_parameter = track.parameters[i];
				TrackerEditorParameter parameter = create_parameter(tracker_parameter.getTypeName());
				parameter.setTrack(track.parameter_tracks[i]);
				parameter.parameter_object = track.parameter_objects[i];
				parameter.parameter_argument = track.parameter_arguments[i];
				parameter.updateCurve();
				parameters.append(parameter);
			}
			updateParameters();
			
			// arrange widgets
			track_w.arrangeXY();
			
			return 1;
		}
		
		/*
		 */
		TrackerTrack getTrack() {
			return tracker_track.clone();
		}
		
		/**********************************************************************\
		*
		* Callbacks
		*
		\**********************************************************************/
		
		/*
		 */
		void object_name_pressed(TrackerEditor editor) {
			
			// check current parameter
			TrackerEditorParameter parameter = editor.current_parameter;
			if(editor.parameters.find(parameter) == -1) return;
			
			// check object type
			string object_type = parameter.parameter.getObjectType();
			if(strlen(object_type) == 0) return;
			
			string name = editor.object_name_el.getText();
			
			// material object
			if(object_type == "Material") {
				Material material = NULL;
				if(strlen(name)) {
					material = engine.materials.findMaterial(name);
					if(material == NULL) {
						dialogMessage(TR("Error"),format(TR("Can't find \"%s\" material"),name));
					}
					if(material != NULL && material.isEditable() == 0) {
						dialogMessage(TR("Error"),format(TR("\"%s\" is not editable material"),name));
						material = NULL;
					}
				}
				parameter.parameter_object = material;
			}
			// property object
			else if(object_type == "Property") {
				Property property = NULL;
				if(strlen(name)) {
					property = engine.properties.findProperty(name);
					if(property == NULL) {
						dialogMessage(TR("Error"),format(TR("Can't find \"%s\" property"),name));
					}
					if(property != NULL && property.isEditable() == 0) {
						dialogMessage(TR("Error"),format(TR("\"%s\" is not editable property"),name));
						property = NULL;
					}
				}
				parameter.parameter_object = property;
			}
			// node object
			else {
				Node node = NULL;
				if(strlen(name)) {
					int num = engine.editor.findNode(name);
					if(num != -1) {
						node = node_cast(engine.editor.getNode(num));
						if(is_base_class(object_type,node)) {
							editor.object_id_l[1].setText(format("0x%08x",node.getID()));
						} else {
							dialogMessage(TR("Error"),format(TR("Bad node type \"%s\" for \"%s\" parameter"),node.getTypeName(),object_type));
							editor.object_id_l[1].setText(TR("Unknown"));
							node = NULL;
						}
					} else {
						dialogMessage(TR("Error"),format(TR("Can't find \"%s\" node"),name));
						editor.object_id_l[1].setText(TR("Unknown"));
					}
				}
				parameter.parameter_object = node;
			}
			
			// update parameter argument
			editor.update_parameter_argument(parameter);
			
			// initialize parameter
			editor.init_parameter(parameter);
			
			// update parameters
			editor.updateParameters();
		}
		
		/*
		 */
		void object_name_double_clicked(TrackerEditor editor) {
			
			// check current parameter
			TrackerEditorParameter parameter = editor.current_parameter;
			if(editor.parameters.find(parameter) == -1) return;
			
			// check object type
			string object_type = parameter.parameter.getObjectType();
			if(strlen(object_type) == 0) return;
			
			string name = editor.object_name_el.getText();
			
			// material object
			if(object_type == "Material") {
				if(dialogMaterial(TR("Select material"),name)) {
					Material material = engine.materials.findMaterial(name);
					if(material == NULL) {
						dialogMessage(TR("Error"),format(TR("Can't find \"%s\" material"),name));
						return;
					}
					if(material.isEditable() == 0) {
						dialogMessage(TR("Error"),format(TR("\"%s\" is not editable material"),name));
						return;
					}
					parameter.parameter_object = material;
					editor.object_name_el.setText(name);
				}
			}
			// property object
			else if(object_type == "Property") {
				if(dialogProperty(TR("Select property"),name)) {
					Property property = engine.properties.findProperty(name);
					if(property == NULL) {
						dialogMessage(TR("Error"),format(TR("Can't find \"%s\" property"),name));
						return;
					}
					if(property.isEditable() == 0) {
						dialogMessage(TR("Error"),format(TR("\"%s\" is not editable property"),name));
						return;
					}
					parameter.parameter_object = property;
					editor.object_name_el.setText(name);
				}
			}
			// node object
			else {
				
				TrackerEditorDialogNode node_dialog = editor.node_dialog;
				
				// select node
				node_dialog.setNode(parameter.parameter_object);
				
				// show dialog
				node_dialog.arrange();
				addChild(node_dialog,ALIGN_OVERLAP | ALIGN_CENTER);
				node_dialog.setPermanentFocus();
				
				// wait dialog
				while(node_dialog.isDone() == 0) {
					node_dialog.update();
					wait;
				}
				
				// hide dialog
				removeChild(node_dialog);
				
				if(node_dialog.isOkClicked()) {
					Node node = node_dialog.getNode();
					if(node == NULL) {
						dialogMessage(TR("Error"),format(TR("Can't select node")));
						return;
					}
					if(is_base_class(object_type,node) == 0) {
						dialogMessage(TR("Error"),format(TR("Bad node type \"%s\" for \"%s\" parameter"),node.getTypeName(),object_type));
						return;
					}
					editor.object_id_l[1].setText(format("0x%08x",node.getID()));
					editor.object_name_el.setText(node.getName());
					parameter.parameter_object = node;
				}
			}
			
			// update parameter argument
			editor.update_parameter_argument(parameter);
			
			// initialize parameter
			editor.init_parameter(parameter);
			
			// update parameters
			editor.updateParameters();
		}
		
		/*
		 */
		void parameter_type_changed() {
			
			// check current parameter
			if(parameters.find(current_parameter) == -1) return;
			
			string type_name = current_parameter.parameter.getTypeName();
			
			// track
			if(match("track*",type_name)) {
				
				// find new parameter type
				type_name = editor_track_types[parameter_type_cb.getCurrentItem()];
				TrackerParameter parameter = tracker.findParameter(type_name);
				if(parameter == NULL) {
					log.error("Unigine::Tracker::TrackerEditor::parameter_type_changed(): can't find \"%s\" parameter\n",type_name);
					return;
				}
				
				// change parameter type
				current_parameter.parameter = parameter;
				tracker.compileParameters((parameter));
				update_parameter_argument(current_parameter);
				updateParameters();
			}
			
			// node position
			else if(match("node.position*",type_name)) {
				
				// find new parameter type
				type_name = editor_node_position_types[parameter_type_cb.getCurrentItem()];
				TrackerParameter parameter = tracker.findParameter(type_name);
				if(parameter == NULL) {
					log.error("Unigine::Tracker::TrackerEditor::parameter_type_changed(): can't find \"%s\" parameter\n",type_name);
					return;
				}
				
				// change parameter type
				current_parameter.parameter = parameter;
				tracker.compileParameters((parameter));
				update_parameter_argument(current_parameter);
				updateParameters();
			}
			
			// node rotation
			else if(match("node.rotation*",type_name)) {
				
				// find new parameter type
				type_name = editor_node_rotation_types[parameter_type_cb.getCurrentItem()];
				TrackerParameter parameter = tracker.findParameter(type_name);
				if(parameter == NULL) {
					log.error("Unigine::Tracker::TrackerEditor::parameter_type_changed(): can't find \"%s\" parameter\n",type_name);
					return;
				}
				
				// change parameter type
				current_parameter.parameter = parameter;
				tracker.compileParameters((parameter));
				update_parameter_argument(current_parameter);
				updateParameters();
			}
		}
		
		/*
		 */
		void argument_value_pressed() {
			
			// check current parameter
			if(parameters.find(current_parameter) == -1) return;
			string argument_type = current_parameter.parameter.getArgumentType();
			if(strlen(argument_type) == 0) return;
			
			string value = argument_value_el.getText();
			
			// convert argument
			if(argument_type == "int") current_parameter.parameter_argument = int(value);
			else if(argument_type == "float") current_parameter.parameter_argument = float(value);
			else if(argument_type == "vec3") current_parameter.parameter_argument = vec3(value);
			else if(argument_type == "vec4") current_parameter.parameter_argument = vec4(value);
			else if(argument_type == "mat4") current_parameter.parameter_argument = mat4(value);
			else if(argument_type == "quat") current_parameter.parameter_argument = quat(value);
			else if(argument_type == "string") current_parameter.parameter_argument = value;
			else throw("Unigine::Tracker::TrackerEditor::argument_value_pressed(): unknown type of argument %s\n",argument_type);
			
			// initialize parameter
			init_parameter(current_parameter);
			
			// update parameters
			updateParameters();
		}
		
		/*
		 */
		void argument_value_changed() {
			
			// check current parameter
			if(parameters.find(current_parameter) == -1) return;
			string argument_type = current_parameter.parameter.getArgumentType();
			if(strlen(argument_type) == 0) return;
			
			string value = argument_value_cb.getCurrentItemText();
			
			// convert argument
			if(argument_type == "int") current_parameter.parameter_argument = int(value);
			else if(argument_type == "float") current_parameter.parameter_argument = float(value);
			else if(argument_type == "vec3") current_parameter.parameter_argument = vec3(value);
			else if(argument_type == "vec4") current_parameter.parameter_argument = vec4(value);
			else if(argument_type == "mat4") current_parameter.parameter_argument = mat4(value);
			else if(argument_type == "quat") current_parameter.parameter_argument = quat(value);
			else if(argument_type == "string") current_parameter.parameter_argument = value;
			else throw("Unigine::Tracker::TrackerEditor::argument_value_changed(): unknown type of argument %s\n",argument_type);
			
			// initialize parameter
			init_parameter(current_parameter);
			
			// update parameters
			updateParameters();
		}
		
		/*
		 */
		void line_add_clicked(TrackerEditor editor) {
			
			TrackerEditorDialogParameter parameter_dialog = editor.parameter_dialog;
			
			// show dialog
			parameter_dialog.arrange();
			addChild(parameter_dialog,ALIGN_OVERLAP | ALIGN_CENTER);
			parameter_dialog.setPermanentFocus();
			
			// wait dialog
			while(parameter_dialog.isDone() == 0) wait;
			
			// hide dialog
			removeChild(parameter_dialog);
			
			// add parameter
			if(parameter_dialog.isOkClicked()) {
				string type_name = parameter_dialog.getTypeName();
				TrackerEditorParameter parameter = editor.add_parameter(type_name);
				parameter.table_row.setValue(EDITOR_TABLE_COLUMN_TRACK,1);
				if(is_base_class("Unigine::Tracker::TrackerEditorParameterVec3",parameter)) {
					TrackerEditorParameterVec3(parameter).table_row_y.setValue(EDITOR_TABLE_COLUMN_TRACK,1);
					TrackerEditorParameterVec3(parameter).table_row_z.setValue(EDITOR_TABLE_COLUMN_TRACK,1);
				}
				editor.table_w.setCurrentRow(parameter.table_row);
				object_name_double_clicked(editor);
				editor.updateParameters();
			}
		}
		
		void line_remove_clicked(TrackerEditor editor) {
			
			// check table
			TableRow table_row = editor.table_w.getCurrentRow();
			if(table_row == NULL) return;
			
			// confirm action
			TrackerEditorParameter parameter = table_row.getData();
			if(dialogMessage(TR("Confirm"),format(TR("Remove \"%s\" parameter?"),parameter.getParameterName()))) {
				editor.remove_parameter(parameter);
				editor.table_w.setCurrentRow(NULL);
				editor.table_row_changed();
				editor.update_selected_line();
			}
		}
		
		void line_clone_clicked() {
			
			// check table
			TableRow table_row = table_w.getCurrentRow();
			if(table_row == NULL) return;
			
			clone_parameter(table_row.getData());
			table_row_changed();
		}
		
		void line_up_clicked() {
			
			// check table
			TableRow table_row = table_w.getCurrentRow();
			if(table_row == NULL) return;
			
			// parameter index
			TrackerEditorParameter parameter = table_row.getData();
			int index = parameters.find(parameter);
			if(index <= 0) return;
			
			// move parameter up
			parameters[index] = parameters[index - 1];
			parameters[index - 1] = parameter;
			updateParameters();
			
			// update row
			table_row_changed();
		}
		
		void line_down_clicked() {
			
			// check table
			TableRow table_row = table_w.getCurrentRow();
			if(table_row == NULL) return;
			
			// parameter index
			TrackerEditorParameter parameter = table_row.getData();
			int index = parameters.find(parameter);
			if(index >= parameters.size() - 1) return;
			
			// move parameter down
			parameters[index] = parameters[index + 1];
			parameters[index + 1] = parameter;
			updateParameters();
			
			// update row
			table_row_changed();
		}
		
		void line_save_clicked() {
			
			// get current parameter
			get_parameter(current_parameter);
		}
		
		/*
		 */
		void table_row_changed() {
			
			// update current parameter
			current_parameter = NULL;
			TableRow table_row = table_w.getCurrentRow();
			if(table_row != NULL) current_parameter = table_row.getData();
			update_parameter(current_parameter);
		}
		
		void table_column_changed() {
			
			// update current parameter
			update_parameter(current_parameter);
			
			// update parameters
			updateParameters();
		}
		
		/*
		 */
		void update_selected_track() {
			TrackerEditorParameter parameters[];
			forloop(int i = 0; track_w.getNumSelectedKeys()) {
				TrackCurve track_curve = track_w.getSelectedCurve(i);
				LineCurve line_curve = track_curve.getData();
				TrackerEditorParameter parameter = line_curve.getData();
				if(parameter != NULL) parameters.append(parameter);
			}
			foreachkey(TrackerEditorParameter parameter; parameters) {
				parameter.updateTrack();
			}
		}
		
		/*
		 */
		void track_key_created() {
			Unigine::Widgets::LineTrack::trackKeyCreated(line_w,track_w);
			update_selected_track();
		}
		
		void track_key_changed() {
			Unigine::Widgets::LineTrack::trackKeyChanged(line_w,track_w);
			update_selected_track();
		}
		
		void track_key_removed() {
			Unigine::Widgets::LineTrack::trackKeyRemoved(line_w,track_w);
			TrackKey selected_keys[0];
			TrackCurve selected_curves[0];
			TrackerEditorParameter parameters[];
			forloop(int i = 0; track_w.getNumSelectedKeys()) {
				selected_keys.append(track_w.getSelectedKey(i));
				selected_curves.append(track_w.getSelectedCurve(i));
			}
			forloop(int i = 0; selected_keys.size()) {
				TrackCurve track_curve = selected_curves[i];
				track_curve.removeKey(selected_keys[i]);
				track_w.clearSelectedKey(selected_keys[i]);
				LineCurve line_curve = track_curve.getData();
				TrackerEditorParameter parameter = line_curve.getData();
				if(parameter != NULL) parameters.append(parameter);
			}
			foreachkey(TrackerEditorParameter parameter; parameters) {
				parameter.updateTrack();
			}
		}
		
		/*
		 */
		void update_selected_line() {
			TrackerEditorParameter parameters[];
			forloop(int i = 0; line_w.getNumSelectedKeys()) {
				LineCurve line_curve = line_w.getSelectedCurve(i);
				TrackerEditorParameter parameter = line_curve.getData();
				if(parameter == NULL || parameters.check(parameter)) continue;
				parameters.append(parameter);
				parameter.updateTrack();
			}
			LineKey line_key = line_w.getCurrentKey();
			LineCurve line_curve = line_w.getCurrentCurve();
			if(line_key != NULL && line_curve != NULL && line_curve.getData() != NULL) {
				track_remove_i.setEnabled(1);
				track_time_el.setEnabled(1);
				track_time_el.setText(widgetFormat(line_key.getTime()));
				if(is_base_class("Unigine::Widgets::LineKeyTrack",line_key)) {
					TrackKey track_key = LineKeyTrack(line_key).getTrackKey();
					if(track_key != NULL) {
						track_value_el.setEnabled(1);
						track_value_el.setText(widgetFormat(track_key.getValue()));
					} else {
						track_value_el.setEnabled(0);
						track_value_el.setText("");
					}
				} else {
					track_value_el.setEnabled(0);
					track_value_el.setText("");
				}
			} else {
				track_remove_i.setEnabled(0);
				track_time_el.setEnabled(0);
				track_time_el.setText("");
				track_value_el.setEnabled(0);
				track_value_el.setText("");
			}
		}
		
		/*
		 */
		void line_key_created() {
			Unigine::Widgets::LineTrack::lineKeyCreated(line_w,track_w);
			update_selected_line();
		}
		
		void line_key_changed() {
			Unigine::Widgets::LineTrack::lineKeyChanged(line_w,track_w);
			update_selected_line();
		}
		
		void line_key_removed() {
			Unigine::Widgets::LineTrack::lineKeyRemoved(line_w,track_w);
			LineKey selected_keys[0];
			LineCurve selected_curves[0];
			TrackerEditorParameter parameters[];
			forloop(int i = 0; line_w.getNumSelectedKeys()) {
				selected_keys.append(line_w.getSelectedKey(i));
				selected_curves.append(line_w.getSelectedCurve(i));
			}
			forloop(int i = 0; selected_keys.size()) {
				LineCurve line_curve = selected_curves[i];
				line_curve.removeKey(selected_keys[i]);
				line_w.clearSelectedKey(selected_keys[i]);
				TrackerEditorParameter parameter = line_curve.getData();
				if(parameter != NULL) parameters.append(parameter);
			}
			foreachkey(TrackerEditorParameter parameter; parameters) {
				parameter.updateTrack();
			}
			track_remove_i.setEnabled(0);
			track_time_el.setEnabled(0);
			track_time_el.setText("");
			track_value_el.setEnabled(0);
			track_value_el.setText("");
		}
		
		/*
		 */
		void track_remove_clicked(TrackerEditor editor) {
			if(dialogMessage(TR("Confirm"),TR("Remove selected keys?"))) {
				editor.line_w.removeSelectedKeys();
				editor.track_w.removeSelectedKeys();
			}
		}
		
		/*
		 */
		void track_time_pressed() {
			float time = clamp(float(track_time_el.getText()),getMinTime(),getMaxTime());
			forloop(int i = 0; track_w.getNumSelectedKeys()) {
				TrackCurve track_curve = track_w.getSelectedCurve(i);
				TrackKey track_key = track_w.getSelectedKey(i);
				track_key.setTime(time);
				track_curve.updateKeys();
			}
			forloop(int i = 0; line_w.getNumSelectedKeys()) {
				LineCurve line_curve = line_w.getSelectedCurve(i);
				LineKey line_key = line_w.getSelectedKey(i);
				line_key.setTime(time);
				line_curve.updateKeys();
			}
			track_time_el.setText(widgetFormat(time));
			update_selected_track();
			update_selected_line();
		}
		
		void track_value_pressed() {
			float value = float(track_value_el.getText());
			forloop(int i = 0; track_w.getNumSelectedKeys()) {
				TrackCurve track_curve = track_w.getSelectedCurve(i);
				TrackKey track_key = track_w.getSelectedKey(i);
				track_key.setValue(value);
				track_curve.updateKeys();
			}
			track_value_el.setText(widgetFormat(value));
			update_selected_track();
		}
		
		/*
		 */
		void time_settings_clicked(TrackerEditor editor) {
			
			TrackerEditorDialogSettings settings_dialog = editor.settings_dialog;
			
			// settings parameters
			settings_dialog.setMinTime(editor.getMinTime());
			settings_dialog.setMaxTime(editor.getMaxTime());
			settings_dialog.setUnitTime(editor.getUnitTime());
			
			// show dialog
			settings_dialog.arrange();
			addChild(settings_dialog,ALIGN_OVERLAP | ALIGN_CENTER);
			settings_dialog.setPermanentFocus();
			
			// wait dialog
			while(settings_dialog.isDone() == 0) wait;
			
			// hide dialog
			removeChild(settings_dialog);
			
			// ok clicked
			if(settings_dialog.isOkClicked()) {
				editor.setMinTime(settings_dialog.getMinTime());
				editor.setMaxTime(settings_dialog.getMaxTime());
				editor.setUnitTime(settings_dialog.getUnitTime());
			}
		}
		
		/*
		 */
		void time_range_from_pressed() {
			time_w.setRangeFrom(float(time_range_from_el.getText()));
			time_range_from_changed();
		}
		
		void time_range_to_pressed() {
			time_w.setRangeTo(float(time_range_to_el.getText()));
			time_range_to_changed();
		}
		
		void time_pressed() {
			time_w.setTime(float(time_el.getText()));
			time_changed();
		}
		
		/*
		 */
		void time_changed() {
			grid_w.setTime(time_w.getTime());
			time_el.setText(widgetFormat(time_w.getTime()));
		}
		
		void time_range_from_changed() {
			grid_w.setRangeFrom(time_w.getRangeFrom());
			time_range_from_el.setText(widgetFormat(time_w.getRangeFrom()));
		}
		
		void time_range_to_changed() {
			grid_w.setRangeTo(time_w.getRangeTo());
			time_range_to_el.setText(widgetFormat(time_w.getRangeTo()));
		}
		
		/*
		 */
		void grid_changed() {
			time_w.setTime(grid_w.getTime());
			time_el.setText(widgetFormat(time_w.getTime()));
		}
		
		void grid_range_from_changed() {
			time_w.setRangeFrom(grid_w.getRangeFrom());
			time_range_from_el.setText(widgetFormat(time_w.getRangeFrom()));
		}
		
		void grid_range_to_changed() {
			time_w.setRangeTo(grid_w.getRangeTo());
			time_range_to_el.setText(widgetFormat(time_w.getRangeTo()));
		}
		
		/*
		 */
		void time_prev_clicked() {
			setTime(line_w.getPrevTime(getTime()));
		}
		
		void time_play_changed() {
			if(time_play_i.isToggled()) {
				state = EDITOR_STATE_PLAY;
				time_loop_i.setCallbackEnabled(CHANGED,0);
				time_loop_i.setToggled(0);
				time_loop_i.setCallbackEnabled(CHANGED,1);
				if(getTime() >= time_w.getRangeTo() - EPSILON) {
					setTime(time_w.getRangeFrom());
				}
			} else {
				state = EDITOR_STATE_STOP;
			}
		}
		
		void time_loop_changed() {
			if(time_loop_i.isToggled()) {
				state = EDITOR_STATE_LOOP;
				time_play_i.setCallbackEnabled(CHANGED,0);
				time_play_i.setToggled(0);
				time_play_i.setCallbackEnabled(CHANGED,1);
			} else {
				state = EDITOR_STATE_STOP;
			}
		}
		
		void time_next_clicked() {
			setTime(line_w.getNextTime(getTime()));
		}
		
		/*
		 */
		void callback_redirector(TrackerEditor editor,string name) {
			editor.call(name);
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_EDITOR_H__ */
