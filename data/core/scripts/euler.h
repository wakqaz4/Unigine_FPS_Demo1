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


#ifndef __UNIGINE_EULER_H__
#define __UNIGINE_EULER_H__

#include <core/unigine.h>

/*
 */
namespace Unigine {
	
	/*
	 */
	vec3 decomposeRotationXYZ(mat4 transform) {
		vec3 r = vec3_zero;
		if(transform.m02 < 1.0f) {
			if(transform.m02 > -1.0f) {
				r.x = atan2(-transform.m12,transform.m22);
				r.y = asin(transform.m02);
				r.z = atan2(-transform.m01,transform.m00);
			} else {
				r.x = -atan2(transform.m10,transform.m11);
				r.y = -PI05;
			}
		} else {
			r.x = atan2(transform.m10,transform.m11);
			r.y = PI05;
		}
		return r * RAD2DEG;
	}
	
	mat4 composeRotationXYZ(vec3 r) {
		return mat4(quat(r.x,r.y,r.z));
	}
	
	/*
	 */
	vec3 decomposeRotationXZY(mat4 transform) {
		vec3 r = vec3_zero;
		if(transform.m01 < 1.0f) {
			if(transform.m01 > -1.0f) {
				r.x = atan2(transform.m21,transform.m11);
				r.y = atan2(transform.m02,transform.m00);
				r.z = asin(-transform.m01);
			} else {
				r.x = -atan2(-transform.m20,transform.m22);
				r.z = PI05;
			}
		} else {
			r.x = atan2(-transform.m20,transform.m22);
			r.z = -PI05;
		}
		return r * RAD2DEG;
	}
	
	mat4 composeRotationXZY(vec3 r) {
		return mat4(quat(1.0f,0.0f,0.0f,r.x) * quat(0.0f,0.0f,1.0f,r.z) * quat(0.0f,1.0f,0.0f,r.y));
	}
	
	/*
	 */
	vec3 decomposeRotationYXZ(mat4 transform) {
		vec3 r = vec3_zero;
		if(transform.m12 < 1.0f) {
			if(transform.m12 > -1.0f) {
				r.x = asin(-transform.m12);
				r.y = atan2(transform.m02,transform.m22);
				r.z = atan2(transform.m10,transform.m11);
			} else {
				r.x = PI05;
				r.y = -atan2(-transform.m01,transform.m00);
			}
		} else {
			r.x = -PI05;
			r.y = atan2(-transform.m01,transform.m00);
		}
		return r * RAD2DEG;
	}
	
	mat4 composeRotationYXZ(vec3 r) {
		return mat4(quat(0.0f,1.0f,0.0f,r.y) * (quat(1.0f,0.0f,0.0f,r.x) * quat(0.0f,0.0f,1.0f,r.z)));
	}
	
	/*
	 */
	vec3 decomposeRotationYZX(mat4 transform) {
		vec3 r = vec3_zero;
		if(transform.m10 < 1.0f) {
			if(transform.m10 > -1.0f) {
				r.x = atan2(-transform.m12,transform.m11);
				r.y = atan2(-transform.m20,transform.m00);
				r.z = asin(transform.m10);
			} else {
				r.y = -atan2(transform.m21,transform.m22);
				r.z = -PI05;
			}
		} else {
			r.y = atan2(transform.m21,transform.m22);
			r.z = PI05;
		}
		return r * RAD2DEG;
	}
	
	mat4 composeRotationYZX(vec3 r) {
		return mat4(quat(0.0f,1.0f,0.0f,r.y) * (quat(0.0f,0.0f,1.0f,r.z) * quat(1.0f,0.0f,0.0f,r.x)));
	}
	
	/*
	 */
	vec3 decomposeRotationZXY(mat4 transform) {
		vec3 r = vec3_zero;
		if(transform.m21 < 1.0f) {
			if(transform.m21 > -1.0f) {
				r.x = asin(transform.m21);
				r.y = atan2(-transform.m20,transform.m22);
				r.z = atan2(-transform.m01,transform.m11);
			} else {
				r.x = -PI05;
				r.z = -atan2(transform.m02,transform.m00);
			}
		} else {
			r.x = PI05;
			r.z = atan2(transform.m02,transform.m00);
		}
		return r * RAD2DEG;
	}
	
	mat4 composeRotationZXY(vec3 r) {
		return mat4(quat(0.0f,0.0f,1.0f,r.z) * (quat(1.0f,0.0f,0.0f,r.x) * quat(0.0f,1.0f,0.0f,r.y)));
	}
	
	/*
	 */
	vec3 decomposeRotationZYX(mat4 transform) {
		vec3 r = vec3_zero;
		if(transform.m20 < 1.0f) {
			if(transform.m20 > -1.0f) {
				r.x = atan2(transform.m21,transform.m22);
				r.y = asin(-transform.m20);
				r.z = atan2(transform.m10,transform.m00);
			} else {
				r.y = PI05;
				r.z = -atan2(transform.m01,transform.m02);
			}
		} else {
			r.y = -PI05;
			r.z = atan2(-transform.m01,-transform.m02);
		}
		return r * RAD2DEG;
	}
	
	mat4 composeRotationZYX(vec3 r) {
		return mat4(quat(0.0f,0.0f,1.0f,r.z) * (quat(0.0f,1.0f,0.0f,r.y) * quat(1.0f,0.0f,0.0f,r.x)));
	}
	
} /* namespace Unigine */

#endif /* __UNIGINE_EULER_H__ */
