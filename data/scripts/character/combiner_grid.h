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


#ifndef __COMBINER_GRID_H__
#define __COMBINER_GRID_H__

#include <unigine.h>
#include <scripts/character/combiner.h>

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * class CombinerGridKey
	 *
	\******************************************************************************/
	
	/*
	 */
	class CombinerGridKey {
		
		int pos_x;
		int pos_y;
		Combiner combiner;
		
		//
		void load(Xml xml,ObjectMeshSkinned mesh,ParameterManager parameters = NULL) {
			pos_x = xml.getIntArg("pos_x");
			pos_y = xml.getIntArg("pos_y");
			Xml child = xml.getChild("combiner");
			combiner = loadCombiner(child,mesh,parameters);
		}
		
		//
		void save(Xml xml) {
			xml.setIntArg("pos_x",pos_x);
			xml.setIntArg("pos_y",pos_y);
			Xml child = xml.addChild("combiner");
			saveCombiner(child,combiner);
		}
		
	};
	
	/******************************************************************************\
	 *
	 * class CombinerGrid
	 *
	\******************************************************************************/
	
	/*
	 */
	class CombinerGrid : Combiner {
		
		CombinerGridKey keys[0];
		int num_keys_x;
		int num_keys_y;
		float step_x;
		float step_y;
		
		Combiner cur_keys[4];
		float weights[4];
		
		//
		CombinerGrid(ObjectMeshSkinned mesh_) {
			combiner_init(mesh_);
		}
		
		//
		float getMaxX() {
			return float(num_keys_x - 1) * step_x;
		}
		
		//
		float getMaxY() {
			return float(num_keys_y - 1) * step_y;
		}
		
		//
		void update(float dt) {
			
			if(isActive() == 0) return;
			
			Input param_x = inputs.getInput("pos_x");
			Input param_y = inputs.getInput("pos_y");
			
			float x = float(param_x.getValue());
			float y = float(param_y.getValue());
			
			float size_x = getMaxX();
			float size_y = getMaxY();
			
			x = clamp(x,0.0f,size_x);
			y = clamp(y,0.0f,size_y);
			
			int x0 = clamp(int(floor(x / step_x)),0,num_keys_x - 2);
			int x1 = x0 + 1;
			int y0 = clamp(int(floor(y / step_y)),0,num_keys_x - 2);
			int y1 = y0 + 1;
			
			float rx = (x - x0 * step_x) / step_x;
			float ry = (y - y0 * step_y) / step_y;
			
			weights[0] = (1.0f - rx) * (1.0f - ry);
			weights[1] = (0.0f + rx) * (1.0f - ry);
			weights[2] = (0.0f + rx) * (0.0f + ry);
			weights[3] = (1.0f - rx) * (0.0f + ry);
			
			int key_0 = y0 * num_keys_x + x0;
			int key_1 = y0 * num_keys_x + x1;
			int key_2 = y1 * num_keys_x + x1;
			int key_3 = y1 * num_keys_x + x0;
			
			cur_keys[0] = keys[key_0].combiner;
			cur_keys[1] = keys[key_1].combiner;
			cur_keys[2] = keys[key_2].combiner;
			cur_keys[3] = keys[key_3].combiner;
			
			// deactivate all the others
			forloop(int i = 0; keys.size()) {
				if(i != key_0 && i != key_1 && i != key_2 && i != key_3) {
					if(keys[i].combiner.active == 1) keys[i].combiner.setActive(0);
				} else {
					if(keys[i].combiner.active == 0) keys[i].combiner.setActive(1);
					keys[i].combiner.update(dt);
				}
			}
			
			Combiner::update(dt);
		}
		
		//
		void setup() {
			
			if(isActive() == 0) return;
			
			float summ_weight = 0.0f;
			
			mesh.clearLayer(layer);
			forloop(int i = 0; 4) {
				if(weights[i] < EPSILON) continue;
				Combiner key = cur_keys[i];
				
				key.setup();
				
				if(summ_weight < EPSILON) {
					summ_weight = weights[i];
					mesh.copyLayer(layer,key.layer);
				} else {
					summ_weight += weights[i];
					float interpolation = weights[i] / summ_weight;
					mesh.lerpLayer(layer,layer,key.layer,interpolation);
				}
			}
		}
		
		//
		void save(Xml xml) {
			Combiner::save(xml);
			Xml parent = xml.addChild("grid");
			parent.setIntArg("num_keys_x",num_keys_x);
			parent.setIntArg("num_keys_y",num_keys_y);
			parent.setFloatArg("max_x",getMaxX());
			parent.setFloatArg("max_y",getMaxY());
			foreach(CombinerGridKey key; keys) {
				Xml child = parent.addChild("key");
				key.save(child);
			}
		}
		
		//
		void load(Xml xml,ParameterManager parameters = NULL) {
			
			Combiner::load(xml,parameters);
			keys.clear();
			Xml parent = xml.getChild("grid");
			
			num_keys_x = parent.getIntArg("num_keys_x");
			num_keys_y = parent.getIntArg("num_keys_y");
			
			if(num_keys_x < 2 || num_keys_y < 2) throw("CombinerGird::load(): num_keys < 2.");
			
			step_x = parent.getFloatArg("max_x") / float(num_keys_x - 1);
			step_y = parent.getFloatArg("max_y") / float(num_keys_y - 1);
			
			keys.resize(num_keys_x * num_keys_y);
			
			forloop(int i = 0; parent.getNumChilds()) {
				Xml child = parent.getChild(i);
				if(child.getName() != "key") continue;
				CombinerGridKey key = new CombinerGridKey();
				key.load(child,mesh,parameters);
				keys[key.pos_y * num_keys_x + key.pos_x] = key;
			}
			forloop(int i = 0; keys.size()) {
				if(keys[i] == NULL) throw("CombinerGrid::load(): not all grid keys was specified. key in pos=[%d][%d] mismatch.",i % num_keys_x,i / num_keys_x);
			}
		}
		
		//
		void onDeactivate() {
			Combiner::onDeactivate();
			foreach(CombinerGridKey key; keys) {
				key.combiner.setActive(0);
			}
		}
		
		//
		void inputs_init() {
			Combiner::inputs_init();
			inputs.addInput("pos_x",new InputConst(0.0f));
			inputs.addInput("pos_y",new InputConst(0.0f));
		}
		
	};
	
}

#endif
