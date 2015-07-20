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


#ifndef __COMBINER_COMPLEX_H__
#define __COMBINER_COMPLEX_H__

#include <unigine.h>
#include <scripts/character/combiner.h>

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * class CombinerComplex
	 *
	\******************************************************************************/
	
	/*
	 */
	class CombinerComplex : Combiner {
		
		Combiner operands[0];
		
		//
		CombinerComplex(ObjectMeshSkinned mesh_) {
			throw("CombinerComplex::CombinerComplex(): call of the abstract method.");
		}
		
		//
		void update(float dt) {
			if(isActive() == 0) return;
			foreach(Combiner operand; operands) {
				operand.setActive(1);
				operand.update(dt);
			}
			Combiner::update(dt);
		}
		
		//
		void setup() {
			if(isActive() == 0) return;
			foreach(Combiner operand; operands) {
				operand.setup();
			}
			mesh.clearLayer(layer);
		}
		
		//
		void save(Xml xml) {
			
			Combiner::save(xml);
			
			Xml parent = xml.addChild("flows");
			foreach(Combiner operand; operands) {
				Xml child = parent.addChild("flow");
				child = child.addChild("combiner");
				saveCombiner(child,operand);
			}
		}
		
		//
		void load(Xml xml,ParameterManager parameters = NULL) {
			
			Combiner::load(xml,parameters);
			
			operands.clear();
			Xml parent = xml.getChild("flows");
			forloop(int i = 0; parent.getNumChilds()) {
				Xml child = parent.getChild(i);
				if(child.getName() != "flow") continue;
				child = child.getChild("combiner");
				Combiner operand = loadCombiner(child,mesh,parameters);
				operands.append(operand);
			}
		}
		
		//
		void onDeactivate() {
			
			Combiner::onDeactivate();
			
			foreach(Combiner operand; operands) {
				operand.setActive(0);
			}
		}
		
	};
}

#endif
