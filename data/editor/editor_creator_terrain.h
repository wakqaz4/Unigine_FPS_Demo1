/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_creator_terrain.h
 * Desc:    Unigine editor
 * Version: 1.00
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

/*
 */
namespace Creator::Terrain {
	
	UserInterface ui;
	Dialog dialog;
	
	ComboBox create_size_x;
	ComboBox create_size_y;
	EditLine create_step;
	EditLine create_lod;
	
	enum {
		
		WAIT_TIME = 1000,
		SLEEP_TIME = 10000,
		
		MASK_THRESHOLD = 0.0001f,
		COMPARE_THRESHOLD = 2,
		
		SURFACE_SIZE = 128,
		TEXTURE_SIZE = 128,
		
		DIFFUSE_R = 127,
		DIFFUSE_G = 127,
		DIFFUSE_B = 127,
		DIFFUSE_A = 255,
		
		NORMAL_R = 127,
		NORMAL_G = 127,
		
		MASK_R = 0,
		
		INDEX_R = 0,
	};
	
	enum {
		TERRAIN_SURFACE_STEP = 256,
	};
	
	ObjectTerrain terrain;
	
	void init() {
		ui = new UserInterface("editor/editor_creator_terrain.ui");
		hotkeysAddUiToolTips(ui);
	}
	
	void shutdown() {
		hotkeysRemoveUiToolTips(ui);
		delete ui;
	}
	
	int show_dialog() {
		
		addChild(dialog,GUI_ALIGN_OVERLAP | GUI_ALIGN_CENTER);
		dialog.setPermanentFocus();
		
		while(dialog.isDone() == 0) {
			wait 1;
		}
		
		removeChild(dialog);
		
		return dialog.isOkClicked();
	}
	
	Node create() {
		
		if(!show_dialog()) return NULL;
		terrain = NULL;
		
		float editline_to_float(EditLine widget) {
			return float(widget.getText());
		}
		
		float combobox_to_int(ComboBox widget) {
			return int(widget.getCurrentItemText());
		}
		
		string terrain_name = "";
		string title = "";
		int mode = DIALOG_FILE_SAVE;
		title = TR("Select a terrain to save");
		
		if(dialogFileTerrain(title,terrain_name,mode) == 0) return NULL;
		
		if(dialogFileConfirm(savePath(terrain_name)) == 0) return NULL;
		
		terrain = new ObjectTerrain();
		
		if(terrain.create(combobox_to_int(create_size_x),combobox_to_int(create_size_y),editline_to_float(create_step)) == 0) {
			dialogMessageOk(TR("Error"));
			delete terrain;
			return NULL;
		}
		
		terrain_name = extension(terrain_name,"ter");
		if(terrain.saveTerrain(terrain_name,1) == 0) {
			dialogMessageOk(TR("Error"));
			delete terrain;
			return NULL;
		}
		
		terrain.setTerrainName(terrain_name);
		terrain.setLodDistance(editline_to_float(create_lod));
		
		
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
		
		terrain_name = extension(systemEditorPath(terrain.getTerrainName()),NULL);
		
		create_texture<Diffuse,"_d",diffuse>;
		create_texture<Normal,"_n",normal>;
		create_texture<Mask,"_m",mask>;
		create_texture<Index,"_i",index>;
		
		Image array =  new Image();
		
		array.create2DArray(TEXTURE_SIZE,TEXTURE_SIZE,1,IMAGE_FORMAT_DXT5);
		
		string array_name = terrain_name + "_array_d.dds";
		array.save(array_name);
		terrain.setDiffuseTextureArrayName(array_name);
		
		array_name = terrain_name + "_array_s.dds";
		array.save(array_name);
		terrain.setSpecularTextureArrayName(array_name);
		
		array.convertToFormat(IMAGE_FORMAT_ATI2);
		array_name = terrain_name + "_array_n.dds";
		array.save(array_name);
		terrain.setNormalTextureArrayName(array_name);
		
		delete diffuse;
		delete normal;
		delete mask;
		delete index;
		delete array;
		
		node_remove(terrain);
		texturesAddTerrain(terrain);
		terrain.setMaterial("terrain_base","*");
		return node_cast(terrain);
	}
	
	
	// templates
	template create_texture<TYPE,POSTFIX,IMAGE> {
		{
			string postfix = POSTFIX + ".dds";
			string coarse_name = terrain_name + postfix;
			mkdir(engine.getDataPath() + terrain_name);
			
			forloop(int y = 0; terrain.getSurfacesY()) {
				forloop(int x = 0; terrain.getSurfacesX()) {
					string surface_name = terrain_name + format("/%02xx%02x",x,y) + postfix;
					IMAGE.save(surface_name);
					terrain.setSurface ## TYPE ## TextureName(x,y,surface_name);
				}
			}
			
			IMAGE.save(coarse_name);
			terrain.set ## TYPE ## TextureName(coarse_name);
			
		}
	}
}
