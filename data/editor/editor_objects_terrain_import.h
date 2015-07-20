/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_objects_terrain_import.h
 * Desc:    Unigine editor
 * Version: 1.23
 * Author:  Mikhail Lyubimov <alterego@unigine.com>
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
* Terrain Import
*
\******************************************************************************/

/*
 */
namespace Import {
	using Unigine::Widgets;
	
	//
	UserInterface ui;
	VBox vb;
	GridBox masks_gb;
	
	//
	EditLine height_name_el;
	EditLine holes_name_el;
	EditLine diffuse_name_el;
	EditLine normal_name_el;
	
	//
	Icon height_view_i;
	Icon holes_view_i;
	Icon diffuse_view_i;
	Icon normal_view_i;
	
	//
	CheckBox height_cb;
	CheckBox holes_cb;
	CheckBox diffuse_cb;
	CheckBox normal_cb;
	CheckBox masks_cb;
	
	CheckBox coarse_cb;
	CheckBox refined_cb;
	
	//
	Slider num_channels_sl;
	
	//
	EditLine height_scale_el;
	
	//
	string masks[];
	
	/*
	 */
	class Mask {
		
		//
		CheckBox checked_cb;
		ComboBox channel_cb;
		ComboBox depth_cb;
		EditLine file_el;
		HBox hb;
		Icon icon_load;
		Icon icon_view;
		
		//
		Mask(string name) {
			
			checked_cb = new CheckBox();
			checked_cb.setChecked(1);
			checked_cb.setText(name);
			
			file_el = new EditLine();
			file_el.setCallback(GUI_CHANGED,functionid(changed_file),this);
			
			hb = new HBox();
			
			channel_cb = new ComboBox();
			channel_cb.addItem("R");
			channel_cb.addItem("G");
			channel_cb.addItem("B");
			channel_cb.addItem("A");
			
			depth_cb = new ComboBox();
			forloop(int i = 0; 128) {
				depth_cb.addItem("" + i);
			}
			depth_cb.setHidden(1);
			
			icon_load = new Icon("editor/gui/action_load.png");
			icon_load.setWidth(22);
			icon_load.setHeight(20);
			icon_load.setCallback(GUI_CLICKED,functionid(open_mask_dialog),file_el.widget);
			
			
			icon_view = new Icon("editor/gui/action_view.png");
			icon_view.setWidth(22);
			icon_view.setHeight(20);
			icon_view.setCallback(GUI_CLICKED,functionid(view_image),file_el.widget);
		}
		
		~Mask() {
			delete checked_cb;
			delete file_el;
			delete hb;
			delete channel_cb;
			delete depth_cb;
			delete icon_load;
			delete icon_view;
		}
		
		//
		void changed_file(Mask mask) {
			string file_input = savePath(mask.file_el.getText());
			
			if(!is_file(file_input)) {
				mask.icon_view.setEnabled(0);
				mask.channel_cb.setEnabled(0);
				mask.depth_cb.setEnabled(0);
				return;
			} else {
				mask.icon_view.setEnabled(1);
				mask.channel_cb.setEnabled(1);
				mask.depth_cb.setEnabled(1);
			}
			
			Image image = new Image();
			if(image.info(file_input) == 0) image.load(file_input);
			
			int num_chanels = image.getNumChannels();
			
			int channel = mask.channel_cb.getCurrentItem();
			
			string names[] = ("R","G","B","A");
			
			mask.channel_cb.clear();
			forloop(int i = 0; num_chanels) {
				mask.channel_cb.addItem(names[i]);
			}
			
			if(num_chanels > channel) mask.channel_cb.setCurrentItem(channel);
			
			int max_depth = image.getDepth(0);
			if(max_depth > 1) {
				
				int depth = mask.depth_cb.getCurrentItem();
				
				mask.depth_cb.clear();
				forloop(int i = 0; max_depth) {
					mask.depth_cb.addItem("" + i);
				}
				
				if(max_depth > depth) mask.depth_cb.setCurrentItem(depth);
				mask.depth_cb.setHidden(0);
				mask.channel_cb.setHidden(1);
			} else {
				mask.depth_cb.setHidden(1);
				mask.channel_cb.setHidden(0);
			}
			
			delete image;
		}
		
		//
		Image getImage(int size,string &error) {
			
			if(!checked_cb.isChecked()) return NULL;
			
			string file_input = savePath(file_el.getText());
			if(!is_file(file_input)) return NULL;
			
			Image image = new Image();
			if(!image.load(file_input)) return NULL;
			
			if(image.isZCompressedFormat()) image.decompress();
			if(image.isCompressedFormat()) image.decompress();
			
			if(image.getDepth(0) > 1) {
				Image source = image;
				image = new Image();
				image.create2D(source.getHeight(),source.getWidth(),IMAGE_FORMAT_R8);
				image.copy(source,0,0,0,source.getHeight() * depth_cb.getCurrentItem(),source.getHeight(),source.getWidth());
				delete source;
			} else {
				image.copy(image,0,channel_cb.getCurrentItem());
				
				if(image.isFloatFormat() || image.isHalfFormat()) {
					error = format(TR("'%s' mask import: unsupported texture format (%s). Only 2D textures with unsigned texture format can be used."),checked_cb.getText(),image.getFormatName());
					image.clear();
					return NULL;
				}
				
				image.convertToFormat(IMAGE_FORMAT_R8);
			}
			
			image.flipY();
			
			image.resize(size,size);
			return image;
		}
		
		//
		void add() {
			masks_gb.addChild(checked_cb,GUI_ALIGN_LEFT);
			masks_gb.addChild(file_el,GUI_ALIGN_EXPAND);
			masks_gb.addChild(hb);
			hb.addChild(channel_cb);
			hb.addChild(depth_cb);
			hb.addChild(icon_load);
			hb.addChild(icon_view);
		}
		
		void remove() {
			masks_gb.removeChild(checked_cb);
			masks_gb.removeChild(file_el);
			masks_gb.removeChild(hb);
		}
		
		//
		void setName(string name) {
			checked_cb.setText(name);
		}
		
		//
		void getImageName() {
			return savePath(file_el.getText());
		}
		
		//
		void load(Xml xml,int i) {
			file_el.setText(xml.getChildData("mask_file_" + i,""));
			checked_cb.setChecked(xml.getChildData("mask_checked_" + i,1));
			
			int channel = xml.getChildData("mask_channel_" + i,0);
			if(channel_cb.getNumItems() > channel) channel_cb.setCurrentItem(channel);
			
			int depth = xml.getChildData("mask_depth_" + i,0);
			if(depth_cb.getNumItems() > depth) depth_cb.setCurrentItem(depth);
		}
		
		void save(Xml xml,int i) {
			xml.setChildData("mask_file_" + i,file_el.getText());
			xml.setChildData("mask_channel_" + i,channel_cb.getCurrentItem());
			xml.setChildData("mask_depth_" + i,depth_cb.getCurrentItem());
			xml.setChildData("mask_checked_" + i,checked_cb.isChecked());
		}
		
		//
		int getSize() {
			Image image = new Image();
			
			string file_input = savePath(file_el.getText());
			if(!is_file(file_input)) return;
			
			if(image.info(file_input) == 0) image.load(file_input);
			
			int size = image.getHeight();
			delete image;
			
			return size;
		}
	};
	
	/*
	 */
	void init() {
		ui = new UserInterface("editor/editor_objects_terrain_import.ui","Nodes::Object::Terrain::");
		hotkeysAddUiToolTips(ui);
		
		parameters_tb.addTab(TR("Import"));
		if(vb == NULL) return;
		parameters_tb.addChild(vb.widget,GUI_ALIGN_EXPAND);
	}
	
	/*
	 */
	void shutdown() {
		hotkeysRemoveUiToolTips(ui);
		delete ui;
		masks.delete();
	}
	
	/*
	 */
	void update_dynamic() {
		masks_gb.setEnabled(masks_cb.isChecked());
		
		height_view_i.setEnabled(is_file(savePath(height_name_el.getText())));
		holes_view_i.setEnabled(is_file(savePath(holes_name_el.getText())));
		diffuse_view_i.setEnabled(is_file(savePath(diffuse_name_el.getText())));
		normal_view_i.setEnabled(is_file(savePath(normal_name_el.getText())));
	}
	
	/*
	 */
	void update() {
		update_materials();
	}
	
	/*
	 */
	void update_materials() {
		foreach(Mask mask; masks) {
			mask.remove();
		}
		
		forloop(int i = 0; terrain.getNumMaterials()) {
			string name = terrain.getMaterialName(i);
			
			if(masks.check(name) == 0) {
				Mask mask = new Mask(name);
				Mask::changed_file(mask);
				masks.append(name,mask);
			}
			
			Mask(masks[name]).add();
		}
	}
	
	/*
	 */
	void import_clicked() {
		
		string errors = "";
		
		Image image_input = new Image();
		
		int load_image(string path) {
			
			image_input.clear();
			
			path = trim(path);
			if(path == "") return false;
			
			path = savePath(path);
			if(!is_file(path)) return false;
			
			if(!image_input.load(path)) {
				errors += format(TR("Failed to load image: %s"),path);
				return false;
			}
			
			if(image_input.isZCompressedFormat()) image_input.decompress();
			if(image_input.isCompressedFormat()) image_input.decompress();
			
			image_input.flipY();
			
			return true;
		}
		
		int check_image() {
			
			if(image_input.getType() != IMAGE_2D) {
				errors += format(TR(" import: unsupported texture type (%s). Only 2D textures with unsigned texture format can be used."),image_input.getTypeName()) + "\n";
				image_input.clear();
				return false;
			}
			
			if(image_input.isFloatFormat() || image_input.isHalfFormat()) {
				errors += format(TR(" import: unsupported texture format (%s). Only 2D textures with unsigned texture format can be used."),image_input.getFormatName()) + "\n";
				image_input.clear();
				return false;
			}
			
			return true;
		}
		
		if(height_cb.isChecked() && load_image(height_name_el.getText())) import_heights(image_input,errors);
		if(holes_cb.isChecked() && load_image(holes_name_el.getText())) import_holes(image_input,errors);
		
		if(refined_cb.isChecked() || coarse_cb.isChecked()) {
			if(diffuse_cb.isChecked() && load_image(diffuse_name_el.getText()) && check_image()) terrain_import<Diffuse,"_d",IMAGE_FORMAT_RGB8,diffuse>;
			if(normal_cb.isChecked() && load_image(normal_name_el.getText()) && check_image()) terrain_import<Normal,"_n",IMAGE_FORMAT_RG8,normal>;
			
			if(masks_cb.isChecked()) import_masks(errors);
		}
		
		delete image_input;
		update_view();
		Surfaces::update_view();
		
		errors = trim(errors,"\n");
		if(errors != "") {
			string title = (strchr(errors,'\n') >= 0 ? TR("Errors") : TR("Error"));
			dialogMessageOk(title,errors);
		}
	}
	
	/*
	 */
	void import_masks(string &errors) {
		
		int num_materials = terrain.getNumMaterials();
		if(num_materials == 0) return;
		
		int is_empty = 1;
		forloop(int i = 0; num_materials) {
			if(is_file(Mask(masks[terrain.getMaterialName(i)]).getImageName())) {
				is_empty = 0;
				break;
			}
		}
		if(is_empty) return;
		
		dialogProgressBegin(TR("Creating surface textures..."),0);
		
		int surface_size = Parameters::get_resolution_refined_mask();
		
		int mask_size = 128;
		
		forloop(int i = 0; num_materials) {
			int new_size = Mask(masks[terrain.getMaterialName(i)]).getSize();
			if(mask_size < new_size) mask_size = new_size;
		}
		
		Image index = new Image();
		Image mask_array = new Image();
		mask_array.create2DArray(mask_size,mask_size,num_materials,IMAGE_FORMAT_R8);
		
		forloop(int i = 0; num_materials) {
			
			string error_message = "";
			Image mask = Mask(masks[terrain.getMaterialName(i)]).getImage(mask_size,error_message);
			
			if(mask == NULL) {
				if(error_message != "") errors += error_message + "\n";
				continue;
			}
			
			mask_array.copy(mask,i);
			delete mask;
			int progress = i * 100 / num_materials;
			dialogProgressUpdate(progress);
		}
		dialogProgressEnd();
		
		
		dialogProgressBegin(TR("Creating surface textures..."),0);
		
		if(refined_cb.isChecked()) {
			forloop(int y = 0; terrain.getSurfacesY()) {
				forloop(int x = 0; terrain.getSurfacesX()) {
					
					int height = mask_size / terrain.getSurfacesY();
					int width = mask_size / terrain.getSurfacesX();
					
					Image layer = new Image();
					layer.create2D(width,height,IMAGE_FORMAT_R8);
					
					Image mask = new Image();
					mask.create2DArray(width,height,num_materials,IMAGE_FORMAT_R8);
					
					forloop(int i = 0; num_materials) {
						layer.copy(mask_array,0,0,width * x,i * mask_array.getHeight() + height * y,width,height);
						mask.copy(layer,i);
					}
					
					if(surface_size < height) mask.resize(surface_size,surface_size);
					
					engine.editor.createTerrainIndex(mask,index,num_channels_sl.getValue());
					mask.resize(surface_size,surface_size);
					
					string name = extension(systemEditorPath(terrain.getTerrainName()),NULL) + format("/%02xx%02x",x,y);
					
					mask.save(name + "_m.dds");
					index.save(name + "_i.dds");
					
					log.message("Save: %s\n",name);
					
					terrain.setSurfaceMaskTextureName(x,y,name + "_m.dds");
					terrain.setSurfaceIndexTextureName(x,y,name + "_i.dds");
					
					delete mask;
					delete layer;
					
					int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
					dialogProgressUpdate(progress);
				}
			}
		}
		
		if(coarse_cb.isChecked()) {
			mask_array.resize(Parameters::get_resolution_coarse_mask(),Parameters::get_resolution_coarse_mask());
			engine.editor.createTerrainIndex(mask_array,index,num_channels_sl.getValue());
			
			string coarse_name =  extension(systemEditorPath(terrain.getTerrainName()),NULL);
			
			mask_array.save(coarse_name + "_m.dds");
			index.save(coarse_name + "_i.dds");
			
			terrain.setMaskTextureName(coarse_name + "_m.dds");
			terrain.setIndexTextureName(coarse_name + "_i.dds");
		}
		
		dialogProgressEnd();
		
		delete mask_array;
		delete index;
	}
	
	/*
	 */
	template terrain_import<TYPE,POSTFIX,IMAGE_FORMAT,RESOLUTION_NAME> {
		{
			int surface_size = Parameters::get_resolution_refined_ ## RESOLUTION_NAME();
			int coarse_size = Parameters::get_resolution_coarse_ ## RESOLUTION_NAME();
			
			image_input.convertToFormat(IMAGE_FORMAT);
			
			string postfix = POSTFIX + ".dds";
			string terrain_name = extension(systemEditorPath(terrain.getTerrainName()),NULL);
			string coarse_name = terrain_name + postfix;
			
			mkdir(engine.getDataPath() + terrain_name);
			
			image_input.resize(terrain.getSizeX(),terrain.getSizeY());
			
			if(refined_cb.isChecked()) {
				dialogProgressBegin(TR("Creating surface textures..."),0);
				
				int height = image_input.getHeight() / terrain.getSurfacesY();
				int width = image_input.getWidth() / terrain.getSurfacesX();
				Image surface = new Image();
				
				forloop(int y = 0; terrain.getSurfacesY()) {
					forloop(int x = 0; terrain.getSurfacesX()) {
						
						// pixel border for seams removing
						int copy_width = height;
						int copy_height = width;
						if(x != terrain.getSurfacesX() - 1) copy_width++;
						if(y != terrain.getSurfacesY() - 1) copy_height++;
						
						surface.create2D(copy_width,copy_height,IMAGE_FORMAT);
						surface.copy(image_input,0,0,width * x,height * y,copy_width,copy_height,1);
						surface.resize(surface_size,surface_size);
						
						string surface_name = terrain_name + format("/%02xx%02x",x,y) + postfix;
						surface.save(surface_name);
						
						surface.clear();
						
						terrain.setSurface ## TYPE ## TextureName(x,y,surface_name);
						
						int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
						dialogProgressUpdate(progress);
					}
				}
				
				delete surface;
				dialogProgressEnd();
			}
			
			if(coarse_cb.isChecked()) {
				image_input.resize(coarse_size,coarse_size);
				
				image_input.save(coarse_name);
				terrain.set ## TYPE ## TextureName(coarse_name);
			}
			
			image_input.clear();
		}
	}
	
	/*
	 */
	void import_heights(Image image_input,string &errors) {
		
		if(image_input.getType() != IMAGE_2D) {
			errors += format(TR("Heights import: unsupported texture type (%s). Only 2D textures with unsigned texture format can be used."),image_input.getTypeName()) + "\n";
			image_input.clear();
			return;
		}
		
		int convert_to[] = (
			IMAGE_FORMAT_RGBA8:		IMAGE_FORMAT_RGB8,
			IMAGE_FORMAT_RG16:		IMAGE_FORMAT_R16,
			IMAGE_FORMAT_RGB16:		IMAGE_FORMAT_R16,
			IMAGE_FORMAT_RGBA16:	IMAGE_FORMAT_R16,
			IMAGE_FORMAT_RG16F:		IMAGE_FORMAT_R16F,
			IMAGE_FORMAT_RGB16F:	IMAGE_FORMAT_R16F,
			IMAGE_FORMAT_RGBA16F:	IMAGE_FORMAT_R16F,
			IMAGE_FORMAT_RG32F:		IMAGE_FORMAT_R32F,
			IMAGE_FORMAT_RGB32F:	IMAGE_FORMAT_R32F,
			IMAGE_FORMAT_RGBA32F:	IMAGE_FORMAT_R32F,
		);
		
		image_input.resize(terrain.getSizeX(),terrain.getSizeY());
		
		int new_format = convert_to.check(image_input.getFormat(),-1);
		if(new_format != -1) image_input.convertToFormat(new_format);
		
		if(terrain.setHeights(0,0,image_input,int(height_scale_el.getText()))) terrain.saveTerrain(systemEditorPath(terrain.getTerrainName()));
		image_input.clear();
	}
	
	/*
	 */
	void import_holes(Image image_input,string &errors) {
		
		if(image_input.getType() != IMAGE_2D) {
			errors += format(TR("Holes import: unsupported texture type (%s). Only 2D textures with unsigned texture format can be used."),image_input.getTypeName()) + "\n";
			image_input.clear();
			return;
		}
		
		if(image_input.isFloatFormat() || image_input.isHalfFormat()) {
			errors += format(TR("Holes import: unsupported texture format (%s). Only 2D textures with unsigned texture format can be used."),image_input.getFormatName()) + "\n";
			image_input.clear();
			return;
		}
		
		image_input.convertToFormat(IMAGE_FORMAT_R8);
		image_input.resize(terrain.getSizeX(),terrain.getSizeY());
		
		if(terrain.setHoles(0,0,image_input)) terrain.saveTerrain(systemEditorPath(terrain.getTerrainName()));
		image_input.clear();
	}
	
	/*
	 */
	void save(Xml xml) {
		xml.setChildData("height_name",height_name_el.getText());
		xml.setChildData("diffuse_name",diffuse_name_el.getText());
		xml.setChildData("normal_name",normal_name_el.getText());
		xml.setChildData("holes_name",holes_name_el.getText());
		xml.setChildData("num_channels",num_channels_sl.getValue());
		
		xml.setChildData("height_checked",height_cb.isChecked());
		xml.setChildData("diffuse_checked",diffuse_cb.isChecked());
		xml.setChildData("normal_checked",normal_cb.isChecked());
		xml.setChildData("holes_checked",holes_cb.isChecked());
		xml.setChildData("masks_checked",masks_cb.isChecked());
		
		xml.setChildData("coarse_checked",coarse_cb.isChecked());
		xml.setChildData("refined_checked",refined_cb.isChecked());
		
		xml.setChildData("height_scale",height_scale_el.getText());
		
		forloop(int i = 0; terrain.getNumMaterials()) {
			
			string name = terrain.getMaterialName(i);
			if(masks.check(name) == 0) continue;
			
			Mask(masks[name]).save(xml,i);
		}
	}
	
	/*
	 */
	void load(Xml xml) {
		height_name_el.setText(xml.getChildData("height_name",""));
		diffuse_name_el.setText(xml.getChildData("diffuse_name",""));
		normal_name_el.setText(xml.getChildData("normal_name",""));
		holes_name_el.setText(xml.getChildData("holes_name",""));
		num_channels_sl.setValue(xml.getChildData("num_channels",4));
		
		height_cb.setChecked(xml.getChildData("height_checked",1));
		diffuse_cb.setChecked(xml.getChildData("diffuse_checked",1));
		normal_cb.setChecked(xml.getChildData("normal_checked",1));
		holes_cb.setChecked(xml.getChildData("holes_checked",1));
		masks_cb.setChecked(xml.getChildData("masks_checked",1));
		
		coarse_cb.setChecked(xml.getChildData("coarse_checked",1));
		refined_cb.setChecked(xml.getChildData("refined_checked",1));
		
		height_scale_el.setText(xml.getChildData("height_scale","1"));
		
		forloop(int i = 0; terrain.getNumMaterials()) {
			string name = terrain.getMaterialName(i);
			
			Mask mask = new Mask(name);
			masks.append(name,mask);
			
			mask.load(xml,i);
		}
	}
	
	/*
	 */
	int check_new_name(string name, string new_name) {
		if(new_name == name) return 0;
		if(masks.check(new_name)) return 0;
		if(masks.check(name) == 0) return 0;
		return 1;
	}
	
	/*
	 */
	int change_name(string name, string new_name) {
		
		if(check_new_name(name,new_name) == 0) return;
		
		Mask mask = masks[name];
		mask.setName(new_name);
		
		masks.append(new_name,mask);
		
		masks.remove(name);
		
		update_materials();
		
		return 1;
	}
	
	/*
	 */
	void addMask(string name) {	
		if(masks.check(name)) return;
		
		Mask mask = new Mask(name);
		masks.append(name,mask);
		update_materials();
	}
	
	/*
	 */
	void removeMask(string name) {
		
		if(masks.check(name) == 0) return;
		
		Mask mask = masks[name];
		mask.remove();
		
		masks.remove(name);
		delete mask;
	}
	
	/*
	 */
	int check_type(string name) {
		
		Image image = new Image();
		int result = true;
		
		string path = savePath(name);
		if(!image.info(path) && !image.load(path)) {
			dialogMessageOk(TR("Error"),format(TR("Failed to get image info: %s"),path));
			result = false;
		} else if(image.getType() != IMAGE_2D) {
			dialogMessageOk(TR("Error"),format(TR("Unsupported texture type (%s). Only 2D textures with unsigned texture format can be used."),image.getTypeName()));
			result = false;
		}
		
		delete image;
		return result;
	}
	
	/*
	 */
	int check_format(string name) {
		
		Image image = new Image();
		int result = true;
		
		string path = savePath(name);
		if(!image.info(path) && !image.load(path)) {
			dialogMessageOk(TR("Error"),format(TR("Failed to get image info: %s"),path));
			result = false;
		} else if(image.isFloatFormat() || image.isHalfFormat()) {
			dialogMessageOk(TR("Error"),format(TR("Unsupported texture format (%s). Only 2D textures with unsigned texture format can be used."),image.getFormatName()));
			result = false;
		}
		
		delete image;
		return result;
	}
	
	/*
	 */
	int check_type_format(string name) {
		
		Image image = new Image();
		int result = true;
		
		string path = savePath(name);
		if(!image.info(path) && !image.load(path)) {
			dialogMessageOk(TR("Error"),format(TR("Failed to get image info: %s"),path));
			result = false;
		} else if(image.getType() != IMAGE_2D) {
			dialogMessageOk(TR("Error"),format(TR("Unsupported texture type (%s). Only 2D textures with unsigned texture format can be used."),image.getTypeName()));
			result = false;
		} else if(image.isFloatFormat() || image.isHalfFormat()) {
			dialogMessageOk(TR("Error"),format(TR("Unsupported texture format (%s). Only 2D textures with unsigned texture format can be used."),image.getFormatName()));
			result = false;
		}
		
		delete image;
		return result;
	}
	
	/*
	 */
	template declare_open_file_dialog<NAME,DIALOG_FUN,CHECK_FUN> {
		void NAME(WidgetEditLine editline) {
			
			string name = editline.getText();
			
			if(DIALOG_FUN(TR("Select texture"),name,DIALOG_FILE_OPEN) == 0) return;
			
			if(CHECK_FUN(name)) editline.setText(name);
		}
	}
	
	declare_open_file_dialog<open_heights_dialog,dialogFileImage,check_type>;
	declare_open_file_dialog<open_texture_dialog,dialogFileImageNoHDR,check_type_format>;
	declare_open_file_dialog<open_mask_dialog,dialogFileImageNoHDR,check_format>;
}
