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


#ifndef __COMBINER_INVERT_H__
#define __COMBINER_INVERT_H__

#include <unigine.h>
#include <scripts/character/combiner.h>

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * class CombinerInvert
	 *
	\******************************************************************************/
	
	/*
	 */
	class CombinerInvert : CombinerComplex {
		
		//
		CombinerInvert(ObjectMeshSkinned mesh_,Combiner op0_) {
			combiner_init(mesh_);
			operands.append(op0_);
		}
		
		//
		CombinerInvert(ObjectMeshSkinned mesh_) {
			combiner_init(mesh_);
		}
		
		//
		void setup() {
			if(isActive() == 0) return;
			if(operands.size() < 1) return;
			CombinerComplex::setup();
			mesh.inverseLayer(layer,operands[0].layer);
		}
		
	};
	
}

#endif
