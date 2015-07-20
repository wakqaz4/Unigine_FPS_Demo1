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


#ifndef __ANIMATION_INPUTS_H__
#define __ANIMATION_INPUTS_H__

#include <unigine.h>
#include <scripts/common/factory.h>
#include <scripts/common/parameter.h>

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * class Input
	 *
	\******************************************************************************/
	
	/*
	 */
	class Input {
		
		//
		Input() { }
		
		//
		float getValue() { return 0.0f; }
		
		//
		void save(Xml xml) { }
		
		//
		void load(Xml xml,ParameterManager parameters = NULL) { }
		
	};
	
	/******************************************************************************\
	 *
	 * class InputConst
	 *
	\******************************************************************************/
	
	/*
	 */
	class InputConst : Input {
		
		float value;
		
		//
		InputConst() { }
		
		//
		InputConst(float value_ = 0.0f) {
			value = value_;
		}
		
		//
		float getValue() {
			return value;
		}
		
		//
		void save(Xml xml) {
			xml.setFloatData(value);
		}
		
		//
		void load(Xml xml,ParameterManager parameters = NULL) {
			value = xml.getFloatData();
		}
		
	};
	
	/******************************************************************************\
	 *
	 * class InputFunction
	 *
	\******************************************************************************/
	
	/*
	 */
	class InputFunction : Input {
		
		string getter;
		
		float scale = 1.0f;
		float shift = 0.0f;
		float min_value = -INFINITY;
		float max_value = +INFINITY;
		float smooth = 0.0f;
		float value;
		
		//
		void setGetter(string getter_) {
			getter = getter_;
		}
		
		//
		string getGetter() {
			return getter;
		}
		
		//
		InputFunction() { }
		
		//
		InputFunction(string getter_) {
			setGetter(getter_);
		}
		
		//
		void getValue() {
			
			if(getter == "") return 0.0f;
			
			float new_value = clamp(call(getter) * scale + shift,min_value,max_value);
			
			if(smooth > EPSILON) {
				float delta = new_value - value;
				float dt = engine.game.getIFps();
				value += sign(delta) * min(abs(delta),dt * smooth);
			} else {
				value = new_value;
			}
			
			return value;
		}
		
		//
		void save(Xml xml) {
			if(scale != 1.0f) xml.setFloatArg("scale",scale);
			if(shift != 0.0f) xml.setFloatArg("shift",shift);
			if(smooth != 0.0f) xml.setFloatArg("smooth",smooth);
			if(min_value != -INFINITY) xml.setFloatArg("min",min_value);
			if(max_value != +INFINITY) xml.setFloatArg("max",max_value);
			xml.setData(getter);
		}
		
		//
		void load(Xml xml,ParameterManager parameters = NULL) {
			
			if(xml.isArg("scale")) {
				scale = xml.getFloatArg("scale");
			} else {
				scale = 1.0f;
			}
			
			if(xml.isArg("shift")) {
				shift = xml.getFloatArg("shift");
			} else {
				shift = 0.0f;
			}
			
			if(xml.isArg("smooth")) {
				smooth = xml.getFloatArg("smooth");
			} else {
				smooth = 0.0f;
			}
			
			if(xml.isArg("min")) {
				min_value = xml.getFloatArg("min");
			} else {
				min_value = -INFINITY;
			}
			
			if(xml.isArg("max")) {
				max_value = xml.getFloatArg("max");
			} else {
				max_value = +INFINITY;
			}
			getter = xml.getData();
		}
		
	};
	
	/******************************************************************************\
	 *
	 * class InputFunction
	 *
	\******************************************************************************/
	
	/*
	 */
	class InputExternal : Input {
		
		Parameter parameter;
		
		string name;
		
		float scale = 1.0f;
		float shift = 0.0f;
		float min_value = -INFINITY;
		float max_value = +INFINITY;
		float smooth = 0.0f;
		float value;
		
		//
		InputExternal() { }
		
		//
		InputExternal(Parameter parameter_) {
			parameter = parameter_;
		}
		
		//
		void getValue() {
			
			if(parameter == NULL) return 0.0f;
			
			float new_value = clamp(parameter.getValue() * scale + shift,min_value,max_value);
			
			if(smooth > EPSILON) {
				float delta = new_value - value;
				float dt = engine.game.getIFps();
				value += sign(delta) * min(abs(delta),dt * smooth);
			} else {
				value = new_value;
			}
			
			return value;
		}
		
		//
		void save(Xml xml) {
			if(scale != 1.0f) xml.setFloatArg("scale",scale);
			if(shift != 0.0f) xml.setFloatArg("shift",shift);
			if(smooth != 0.0f) xml.setFloatArg("smooth",smooth);
			if(min_value != -INFINITY) xml.setFloatArg("min",min_value);
			if(max_value != +INFINITY) xml.setFloatArg("max",max_value);
			xml.setData(name);
		}
		
		//
		void load(Xml xml,ParameterManager parameters = NULL) {
			
			if(xml.isArg("scale")) {
				scale = xml.getFloatArg("scale");
			} else {
				scale = 1.0f;
			}
			
			if(xml.isArg("shift")) {
				shift = xml.getFloatArg("shift");
			} else {
				shift = 0.0f;
			}
			
			if(xml.isArg("smooth")) {
				smooth = xml.getFloatArg("smooth");
			} else {
				smooth = 0.0f;
			}
			
			if(xml.isArg("min")) {
				min_value = xml.getFloatArg("min");
			} else {
				min_value = -INFINITY;
			}
			
			if(xml.isArg("max")) {
				max_value = xml.getFloatArg("max");
			} else {
				max_value = +INFINITY;
			}
			
			name = xml.getData();
			parameter = parameters.getItem(name);
			log.message("extern: %s (%s)\n",name,typeinfo(parameter));
		}
		
	};
	
	/******************************************************************************\
	 *
	 * variables
	 *
	\******************************************************************************/
	
	Factory inputs_factory;
	
	/******************************************************************************\
	 *
	 * public
	 *
	\******************************************************************************/
	
	/* registration of the animation input types
	 */
	void initAnimationInputs() {
		
		inputs_factory = new Factory();
		inputs_factory.addClass("const","InputConst");
		inputs_factory.addClass("function","InputFunction");
		inputs_factory.addClass("external","InputExternal");
		
	}
	
	/******************************************************************************\
	 *
	 * class InputManager
	 *
	\******************************************************************************/
	
	/*
	 */
	class InputManager {
		
		string inputs[];
		
		//
		void clear() {
			inputs.clear();
		}
		
		//
		Input getInput(string id) {
			return inputs.check(id,NULL);
		}
		
		//
		Input addInput(string id,Input input) {
			inputs.append(id,input);
		}
		
		//
		void save(Xml xml) {
			if(inputs.size() == 0) return;
			Xml parent = xml.addChild("inputs");
			foreachkey(string name; inputs) {
				Xml child = parent.addChild("input");
				Input input = inputs[name];
				child.setArg("name",name);
				child.setArg("type",inputs_factory.getType(typeof(input)));
				input.save(child);
			}
			
		}
		
		//
		void load(Xml xml,ParameterManager parameters = NULL) {
			if(xml.isChild("inputs") == 0) return;
			Xml parent = xml.getChild("inputs");
			forloop(int i = 0; parent.getNumChilds()) {
				Xml child = parent.getChild(i);
				if(child.getName() != "input") continue;
				
				Input input = inputs_factory.create(child.getArg("type"));
				input.load(child,parameters);
				inputs.append(child.getArg("name"),input);
			}
		}
		
	};
	
}

#endif
