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


#ifndef __ANIMATION_TREE_H__
#define __ANIMATION_TREE_H__

#include <unigine.h>
#include <scripts/character/combiners.h>
#include <scripts/common/parameter.h>

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * pubilc
	 *
	\******************************************************************************/
	
	/* initialize animation tree
	 */
	void initAnimationTree() {
		
		initAnimationInputs();
		initAnimationCombiners();
		
	}
	
	/******************************************************************************\
	 *
	 * class AnimationTree
	 *
	\******************************************************************************/
	
	/*
	 */
	class AnimationTree {
		
		ObjectMeshSkinned mesh;
		Combiner root;
		ParameterManager parameters;
		
		//
		AnimationTree(ObjectMeshSkinned mesh_) {
			
			mesh = mesh_;
			
			parameters = new ParameterManager();
		}
		
		~AnimationTree() {
			delete root;
			delete parameters;
			mesh = NULL;
		}
		
		//
		void load(Xml xml) {
			
			Xml child = xml.getChild("combiner");
			if(child == NULL) throw("AnimationTree::load(): root combiner not found.");
			root = combiners_factory.create(child.getArg("type"),mesh);
			root.load(child,parameters);
			
		}
		
		//
		void save(Xml xml) {
			
			Xml child = xml.addChild("combiner");
			child.setArg("type",combiners_factory.getType(typeof(root)));
			root.save(child);
			
		}
		
		//
		void load_from_file(string file_name) {
			
			Xml xml = new Xml("animation_graph");
			xml.load(file_name);
			load(xml);
			delete xml;
			
		}
		
		//
		void save_to_file(string file_name) {
			
			Xml xml = new Xml("animation_graph");
			save(xml);
			xml.save(file_name);
			delete xml;
			
		}
		
		//
		void setup() {
			
			engine.profiler.begin("anim setup",vec4_one);
			
			if(root == NULL) return;
			
			root.setup();
			mesh.copyLayer(0,root.layer);
			engine.profiler.end();
		}
		
		//
		void update(float dt) {
			engine.profiler.begin("anim update",vec4_one);
			if(root == NULL) return;
			num_active_combiners = 0;
			combiners_time = 0.0f;
			root.setActive(1);
			root.update(dt);
			engine.profiler.end();
		}
		
		//
		ParameterManager getExternalParameterManager() {
			return parameters;
		}
		
	};
	
}

#endif
