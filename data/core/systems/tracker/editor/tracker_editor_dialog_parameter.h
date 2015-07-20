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


#ifndef __UNIGINE_TRACKER_EDITOR_DIALOG_PARAMETER_H__
#define __UNIGINE_TRACKER_EDITOR_DIALOG_PARAMETER_H__

/*
 */
namespace Unigine::Tracker {
	
	/*
	 */
	enum {
		
		EDITOR_DIALOG_PARAMETER_WIDTH = 240,	// dialog parameter width
		EDITOR_DIALOG_PARAMETER_HEIGHT = 320,	// dialog parameter height
	};
	
	/**************************************************************************\
	*
	* TrackerEditorDialogParameter
	*
	\**************************************************************************/
	
	/*
	 */
	class TrackerEditorDialogParameter : Unigine::Widgets::Dialog {
		
		ScrollBox types_sb;			// types scrollbox
		TreeBox types_tb;			// types treebox
		
		string parameters[];		// tracker parameters
		
		/*
		 */
		TrackerEditorDialogParameter(Tracker tracker,string str) : Dialog(str) {
			
			// types treebox
			types_sb = new ScrollBox();
			types_tb = new TreeBox();
			types_sb.setWidth(EDITOR_DIALOG_PARAMETER_WIDTH);
			types_sb.setHeight(EDITOR_DIALOG_PARAMETER_HEIGHT);
			types_tb.setCallback(CHANGED,functionid(callback_redirector),this,"types_changed");
			types_tb.setCallback(DOUBLE_CLICKED,functionid(callback_redirector),this,"types_double_clicked");
			types_sb.addChild(types_tb,ALIGN_EXPAND);
			addChild(types_sb,ALIGN_EXPAND);
			
			// tracker types
			string type_names[0];
			int has_track = 0;
			int has_node_position = 0;
			int has_node_rotation = 0;
			forloop(int i = 0; tracker.getNumParameters()) {
				TrackerParameter parameter = tracker.getParameter(i);
				string type_name = parameter.getTypeName();
				if(match("track*",type_name)) {
					if(has_track == 0) type_names.append("track");
					has_track = 1;
				} else if(match("node.position*",type_name)) {
					if(has_node_position == 0) type_names.append("node.position");
					has_node_position = 1;
				} else if(match("node.rotation*",type_name)) {
					if(has_node_rotation == 0) type_names.append("node.rotation");
					has_node_rotation = 1;
				} else {
					type_names.append(type_name);
				}
				parameters.append(type_name);
			}
			types_tb.setHierarchy(type_names,".");
		}
		
		~TrackerEditorDialogParameter() {
			
			delete types_sb;
			delete types_tb;
		}
		
		/*
		 */
		string getTypeName() {
			return types_tb.getHierarchy(".");
		}
		
		/*
		 */
		void types_changed() {
			Button button = getOkButton();
			string type_name = getTypeName();
			if(type_name == "node.position") button.setEnabled(1);
			else if(type_name == "node.rotation") button.setEnabled(1);
			else button.setEnabled(parameters.check(type_name));
		}
		
		/*
		 */
		void types_double_clicked() {
			Button button = getOkButton();
			int id = types_tb.getCurrentItem();
			if(types_tb.getNumItemChilds(id) == 0) button.runCallback(CLICKED);
			else types_tb.setItemFolded(id,!types_tb.isItemFolded(id));
		}
		
		/*
		 */
		void callback_redirector(TrackerEditorDialogSettings dialog,string name) {
			dialog.call(name);
		}
	};
	
} /* namespace Unigine::Tracker */

#endif /* __UNIGINE_TRACKER_EDITOR_DIALOG_PARAMETER_H__ */
