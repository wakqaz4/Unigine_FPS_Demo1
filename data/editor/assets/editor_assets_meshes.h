/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_assets_meshes.h
 * Desc:    Unigine editor
 * Version: 1.00
 * Author:  Maxim Belobryukhov <maxi@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

#ifndef __EDITOR_ASSETS_MESHES_H__
#define __EDITOR_ASSETS_MESHES_H__

namespace Assets::Meshes {
	
	/*
	 */
	int process(Asset asset,AssetRuntime runtime,string source,string destination) {
		
		Mesh m = new Mesh();
		if(m.load(source) == 0) {
			delete m;
			return 0;
		}
		
		Mesh res;
		
		if(asset.parameters.size()) {
			
			float import_scale = asset.getImportParameterFloat("scale",1.0f);
			int import_animation = asset.getImportParameterInt("animation",1);
			int vertex_cache = asset.getImportParameterInt("vertex_cache",1);
			
			// empty list = all surfaces
			string surfaces[0] = ();
			string surfaces_str = asset.getImportParameterString("surfaces","");
			if(strlen(surfaces_str)) strsplit(surfaces_str,",",surfaces);
			
			res = new Mesh();
			
			// create bones
			forloop(int i = 0; m.getNumBones()) {
				res.addBone(m.getBoneName(i),m.getBoneParent(i));
				res.setBoneTransform(i,m.getBoneTransform(i));
			}
			
			// surfaces
			forloop(int i = 0; m.getNumSurfaces()) {
				string surface_name = m.getSurfaceName(i);
				if(surfaces.size() && surfaces.find(surface_name) == -1) continue;
				
				res.addMeshSurface(surface_name,m,i);
			}
			
			res.createBounds();
			
			// animation
			if(import_animation) {
				forloop(int i = 0; m.getNumAnimations()) {
					int bones[0] = ();
					res.addAnimation(m.getAnimationName(i));
					
					forloop(int j = 0; m.getNumAnimationFrames(i)) {
						int frame[0] = ();
						m.getAnimationFrame(i,j,frame);
						res.setAnimationFrame(i,j,frame);
					}
					
					m.getAnimationBones(i,bones);
					res.setAnimationBones(i,bones);
				}
			}
			
			if(vertex_cache) res.optimizeIndices(MESH_VERTEX_CACHE);
			
			res.setSurfaceTransform(scale(import_scale,import_scale,import_scale));
			
			delete m;
			
		} else res = m;
		
		if(res.save(destination) == 0) {
			delete res;
			return 0;
		}
		
		delete res;
		return 1;
	}
	
	/*
	 */
	int import(Asset asset,string source) {
		
		if(!is_file(source)) return 0;
		
		// default parameters = no parameters
		asset.clearImportParameters();
		
		asset.addRuntime(basename(source),"Assets::Meshes::process");
		
		return 1;
	}
}

#endif /* __EDITOR_ASSETS_MESHES_H__ */
