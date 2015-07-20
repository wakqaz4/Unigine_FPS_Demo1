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


#ifndef __COMBINER_NULL_H__
#define __COMBINER_NULL_H__

#include <unigine.h>
#include <scripts/character/combiner.h>

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * class CombinerNull
	 *
	\******************************************************************************/
	
	/*
	 */
	class CombinerNull : Combiner {
		
		//
		CombinerNull(ObjectMeshSkinned mesh_) {
			combiner_init(mesh_);
		}
		
		//
		void setup() { }
		
	};
	
}

#endif
