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


#ifndef __UNIGINE_TRACKER_EDITOR_PARAMETER_H__
#define __UNIGINE_TRACKER_EDITOR_PARAMETER_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	enum {
		
		EDITOR_TABLE_OFFSET = 28,		// table text offset
		EDITOR_TABLE_COLUMN_ENABLE = 0,	// table column enable
		EDITOR_TABLE_COLUMN_TRACK,		// table column track
		EDITOR_TABLE_NUM_COLUMNS,		// number of table columns
		
		EDITOR_CURVE_POINTS = 16,		// curve points
		EDITOR_MANIPULATOR_SIZE = 64,	// manipulator size
	};
	
	/**************************************************************************\
	*
	* TrackerEditorParameter
	*
	\**************************************************************************/
	
	/*
	 */
	class TrackerEditorParameter {
		
		using Unigine::Widgets;
		
		TableRow table_row;				// table row
		
		TrackerParameter parameter;		// tracker parameter
		
		int parameter_object;			// parameter object
		int parameter_argument;			// parameter argument
		
		/*
		 */
		TrackerEditorParameter(TrackerParameter p) {
			
			parameter = p;
			
			parameter_object = NULL;
			parameter_argument = NULL;
		}
		
		/*
		 */
		void addParameter(Table table,Track track,Line line,TrackerTrack tracker_track) {
			throw("Unigine::Tracker::TrackerEditorParameter::addParameter(): called\n");
		}
		
		void add_parameter(TrackerTrack tracker_track,int parameter_track) {
			
			// parameter name
			table_row.setText(getParameterName());
			
			// check object
			if(strlen(parameter.object_type)) {
				if(parameter_object == NULL) return;
				if(is_base_class(parameter.object_type,parameter_object) == 0) {
					log.warning("Unigine::Tracker::TrackerEditorParameter::add_parameter(): bad object type \"%s\" for \"%s\" parameter\n",typeof(parameter_object),parameter.getTypeName());
					return;
				}
			}
			
			// check argument
			if(strlen(parameter.argument_type)) {
				if(parameter_argument == NULL && parameter.argument_type == "string") return;
				if(parameter.argument_type != typeof(parameter_argument)) {
					log.warning("Unigine::Tracker::TrackerEditorParameter::add_parameter(): bad argument type \"%s\" for \"%s\" parameter\n",typeof(parameter_argument),parameter.getTypeName());
					return;
				}
			}
			
			// add tracker parameter
			tracker_track.parameters.append(parameter);
			tracker_track.parameter_tracks.append(parameter_track);
			tracker_track.parameter_objects.append(parameter_object);
			tracker_track.parameter_arguments.append(parameter_argument);
		}
		
		/*
		 */
		string getParameterName() {
			
			string type_name = parameter.getTypeName();
			string object_type = parameter.getObjectType();
			string argument_type = parameter.getArgumentType();
			
			if(strlen(object_type) == 0) return type_name;
			
			// material object
			if(object_type == "Material") {
				Material material = parameter_object;
				if(is_base_class(object_type,material)) {
					if(argument_type == typeof(parameter_argument)) {
						if(type_name == "material.stateToggle") return material.getName() + "." + parameter_argument;
						if(type_name == "material.stateSwitch") return material.getName() + "." + parameter_argument;
						if(type_name == "material.textureImage") return material.getName() + "." + parameter_argument;
						if(type_name == "material.parameterColor") return material.getName() + "." + parameter_argument;
						if(type_name == "material.parameterSlider") return material.getName() + "." + parameter_argument;
					}
					return replace(type_name,"material",material.getName());
				}
			}
			
			// property object
			if(object_type == "Property") {
				Property property = parameter_object;
				if(is_base_class(object_type,property)) {
					if(argument_type == typeof(parameter_argument)) {
						if(type_name == "property.stateToggle") return property.getName() + "." + parameter_argument;
						if(type_name == "property.stateSwitch") return property.getName() + "." + parameter_argument;
						if(type_name == "property.parameterFloat") return property.getName() + "." + parameter_argument;
						if(type_name == "property.parameterToggle") return property.getName() + "." + parameter_argument;
						if(type_name == "property.parameterSwitch") return property.getName() + "." + parameter_argument;
						if(type_name == "property.parameterString") return property.getName() + "." + parameter_argument;
						if(type_name == "property.parameterColor") return property.getName() + "." + parameter_argument;
					}
					return replace(type_name,"property",property.getName());
				}
			}
			
			// node parameters
			if(match("node.position*",type_name)) type_name = "node.position";
			if(match("node.rotation*",type_name)) type_name = "node.rotation";
			
			// node object
			Node node = parameter_object;
			if(is_base_class("Node",node) && is_base_class(object_type,node)) {
				if(strlen(node.getName())) return replace(type_name,"node",node.getName());
			}
			
			return type_name;
		}
		
		/*
		 */
		void setTrack(int track) {
			throw("Unigine::Tracker::TrackerEditorParameter::setTrack(): called\n");
		}
		
		int getTrack() {
			throw("Unigine::Tracker::TrackerEditorParameter::getTrack(): called\n");
		}
		
		/*
		 */
		void updateTrack() {
			throw("Unigine::Tracker::TrackerEditorParameter::updateTrack(): called\n");
		}
		
		void update_track_float(TrackerParameter::KeyFloat keys[],TrackCurve track_curve) {
			keys.delete();
			foreach(TrackKey key; track_curve.keys) {
				TrackerParameter::KeyFloat track_key = new TrackerParameter::KeyFloat();
				track_key.time = key.getTime();
				track_key.value = key.getValue();
				track_key.handles = key.getHandles();
				track_key.type = key.getType();
				keys.append(track_key);
			}
		}
		
		/*
		 */
		void updateCurve() {
			throw("Unigine::Tracker::TrackerEditorParameter::updateCurve(): called\n");
		}
		
		void update_curve_float(TrackCurve track_curve,LineCurveTrack line_curve,TrackerParameter::KeyFloat keys[]) {
			track_curve.clear();
			line_curve.clear();
			foreach(TrackerParameter::KeyFloat key; keys) {
				TrackKey track_key = new TrackKey(key.time,key.value);
				LineKeyTrack line_key = new LineKeyTrack(key.time,track_key);
				track_key.setData(line_key);
				track_key.setHandles(key.handles);
				track_key.setType(key.type);
				track_curve.addKey(track_key);
				line_curve.addKey(line_key);
			}
		}
		
		/*
		 */
		void renderTrack(float time) {
			
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_EDITOR_PARAMETER_H__ */
