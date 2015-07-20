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


#ifndef __CHARACTER_UTILS_H__
#define __CHARACTER_UTILS_H__

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * functions
	 *
	\******************************************************************************/
	
	/*
	 */
	float getAngleZ(Vec3 dir0_,Vec3 dir1_) {
		
		Vec3 dir0 = Vec3(dir0_.x,dir0_.y,0.0f);
		Vec3 dir1 = Vec3(dir1_.x,dir1_.y,0.0f);
		
		float length0 = length(dir0);
		if(length0 < EPSILON) return 0.0f;
		dir0 /= length0;
		
		float length1 = length(dir0);
		if(length1 < EPSILON) return 0.0f;
		dir1 /= length1;
		
		if(length(dir1 - dir0) < 0.002f) return 0.0f;
		
		float da = acos(clamp(dot(dir0,dir1),-1.0f + EPSILON,1.0f - EPSILON)) * RAD2DEG;
		
		Vec3 axis = cross(dir0,dir1);
		if(axis.z < 0.0f) da = -da;
		
		return da;
	}
	
	/*
	 */
	float getAngleYawPitchZ(Vec3 dir0_,Vec3 dir1_,float &yaw,float &pitch) {
		
		Vec3 dir1 = Vec3(normalize(dir1_));
		Vec3 dir0 = Vec3(normalize(dir0_));
		
		// yaw angle calculation
		dir0.z = 0.0f;
		dir0 = normalize(dir0);
		yaw = acos(clamp(dot(dir0,dir1),-1.0f + EPSILON,1.0f - EPSILON)) * RAD2DEG;
		Vec3 axis = cross(dir1,dir0);
		if(axis.z < 0.0f) yaw = -yaw;
		
		// pitch angle calculation
		Vec3 dir = dir0_;
		float dir_z = dir.z;
		dir.z = 0.0f;
		float dir_len = length(dir);
		dir0 = dir1 * dir_len + Vec3(0.0f,0.0f,dir_z);
		
		pitch = acos(clamp(dot(dir0,dir1),-1.0f + EPSILON,1.0f - EPSILON)) * RAD2DEG;
		axis = cross(dir1,dir0);
		if(dot(axis,Vec3(-dir1.y,dir1.x,0.0f)) < 0.0f) pitch = -pitch;
	}
	
	/******************************************************************************\
	 *
	 * class AngleYaw
	 *
	\******************************************************************************/
	
	/*
	 */
	class AngleYaw {
		
		float angle;
		float dest_angle;
		Vec3 direction;
		Vec3 dest_direction;
		
		//
		float clamp_angle(float angle) {
			while(angle > 180.0f) angle -= 360.0f;
			while(angle < -180.0f) angle += 360.0f;
			return angle;
		}
		
		//
		Vec3 get_direction_by_angle(float angle) {
			Vec3 ret;
			ret.x = cos(angle * DEG2RAD);
			ret.y = sin(angle * DEG2RAD);
			ret.z = 0.0f;
			return ret;
		}
		
		//
		float get_angle_by_direction(Vec3 direction) {
			
			/// Note: this code is wrong,
			/// because asin can return angles only between -90 and 90 degress
			/// And for character we have angles between -180 and 180
			//Vec3 dir = normalize(direction);
			//return asin(dir.x) * RAD2DEG;
			
			// This piece of code calculates angle between x-axis and direction
			// It can return angles between -180 and 180.
			Vec3 v0 = Vec3(1.0f,0.0f,0.0f);
			Vec3 v1 = direction;
			Vec3 up = Vec3(0.0f,0.0f,1.0f);
			Vec3 c = cross(v0,v1);
			float angle = atan2(length(c),dot(v0,v1)) * RAD2DEG;
			if(dot(c,up) < 0.0f) angle = -angle;
			return angle;
		}
		
		//
		float getAngle() { return angle; }
		float getDestAngle() { return dest_angle; }
		Vec3 getDirection() { return direction; }
		Vec3 getForwardDirection() { return get_direction_by_angle(angle); }
		Vec3 getRightDirection() { return get_direction_by_angle(angle + 90.0f); }
		Vec3 getDestDirection() { return dest_direction; }
		
		//
		void setDestAngle(float angle_,float speed = 1.0f) {
			dest_angle = clamp_angle(angle_);
			dest_direction = get_direction_by_angle(dest_angle) * speed;
		}
		
		//
		void setDestDirection(Vec3 direction_) {
			dest_direction = direction_;
			dest_direction.z = 0.0f;
			dest_angle = get_angle_by_direction(dest_direction);
		}
		
		//
		void setAngle(float angle_) {
			angle = clamp_angle(angle_);
			direction = get_direction_by_angle(angle);
		}
		
		//
		void setDirection(Vec3 direction_) {
			direction = direction_;
			direction.z = 0.0f;
			angle = get_angle_by_direction(direction);
		}
		
		//
		float update(float dt) {
			
			if(dt == 0.0f) return 0.0f;
			
			float speed = length(dest_direction);
			
			// no moving - no rotation
			if(speed < EPSILON) return 0.0f;
			
			// linear rotation
			float da = getAngleZ(dest_direction,direction);
			da = min(abs(da),dt * speed) * sign(da);
			
			setAngle(angle - da);
			return da / dt;
			
		}
		
		//
		Mat4 getTransform(float angle_shift = 0.0f) {
			return Mat4(quat(Vec3(0.0f,0.0f,1.0f),angle + angle_shift))
		}
		
		//
		AngleYaw(float angle_ = 0.0f) {
			setAngle(angle_);
			setDestAngle(angle_);
		}
		
	};
	
}

#endif
