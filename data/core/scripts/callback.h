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


#ifndef __UNIGINE_CALLBACK_H__
#define __UNIGINE_CALLBACK_H__

/*
 */
namespace Unigine {
	
	/*
	 */
	class CallbackBase {
		void run() { throw("Unigine::CallbackBase::run(): called\n"); }
		void run(int a0) { throw("Unigine::CallbackBase::run(): called\n"); }
	};
	
	/**************************************************************************\
	*
	* Function callbacks
	*
	\**************************************************************************/
	
	/*
	 */
	class Callback0 : CallbackBase {
		int func;
		Callback0(int f) { func = f; }
		void run() { call(func); }
		void run(int a0) { call(func,a0); }
	};
	
	class Callback1 : CallbackBase {
		int func,a0;
		Callback1(int f,int a0_) { func = f; a0 = a0_; }
		void run() { call(func,a0); }
		void run(int a1) { call(func,a0,a1); }
	};
	
	class Callback2 : CallbackBase {
		int func,a0,a1;
		Callback2(int f,int a0_,int a1_) { func = f; a0 = a0_; a1 = a1_; }
		void run() { call(func,a0,a1); }
		void run(int a2) { call(func,a0,a1,a2); }
	};
	
	class Callback3 : CallbackBase {
		int func,a0,a1,a2;
		Callback3(int f,int a0_,int a1_,int a2_) { func = f; a0 = a0_; a1 = a1_; a2 = a2_; }
		void run() { call(func,a0,a1,a2); }
		void run(int a3) { call(func,a0,a1,a2,a3); }
	};
	
	class Callback4 : CallbackBase {
		int func,a0,a1,a2,a3;
		Callback4(int f,int a0_,int a1_,int a2_,int a3_) { func = f; a0 = a0_; a1 = a1_; a2 = a2_; a3 = a3_; }
		void run() { call(func,a0,a1,a2,a3); }
		void run(int a4) { call(func,a0,a1,a2,a3,a4); }
	};
	
	/*
	 */
	CallbackBase MakeCallback(int func) {
		return new Callback0(func);
	}
	
	CallbackBase MakeCallback(int func,int a0) {
		return new Callback1(func,a0);
	}
	
	CallbackBase MakeCallback(int func,int a0,int a1) {
		return new Callback2(func,a0,a1);
	}
	
	CallbackBase MakeCallback(int func,int a0,int a1,int a2) {
		return new Callback3(func,a0,a1,a2);
	}
	
	CallbackBase MakeCallback(int func,int a0,int a1,int a2,int a3) {
		return new Callback4(func,a0,a1,a2,a3);
	}
	
	/**************************************************************************\
	*
	* Object function callbacks
	*
	\**************************************************************************/
	
	/*
	 */
	class CallbackObject0 : CallbackBase {
		int object,func;
		CallbackObject0(int o,int f) { object = o; func = f; }
		void run() { object.call(func); }
		void run(int a0) { object.call(func,a0); }
	};
	
	class CallbackObject1 : CallbackBase {
		int object,func,a0;
		CallbackObject1(int o,int f,int a0_) { object = o; func = f; a0 = a0_; }
		void run() { object.call(func,a0); }
		void run(int a1) { object.call(func,a0,a1); }
	};
	
	class CallbackObject2 : CallbackBase {
		int object,func,a0,a1;
		CallbackObject2(int o,int f,int a0_,int a1_) { object = o; func = f; a0 = a0_; a1 = a1_; }
		void run() { object.call(func,a0,a1); }
		void run(int a2) { object.call(func,a0,a1,a2); }
	};
	
	class CallbackObject3 : CallbackBase {
		int object,func,a0,a1,a2;
		CallbackObject3(int o,int f,int a0_,int a1_,int a2_) { object = o; func = f; a0 = a0_; a1 = a1_; a2 = a2_; }
		void run() { object.call(func,a0,a1,a2); }
		void run(int a3) { object.call(func,a0,a1,a2,a3); }
	};
	
	class CallbackObject4 : CallbackBase {
		int object,func,a0,a1,a2,a3;
		CallbackObject4(int o,int f,int a0_,int a1_,int a2_,int a3_) { object = o; func = f; a0 = a0_; a1 = a1_; a2 = a2_; a3 = a3_; }
		void run() { object.call(func,a0,a1,a2,a3); }
		void run(int a4) { object.call(func,a0,a1,a2,a3,a4); }
	};
	
	/*
	 */
	CallbackBase MakeCallbackObject(int object,int func) {
		return new CallbackObject0(object,func);
	}
	
	CallbackBase MakeCallbackObject(int object,int func,int a0) {
		return new CallbackObject1(object,func,a0);
	}
	
	CallbackBase MakeCallbackObject(int object,int func,int a0,int a1) {
		return new CallbackObject2(object,func,a0,a1);
	}
	
	CallbackBase MakeCallbackObject(int object,int func,int a0,int a1,int a2) {
		return new CallbackObject3(object,func,a0,a1,a2);
	}
	
	CallbackBase MakeCallbackObject(int object,int func,int a0,int a1,int a2,int a3) {
		return new CallbackObject4(object,func,a0,a1,a2,a3);
	}
	
} /* namespace Unigine */

#endif /* __UNIGINE_CALLBACK_H__ */
