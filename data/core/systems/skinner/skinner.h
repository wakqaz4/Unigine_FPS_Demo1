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


#ifndef __UNIGINE_SKINNER_H__
#define __UNIGINE_SKINNER_H__

#include <core/systems/schemer/schemer.h>
#include <core/systems/skinner/skinner_mesh.h>

/*
 */
namespace Unigine::Skinner {
	
	/**************************************************************************\
	*
	* Skinner
	*
	\**************************************************************************/
	
	/*
	 */
	class Skinner {
		
		using Unigine::Schemer;
		
		Schemer schemer;			// schemer
		SchemerScript scripts[];	// scripts
		
		/*
		 */
		Skinner() {
			
			// create schemer
			schemer = new Schemer();
			
			// load skinner blocks
			if(schemer.loadBlocks("core/systems/skinner/blocks/skinner.blocks") == 0) {
				throw("Unigine::Skinner::Skinner::Skinner(): can't load skinner blocks\n");
			}
		}
		
		~Skinner() {
			
			delete schemer;
			scripts.delete();
		}
		
		/*
		 */
		Schemer getSchemer() {
			return schemer;
		}
		
		/**********************************************************************\
		*
		* Load script
		*
		\**********************************************************************/
		
		/*
		 */
		SchemerScript loadScript(string name) {
			SchemerScript script = scripts.check(name,NULL);
			if(script == NULL) {
				script = new SchemerScript(schemer);
				if(script.load(name) == 0) {
					log.error("Unigine::Skinner::Skinner::loadScript(): can't load \"%s\" script\n",name);
					delete script;
					return NULL;
				}
				scripts.append(name,script);
			}
			return script;
		}
	};
	
} /* namespace Unigine::Skinner */

#endif /* __UNIGINE_SKINNER_H__ */
