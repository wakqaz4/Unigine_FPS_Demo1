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


#ifndef __UNIGINE_SYSTEM_WALL_H__
#define __UNIGINE_SYSTEM_WALL_H__

//#define PROJECTION_USER

#ifdef HAS_APP_WALL || HAS_APP_PROJECTION || HAS_APP_SURROUND
	#include <core/scripts/projection.h>
#endif

#ifdef HAS_APP_PROJECTION
	#include <core/scripts/system/projection.h>
#endif

/*
 */
namespace Wall {
	
	float fov;							// fov
	float angle;						// angle
	float bezel_x;						// bezel x
	float bezel_y;						// bezel y
	
	WidgetEditLine fov_el;				// fov
	WidgetEditLine angle_el;			// angle
	WidgetEditLine bezel_x_el;			// bezel x
	WidgetEditLine bezel_y_el;			// bezel y
	WidgetButton projection_b;			// projection
	
	string changed_callback;			// changed callback
	string changed_world_callback;		// changed world callback
	string changed_editor_callback;		// changed editor callback
	
	/*
	 */
	void init(float f,float a) {
		
		// load config
		fov = engine.config.getFloat("wall_fov",f);
		angle = engine.config.getFloat("wall_angle",a);
		bezel_x = engine.config.getFloat("wall_bezel_x",0.0f);
		bezel_y = engine.config.getFloat("wall_bezel_y",0.0f);
		projection_b = projection_b;
		
		#ifdef HAS_APP_WALL
			
			// wall configuration
			int wall_width = engine.wall.getWidth();
			int wall_height = engine.wall.getHeight();
			if(wall_width == 1 && wall_height == 1) engine.wall.setPrimary(0,0);
			else if(wall_width == 1 && wall_height == 2) engine.wall.setPrimary(0,0);
			else if(wall_width == 2 && wall_height == 1) engine.wall.setPrimary(0,0);
			else if(wall_width == 2 && wall_height == 2) engine.wall.setPrimary(0,0);
			else if(wall_width == 3 && wall_height == 1) engine.wall.setPrimary(1,0);
			else if(wall_width == 3 && wall_height == 2) engine.wall.setPrimary(1,0);
			else if(wall_width == 4 && wall_height == 1) engine.wall.setPrimary(1,0);
			else if(wall_width == 5 && wall_height == 1) engine.wall.setPrimary(2,0);
			else log.error("Wall::init(): unknown wall configuration %dx%d\n",wall_width,wall_height);
			
			// update sliders
			if(fov_el != NULL) fov_el.setText(format("%.1f",fov));
			if(angle_el != NULL) angle_el.setText(format("%.1f",angle));
			if(bezel_x_el != NULL) bezel_x_el.setText(format("%.3f",bezel_x));
			if(bezel_y_el != NULL) bezel_y_el.setText(format("%.3f",bezel_y));
			if(fov_el != NULL) fov_el.setEnabled((wall_width > 1 || wall_height > 1));
			if(angle_el != NULL) angle_el.setEnabled((wall_width > 1 || wall_height > 1));
			if(bezel_x_el != NULL) bezel_x_el.setEnabled((wall_width > 1));
			if(bezel_y_el != NULL) bezel_y_el.setEnabled((wall_height > 1));
			if(projection_b != NULL) projection_b.setHidden(1);
			
		#elif HAS_APP_PROJECTION
			
			// projection configuration
			int projection_width = engine.projection.getWidth();
			if(projection_width == 1) engine.projection.setPrimary(0);
			else if(projection_width == 2) engine.projection.setPrimary(0);
			else if(projection_width == 3) engine.projection.setPrimary(1);
			else if(projection_width == 4) engine.projection.setPrimary(1);
			else if(projection_width == 5) engine.projection.setPrimary(2);
			else log.error("Wall::init(): unknown projection configuration %d\n",projection_width);
			
			// update sliders
			if(fov_el != NULL) fov_el.setText(format("%.1f",fov));
			if(angle_el != NULL) angle_el.setText(format("%.1f",angle));
			if(bezel_x_el != NULL) bezel_x_el.setText(format("%.3f",bezel_x));
			if(bezel_y_el != NULL) bezel_y_el.setText(format("%.3f",bezel_y));
			if(fov_el != NULL) fov_el.setEnabled((projection_width > 1));
			if(angle_el != NULL) angle_el.setEnabled((projection_width > 1));
			
			// init projection
			projectionInit();
			
		#elif HAS_APP_SURROUND
			
			// update sliders
			if(fov_el != NULL) fov_el.setText(format("%.1f",fov));
			if(angle_el != NULL) angle_el.setText(format("%.1f",angle));
			if(bezel_x_el != NULL) bezel_x_el.setText(format("%.3f",bezel_x));
			if(bezel_y_el != NULL) bezel_y_el.setText(format("%.3f",bezel_y));
			if(bezel_y_el != NULL) bezel_y_el.setEnabled(0);
			if(projection_b != NULL) projection_b.setHidden(1);
			
		#else
			
			// clear system menu
			forloop(int i = 0; System::main_tb.getNumTabs()) {
				if(System::main_tb.getTabText(i) == engine.gui.translate("Wall")) {
					System::main_tb.setTabHidden(i,1);
				}
			}
			
		#endif
	}
	
	void shutdown() {
		
		// save config
		engine.config.setFloat("wall_fov",fov);
		engine.config.setFloat("wall_angle",angle);
		engine.config.setFloat("wall_bezel_x",bezel_x);
		engine.config.setFloat("wall_bezel_y",bezel_y);
		
		// shutdown projection
		#ifdef HAS_APP_PROJECTION
			projectionShutdown();
		#endif
	}
	
	/*
	 */
	void render() {
		
		// don't handle projection
		#ifdef PROJECTION_USER
			return;
		#endif
		
		#ifdef HAS_APP_WALL || HAS_APP_PROJECTION || HAS_APP_SURROUND
			
			// enable render
			engine.render.setEnabled(1);
			
			// get player
			Player player = engine.editor.getPlayer();
			if(player == NULL) player = engine.game.getPlayer();
			if(player == NULL) return;
			
			// display configuration
			float aspect = float(engine.app.getWidth()) / engine.app.getHeight();
			
			// player matrices
			mat4 projection = perspective(fov,1.0f,player.getZNear(),player.getZFar());
			Mat4 modelview = player.getModelview();
			
		#endif
		
		#ifdef HAS_APP_WALL
			
			// wall configuration
			int wall_width = engine.wall.getWidth();
			int wall_height = engine.wall.getHeight();
			
			mat4 wall_projection = mat4_identity;
			Mat4 wall_modelview = Mat4_identity;
			
			/////////////////////////////////
			// 1x2 configuration
			/////////////////////////////////
			
			if(wall_width == 1 && wall_height == 2) {
				
				// disable engine render
				engine.render.setEnabled(0);
				
				// top bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,0,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,0,wall_projection,wall_modelview);
				
				// bottom bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,0,1,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,1,wall_projection,wall_modelview);
			}
			
			/////////////////////////////////
			// 2x1 configuration
			/////////////////////////////////
			
			else if(wall_width == 2 && wall_height == 1) {
				
				// disable engine render
				engine.render.setEnabled(0);
				
				// left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,0,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,0,wall_projection,wall_modelview);
				
				// right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,1,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(1,0,wall_projection,wall_modelview);
			}
			
			/////////////////////////////////
			// 2x2 configuration
			/////////////////////////////////
			
			else if(wall_width == 2 && wall_height == 2) {
				
				// disable engine render
				engine.render.setEnabled(0);
				
				// top left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,0,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,0,wall_projection,wall_modelview);
				
				// top right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,1,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(1,0,wall_projection,wall_modelview);
				
				// bottom left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,0,1,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,1,wall_projection,wall_modelview);
				
				// bottom right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,1,1,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(1,1,wall_projection,wall_modelview);
			}
			
			/////////////////////////////////
			// 3x1 configuration
			/////////////////////////////////
			
			else if(wall_width == 3 && wall_height == 1) {
				
				// primary display
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,1,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				engine.wall.setProjection(1,0,wall_projection);
				engine.wall.setModelview(1,0,wall_modelview);
				player.setProjection(wall_projection);
				player.setModelview(wall_modelview);
				
				// left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,0,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,0,wall_projection,wall_modelview);
				
				// right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,2,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(2,0,wall_projection,wall_modelview);
			}
			
			/////////////////////////////////
			// 3x2 configuration
			/////////////////////////////////
			
			else if(wall_width == 3 && wall_height == 2) {
				
				// disable engine render
				engine.render.setEnabled(0);
				
				// top bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,1,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(1,0,wall_projection,wall_modelview);
				
				// bottom bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,1,1,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(1,1,wall_projection,wall_modelview);
				
				// top left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,0,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,0,wall_projection,wall_modelview);
				
				// bottom left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,0,1,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,1,wall_projection,wall_modelview);
				
				// top right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,2,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(2,0,wall_projection,wall_modelview);
				
				// bottom right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,2,1,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(2,1,wall_projection,wall_modelview);
			}
			
			/////////////////////////////////
			// 4x1 configuration
			/////////////////////////////////
			
			else if(wall_width == 4 && wall_height == 1) {
				
				// disable engine render
				engine.render.setEnabled(0);
				
				// outer left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,0,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,0,wall_projection,wall_modelview);
				
				// inner left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,1,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(1,0,wall_projection,wall_modelview);
				
				// inner right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,2,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(2,0,wall_projection,wall_modelview);
				
				// outer right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,3,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(3,0,wall_projection,wall_modelview);
			}
			
			/////////////////////////////////
			// 5x1 configuration
			/////////////////////////////////
			
			else if(wall_width == 5 && wall_height == 1) {
				
				// primary display
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,2,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				engine.wall.setProjection(2,0,wall_projection);
				engine.wall.setModelview(2,0,wall_modelview);
				player.setProjection(wall_projection);
				player.setModelview(wall_modelview);
				
				// outer left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,0,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,0,wall_projection,wall_modelview);
				
				// inner left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,1,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(1,0,wall_projection,wall_modelview);
				
				// inner right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,3,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(3,0,wall_projection,wall_modelview);
				
				// outer right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,wall_width,wall_height,4,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(4,0,wall_projection,wall_modelview);
			}
			
		#elif HAS_APP_PROJECTION
			
			// projection configuration
			int projection_width = engine.projection.getWidth();
			
			mat4 wall_projection = mat4_identity;
			Mat4 wall_modelview = Mat4_identity;
			
			/////////////////////////////////
			// 1x1 configuration
			/////////////////////////////////
			
			if(projection_width == 1) {
				
				// primary display
				projection = perspective(player.getFov(),1.0f,player.getZNear(),player.getZFar());
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,0,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				engine.projection.setProjection(0,wall_projection);
				engine.projection.setModelview(0,wall_modelview);
				player.setProjection(wall_projection);
				player.setModelview(wall_modelview);
			}
			
			/////////////////////////////////
			// 2x1 configuration
			/////////////////////////////////
			
			else if(projection_width == 2) {
				
				// disable engine render
				engine.render.setEnabled(0);
				
				// left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,0,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,0,wall_projection,wall_modelview);
				
				// right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,1,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(1,0,wall_projection,wall_modelview);
			}
			
			/////////////////////////////////
			// 3x1 configuration
			/////////////////////////////////
			
			else if(projection_width == 3) {
				
				// primary display
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,1,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				engine.projection.setProjection(1,wall_projection);
				engine.projection.setModelview(1,wall_modelview);
				player.setProjection(wall_projection);
				player.setModelview(wall_modelview);
				
				// left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,0,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,0,wall_projection,wall_modelview);
				
				// right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,2,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(2,0,wall_projection,wall_modelview);
			}
			
			/////////////////////////////////
			// 4x1 configuration
			/////////////////////////////////
			
			else if(projection_width == 4) {
				
				// disable engine render
				engine.render.setEnabled(0);
				
				// outer left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,0,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,0,wall_projection,wall_modelview);
				
				// inner left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,1,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(1,0,wall_projection,wall_modelview);
				
				// inner right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,2,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(2,0,wall_projection,wall_modelview);
				
				// outer right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,3,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(3,0,wall_projection,wall_modelview);
			}
			
			/////////////////////////////////
			// 5x1 configuration
			/////////////////////////////////
			
			else if(projection_width == 5) {
				
				// primary display
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,2,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				engine.projection.setProjection(3,wall_projection);
				engine.projection.setModelview(3,wall_modelview);
				player.setProjection(wall_projection);
				player.setModelview(wall_modelview);
				
				// outer left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,0,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(0,0,wall_projection,wall_modelview);
				
				// inner left bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,1,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(1,0,wall_projection,wall_modelview);
				
				// inner right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,3,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(3,0,wall_projection,wall_modelview);
				
				// outer right bounding frustum
				Unigine::getWallProjection(projection,modelview,aspect,projection_width,1,4,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
				set_projection(4,0,wall_projection,wall_modelview);
			}
			
		#elif HAS_APP_SURROUND
			
			mat4 wall_projection = mat4_identity;
			Mat4 wall_modelview = Mat4_identity;
			
			// primary display
			Unigine::getWallProjection(projection,modelview,aspect,3,1,1,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
			engine.surround.setProjection(1,wall_projection);
			engine.surround.setModelview(1,wall_modelview);
			player.setProjection(wall_projection);
			player.setModelview(wall_modelview);
			
			// left bounding frustum
			Unigine::getWallProjection(projection,modelview,aspect,3,1,0,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
			set_projection(0,0,wall_projection,wall_modelview);
			
			// right bounding frustum
			Unigine::getWallProjection(projection,modelview,aspect,3,1,2,0,bezel_x,bezel_y,angle,wall_projection,wall_modelview);
			set_projection(2,0,wall_projection,wall_modelview);
			
		#endif
	}
	
	/*
	 */
	void set_projection(int x,int y,mat4 projection,Mat4 modelview) {
		
		#ifdef HAS_APP_WALL
			
			engine.wall.setEnabled(x,y,1);
			engine.wall.setProjection(x,y,projection);
			engine.wall.setModelview(x,y,modelview);
			
		#elif HAS_APP_PROJECTION
			
			engine.projection.setEnabled(x,1);
			engine.projection.setProjection(x,projection);
			engine.projection.setModelview(x,modelview);
			
		#elif HAS_APP_SURROUND
			
			engine.surround.setEnabled(x,1);
			engine.surround.setProjection(x,projection);
			engine.surround.setModelview(x,modelview);
			
		#endif
	}
	
	/*
	 */
	void changed() {
		if(changed_callback != NULL) call(changed_callback);
		if(changed_world_callback != NULL) engine.world.call(changed_world_callback);
		if(changed_editor_callback != NULL) engine.editor.call(changed_editor_callback);
	}
	
	/*
	 */
	void fov_pressed() {
		fov = clamp(float(fov_el.getText()),1.0f,179.0f);
		fov_el.setText(format("%.1f",fov));
		changed();
	}
	
	void angle_pressed() {
		angle = clamp(float(angle_el.getText()),-89.0f,89.0f);
		angle_el.setText(format("%.1f",angle));
		changed();
	}
	
	void bezel_x_pressed() {
		bezel_x = clamp(float(bezel_x_el.getText()),-1.0f,1.0f);
		bezel_x_el.setText(format("%.3f",bezel_x));
		#ifdef HAS_APP_PROJECTION
			projectionUpdate();
		#endif
		changed();
	}
	
	void bezel_y_pressed() {
		bezel_y = clamp(float(bezel_y_el.getText()),-1.0f,1.0f);
		bezel_y_el.setText(format("%.3f",bezel_y));
		#ifdef HAS_APP_PROJECTION
			projectionUpdate();
		#endif
		changed();
	}
	
	void projection_clicked() {
		#ifdef HAS_APP_PROJECTION
			projectionRun();
		#endif
	}
}

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void wallInit(float fov = 60.0f,float angle = 20.0f) {
	Wall::init(fov,angle);
}

void wallShutdown() {
	Wall::shutdown();
}

/*
 */
void wallRender() {
	Wall::render();
}

/*
 */
void wallSetFov(float fov) {
	Wall::fov = fov;
}

float wallGetFov() {
	return Wall::fov;
}

void wallSetAngle(float angle) {
	Wall::angle = angle;
}

float wallGetAngle() {
	return Wall::angle;
}

/*
 */
void wallSetBezelX(float bezel) {
	Wall::bezel_x = bezel;
}

float wallGetBezelX() {
	return Wall::bezel_x;
}

void wallSetBezelY(float bezel) {
	Wall::bezel_y = bezel;
}

float wallGetBezelY() {
	return Wall::bezel_y;
}

/*
 */
void wallSetChangedCallback(string callback) {
	Wall::changed_callback = callback;
}

string wallGetChangedCallback() {
	return Wall::changed_callback;
}

void wallSetChangedWorldCallback(string callback) {
	Wall::changed_world_callback = callback;
}

string wallGetChangedWorldCallback() {
	return Wall::changed_world_callback;
}

void wallSetChangedEditorCallback(string callback) {
	Wall::changed_editor_callback = callback;
}

string wallGetChangedEditorCallback() {
	return Wall::changed_editor_callback;
}

#endif /* __UNIGINE_SYSTEM_WALL_H__ */
