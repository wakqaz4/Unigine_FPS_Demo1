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


#ifndef __FACTORY_H__
#define __FACTORY_H__

#include <unigine.h>

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * class Factory
	 *
	\******************************************************************************/
	
	/*
	 */
	class Factory {
		
		string types[];
		
		//
		void addClass(string type_,string class_) {
			types.append(type_,class_);
		}
		
		//
		string getClass(string type_) {
			if(types.check(type_) == 0) return "";
			return types[type_];
		}
		
		//
		string getType(string class_) {
			return types.find(class_,"");
		}
		
		//
		void create(string type_) {
			string class_ = getClass(type_);
			if(class_ == "") throw("Factory::create(): can't find class of the type \"%s\"",type_);
			return new(class_);
		}
		
		//
		void create(string type_,void p0) {
			string class_ = getClass(type_);
			if(class_ == "") throw("Factory::create(): can`t find class of the type \"%s\"",type_);
			return new(class_,p0);
		}
		
		//
		void create(string type_,void p0,void p1) {
			string class_ = getClass(type_);
			if(class_ == "") throw("Factory::create(): can`t find class of the type \"%s\"",type_);
			return new(class_,p0,p1);
		}
		
		//
		void create(string type_,void p0,void p1,void p2) {
			string class_ = getClass(type_);
			if(class_ == "") throw("Factory::create(): can`t find class of the type \"%s\"",type_);
			return new(class_,p0,p1,p2);
		}
		
		//
		void create(string type_,void p0,void p1,void p2,void p3) {
			string class_ = getClass(type_);
			if(class_ == "") throw("Factory::create(): can`t find class of the type \"%s\"",type_);
			return new(class_,p0,p1,p2,p3);
		}
		
	};
}

#endif
