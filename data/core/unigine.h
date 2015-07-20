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


#ifndef __UNIGINE_H__
#define __UNIGINE_H__

/* Color codes
 */
#define FONT_BLACK		"\033[30m"
#define FONT_RED		"\033[31m"
#define FONT_GREEN		"\033[32m"
#define FONT_BROWN		"\033[33m"
#define FONT_BLUE		"\033[34m"
#define FONT_MAGENTA	"\033[35m"
#define FONT_CYAN		"\033[36m"
#define FONT_WHITE		"\033[37m"
#define FONT_DEFAULT	"\033[0m"

/* Data types
 */
#ifdef USE_DOUBLE
	#define Scalar			double
	#define Vec3			dvec3
	#define Vec4			dvec4
	#define Mat4			dmat4
	#define Vec3_zero		dvec3_zero
	#define Vec4_zero		dvec4_zero
	#define Vec3_one		dvec3_one
	#define Vec4_one		dvec4_one
	#define Vec3_epsilon	dvec3_epsilon
	#define Vec4_epsilon	dvec4_epsilon
	#define Vec3_infinity	dvec3_infinity
	#define Vec4_infinity	dvec4_infinity
	#define Mat4_identity	dmat4_identity
#else
	#define Scalar			float
	#define Vec3			vec3
	#define Vec4			vec4
	#define Mat4			mat4
	#define Vec3_zero		vec3_zero
	#define Vec4_zero		vec4_zero
	#define Vec3_one		vec3_one
	#define Vec4_one		vec4_one
	#define Vec3_epsilon	vec3_epsilon
	#define Vec4_epsilon	vec4_epsilon
	#define Vec3_infinity	vec3_infinity
	#define Vec4_infinity	vec4_infinity
	#define Mat4_identity	mat4_identity
#endif

#ifndef USE_DOUBLE
	#define WorldBoundSphere	BoundSphere
	#define WorldBoundBox		BoundBox
	#define WorldBoundFrustum	BoundFrustum
#endif

/******************************************************************************\
*
* Debugging
*
\******************************************************************************/

/*
 */
#ifdef NDEBUG
	#define assert(EXP)
#else
	#define assert(EXP) { if(EXP) { } else { throw("%s:%d: %s: Assertion: '%s'\n",__FILE__,__LINE__,__FUNC__,#EXP); } }
#endif

/*
 */
#ifdef NDEBUG
	#define breakpoint
#else
	#define breakpoint { engine.app.stopFps(); log.message("\n%s:%d: %s:\n",__FILE__,__LINE__,__FUNC__); breakpoint; engine.app.startFps(); }
#endif

/******************************************************************************\
*
* Utils
*
\******************************************************************************/

/*
 */
void swap(int &v0,int &v1) {
	int temp = v0;
	v0 = v1;
	v1 = temp;
}

/******************************************************************************\
*
* Threading
*
\******************************************************************************/

/*
 */
void sleep(float time) {
	float end[];
	end[get_thread()] = engine.game.getTime() + time;
	wait;
	while(engine.game.getTime() < end[get_thread()]) wait;
}

/******************************************************************************\
*
* Nodes
*
\******************************************************************************/

/*
 */
Node node_cast(Node node) {
	assert(engine.world.isNode(node) == 1);
	return class_cast(node.getTypeName(),node);
}

Node node_clone(Node node) {
	assert(engine.world.isNode(node) == 1);
	engine.editor.clearBindings();
	return node_cast(node.clone());
}

/*
 */
Node node_append(Node node) {
	assert(engine.world.isNode(node) == 1);
	forloop(int i = 0; node.getNumChilds()) {
		node_append(node_cast(node.getChild(i)));
	}
	return class_append(node_cast(node));
}

Node node_remove(Node node) {
	assert(engine.world.isNode(node) == 1);
	forloop(int i = 0; node.getNumChilds()) {
		node_remove(node_cast(node.getChild(i)));
	}
	return class_remove(node_cast(node));
}

/*
 */
void node_delete(Node node) {
	assert(engine.world.isNode(node) == 1);
	for(int i = node.getNumChilds() - 1; i >= 0; i--) {
		node_delete(node.getChild(i));
	}
	delete node_cast(node);
}

/*
 */
Node node_load(string name,int cache = 1) {
	return node_cast(node_append(engine.world.loadNode(name,cache)));
}

int node_save(string name,Node node,int materials = 1,int binary = 0) {
	assert(engine.world.isNode(node) == 1);
	return engine.world.saveNode(name,node,materials,binary);
}

/*
 */
Node node_editor(string name) {
	return node_cast(engine.editor.getNodeByName(name));
}

/******************************************************************************\
*
* Bodies
*
\******************************************************************************/

/*
 */
Body body_cast(Body body) {
	assert(engine.physics.isBody(body) == 1);
	return class_cast(body.getTypeName(),body);
}

Body body_clone(Body body,Object object = 0) {
	assert(engine.physics.isBody(body) == 1);
	engine.editor.clearBindings();
	return body_cast(body.clone(object));
}

/*
 */
Shape shape_cast(Shape shape) {
	assert(engine.physics.isShape(shape) == 1);
	return class_cast(shape.getTypeName(),shape);
}

Shape shape_clone(Shape shape) {
	assert(engine.physics.isShape(shape) == 1);
	engine.editor.clearBindings();
	return shape_cast(shape.clone());
}

/*
 */
Joint joint_cast(Joint joint) {
	assert(engine.physics.isJoint(joint) == 1);
	return class_cast(joint.getTypeName(),joint);
}

Joint joint_clone(Joint joint) {
	assert(engine.physics.isJoint(joint) == 1);
	engine.editor.clearBindings();
	return joint_cast(joint.clone());
}

/******************************************************************************\
*
* Widgets
*
\******************************************************************************/

/*
 */
Widget widget_cast(Widget widget) {
	return class_cast(widget.getTypeName(),widget);
}

/******************************************************************************\
*
* Hierarchy
*
\******************************************************************************/

/*
 */
void body_traversal(Body body,string name) {
	assert(engine.physics.isBody(body) == 1);
	call(name,body);
	forloop(int i = 0; body.getNumChilds()) {
		body_traversal(body.getChild(i),name);
	}
}

void body_shape_traversal(Body body,string name) {
	assert(engine.physics.isBody(body) == 1);
	forloop(int i = 0; body.getNumShapes()) {
		call(name,body.getShape(i));
	}
	forloop(int i = 0; body.getNumChilds()) {
		body_shape_traversal(body.getChild(i),name);
	}
}

void body_joint_traversal(Body body,string name) {
	assert(engine.physics.isBody(body) == 1);
	forloop(int i = 0; body.getNumJoints()) {
		call(name,body.getJoint(i));
	}
	forloop(int i = 0; body.getNumChilds()) {
		body_joint_traversal(body.getChild(i),name);
	}
}

/*
 */
void node_traversal(Node node,string name) {
	assert(engine.world.isNode(node) == 1);
	call(name,node);
	forloop(int i = 0; node.getNumChilds()) {
		node_traversal(node.getChild(i),name);
	}
}

void node_body_traversal(Node node,string name) {
	assert(engine.world.isNode(node) == 1);
	if(node.isObject()) {
		Object object = node_cast(node);
		Body body = object.getBody();
		if(body != NULL) body_traversal(body,name);
	}
	forloop(int i = 0; node.getNumChilds()) {
		node_body_traversal(node.getChild(i),name);
	}
}

void node_shape_traversal(Node node,string name) {
	assert(engine.world.isNode(node) == 1);
	if(node.isObject()) {
		Object object = node_cast(node);
		Body body = object.getBody();
		if(body != NULL) body_shape_traversal(body,name);
	}
	forloop(int i = 0; node.getNumChilds()) {
		node_shape_traversal(node.getChild(i),name);
	}
}

void node_joint_traversal(Node node,string name) {
	assert(engine.world.isNode(node) == 1);
	if(node.isObject()) {
		Object object = node_cast(node);
		Body body = object.getBody();
		if(body != NULL) body_joint_traversal(body,name);
	}
	forloop(int i = 0; node.getNumChilds()) {
		node_joint_traversal(node.getChild(i),name);
	}
}

#endif /* __UNIGINE_H__ */
