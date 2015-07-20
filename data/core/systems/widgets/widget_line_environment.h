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


#ifndef __UNIGINE_WIDGET_LINE_ENVIRONMENT_H__
#define __UNIGINE_WIDGET_LINE_ENVIRONMENT_H__

#include <core/systems/widgets/widget_line.h>
#include <core/systems/widgets/widget_dialog_file.h>
#include <core/systems/widgets/widget_dialog_message.h>

/*
 */
namespace Unigine::Widgets {
	
	/**************************************************************************\
	*
	* LineKeyEnvironment
	*
	\**************************************************************************/
	
	/*
	 */
	class LineKeyEnvironment : LineKey {
		
		vec3 coefficients[9];
		
		string name;
		
		/*
		 */
		LineKeyEnvironment(float t,vec3 c[],string n = "") : LineKey(t) {
			coefficients.copy(c);
			name = n;
		}
		
		/*
		 */
		void setCoefficients(vec3 c[]) {
			coefficients.copy(c);
		}
		
		void getCoefficients(vec3 c[]) {
			c.copy(coefficients);
		}
		
		/*
		 */
		void setName(string n) {
			name = n;
		}
		
		string getName() {
			return name;
		}
		
		/*
		 */
		vec4 get_color() {
			float scale = LineCurveEnvironment(curve).getScale();
			return saturate(vec4(coefficients[0] * 0.282095f * scale,1.0f));
		}
	};
	
	/**************************************************************************\
	*
	* LineCurveEnvironment
	*
	\**************************************************************************/
	
	/*
	 */
	class LineCurveEnvironment : LineCurve {
		
		float scale;
		
		/*
		 */
		LineCurveEnvironment() {
			
		}
		
		/*
		 */
		float getScale() {
			return scale;
		}
		
		/*
		 */
		int create_coefficients(string name,vec3 coefficients[]) {
			
			Image image = new Image();
			if(image.load(name) == 0) {
				log.error("Unigine::Widgets::LineCurveEnvironment::create_coefficients(): can't load \"%s\" file\n",name);
				delete image;
				return 0;
			}
			if(image.isCombinedFormat() && image.decombine() == 0) {
				log.error("Unigine::Widgets::LineCurveEnvironment::create_coefficients(): bad image format %s in \"%s\" file\n",image.getFormatName(),name);
				delete image;
				return 0;
			}
			if(image.isCompressedFormat() && image.decompress() == 0) {
				log.error("Unigine::Widgets::LineCurveEnvironment::create_coefficients(): bad image format %s in \"%s\" file\n",image.getFormatName(),name);
				delete image;
				return 0;
			}
			if(image.getType() != IMAGE_CUBE && image.convertToType(IMAGE_CUBE) == 0) {
				log.error("Unigine::Widgets::LineCurveEnvironment::create_coefficients(): bad image type %s in \"%s\" file\n",image.getTypeName(),name);
				delete image;
				return 0;
			}
			
			forloop(int i = 0; coefficients.size()) {
				coefficients[i] = vec3_zero;
			}
			
			float weight = 0.0f;
			float iwidth = 1.0f / image.getWidth();
			float iheight = 1.0f / image.getHeight();
			forloop(int i = 0; 6) {
				mat4 transform = cubeTransform(i);
				forloop(int y = 0; image.getHeight()) {
					float v = 2.0f * (y + 0.5f) * iheight - 1.0f;
					forloop(int x = 0; image.getWidth()) {
						float u = 2.0f * (x + 0.5f) * iwidth - 1.0f;
						
						vec3 color = vec3(image.getCube(x,y,i));
						
						float radius = u * u + v * v + 1.0f;
						float scale = 1.0f / (radius * sqrt(radius));
						
						vec3 direction = normalize(transform * vec3(u,-v,-1.0f));
						
						float Ylm_0 =  0.282095f * scale;
						float Ylm_1 =  0.488603f * scale * direction.y;
						float Ylm_2 =  0.488603f * scale * direction.z;
						float Ylm_3 =  0.488603f * scale * direction.x;
						float Ylm_4 = -1.092548f * scale * direction.x * direction.y;
						float Ylm_5 = -1.092548f * scale * direction.y * direction.z;
						float Ylm_6 =  1.092548f * scale * direction.z * direction.x;
						float Ylm_7 =  0.946176f * scale * direction.z * direction.z - 0.315392f * scale;
						float Ylm_8 =  0.546274f * scale * direction.x * direction.x - 0.546274f * scale * direction.y * direction.y;
						
						coefficients[0] += color * (Ylm_0 * 3.141593f);
						coefficients[1] += color * (Ylm_1 * 2.094395f);
						coefficients[2] += color * (Ylm_2 * 2.094395f);
						coefficients[3] += color * (Ylm_3 * 2.094395f);
						coefficients[4] += color * (Ylm_4 * 0.785398f);
						coefficients[5] += color * (Ylm_5 * 0.785398f);
						coefficients[6] += color * (Ylm_6 * 0.785398f);
						coefficients[7] += color * (Ylm_7 * 0.785398f);
						coefficients[8] += color * (Ylm_8 * 0.785398f);
						
						weight += Ylm_0;
						weight += Ylm_1;
						weight += Ylm_2;
						weight += Ylm_3;
						weight += Ylm_4;
						weight += Ylm_5;
						weight += Ylm_6;
						weight += Ylm_7;
						weight += Ylm_8;
					}
				}
			}
			
			float iweight = PI * 4.0f / (weight * 6.0f);
			coefficients[0] *= 0.282095f * iweight;
			coefficients[1] *= 0.488603f * iweight;
			coefficients[2] *= 0.488603f * iweight;
			coefficients[3] *= 0.488603f * iweight;
			coefficients[4] *= 1.092548f * iweight;
			coefficients[5] *= 1.092548f * iweight;
			coefficients[6] *= 1.092548f * iweight;
			coefficients[7] *= 0.315392f * iweight;
			coefficients[8] *= 0.546274f * iweight;
			
			return 1;
		}
		
		/*
		 */
		void get_coefficients(float time,vec3 coefficients[]) {
			
			int left = indices.left(time);
			int right = indices.right(time);
			
			if(left != -1 && right != -1) {
				
				LineKeyEnvironment k0 = keys[left];
				LineKeyEnvironment k1 = keys[right];
				float t0 = k0.time;
				float t1 = k1.time;
				
				if(t0 == t1) return coefficients.copy(k0.coefficients);
				float k = (time - t0) / (t1 - t0);
				k = k * k * (3.0f - 2.0f * k);
				
				return coefficients.lerp(k0.coefficients,k1.coefficients,k);
			}
			
			if(left != -1) {
				LineKeyEnvironment key = keys[left];
				return coefficients.copy(key.coefficients);
			}
			if(right != -1) {
				LineKeyEnvironment key = keys[right];
				return coefficients.copy(key.coefficients);
			}
			
			forloop(int i = 0; coefficients.size()) {
				coefficients[i] = vec3_zero;
			}
		}
		
		/*
		 */
		LineKeyEnvironment create_key(float time) {
			vec3 coefficients[9];
			get_coefficients(time,coefficients);
			return new LineKeyEnvironment(time,coefficients);
		}
		
		LineKeyEnvironment clone_key(LineKeyEnvironment key) {
			return new LineKeyEnvironment(key.getTime(),key.coefficients,key.getName());
		}
		
		/*
		 */
		void update_key(LineKeyEnvironment key) {
			
			// line
			LineCurve curve = key.getCurve();
			Line line = curve.getLine();
			
			// dialog result
			vec3 coefficients[9];
			string name = key.getName();
			if(dialogFileImage("Select environment",name)) {
				if(create_coefficients(name,coefficients)) {
					foreach(LineKey key; line.selected_keys) {
						if(key is LineKeyEnvironment) {
							LineKeyEnvironment(key).setCoefficients(coefficients);
							LineKeyEnvironment(key).setName(name);
						}
					}
					line.updateSelectedKeys();
				} else {
					dialogMessage("Error",engine.getError());
				}
			}
		}
		
		string get_update_func() {
			return "Unigine::Widgets::LineCurveEnvironment::update_key";
		}
		
		/*
		 */
		void create_image(int width) {
			image.create2D(width,6,IMAGE_FORMAT_RGBA8);
		}
		
		void update_image(float step,float offset) {
			
			// normalize colors
			vec3 color = vec3_one;
			foreach(LineKeyEnvironment key; keys) {
				color = max(color,key.coefficients[0]);
			}
			scale = 1.0f / max(max(color.x,color.y),color.z);
			
			// render image
			vec3 coefficients[9];
			forloop(int x = 0; image.getWidth()) {
				
				float time = step * x + offset;
				get_coefficients(time,coefficients);
				
				vec3 color = coefficients[0] - coefficients[7];
				vec3 color_x = color + coefficients[8];
				vec3 color_y = color - coefficients[8];
				vec3 color_z = color + coefficients[7] * 3.0f;
				
				image.set2D(x,0,vec4((color_x + coefficients[3]) * scale,1.0f));
				image.set2D(x,1,vec4((color_y + coefficients[1]) * scale,1.0f));
				image.set2D(x,2,vec4((color_x - coefficients[3]) * scale,1.0f));
				image.set2D(x,3,vec4((color_y - coefficients[1]) * scale,1.0f));
				image.set2D(x,4,vec4((color_z + coefficients[2]) * scale,1.0f));
				image.set2D(x,5,vec4((color_z - coefficients[2]) * scale,1.0f));
			}
		}
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_LINE_ENVIRONMENT_H__ */
