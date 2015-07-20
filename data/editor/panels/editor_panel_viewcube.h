/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_panel_viewcube.h
 * Desc:    Unigine editor
 * Version: 1.0
 * Author:  Mikhail Lyubimov <alterego@unigine.com>
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

#ifndef __EDITOR_PANEL_VIEWCUBE_H__
#define __EDITOR_PANEL_VIEWCUBE_H__

#include <editor/panels/editor_panel_base.h>
#include <editor/widgets/editor_widget_viewport_layout.h>

/*
 */
namespace Editor::Panels {
	
	/**************************************************************************\
	*
	* class PanelViewCube
	*
	\**************************************************************************/
	
	/*
	 */
	class PanelViewCube : PanelBase {
		
		//
		using Unigine;
		using Unigine::Widgets;
		using Editor::Widgets;
		
		WidgetViewportLayout layout;
		
		HBox main_widget;
		HBox ortho_hbox;
		Icon ortho_i;
		Canvas cube;
		Canvas arrows;
		
		mat4 transform_perspective;
		mat4 transform_orthographic;
		
		quat cam_rotation;
		
		string texture_touch = "editor/resources/textures/viewcube_touch_d.png";
		string texture_untouch = "editor/resources/textures/viewcube_untouch_d.png";
		
		vec3 rotations[0];
		vec3 positions[0];
		
		int mouse_state_old;
		
		int width;
		int height;
		
		float width_half;
		float height_half;
		
		int is_mouse_over;
		
		//
		vec3 get_polygon_pos(string name) {
			
			void search(string before) {
				string new_name = replace(name,before,"");
				if(new_name == name) return 0;
				name = new_name;
				return 1;
			}
			
			vec3 pos = vec3_zero;
			
			if(search("px")) pos.x = 1;
			if(search("py")) pos.y = 1;
			if(search("pz")) pos.z = 1;
			
			if(search("nx")) pos.x = -1;
			if(search("ny")) pos.y = -1;
			if(search("nz")) pos.z = -1;
			
			return pos;
		}
		
		//
		quat get_polygon_rot(vec3 pos) {
			vec3 dir = normalize(-pos);
			vec3 up = vec3(0,0,1);
			if(dir == vec3(0,0,1)) up = vec3(0,1,0);
			quat rot = quat(lookAt(vec3_zero,dir,up));
			rot = inverse(normalize(rot));
			return rot;
		}
		
		//
		PanelViewCube(WidgetViewportLayout layout_) {
			layout = layout_;
			
			init("editor/panels/editor_panel_viewcube.ui","Editor::Panels::PanelViewCube::");
			layout.addWidget("panel_viewcube",main_widget);
			layout.setBackground("panel_viewcube",0);
			
			Mesh mesh = new Mesh("editor/resources/meshes/viewcube.mesh");
			cube = new Canvas();
			
			forloop(int surface = 0; mesh.getNumSurfaces()) {
				string name = mesh.getSurfaceName(surface);
				vec3 pos = get_polygon_pos(name);
				quat rot = get_polygon_rot(pos);
				positions.append(pos);
				rotations.append(rot);
				
				mesh.remapCVertex(surface);
				int polygon = cube.addPolygon();
				cube.setPolygonTwoSided(polygon,0);
				
				forloop(int i = 0; mesh.getNumVertex(surface)) {
					vec3 point = mesh.getVertex(i,surface);
					cube.addPolygonPoint(polygon,point);
					
					vec3 uv = mesh.getTexCoord0(i, surface);
					cube.setPolygonTexCoord(polygon,uv);
				}
				
				forloop(int i = 0; mesh.getNumCIndices(surface)) {
					int index = mesh.getCIndex(i,surface);
					cube.addPolygonIndex(polygon,index);
				}
			}
			
			delete mesh;
			
			mesh = new Mesh("editor/resources/meshes/viewcube_arrows.mesh");
			arrows = new Canvas();
			
			forloop(int surface = 0; mesh.getNumSurfaces()) {
				string name = mesh.getSurfaceName(surface);
				vec3 pos = get_polygon_pos(name);
				quat rot = get_polygon_rot(pos);
				rotations.append(rot);
				
				mesh.remapCVertex(surface);
				int polygon = arrows.addPolygon();
				
				forloop(int i = 0; mesh.getNumVertex(surface)) {
					vec3 point = mesh.getVertex(i,surface);
					arrows.addPolygonPoint(polygon,point);
					
					vec3 uv = mesh.getTexCoord0(i, surface);
					arrows.setPolygonTexCoord(polygon,uv);
				}
				
				forloop(int i = 0; mesh.getNumCIndices(surface)) {
					int index = mesh.getCIndex(i,surface);
					arrows.addPolygonIndex(polygon,index);
				}
			}
			
			delete mesh;
			
			width = main_widget.getWidth();
			height = main_widget.getHeight();
			
			width_half = width / 2.0f;
			height_half = height / 2.0f;
			
			transform_orthographic = translate(width_half,height_half,0.0f) * scale(55.0f,-55.0f,55.0f);
			transform_perspective = transform_orthographic * perspective(40.0f,1.0f,0.0f,100.0f) * translate(0,0,-3.3f);
			
			main_widget.addChild(arrows,GUI_ALIGN_OVERLAP | GUI_ALIGN_BACKGROUND);
			main_widget.addChild(cube,GUI_ALIGN_OVERLAP | GUI_ALIGN_BACKGROUND);
			
			forloop(int i = 0; cube.getNumPolygons()) cube.setPolygonTexture(i,texture_untouch);
			forloop(int i = 0; arrows.getNumPolygons()) arrows.setPolygonTexture(i,texture_untouch);
			arrows.setHidden(1);
			
			layout.arrangePanel("panel_viewcube");
		}
		
		~PanelViewCube() {
			delete cube;
		}
		
		//
		void render() {
			if(layout.isHidden("panel_viewcube")) return;
			
			Player player = controlsGetCamera();
			cam_rotation = inverse(player.getWorldRotation());
			Mat4 projection = player.getProjection();
			int is_ortho = projection.m33 == 1.0f;
			
			if(!is_mouse_over && is_ortho == ortho_i.isToggled()) {
				ortho_i.setCallbackEnabled(GUI_CLICKED,0);
				ortho_i.setToggled(!is_ortho);
				ortho_i.setCallbackEnabled(GUI_CLICKED,1);
			}
			
			if(is_ortho) cube.setTransform(transform_orthographic * cam_rotation);
			else cube.setTransform(transform_perspective * cam_rotation);
			
			arrows.setTransform(transform_orthographic * cam_rotation);
		}
		
		//
		void update(int need_reload) {
			if(layout.isHidden("panel_viewcube")) return;
			
			int mouse_x = cube.getMouseX();
			int mouse_y = cube.getMouseY();
			
			is_mouse_over = mouse_x > 0 && mouse_x <= width && mouse_y > 0 && mouse_y <= height;
			
			float mouse_distance = sqrt(pow(mouse_x - width_half,2) + pow(mouse_y - height_half,2));
			vec4 color = vec4_one;
			color.a = 1.0f - saturate(mouse_distance / width / 4.0f - 0.1f);
			if(ortho_hbox.getColor() != color) {
				ortho_hbox.setColor(color);
				forloop(int i = 0; arrows.getNumPolygons()) arrows.setPolygonColor(i,color);
			}
			
			//cube
			int cube_polygon = -1;
			
			if(is_mouse_over) {
				forloop(int i = 0; cube.getNumPolygons()) cube.setPolygonTexture(i,texture_untouch);
				cube_polygon = cube.getPolygonIntersection(mouse_x,mouse_y);
				if(cube_polygon != -1 && cube.getPolygonTexture(cube_polygon) != texture_touch) {
					cube.setPolygonTexture(cube_polygon,texture_touch);
				}
				
				forloop(int i = 0; cube.getNumPolygons()) {
					vec3 pos = cam_rotation * positions[i];
					int oreder = int((1 + pos.z) * 100);
					cube.setPolygonOrder(i,oreder);
				}
			}
			
			//arrows
			if(color.a <= 0.0f && arrows.isHidden()) return;
			
			int arrows_hidden = 1;
			
			vec3 dir = abs(normalize(inverse(cam_rotation) * vec3(0,0,1)));
			if(length(dir - vec3(1,0,0)) < 0.1f) arrows_hidden = 0;
			else if(length(dir - vec3(0,1,0)) < 0.1f) arrows_hidden = 0;
			else if(length(dir - vec3(0,0,1)) < 0.1f) arrows_hidden = 0;
			
			arrows.setHidden(arrows_hidden);
			
			int arrow_polygon = -1;
			if(!arrows_hidden && is_mouse_over) {
				forloop(int i = 0; arrows.getNumPolygons()) arrows.setPolygonTexture(i,texture_untouch);
				
				arrow_polygon = arrows.getPolygonIntersection(mouse_x,mouse_y);
				if(arrow_polygon != -1 && arrows.getPolygonTexture(arrow_polygon) != texture_touch) {
					arrows.setPolygonTexture(arrow_polygon,texture_touch);
				}
			}
			
			//mouse click
			if(!is_mouse_over) return;
			
			::Widget focus = gui.getFocus();
			
			::Widget viewport_widget = NULL;
			if(engine.gui.getNumChilds() > 0) {
				::Widget child = engine.gui.getChild(0);
				viewport_widget = child.getParent();
			}
			
			if(viewport_widget != NULL && focus == viewport_widget) {
				int mouse_state = engine.app.getMouseButtonState(APP_BUTTON_LEFT | APP_BUTTON_DCLICK);
				if(mouse_state != mouse_state_old) {
					mouse_state_old = mouse_state;
					if(mouse_state) {
						if(cube_polygon != -1) {
							controlsAnimateRotation(rotations[cube_polygon]);
						}
						
						if(arrow_polygon != -1) {
							controlsAnimateRotation(rotations[cube.getNumPolygons() + arrow_polygon]);
						}
					}
				}
			}
		}
		
		//
		void ortho_clicked(WidgetIcon icon) {
			Player player = controlsGetCameraTarget();
			
			mat4 projection = player.getProjection();
			int is_ortho = projection.m33 != 1.0f;
			
			if(is_ortho == icon.isToggled()) return;
			
			float fov = player.getFov();
			float znear = player.getZNear();
			float zfar = player.getZFar();
			float h = 16.0f;
			string data = player.getData();
			if(data != "" ) {
				int value[0];
				sscanf(data,"%d %d",value);
				h = float(value[1]);
			}
			
			if(is_ortho) projection = ortho(-h / 2.0f, h / 2.0f, -h / 2.0f, h / 2.0f,znear,zfar);
			else projection = perspective(fov,1.0f,znear,zfar);
			
			player.setData(format("%d %f",is_ortho,h));
			player.setProjection(projection);
		}
	};
} /* namespace Editor::Panels */

#endif /* __EDITOR_PANEL_VIEWCUBE_H__ */
