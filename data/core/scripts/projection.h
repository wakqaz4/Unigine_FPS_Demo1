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


#ifndef __UNIGINE_PROJECTION_H__
#define __UNIGINE_PROJECTION_H__

#include <core/unigine.h>

/*
 */
namespace Unigine {
	
	/*
	 */
	void getMeshProjection(string name,vec4 tangents[],vec3 binormals[],vec3 positions[]) {
		
		// load mesh
		Mesh mesh = new Mesh(name);
		
		// process surfaces
		forloop(int i = 0; mesh.getNumSurfaces()) {
			string name = mesh.getSurfaceName(i);
			
			// surface triangles
			forloop(int j = 0; mesh.getNumCIndices(i); 3) {
				
				// first triangle
				int i0 = mesh.getCIndex(j + 0,i);
				int i1 = mesh.getCIndex(j + 1,i);
				int i2 = mesh.getCIndex(j + 2,i);
				vec3 p0 = mesh.getVertex(i0,i);
				vec3 p1 = mesh.getVertex(i1,i);
				vec3 p2 = mesh.getVertex(i2,i);
				
				// unique indices
				int indices[0];
				forloop(int k = 0; mesh.getNumCIndices(i)) {
					int index = indices.find(mesh.getCIndex(k,i));
					if(index != -1) indices.remove(index);
					else indices.append(mesh.getCIndex(k,i));
				}
				
				vec3 tangent;
				vec3 binormal;
				vec3 position;
				
				// surface orientation
				if(indices[0] == i0) {
					vec3 p10 = p1 - p0;
					vec3 p20 = p2 - p0;
					if(length(p10) > length(p20)) { tangent = p10; binormal = p20; }
					else { tangent = p20; binormal = p10; }
					position = p0;
				} else if(indices[0] == i1) {
					vec3 p01 = p0 - p1;
					vec3 p21 = p2 - p1;
					if(length(p01) > length(p21)) { tangent = p01; binormal = p21; }
					else { tangent = p21; binormal = p01; }
					position = p1;
				} else if(indices[0] == i2) {
					vec3 p02 = p0 - p2;
					vec3 p12 = p1 - p2;
					if(length(p02) > length(p12)) { tangent = p02; binormal = p12; }
					else { tangent = p12; binormal = p02; }
					position = p2;
				}
				
				// add surface
				tangents.append(name,tangent);
				binormals.append(name,binormal);
				positions.append(name,position + (tangent + binormal) * 0.5f);
				
				break;
			}
		}
		
		delete mesh;
	}
	
	/*
	 */
	void getViewProjection(Vec3 camera,Vec3 position,vec3 tangent,vec3 binormal,float near,float far,mat4 &projection,Mat4 &modelview) {
		
		// view basis
		vec3 normal = normalize(cross(tangent,binormal));
		vec3 right = normalize(tangent);
		vec3 up = normalize(binormal);
		
		// relative position
		position = vec3(position - camera);
		
		// projection matrix
		float scale = -near / dot(normal,position);
		float aspect = length(binormal) / length(tangent);
		float l = dot(right,position - tangent * 0.5f) * scale * aspect;
		float r = dot(right,position + tangent * 0.5f) * scale * aspect;
		float b = dot(up,position - binormal * 0.5f) * scale;
		float t = dot(up,position + binormal * 0.5f) * scale;
		projection = frustum(l,r,b,t,near,far);
		
		// modelview matrix
		modelview = Mat4_identity;
		modelview.row03 = right;
		modelview.row13 = up;
		modelview.row23 = normal;
		modelview *= translate(-camera);
	}
	
	/*
	 */
	void getWallProjection(mat4 projection,Mat4 modelview,float aspect,int grid_x,int grid_y,int view_x,int view_y,float bezel_x,float bezel_y,float angle,mat4 &ret_projection,Mat4 &ret_modelview) {
		
		assert(grid_x > 0 && grid_y > 0 && "Unigine::getWallProjection(): bad grid configuration");
		assert(view_x >= 0 && view_x < grid_x && "Unigine::getWallProjection(): bad viewport X index");
		assert(view_y >= 0 && view_y < grid_y && "Unigine::getWallProjection(): bad viewport Y index");
		
		// bounding frustum points
		vec3 points_0[8] = (
			vec3(-1.0f,-1.0f,-1.0f), vec3( 1.0f,-1.0f,-1.0f),
			vec3(-1.0f, 1.0f,-1.0f), vec3( 1.0f, 1.0f,-1.0f),
			vec3(-1.0f,-1.0f, 1.0f), vec3( 1.0f,-1.0f, 1.0f),
			vec3(-1.0f, 1.0f, 1.0f), vec3( 1.0f, 1.0f, 1.0f),
		);
		
		// transform points
		projection.m00 /= aspect;
		mat4 iprojection = inverse(projection);
		forloop(int i = 0; 8) {
			vec4 p = iprojection * vec4(points_0[i]);
			points_0[i] = vec3(p) / p.w;
		}
		
		// central viewport
		int center_x = grid_x / 2;
		int center_y = grid_y / 2;
		
		// even vertical split
		if((grid_x & 0x01) == 0) {
			float w1 = points_0[1] - points_0[0];
			float w3 = points_0[3] - points_0[2];
			float offset = 0.5f;
			if(view_x >= center_x) {
				offset = -0.5f;
				center_x--;
			}
			points_0[0] += w1 * offset;
			points_0[1] += w1 * offset;
			points_0[2] += w3 * offset;
			points_0[3] += w3 * offset;
		}
		
		// even horizontal split
		if((grid_y & 0x01) == 0) {
			float h2 = points_0[2] - points_0[0];
			float h3 = points_0[3] - points_0[1];
			float offset = -0.5f;
			if(view_y >= center_y) {
				offset = 0.5f;
				center_y--;
			}
			points_0[0] += h2 * offset;
			points_0[1] += h3 * offset;
			points_0[2] += h2 * offset;
			points_0[3] += h3 * offset;
		}
		
		// projection points
		vec3 points_1[8];
		
		// modelview matrix
		ret_modelview = modelview;
		
		// horizontal angle
		if(grid_x >= grid_y) {
			if(view_y < center_y) {
				for(int i = center_y; i > view_y; i--) {
					move_points_up(points_0,points_0);
				}
			} else if(view_y > center_y) {
				for(int i = center_y; i < view_y; i++) {
					move_points_down(points_0,points_0);
				}
			}
			if(view_x > center_x) {
				for(int i = center_x; i < view_x; i++) {
					transform_points(points_1,rotateY(-angle),points_0);
					move_points_right(points_1,points_0);
					transform_points(points_0,rotateY(angle),points_1);
					ret_modelview = rotateY(angle) * ret_modelview;
				}
			} else if(view_x < center_x) {
				for(int i = center_x; i > view_x; i--) {
					transform_points(points_1,rotateY(angle),points_0);
					move_points_left(points_1,points_0);
					transform_points(points_0,rotateY(-angle),points_1);
					ret_modelview = rotateY(-angle) * ret_modelview;
				}
			}
		}
		// vertical angle
		else {
			if(view_x > center_x) {
				for(int i = center_x; i < view_x; i++) {
					move_points_right(points_0,points_0);
				}
			} else if(view_x < center_x) {
				for(int i = center_x; i > view_x; i--) {
					move_points_left(points_0,points_0);
				}
			}
			if(view_y < center_y) {
				for(int i = center_y; i > view_y; i--) {
					transform_points(points_1,rotateX(-angle),points_0);
					move_points_up(points_1,points_0);
					transform_points(points_0,rotateX(angle),points_1);
					ret_modelview = rotateX(angle) * ret_modelview;
				}
			} else if(view_y > center_y) {
				for(int i = center_y; i < view_y; i++) {
					transform_points(points_1,rotateX(angle),points_0);
					move_points_down(points_1,points_0);
					transform_points(points_0,rotateX(-angle),points_1);
					ret_modelview = rotateX(-angle) * ret_modelview;
				}
			}
		}
		
		// projection matrix
		float left = points_0[0].x;
		float right = points_0[1].x;
		float bottom = points_0[0].y;
		float top = points_0[2].y;
		
		// horizontal bezel compensation
		float width = (right - left) * 0.5f;
		left += width * bezel_x;
		right -= width * bezel_x;
		
		// vertical bezel compensation
		float height = (top - bottom) * 0.5f;
		bottom += height * bezel_y;
		top -= height * bezel_y;
		
		ret_projection = frustum(left,right,bottom,top,-points_0[0].z,-points_0[4].z);
		ret_projection.m00 *= aspect;
	}
	
	/*
	 */
	void transform_points(vec3 dest[],mat4 transform,vec3 src[]) {
		dest[0] = transform * src[0];
		dest[1] = transform * src[1];
		dest[2] = transform * src[2];
		dest[3] = transform * src[3];
	}
	
	/*
	 */
	void move_points_left(vec3 dest[],vec3 src[]) {
		float s0 = src[0];
		float s2 = src[2];
		dest[0] = s0 + dest[0] - dest[1];
		dest[2] = s2 + dest[2] - dest[3];
		dest[1] = s0;
		dest[3] = s2;
	}
	
	void move_points_right(vec3 dest[],vec3 src[]) {
		float s1 = src[1];
		float s3 = src[3];
		dest[1] = s1 + dest[1] - dest[0];
		dest[3] = s3 + dest[3] - dest[2];
		dest[0] = s1;
		dest[2] = s3;
	}
	
	void move_points_up(vec3 dest[],vec3 src[]) {
		float s2 = src[2];
		float s3 = src[3];
		dest[2] = s2 + dest[2] - dest[0];
		dest[3] = s3 + dest[3] - dest[1];
		dest[0] = s2;
		dest[1] = s3;
	}
	
	void move_points_down(vec3 dest[],vec3 src[]) {
		float s0 = src[0];
		float s1 = src[1];
		dest[0] = s0 + dest[0] - dest[2];
		dest[1] = s1 + dest[1] - dest[3];
		dest[2] = s0;
		dest[3] = s1;
	}
	
} /* namespace Unigine */

#endif /* __UNIGINE_PROJECTION_H__ */
