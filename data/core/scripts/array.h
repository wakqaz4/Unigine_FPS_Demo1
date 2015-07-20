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


#ifndef __UNIGINE_ARRAY_H__
#define __UNIGINE_ARRAY_H__

/*
 */
namespace Unigine {
	
	/*
	 */
	class Vector {
		
		int data[0];
		
		Vector() {
			
		}
		Vector(int d[]) {
			data.copy(d);
		}
		Vector(int value,int size) {
			data.resize(size);
			forloop(int i = 0; size) {
				data[i] = value;
			}
		}
		~Vector() {
			// there should be data.delete()
			// but it would break existing code
		}
		
		void set(int index,int value) {
			data[index] = value;
		}
		int get(int index) {
			return data[index];
		}
		
		void __set__(Vector v,int index,int value) {
			v.data[index] = value;
		}
		int __get__(Vector v,int index) {
			return v.data[index];
		}
		
		int size() {
			return data.size();
		}
		void resize(int size) {
			data.resize(size);
		}
		void clear() {
			data.clear();
		}
		
		void append(int value) {
			data.append(value);
		}
		void append(int index,int value) {
			data.append(index,value);
		}
		void append(int d[],int pos,int size) {
			data.append(d,pos,size);
		}
		
		void remove() {
			data.remove();
		}
		void remove(int pos) {
			data.remove(pos);
		}
		void remove(int pos,int size) {
			data.remove(pos,size);
		}
		
		int find(int value) {
			return data.find(value);
		}
		int find(int value,int index) {
			return data.find(value,index);
		}
		
		int check(int index) {
			return data.check(index);
		}
		int check(int index,int value) {
			return data.check(index,value);
		}
		
		int left(int value) {
			return data.left(value);
		}
		int right(int value) {
			return data.right(value);
		}
		
		int compare(int d[]) {
			return data.compare(d);
		}
		void copy(int d[]) {
			data.copy(d);
		}
		void swap(int d[]) {
			data.swap(d);
		}
		void merge(int d[]) {
			data.merge(d);
		}
		
		void sort() {
			data.sort();
		}
		void sort(int d[]) {
			data.sort(d);
		}
	};
	
	/*
	 */
	class Map {
		
		int data[];
		
		Map() {
			
		}
		Map(int d[]) {
			data.copy(d);
		}
		~Map() {
			// there should be data.delete()
			// but it would break existing code
		}
		
		void set(int index,int value) {
			data[index] = value;
		}
		int get(int index) {
			return data[index];
		}
		
		void __set__(Map m,int index,int value) {
			m.data[index] = value;
		}
		int __get__(Map m,int index) {
			return m.data[index];
		}
		
		int size() {
			return data.size();
		}
		void clear() {
			data.clear();
		}
		
		void append(int key) {
			data.append(key);
		}
		void append(int key,int value) {
			data.append(key,value);
		}
		
		void remove(int key) {
			data.remove(key);
		}
		
		int find(int value) {
			return data.find(value);
		}
		int find(int value,int key) {
			return data.find(value,key);
		}
		
		int check(int key) {
			return data.check(key);
		}
		int check(int key,int value) {
			return data.check(key,value);
		}
		
		int compare(int d[]) {
			return data.compare(d);
		}
		void copy(int d[]) {
			data.copy(d);
		}
		void swap(int d[]) {
			data.swap(d);
		}
		void merge(int d[]) {
			data.merge(d);
		}
	};
	
} /* namespace Unigine */

#endif /* __UNIGINE_ARRAY_H__ */
