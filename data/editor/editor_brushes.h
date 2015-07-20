/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_brushes.h
 * Desc:    Unigine editor
 * Version: 1.15
 * Author:  Mikhail Lyubimov <alterego@unigine.com>
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
namespace Brushes {
	
	/*
	 */
	string brush_text[] = ( TR("Heights"), TR("Holes"), TR("Diffuse"), TR("Mask"), TR("Diffuse+Mask"), TR("Empty"));
	string shape_text[] = ( TR("Circle"), TR("Square"), TR("Diamond"), TR("Texture") );
	
	int brush;
	int old_brush = -1;
	
	int mode = 0;
	
	int shape = SHAPE_CIRCLE;
	Image shape_image;
	string shape_default_texture = "editor/gui/brush_circle.png";
	string shape_texture = shape_default_texture;
	
	Image diffuse_brush_image[];
	dvec4 last_diffuse_value;
	
	float radius = 10.0f;
	float power = 1.0f;
	float opacity = 0.5f;
	
	int grab;
	
	Editor::Panels::PanelBrush panel_brush;
	int is_terrain;
	int is_enabled;
	
	Node node;
	
	/******************************************************************************\
	*
	* BrushDecalBase
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushDecalBase {
		
		//
		Material material;
		
		//
		virtual void update(vec3 point) = 0;
		virtual void setRadius(float radius) = 0;
		
		// configure decal
		void setShape(int shape,string texture = "") {
			texture = savePath(texture);
			int diffuse_id = material.findTexture("diffuse");
			if(shape == SHAPE_CIRCLE) material.setImageTextureName(diffuse_id,"editor/gui/brush_circle.png");
			else if(shape == SHAPE_SQUARE) material.setImageTextureName(diffuse_id,"editor/gui/brush_square.png");
			else if(shape == SHAPE_DIAMOND) material.setImageTextureName(diffuse_id,"editor/gui/brush_diamond.png");
			else if(shape == SHAPE_TEXTURE && is_file(texture)) material.setImageTextureName(diffuse_id,texture);
		}
		
		void setPower(float power) {
			int diffuse_color_id = material.findParameter("diffuse_color");
			material.setParameter(diffuse_color_id,vec4(power + 0.5f));
		}
		
		void configure_material() {
			material.setBlendFunc(MATERIAL_BLEND_SRC_ALPHA,MATERIAL_BLEND_ONE_MINUS_SRC_ALPHA);
			material.setState(material.findState("deferred"),0);
			material.setState(material.findState("ambient"),2);
			material.setState(material.findState("light_spot"),0);
			material.setState(material.findState("light_omni"),0);
			material.setState(material.findState("light_proj"),0);
			material.setState(material.findState("light_world"),0);
			material.setState(material.findState("emission"),1);
		}
	};
	
	BrushDecalBase brush_decal;
	
	/******************************************************************************\
	*
	* BrushPaletteBase
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushPaletteBase {
		
		class SlotBase {
			int brush = BRUSH_EMPTY;
			int shape = 0;
			int mode = 0;
			string shape_texture = shape_default_texture;
			
			float radius = 10.0f;
			float power = 1.0f;
			float opacity = 0.5f;
			
			virtual string get_info() {
				string text = TR("Shape: ") + shape_text[shape] + "<br>";
				
				if(shape == SHAPE_TEXTURE) text += TR("Shape texture: ") + shape_texture + "<br>";
				
				text += TR("Mode: ") + panel_brush.get_mode_text(mode,brush) + "<br>";
				
				text += TR("Radius: ") + editorFormat(radius) + "<br>";
				text += TR("Power: ") + editorFormat(power) + "<br>";
				text += TR("Opacity: ") + editorFormat(opacity) + "<br>";
				
				return text;
			}
			
			virtual string get_config_value() {
				string value = format("%d",brush);
				value += format(" %d",shape);
				value += format(" %d",mode);
				value += format(" %f",radius);
				value += format(" %f",power);
				value += format(" %f",opacity);
				value += format(" %s",shape_texture);
				return value;
			}
			
			virtual void config_apply(int value[]) {
				brush = value[0];
				shape = value[1];
				mode = value[2];
				radius = value[3];
				power = value[4];
				opacity = value[5];
				shape_texture = value[6];
			}
			
		};
		
		SlotBase slots[0];
		
		BrushPaletteBase() {
			forloop(int i = 0; 8) slots.append(new_slot());
			
			configGet();
			update();
		}
		
		~BrushPaletteBase() {
			configSet();
			slots.delete();
		}
		
		SlotBase new_slot() {
		}
		
		void update_slot(int num) {
			SlotBase slot = slots[num];
			panel_brush.update_slot(num,slot.brush,slot.get_info());
		}
		
		void update() {
			forloop(int i = 0; slots.size()) {
				update_slot(i);
			}
		}
		
		// set/get/copy palette
		void set(int num) {
			SlotBase slot = slots[num];
			
			brush_changed(slot.brush);
			shape_texture_changed(slot.shape_texture);
			shape_changed(slot.shape);
			mode_changed(slot.mode);
			radius_changed(slot.radius);
			power_changed(slot.power);
			opacity_changed(slot.opacity);
		}
		
		void get(int num) {
			SlotBase slot = slots[num];
			
			slot.brush = brush;
			slot.shape = shape;
			slot.mode = mode;
			
			slot.radius = radius;
			slot.power = power;
			slot.opacity = opacity;
			
			slot.shape_texture = shape_texture;
			
			update_slot(num);
		}
		
		void swap(int out_num,int in_num) {
			SlotBase slot = slots[in_num];
			slots[in_num] = slots[out_num];
			slots[out_num] = slot;
			update_slot(in_num);
			update_slot(out_num);
		}
		
		// set/get config
		string get_config_format() {
			return "%d %d %d %f %f %f %s";
		}
		
		string get_config_name(int num) {
			return format("palette_%d",num);
		}
		
		void configSet() {
			forloop(int i = 0; slots.size()) {
				::configSet(get_config_name(i),slots[i].get_config_value());
			}
		}
		
		void configGet() {
			forloop(int i = 0; slots.size()) {
				SlotBase slot = slots[i];
				string config = ::configGet(get_config_name(i),slot.get_config_value());
				
				int ret[0];
				sscanf(config,get_config_format(),ret);
				
				slot.config_apply(ret);
			}
		}
	};
	
	BrushPaletteBase brush_palette;
	
	/******************************************************************************\
	*
	* BrushToolBase
	*
	\******************************************************************************/
	
	/*
	 */
	class BrushToolBase {
			
		private:
			
			/*
			 */
			float calc_shape_radius(float dx,float dy,float tex_x,float tex_y,float radius_ = 1.0f) {
				switch(shape) {
					case SHAPE_CIRCLE: return sqrt(dx * dx + dy * dy);
					case SHAPE_SQUARE: return max(dx,dy);
					case SHAPE_DIAMOND: return dx + dy;
					case SHAPE_TEXTURE:
						if(shape_image == NULL) return INFINITY;
						vec4 color = shape_image.get2D(tex_x,tex_y);
						return radius_ * (1.0f - color.x);
				}
				return 0.0f;
			}
			
		public:
			
			int brush;
			
			void update(float X,float Y) = 0;
			void flush() = 0;
			void reset() = 0;
	};
	
	BrushToolBase brush_tools[0];
	
	/*
	 */
	float calc_shape_value(float dx,float dy,float tex_x,float tex_y,float radius_ = 1.0f) {
		switch(shape) {
			case SHAPE_CIRCLE: return sqrt(dx * dx + dy * dy);
			case SHAPE_SQUARE: return max(dx,dy);
			case SHAPE_DIAMOND: return dx + dy;
			case SHAPE_TEXTURE:
				if(shape_image == NULL) return INFINITY;
				vec4 color = shape_image.get2D(tex_x,tex_y);
				return radius_ * (1.0f - color.x);
		}
		return 0.0f;
	}
	
	void brush_changed(int new_brush) {
		
		brush = new_brush;
		if(is_terrain) {
			int safe_brush = Terrain::getSafeBrush(brush);
			if(safe_brush != brush) {
				dialogMessage(TR("Warning"),TR("No terrain layer materials found. Please, create at least one material at Layers tab."));
				brush = safe_brush;
			}
			
			Terrain::brush_changed();
		}
		panel_brush.set_brush(brush);
		old_brush = brush;
		diffuse_brush_image.delete();
	}
	
	Image get_brush_image(int width,int height,dvec4 value,int is_rgba,float sx,float sy) {
		if(last_diffuse_value != value) diffuse_brush_image.delete();
		last_diffuse_value = value;
		
		if(radius < 1) return NULL;
		
		string key = string(width) + "x" + string(height);
		Image res = diffuse_brush_image.check(key,NULL);
		
		if(res != NULL) return res;
		res = new Image();
		diffuse_brush_image.append(key,res);
		if(is_rgba) {
			res.create2D(width,height,IMAGE_FORMAT_RGBA8);
		} else {
			res.create2D(width,height,IMAGE_FORMAT_RGB8);
		}
		
		float iwidth = 1.0f / width;
		float iheight = 1.0f / height;
		float iradius_x = 1.0f / radius / sx;
		float iradius_y = 1.0f / radius / sy;
		
		forloop(int x = 0; width) {
			float x0 = abs(x - width / 2.0f) * iradius_x;
			float x1 = float(x) * iwidth;
			forloop(int y = 0; height) {
				float y0 = abs(y - height / 2.0f) * iradius_y;
				float y1 = float(y) * iheight;
				float f = calc_shape_value(x0,y0,x1,y1);
				float a = opacity * saturate(1.0f - pow(f,power));
				if(is_rgba) res.set2D(x,y,vec4(vec3(value),a));
				else res.set2D(x,y,vec4(vec3(value),1.0f) * a);
			}
		}
		
		return res;
	}
	
	/*
	 */
	void shape_changed(int new_shape) {
		if(new_shape == SHAPE_TEXTURE && !is_file(savePath(shape_texture))) shape_texture = shape_default_texture;
		
		shape = new_shape;
		panel_brush.set_shape(shape);
		
		if(brush_decal != NULL) brush_decal.setShape(shape,shape_texture);
		diffuse_brush_image.delete();
	}
	
	void shape_texture_changed(string new_shape_texture) {
		
		string old_shape_texture = shape_texture;
		
		if(is_file(savePath(new_shape_texture))) shape_texture = new_shape_texture;
		else shape_texture = shape_default_texture;
		
		shape_image = new Image(shape_texture);
		
		if(shape_image.getType() != IMAGE_2D) {
			dialogMessageOk(TR("Error"),format(TR("Unsupported texture type (%s). Only 2D textures with unsigned texture format can be used."),shape_image.getTypeName()));
			shape_texture = old_shape_texture;
			shape_image.load(shape_texture);
			return;
		} else if(shape_image.isFloatFormat() || shape_image.isHalfFormat()) {
			dialogMessageOk(TR("Error"),format(TR("Unsupported texture format (%s). Only 2D textures with unsigned texture format can be used."),shape_image.getFormatName()));
			shape_texture = old_shape_texture;
			shape_image.load(shape_texture);
			return;
		}
	
		if(shape_image.isCombinedFormat()) shape_image.decombine();
		if(shape_image.isCompressedFormat()) shape_image.decompress();
		
		if(shape_image != NULL && shape_image.getNumChannels() == 4) {
			shape_image.copy(shape_image,0,3);
		}
		panel_brush.shape_texture_changed(shape_texture);
		
		if(brush_decal != NULL) brush_decal.setShape(shape,shape_texture);
		diffuse_brush_image.delete();
	}
	
	string getShapeTexture() {
		if(!is_file(savePath(shape_texture)) || shape_texture == shape_default_texture) return "";
		return shape_texture;
	}
	
	void mode_changed(int new_mode) {
		mode = new_mode;
		panel_brush.set_mode(mode);
		diffuse_brush_image.delete();
	}
	
	/*
	 */
	void radius_changed(float new_radius) {
		radius = new_radius;
		panel_brush.set_radius(radius);
		if(brush_decal != NULL) brush_decal.setRadius(radius);
		diffuse_brush_image.delete();
	}
	
	void power_changed(float new_power) {
		power = new_power;
		panel_brush.set_power(power);
		if(brush_decal != NULL) brush_decal.setPower(power);
		diffuse_brush_image.delete();
	}
	
	void opacity_changed(float new_opacity) {
		opacity = new_opacity;
		panel_brush.set_opacity(opacity);
		diffuse_brush_image.delete();
	}
	
	/*
	 */
	void slot_set(int num) {
		brush_palette.set(num);
	}
	
	void slot_get(int num) {
		brush_palette.get(num);
	}
	
	void slot_swap(int out_num,int in_num) {
		brush_palette.swap(out_num,in_num);
	}
	
	/*
	 */
	void apply() {
		unredoRemoveBrushEditStates();
		disable();
	}
	
	void cancel() {
		unredoRemoveBrushEditStates();
		disable(1);
	}
	
	void enable() {
		if(is_enabled) return;
		
		is_enabled = true;
		
		brush_decal.setShape(shape,shape_texture);
		brush_decal.setRadius(radius);
		brush_decal.setPower(power);
		
		nodesHide();
		
		panel_brush.enable(is_terrain);
		
		hotkeysSetPriorityHotkeys((
			"brushes_apply_brush",
			"brushes_get_brush_value",
			"brushes_decrement_brush_radius",
			"brushes_increment_brush_radius"
		));
		
		unredoAddBrushEditState();
	}
	
	void disable(int reset = 0) {
		if(!is_enabled) return;
		
		is_enabled = false;
		
		if(reset) {
			for(int i = brush_tools.size() - 1; i >= 0; i--) {
				brush_tools[i].reset();
			}
		}
		nodesShow();
		
		panel_brush.disable();
		
		brush_tools.delete();
		
		delete brush_palette;
		
		delete brush_decal;
		brush_decal = NULL;
		
		if(grab) {
			setGuiActivity(0);
			grab = 0;
		}
		
		if(is_terrain) Terrain::disable(reset);
		else Mask::disable(reset);
		
		node = NULL;
		
		hotkeysResetPriorityHotkeys();
	}
	
	/******************************************************************************\
	*
	* Update
	*
	\******************************************************************************/
	
	/*
	 */
	void update(int need_reload) {
		
		if(node == NULL) return;
		
		if(need_reload || engine.editor.isNode(node) == 0) {
			disable();
			return;
		}
		
		float x,y;
		vec3 point;
		
		BrushToolBase brush_tool = (brush_tools.size() == 0) ? NULL : (brush_tools[brush_tools.size() - 1]);
		
		if(!get_intersection_point(point,x,y)) {
			// ungrab mouse
			if(grab && !hotkeysGetState("brushes_get_brush_value") && !hotkeysGetState("brushes_apply_brush")) {
				
				if(brush_tool != NULL && brush_tools.size()) {
					brush_tool.flush();
					unredoAddBrushState();
				}
				setGuiActivity(0);
				grab = 0;
			}
			
			if(is_terrain) Terrain::setBrushEnabled(0);
			
			return;
		}
		brush_decal.update(point);
		
		if(is_terrain) Terrain::update();
		
		// brush node
		if(getGuiActivity() == 0 || grab == 1) {
			
			// update brush
			if(hotkeysGetState("brushes_apply_brush") && !gui.getActivity()) {
				setGuiActivity(1);
				grab = 1;
				
				if(brush_tool == NULL || brush_tool.brush != brush) {
					brush_tool = new_brush_tools();
					if(brush_tool != NULL) brush_tools.append(brush_tool);
				}
				
				if(brush_tool != NULL) brush_tool.update(x,y);
			}
			
			// get brush value
			if(hotkeysGetState("brushes_get_brush_value") && !gui.getActivity() && is_terrain) {
				setGuiActivity(1);
				grab = 1;
				
				Terrain::grab_brush_value(x,y);
			}
			
			// release brush
			if(!hotkeysGetState("brushes_get_brush_value") && !hotkeysGetState("brushes_apply_brush") && grab) {
				
				if(brush_tools.size()) brush_tool.flush();
				
				unredoAddBrushState();
				setGuiActivity(0);
				grab = 0;
			}
		}
	}
	
	/*
	 */
	int get_intersection_point(vec3 &ip,float &x,float &y) {
		if(is_terrain) return Terrain::get_intersection_point(ip,x,y);
		return Mask::get_intersection_point(ip,x,y);
	}
	
	/*
	 */
	BrushToolBase new_brush_tools() {
		if(is_terrain) return Terrain::new_brush_tools();
		return Mask::new_brush_tools();
	}
	
	/*
	 */
	void shutdown() {
		if(is_terrain) Terrain::shutdown();
		if(is_enabled) disable();
	}
	
	/**************************************************************************\
	*
	* Undo/redo
	*
	\**************************************************************************/
	
	/*
	 */
	void undo() {
		if(brush_tools.size() == 0) {
			disable();
		}
		if(brush_tools.size()) {
			brush_tools[brush_tools.size() - 1].reset();
			delete brush_tools[brush_tools.size() - 1];
			brush_tools.remove();
		}
	}
	
	void redo() {
		
	}
}
/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void brushesShutdown() {
	Brushes::shutdown();
}

int brushesUpdate(int need_reload) {
	Brushes::update(need_reload);
}

void brushesDisable(int reset = 0) {
	if(!Brushes::is_enabled) return;
	Brushes::disable(reset);
}

int brushesIsEnabled() {
	return Brushes::is_enabled;
}

void brushesIncRadius() {
	if(!Brushes::is_enabled) return;
	Brushes::radius_changed(Brushes::radius + 1.0f);
}

void brushesDecRadius() {
	if(!Brushes::is_enabled) return;
	Brushes::radius_changed(Brushes::radius - 1.0f);
}
