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


#ifndef __CHARACTER_CAMERA_H__
#define __CHARACTER_CAMERA_H__

#include <core/unigine.h>
#include <core/scripts/utils.h>
#include <scripts/character/character_utils.h>
#include <scripts/character/character.h>

namespace Unigine {
	
	/**************************************************************************\
	 *
	 * class CharacterCamera
	 *
	\**************************************************************************/
	
	/*
	 */
	class CharacterCamera {
		
		PlayerPersecutor persecutor;
		PlayerActor actor;
		Character character;
		ObjectMeshSkinned skin;
		
		Mat4 offset_matrix;
		Vec3 shift;
		string locator;
		
		float fov;
		float distance_change_speed;
		float distance;
		float zoom_step;
		
		int type;
		int fixed_distance;
		int collision;
		int smooth_corners;
		int smooth_corners_steps;
		
		Character getCharacter() {
			return character;
		}
		
		void setCharacter(Character character_) {
			character = character_;
			if(character != NULL) {
				actor = character.getActor();
				skin = character.getSkin();
			} else {
				actor = NULL;
				skin = NULL;
			}
			persecutor.setTarget(actor);
			persecutor.setDistance(distance);
		}
		
		//
		CharacterCamera(Character character_) {
			
			// create persecutor and setup its default values
			persecutor = new PlayerPersecutor();
			
			setCharacter(character_);
			
			persecutor.setThetaAngle(0.0f);
			persecutor.setPhiAngle(0.0f);
			
			persecutor.setControls(NULL);
			persecutor.setMinDistance(1.0f);
			persecutor.setMaxDistance(10.0f);
			persecutor.setDistance(4.0f);
			persecutor.setCollision(0);
			persecutor.setMinThetaAngle(-30.0f);
			persecutor.setZFar(10000.0f);
			//persecutor.setFixed(1);
			
			persecutor.setAnchor(Vec3(0.0f,0.0f,1.5f));
			
			distance_change_speed = 10.0f;
			fov = 60.0f;
			type = 0;
			fixed_distance = 0;
			distance = 10.0f;
			zoom_step = 1.0f;
			collision = 0;
			smooth_corners = 0;
			smooth_corners_steps = 10;
		}
		
		//
		~CharacterCamera() {
			
			if(engine.game.getPlayer() == persecutor) {
				engine.game.setPlayer(NULL);
			}
			delete persecutor;
		}
		
		/**********************************************************************\
		 *
		 * public interface
		 *
		\**********************************************************************/
		
		//
		void setActive() {
			engine.game.setPlayer(persecutor);
		}
		
		//
		void setOffsetMatrix(Mat4 offset) {
			
			Vec3 size = getTransformScale(offset);
			persecutor.setFov(fov * size.z);
			offset_matrix = offset;
			Vec3 isize = Vec3(1.0f / size.x,1.0f / size.y,1.0f / size.z);
			persecutor.setOffset(mat4(scale(isize) * offset_matrix * translate(shift)));
		}
		
		//
		Vec3 getPosition() {
			return persecutor.getPosition();
		}
		
		//
		Vec3 getDirection() {
			return persecutor.getDirection();
		}
		
		//
		Vec3 getShift() {
			return shift;
		}
		
		//
		void setShift(Vec3 shift_) {
			shift = shift_;
			setOffsetMatrix(offset_matrix);
		}
		
		//
		string getLocator() {
			return locator;
		}
		
		//
		void setLocator(string locator_) {
			locator = locator_;
		}
		
		//
		float getFov() {
			return fov;
		}
		
		//
		void setFov(float fov_) {
			fov = fov_;
			setOffsetMatrix(offset_matrix);
		}
		
		//
		void zoom(float value) {
			distance = clamp(distance + sign(value) * zoom_step,persecutor.getMinDistance(),persecutor.getMaxDistance());
		}
		
		/**********************************************************************\
		 *
		 * serialization
		 *
		\**********************************************************************/
		
		//
		void save(Xml xml) {
			
			Xml node = xml.addChild("camera");
			Xml child;
			
			node.setArg("locator",locator);
			
			// anchor
			child = node.addChild("anchor");
			child.setVec3Data(persecutor.getAnchor());
			
			// shift
			child = node.addChild("shift");
			child.setVec3Data(getShift());
			
			// fixed_distance
			child = node.addChild("fixed_distance");
			child.setIntData(fixed_distance);
			
			// distance
			child = node.addChild("distance");
			child.setFloatData(distance);
			
			// distance_change_speed
			child = node.addChild("distance_change_speed");
			child.setFloatData(distance_change_speed);
			
			// type
			child = node.addChild("type");
			child.setIntData(type);
			
			// collision
			child = node.addChild("collision");
			child.setIntData(collision);
			
			// min_angle
			child = node.addChild("min_angle");
			child.setFloatData(persecutor.getMinThetaAngle());
			
			// max_angle
			child = node.addChild("max_angle");
			child.setFloatData(persecutor.getMaxThetaAngle());
			
			// radius
			child = node.addChild("radius");
			child.setFloatData(persecutor.getCollisionRadius());
			
			// turning
			child = node.addChild("turning");
			child.setFloatData(persecutor.getTurning());
			
			// z_near
			child = node.addChild("z_near");
			child.setFloatData(persecutor.getZNear());
			
			// z_far
			child = node.addChild("z_far");
			child.setFloatData(persecutor.getZFar());
			
			// fov
			child = node.addChild("fov");
			child.setFloatData(fov);
			
			// collision_mask
			child = node.addChild("collision_mask");
			child.setIntData(persecutor.getCollisionMask());
			
			// min_distance
			child = node.addChild("min_distance");
			child.setFloatData(persecutor.getMinDistance());
			
			// max_distance
			child = node.addChild("max_distance");
			child.setFloatData(persecutor.getMaxDistance());
			
			// zoom_step
			child = node.addChild("zoom_step");
			child.setFloatData(zoom_step);
			
			// smooth_corners
			child = node.addChild("smooth_corners");
			child.setIntData(smooth_corners);
			
			// smooth_corners_steps
			child = node.addChild("smooth_corners_steps");
			child.setIntData(smooth_corners_steps);
			
		}
		
		//
		void load(Xml xml) {
			
			//if(xml.isChild("camera") == 0) return;
			
			Xml node = xml;//.getChild("camera");
			
			if(node.isArg("locator")) {
				locator = node.getArg("locator");
			}
			
			Xml child;
			
			// anchor
			if(node.isChild("anchor")) {
				child = node.getChild("anchor");
				persecutor.setAnchor(Vec3(child.getVec3Data()));
			}
			
			// shift
			if(node.isChild("shift")) {
				child = node.getChild("shift");
				setShift(Vec3(child.getVec3Data()));
			}
			
			// fixed_distance
			if(node.isChild("fixed_distance")) {
				child = node.getChild("fixed_distance");
				fixed_distance = child.getIntData();
			}
			
			// distance
			if(node.isChild("distance")) {
				child = node.getChild("distance");
				distance = child.getFloatData();
			}
			
			// distance_change_speed
			if(node.isChild("distance_change_speed")) {
				child = node.getChild("distance_change_speed");
				distance_change_speed = child.getFloatData();
			}
			
			// type
			if(node.isChild("type")) {
				child = node.getChild("type");
				type = child.getIntData();
			}
			
			// collision
			if(node.isChild("collision")) {
				child = node.getChild("collision");
				collision = child.getIntData();
			}
			
			// min_angle
			if(node.isChild("min_angle")) {
				child = node.getChild("min_angle");
				persecutor.setMinThetaAngle(child.getFloatData());
			}
			
			// max_angle
			if(node.isChild("max_angle")) {
				child = node.getChild("max_angle");
				persecutor.setMaxThetaAngle(child.getFloatData());
			}
			
			// radius
			if(node.isChild("radius")) {
				child = node.getChild("radius");
				persecutor.setCollisionRadius(child.getFloatData());
			}
			
			// turning
			if(node.isChild("turning")) {
				child = node.getChild("turning");
				persecutor.setTurning(child.getFloatData());
			}
			
			// z_near
			if(node.isChild("z_near")) {
				child = node.getChild("z_near");
				persecutor.setZNear(child.getFloatData());
			}
			
			// z_far
			if(node.isChild("z_far")) {
				child = node.getChild("z_far");
				persecutor.setZFar(child.getFloatData());
			}
			
			// fov
			if(node.isChild("fov")) {
				child = node.getChild("fov");
				setFov(child.getFloatData());
			}
			
			// collision_mask
			if(node.isChild("collision_mask")) {
				child = node.getChild("collision_mask");
				persecutor.setCollisionMask(child.getIntData());
			}
			
			// min_distance
			if(node.isChild("min_distance")) {
				child = node.getChild("min_distance");
				persecutor.setMinDistance(child.getFloatData());
			}
			
			// max_distance
			if(node.isChild("max_distance")) {
				child = node.getChild("max_distance");
				persecutor.setMaxDistance(child.getFloatData());
			}
			
			// zoom_step
			if(node.isChild("zoom_step")) {
				child = node.getChild("zoom_step");
				zoom_step = child.getFloatData();
			}
			
			// smooth_corners
			if(node.isChild("smooth_corners")) {
				child = node.getChild("smooth_corners");
				smooth_corners = child.getIntData();
			}
			
			// smooth_corners_steps
			if(node.isChild("smooth_corners_steps")) {
				child = node.getChild("smooth_corners_steps");
				smooth_corners_steps = child.getIntData();
			}
			
			persecutor.setDistance(distance);
			
		}
		
		void load_from_file(string name) {
			
			Xml xml = new Xml("camera");
			xml.load(name);
			load(xml);
			delete xml;
		}
		
		void save_to_file(string name) {
			
			Xml xml = new Xml("camera");
			save(xml);
			xml.save(name);
			delete xml;
		}
		
		/**********************************************************************\
		 *
		 * update
		 *
		\**********************************************************************/
		
		//
		void update(float dt) {
			
			if(character == NULL) return;
			
			if(type == 0) {
				// fixed both
				persecutor.setPhiAngle(actor.getPhiAngle());
				persecutor.setThetaAngle(actor.getThetaAngle());
			} else 
			if(type == 1) {
				// fixed theta
				actor.setPhiAngle(persecutor.getPhiAngle());
				persecutor.setThetaAngle(actor.getThetaAngle());
			} else
			if(type == 2) {
				// fixed phi
				actor.setThetaAngle(persecutor.getThetaAngle());
				persecutor.setPhiAngle(actor.getPhiAngle());
			} else 
			if(type == 3) {
				// free
				actor.setPhiAngle(persecutor.getPhiAngle());
				actor.setThetaAngle(persecutor.getThetaAngle());
			}
			
			if(collision) {
				
				// this is a point to wich our persecutor is bound
				Vec3 p0 = actor.getWorldTransform() * persecutor.getAnchor();
				// this is our persecutor's position
				Vec3 pp = p0 + normalize(persecutor.getPosition() - p0) * (distance + persecutor.getCollisionRadius());
				
				// we should avoid intersections with our own skin
				WorldIntersection intersection = new WorldIntersection();
				if(engine.world.getIntersection(p0,pp,~0,(skin),intersection) != NULL) {
					
					// if we have any intersections on our way to the target zoom in
					Vec3 direction = p0 - intersection.getPoint();
					float new_distance = length(direction) - persecutor.getCollisionRadius();
					if(persecutor.getDistance() > new_distance) {
						persecutor.setDistance(new_distance);
					}
				}
				else {
					
					float dest_distance = distance;
					
					if(smooth_corners) {
						
						Vec3 shifts[] = (Vec3(1.0f,0.0f,0.0f), Vec3(-1.0f,0.0f,0.0f), Vec3(0.0f,1.0f,0.0f), Vec3(0.0f,-1.0f,0.0f));
						
						forloop(int j = 0; shifts.size()) {
							
							// this is our persecutor's position
							Vec3 p1 = pp;
							Vec3 p2 = p0 + normalize(p1 - p0) * distance + 
								normalize(persecutor.getTransform() * shifts[j] - 
								persecutor.getTransform() * Vec3(0.0f,0.0f,0.0f)) * persecutor.getCollisionRadius();
							Vec3 p3;
							float dir = normalize(p1 - p0);
							float angle = acos(dot(dir,normalize(p2 - p0)));
							float corner_distance;
							
							int corner = 0;
							
							for(int i = 1; i <= smooth_corners_steps; i++) {
								
								float k = float(i) / smooth_corners_steps;
								
								if(engine.world.getIntersection(p0,lerp(p1,p2,k),~0,(skin),NULL) != NULL) {
									p1 = lerp(p1,p2,k - 1.0f / smooth_corners_steps);
									p2 = lerp(p1,p2,k);
									corner = 1;
									break;
								}
								
							}
							
							if(corner == 1) {
								Vec3 old_p3;
								int i = 0;
								forloop(i = 0; 10) {
									
									p3 = lerp(p1,p2,0.5f);
									
									if(engine.world.getIntersection(p0,p3,~0,(skin),intersection) != NULL) {
										p2 = p3;
										corner_distance = length(intersection.getPoint() - p0) - persecutor.getCollisionRadius();
									} else {
										p1 = p3;
									}
									
									if(i != 0 && abs(1.0f - dot(normalize(p3 - p0),normalize(old_p3 - p0))) < EPSILON) break;
									
									old_p3 = p3;
								}
								
								p3 = lerp(p1,p2,0.5f);
								
								float new_distance = lerp(corner_distance,distance,acos(dot(dir,normalize(p3 - p0))) / angle);
								if(dest_distance > new_distance) dest_distance = new_distance;
							}
						}
					}
					
					if(fixed_distance != 0) {
						
						float cur_distance = persecutor.getDistance();
						float delta_distance = dest_distance - cur_distance;
						
						if(abs(delta_distance) > dest_distance * 0.001f) {
							cur_distance += min(abs(delta_distance),dt * distance_change_speed) * sign(delta_distance);
							persecutor.setDistance(cur_distance);
						}
					}
				}
			}
			
			// camera shaking
			if(getLocator() != "") {
				Locator camera_locator = character.locators.getLocator(getLocator());
				if(camera_locator != NULL) setOffsetMatrix(camera_locator.getRelativeTransform());
			}
		}
	};
	
}

#endif
