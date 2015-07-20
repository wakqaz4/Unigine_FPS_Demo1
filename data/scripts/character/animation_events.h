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


#ifndef __ANIMATION_EVENTS_H__
#define __ANIMATION_EVENTS_H__

#include <unigine.h>
#include <scripts/character/animation_inputs.h>

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * class AnimationEvent
	 *
	\******************************************************************************/
	
	/*
	 */
	class AnimationEvent {
		
		string callback;
		
		Input args[0];
		
		//
		void setCallback(string callback_) {
			callback = callback_;
		}
		
		//
		string getCallback() {
			return callback;
		}
		
		//
		AnimationEvent() {}
		
		//
		AnimationEvent(string callback_) {
			setCallback(callback_);
		}
		
		//
		void save(Xml xml) {
			xml.setArg("callback",callback);
			foreach(Input arg; args) {
				Xml child = xml.addChild("arg");
				child.setArg("type",inputs_factory.getType(typeof(arg)));
				arg.save(child);
			}
			
		}
		
		//
		void load(Xml xml,ParameterManager parameters = NULL) {
			callback = xml.getArg("callback");
			forloop(int i = 0; xml.getNumChilds()) {
				Xml child = xml.getChild(i);
				if(child.getName() != "arg") continue;
				Input arg = inputs_factory.create(child.getArg("type"));
				arg.load(child,parameters);
				args.append(arg);
			}
		}
		
		//
		void invoke() {
			if(callback == "") return;
			switch(args.size()) {
				case 0: call(callback); break;
				case 1: call(callback,args[0].getValue()); break;
				case 2: call(callback,args[0].getValue(),args[1].getValue()); break;
				case 3: call(callback,args[0].getValue(),args[1].getValue(),args[2].getValue()); break;
				case 4: call(callback,args[0].getValue(),args[1].getValue(),args[2].getValue(),args[3].getValue()); break;
				case 5: call(callback,args[0].getValue(),args[1].getValue(),args[2].getValue(),args[3].getValue(),args[4].getValue()); break;
				case 6: call(callback,args[0].getValue(),args[1].getValue(),args[2].getValue(),args[3].getValue(),args[4].getValue(),args[5].getValue()); break;
				case 7: call(callback,args[0].getValue(),args[1].getValue(),args[2].getValue(),args[3].getValue(),args[4].getValue(),args[5].getValue(),args[6].getValue()); break;
				case 8: call(callback,args[0].getValue(),args[1].getValue(),args[2].getValue(),args[3].getValue(),args[4].getValue(),args[5].getValue(),args[6].getValue(),args[7].getValue()); break;
				default: throw("AnimationEvent::invoke(): unsupported arguments count. Max is 8.");
			}
		}
		
	};
	
	/******************************************************************************\
	 *
	 * class AnimationEventManager
	 *
	\******************************************************************************/
	
	/*
	 */
	class AnimationEventManager {
		
		string events[];
		
		//
		void clear() {
			events.clear();
		}
		
		//
		AnimationEvent getAnimationEvent(string id) {
			
			if(events.check(id) == 0) return NULL;
			return events[id];
			
		}
		
		//
		void addAnimationEvent(string id,AnimationEvent event) {
			events.append(id,event);
		}
		
		//
		void save(Xml xml) {
			if(events.size() == 0) return;
			Xml parent = xml.addChild("events");
			foreachkey(string name; events) {
				
				Xml child = parent.addChild("event");
				AnimationEvent event = events[name];
				child.setArg("on",name);
				event.save(child);
			}
			
		}
		
		//
		void load(Xml xml,ParameterManager parameters = NULL) {
			
			if(xml.isChild("events") == 0) return;
			Xml parent = xml.getChild("events");
			
			forloop(int i = 0; parent.getNumChilds()) {
				
				Xml child = parent.getChild(i);
				if(child.getName() != "event") continue;
				
				AnimationEvent event = new AnimationEvent();
				event.load(child,parameters);
				events.append(child.getArg("on"),event);
			}
			
		}
		
		//
		void invoke(string id) {
			AnimationEvent event = getAnimationEvent(id);
			if(event != NULL) event.invoke();
		}
		
	};
	
}

#endif
