/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    widget_sprite_viewport.h
 * Desc:    Unigine widgets
 * Version: 1.00
 * Author:  Vladimir Borovskikh <futurist@unigine.com>
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

#ifndef __EDITOR_WIDGET_SPRITE_VIEWPORT_H__
#define __EDITOR_WIDGET_SPRITE_VIEWPORT_H__

/*
 */
namespace Editor::Widgets {
	
	/*
	 */
	class SpriteViewport : Unigine::Widgets::Widget {
		
		WidgetSpriteViewport sprite_viewport;
		
		// constructor
		SpriteViewport(int width,int height) {
			if(width is WidgetSpriteViewport) {
				owner = 0;
				sprite_viewport = width;
			} else {
				sprite_viewport = new WidgetSpriteViewport(engine.getGui(),width,height);
			}
			widget = sprite_viewport;
		}
		
		// camera offset
		void setCameraOffset(vec3 offset) { sprite_viewport.setCameraOffset(offset) }
		vec3 getCameraOffset() { return sprite_viewport.getCameraOffset() }
		
		// inverse fps
		void setIFps(float time) { sprite_viewport.setIFps(time) }
		float getIFps() { return sprite_viewport.getIFps() }
		
		// postprocessing materials
		void setMaterials(string materials) { sprite_viewport.setMaterials(materials) }
		string getMaterials() { return sprite_viewport.getMaterials() }
		
		// model-view matrix
		void setModelview(mat4 matrix) { sprite_viewport.setModelview(matrix) }
		mat4 getModelview() { return sprite_viewport.getModelview() }
		
		// projection matrix
		void setProjection(mat4 matrix) { sprite_viewport.setProjection(matrix) }
		mat4 getProjection() { return sprite_viewport.getProjection() }
		
		// reflections mask
		void setReflectionMask(int mask) { sprite_viewport.setReflectionMask(mask) }
		int getReflectionMask() { return sprite_viewport.getReflectionMask() }
		
		// shadows rendering
		void setShadows(int shadows) { sprite_viewport.setShadows(shadows) }
		int getShadows() { return sprite_viewport.getShadows() }
		
		// height of the texture buffer
		void setTextureHeight(int height) { sprite_viewport.setTextureHeight(height) }
		int getTextureHeight() { return sprite_viewport.getTextureHeight() }
		
		// width of the texture buffer
		void setTextureWidth(int width) { sprite_viewport.setTextureWidth(width) }
		int getTextureWidth() { return sprite_viewport.getTextureWidth() }
		
		// rendering mask
		void setViewportMask(int mask) { sprite_viewport.setViewportMask(mask) }
		int getViewportMask() { return sprite_viewport.getViewportMask() }
		
		// renders into the image.
		void renderImage(Image image) { sprite_viewport.renderImage(image) }
	};
	
} /* namespace Editor */

#endif /* __EDITOR_WIDGET_SPRITE_VIEWPORT_H__ */
