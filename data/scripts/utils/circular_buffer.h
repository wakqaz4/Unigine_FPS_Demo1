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


#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <unigine.h>

class CircularBuffer {
	
	private:
		
		int max_size = 100;
		
		int buffer[0];
		int first = 0;
		int last = 0;
		int cur_size = 0;
		
	public:
		
		CircularBuffer() {
			buffer.resize(max_size);
		}
		
		CircularBuffer(int max_size_) {
			max_size = max_size_;
			buffer.resize(max_size);
		}
		
		~CircularBuffer() {
			buffer.delete();
		}
		
		void clear() {
			cur_size = 0;
			first = 0;
			last = 0;
		}
		
		int getSize() {
			return cur_size;
		}
		
		int getMaxSize() {
			return max_size;
		}
		
		int getPosition(int index) {
			return (first + index) % max_size;
		}
		
		void increment(int &counter) {
			if(++counter == max_size) counter = 0;
		}
		
		void decrement(int &counter) {
			if(--counter < 0) counter = max_size - 1;
		}
		
		int front() {
			return buffer[first];
		}
		
		int back() {
			
			if(last == 0) return buffer[max_size - 1];
			
			int back_pos = last;
			decrement(back_pos);
			return buffer[back_pos];
		}
		
		int full() {
			return getSize() == max_size;
		}
		
		int get(int index) {
			
			assert(index < buffer.size());
			return buffer[getPosition(index)];
		}
		
		// returns "end" element (one behind back),
		// which can be used to be rewritten to avoid memory reallocation
		int end() {
			
			if(max_size == 0) return NULL;
			if(buffer.check(last) == 0) return NULL;
			return buffer[last];
		}
		
		// mark end element as used
		void useEnd() {
			
			if(end() == NULL) return;
			increment(last);
			if(full()) first = last;
			else cur_size++;
		}
		
		void append(int element) {
			
			if(max_size == 0) return;
			
			if(buffer.check(last) == 0) buffer.append(last,element);
			else buffer[last] = element;
			increment(last);
			
			if(full()) first = last;
			else cur_size++;
		}
		
		// removes first element of buffer
		void remove() {
			increment(first);
			cur_size--;
		}
};

#endif /* __CIRCULAR_BUFFER_H__ */
