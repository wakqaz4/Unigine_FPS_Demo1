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


#ifndef __RANDOM_H__
#define __RANDOM_H__

class Random {
	
	private:
		
		enum {
			A = 1103515245,
			C = 12345,
			MAX_RANDOM = 2147483647,
		};
		
		int seed;
		
		int get() {
			seed = int((long(seed) * A + C) % MAX_RANDOM);
			return seed;
		}
		
	public:
		
		Random() {
			setSeed(0);
		}
		
		Random(int s) {
			setSeed(s);
		}
		
		~Random() {
			
		}
		
		void setSeed(int s) {
			seed = s;
		}
		
		int getSeed() {
			return seed;
		}
		
		/*
		 */
		int getInt(int from,int to) {
			
			int range = to - from;
			if(range <= 1) return from;
			
			int rand = get();
			
			if(range < 0xffff) {
				rand >>= 16;
				int rand_max = ((MAX_RANDOM >> 16) / range) * range;
				while(rand > rand_max) rand = get() >> 16;
			} else {
				int rand_max = (MAX_RANDOM / range) * range;
				while(rand > rand_max) rand = get();
			}
			
			return from + rand % range;
		}
		
		/*
		 */
		float getFloat(float from,float to) {
			return from + float(get()) / MAX_RANDOM * (to - from);
		}
		
		/*
		 */
		double getDouble(double from,double to) {
			return from + double(get()) / MAX_RANDOM * (to - from);
		}
};

#endif /* __RANDOM_H__ */
