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


#ifndef __UNIGINE_SCHEMER_JOINT_H__
#define __UNIGINE_SCHEMER_JOINT_H__

/*
 */
namespace Unigine::Schemer {
	
	/*
	 */
	class SchemerJoint {
		
		SchemerNode node_0;			// from node
		SchemerNode node_1;			// to node
		
		string anchor_0;			// from anchor
		string anchor_1;			// to anchor
		
		/*
		 */
		SchemerJoint() {
			
		}
		
		SchemerJoint(SchemerNode n0,SchemerNode n1) {
			
			node_0 = n0;
			node_1 = n1;
		}
		
		/*
		 */
		SchemerNode getNode0() {
			return node_0;
		}
		
		SchemerNode getNode1() {
			return node_1;
		}
		
		SchemerBlock getBlock0() {
			return node_0.block;
		}
		
		SchemerBlock getBlock1() {
			return node_1.block;
		}
		
		/*
		 */
		void setAnchor0(string anchor) {
			anchor_0 = anchor;
		}
		
		string getAnchor0() {
			return anchor_0;
		}
		
		/*
		 */
		void setAnchor1(string anchor) {
			anchor_1 = anchor;
		}
		
		string getAnchor1() {
			return anchor_1;
		}
		
		/**********************************************************************\
		*
		* Load/save script
		*
		\**********************************************************************/
		
		/*
		 */
		int load(Xml xml,SchemerNode identifiers[]) {
			
			// check node identifiers
			if(xml.isArg("node_0") == 0 || xml.isArg("node_1") == 0) {
				log.error("Unigine::Schemer::SchemerJoint::load(): can't find node identifiers\n");
				return 0;
			}
			
			// check anchor names
			if(xml.isArg("anchor_0") == 0 || xml.isArg("anchor_1") == 0) {
				log.error("Unigine::Schemer::Script::load(): can't find anchor names\n");
				return 0;
			}
			
			// from node identifier
			int id_0 = xml.getIntArg("node_0");
			if(identifiers.check(id_0) == 0) {
				log.error("Unigine::Schemer::Script::load(): can't find %d node\n",id_0);
				return 0;
			}
			
			// to node identifier
			int id_1 = xml.getIntArg("node_1");
			if(identifiers.check(id_1) == 0) {
				log.error("Unigine::Schemer::Script::load(): can't find %d node\n",id_1);
				return 0;
			}
			
			// nodes
			node_0 = identifiers[id_0];
			node_1 = identifiers[id_1];
			
			// anchor names
			anchor_0 = xml.getArg("anchor_0");
			anchor_1 = xml.getArg("anchor_1");
			
			return 1;
		}
		
		/*
		 */
		int save(Xml xml) {
			
			// node identifiers
			xml.setIntArg("node_0",node_0.getID());
			xml.setIntArg("node_1",node_1.getID());
			
			// anchor names
			xml.setArg("anchor_0",anchor_0);
			xml.setArg("anchor_1",anchor_1);
			
			return 1;
		}
	};
	
} /* namespace Unigine::Schemer */

#endif /* __UNIGINE_SCHEMER_JOINT_H__ */
