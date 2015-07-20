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


#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <unigine.h>
#include <scripts/character/character_utils.h>
#include <scripts/character/animation_tree.h>
#include <scripts/character/character_locators.h>
#include <scripts/character/character_attachments.h>

/*
 */
namespace Unigine {
	
	/******************************************************************************\
	 *
	 * public
	 *
	\******************************************************************************/
	
	/*
	 */
	void initCharacter() {
		
		initAnimationTree();
	}
	
	/******************************************************************************\
	 *
	 * class Character
	 *
	\******************************************************************************/
	
	/*
	 */
	class Character {
		
		/**************************************************************************\
		 *
		 * variables
		 *
		\**************************************************************************/
		
		float speed;
		float speed_z;
		float yaw;
		float pitch;
		float step_phase;
		float rotation_speed;
		
		int ground;
		
		AngleYaw angle = new AngleYaw(0.0f);
		
		/**************************************************************************\
		 *
		 * settings
		 *
		\**************************************************************************/
		
		string name;
		
		int turn_enabled = 0;
		
		float turn_speed			= 120.0f;
		float turn_angle_threshold	= 90.0f;
		float move_rotation_speed	= 120.0f;
		float idle_speed_threshold	= 0.2f;
		float ground_threshold		= 0.1f;
		float height_stand			= 1.0f;
		float height_crouch			= 0.5f;
		float height_change_speed	= 1.0f;
		
		Vec3 skin_scale = Vec3_one;
		
		/**************************************************************************\
		 *
		 * private
		 *
		\**************************************************************************/
		
		string mesh_file_name;
		
		PlayerActor actor;
		ObjectMeshSkinned mesh;
		AnimationTree anim_tree;
		ControlsDummy controls;
		LocatorManager locators;
		AttachmentManager attachments;
		
		int own_mesh = 1;
		
		/**************************************************************************\
		 *
		 * states
		 *
		\**************************************************************************/
		
		enum {
			GROUND_STATE_STAND,
			GROUND_STATE_CROUCH,
			GROUND_STATE_JUMP,
		};
		
		enum {
			MOVE_STATE_IDLE,
			MOVE_STATE_WALK,
			MOVE_STATE_RUN,
		};
		
		enum {
			ACTION_STATE_IDLE,
			ACTION_STATE_FIRE,
			ACTION_STATE_USE,
		};
		
		enum {
			JUMP_STATE_UP,
			JUMP_STATE_FLY,
			JUMP_STATE_DOWN,
		};
		
		int getJumpState() {
			if(speed_z > 1.5f) return JUMP_STATE_UP;
			if(speed_z < -1.5f) return JUMP_STATE_DOWN;
			return JUMP_STATE_FLY;
		}
		
		void setGroundState(int ground_state) {
			
			switch(ground_state) {
				
				case GROUND_STATE_JUMP:
					controls.setState(CONTROLS_STATE_CROUCH,0);
					break;
				case GROUND_STATE_CROUCH:
					controls.setState(CONTROLS_STATE_CROUCH,1);
					break;
				case GROUND_STATE_STAND:
					controls.setState(CONTROLS_STATE_CROUCH,0);
					break;
				default:
					log.warning("Character::setGroundState(): unuspported ground_state = %d\n",ground_state);
			}
		}
		
		int getGroundState() {
			if(!ground) return GROUND_STATE_JUMP;
			if(actor.getState(PLAYER_ACTOR_STATE_CROUCH)) return GROUND_STATE_CROUCH;
			return GROUND_STATE_STAND;
		}
		
		int getMoveState() {
			if(speed > actor.getMinVelocity() + (actor.getMaxVelocity() - actor.getMinVelocity()) * 0.5f) return MOVE_STATE_RUN;
			if(speed > idle_speed_threshold) return MOVE_STATE_WALK;
			return MOVE_STATE_IDLE;
		}
		
		void setActionState(int action_state) {
			
			switch(action_state) {
				case ACTION_STATE_FIRE:
					controls.setState(CONTROLS_STATE_USE,0);
					controls.setState(CONTROLS_STATE_FIRE,1);
					break;
				case ACTION_STATE_USE:
					controls.setState(CONTROLS_STATE_FIRE,0);
					controls.setState(CONTROLS_STATE_USE,1);
					break;
				case ACTION_STATE_IDLE:
					controls.setState(CONTROLS_STATE_FIRE,0);
					controls.setState(CONTROLS_STATE_USE,0);
					break;
				default:
					log.warning("Character::setActionState(): unuspported action_state = %d\n",action_state);
			}
		}
		
		int getActionState() {
			if(actor.getState(PLAYER_ACTOR_STATE_FIRE)) return ACTION_STATE_FIRE;
			if(actor.getState(PLAYER_ACTOR_STATE_USE)) return ACTION_STATE_USE;
			return ACTION_STATE_IDLE;
		}
		
		/**************************************************************************\
		 *
		 * public interface
		 *
		\**************************************************************************/
		
		//
		void setPosition(Vec3 position) {
			actor.setPosition(position);
		}
		Vec3 getPosition() { return actor.getPosition(); }
		
		//
		void setName(string name_) { name = name_; }
		string getName() { return name; }
		
		//
		void setControlEnabled(int state) { actor.setControlled(state); }
		int isControlEnabled() { return actor.isControlled(); }
		
		//
		void setTurnEnabled(int state) { turn_enabled = state; }
		int isTurnEnabled() { return turn_enabled; }
		
		//
		void setStepPhase(float step_phase_) { step_phase = step_phase_; }
		float getStepPhase() { return step_phase; }
		
		//
		float getSpeed() { return speed; }
		float getSpeedZ() { if(getGroundState() != GROUND_STATE_JUMP) return 0.0f; return speed_z; }
		
		float getYaw() { return yaw; }
		float getPitch() { return pitch; }
		
		float getAngle() { return angle.getAngle(); }
		
		float getRotationSpeed() { return rotation_speed; }
		float getRotationSpeedAbs() { return abs(rotation_speed); }
		float getRotationSpeedSign() { return sign(rotation_speed); }
		
		//
		PlayerActor getActor() { return actor; }
		
		//
		ObjectMeshSkinned getSkin() { return mesh; }
		
		//
		ControlsDummy getControls() { return controls; }
		
		void setActorDirection(Vec3 direction) {
			
			actor.setDirection(direction);
		}
		
		//
		Vec3 getActorDirection() {
			
			Mat4 actor_transform = rotateZ(-actor.getPhiAngle()) * rotateY(actor.getThetaAngle());
			Vec3 actor_direction = normalize(actor_transform.m00m10m20);
			return actor_direction;
		}
		
		//
		void setForwardDirection(Vec3 direction) {
			
			// Note: this should be replaced by smth. like Unigine::getAngle()
			// or something more beautiful (now I'm trying to calculate angle betweend direction and x-axis)
			// We need to find angle using specified direction.
			// But the function should return correct values between -180 and 180 degrees!
			Vec3 v0 = Vec3(1.0f,0.0f,0.0f);
			Vec3 v1 = direction;
			Vec3 up = Vec3(0.0f,0.0f,1.0f);
			Vec3 c = cross(v0,v1);
			float angle = atan2(length(c),dot(v0,v1)) * RAD2DEG;
			if(dot(c,up) < 0.0f) angle = -angle;
			setAbsYaw(-angle);
		}
		
		//
		Vec3 getForwardDirection() {
			
			Mat4 actor_transform = rotateZ(-actor.getPhiAngle());
			Vec3 actor_direction = normalize(actor_transform.m00m10m20);
			return actor_direction;
		}
		
		//
		Vec3 getLeftDirection() {
			
			Mat4 actor_transform = rotateZ(-actor.getPhiAngle());
			Vec3 actor_direction = normalize(actor_transform.m01m11m21);
			return actor_direction;
		}
		
		void setInitialDirection(Vec3 direction) {
			
			setForwardDirection(direction);
			
			// to prevent turning to specified direction we set it as initial.
			angle.setDirection(direction);
			angle.setDestDirection(direction);
		}
		
		Vec3 getActorVelocity() {
			return actor.getVelocity();
		}
		
		void setActorVelocity(Vec3 velocity) {
			actor.setVelocity(velocity);
		}
		
		float getTurning() {
			actor.getTurning();
		}
		
		void setTurning(float turning) {
			actor.setTurning(turning);
		}
		
		void setEnabled(int enabled) {
			
			actor.setEnabled(enabled);
			mesh.setEnabled(enabled);
			attachments.setEnabled(enabled);
		}
		
		int isEnabled() {
			return (actor.isEnabled() && mesh.isEnabled());
		}
		
		int isVisible() {
			return (mesh.isVisible());
		}
		
		/**********************************************************************\
		 *
		 * serialization
		 *
		\**********************************************************************/
		
		//
		void load_skin(Xml xml) {
			
			// skin
			if(xml.isChild("skin")) {
				
				Xml node = xml.getChild("skin");
				Xml child;
				
				// node
				if(node.isChild("node")) {
					child = node.getChild("node");
					mesh_file_name = child.getData();
				} else {
					throw("Character::load(): character has no skin node.");
				}
				
				// scale
				if(node.isChild("scale")) {
					child = node.getChild("scale");
					skin_scale = child.getVec3Data();
				}
				
				if(mesh != NULL) node_delete(mesh);
				mesh = node_load(mesh_file_name);
				forloop(int i = 0; mesh.getNumSurfaces()) {
					mesh.setCollision(0,i);
				}
			}
			else {
				throw("Character::load(): character has no skin.");
			}
		}
		
		//
		void load_character(Xml xml) {
			
			Xml child = NULL;
			
			// name
			if(xml.isArg("name")) setName(xml.getArg("name"));
			
			// locators
			locators = new LocatorManager(mesh);
			locators.load(xml);
			
			// attachments
			attachments = new AttachmentManager(locators);
			attachments.load(xml);
			
			// animation tree
			child = xml.getChild("animation_graph");
			if(child == NULL) throw("Character::load_character(): animation tree not found.");
			anim_tree = new AnimationTree(mesh);
			anim_tree.load(child);
			
			// actor
			if(actor != NULL) delete actor;
			actor = new PlayerActor();
			
			if(xml.isChild("actor") != 0) {
				
				Xml node = xml.getChild("actor");
				Xml child;
				
				// min_velocity
				if(node.isChild("min_velocity")) {
					child = node.getChild("min_velocity");
					actor.setMinVelocity(child.getFloatData());
				}
				
				// max_velocity
				if(node.isChild("max_velocity")) {
					child = node.getChild("max_velocity");
					actor.setMaxVelocity(child.getFloatData());
				}
				
				// min_friction
				if(node.isChild("min_friction")) {
					child = node.getChild("min_friction");
					actor.setMinFriction(child.getFloatData());
				}
				
				// max_friction
				if(node.isChild("max_friction")) {
					child = node.getChild("max_friction");
					actor.setMaxFriction(child.getFloatData());
				}
				
				// damping
				if(node.isChild("damping")) {
					child = node.getChild("damping");
					actor.setDamping(child.getFloatData());
				}
				
				// jumping
				if(node.isChild("jumping")) {
					child = node.getChild("jumping");
					actor.setJumping(child.getFloatData());
				}
				
				// turn_speed
				if(node.isChild("turn_speed")) {
					child = node.getChild("turn_speed");
					turn_speed = child.getFloatData();
				}
				
				// turn_angle_threshold
				if(node.isChild("turn_angle_threshold")) {
					child = node.getChild("turn_angle_threshold");
					turn_angle_threshold = child.getFloatData();
				}
				
				// move_rotation_speed
				if(node.isChild("move_rotation_speed")) {
					child = node.getChild("move_rotation_speed");
					move_rotation_speed = child.getFloatData();
				}
				
				// idle_speed_threshold
				if(node.isChild("idle_speed_threshold")) {
					child = node.getChild("idle_speed_threshold");
					idle_speed_threshold = child.getFloatData();
				}
				
				// idle_speed_threshold
				if(node.isChild("idle_speed_threshold")) {
					child = node.getChild("idle_speed_threshold");
					idle_speed_threshold = child.getFloatData();
				}
				
				// ground_threshold
				if(node.isChild("ground_threshold")) {
					child = node.getChild("ground_threshold");
					ground_threshold = child.getFloatData();
				}
				
				// acceleration
				if(node.isChild("acceleration")) {
					child = node.getChild("acceleration");
					actor.setAcceleration(child.getFloatData());
				}
				
				// mass
				if(node.isChild("mass")) {
					child = node.getChild("mass");
					actor.setPhysicalMass(child.getFloatData());
				}
				
				// turning
				if(node.isChild("turning")) {
					child = node.getChild("turning");
					actor.setTurning(child.getFloatData());
				}
				
				// height_stand
				if(node.isChild("height_stand")) {
					child = node.getChild("height_stand");
					height_stand = child.getFloatData();
				}
				
				// height_crouch
				if(node.isChild("height_crouch")) {
					child = node.getChild("height_crouch");
					height_crouch = child.getFloatData();
				}
				
				// height_change_speed
				if(node.isChild("height_change_speed")) {
					child = node.getChild("height_change_speed");
					height_change_speed = child.getFloatData();
				}
				
				// radius
				if(node.isChild("radius")) {
					child = node.getChild("radius");
					actor.setCollisionRadius(child.getFloatData());
				}
				
				// physical
				if(node.isChild("physical")) {
					child = node.getChild("physical");
					actor.setPhysical(child.getIntData());
				}
				
				// physical mask
				if(node.isChild("physical_mask")) {
					child = node.getChild("physical_mask");
					actor.setPhysicalMask(child.getIntData());
				}
				
				// intersection mask
				if(node.isChild("intersection_mask")) {
					child = node.getChild("intersection_mask");
					actor.setIntersectionMask(child.getIntData());
				}
				
				// collision_mask
				if(node.isChild("collision_mask")) {
					child = node.getChild("collision_mask");
					actor.setCollisionMask(child.getIntData());
				}
				
				// min_angle
				if(node.isChild("min_angle")) {
					child = node.getChild("min_angle");
					actor.setMinThetaAngle(child.getFloatData());
				}
				
				// max_angle
				if(node.isChild("max_angle")) {
					child = node.getChild("max_angle");
					actor.setMaxThetaAngle(child.getFloatData());
				}
				
				actor.setCollisionHeight(height_stand);
				actor.setPhiAngle(0.0f);
				actor.setThetaAngle(0.0f);
			}
			
			height_stand = actor.getCollisionHeight();
		}
		
		//
		void load(string name) {
			
			Xml xml = new Xml("character");
			xml.load(name);
			
			load_skin(xml);
			load_character(xml);
			
			delete xml;
		}
		
		//
		void save(string name) {
			
			Xml xml = new Xml("character");
			xml.setArg("name",getName());
			
			Xml node;
			Xml child;
			
			// skin
			{
				xml.addChild("!-- skin --");
				node = xml.addChild("skin");
				
				// node
				child = node.addChild("node");
				child.setData(mesh_file_name);
				
				// scale
				child = node.addChild("scale");
				child.setVec3Data(skin_scale);
			}
			
			// actor
			{
				xml.addChild("!-- actor --");
				node = xml.addChild("actor");
				
				// min_velocity
				child = node.addChild("min_velocity");
				child.setFloatData(actor.getMinVelocity());
				
				// max_velocity
				child = node.addChild("max_velocity");
				child.setFloatData(actor.getMaxVelocity());
				
				// min_friction
				child = node.addChild("min_friction");
				child.setFloatData(actor.getMinFriction());
				
				// max_friction
				child = node.addChild("max_friction");
				child.setFloatData(actor.getMaxFriction());
				
				// damping
				child = node.addChild("damping");
				child.setFloatData(actor.getDamping());
				
				// jumping
				child = node.addChild("jumping");
				child.setFloatData(actor.getJumping());
				
				// turn_speed
				child = node.addChild("turn_speed");
				child.setFloatData(turn_speed);
				
				// turn_angle_threshold
				child = node.addChild("turn_angle_threshold");
				child.setFloatData(turn_angle_threshold);
				
				// move_rotation_speed
				child = node.addChild("move_rotation_speed");
				child.setFloatData(move_rotation_speed);
				
				// idle_speed_threshold
				child = node.addChild("idle_speed_threshold");
				child.setFloatData(idle_speed_threshold);
				
				// ground_threshold
				child = node.addChild("ground_threshold");
				child.setFloatData(ground_threshold);
				
				// acceleration
				child = node.addChild("acceleration");
				child.setFloatData(actor.getAcceleration());
				
				// mass
				child = node.addChild("mass");
				child.setFloatData(actor.getPhysicalMass());
				
				// turning
				child = node.addChild("turning");
				child.setFloatData(actor.getTurning());
				
				// height_stand
				child = node.addChild("height_stand");
				child.setFloatData(height_stand);
				
				// height_crouch
				child = node.addChild("height_crouch");
				child.setFloatData(height_crouch);
				
				// height_change_speed
				child = node.addChild("height_change_speed");
				child.setFloatData(height_change_speed);
				
				// radius
				child = node.addChild("radius");
				child.setFloatData(actor.getCollisionRadius());
				
				// physical
				child = node.addChild("physical");
				child.setIntData(actor.isPhysical());
				
				// physical mask
				child = node.addChild("physical_mask");
				child.setIntData(actor.getPhysicalMask());
				
				// intersection mask
				child = node.addChild("intersection_mask");
				child.setIntData(actor.getIntersectionMask());
				
				// collision mask
				child = node.addChild("collision_mask");
				child.setIntData(actor.getCollisionMask());
				
				// min angle
				child = node.addChild("min_angle");
				child.setFloatData(actor.getMinThetaAngle());
				
				// max angle
				child = node.addChild("max_angle");
				child.setFloatData(actor.getMaxThetaAngle());
				
			}
			
			// locators
			xml.addChild("!-- locators --");
			locators.save(xml);
			
			// attachments
			xml.addChild("!-- attachments --");
			attachments.save(xml);
			
			// anim_tree
			xml.addChild("!-- animation tree --");
			anim_tree.save(xml.addChild("animation_graph"));
			
			xml.save(name);
			delete xml;
			
		}
		
		/**********************************************************************\
		 *
		 * initialization
		 *
		\**********************************************************************/
		
		//
		void init(string template_file_name,ObjectMeshSkinned mesh_ = NULL) {
			
			// load character
			Xml xml = new Xml("character");
			xml.load(template_file_name);
			
			if(mesh_ == NULL) {
				load_skin(xml);
			} else {
				mesh = mesh_;
				own_mesh = 0;
			}
			
			load_character(xml);
			
			delete xml;
			
			// setup params
			actor.setTransform(mesh.getTransform());
			actor.setThetaAngle(0.0f);
			
			controls = new ControlsDummy();
			
			actor.setControls(controls);
			// mesh.setParent(actor);
			// mesh.setTransform(Mat4_identity);
		}
		
		//
		Character(string name) {
			init(name);
		}
		
		//
		Character(string name,ObjectMeshSkinned mesh_) {
			init(name,mesh_);
		}
		
		~Character() {
			delete actor;
			delete controls;
			if(own_mesh) node_delete(mesh);
			delete attachments;
		}
		
		/**********************************************************************\
		 *
		 * update
		 *
		\**********************************************************************/
		
		//
		void update(float dt) {
			
			// ground
			// clearControls();
			
			ground = actor.getGround();
			
			if(ground == 0) {
				Vec3 p0 = actor.getPosition() + Vec3(0.0f,0.0f,actor.getCollisionRadius());
				Vec3 p1 = p0 - Vec3(0.0f,0.0f,actor.getCollisionRadius() + ground_threshold);
				ground = engine.world.getIntersection(p0,p1,~1,NULL) != NULL;
			}
			
			// height
			int is_crouch = getGroundState() == GROUND_STATE_CROUCH;
			
			float target_height = (is_crouch) ? height_crouch : height_stand;
			float current_height = actor.getCollisionHeight();
			
			if(current_height != target_height) {
				current_height += dt * height_change_speed * sign(target_height - current_height);
				current_height = clamp(current_height,height_crouch,height_stand);
			}
			
			if(actor.getCollisionHeight() != current_height) {
				actor.setCollisionHeight(current_height);
			}
			
			// velocity
			Vec3 velocity = Vec3(actor.getVelocity());
			
			// speed_z
			speed_z = velocity.z;
			
			// speed
			float old_speed = speed;
			velocity.z = 0.0f;
			speed = length(velocity);
			
			// actor_direction
			Vec3 actor_direction = getActorDirection();
			
			// yaw & pitch
			getAngleYawPitchZ(actor_direction,angle.getDirection(),yaw,pitch);
			
			// turn if threshold angle or turn when move
			if(getMoveState() != MOVE_STATE_IDLE) {
				// turn when move
				angle.setDestDirection(velocity * move_rotation_speed);
			} else {
				
				if(old_speed >= idle_speed_threshold) {
					angle.setDestAngle(angle.getAngle());
				}
				
				if(turn_enabled) {
					float rel_yaw;
					float rel_pitch;
					
					getAngleYawPitchZ(actor_direction,angle.getDestDirection(),rel_yaw,rel_pitch);
					
					if(rel_yaw < -turn_angle_threshold) {
						angle.setDestAngle(angle.getDestAngle() - turn_angle_threshold,turn_speed);
					} else if(rel_yaw > turn_angle_threshold) {
						angle.setDestAngle(angle.getDestAngle() + turn_angle_threshold,turn_speed);
					}
				}
				
			}
			
			// update angle
			rotation_speed = angle.update(dt);
			
			// animation
			anim_tree.update(dt);
			anim_tree.setup();
			
			// mesh transform
			mesh.setTransform(Mat4(getPosition()) * angle.getTransform(90.0f) * scale(skin_scale));
			
			// attachments
			attachments.update();
			
			// magic hack :)
			engine.world.addUpdateNode(actor);
		}
		
		/**************************************************************************\
		 *
		 * controls
		 *
		\**************************************************************************/
		
		//
		void clearControls() {
			controls.setState(CONTROLS_STATE_FORWARD,0);
			controls.setState(CONTROLS_STATE_BACKWARD,0);
			controls.setState(CONTROLS_STATE_MOVE_LEFT,0);
			controls.setState(CONTROLS_STATE_MOVE_RIGHT,0);
			controls.setState(CONTROLS_STATE_TURN_LEFT,0);
			controls.setState(CONTROLS_STATE_TURN_RIGHT,0);
			controls.setState(CONTROLS_STATE_JUMP,0);
			controls.setState(CONTROLS_STATE_CROUCH,0);
			controls.setState(CONTROLS_STATE_RUN,0);
			controls.setMouseDX(0.0f);
			controls.setMouseDY(0.0f);
		}
		
		//
		void moveLeft() {
			controls.setState(CONTROLS_STATE_MOVE_LEFT,1);
		}
		
		//
		void moveRight() {
			controls.setState(CONTROLS_STATE_MOVE_RIGHT,1);
		}
		
		//
		void moveForward() {
			controls.setState(CONTROLS_STATE_FORWARD,1);
		}
		
		//
		void moveBackward() {
			controls.setState(CONTROLS_STATE_BACKWARD,1);
		}
		
		//
		void turnLeft() {
			controls.setState(CONTROLS_STATE_TURN_LEFT,1);
		}
		
		//
		void turnRight() {
			controls.setState(CONTROLS_STATE_TURN_RIGHT,1);
		}
		
		//
		void turnUp() { }
		
		//
		void turnDown() { }
		
		//
		void turnPitch(float angle) {
			controls.setMouseDY(angle);
		}
		
		//
		void turnYaw(float angle) {
			controls.setMouseDX(angle);
		}
		
		//
		float getAbsYaw() {
			return actor.getPhiAngle();
		}
		
		//
		void setAbsYaw(float angle) {
			actor.setPhiAngle(angle);
		}
		
		//
		float getAbsPitch() {
			return actor.getThetaAngle();
		}
		
		//
		void setAbsPitch(float angle) {
			actor.setThetaAngle(angle);
		}
		
		//
		void crouch() {
			controls.setState(CONTROLS_STATE_CROUCH,1);
		}
		
		//
		void jump() {
			controls.setState(CONTROLS_STATE_JUMP,1);
		}
		
		//
		void run() {
			controls.setState(CONTROLS_STATE_RUN,1)
		}
		
	};
	
}

#endif
