/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_cameras.h
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

#ifndef __EDITOR_CAMERAS_H__
#define __EDITOR_CAMERAS_H__

#include <editor/panels/editor_panel_base.h>
#include <editor/panels/editor_panel_navigation.h>
#include <editor/panels/editor_panel_camera.h>

/*
 */
namespace Cameras {
	
	/*
	 */
	float velocity[0] = (5.0f,50.0f,500.0f);
	float velocity_multiplier[0] = (4.0f,4.0f,4.0f);
	int velocity_index = 0;
	
	Player editor_camera;
	Player cameras[0];
	int old_size;
	int game_camera_controlled;
	
	ControlsDummy controls_dummy;
	Controls cameras_controls[];
	
	string config_world_item;
	
	/*
	 */
	void init() {
		forloop(int i = 0; 3) {
			velocity[i] = configGet(format("camera_velocity_%f",i),velocity[i]);
			velocity_multiplier[i] = configGet(format("camera_velocity_multiplier_%f",i),velocity_multiplier[i]);
		}
		
		velocity_index = configGet("camera_velocity_index",velocity_index);
		update_velocity();
		
		editor_camera = controlsGetEditorCamera();
		
		update_cameras();
		
		load_editor_camera();
		
		controls_dummy = new ControlsDummy();
	}
	
	void shutdown() {
		Player game_camera = engine.game.getPlayer();
		if(game_camera != NULL) {
			game_camera.setControlled(game_camera_controlled);
		}
		
		restore_controls();
		
		forloop(int i = 0; 3) {
			configSet(format("camera_velocity_%f",i),velocity[i]);
			configSet(format("camera_velocity_multiplier_%f",i),velocity_multiplier[i]);
		}
		
		configSet("camera_velocity_index",velocity_index);
		
		save_editor_camera();
		
		delete controls_dummy;
	}
	
	void load_editor_camera() {
		editor_camera.setViewportMask(configGet("camera_viewport_mask",editor_camera.getViewportMask()));
		editor_camera.setReflectionMask(configGet("camera_reflection_mask",editor_camera.getReflectionMask()));
		editor_camera.setSourceMask(configGet("camera_source_mask",editor_camera.getSourceMask()));
		editor_camera.setReverbMask(configGet("camera_reverb_mask",editor_camera.getReverbMask()));
		editor_camera.setFov(configGet("camera_fov",editor_camera.getFov()));
		editor_camera.setZNear(configGet("camera_znear",editor_camera.getZNear()));
		editor_camera.setZFar(configGet("camera_zfar",editor_camera.getZFar()));
		editor_camera.setZFar(configGet("camera_zfar",editor_camera.getZFar()));
	}
	
	void save_editor_camera() {
		configSet("camera_viewport_mask",editor_camera.getViewportMask());
		configSet("camera_reflection_mask",editor_camera.getReflectionMask());
		configSet("camera_source_mask",editor_camera.getSourceMask());
		configSet("camera_reverb_mask",editor_camera.getReverbMask());
		configSet("camera_fov",editor_camera.getFov());
		configSet("camera_znear",editor_camera.getZNear());
		configSet("camera_zfar",editor_camera.getZFar());
	}
	
	void update_velocity() {
		controlsSetCameraMinVelocity(velocity[velocity_index]);
		controlsSetCameraMaxVelocity(velocity[velocity_index] * velocity_multiplier[velocity_index]);
	}
	
	void update_cameras() {
		old_size = cameras.size();
		cameras.clear();
		
		Player game_camera = engine.game.getPlayer();
		if(game_camera != NULL) {
			cameras.append(game_camera);
		}
		
		cameras.append(editor_camera);
		
		forloop(int i = 0; engine.editor.getNumNodes()) {
			Node node = engine.editor.getNode(i);
			if(node.isPlayer() == 0) continue;
			
			Player p = node_cast(node);
			cameras.append(p);
			
			if(p.getType() == NODE_PLAYER_PERSECUTOR) {
				
				if(!cameras_controls.check(p)) {
					cameras_controls.append(p,p.getControls());
				}
				
				p.setControls(controls_dummy);
			}
		}
		
		forloop(int i = 0; cameras.size()) {
			Player player = cameras[i];
			if(player == NULL) continue;
			
			int is_ortho = 0;
			float h = 16.0f;
			string data = player.getData();
			if(data != "") {
				int value[0];
				sscanf(data,"%d %d",value);
				is_ortho = value[0];
				h = float(value[1]);
			}
			
			float fov = player.getFov();
			float znear = player.getZNear();
			float zfar = player.getZFar();
			
			mat4 projection;
			if(is_ortho) projection = ortho(-h / 2.0f, h / 2.0f, -h / 2.0f, h / 2.0f,znear,zfar);
			else projection = perspective(fov,1.0f,znear,zfar);
			
			player.setProjection(projection);
		}
	}
	
	int is_changed() {
		return old_size != cameras.size();
	}
	
	/*
	 */
	void set_velocity(float value) {
		velocity[velocity_index] = value;
		update_velocity();
	}
	
	float get_velocity() {
		return velocity[velocity_index];
	}
	
	/*
	 */
	void set_velocity_index(int i) {
		velocity_index = clamp(i,0,2);
		controlsSetCameraMinVelocity(get_velocity());
		update_velocity();
	}
	
	/*
	 */
	void remove_camera(Player camera) {
		
	}
	
	/*
	 */
	void restore_controls() {
		
		foreachkey(Player p; cameras_controls) {
			if(!engine.editor.isNode(p)) continue;
			p.setControls(cameras_controls[p]);
		}
		
		cameras_controls.clear();
	}
	
	/*
	 */
	void world_init() {
		Player game_camera = engine.game.getPlayer();
		if(game_camera != NULL) {
			game_camera_controlled = game_camera.isControlled();
			game_camera.setControlled(0);
		}
		
		config_world_item = replace(format("camera_world_%s_transform",getCurrentWorldName()),"/","_");
		if(dataGet("save_camera",false) == false || configIsItem(config_world_item) == false) {
			panelCamerasReset();
			return;
		}
		editor_camera.setWorldTransform(Mat4(configGet(config_world_item,mat4(translate(vec3_one)))));
		controlsSetCamera(editor_camera);
	}
	
	/*
	 */
	void world_save() {
		if(config_world_item == "") return;
		dataSet("save_camera",true);
	}
	
	/*
	 */
	void world_shutdown() {
		Player game_camera = engine.game.getPlayer();
		if(game_camera != NULL) {
			game_camera.setControlled(game_camera_controlled);
		}
		
		restore_controls();
		
		if(config_world_item == "") return;
		if(dataGet("save_camera",false) == false) return;
		configSet(config_world_item,mat4(editor_camera.getWorldTransform()));
		config_world_item = "";
	}
}

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void camerasInit() {
	Cameras::init();
}

void camerasShutdown() {
	Cameras::shutdown();
}

void camerasUpdate() {
	Cameras::update_cameras();
}

float camerasGetVelocity() {
	return Cameras::get_velocity();
}

void camerasSetVelocity(float value) {
	Cameras::set_velocity(value);
}

void camerasSetVelocityIndex(int i) {
	Cameras::set_velocity_index(i);
}

int camerasGetVelocityIndex() {
	return Cameras::velocity_index;
}

Player camerasGetCamera(int i) {
	return Cameras::cameras[i];
}

Player camerasGetEditorCamera() {
	return Cameras::editor_camera;
}

int camerasGetNumCameras() {
	return Cameras::cameras.size();
}

void camerasRemoveCamera(Player camera) {
	Cameras::remove_camera(camera);
}

int camerasIsChanged() {
	return Cameras::is_changed();
}

void camerasWorldInit() {
	Cameras::world_init();
}

void camerasWorldSave() {
	Cameras::world_save();
}

void camerasWorldShutdown() {
	Cameras::world_shutdown();
}

#endif /* __EDITOR_CAMERAS_H__ */
