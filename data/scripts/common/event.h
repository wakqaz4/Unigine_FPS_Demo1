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


#ifndef __EVENT_H__
#define __EVENT_H__

/*****************************************************************************\
 *
 * class Callback
 *
\*****************************************************************************/

/* responsible for calling single function or method
 */
class Callback {
	
	// varialbles
	void instance;
	string method;
	int args[0];
	
	// constructor
	Callback(string method_,void instance_) {
		instance = instance_;
		method = method_;
	}
	
	// constructor
	Callback(string method_,void instance_,void p1) {
		instance = instance_;
		method = method_;
		args = (p1);
	}
	
	// constructor
	Callback(string method_,void instance_,void p1,void p2) {
		instance = instance_;
		method = method_;
		args = (p1, p2);
	}
	
	// constructor
	Callback(string method_,void instance_,void p1,void p2,void p3) {
		instance = instance_;
		method = method_;
		args = (p1, p2, p3);
	}
	
	// constructor
	Callback(string method_,void instance_,void p1,void p2,void p3,void p4) {
		instance = instance_;
		method = method_;
		args = (p1, p2, p3, p4);
	}
	
	// call the callback
	int invoke() {
		switch (args.size()) {
			case 0 : return (instance != NULL) ? instance.call(method) : call(method);
			case 1 : return (instance != NULL) ? instance.call(method,args[0]) : call(method,args[0]);
			case 2 : return (instance != NULL) ? instance.call(method,args[0],args[1]) : call(method,args[0],args[1]);
			case 3 : return (instance != NULL) ? instance.call(method,args[0],args[1],args[2]) : call(method,args[0],args[1],args[2]);
			case 4 : return (instance != NULL) ? instance.call(method,args[0],args[1],args[2],args[3]) : call(method,args[0],args[1],args[2],args[3]);
		}
	}
	
	// call the callback
	int invoke(void p1) {
		switch (args.size()) {
			case 0 : return (instance != NULL) ? instance.call(method,p1) : call(method,p1);
			case 1 : return (instance != NULL) ? instance.call(method,p1,args[0]) : call(method,p1,args[0]);
			case 2 : return (instance != NULL) ? instance.call(method,p1,args[0],args[1]) : call(method,p1,args[0],args[1]);
			case 3 : return (instance != NULL) ? instance.call(method,p1,args[0],args[1],args[2]) : call(method,p1,args[0],args[1],args[2]);
			case 4 : return (instance != NULL) ? instance.call(method,p1,args[0],args[1],args[2],args[3]) : call(method,p1,args[0],args[1],args[2],args[3]);
		}
	}
	
	// call the callback
	int invoke(void p1,void p2) {
		switch (args.size()) {
			case 0 : return (instance != NULL) ? instance.call(method,p1,p2) : call(method,p1,p2);
			case 1 : return (instance != NULL) ? instance.call(method,p1,p2,args[0]) : call(method,p1,p2,args[0]);
			case 2 : return (instance != NULL) ? instance.call(method,p1,p2,args[0],args[1]) : call(method,p1,p2,args[0],args[1]);
			case 3 : return (instance != NULL) ? instance.call(method,p1,p2,args[0],args[1],args[2]) : call(method,p1,p2,args[0],args[1],args[2]);
			case 4 : return (instance != NULL) ? instance.call(method,p1,p2,args[0],args[1],args[2],args[3]) : call(method,p1,p2,args[0],args[1],args[2],args[3]);
		}
	}
	
	// call the callback
	int invoke(void p1,void p2,void p3) {
		switch (args.size()) {
			case 0 : return (instance != NULL) ? instance.call(method,p1,p2,p3) : call(method,p1,p2,p3);
			case 1 : return (instance != NULL) ? instance.call(method,p1,p2,p3,args[0]) : call(method,p1,p2,p3,args[0]);
			case 2 : return (instance != NULL) ? instance.call(method,p1,p2,p3,args[0],args[1]) : call(method,p1,p2,p3,args[0],args[1]);
			case 3 : return (instance != NULL) ? instance.call(method,p1,p2,p3,args[0],args[1],args[2]) : call(method,p1,p2,p3,args[0],args[1],args[2]);
			case 4 : return (instance != NULL) ? instance.call(method,p1,p2,p3,args[0],args[1],args[2],args[3]) : call(method,p1,p2,p3,args[0],args[1],args[2],args[3]);
		}
	}
	
	// call the callback
	int invoke(void p1,void p2,void p3,void p4) {
		switch (args.size()) {
			case 0 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4) : call(method,p1,p2,p3,p4);
			case 1 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,args[0]) : call(method,p1,p2,p3,p4,args[0]);
			case 2 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,args[0],args[1]) : call(method,p1,p2,p3,p4,args[0],args[1]);
			case 3 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,args[0],args[1],args[2]) : call(method,p1,p2,p3,p4,args[0],args[1],args[2]);
			case 4 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,args[0],args[1],args[2],args[3]) : call(method,p1,p2,p3,p4,args[0],args[1],args[2],args[3]);
		}
	}
	
	// call the callback
	int invoke(void p1,void p2,void p3,void p4,void p5) {
		switch (args.size()) {
			case 0 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,p5) : call(method,p1,p2,p3,p4,p5);
			case 1 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,p5,args[0]) : call(method,p1,p2,p3,p4,p5,args[0]);
			case 2 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,p5,args[0],args[1]) : call(method,p1,p2,p3,p4,p5,args[0],args[1]);
			case 3 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,p5,args[0],args[1],args[2]) : call(method,p1,p2,p3,p4,p5,args[0],args[1],args[2]);
			case 4 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,p5,args[0],args[1],args[2],args[3]) : call(method,p1,p2,p3,p4,p5,args[0],args[1],args[2],args[3]);
		}
	}
	
	// call the callback
	int invoke(void p1,void p2,void p3,void p4,void p5,void p6) {
		switch (args.size()) {
			case 0 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,p5,p6) : call(method,p1,p2,p3,p4,p5,p6);
			case 1 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,p5,p6,args[0]) : call(method,p1,p2,p3,p4,p5,p6,args[0]);
			case 2 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,p5,p6,args[0],args[1]) : call(method,p1,p2,p3,p4,p5,p6,args[0],args[1]);
			case 3 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,p5,p6,args[0],args[1],args[2]) : call(method,p1,p2,p3,p4,p5,p6,args[0],args[1],args[2]);
			case 4 : return (instance != NULL) ? instance.call(method,p1,p2,p3,p4,p5,p6,args[0],args[1],args[2],args[3]) : call(method,p1,p2,p3,p4,p5,p6,args[0],args[1],args[2],args[3]);
		}
	}
	
	// compare this Event to another. Return true weather instances are equal
	// @TODO: use == operator instead
	int compareTo(Callback c) {
		if(instance != c.instance) return 0;
		if(method != c.method) return 0;
		return 1;
	}
};

/*****************************************************************************\
 *
 * class Event
 *
\*****************************************************************************/

/* handles group of events
 */
class Event {
	Callback callbacks[0];
	
	// add new callback
	void addCallback(Callback callback) {
		// check if such callback already added
		foreach(Callback c; callbacks) {
			if(c.compareTo(callback)) return;
		}
		
		callbacks.append(callback);
	}
	
	// add new callback
	void addCallback(string function,void instance) {
		addCallback(new Callback(function, instance));
	}
	
	// remove specified callback
	void removeCallback(Callback callback) {
		
		forloop(int i = 0; callbacks.size()) {
			if(callbacks[i].compareTo(callback)) {
				callbacks.remove(i);
				break;
			}
		}
	}
	
	// remove callback
	void removeCallback(string function,void instance) {
		removeCallback(new Callback(function, instance));
	}
	
	// invoke all the callbacks
	int invoke(){
		foreach(Callback callback; callbacks) {
			callback.invoke();
		}
	}
	
	// invoke all the callbacks
	int invoke(void p1){
		foreach(Callback callback; callbacks) {
			callback.invoke(p1);
		}
	}
	
	// invoke all the callbacks
	int invoke(void p1,void p2){
		foreach(Callback callback; callbacks) {
			callback.invoke(p1,p2);
		}
	}
	
	// invoke all the callbacks
	int invoke(void p1,void p2,void p3){
		foreach(Callback callback; callbacks) {
			callback.invoke(p1,p2,p3);
		}
	}
	
	// invoke all the callbacks
	int invoke(void p1,void p2,void p3,void p4) {
		foreach(Callback callback; callbacks) {
			callback.invoke(p1,p2,p3,p4);
		}
	}
	
	// invoke all the callbacks
	int invoke(void p1,void p2,void p3,void p4,void p5) {
		foreach(Callback callback; callbacks) {
			callback.invoke(p1,p2,p3,p4,p5);
		}
	}
	
	// invoke all the callbacks
	int invoke(void p1,void p2,void p3,void p4,void p5,void p6) {
		foreach(Callback callback; callbacks) {
			callback.invoke(p1,p2,p3,p4,p5,p6);
		}
	}
	
};

#endif // __EVENT_H__