/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    landscape.cpp
 * Desc:    Landscape plugin
 * Version: 1.03
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
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

/******************************************************************************\
*
* Plugin
*
\******************************************************************************/

/*
 */
string getName() {
	return "Landscape";
}

/*
 */
void init(PluginMeta m) {
	Landscape::init(m);
}

void shutdown() {
	Landscape::shutdown();
}

void update(int need_reload) {
	Landscape::update(need_reload);
}

void save() {
	Landscape::save();
}

void show() {
	Landscape::show();
}
/******************************************************************************\
*
* Landscape
*
\******************************************************************************/

/*
 */
namespace Landscape {
	
	/*
	 */
	using Unigine::Widgets;
	
	/*
	 */
	enum {
		
		WAIT_TIME = 1000,
		SLEEP_TIME = 10000,
		
		MASK_THRESHOLD = 0.0001f,
		COMPARE_THRESHOLD = 2,
		
		SURFACE_SIZE = 256,
		TEXTURE_SIZE = 16,
		
		DIFFUSE_R = 127,
		DIFFUSE_G = 127,
		DIFFUSE_B = 127,
		DIFFUSE_A = 255,
		
		NORMAL_R = 127,
		NORMAL_G = 127,
		
		MASK_R = 0,
		
		INDEX_R = 0,
	};
	
	/*
	 */
	UserInterface interface;
	
	Window window;
	TabBox tabbox;
	
	Node old_node;
	Node landscape;
	
	/*
	 */
	PluginMeta meta;
	
	/*
	 */
	void init(PluginMeta m) {
		
		meta = m;
		
		interface = new UserInterface(replace(meta.source,".cpp",".ui"),getName() + "::");
		hotkeysAddUiToolTips(interface);
		
		Common::clear();
		Materials::clear();
		Import::clear();
		Nodes::clear();
		
		window.arrange();
		pluginsAddWindow(window,meta.title,meta.name);
		tabbox.setCurrentTab(configGet("plugin_landscape_tab",0));
	}
	
	/*
	 */
	void shutdown() {
		
		update(1);
		
		configSet("plugin_landscape_tab",tabbox.getCurrentTab());
		pluginsRemoveWindow(window,meta.name);
		
		hotkeysRemoveUiToolTips(interface);
		delete interface;
	}
	
	void show() {
		pluginsShowWindow(window,meta.name);
	}
	
	/*
	 */
	void update(int need_reload) {
		
		Node node = nodesGetNode();
		if(need_reload || node != old_node) {
			old_node = node;
			
			save();
			
			if(engine.editor.isNode(node) || engine.editor.isNode(landscape) == 0) {
				
				landscape = NULL;
				Common::clear();
				Materials::clear();
				Import::clear();
				Nodes::clear();
				
				while(engine.editor.isNode(node)) {
					if(node.getType() == NODE_DUMMY) {
						string data = node.getData();
						if(is_string(data) && strlen(data) && data[0] == '<') {
							Xml xml = new Xml();
							if(xml.parse(data) && xml.getName() == "landscape") {
								landscape = node;
								if(xml.isChild("common")) Common::load(xml.getChild("common"));
								if(xml.isChild("materials")) Materials::load(xml.getChild("materials"));
								if(xml.isChild("import")) Import::load(xml.getChild("import"));
								if(xml.isChild("nodes")) Nodes::load(xml.getChild("nodes"));
							}
							delete xml;
							break;
						}
					}
					node = node.getParent();
				}
			}
		}
	}
	
	/*
	 */
	void save() {
		
		if(engine.editor.isNode(landscape)) {
			Xml xml = new Xml("landscape");
			Common::save(xml.addChild("common"));
			Materials::save(xml.addChild("materials"));
			Import::save(xml.addChild("import"));
			Nodes::save(xml.addChild("nodes"));
			landscape.setData(xml.getSubTree());
			delete xml;
		}
	}
	
	/*
	 */
	string get_name(string name,int x,int y) {
		name = replace(name,"%xx",format("%02x",x));
		name = replace(name,"%xy",format("%02x",y));
		name = replace(name,"%dx",format("%02d",x));
		name = replace(name,"%dy",format("%02d",y));
		return name;
	}
	
	int check_path(string path) {
		if(landscape == NULL) return 0;
		
		forloop(int i = 0; landscape.getNumChilds()) {
			ObjectTerrain terrain = node_cast(landscape.getChild(i));
			
			if(terrain.getDiffuseTextureName() == path) return 0;
			if(terrain.getNormalTextureName() == path) return 0;
			if(terrain.getMaskTextureName() == path) return 0;
			if(terrain.getIndexTextureName() == path) return 0;
			
			forloop(int x = 0; terrain.getSurfacesX()) {
				forloop(int y = 0; terrain.getSurfacesY()) {
					if(terrain.getSurfaceDiffuseTextureName(x,y) == path) return 0;
					if(terrain.getSurfaceNormalTextureName(x,y) == path) return 0;
					if(terrain.getSurfaceMaskTextureName(x,y) == path) return 0;
					if(terrain.getSurfaceIndexTextureName(x,y) == path) return 0;
				}
			}
			
			forloop(int material = 0; terrain.getNumMaterials()) {
				if(terrain.getMaterialDiffuseTextureName(material) == path) return 0;
				if(terrain.getMaterialNormalTextureName(material) == path) return 0;
			}
		}
		
		return 1;
	}
	
	/*
	 */
	void downsize_image(Image image,string name) {
		
		Image buffer = new Image();
		
		int width = image.getWidth();
		int height = image.getHeight();
		
		void clear() {
			if(image.getWidth() != width || image.getHeight() != height) {
				log.message("Downsizing \"%s\" file %dx%d > %dx%d\n",name,width,height,image.getWidth(),image.getHeight());
			}
			delete buffer;
		}
		
		if(image.isCompressedFormat()) image.decompress();
		
		// minimal size
		buffer.copy(image);
		buffer.resize(TEXTURE_SIZE,TEXTURE_SIZE);
		buffer.resize(width,height);
		if(image.compare(buffer,0,0,0,0,width,height,COMPARE_THRESHOLD)) {
			image.resize(TEXTURE_SIZE,TEXTURE_SIZE);
			clear();
			return;
		}
		
		// two times
		buffer.copy(image);
		buffer.resize(max(TEXTURE_SIZE,width / 2),max(TEXTURE_SIZE,height / 2));
		buffer.resize(width,height);
		if(image.compare(buffer,0,0,0,0,width,height,COMPARE_THRESHOLD) == 0) {
			clear();
			return;
		}
		
		// four times
		buffer.copy(image);
		buffer.resize(max(TEXTURE_SIZE,width / 4),max(TEXTURE_SIZE,buffer.getHeight() / 4));
		buffer.resize(width,height);
		if(image.compare(buffer,0,0,0,0,width,height,COMPARE_THRESHOLD) == 0) {
			image.resize(max(TEXTURE_SIZE,width / 2),max(TEXTURE_SIZE,height / 2));
			clear();
			return;
		}
		
		// eight times
		buffer.copy(image);
		buffer.resize(max(TEXTURE_SIZE,width / 8),max(TEXTURE_SIZE,buffer.getHeight() / 8));
		buffer.resize(width,height);
		if(image.compare(buffer,0,0,0,0,width,height,COMPARE_THRESHOLD) == 0) {
			image.resize(max(TEXTURE_SIZE,width / 4),max(TEXTURE_SIZE,height / 4));
			clear();
			return;
		}
		
		clear();
	}
	
	/*
	 */
	void update_neighborhood(int size_x,int size_y) {
		
		forloop(int Y = 0; size_y) {
			forloop(int X = 0; size_x) {
				int current = Y * size_x + X;
				
				ObjectTerrain terrain = node_cast(landscape.getChild(current));
				
				if(X > 0) {
					ObjectTerrain neighbor = node_cast(landscape.getChild(current - 1));
					terrain.setLodTerrainLeft(neighbor);
				}
				else if(X < (size_x - 1)) {
					ObjectTerrain neighbor = node_cast(landscape.getChild(current + 1));
					terrain.setLodTerrainRight(neighbor);
				}
				
				if(Y > 0) {
					ObjectTerrain neighbor = node_cast(landscape.getChild(current - size_x));
					terrain.setLodTerrainBottom(neighbor);
				}
				else if(Y < (size_y - 1)) {
					ObjectTerrain neighbor = node_cast(landscape.getChild(current + size_x));
					terrain.setLodTerrainTop(neighbor);
				}
			}
		}
	}
	
	/**************************************************************************\
	*
	* TiledImage
	*
	\**************************************************************************/
	
	/*
	 */
	namespace TiledImage {
		
		enum {
			MAX_IMAGE_SIZE = 16384,
		};
		
		int tile_width;			// tile width
		int tile_height;		// tile height
		int tile_format;		// tile format
		
		int tile_size_x;		// tiles x size
		int tile_size_y;		// tiles y size
		int tile_flip_y;		// tiles flip y
		
		string tile_names[0];	// image names
		
		int frame;				// frame number
		int tile_frames[0];		// image frames
		Image tile_cache[0];	// image cache
		long memory_limit;		// memory limit
		
		/*
		 */
		int load(string name,int offset_x,int offset_y,int size_x,int size_y,int flip_y) {
			
			clear();
			
			tile_width = -1;
			tile_height = -1;
			tile_format = -1;
			
			tile_size_x = size_x;
			tile_size_y = size_y;
			tile_flip_y = flip_y;
			
			tile_names.resize(size_x * size_y);
			
			tile_cache.resize(size_x * size_y);
			tile_frames.resize(size_x * size_y);
			
			memory_limit = long(max(engine.console.getInt("system_memory") / 8,256)) * 1024 * 1024;
			#ifdef ARCH_X86
				memory_limit = min(memory_limit,1024 * 1024 * 512);
			#elif ARCH_X64
				memory_limit = min(memory_limit,1024 * 1024 * 2048);
			#endif
			
			log.message("Loading \"%s\" image\n",name);
			
			Image image = new Image();
			
			forloop(int y = 0; size_y) {
				forloop(int x = 0; size_x) {
					
					string tile_name = get_name(name,offset_x + x,offset_y + y);
					tile_names[size_x * y + x] = tile_name;
					
					// info image
					if(image.info(tile_name) == 0 && image.load(tile_name) == 0) {
						dialogMessageOk(TR("Error"));
						delete image;
						clear();
						return 0;
					}
					
					// image size
					if(tile_width == -1) {
						tile_width = image.getWidth();
						tile_height = image.getHeight();
					} else if(tile_width != image.getWidth() || tile_height != image.getHeight()) {
						dialogMessageOk(TR("Error"),format(TR("Bad \"%s\" image size %dx%d"),tile_name,image.getWidth(),image.getHeight()));
						delete image;
						clear();
						return 0;
					}
					
					// image format
					if(tile_format == -1) {
						tile_format = image.getFormat();
					} else if(tile_format != image.getFormat()) {
						dialogMessageOk(TR("Error"),format(TR("Bad \"%s\" image format %s"),tile_name,image.getFormatName()));
						delete image;
						clear();
						return 0;
					}
					
					// update progress dialog
					int progress = (size_x * y + x + 1) * 100 / (size_x * size_y);
					if(dialogProgressUpdate2(progress) == 0) {
						delete image;
						clear();
						return 0;
					}
				}
			}
			
			delete image;
			
			return 1;
		}
		
		/*
		 */
		int clear() {
			
			tile_width = -1;
			tile_height = -1;
			tile_format = -1;
			
			tile_size_x = 0;
			tile_size_y = 0;
			tile_flip_y = 0;
			
			tile_names.clear();
			
			tile_cache.delete();
			tile_frames.clear();
			
			return 1;
		}
		
		/*
		 */
		int getWidth() {
			return tile_width * tile_size_x;
		}
		
		int getHeight() {
			return tile_height * tile_size_y;
		}
		
		int getFormat() {
			return tile_format;
		}
		
		/*
		 */
		int getTileWidth() {
			return tile_width;
		}
		
		int getTileHeight() {
			return tile_height;
		}
		
		/*
		 */
		long getMemoryUsage() {
			long memory_usage = 0;
			foreach(Image image; tile_cache) {
				if(image != NULL) memory_usage += image.getSize();
			}
			return memory_usage;
		}
		
		/*
		 */
		Image get_tile(Async async,int x,int y) {
			
			// check coordinates
			if(x < 0 || x >= tile_size_x) return NULL;
			if(y < 0 || y >= tile_size_y) return NULL;
			
			// cached image
			int num = tile_size_x * y + x;
			if(tile_cache[num] != NULL) {
				tile_frames[num] = frame;
				return tile_cache[num];
			}
			
			// memory usage
			while(getMemoryUsage() > memory_limit) {
				Image image = NULL;
				int min_frame = 1000000;
				forloop(int i = 0; tile_cache.size()) {
					if(tile_cache[i] != NULL && tile_frames[i] < min_frame) {
						min_frame = tile_frames[i];
						image = tile_cache[i];
					}
				}
				if(image == NULL) break;
				delete image;
			}
			
			// load image
			Image image = new Image();
			int id = async.run(image,"load",tile_names[num]);
			while(async.isRunning(id)) {
				usleep(WAIT_TIME);
				wait;
			}
			if(async.getResult(id) == 0) {
				log.error("Landscape::TiledImage::get_tile(): can't load \"%s\" image\n",tile_names[num]);
				delete image;
				return NULL;
			}
			
			if(tile_flip_y) {
				if(image.isCompressedFormat()) image.decompress();
				image.flipY();
			}
			
			tile_cache[num] = image;
			tile_frames[num] = frame;
			return image;
		}
		
		/*
		 */
		Image getImage(Async async,int offset_x,int offset_y,int width,int height,int format) {
			
			if(width > MAX_IMAGE_SIZE) return NULL;
			if(height > MAX_IMAGE_SIZE) return NULL;
			
			// update frame
			frame++;
			
			// destination image
			Image dest = new Image();
			dest.create2D(width,height,tile_format);
			if(dest.isLoaded() == 0) {
				delete dest;
				return NULL;
			}
			
			// read source image
			for(int y0 = 0; y0 < dest.getHeight(); y0++) {
				int Y = offset_y + y0;
				if(Y < 0) continue;
				
				int y1 = Y % tile_height;
				int height = tile_height - y1;
				if(y0 + height > dest.getHeight()) height = dest.getHeight() - y0;
				
				for(int x0 = 0; x0 < dest.getWidth(); x0++) {
					int X = offset_x + x0;
					if(X < 0) continue;
					
					int x1 = X % tile_width;
					int width = tile_width - x1;
					if(x0 + width > dest.getWidth()) width = dest.getWidth() - x0;
					
					// update image
					Image src = get_tile(async,X / tile_width,Y / tile_height);
					if(src != NULL) dest.copy(src,x0,y0,x1,y1,width,height);
					
					x0 += width - 1;
					
					// update progress dialog
					int progress = int(long(dest.getWidth() * y0 + height * (x0 + 1)) * 100 / (dest.getWidth() * dest.getHeight()));
					if(dialogProgressUpdate2(progress) == 0) {
						delete dest;
						return NULL;
					}
				}
				
				y0 += height - 1;
			}
			
			// update format
			dest.convertToFormat(format);
			
			return dest;
		}
		
		/*
		 */
		Image getImage(Async async,int image_width,int image_height,int offset_x,int offset_y,int width,int height,int format) {
			
			int border_x = 0;
			int border_y = 0;
			
			int old_width = width;
			int old_height = height;
			
			// scale width
			if(image_width < getWidth()) {
				border_x = 1;
				offset_x = int(long(offset_x - 1) * getWidth() / image_width);
				width = int(long(width + 1) * getWidth() / image_width);
			} else if(image_width > getWidth()) {
				offset_x = int(long(offset_x) * image_width / getWidth());
				width = int(long(width) * image_width / getWidth());
			}
			
			// scale height
			if(image_height < getHeight()) {
				border_y = 1;
				offset_y = int(long(offset_y - 1) * getHeight() / image_height);
				height = int(long(height + 1) * getHeight() / image_height);
			} else if(image_height > getHeight()) {
				offset_y = int(long(offset_y) * image_height / getHeight());
				height = int(long(height) * image_height / getHeight());
			}
			
			// get image
			Image image = getImage(async,offset_x,offset_y,width,height,format);
			if(image == NULL) return NULL;
			
			if(image.isCompressedFormat()) image.decompress();
			
			// resize image
			if(image.resize(old_width + border_x * 2,old_height + border_y * 2) == 0) {
				delete image;
				return NULL;
			}
			
			// remove border
			if(border_x || border_y) {
				Image border = image;
				image = new Image();
				image.create2D(old_width,old_height,format);
				image.copy(border,0,0,border_x,border_y,old_width,old_height);
				delete border;
			}
			
			return image;
		}
	}
	
	/**************************************************************************\
	*
	* Common
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Common {
		
		/*
		 */
		EditLine terrain_step_el;
		ComboBox terrain_size_cb;
		EditLine terrain_elevation_el;
		EditLine terrain_size_x_el;
		EditLine terrain_size_y_el;
		
		Label terrain_storage_l;
		Label terrain_width_l;
		Label terrain_height_l;
		
		float terrain_step;
		int terrain_size;
		float terrain_elevation;
		int terrain_size_x;
		int terrain_size_y;
		
		/*
		 */
		EditLine terrain_flatness_el;
		EditLine terrain_lod_distance_el;
		EditLine terrain_lod_progression_el;
		EditLine terrain_visible_distance_el;
		EditLine terrain_shadow_distance_el;
		EditLine terrain_shadow_radius_el;
		CheckBox terrain_cast_shadow_cb;
		
		float terrain_flatness;
		float terrain_lod_distance;
		float terrain_lod_progression;
		float terrain_visible_distance;
		float terrain_shadow_distance;
		float terrain_shadow_radius;
		int terrain_cast_shadow;
		
		/*
		 */
		ComboBox diffuse_size_cb;
		ComboBox diffuse_coarse_cb;
		ComboBox normal_size_cb;
		ComboBox normal_coarse_cb;
		ComboBox mask_size_cb;
		ComboBox mask_coarse_cb;
		
		int diffuse_size;
		int diffuse_coarse;
		int normal_size;
		int normal_coarse;
		int mask_size;
		int mask_coarse;
		
		/*
		 */
		EditLine landscape_path_el;
		EditLine terrain_name_el;
		EditLine diffuse_name_el;
		EditLine normal_name_el;
		EditLine mask_name_el;
		EditLine index_name_el;
		
		string landscape_path;
		string terrain_name;
		string diffuse_name;
		string normal_name;
		string mask_name;
		string index_name;
		
		/*
		 */
		Button load_b;
		Button save_b;
		Button create_b;
		
		/*
		 */
		Button normal_b;
		Button compress_b;
		Button decompress_b;
		
		/*
		 */
		void clear() {
			
			terrain_step = 1.0f;
			terrain_size = 513;
			terrain_elevation = 100.0f;
			terrain_size_x = 4;
			terrain_size_y = 4;
			
			terrain_flatness = 1.0f;
			terrain_lod_distance = 100.0f;
			terrain_lod_progression = 2.0f;
			terrain_visible_distance = 1e9f;
			terrain_shadow_distance = 1e9f;
			terrain_shadow_radius = 0.0f;
			terrain_cast_shadow = 0;
			
			diffuse_size = 1024;
			diffuse_coarse = 512;
			normal_size = 1024;
			normal_coarse = 512;
			mask_size = 1024;
			mask_coarse = 512;
			
			landscape_path = "landscape";
			terrain_name = "%xxx%xy/terrain";
			diffuse_name = "%xxx%xy_d.dds";
			normal_name = "%xxx%xy_n.dds";
			mask_name = "%xxx%xy_m.dds";
			index_name = "%xxx%xy_i.dds";
			
			update();
			
			update_terrain();
		}
		
		/*
		 */
		void update() {
			
			terrain_step_el.setText(editorFormat(terrain_step,5));
			terrain_size_cb.setCurrentItemText(terrain_size);
			terrain_elevation_el.setText(editorFormat(terrain_elevation));
			terrain_size_x_el.setText(string(terrain_size_x));
			terrain_size_y_el.setText(string(terrain_size_y));
			
			terrain_flatness_el.setText(editorFormat(terrain_flatness));
			terrain_lod_distance_el.setText(editorFormat(terrain_lod_distance));
			terrain_lod_progression_el.setText(editorFormat(terrain_lod_progression));
			terrain_visible_distance_el.setText(editorFormat(terrain_visible_distance));
			terrain_shadow_distance_el.setText(editorFormat(terrain_shadow_distance));
			terrain_shadow_radius_el.setText(editorFormat(terrain_shadow_radius));
			terrain_cast_shadow_cb.setChecked(terrain_cast_shadow);
			
			diffuse_size_cb.setCurrentItemText(diffuse_size);
			diffuse_coarse_cb.setCurrentItemText(diffuse_coarse);
			normal_size_cb.setCurrentItemText(normal_size);
			normal_coarse_cb.setCurrentItemText(normal_coarse);
			mask_size_cb.setCurrentItemText(mask_size);
			mask_coarse_cb.setCurrentItemText(mask_coarse);
			
			landscape_path_el.setText(landscape_path);
			terrain_name_el.setText(terrain_name);
			diffuse_name_el.setText(diffuse_name);
			normal_name_el.setText(normal_name);
			mask_name_el.setText(mask_name);
			index_name_el.setText(index_name);
		}
		
		void update_terrain() {
			
			long uncompressed = long(terrain_size) * terrain_size * 2;
			uncompressed += long(diffuse_size) * diffuse_size * 3;
			uncompressed += long(diffuse_coarse) * diffuse_coarse * 3;
			uncompressed += long(normal_size) * normal_size * 2;
			uncompressed += long(normal_coarse) * normal_coarse * 2;
			uncompressed += max(long(mask_size) * mask_size,long(mask_coarse) * mask_coarse) * 4;
			uncompressed *= terrain_size_x * terrain_size_y;
			
			long compressed = long(terrain_size) * terrain_size * 2;
			compressed += long(diffuse_size) * diffuse_size / 2;
			compressed += long(diffuse_coarse) * diffuse_coarse / 2;
			compressed += long(normal_size) * normal_size;
			compressed += long(normal_coarse) * normal_coarse;
			compressed += max(long(mask_size) * mask_size,long(mask_coarse) * mask_coarse);
			compressed *= terrain_size_x * terrain_size_y;
			
			terrain_storage_l.setText(memory(uncompressed) + "/" + memory(compressed));
			
			double width = double(terrain_size_x) * (terrain_size - 1) * terrain_step;
			double height = double(terrain_size_y) * (terrain_size - 1) * terrain_step;
			
			terrain_width_l.setText(format("%.1f units",width));
			terrain_height_l.setText(format("%.1f units",height));
			
			Widget widgets[] = (
				terrain_step_el,
				terrain_size_cb,
				terrain_elevation_el,
				terrain_size_x_el,
				terrain_size_y_el,
				diffuse_size_cb,
				diffuse_coarse_cb,
				normal_size_cb,
				normal_coarse_cb,
				mask_size_cb,
				mask_coarse_cb,
				landscape_path_el,
				terrain_name_el,
				diffuse_name_el,
				normal_name_el,
				mask_name_el,
				index_name_el,
			);
			
			if(landscape == NULL) {
				
				foreach(Widget widget; widgets) {
					widget.setEnabled(1);
				}
				
				load_b.setEnabled(1);
				save_b.setEnabled(0);
				create_b.setEnabled(1);
				
				normal_b.setEnabled(0);
				compress_b.setEnabled(0);
				decompress_b.setEnabled(0);
				
			} else {
				
				foreach(Widget widget; widgets) {
					widget.setEnabled(0);
				}
				
				load_b.setEnabled(0);
				save_b.setEnabled(1);
				create_b.setEnabled(0);
				
				normal_b.setEnabled(1);
				compress_b.setEnabled(1);
				decompress_b.setEnabled(1);
			}
			
			terrain_shadow_distance_el.setEnabled(terrain_cast_shadow);
			terrain_shadow_radius_el.setEnabled(terrain_cast_shadow);
		}
		
		/*
		 */
		void load(Xml xml) {
			
			terrain_step = xml.getChildData("terrain_step",terrain_step);
			terrain_size = xml.getChildData("terrain_size",terrain_size);
			terrain_elevation = xml.getChildData("terrain_elevation",terrain_elevation);
			terrain_size_x = xml.getChildData("terrain_size_x",terrain_size_x);
			terrain_size_y = xml.getChildData("terrain_size_y",terrain_size_y);
			
			terrain_flatness = xml.getChildData("terrain_flatness",terrain_flatness);
			terrain_lod_distance = xml.getChildData("terrain_lod_distance",terrain_lod_distance);
			terrain_lod_progression = xml.getChildData("terrain_lod_progression",terrain_lod_progression);
			terrain_visible_distance = xml.getChildData("terrain_visible_distance",terrain_visible_distance);
			terrain_shadow_distance = xml.getChildData("terrain_shadow_distance",terrain_shadow_distance);
			terrain_shadow_radius = xml.getChildData("terrain_shadow_radius",terrain_shadow_radius);
			terrain_cast_shadow = xml.getChildData("terrain_cast_shadow",terrain_cast_shadow);
			
			diffuse_size = xml.getChildData("diffuse_size",diffuse_size);
			diffuse_coarse = xml.getChildData("diffuse_coarse",diffuse_coarse);
			normal_size = xml.getChildData("normal_size",normal_size);
			normal_coarse = xml.getChildData("normal_coarse",normal_coarse);
			mask_size = xml.getChildData("mask_size",mask_size);
			mask_coarse = xml.getChildData("mask_coarse",mask_coarse);
			
			landscape_path = xml.getChildData("landscape_path",landscape_path);
			terrain_name = xml.getChildData("terrain_name",terrain_name);
			diffuse_name = xml.getChildData("diffuse_name",diffuse_name);
			normal_name = xml.getChildData("normal_name",normal_name);
			mask_name = xml.getChildData("mask_name",mask_name);
			index_name = xml.getChildData("index_name",index_name);
			
			update();
			
			update_terrain();
		}
		
		/*
		 */
		void save(Xml xml) {
			
			xml.setChildData("terrain_step",terrain_step);
			xml.setChildData("terrain_size",terrain_size);
			xml.setChildData("terrain_elevation",terrain_elevation);
			xml.setChildData("terrain_size_x",terrain_size_x);
			xml.setChildData("terrain_size_y",terrain_size_y);
			
			xml.setChildData("terrain_flatness",terrain_flatness);
			xml.setChildData("terrain_lod_distance",terrain_lod_distance);
			xml.setChildData("terrain_lod_progression",terrain_lod_progression);
			xml.setChildData("terrain_visible_distance",terrain_visible_distance);
			xml.setChildData("terrain_shadow_distance",terrain_shadow_distance);
			xml.setChildData("terrain_shadow_radius",terrain_shadow_radius);
			xml.setChildData("terrain_cast_shadow",terrain_cast_shadow);
			
			xml.setChildData("diffuse_size",diffuse_size);
			xml.setChildData("diffuse_coarse",diffuse_coarse);
			xml.setChildData("normal_size",normal_size);
			xml.setChildData("normal_coarse",normal_coarse);
			xml.setChildData("mask_size",mask_size);
			xml.setChildData("mask_coarse",mask_coarse);
			
			xml.setChildData("landscape_path",landscape_path);
			xml.setChildData("terrain_name",terrain_name);
			xml.setChildData("diffuse_name",diffuse_name);
			xml.setChildData("normal_name",normal_name);
			xml.setChildData("mask_name",mask_name);
			xml.setChildData("index_name",index_name);
		}
		
		/*
		 */
		void terrain_step_pressed() {
			terrain_step = float(terrain_step_el.getText());
			terrain_step_el.setText(editorFormat(terrain_step,5));
			update_terrain();
		}
		
		void terrain_size_changed() {
			terrain_size = int(terrain_size_cb.getCurrentItemText());
			terrain_size_cb.setCurrentItemText(terrain_size);
			update_terrain();
		}
		
		void terrain_elevation_pressed() {
			terrain_elevation = float(terrain_elevation_el.getText());
			terrain_elevation_el.setText(editorFormat(terrain_elevation));
		}
		
		void terrain_size_x_pressed() {
			terrain_size_x = int(terrain_size_x_el.getText());
			terrain_size_x_el.setText(string(terrain_size_x));
			update_terrain();
		}
		
		void terrain_size_y_pressed() {
			terrain_size_y = int(terrain_size_y_el.getText());
			terrain_size_y_el.setText(string(terrain_size_y));
			update_terrain();
		}
		
		/*
		 */
		void terrain_set_parameter(string name,int values[]) {
			if(landscape != NULL) {
				forloop(int i = 0; landscape.getNumChilds()) {
					Node node = node_cast(landscape.getChild(i));
					if(node.getType() == NODE_OBJECT_TERRAIN) {
						ObjectTerrain terrain = node;
						terrain.call(name,values);
					}
				}
			}
		}
		
		void terrain_flatness_pressed() {
			terrain_flatness = float(terrain_flatness_el.getText());
			terrain_flatness_el.setText(editorFormat(terrain_flatness));
			terrain_set_parameter("setLodThreshold",(terrain_flatness,0));
		}
		
		void terrain_lod_distance_pressed() {
			terrain_lod_distance = float(terrain_lod_distance_el.getText());
			terrain_lod_distance_el.setText(editorFormat(terrain_lod_distance));
			terrain_set_parameter("setLodDistance",(terrain_lod_distance));
		}
		
		void terrain_lod_progression_pressed() {
			terrain_lod_progression = float(terrain_lod_progression_el.getText());
			terrain_lod_progression_el.setText(editorFormat(terrain_lod_progression));
			terrain_set_parameter("setLodProgression",(terrain_lod_progression));
		}
		
		void terrain_visible_distance_pressed() {
			terrain_visible_distance = float(terrain_visible_distance_el.getText());
			terrain_visible_distance_el.setText(editorFormat(terrain_visible_distance));
			terrain_set_parameter("setVisibleDistance",(terrain_visible_distance));
		}
		
		void terrain_shadow_distance_pressed() {
			terrain_shadow_distance = float(terrain_shadow_distance_el.getText());
			terrain_shadow_distance_el.setText(editorFormat(terrain_shadow_distance));
			terrain_set_parameter("setShadowDistance",(terrain_shadow_distance));
		}
		
		void terrain_shadow_radius_pressed() {
			terrain_shadow_radius = float(terrain_shadow_radius_el.getText());
			terrain_shadow_radius_el.setText(editorFormat(terrain_shadow_radius));
			terrain_set_parameter("setShadowRadius",(terrain_shadow_radius));
		}
		
		void terrain_cast_shadow_changed() {
			terrain_cast_shadow = terrain_cast_shadow_cb.isChecked();
			update_terrain();
			if(landscape != NULL) {
				forloop(int i = 0; landscape.getNumChilds()) {
					Node node = node_cast(landscape.getChild(i));
					if(node.getType() == NODE_OBJECT_TERRAIN) {
						ObjectTerrain terrain = node;
						terrain.setCastShadow(terrain_cast_shadow,0);
						terrain.setCastWorldShadow(terrain_cast_shadow,0);
						forloop(int y = 0; terrain.getSurfacesY()) {
							forloop(int x = 0; terrain.getSurfacesX()) {
								terrain.setSurfaceCastShadow(x,y,terrain_cast_shadow);
							}
						}
					}
				}
			}
		}
		
		/*
		 */
		void diffuse_size_changed() {
			diffuse_size = int(diffuse_size_cb.getCurrentItemText());
			diffuse_size_cb.setCurrentItemText(diffuse_size);
			update_terrain();
		}
		
		void diffuse_coarse_changed() {
			diffuse_coarse = int(diffuse_coarse_cb.getCurrentItemText());
			diffuse_coarse_cb.setCurrentItemText(diffuse_coarse);
			update_terrain();
		}
		
		void normal_size_changed() {
			normal_size = int(normal_size_cb.getCurrentItemText());
			normal_size_cb.setCurrentItemText(normal_size);
			update_terrain();
		}
		
		void normal_coarse_changed() {
			normal_coarse = int(normal_coarse_cb.getCurrentItemText());
			normal_coarse_cb.setCurrentItemText(normal_coarse);
			update_terrain();
		}
		
		void mask_size_changed() {
			mask_size = int(mask_size_cb.getCurrentItemText());
			mask_size_cb.setCurrentItemText(mask_size);
			update_terrain();
		}
		
		void mask_coarse_changed() {
			mask_coarse = int(mask_coarse_cb.getCurrentItemText());
			mask_coarse_cb.setCurrentItemText(mask_coarse);
			update_terrain();
		}
		
		/*
		 */
		void landscape_path_pressed() {
			landscape_path = landscape_path_el.getText();
			landscape_path_el.setText(landscape_path);
		}
		
		void terrain_name_pressed() {
			terrain_name = terrain_name_el.getText();
			terrain_name_el.setText(terrain_name);
		}
		
		void diffuse_name_pressed() {
			diffuse_name = diffuse_name_el.getText();
			diffuse_name_el.setText(diffuse_name);
		}
		
		void normal_name_pressed() {
			normal_name = normal_name_el.getText();
			normal_name_el.setText(normal_name);
		}
		
		void mask_name_pressed() {
			mask_name = mask_name_el.getText();
			mask_name_el.setText(mask_name);
		}
		
		void index_name_pressed() {
			index_name = index_name_el.getText();
			index_name_el.setText(index_name);
		}
		
		/**********************************************************************\
		*
		* Load/Save
		*
		\**********************************************************************/
		
		/*
		 */
		void load_clicked() {
			
			string name;
			if(dialogFileSettings(TR("Select landscape settings to load"),name)) {
				Xml xml = new Xml();
				if(xml.load(name)) {
					if(xml.isChild("common")) Common::load(xml.getChild("common"));
					if(xml.isChild("materials")) Materials::load(xml.getChild("materials"));
					if(xml.isChild("import")) Import::load(xml.getChild("import"));
					if(xml.isChild("nodes")) Nodes::load(xml.getChild("nodes"));
				} else {
					dialogMessageOk(TR("Error"));
				}
				delete xml;
			}
		}
		
		/*
		 */
		void save_clicked() {
			
			string name;
			if(dialogFileSettings(TR("Select landscape settings to save"),name)) {
				Xml xml = new Xml("landscape");
				Common::save(xml.addChild("common"));
				Materials::save(xml.addChild("materials"));
				Import::save(xml.addChild("import"));
				Nodes::save(xml.addChild("nodes"));
				if(xml.save(name) == 0) {
					dialogMessageOk(TR("Error"));
				}
				delete xml;
			}
		}
		
		/**********************************************************************\
		*
		* Create
		*
		\**********************************************************************/
		
		/*
		 */
		void create_clicked() {
			
			if(dialogMessageYesNo(TR("Confirm"),TR("Create an empty landscape?")) == 0) {
				return;
			}
			
			// landscape directory
			string path = landscape_path + "/";
			
			if(is_dir(engine.getDataPath() + path)) {
				dialogMessageOk(TR("Error"),TR("Directory is not empty"));
				return;
			}
			
			mkdir(engine.getDataPath() + path,1);
			
			// create landscape
			landscape = new NodeDummy();
			landscape.setName(basename(landscape_path));
			
			// begin progress dialog
			dialogProgressBegin(TR("Creating Terrains..."));
			
			// asynchronous processing
			Async async = asyncGet();
			
			// empty diffuse texture
			Image diffuse = new Image();
			diffuse.create2D(TEXTURE_SIZE,TEXTURE_SIZE,IMAGE_FORMAT_RGBA8);
			diffuse.set(0,DIFFUSE_R);
			diffuse.set(1,DIFFUSE_G);
			diffuse.set(2,DIFFUSE_B);
			diffuse.set(3,DIFFUSE_A);
			
			// empty normal texture
			Image normal = new Image();
			normal.create2D(TEXTURE_SIZE,TEXTURE_SIZE,IMAGE_FORMAT_RG8);
			normal.set(0,NORMAL_R);
			normal.set(1,NORMAL_G);
			
			// empty mask texture
			Image mask = new Image();
			mask.create2D(TEXTURE_SIZE,TEXTURE_SIZE,IMAGE_FORMAT_R8);
			mask.set(0,MASK_R);
			
			// empty index texture
			Image index = new Image();
			index.create2D(TEXTURE_SIZE,TEXTURE_SIZE,IMAGE_FORMAT_R8);
			index.set(0,INDEX_R);
			
			// clear
			void clear() {
				node_delete(landscape);
				dialogProgressEnd();
				delete diffuse;
				delete normal;
				delete mask;
				delete index;
			}
			
			// templates
			template create_coarse<TYPE,POSTFIX,IMAGE> {
				{
					string coarse_name = path + name + POSTFIX + ".dds";
					if(IMAGE.save(coarse_name) == 0) {
						dialogMessageOk(TR("Error"));
						clear();
						return;
					}
					terrain.set ## TYPE ## TextureName(coarse_name);
				}
			}
			
			template create_surface<TYPE,COARSE,SIZE,NAME,IMAGE> {
				{
					int threads[] = ();
					
					string surface_name = path + name + "/" + get_name(NAME,0,0);
					mkdir(engine.getDataPath() + dirname(surface_name),1);
					
					forloop(int y = 0; terrain.getSurfacesY()) {
						forloop(int x = 0; terrain.getSurfacesX()) {
							string surface_name = path + name + "/" + get_name(NAME,x,y);
							terrain.setSurface ## TYPE ## TextureName(x,y,surface_name);
							threads.append(async.run(IMAGE,"save",surface_name));
						}
					}
					
					while(async.isRunning()) {
						usleep(SLEEP_TIME);
						wait;
					}
					
					foreach(int id; threads) {
						if(async.getResult(id) == 0) {
							dialogMessageOk(TR("Error"));
							clear();
							return;
						}
					}
				}
			}
			
			// create terrains
			forloop(int Y = 0; terrain_size_y) {
				forloop(int X = 0; terrain_size_x) {
					
					ObjectTerrain terrain = new ObjectTerrain();
					texturesAddTerrain(terrain);
					landscape.addChild(terrain);
					
					// create terrain
					if(terrain.create(terrain_size,terrain_size,terrain_step) == 0) {
						dialogMessageOk(TR("Error"));
						clear();
						return;
					}
					
					// terrain path
					string name = get_name(terrain_name,X,Y);
					mkdir(engine.getDataPath() + path + name,1);
					
					// terrain name
					if(strlen(dirname(name))) terrain.setName(replace(dirname(name),"/",""));
					else terrain.setName(name);
					
					// terrain parameters
					terrain.setEnabled(0);
					terrain.setMaterial("terrain_base","*");
					terrain.setProperty("surface_base","*");
					terrain.setLodThreshold(terrain_flatness);
					terrain.setLodDistance(terrain_lod_distance);
					terrain.setLodProgression(terrain_lod_progression);
					terrain.setVisibleDistance(terrain_visible_distance);
					terrain.setShadowDistance(terrain_shadow_distance);
					terrain.setShadowRadius(terrain_shadow_radius);
					terrain.setCastShadow(terrain_cast_shadow,0);
					terrain.setCastWorldShadow(terrain_cast_shadow,0);
					forloop(int y = 0; terrain.getSurfacesY()) {
						forloop(int x = 0; terrain.getSurfacesX()) {
							terrain.setSurfaceCastShadow(x,y,terrain_cast_shadow);
						}
					}
					
					// terrain transformation
					float terrain_x = terrain_step * (X - terrain_size_x / 2.0f);
					float terrain_y = terrain_step * (Y - terrain_size_y / 2.0f);
					#ifdef USE_DOUBLE
						terrain.setTransform(translate((terrain_size - 1.0) * terrain_x,(terrain_size - 1.0) * terrain_y,0.0));
					#else
						terrain.setTransform(translate((terrain_size - 1.0f) * terrain_x,(terrain_size - 1.0f) * terrain_y,0.0f));
					#endif
					
					// terrain name
					terrain.setTerrainName(path + name + ".ter");
					
					log.message("Creating \"%s\" terrain\n",terrain.getTerrainName());
					
					// save terrain
					int id = async.run(terrain,"saveTerrain",systemEditorPath(terrain.getTerrainName()),1);
					while(async.isRunning(id)) {
						usleep(WAIT_TIME);
						wait;
					}
					if(async.getResult(id) == 0) {
						dialogMessageOk(TR("Error"));
						clear();
						return;
					}
					
					create_coarse<Diffuse,"_d",diffuse>;
					create_coarse<Normal,"_n",normal>;
					create_coarse<Mask,"_m",mask>;
					create_coarse<Index,"_i",index>;
					
					create_surface<Diffuse,diffuse_coarse,diffuse_size,diffuse_name,diffuse>;
					create_surface<Normal,normal_coarse,normal_size,normal_name,normal>;
					create_surface<Mask,mask_coarse,mask_size,mask_name,mask>;
					create_surface<Index,mask_coarse,mask_size,index_name,index>;
					
					// enable terrain
					terrain.setEnabled(1);
					
					// update progress dialog
					int progress = (terrain_size_x * Y + X) * 100 / (terrain_size_x * terrain_size_y);
					if(dialogProgressUpdate(progress) == 0) {
						clear();
						return;
					}
				}
			}
			
			delete diffuse;
			delete normal;
			delete mask;
			delete index;
			
			update_neighborhood(terrain_size_x,terrain_size_y);
			
			engine.editor.addNode(node_remove(landscape));
			
			// end progress dialog
			dialogProgressEnd();
			
			nodesReload();
		}
		
		/*
		 */
		ObjectTerrain get_terrain(int x,int y) {
			if(landscape == NULL) {
				log.error("Landscape::Common::get_terrain(): landscape is NULL\n");
				return NULL;
			}
			int id = terrain_size_x * y + x;
			if(id < landscape.getNumChilds()) {
				Node node = node_cast(landscape.getChild(id));
				if(node.getType() == NODE_OBJECT_TERRAIN) {
					return node;
				}
			}
			log.error("Landscape::Common::get_terrain(): can't get %dx%d terrain\n",x,y);
			return NULL;
		}
		
		/**********************************************************************\
		*
		* Normals
		*
		\**********************************************************************/
		
		/*
		 */
		void create_normal(Async async,ObjectTerrain terrain) {
			
			log.message("Creating \"%s\" normal textures\n",terrain.getTerrainName());
			
			// terrain size
			int width = terrain.getSizeX() - 1;
			int height = terrain.getSizeY() - 1;
			int size = max(width,height);
			
			// create coarse normal texture
			Image image = new Image();
			if(terrain.getNormals(0,0,width,height,image) == 0) {
				delete image;
				return;
			}
			
			// create multiple normal textures
			if(size > normal_coarse) {
				
				Image surface = new Image();
				int surface_width = size / terrain.getSurfacesX();
				int surface_height = size / terrain.getSurfacesY();
				
				// create normal textures
				forloop(int y = 0; terrain.getSurfacesY()) {
					forloop(int x = 0; terrain.getSurfacesX()) {
						
						// pixel border for seams removing
						int surface_copy_width = surface_width;
						int surface_copy_height = surface_height;
						if(x != terrain.getSurfacesX() - 1) surface_copy_width++;
						if(y != terrain.getSurfacesY() - 1) surface_copy_height++;
						surface.create2D(surface_copy_width,surface_copy_height,image.getFormat());
						
						// copy surface normal texture
						surface.copy(image,0,0,surface_width * x,surface_height * y,surface_copy_width,surface_copy_height);
						surface.resize(surface_width,surface_height);
						
						// save surface normal texture
						int id = async.run(surface,"save",terrain.getSurfaceNormalTextureName(x,y));
						while(async.isRunning(id)) {
							usleep(WAIT_TIME);
							wait;
						}
						if(async.getResult(id) == 0) {
							log.error("Landscape::Common::create_normal(): can't save \"%s\" image\n",terrain.getSurfaceNormalTextureName(x,y));
							delete surface;
							delete image;
							return;
						}
						
						// update progress dialog
						int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
						if(dialogProgressUpdate2(progress) == 0) {
							delete surface;
							delete image;
							return;
						}
					}
				}
				
				delete surface;
			}
			
			log.message("Creating \"%s\" texture\n",terrain.getNormalTextureName());
			
			// create coarse normal texture
			async.run(image,"resize",Common::normal_coarse,Common::normal_coarse,IMAGE_FILTER_LINEAR);
			int id = async.run(image,"save",terrain.getNormalTextureName());
			while(async.isRunning(id)) {
				usleep(WAIT_TIME);
				wait;
			}
			if(async.getResult(id) == 0) {
				log.error("Landscape::Common::create_normal(): can't save \"%s\" image\n",terrain.getNormalTextureName());
				return 0;
			}
		}
		
		/*
		 */
		void normal_clicked() {
			
			if(dialogMessageYesNo(TR("Confirm"),TR("Create landscape normal textures?")) == 0) {
				return;
			}
			
			// asynchronous processing
			dialogProgressBegin(TR("Creating normal textures..."));
			panelStatusBarInfoStart("Creating normal textures...");
			forloop(int i = 0; landscape.getNumChilds()) {
				Node node = node_cast(landscape.getChild(i));
				if(node.getType() != NODE_OBJECT_TERRAIN) continue;
				ObjectTerrain terrain = node;
				
				asyncRunThread(functionid(create_normal),(terrain));
				asyncWaitThreads();
				
				int progress = (i + 1) * 100 / landscape.getNumChilds();
				if(dialogProgressUpdate(progress) == 0) {
					dialogProgressEnd();
					panelStatusBarInfoEnd("Creating normal textures...");
					return;
				}
			}
			
			// end progress dialog
			panelStatusBarInfoEnd("Creating normal textures...");
			dialogProgressEnd();
		}
		
		/**********************************************************************\
		*
		* Compress
		*
		\**********************************************************************/
		
		/*
		 */
		void compress_image(Async async,string name,float gamma,int zcompress) {
			
			// check name
			if(strlen(name) == 0) return;
			
			// info image
			Image image = new Image();
			int id = async.run(image,"info",name);
			while(async.isRunning(id)) wait;
			if(async.getResult(id) == 0) {
				delete image;
				return;
			}
			
			// check image format
			if(image.isCompressedFormat() && image.isZCompressedFormat() == zcompress) {
				delete image;
				return;
			}
			
			// load image
			id = async.run(image,"load",name);
			while(async.isRunning(id)) wait;
			if(async.getResult(id) == 0) {
				delete image;
				return;
			}
			
			log.message("Compressing \"%s\" file\n",name);
			
			// compress image
			if(image.isCompressedFormat() == 0) {
				downsize_image(image,name);
				async.run(image,"createMipmaps",IMAGE_FILTER_LINEAR,gamma);
				id = async.run(image,"compress",-1);
				while(async.isRunning(id)) wait;
			}
			
			// zcompress image
			if(zcompress) image.convertToFormat(IMAGE_FORMAT_ZLC2);
			
			// save image
			image.save(name);
			
			delete image;
		}
		
		/*
		 */
		void compress_clicked() {
			
			if(dialogMessageYesNo(TR("Confirm"),TR("Compress all landscape textures?")) == 0) {
				return;
			}
			
			log.message("Compressing terrain textures\n");
			panelStatusBarInfoStart("Compressing textures...");
			// asynchronous processing
			dialogProgressBegin(TR("Compressing Textures..."));
			
			forloop(int i = 0; landscape.getNumChilds()) {
				Node node = node_cast(landscape.getChild(i));
				if(node.getType() != NODE_OBJECT_TERRAIN) continue;
				ObjectTerrain terrain = node;
				
				log.message("Compressing \"%s\" terrain\n",terrain.getTerrainName());
				
				asyncRunThread(functionid(compress_image),(terrain.getDiffuseTextureName(),2.2f,0));
				asyncRunThread(functionid(compress_image),(terrain.getNormalTextureName(),1.0f,0));
				asyncRunThread(functionid(compress_image),(terrain.getMaskTextureName(),1.0f,0));
				
				forloop(int y = 0; terrain.getSurfacesY()) {
					forloop(int x = 0; terrain.getSurfacesX()) {
						
						asyncRunThread(functionid(compress_image),(terrain.getSurfaceDiffuseTextureName(x,y),2.2f,1));
						asyncRunThread(functionid(compress_image),(terrain.getSurfaceNormalTextureName(x,y),1.0f,1));
						asyncRunThread(functionid(compress_image),(terrain.getSurfaceMaskTextureName(x,y),1.0f,0));
						
						asyncWaitThreads();
						
						// update progress dialog
						int progress = (terrain.getSurfacesY() * terrain.getSurfacesY() * i + terrain.getSurfacesX() * y + x + 1) * 100 / (landscape.getNumChilds() * terrain.getSurfacesX() * terrain.getSurfacesY());
						if(dialogProgressUpdate(progress) == 0) {
							dialogProgressEnd();
							panelStatusBarInfoEnd("Compressing textures...");
							return;
						}
					}
				}
				
				asyncWaitThreads();
			}
			panelStatusBarInfoEnd("Compressing textures...");
			dialogProgressEnd();
		}
		
		/**********************************************************************\
		*
		* Decompress
		*
		\**********************************************************************/
		
		/*
		 */
		void decompress_image(Async async,string name) {
			
			// check name
			if(strlen(name) == 0) return;
			
			// info image
			Image image = new Image();
			int id = async.run(image,"info",name);
			while(async.isRunning(id)) wait;
			if(async.getResult(id) == 0) {
				delete image;
				return;
			}
			
			// check image format
			if(image.isCompressedFormat() == 0) {
				delete image;
				return;
			}
			
			// load image
			id = async.run(image,"load",name);
			while(async.isRunning(id)) wait;
			if(async.getResult(id) == 0) {
				delete image;
				return;
			}
			
			log.message("Decompressing \"%s\" file\n",name);
			
			// decompress image
			if(image.isZCompressedFormat()) image.decompress();
			if(image.isCompressedFormat()) image.decompress();
			
			// save image
			image.save(name);
			
			delete image;
		}
		
		/*
		 */
		void decompress_clicked() {
			
			if(dialogMessageYesNo(TR("Confirm"),TR("Decompress all landscape textures?")) == 0) {
				return;
			}
			panelStatusBarInfoStart("Decompressing textures...");
			log.message("Decompressing terrain textures\n");
			
			// asynchronous processing
			dialogProgressBegin(TR("Decompressing textures..."));
			
			forloop(int i = 0; landscape.getNumChilds()) {
				Node node = node_cast(landscape.getChild(i));
				if(node.getType() != NODE_OBJECT_TERRAIN) continue;
				ObjectTerrain terrain = node;
				
				log.message("Decompressing \"%s\" terrain\n",terrain.getTerrainName());
				
				asyncRunThread(functionid(decompress_image),(terrain.getDiffuseTextureName()));
				asyncRunThread(functionid(decompress_image),(terrain.getNormalTextureName()));
				asyncRunThread(functionid(decompress_image),(terrain.getMaskTextureName()));
				
				forloop(int y = 0; terrain.getSurfacesY()) {
					forloop(int x = 0; terrain.getSurfacesX()) {
						
						asyncRunThread(functionid(decompress_image),(terrain.getSurfaceDiffuseTextureName(x,y)));
						asyncRunThread(functionid(decompress_image),(terrain.getSurfaceNormalTextureName(x,y)));
						asyncRunThread(functionid(decompress_image),(terrain.getSurfaceMaskTextureName(x,y)));
						
						asyncWaitThreads();
						
						// update progress dialog
						int progress = (terrain.getSurfacesY() * terrain.getSurfacesY() * i + terrain.getSurfacesX() * y + x + 1) * 100 / (landscape.getNumChilds() * terrain.getSurfacesX() * terrain.getSurfacesY());
						if(dialogProgressUpdate(progress) == 0) {
							dialogProgressEnd();
							panelStatusBarInfoEnd("Decompressing textures...");
							return;
						}
					}
				}
				
				asyncRunThread(functionid(decompress_image),(terrain.getDiffuseTextureArrayName()));
				asyncRunThread(functionid(decompress_image),(terrain.getNormalTextureArrayName()));
				
				asyncWaitThreads();
			}
			panelStatusBarInfoEnd("Decompressing textures...");
			dialogProgressEnd();
		}
	}
	
	/**************************************************************************\
	*
	* Materials
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Materials {
		
		/*
		 */
		Button add_b;
		Button clone_b;
		Button remove_b;
		Button up_b;
		Button down_b;
		
		ListBox materials_lb;
		
		EditLine name_el;
		
		EditLine transform_el[4];
		
		CheckBox overlap_cb;
		CheckBox mask_base_cb;
		ComboBox parent_cb;
		
		
		EditLine mask_value_el;
		EditLine mask_width_el;
		EditLine mask_threshold_el;
		
		Icon diffuse_texture_load_i;
		Icon diffuse_texture_view_i;
		Icon diffuse_texture_edit_i;
		EditLine diffuse_texture_el;
		EditLine diffuse_scale_el;
		
		Icon normal_texture_load_i;
		Icon normal_texture_view_i;
		Icon normal_texture_edit_i;
		EditLine normal_texture_el;
		EditLine normal_scale_el;
		
		Icon specular_texture_load_i;
		Icon specular_texture_view_i;
		Icon specular_texture_edit_i;
		EditLine specular_texture_el;
		EditLine specular_scale_el;
		
		EditLine diffuse_texture_array_el;
		EditLine normal_texture_array_el;
		EditLine specular_texture_array_el;
		
		ComboBox diffuse_texture_array_cb;
		ComboBox normal_texture_array_cb;
		ComboBox specular_texture_array_cb;
		
		int material_num = -1;
		Material materials[0];
		
		/*
		 */
		class Material {
			
			string name;
			
			int overlap;
			int parent;
			int mask_base;
			
			vec4 transform;
			
			float mask_value;
			float mask_width;
			float mask_threshold;
			
			string diffuse_texture;
			float diffuse_scale;
			
			string normal_texture;
			float normal_scale;
			
			string specular_texture;
			float specular_scale;
			
			Material() {
				
				name = format("Material %d",materials.size());
				
				overlap = 0;
				parent = -1;
				mask_base = 0;
				
				transform = vec4(1.0f,1.0f,0.0f,0.0f);
				
				mask_value = 0.5f;
				mask_width = 0.5f;
				mask_threshold = 0.5f;
				
				diffuse_texture = "";
				diffuse_scale = 0.5f;
				
				normal_texture = "";
				normal_scale = 0.5f;
				
				specular_texture = "";
				specular_scale = 0.5f;
				
			}
			
			Material(Material material) {
				
				name = material.name;
				
				overlap = material.overlap;
				parent = material.parent;
				mask_base = material.mask_base;
				
				transform = material.transform;
				
				mask_value = material.mask_value;
				mask_width = material.mask_width;
				mask_threshold = material.mask_threshold;
				
				diffuse_texture = material.diffuse_texture;
				diffuse_scale = material.diffuse_scale;
				
				normal_texture = material.normal_texture;
				normal_scale = material.normal_scale;
				
				specular_texture = material.specular_texture;
				specular_scale = material.specular_scale;
			}
			
			string getName(int num,int type) {
				string ret = name;
				if(ret == "") ret = format(TR("Material %d"),num);
				if(type) {
					ret += (overlap) ? TR(" - Overlap") : TR(" - Overlay");
					
					if(parent >= 0 && parent < materials.size()) {
						if(materials[parent].name == "") ret += " - " + format(TR("Material %d"),parent);
						else ret += " - " + materials[parent].name;
					}
					
					if(overlap) materials_lb.setItemColor(num,vec4(1.0f,1.0f,0.0f,1.0f));
					else materials_lb.setItemColor(num,vec4(1.0f));
				}
				
				return ret;
			}
		};
		
		string diffuse_texture_array;
		string normal_texture_array;
		string specular_texture_array;
		
		/*
		 */
		void clear() {
			
			material_num = -1;
			materials.delete();
			
			diffuse_texture_array = "";
			normal_texture_array = "";
			specular_texture_array = "";
			
			update();
		}
		
		/*
		 */
		void update() {
			
			material_num = -1;
			materials_lb.setCallbackEnabled(GUI_CHANGED,0);
			materials_lb.clear();
			parent_cb.clear();
			parent_cb.addItem("None");
			forloop(int i = 0; materials.size()) {
				materials_lb.addItem("");
				materials_lb.setItemText(i,materials[i].getName(i,1));
				parent_cb.addItem(materials[i].getName(i,0));
			}
			materials_lb.setCallbackEnabled(GUI_CHANGED,1);
			
			diffuse_texture_array_el.setText(diffuse_texture_array);
			normal_texture_array_el.setText(normal_texture_array);
			specular_texture_array_el.setText(specular_texture_array);
			
			material_changed();
		}
		
		void update_materials() {
			
			forloop(int i = 0; landscape.getNumChilds()) {
				Node node = node_cast(landscape.getChild(i));
				if(node.getType() != NODE_OBJECT_TERRAIN) continue;
				ObjectTerrain terrain = node;
				terrain.setNumMaterials(0);
				
				int num = 0;
				foreach(Material material; materials) {
					terrain.addMaterial(material.getName(num,0));
					terrain.setMaterialTransform(num,material.transform);
					terrain.setMaterialOverlap(num,material.overlap);
					terrain.setMaterialMaskBase(num,material.mask_base);
					terrain.setMaterialParent(num,material.parent);
					terrain.setMaterialMaskValue(num,material.mask_value);
					terrain.setMaterialMaskWidth(num,material.mask_width);
					terrain.setMaterialMaskThreshold(num,material.mask_threshold);
					terrain.setMaterialDiffuseTextureName(num,material.diffuse_texture);
					terrain.setMaterialDiffuseScale(num,material.diffuse_scale);
					terrain.setMaterialNormalTextureName(num,material.normal_texture);
					terrain.setMaterialNormalScale(num,material.normal_scale);
					terrain.setMaterialSpecularTextureName(num,material.specular_texture);
					terrain.setMaterialSpecularScale(num,material.specular_scale);
					num++;
				}
				
				terrain.setDiffuseTextureArrayName(diffuse_texture_array);
				terrain.setNormalTextureArrayName(normal_texture_array);
				terrain.setSpecularTextureArrayName(specular_texture_array);
			}
		}
		
		void update_texture_arrays(Widget widget) {
			string diffuse_names[] = ();
			string normal_names[] = ();
			string specular_names[] = ();
			
			ObjectTerrain terrain;
			forloop(int i = 0; landscape.getNumChilds()) {
				Node node = node_cast(landscape.getChild(i));
				if(node.getType() != NODE_OBJECT_TERRAIN) continue;
				terrain = node;
				break;
			}
			
			if(terrain == NULL) return;
			
			foreach(Material material; materials) {
				diffuse_names.append(material.diffuse_texture);
				normal_names.append(material.normal_texture);
				specular_names.append(material.specular_texture);
			}
			
			if(widget == NULL || widget == diffuse_texture_array_el) {
				if(createDummyTextureArray(TR("Select diffuse texture array to save"),diffuse_texture_array)) {
					diffuse_texture_array_el.setText(diffuse_texture_array);
					texturesUpdateTerrainDiffuseArray(terrain,int(diffuse_texture_array_cb.getCurrentItemText()));
				}
			}
			
			if(widget == NULL || widget == normal_texture_array_el) {
				if(createDummyTextureArray(TR("Select normal texture array to save"),normal_texture_array)) {
					normal_texture_array_el.setText(normal_texture_array);
					texturesUpdateTerrainNormalArray(terrain,int(normal_texture_array_cb.getCurrentItemText()));
				}
			}
			
			if(widget == NULL || widget == specular_texture_array_el) {
				if(createDummyTextureArray(TR("Select specular texture array to save"),specular_texture_array)) {
					specular_texture_array_el.setText(specular_texture_array);
					texturesUpdateTerrainSpecularArray(terrain,int(specular_texture_array_cb.getCurrentItemText()));
				}
			}
		}
		
		/*
		 */
		void load(Xml xml) {
			
			forloop(int i = 0; xml.getNumChilds()) {
				Xml x = xml.getChild(i);
				if(x.getName() == "material") {
					Material material = new Material();
					material.name = x.getChildData("name",material.name);
					material.transform = x.getChildData("transform",material.transform);
					material.overlap = x.getChildData("overlap",material.overlap);
					material.mask_base = x.getChildData("mask_base",material.mask_base);
					material.parent = x.getChildData("parent",material.parent);
					material.mask_value = x.getChildData("mask_value",material.mask_value);
					material.mask_width = x.getChildData("mask_width",material.mask_width);
					material.mask_threshold = x.getChildData("mask_threshold",material.mask_threshold);
					material.diffuse_texture = x.getChildData("diffuse_texture",material.diffuse_texture);
					material.diffuse_scale = x.getChildData("diffuse_scale",material.diffuse_scale);
					material.normal_texture = x.getChildData("normal_texture",material.normal_texture);
					material.normal_scale = x.getChildData("normal_scale",material.normal_scale);
					material.specular_texture = x.getChildData("specular_texture",material.specular_texture);
					material.specular_scale = x.getChildData("specular_scale",material.specular_scale);
					materials.append(material);
				}
			}
			
			diffuse_texture_array = xml.getChildData("diffuse_texture_array",diffuse_texture_array);
			normal_texture_array = xml.getChildData("normal_texture_array",normal_texture_array);
			specular_texture_array = xml.getChildData("specular_texture_array",specular_texture_array);
			
			diffuse_texture_array_cb.setCurrentItem(xml.getChildData("diffuse_texture_array_size",0));
			normal_texture_array_cb.setCurrentItem(xml.getChildData("normal_texture_array_size",0));
			specular_texture_array_cb.setCurrentItem(xml.getChildData("specular_texture_array_size",0));
			
			update();
		}
		
		/*
		 */
		void save(Xml xml) {
			
			foreach(Material material; materials) {
				Xml x = xml.addChild("material");
				x.setChildData("name",material.name);
				x.setChildData("transform",material.transform);
				x.setChildData("overlap",material.overlap);
				x.setChildData("mask_base",material.mask_base);
				x.setChildData("parent",material.parent);
				x.setChildData("mask_value",material.mask_value);
				x.setChildData("mask_width",material.mask_width);
				x.setChildData("mask_threshold",material.mask_threshold);
				x.setChildData("diffuse_texture",material.diffuse_texture);
				x.setChildData("diffuse_scale",material.diffuse_scale);
				x.setChildData("normal_texture",material.normal_texture);
				x.setChildData("normal_scale",material.normal_scale);
				x.setChildData("specular_texture",material.specular_texture);
				x.setChildData("specular_scale",material.specular_scale);
			}
			
			xml.setChildData("diffuse_texture_array",diffuse_texture_array);
			xml.setChildData("normal_texture_array",normal_texture_array);
			xml.setChildData("specular_texture_array",specular_texture_array);
			
			xml.setChildData("diffuse_texture_array_size",diffuse_texture_array_cb.getCurrentItem());
			xml.setChildData("normal_texture_array_size",normal_texture_array_cb.getCurrentItem());
			xml.setChildData("specular_texture_array_size",specular_texture_array_cb.getCurrentItem());
		}
		
		/*
		 */
		Material get_material() {
			if(material_num == -1) return NULL;
			return materials[material_num];
		}
		
		/*
		 */
		void add_clicked() {
			materials.append(new Material());
			update_materials();
			update_texture_arrays(NULL);
			update();
			material_num = materials.size() - 1;
			materials_lb.setCurrentItem(material_num);
			Import::update_materials();
		}
		
		void clone_clicked() {
			materials.append(new Material(get_material()));
			update_materials();
			update_texture_arrays(NULL);
			update();
			material_num = materials.size() - 1;
			materials_lb.setCurrentItem(material_num);
			Import::update_materials();
		}
		
		void remove_clicked() {
			Material material = get_material();
			if(dialogMessageYesNo(TR("Confirm"),format(TR("Remove \"%s\" material?"),material.getName(material_num,0)))) {
				int num = material_num;
				delete materials[material_num];
				materials.remove(material_num);
				update_materials();
				update_texture_arrays(NULL);
				update();
				if(materials.size()) materials_lb.setCurrentItem(max(num - 1,0));
				Import::update_materials();
			}
		}
		
		void swap(int num_0,int num_1) {
			
			Material matertial_0 = materials[num_0];
			Material matertial_1 = materials[num_1];
			
			materials[num_1] = matertial_0;
			materials[num_0] = matertial_1;
			
			Import::swap_mask_material(num_1,num_0);
			
			forloop(int i = 0; materials.size()) {
				if(i == num_0 || i == num_1) continue;
				
				if(materials[i].parent == num_0) {
					materials[i].parent = num_1;
				}
				else if(materials[i].parent == num_1) {
					materials[i].parent = num_0;
				}
			}
			
			update_materials();
			update_texture_arrays(NULL);
			update();
			Import::update_materials();
		}
		
		void up_clicked() {
			int num = materials_lb.getCurrentItem();
			if(num == -1 || num == 0) return;
			
			swap(num - 1,num);
			materials_lb.setCurrentItem(num - 1);
		}
		
		void down_clicked() {
			int num = materials_lb.getCurrentItem();
			if(num == -1 || num == materials.size() - 1) return;
			swap(num,num + 1);
			materials_lb.setCurrentItem(num + 1);
		}
		
		/*
		 */
		void material_changed() {
			
			Widget widgets[] = (
				name_el,
				transform_el[0],
				transform_el[1],
				transform_el[2],
				transform_el[3],
				overlap_cb,
				mask_base_cb,
				parent_cb,
				mask_value_el,
				mask_width_el,
				mask_threshold_el,
				diffuse_texture_load_i,
				diffuse_texture_view_i,
				diffuse_texture_edit_i,
				diffuse_texture_el,
				diffuse_scale_el,
				normal_texture_load_i,
				normal_texture_view_i,
				normal_texture_edit_i,
				normal_texture_el,
				normal_scale_el,
				specular_texture_load_i,
				specular_texture_view_i,
				specular_texture_edit_i,
				specular_texture_el,
				specular_scale_el,
				diffuse_texture_array_cb,
				normal_texture_array_cb,
				specular_texture_array_cb,
			);
			material_num = materials_lb.getCurrentItem();
			
			if(material_num == -1) {
				
				add_b.setEnabled((landscape != NULL));
				clone_b.setEnabled(0);
				remove_b.setEnabled(0);
				up_b.setEnabled(0);
				down_b.setEnabled(0);
				
				foreach(Widget widget; widgets) {
					widget.setEnabled(0);
				}
				
				name_el.setText("");
				
				overlap_cb.setCallbackEnabled(GUI_CHANGED,0);
				overlap_cb.setChecked(0);
				overlap_cb.setCallbackEnabled(GUI_CHANGED,1);
				
				transform_el[0].setText(editorFormat(1.0f));
				transform_el[1].setText(editorFormat(1.0f));
				transform_el[2].setText(editorFormat(0.0f));
				transform_el[3].setText(editorFormat(0.0f));
				
				mask_value_el.setText(editorFormat(0.5f));
				mask_threshold_el.setText(editorFormat(0.5f));
				mask_width_el.setText(editorFormat(0.5f));
				
				diffuse_texture_el.setText("");
				diffuse_scale_el.setText(editorFormat(0.5f));
				
				normal_texture_el.setText("");
				normal_scale_el.setText(editorFormat(0.5f));
				
				specular_texture_el.setText("");
				specular_scale_el.setText(editorFormat(0.5f));
			}
			else {
				
				add_b.setEnabled(1);
				clone_b.setEnabled(1);
				remove_b.setEnabled(1);
				up_b.setEnabled(1);
				down_b.setEnabled(1);
				
				foreach(Widget widget; widgets) {
					widget.setEnabled(1);
				}
				
				Material material = get_material();
				
				name_el.setText(material.name);
				
				overlap_cb.setCallbackEnabled(GUI_CHANGED,0);
				overlap_cb.setChecked(material.overlap);
				overlap_cb.setCallbackEnabled(GUI_CHANGED,1);
				
				mask_base_cb.setCallbackEnabled(GUI_CHANGED,0);
				mask_base_cb.setChecked(material.mask_base);
				mask_base_cb.setCallbackEnabled(GUI_CHANGED,1);
				
				parent_cb.setCallbackEnabled(GUI_CHANGED,0);
				parent_cb.setCurrentItem(material.parent + 1);
				parent_cb.setCallbackEnabled(GUI_CHANGED,1);
				
				transform_el[0].setText(editorFormat(material.transform.x));
				transform_el[1].setText(editorFormat(material.transform.y));
				transform_el[2].setText(editorFormat(material.transform.z));
				transform_el[3].setText(editorFormat(material.transform.w));
				
				mask_value_el.setText(editorFormat(material.mask_value));
				mask_width_el.setText(editorFormat(material.mask_width));
				mask_threshold_el.setText(editorFormat(material.mask_threshold));
				
				diffuse_texture_el.setText(material.diffuse_texture);
				diffuse_scale_el.setText(editorFormat(material.diffuse_scale));
				
				normal_texture_el.setText(material.normal_texture);
				normal_scale_el.setText(editorFormat(material.normal_scale));
				
				specular_texture_el.setText(material.specular_texture);
				specular_scale_el.setText(editorFormat(material.specular_scale));
				
				mask_value_el.setEnabled(material.overlap);
				mask_width_el.setEnabled(material.overlap);
				mask_threshold_el.setEnabled(material.overlap);
			}
		}
		
		/*
		 */
		void name_pressed() {
			Material material = get_material();
			material.name = name_el.getText();
			name_el.setText(material.name);
			materials_lb.setItemText(material_num,material.getName(material_num,1));
			update_materials();
			Import::update_materials();
		}
		
		void overlap_changed() {
			Material material = get_material();
			material.overlap = overlap_cb.isChecked();
			materials_lb.setItemText(material_num,material.getName(material_num,1));
			material_changed();
			update_materials();
		}
		
		void mask_base_changed() {
			Material material = get_material();
			material.mask_base = mask_base_cb.isChecked();
			material_changed();
			update_materials();
		}
		
		void parent_changed() {
			Material material = get_material();
			material.parent = parent_cb.getCurrentItem() - 1;
			materials_lb.setItemText(material_num,material.getName(material_num,1));
			material_changed();
			update_materials();
		}
		
		/*
		 */
		void transform_pressed() {
			Material material = get_material();
			material.transform.x = float(transform_el[0].getText());
			material.transform.y = float(transform_el[1].getText());
			material.transform.z = float(transform_el[2].getText());
			material.transform.w = float(transform_el[3].getText());
			transform_el[0].setText(editorFormat(material.transform.x));
			transform_el[1].setText(editorFormat(material.transform.y));
			transform_el[2].setText(editorFormat(material.transform.z));
			transform_el[3].setText(editorFormat(material.transform.w));
			material_changed();
			update_materials();
		}
		
		/*
		 */
		void mask_value_pressed() {
			Material material = get_material();
			material.mask_value = saturate(float(mask_value_el.getText()));
			mask_value_el.setText(editorFormat(material.mask_value));
			material_changed();
			update_materials();
		}
		
		void mask_width_pressed() {
			Material material = get_material();
			material.mask_width = saturate(float(mask_width_el.getText()));
			mask_width_el.setText(editorFormat(material.mask_width));
			material_changed();
			update_materials();
		}
		
		void mask_threshold_pressed() {
			Material material = get_material();
			material.mask_threshold = saturate(float(mask_threshold_el.getText()));
			mask_threshold_el.setText(editorFormat(material.mask_threshold));
			material_changed();
			update_materials();
		}
		
		/*
		 */
		void diffuse_texture_pressed() {
			Material material = get_material();
			material.diffuse_texture = diffuse_texture_el.getText();
			diffuse_texture_el.setText(material.diffuse_texture);
			update_materials();
			update_texture_arrays(diffuse_texture_array_el);
		}
		
		void diffuse_texture_load_clicked() {
			Material material = get_material();
			string name = material.diffuse_texture;
			if(texturesDialogImage(TR("Select material diffuse texture"),"diffuse",name)) {
				if(material.diffuse_texture != name) {
					material.diffuse_texture = name;
					material_changed();
					update_materials();
					update_texture_arrays(diffuse_texture_array_el);
				}
			}
		}
		
		void diffuse_texture_view_clicked() {
			Material material = get_material();
			if(strlen(material.diffuse_texture)) dialogImage(TR("Material diffuse texture"),material.diffuse_texture);
		}
		
		void diffuse_texture_edit_clicked() {
			Material material = get_material();
			if(strlen(material.diffuse_texture) == 0) return;
			if(dialogTexture(TR("diffuse texture"),"diffuse",material.diffuse_texture,1)) {
				update_texture_arrays(diffuse_texture_array_el);
			}
		}
		
		void diffuse_scale_pressed() {
			Material material = get_material();
			material.diffuse_scale = saturate(float(diffuse_scale_el.getText()));
			diffuse_scale_el.setText(editorFormat(material.diffuse_scale));
			material_changed();
			update_materials();
		}
		
		/*
		 */
		void normal_texture_pressed() {
			Material material = get_material();
			material.normal_texture = normal_texture_el.getText();
			normal_texture_el.setText(material.normal_texture);
			update_materials();
			update_texture_arrays(normal_texture_array_el);
		}
		
		void normal_texture_load_clicked() {
			Material material = get_material();
			string name = material.normal_texture;
			if(texturesDialogImage(TR("Select material normal texture"),"normal",name)) {
				if(material.normal_texture != name) {
					material.normal_texture = name;
					material_changed();
					update_materials();
					update_texture_arrays(normal_texture_array_el);
				}
			}
		}
		
		void normal_texture_view_clicked() {
			Material material = get_material();
			if(strlen(material.normal_texture)) dialogImage(TR("Material normal texture"),material.normal_texture);
		}
		
		void normal_texture_edit_clicked() {
			Material material = get_material();
			if(strlen(material.normal_texture) == 0) return;
			if(dialogTexture(TR("normal texture"),"normal",material.normal_texture,1)) {
				update_texture_arrays(normal_texture_array_el);
			}
		}
		
		void normal_scale_pressed() {
			Material material = get_material();
			material.normal_scale = saturate(float(normal_scale_el.getText()));
			normal_scale_el.setText(editorFormat(material.normal_scale));
			material_changed();
			update_materials();
		}
		
		/*
		 */
		void specular_texture_pressed() {
			Material material = get_material();
			material.specular_texture = specular_texture_el.getText();
			specular_texture_el.setText(material.specular_texture);
			update_materials();
			update_texture_arrays(specular_texture_array_el);
		}
		
		void specular_texture_load_clicked() {
			Material material = get_material();
			string name = material.specular_texture;
			if(texturesDialogImage(TR("Select material specular texture"),"specular",name)) {
				if(material.specular_texture != name) {
					material.specular_texture = name;
					material_changed();
					update_materials();
					update_texture_arrays(specular_texture_array_el);
				}
			}
		}
		
		void specular_texture_view_clicked() {
			Material material = get_material();
			if(strlen(material.specular_texture)) dialogImage(TR("Material specular texture"),material.specular_texture);
		}
		
		void specular_texture_edit_clicked() {
			Material material = get_material();
			if(strlen(material.specular_texture) == 0) return;
			if(dialogTexture(TR("specular texture"),"specular",material.specular_texture,1)) {
				update_texture_arrays(specular_texture_array_el);
			}
		}
		
		void specular_scale_pressed() {
			Material material = get_material();
			material.specular_scale = saturate(float(specular_scale_el.getText()));
			specular_scale_el.setText(editorFormat(material.specular_scale));
			material_changed();
			update_materials();
		}
		
		/*
		 */
		void diffuse_texture_array_pressed() {
			diffuse_texture_array = diffuse_texture_array_el.getText();
			diffuse_texture_array_el.setText(diffuse_texture_array);
			update_materials();
			update_texture_arrays(diffuse_texture_array_el);
		}
		
		void diffuse_texture_array_load_clicked() {
			string name = diffuse_texture_array;
			if(texturesDialogImage(TR("Select diffuse texture array"),"diffuse",name)) {
				name = setExtensionImage(name,"dds");
				
				if(diffuse_texture_array != name) {
					diffuse_texture_array = name;
					diffuse_texture_array_el.setText(diffuse_texture_array);
					update_materials();
					update_texture_arrays(diffuse_texture_array_el);
				}
			}
		}
		
		void diffuse_texture_array_view_clicked() {
			if(strlen(diffuse_texture_array)) dialogImage(TR("Material diffuse texture"),diffuse_texture_array);
		}
		
		void diffuse_texture_array_clear_clicked() {
			if(dialogMessageYesNo(TR("Confirm"),TR("Clear materials diffuse texture?"))) {
				diffuse_texture_array = "";
				diffuse_texture_array_el.setText("");
				update_materials();
			}
		}
		
		/*
		 */
		void normal_texture_array_pressed() {
			normal_texture_array = normal_texture_array_el.getText();
			normal_texture_array_el.setText(normal_texture_array);
			update_materials();
			update_texture_arrays(normal_texture_array_el);
		}
		
		void normal_texture_array_load_clicked() {
			string name = normal_texture_array;
			if(texturesDialogImage(TR("Select normal texture array"),"normal",name)) {
				name = setExtensionImage(name,"dds");
				
				if(normal_texture_array != name) {
					normal_texture_array = name;
					normal_texture_array_el.setText(normal_texture_array);
					update_materials();
					update_texture_arrays(normal_texture_array_el);
				}
			}
		}
		
		void normal_texture_array_view_clicked() {
			if(strlen(normal_texture_array)) dialogImage(TR("Material normal texture"),normal_texture_array);
		}
		
		void normal_texture_array_clear_clicked() {
			if(dialogMessageYesNo(TR("Confirm"),TR("Clear materials normal texture?"))) {
				normal_texture_array = "";
				normal_texture_array_el.setText("");
				update_materials();
			}
		}
		
		/*
		 */
		void specular_texture_array_pressed() {
			specular_texture_array = specular_texture_array_el.getText();
			specular_texture_array_el.setText(specular_texture_array);
			update_materials();
			update_texture_arrays(specular_texture_array_el);
		}
		
		void specular_texture_array_load_clicked() {
			string name = specular_texture_array;
			if(texturesDialogImage(TR("Select normal texture array"),"normal",name)) {
				name = setExtensionImage(name,"dds");
				
				if(specular_texture_array != name) {
					specular_texture_array = name;
					specular_texture_array_el.setText(specular_texture_array);
					update_materials();
					update_texture_arrays(specular_texture_array_el);
				}
			}
		}
		
		void specular_texture_array_view_clicked() {
			if(strlen(specular_texture_array)) dialogImage(TR("Material specular texture"),specular_texture_array);
		}
		
		void specular_texture_array_clear_clicked() {
			if(dialogMessageYesNo(TR("Confirm"),TR("Clear materials specular texture?"))) {
				specular_texture_array = "";
				specular_texture_array_el.setText("");
				update_materials();
			}
		}
	}
	
	/**************************************************************************\
	*
	* Import
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Import {
		
		/*
		 */
		enum {
			
			UNIT_TERRAINS = 0,
			UNIT_SURFACES = 1,
			UNIT_PIXELS = 2,
			UNIT_SOURCE = 3,
			
			NUM_MATERIALS = 16,
		};
		
		/*
		 */
		EditLine import_offset_x_el;
		EditLine import_offset_y_el;
		EditLine import_size_x_el;
		EditLine import_size_y_el;
		ComboBox import_unit_x_cb;
		ComboBox import_unit_y_cb;
		
		int import_offset_x;
		int import_offset_y;
		int import_size_x;
		int import_size_y;
		int import_unit_x;
		int import_unit_y;
		
		/*
		 */
		EditLine source_offset_x_el;
		EditLine source_offset_y_el;
		EditLine source_size_x_el;
		EditLine source_size_y_el;
		
		int source_offset_x;
		int source_offset_y;
		int source_size_x;
		int source_size_y;
		
		/*
		 */
		CheckBox height_import_cb;
		CheckBox diffuse_import_cb;
		CheckBox normal_import_cb;
		
		EditLine height_name_el;
		EditLine height_scale_el;
		EditLine diffuse_name_el;
		EditLine normal_name_el;
		
		int height_import;
		int diffuse_import;
		int normal_import;
		
		string height_name;
		string diffuse_name;
		string normal_name;
		
		float height_scale;
		
		/*
		 */
		CheckBox mask_import_cb[NUM_MATERIALS];
		EditLine mask_name_el[NUM_MATERIALS];
		ComboBox mask_material_cb[NUM_MATERIALS];
		
		int mask_import[NUM_MATERIALS];
		string mask_name[NUM_MATERIALS];
		int mask_material[NUM_MATERIALS];
		
		/*
		 */
		CheckBox flip_y_cb;
		CheckBox clean_cb;
		CheckBox fast_mode_cb;
		
		int flip_y;
		int clean;
		int fast_mode;
		
		Button import_b;
		
		Slider num_channels_sl;
		
		/*
		 */
		void clear() {
			
			import_offset_x = 0;
			import_offset_y = 0;
			import_size_x = 1;
			import_size_y = 1;
			import_unit_x = UNIT_TERRAINS;
			import_unit_y = UNIT_TERRAINS;
			
			source_offset_x = 0;
			source_offset_y = 0;
			source_size_x = 1;
			source_size_y = 1;
			
			height_import = 0;
			height_name = "height_%xx_%xy.png";
			height_scale = 1.0f;
			
			diffuse_import = 0;
			diffuse_name = "diffuse_%xx_%xy.png";
			
			normal_import = 0;
			normal_name = "normal_%xx_%xy.png";
			
			forloop(int i = 0; NUM_MATERIALS) {
				mask_import[i] = 0;
				mask_name[i] = "";
				mask_material[i] = -1;
			}
			
			flip_y = 0;
			clean = 0;
			fast_mode = 0;
			
			update();
		}
		
		/*
		 */
		void update() {
			
			import_offset_x_el.setText(string(import_offset_x));
			import_offset_y_el.setText(string(import_offset_y));
			import_size_x_el.setText(string(import_size_x));
			import_size_y_el.setText(string(import_size_y));
			import_unit_x_cb.setCurrentItem(import_unit_x);
			import_unit_y_cb.setCurrentItem(import_unit_y);
			
			source_offset_x_el.setText(string(source_offset_x));
			source_offset_y_el.setText(string(source_offset_y));
			source_size_x_el.setText(string(source_size_x));
			source_size_y_el.setText(string(source_size_y));
			
			height_import_cb.setChecked(height_import);
			height_name_el.setText(height_name);
			height_scale_el.setText(string(height_scale));
			
			diffuse_import_cb.setChecked(diffuse_import);
			diffuse_name_el.setText(diffuse_name);
			
			normal_import_cb.setChecked(normal_import);
			normal_name_el.setText(normal_name);
			
			update_materials();
			
			forloop(int i = 0; NUM_MATERIALS) {
				mask_import_cb[i].setChecked(mask_import[i]);
				mask_name_el[i].setText(mask_name[i]);
				mask_material_cb[i].setCurrentItem(mask_material[i]);
			}
			
			flip_y_cb.setChecked(flip_y);
			clean_cb.setChecked(clean);
			fast_mode_cb.setChecked(fast_mode);
			
			Widget widgets[] = (
				import_offset_x_el,
				import_offset_y_el,
				import_size_x_el,
				import_size_y_el,
				import_unit_x_cb,
				import_unit_y_cb,
				source_offset_x_el,
				source_offset_y_el,
				source_size_x_el,
				source_size_y_el,
				height_import_cb,
				height_name_el,
				height_scale_el,
				diffuse_import_cb,
				diffuse_name_el,
				normal_import_cb,
				normal_name_el,
				flip_y_cb,
				clean_cb,
				fast_mode_cb,
				import_b,
				num_channels_sl,
			);
			
			if(landscape == NULL) {
				
				foreach(Widget widget; widgets) {
					widget.setEnabled(0);
				}
				
				forloop(int i = 0; NUM_MATERIALS) {
					mask_import_cb[i].setEnabled(0);
					mask_name_el[i].setEnabled(0);
					mask_material_cb[i].setEnabled(0);
				}
				
			} else {
				
				foreach(Widget widget; widgets) {
					widget.setEnabled(1);
				}
				
				forloop(int i = 0; NUM_MATERIALS) {
					mask_import_cb[i].setEnabled(1);
					mask_name_el[i].setEnabled(1);
					mask_material_cb[i].setEnabled(1);
				}
				
				height_import_changed();
				diffuse_import_changed();
				normal_import_changed();
				mask_import_changed();
			}
		}
		
		void update_materials() {
			
			forloop(int i = 0; NUM_MATERIALS) {
				mask_material_cb[i].setCallbackEnabled(GUI_CHANGED,0);
				mask_material_cb[i].clear();
				forloop(int j = 0; Materials::materials.size()) {
					Materials::Material material = Materials::materials[j];
					mask_material_cb[i].addItem(material.getName(j,0));
				}
				mask_material_cb[i].setCurrentItem(mask_material[i]);
				mask_material_cb[i].setCallbackEnabled(GUI_CHANGED,1);
			}
		}
		
		/*
		 */
		void load(Xml xml) {
			
			import_offset_x = xml.getChildData("import_offset_x",import_offset_x);
			import_offset_y = xml.getChildData("import_offset_y",import_offset_y);
			import_size_x = xml.getChildData("import_size_x",import_size_x);
			import_size_y = xml.getChildData("import_size_y",import_size_y);
			import_unit_x = xml.getChildData("import_unit_x",import_unit_x);
			import_unit_y = xml.getChildData("import_unit_y",import_unit_y);
			
			source_offset_x = xml.getChildData("source_offset_x",source_offset_x);
			source_offset_y = xml.getChildData("source_offset_y",source_offset_y);
			source_size_x = xml.getChildData("source_size_x",source_size_x);
			source_size_y = xml.getChildData("source_size_y",source_size_y);
			
			height_import = xml.getChildData("height_import",height_import);
			height_name = xml.getChildData("height_name",height_name);
			height_scale = xml.getChildData("height_scale",height_scale);
			
			diffuse_import = xml.getChildData("diffuse_import",diffuse_import);
			diffuse_name = xml.getChildData("diffuse_name",diffuse_name);
			
			normal_import = xml.getChildData("normal_import",normal_import);
			normal_name = xml.getChildData("normal_name",normal_name);
			
			forloop(int i = 0; NUM_MATERIALS) {
				mask_import[i] = xml.getChildData("mask_import_" + i,mask_import[i]);
				mask_name[i] = xml.getChildData("mask_name_" + i,mask_name[i]);
				mask_material[i] = xml.getChildData("mask_material_" + i,mask_material[i]);
			}
			
			flip_y = xml.getChildData("flip_y",flip_y);
			clean = xml.getChildData("clean",clean);
			fast_mode = xml.getChildData("fast_mode",fast_mode);
			
			num_channels_sl.setValue(xml.getChildData("num_channels",4));
			
			update();
		}
		
		/*
		 */
		void save(Xml xml) {
			
			xml.setChildData("import_offset_x",import_offset_x);
			xml.setChildData("import_offset_y",import_offset_y);
			xml.setChildData("import_size_x",import_size_x);
			xml.setChildData("import_size_y",import_size_y);
			xml.setChildData("import_unit_x",import_unit_x);
			xml.setChildData("import_unit_y",import_unit_y);
			
			xml.setChildData("source_offset_x",source_offset_x);
			xml.setChildData("source_offset_y",source_offset_y);
			xml.setChildData("source_size_x",source_size_x);
			xml.setChildData("source_size_y",source_size_y);
			
			xml.setChildData("height_import",height_import);
			xml.setChildData("height_name",height_name);
			xml.setChildData("height_scale",height_scale);
			
			xml.setChildData("diffuse_import",diffuse_import);
			xml.setChildData("diffuse_name",diffuse_name);
			
			xml.setChildData("normal_import",normal_import);
			xml.setChildData("normal_name",normal_name);
			
			forloop(int i = 0; NUM_MATERIALS) {
				xml.setChildData("mask_import_" + i,mask_import[i]);
				xml.setChildData("mask_name_" + i,mask_name[i]);
				xml.setChildData("mask_material_" + i,mask_material[i]);
			}
			
			xml.setChildData("flip_y",flip_y);
			xml.setChildData("clean",clean);
			xml.setChildData("fast_mode",fast_mode);
			
			xml.setChildData("num_channels",num_channels_sl.getValue());
		}
		
		/*
		 */
		void import_offset_x_pressed() {
			import_offset_x = int(import_offset_x_el.getText());
			import_offset_x_el.setText(string(import_offset_x));
		}
		
		void import_offset_y_pressed() {
			import_offset_y = int(import_offset_y_el.getText());
			import_offset_y_el.setText(string(import_offset_y));
		}
		
		void import_size_x_pressed() {
			import_size_x = max(int(import_size_x_el.getText()),1);
			import_size_x_el.setText(string(import_size_x));
		}
		
		void import_size_y_pressed() {
			import_size_y = max(int(import_size_y_el.getText()),1);
			import_size_y_el.setText(string(import_size_y));
		}
		
		void import_unit_x_changed() {
			import_unit_x = import_unit_x_cb.getCurrentItem();
		}
		
		void import_unit_y_changed() {
			import_unit_y = import_unit_y_cb.getCurrentItem();
		}
		
		/*
		 */
		void source_offset_x_pressed() {
			source_offset_x = int(source_offset_x_el.getText());
			source_offset_x_el.setText(string(source_offset_x));
		}
		
		void source_offset_y_pressed() {
			source_offset_y = int(source_offset_y_el.getText());
			source_offset_y_el.setText(string(source_offset_y));
		}
		
		void source_size_x_pressed() {
			source_size_x = max(int(source_size_x_el.getText()),1);
			source_size_x_el.setText(string(source_size_x));
		}
		
		void source_size_y_pressed() {
			source_size_y = max(int(source_size_y_el.getText()),1);
			source_size_y_el.setText(string(source_size_y));
		}
		
		/*
		 */
		void height_import_changed() {
			height_import = height_import_cb.isChecked();
			height_name_el.setEnabled(height_import);
		}
		
		void height_name_pressed() {
			height_name = height_name_el.getText();
			height_name_el.setText(height_name);
		}
		
		void height_name_double_clicked() {
			if(dialogFileImage(TR("Select height texture"),height_name)) {
				height_name_el.setText(height_name);
			}
		}
		
		void height_scale_pressed() {
			height_scale = float(height_scale_el.getText());
			height_scale_el.setText(editorFormat(height_scale));
		}
		
		/*
		 */
		void diffuse_import_changed() {
			diffuse_import = diffuse_import_cb.isChecked();
			diffuse_name_el.setEnabled(diffuse_import);
		}
		
		void diffuse_name_pressed() {
			diffuse_name = diffuse_name_el.getText();
			diffuse_name_el.setText(diffuse_name);
		}
		
		void diffuse_name_double_clicked() {
			if(dialogFileImage(TR("Select diffuse texture"),diffuse_name)) {
				diffuse_name_el.setText(diffuse_name);
			}
		}
		
		/*
		 */
		void normal_import_changed() {
			normal_import = normal_import_cb.isChecked();
			normal_name_el.setEnabled(normal_import);
		}
		
		void normal_name_pressed() {
			normal_name = normal_name_el.getText();
			normal_name_el.setText(normal_name);
		}
		
		void normal_name_double_clicked() {
			if(dialogFileImage(TR("Select normal texture"),normal_name)) {
				normal_name_el.setText(normal_name);
			}
		}
		
		/*
		 */
		void mask_import_changed() {
			forloop(int i = 0; NUM_MATERIALS) {
				mask_import[i] = mask_import_cb[i].isChecked();
				mask_name_el[i].setEnabled(mask_import[i]);
				mask_material_cb[i].setEnabled(mask_import[i]);
			}
		}
		
		void mask_name_pressed() {
			forloop(int i = 0; NUM_MATERIALS) {
				mask_name[i] = mask_name_el[i].getText();
				mask_name_el[i].setText(mask_name[i]);
			}
		}
		
		void mask_name_double_clicked(string num) {
			num = int(num);
			string name = mask_name[num];
			if(dialogFileImage(TR("Select mask texture"),name)) {
				mask_name_el[num].setText(name);
				mask_name[num] = name;
			}
		}
		
		void mask_material_changed() {
			forloop(int i = 0; NUM_MATERIALS) {
				mask_material[i] = mask_material_cb[i].getCurrentItem();
			}
		}
		
		/*
		 */
		void flip_y_changed() {
			flip_y = flip_y_cb.isChecked();
		}
		
		void clean_changed() {
			clean = clean_cb.isChecked();
		}
		
		void fast_mode_changed() {
			fast_mode = fast_mode_cb.isChecked();
		}
		
		void swap_mask_material(int m_0,int m_1) {
			forloop(int i = 0; NUM_MATERIALS) {
				if(mask_material[i] == m_0) mask_material[i] = m_1;
				else if(mask_material[i] == m_1) mask_material[i] = m_0;
			}
		}
		
		/**********************************************************************\
		*
		* Import height
		*
		\**********************************************************************/
		
		/*
		 */
		int get_height_x0() {
			int x0 = import_offset_x;
			if(import_unit_x == UNIT_SURFACES) x0 *= SURFACE_SIZE;
			if(import_unit_x == UNIT_TERRAINS) x0 *= (Common::terrain_size - 1);
			if(import_unit_x == UNIT_SOURCE) x0 = Common::terrain_size_x * (Common::terrain_size - 1) * (source_offset_x - import_offset_x) / import_size_x;
			return clamp(x0,0,Common::terrain_size_x * (Common::terrain_size - 1));
		}
		
		int get_height_x1() {
			int x1 = import_offset_x + import_size_x;
			if(import_unit_x == UNIT_SURFACES) x1 *= SURFACE_SIZE;
			if(import_unit_x == UNIT_TERRAINS) x1 *= (Common::terrain_size - 1);
			if(import_unit_x == UNIT_SOURCE) x1 = Common::terrain_size_x * (Common::terrain_size - 1) * (source_offset_x - import_offset_x + source_size_x) / import_size_x;
			return clamp(x1,0,Common::terrain_size_x * (Common::terrain_size - 1));
		}
		
		int get_height_y0() {
			int y0 = import_offset_y;
			if(import_unit_y == UNIT_SURFACES) y0 *= SURFACE_SIZE;
			if(import_unit_y == UNIT_TERRAINS) y0 *= (Common::terrain_size - 1);
			if(import_unit_y == UNIT_SOURCE) y0 = Common::terrain_size_y * (Common::terrain_size - 1) * (source_offset_y - import_offset_y) / import_size_y;
			return clamp(y0,0,Common::terrain_size_y * (Common::terrain_size - 1));
		}
		
		int get_height_y1() {
			int y1 = import_offset_y + import_size_y;
			if(import_unit_y == UNIT_SURFACES) y1 *= SURFACE_SIZE;
			if(import_unit_y == UNIT_TERRAINS) y1 *= (Common::terrain_size - 1);
			if(import_unit_y == UNIT_SOURCE) y1 = Common::terrain_size_y * (Common::terrain_size - 1) * (source_offset_y - import_offset_y + source_size_y) / import_size_y;
			return clamp(y1,0,Common::terrain_size_y * (Common::terrain_size - 1));
		}
		
		/*
		 */
		int import_height(Async async) {
			
			log.message("Importing terrain height\n");
			
			// begin progress dialog
			dialogProgressBegin(TR("Importing Terrain Height..."));
			
			// load height image
			if(TiledImage::load(height_name,source_offset_x,source_offset_y,source_size_x,source_size_y,flip_y) == 0) {
				dialogProgressEnd();
				return 0;
			}
			
			// height bounds
			int height_x0 = get_height_x0();
			int height_x1 = get_height_x1();
			int height_y0 = get_height_y0();
			int height_y1 = get_height_y1();
			
			// height size
			int height_size_x = height_x1 - height_x0;
			int height_size_y = height_y1 - height_y0;
			int height_size = Common::terrain_size - 1;
			
			// import terrain height
			for(int height_y = 0; height_y < height_size_y;) {
				int Y = height_y0 + height_y;
				int height_offset_y = Y % height_size;
				int height_height = height_size - height_offset_y;
				if(height_y + height_height > height_size_y) height_height = height_size_y - height_y;
				
				for(int height_x = 0; height_x < height_size_x;) {
					int X = height_x0 + height_x;
					int height_offset_x = X % height_size;
					int height_width = height_size - height_offset_x;
					if(height_x + height_width > height_size_x) height_width = height_size_x - height_x;
					
					// update terrain height
					ObjectTerrain terrain = Common::get_terrain(X / height_size,Y / height_size);
					if(terrain != NULL) {
						Image image = TiledImage::getImage(async,height_size_x,height_size_y,height_x,height_y,height_width + 1,height_height + 1,IMAGE_FORMAT_R16);
						if(image == NULL) {
							dialogProgressEnd();
							TiledImage::clear();
							return 0;
						}
						
						log.message("Creating \"%s\" terrain\n",terrain.getTerrainName());
						terrain.setHeights(height_offset_x,height_offset_y,image,height_scale);
						if(terrain.saveTerrain(systemEditorPath(terrain.getTerrainName())) == 0) {
							dialogMessageOk(TR("Error"),format(TR("Can't save \"%s\" terrain"),terrain.getTerrainName()));
							dialogProgressEnd();
							TiledImage::clear();
							return 0;
						}
						delete image;
					}
					
					height_x += height_width;
					
					// update progress dialog
					int progress = int((long(height_size_x) * height_y + long(height_height) * height_x) * 100 / (long(height_size_x) * height_size_y));
					if(dialogProgressUpdate(progress) == 0) {
						dialogProgressEnd();
						TiledImage::clear();
						return 0;
					}
				}
				height_y += height_height;
			}
			
			TiledImage::clear();
			
			// end progress dialog
			dialogProgressEnd();
			
			update_neighborhood(Common::terrain_size_x,Common::terrain_size_y);
			
			return 1;
		}
		
		/**********************************************************************\
		*
		* Import diffuse
		*
		\**********************************************************************/
		
		/*
		 */
		int set_diffuse(Async async,ObjectTerrain terrain,string name,int offset_x,int offset_y,Image image) {
			
			Image surface = new Image();
			surface.create2D(TEXTURE_SIZE,TEXTURE_SIZE,IMAGE_FORMAT_R8);
			surface.set(0,MASK_R);
			
			// check paths
			forloop(int y = 0; terrain.getSurfacesY()) {
				forloop(int x = 0; terrain.getSurfacesX()) {
					string surface_name = terrain.getSurfaceDiffuseTextureName(x,y);
					
					if(strlen(surface_name) == 0) {
						surface_name = Common::landscape_path + "/" + name + "/" + get_name(Common::diffuse_name,x,y);
						mkdir(dirname(savePath(surface_name)),1);
						surface.save(surface_name);
						terrain.setSurfaceDiffuseTextureName(x,y,surface_name);
					}
					
				}
			}
			
			// create multiple diffuse textures
			int progress_offset = 0;
			
			int image_width = image.getWidth();
			int image_height = image.getHeight();
			
			int surface_width = Common::diffuse_size / terrain.getSurfacesX();
			int surface_height = Common::diffuse_size / terrain.getSurfacesY();
			
			// partial diffuse update
			if(offset_x || offset_y || image_width != Common::diffuse_size || image_height != Common::diffuse_size) {
				
				progress_offset = 50;
				
				Image diffuse = new Image();
				diffuse.create2D(Common::diffuse_size,Common::diffuse_size,image.getFormat());
				diffuse.set(0,DIFFUSE_R);
				diffuse.set(1,DIFFUSE_G);
				diffuse.set(2,DIFFUSE_B);
				if(image.getFormat() == IMAGE_FORMAT_RGBA8) diffuse.set(3,DIFFUSE_A);
				
				log.message("Loading \"%s\" diffuse textures\n",terrain.getTerrainName());
				
				// load diffuse textures
				forloop(int y = 0; terrain.getSurfacesY()) {
					forloop(int x = 0; terrain.getSurfacesX()) {
						
						string name = terrain.getSurfaceDiffuseTextureName(x,y);
						
						// load surface diffuse texture
						int id = async.run(surface,"load",name);
						while(async.isRunning(id)) {
							usleep(WAIT_TIME);
							wait;
						}
						if(async.getResult(id)) {
							if(surface.getWidth() == surface_width && surface.getHeight() == surface_height && surface.convertToFormat(diffuse.getFormat())) {
								diffuse.copy(surface,surface_width * x,surface_height * y,0,0,surface_width,surface_height);
							}
						} else {
							log.error("Landscape::Import::set_diffuse(): can't load \"%s\" image\n",name);
							delete surface;
							delete diffuse;
							return 0;
						}
						
						// update progress dialog
						int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
						if(dialogProgressUpdate2(progress * progress_offset / 100) == 0) {
							delete surface;
							delete diffuse;
							return 0;
						}
					}
				}
				
				diffuse.copy(image,offset_x,offset_y,0,0,image_width,image_height);
				delete image;
				
				image = diffuse;
			}
			
			log.message("Creating \"%s\" diffuse textures\n",terrain.getTerrainName());
			
			// create diffuse textures
			forloop(int y = 0; terrain.getSurfacesY()) {
				if(surface_height * y < offset_y) continue;
				if(surface_height * y >= offset_y + image_height) continue;
				
				forloop(int x = 0; terrain.getSurfacesX()) {
					if(surface_width * x < offset_x) continue;
					if(surface_width * x >= offset_x + image_width) continue;
					
					// pixel border for seams removing
					int surface_copy_width = surface_width;
					int surface_copy_height = surface_height;
					if(x != terrain.getSurfacesX() - 1) surface_copy_width++;
					if(y != terrain.getSurfacesY() - 1) surface_copy_height++;
					surface.create2D(surface_copy_width,surface_copy_height,image.getFormat());
					
					// copy surface diffuse texture
					surface.copy(image,0,0,surface_width * x,surface_height * y,surface_copy_width,surface_copy_height);
					surface.resize(surface_width,surface_height);
					
					string name = terrain.getSurfaceDiffuseTextureName(x,y);
					
					// save surface diffuse texture
					int id = async.run(surface,"save",name);
					while(async.isRunning(id)) {
						usleep(WAIT_TIME);
						wait;
					}
					if(async.getResult(id) == 0) {
						log.error("Landscape::Import::set_diffuse(): can't save \"%s\" image\n",name);
						delete surface;
						return 0;
					}
					
					// update progress dialog
					int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
					if(dialogProgressUpdate2(progress * (100 - progress_offset) / 100 + progress_offset) == 0) {
						delete surface;
						return 0;
					}
				}
			}
			
			delete surface;
			
			log.message("Creating \"%s\" texture\n",terrain.getDiffuseTextureName());
			
			Image diffuse = new Image();
			diffuse.load(terrain.getDiffuseTextureName());
			
			diffuse.resize(Common::diffuse_coarse,Common::diffuse_coarse);
			image.resize(Common::diffuse_coarse,Common::diffuse_coarse);
			
			diffuse.copy(image,offset_x,offset_y,0,0,image.getHeight(),image.getWidth(),1);
			
			diffuse.save(terrain.getDiffuseTextureName());
			
			delete diffuse;
			
			return 1;
		}
		
		/*
		 */
		int import_diffuse(Async async) {
			
			if(check_path(diffuse_name) == 0) {
				dialogMessageOk(TR("Error"),TR("Diffuse texture you want to import is already used by landscape"));
				return 0;
			}
			
			log.message("Importing terrain diffuse\n");
			
			// begin progress dialog
			dialogProgressBegin(TR("Importing Terrain Diffuse..."));
			
			// load diffuse image
			if(TiledImage::load(diffuse_name,source_offset_x,source_offset_y,source_size_x,source_size_y,flip_y) == 0) {
				dialogProgressEnd();
				return 0;
			}
			
			// height bounds
			int height_x0 = get_height_x0();
			int height_x1 = get_height_x1();
			int height_y0 = get_height_y0();
			int height_y1 = get_height_y1();
			
			// height size
			int height_size_x = height_x1 - height_x0;
			int height_size_y = height_y1 - height_y0;
			int height_size = Common::terrain_size - 1;
			
			// diffuse size
			int diffuse_size = max(Common::diffuse_size,Common::diffuse_coarse);
			int diffuse_size_x = height_size_x * diffuse_size / height_size;
			int diffuse_size_y = height_size_y * diffuse_size / height_size;
			
			// import terrain diffuse
			for(int height_y = 0; height_y < height_size_y;) {
				int Y = height_y0 + height_y;
				int height_offset_y = Y % height_size;
				int height_height = height_size - height_offset_y;
				int diffuse_y = height_y * diffuse_size / height_size;
				int diffuse_height = height_height * diffuse_size / height_size;
				if(diffuse_y + diffuse_height > diffuse_size_y) diffuse_height = diffuse_size_y - diffuse_y;
				
				for(int height_x = 0; height_x < height_size_x;) {
					int X = height_x0 + height_x;
					int height_offset_x = X % height_size;
					int height_width = height_size - height_offset_x;
					int diffuse_x = height_x * diffuse_size / height_size;
					int diffuse_width = height_width * diffuse_size / height_size;
					if(diffuse_x + diffuse_width > diffuse_size_x) diffuse_width = diffuse_size_x - diffuse_x;
					
					// update terrain diffuse
					ObjectTerrain terrain = Common::get_terrain(X / height_size,Y / height_size);
					string name = get_name(Common::terrain_name,X / height_size,Y / height_size);
					
					if(terrain != NULL) {
						Image image = TiledImage::getImage(async,diffuse_size_x,diffuse_size_y,diffuse_x,diffuse_y,diffuse_width,diffuse_height,TiledImage::getFormat());
						
						if(image == NULL) {
							dialogProgressEnd();
							TiledImage::clear();
							return 0;
						}
						int diffuse_offset_x = height_offset_x * diffuse_size / height_size;
						int diffuse_offset_y = height_offset_y * diffuse_size / height_size;
						if(set_diffuse(async,terrain,name,diffuse_offset_x,diffuse_offset_y,image) == 0) {
							dialogProgressEnd();
							TiledImage::clear();
							delete image;
							return 0;
						}
						delete image;
					}
					
					height_x += height_width;
					
					// update progress dialog
					int progress = int((long(height_size_x) * height_y + long(height_height) * height_x) * 100 / (long(height_size_x) * height_size_y));
					if(dialogProgressUpdate(progress) == 0) {
						dialogProgressEnd();
						TiledImage::clear();
						return 0;
					}
				}
				height_y += height_height;
			}
			
			TiledImage::clear();
			
			// end progress dialog
			dialogProgressEnd();
			
			return 1;
		}
		
		/*
		 */
		int fast_import_diffuse() {
			if(diffuse_import == 0) return;
			dialogProgressBegin(TR("Importing Terrain Diffuse..."),0);
			
			int import_x0 = Nodes::get_import_x0();
			int import_x1 = Nodes::get_import_x1();
			int import_y0 = Nodes::get_import_y0();
			int import_y1 = Nodes::get_import_y1();
			
			int import_size_x = import_x1 - import_x0;
			int import_size_y = import_y1 - import_y0;
			
			forloop(int X = import_x0; import_x1) {
				forloop(int Y = import_y0; import_y1) {
					int progress = ((X * import_size_y + Y) * 100) / (import_size_x * Common::terrain_size_y);
					dialogProgressUpdate(progress);
					
					ObjectTerrain terrain = Common::get_terrain(X,Y);
					string name = Common::landscape_path + "/" + get_name(Common::terrain_name,X,Y);
					
					int terrain_size_surface_x = terrain.getSurfacesX();
					int terrain_size_surface_y = terrain.getSurfacesY();
					
					int surface_w = Common::diffuse_size / terrain_size_surface_x;
					int surface_h = Common::diffuse_size / terrain_size_surface_y;
					
					int size_source_x = source_size_x / Common::terrain_size_x;
					int size_source_y = source_size_y / Common::terrain_size_y;
					
					int size_surface_x = terrain_size_surface_x / size_source_x;
					int size_surface_y = terrain_size_surface_y / size_source_y;
					
					Image coarse = new Image();
					coarse.create2D(Common::diffuse_coarse,Common::diffuse_coarse,IMAGE_FORMAT_RGBA8);
					
					Image surface = new Image();
					surface.create2D(surface_w,surface_h,IMAGE_FORMAT_RGBA8);
					
					Image source = new Image();
					
					forloop(int x_source = size_source_x * X; size_source_x * (X + 1)) {
						forloop(int y_source = size_source_y * Y; size_source_y * (Y + 1)) {
							
							int progress = (size_source_y * (x_source - size_source_x * X) + y_source - size_source_y * Y) * 100 / (size_source_x * size_source_y);
							dialogProgressUpdate2(progress);
							
							source.load(get_name(diffuse_name,x_source,y_source));
							source.convertToFormat(IMAGE_FORMAT_RGBA8);
							source.resize(surface_w * size_surface_x,surface_h * size_surface_y);
							source.flipY();
							
							forloop(int x = 0; size_surface_x) {
								forloop(int y = 0; size_surface_y) {
									int x_surface = x_source * size_surface_x + x - X * terrain_size_surface_x;
									int y_surface = y_source * size_surface_y + y - Y * terrain_size_surface_y;
									
									string surface_name = name + "/" + get_name(Common::diffuse_name,x_surface,y_surface);
									mkdir(dirname(savePath(surface_name)),1);
									terrain.setSurfaceDiffuseTextureName(x_surface,y_surface,surface_name);
									
									surface.copy(source,0,0,x * surface_w,y * surface_h,surface_h,surface_w,1);
									surface.save(surface_name);
									log.message("Save: %s \n",surface_name);
									
									wait;
								}
							}
							
							int source_h = coarse.getHeight() / size_source_x;
							int source_w = coarse.getWidth() / size_source_y;
							
							source.resize(source_w,source_h);
							
							int x_dst = source_w * (x_source - size_source_x * X);
							int y_dst = source_h * (y_source - size_source_y * Y);
							
							coarse.copy(source,x_dst,y_dst,0,0,source_w,source_h,1);
						}
					}
					
					coarse.save(terrain.getDiffuseTextureName());
					log.message("Save: %s \n",terrain.getDiffuseTextureName());
					
					delete coarse;
					delete surface;
					delete source;
				}
			}
			
			dialogProgressEnd();
		}
		
		/**********************************************************************\
		*
		* Import normal
		*
		\**********************************************************************/
		
		/*
		 */
		int set_normal(Async async,ObjectTerrain terrain,string name,int offset_x,int offset_y,Image image) {
			
			Image surface = new Image();
			surface.create2D(TEXTURE_SIZE,TEXTURE_SIZE,IMAGE_FORMAT_R8);
			surface.set(0,MASK_R);
			
			// check paths
			forloop(int y = 0; terrain.getSurfacesY()) {
				forloop(int x = 0; terrain.getSurfacesX()) {
					string surface_name = terrain.getSurfaceNormalTextureName(x,y);
					
					if(strlen(surface_name) == 0) {
						surface_name = Common::landscape_path + "/" + name + "/" + get_name(Common::normal_name,x,y);
						mkdir(dirname(savePath(surface_name)),1);
						surface.save(surface_name);
						terrain.setSurfaceNormalTextureName(x,y,surface_name);
					}
				}
			}
			
			int progress_offset = 0;
			
			int image_width = image.getWidth();
			int image_height = image.getHeight();
			
			int surface_width = Common::normal_size / terrain.getSurfacesX();
			int surface_height = Common::normal_size / terrain.getSurfacesY();
			
			// partial normal update
			if(offset_x || offset_y || image_height != Common::normal_size || image_width != Common::normal_size) {
				
				progress_offset = 50;
				
				Image normal = new Image();
				normal.create2D(Common::normal_size,Common::normal_size,image.getFormat());
				normal.set(0,NORMAL_R);
				normal.set(1,NORMAL_G);
				
				log.message("Loading \"%s\" normal textures\n",terrain.getTerrainName());
				
				// load normal textures
				forloop(int y = 0; terrain.getSurfacesY()) {
					forloop(int x = 0; terrain.getSurfacesX()) {
						
						// load surface normal texture
						int id = async.run(surface,"load",terrain.getSurfaceNormalTextureName(x,y));
						while(async.isRunning(id)) {
							usleep(WAIT_TIME);
							wait;
						}
						if(async.getResult(id)) {
							if(surface.getWidth() == surface_width && surface.getHeight() == surface_height && surface.convertToFormat(normal.getFormat())) {
								normal.copy(surface,surface_width * x,surface_height * y,0,0,surface_width,surface_height);
							}
						} else {
							log.error("Landscape::Import::set_normal(): can't load \"%s\" image\n",terrain.getSurfaceNormalTextureName(x,y));
							delete surface;
							delete normal;
							return 0;
						}
						
						// update progress dialog
						int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
						if(dialogProgressUpdate2(progress * progress_offset / 100) == 0) {
							delete surface;
							delete normal;
							return 0;
						}
					}
				}
				
				normal.copy(image,offset_x,offset_y,0,0,image_width,image_height);
				delete image;
				
				image = normal;
			}
			
			log.message("Creating \"%s\" normal textures\n",terrain.getTerrainName());
			
			// create normal textures
			forloop(int y = 0; terrain.getSurfacesY()) {
				if(surface_height * y < offset_y) continue;
				if(surface_height * y >= offset_y + image_height) continue;
				
				forloop(int x = 0; terrain.getSurfacesX()) {
					if(surface_width * x < offset_x) continue;
					if(surface_width * x >= offset_x + image_width) continue;
					
					// pixel border for seams removing
					int surface_copy_width = surface_width;
					int surface_copy_height = surface_height;
					if(x != terrain.getSurfacesX() - 1) surface_copy_width++;
					if(y != terrain.getSurfacesY() - 1) surface_copy_height++;
					surface.create2D(surface_copy_width,surface_copy_height,image.getFormat());
					
					// copy surface normal texture
					surface.copy(image,0,0,surface_width * x,surface_height * y,surface_copy_width,surface_copy_height);
					surface.resize(surface_width,surface_height);
					
					// save surface normal texture
					int id = async.run(surface,"save",terrain.getSurfaceNormalTextureName(x,y));
					while(async.isRunning(id)) {
						usleep(WAIT_TIME);
						wait;
					}
					if(async.getResult(id) == 0) {
						log.error("Landscape::Import::set_normal(): can't save \"%s\" image\n",terrain.getSurfaceNormalTextureName(x,y));
						delete surface;
						return 0;
					}
					
					// update progress dialog
					int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
					if(dialogProgressUpdate2(progress * (100 - progress_offset) / 100 + progress_offset) == 0) {
						delete surface;
						return 0;
					}
				}
			}
			
			delete surface;
			
			Image normal = new Image();
			normal.load(terrain.getNormalTextureName());
			normal.resize(Common::normal_coarse,Common::normal_coarse);
			image.resize(Common::normal_coarse,Common::normal_coarse);
			
			normal.copy(image,offset_x,offset_y,0,0,image.getHeight(),image.getWidth(),1);
			
			normal.save(terrain.getNormalTextureName());
			log.message(terrain.getNormalTextureName());
			delete normal;
			
			return 1;
		}
		
		/*
		 */
		int import_normal(Async async) {
			
			if(check_path(normal_name) == 0) {
				dialogMessageOk(TR("Error"),TR("Normal texture you want to import is already used by landscape"));
				return 0;
			}
			
			log.message("Importing terrain normal\n");
			
			// begin progress dialog
			dialogProgressBegin(TR("Importing Terrain Normal..."));
			
			// load normal image
			if(TiledImage::load(normal_name,source_offset_x,source_offset_y,source_size_x,source_size_y,flip_y) == 0) {
				dialogProgressEnd();
				return 0;
			}
			
			// height bounds
			int height_x0 = get_height_x0();
			int height_x1 = get_height_x1();
			int height_y0 = get_height_y0();
			int height_y1 = get_height_y1();
			
			// height size
			int height_size_x = height_x1 - height_x0;
			int height_size_y = height_y1 - height_y0;
			int height_size = Common::terrain_size - 1;
			
			// normal size
			int normal_size = max(Common::normal_size,Common::normal_coarse);
			int normal_size_x = height_size_x * normal_size / height_size;
			int normal_size_y = height_size_y * normal_size / height_size;
			
			// import terrain normal
			for(int height_y = 0; height_y < height_size_y;) {
				int Y = height_y0 + height_y;
				int height_offset_y = Y % height_size;
				int height_height = height_size - height_offset_y;
				int normal_y = height_y * normal_size / height_size;
				int normal_height = height_height * normal_size / height_size;
				if(normal_y + normal_height > normal_size_y) normal_height = normal_size_y - normal_y;
				
				for(int height_x = 0; height_x < height_size_x;) {
					int X = height_x0 + height_x;
					int height_offset_x = X % height_size;
					int height_width = height_size - height_offset_x;
					int normal_x = height_x * normal_size / height_size;
					int normal_width = height_width * normal_size / height_size;
					if(normal_x + normal_width > normal_size_x) normal_width = normal_size_x - normal_x;
					
					// update terrain normal
					ObjectTerrain terrain = Common::get_terrain(X / height_size,Y / height_size);
					string name = get_name(Common::terrain_name,X / height_size,Y / height_size);
					if(terrain != NULL) {
						Image image = TiledImage::getImage(async,normal_size_x,normal_size_y,normal_x,normal_y,normal_width,normal_height,IMAGE_FORMAT_RG8);
						if(image == NULL) {
							dialogProgressEnd();
							TiledImage::clear();
							return 0;
						}
						int normal_offset_x = height_offset_x * normal_size / height_size;
						int normal_offset_y = height_offset_y * normal_size / height_size;
						if(set_normal(async,terrain,name,normal_offset_x,normal_offset_y,image) == 0) {
							dialogProgressEnd();
							TiledImage::clear();
							delete image;
							return 0;
						}
						delete image;
					}
					
					height_x += height_width;
					
					// update progress dialog
					int progress = int((long(height_size_x) * height_y + long(height_height) * height_x) * 100 / (long(height_size_x) * height_size_y));
					if(dialogProgressUpdate(progress) == 0) {
						dialogProgressEnd();
						TiledImage::clear();
						return 0;
					}
				}
				height_y += height_height;
			}
			
			TiledImage::clear();
			
			// end progress dialog
			dialogProgressEnd();
			
			return 1;
		}
		
		/*
		 */
		int fast_import_normal() {
			if(normal_import == 0) return;
			dialogProgressBegin(TR("Importing Terrain Normal ..."),0);
			
			int import_x0 = Nodes::get_import_x0();
			int import_x1 = Nodes::get_import_x1();
			int import_y0 = Nodes::get_import_y0();
			int import_y1 = Nodes::get_import_y1();
			
			int import_size_x = import_x1 - import_x0;
			int import_size_y = import_y1 - import_y0;
			
			forloop(int X = import_x0; import_x1) {
				forloop(int Y = import_y0; import_y1) {
					int progress = ((X * import_size_y + Y) * 100) / (import_size_x * Common::terrain_size_y);
					dialogProgressUpdate(progress);
					
					ObjectTerrain terrain = Common::get_terrain(X,Y);
					string name = Common::landscape_path + "/" + get_name(Common::terrain_name,X,Y);
					
					int terrain_size_surface_x = terrain.getSurfacesX();
					int terrain_size_surface_y = terrain.getSurfacesY();
					
					int surface_w = Common::normal_size / terrain_size_surface_x;
					int surface_h = Common::normal_size / terrain_size_surface_y;
					
					int size_source_x = source_size_x / Common::terrain_size_x;
					int size_source_y = source_size_y / Common::terrain_size_y;
					
					int size_surface_x = terrain_size_surface_x / size_source_x;
					int size_surface_y = terrain_size_surface_y / size_source_y;
					
					Image coarse = new Image();
					coarse.create2D(Common::normal_coarse,Common::normal_coarse,IMAGE_FORMAT_RG8);
					
					Image surface = new Image();
					surface.create2D(surface_w,surface_h,IMAGE_FORMAT_RG8);
					
					Image source = new Image();
					
					forloop(int x_source = size_source_x * X; size_source_x * (X + 1)) {
						forloop(int y_source = size_source_y * Y; size_source_y * (Y + 1)) {
							
							int progress = (size_source_y * (x_source - size_source_x * X) + y_source - size_source_y * Y) * 100 / (size_source_x * size_source_y);
							dialogProgressUpdate2(progress);
							
							source.load(get_name(normal_name,x_source,y_source));
							source.convertToFormat(IMAGE_FORMAT_RG8);
							source.resize(surface_w * size_surface_x,surface_h * size_surface_y);
							source.flipY();
							
							forloop(int x = 0; size_surface_x) {
								forloop(int y = 0; size_surface_y) {
									int x_surface = x_source * size_surface_x + x - X * terrain_size_surface_x;
									int y_surface = y_source * size_surface_y + y - Y * terrain_size_surface_y;
									
									string surface_name = name + "/" + get_name(Common::diffuse_name,x_surface,y_surface);
									mkdir(dirname(savePath(surface_name)),1);
									terrain.setSurfaceNormalTextureName(x_surface,y_surface,surface_name);
									
									surface.copy(source,0,0,x * surface_w,y * surface_h,surface_h,surface_w,1);
									surface.save(surface_name);
									log.message("Save: %s \n",surface_name);
									
									wait;
								}
							}
							
							int source_h = coarse.getHeight() / size_source_x;
							int source_w = coarse.getWidth() / size_source_y;
							
							source.resize(source_w,source_h);
							
							int x_dst = source_w * (x_source - size_source_x * X);
							int y_dst = source_h * (y_source - size_source_y * Y);
							
							coarse.copy(source,x_dst,y_dst,0,0,source_w,source_h,1);
						}
					}
					
					coarse.save(terrain.getNormalTextureName());
					log.message("Save: %s \n",terrain.getNormalTextureName());
					
					delete coarse;
					delete surface;
					delete source;
				}
			}
			
			dialogProgressEnd();
		}
		
		/**********************************************************************\
		*
		* Import mask
		*
		\**********************************************************************/
		
		/*
		 */
		int set_mask(Async async,ObjectTerrain terrain,string name,int offset_x,int offset_y,Image image,int material,int clean) {
			
			Image surface = new Image();
			surface.create2D(TEXTURE_SIZE,TEXTURE_SIZE,IMAGE_FORMAT_R8);
			surface.set(0,MASK_R);
			
			// check paths
			forloop(int y = 0; terrain.getSurfacesY()) {
				forloop(int x = 0; terrain.getSurfacesX()) {
					string mask_surface_name = terrain.getSurfaceMaskTextureName(x,y);
					string index_surface_name = terrain.getSurfaceIndexTextureName(x,y);
					
					if(strlen(mask_surface_name) == 0) {
						mask_surface_name = Common::landscape_path + "/" + name + "/" + get_name(Common::mask_name,x,y);
						mkdir(dirname(savePath(mask_surface_name)),1);
						surface.save(mask_surface_name);
						terrain.setSurfaceMaskTextureName(x,y,mask_surface_name);
					}
					
					if(strlen(index_surface_name) == 0) {
						index_surface_name = Common::landscape_path + "/" + name + "/" + get_name(Common::index_name,x,y);
						mkdir(dirname(savePath(index_surface_name)),1);
						surface.save(index_surface_name);
						terrain.setSurfaceIndexTextureName(x,y,index_surface_name);
					}
				}
			}
			
			// clean mask textures
			if(clean) {
				
				int threads[] = ();
				forloop(int y = 0; terrain.getSurfacesY()) {
					forloop(int x = 0; terrain.getSurfacesX()) {
						threads.append(async.run(surface,"save",terrain.getSurfaceMaskTextureName(x,y)));
						threads.append(async.run(surface,"save",terrain.getSurfaceIndexTextureName(x,y)));
					}
				}
				threads.append(async.run(surface,"save",terrain.getMaskTextureName()));
				threads.append(async.run(surface,"save",terrain.getIndexTextureName()));
				
				while(async.isRunning()) {
					usleep(SLEEP_TIME);
					wait;
				}
				
				foreach(int id; threads) {
					if(async.getResult(id) == 0) {
						dialogMessageOk(TR("Error"));
						delete surface;
						return 0;
					}
				}
			}
			
			int surface_width = Common::mask_size / terrain.getSurfacesX();
			int surface_height = Common::mask_size / terrain.getSurfacesY();
			
			Image index = new Image();
			Image layer = new Image();
			layer.create2D(surface_width,surface_height,IMAGE_FORMAT_R8);
			
			// load mask textures
			forloop(int y = 0; terrain.getSurfacesY()) {
				forloop(int x = 0; terrain.getSurfacesX()) {
					
					// load surface mask & index textures
					int threads[] = ();
					threads.append(async.run(surface,"load",terrain.getSurfaceMaskTextureName(x,y)));
					threads.append(async.run(index,"load",terrain.getSurfaceIndexTextureName(x,y)));
					
					while(async.isRunning()) {
						usleep(WAIT_TIME);
						wait;
					}
					
					foreach(int id; threads) {
						if(async.getResult(id) == 0) {
							log.error("Landscape::Import::set_mask(): can't load \"%s\" image\n",terrain.getSurfaceMaskTextureName(x,y));
							delete index;
							delete surface;
							delete layer;
							
							return 0;
						}
					}
					
					if(surface.convertToFormat(IMAGE_FORMAT_RGBA8) == 0) continue;
					if(index.convertToFormat(IMAGE_FORMAT_RGBA8) == 0) continue;
					
					if(surface.resize(surface_width,surface_height) == 0) continue;
					
					// get surface mask array
					if(engine.editor.createTerrainMask(surface,index,terrain.getNumMaterials()) == 0) continue;
					
					// update surface array
					int local_x = offset_x - surface_width * x;
					int local_y = offset_y - surface_height * y;
					
					layer.copy(surface,material);
					layer.copy(image,local_x,local_y,0,0,image.getWidth(),image.getHeight(),1);
					
					surface.copy(layer,material);
					
					// create back mask & index textures
					if(engine.editor.createTerrainIndex(surface,index,num_channels_sl.getValue()) == 0) continue;
					
					// save mask & index
					threads.clear();
					
					threads.append(async.run(surface,"save",terrain.getSurfaceMaskTextureName(x,y)));
					threads.append(async.run(index,"save",terrain.getSurfaceIndexTextureName(x,y)));
					
					while(async.isRunning()) {
						usleep(WAIT_TIME);
						wait;
					}
					foreach(int id; threads) {
						if(async.getResult(id) == 0) {
							log.error("Landscape::Import::set_mask(): can't save \"%s\" image\n",terrain.getSurfaceMaskTextureName(x,y));
							delete index;
							delete surface;
							delete layer;
							
							return 0;
						}
					}
					
					// update progress dialog
					int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
					if(dialogProgressUpdate2(progress) == 0) {
						delete index;
						delete surface;
						delete layer;
						
						return 0;
					}
				}
			}
			
			delete surface;
			
			Image mask = new Image();
			
			mask.load(terrain.getMaskTextureName());
			index.load(terrain.getIndexTextureName());
			
			mask.resize(Common::mask_coarse,Common::mask_coarse);
			
			engine.editor.createTerrainMask(mask,index,terrain.getNumMaterials());
			
			layer.create2D(Common::mask_coarse,Common::mask_coarse,IMAGE_FORMAT_R8);
			layer.copy(mask,0,0,0,material * Common::mask_coarse,Common::mask_coarse,Common::mask_coarse,1);
			
			image.resize(Common::mask_coarse, Common::mask_coarse);
			
			layer.copy(image,offset_x,offset_y,0,0,Common::mask_coarse,Common::mask_coarse,1);
			
			mask.copy(layer,material);
			
			engine.editor.createTerrainIndex(mask,index,num_channels_sl.getValue());
			
			mask.save(terrain.getMaskTextureName());
			index.save(terrain.getIndexTextureName());
			
			delete layer;
			delete mask;
			delete index;
			
			
			return 1;
		}
		
		/*
		 */
		int import_mask(Async async,string mask_name,int mask_material,int clean_materials) {
			
			if(check_path(mask_name) == 0) {
				dialogMessageOk(TR("Error"),TR("Mask texture you want to import is already used by landscape"));
				return 0;
			}
			
			log.message("Importing terrain mask\n");
			
			// begin progress dialog
			dialogProgressBegin(TR("Importing Terrain Mask..."));
			
			// load mask image
			if(TiledImage::load(mask_name,source_offset_x,source_offset_y,source_size_x,source_size_y,flip_y) == 0) {
				dialogProgressEnd();
				return 0;
			}
			
			// height bounds
			int height_x0 = get_height_x0();
			int height_x1 = get_height_x1();
			int height_y0 = get_height_y0();
			int height_y1 = get_height_y1();
			
			// height size
			int height_size_x = height_x1 - height_x0;
			int height_size_y = height_y1 - height_y0;
			int height_size = Common::terrain_size - 1;
			
			// mask size
			int mask_size = max(Common::mask_size,Common::mask_coarse);
			int mask_size_x = height_size_x * mask_size / height_size;
			int mask_size_y = height_size_y * mask_size / height_size;
			
			// import terrain mask
			for(int height_y = 0; height_y < height_size_y;) {
				int Y = height_y0 + height_y;
				int height_offset_y = Y % height_size;
				int height_height = height_size - height_offset_y;
				int mask_y = height_y * mask_size / height_size;
				int mask_h = height_height * mask_size / height_size;
				if(mask_y + mask_h > mask_size_y) mask_h = mask_size_y - mask_y;
				
				for(int height_x = 0; height_x < height_size_x;) {
					int X = height_x0 + height_x;
					int height_offset_x = X % height_size;
					int height_width = height_size - height_offset_x;
					int mask_x = height_x * mask_size / height_size;
					int mask_w = height_width * mask_size / height_size;
					if(mask_x + mask_w > mask_size_x) mask_w = mask_size_x - mask_x;
					
					// update terrain mask
					ObjectTerrain terrain = Common::get_terrain(X / height_size,Y / height_size);
					string name = get_name(Common::terrain_name,X / height_size,Y / height_size);
					
					if(terrain != NULL) {
						Image image = TiledImage::getImage(async,mask_size_x,mask_size_y,mask_x,mask_y,mask_w,mask_h,IMAGE_FORMAT_R8);
						if(image == NULL) {
							dialogProgressEnd();
							TiledImage::clear();
							return 0;
						}
						int mask_offset_x = height_offset_x * mask_size / height_size;
						int mask_offset_y = height_offset_y * mask_size / height_size;
						if(set_mask(async,terrain,name,mask_offset_x,mask_offset_y,image,mask_material,clean_materials) == 0) {
							dialogProgressEnd();
							TiledImage::clear();
							delete image;
							return 0;
						}
						delete image;
					}
					
					height_x += height_width;
					
					// update progress dialog
					int progress = int((long(height_size_x) * height_y + long(height_height) * height_x) * 100 / (long(height_size_x) * height_size_y));
					if(dialogProgressUpdate(progress) == 0) {
						dialogProgressEnd();
						TiledImage::clear();
						return 0;
					}
				}
				height_y += height_height;
			}
			
			TiledImage::clear();
			
			// end progress dialog
			dialogProgressEnd();
			
			return 1;
		}
		
		/*
		 */
		int fast_import_mask() {
			int materials_size = Materials::materials.size();
			if(materials_size == 0) return;
			
			int flag_mask_import = 0;
			forloop(int i = 0; NUM_MATERIALS) {
				int material_valid = mask_material[i] >= 0 && mask_material[i] < Materials::materials.size();
				
				if(mask_import[i] && material_valid) {
					flag_mask_import = 1;
					break;
				}
			}
			if(flag_mask_import == 0) return;
			
			dialogProgressBegin(TR("Importing Terrain Mask..."),0);
			
			int import_x0 = Nodes::get_import_x0();
			int import_x1 = Nodes::get_import_x1();
			int import_y0 = Nodes::get_import_y0();
			int import_y1 = Nodes::get_import_y1();
			
			int import_size_x = import_x1 - import_x0;
			int import_size_y = import_y1 - import_y0;
			
			forloop(int X = import_x0; import_x1) {
				forloop(int Y = import_y0; import_y1) {
					int progress = ((X * import_size_y + Y) * 100) / (import_size_x * Common::terrain_size_y);
					dialogProgressUpdate(progress);
					
					ObjectTerrain terrain = Common::get_terrain(X,Y);
					
					int terrain_size_surface_x = terrain.getSurfacesX();
					int terrain_size_surface_y = terrain.getSurfacesY();
					
					int surface_w = Common::mask_size / terrain_size_surface_x;
					int surface_h = Common::mask_size / terrain_size_surface_y;
					
					int size_source_x = source_size_x / Common::terrain_size_x;
					int size_source_y = source_size_y / Common::terrain_size_y;
					
					int size_surface_x = terrain_size_surface_x / size_source_x;
					int size_surface_y = terrain_size_surface_y / size_source_y;
					
					Image coarse_m = class_manage(new Image());
					coarse_m.create2DArray(Common::mask_coarse,Common::mask_coarse,materials_size,IMAGE_FORMAT_R8);
					
					forloop(int x_source = size_source_x * X; size_source_x * (X + 1)) {
						forloop(int y_source = size_source_y * Y; size_source_y * (Y + 1)) {
							
							int progress = (size_source_y * (x_source - size_source_x * X) + y_source - size_source_y * Y) * 100 / (size_source_x * size_source_y);
							dialogProgressUpdate2(progress);
							
							Image source[];
							
							forloop(int i = 0; NUM_MATERIALS) {
									int material_valid = mask_material[i] >= 0 && mask_material[i] < Materials::materials.size();
									
									if(mask_import[i] && material_valid) {
										string mask_name = get_name(mask_name_el[i].getText(),x_source,y_source);
										int mask_material = mask_material[i];
										
										Image image = class_manage(new Image());
										image.load(mask_name);
										image.convertToFormat(IMAGE_FORMAT_R8);
										image.resize(surface_w * size_surface_x,surface_h * size_surface_y);
										image.flipY();
										
										source.append(mask_material,image);
									}
							}
							
							forloop(int x = 0; size_surface_x) {
								forloop(int y = 0; size_surface_y) {
								
									int x_surface = x_source * size_surface_x + x - X * terrain_size_surface_x;
									int y_surface = y_source * size_surface_y + y - Y * terrain_size_surface_y;
									
									string mask_surface_name = terrain.getSurfaceMaskTextureName(x_surface,y_surface);
									string index_surface_name = terrain.getSurfaceIndexTextureName(x_surface,y_surface);
									
									Image surface = class_manage(new Image());
									surface.create2DArray(surface_w,surface_h,materials_size,IMAGE_FORMAT_R8);
									
									forloop(int i = 0; Materials::materials.size()) {
										if(source.check(i)) {
											Image image = source[i];
											surface.copy(image,0,i * surface_h,x * surface_w,y * surface_h,surface_h,surface_w);
										}
									}
									
									asyncRunNamedThread(functionid(create_index),(surface,num_channels_sl.getValue(),mask_surface_name,index_surface_name),"landscape_import_mask");
									
									wait;
								}
							}
							
							int source_h = coarse_m.getHeight() / size_source_x;
							int source_w = coarse_m.getWidth() / size_source_y;
							
							forloop(int i = 0; Materials::materials.size()) {
								if(source.check(i)) {
									Image image = source[i];
									image.resize(source_w,source_h);
									
									int x_dst = source_w * (x_source - size_source_x * X);
									int y_dst = source_h * (y_source - size_source_y * Y) + i * coarse_m.getHeight();
									
									coarse_m.copy(image,x_dst,y_dst,0,0,source_w,source_h);
								}
							}
							
							source.delete();
						}
					}
					asyncRunNamedThread(functionid(create_index),(coarse_m,num_channels_sl.getValue(),terrain.getMaskTextureName(),terrain.getIndexTextureName()),"landscape_import_mask");
				}
			}
			
			while(asyncIsRunning("landscape_import_mask")) wait;
			
			dialogProgressEnd();
		}
		
		void create_index(Async async,Image mask,int num_channels,string mask_name,string index_name) {
			Image index = class_manage(new Image());
			async.run(functionid(engine.editor.createTerrainIndex),mask,index,num_channels);
			while(async.isRunning()) wait;
			
			async.run(mask,"save",mask_name);
			async.run(index,"save",index_name);
			while(async.isRunning()) wait;
			
			log.message("Save: %s \n",mask_name);
		}
		/**********************************************************************\
		*
		* Import
		*
		\**********************************************************************/
		
		/*
		 */
		void import_clicked() {
			
			if(dialogMessageYesNo(TR("Confirm"),TR("Import selected landscape resources?")) == 0) {
				return;
			}
			
			// asynchronous processing
			Async async = asyncGet();
			
			int ret = 1;
			
			// import height
			if(ret && height_import) ret = import_height(async);
			
			if(fast_mode) {
				fast_import_diffuse();
				fast_import_normal();
				fast_import_mask();
			} else {
				if(ret && diffuse_import) ret = import_diffuse(async);
				if(ret && normal_import) ret = import_normal(async);
				
				int clean_materials = clean;
				forloop(int i = 0; NUM_MATERIALS) {
					int material_valid = mask_material[i] >= 0 && mask_material[i] < Materials::materials.size();
					
					if(ret && mask_import[i] && material_valid) {
						ret = import_mask(async,mask_name[i],mask_material[i],clean_materials);
						clean_materials = 0;
					}
				}
			}
		}
	}
	
	/**************************************************************************\
	*
	* Nodes
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Nodes {
		
		/*
		 */
		enum {
			UNIT_TERRAINS = 0,
			UNIT_SOURCE = 1,
		};
		
		/*
		 */
		EditLine import_offset_x_el;
		EditLine import_offset_y_el;
		EditLine import_size_x_el;
		EditLine import_size_y_el;
		ComboBox import_unit_x_cb;
		ComboBox import_unit_y_cb;
		
		int import_offset_x;
		int import_offset_y;
		int import_size_x;
		int import_size_y;
		int import_unit_x;
		int import_unit_y;
		
		/*
		 */
		EditLine source_offset_x_el;
		EditLine source_offset_y_el;
		EditLine source_size_x_el;
		EditLine source_size_y_el;
		
		int source_offset_x;
		int source_offset_y;
		int source_size_x;
		int source_size_y;
		
		/*
		 */
		CheckBox enabled_cb;
		
		int enabled;
		
		/*
		 */
		Button add_b;
		Button clone_b;
		Button remove_b;
		
		ListBox references_lb;
		
		CheckBox node_import_cb;
		EditLine node_name_el;
		Icon node_load_i;
		
		EditLine mask_image_import_el;
		EditLine mask_image_name_el;
		ComboBox mask_image_size_cb;
		ComboBox mask_image_format_cb;
		
		EditLine mask_mesh_name_el;
		EditLine node_layer_name_el;
		
		EditLine node_offset_el[3];
		
		ComboBox depth_cb;
		
		/*
		 */
		CheckBox combine_cb;
		CheckBox flip_y_cb;
		
		int combine;
		int flip_y;
		
		Button update_b;
		Button create_b;
		
		/*
		 */
		class Reference {
			
			int node_import;
			string node_name;
			
			string mask_image_import;
			string mask_image_name;
			int mask_image_size;
			int mask_image_format;
			
			string mask_mesh_name;
			string node_layer_name;
			
			vec3 node_offset;
			
			int depth;
			
			Reference() {
				
				node_import = 1;
				node_name = "";
				
				mask_image_import = "";
				mask_image_name = "";
				mask_image_size = 512;
				mask_image_format = 0;
				
				mask_mesh_name = "";
				node_layer_name = "";
				
				node_offset = vec3_zero;
				
				depth = 0;
			}
			
			Reference(Reference reference) {
				
				node_import = reference.node_import;
				node_name = reference.node_name;
				
				mask_image_import = reference.mask_image_import;
				mask_image_name = reference.mask_image_name;
				mask_image_size = reference.mask_image_size;
				mask_image_format = reference.mask_image_format;
				
				mask_mesh_name = reference.mask_mesh_name;
				node_layer_name = reference.node_layer_name;
				
				node_offset = reference.node_offset;
				
				depth = reference.depth;
			}
			
			string getName(int num) {
				string ret = basename(node_name);
				if(ret == "") ret = format(TR("Node  %d"),num);
				ret += format(" - %d",depth);
				if(node_import == 0) ret += TR(" - disabled");
				return ret;
			}
			
			void setMaskImageFormatName(string format) {
				if(format == "R8") mask_image_format = 0;
				if(format == "RG8") mask_image_format = 1;
				if(format == "RGB8") mask_image_format = 2;
				if(format == "RGBA8") mask_image_format = 3;
			}
			string getMaskImageFormatName() {
				if(mask_image_format == 0) return "R8";
				if(mask_image_format == 1) return "RG8";
				if(mask_image_format == 2) return "RGB8";
				if(mask_image_format == 3) return "RGBA8";
			}
		};
		
		int reference_num = -1;
		Reference references[0];
		
		/*
		 */
		void clear() {
			
			import_offset_x = 0;
			import_offset_y = 0;
			import_size_x = 1;
			import_size_y = 1;
			import_unit_x = UNIT_TERRAINS;
			import_unit_y = UNIT_TERRAINS;
			
			source_offset_x = 0;
			source_offset_y = 0;
			source_size_x = 1;
			source_size_y = 1;
			
			enabled = 1;
			
			combine = 0;
			flip_y = 0;
			
			reference_num = -1;
			references.delete();
			
			update();
		}
		
		/*
		 */
		void update() {
			
			import_offset_x_el.setText(string(import_offset_x));
			import_offset_y_el.setText(string(import_offset_y));
			import_size_x_el.setText(string(import_size_x));
			import_size_y_el.setText(string(import_size_y));
			import_unit_x_cb.setCurrentItem(import_unit_x);
			import_unit_y_cb.setCurrentItem(import_unit_y);
			
			source_offset_x_el.setText(string(source_offset_x));
			source_offset_y_el.setText(string(source_offset_y));
			source_size_x_el.setText(string(source_size_x));
			source_size_y_el.setText(string(source_size_y));
			
			enabled_cb.setChecked(enabled);
			
			combine_cb.setChecked(combine);
			flip_y_cb.setChecked(flip_y);
			
			Widget widgets[] = (
				import_offset_x_el,
				import_offset_y_el,
				import_size_x_el,
				import_size_y_el,
				import_unit_x_cb,
				import_unit_y_cb,
				source_offset_x_el,
				source_offset_y_el,
				source_size_x_el,
				source_size_y_el,
				combine_cb,
				flip_y_cb,
				update_b,
				create_b,
			);
			
			foreach(Widget widget; widgets) {
				widget.setEnabled((landscape != NULL));
			}
			
			reference_num = -1;
			references_lb.setCallbackEnabled(GUI_CHANGED,0);
			references_lb.clear();
			forloop(int i = 0; references.size()) {
				references_lb.addItem(references[i].getName(i));
			}
			references_lb.setCallbackEnabled(GUI_CHANGED,1);
			
			reference_changed();
		}
		
		/*
		 */
		void load(Xml xml) {
			
			import_offset_x = xml.getChildData("import_offset_x",import_offset_x);
			import_offset_y = xml.getChildData("import_offset_y",import_offset_y);
			import_size_x = xml.getChildData("import_size_x",import_size_x);
			import_size_y = xml.getChildData("import_size_y",import_size_y);
			import_unit_x = xml.getChildData("import_unit_x",import_unit_x);
			import_unit_y = xml.getChildData("import_unit_y",import_unit_y);
			
			source_offset_x = xml.getChildData("source_offset_x",source_offset_x);
			source_offset_y = xml.getChildData("source_offset_y",source_offset_y);
			source_size_x = xml.getChildData("source_size_x",source_size_x);
			source_size_y = xml.getChildData("source_size_y",source_size_y);
			
			enabled = xml.getChildData("enabled",enabled);
			
			forloop(int i = 0; xml.getNumChilds()) {
				Xml x = xml.getChild(i);
				if(x.getName() == "reference") {
					Reference reference = new Reference();
					reference.node_import = x.getChildData("node_import",reference.node_import);
					reference.node_name = x.getChildData("node_name",reference.node_name);
					reference.mask_image_import = x.getChildData("mask_image_import",reference.mask_image_import);
					reference.mask_image_name = x.getChildData("mask_image_name",reference.mask_image_name);
					reference.mask_image_size = x.getChildData("mask_image_size",reference.mask_image_size);
					reference.mask_image_format = x.getChildData("mask_image_format",reference.mask_image_format);
					reference.mask_mesh_name = x.getChildData("mask_mesh_name",reference.mask_mesh_name);
					reference.node_layer_name = x.getChildData("node_layer_name",reference.node_layer_name);
					reference.node_offset = x.getChildData("node_offset",reference.node_offset);
					reference.depth = x.getChildData("depth",reference.depth);
					references.append(reference);
				}
			}
			
			combine = xml.getChildData("combine",combine);
			flip_y = xml.getChildData("flip_y",flip_y);
			
			update();
		}
		
		/*
		 */
		void save(Xml xml) {
			
			xml.setChildData("import_offset_x",import_offset_x);
			xml.setChildData("import_offset_y",import_offset_y);
			xml.setChildData("import_size_x",import_size_x);
			xml.setChildData("import_size_y",import_size_y);
			xml.setChildData("import_unit_x",import_unit_x);
			xml.setChildData("import_unit_y",import_unit_y);
			
			xml.setChildData("source_offset_x",source_offset_x);
			xml.setChildData("source_offset_y",source_offset_y);
			xml.setChildData("source_size_x",source_size_x);
			xml.setChildData("source_size_y",source_size_y);
			
			xml.setChildData("enabled",enabled);
			
			foreach(Reference reference; references) {
				Xml x = xml.addChild("reference");
				x.setChildData("node_import",reference.node_import);
				x.setChildData("node_name",reference.node_name);
				x.setChildData("mask_image_import",reference.mask_image_import);
				x.setChildData("mask_image_name",reference.mask_image_name);
				x.setChildData("mask_image_size",reference.mask_image_size);
				x.setChildData("mask_image_format",reference.mask_image_format);
				x.setChildData("mask_mesh_name",reference.mask_mesh_name);
				x.setChildData("node_layer_name",reference.node_layer_name);
				x.setChildData("node_offset",reference.node_offset);
				x.setChildData("depth",reference.depth);
			}
			
			xml.setChildData("combine",combine);
			xml.setChildData("flip_y",flip_y);
		}
		
		/*
		 */
		void import_offset_x_pressed() {
			import_offset_x = int(import_offset_x_el.getText());
			import_offset_x_el.setText(string(import_offset_x));
		}
		
		void import_offset_y_pressed() {
			import_offset_y = int(import_offset_y_el.getText());
			import_offset_y_el.setText(string(import_offset_y));
		}
		
		void import_size_x_pressed() {
			import_size_x = max(int(import_size_x_el.getText()),1);
			import_size_x_el.setText(string(import_size_x));
		}
		
		void import_size_y_pressed() {
			import_size_y = max(int(import_size_y_el.getText()),1);
			import_size_y_el.setText(string(import_size_y));
		}
		
		void import_unit_x_changed() {
			import_unit_x = import_unit_x_cb.getCurrentItem();
		}
		
		void import_unit_y_changed() {
			import_unit_y = import_unit_y_cb.getCurrentItem();
		}
		
		/*
		 */
		void source_offset_x_pressed() {
			source_offset_x = int(source_offset_x_el.getText());
			source_offset_x_el.setText(string(source_offset_x));
		}
		
		void source_offset_y_pressed() {
			source_offset_y = int(source_offset_y_el.getText());
			source_offset_y_el.setText(string(source_offset_y));
		}
		
		void source_size_x_pressed() {
			source_size_x = max(int(source_size_x_el.getText()),1);
			source_size_x_el.setText(string(source_size_x));
		}
		
		void source_size_y_pressed() {
			source_size_y = max(int(source_size_y_el.getText()),1);
			source_size_y_el.setText(string(source_size_y));
		}
		
		/*
		 */
		Reference get_reference() {
			if(reference_num == -1) return NULL;
			return references[reference_num];
		}
		
		/*
		 */
		void enabled_clicked() {
			enabled = enabled_cb.isChecked();
			if(landscape != NULL) {
				forloop(int i = 0; landscape.getNumChilds()) {
					Node node = landscape.getChild(i);
					if(node.getType() == NODE_OBJECT_TERRAIN) {
						forloop(int j = 0; node.getNumChilds()) {
							Node child = node.getChild(j);
							child.setEnabled(enabled);
						}
					}
				}
			}
		}
		
		/*
		 */
		void add_clicked() {
			references.append(new Reference());
			update();
			reference_num = references.size() - 1;
			references_lb.setCurrentItem(reference_num);
		}
		
		void clone_clicked() {
			references.append(new Reference(get_reference()));
			update();
			reference_num = references.size() - 1;
			references_lb.setCurrentItem(reference_num);
		}
		
		void remove_clicked() {
			Reference reference = get_reference();
			if(dialogMessageYesNo(TR("Confirm"),format(TR("Remove \"%s\" reference ?"),reference.getName(reference_num)))) {
				int num = reference_num;
				delete references[reference_num];
				references.remove(reference_num);
				update();
				if(references.size()) references_lb.setCurrentItem(max(num - 1,0));
			}
		}
		
		/*
		 */
		void reference_changed() {
			
			Widget widgets[] = (
				node_name_el,
				node_load_i,
				mask_image_import_el,
				mask_image_name_el,
				mask_image_size_cb,
				mask_image_format_cb,
				mask_mesh_name_el,
				node_layer_name_el,
				node_offset_el[0],
				node_offset_el[1],
				node_offset_el[2],
				depth_cb,
			);
			
			reference_num = references_lb.getCurrentItem();
			
			if(reference_num == -1) {
				
				enabled_cb.setEnabled((landscape != NULL));
				
				add_b.setEnabled((landscape != NULL));
				clone_b.setEnabled(0);
				remove_b.setEnabled(0);
				
				node_import_cb.setEnabled(0);
				
				foreach(Widget widget; widgets) {
					widget.setEnabled(0);
				}
				
				node_import_cb.setChecked(0);
				node_name_el.setText("");
				
				mask_image_import_el.setText("");
				mask_image_name_el.setText("");
				mask_image_size_cb.setCurrentItemText(512);
				mask_image_format_cb.setCurrentItemText("R8");
				
				mask_mesh_name_el.setText("");
				node_layer_name_el.setText("");
				
				node_offset_el[0].setText("");
				node_offset_el[1].setText("");
				node_offset_el[2].setText("");
				
				depth_cb.setCurrentItemText(0);
			}
			else {
				
				enabled_cb.setEnabled(1);
				
				add_b.setEnabled(1);
				clone_b.setEnabled(1);
				remove_b.setEnabled(1);
				
				node_import_cb.setEnabled(1);
				
				Reference reference = get_reference();
				
				foreach(Widget widget; widgets) {
					widget.setEnabled(reference.node_import);
				}
				
				node_import_cb.setChecked(reference.node_import);
				node_name_el.setText(reference.node_name);
				
				mask_image_import_el.setText(reference.mask_image_import);
				mask_image_name_el.setText(reference.mask_image_name);
				mask_image_size_cb.setCurrentItemText(reference.mask_image_size);
				mask_image_format_cb.setCurrentItemText(reference.getMaskImageFormatName());
				
				mask_mesh_name_el.setText(reference.mask_mesh_name);
				node_layer_name_el.setText(reference.node_layer_name);
				
				node_offset_el[0].setText(editorFormat(reference.node_offset.x));
				node_offset_el[1].setText(editorFormat(reference.node_offset.y));
				node_offset_el[2].setText(editorFormat(reference.node_offset.z));
				
				depth_cb.setCurrentItemText(reference.depth);
			}
		}
		
		/*
		 */
		void node_import_changed() {
			Reference reference = get_reference();
			reference.node_import = node_import_cb.isChecked();
			references_lb.setItemText(reference_num,reference.getName(reference_num));
			reference_changed();
		}
		
		/*
		 */
		void node_name_pressed() {
			Reference reference = get_reference();
			reference.node_name = node_name_el.getText();
			node_name_el.setText(reference.node_name);
			references_lb.setItemText(reference_num,reference.getName(reference_num));
		}
		
		void node_load_clicked() {
			Reference reference = get_reference();
			string node_name = reference.node_name;
			if(dialogFileNode(TR("Select node reference"),node_name)) {
				reference.node_name = node_name;
				node_name_el.setText(node_name);
				references_lb.setItemText(reference_num,reference.getName(reference_num));
			}
		}
		
		/*
		 */
		void mask_image_import_pressed() {
			Reference reference = get_reference();
			reference.mask_image_import = mask_image_import_el.getText();
			mask_image_import_el.setText(reference.mask_image_import);
		}
		
		void mask_image_import_double_clicked() {
			Reference reference = get_reference();
			string mask_image_import = reference.mask_image_import;
			if(dialogFileImage(TR("Select mask image texture"),mask_image_import)) {
				reference.mask_image_import = mask_image_import;
				mask_image_import_el.setText(mask_image_import);
			}
		}
		
		/*
		 */
		void mask_image_name_pressed() {
			Reference reference = get_reference();
			reference.mask_image_name = mask_image_name_el.getText();
			mask_image_name_el.setText(reference.mask_image_name);
		}
		
		void mask_image_size_changed() {
			Reference reference = get_reference();
			reference.mask_image_size = int(mask_image_size_cb.getCurrentItemText());
			mask_image_size_cb.setCurrentItemText(reference.mask_image_size);
		}
		
		void mask_image_format_changed() {
			Reference reference = get_reference();
			reference.setMaskImageFormatName(mask_image_format_cb.getCurrentItemText());
			mask_image_format_cb.setCurrentItemText(reference.getMaskImageFormatName());
		}
		
		/*
		 */
		void mask_mesh_name_pressed() {
			Reference reference = get_reference();
			reference.mask_mesh_name = mask_mesh_name_el.getText();
			mask_mesh_name_el.setText(reference.mask_mesh_name);
		}
		
		void node_layer_name_pressed() {
			Reference reference = get_reference();
			reference.node_layer_name = node_layer_name_el.getText();
			node_layer_name_el.setText(reference.node_layer_name);
		}
		
		/*
		 */
		void node_offset_pressed() {
			Reference reference = get_reference();
			reference.node_offset.x = float(node_offset_el[0].getText());
			reference.node_offset.y = float(node_offset_el[1].getText());
			reference.node_offset.z = float(node_offset_el[2].getText());
			
			node_offset_el[0].setText(editorFormat(reference.node_offset.x));
			node_offset_el[1].setText(editorFormat(reference.node_offset.y));
			node_offset_el[2].setText(editorFormat(reference.node_offset.z));
		}
		
		/*
		 */
		void depth_changed() {
			Reference reference = get_reference();
			reference.depth = int(depth_cb.getCurrentItemText());
			depth_cb.setCurrentItemText(reference.depth);
			references_lb.setItemText(reference_num,reference.getName(reference_num));
		}
		
		/*
		 */
		void combine_changed() {
			combine = combine_cb.isChecked();
		}
		
		void flip_y_changed() {
			flip_y = flip_y_cb.isChecked();
		}
		
		/**********************************************************************\
		*
		* Create
		*
		\**********************************************************************/
		
		/*
		 */
		int get_import_x0() {
			int x0 = import_offset_x;
			if(import_unit_x == UNIT_SOURCE) x0 = Common::terrain_size_x * (source_offset_x - import_offset_x) / import_size_x;
			return clamp(x0,0,Common::terrain_size_x);
		}
		
		int get_import_x1() {
			int x1 = import_offset_x + import_size_x;
			if(import_unit_x == UNIT_SOURCE) x1 = Common::terrain_size_x * (source_offset_x - import_offset_x + source_size_x) / import_size_x;
			return clamp(x1,0,Common::terrain_size_x);
		}
		
		int get_import_y0() {
			int y0 = import_offset_y;
			if(import_unit_y == UNIT_SOURCE) y0 = Common::terrain_size_y * (source_offset_y - import_offset_y) / import_size_y;
			return clamp(y0,0,Common::terrain_size_y);
		}
		
		int get_import_y1() {
			int y1 = import_offset_y + import_size_y;
			if(import_unit_y == UNIT_SOURCE) y1 = Common::terrain_size_y * (source_offset_y - import_offset_y + source_size_y) / import_size_y;
			return clamp(y1,0,Common::terrain_size_y);
		}
		
		/*
		 */
		int create_mask(Async async,Reference reference) {
			
			log.message("Importing node mask\n");
			
			// begin progress dialog
			dialogProgressBegin(TR("Importing Node Mask..."));
			
			// load diffuse image
			if(TiledImage::load(reference.mask_image_import,source_offset_x,source_offset_y,source_size_x,source_size_y,flip_y) == 0) {
				dialogProgressEnd();
				return 0;
			}
			
			// import size
			int import_x0 = get_import_x0();
			int import_x1 = get_import_x1();
			int import_y0 = get_import_y0();
			int import_y1 = get_import_y1();
			int import_size_x = import_x1 - import_x0;
			int import_size_y = import_y1 - import_y0;
			
			// mask size
			int mask_size = reference.mask_image_size;
			int mask_size_x = import_size_x * mask_size;
			int mask_size_y = import_size_y * mask_size;
			
			// import node mask
			forloop(int y = 0; import_size_y) {
				int Y = import_y0 + y;
				int mask_y = mask_size * y;
				
				forloop(int x = 0; import_size_x) {
					int X = import_x0 + x;
					int mask_x = mask_size * x;
					
					// create node mask
					ObjectTerrain terrain = Common::get_terrain(X,Y);
					if(terrain != NULL) {
						Image image = TiledImage::getImage(async,mask_size_x,mask_size_y,mask_x,mask_y,mask_size,mask_size,IMAGE_FORMAT_R8 + reference.mask_image_format);
						if(image == NULL) {
							dialogProgressEnd();
							TiledImage::clear();
							return 0;
						}
						string mask_name = dirname(systemEditorPath(terrain.getTerrainName())) + reference.mask_image_name;
						mkdir(engine.getDataPath() + dirname(mask_name),1);
						log.message("Creating \"%s\" image\n",mask_name);
						downsize_image(image,mask_name);
						int id = async.run(image,"save",mask_name);
						while(async.isRunning(id)) {
							usleep(WAIT_TIME);
							wait;
						}
						if(async.getResult(id) == 0) {
							log.error("Landscape::Nodes::create_mask(): can't save \"%s\" image\n",mask_name);
							dialogProgressEnd();
							TiledImage::clear();
							return 0;
						}
						delete image;
					}
					
					// update progress dialog
					int progress = (import_size_x * y + x + 1) * 100 / (import_size_x * import_size_y);
					if(dialogProgressUpdate(progress) == 0) {
						dialogProgressEnd();
						TiledImage::clear();
						return 0;
					}
				}
			}
			
			TiledImage::clear();
			
			// end progress dialog
			dialogProgressEnd();
			
			return 1;
		}
		
		/*
		 */
		Node create_node(ObjectTerrain terrain,Node parent,Node source,Reference reference) {
			
			// clone node
			Node node = node_append(node_cast(node_clone(source)));
			node.setWorldTransform(terrain.getWorldTransform() * translate(reference.node_offset));
			parent.addWorldChild(node);
			
			// relative names
			string mask_image_name = "";
			string mask_mesh_name = "";
			string node_layer_name = "";
			string terrain_name = dirname(systemEditorPath(terrain.getTerrainName()));
			if(strlen(reference.mask_image_name)) mask_image_name = terrain_name + reference.mask_image_name;
			if(strlen(reference.mask_mesh_name)) mask_mesh_name = terrain_name + reference.mask_mesh_name;
			if(strlen(reference.node_layer_name)) node_layer_name = terrain_name + reference.node_layer_name;
			
			// update node
			void update_node(Node node) {
				if(node.getType() == NODE_OBJECT_GRASS) {
					ObjectGrass grass = node;
					grass.setMaskImageName(mask_image_name);
					grass.setMaskMeshName(mask_mesh_name);
					grass.setMaskFlipX(0);
					grass.setMaskFlipY(0);
				}
				else if(node.getType() == NODE_OBJECT_MESH_CLUTTER) {
					ObjectMeshClutter clutter = node;
					clutter.setMaskImageName(mask_image_name);
					clutter.setMaskMeshName(mask_mesh_name);
					clutter.setMaskFlipX(0);
					clutter.setMaskFlipY(0);
				}
				else if(node.getType() == NODE_WORLD_CLUTTER) {
					WorldClutter clutter = node;
					clutter.setMaskImageName(mask_image_name);
					clutter.setMaskMeshName(mask_mesh_name);
					clutter.setMaskFlipX(0);
					clutter.setMaskFlipY(0);
				}
				else if(node.getType() == NODE_LAYER) {
					NodeLayer layer = node;
					layer.setNodeName(node_layer_name);
				}
				else if(node.getType() == NODE_WORLD_LAYER) {
					WorldLayer layer = node;
					layer.setNodeName(node_layer_name);
				}
				forloop(int i = 0; node.getNumChilds()) {
					update_node(node_cast(node.getChild(i)));
				}
			}
			update_node(node);
			
			return node;
		}
		
		/*
		 */
		Node get_parent(Node node,int depth) {
			if(depth == 0) return node;
			forloop(int i = 0; node.getNumChilds()) {
				Node parent = get_parent(node.getChild(i),depth - 1);
				if(parent != NULL) return parent;
			}
			return NULL;
		}
		
		/*
		 */
		int create_reference(Async async,Reference reference) {
			
			// load node reference
			Node root = engine.world.loadNode(reference.node_name);
			if(root == NULL) {
				dialogMessageOk(TR("Error"));
				return 0;
			}
			root = node_append(root);
			
			log.message("Creating terrain nodes\n");
			
			// begin progress dialog
			dialogProgressBegin(TR("Creating Terrain Nodes..."));
			
			// import size
			int import_x0 = get_import_x0();
			int import_x1 = get_import_x1();
			int import_y0 = get_import_y0();
			int import_y1 = get_import_y1();
			
			int import_size_x = import_x1 - import_x0;
			int import_size_y = import_y1 - import_y0;
			
			// nodes
			Node nodes[] = ();
			
			Image mask = new Image();
			
			// clear
			void clear() {
				forloop(int i = 0; nodes.size()) {
					node_delete(nodes[i]);
				}
				delete node_delete(root);
				dialogProgressEnd();
				delete mask;
			}
			
			// create terrain nodes
			forloop(int y = 0; import_size_y) {
				int Y = import_y0 + y;
				
				forloop(int x = 0; import_size_x) {
					int X = import_x0 + x;
					
					// create terrain nodes
					ObjectTerrain terrain = Common::get_terrain(X,Y);
					if(terrain != NULL) {
						
						log.message("Creating \"%s\" nodes\n",terrain.getTerrainName());
						
						// parent node
						Node parent = get_parent(terrain,reference.depth);
						if(parent == NULL) {
							dialogMessageOk(TR("Error"),format(TR("Can't find parent for \"%d\" depth"),reference.depth));
							clear();
							return 0;
						}
						
						// load mask image
						if(strlen(reference.mask_image_name)) {
							
							// check mask image
							string mask_name = dirname(systemEditorPath(terrain.getTerrainName())) + reference.mask_image_name;
							int id = async.run(mask,"load",mask_name);
							while(async.isRunning(id)) {
								usleep(WAIT_TIME);
								wait;
							}
							if(async.getResult(id) == 0) {
								dialogMessageOk(TR("Error"));
								clear();
								return 0;
							}
							
							// resize mask
							mask.convertToFormat(IMAGE_FORMAT_R16);
							mask.resize(min(64,mask.getWidth()),min(64,mask.getHeight()));
							mask.resize(1,1);
							
							// average mask color
							vec4 color = mask.get2D(0,0);
							if(color.x < MASK_THRESHOLD) continue;
						}
						
						// create nodes
						nodes.append(create_node(terrain,parent,root,reference));
					}
					
					// update progress dialog
					int progress = (import_size_x * y + x + 1) * 100 / (import_size_x * import_size_y);
					if(dialogProgressUpdate(progress) == 0) {
						clear();
						return 0;
					}
				}
			}
			
			// reassign nodes
			forloop(int i = 0; nodes.size()) {
				engine.editor.addNode(node_remove(nodes[i]));
			}
			
			nodesReload();
			
			delete node_delete(root);
			
			delete mask;
			
			// end progress dialog
			dialogProgressEnd();
			
			return 1;
		}
		
		/*
		 */
		void remove_node(Node node) {
			
			// remove child nodes
			for(int i = node.getNumChilds() - 1; i >= 0; i--) {
				remove_node(node.getChild(i));
			}
			
			// remove node
			if(engine.editor.isNode(node)) {
				engine.editor.removeNode(node);
			}
		}
		
		/*
		 */
		void clean_nodes(Async async) {
			
			log.message("Cleaning terrain nodes\n");
			
			// import size
			int import_x0 = get_import_x0();
			int import_x1 = get_import_x1();
			int import_y0 = get_import_y0();
			int import_y1 = get_import_y1();
			
			int import_size_x = import_x1 - import_x0;
			int import_size_y = import_y1 - import_y0;
			
			// clean terrain nodes
			forloop(int y = 0; import_size_y) {
				int Y = import_y0 + y;
				
				forloop(int x = 0; import_size_x) {
					int X = import_x0 + x;
					
					// remove child nodes
					ObjectTerrain terrain = Common::get_terrain(X,Y);
					if(terrain != NULL) {
						for(int i = terrain.getNumChilds() - 1; i >= 0; i--) {
							remove_node(terrain.getChild(i));
						}
					}
				}
			}
			
			nodesReload();
		}
		
		/*
		 */
		void combine_nodes(Async async) {
			
			log.message("Combining terrain nodes\n");
			
			// import size
			int import_x0 = get_import_x0();
			int import_x1 = get_import_x1();
			int import_y0 = get_import_y0();
			int import_y1 = get_import_y1();
			
			int import_size_x = import_x1 - import_x0;
			int import_size_y = import_y1 - import_y0;
			
			// combine terrain nodes
			forloop(int y = 0; import_size_y) {
				int Y = import_y0 + y;
				
				forloop(int x = 0; import_size_x) {
					int X = import_x0 + x;
					
					// combine terrain nodes
					ObjectTerrain terrain = Common::get_terrain(X,Y);
					if(terrain != NULL) {
						
						string reference_name = replace(systemEditorPath(terrain.getTerrainName()),".ter",".node");
						
						// get child nodes
						Node nodes[] = ();
						forloop(int i = 0; terrain.getNumChilds()) {
							Node node = terrain.getChild(i);
							if(engine.editor.isNode(node)) nodes.append(node);
						}
						
						// check terrain nodes
						if(nodes.size() != 0) {
							
							// reparent child nodes
							NodeDummy root = new NodeDummy();
							forloop(int i = 0; nodes.size()) {
								root.addChild(nodes[i]);
							}
							
							// save node reference
							log.message("Creating \"%s\" node\n",reference_name);
							engine.world.saveNode(reference_name,root,0);
							
							// remove child nodes
							forloop(int i = 0; nodes.size()) {
								remove_node(nodes[i]);
							}
							delete root;
							
							// create node reference
							NodeReference reference = new NodeReference(reference_name);
							engine.editor.addNode(class_remove(reference));
							reference.setName(terrain.getName());
							terrain.addChild(reference);
						}
						// remove node file
						else {
							
							log.message("Removing \"%s\" node\n",reference_name);
							remove(engine.getDataPath() + reference_name);
						}
					}
				}
			}
		}
		
		/*
		 */
		void update_clicked() {
			
			if(dialogMessageYesNo(TR("Confirm"),TR("Update landscape nodes?")) == 0) {
				return;
			}
			
			// check import size
			int import_x0 = get_import_x0();
			int import_x1 = get_import_x1();
			int import_y0 = get_import_y0();
			int import_y1 = get_import_y1();
			if(import_x0 == import_x1 || import_y0 == import_y1) {
				dialogMessageOk(TR("Error"),TR("Importing area is lower than terrain size"));
				return 0;
			}
			
			// asynchronous processing
			Async async = asyncGet();
			int ret = 1;
			
			// clean terrain nodes
			clean_nodes(async);
			
			// create nodes
			forloop(int i = 0; references.size()) {
				Reference reference = references[i];
				if(reference.node_import && strlen(reference.node_name)) {
					ret = create_reference(async,reference);
				}
			}
			
			// combine terrain nodes
			if(ret && combine) combine_nodes(async);
		}
		
		/*
		 */
		void create_clicked() {
			
			if(dialogMessageYesNo(TR("Confirm"),TR("Create landscape nodes?")) == 0) {
				return;
			}
			
			// check import size
			int import_x0 = get_import_x0();
			int import_x1 = get_import_x1();
			int import_y0 = get_import_y0();
			int import_y1 = get_import_y1();
			if(import_x0 == import_x1 || import_y0 == import_y1) {
				dialogMessageOk(TR("Error"),TR("Importing area is lower than terrain size"));
				return 0;
			}
			
			// asynchronous processing
			Async async = asyncGet();
			int ret = 1;
			
			// clean terrain nodes
			clean_nodes(async);
			
			// create nodes
			forloop(int i = 0; references.size()) {
				Reference reference = references[i];
				if(reference.node_import && strlen(reference.node_name)) {
					if(ret && strlen(reference.mask_image_name)) ret = create_mask(async,reference);
					if(ret) ret = create_reference(async,reference);
				}
			}
			
			// combine terrain nodes
			if(ret && combine) combine_nodes(async);
		}
	}
	
	void windowHide() {
		windowHide(window,meta.name);
	}
}
