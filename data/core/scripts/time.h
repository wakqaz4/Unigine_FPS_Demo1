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


#ifndef __UNIGINE_TIME_H__
#define __UNIGINE_TIME_H__

#include <core/unigine.h>

/*
 */
namespace Unigine {
	
	/*
	 */
	class Time {
		
		int year = 2000;
		int month = 1;
		int day = 1;
		int hour;
		int minute;
		float second;
		
		float time_zone;
		float latitude;
		float longitude;
		
		float altitude;
		float azimuth;
		
		string day_names[7] = (
			"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday",
		);
		
		string month_names[12] = (
			"January", "February", "March", "April", "May", "June",
			"July", "August", "September", "October", "November", "December",
		);
		
		/*
		 */
		Time(float zone = 9.0f,float lat = 34.0f,float lon = 144.0f) {
			time_zone = zone;
			latitude = lat;
			longitude = lon;
		}
		
		/*
		 */
		void update(float ifps) {
			second += ifps;
			while(second >= 60.0f) {
				second -= 60.0f;
				minute++;
			}
			while(minute >= 60) {
				minute -= 60;
				hour++;
			}
			while(hour >= 24) {
				hour -= 24;
				day++;
			}
			while(day > 30) {
				day -= 30;
				month++;
			}
			while(month > 12) {
				month -= 12;
				year++;
			}
		}
		
		/*
		 */
		void setDate(int y,int m,int d) {
			year = int(y);
			month = int(m);
			day = int(d);
			update(0.0f);
		}
		
		void setTime(int h,int m,float s) {
			hour = int(h);
			minute = int(m);
			second = float(s);
			update(0.0f);
		}
		
		void setSeconds(float s) {
			hour = int(s / 3600.0f);
			minute = int(s / 60.0f) % 60;
			second = float(s) - hour * 3600.f - minute * 60.f;
			update(0.0f);
		}
		
		/*
		 */
		float getSeconds() {
			return hour * 3600.0f + minute * 60.0f + second;
		}
		
		float getMinutes() {
			return hour * 60.0f + minute + second / 60.0f;
		}
		
		float getHours() {
			return hour + minute / 60.0f + second / 3600.0f;
		}
		
		/*
		 */
		string getDate(string format = "%D %M %y") {
			string ret = "";
			forloop(int i = 0; strlen(format)) {
				if(format[i] == '%') {
					switch(format[++i]) {
						case 'd': ret += string(day); break;
						case 'D': ret += day_names[(day - 1) % 7]; break;
						case 'm': ret += string(month); break;
						case 'M': ret += month_names[month - 1]; break;
						case 'y': ret += string(year); break;
						case '%': ret += "%"; break;
						default: throw("Unigine::Time::getDate(): unknown format \"%c\" in \"%s\" string\n",format[i],format);
					}
				} else {
					ret += ::format("%c",format[i]);
				}
			}
			return ret;
		}
		
		string getTime(string format = "%h:%m:%s") {
			string ret = "";
			forloop(int i = 0; strlen(format)) {
				if(format[i] == '%') {
					switch(format[++i]) {
						case 'h': ret += ::format("%02d",hour); break;
						case 'm': ret += ::format("%02d",minute); break;
						case 's': ret += ::format("%02d",int(second)); break;
						case '%': ret += "%"; break;
						default: throw("Unigine::Time::getTime(): unknown format \"%c\" in \"%s\" string\n",format[i],format);
					}
				} else {
					ret += ::format("%c",format[i]);
				}
			}
			return ret;
		}
		
		/*
		 */
		float get_ut() {
			return getHours() - time_zone;
		}
		
		float get_day(float ut) {
			float d = day;
			if(ut < 0.0f) {
				d -= 1;
				ut += 24.0f;
			}
			if(ut > 24.0f) {
				d += 1;
				ut -= 24.0f;
			}
			d += 367 * year - (7 * (year + (month + 9) / 12)) / 4 + (275 * month) / 9 - 730530 + ut / 24.0f;
			return d;
		}
		
		/*
		 */
		Vec3 getSunDirection() {
			
			float ut = get_ut();
			float d = get_day(ut);
			
			float w = 282.9404f + 4.70935e-5f * d;
			float e = 0.016709f - 1.151e-9f * d;
			float m = 356.0470f + 0.9856002585f * d;
			float l = w + m;
			
			float oblecl = (23.4393f - 3.563e-7f * d) * DEG2RAD;
			float E = m + (180.0f / PI) * e * sin(m * DEG2RAD) * (1.0f + e * cos(m * DEG2RAD));
			
			float x = cos(E * DEG2RAD) - e;
			float y = sin(E * DEG2RAD) * sqrt(1.0f - e * e);
			
			float v = atan2(y,x) * RAD2DEG;
			float lon = (v + w) * DEG2RAD;
			
			float dec = asin(sin(oblecl) * sin(lon));
			float ra = atan2(cos(oblecl) * sin(lon),cos(lon)) / 15.0f * RAD2DEG;
			
			float gmst0 = l / 15.0f + 12.0f;
			float sidtime = gmst0 + ut + longitude / 15.0f;
			float ha = (sidtime - ra) * 15.0f * DEG2RAD;
			
			altitude = asin(sin(latitude * DEG2RAD) * sin(dec) + cos(latitude * DEG2RAD) * cos(dec) * cos(ha));
			azimuth = atan2(sin(ha),cos(ha) * sin(latitude * DEG2RAD) - tan(dec) * cos(latitude * DEG2RAD)) + PI;
			
			return normalize(Vec3(cos(azimuth) * cos(altitude),sin(azimuth) * cos(altitude),sin(altitude)));
		}
		
		Mat4 getSunTransform() {
			return setTo(Vec3_zero,-getSunDirection(),vec3(0.0f,0.0f,1.0f));
		}
	};
	
} /* namespace Unigine */

#endif /* __UNIGINE_TIME_H__ */
