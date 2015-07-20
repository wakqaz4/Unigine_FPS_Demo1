/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_widgets.h
 * Desc:    Unigine editor
 * Version: 1.19
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
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

/******************************************************************************\
*
* WidgetSpriteMaterialView
*
\******************************************************************************/

/*
 */
class WidgetSpriteMaterialView {
	
	float phi = 75.0f;
	float theta = 0.0f;
	float dist = 4.0f;
	
	WidgetVBox vbox;
	WidgetSpriteMaterial sprite;
	WidgetComboBox primitive_cb;
	
	// widget
	Widget getWidget() {
		return vbox;
	}
	
	// set material
	void setMaterial(string name) {
		sprite.setMaterial(engine.materials.findMaterial(name));
	}
	
	// update matrices
	void update() {
		
		float radius;
		if(primitive_cb.getCurrentItem() == RENDER_BOX) radius = dist;
		else if(primitive_cb.getCurrentItem() == RENDER_SPHERE) radius = dist * rsqrt(2.0f);
		else if(primitive_cb.getCurrentItem() == RENDER_ICOSAHEDRON) radius = dist * rsqrt(2.0f);
		else if(primitive_cb.getCurrentItem() == RENDER_DODECAHEDRON) radius = dist * rsqrt(2.0f);
		else throw("MaterialSprite::update(): unknown primitive\n");
		
		sprite.setProjection(perspective(60.0f,float(vbox.getWidth()) / vbox.getHeight(),0.01f,100.0f));
		sprite.setModelview(Mat4(translate(0.0f,0.0f,-radius) * mat4(quat(1.0f,0.0f,0.0f,-phi) * quat(0.0f,0.0f,1.0f,-theta))));
	}
	
	// callbacks
	void pressed_callback() {
		
		int dx = gui.getMouseDX();
		int dy = gui.getMouseDY();
		int button = gui.getMouseButton();
		
		if(button & APP_BUTTON_LEFT) {
			phi -= dy * 0.25f;
			theta -= dx * 0.25f;
		}
		if(button & APP_BUTTON_RIGHT) {
			dist -= dy * 0.05f;
			dist = clamp(dist,1.8f,10.0f);
		}
	}
	
	void changed_callback() {
		sprite.setPrimitive(primitive_cb.getCurrentItem());
	}
	
	// callback redirectors
	void pressed_redirector(WidgetSpriteMaterialView view) {
		view.pressed_callback();
	}
	
	void changed_redirector(WidgetSpriteMaterialView view) {
		view.changed_callback();
	}
	
	// constructor
	WidgetSpriteMaterialView(Gui gui) {
		
		// vbox
		vbox = new WidgetVBox(gui);
		
		// create material
		sprite = new WidgetSpriteMaterial(gui,300,300);
		sprite.setBlendFunc(GUI_BLEND_NONE,GUI_BLEND_NONE);
		vbox.addChild(sprite,GUI_ALIGN_EXPAND);
		
		// primitive
		primitive_cb = new WidgetComboBox(gui);
		primitive_cb.addItem(TR("Box"));
		primitive_cb.addItem(TR("Sphere"));
		primitive_cb.addItem(TR("Icosahedron"));
		primitive_cb.addItem(TR("Dodecahedron"));
		primitive_cb.setPosition(-4,4);
		vbox.addChild(primitive_cb,GUI_ALIGN_OVERLAP | GUI_ALIGN_RIGHT);
		
		// callbacks
		sprite.setCallback(GUI_PRESSED,functionid(pressed_redirector),this);
		primitive_cb.setCallback(GUI_CHANGED,functionid(changed_redirector),this);
	}
	
	// destructor
	~WidgetSpriteMaterialView() {
		delete vbox;
		delete sprite;
		delete primitive_cb;
	}
};

/******************************************************************************\
*
* WidgetSpriteNodeView
*
\******************************************************************************/

/*
 */
class WidgetSpriteNodeView {
	
	float phi = 75.0f;
	float theta = 0.0f;
	float dist = 4.0f;
	
	WidgetVBox vbox;
	WidgetSpriteNode sprite;
	
	// widget
	Widget getWidget() {
		return vbox;
	}
	
	// set node
	void setNode(Node node) {
		sprite.setNode(node);
	}
	
	// update matrices
	void update() {
		
		vec3 center = vec3_zero;
		float radius = dist;
		
		Node node = sprite.getNode();
		if(engine.world.isNode(node) == 0) {
			sprite.setNode(NULL);
			node = NULL;
		}
		
		if(node != NULL) {
			center = nodesGetWorldBoundCenter(node);
			radius = nodesGetBoundRadius(node) * dist;
		}
		
		sprite.setProjection(perspective(60.0f,float(vbox.getWidth()) / vbox.getHeight(),radius * 0.01f,radius * 2.0f));
		sprite.setModelview(Mat4(translate(0.0f,0.0f,-radius) * mat4(quat(1.0f,0.0f,0.0f,-phi) * quat(0.0f,0.0f,1.0f,-theta)) * translate(-center)));
	}
	
	// callbacks
	void pressed_callback() {
		
		int dx = gui.getMouseDX();
		int dy = gui.getMouseDY();
		int button = gui.getMouseButton();
		
		if(button & APP_BUTTON_LEFT) {
			phi -= dy * 0.25f;
			theta -= dx * 0.25f;
		}
		if(button & APP_BUTTON_RIGHT) {
			dist -= dy * 0.05f;
			dist = clamp(dist,1.8f,10.0f);
		}
	}
	
	// callback redirectors
	void pressed_redirector(WidgetSpriteNodeView view) {
		view.pressed_callback();
	}
	
	// constructor
	WidgetSpriteNodeView(Gui gui) {
		
		// vbox
		vbox = new WidgetVBox(gui);
		
		// create material
		sprite = new WidgetSpriteNode(gui,512,512);
		sprite.setBlendFunc(GUI_BLEND_NONE,GUI_BLEND_NONE);
		vbox.addChild(sprite,GUI_ALIGN_EXPAND);
		
		// callbacks
		sprite.setCallback(GUI_PRESSED,functionid(pressed_redirector),this);
	}
	
	// destructor
	~WidgetSpriteNodeView() {
		delete vbox;
		delete sprite;
	}
};
