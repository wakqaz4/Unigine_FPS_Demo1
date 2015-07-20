/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_panels.h
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

#ifndef __EDITOR_PANELS_H__
#define __EDITOR_PANELS_H__

#include <editor/panels/editor_panel_base.h>
#include <editor/panels/editor_panel_navigation.h>
#include <editor/panels/editor_panel_camera.h>
#include <editor/panels/editor_panel_brush.h>
#include <editor/panels/editor_panel_placement.h>
#include <editor/panels/editor_panel_render.h>
#include <editor/panels/editor_panel_helpers.h>
#include <editor/panels/editor_panel_viewcube.h>
#include <editor/panels/editor_panel_resolution.h>
#include <editor/panels/editor_panel_status_bar.h>

#include <editor/editor_viewports.h>

/*
 */
namespace Panels {
	
	using Editor::Widgets;
	using Editor::Panels;
	
	/*
	 */
	PanelPlacement main_placement;
	PanelNavigation main_navigation;
	PanelCamera main_camera;
	PanelBrush main_brush;
	PanelRender main_render;
	PanelHelpers main_helpers;
	PanelViewCube main_viewcube;
	PanelStatusBar main_status_bar;
	PanelBase panels[0];
	
	/*
	 */
	void init() {
		WidgetViewportLayout layout = viewportsLayoutGet();
		
		main_placement = new PanelPlacement(layout);
		main_navigation = new PanelNavigation(layout);
		main_camera = new PanelCamera();
		main_brush = new PanelBrush(layout);
		main_render = new PanelRender(layout);
		main_helpers = new PanelHelpers(layout);
		main_viewcube = new PanelViewCube(layout);
		main_status_bar = new PanelStatusBar(layout);
		
		panels.append(main_placement);
		panels.append(main_navigation);
		panels.append(main_camera);
		panels.append(main_brush);
		panels.append(main_render);
		panels.append(main_helpers);
		panels.append(main_viewcube);
		panels.append(main_status_bar);
		
		forloop(int i = 0; viewportsGetNumWindows()) {
			WidgetViewportWindow viewport = viewportsGetWindow(i);
			
			panels.append(new PanelCamera(viewport));
			panels.append(new PanelResolution(viewport));
			viewport.getLayout().resize();
		}
	}
	
	void shutdown() {
		main_placement = NULL;
		main_navigation = NULL;
		main_camera = NULL;
		main_brush = NULL;
		
		panels.delete();
	}
	
	void world_init() {
		panels.call("worldInit");
	}
	
	/*
	 */
	void update(int need_reload) {
		panels.call("update",need_reload);
	}
	
	/*
	 */
	void update_nodes() {
		panels.call("update_nodes");
	}
	
	/*
	 */
	void render() {
		panels.call("render");
	}
	
	/*
	 */
	void cameras_reset() {
		foreach(PanelBase panel; panels) {
			if(panel is PanelCamera) PanelCamera(panel).reset();
		}
	}
	
	void cameras_sync(PanelCamera panel_camera) {
		foreach(PanelBase panel; panels) {
			if(!(panel is PanelCamera)) continue;
			if(panel_camera == panel) continue;
			PanelCamera(panel).sync(panel_camera);
		}
	}
}

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
Editor::Panels::PanelBrush panelsGetPanelBrush() {
	return Panels::main_brush;
}

/*
 */
void panelBrushUpdateHotkey() {
	Panels::main_brush.update_hotkey();
}

/*
 */
void panelsInit() {
	Panels::init();
}

void panelsShutdown() {
	Panels::shutdown();
}

void panelsUpdate(int need_reload) {
	Panels::update(need_reload);
}

void panelsRender() {
	Panels::render();
}

void panelsUpdateNodes() {
	Panels::update_nodes();
}

/*
 */
void panelsSwitchCameraSpeed() {
	Panels::main_navigation.switch_speed();
}

void panelsSetCameraSpeed(int num) {
	Panels::main_navigation.set_toggled(num);
}

/*
 */
int panelPlacementGetSelector() {
	return Panels::main_placement.getSelector();
}

int panelPlacementGetTranslator() {
	return Panels::main_placement.getTranslator();
}

int panelPlacementGetRotator() {
	return Panels::main_placement.getRotator();
}

int panelPlacementGetScaler() {
	return Panels::main_placement.getScaler();
}

int panelPlacementGetBasis() {
	return Panels::main_placement.getManipulatorBasis();
}

int panelPlacementSetAxisLock(string axis,int value) {
	return Panels::main_placement.set_axis_lock(axis,value);
}

void panelPlacementSetTranslatorEnabled(int mode) {
	Panels::main_placement.setTranslatorEnabled(mode);
}

void panelPlacementSetRotatorEnabled(int mode) {
	Panels::main_placement.setRotatorEnabled(mode);
}

void panelPlacementSetScalerEnabled(int mode) {
	if(Panels::main_placement != NULL) Panels::main_placement.setScalerEnabled(mode);
}

void panelPlacementSetManipulator(string name) {
	Panels::main_placement.setManipulator(name);
}

/*
 */
void panelPlacementSetSnapToSurface(int mode) {
	Panels::main_placement.setSnapToSurface(mode);
}

/*
 */
void panelPlacementSetSnapToSurfaceOffset(int offset) {
	Panels::main_placement.setSnapToSurfaceOffset(offset);
}

int panelPlacementGetSnapToSurfaceOffset() {
	return Panels::main_placement.getSnapToSurfaceOffset();
}

/*
 */
void panelPlacementSetSnapToSurfaceRelativeTransform(int value) {
	Panels::main_placement.setSnapToSurfaceRelativeTransform(value);
}

int panelPlacementGetSnapToSurfaceRelativeTransform() {
	return Panels::main_placement.getSnapToSurfaceRelativeTransform();
}

/*
 */
void panelPlacementSetPositionStep(float step) {
	Panels::main_placement.setPositionStep(step);
}

float panelPlacementGetPositionStep() {
	return Panels::main_placement.getPositionStep();
}

/*
 */
int panelPlacementGetPositionSnap() {
	return Panels::main_placement.getPositionSnap();
}

void panelPlacementSetPositionSnap(int mode) {
	Panels::main_placement.setPositionSnap(mode);
}

/*
 */
void panelPlacementTogglePositionSnap() {
	Panels::main_placement.togglePositionSnap();
}

/*
 */
int panelPlacementGetPositionGrid() { return 1; }
int panelPlacementGetPositionGridDepthTest() { return 1; }
int panelPlacementGetPositionGridShowFocused() { return 1; }

/*
 */
void panelPlacementSetRotationStep(float step) {
	Panels::main_placement.setRotationStep(step);
}

float panelPlacementGetRotationStep() {
	return Panels::main_placement.getRotationStep();
}

/*
 */
int panelPlacementSetRotationSnap(int mode) {
	return Panels::main_placement.setRotationSnap(mode);
}

int panelPlacementGetRotationSnap() {
	return Panels::main_placement.getRotationSnap();
}

/*
 */
void panelPlacementToggleRotationSnap() {
	Panels::main_placement.toggleRotationSnap();
}

/*
 */
int panelPlacementGetManipulatorPivot() {
	return Panels::main_placement.getManipulatorPivot();
}

/*
 */
void panelPlacementToggleManipulatorPivot() {
	Panels::main_placement.toggleManipulatorPivot();
}

/*
 */
void panelPlacementToggleManipulatorBasis() {
	Panels::main_placement.toggleManipulatorBasis();
}

/*
 */
void panelPlacementDropDown() {
	Panels::main_placement.dropDown();
}

/*
 */
void panelStatusBarInfoStart(string title) {
	Panels::main_status_bar.infoStart(title);
}

/*
 */
void panelStatusBarInfoEnd(string title) {
	Panels::main_status_bar.infoEnd(title);
}

/*
 */
void panelRenderSetMode(string mode) {
	Panels::main_render.setMode(mode);
}

/*
 */
void panelRenderToggleRenderMode() {
	Panels::main_render.toggleRenderMode();
}

/*
 */
void panelRenderSetBuffer(string name) {
	Panels::main_render.setBuffer(name);
}

/*
 */
void panelRenderUpdateItemsHotkeys() {
	Panels::main_render.updateItemsHotkeys();
}

/*
 */
void panelCamerasReset() {
	Panels::cameras_reset();
}

void panelCamerasSync(Editor::Panels::PanelCamera panel_camera) {
	Panels::cameras_sync(panel_camera);
}

/*
 */
void panelWorldInit() {
	Panels::world_init();
}

/*
 */
void panelViewCubeFocused() {
	return Panels::main_viewcube.is_mouse_over;
}


/*
 */
void panelHelpersToggleIconGizmo(string name) {
	Panels::main_helpers.toggle_icon_gizmo(name);
}

int helpersGetNodeInfo() {
	return Panels::main_helpers.node_info_cb.isChecked();
}

int helpersGetDynamicInfo() {
	return Panels::main_helpers.dynamic_info_cb.isChecked();
}

int helpersGetObjectBounds() {
	return Panels::main_helpers.object_bounds_cb.isChecked();
}

int helpersGetObjectWireframe() {
	return Panels::main_helpers.object_wireframe_cb.isChecked();
}

int helpersGetMeshSkinnedBones() {
	return Panels::main_helpers.mesh_skinned_bones_cb.isChecked();
}

int helpersGetParticlesPhysics() {
	return Panels::main_helpers.particles_physics_cb.isChecked();
}

void helpersToggleNodeInfo() {
	Unigine::Widgets::CheckBox cb = Panels::main_helpers.node_info_cb;
	cb.setChecked(!cb.isChecked());
}

void helpersToggleObjectWireframe() {
	Unigine::Widgets::CheckBox cb = Panels::main_helpers.object_wireframe_cb;
	cb.setChecked(!cb.isChecked());
}
void helpersToggleObjectBounds() {
	Unigine::Widgets::CheckBox cb = Panels::main_helpers.object_bounds_cb;
	cb.setChecked(!cb.isChecked());
}

void helpersToggleMeshBones() {
	Unigine::Widgets::CheckBox cb = Panels::main_helpers.mesh_skinned_bones_cb;
	cb.setChecked(!cb.isChecked());
}

#endif /* __EDITOR_PANELS_H__ */
