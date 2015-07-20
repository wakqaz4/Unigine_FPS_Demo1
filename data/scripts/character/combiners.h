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


#ifndef __COMBINERS_H__
#define __COMBINERS_H__

#include <unigine.h>
#include <scripts/common/factory.h>
#include <scripts/character/combiner.h>

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * variables
	 *
	\******************************************************************************/
	
	Factory combiners_factory;
	
	/******************************************************************************\
	 *
	 * public
	 *
	\******************************************************************************/
	
	/* registration of the combiner types
	 */
	void initAnimationCombiners() {
		
		combiners_factory = new Factory();
		combiners_factory.addClass("blend","CombinerBlend");
		combiners_factory.addClass("add","CombinerAdd");
		combiners_factory.addClass("sub","CombinerSub");
		combiners_factory.addClass("invert","CombinerInvert");
		combiners_factory.addClass("identity","CombinerIdentity");
		combiners_factory.addClass("animation","CombinerAnimation");
		combiners_factory.addClass("switch","CombinerSwitch");
		combiners_factory.addClass("grid","CombinerGrid");
		combiners_factory.addClass("line","CombinerLine");
		combiners_factory.addClass("null","CombinerNull");
		combiners_factory.addClass("substitute","CombinerSubstitute");
		
	}
	
	/* create combiner by the type
	 */
	Combiner createCombiner(string type,ObjectMeshSkinned mesh) {
		return combiners_factory.create(type,mesh);
	}
	
	/* load combiner from an XML node
	 */
	Combiner loadCombiner(Xml xml,ObjectMeshSkinned mesh,ParameterManager parameters = NULL) {
		Combiner combiner = createCombiner(xml.getArg("type"),mesh);
		combiner.load(xml,parameters);
		return combiner;
	}
	
	/* save combiner to an XML node
	 */
	void saveCombiner(Xml xml,Combiner combiner) {
		xml.setArg("type",combiners_factory.getType(typeof(combiner)));
		combiner.save(xml);
	}
	
}

/**********************************************************************************\
 *
 * combiners include
 *
\**********************************************************************************/

#include <combiner_animation.h>
#include <combiner_add.h>
#include <combiner_sub.h>
#include <combiner_blend.h>
#include <combiner_invert.h>
#include <combiner_identity.h>
#include <combiner_switch.h>
#include <combiner_line.h>
#include <combiner_grid.h>
#include <combiner_null.h>
#include <combiner_substitute.h>

#endif
