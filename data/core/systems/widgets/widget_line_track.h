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


#ifndef __UNIGINE_WIDGET_LINE_TRACK_H__
#define __UNIGINE_WIDGET_LINE_TRACK_H__

#include <core/systems/widgets/widget_dialog.h>
#include <core/systems/widgets/widget_gridbox.h>
#include <core/systems/widgets/widget_label.h>
#include <core/systems/widgets/widget_editline.h>
#include <core/systems/widgets/widget_combobox.h>
#include <core/systems/widgets/widget_track.h>
#include <core/systems/widgets/widget_line.h>

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	vec4 line_track_color = vec4(0.06f,0.1f,0.12f,1.0f);
	
	/**************************************************************************\
	*
	* LineKeyTrack
	*
	\**************************************************************************/
	
	/*
	 */
	class LineKeyTrack : LineKey {
		
		TrackKey key;
		
		/*
		 */
		LineKeyTrack(float t,TrackKey k) : LineKey(t) {
			key = k;
		}
		
		/*
		 */
		void setTrackKey(TrackKey k) {
			key = k;
		}
		
		TrackKey getTrackKey() {
			return key;
		}
		
		/*
		 */
		void setValue(float value) {
			key.setValue(value);
		}
		
		float getValue() {
			return key.getValue();
		}
		
		/*
		 */
		void setHandles(vec4 handles) {
			key.setHandles(handles);
		}
		
		vec4 getHandles() {
			return key.getHandles();
		}
		
		/*
		 */
		void setType(int type) {
			key.setType(type);
		}
		
		int getType() {
			return key.getType();
		}
		
		/*
		 */
		vec4 get_color() {
			return TrackCurve(LineCurveTrack(curve).getTrackCurve()).getColor();
		}
	};
	
	/**************************************************************************\
	*
	* LineCurveTrack
	*
	\**************************************************************************/
	
	/*
	 */
	class LineCurveTrack : LineCurve {
		
		TrackCurve curve;
		
		/*
		 */
		LineCurveTrack(TrackCurve c) {
			
			curve = c;
		}
		
		/*
		 */
		void setTrackCurve(TrackCurve c) {
			curve = c;
		}
		
		TrackCurve getTrackCurve() {
			return curve;
		}
		
		/*
		 */
		LineKeyTrack create_key(float time) {
			return new LineKeyTrack(time,NULL);
		}
		
		LineKeyTrack clone_key(LineKeyTrack key) {
			TrackKey track_key = key.getTrackKey();
			if(track_key != NULL) {
				TrackCurve track_curve = track_key.getCurve();
				track_key = track_curve.cloneKey(track_key);
				LineKeyTrack line_key = new LineKeyTrack(key.getTime(),track_key);
				line_key.setTrackKey(track_key);
				track_key.setData(line_key);
				return line_key;
			}
			return new LineKeyTrack(key.getTime(),NULL);
		}
		
		/*
		 */
		void update_key(LineKeyTrack key) {
			
			Dialog dialog;
			EditLine time_el;
			EditLine value_el;
			ComboBox type_cb;
			
			// create dialog
			if(dialog == NULL) {
				
				dialog = new Dialog("Key parameters");
				dialog.setSizeable(1);
				
				VBox vbox = new VBox();
				dialog.addChild(vbox);
				
				GridBox gridbox = new GridBox(2,4,4);
				vbox.addChild(gridbox,ALIGN_EXPAND);
				
				gridbox.addChild(new Label(TR("Time:")),ALIGN_RIGHT);
				time_el = new EditLine();
				time_el.setValidator(VALIDATOR_FLOAT);
				gridbox.addChild(time_el,ALIGN_EXPAND);
				
				gridbox.addChild(new Label(TR("Value:")),ALIGN_RIGHT);
				value_el = new EditLine();
				value_el.setValidator(VALIDATOR_FLOAT);
				gridbox.addChild(value_el,ALIGN_EXPAND);
				
				gridbox.addChild(new Label(TR("Type:")),ALIGN_RIGHT);
				type_cb = new ComboBox();
				type_cb.addItem(TR("Constant"));
				type_cb.addItem(TR("Linear"));
				type_cb.addItem(TR("Smooth"));
				type_cb.addItem(TR("Break"));
				type_cb.addItem(TR("Auto"));
				gridbox.addChild(type_cb,ALIGN_EXPAND);
			}
			
			// line
			LineCurveTrack curve = key.getCurve();
			Line line = curve.getLine();
			
			// track
			TrackKey track_key = key.getTrackKey();
			TrackCurve track_curve = track_key.getCurve();
			Track track = track_curve.getTrack();
			
			// save keys
			int has_time = 1;
			int has_value = 1;
			float times[line.getNumSelectedKeys()];
			float values[line.getNumSelectedKeys()];
			forloop(int i = 0; line.getNumSelectedKeys()) {
				LineKey key = line.getSelectedKey(i);
				if(key is LineKeyTrack) {
					values[i] = LineKeyTrack(key).getValue();
					if(i && values[0] != values[i]) has_value = 0;
				}
				times[i] = key.getTime();
				if(i && times[0] != times[i]) has_time = 0;
			}
			
			// key parameters
			time_el.setText((has_time) ? widgetFormat(track_key.getTime()) : "");
			value_el.setText((has_value) ? widgetFormat(track_key.getValue()) : "");
			type_cb.setCurrentItem(track_key.getType());
			
			// register callbacks
			time_el.setCallback(PRESSED,functionid(time_pressed),line,track,time_el);
			value_el.setCallback(PRESSED,functionid(value_pressed),line,track,value_el);
			
			// show dialog
			dialog.arrange();
			Unigine::Widgets::addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
			dialog.setPermanentFocus();
			
			// wait dialog
			while(dialog.isDone() == 0) wait;
			
			// dialog result
			if(dialog.isOkClicked()) {
				float time = clamp(float(time_el.getText()),line.getMinTime(),line.getMaxTime());
				foreach(LineKey key; line.selected_keys) {
					if(key is LineKeyTrack) {
						TrackKey track_key = LineKeyTrack(key).getTrackKey();
						TrackCurve track_curve = track_key.getCurve();
						if(strlen(value_el.getText())) track_key.setValue(clamp(float(value_el.getText()),track_curve.getMinValue(),track_curve.getMaxValue()));
						if(strlen(time_el.getText())) track_key.setTime(time);
					}
					if(strlen(time_el.getText())) key.setTime(time);
				}
			} else {
				forloop(int i = 0; line.getNumSelectedKeys()) {
					LineKey key = line.getSelectedKey(i);
					if(key is LineKeyTrack) {
						TrackKey track_key = LineKeyTrack(key).getTrackKey();
						track_key.setTime(times[i]);
						track_key.setValue(values[i]);
					}
					key.setTime(times[i]);
				}
			}
			
			// update widgets
			line.updateSelectedKeys();
			if(track != NULL) track.updateSelectedKeys();
			
			// hide dialog
			Unigine::Widgets::removeChild(dialog);
		}
		
		void time_pressed(Line line,Track track,EditLine time_el) {
			if(strlen(time_el.getText()) == 0) return;
			float time = clamp(float(time_el.getText()),line.getMinTime(),line.getMaxTime());
			foreach(LineKey key; line.selected_keys) {
				if(key is LineKeyTrack) {
					TrackKey track_key = LineKeyTrack(key).getTrackKey();
					track_key.setTime(time);
				}
				key.setTime(time);
			}
			line.updateSelectedKeys();
			if(track != NULL) track.updateSelectedKeys();
		}
		
		void value_pressed(Line line,Track track,EditLine value_el) {
			if(strlen(value_el.getText()) == 0) return;
			foreach(LineKey key; line.selected_keys) {
				if(key is LineKeyTrack) {
					TrackKey track_key = LineKeyTrack(key).getTrackKey();
					TrackCurve track_curve = track_key.getCurve();
					track_key.setValue(clamp(float(value_el.getText()),track_curve.getMinValue(),track_curve.getMaxValue()));
				}
			}
			line.updateSelectedKeys();
			if(track != NULL) track.updateSelectedKeys();
		}
		
		string get_update_func() {
			return "Unigine::Widgets::LineCurveTrack::update_key";
		}
		
		/*
		 */
		void create_image(int width) {
			
		}
		
		void update_image(float step,float offset) {
			
		}
		
		/*
		 */
		void create(int position_y,LineKey selected_keys[]) {
			
			LineCurve::create(position_y,selected_keys);
			
			canvas.setPolygonColor(polygon_id,line_track_color);
		}
	};
	
	/**************************************************************************\
	*
	* LineTrack
	*
	\**************************************************************************/
	
	/*
	 */
	namespace LineTrack {
		
		/*
		 */
		void lineKeyCreated(Line line,Track track) {
			
			// create track keys
			forloop(int i = 0; line.getNumSelectedKeys()) {
				
				LineCurveTrack line_curve = line.getSelectedCurve(i);
				if(line_curve is LineCurveTrack == 0) continue;
				TrackCurve track_curve = line_curve.getTrackCurve();
				if(track_curve == NULL) continue;
				
				LineKeyTrack line_key = line.getSelectedKey(i);
				if(line_key is LineKeyTrack == 0) continue;
				TrackKey track_key = line_key.getTrackKey();
				if(track_key != NULL) continue;
				
				track_key = new TrackKey(line_key.getTime(),track_curve.getValue(line_key.getTime()));
				line_key.setTrackKey(track_key);
				track_key.setData(line_key);
				track_curve.addKey(track_key);
				
				TrackKey prev_key = track_curve.getPrevKey(track_key);
				if(prev_key != NULL) track_curve.setKeyType(track_key,prev_key.getType());
			}
			
			// update track keys
			lineKeyChanged(line,track);
		}
		
		/*
		 */
		void lineKeyChanged(Line line,Track track) {
			
			TrackCurve selected_curves[0];
			TrackKey selected_keys[0];
			
			// update current track key
			LineCurveTrack line_curve = line.getCurrentCurve();
			LineKeyTrack line_key = line.getCurrentKey();
			if(line_curve is LineCurveTrack && line_key is LineKeyTrack) {
				TrackCurve track_curve = line_curve.getTrackCurve();
				TrackKey track_key = line_key.getTrackKey();
				if(track_curve != NULL && track_key != NULL && track.isCurve(track_curve)) {
					track.setCallbackEnabled(TRACK_KEY_CHANGED,0);
					track.setCurrentCurve(track_curve);
					track.setCurrentKey(track_key);
					track.updateSelectedKeys();
					track.setCallbackEnabled(TRACK_KEY_CHANGED,1);
				}
			} else {
				track.setCurrentCurve(NULL);
				track.setCurrentKey(NULL);
			}
			
			// update selected track keys
			forloop(int i = 0; line.getNumSelectedKeys()) {
				
				LineCurveTrack line_curve = line.getSelectedCurve(i);
				if(line_curve is LineCurveTrack == 0) continue;
				TrackCurve track_curve = line_curve.getTrackCurve();
				if(track_curve == NULL) continue;
				
				LineKeyTrack line_key = line.getSelectedKey(i);
				if(line_key is LineKeyTrack == 0) continue;
				TrackKey track_key = line_key.getTrackKey();
				if(track_key == NULL) continue;
				
				selected_curves.append(track_curve);
				selected_keys.append(track_key);
				
				track_key.setTime(line_key.getTime());
				track_curve.updateKeys();
			}
			
			track.setSelectedCurves(selected_curves);
			track.setSelectedKeys(selected_keys);
		}
		
		/*
		 */
		void lineKeyRemoved(Line line,Track track) {
			
			// remove selected track keys
			forloop(int i = 0; line.getNumSelectedKeys()) {
				
				LineCurveTrack line_curve = line.getSelectedCurve(i);
				if(line_curve is LineCurveTrack == 0) continue;
				TrackCurve track_curve = line_curve.getTrackCurve();
				if(track_curve == NULL) continue;
				
				LineKeyTrack line_key = line.getSelectedKey(i);
				if(line_key is LineKeyTrack == 0) continue;
				TrackKey track_key = line_key.getTrackKey();
				if(track_key == NULL) continue;
				
				track_curve.removeKey(track_key);
				track.clearSelectedKey(track_key);
			}
		}
		
		/*
		 */
		void trackKeyCreated(Line line,Track track) {
			
			// create line keys
			forloop(int i = 0; track.getNumSelectedKeys()) {
				
				TrackCurve track_curve = track.getSelectedCurve(i);
				LineCurve line_curve = track_curve.getData();
				if(line_curve == NULL) continue;
				
				TrackKey track_key = track.getSelectedKey(i);
				LineKeyTrack line_key = track_key.getData();
				if(line_key != NULL) continue;
				
				line_key = new LineKeyTrack(track_key.getTime(),track_key);
				track_key.setData(line_key);
				line_curve.addKey(line_key);
			}
			
			// update line keys
			trackKeyChanged(line,track);
		}
		
		/*
		 */
		void trackKeyChanged(Line line,Track track) {
			
			LineCurve selected_curves[0];
			LineKey selected_keys[0];
			
			// update current line key
			TrackCurve track_curve = track.getCurrentCurve();
			TrackKey track_key = track.getCurrentKey();
			if(track_curve != NULL && track_key != NULL) {
				line.setCurrentCurve(track_curve.getData());
				line.setCurrentKey(track_key.getData());
			} else {
				line.setCurrentCurve(NULL);
				line.setCurrentKey(NULL);
			}
			
			// update selected line keys
			forloop(int i = 0; track.getNumSelectedKeys()) {
				
				TrackCurve track_curve = track.getSelectedCurve(i);
				LineCurve line_curve = track_curve.getData();
				if(line_curve == NULL) continue;
				
				TrackKey track_key = track.getSelectedKey(i);
				LineKeyTrack line_key = track_key.getData();
				if(line_key == NULL) continue;
				
				selected_curves.append(line_curve);
				selected_keys.append(line_key);
				
				line_key.setTime(track_key.getTime());
				line_curve.updateKeys();
			}
			
			line.setSelectedCurves(selected_curves);
			line.setSelectedKeys(selected_keys);
		}
		
		/*
		 */
		void trackKeyRemoved(Line line,Track track) {
			
			// remove selected line keys
			forloop(int i = 0; track.getNumSelectedKeys()) {
				
				TrackCurve track_curve = track.getSelectedCurve(i);
				LineCurve line_curve = track_curve.getData();
				if(line_curve == NULL) continue;
				
				TrackKey track_key = track.getSelectedKey(i);
				LineKeyTrack line_key = track_key.getData();
				if(line_key == NULL) continue;
				
				line_curve.removeKey(line_key);
				line.clearSelectedKey(line_key);
			}
		}
	}
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_LINE_TRACK_H__ */
