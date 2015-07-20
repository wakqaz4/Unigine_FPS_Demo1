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


#ifndef __UNIGINE_SCHEMER_SCRIPT_H__
#define __UNIGINE_SCHEMER_SCRIPT_H__

/*
 */
namespace Unigine::Schemer {
	
	/*
	 */
	class SchemerScript {
		
		Schemer schemer;			// schemer
		
		SchemerNode nodes[0];		// nodes
		SchemerJoint paths[0];		// paths
		SchemerJoint links[0];		// links
		
		int entry_indices[];		// entry node indices
		int input_indices[];		// input node indices
		int output_indices[];		// output node indices
		
		string common_source;		// common source
		string init_source;			// init source
		string update_source;		// update source
		string shutdown_source;		// shutdown source
		
		Expression expression;		// expression
		
		int input_id;				// input function id
		int output_id;				// output function id
		int init_id;				// init function id
		int update_id;				// update function id
		int shutdown_id;			// shutdown function id
		
		/*
		 */
		SchemerScript(Schemer s) {
			
			schemer = s;
			
			input_id = -1;
			output_id = -1;
			init_id = -1;
			update_id = -1;
			shutdown_id = -1;
		}
		
		~SchemerScript() {
			
			clear();
		}
		
		/*
		 */
		void clear() {
			
			nodes.delete();
			paths.delete();
			links.delete();
			
			entry_indices.delete();
			input_indices.delete();
			output_indices.delete();
			
			common_source = "";
			init_source = "";
			update_source = "";
			shutdown_source = "";
			
			delete expression;
			
			input_id = -1;
			output_id = -1;
			init_id = -1;
			update_id = -1;
			shutdown_id = -1;
		}
		
		/*
		 */
		int addNode(SchemerNode node) {
			
			// node type
			string type = node.getType();
			string name = node.getName();
			
			// base node types
			if(strlen(name)) {
				if(type == "schemer.entry") {
					if(entry_indices.check(name)) {
						log.error("Unigine::Schemer::SchemerScript::addNode(): \"%s\" entry node is already defined",name);
						return 0;
					}
					entry_indices.append(name,nodes.size());
				}
				else if(type == "schemer.input") {
					if(input_indices.check(name)) {
						log.error("Unigine::Schemer::SchemerScript::addNode(): \"%s\" input node is already defined",name);
						return 0;
					}
					input_indices.append(name,nodes.size());
				}
				else if(type == "schemer.output") {
					if(output_indices.check(name)) {
						log.error("Unigine::Schemer::SchemerScript::addNode(): \"%s\" output node is already defined",name);
						return 0;
					}
					output_indices.append(name,nodes.size());
				}
			}
			
			// add node
			nodes.append(node);
			return 1;
		}
		
		/*
		 */
		int getNumNodes() {
			return nodes.size();
		}
		
		SchemerNode getNode(int num) {
			return nodes[num];
		}
		
		int findNode(SchemerNode node) {
			return nodes.find(node);
		}
		
		/*
		 */
		int addPathJoint(SchemerJoint joint) {
			
			// check output anchor
			SchemerBlock block_0 = joint.getBlock0();
			if(block_0.findOutputPath(joint.getAnchor0()) == -1) {
				log.error("Unigine::Schemer::SchemerScript::addPathJoint(): can't find \"%s\" output path in \"%s\" block\n",joint.getAnchor0(),block_0.getType());
				return 0;
			}
			
			// check input anchor
			SchemerBlock block_1 = joint.getBlock1();
			if(block_1.findInputPath(joint.getAnchor1()) == -1) {
				log.error("Unigine::Schemer::SchemerScript::addPathJoint(): can't find \"%s\" input path in \"%s\" block\n",joint.getAnchor1(),block_1.getType());
				return 0;
			}
			
			// add joint
			paths.append(joint);
			return 1;
		}
		
		/*
		 */
		int getNumPathJoints() {
			return paths.size();
		}
		
		SchemerJoint getPathJoint(int num) {
			return paths[num];
		}
		
		/*
		 */
		int addLinkJoint(SchemerJoint joint) {
			
			// check output anchor
			SchemerBlock block_0 = joint.getBlock0();
			if(block_0.findOutputLink(joint.getAnchor0()) == -1) {
				log.error("Unigine::Schemer::SchemerScript::addLinkJoint(): can't find \"%s\" output link in \"%s\" block\n",joint.getAnchor0(),block_0.getType());
				return 0;
			}
			
			// check input anchor
			SchemerBlock block_1 = joint.getBlock1();
			if(block_1.findInputLink(joint.getAnchor1()) == -1) {
				log.error("Unigine::Schemer::SchemerScript::addLinkJoint(): can't find \"%s\" input link in \"%s\" block\n",joint.getAnchor1(),block_1.getType());
				return 0;
			}
			
			// add joint
			links.append(joint);
			return 1;
		}
		
		/*
		 */
		int getNumLinkJoints() {
			return links.size();
		}
		
		SchemerJoint getLinkJoint(int num) {
			return links[num];
		}
		
		/*
		 */
		void setCommonSource(string source) {
			common_source = source;
		}
		string getCommonSource() {
			return common_source;
		}
		
		void setInitSource(string source) {
			init_source = source;
		}
		string getInitSource() {
			return init_source;
		}
		
		void setUpdateSource(string source) {
			update_source = source;
		}
		string getUpdateSource() {
			return update_source;
		}
		
		void setShutdownSource(string source) {
			shutdown_source = source;
		}
		string getShutdownSource() {
			return shutdown_source;
		}
		
		/**********************************************************************\
		*
		* Clone script
		*
		\**********************************************************************/
		
		/*
		 */
		SchemerScript clone() {
			
			// schemer
			SchemerScript script = new SchemerScript(schemer);
			
			// clone nodes
			forloop(int i = 0; nodes.size()) {
				SchemerNode n = nodes[i];
				SchemerNode node = new SchemerNode(n.getBlock(),n.getID());
				node.setName(n.getName());
				node.setPosition(n.getPositionX(),n.getPositionY());
				node.input_link_values.copy(n.input_link_values);
				node.output_link_values.copy(n.output_link_values);
				script.addNode(node);
			}
			
			// clone path joints
			forloop(int i = 0; paths.size()) {
				SchemerJoint j = paths[i];
				SchemerNode node_0 = script.nodes[findNode(j.getNode0())];
				SchemerNode node_1 = script.nodes[findNode(j.getNode1())];
				SchemerJoint joint = new SchemerJoint(node_0,node_1);
				joint.setAnchor0(j.getAnchor0());
				joint.setAnchor1(j.getAnchor1());
				script.paths.append(joint);
			}
			
			// clone link joints
			forloop(int i = 0; links.size()) {
				SchemerJoint j = links[i];
				SchemerNode node_0 = script.nodes[findNode(j.getNode0())];
				SchemerNode node_1 = script.nodes[findNode(j.getNode1())];
				SchemerJoint joint = new SchemerJoint(node_0,node_1);
				joint.setAnchor0(j.getAnchor0());
				joint.setAnchor1(j.getAnchor1());
				script.links.append(joint);
			}
			
			// copy sources
			script.setCommonSource(getCommonSource());
			script.setInitSource(getInitSource());
			script.setUpdateSource(getUpdateSource());
			script.setShutdownSource(getShutdownSource());
			
			return script;
		}
		
		/**********************************************************************\
		*
		* Load/save script
		*
		\**********************************************************************/
		
		/*
		 */
		int load(string name) {
			
			clear();
			
			Xml xml = new Xml();
			
			// load script
			if(xml.load(name) == 0) {
				log.error("Unigine::Schemer::SchemerScript::load(): can't load \"%s\" file\n",name);
				delete xml;
				return 0;
			}
			
			// check script version
			if(xml.getName() != "script" || xml.isArg("version") == 0 || strlen(xml.getArg("version")) == 0) {
				log.error("Unigine::Schemer::SchemerScript::load(): bad script file \"%s\"\n",name);
				delete xml;
				return 0;
			}
			
			// load script
			SchemerNode identifiers[];
			forloop(int i = 0; xml.getNumChilds()) {
				Xml x = xml.getChild(i);
				
				// node
				if(x.getName() == "node") {
					
					// identifier
					if(x.isArg("id") == 0) {
						log.error("Unigine::Schemer::SchemerScript::load(): can't find node identifier\n");
						delete xml;
						return 0;
					}
					int id = x.getIntArg("id");
					
					// check identifier
					if(identifiers.check(id) != 0) {
						log.error("Unigine::Schemer::SchemerScript::load(): node identifier %d is used\n",id);
						delete xml;
						return 0;
					}
					
					// node type
					if(x.isArg("type") == 0) {
						log.error("Unigine::Schemer::SchemerScript::load(): can't find node type\n");
						delete xml;
						return 0;
					}
					string type = x.getArg("type");
					
					// find block
					SchemerBlock block = schemer.findBlock(type);
					if(block == NULL) {
						log.error("Unigine::Schemer::SchemerScript::load(): can't find \"%s\" block\n",type);
						delete xml;
						return 0;
					}
					
					// load node
					SchemerNode node = new SchemerNode(block,id);
					if(node.load(x) == 0) {
						log.error("Unigine::Schemer::SchemerScript::load(): can't load node from \"%s\" file\n",name);
						delete node;
						delete xml;
						return 0;
					}
					
					// add node
					if(addNode(node) == 0) {
						log.error("Unigine::Schemer::SchemerScript::load(): can't add node\n");
						delete node;
						delete xml;
						return 0;
					}
					
					// node identifier
					identifiers.append(id,node);
				}
				
				// joint
				else if(x.getName() == "path" || x.getName() == "link") {
					
					// load joint
					SchemerJoint joint = new SchemerJoint();
					if(joint.load(x,identifiers) == 0) {
						log.error("Unigine::Schemer::SchemerScript::load(): can't load joint from \"%s\" file\n",name);
						delete joint;
						return 0;
					}
					
					// path joint
					if(x.getName() == "path") {
						if(addPathJoint(joint) == 0) {
							log.error("Unigine::Schemer::SchemerScript::load(): can't add \"%s\"-\"%s\" path joint\n",joint.getAnchor0(),joint.getAnchor1());
							delete joint;
							delete xml;
							return 0;
						}
					}
					// link joint
					else {
						if(addLinkJoint(joint) == 0) {
							log.error("Unigine::Schemer::SchemerScript::load(): can't add \"%s\"-\"%s\" link joint\n",joint.getAnchor0(),joint.getAnchor1());
							delete joint;
							delete xml;
							return 0;
						}
					}
				}
				
				else {
					log.error("Unigine::Schemer::SchemerScript::load(): unknown node \"%s\" in \"%s\" file\n",x.getName(),name);
					continue;
				}
			}
			
			// check entry indices
			if(entry_indices.size() == 0) {
				log.error("Unigine::Schemer::SchemerScript::load(): can't find entry in \"%s\" file\n",name);
				delete xml;
				return 0;
			}
			
			// print unused data
			xml.printUnusedData(name);
			delete xml;
			
			return 1;
		}
		
		/*
		 */
		int save(string name) {
			
			Xml xml = new Xml("script","version=\"1.00\"");
			
			// nodes
			forloop(int i = 0; nodes.size()) {
				SchemerNode node = nodes[i];
				SchemerBlock block = node.block;
				
				Xml x = xml.addChild("node");
				x.setIntArg("id",node.id);
				x.setArg("type",block.getType());
				
				// save node
				if(node.save(x) == 0) {
					log.error("Unigine::Schemer::SchemerScript::save(): can't save node\n");
					delete xml;
					return 0;
				}
			}
			
			// paths
			forloop(int i = 0; paths.size()) {
				SchemerJoint joint = paths[i];
				
				// save joint
				if(joint.save(xml.addChild("path")) == 0) {
					log.error("Unigine::Schemer::SchemerScript::save(): can't save joint\n");
					delete xml;
					return 0;
				}
			}
			
			// links
			forloop(int i = 0; links.size()) {
				SchemerJoint joint = links[i];
				
				// save joint
				if(joint.save(xml.addChild("link")) == 0) {
					log.error("Unigine::Schemer::SchemerScript::save(): can't save joint\n");
					delete xml;
					return 0;
				}
			}
			
			// save script
			if(xml.save(name) == 0) {
				log.error("Unigine::Schemer::SchemerScript::save(): can't save \"%s\" file\n",name);
				delete xml;
				return 0;
			}
			
			delete xml;
			return 1;
		}
		
		/**********************************************************************\
		*
		* Compile script
		*
		\**********************************************************************/
		
		/*
		 */
		string copy_links(SchemerNode node) {
			string source = "";
			foreach(SchemerJoint joint; links) {
				if(joint.getNode0() == node) {
					int i = nodes.find(joint.getNode0());
					int j = nodes.find(joint.getNode1());
					source += format("n%d::%s = ",j,joint.getAnchor1());
					source += format("n%d::%s;\n",i,joint.getAnchor0());
				}
			}
			return source;
		}
		
		/*
		 */
		int compile() {
			
			// clear expression
			delete expression;
			
			// script source
			string source = "{\n";
			
			// common source
			if(strlen(common_source)) source += common_source + "\n";
			
			/////////////////////////////////
			// node sources
			/////////////////////////////////
			
			forloop(int i = 0; nodes.size()) {
				SchemerNode node = nodes[i];
				SchemerBlock block = node.getBlock();
				
				source += format("// %s\n",block.getType());
				source += format("namespace n%d {\n",i);
				
				// input links
				if(block.input_links.size()) {
					foreach(SchemerAnchor anchor; block.input_links) {
						source += format("\tint %s;\n",anchor.getName());
					}
				}
				
				// output links
				if(block.output_links.size()) {
					foreach(SchemerAnchor anchor; block.output_links) {
						source += format("\tint %s;\n",anchor.getName());
					}
				}
				
				// common source
				if(strlen(block.getCommonSource())) source += "\t// common source\n";
				if(strlen(block.getCommonSource())) source += block.getCommonSource() + "\n";
				
				// init source
				source += "\t{\n";
				source += format("\t\tn%dii:\n",i);
				if(strlen(block.getInitSource())) source += block.getInitSource() + "\n";
				source += format("\t\tgoto n%dio;\n",i);
				source += "\t}\n";
				
				// shutdown source
				source += "\t{\n";
				source += format("\t\tn%dsi:\n",i);
				if(strlen(block.getShutdownSource())) source += block.getShutdownSource() + "\n";
				source += format("\t\tgoto n%dso;\n",i);
				source += "\t}\n";
				
				// update source
				source += "\t{\n";
				source += format("\t\tn%dui:\n",i);
				if(strlen(block.getUpdateSource())) {
					string update_source = block.getUpdateSource();
					if(block.input_paths.size() > 1) {
						forloop(int j = 0; block.input_paths.size()) {
							SchemerAnchor anchor = block.input_paths[j];
							string name = format("n%dui%d",i,j);
							update_source = re_replace(update_source,"[ \t]+(" + anchor.getName() + "):",name);
						}
					} else if(block.input_paths.size()) {
						SchemerAnchor anchor = block.input_paths[0];
						string name = format("n%dui",i);
						update_source = re_replace(update_source,"[ \t]+(" + anchor.getName() + "):",name);
					}
					if(block.output_paths.size() > 1) {
						forloop(int j = 0; block.output_paths.size()) {
							SchemerAnchor anchor = block.output_paths[j];
							string name = format("n%duo%d",i,j);
							update_source = re_replace(update_source,"goto[ \t]+(" + anchor.getName() + ");",name);
						}
					} else if(block.output_paths.size()) {
						SchemerAnchor anchor = block.output_paths[0];
						string name = format("n%duo;",i);
						update_source = re_replace(update_source,"goto[ \t]+(" + anchor.getName() + ");",name);
					}
					source += update_source + "\n";
				}
				source += format("\t\tgoto n%duo;\n",i);
				source += "\t}\n";
				
				source += format("}\n");
			}
			
			/////////////////////////////////
			// input parameters
			/////////////////////////////////
			
			// input
			source += "// input\n";
			source += "void input(int id,int value) {\n";
			source += "\tswitch(id) {\n";
			foreach(int i; input_indices) {
				source += format("\t\tcase %d:\n",i);
				source += format("\t\t\tn%d::value = value;\n",i);
				source += copy_links(nodes[i]);
				source += format("\t\t\nreturn;\n");
			}
			source += "\t}\n";
			source += "\tthrow(\"Unigine::Schemer::SchemerScript::input(): unknown id \\\"%d\\\"\\n\",id);\n";
			source += "}\n";
			
			/////////////////////////////////
			// output parameters
			/////////////////////////////////
			
			// output
			source += "// output\n";
			source += "int output(int id) {\n";
			source += "\tswitch(id) {\n";
			foreach(int i; output_indices) {
				source += format("\t\tcase %d: return n%d::value;\n",i,i);
			}
			source += "\t}\n";
			source += "\tthrow(\"Unigine::Schemer::SchemerScript::output(): unknown id \\\"%d\\\"\\n\",id);\n";
			source += "}\n";
			
			/////////////////////////////////
			// init
			/////////////////////////////////
			
			// init order
			int init_order[0];
			forloop(int i = 0; nodes.size()) {
				SchemerBlock block = nodes[i].getBlock();
				if(block.getNumInputPaths() != 0) continue;
				init_order.append(i);
			}
			forloop(int i = 0; nodes.size()) {
				SchemerBlock block = nodes[i].getBlock();
				if(block.getNumInputPaths() == 0) continue;
				init_order.append(i);
			}
			
			// paths
			source += "// init\n";
			forloop(int i = 0; nodes.size()) {
				SchemerNode node = nodes[init_order[i]];
				SchemerBlock block = node.getBlock();
				
				// output label
				source += format("n%dio:\n",init_order[i]);
				
				// copy links
				if(block.getNumInputPaths() == 0) {
					source += copy_links(node);
				}
				
				// next node
				if(i + 1 == nodes.size()) source += "goto init_output;\n";
				else source += format("goto n%dii;\n",init_order[i + 1]);
			}
			
			// init
			source += "void init() {\n";
			forloop(int i = 0; nodes.size()) {
				SchemerNode node = nodes[i];
				SchemerBlock block = node.getBlock();
				foreach(SchemerAnchor anchor; block.input_links) {
					string name = anchor.getName();
					string value = node.getInputLinkValue(name);
					if(strlen(value)) source += format("\tn%d::%s = %s;\n",i,name,value);
				}
				foreach(SchemerAnchor anchor; block.output_links) {
					string name = anchor.getName();
					string value = node.getOutputLinkValue(name);
					if(strlen(value)) source += format("\tn%d::%s = %s;\n",i,name,value);
				}
			}
			if(strlen(init_source)) source += init_source + "\n";
			source += format("\tgoto n%dii;\n",init_order[0]);
			source += "\tinit_output:\n";
			source += "}\n";
			
			/////////////////////////////////
			// update
			/////////////////////////////////
			
			// paths
			source += "// update\n";
			forloop(int i = 0; nodes.size()) {
				SchemerNode node = nodes[i];
				SchemerBlock block = node.block;
				
				// output label
				source += format("n%duo:\n",i);
				
				// multiple output paths
				if(block.output_paths.size() > 1) {
					
					source += "goto update_output;\n";
					
					// output paths
					forloop(int j = 0; block.output_paths.size()) {
						SchemerAnchor anchor = block.output_paths[j];
						
						// output label
						source += format("n%duo%d:\n",i,j);
						
						// copy links
						source += copy_links(node);
						
						// next node
						forloop(int k = 0; paths.size()) {
							SchemerJoint joint = paths[k];
							if(joint.getNode0() == node && joint.getAnchor0() == anchor.getName()) {
								int l = nodes.find(joint.getNode1());
								SchemerBlock block = joint.getBlock1();
								if(block.input_paths.size() > 1) {
									int m = block.findInputPath(joint.getAnchor1());
									source += format("goto n%dui%d;\n",l,m);
								} else {
									source += format("goto n%dui;\n",l);
								}
							}
						}
						source += "goto update_output;\n";
					}
				}
				// single output path
				else {
					
					// copy links
					source += copy_links(node);
					
					// next node
					forloop(int j = 0; paths.size()) {
						SchemerJoint joint = paths[j];
						if(joint.getNode0() == node) {
							int k = nodes.find(joint.getNode1());
							SchemerBlock block = joint.getBlock1();
							if(block.input_paths.size() > 1) {
								int l = block.findInputPath(joint.getAnchor1());
								source += format("goto n%dui%d;\n",k,l);
							} else {
								source += format("goto n%dui;\n",k);
							}
						}
					}
					source += "goto update_output;\n";
				}
			}
			
			// update
			source += "void update(int id) {\n";
			if(strlen(update_source)) source += update_source + "\n";
			source += "\tswitch(id) {\n";
			foreach(int i; entry_indices) {
				source += format("\t\tcase %d: goto n%dui;\n",i,i);
			}
			source += "\t}\n";
			source += "\tthrow(\"Unigine::Schemer::SchemerScript::update(): unknown id \\\"%d\\\"\\n\",id);\n";
			source += "\tupdate_output:\n";
			source += "}\n";
			
			/////////////////////////////////
			// shutdown
			/////////////////////////////////
			
			// paths
			source += "// shutdown\n";
			forloop(int i = 0; nodes.size()) {
				
				// output label
				source += format("n%dso:\n",i);
				
				// next node
				if(i + 1 == nodes.size()) source += "goto shutdown_output;\n";
				else source += format("goto n%dsi;\n",i + 1);
			}
			
			// shutdown
			source += "void shutdown() {\n";
			if(strlen(shutdown_source)) source += shutdown_source + "\n";
			source += "\tgoto n0si;\n";
			source += "\tshutdown_output:\n";
			source += "}\n";
			
			/////////////////////////////////
			// compilation
			/////////////////////////////////
			
			source += "}\n";
			
			// create expression
			expression = new Expression(source,1);
			if(expression.isCompiled() == NULL) {
				log.error("Unigine::Schemer::SchemerScript::compile(): can't compile script\n");
				delete expression;
				source = 0;
				return 0;
			}
			
			source = 0;
			
			// get functions
			input_id = expression.getFunction("input",2);
			output_id = expression.getFunction("output",1);
			init_id = expression.getFunction("init",0);
			update_id = expression.getFunction("update",1);
			shutdown_id = expression.getFunction("shutdown",0);
			
			return 1
		}
		
		/**********************************************************************\
		*
		* Variables
		*
		\**********************************************************************/
		
		/*
		 */
		void setVariable(string name,int value) {
			if(expression == NULL) throw("Unigine::Schemer::SchemerScript::setVariable(): expression is NULL\n");
			expression.setVariable(name,value);
		}
		
		int getVariable(string name) {
			if(expression == NULL) throw("Unigine::Schemer::SchemerScript::getVariable(): expression is NULL\n");
			return expression.getVariable(name);
		}
		
		/**********************************************************************\
		*
		* Parameters
		*
		\**********************************************************************/
		
		/*
		 */
		int getInputID(string name) {
			return input_indices.check(name,-1);
		}
		
		void setInput(int id,int value) {
			if(expression == NULL) throw("Unigine::Schemer::SchemerScript::setInput(): expression is NULL\n");
			call(input_id,id,value);
		}
		
		/*
		 */
		int getOutputID(string name) {
			return output_indices.check(name,-1);
		}
		
		int getOutput(int id) {
			if(expression == NULL) throw("Unigine::Schemer::SchemerScript::getOutput(): expression is NULL\n");
			return call(output_id,id);
		}
		
		/**********************************************************************\
		*
		* Scripts
		*
		\**********************************************************************/
		
		/*
		 */
		void init() {
			if(expression == NULL) throw("Unigine::Schemer::SchemerScript::init(): expression is NULL\n");
			call(init_id);
		}
		
		/*
		 */
		void shutdown() {
			if(expression == NULL) throw("Unigine::Schemer::SchemerScript::shutdown(): expression is NULL\n");
			call(shutdown_id);
		}
		
		/*
		 */
		int getEntryID(string name) {
			return entry_indices.check(name,-1);
		}
		
		void update(int id) {
			if(expression == NULL) throw("Unigine::Schemer::SchemerScript::update(): expression is NULL\n");
			call(update_id,id);
		}
	};
	
} /* namespace Unigine::Schemer */

#endif /* __UNIGINE_SCHEMER_SCRIPT_H__ */
