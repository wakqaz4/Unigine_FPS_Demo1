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


#ifndef __COMBINER_LINE_H__
#define __COMBINER_LINE_H__

#include <unigine.h>
#include <scripts/character/combiner.h>

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * class CombinerLineKey
	 *
	\******************************************************************************/
	
	/*
	 */
	class CombinerLineKey {
		
		int pos;
		Combiner combiner;
		
		//
		void load(Xml xml,ObjectMeshSkinned mesh,ParameterManager parameters = NULL) {
			pos = xml.getIntArg("pos");
			Xml child = xml.getChild("combiner");
			combiner = loadCombiner(child,mesh,parameters);
		}
		
		//
		void save(Xml xml) {
			xml.setIntArg("pos",pos);
			Xml child = xml.addChild("combiner");
			saveCombiner(child,combiner);
		}
	};
	
	/******************************************************************************\
	 *
	 * class CombinerLine
	 *
	\******************************************************************************/
	
	/*
	 */
	class CombinerLine : Combiner {
		
		
		int min_value;
		int max_value;
		
		CombinerLineKey keys[];
		
		CombinerLineKey prev_key;
		CombinerLineKey next_key;
		float interpolation;
		
		CombinerLine(ObjectMeshSkinned mesh_) {
			combiner_init(mesh_);
		}
		
		// return last key pos, which lower or equal to curr_pos, or last key pos
		int get_prev_key_pos(float curr_pos) {
			
			int first = 1;
			int res;
			int key_pos;
			foreachkey(key_pos; keys) {
				if(first == 1) {
					res = key_pos;
					first = 0;
				} else {
					if(key_pos <= curr_pos) res = key_pos;
				}
			}
			// if no prev key, return last key pos
			return (res > curr_pos) ? key_pos : res;
		}
		
		// return first key pos, which greater then current_pos, or first key pos
		int get_next_key_pos(float curr_pos) {
			
			int first = 1;
			int first_pos;
			foreachkey(int key_pos; keys) {
				if(key_pos > curr_pos) return key_pos;
				if(first == 1) {
					first_pos = key_pos;
					first = 0;
				}
			}
			return first_pos;
		}
		
		// return interpolation coefficient
		float get_weight(int prev,int next,float curr_pos) {
			
			// prev <= next
			if(prev == next) return 0.0f;
			if(prev < next) return float(curr_pos - prev) / (next - prev);
			
			// prev > next
			float to_end = float(max_value - prev);
			float to_begin = float(next - min_value);
			
			if(abs(to_begin + to_end) < EPSILON) return 0.0f;
			
			// prev > next &&
			if(curr_pos >= prev) return (curr_pos - prev) / (to_begin + to_end);
			
			// prev > next && curr_pos < prev
			return (curr_pos - min_value + to_end) / (to_begin + to_end);
		}
		
		//
		void update(float dt) {
			
			if(isActive() == 0) return;
			
			Input input = inputs.getInput("pos");
			
			float curr_pos = float(input.getValue());
			
			if(curr_pos < min_value) curr_pos = min_value;
			if(curr_pos > max_value) curr_pos = max_value;
			
			int prev_pos = get_prev_key_pos(curr_pos);
			int next_pos = get_next_key_pos(curr_pos);
			
			interpolation = get_weight(prev_pos,next_pos,curr_pos);
			
			prev_key = keys[prev_pos];
			next_key = keys[next_pos];
			
			prev_key.combiner.setActive(1);
			next_key.combiner.setActive(1);
			
			prev_key.combiner.update(dt);
			next_key.combiner.update(dt);
			
			foreach(CombinerLineKey key; keys) {
				if(key == prev_key || key == next_key) continue;
				key.combiner.setActive(0);
			}
			
			Combiner::update(dt);
			
		}
		
		//
		void setup() {
			
			if(isActive() == 0) return;
			
			prev_key.combiner.setup();
			next_key.combiner.setup();
			mesh.lerpLayer(layer,prev_key.combiner.layer,next_key.combiner.layer,interpolation);
		}
		
		//
		void save(Xml xml) {
			Combiner::save(xml);
			Xml parent = xml.addChild("line");
			parent.setIntArg("min",min_value);
			parent.setIntArg("max",max_value);
			foreach(CombinerLineKey key; keys) {
				Xml child = parent.addChild("key");
				key.save(child);
			}
		}
		
		//
		void load(Xml xml,ParameterManager parameters = NULL) {
			
			Combiner::load(xml,parameters);
			keys.clear();
			Xml parent = xml.getChild("line");
			
			int has_min = 0;
			int has_max = 0;
			
			if(parent.isArg("min")) {
				min_value = parent.getIntArg("min");
				has_min = 1;
			}
			
			if(parent.isArg("max")) {
				max_value = parent.getIntArg("max");
				has_max = 1;
			}
			
			int first = 1;
			forloop(int i = 0; parent.getNumChilds()) {
				Xml child = parent.getChild(i);
				if(child.getName() != "key") continue;
				CombinerLineKey key = new CombinerLineKey();
				key.load(child,mesh,parameters);
				keys.append(key.pos,key);
				if(has_min == 0 && (min_value > key.pos || first)) min_value = key.pos;
				if(has_max == 0 && (max_value < key.pos || first)) max_value = key.pos;
				first = 0;
			}
		}
		
		//
		void onDeactivate() {
			Combiner::onDeactivate();
			foreach(CombinerLineKey key; keys) {
				key.combiner.setActive(0);
			}
		}
		
		//
		void inputs_init() {
			Combiner::inputs_init();
			inputs.addInput("pos",new InputConst(0.0f));
		}
		
	};
	
}

#endif
