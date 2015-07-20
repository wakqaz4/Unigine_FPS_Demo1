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


#ifndef __UNIGINE_SCHEMER_H__
#define __UNIGINE_SCHEMER_H__

#include <core/systems/schemer/schemer_node.h>
#include <core/systems/schemer/schemer_joint.h>
#include <core/systems/schemer/schemer_anchor.h>
#include <core/systems/schemer/schemer_blocks.h>
#include <core/systems/schemer/schemer_script.h>

/*
 */
namespace Unigine::Schemer {
	
	/**************************************************************************\
	*
	* Schemer
	*
	\**************************************************************************/
	
	/*
	 */
	class Schemer {
		
		SchemerBlock blocks[0];		// blocks
		
		/*
		 */
		Schemer() {
			
			// load schemer blocks
			if(loadBlocks("core/systems/schemer/blocks/schemer.blocks") == 0) {
				throw("Unigine::Schemer::Schemer::Schemer(): can't load schemer blocks\n");
			}
		}
		
		~Schemer() {
			
			blocks.delete();
		}
		
		/*
		 */
		int getNumBlocks() {
			return blocks.size();
		}
		
		SchemerBlock getBlock(int num) {
			return blocks[num];
		}
		
		/*
		 */
		int addBlock(SchemerBlock block) {
			if(findBlock(block.getType()) != NULL) {
				log.error("Unigine::Schemer::Schemer::addBlock(): \"%s\" block is already defined\n",block.getType());
				return 0;
			}
			blocks.append(block);
			return 1;
		}
		
		SchemerBlock findBlock(string type) {
			foreach(SchemerBlock block; blocks) {
				if(block.getType() == type) return block;
			}
			return NULL;
		}
		
		/**********************************************************************\
		*
		* Load blocks
		*
		\**********************************************************************/
		
		/*
		 */
		int loadBlocks(string name) {
			
			Xml xml = new Xml();
			
			// load blocks
			if(xml.load(name) == 0) {
				log.error("Unigine::Schemer::Schemer::loadBlocks(): can't open \"%s\" file\n",name);
				delete xml;
				return 0;
			}
			
			// check blocks version
			if(xml.getName() != "blocks" || xml.isArg("version") == 0 || strlen(xml.getArg("version")) == 0) {
				log.error("Unigine::Schemer::Schemer::loadBlocks(): bad blocks file \"%s\"\n",name);
				delete xml;
				return 0;
			}
			
			// load blocks
			forloop(int i = 0; xml.getNumChilds()) {
				Xml x = xml.getChild(i);
				
				// block
				if(x.getName() == "block") {
					SchemerBlock block = new SchemerBlock();
					if(block.load(x) == 0) {
						log.error("Unigine::Schemer::Schemer::loadBlocks(): can't load block from \"%s\" file\n",name);
						delete block;
						delete xml;
						return 0;
					}
					if(addBlock(block) == 0) {
						log.error("Unigine::Schemer::Schemer::loadBlocks(): can't add block\n");
						delete block;
						delete xml;
						return 0;
					}
				}
				
				else {
					log.error("Unigine::Schemer::Schemer::loadBlocks(): unknown node \"%s\" in \"%s\" file\n",x.getName(),name);
					continue;
				}
			}
			
			// print unused data
			xml.printUnusedData(name);
			delete xml;
			
			return 1;
		}
	};
	
} /* namespace Unigine::Schemer */

#endif /* __UNIGINE_SCHEMER_H__ */
