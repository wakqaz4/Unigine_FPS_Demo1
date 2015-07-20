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


#ifndef __COMBINER_SUBSTITUTE_H__
#define __COMBINER_SUBSTITUTE_H__

#include <unigine.h>
#include <scripts/character/combiner.h>

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * class CombinerBlend
	 *
	\******************************************************************************/
	
	/*
	 */
	class CombinerSubstitute : CombinerComplex {
		
		//
		CombinerSubstitute(ObjectMeshSkinned mesh_,Combiner op0_,Combiner op1_) {
			combiner_init(mesh_);
			operands.append(op0_);
			operands.append(op1_);
		}
		
		//
		CombinerSubstitute(ObjectMeshSkinned mesh_) {
			combiner_init(mesh_);
		}
		
		//
		void setup() {
			if(isActive() == 0) return;
			CombinerComplex::setup();
			foreach(Combiner operand; operands) {
				mesh.copyLayer(layer,operand.layer);
			}
		}
		
	};
	
}

#endif
