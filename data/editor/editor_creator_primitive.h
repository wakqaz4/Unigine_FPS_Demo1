/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_creator_primitive.h
 * Desc:    Unigine editor
 * Version: 1.02
 * Author:  Mikhail Lyubimov <alterego@unigine.com>
 *          Andrey Viktorov <unclebob@unigine.com>
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
namespace Creator {
	
	using Unigine::Widgets;
	
	namespace Primitive {
		
		UserInterface ui;
		
		void init() {
			ui = new UserInterface("editor/editor_creator_primitive.ui");
			hotkeysAddUiToolTips(ui);
		}
		
		void shutdown() {
			hotkeysRemoveUiToolTips(ui);
			delete ui;
		}
	}
	
	namespace PrimitiveBox {
		
		Dialog dialog;
		
		EditLine size_el[3];
		
		int show_dialog() {
			
			addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
			dialog.setPermanentFocus();
			
			while(dialog.isDone() == 0) wait 1;
			
			removeChild(dialog);
			
			return dialog.isOkClicked();
		}
		
		Node create() {
			if(!show_dialog()) return NULL;
			
			ObjectMeshDynamic object = node_remove(new ObjectMeshDynamic());
			Mesh mesh = new Mesh();
			vec3 size = vec3_one;
			size.x = abs(float(size_el[0].getText()));
			size.y = abs(float(size_el[1].getText()));
			size.z = abs(float(size_el[2].getText()));
			
			mesh.addBoxSurface("box",size);
			object.setMesh(mesh);
			object.setMaterial("mesh_pbr_base","*");
			object.setName("Cuboid");
			return object;
		}
	}
	
	namespace PrimitiveSphere {
		
		Dialog dialog;
		
		EditLine radius_el;
		EditLine segments_el;
		
		int show_dialog() {
			addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
			dialog.setPermanentFocus();
			
			while(dialog.isDone() == 0) wait 1;
			
			removeChild(dialog);
			
			return dialog.isOkClicked();
		}
		
		Node create() {
			if(!show_dialog()) return NULL;
			
			ObjectMeshDynamic object = node_remove(new ObjectMeshDynamic());
			Mesh mesh = new Mesh();
			
			float radius = abs(float(radius_el.getText()));
			float segments = float(segments_el.getText()) / 2.0f;
			
			mesh.addSphereSurface("sphere", radius, segments, segments);
			object.setMesh(mesh);
			object.setMaterial("mesh_pbr_base","*");
			object.setName("Sphere");
			return object;
		}
	}
	
	namespace PrimitivePlane {
		
		Dialog dialog;
		
		EditLine width_el;
		EditLine height_el;
		EditLine segments_el;
		
		int show_dialog() {
			addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
			dialog.setPermanentFocus();
			
			while(dialog.isDone() == 0) {
				int segments = int(segments_el.getText());
				if(segments > 1000) {
					segments = min(segments,1000);
					segments_el.setText(string(segments));
				}
				wait 1;
			}
			removeChild(dialog);
			
			return dialog.isOkClicked();
		}
		
		Node create() {
			if(!show_dialog()) return NULL;
			
			ObjectMeshDynamic object = node_remove(new ObjectMeshDynamic());
			Mesh mesh = new Mesh();
			
			float width = abs(float(width_el.getText()));
			float height = abs(float(height_el.getText()));
			float segments = float(segments_el.getText()) / 2.0f;
			
			float step = max(width,height) / segments;
			
			mesh.addPlaneSurface("plane",width,height,step);
			object.setMesh(mesh);
			object.setMaterial("mesh_pbr_base","*");
			object.setName("Plane");
			return object;
		}
	}
	
	namespace PrimitiveCylinder {
		
		Dialog dialog;
		
		EditLine radius_el;
		EditLine height_el;
		EditLine stacks_el;
		EditLine slices_el;
		
		int show_dialog() {
			addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
			dialog.setPermanentFocus();
			
			while(dialog.isDone() == 0) wait 1;
			
			removeChild(dialog);
			
			return dialog.isOkClicked();
		}
		
		Node create() {
			if(!show_dialog()) return NULL;
			
			ObjectMeshDynamic object = node_remove(new ObjectMeshDynamic());
			Mesh mesh = new Mesh();
			
			float radius = abs(float(radius_el.getText()));
			float height = abs(float(height_el.getText()));
			float stacks = float(stacks_el.getText());
			float slices = float(slices_el.getText());
			
			mesh.addCylinderSurface("cylinder",radius,height,stacks,slices);
			object.setMesh(mesh);
			object.setMaterial("mesh_pbr_base","*");
			object.setName("Cylinder");
			return object;
		}
	}
	
	namespace PrimitiveCapsule {
		
		Dialog dialog;
		
		EditLine radius_el;
		EditLine height_el;
		EditLine stacks_el;
		EditLine slices_el;
		
		int show_dialog() {
			addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
			dialog.setPermanentFocus();
			
			while(dialog.isDone() == 0) wait 1;
			
			removeChild(dialog);
			
			return dialog.isOkClicked();
		}
		
		Node create() {
			if(!show_dialog()) return NULL;
			
			ObjectMeshDynamic object = node_remove(new ObjectMeshDynamic());
			Mesh mesh = new Mesh();
			
			float radius = abs(float(radius_el.getText()));
			float height = abs(float(height_el.getText()));
			float stacks = float(stacks_el.getText());
			float slices = float(slices_el.getText());
			
			mesh.addCapsuleSurface ("capsule", radius, height, stacks, slices);
			object.setMesh(mesh);
			object.setMaterial("mesh_pbr_base","*");
			object.setName("Capsule");
			return object;
		}
	}
	
	void validate_editline(WidgetEditLine line, string par) {
		float bound[0];
		if(sscanf(par,"%f,%f",bound) < 2) return;
		
		float value = float(line.getText());
		
		if(value >= 0.0f && value <= bound[1]) return;
		
		line.setCallbackEnabled(GUI_CHANGED,0);
		
		int cursor = line.getCursor();
		string str = line.getText();
		line.setText(substr(str,0,cursor - 1) + substr(str,cursor,strlen(str) - cursor));
		line.setCursor(cursor - 1);
		
		line.setCallbackEnabled(GUI_CHANGED,1);
	}
	
	void validate_float_editline_focus_out(WidgetEditLine line, string par) {
		float bound[0];
		if(sscanf(par,"%f,%f",bound) < 2) return;
		
		line.setCallbackEnabled(GUI_CHANGED,0);
		
		float value = float(line.getText());
		line.setText(format("%.1f",clamp(value,bound[0],bound[1])));
		
		line.setCallbackEnabled(GUI_CHANGED,1);
	}
	
	void validate_int_editline_focus_out(WidgetEditLine line, string par) {
		int bound[0];
		if(sscanf(par,"%d,%d",bound) < 2) return;
		
		line.setCallbackEnabled(GUI_CHANGED,0);
		
		int value = int(line.getText());
		line.setText(string(clamp(value,bound[0],bound[1])));
		
		line.setCallbackEnabled(GUI_CHANGED,1);
	}
	
	void validate_spinbox(WidgetSpinBox box) {
		int value = box.getValue();
		if(value >= box.getMinValue() && value <= box.getMaxValue()) return;
		
		box.setCallbackEnabled(GUI_CHANGED,0);
		
		value = clamp(value,box.getMinValue(),box.getMaxValue());
		box.setValue(value);
		
		box.setCallbackEnabled(GUI_CHANGED,1);
	}
}