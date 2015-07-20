/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_objects_terrain_parameters.h
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
* Terrain
*
\******************************************************************************/

/*
 */
namespace Parameters {
	using Unigine::Widgets;
	
	//
	template parameter_float<WIDGET,FUNC> {
		
		EditLine WIDGET ## _el;
		
		void WIDGET ## _changed(EditLine editline) {
			terrain.set ## FUNC(float(editline.getText()));
		}
		
		void WIDGET ## _update() {
			WIDGET ## _el.setText(editorFormat(terrain.get ## FUNC()));
			WIDGET ## _el.setCallback(GUI_PRESSED,functionid(WIDGET ## _changed),WIDGET ## _el);
		}
	}
	
	//
	template parameter_string<WIDGET,FUNC> {
		
		EditLine WIDGET ## _el;
		
		void WIDGET ## _changed(EditLine editline) {
			terrain.set ## FUNC(editline.getText());
		}
		
		void WIDGET ## _update() {
			WIDGET ## _el.setText(terrain.get ## FUNC());
			WIDGET ## _el.setCallback(GUI_CHANGED,functionid(WIDGET ## _changed),WIDGET ## _el);
		}
	}
	
	//
	template parameter_resolution<WIDGET,TYPE,FUNC> {
		
		ComboBox WIDGET ## _resolution_cb;
		Label WIDGET ## _resolution_l;
		
		void WIDGET ## _resolution_changed(ComboBox combobox) {
			resize_ ## TYPE<FUNC,get_resolution_ ## WIDGET()>;
			WIDGET ## _resolution_l.setText(get_density_ ## TYPE(get_resolution_ ## WIDGET()));
		}
		
		void WIDGET ## _resolution_init() {
			WIDGET ## _resolution_cb.setCallback(GUI_CHANGED,functionid(WIDGET ## _resolution_changed),WIDGET ## _resolution_cb);
		}
		
		void WIDGET ## _resolution_save(Xml xml) {
			xml.setChildData("resolution_"#WIDGET,WIDGET ## _resolution_cb.getCurrentItem());
		}
		
		void WIDGET ## _resolution_load(Xml xml) {
			WIDGET ## _resolution_cb.setCurrentItem(xml.getChildData("resolution_"#WIDGET,0));
			WIDGET ## _resolution_l.setText(get_density_ ## TYPE(get_resolution_ ## WIDGET()));
		}
		
		int get_resolution_ ## WIDGET() {
			return int(WIDGET ## _resolution_cb.getCurrentItemText());
		}
	}
	
	//
	template resize_coarse<FUNC,SIZE> {
		dialogProgressBegin(TR("Resize refined textures..."),0);
		string name = terrain.get ## FUNC ## TextureName();
		Image image = new Image(name);
		
		dialogProgressUpdate(33);
		if(image.isCompressedFormat() && image.getHeight() < SIZE) {
			image.decompress();
		}
		
		dialogProgressUpdate(66);
		image.resize(SIZE,SIZE);
		
		image.save(name);
		dialogProgressEnd();
	}
	
	//
	template resize_refined<FUNC,SIZE> {
		dialogProgressBegin(TR("Resize refined textures..."),0);
		forloop(int y = 0; terrain.getSurfacesY()) {
			forloop(int x = 0; terrain.getSurfacesX()) {
				int progress = (terrain.getSurfacesX() * y + x + 1) * 100 / (terrain.getSurfacesX() * terrain.getSurfacesY());
				dialogProgressUpdate(progress);
				
				string name = terrain.getSurface ## FUNC ## TextureName(x,y);
				Image image = new Image(name);
				
				if(image.getHeight() != 4) {
					if(image.isCompressedFormat() || image.isZCompressedFormat()) image.decompress();
					
					image.resize(SIZE,SIZE);
					image.save(name);
				}
				delete image;
			}
		}
		dialogProgressEnd();
	}
	
	//
	UserInterface ui;
	VBox vb;
	
	Label grid_l;
	Label step_l;
	Label chunks_l;
	
	Label root_folder_l;
	Label terrain_name_l;
	
	Label refined_diffuse_name_l;
	Label refined_normal_name_l;
	Label refined_mask_name_l;
	Label refined_index_name_l;
	
	parameter_resolution<refined_diffuse,refined,Diffuse>;
	parameter_resolution<refined_normal,refined,Normal>;
	parameter_resolution<refined_mask,refined,Mask>;
	
	parameter_resolution<coarse_diffuse,coarse,Diffuse>;
	parameter_resolution<coarse_normal,coarse,Normal>;
	parameter_resolution<coarse_mask,coarse,Mask>;
	
	
	parameter_string<coarse_diffuse_texture,DiffuseTextureName>;
	parameter_string<coarse_normal_texture,NormalTextureName>;
	parameter_string<coarse_index_texture,IndexTextureName>;
	parameter_string<coarse_mask_texture,MaskTextureName>;
	
	parameter_float<coarse_distance,LodProgression>;
	parameter_float<lod_distance,LodDistance>;
	parameter_float<visible_distance,VisibleDistance>;
	parameter_float<shadow_distance,ShadowDistance>;
	parameter_float<shadow_radius,ShadowRadius>;
	parameter_float<lod_threshold,LodThreshold>;
	
	/*
	 */
	void init() {
		ui = new UserInterface("editor/editor_objects_terrain_parameters.ui","Nodes::Object::Terrain::");
		hotkeysAddUiToolTips(ui);
		
		parameters_tb.addTab(TR("Parameters"));
		if(vb == NULL) return;
		parameters_tb.addChild(vb.widget,GUI_ALIGN_EXPAND);
	}
	
	/*
	 */
	void shutdown() {
		hotkeysRemoveUiToolTips(ui);
		delete ui;
	}
	
	/*
	 */
	void update_dynamic() {
	}
	
	/*
	 */
	void update() {
		
		root_folder_l.setText(dirname(systemEditorPath(terrain.getTerrainName())));
		
		string terrain_name = extension(basename(systemEditorPath(terrain.getTerrainName())),NULL);
		terrain_name_l.setText(terrain_name);
		
		grid_l.setText(format("%dx%d",terrain.getSizeX(),terrain.getSizeY()));
		step_l.setText(editorFormat(terrain.getStep()));
		chunks_l.setText(format("%dx%d",terrain.getSurfacesX(),terrain.getSurfacesY()));
		
		refined_diffuse_name_l.setText(terrain_name +"/%xy_d.dds");
		refined_normal_name_l.setText(terrain_name +"/%xy_n.dds");
		refined_mask_name_l.setText(terrain_name +"/%xy_m.dds");
		refined_index_name_l.setText(terrain_name +"/%xy_i.dds");
		
		refined_diffuse_resolution_init();
		refined_normal_resolution_init();
		refined_mask_resolution_init();
		
		coarse_diffuse_resolution_init();
		coarse_normal_resolution_init();
		coarse_mask_resolution_init();
		
		coarse_diffuse_texture_update();
		coarse_normal_texture_update();
		coarse_index_texture_update();
		coarse_mask_texture_update();
		
		lod_threshold_update();
		coarse_distance_update();
		lod_distance_update();
		visible_distance_update();
		shadow_distance_update();
		shadow_radius_update();
		lod_threshold_update();
	}
	
	/*
	 */
	void edit_clicked() {
		terrainEnable(terrain,Parameters::get_resolution_refined_mask(),Parameters::get_resolution_coarse_mask());
	}
	
	/*
	 */
	void save(Xml xml) {
		refined_diffuse_resolution_save(xml);
		refined_normal_resolution_save(xml);
		refined_mask_resolution_save(xml);
		
		coarse_diffuse_resolution_save(xml);
		coarse_normal_resolution_save(xml);
		coarse_mask_resolution_save(xml);
	}
	
	/*
	 */
	void load(Xml xml) {
		refined_diffuse_resolution_load(xml);
		refined_normal_resolution_load(xml);
		refined_mask_resolution_load(xml);
		
		coarse_diffuse_resolution_load(xml);
		coarse_normal_resolution_load(xml);
		coarse_mask_resolution_load(xml);
	}
	
	/*
	 */
	void open_dialog_file(WidgetEditLine editline) {
		string name = editline.getText();
		if(dialogFile(TR("Select texture"),".dds",name,DIALOG_FILE_OPEN) == 0) return;
		editline.setText(name);
	}
	
	/*
	 */
	string get_density_refined(float resolution) {
		return editorFormat(((terrain.getSizeX() -1) * terrain.getStep()) / (terrain.getSurfacesX() * resolution),1);
	}
	
	string get_density_coarse(float resolution) {
		return editorFormat(((terrain.getSizeX() - 1) * terrain.getStep()) / resolution,1);
	}
}
