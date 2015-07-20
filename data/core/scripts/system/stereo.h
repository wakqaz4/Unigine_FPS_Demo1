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


#ifndef __UNIGINE_SYSTEM_STEREO_H__
#define __UNIGINE_SYSTEM_STEREO_H__

//#define STEREO_HORIZONTAL
//#define STEREO_VERTICAL
//#define STEREO_INTERLACED
//#define STEREO_QUAD_BUFFER
//#define STEREO_SEPARATE
//#define STEREO_ANAGLYPH
//#define STEREO_3D_VISION
//#define STEREO_3D_SURROUND
//#define STEREO_OCULUS

#ifdef HAS_APP_3D_SURROUND && STEREO_3D_SURROUND
	#include <core/scripts/projection.h>
#endif

/*
 */
namespace Stereo {
	
	float depth;					// depth
	
	float distance;					// distance
	float radius;					// radius
	float offset;					// offset
	float angle;					// angle
	float bezel;					// bezel
	
	WidgetEditLine distance_el;		// distance
	WidgetEditLine radius_el;		// radius
	WidgetEditLine offset_el;		// offset
	WidgetEditLine angle_el;		// angle
	WidgetEditLine bezel_el;		// bezel
	
	/*
	 */
	void init(float d,float r,float a) {
		
		depth = 1.0f;
		
		// load config
		distance = engine.config.getFloat("stereo_distance",d);
		radius = engine.config.getFloat("stereo_radius",r);
		offset = engine.config.getFloat("stereo_offset",0.0f);
		angle = engine.config.getFloat("stereo_angle",a);
		bezel = engine.config.getFloat("stereo_bezel",0.0f);
		
		#ifdef STEREO_HORIZONTAL || STEREO_VERTICAL || STEREO_INTERLACED || STEREO_QUAD_BUFFER || STEREO_SEPARATE || STEREO_ANAGLYPH || STEREO_3D_VISION || STEREO_3D_SURROUND
			
			// update sliders
			if(distance_el != NULL) distance_el.setText(format("%.2f",distance));
			if(radius_el != NULL) radius_el.setText(format("%.3f",radius));
			if(offset_el != NULL) offset_el.setText(format("%.3f",offset));
			if(angle_el != NULL) angle_el.setText(format("%.1f",angle));
			if(bezel_el != NULL) bezel_el.setText(format("%.3f",bezel));
			
			#ifndef STEREO_3D_SURROUND
				if(angle_el != NULL) angle_el.setEnabled(0);
				if(bezel_el != NULL) bezel_el.setEnabled(0);
			#endif
			
		#else
			
			// clear system menu
			forloop(int i = 0; System::main_tb.getNumTabs()) {
				if(System::main_tb.getTabText(i) == engine.gui.translate("Stereo")) {
					System::main_tb.setTabHidden(i,1);
				}
			}
			
			// configure Oculus
			#ifdef HAS_APP_OCULUS
				engine.oculus.setEnabledCaps(OCULUS_HMD_CAP_LOW_PERSISTENCE | OCULUS_HMD_CAP_DYNAMIC_PREDICTION);
				engine.oculus.configureTracking(OCULUS_TRACKING_CAP_POSITION | OCULUS_TRACKING_CAP_ROTATION | OCULUS_TRACKING_CAP_MAG_YAW_CORRECTION,0);
			#endif
			
		#endif
	}
	
	void shutdown() {
		
		// save config
		engine.config.setFloat("stereo_distance",distance);
		engine.config.setFloat("stereo_radius",radius);
		engine.config.setFloat("stereo_offset",offset);
		engine.config.setFloat("stereo_angle",angle);
		engine.config.setFloat("stereo_bezel",bezel);
	}
	
	/*
	 */
	void update() {
		
		// get post materials
		string post_materials = engine.render.getPostMaterials();
		post_materials = replace(post_materials,"post_stereo_separate","");
		post_materials = replace(post_materials,"post_stereo_horizontal","");
		post_materials = replace(post_materials,"post_stereo_vertical","");
		post_materials = replace(post_materials,"post_stereo_interlaced","");
		post_materials = replace(post_materials,"post_stereo_anaglyph","");
		post_materials = replace(post_materials,"post_stereo_replicate","");
		post_materials = replace(post_materials,",,",",");
		
		#ifdef HAS_APP_3D_VISION && (STEREO_3D_VISION || STEREO_3D_SURROUND)
			
			// control stereo
			if(engine.stereo.isEnabled()) {
				if(engine.render.getStereo() == RENDER_STEREO_DISABLED) {
					engine.console.run("render_stereo %d && render_restart",RENDER_STEREO_ENABLED);
				}
			} else {
				if(engine.render.getStereo() != RENDER_STEREO_DISABLED) {
					engine.console.run("render_stereo %d && render_restart",RENDER_STEREO_DISABLED);
				}
			}
			
			// stereo sliders
			if(engine.stereo.isEnabled()) {
				if(distance_el != NULL) distance_el.setEnabled(1);
				if(radius_el != NULL) radius_el.setEnabled(1);
				if(offset_el != NULL) offset_el.setEnabled(1);
			} else {
				if(distance_el != NULL) distance_el.setEnabled(0);
				if(radius_el != NULL) radius_el.setEnabled(0);
				if(offset_el != NULL) offset_el.setEnabled(0);
			}
			
			// post material
			if(engine.render.getStereo() == RENDER_STEREO_DISABLED) {
				post_materials += ",post_stereo_replicate";
			} else {
				post_materials += ",post_stereo_separate";
			}
			
			// stereo depth
			depth = engine.stereo.getSeparation() / 30.0f;
			
			// set stereo parameters
			engine.render.setStereoDistance(distance);
			engine.render.setStereoRadius(radius * depth);
			engine.render.setStereoOffset(offset);
			
			#ifdef HAS_APP_3D_SURROUND && STEREO_3D_SURROUND
				
				// post materials
				if(engine.stereo.isEnabled()) {
					engine.surround.setMaterials(0,"post_stereo_separate");
					engine.surround.setMaterials(2,"post_stereo_separate");
				} else {
					engine.surround.setMaterials(0,"post_stereo_replicate");
					engine.surround.setMaterials(2,"post_stereo_replicate");
				}
				
			#endif
			
		#elif STEREO_HORIZONTAL || STEREO_VERTICAL || STEREO_INTERLACED || STEREO_QUAD_BUFFER || STEREO_SEPARATE || STEREO_ANAGLYPH || STEREO_3D_VISION || STEREO_3D_SURROUND || STEREO_OCULUS
			
			// enable stereo
			#ifdef STEREO_HORIZONTAL
				if(engine.render.getStereo() != RENDER_STEREO_HORIZONTAL) {
					engine.console.run("render_stereo %d && render_restart",RENDER_STEREO_HORIZONTAL);
				}
			#elif STEREO_VERTICAL || STEREO_INTERLACED
				if(engine.render.getStereo() != RENDER_STEREO_VERTICAL) {
					engine.console.run("render_stereo %d && render_restart",RENDER_STEREO_VERTICAL);
				}
			#else
				if(engine.render.getStereo() != RENDER_STEREO_ENABLED) {
					engine.console.run("render_stereo %d && render_restart",RENDER_STEREO_ENABLED);
				}
			#endif
			
			// post material
			if(engine.render.getStereo() != RENDER_STEREO_DISABLED) {
				#ifdef STEREO_HORIZONTAL
					post_materials += ",post_stereo_horizontal";
				#elif STEREO_VERTICAL
					post_materials += ",post_stereo_vertical";
				#elif STEREO_INTERLACED
					post_materials += ",post_stereo_interlaced";
				#elif STEREO_QUAD_BUFFER || STEREO_SEPARATE || STEREO_3D_VISION || STEREO_3D_SURROUND || STEREO_OCULUS
					post_materials += ",post_stereo_separate";
				#elif STEREO_ANAGLYPH
					post_materials += ",post_stereo_anaglyph";
				#endif
			} else {
				post_materials += ",post_stereo_replicate";
			}
			
			// set stereo parameters
			engine.render.setStereoDistance(distance);
			engine.render.setStereoRadius(radius * depth);
			engine.render.setStereoOffset(offset);
			
		#else
			
			// disable stereo
			if(engine.render.getStereo() != RENDER_STEREO_DISABLED) {
				engine.console.run("render_stereo %d && render_restart",RENDER_STEREO_DISABLED);
			}
			
		#endif
		
		// set post materials
		engine.render.setPostMaterials(post_materials);
	}
	
	/*
	 */
	void render() {
		
		#ifdef HAS_APP_3D_SURROUND && STEREO_3D_SURROUND
			
			// get player
			Player player = engine.editor.getPlayer();
			if(player == NULL) player = engine.game.getPlayer();
			if(player == NULL) return;
			
			// display configuration
			float aspect = float(engine.app.getWidth()) / engine.app.getHeight();
			
			// player matrices
			mat4 projection = player.getProjection();
			Mat4 modelview = player.getModelview();
			
			mat4 wall_projection = mat4_identity;
			Mat4 wall_modelview = Mat4_identity;
			
			// primary display
			Unigine::getWallProjection(projection,modelview,aspect,3,1,1,0,bezel,0.0f,angle,wall_projection,wall_modelview);
			engine.surround.setProjection(1,wall_projection);
			engine.surround.setModelview(1,wall_modelview);
			player.setProjection(wall_projection);
			player.setModelview(wall_modelview);
			
			// left bounding frustum
			Unigine::getWallProjection(projection,modelview,aspect,3,1,0,0,bezel,0.0f,angle,wall_projection,wall_modelview);
			set_projection(0,projection.m00,wall_projection,wall_modelview);
			
			// right bounding frustum
			Unigine::getWallProjection(projection,modelview,aspect,3,1,2,0,bezel,0.0f,angle,wall_projection,wall_modelview);
			set_projection(2,projection.m00,wall_projection,wall_modelview);
			
		#endif
		
		#ifdef HAS_APP_OCULUS
			
			// hide health safety warning
			if(engine.oculus.getHSWDisplayState()) {
				engine.oculus.dismissHSWDisplay();
			}
			
			// reset sensor
			if(System::Controls::aux_f_b != NULL) {
				System::Controls::aux_f_b.setEnabled(1);
				if(engine.controls.getState(CONTROLS_STATE_AUX_F)) engine.oculus.resetTracking();
			}
			
			// handle mouse
			#ifdef MOUSE_SOFT
				if(engine.app.getMouseShow()) engine.gui.setMouseCursor(GUI_CURSOR_ARROW);
				engine.app.setMouseShow(0);
			#endif
			
			// default parameters
			float znear = 0.1f;
			float zfar = 10000.0f;
			Mat4 modelview = Mat4_identity;
			
			// get player
			Player player = engine.editor.getPlayer();
			if(player == NULL) player = engine.game.getPlayer();
			
			// player parameters
			if(player != NULL) {
				znear = player.getZNear();
				zfar = player.getZFar();
				modelview = player.getModelview();
			}
			
			// oculus projection matrices
			vec3 left_fov = engine.oculus.getEyeFov(0);
			vec3 right_fov = engine.oculus.getEyeFov(1);
			mat4 projection = engine.oculus.getProjection(max(left_fov,right_fov),znear,zfar);
			mat4 left_projection = engine.oculus.getProjection(left_fov,znear,zfar);
			mat4 right_projection = engine.oculus.getProjection(right_fov,znear,zfar);
			
			float aspect = float(engine.app.getWidth()) / engine.app.getHeight();
			projection.m00 *= aspect;
			left_projection.m00 *= aspect;
			right_projection.m00 *= aspect;
			
			engine.oculus.setProjection(projection);
			engine.oculus.setLeftProjection(left_projection);
			engine.oculus.setRightProjection(right_projection);
			
			// oculus modelview matrices
			vec3 position = engine.oculus.getHeadPosition();
			quat rotation = engine.oculus.getHeadRotation();
			mat4 offset = mat4(inverse(rotation)) * translate(-position);
			engine.oculus.setModelview(offset * modelview);
			
			position = engine.oculus.getEyePosition(0);
			rotation = engine.oculus.getEyeRotation(0);
			engine.oculus.setLeftModelview(mat4(inverse(rotation)) * translate(-position) * modelview);
			
			position = engine.oculus.getEyePosition(1);
			rotation = engine.oculus.getEyeRotation(1);
			engine.oculus.setRightModelview(mat4(inverse(rotation)) * translate(-position) * modelview);
			
			// update player
			if(player != NULL) {
				player.setProjection(projection);
				player.setOffset(offset);
			}
			
		#else
			
			// disable sensor
			if(System::Controls::aux_f_b != NULL) {
				System::Controls::aux_f_b.setEnabled(0);
			}
			
		#endif
	}
	
	/*
	 */
	void set_projection(int num,float scale,mat4 projection,Mat4 modelview) {
		
		#ifdef HAS_APP_3D_SURROUND
			
			float idistance = 1.0f / distance;
			
			mat4 projections[2];
			projections[0] = projection;
			projections[1] = projection;
			projections[0].m02 += scale * radius * depth * idistance + offset;
			projections[1].m02 -= scale * radius * depth * idistance + offset;
			
			Mat4 modelviews[2];
			modelviews[0] = Mat4(translate(radius * depth,0.0f,0.0f)) * modelview;
			modelviews[1] = Mat4(translate(-radius * depth,0.0f,0.0f)) * modelview;
			
			engine.surround.setEnabled(num,1);
			engine.surround.setProjection(num,projection);
			engine.surround.setLeftProjection(num,projections[0]);
			engine.surround.setRightProjection(num,projections[1]);
			engine.surround.setModelview(num,modelview);
			engine.surround.setLeftModelview(num,modelviews[0]);
			engine.surround.setRightModelview(num,modelviews[1]);
			
		#endif
	}
	
	/*
	 */
	void distance_pressed() {
		distance = float(distance_el.getText());
		distance_el.setText(format("%.2f",distance));
	}
	
	void radius_pressed() {
		radius = float(radius_el.getText());
		radius_el.setText(format("%.3f",radius));
	}
	
	void offset_pressed() {
		offset = float(offset_el.getText());
		offset_el.setText(format("%.3f",offset));
	}
	
	void angle_pressed() {
		angle = float(angle_el.getText());
		angle_el.setText(format("%.1f",angle));
	}
	
	void bezel_pressed() {
		bezel = float(bezel_el.getText());
		bezel_el.setText(format("%.3f",bezel));
	}
}

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void stereoInit(float distance = 4.0f,float radius = 0.032f,float angle = 20.0f) {
	Stereo::init(distance,radius,angle);
}

void stereoShutdown() {
	Stereo::shutdown();
}

/*
 */
void stereoUpdate() {
	Stereo::update();
}

/*
 */
void stereoRender() {
	Stereo::render();
}

#endif /* __UNIGINE_SYSTEM_STEREO_H__ */
