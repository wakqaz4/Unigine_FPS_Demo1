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


#ifndef __COMBINER_ANIMATION_H__
#define __COMBINER_ANIMATION_H__

#include <unigine.h>
#include <scripts/character/combiner.h>

namespace Unigine {
	
	/******************************************************************************\
	 *
	 * class Animation
	 *
	\******************************************************************************/
	
	/*
	 */
	class Animation {
		
		ObjectMeshSkinned mesh;
		
		string animation;
		int animation_id;
		int start;
		int end;
		int num_frames;
		float frame;
		float speed;
		
		//
		void update(float dt) {
			frame += dt * speed;
			while(frame < 0.0f) frame += num_frames;
		}
		
		//
		void setup(int layer) {
			mesh.setAnimation(layer,animation_id);
			mesh.setFrame(layer,frame,start,end);
		}
		
		//
		void setAnimation(string animation_) {
			animation = animation_;
			if(animation == "") return;
			animation_id = mesh.addAnimation(animation);
			num_frames = mesh.getNumAnimationFrames(animation_id);
		}
		
		//
		string getAnimation() {
			return animation;
		}
		
		//
		void save(Xml xml) {
			xml.setIntArg("start",start);
			xml.setIntArg("end",end);
			xml.setData(animation);
		}
		
		//
		void load(Xml xml) {
			start = xml.getIntArg("start");
			end = xml.getIntArg("end");
			animation = xml.getData();
			setAnimation(animation);
		}
		
		//
		int getNumFrames() {
			return num_frames;
		}
		
		//
		void setPhase(float phase) {
			phase -= floor(phase);
			frame = phase * float(num_frames - 1);
		}
		
		//
		float getPhase() {
			float phase = frame / float(num_frames - 1);
			return phase - floor(phase);
		}
		
		//
		float getFrame() { return frame; }
		void setFrame(float frame_) { frame = frame_; }
		
		//
		float getSpeed() { return speed; }
		void setSpeed(float speed_) { speed = speed_; }
		
		//
		int getStartFrame() { return start; }
		void setStartFrame(int frame_) { start = frame_; }
		
		//
		int getEndFrame() { return end; }
		void setEndFrame(int frame_) { end = frame_; }
		
		//
		Animation(ObjectMeshSkinned mesh_,string animation_ = "",int start_ = 0,int end_ = -1,float speed_ = 1.0f) {
			
			mesh = mesh_;
			start = start_;
			end = end_;
			speed = speed_;
			setAnimation(animation);
		}
	};
	
	/******************************************************************************\
	 *
	 * class CombinerAnimation
	 *
	\******************************************************************************/
	
	/*
	 */
	class CombinerAnimation : Combiner {
		
		Animation anim;
		
		//
		CombinerAnimation(ObjectMeshSkinned mesh_) {
			combiner_init(mesh_);
			anim = new Animation(mesh_);
		}
		
		//
		void update(float dt) {
			if(isActive() == 0) return;
			Input speed = inputs.getInput("speed");
			anim.speed = float(speed.getValue());
			anim.update(dt);
			Combiner::update(dt);
		}
		
		//
		void setup() {
			if(isActive() == 0) return;
			anim.setup(layer);
		}
		
		//
		void save(Xml xml) {
			Combiner::save(xml);
			anim.save(xml.addChild("animation"));
		}
		
		//
		void load(Xml xml,ParameterManager parameters = NULL) {
			Combiner::load(xml,parameters);
			anim = new Animation(mesh);
			forloop(int i = 0; xml.getNumChilds()) {
				Xml child = xml.getChild(i);
				if(child.getName() != "animation") continue;
				anim.load(xml.getChild(i));
				break;
			}
		}
		
		//
		void inputs_init() {
			Combiner::inputs_init();
			inputs.addInput("speed",new InputConst(1.0f));
			inputs.addInput("phase",new InputConst(0.0f));
		}
		
		//
		float getPhase() {
			return anim.getPhase();
		}
		
		//
		void setPhase(float phase) {
			anim.setPhase(phase);
		}
		
		//
		void onActivate() {
			anim.setFrame(0.0f);
			Combiner::onActivate();
		}
		
	};
	
}

#endif
