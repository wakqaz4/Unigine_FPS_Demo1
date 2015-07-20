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


#ifndef __CHARACTER_LOCATORS_H__
#define __CHARACTER_LOCATORS_H__

#include <unigine.h>
#include <core/scripts/utils.h>

namespace Unigine {

	/******************************************************************************\
	 *
	 * class Locator
	 *
	\******************************************************************************/
	
	/*
	 */
	class Locator {
		
		ObjectMeshSkinned mesh;
		
		string name;
		string bone;
		int bone_index;
		
		Mat4 bind_matrix_inverse;
		Mat4 bind_scale_inverse;
		
		// constructor
		Locator(ObjectMeshSkinned mesh_) {
			mesh = mesh_;
		}
		
		// get locator name
		string getName() {
			return name;
		}
		
		// set locator name
		void setName(string name_) {
			name = name_;
		}
		
		// set locator bone
		void setBone(string bone_) {
			bone = bone_;
			bone_index = mesh.findBone(bone);
			if(bone_index < 0) throw("Locator::setBone(): bone \"%s\" is not found.",bone);
			bind_matrix_inverse = mesh.getBoneTransform(bone_index);
			bind_matrix_inverse = inverse(bind_matrix_inverse);
			bind_scale_inverse = scale(getTransformScale(bind_matrix_inverse));
		}
		
		// get locator bone
		string getBone() {
			return bone;
		}
		
		// get index of the assigned bone
		int getBoneIndex() {
			return bone_index;
		}
		
		// get the locator transform relative to bind pose of the bone
		Mat4 getRelativeTransform() {
			return bind_matrix_inverse * mesh.getBoneTransform(bone_index);
		}
		
		// get the locator local transform
		Mat4 getTransform() {
			return mesh.getBoneTransform(bone_index);
		}
		
		// get the locator absolute transform
		Mat4 getWorldTransform() {
			return mesh.getWorldBoneTransform(bone_index) * bind_scale_inverse;
		}
		
		//
		void load(Xml xml) {
			setName(xml.getArg("name"));
			setBone(xml.getData());
		}
		
		//
		void save(Xml xml) {
			xml.setArg("name",name);
			xml.setData(bone);
		}
		
	};
	
	/******************************************************************************\
	 *
	 * class LocatorManager
	 *
	\******************************************************************************/
	
	/*
	 */
	class LocatorManager {
		
		ObjectMeshSkinned mesh;
		string locators[];
		
		//
		LocatorManager(ObjectMeshSkinned mesh_) {
			mesh = mesh_;
		}
		
		//
		void clear() {
			locators.clear();
		}
		
		//
		void load(Xml xml) {
			
			clear();
			
			if(xml.isChild("locators") == 0) return;
			Xml node = xml.getChild("locators");
			
			forloop(int i = 0; node.getNumChilds()) {
				Xml child = node.getChild(i);
				if(child.getName() != "locator") continue;
				Locator locator = new Locator(mesh);
				locator.load(child);
				locators.append(locator.getName(),locator);
			}
		}
		
		//
		void save(Xml xml) {
			
			if(locators.size() == 0) return;
			Xml node = xml.addChild("locators");
			
			foreach(Locator locator; locators) {
				locator.save(node.addChild("locator"));
			}
		}
		
		//
		Locator getLocator(string name) {
			if(locators.check(name) == 0) return NULL;
			return locators[name];
		}
		
		
	};
	
}

#endif
