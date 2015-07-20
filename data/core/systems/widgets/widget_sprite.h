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


#ifndef __UNIGINE_WIDGET_SPRITE_H__
#define __UNIGINE_WIDGET_SPRITE_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class Sprite : Widget {
		
		WidgetSprite sprite;
		
		// constructor
		Sprite(string name = 0) {
			if(name is WidgetSprite) {
				owner = 0;
				sprite = name;
			} else {
				sprite = new WidgetSprite(engine.getGui(),name);
			}
			widget = sprite;
		}
		
		// layers
		int addLayer() { return sprite.addLayer(); }
		void removeLayer(int layer) { sprite.removeLayer(layer); }
		int getNumLayers() { return sprite.getNumLayers(); }
		
		// first layer parameters
		void setColor(vec4 color) { sprite.setColor(color); }
		vec4 getColor() { return sprite.getColor(); }
		void setWrapRepeat(int repeat) { sprite.setWrapRepeat(repeat); }
		int getWrapRepeat() { return sprite.getWrapRepeat(); }
		void setBlendFunc(int src,int dest) { sprite.setBlendFunc(src,dest); }
		int getBlendSrcFunc() { return sprite.getBlendSrcFunc(); }
		int getBlendDestFunc() { return sprite.getBlendDestFunc(); }
		void setTexCoord(vec4 texcoord) { sprite.setTexCoord(texcoord); }
		vec4 getTexCoord() { return sprite.getTexCoord(); }
		void setTransform(mat4 transform) { sprite.setTransform(transform); }
		mat4 getTransform() { return sprite.getTransform(); }
		void setImage(Image image) { sprite.setImage(image); }
		Image getImage() { return sprite.getImage(); }
		void setTexture(string name) { sprite.setTexture(name); }
		string getTexture() { return sprite.getTexture(); }
		
		// arbitrary layer parameters
		void setLayerEnabled(int layer,int enable) { sprite.setLayerEnabled(layer,enable); }
		int isLayerEnabled(int layer) { return sprite.isLayerEnabled(layer); }
		int getLayerWidth(int layer) { return sprite.getLayerWidth(layer); }
		int getLayerHeight(int layer) { return sprite.getLayerHeight(layer); }
		void setLayerColor(int layer,vec4 color) { sprite.setLayerColor(layer,color); }
		vec4 getLayerColor(int layer) { return sprite.getLayerColor(layer); }
		void setLayerWrapRepeat(int layer,int repeat) { sprite.setLayerWrapRepeat(layer,repeat); }
		int getLayerWrapRepeat(int layer) { return sprite.getLayerWrapRepeat(layer); }
		void setLayerBlendFunc(int layer,int src,int dest) { sprite.setLayerBlendFunc(layer,src,dest); }
		int getLayerBlendSrcFunc(int layer) { return sprite.getLayerBlendSrcFunc(layer); }
		int getLayerBlendDestFunc(int layer) { return sprite.getLayerBlendDestFunc(layer); }
		void setLayerTexCoord(int layer,vec4 texcoord) { sprite.setLayerTexCoord(layer,texcoord); }
		vec4 getLayerTexCoord(int layer) { return sprite.getLayerTexCoord(layer); }
		void setLayerTransform(int layer,mat4 transform) { sprite.setLayerTransform(layer,transform); }
		mat4 getLayerTransform(int layer) { return sprite.getLayerTransform(layer); }
		void setLayerImage(int layer,Image image) { sprite.setLayerImage(layer,image); }
		Image getLayerImage(int layer) { return sprite.getLayerImage(layer); }
		void setLayerTexture(int layer,string name) { sprite.setLayerTexture(layer,name); }
		string getLayerTexture(int layer) { return sprite.getLayerTexture(layer); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_SPRITE_H__ */
