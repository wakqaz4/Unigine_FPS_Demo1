/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_panel_camera.h
 * Desc:    Unigine editor
 * Version: 1.01
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

#ifndef __EDITOR_PANEL_CAMERA_H__
#define __EDITOR_PANEL_CAMERA_H__

#include <editor/panels/editor_panel_base.h>
#include <editor/widgets/editor_widget_viewport_layout.h>

/*
 */
namespace Editor::Panels {
	
	/**************************************************************************\
	*
	* class PanelCamera
	*
	\**************************************************************************/
	
	/*
	 */
	class PanelCamera : PanelBase {
		
		//
		enum {
			MAX_LENGTH_NAME = 18,
		};
		
		//
		using Unigine::Widgets;
		using Editor::Widgets
		
		Dialog dialog_settings;
		EditLine viewport_mask_el;
		EditLine reflection_mask_el;
		EditLine source_mask_el;
		EditLine reverb_mask_el;
		EditLine fov_el;
		EditLine fov_film_el;
		EditLine znear_el;
		EditLine zfar_el;
		EditLine name_el;
		EditLine height_el;
		Label height_l;
		CheckBox ortho_cb;
		GroupBox fov_gb;
		
		Icon lock_i;
		Icon settings_i;
		int is_game_camera;
		
		Widget groupbox;
		ComboBox camera_cb;
		
		WidgetViewportWindow viewport;
		
		WidgetViewportLayout layout;
		
		//
		PanelCamera(WidgetViewportWindow viewport_ = NULL) {
			
			init("editor/panels/editor_panel_camera.ui","Editor::Panels::PanelCamera::");
			
			viewport = viewport_;
			if(viewport == NULL) layout = viewportsLayoutGet();
			else layout = viewport.getLayout();
			
			layout.addWidget("panel_camera",groupbox);
			
			lock_camera_changed();
			
			layout.arrangePanel("panel_camera");
		}
		
		//
		Player getCurrentPlayer() {
			if(viewport == NULL) return controlsGetCameraTarget();
			return viewport.getViewportCamera();
		}
		
		//
		void reset() {
			Player editor_camera = camerasGetEditorCamera();
			Player game_camera = engine.game.getPlayer();
			if(game_camera != NULL) {
				editor_camera.setWorldTransform(game_camera.getWorldTransform());
			}
			if(viewport == NULL) controlsSetCamera(editor_camera);
			else viewport.setViewportCamera(editor_camera);
		}
		
		//
		void update_nodes() {
			settings_i.setEnabled(lock_i.isToggled());
			
			groupbox.setEnabled(engine.world.isLoaded());
			camera_cb.clear();
			
			int flag = false;
			Player current_player = getCurrentPlayer();
			Player game_camera = engine.game.getPlayer();
			
			forloop(int i = 0; camerasGetNumCameras()) {
				Player player = camerasGetCamera(i);
				string name = player.getName();
				
				if(strlen(name) > MAX_LENGTH_NAME) camera_cb.addItem(substr(name,0,MAX_LENGTH_NAME) + "...");
				else if(strlen(name)) camera_cb.addItem(name);
				else if(i == 0 && player == game_camera) camera_cb.addItem(TR("Game Camera"));
				else camera_cb.addItem(format(TR("Unnamed Player %d"),i));
				
				if((!is_game_camera || !flag) && player == current_player) {
					camera_cb.setCurrentItem(i);
					flag = true;
				}
			}
			
			if(!flag) set_camera(camerasGetEditorCamera());
			
			layout.arrangePanel("panel_camera");
		}
		
		//
		void set_camera(Player player) {
			if(getCurrentPlayer() == player) return;
			lock_i.setCallbackEnabled(CHANGED,0);
			lock_i.setCallbackEnabled(CLICKED,0);
			lock_i.setCallbackEnabled(PRESSED,0);
			if(camerasGetEditorCamera() == player) {
				lock_i.setToggled(1);
				lock_i.setEnabled(0);
				if(viewport == NULL) controlsSetCameraLock(0);
				else viewport.setCameraLock(0);
			} else {
				lock_i.setToggled(0);
				lock_i.setEnabled(1);
				if(viewport == NULL) controlsSetCameraLock(1);
				else viewport.setCameraLock(1);
			}
			lock_i.setCallbackEnabled(CHANGED,1);
			lock_i.setCallbackEnabled(CLICKED,1);
			lock_i.setCallbackEnabled(PRESSED,1);
			if(viewport == NULL) controlsSetCamera(player);
			else viewport.setViewportCamera(player);
		}
		
		//
		void lock_camera_changed() {
			if(viewport == NULL) controlsSetCameraLock(!lock_i.isToggled());
			else viewport.setCameraLock(!lock_i.isToggled());
			panelCamerasSync(this);
			update_nodes();
		}
		
		//
		void select_camera() {
			int item = camera_cb.getCurrentItem();
			set_camera(camerasGetCamera(item));
			is_game_camera = getCurrentPlayer() == engine.game.getPlayer() && item == 0;
			panelCamerasSync(this);
			
			settings_i.setEnabled(lock_i.isToggled());
		}
		
		void create_camera() {
			
			PlayerDummy player = node_remove(new PlayerDummy());
			Player current_player = getCurrentPlayer();
			
			template camera_copy_value<FUNC> {
				player.set ## FUNC(current_player.get ## FUNC());
			}
			
			camera_copy_value<ViewportMask>;
			camera_copy_value<ReflectionMask>;
			camera_copy_value<SourceMask>;
			camera_copy_value<ReverbMask>;
			camera_copy_value<Fov>;
			camera_copy_value<ZNear>;
			camera_copy_value<ZFar>;
			camera_copy_value<Projection>;
			camera_copy_value<Data>;
			
			player.setName(getNewNodeName("PlayerDummy"));
			if(viewport == NULL) player.setTransform(controlsGetModelview());
			else player.setTransform(viewport.getModelview());
			
			// undo/redo
			Node selection[] = ();
			nodesGetNodes(selection);
			
			Node last_selected = Nodes::node;
			Nodes::NodesState_ s = Nodes::create_state_new((player),selection,last_selected);
			Nodes::save_state(s);
			
			engine.editor.addNode(player);
			
			// update node
			nodesUpdate(1);
			forloop(int i = 0; selection.size()) {
				nodesSelectNew(selection[i],1);
			}
			
			engine.editor.needReload();
			
			lock_i.setToggled(0);
			lock_i.setEnabled(1);
			
			set_camera(player);
			
			update_nodes();
		}
		
		void settings_camera() {
			thread(functionid(settings_camera_thread),this);
		}
		
		//
		void settings_camera_thread(PanelCamera panel) {
			
			template set_starting_mask<NAME,FUNC> {
				int NAME = player.get ## FUNC();
				panel.NAME ## _mask_el.setText(editorFormatMask(NAME));
			}
			
			template set_starting_string<NAME,FUNC> {
				string NAME = player.get ## FUNC();
				panel.NAME ## _el.setText(NAME);
			}
			
			template update_mask<NAME,FUNC> {
				if(panel.NAME ## _mask_el.isFocused()) player.set ## FUNC(int("0x" + panel.NAME ## _mask_el.getText()));
				else panel.NAME ## _mask_el.setText(editorFormatMask(player.get ## FUNC()));
			}
			
			template update_string<NAME,FUNC> {
				if(panel.NAME ## _el.isFocused()) player.set ## FUNC(panel.NAME ## _el.getText());
				else panel.NAME ## _el.setText(player.get ## FUNC());
			}
			
			template revert_value<NAME,FUNC> {
				player.set ## FUNC(NAME);
			}
			
			Player player = panel.getCurrentPlayer();
			Dialog dialog = panel.dialog_settings;
			
			set_starting_mask<viewport,ViewportMask>;
			set_starting_mask<reflection,ReflectionMask>;
			set_starting_mask<source,SourceMask>;
			set_starting_mask<reverb,ReverbMask>;
			set_starting_string<name,Name>;
			
			int is_ortho = 0;
			float h = 16.0f;
			string data = player.getData();
			if(data != "" ) {
				int value[0];
				sscanf(data,"%d %d",value);
				is_ortho = value[0];
				h = float(value[1]);
			}
			
			panel.ortho_cb.setChecked(is_ortho);
			panel.height_el.setText(editorFormat(h));
			panel.fov_el.setText(editorFormat(player.getFov()));
			panel.znear_el.setText(editorFormat(player.getZNear()));
			panel.zfar_el.setText(editorFormat(player.getZFar()));
			
			addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
			dialog.setPermanentFocus();
			
			while(dialog.isDone() == 0) {
				
				update_mask<viewport,ViewportMask>;
				update_mask<reflection,ReflectionMask>;
				update_mask<source,SourceMask>;
				update_mask<reverb,ReverbMask>;
				update_string<name,Name>;
				
				panel.fov_gb.setHidden(panel.ortho_cb.isChecked());
				panel.height_l.setHidden(!panel.ortho_cb.isChecked());
				panel.height_el.setHidden(!panel.ortho_cb.isChecked());
				
				wait 1;
			}
			
			if(!dialog.isOkClicked()) {
				revert_value<viewport,ViewportMask>;
				revert_value<reflection,ReflectionMask>;
				revert_value<source,SourceMask>;
				revert_value<reverb,ReverbMask>;
				revert_value<name,Name>;
				
				float fov = player.getFov();
				float znear = player.getZNear();
				float zfar = player.getZFar();
				
				mat4 projection;
				if(is_ortho) projection = ortho(-h / 2.0f, h / 2.0f, -h / 2.0f, h / 2.0f,znear,zfar);
				else projection = perspective(fov,1.0f,znear,zfar);
				
				player.setProjection(projection);
				
			} else {
				h = float(panel.height_el.getText());
				is_ortho = int(panel.ortho_cb.isChecked());
				player.setData(format("%d %f",is_ortho,h));
				
				float fov = float(panel.fov_el.getText());
				float znear = float(panel.znear_el.getText());
				float zfar = float(panel.zfar_el.getText());
				
				player.setFov(fov);
				player.setZNear(znear);
				player.setZFar(zfar);
				
				panel.update_projection();
				nodesReload();
			}
			
			panel.layout.arrangePanel("panel_camera");
			
			removeChild(dialog);
		}
		
		//
		void update_projection() {
			Player player = getCurrentPlayer();
			
			float fov = float(fov_el.getText());
			
			float aspect = 8.0f / 9.0f;
			if(fov_film_el.isFocused()) {
				float fov_film = float(fov_film_el.getText());
				fov = atan(16.0f / (aspect * fov_film)) * RAD2DEG * 2.0f;
				fov_el.setCallbackEnabled(CHANGED,0);
				fov_el.setText(editorFormat(fov));
				fov_el.setCallbackEnabled(CHANGED,1);
			} else {
				float fov_film = 16.0f / (tan(fov * DEG2RAD / 2.0f ) * aspect);
				fov_film_el.setCallbackEnabled(CHANGED,0);
				fov_film_el.setText(editorFormat(fov_film));
				fov_film_el.setCallbackEnabled(CHANGED,1);
			}
			
			float h = float(height_el.getText());
			float znear = float(znear_el.getText());
			float zfar = float(zfar_el.getText());
			
			mat4 projection;
			if(ortho_cb.isChecked()) projection = ortho(-h / 2.0f, h / 2.0f, -h / 2.0f, h / 2.0f,znear,zfar);
			else projection = perspective(fov,1.0f,znear,zfar);
			
			player.setProjection(projection);
		}
		
		void sync(PanelCamera panel) {
			if(getActiveCameraIndex() != panel.getActiveCameraIndex()) return;
			if(isLocked() != panel.isLocked()) lock_i.setToggled(!panel.isLocked());
		}
		
		//
		template callback_mask<WIDGET,FUNC,TITLE> {
			void WIDGET ## _changed(int mask) {
				Player current_player = getCurrentPlayer();
				current_player.set ## FUNC(mask);
				WIDGET ## _el.setText(editorFormatMask(current_player.get ## FUNC()));
			}
			void WIDGET ## _clicked() {
				int old_mask = getCurrentPlayer().get ## FUNC();
				dialogMask(TR(TITLE),functionid(WIDGET ## _changed),old_mask,this);
			}
		}
		
		callback_mask<viewport_mask,ViewportMask,"Viewport mask">;
		callback_mask<reflection_mask,ReflectionMask,"Reflection mask">;
		callback_mask<source_mask,SourceMask,"Source mask">;
		callback_mask<reverb_mask,ReverbMask,"Reverb mask">;
		
		int getActiveCameraIndex() {
			return camera_cb.getCurrentItem();
		}
		
		int isLocked() {
			return !lock_i.isToggled();
		}
	};
	
} /* namespace Editor::Panels */

#endif /* __EDITOR_PANEL_CAMERA_H__ */
