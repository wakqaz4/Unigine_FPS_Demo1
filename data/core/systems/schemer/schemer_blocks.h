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


#ifndef __UNIGINE_SCHEMER_BLOCK_H__
#define __UNIGINE_SCHEMER_BLOCK_H__

/*
 */
namespace Unigine::Schemer {
	
	/*
	 */
	class SchemerBlock {
		
		string type;					// type name
		
		SchemerAnchor input_paths[0];	// input paths
		SchemerAnchor output_paths[0];	// output paths
		
		SchemerAnchor input_links[0];	// input links
		SchemerAnchor output_links[0];	// output links
		
		string common_source;			// common source
		string init_source;				// init source
		string update_source;			// update source
		string shutdown_source;			// shutdown source
		
		/*
		 */
		SchemerBlock() {
			
		}
		
		/*
		 */
		string getType() {
			return type;
		}
		
		/*
		 */
		int getNumInputPaths() {
			return input_paths.size();
		}
		
		SchemerAnchor getInputPath(int num) {
			return input_paths[num];
		}
		
		int findInputPath(string name) {
			forloop(int i = 0; input_paths.size()) {
				if(input_paths[i].name == name) return i;
			}
			return -1;
		}
		
		/*
		 */
		int getNumOutputPaths() {
			return output_paths.size();
		}
		
		SchemerAnchor getOutputPath(int num) {
			return output_paths[num];
		}
		
		int findOutputPath(string name) {
			forloop(int i = 0; output_paths.size()) {
				if(output_paths[i].name == name) return i;
			}
			return -1;
		}
		
		/*
		 */
		int getNumInputLinks() {
			return input_links.size();
		}
		
		SchemerAnchor getInputLink(int num) {
			return input_links[num];
		}
		
		int findInputLink(string name) {
			forloop(int i = 0; input_links.size()) {
				if(input_links[i].name == name) return i;
			}
			return -1;
		}
		
		/*
		 */
		int getNumOutputLinks() {
			return output_links.size();
		}
		
		SchemerAnchor getOutputLink(int num) {
			return output_links[num];
		}
		
		int findOutputLink(string name) {
			forloop(int i = 0; output_links.size()) {
				if(output_links[i].name == name) return i;
			}
			return -1;
		}
		
		/*
		 */
		string getCommonSource() {
			return common_source;
		}
		
		string getInitSource() {
			return init_source;
		}
		
		string getUpdateSource() {
			return update_source;
		}
		
		string getShutdownSource() {
			return shutdown_source;
		}
		
		/**********************************************************************\
		*
		* Load block
		*
		\**********************************************************************/
		
		/*
		 */
		int load(Xml xml) {
			
			// node type
			if(xml.isArg("type") == 0) {
				log.error("Unigine::Schemer::SchemerBlock::load(): can't find block type\n");
				return 0;
			}
			type = xml.getArg("type");
			
			// load block
			forloop(int i = 0; xml.getNumChilds()) {
				Xml x = xml.getChild(i);
				string name = x.getName();
				
				// anchors
				if(name == "input_path") {
					SchemerAnchor anchor = new SchemerAnchor(i,x.getData());
					if(anchor.load(x) == 0) {
						log.error("Unigine::Schemer::SchemerBlock::load(): can't load anchor\n");
						delete anchor;
						return 0;
					}
					input_paths.append(anchor);
				}
				else if(name == "output_path") {
					SchemerAnchor anchor = new SchemerAnchor(i,x.getData());
					if(anchor.load(x) == 0) {
						log.error("Unigine::Schemer::SchemerBlock::load(): can't load anchor\n");
						delete anchor;
						return 0;
					}
					output_paths.append(anchor);
				}
				else if(name == "input_link") {
					SchemerAnchor anchor = new SchemerAnchor(i,x.getData());
					if(anchor.load(x) == 0) {
						log.error("Unigine::Schemer::SchemerBlock::load(): can't load anchor\n");
						delete anchor;
						return 0;
					}
					input_links.append(anchor);
				}
				else if(name == "output_link") {
					SchemerAnchor anchor = new SchemerAnchor(i,x.getData());
					if(anchor.load(x) == 0) {
						log.error("Unigine::Schemer::SchemerBlock::load(): can't load anchor\n");
						delete anchor;
						return 0;
					}
					output_links.append(anchor);
				}
				
				// source code
				else if(name == "common") common_source = x.getData();
				else if(name == "init") init_source = x.getData();
				else if(name == "update") update_source = x.getData();
				else if(name == "shutdown") shutdown_source = x.getData();
				
				else {
					log.error("Unigine::Schemer::SchemerBlock::load(): unknown node \"%s\"\n",name);
					continue;
				}
			}
			
			return 1;
		}
	};
	
} /* namespace Unigine::Schemer */

#endif /* __UNIGINE_SCHEMER_BLOCK_H__ */
