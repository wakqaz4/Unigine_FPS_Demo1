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


#ifndef __UNIGINE_UTILS_H__
#define __UNIGINE_UTILS_H__

#include <core/unigine.h>

/*
 */
namespace Unigine {
	
	/*
	 */
	Player getPlayer() {
		Player player = engine.editor.getPlayer();
		if(player == NULL) player = engine.game.getPlayer();
		return player;
	}
	
	int getPlayerMouseDirection(Vec3 &p0,Vec3 &p1) {
		Player player = getPlayer();
		if(player == NULL) return 0;
		int width = engine.app.getWidth();
		int height = engine.app.getHeight();
		int mouse_x = engine.app.getMouseX();
		int mouse_y = engine.app.getMouseY();
		mat4 projection = player.getProjection();
		mat4 imodelview = player.getIModelview() * inverse(player.getOffset());
		#ifdef HAS_APP_WALL
			if(engine.wall.getWidth() > 1 || engine.wall.getHeight() > 1) {
				int wall_x = clamp(engine.wall.getPrimaryX() + mouse_x / width - (mouse_x < 0),0,engine.wall.getWidth() - 1);
				int wall_y = clamp(engine.wall.getPrimaryY() + mouse_y / height - (mouse_y < 0),0,engine.wall.getHeight() - 1);
				projection = engine.wall.getProjection(wall_x,wall_y);
				imodelview = inverse(engine.wall.getModelview(wall_x,wall_y));
				mouse_x -= width * (wall_x - engine.wall.getPrimaryX());
				mouse_y -= height * (wall_y - engine.wall.getPrimaryY());
			}
		#elif HAS_APP_PROJECTION
			if(engine.projection.getWidth() > 1) {
				int wall_x = clamp(engine.projection.getPrimary() + mouse_x / width - (mouse_x < 0),0,engine.projection.getWidth() - 1);
				projection = engine.projection.getProjection(wall_x);
				imodelview = inverse(engine.projection.getModelview(wall_x));
				mouse_x -= width * (wall_x - engine.projection.getPrimary());
			}
		#elif HAS_APP_SURROUND || HAS_APP_3D_SURROUND
			int wall_x = clamp(1 + mouse_x / width - (mouse_x < 0),0,2);
			projection = engine.surround.getProjection(wall_x);
			imodelview = inverse(engine.surround.getModelview(wall_x));
			mouse_x -= width * (wall_x - 1);
		#endif
		projection.m00 *= float(height) / width;
		float x = -(float(mouse_x) / width * 2.0f - 1.0f + projection.m02) / projection.m00;
		float y = (float(mouse_y) / height * 2.0f - 1.0f + projection.m12) / projection.m11;
		if(projection.m32 == 0.0f) {
			p0 = imodelview * Vec3(-x,-y,-1.0f);
			p1 = imodelview * Vec3(-x,-y,1.0f);
		} else {
			p0 = imodelview.m03m13m23;
			p1 = imodelview * Vec3(x,y,1.0f);
		}
		p1 = p0 - normalize(p1 - p0) * player.getZFar();
		return 1;
	}
	
	int getPlayerMouseSelection(float x0,float y0,float x1,float y1,mat4 &projection) {
		Player player = getPlayer();
		if(player == NULL) return 0;
		int width = engine.app.getWidth();
		int height = engine.app.getHeight();
		projection = player.getProjection();
		projection.m00 *= float(height) / width;
		vec3 points[8];
		points[0] = vec3(-1.0f,-1.0f,-1.0f);
		points[1] = vec3( 1.0f,-1.0f,-1.0f);
		points[2] = vec3(-1.0f, 1.0f,-1.0f);
		points[3] = vec3( 1.0f, 1.0f,-1.0f);
		points[4] = vec3(-1.0f,-1.0f, 1.0f);
		points[5] = vec3( 1.0f,-1.0f, 1.0f);
		points[6] = vec3(-1.0f, 1.0f, 1.0f);
		points[7] = vec3( 1.0f, 1.0f, 1.0f);
		mat4 iprojection = inverse(projection);
		forloop(int i = 0; 8) {
			vec4 p = iprojection * vec4(points[i]);
			points[i] = vec3(p) / p.w;
		}
		float dx = points[1].x - points[0].x;
		float dy = points[0].y - points[2].y;
		x0 = points[0].x + dx * x0;
		x1 = points[0].x + dx * x1;
		y0 = points[2].y + dy * y0;
		y1 = points[2].y + dy * y1;
		if(projection.m32 == 0.0f) projection = ortho(x0,x1,y0,y1,-points[0].z,-points[4].z);
		else projection = frustum(x0,x1,y0,y1,-points[0].z,-points[4].z);
		return 1;
	}
	
	int getScreenPosition(Vec3 point,int &x,int &y) {
		Player player = getPlayer();
		if(player == NULL) return 0;
		int width = engine.app.getWidth();
		int height = engine.app.getHeight();
		mat4 projection = player.getProjection();
		Mat4 modelview = player.getModelview();
		projection.m00 *= float(height) / width;
		vec4 p = projection * vec4(modelview * Vec4(point,1.0f));
		if(p.w > 0.0f) {
			x = int(width * (0.5f + p.x * 0.5f / p.w));
			y = int(height * (0.5f - p.y * 0.5f / p.w));
			return 1;
		}
		return 0;
	}
	
	/*
	 */
	Vec3 getTransformTranslate(Mat4 transform) {
		return transform.m03m13m23;
	}
	
	Vec3 getTransformDirection(Mat4 transform) {
		return -transform.m02m12m22;
	}
	
	quat getTransformRotate(Mat4 transform) {
		return quat(orthonormalize(transform));
	}
	
	Vec3 getTransformScale(Mat4 transform) {
		mat4 rotation = rotation(mat4(transform));
		mat4 rotate = orthonormalize(rotation);
		mat4 scale = transpose(rotate) * rotation;
		return scale.m00m11m22;
	}
	
	/*
	 */
	void decomposeTransform(Mat4 transform,Vec3 &p,quat &r,Vec3 &s) {
		mat4 rotation = rotation(mat4(transform));
		mat4 rotate = orthonormalize(rotation);
		mat4 scale = transpose(rotate) * rotation;
		p = transform.m03m13m23;
		r = quat(rotate);
		s = scale.m00m11m22;
	}
	
	Mat4 composeTransform(Vec3 p,quat r,Vec3 s) {
		Mat4 transform = Mat4(rotation(r) * scale(s));
		transform.m03m13m23 = p;
		return transform;
	}
	
	/*
	 */
	void decomposeTransformDirection(Mat4 transform,Vec3 &p,Vec3 &d) {
		p = transform.m03m13m23;
		d = -transform.m02m12m22;
	}
	
	Mat4 composeTransformDirection(Vec3 p,Vec3 d,Vec3 up = Vec3(0.0f,0.0f,1.0f)) {
		Mat4 transform;
		Vec3 z = normalize(-d);
		Vec3 x = normalize(cross(up,z));
		Vec3 y = normalize(cross(z,x));
		transform.m00m10m20 = x;
		transform.m01m11m21 = y;
		transform.m02m12m22 = z;
		transform.m03m13m23 = p;
		return transform;
	}
	
	/*
	 */
	float getDistance(Vec3 v0,Vec3 v1) {
		return length(v1 - v0);
	}
	
	Scalar getAngle(Vec3 v0,Vec3 v1,Vec3 up = Vec3(0.0f,0.0f,1.0f)) {
		Vec3 c = cross(v0,v1);
		Scalar d = dot(c,up);
		Scalar a = atan2(length(c),dot(v0,v1));
		return a * sign(d) * RAD2DEG;
	}
	
} /* namespace Unigine */

#endif /* __UNIGINE_UTILS_H__ */
