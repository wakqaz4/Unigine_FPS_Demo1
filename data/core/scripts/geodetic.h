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


#ifndef __UNIGINE_GEODETIC_H__
#define __UNIGINE_GEODETIC_H__

/*
 */
namespace Unigine {
	
	/*
	 */
	class Geodetic {
		
		enum {
			RAD2DEG = 57.29577951,
			DEG2RAD = 0.017453293,
		};
		
		double radius_a = 6378137.0;
		double flattening = 1.0 / 298.257223563;
		double radius_b = radius_a * (1.0 - flattening);
		double eccentricity2 = (2.0 - flattening) * flattening;
		double eccentricity22 = eccentricity2 / (1.0 - 2.0 * flattening + flattening * flattening);
		
		dvec3 wsg_origin;
		dvec4 ecf_origin;
		
		mat4 rotation;
		mat4 irotation;
		
		/*
		 */
		Geodetic() {
			setWGSOrigin(dvec3_zero);
		}
		
		Geodetic(dvec3 origin) {
			setWGSOrigin(origin);
		}
		
		Geodetic(double latitude,double longitude,double height) {
			setWGSOrigin(dvec3(latitude,longitude,height));
		}
		
		/*
		 */
		double getDegrees(double degrees,double minutes,double seconds) {
			return degrees + minutes / 60.0 + seconds / 3600.0;
		}
		
		/*
		 */
		double getDelta(dvec3 position) {
			double radius2 = length2(position.xy0);
			return sqrt(radius_a * radius_a + radius2) - radius_a;
		}
		
		/*
		 */
		double getRadius(double sin_latitude) {
			return radius_a / sqrt(1.0 - eccentricity2 * sin_latitude * sin_latitude);
		}
		
		/* Origin position for NED conversion
		 */
		void setWGSOrigin(dvec3 position) {
			wsg_origin = position;
			ecf_origin = WGSToECF(position);
			float latitude = float(position.x * DEG2RAD);
			float longitude = float(position.y * DEG2RAD);
			float sin_latitude = sin(latitude);
			float cos_latitude = cos(latitude);
			float sin_longitude = sin(longitude);
			float cos_longitude = cos(longitude);
			rotation.m00 = -sin_latitude * cos_longitude;
			rotation.m01 = -sin_latitude * sin_longitude;
			rotation.m02 =  cos_latitude;
			rotation.m10 = -sin_longitude;
			rotation.m11 =  cos_longitude;
			rotation.m12 =  0.0f;
			rotation.m20 = -cos_latitude * cos_longitude;
			rotation.m21 = -cos_latitude * sin_longitude;
			rotation.m22 = -sin_latitude;
			irotation = inverse(rotation);
		}
		
		dvec3 getWGSOrigin() {
			return wsg_origin;
		}
		
		dvec3 getECFOrigin() {
			return ecf_origin;
		}
		
		mat4 getRotation() {
			return rotation;
		}
		
		mat4 getIRotation() {
			return irotation;
		}
		
		/* Converts WGS84 position to NED relative to origin
		 */
		dvec3 getNEDPosition(dvec3 position) {
			return rotation * (WGSToECF(position) - ecf_origin);
		}
		
		/* Converts NED position to WGS84 relative to origin
		 */
		dvec3 getWGSPosition(dvec3 position) {
			return ECFToWGS(irotation * position + ecf_origin);
		}
		
		/* WGS84 to ECF
		 */
		dvec3 WGSToECF(dvec3 position) {
			dvec3 ret;
			double latitude = position.x * DEG2RAD;
			double sin_latitude = sin(latitude);
			double cos_latitude = cos(latitude);
			double radius = getRadius(sin_latitude);
			ret.x = (radius + position.z) * cos_latitude * cos(position.y * DEG2RAD);
			ret.y = (radius + position.z) * cos_latitude * sin(position.y * DEG2RAD);
			ret.z = (radius * (1.0 - eccentricity2) + position.z) * sin_latitude;
			return ret;
		}
		
		dvec3 ECFToWGS(dvec3 position) {
			dvec3 ret;
			double position_xy = length(position.xy0);
			double theta = atan2(position.z * radius_a,position_xy * radius_b);
			double sin_theta = sin(theta);
			double cos_theta = cos(theta);
			double numerator = position.z + eccentricity22 * radius_b * sin_theta * sin_theta * sin_theta;
			double denominator = position_xy - eccentricity2 * radius_a * cos_theta * cos_theta * cos_theta;
			double latitude = atan2(numerator,denominator);
			ret.x = latitude * RAD2DEG;
			ret.y = atan2(position.y,position.x) * RAD2DEG;
			ret.z = position_xy / cos(latitude) - getRadius(sin(latitude));
			return ret;
		}
	};
	
} /* namespace Unigine */

#endif /* __UNIGINE_GEODETIC_H__ */
