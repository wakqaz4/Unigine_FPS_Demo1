/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_terrain.h
 * Desc:    Unigine editor
 * Version: 1.15
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Andrey Viktorov <unclebob@unigine.com>
 *          Mikhail Lyubimov <alterego@unigine.com>
 *          Maxim Belobryukhov <maxi@unigine.com>
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

namespace Brushes::Terrain {
	
	/*
	 */
	enum {
		TERRAIN_SURFACE_STEP = 256,
	};
	
	enum {
		TERRAIN_HEIGHT_OFFSET = 0.1f,
		TERRAIN_MIN_HEIGHT = -100.0f,
		TERRAIN_MAX_HEIGHT = 100.0f,
		TERRAIN_MIN_ANGLE = 0.0f,
		TERRAIN_MAX_ANGLE = 90.0f,
	};
	
	enum {
		MODE_HEIGHTS_NORMAL = 0,
		MODE_HEIGHTS_ADDITION,
		MODE_HEIGHTS_SUBTRACTION,
		MODE_HEIGHTS_SMOOTH,
		MODE_HEIGHTS_DILATE,
		MODE_HEIGHTS_ERODE,
	};
	
	enum {
		MODE_DIFFUSE_NORMAL = 0,
		MODE_DIFFUSE_ADDITION,
		MODE_DIFFUSE_SUBTRACTION,
		MODE_DIFFUSE_SMOOTH,
		MODE_DIFFUSE_MULTIPLY,
	};
	
	enum {
		MODE_MASK_NORMAL = 0,
		MODE_MASK_ADDITION,
		MODE_MASK_SUBTRACTION,
		MODE_MASK_SMOOTH,
	};
	
	enum {
		MODE_MATERIAL_NORMAL = 0,
		MODE_MATERIAL_ADDITION,
		MODE_MATERIAL_SUBTRACTION,
		MODE_MATERIAL_SMOOTH,
	};
	
	float min_angle = TERRAIN_MIN_ANGLE;
	float max_angle = TERRAIN_MAX_ANGLE;
	
	float cos_min_angle = 1.0f;
	float cos_max_angle = 0.0f;
	
	float min_height = TERRAIN_MIN_HEIGHT;
	float max_height = TERRAIN_MAX_HEIGHT;
	
	float heights_value;
	vec4 diffuse_value = vec4_one;
	int mask_value;
	
	ObjectTerrain terrain;
	
	string surface_diffuse_name;
	Image surface_diffuse_cache;
	
	string surface_index_name;
	Image surface_index_cache;
	
	string surface_mask_name;
	Image surface_mask_cache;
	
	int surface_resolution;
	int coarse_resolution;
	
	/******************************************************************************\
	*
	* BrushDecalTerrain
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushDecalTerrain : BrushDecalBase {
		
		DecalTerrainOrtho decal;
		
		BrushDecalTerrain() {
			decal = new DecalTerrainOrtho(1.0f,1.0f,1.0f,"decal_base");
			material = decal.getMaterialInherit();
			configure_material();
			decal.setParent(terrain);
			decal.setEnabled(0);
			decal.setHandled(0);
		}
		~BrushDecalTerrain() {
			delete decal;
		}
		
		void update(Vec3 point) {
			Mat4 transform = translate(point) * translate(0.0f,0.0f,terrain.getStep() * radius * 2.0f);
			decal.setWorldTransform(terrain.getWorldTransform() * transform);
			decal.setEnabled(1);
		}
		
		void setRadius(float radius) {
			float step = terrain.getStep();
			decal.setWidth(step * radius * 2.0f);
			decal.setHeight(step * radius * 2.0f);
			decal.setRadius(step * radius * 4.0f);
		}
		
		void setEnabled(int val) {
			if(val != decal.isEnabled()) decal.setEnabled(val);
		}
		
		void isEnabled() {
			return decal.isEnabled();
		}
	};
	
	BrushDecalTerrain brush_decal_terrain;
	
	/******************************************************************************\
	*
	* BrushPaletteTerrain
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushPaletteTerrain : BrushPaletteBase {
		
		class SlotTerrain : SlotBase {
		
			float heights_value = 0.0f;
			vec4 diffuse_value = vec4_one;
			int mask_value = 0;
			
			string get_info() {
				if(brush == BRUSH_EMPTY) return TR("Empty");
				
				string text = TR("Brush: ") + brush_text[brush] + "<br>";
				text += super.get_info();
				
				if(brush == BRUSH_HEIGHTS) text += TR("Height: ") + editorFormat(heights_value);
				else if(brush == BRUSH_DIFFUSE) text += format(TR("Color: %.02g %.02g %.02g"),diffuse_value.x,diffuse_value.y,diffuse_value.z);
				else if(brush == BRUSH_MASK) text += TR("Material: ") + get_mask_value_text(mask_value);
				else if(brush == BRUSH_MATERIAL) text += format(TR("Color: %.02g %.02g %.02g<br>Material: "),diffuse_value.x,diffuse_value.y,diffuse_value.z) + get_mask_value_text(mask_value);
				
				return text;
			}
			
			string get_config_value() {
				string value = super.get_config_value();
				
				value += format(" %d",heights_value);
				value += format(" %f %f %f %f",
					diffuse_value.x,
					diffuse_value.y,
					diffuse_value.z,
					diffuse_value.w
				);
				value += format(" %d",mask_value);
				
				return value;
			}
			
			void config_apply(int value[]) {
				if(value.size() < 13) return;
				
				super.config_apply(value);
				
				heights_value = value[7];
				diffuse_value = vec4(value[8],value[9],value[10],value[11]);
				mask_value = value[12];
			}
		};
		
		
		SlotBase new_slot() {
			return new SlotTerrain();
		}
		
		void update_slot(int num) {
			SlotTerrain slot = slots[num];
			panel_brush.update_slot_color(num,slot.diffuse_value);
			super.update_slot(num);
		}
		
		// set/get/copy palette
		void set(int num) {
			SlotTerrain slot = slots[num];
			if(slot == NULL || slot.brush == BRUSH_EMPTY) return;
			
			if(slot.brush == BRUSH_DIFFUSE || slot.brush == BRUSH_MATERIAL) diffuse_value = slot.diffuse_value;
			if(slot.brush == BRUSH_MASK || slot.brush == BRUSH_MATERIAL) mask_value = slot.mask_value;
			
			heights_value_changed(slot.heights_value);
			
			super.set(num);
		}
		void get(int num) {
			SlotTerrain slot = slots[num];
			
			slot.heights_value = heights_value;
			slot.diffuse_value = diffuse_value;
			slot.mask_value = mask_value;
			
			super.get(num);
		}
		
		// set/get config
		string get_config_format() {
			return super.get_config_format() + " %d %f %f %f %f %d";
		}
		
		string get_config_name(int num) {
			return "terrain_" + super.get_config_name(num);
		}
	};
	
	/******************************************************************************\
	*
	* TerrainLimits
	*
	\******************************************************************************/
	
	/*
	 */
	class TerrainLimits {
		
		ObjectMeshDynamic min_height_plane;
		ObjectMeshDynamic max_height_plane;
		
		float fade_time = 0.5f;
		float visible_time = 1.0f;
		float current_time = 0.0f;
		float alpha = 0.5f;
		
		enum {
			STATE_HIDDEN = 0,
			STATE_FADE_IN,
			STATE_FADE_OUT,
			STATE_VISIBLE,
		};
		
		int state;
		
		/*
		 */
		TerrainLimits() {
			
			min_height_plane = new ObjectMeshDynamic();
			max_height_plane = new ObjectMeshDynamic();
			
			create_plane(min_height_plane);
			create_plane(max_height_plane);
			
			state = STATE_HIDDEN;
		}
		
		/*
		 */
		~TerrainLimits() {
			delete min_height_plane;
			delete max_height_plane;
		}
		
		/*
		 */
		void create_plane(ObjectMeshDynamic plane) {
			
			plane.setMaterial("mesh_base","*");
			Material material = plane.getMaterialInherit(0);
			
			plane.setMaterialState("deferred",0,0);
			plane.setMaterialParameter("diffuse_color",vec4(0.0f,0.0f,1.0f,alpha),0);
			
			material.setBlendFunc(MATERIAL_BLEND_SRC_ALPHA,MATERIAL_BLEND_ONE_MINUS_SRC_ALPHA);
			material.setState("deferred",0);
			material.setState("ambient",2);
			material.setState("light_spot",0);
			material.setState("light_omni",0);
			material.setState("light_proj",0);
			material.setState("light_world",0);
			material.setDepthMask(0);
			material.setCastShadow(0);
			material.setCastWorldShadow(0);
			material.setReceiveShadow(0);
			material.setReceiveWorldShadow(0);
			
			float size_x = terrain.getSizeX() * terrain.getStep();
			float size_y = terrain.getSizeY() * terrain.getStep();
			
			plane.addTriangleQuads(1);
			plane.addVertex(vec3(0,0,0.0f));
			plane.addTexCoord(vec4(0.0f,0.0f,0.0f,0.0f));
			plane.addVertex(vec3(size_x,0.0f,0.0f));
			plane.addTexCoord(vec4(1.0f,0.0f,0.0f,0.0f));
			plane.addVertex(vec3(size_x,size_y,0.0f));
			plane.addTexCoord(vec4(1.0f,1.0f,0.0f,0.0f));
			plane.addVertex(vec3(0.0f,size_y,0.0f));
			plane.addTexCoord(vec4(0.0f,1.0f,0.0f,0.0f));
			
			plane.updateBounds();
			plane.flushVertex();
			plane.flushIndices();
		}
		
		/*
		 */
		void update_plane_position(ObjectMeshDynamic plane,float height) {
			Mat4 transform = terrain.getWorldTransform();
			plane.setWorldTransform(transform * translate(0.0f,0.0f,height));
		}
		
		void updateHeights() {
			
			update_plane_position(min_height_plane,min_height - TERRAIN_HEIGHT_OFFSET);
			update_plane_position(max_height_plane,max_height + TERRAIN_HEIGHT_OFFSET);
			
			if(state == STATE_HIDDEN) {
				state = STATE_FADE_IN;
				current_time = fade_time;
			}
			
			if(state == STATE_VISIBLE) {
				current_time = visible_time;
			}
		}
		
		void update(float ifps) {
			
			if(state == STATE_HIDDEN) return;
			
			int is_fading = (state == STATE_FADE_IN) || (state == STATE_FADE_OUT);
			
			if(is_fading) {
				
				float k = current_time / fade_time;
				float current_alpha = lerp(0.0f,alpha,(state == STATE_FADE_OUT) ? k : 1 - k);
				
				max_height_plane.setMaterialParameter("diffuse_color",vec4(0.0f,0.0f,1.0f,current_alpha),0);
			}
			
			current_time -= ifps;
			
			if(current_time > 0.0f) return;
			
			switch(state) {
				case STATE_FADE_IN: 
					state = STATE_VISIBLE;
					current_time = visible_time;
					break;
				case STATE_FADE_OUT:
					state = STATE_HIDDEN;
					current_time = 0.0f;
					break;
				case STATE_VISIBLE:
					state = STATE_FADE_OUT;
					current_time = fade_time;
					break;
			};
		}
	};
	
	TerrainLimits terrain_limits;
	
	/******************************************************************************\
	*
	* BrushSettings
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushSettings {
		
		int brush = BRUSH_DIFFUSE;
		
		int shape = 0;
		int mode = 0;
		string shape_texture = shape_default_texture;
		
		float radius = 10.0f;
		float power = 1.0f;
		float opacity = 0.5f;
		
		BrushSettings(int b) {
			brush = b;
		}
		
		// set/get settings
		void set() {
			shape_texture_changed(shape_texture);
			shape_changed(shape);
			mode_changed(mode);
			radius_changed(radius);
			power_changed(power);
			opacity_changed(opacity);
		}
		
		void get() {
			
			shape = Brushes::shape;
			mode = Brushes::mode;
			
			shape_texture = Brushes::shape_texture;
			
			radius = Brushes::radius;
			power = Brushes::power;
			opacity = Brushes::opacity;
		}
		
		// set/get config
		string get_name() {
			if(brush == BRUSH_HEIGHTS) return "terrain_brush_heights";
			if(brush == BRUSH_HOLES) return "terrain_brush_holes";
			if(brush == BRUSH_DIFFUSE) return "terrain_brush_diffuse";
			if(brush == BRUSH_MASK) return "terrain_brush_mask";
			if(brush == BRUSH_MATERIAL) return "terrain_brush_diffuse_material";
			else throw("Terrain::BrushSettings::update(): unknown brush\n");
		}
		
		string get_current_config() {
			string config = format("%d %d %f %f %f %s",shape,mode,radius,power,opacity,shape_texture);
			return config;
		}
		
		void configSet() {
			::configSet(get_name(),get_current_config());
		}
		
		void configGet() {
			string config = ::configGet(get_name(),get_current_config());
			
			string ret[0];
			if(sscanf(config,"%d %d %f %f %f %s",ret) < 6) {
				
				shape = ret[0];
				mode = ret[1];
				
				radius = ret[2];
				power = ret[3];
				opacity = ret[4];
				
				shape_texture = ret[5];
			}
		}
	};
	
	BrushSettings brush_settings_heights = new BrushSettings(BRUSH_HEIGHTS);
	BrushSettings brush_settings_holes = new BrushSettings(BRUSH_HOLES);
	BrushSettings brush_settings_diffuse = new BrushSettings(BRUSH_DIFFUSE);
	BrushSettings brush_settings_mask = new BrushSettings(BRUSH_MASK);
	BrushSettings brush_settings_material = new BrushSettings(BRUSH_MATERIAL);
	
	/******************************************************************************\
	*
	* BrushToolTerrain
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushToolTerrain : BrushToolBase {
			
		private:
			
			/*
			 */
			int check_limits(float x,float y) {
				
				// height limits check
				float height = terrain.getHeight(x,y);
				if(height < min_height) return 0;
				if(height > max_height) return 0;
				
				// angle limits check
				vec3 normal = terrain.getNormal(x,y);
				if(normal.z > cos_min_angle) return 0;
				if(normal.z < cos_max_angle) return 0;
				
				return 1;
			}
			
	};
	
	/******************************************************************************\
	*
	* BrushToolHeights
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushToolHeights : BrushToolTerrain {
		
		float heights[];
		float filter[0];
		
		int min_surface_x0;
		int min_surface_y0;
		int max_surface_x1;
		int max_surface_y1;
		
		Image normals;
		
		
		BrushToolHeights() {
			brush = BRUSH_HEIGHTS;
			min_surface_x0 = 1000000;
			min_surface_y0 = 1000000;
			max_surface_x1 = 0;
			max_surface_y1 = 0;
			normals = new Image();
		}
		~BrushToolHeights() {
			heights.delete();
			filter.delete();
			delete normals;
		}
		
		void update(float X,float Y) {
			
			// brush parameters
			float p = power * 10.0f + 0.1f;
			float v = heights_value;
			float o = opacity * engine.app.getIFps() * 8.0f;
			if(mode == MODE_HEIGHTS_ADDITION) o /= 20.0f;
			if(mode == MODE_HEIGHTS_SUBTRACTION) o /= 20.0f;
			int size_x = terrain.getSizeX();
			
			// brush bounds
			int x0 = clamp(int(floor(X - radius)),0,terrain.getSizeX());
			int y0 = clamp(int(floor(Y - radius)),0,terrain.getSizeY());
			int x1 = clamp(int(ceil(X + radius)),0,terrain.getSizeX());
			int y1 = clamp(int(ceil(Y + radius)),0,terrain.getSizeY());
			
			int surface_x0 = clamp(int(floor((X - radius) / TERRAIN_SURFACE_STEP)),0,terrain.getSurfacesX());
			int surface_y0 = clamp(int(floor((Y - radius) / TERRAIN_SURFACE_STEP)),0,terrain.getSurfacesY());
			int surface_x1 = clamp(int(ceil((X + radius) / TERRAIN_SURFACE_STEP)),0,terrain.getSurfacesX());
			int surface_y1 = clamp(int(ceil((Y + radius) / TERRAIN_SURFACE_STEP)),0,terrain.getSurfacesY());
			
			min_surface_x0 = min(min_surface_x0,surface_x0);
			min_surface_y0 = min(min_surface_y0,surface_y0);
			max_surface_x1 = max(max_surface_x1,surface_x1);
			max_surface_y1 = max(max_surface_y1,surface_y1);
			
			int width = x1 - x0;
			int height = y1 - y0;
			
			// smooth filter
			if(mode == MODE_HEIGHTS_SMOOTH) {
				
				filter.resize(width * height);
				float buffer[max(width,height) + 4];
				
				// get heights
				forloop(int y = 0; height) {
					forloop(int x = 0; width) {
						filter[width * y + x] = terrain.getHeight(x0 + x,y0 + y);
					}
				}
				
				// horizontal blur
				forloop(int y = 0; height) {
					int offset = width * y;
					forloop(int x = -2; width + 2) {
						buffer[x + 2] = filter[offset + clamp(x,0,width - 1)];
					}
					forloop(int x = 0; width) {
						filter[offset + x] = (buffer[x] + buffer[x + 1] + buffer[x + 2] + buffer[x + 3] + buffer[x + 4]) / 5.0f;
					}
				}
				
				// vertical blur
				forloop(int x = 0; width) {
					forloop(int y = -2; height + 2) {
						buffer[y + 2] = filter[width * clamp(y,0,height - 1) + x];
					}
					forloop(int y = 0; height) {
						filter[width * y + x] = (buffer[y] + buffer[y + 1] + buffer[y + 2] + buffer[y + 3] + buffer[y + 4]) / 5.0f;
					}
				}
			}
			// dilate filter
			else if(mode == MODE_HEIGHTS_DILATE) {
				
				filter.resize(width * height);
				int size_x_minus_two = terrain.getSizeX() - 2;
				int size_y_minus_two = terrain.getSizeY() - 2;
				
				// get heights
				forloop(int y = y0; y1) {
					int yn = max(y - 1,0);
					int yp = min(y + 1,size_y_minus_two);
					forloop(int x = x0; x1) {
						int xn = max(x - 1,0);
						int xp = min(x + 1,size_x_minus_two);
						float h = terrain.getHeight(x,y);
						h = max(h,terrain.getHeight(x,yn));
						h = max(h,terrain.getHeight(x,yp));
						h = max(h,terrain.getHeight(xn,y));
						h = max(h,terrain.getHeight(xp,y));
						h = max(h,terrain.getHeight(xn,yn));
						h = max(h,terrain.getHeight(xp,yn));
						h = max(h,terrain.getHeight(xn,yp));
						h = max(h,terrain.getHeight(xp,yp));
						filter[width * (y - y0) + x - x0] = h;
					}
				}
			}
			// erode filter
			else if(mode == MODE_HEIGHTS_ERODE) {
				
				filter.resize(width * height);
				int size_x_minus_two = terrain.getSizeX() - 2;
				int size_y_minus_two = terrain.getSizeY() - 2;
				
				// get heights
				forloop(int y = y0; y1) {
					int yn = max(y - 1,0);
					int yp = min(y + 1,size_y_minus_two);
					forloop(int x = x0; x1) {
						int xn = max(x - 1,0);
						int xp = min(x + 1,size_x_minus_two);
						float h = terrain.getHeight(x,y);
						h = min(h,terrain.getHeight(x,yn));
						h = min(h,terrain.getHeight(x,yp));
						h = min(h,terrain.getHeight(xn,y));
						h = min(h,terrain.getHeight(xp,y));
						h = min(h,terrain.getHeight(xn,yn));
						h = min(h,terrain.getHeight(xp,yn));
						h = min(h,terrain.getHeight(xn,yp));
						h = min(h,terrain.getHeight(xp,yp));
						filter[width * (y - y0) + x - x0] = h;
					}
				}
			}
			
			// brush
			float r = 0.0f;
			float height_limit_min = min_height;
			float height_limit_max = max_height;
			
			forloop(int y = y0; y1) {
				float dy = abs(y - Y);
				float tex_y = float(y - y0) / float(height);
				
				forloop(int x = x0; x1) {
					float dx = abs(x - X);
					float tex_x = float(x - x0) / float(width);
					
					if(check_limits(x,y) == 0) continue;
					
					r = calc_shape_radius(dx,dy,tex_x,tex_y,radius);
					if(r > radius) continue;
					
					float k = o * saturate(1.0f - pow(r / radius,p));
					float h = terrain.getHeight(x,y);
					
					int id = size_x * y + x;
					
					if(heights.check(id) == 0) heights.append(id,h);
					
					switch(mode) {
						case MODE_HEIGHTS_NORMAL: h = lerp(h,v,k); break;
						case MODE_HEIGHTS_ADDITION: h += v * k; break;
						case MODE_HEIGHTS_SUBTRACTION: h -= v * k; break;
						case MODE_HEIGHTS_SMOOTH:
						case MODE_HEIGHTS_DILATE:
						case MODE_HEIGHTS_ERODE: h = lerp(h,filter[width * (y - y0) + x - x0],k); break;
					}
					
					h = clamp(h,height_limit_min,height_limit_max);
					
					terrain.setHeight(x,y,h);
				}
			}
			
			forloop(int y = surface_y0;surface_y1) {
				forloop(int x = surface_x0;surface_x1) {
					terrain.getNormals(TERRAIN_SURFACE_STEP * x,TERRAIN_SURFACE_STEP * y,TERRAIN_SURFACE_STEP,TERRAIN_SURFACE_STEP,normals);
					terrain.setSurfaceNormalTextureImage(x,y,normals);
				}
			}
		}
		
		void flush() {
			
			if(brush == -1) return;
			
			brush = -1;
			
			save_normals();
			
			filter.delete();
		}
		
		void save_normals() {
			
			string name = terrain.getNormalTextureName();
			if(strlen(name) == 0) return;
			
			if(normals.info(terrain.getSurfaceNormalTextureName(0,0)) == 0) return;
			int s_w = normals.getHeight();
			int s_h = normals.getWidth();
			
			
			Image coarse_normals = new Image();
			if(coarse_normals.load(name) == 0) return;
			int h = coarse_normals.getHeight();
			int w = (h * terrain.getSurfacesY()) / terrain.getSurfacesX();
			coarse_normals.resize(w,h);
			
			for(int y = min_surface_y0; y < max_surface_y1; y++) {
				for(int x = min_surface_x0; x < max_surface_x1; x++) {
					
					string s_name = terrain.getSurfaceNormalTextureName(x,y);
					if(strlen(s_name) == 0) continue;
					
					terrain.getNormals(TERRAIN_SURFACE_STEP * x,TERRAIN_SURFACE_STEP * y,TERRAIN_SURFACE_STEP,TERRAIN_SURFACE_STEP,normals);
					
					normals.resize(s_w,s_h);
					normals.save(s_name);
					
					normals.resize(w / terrain.getSurfacesX(),h / terrain.getSurfacesY());
					coarse_normals.copy(normals,x * normals.getWidth(),y * normals.getHeight(),0,0,normals.getWidth(),normals.getHeight(),1);
					normals.clear();
				}
			}
			
			coarse_normals.save(name);
			delete coarse_normals;
		}
		
		
		void reset() {
			
			int size_x = terrain.getSizeX();
			
			// restore heights changes
			foreachkey(int id; heights) {
				int x = id % size_x;
				int y = id / size_x;
				terrain.setHeight(x,y,heights[id]);
			}
			heights.clear();
			
			save_normals();
		}
	};
	
	/******************************************************************************\
	*
	* BrushToolHoles
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushToolHoles : BrushToolTerrain {
		
		int holes[];
		
		BrushToolHoles() {
			brush = BRUSH_HOLES;
		}
		~BrushToolHoles() {
			holes.delete();
		}
		
		void update(float X,float Y) {
			
			// brush parameters
			float p = power * 10.0f + 0.1f;
			int size_x = terrain.getSizeX();
			
			// brush bounds
			int x0 = clamp(int(floor(X - radius)),0,terrain.getSizeX());
			int y0 = clamp(int(floor(Y - radius)),0,terrain.getSizeY());
			int x1 = clamp(int(ceil(X + radius)),0,terrain.getSizeX());
			int y1 = clamp(int(ceil(Y + radius)),0,terrain.getSizeY());
			
			// brush
			float r = 0.0f;
			forloop(int y = y0; y1) {
				float dy = abs(y - Y);
				float tex_y = float(y - y0) / float(y1 - y0);
				
				forloop(int x = x0; x1) {
					float dx = abs(x - X);
					float tex_x = float(x - x0) / float(x1 - x0);
					
					if(check_limits(x,y) == 0) continue;
					
					r = calc_shape_radius(dx,dy,tex_x,tex_y,radius);
					if(r > radius) continue;
					
					float k = saturate(1.0f - pow(r / radius,p));
					if(k < 0.5f) continue;
					
					int id = size_x * y + x;
					
					if(holes.check(id) == 0) holes.append(id,terrain.getHole(x,y));
					terrain.setHole(x,y,!mode);
				}
			}
		}
		
		void flush() { brush = -1; }
		
		void reset() {
			
			int size_x = terrain.getSizeX();
			
			// restore holes changes
			foreachkey(int id; holes) {
				int x = id % size_x;
				int y = id / size_x;
				terrain.setHole(x,y,holes[id]);
			}
			holes.clear();
		}
	};
	
	/******************************************************************************\
	*
	* BrushToolDiffuse
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushToolDiffuse : BrushToolTerrain {
		
		class Surface {
			
			int surface_x,surface_y;
			
			Image diffuse;
			Image surface;
			
			int width;
			int height;
			
			Surface(int surface_x_,int surface_y_,string name) {
				surface_x = surface_x_;
				surface_y = surface_y_;
				
				diffuse = new Image();
				surface = new Image();
				
				// load diffuse texture
				if(diffuse.load(name) == 0) return;
				
				// decompress diffuse
				if(diffuse.isZCompressedFormat()) diffuse.decompress();
				
				// copy surface
				width = diffuse.getWidth();
				height = diffuse.getHeight();
				surface.create2D(width,height,diffuse.getFormat());
				surface.copy(diffuse,0,0,0,0,width,height);
				
				// decompress surface
				if(surface.isCompressedFormat()) surface.decompress();
			}
			~Surface() {
				delete diffuse;
				delete surface;
			}
			
			void update(float X,float Y) {
				
				// check diffuse texture
				if(diffuse.isLoaded() == 0) return;
				
				// check surface format
				if(surface.getFormat() != IMAGE_FORMAT_RGBA8) surface.convertToFormat(IMAGE_FORMAT_RGBA8);
				
				// scale coordinates
				float scale_x = float(width) / TERRAIN_SURFACE_STEP;
				float scale_y = float(height) / TERRAIN_SURFACE_STEP;
				
				float radius_x = radius * scale_x;
				float radius_y = radius * scale_y;
				X = X * scale_x;
				Y = Y * scale_y;
				
				int is_rgba = true;
				if(mode == MODE_DIFFUSE_ADDITION || mode == MODE_DIFFUSE_SUBTRACTION) is_rgba = false;
				
				Image br = get_brush_image(radius_x * 2,radius_y * 2,diffuse_value,is_rgba,scale_x,scale_y);
				if(br == NULL) return;
				
				if(mode == MODE_DIFFUSE_NORMAL) {
					surface.blend(br,X - br.getWidth() / 2,Y - br.getWidth() / 2,0,0,br.getWidth(),br.getHeight(),1.0f,1);
				} else if(mode == MODE_DIFFUSE_ADDITION || mode == MODE_DIFFUSE_SUBTRACTION) {
					Image alpha = new Image();
					// copy alpha
					alpha.create2D(surface.getWidth(),surface.getHeight(),IMAGE_FORMAT_R8);
					alpha.copy(surface,0,3);
					
					surface.convertToFormat(IMAGE_FORMAT_RGB8);
					surface.blend(br,X - br.getWidth() / 2,Y - br.getWidth() / 2,0,0,br.getWidth(),br.getHeight(),(mode == MODE_DIFFUSE_ADDITION) ? 1.0f : -1.0f,1);
					
					surface.convertToFormat(IMAGE_FORMAT_RGBA8);
					surface.copy(alpha,3,0);
					
					delete alpha;
				} else if(mode == MODE_DIFFUSE_SMOOTH || MODE_DIFFUSE_MULTIPLY) {
					int offset = (mode == MODE_DIFFUSE_SMOOTH) ? 3 : 0; 
					
					// brush bounds
					int x0 = clamp(int(floor(X - radius_x - offset)),0,width);
					int y0 = clamp(int(floor(Y - radius_y - offset)),0,height);
					int x1 = clamp(int(ceil(X + radius_x + offset)),0,width);
					int y1 = clamp(int(ceil(Y + radius_y + offset)),0,height);
					
					int shift_x = radius_x + x0 - X;
					int shift_y = radius_y + y0 - Y;
					
					Image f = new Image();
					int w = x1 - x0;
					int h = y1 - y0;
					
					f.create2D(w,h,IMAGE_FORMAT_RGBA8);
					f.copy(surface,0,0,x0,y0,w,h);
					
					if(mode == MODE_DIFFUSE_SMOOTH) f.blur(3);
					
					Image brush = new Image();
					brush.create2D(radius_x * 2,radius_y * 2,IMAGE_FORMAT_RGBA8);
					brush.copy(f,shift_x,shift_y,0,0,w,h,1);
					
					if(mode == MODE_DIFFUSE_MULTIPLY) {
						forloop(int i = 0; brush.getWidth()) {
							forloop(int j = 0; brush.getHeight()) {
								vec4 c = brush.get2D(i,j) * vec4(diffuse_value);
								brush.set2D(i,j,c);
							}
						}
					}
					
					brush.copy(br,3,3);
					
					surface.blend(brush,X - br.getWidth() / 2,Y - br.getWidth() / 2,0,0,br.getWidth(),br.getHeight(),1.0f,1);
					delete f;
					delete brush;
				}
				
				// update surface diffuse texture
				terrain.setSurfaceDiffuseTextureImage(surface_x,surface_y,surface);
			}
			
			void flush(string name) {
				
				// check diffuse texture
				if(diffuse.isLoaded() == 0) return;
				
				// save diffuse texture
				surface.save(name);
				
				// update surface diffuse cache
				if(surface_diffuse_name == name) {
					surface_diffuse_cache.copy(surface);
				}
				
				log.message("Save surface diffuse texture into the \"%s\" file\n",name);
			}
			
			void reset(string name) {
				
				// check diffuse texture
				if(diffuse.isLoaded() == 0) return;
				
				// restore diffuse texture
				diffuse.save(name);
				
				// update surface diffuse cache
				if(surface_diffuse_name == name) {
					surface_diffuse_cache.copy(diffuse);
				}
				
				log.message("Save surface diffuse texture into the \"%s\" file\n",name);
			}
		};
		
		Surface surfaces[];
		
		Image coarse;
		Image coarse_new;
		
		string coarse_name;
		
		int surface_resolution;
		int coarse_resolution;
		
		BrushToolDiffuse() {
			brush = BRUSH_DIFFUSE;
			
			coarse_name = savePath(terrain.getDiffuseTextureName());
			coarse = new Image();
			if(is_file(coarse_name) && coarse.load(coarse_name)) {
				coarse_new = new Image(coarse);
				if(coarse_new.isCompressedFormat()) coarse_new.decompress();
				coarse_resolution = coarse.getHeight();
			}
			
			Image info_surface = new Image();
			info_surface.info(terrain.getSurfaceDiffuseTextureName(0,0));
			surface_resolution = info_surface.getHeight();
			delete info_surface;
		}
		~BrushToolDiffuse() {
			surfaces.delete();
			delete coarse;
			delete coarse_new;
		}
		
		void update(float X,float Y) {
			
			// surface bounds
			int surface_x0 = clamp(int(floor((X - radius) / TERRAIN_SURFACE_STEP)),0,terrain.getSurfacesX());
			int surface_y0 = clamp(int(floor((Y - radius) / TERRAIN_SURFACE_STEP)),0,terrain.getSurfacesY());
			int surface_x1 = clamp(int(ceil((X + radius) / TERRAIN_SURFACE_STEP)),0,terrain.getSurfacesX());
			int surface_y1 = clamp(int(ceil((Y + radius) / TERRAIN_SURFACE_STEP)),0,terrain.getSurfacesY());
			
			// update surface diffuse
			for(int y = surface_y0; y < surface_y1; y++) {
				for(int x = surface_x0; x < surface_x1; x++) {
					string name = terrain.getSurfaceDiffuseTextureName(x,y);
					if(strlen(name) == 0) continue;
					
					// get surface
					Surface surface = NULL;
					if(surfaces.check(name) == 0) {
						surface = new Surface(x,y,name);
						surfaces.append(name,surface);
					} else {
						surface = surfaces[name];
					}
					
					// update surface
					float surface_x = X - x * TERRAIN_SURFACE_STEP;
					float surface_y = Y - y * TERRAIN_SURFACE_STEP;
					surface.update(surface_x,surface_y);
				}
			}
		}
		
		void flush() {
			
			if(brush == -1) return;
			
			brush = -1;
			
			foreachkey(string name; surfaces) {
				Surface surface = surfaces[name];
				surface.flush(name);
				
				if(coarse_new == NULL) continue;
				
				Image temp =new Image(surface.surface);
				
				int height = coarse_resolution / terrain.getSurfacesY();
				int width = coarse_resolution / terrain.getSurfacesX();
				
				if(temp.isCompressedFormat()) temp.decompress();
				temp.resize(width,height);
				
				int x_dst = width * surface.surface_x;
				int y_dst = height * surface.surface_y;
				coarse_new.copy(temp,x_dst,y_dst,0,0,width,height);
				
				delete temp;
			}
			
			if(coarse_new != NULL) {
				coarse_new.save(coarse_name);
			}
		}
		
		void reset() {
			
			// restore surface diffuse
			foreachkey(string name; surfaces) {
				surfaces[name].reset(name);
			}
			
			if(coarse_new != NULL) {
				coarse.save(coarse_name);
				delete coarse;
				delete coarse_new;
			}
			
			surfaces.delete();
		}
	};
	
	/******************************************************************************\
	*
	* BrushToolMask
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushToolMask : BrushToolTerrain {
		
		class Surface {
			
			int surface_x,surface_y;
			
			Image mask;
			Image index;
			Image surface;
			Image filter;
			
			int width;
			int height;
			
			int changed;
			float change_x;
			float change_y;
			
			float scale_x;
			float scale_y;
			
			int x0,y0;
			int x1,y1;
			
			string mask_name;
			string index_name;
			
			Surface(int surface_x_,int surface_y_,string mask_name_,string index_name_) {
				
				surface_x = surface_x_;
				surface_y = surface_y_;
				
				mask_name = mask_name_;
				index_name = index_name_;
				
				mask = new Image();
				index = new Image();
				surface = new Image();
				
				// load mask texture
				if(mask.load(mask_name) == 0) return;
				
				// decompress mask
				if(mask.isZCompressedFormat()) mask.decompress();
				if(mask.isCompressedFormat()) mask.decompress();
				
				// load index texture
				if(index.load(index_name) == 0) return;
				
				mask.resize(surface_resolution,surface_resolution);
				index.resize(surface_resolution,surface_resolution,IMAGE_FILTER_POINT);
				
				// get texture parameters
				width = mask.getWidth();
				height = mask.getHeight();
				
				// cache brush parameters
				scale_x = float(width) / TERRAIN_SURFACE_STEP;
				scale_y = float(height) / TERRAIN_SURFACE_STEP;
				
				// create surface mask array
				surface.copy(mask);
				engine.editor.createTerrainMask(surface,index,terrain.getNumMaterials());
			}
			
			~Surface() {
				delete mask;
				delete index;
				delete surface;
				delete filter;
			}
			
			int isChanged() { return changed; }
			void setChanged(int value) { changed = value; }
			
			void prepare(int x,int y) {
				
				// check textures
				if(mask.isLoaded() == 0 || index.isLoaded() == 0 || mask_value == -1) return;
				
				// scale coordinates
				changed = 1;
				change_x = x * scale_x;
				change_y = y * scale_y;
				
				// brush parameters
				float radius_x = radius * scale_x;
				float radius_y = radius * scale_y;
				
				// brush bounds
				x0 = clamp(int(floor(change_x - radius_x)),0,width);
				y0 = clamp(int(floor(change_y - radius_y)),0,height);
				x1 = clamp(int(ceil(change_x + radius_x)),0,width);
				y1 = clamp(int(ceil(change_y + radius_y)),0,height);
				
				// smooth filter
				if(mode == MODE_MASK_SMOOTH) {
					if(filter == NULL) {
						filter = new Image();
						filter.create2D(surface.getHeight(),surface.getWidth(),surface.getFormat());
					}
					filter.copy(surface,mask_value);
					filter.blur(3);
				}
			}
			
			void fill() {
				
				if(changed == 0) return;
				
				// brush parameters
				float p = power * 10.0f + 0.1f;
				float r = 0.0f;
				float o = opacity * engine.app.getIFps() * 8.0f;
				
				float step = terrain.getStep();
				
				float inv_radius_x = 1.0f / (radius * scale_x);
				float inv_radius_y = 1.0f / (radius * scale_y);
				
				float inv_width = 1.0f / float(x1 - x0);
				float inv_height = 1.0f / float(y1 - y0);
				
				// fill mask
				forloop(int y = y0; y1) {
					float dy = abs((y - change_y) * inv_radius_y);
					float tex_y = float(y - y0) * inv_height;
					
					float terrain_y = y / scale_y + surface_y * TERRAIN_SURFACE_STEP;
					terrain_y *= step;
					
					forloop(int x = x0; x1) {
						float dx = abs((x - change_x) * inv_radius_x);
						float tex_x = float(x - x0) * inv_width;
						
						float terrain_x = x / scale_x + surface_x * TERRAIN_SURFACE_STEP;
						terrain_x *= step;
						
						if(check_limits(terrain_x,terrain_y) == 0) continue;
						r = calc_shape_radius(dx,dy,tex_x,tex_y);
						if(r > 1.0f) continue;
						
						float k = o * saturate(1.0f - pow(r,p));
						vec4 c = surface.get2DArray(x,y,mask_value);
						float v = c.r;
						
						switch(mode) {
							case MODE_MASK_NORMAL: v = lerp(v,1.0f,k); break;
							case MODE_MASK_ADDITION: v = saturate(v + k); break;
							case MODE_MASK_SUBTRACTION: v = saturate(v - k); break;
							case MODE_MASK_SMOOTH: { vec4 f = filter.get2D(x,y); v = lerp(v,f.r,k); } break;
						}
						surface.set2DArray(x,y,mask_value,v,0,0,0);
					}
				}
			}
			
			void reset() {
				
				// check textures
				if(mask.isLoaded() == 0 || index.isLoaded() == 0 || mask_value == -1) return;
				
				// restore mask texture
				mask.save(mask_name);
				index.save(index_name);
				
				// update surface mask cache
				if(surface_mask_name == mask_name) {
					surface_mask_cache.copy(mask);
				}
				
				// update surface index cache
				if(surface_index_name == index_name) {
					surface_index_cache.copy(index);
				}
				
				log.message("Save surface mask texture into the \"%s\" file\n",mask_name);
				log.message("Save surface index texture into the \"%s\" file\n",index_name);
			}
		};
		
		Surface surfaces[];
		
		Image coarse_array_mask;
		Image coarse_mask;
		Image coarse_index;
		
		string coarse_mask_name;
		string coarse_index_name;
		
		BrushToolMask() {
			coarse_mask_name = savePath(terrain.getMaskTextureName());
			coarse_index_name = savePath(terrain.getIndexTextureName());
			
			coarse_mask = new Image();
			coarse_index = new Image();
			
			if(coarse_mask.load(coarse_mask_name) && coarse_index.load(coarse_index_name)) {
				if(coarse_mask.isCompressedFormat()) coarse_mask.decompress();
				if(coarse_index.isCompressedFormat()) coarse_index.decompress();
				
				coarse_array_mask = new Image(coarse_mask);
				engine.editor.createTerrainMask(coarse_array_mask,coarse_index,terrain.getNumMaterials());
				coarse_array_mask.resize(coarse_resolution,coarse_resolution);
			}
			BrushAsync::setBrush(this);
			brush = BRUSH_MASK;
		}
		~BrushToolMask() {
			surfaces.delete();
		}
		
		void update(float X,float Y) {
			
			// check mask material
			if(mask_value < 0 || mask_value >= terrain.getNumMaterials()) return;
			
			// surface bounds
			int surface_x0 = clamp(int(floor((X - radius) / TERRAIN_SURFACE_STEP)),0,terrain.getSurfacesX());
			int surface_y0 = clamp(int(floor((Y - radius) / TERRAIN_SURFACE_STEP)),0,terrain.getSurfacesY());
			int surface_x1 = clamp(int(ceil((X + radius) / TERRAIN_SURFACE_STEP)),0,terrain.getSurfacesX());
			int surface_y1 = clamp(int(ceil((Y + radius) / TERRAIN_SURFACE_STEP)),0,terrain.getSurfacesY());
			
			// update surface mask
			for(int y = surface_y0; y < surface_y1; y++) {
				for(int x = surface_x0; x < surface_x1; x++) {
					string mask_name = terrain.getSurfaceMaskTextureName(x,y);
					string index_name = terrain.getSurfaceIndexTextureName(x,y);
					
					if(strlen(mask_name) == 0) continue;
					if(strlen(index_name) == 0) continue;
					
					// get surface
					Surface surface = NULL;
					int id = y * terrain.getSurfacesX() + x;
					
					if(surfaces.check(id) == 0) {
						surface = new Surface(x,y,mask_name,index_name);
						surfaces.append(id,surface);
					} else {
						surface = surfaces[id];
					}
					
					// change surface
					float surface_x = X - x * TERRAIN_SURFACE_STEP;
					float surface_y = Y - y * TERRAIN_SURFACE_STEP;
					
					surface.prepare(surface_x,surface_y);
				}
			}
		}
		
		void flush() {
			
			if(brush == -1) return;
			brush = -1;
			
			BrushAsync::flush();
		}
		
		void reset() {
			
			// restore surface mask
			surfaces.call("reset");
			
			if(coarse_array_mask != NULL) {
				coarse_mask.save(coarse_mask_name);
				coarse_index.save(coarse_index_name);
			}
			
			delete coarse_mask;
			delete coarse_index;
			delete coarse_array_mask;
			
			surfaces.delete();
		}
	};
	
	namespace BrushAsync {
		
		/*
		 */
		BrushToolMask brush;
		int thread_id;
		int running;
		int flushing;
		
		/*
		 */
		void init() {
			brush = NULL;
			running = 0;
			flushing = 0;
			
			thread_id = thread(functionid(update));
		}
		
		void shutdown() {
			kill_thread(thread_id);
			
			brush = NULL;
			running = 0;
			flushing = 0;
		}
		
		/*
		 */
		void flush() {
			flushing = 1;
		}
		
		/*
		 */
		void update() {
			while(true) {
				if(brush != NULL) {
					running = 1;
					
					if(flushing) {
						foreach(BrushToolMask::Surface surface; brush.surfaces) {
							if(mask_value == -1) continue;
							
							if(surface.mask.isLoaded() == 0) continue;
							if(surface.index.isLoaded() == 0) continue;
							
							asyncRunNamedThread(functionid(save_mask),(surface.surface,surface.mask_name,surface.index_name),"brush");
							
							if(brush.coarse_array_mask == NULL) continue;
							
							Image temp = new Image(surface.surface);
							
							int height = coarse_resolution / terrain.getSurfacesY();
							int width = coarse_resolution / terrain.getSurfacesX();
							
							if(temp.isCompressedFormat()) temp.decompress();
							temp.resize(width,height);
							
							forloop(int i = 0; terrain.getNumMaterials()) {
								int x_dst = width * surface.surface_x;
								int y_dst = i * coarse_resolution + height * surface.surface_y;
								int x_src = 0;
								int y_src = i * height;
								brush.coarse_array_mask.copy(temp,x_dst,y_dst,x_src,y_src,width,height);
							}
							
							delete temp;
							
						}
						if(brush.coarse_array_mask != NULL) {
							asyncRunNamedThread(functionid(save_mask),(brush.coarse_array_mask,terrain.getMaskTextureName(),terrain.getIndexTextureName()),"brush");
						}
						
						asyncWaitThreads("brush");
						
						if(brush != NULL) {
							foreach(BrushToolMask::Surface surface; brush.surfaces) {
								delete surface.filter;
							}
						}
						
						
						flushing = 0;
					}
					else {
						brush.surfaces.call("fill");
						
						foreach(BrushToolMask::Surface surface; brush.surfaces) {
							if(surface.isChanged() == 0) continue;
							
							asyncRunNamedThread(functionid(draw_surface),(surface.surface,surface.surface_x,surface.surface_y,surface.mask_name,surface.index_name),"brush");
						}
						asyncWaitThreads("brush");
						
						if(brush != NULL) brush.surfaces.call("setChanged",0);
					}
					running = 0;
				}
				
				wait;
			}
		}
		
		/*
		 */
		void save_mask(Async async,Image mask,string mask_name,string index_name) {
			
			Image temp_index = class_manage(new Image());
			Image temp_mask = class_manage(new Image(mask));
			
			int id = async.run(functionid(engine.editor.createTerrainIndex),temp_mask,temp_index,16);
			while(async.isRunning(id)) wait;
			
			if(async.getResult(id) == 0) return;
			
			async.run(temp_mask,"save",mask_name);
			async.run(temp_index,"save",index_name);
			while(async.isRunning()) wait;
			
			log.message("Save surface mask texture into the \"%s\" file\n",mask_name);
			log.message("Save surface index texture into the \"%s\" file\n",index_name);
		}
		
		void draw_surface(Async async,Image mask,int surface_x,int surface_y,string mask_name,string index_name) {
			
			Image temp_index = class_manage(new Image());
			Image temp_mask = class_manage(new Image(mask));
			
			int id = async.run(functionid(engine.editor.createTerrainIndex),temp_mask,temp_index,16);
			while(async.isRunning(id)) wait;
			if(async.getResult(id) == 0) return;
			
			terrain.setSurfaceMaskTextureImage(surface_x,surface_y,temp_mask);
			terrain.setSurfaceIndexTextureImage(surface_x,surface_y,temp_index);
		}
		
		/*
		 */
		void setBrush(BrushToolMask brush_) { brush = brush_; }
		
		/*
		 */
		int isRunning() { return running || flushing; }
	}
	
	/******************************************************************************\
	*
	* BrushToolMaterial
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushToolMaterial : BrushToolTerrain {
		
		BrushToolDiffuse brush_diffuse;
		BrushToolMask brush_mask;
		
		BrushToolMaterial() {
			brush = BRUSH_MATERIAL;
			brush_diffuse = new BrushToolDiffuse();
			brush_mask = new BrushToolMask();
		}
		~BrushToolMaterial() {
			delete brush_diffuse;
			delete brush_mask;
		}
		
		void update(float X,float Y) {
			brush_diffuse.update(X,Y);
			brush_mask.update(X,Y);
		}
		
		void flush() {
			brush = -1;
			brush_diffuse.flush();
			brush_mask.flush();
		}
		
		void reset() {
			brush_diffuse.reset();
			brush_mask.reset();
		}
	};
	
	/******************************************************************************\
	*
	* Update
	*
	\******************************************************************************/
	
	/*
	 */
	void enable(Node n,int surface_resolution_,int coarse_resolution_) {
		surface_resolution = surface_resolution_;
		coarse_resolution = coarse_resolution_;
		is_terrain = true;
		panel_brush = panelsGetPanelBrush();
		node = node_cast(n);
		
		terrain = node;
		assert(terrain.isLoaded());
		
		panel_brush.update_materials(terrain);
		
		brush = configGet("terrain_brush",BRUSH_DIFFUSE);
		brush = getSafeBrush(brush);
		
		brush_settings_heights.configGet();
		brush_settings_holes.configGet();
		brush_settings_diffuse.configGet();
		brush_settings_mask.configGet();
		brush_settings_material.configGet();
		
		heights_value_changed(configGet("terrain_heights_value",heights_value));
		diffuse_value_changed(configGet("terrain_diffuse_value",diffuse_value));
		mask_value_changed(configGet("terrain_mask_value",mask_value));
		
		terrain_limits = new TerrainLimits();
		
		brush_decal_terrain = new BrushDecalTerrain();
		brush_decal = brush_decal_terrain;
		
		brush_palette = new BrushPaletteTerrain();
		
		get_terrain_limits();
		
		BrushAsync::init();
		
		brush_changed(brush);
		
		Brushes::enable();
	}
	
	void disable(int reset = 0) {
		
		configSet("terrain_brush",brush);
		configSet("terrain_heights_value",heights_value);
		configSet("terrain_diffuse_value",diffuse_value);
		configSet("terrain_mask_value",mask_value);
		configSet("terrain_min_angle",min_angle);
		configSet("terrain_max_angle",max_angle);
		
		brush_settings_heights.configSet();
		brush_settings_holes.configSet();
		brush_settings_diffuse.configSet();
		brush_settings_mask.configSet();
		brush_settings_material.configSet();
		
		
		BrushAsync::shutdown();
		
		if(engine.world.isNode(terrain) && !reset) {
			terrain.saveTerrain(savePath(systemEditorPath(terrain.getTerrainName())));
			log.message("Save terrain \"%s\" file\n",terrain.getTerrainName());
		}
		
		surface_diffuse_name = "";
		delete surface_diffuse_cache;
		surface_diffuse_cache = NULL;
		
		surface_mask_name = "";
		delete surface_mask_cache;
		surface_mask_cache = NULL;
		
		surface_index_name = "";
		delete surface_index_cache;
		surface_index_cache = NULL;
		
		terrain = NULL;
		
		delete terrain_limits;
		terrain_limits = NULL;
	}
	
	/*
	 */
	void get_terrain_limits() {
		
		// angle
		min_angle_changed(configGet("terrain_min_angle",min_angle));
		max_angle_changed(configGet("terrain_max_angle",max_angle));
		
		// height
		BoundBox box = terrain.getBoundBox();
		
		vec3 box_min = box.getMin();
		vec3 box_max = box.getMax();
		
		min_height_changed(min(box_min.z,TERRAIN_MIN_HEIGHT));
		max_height_changed(max(box_max.z,TERRAIN_MAX_HEIGHT));
		
		panel_brush.set_limits(box_min.z,box_max.z);
	}
	
	/*
	 */
	int get_intersection_point(vec3 &ip,float &x,float &y) {
		
		vec3 p0,p1;
		Unigine::getPlayerMouseDirection(p0,p1);
		p0 = terrain.getIWorldTransform() * p0;
		p1 = terrain.getIWorldTransform() * p1;
		
		ObjectIntersectionTexCoord intersection = new ObjectIntersectionTexCoord();
		if(terrain.getIntersection(p0,p1,intersection,0,0) == NULL) return 0;
		
		ip = intersection.getPoint();
		
		vec4 texcoord = intersection.getTexCoord();
		x = terrain.getSizeX() * texcoord.x;
		y = terrain.getSizeY() * texcoord.y;
		
		return 1;
	}
	
	/*
	 */
	BrushToolBase new_brush_tools() {
		if(brush == BRUSH_HEIGHTS) return new BrushToolHeights();
		else if(brush == BRUSH_HOLES) return new BrushToolHoles();
		else if(brush == BRUSH_DIFFUSE) return new BrushToolDiffuse();
		
		if(terrain.getNumMaterials() == 0) return NULL;
		else if(brush == BRUSH_MASK) return new BrushToolMask();
		else if(brush == BRUSH_MATERIAL) return new BrushToolMaterial();
		
		return NULL;
	}
	
	/*
	 */
	void grab_brush_value(float x,float y) {
		
		// grab height value
		if(brush == BRUSH_HEIGHTS) {
			float step = terrain.getStep();
			heights_value_changed(terrain.getHeight(step * x,step * y));
		}
		
		// grab diffuse color
		if(brush == BRUSH_DIFFUSE || brush == BRUSH_MATERIAL) {
			
			int surface_x = clamp(int(x / TERRAIN_SURFACE_STEP),0,terrain.getSurfacesX() - 1);
			int surface_y = clamp(int(y / TERRAIN_SURFACE_STEP),0,terrain.getSurfacesY() - 1);
			string name = terrain.getSurfaceDiffuseTextureName(surface_x,surface_y);
			
			if(strlen(name)) {
				
				// load surface diffuse texture
				if(surface_diffuse_name != name) {
					surface_diffuse_name = name;
					delete surface_diffuse_cache;
					surface_diffuse_cache = new Image();
					if(surface_diffuse_cache.load(surface_diffuse_name)) {
						if(surface_diffuse_cache.isZCompressedFormat()) surface_diffuse_cache.decompress();
						if(surface_diffuse_cache.isCompressedFormat()) surface_diffuse_cache.decompress();
					}
				}
				
				if(surface_diffuse_cache.isLoaded() && surface_diffuse_cache.getType() == IMAGE_2D) {
					float texcoord_x = (x - surface_x * TERRAIN_SURFACE_STEP) / TERRAIN_SURFACE_STEP;
					float texcoord_y = (y - surface_y * TERRAIN_SURFACE_STEP) / TERRAIN_SURFACE_STEP;
					diffuse_value.xyz = vec3(surface_diffuse_cache.get2D(texcoord_x,texcoord_y));
					panel_brush.set_diffuse_value(diffuse_value);
				}
			}
		}
		
		// grab mask material
		if(brush == BRUSH_MASK || brush == BRUSH_MATERIAL) {
			
			int surface_x = clamp(int(x / TERRAIN_SURFACE_STEP),0,terrain.getSurfacesX() - 1);
			int surface_y = clamp(int(y / TERRAIN_SURFACE_STEP),0,terrain.getSurfacesY() - 1);
			
			string index_name = terrain.getSurfaceIndexTextureName(surface_x,surface_y);
			string mask_name = terrain.getSurfaceMaskTextureName(surface_x,surface_y);
			
			if(strlen(index_name) && strlen(mask_name)) {
				if(surface_index_name != index_name) {
					surface_index_name = index_name;
					delete surface_index_cache;
					surface_index_cache = new Image();
					
					if(surface_index_cache.load(surface_index_name)) {
						if(surface_index_cache.isZCompressedFormat()) surface_index_cache.decompress();
						if(surface_index_cache.isCompressedFormat()) surface_index_cache.decompress();
					}
				}
				
				if(surface_mask_name != mask_name) {
					surface_mask_name = mask_name;
					delete surface_mask_cache;
					surface_mask_cache = new Image();
					
					if(surface_mask_cache.load(surface_mask_name)) {
						if(surface_mask_cache.isZCompressedFormat()) surface_mask_cache.decompress();
						if(surface_mask_cache.isCompressedFormat()) surface_mask_cache.decompress();
					}
				}
				
				if(surface_index_cache.isLoaded() && surface_mask_cache.isLoaded()) {
					float texcoord_x = (x - surface_x * TERRAIN_SURFACE_STEP) / TERRAIN_SURFACE_STEP;
					float texcoord_y = (y - surface_y * TERRAIN_SURFACE_STEP) / TERRAIN_SURFACE_STEP;
					
					vec4 index = surface_index_cache.get2D(texcoord_x,texcoord_y);
					vec4 mask = surface_mask_cache.get2D(texcoord_x,texcoord_y);
					
					float current_index = index[0];
					float current_mask = mask[0];
					
					forloop(int i = 0; 4) {
						if(mask[i] < current_mask) continue;
						
						current_index = index[i];
						current_mask = mask[i];
					}
					
					int material_index = int(floor(current_index * 255.0f));
					
					if(material_index >= 0 && material_index < terrain.getNumMaterials()) {
						mask_value_changed(material_index);
					}
				}
			}
		}
	}
	
	/*
	 */
	void update() {
		terrain_limits.update(engine.app.getIFps());
		brush_decal_terrain.setEnabled(1);
	}
	
	/*
	 */
	void setBrushEnabled(int val) {
		brush_decal_terrain.setEnabled(val);
	}
	
	/*
	 */
	int hasMaterials() {
		return (terrain.getNumMaterials() != 0);
	}
	
	/*
	 */
	int getSafeBrush(int brush) {
		if((brush == BRUSH_MASK || brush == BRUSH_MATERIAL) && (!hasMaterials())) {
			brush = BRUSH_DIFFUSE;
		}
		
		return brush;
	}
	
	/*
	 */
	void brush_changed() {
		
		// save old brush
		if(old_brush != -1) {
			if(old_brush == BRUSH_HEIGHTS) brush_settings_heights.get();
			else if(old_brush == BRUSH_HOLES) brush_settings_holes.get();
			else if(old_brush == BRUSH_DIFFUSE) brush_settings_diffuse.get();
			else if(old_brush == BRUSH_MASK) brush_settings_mask.get();
			else if(old_brush == BRUSH_MATERIAL) brush_settings_material.get();
		}
		
		switch(brush) {
			case BRUSH_HEIGHTS:
				brush_settings_heights.set();
				heights_value_changed(heights_value);
				break;
				
			case BRUSH_HOLES:
				brush_settings_holes.set();
				break;
				
			case BRUSH_DIFFUSE:
				brush_settings_diffuse.set();
				panel_brush.set_diffuse_value(diffuse_value);
				break;
				
			case BRUSH_MASK:
				brush_settings_mask.set();
				mask_value_changed(mask_value);
				break;
				
			case BRUSH_MATERIAL:
				brush_settings_material.set();
				panel_brush.set_diffuse_value(diffuse_value);
				mask_value_changed(mask_value);
				break;
		}
	}
	
	/*
	 */
	void min_angle_changed(float new_min_angle) {
		min_angle = min(new_min_angle,max_angle);
		cos_min_angle = cos(min_angle * DEG2RAD);
		
		panel_brush.set_min_angle(min_angle);
	}
	
	void max_angle_changed(float new_max_angle) {
		max_angle = max(new_max_angle,min_angle);
		cos_max_angle = cos(max_angle * DEG2RAD);
		
		panel_brush.set_max_angle(max_angle);
	}
	
	/*
	 */
	void min_height_changed(float new_min_height) {
		min_height = min(new_min_height,max_height);
		panel_brush.set_min_height(min_height);
		if(terrain_limits != NULL) terrain_limits.updateHeights();
	}
	
	void max_height_changed(float new_max_height) {
		max_height = max(new_max_height,min_height);
		panel_brush.set_max_height(max_height);
		if(terrain_limits != NULL) terrain_limits.updateHeights();
	}
	
	/*
	 */
	void heights_value_changed(float new_heights_value) {
		heights_value = new_heights_value;
		panel_brush.set_heights_value(heights_value);
	}
	
	/*
	 */
	void diffuse_value_changed(vec3 new_diffuse_value) {
		diffuse_value = new_diffuse_value;
		panel_brush.set_diffuse_value(diffuse_value);
	}
	
	vec3 get_diffuse_value() {
		return diffuse_value;
	}
	
	/*
	 */
	string get_mask_value_text() {
		return get_mask_value_text(mask_value);
	}
	
	string get_mask_value_text(int mask_value) {
		if(terrain != NULL && mask_value >= 0 && mask_value < terrain.getNumMaterials()) {
			string name = terrain.getMaterialName(mask_value);
			if(strlen(name) == 0) name = format(TR("Material %d"),mask_value);
			return name;
		}
		return TR("Select");
	}
	
	void mask_value_changed(float new_mask_value) {
		mask_value = new_mask_value;
		panel_brush.set_mask_value(new_mask_value);
	}
	
	/*
	 */
	ObjectTerrain get_terrain() {
		return terrain
	}
	
	/*
	 */
	void shutdown() {
		delete surface_diffuse_cache;
		surface_diffuse_cache = NULL;
		
		delete surface_mask_cache;
		surface_mask_cache = NULL;
		
		delete surface_index_cache;
		surface_index_cache = NULL;
	}
	
}

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void terrainEnable(Node node,int surface_resolution,int coarse_resolution) {
	Brushes::Terrain::enable(node,surface_resolution,coarse_resolution);
}
