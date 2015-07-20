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


#include <core/unigine.h>
#include <core/scripts/projection.h>
#include <core/systems/syncker/syncker_slave.h>
#include <core/systems/syncker/syncker_slave_console.h>

/******************************************************************************\
*
* Initialize/shutdown
*
\******************************************************************************/

/*
 */
int init() {
	
	// disable the editor
	engine.editor.setEnabled(0);
	
	// initialize syncker console
	Unigine::Syncker::Slave::Console::init();
	
	// initialize syncker
	Unigine::Syncker::Slave::init(slave_name);
	
	return 1;
}

/*
 */
int shutdown() {
	
	// shutdown syncker console
	Unigine::Syncker::Slave::Console::shutdown();
	
	// shutdown syncker
	Unigine::Syncker::Slave::shutdown();
	
	return 1;
}

/******************************************************************************\
*
* Render
*
\******************************************************************************/

/*
 */
vec3 tangents[];
vec3 binormals[];
vec3 positions[];
string current_mesh;

/*
 */
int render() {
	
	// update syncker
	Unigine::Syncker::Slave::update();
	
	// update mesh
	if(current_mesh != slave_mesh) {
		tangents.clear();
		binormals.clear();
		positions.clear();
		Unigine::getMeshProjection(slave_mesh,tangents,binormals,positions);
		current_mesh = slave_mesh;
	}
	
	// update player
	Player player = engine.editor.getPlayer();
	if(player == NULL) player = engine.game.getPlayer();
	if(player != NULL) {
		mat4 projection;
		Mat4 modelview;
		if(Unigine::Syncker::Slave::hasPlayer()) {
			projection = player.getProjection();
			modelview = player.getModelview();
			if(tangents.check(slave_name)) {
				mat4 view_modelview;
				vec3 tangent = tangents[slave_name];
				vec3 binormal = binormals[slave_name];
				vec3 position = positions[slave_name];
				Unigine::getViewProjection(Vec3_zero,position,tangent,binormal,player.getZNear(),player.getZFar(),projection,view_modelview);
				modelview = view_modelview * rotateZ(90.0f) * rotateX(90.0f) * modelview;
			} else {
				int view_x = clamp(slave_view_x,0,slave_grid_x - 1);
				int view_y = clamp(slave_view_y,0,slave_grid_y - 1);
				float aspect = float(engine.app.getWidth()) / engine.app.getHeight();
				Unigine::getWallProjection(projection,modelview,aspect,slave_grid_x,slave_grid_y,view_x,view_y,slave_bezel_x,slave_bezel_y,slave_angle,projection,modelview);
			}
		}
		player.setProjection(projection);
		player.setModelview(modelview);
		#ifdef HAS_APP_PROJECTION
			engine.projection.setProjection(0,projection);
			engine.projection.setModelview(0,modelview);
		#endif
	}
	
	return 1;
}
