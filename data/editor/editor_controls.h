/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_controls.cpp
 * Desc:    Unigine editor
 * Version: 1.26
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
 *          Anna Ishina <anet@unigine.com>
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

/*
 */
namespace Controls {
	
	/* relative to distance camera shift which is unnoticeable to user
	 */
	float UNNOTICEABLE_CAMERA_SHIFT = 1e-3f;
	
	/*
	 */
	Player camera;
	Player editor_camera;
	Player camera_target;
	int camera_lock = true;
	string post_materials;
	
	Vec3 prev_mouse_direction;
	Vec3 curr_mouse_direction;
	
	Vec3 position;
	Vec3 target_position;
	int animate_focus;
	Vec3 focused_position;
	float min_velocity;
	float max_velocity;
	
	quat old_rotation;
	quat new_rotation;
	float timer_rotation;
	
	float distance;
	
	Mat4 imodelview;
	
	int enabled = 1;
	int mouse_status;
	int multiple_selection;
	int orbit_camera_enabled;
	
	/*
	 */
	void get_mouse_direction() {
		Vec3 p0,p1;
		Unigine::getPlayerMouseDirection(p0,p1);
		return normalize(p1 - p0);
	}
	
	/*
	 */
	void set_position(Vec3 pos,int orbit = false) {
		position = pos;
		if(!orbit) {
			orbit_camera_enabled = false;
			target_position = position + get_direction() * 5.0f;
		}
	}
	
	Vec3 get_position() {
		return position;
	}
	
	/*
	 */
	void set_direction(vec3 dir,vec3 up) {
		dir = normalize(dir);
		up = normalize(up);
		quat camera_rotation = quat(lookAt(vec3_zero, dir, up));
		camera.setWorldRotation(inverse(camera_rotation));
	}
	
	vec3 get_direction() {
		return rotation(imodelview) * vec3(0.0f,0.0f,-1.0f);
	}
	
	vec3 get_up() {
		return rotation(imodelview) * vec3(0.0f,1.0f,0.0f);
	}
	
	/*
	 */
	void set_camera(Player camera_,int orbit = false) {
		camera_target = node_cast(camera_);
		
		imodelview = camera_target.getWorldTransform();
		camera.setWorldTransform(imodelview);
		
		set_position(camera.getWorldPosition(),orbit);
	}
	
	/*
	 */
	void set_camera_lock(int lock) {
		camera_lock = lock;
	}
	
	int is_camera_lock() {
		return camera_lock || !engine.world.isNode(camera_target);
	}
	
	/*
	 */
	int is_control() {
		if(!(enabled && viewportsGetFocusedWindow() == NULL)) return false;
		if(engine.gui.getPermanentFocus() != NULL) return false;
		
		return	hotkeysGetState("orbit",0) ||
				hotkeysGetState("dolly",0) ||
				hotkeysGetState("track",0) ||
				hotkeysGetState("zoom_in",0) ||
				hotkeysGetState("zoom_out",0) ||
				hotkeysGetState("change_direction",0) ||
				hotkeysGetState("move_forward",0) ||
				hotkeysGetState("move_backward",0) ||
				hotkeysGetState("move_left",0) ||
				hotkeysGetState("move_right",0) ||
				hotkeysGetState("move_crouch",0) ||
				hotkeysGetState("move_jump",0) ||
				hotkeysGetState("inclination_right",0) ||
				hotkeysGetState("inclination_left",0) ||
				timer_rotation > 0;
	}
	
	/*
	 */
	void control() {
		
		float ifps = engine.app.getIFps();
		
		quat camera_rotation = camera.getWorldRotation();
		
		// camera basis
		Vec3 x = camera_rotation * Vec3(1.0f,0.0f,0.0f);
		Vec3 y = camera_rotation * Vec3(0.0f,1.0f,0.0f);
		Vec3 z = camera_rotation * Vec3(0.0f,0.0f,1.0f);
		
		hotkeysSetSkipModifier("move_run");
		
		if(is_control()) {
			if(hotkeysGetState("camera_speed_1",0)) panelsSetCameraSpeed(0);
			if(hotkeysGetState("camera_speed_2",0)) panelsSetCameraSpeed(1);
			if(hotkeysGetState("camera_speed_3",0)) panelsSetCameraSpeed(2);
		}
		
		int inclination = 0;
		if(hotkeysGetState("inclination_right",0)) inclination = 180;
		if(hotkeysGetState("inclination_left",0)) inclination = -180;
		
		float camera_velocity = min_velocity;
		if(hotkeysGetModifierState("move_run")) camera_velocity = max_velocity;
		
		orbit_camera_enabled = false;
		
		// mouse controls
		if(inclination != 0 || (engine.controls.isMouseEnabled() && (getGuiActivity() == 0 || nodesIsManipulatorFocused()))) {
			float dx = engine.controls.getMouseDX();
			float dy = engine.controls.getMouseDY();
			
			if(hotkeysGetState("dolly",0)) {
				// TOOD: get current camera znear
				float ratio = min(1.0f,max(0.0f,(max(distance,1.0f) + EPSILON) / camera_velocity / 2.0f));
				distance = max(distance - camera_velocity * (dy + dx) / 40.0f * ratio,EPSILON); // only camera reset can set distance to 0.0f
			} else if(hotkeysGetState("change_direction",0) || hotkeysGetState("orbit",0) || inclination != 0) {
				
				quat i_rotation = inverse(camera_rotation);
				i_rotation = quat(vec3(0,0,1),inclination * ifps) * quat(vec3(1,0,0),dy) * i_rotation * quat(vec3(0,0,1),dx);
				camera_rotation = inverse(i_rotation);
				
			} else if(hotkeysGetState("track",0)) {
				
				if(distance == 0.0f) { // that's right, strict equal, case when camera was reset
					Vec3 track = (x * dx - y * dy) * camera_velocity / 200.0f;
					position -= track;
					target_position -= track;
				} else if(!compare(prev_mouse_direction,Vec3_zero,EPSILON)) { // "smart" track
					
					Vec3 get_plane_intersecton(Vec3 ray_origin,Vec3 ray_vector,Vec3 plane_point,Vec3 plane_normal) {
						return ray_origin - ray_vector * dot(ray_origin - plane_point,plane_normal) / dot(ray_vector,plane_normal);
					}
					
					Vec3 camera_position = camera.getWorldPosition();
					Vec3 camera_direction = Vec3(camera.getWorldDirection());
					Vec3 plane_point = camera_position + camera_direction * max(distance,1.0f);
					Vec3 plane_normal = -camera_direction;
					Vec3 old_intersection = get_plane_intersecton(camera_position,prev_mouse_direction,plane_point,plane_normal);
					Vec3 new_intersection = get_plane_intersecton(camera_position,curr_mouse_direction,plane_point,plane_normal);
					
					Vec3 track = new_intersection - old_intersection;
					
					position -= track;
					target_position -= track;
				}
			}
			if(hotkeysGetState("dolly",0) || hotkeysGetState("track",0) || hotkeysGetState("orbit",0)) orbit_camera_enabled = true;
		}
		
		// impulse
		vec3 impulse = vec3_zero;
		vec3 zoom_impulse = vec3_zero;
		
		// keyboard controls
		int zoom_direction = 0;
		if(hotkeysGetState("zoom_in",0)) { zoom_direction = -1; zoom_impulse -= z * max_velocity; }
		if(hotkeysGetState("zoom_out",0)) { zoom_direction = 1; zoom_impulse += z * max_velocity; }
		if(hotkeysGetState("move_forward",0)) impulse -= z;
		if(hotkeysGetState("move_backward",0)) impulse += z;
		if(hotkeysGetState("move_left",0)) impulse -= x;
		if(hotkeysGetState("move_right",0)) impulse += x;
		if(hotkeysGetState("move_crouch",0)) impulse -= y;
		if(hotkeysGetState("move_jump",0)) impulse += y;
		
		impulse = normalize(impulse) * camera_velocity;
		
		hotkeysSetSkipModifier(NULL);
		
		Vec3 pos = position;
		float dist = 0;
		
		// orbit camera
		if((orbit_camera_enabled || timer_rotation > 0) && !compare(position,target_position,EPSILON) && distance != 0) {
			target_position += impulse * ifps;
			pos = target_position;
			dist = distance;
			focused_position = position;
			animate_focus = false;
		}
		// free camera
		else {
			if(animate_focus) {
				Vec3 shift = (focused_position - position) * ifps * 10;
				Vec3 new_position = position + shift;
				
				float shift_length = length(shift);
				float delta = length(focused_position - position);
				float relative_delta = delta / distance;
				
				if(relative_delta <= UNNOTICEABLE_CAMERA_SHIFT || shift_length >= delta || is_control()) {
					// jump to target position
					position = focused_position;
					animate_focus = false;
				} else {
					position = new_position;
				}
				
			} else if(zoom_direction != 0) {
				position += zoom_impulse * ifps;
				distance += length(zoom_impulse * ifps) * zoom_direction;
				if(distance < 0) distance = 0;
				focused_position = position;
			} else {
				position += impulse * ifps;
				focused_position = position;
				
				target_position = position + get_direction() * distance;
			}
			
			pos = position;
		}
		
		if(timer_rotation > 0) {
			timer_rotation -= ifps * 3;
			camera_rotation = lerp(new_rotation,old_rotation,timer_rotation);
			if(timer_rotation <= 0) camera_rotation = new_rotation;
		}
		
		// matrices
		imodelview = translate(pos) * inverse(translate(0.0f,0.0f,-dist) * inverse(camera_rotation));
		
		camera.setWorldTransform(imodelview);
		if(!is_camera_lock()) camera_target.setWorldTransform(imodelview);
		
		if(orbit_camera_enabled || timer_rotation > 0) {
			multiple_selection = 0;
			position = imodelview.col33;
		}
	}
	
	/*
	 */
	void reset_offset() {
		target_position = position;
		focused_position = position;
		distance = 0.0f;
		control();
	}
	
	
	/*
	 */
	void show_node() {
		
		::Widget focused_window = viewportsGetFocusedWindow();
		if(focused_window != NULL && focused_window != nodesGetWindow()) return;
		
		Node nodes[];
		nodesGetNodes(nodes);
		
		// empty selection
		if(!nodes.size()) return;
		
		int surfaces[0];
		Node node = nodesGetNode(surfaces);
		
		// focusing camera on itself
		if(node == camera_target) return;
		
		void focus_on_node() {
			float radius = nodesGetNodesWorldBoundRadius();
			if(MANIPULATOR_CENTER != panelPlacementGetManipulatorPivot()) radius = nodesGetBoundRadius(node);
			
			// compensate for non-standard radii
			float znear = camera.getZNear() * 10.0f;
			if(radius <= znear || radius >= 1.0e6f) {
				radius = znear;
			}
			
			target_position = nodesGetNodesWorldBoundCenter();
			if(MANIPULATOR_CENTER != panelPlacementGetManipulatorPivot()) target_position = nodesGetWorldBoundCenter(node);
			distance = radius * 3.0f; 
			focused_position = target_position - get_direction() * distance;
			animate_focus = true;
		}
		
		// focus on object surface
		if(nodes.size() == 1 && node.isObject() && surfaces.size()) {
			Object object = node_cast(node);
			
			int surface = -1;
			forloop(int i = 0; surfaces.size()) {
				if(!object.isEnabled(surfaces[i])) continue;
				surface = surfaces[i];
				break;
			}
			
			if(surface != -1) {
				WorldBoundSphere bs = new WorldBoundSphere(object.getWorldBoundSphere(surface));
				WorldBoundSphere bs_max = object.getWorldBoundSphere();
				float r_max = bs_max.getRadius();
				forloop(int i = 0; surfaces.size()) {
					if(!object.isEnabled(surfaces[i])) continue;
					if(surfaces[i] == surface) continue;
					WorldBoundSphere b = object.getWorldBoundSphere(surfaces[i]);
					bs.expand(b);
					float r = bs.getRadius();
					if(b.getRadius() > r) r = b.getRadius();
					r += length(b.getCenter() - bs.getCenter());
					bs.set(bs.getCenter(),min(r,r_max));
				}
				target_position = bs.getCenter();
				distance = bs.getRadius() * 3.0f;
				focused_position = target_position - get_direction() * distance;
				animate_focus = true;
			} else {
				focus_on_node();
			}
		}
		// focus on node
		else {
			focus_on_node();
		}
	}
	
	void animate_rotation(quat rot) {
		
		old_rotation = camera.getWorldRotation();
		new_rotation = rot;
		
		timer_rotation = 1.0f;
	}
	
	void show_object(Vec3 p,float r) {
		target_position = p;
		distance = r;
		focused_position = target_position - get_direction() * distance;
		animate_focus = true;
	}
	
	
	/*
	 */
	void init() {
		
		camera = new PlayerDummy();
		camera.addChild(toolsGetLight());
		
		editor_camera = new PlayerDummy();
		editor_camera.setName("Editor Camera");
		
		world_init();
	}
	
	void world_init() {
		distance = 0.0f;
		animate_focus = false;
		control();
	}
	
	void shutdown() {
		engine.editor.setPlayer(NULL);
		delete camera;
		delete editor_camera;
	}
	
	/*
	 */
	void snap_node(Node node) {
		node.setWorldPosition(nodesSnapPoint(node.getWorldPosition()));
	}
	
	/*
	 */
	void get_intersection(float x0,float y0,float x1,float y1,Node nodes[]) {
		
		// mouse selection
		mat4 projection;
		Mat4 modelview = camera.getModelview();
		if(compare(x0,x1,EPSILON) || compare(y0,y1,EPSILON)) return;
		if(Unigine::getPlayerMouseSelection(x0,y0,x1,y1,projection) == 0) return;
		
		// intersection nodes
		Node intersection[0];
		WorldBoundFrustum bound_frustum = new WorldBoundFrustum(projection,modelview);
		engine.world.getIntersectionNodes(bound_frustum,-1,intersection);
		
		// filter nodes
		nodes.clear();
		foreach(Node node; intersection) {
			
			if(node.isPlayer()) continue;
			if(!nodesTypeCheck(node.getType())) continue;
			
			// object surface bounding box
			if(node.isObject()) {
				int inside = 0;
				Object object = node_cast(node);
				forloop(int i = 0; object.getNumSurfaces()) {
					if(object.isEnabled(i) == 0) continue;
					if(bound_frustum.insideAll(object.getWorldBoundBox(i))) {
						inside = 1;
						break;
					}
				}
				if(inside == 0) continue;
			}
			if(node.isLight()) {
				if(node.getType() == NODE_LIGHT_WORLD) continue;
				if(!bound_frustum.inside(node.getWorldPosition())) continue;
			}
			// node bounding box
			else {
				if(bound_frustum.insideAll(node.getWorldBoundBox()) == 0) continue;
			}
			
			// node is inside
			nodes.append(node);
		}
		
		delete bound_frustum;
		return -1;
	}
	
	/*
	 */
	void update(int need_reload) {
		
		prev_mouse_direction = curr_mouse_direction;
		curr_mouse_direction = get_mouse_direction();
		
		// enable mouse
		engine.gui.setMouseEnabled(1);
		
		// mouse status
		if(engine.app.getMouseButton()) mouse_status++;
		else mouse_status = 0;
		
		// show cursor
		engine.app.setMouseShow(toolsGetSystemMouse() && gui.getMouseCursor() == GUI_CURSOR_ARROW && creatorGetNode() == NULL);
		
		if(getGuiActivity() == 0) {
			
			hotkeysSetSkipModifier("move_run");
			
			// select multiple nodes
			if(!hotkeysGetState("orbit") && !hotkeysGetState("track") && !hotkeysGetState("dolly") && !hotkeysGetState("change_direction") && enabled && (multiple_selection || (hotkeysGetState("select",0) || hotkeysGetState("rectangle_select",0) || hotkeysGetState("add_selection",0) || hotkeysGetState("remove_selection",0)) && brushesIsEnabled() == 0)) {
				
				float x0,y0;
				int mouse_x0,mouse_y0;
				
				if(multiple_selection == 0) {
					multiple_selection = 1;
					mouse_x0 = engine.app.getMouseX();
					mouse_y0 = engine.app.getMouseY();
					x0 = float(gui.getMouseX()) / gui.getWidth();
					y0 = float(gui.getMouseY()) / gui.getHeight();
				}
				
				int mouse_x1 = engine.app.getMouseX();
				int mouse_y1 = engine.app.getMouseY();
				float x1 = float(gui.getMouseX()) / gui.getWidth();
				float y1 = float(gui.getMouseY()) / gui.getHeight();
				
				if(multiple_selection == 1 && (abs(mouse_x0 - mouse_x1) > MULTIPLE_SELECTION_MIN_SIZE || abs(mouse_y0 - mouse_y1) > MULTIPLE_SELECTION_MIN_SIZE)) {
					multiple_selection = 2;
					if((hotkeysGetState("select",1) || hotkeysGetState("rectangle_select",1)) && !hotkeysGetState("add_selection",0) && !hotkeysGetState("remove_selection",0)) Nodes::deselect_node();
				}
				
				if(multiple_selection == 2) {
					Nodes::Object::set_wireframe_mode(1);
				}
				
				void line(vec3 v0,vec3 v1) {
					engine.visualizer.renderLine2D(v0,v1,vec4(69.0f / 255.0f,251.0f / 255.0f,166.0f / 255.0f,1.0f));
				}
				line(vec3(x0,y0,0.0f),vec3(x1,y0,0.0f));
				line(vec3(x1,y0,0.0f),vec3(x1,y1,0.0f));
				line(vec3(x1,y1,0.0f),vec3(x0,y1,0.0f));
				line(vec3(x0,y1,0.0f),vec3(x0,y0,0.0f));
				
				Node nodes[0];
				get_intersection(x0,y0,x1,y1,nodes);
				if(hotkeysGetModifierState("remove_selection")) {
					nodesSetSkipVisualizerNodes(nodes);
				} else {
					foreach(Node node; nodes) {
						nodesRenderVisualizer(node);
					}
				}
				Node selected[] = (:);
				if(!hotkeysGetState("select",0) && !hotkeysGetState("rectangle_select",0) && !hotkeysGetState("add_selection",0) && !hotkeysGetState("remove_selection",0)) {
					multiple_selection = 0;
					int toggle = 0;
					
					if(hotkeysGetModifierState("remove_selection")) toggle = -1;
					if(hotkeysGetModifierState("add_selection")) toggle = 1;
					
					foreach(Node node; nodes) {
						if(engine.editor.isNode(node)) {
							selected.append(node);
						} else {
							while(node.getParent() != NULL) node = node.getParent();
							Node possessor = node;
							while(possessor.getPossessor() != NULL) possessor = possessor.getPossessor();
							
							if(engine.editor.isNode(possessor)) {
								selected.append(possessor);
							} else {
								// select for NodeReference inside another NodeReference
								while(possessor != NULL) {
									if(engine.editor.isNode(possessor)) {
										selected.append(possessor);
										node = NULL;
										break;
									}
									
									if(possessor.getPossessor() != NULL) {
										possessor = possessor.getPossessor();
									} else {
										Node parent = possessor.getParent();
										while(parent != NULL && parent.getParent() != NULL) parent = parent.getParent();
										
										if(parent == NULL) break;
										
										possessor = parent.getPossessor();
									}
								}
							}
						}
					}
					
					if(selected.size()) {
						nodesSelectNodes(selected,toggle);
					}
					
					// Try select single node
					if(selected.size() == 0 && abs(mouse_x0 - mouse_x1) < MULTIPLE_SELECTION_MIN_SIZE && abs(mouse_y0 - mouse_y1) < MULTIPLE_SELECTION_MIN_SIZE) {
						
						engine.controls.setMouseEnabled(0);
						Vec3 p0,p1;
						Unigine::getPlayerMouseDirection(p0,p1);
						
						Node exclude[0];
						nodesGetExcludedNodes(exclude);
						
						WorldIntersection intersection = new WorldIntersection();
						Node node = engine.editor.getIntersection(p0,p1,exclude,intersection);
						
						if(node != NULL && node.getType() == NODE_OBJECT_TERRAIN) {
							node = engine.world.getIntersection(p0,p1,~0,intersection);
						}
						if(node != NULL) {
							if(engine.editor.isNode(node)) {
								if(node.isObject()) {
									Object object = node_cast(node);
									nodesSelect(object,intersection.getSurface(),toggle);
								} else {
									nodesSelect(node,toggle);
								}
							} else {
								while(node.getParent() != NULL) node = node.getParent();
								Node possessor = node;
								
								while(possessor.getPossessor() != NULL) possessor = possessor.getPossessor();
								
								if(engine.editor.isNode(possessor)) {
									nodesSelect(possessor,toggle);
									if(node.isObject() && intersection.getSurface() != -1) {
										Object object = node_cast(node);
										if(intersection.getSurface() < object.getNumSurfaces()) {
											Material material = object.getMaterial(intersection.getSurface());
											Property property = object.getProperty(intersection.getSurface());
											if(material != NULL) materialsSelect(material.getName());
											if(property != NULL) propertiesSelect(property.getName());
										}
									}
									node = NULL;
								} else {
									// select for NodeReference inside another NodeReference
									while(possessor != NULL) {
										if(engine.editor.isNode(possessor)) {
											nodesSelect(possessor,toggle);
											node = NULL;
											break;
										}
										
										if(possessor.getPossessor() != NULL) {
											possessor = possessor.getPossessor();
										} else {
											Node parent = possessor.getParent();
											while(parent != NULL && parent.getParent() != NULL) parent = parent.getParent();
											
											if(parent == NULL) break;
											
											possessor = parent.getPossessor();
										}
									}
								}
								
								if(node != NULL) {
									dialogMessageOk(TR("Error"),format(TR("%s is not an editor node"),typeinfo(node_cast(node))));
								}
							}
						} else {
							if(toggle == 0) nodesDeselectNode();
						}
					} else {
						if(selected.size() == 0 && toggle == 0) nodesDeselectNode();
					}
				}
				
				hotkeysSetSkipModifier(NULL);
			} else {
				engine.controls.setMouseEnabled(mouse_status > 1);
			}
		} else {
			engine.controls.setMouseEnabled(mouse_status > 1);
		}
		
		// show dynamic node info
		if(helpersGetDynamicInfo() && gui.getOverlappedFocus() == NULL) {
			float mouse_time;
			if(gui.getMouseDX() == 0 && gui.getMouseDY() == 0) {
				if(clock() - mouse_time > 0.5f) {
					
					Vec3 p0,p1;
					Unigine::getPlayerMouseDirection(p0,p1);
					
					WorldIntersection intersection = new WorldIntersection();
					Node node = engine.editor.getIntersection(p0,p1,intersection);
					
					if(node != NULL && !(node == nodesGetNode() && helpersGetNodeInfo())) {
						string info = nodesGetInfo(node);
						
						if(node.isObject() && intersection.getSurface() != -1) {
							Object object = node_cast(node);
							info = nodesGetObjectDynamicInfo(object,intersection.getSurface());
						}
						
						int x,y;
						if(Unigine::getScreenPosition(intersection.getPoint(),x,y)) {
							int width = engine.app.getWidth();
							int height = engine.app.getHeight();
							x = float((x / 4) * 4) / width;
							y = float((y / 4) * 4) / height - 0.05f;
							engine.visualizer.renderMessage2D(vec3(x,y,0.0f),vec3_zero,info,vec4_one,1);
						}
					}
				}
			} else {
				mouse_time = clock();
			}
		}
		
		engine.editor.setPlayer(camera);
		
		if(is_control() || animate_focus) {
			control();
		} else {
			camera.setWorldTransform(imodelview);
			if(!is_camera_lock()) set_camera(camera_target,true);
			target_position = position + get_direction() * distance;
		}
		
		template camera_copy_value<FUNC> {
			camera.set ## FUNC(camera_target.get ## FUNC());
		}
		
		if(engine.world.isNode(camera_target)) {
			camera_copy_value<ViewportMask>;
			camera_copy_value<ReflectionMask>;
			camera_copy_value<SourceMask>;
			camera_copy_value<ReverbMask>;
			camera_copy_value<Fov>;
			camera_copy_value<ZNear>;
			camera_copy_value<ZFar>;
			camera_copy_value<Projection>;
			if(post_materials == "") camera_copy_value<PostMaterials>;
			else camera.setPostMaterials(post_materials);
		}
		
		camera.setHandled(false);
		editor_camera.setEnabled(false);
		editor_camera.setHandled(false);
	}
	
	void render() {
		// visualizer
		engine.visualizer.setProjection(camera.getProjection());
		engine.visualizer.setModelview(camera.getModelview());
	}
	
	void set_enabled(int e) {
		enabled = e;
	}
	
	int is_enabled() {
		return enabled;
	}
	
} /* namespace Controls */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void controlsInit() {
	Controls::init();
}

void controlsWorldInit() {
	Controls::world_init();
}

void controlsShutdown() {
	Controls::shutdown();
}

Mat4 controlsGetModelview() {
	return Controls::imodelview;
}

void controlsSetCameraMinVelocity(float value) {
	Controls::min_velocity = value;
}

float controlsGetCameraMinVelocity() {
	return Controls::min_velocity;
}

void controlsSetCameraMaxVelocity(float value) {
	Controls::max_velocity = value;
}

float controlsGetCameraMaxVelocity() {
	return Controls::max_velocity;
}

/*
 */
void controlsSetPosition(vec3 position) {
	Controls::set_position(position);
	Controls::control();
}

vec3 controlsGetPosition() {
	return Controls::get_position();
}

/*
 */
void controlsSetDirection(vec3 direction,vec3 up) {
	Controls::set_direction(direction,up);
	Controls::control();
}

/*
 */
vec3 controlsGetDirection() {
	return Controls::get_direction();
}

/*
 */
vec3 controlsGetUp() {
	return Controls::get_up();
}

/*
 */
void controlsSetCamera(Player camera) {
	Controls::set_camera(camera);
}

/*
 */
void controlsSetCameraLock(int lock) {
	Controls::set_camera_lock(lock);
}

/*
 */
void controlsSetPostMaterials(string post_materials) {
	Controls::post_materials = post_materials;
}

/*
 */
string controlsGetPostMaterials() {
	return Controls::post_materials;
}

/*
 */
Player controlsGetCameraTarget() {
	return Controls::camera_target;
}

/*
 */
Player controlsGetCamera() {
	return Controls::camera;
}

/*
 */
Player controlsGetEditorCamera() {
	return Controls::editor_camera;
}

/*
 */
void controlsShowNode() {
	Controls::show_node();
}

/*
 */
void controlsAnimateRotation(quat rot) {
	Controls::animate_rotation(rot);
}

/*
 */
void controlsIsAnimateFocus() {
	return Controls::animate_focus;
}

void controlsShowObject(vec3 position,float radius) {
	Controls::show_object(position,radius);
}

/*
 */
void controlsUpdate(int need_reload) {
	Controls::update(need_reload);
}

/*
 */
void controlsRender() {
	Controls::render();
}

/*
 */
void controlsResetOffset() {
	Controls::reset_offset();
}

/*
 */
void controlsSetEnabled(int enabled) {
	Controls::set_enabled(enabled);
}

/*
 */
int controlsIsEnabled() {
	return Controls::is_enabled();
}

/*
 */
int controlsIsMultipleSelection() {
	return Controls::multiple_selection;
}

/*
 */
void controlsSetEditorCameraProjection(Mat4 mat) {
	Controls::editor_camera.setProjection(mat);
}

/*
 */
Mat4 controlsGetEditorCameraProjection() {
	return Controls::editor_camera.getProjection();
}
