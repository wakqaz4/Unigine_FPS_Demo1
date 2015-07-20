/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_mask.h
 * Desc:    Unigine editor
 * Version: 1.08
 * Author:  Andrey Viktorov <unclebob@unigine.com>
 *          Alexander Zapryagaev <frustum@unigine.com>
 *          Mikhail Lyubimov <alterego@unigine.com>
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

/*
 */
namespace Brushes::Mask {
	
	/*
	 */
	enum {
		MAX_SPAWN_RATE = 32,
	};
	
	enum {
		CHANNEL_NONE = 0,
		CHANNEL_R = 1,
		CHANNEL_G = 2,
		CHANNEL_B = 4,
		CHANNEL_A = 8,
		CHANNEL_RGBA = 15,
	};
	
	enum {
		MODE_MASK_NORMAL = 0,
		MODE_MASK_ADDITION,
		MODE_MASK_SUBTRACTION,
		MODE_MASK_SMOOTH,
	};
	
	vec4 channels_value;
	float min_channels_value;
	float max_channels_value;
	
	Node shared_nodes[0];
	int old_spawn_rate;
	
	/******************************************************************************\
	*
	* BrushDecalMask
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushDecalMask : BrushDecalBase {
		
		DecalObjectOrtho decal;
		
		BrushDecalMask() {
			decal = new DecalObjectOrtho(1.0f,1.0f,1.0f,"decal_base");
			material = decal.getMaterialInherit();
			configure_material();
		}
		
		~BrushDecalMask() {
			delete decal;
		}
		
		void update(vec3 point) {
			decal.clear();
			mat4 transform = translate(point) * translate(0.0f,0.0f,radius * 2.0f);
			decal.setWorldTransform(node.getWorldTransform() * transform);
			Node parent = node.getParent();
			while(parent != NULL && parent.getType() == NODE_DUMMY) {
				parent = parent.getParent();
			}
			if(parent != NULL && parent.isObject()) {
				decal.create(node_cast(parent));
			} else {
				decal.create();
			}
		}
		
		void setRadius(float radius) {
			decal.setWidth(radius * 2.0f);
			decal.setHeight(radius * 2.0f);
			decal.setRadius(radius * 4.0f);
			decal.setOffset(0.5f);
		}
	};
	
	
	/******************************************************************************\
	*
	* BrushPaletteMask
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushPaletteMask : BrushPaletteBase {
		
		class SlotMask : SlotBase {
			vec4 channels_value = vec4_one;
			
			string get_info() {
				if(brush == BRUSH_EMPTY) return TR("Empty");
				string text = super.get_info();
				text += format(TR("Channels: R%d G%d B%d A%d"),channels_value.x,channels_value.y,channels_value.z,channels_value.w) + "<br>";
				return text;
			}
			
			
			string get_config_value() {
				string value = super.get_config_value();
				
				value += format(" %f %f %f %f",
					channels_value.x,
					channels_value.y,
					channels_value.z,
					channels_value.w
				);
				
				return value;
			}
			
			void config_apply(int value[]) {
				if(value.size() != 11) return;
				super.config_apply(value);
				channels_value = vec4(value[7],value[8],value[9],value[10]);
			}
		};
		
		SlotBase new_slot() {
			return new SlotMask();
		}
		
		// set/get/copy palette
		void set(int num) {
			SlotMask slot = slots[num];
			if(slot == NULL || slot.brush == BRUSH_EMPTY) return;
			channels_value_changed(slot.channels_value);
			super.set(num);
		}
		
		void get(int num) {
			SlotMask slot = slots[num];
			slot.channels_value = channels_value;
			super.get(num);
		}
		
		// set/get config
		string get_config_format() {
			return super.get_config_format() + " %f %f %f %f";
		}
		
		string get_config_name(int num) {
			return "mask_" + super.get_config_name(num);
		}
		
	};
	
	/******************************************************************************\
	*
	* BrushToolMask
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushToolMask : BrushToolBase {
			
		private:
			
			/*
			 */
			Image mask;
			Image image;
			Image filter;
			
			int width;
			int height;
			
			string name;
			
		public:
			
			/*
			 */
			BrushToolMask() {
				brush = BRUSH_MASK_CHANNELS;
				
				name = getNodeMaskImageName();
				
				mask = new Image();
				image = new Image();
				
				// load mask texture
				if(mask.load(name) == 0) return;
				
				// copy image
				width = mask.getWidth();
				height = mask.getHeight();
				image.create2D(width,height,mask.getFormat());
				image.copy(mask,0,0,0,0,width,height);
			}
			
			~BrushToolMask() {
				
				delete mask;
				delete image;
				delete filter;
			}
			
			void update(float X,float Y) {
				
				// check mask texture
				if(mask.isLoaded() == 0) return;
				
				// check mask levels
				if(max_channels_value < min_channels_value) return;
				
				// brush parameters
				float p = power * 10.0f + 0.1f;
				float o = opacity * engine.app.getIFps() * 8.0f;
				
				// scale coordinates
				float radius_x = radius / getNodeSizeX();
				float radius_y = radius / getNodeSizeY();
				
				X *= width - 1;
				Y *= height - 1;
				
				radius_x *= width;
				radius_y *= height;
				
				// brush bounds
				int x0 = clamp(int(floor(X - radius_x)),0,width);
				int y0 = clamp(int(floor(Y - radius_y)),0,height);
				int x1 = clamp(int(ceil(X + radius_x)),0,width);
				int y1 = clamp(int(ceil(Y + radius_y)),0,height);
				
				// smooth filter
				if(mode == MODE_MASK_SMOOTH) {
					if(filter == NULL) filter = new Image();
					filter.copy(image);
					filter.blur(3);
				}
				
				// brush
				float r = 0.0f;
				forloop(int y = y0; y1) {
					float dy = abs((y - Y) / radius_y);
					float tex_y = float(y - y0) / float(y1 - y0);
					
					forloop(int x = x0; x1) {
						float dx = abs((x - X) / radius_x);
						float tex_x = float(x - x0) / float(x1 - x0);
						
						r = calc_shape_radius(dx,dy,tex_x,tex_y);
						if(r > 1.0f) continue;
						
						float k = o * saturate(1.0f - pow(r,p));
						
						vec4 c = image.get2D(x,y);
						
						switch(mode) {
							case MODE_MASK_NORMAL: c = lerp(c,channels_value,k); break;
							case MODE_MASK_ADDITION: c = saturate(c + channels_value * k); break;
							case MODE_MASK_SUBTRACTION: c = saturate(c - channels_value * k); break;
							case MODE_MASK_SMOOTH: c = lerp(c,filter.get2D(x,y),k); break;
						}
						
						float min_value = (mode == MODE_MASK_SUBTRACTION) ? 0.0f : min_channels_value;
						
						c.r = clamp(c.r,min_value,max_channels_value);
						c.g = clamp(c.g,min_value,max_channels_value);
						c.b = clamp(c.b,min_value,max_channels_value);
						c.a = clamp(c.a,min_value,max_channels_value);
						
						image.set2D(x,y,vec4(c));
					}
				}
				
				setNodesMaskImage(image);
			}
			
			void flush() {
				
				if(brush == -1) return;
				
				brush = -1;
				
				// check diffuse texture
				if(image.isLoaded() == 0) return;
				
				// save diffuse texture
				image.save(name);
				
				delete filter;
			}
			
			void reset() {
				
				// check diffuse texture
				if(mask.isLoaded() == 0) return;
				
				// restore diffuse texture
				mask.save(name);
				
				setNodesMaskImage(mask);
			}
	};
	
	
	/******************************************************************************\
	*
	* Enable/disable
	*
	\******************************************************************************/
	
	/*
	 */
	void enable(Node n,int channels,float min_mask,float max_mask) {
		
		is_terrain = false;
		panel_brush = panelsGetPanelBrush();
		node = node_cast(n);
		
		brush = BRUSH_MASK_CHANNELS;
		panel_brush.set_brush(BRUSH_MASK_CHANNELS);
		
		string config = format("%d %d %f %f %f %f %f %f %f %s",
			shape,
			mode,
			radius,
			power,
			opacity,
			channels_value.x,
			channels_value.y,
			channels_value.z,
			channels_value.w,
			shape_texture,
		);
		
		config = configGet("mask_editor_settings",config);
		
		int ret[0];
		if(sscanf(config,"%d %d %f %f %f %f %f %f %f %s",ret) == 10) {
			
			shape = ret[0];
			mode = ret[1];
			
			radius = ret[2];
			power = ret[3];
			opacity = ret[4];
			
			channels_value = vec4(ret[5],ret[6],ret[7],ret[8]);
			
			shape_texture = ret[9];
		}
		
		shape_changed(shape);
		mode_changed(mode);
		radius_changed(radius);
		power_changed(power);
		opacity_changed(opacity);
		shape_texture_changed(shape_texture);
		channels_value_changed(channels_value);
		
		min_channels_value = 0.0f;
		max_channels_value = 1.0f;
		
		
		old_spawn_rate = getNodeSpawnRate();
		setNodeSpawnRate(max(old_spawn_rate,min(getNodeSizeX() * getNodeSizeY(),MAX_SPAWN_RATE)));
		
		brush_decal = new BrushDecalMask();
		
		brush_palette = new BrushPaletteMask();
		
		panel_brush.set_channels_enabled(0,(channels & CHANNEL_R) == CHANNEL_R);
		panel_brush.set_channels_enabled(1,(channels & CHANNEL_G) == CHANNEL_G);
		panel_brush.set_channels_enabled(2,(channels & CHANNEL_B) == CHANNEL_B);
		panel_brush.set_channels_enabled(3,(channels & CHANNEL_A) == CHANNEL_A);
		
		min_channels_value = min_mask;
		max_channels_value = max_mask;
		
		shared_nodes.clear();
		
		string name = getNodeMaskImageName();
		
		forloop(int i = 0; engine.editor.getNumNodes()) {
			Node editor_node = node_cast(engine.editor.getNode(i));
			
			if(is_valid_node(editor_node.getType()) == false) continue;
			if(editor_node.call("getMaskImageName") != name) continue;
			
			shared_nodes.append(editor_node);
		}
		
		Brushes::enable();
	}
	
	void disable(int reset = 0) {
		
		string config = format(
			"%d %d %f %f %f %f %f %f %f %s",
			shape,mode,radius,power,opacity,channels_value.x,channels_value.y,channels_value.z,channels_value.w,shape_texture
		);
		
		configSet("mask_editor_settings",config);
		
		setNodeSpawnRate(old_spawn_rate);
		
		shared_nodes.clear();
	}
	
	/*
	 */
	int get_intersection_point(vec3 &ip,float &x,float &y) {
		
		vec3 p0,p1;
		Unigine::getPlayerMouseDirection(p0,p1);
		
		WorldIntersection intersection = new WorldIntersection();
		if(engine.world.getIntersection(p0,p1,~0,intersection) == NULL) return 0;
		
		ip = node.getIWorldTransform() * intersection.getPoint();
		
		x = ip.x / getNodeSizeX();
		y = ip.y / getNodeSizeY();
		
		if(getNodeMaskFlipX()) x = 1.0f - x;
		if(getNodeMaskFlipY()) y = 1.0f - y;
		
		return 1;
	}
	
	/*
	 */
	BrushToolBase new_brush_tools() {
		return new BrushToolMask()
	}
	
	/******************************************************************************\
	*
	* GUI callbacks
	*
	\******************************************************************************/
	
	/*
	 */
	void channels_value_changed(vec4 new_channels_value) {
		channels_value = new_channels_value;
		panel_brush.set_channels_value(channels_value);
	}
	
	/******************************************************************************\
	*
	* Node helpers
	*
	\******************************************************************************/
	
	/*
	 */
	int is_valid_node(int type) {
		switch(type) {
			case NODE_OBJECT_GRASS: return 1;
			case NODE_OBJECT_MESH_CLUTTER: return 1;
			case NODE_WORLD_CLUTTER: return 1;
		}
		return 0;
	}
	
	/*
	 */
	int getNodeSizeX() {
		if(!is_valid_node(node.getType())) return 0;
		return node.call("getSizeX");
	}
	
	int getNodeSizeY() {
		if(!is_valid_node(node.getType())) return 0;
		return node.call("getSizeY");
	}
	
	int getNodeStep() {
		if(!is_valid_node(node.getType())) return 0;
		return node.call("getStep");
	}
	
	/*
	 */
	void setNodesMaskImage(Image mask) {
		forloop(int i = 0; shared_nodes.size()) {
			Node shared_node = shared_nodes[i];
			shared_node.call("setMaskImage",mask);
		}
		
		node.call("setMaskImage",mask);
	}
	
	string getNodeMaskImageName() {
		if(!is_valid_node(node.getType())) return "";
		return node.call("getMaskImageName");
	}
	
	/*
	 */
	int getNodeMaskFlipX() {
		if(!is_valid_node(node.getType())) return 0;
		return node.call("getMaskFlipX");
	}
	
	int getNodeMaskFlipY() {
		if(!is_valid_node(node.getType())) return 0;
		return node.call("getMaskFlipY");
	}
	
	/*
	 */
	int getNodeSpawnRate() {
		if(!is_valid_node(node.getType())) return 0;
		return node.call("getSpawnRate");
	}
	
	void setNodeSpawnRate(int spawn_rate) {
		if(!is_valid_node(node.getType())) return;
		return node.call("setSpawnRate",spawn_rate);
	}
}
/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void maskEnable(Node node,int channels,float min_mask = 0.0f,float max_mask = 1.0f) {
	Brushes::Mask::enable(node,channels,min_mask,max_mask);
}
