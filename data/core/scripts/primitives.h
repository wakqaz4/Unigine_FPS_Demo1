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


#ifndef __UNIGINE_PRIMITIVES_H__
#define __UNIGINE_PRIMITIVES_H__

#include <core/unigine.h>

/*
 */
namespace Unigine {
	
	/*
	 */
	ObjectMeshDynamic createSurface(ObjectMeshDynamic object,Mesh mesh,int surface) {
		
		mesh.remapCVertex(surface);
		
		int num_vertex = object.getNumVertex();
		
		// vertices
		vec4 color = vec4_one;
		vec4 texcoord = vec4_zero;
		forloop(int i = 0; mesh.getNumVertex(surface)) {
			object.addVertex(mesh.getVertex(i,surface));
			if(mesh.getNumTexCoords0(surface)) texcoord.xy = mesh.getTexCoord0(i,surface);
			if(mesh.getNumTexCoords1(surface)) texcoord.zw = mesh.getTexCoord1(i,surface);
			if(mesh.getNumTangents(surface)) object.addTangent(mesh.getTangent(i,surface));
			if(mesh.getNumColors(surface)) color = mesh.getColor(i,surface);
			object.addTexCoord(texcoord);
			object.addColor(color);
		}
		
		// indices
		forloop(int i = 0; mesh.getNumIndices(surface)) {
			object.addIndex(num_vertex + mesh.getIndex(i,surface));
		}
		
		// bounds
		object.updateBounds();
		
		delete mesh;
		
		return object;
	}
	
	/*
	 */
	ObjectMeshDynamic createBox(ObjectMeshDynamic object,vec3 size,Mat4 transform = Mat4_identity) {
		Mesh mesh = new Mesh();
		int surface = mesh.addBoxSurface("box",size);
		mesh.setSurfaceTransform(mat4(transform),surface);
		return createSurface(object,mesh,surface);
	}
	
	ObjectMeshDynamic createBox(vec3 size) {
		return createBox(new ObjectMeshDynamic(),size);
	}
	
	/*
	 */
	ObjectMeshDynamic createPlane(ObjectMeshDynamic object,float width,float height,float step,Mat4 transform = Mat4_identity) {
		Mesh mesh = new Mesh();
		int surface = mesh.addPlaneSurface("plane",width,height,step);
		mesh.setSurfaceTransform(mat4(transform),surface);
		return createSurface(object,mesh,surface);
	}
	
	ObjectMeshDynamic createPlane(float width,float height,float step) {
		return createPlane(new ObjectMeshDynamic(),width,height,step);
	}
	
	/*
	 */
	ObjectMeshDynamic createSphere(ObjectMeshDynamic object,float radius,int stacks = 16,int slices = 32,Mat4 transform = Mat4_identity) {
		Mesh mesh = new Mesh();
		int surface = mesh.addSphereSurface("sphere",radius,stacks,slices);
		mesh.setSurfaceTransform(mat4(transform),surface);
		return createSurface(object,mesh,surface);
	}
	
	ObjectMeshDynamic createSphere(float radius,int stacks = 16,int slices = 32) {
		return createSphere(new ObjectMeshDynamic(),radius,stacks,slices);
	}
	
	/*
	 */
	ObjectMeshDynamic createCapsule(ObjectMeshDynamic object,float radius,float height,int stacks = 16,int slices = 32,Mat4 transform = Mat4_identity) {
		Mesh mesh = new Mesh();
		int surface = mesh.addCapsuleSurface("capsule",radius,height,stacks,slices);
		mesh.setSurfaceTransform(mat4(transform),surface);
		return createSurface(object,mesh,surface);
	}
	
	ObjectMeshDynamic createCapsule(float radius,float height,int stacks = 16,int slices = 32) {
		return createCapsule(new ObjectMeshDynamic(),radius,height,stacks,slices);
	}
	
	/*
	 */
	ObjectMeshDynamic createCylinder(ObjectMeshDynamic object,float radius,float height,int stacks = 1,int slices = 32,Mat4 transform = Mat4_identity) {
		Mesh mesh = new Mesh();
		int surface = mesh.addCylinderSurface("cylinder",radius,height,stacks,slices);
		mesh.setSurfaceTransform(mat4(transform),surface);
		return createSurface(object,mesh,surface);
	}
	
	ObjectMeshDynamic createCylinder(float radius,float height,int stacks = 1,int slices = 32) {
		return createCylinder(new ObjectMeshDynamic(),radius,height,stacks,slices);
	}
	
	/*
	 */
	ObjectMeshDynamic createPrism(ObjectMeshDynamic object,float size_0,float size_1,float height,int sides = 8,Mat4 transform = Mat4_identity) {
		Mesh mesh = new Mesh();
		int surface = mesh.addPrismSurface("prism",size_0,size_1,height,sides);
		mesh.setSurfaceTransform(mat4(transform),surface);
		return createSurface(object,mesh,surface);
	}
	
	ObjectMeshDynamic createPrism(float size_0,float size_1,float height,int sides = 8) {
		return createPrism(new ObjectMeshDynamic(),size_0,size_1,height,sides);
	}
	
	/*
	 */
	ObjectMeshDynamic createIcosahedron(ObjectMeshDynamic object,float radius,Mat4 transform = Mat4_identity) {
		Mesh mesh = new Mesh();
		int surface = mesh.addIcosahedronSurface("icosahedron",radius);
		mesh.setSurfaceTransform(mat4(transform),surface);
		return createSurface(object,mesh,surface);
	}
	
	ObjectMeshDynamic createIcosahedron(float radius) {
		return createIcosahedron(new ObjectMeshDynamic(),radius);
	}
	
	/*
	 */
	ObjectMeshDynamic createDodecahedron(ObjectMeshDynamic object,float radius,Mat4 transform = Mat4_identity) {
		Mesh mesh = new Mesh();
		int surface = mesh.addDodecahedronSurface("dodecahedron",radius);
		mesh.setSurfaceTransform(mat4(transform),surface);
		return createSurface(object,mesh,surface);
	}
	
	ObjectMeshDynamic createDodecahedron(float radius) {
		return createDodecahedron(new ObjectMeshDynamic(),radius);
	}
	
} /* namespace Unigine */

#endif /* __UNIGINE_PRIMITIVES_H__ */
