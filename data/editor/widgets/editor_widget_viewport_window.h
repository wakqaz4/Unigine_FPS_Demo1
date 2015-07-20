/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_widget_viewport_window.h
 * Desc:    Unigine editor
 * Version: 1.02
 * Author:  Vladimir Borovskikh <futurist@unigine.com>
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

#ifndef __EDITOR_WIDGET_VIEWPORT_WINDOW_H__
#define __EDITOR_WIDGET_VIEWPORT_WINDOW_H__

#include <core/systems/widgets/widget.h>
#include <editor/widgets/editor_widget_viewport_layout.h>
#include <editor/widgets/editor_widget_sprite_viewport.h>

/*
 */
namespace Editor::Widgets {
	
	/*
	 */
	class WidgetViewportWindow : Unigine::Widgets::Window {
		
		//
		using Unigine::Widgets;
		
		//
		Icon close_i;
		Editor::Widgets::SpriteViewport viewport;
		WidgetViewportLayout layout;
		VBox vbox;
		
		//
		int width;
		int height;
		int resize_frame;
		int update_thread_id;
		
		//
		Player camera;
		int camera_lock = true;
		Vec3 position;
		vec3 velocity;
		quat camera_rotation;
		
		Mat4 imodelview;
		
		//
		WidgetViewportWindow(string name,int x = 0,int y = 0,int width = 640,int height = 480) {
			__Window__(name,x,y);
			
			// window
			vbox = new VBox();
			this.addChild(vbox,GUI_ALIGN_CENTER);
			
			setMoveable(1);
			setSizeable(1);
			
			setWidth(width);
			setHeight(height);
			
			// close icon
			close_i = new Icon("core/gui/window_close.png");
			this.addChild(close_i,GUI_ALIGN_OVERLAP | GUI_ALIGN_TOP | GUI_ALIGN_RIGHT);
			
			close_i.setCallback(GUI_CLICKED,functionid(close_callback_redirector),this);
			close_i.setPosition(4,-24);
			
			// sprite viewport
			resolution_changed();
			update_thread_id = thread(functionid(WidgetViewportWindow::thread_redirector),this,functionid(WidgetViewportWindow::update),0.0f);
			
			// layout
			layout = new WidgetViewportLayout(this);
			layout.setBorder("panel_camera",1);
			layout.setBorder("panel_resolution",1);
		}
		
		~WidgetViewportWindow() {
			kill_thread(update_thread_id);
			delete layout;
			delete viewport;
		}
		//
		void setWidth(int width) {
			if(!isSizeable()) setMinWidth(width);
			super.setWidth(width);
		}
		
		void setHeight(int height) {
			if(!isSizeable()) setMinHeight(height);
			super.setHeight(height);
		}
		
		void setMinWidth(int width) {
			vbox.setWidth(width - 16);
		}
		
		void setMinHeight(int height) {
			vbox.setHeight(height + 13);
		}
		
		
		void setPosition(int x,int y) {
			#ifdef HAS_INTERFACE
				InterfaceWindow interface = getInterface();
				if(interface != NULL) {
					interface.setPosition(x,y);
					return;
				}
			#endif
			widget.setPosition(x,y);
		}
		int getPositionX() {
			#ifdef HAS_INTERFACE
				InterfaceWindow interface = getInterface();
				if(interface != NULL) return interface.getPositionX();
			#endif
			return widget.getPositionX();
		}
		int getPositionY() {
			#ifdef HAS_INTERFACE
				InterfaceWindow interface = getInterface();
				if(interface != NULL) return interface.getPositionY();
			#endif
			return widget.getPositionY();
		}
		
		//
		int isFocused() {
			if(widget.isFocused()) return true;
			if(layout.isFocused()) return true;
			if(viewport.isFocused()) return true;
			return false;
		}
		
		//
		int setCameraLock(int lock) {
			camera_lock = lock;
			viewport.setFocus();
		}
		
		int isCameraLock() {
			return camera_lock || !engine.world.isNode(camera);
		}
		
		//
		int is_control() {
			if(!viewport.isFocused()) return false;
			
			return	hotkeysGetState("orbit",0) ||
					hotkeysGetState("dolly",0) ||
					hotkeysGetState("track",0) ||
					hotkeysGetState("change_direction",0) ||
					hotkeysGetState("move_forward",0) ||
					hotkeysGetState("move_backward",0) ||
					hotkeysGetState("move_left",0) ||
					hotkeysGetState("move_right",0) ||
					hotkeysGetState("move_crouch",0) ||
					hotkeysGetState("move_jump",0) ||
					hotkeysGetState("inclination_right",0) ||
					hotkeysGetState("inclination_left",0);
		}
		
		//
		WidgetViewportLayout getLayout() { return layout; }
		
		//
		void setViewportCamera(Player camera_) {
			camera = node_cast(camera_);
			imodelview = camera.getWorldTransform();
			camera_rotation = camera.getWorldRotation();
			setCameraPosition(camera.getPosition());
		}
		
		Player getViewportCamera() {
			return camera;
		}
		
		//
		void setCameraDirection(vec3 dir,vec3 up) {
			dir = normalize(dir);
			up = normalize(up);
			camera_rotation = quat(lookAt(vec3_zero, dir, up));
		}
		
		vec3 getCameraDirection() {
			return rotation(imodelview) * vec3(0.0f,0.0f,-1.0f);
		}
		
		//
		Mat4 getModelview() {
			return imodelview;
		}
		
		//
		void setCameraPosition(Vec3 pos) {
			position = pos;
			velocity = vec3_zero;
		}
		
		Vec3 getCameraPosition() {
			return position;
		}
		
		void control() {
			
			float ifps = engine.app.getIFps();
			
			// camera basis
			Vec3 x = camera_rotation * Vec3(1.0f,0.0f,0.0f);
			Vec3 y = camera_rotation * Vec3(0.0f,1.0f,0.0f);
			Vec3 z = camera_rotation * Vec3(0.0f,0.0f,1.0f);
			
			hotkeysSetSkipModifier("move_run");
			
			int inclination = 0;
			if(hotkeysGetState("inclination_right",0)) inclination = 180;
			if(hotkeysGetState("inclination_left",0)) inclination = -180;
			
			float camera_velocity = controlsGetCameraMinVelocity();
			if(hotkeysGetModifierState("move_run")) camera_velocity = controlsGetCameraMaxVelocity();
			
			// mouse controls
			if(engine.controls.isMouseEnabled() || inclination != 0) {
				float dx = engine.controls.getMouseDX();
				float dy = engine.controls.getMouseDY();
				
				if(hotkeysGetState("change_direction",0) || hotkeysGetState("orbit",0) || inclination != 0) {
					quat i_rotation = inverse(camera_rotation);
					i_rotation = quat(vec3(0,0,1),inclination * ifps) * quat(vec3(1,0,0),dy) * i_rotation * quat(vec3(0,0,1),dx);
					camera_rotation = inverse(i_rotation);
				}
			}
			
			// impulse
			vec3 impulse = vec3_zero;
			
			// keyboard controls
			if(hotkeysGetState("move_forward",0)) impulse -= z;
			if(hotkeysGetState("move_backward",0)) impulse += z;
			if(hotkeysGetState("move_left",0)) impulse -= x;
			if(hotkeysGetState("move_right",0)) impulse += x;
			if(hotkeysGetState("move_crouch",0)) impulse -= y;
			if(hotkeysGetState("move_jump",0)) impulse += y;
			
			impulse = normalize(impulse);
			
			if(hotkeysGetModifierState("move_run")) impulse *= controlsGetCameraMaxVelocity();
			else impulse *= controlsGetCameraMinVelocity();
			
			hotkeysSetSkipModifier(NULL);
			
			velocity = impulse;
			position += velocity * ifps;
			
			// matrices
			imodelview = translate(position) * camera_rotation;
			
		}
		
		//
		void resolution_changed() {
			delete viewport;
			
			int new_width = this.getWidth() - 8;
			int new_height = this.getHeight() - 29;
			
			if(new_width <= 0) new_width = 1;
			if(new_height <= 0) new_height = 1;
			
			viewport = new Editor::Widgets::SpriteViewport(new_width,new_height);
			viewport.setPosition(-12,-5);
			viewport.setCallback(GUI_PRESSED,functionid(pressed_callback_redirector),this);
			
			this.addChild(viewport,GUI_ALIGN_OVERLAP | GUI_ALIGN_BACKGROUND);
		}
		
		//
		void update() {
			if(getPositionY() < 0) setPosition(getPositionX(),0);
			
			if(layout != NULL) layout.update();
			
			if(width != this.getWidth() || height != this.getHeight()) {
				width = this.getWidth();
				height = this.getHeight();
				this.arrange();
				resize_frame = 0;
			} else if(resize_frame < 1) {
				resize_frame++;
			}
			
			viewport.setHidden(resize_frame);
			
			if(resize_frame == 1) {
				resolution_changed();
				resize_frame = INFINITY;
			}
			
			if(camera == NULL) return;
			if(viewport == NULL) return;
			
			
			if(is_control()) control();
			else if(!isCameraLock()) setViewportCamera(camera);
			
			if(!isCameraLock()) camera.setWorldTransform(imodelview);
			
			viewport.setShadows(1);
			viewport.setViewportMask(camera.getViewportMask());
			viewport.setReflectionMask(camera.getReflectionMask());
			viewport.setProjection(camera.getProjection());
			viewport.setModelview(inverse(imodelview));
			viewport.setMaterials(camera.getPostMaterials());
		}
		
		//
		void close_callback_redirector(WidgetViewportWindow window) {
			window.close_callback();
		}
		
		void close_callback() {
			this.runCallback(GUI_HIDE);
			this.setHidden(1);
		}
		
		//
		void pressed_callback_redirector(WidgetViewportWindow window) {
			window.pressed_callback();
		}
		
		void pressed_callback() {
			control();
		}
		
		//
		void thread_redirector(int instance,string method,float interval) {
			while(1) {
				instance.call(method);
				wait;
				
				if(interval) sleep(interval);
			}
		}
		
		void renderImage(Image image) {
			viewport.renderImage(image);
		}
	};
	
} /* namespace Editor::Widgets */

#endif // __EDITOR_WIDGET_VIEWPORT_WINDOW_H__
