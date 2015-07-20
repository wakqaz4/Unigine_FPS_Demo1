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


#ifndef __UNIGINE_SCHEMER_NODE_H__
#define __UNIGINE_SCHEMER_NODE_H__

/*
 */
namespace Unigine::Schemer {
	
	/*
	 */
	class SchemerNode {
		
		SchemerBlock block;				// block
		
		int id;							// identifier
		
		string name;					// node name
		
		int position_x;					// position x
		int position_y;					// position y
		
		string input_link_values[];		// input link values
		string output_link_values[];	// output link values
		
		/*
		 */
		SchemerNode(SchemerBlock b,int i) {
			
			block = b;
			id = i;
		}
		
		/*
		 */
		string getType() {
			return block.getType();
		}
		
		SchemerBlock getBlock() {
			return block;
		}
		
		int getID() {
			return id;
		}
		
		/*
		 */
		void setName(string n) {
			name = n;
		}
		
		string getName() {
			return name;
		}
		
		/*
		 */
		void setPosition(int x,int y) {
			position_x = x;
			position_y = y;
		}
		
		int getPositionX() {
			return position_x;
		}
		
		int getPositionY() {
			return position_y;
		}
		
		/*
		 */
		void setInputLinkValue(string name,string value) {
			
			// find input link
			int id = block.findInputLink(name);
			if(id == -1) {
				log.error("Unigine::Schemer::SchemerNode::setInputLinkValue(): can't find \"%s\" input link in \"%s\" block\n",name,block.getType());
				return;
			}
			
			// set input link value
			SchemerAnchor anchor = block.getInputLink(id);
			if(anchor.getValue() != value) input_link_values.append(name,value);
			else input_link_values.remove(name);
		}
		
		string getInputLinkValue(string name) {
			
			// find input link
			int id = block.findInputLink(name);
			if(id == -1) {
				log.error("Unigine::Schemer::SchemerNode::getInputLinkValue(): can't find \"%s\" input link in \"%s\" block\n",name,block.getType());
				return;
			}
			
			// get input link value
			SchemerAnchor anchor = block.getInputLink(id);
			string value = input_link_values.check(name,"");
			if(strlen(value) == 0) value = anchor.getValue();
			return value;
		}
		
		/*
		 */
		void setOutputLinkValue(string name,string value) {
			
			// find output link
			int id = block.findOutputLink(name);
			if(id == -1) {
				log.error("Unigine::Schemer::SchemerNode::setOutputLinkValue(): can't find \"%s\" output link in \"%s\" block\n",name,block.getType());
				return;
			}
			
			// set output link value
			SchemerAnchor anchor = block.getOutputLink(id);
			if(anchor.getValue() != value) output_link_values.append(name,value);
			else output_link_values.remove(name);
		}
		
		string getOutputLinkValue(string name) {
			
			// find output link
			int id = block.findOutputLink(name);
			if(id == -1) {
				log.error("Unigine::Schemer::SchemerNode::getOutputLinkValue(): can't find \"%s\" output link in \"%s\" block\n",name,block.getType());
				return;
			}
			
			// get output link value
			SchemerAnchor anchor = block.getOutputLink(id);
			string value = output_link_values.check(name,"");
			if(strlen(value) == 0) value = anchor.getValue();
			return value;
		}
		
		/**********************************************************************\
		*
		* Load/save script
		*
		\**********************************************************************/
		
		/*
		 */
		int load(Xml xml) {
			
			// node name
			if(xml.isArg("name")) name = xml.getArg("name");
			
			// node position
			if(xml.isArg("pos_x")) position_x = xml.getIntArg("pos_x");
			if(xml.isArg("pos_y")) position_y = xml.getIntArg("pos_y");
			
			// load node
			forloop(int i = 0; xml.getNumChilds()) {
				Xml x = xml.getChild(i);
				string name = x.getName();
				
				// link values
				if(name == "input_link") setInputLinkValue(x.getArg("name"),x.getData());
				else if(name == "output_link") setOutputLinkValue(x.getArg("name"),x.getData());
				
				else {
					log.error("Unigine::Schemer::SchemerNode::load(): unknown node \"%s\"\n",name);
					continue;
				}
			}
			
			return 1;
		}
		
		/*
		 */
		int save(Xml xml) {
			
			// node name
			if(strlen(name)) xml.setArg("name",name);
			
			// node position
			if(position_x) xml.setIntArg("pos_x",position_x);
			if(position_y) xml.setIntArg("pos_y",position_y);
			
			// input link values
			foreachkey(string name; input_link_values) {
				Xml x = xml.addChild("input_link");
				x.setArg("name",name);
				x.setData(input_link_values[name]);
			}
			
			// output link values
			foreachkey(string name; output_link_values) {
				Xml x = xml.addChild("output_link");
				x.setArg("name",name);
				x.setData(output_link_values[name]);
			}
			
			return 1;
		}
	};
	
} /* namespace Unigine::Schemer */

#endif /* __UNIGINE_SCHEMER_NODE_H__ */
