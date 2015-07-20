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


#ifndef __UNIGINE_TUPLE_H__
#define __UNIGINE_TUPLE_H__

/*
 */
namespace Unigine {
	
	/*
	 */
	class Pair {
		
		int first;
		int second;
		
		Pair(int f = 0,int s = 0) {
			first = f;
			second = s;
		}
		
		void setFirst(int f) {
			first = f;
		}
		int getFirst() {
			return first;
		}
		
		void setSecond(int s) {
			second = s;
		}
		int getSecond() {
			return second;
		}
	};
	
	/*
	 */
	class Triple {
		
		int first;
		int second;
		int third;
		
		Triple(int f = 0,int s = 0,int t = 0) {
			first = f;
			second = s;
			third = t;
		}
		
		void setFirst(int f) {
			first = f;
		}
		int getFirst() {
			return first;
		}
		
		void setSecond(int s) {
			second = s;
		}
		int getSecond() {
			return second;
		}
		
		void setThird(int t) {
			third = t;
		}
		int getThird() {
			return third;
		}
	};
	
	/*
	 */
	class Quadruple {
		
		int first;
		int second;
		int third;
		int fourth;
		
		Quadruple(int f = 0,int s = 0,int t = 0,int fo = 0) {
			first = f;
			second = s;
			third = t;
			fourth = fo;
		}
		
		void setFirst(int f) {
			first = f;
		}
		int getFirst() {
			return first;
		}
		
		void setSecond(int s) {
			second = s;
		}
		int getSecond() {
			return second;
		}
		
		void setThird(int t) {
			third = t;
		}
		int getThird() {
			return third;
		}
		
		void setFourth(int f) {
			fourth = f;
		}
		int getFourth() {
			return fourth;
		}
	};
	
} /* namespace Unigine */

#endif /* __UNIGINE_TUPLE_H__ */
