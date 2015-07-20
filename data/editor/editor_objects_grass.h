/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_objects_grass.h
 * Desc:    Unigine editor
 * Version: 1.22
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Andrey Viktorov <unclebob@unigine.com>
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
* ObjectGrass
*
\******************************************************************************/

/*
 */
namespace Grass {
	
	/*
	 */
	WidgetVBox main_vb;
	
	WidgetLabel num_triangles_l;
	
	WidgetCheckBox thinning_cb;
	WidgetCheckBox variation_cb;
	WidgetCheckBox orientation_cb;
	WidgetCheckBox intersection_cb;
	WidgetCheckBox mask_flip_x_cb;
	WidgetCheckBox mask_flip_y_cb;
	WidgetCheckBox mask_inverse_cb;
	WidgetEditLine num_textures_el;
	WidgetEditLine mask_image_el;
	WidgetEditLine mask_mesh_el;
	WidgetIcon mask_edit_i;
	
	WidgetEditLine mask_min_value_el;
	WidgetEditLine mask_max_value_el;
	
	WidgetEditLine field_mask_el;
	WidgetEditLine shadow_radius_el;
	
	WidgetEditLine spawn_rate_el;
	WidgetEditLine seed_el;
	
	WidgetEditLine size_x_el;
	WidgetEditLine size_y_el;
	WidgetEditLine step_el;
	WidgetEditLine subdivision_el;
	
	WidgetEditLine density_el;
	WidgetSlider density_sl;
	WidgetEditLine threshold_el;
	WidgetSlider threshold_sl;
	WidgetEditLine angle_el;
	WidgetSlider angle_sl;
	
	WidgetEditLine probability_el[4];
	
	WidgetEditLine min_height_mean_el[4];
	WidgetEditLine min_height_spread_el[4];
	WidgetEditLine max_height_mean_el[4];
	WidgetEditLine max_height_spread_el[4];
	
	WidgetEditLine aspect_mean_el[4];
	WidgetEditLine aspect_spread_el[4];
	WidgetEditLine offset_mean_el[4];
	WidgetEditLine offset_spread_el[4];
	
	ObjectGrass grass;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("Grass"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
	}
	
	void shutdown() {
		
		brushesDisable();
		
		Nodes::Object::shutdown();
	}
	
	/*
	 */
	void update() {
		
		Nodes::Object::update();
		
		grass = node_cast(node);
		
		setCallbackEnabled(GUI_CLICKED,0,(
			thinning_cb,
			variation_cb,
			orientation_cb,
			intersection_cb,
			mask_flip_x_cb,
			mask_flip_y_cb,
			mask_inverse_cb,
			mask_edit_i,
		));
		
		thinning_cb.setChecked(grass.getThinning());
		variation_cb.setChecked(grass.getVariation());
		orientation_cb.setChecked(grass.getOrientation());
		intersection_cb.setChecked(grass.getIntersection());
		mask_flip_x_cb.setChecked(grass.getMaskFlipX());
		mask_flip_y_cb.setChecked(grass.getMaskFlipY());
		mask_inverse_cb.setChecked(grass.getMaskInverse());
		num_textures_el.setText(string(grass.getNumTextures()));
		
		mask_image_el.setText(grass.getMaskImageName());
		mask_mesh_el.setText(grass.getMaskMeshName());
		
		mask_min_value_el.setText(string(grass.getMaskMinValue()));
		mask_max_value_el.setText(string(grass.getMaskMaxValue()));
		
		field_mask_el.setText(editorFormatMask(grass.getFieldMask()));
		shadow_radius_el.setText(editorFormat(grass.getShadowRadius()));
		
		spawn_rate_el.setText(string(grass.getSpawnRate()));
		seed_el.setText(string(grass.getSeed()));
		
		size_x_el.setText(editorFormat(grass.getSizeX()));
		size_y_el.setText(editorFormat(grass.getSizeY()));
		step_el.setText(editorFormat(grass.getStep()));
		subdivision_el.setText(string(grass.getSubdivision()));
		
		density_el.setText(editorFormat(grass.getDensity(),5));
		threshold_el.setText(editorFormat(grass.getThreshold()));
		angle_el.setText(editorFormat(grass.getAngle()));
		
		density_sl.setCallbackEnabled(GUI_CHANGED,0);
		density_sl.setValue(grass.getDensity() * density_sl.getMaxValue());
		density_sl.setCallbackEnabled(GUI_CHANGED,1);
		
		threshold_sl.setCallbackEnabled(GUI_CHANGED,0);
		threshold_sl.setValue(grass.getThreshold() * threshold_sl.getMaxValue());
		threshold_sl.setCallbackEnabled(GUI_CHANGED,1);
		
		angle_sl.setCallbackEnabled(GUI_CHANGED,0);
		angle_sl.setValue(grass.getAngle() * angle_sl.getMaxValue());
		angle_sl.setCallbackEnabled(GUI_CHANGED,1);
		
		setEditLineVec4(probability_el,grass.getProbability());
		
		setEditLineSameVec4(min_height_mean_el,grass.getMinHeightMean());
		setEditLineSameVec4(min_height_spread_el,grass.getMinHeightSpread());
		setEditLineSameVec4(max_height_mean_el,grass.getMaxHeightMean());
		setEditLineSameVec4(max_height_spread_el,grass.getMaxHeightSpread());
		
		setEditLineSameVec4(aspect_mean_el,grass.getAspectMean());
		setEditLineSameVec4(aspect_spread_el,grass.getAspectSpread());
		setEditLineSameVec4(offset_mean_el,grass.getOffsetMean());
		setEditLineSameVec4(offset_spread_el,grass.getOffsetSpread());
		
		update_mask_icons();
		
		setCallbackEnabled(GUI_CLICKED,1,(
			thinning_cb,
			variation_cb,
			orientation_cb,
			intersection_cb,
			mask_flip_x_cb,
			mask_flip_y_cb,
			mask_inverse_cb,
			mask_edit_i,
		));
		enableCallbacks(window);
	}
	
	/*
	 */
	void update_mask_icons() {
		mask_edit_i.setEnabled(mask_image_el.getText() != "");
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
		
		num_triangles_l.setText(string(grass.getNumTriangles(0)));
	}
	
	/*
	 */
	void randomize_clicked() {
		seed_el.setText(string(engine.game.getRandom(0,0x0fffffff)));
		seed_el.runCallback(GUI_PRESSED);
	}
	
	/*
	 */
	void mask_image_update_pressed() {
		mask_image_pressed();
		update_mask_icons();
	}
	
	void mask_image_load_update_clicked() {
		mask_image_load_clicked();
		update_mask_icons();
	}
	
	void mask_image_create_clicked() {
		
		int size = 0;
		int format = 0;
		if(dialogImageSizeFormat(TR("Mask size"),size,format) == 0) return;
		
		string name;
		if(dialogFileImage(TR("Select texture to save"),name,DIALOG_FILE_SAVE) == 0) return;
		
		Image image = new Image();
		image.create2D(size,size,format);
		
		if(image.save(name) == 0) {
			dialogMessageOk(TR("Error"));
			delete image;
			return;
		}
		
		log.message("Save %s %s into the \"%s\" file\n",image.getTypeName(),image.getFormatName(),name);
		
		delete image;
		
		grass.setMaskImageName(name);
		mask_image_el.setText(name);
		
		update_mask_icons();
	}
	
	void mask_image_edit_clicked() {
		maskEnable(grass,Brushes::Mask::CHANNEL_RGBA);
	}
	
	void mask_image_clear_update_clicked() {
		mask_image_clear_clicked();
		update_mask_icons();
	}
	
	/*
	 */
	void field_mask_changed(int mask) {
		grass.setFieldMask(mask);
		field_mask_el.setText(editorFormatMask(grass.getFieldMask()));
	}
	
	/*
	 */
	CLICKED_CALLBACK(thinning,grass,Thinning,grass)
	CLICKED_CALLBACK(variation,grass,Variation,grass)
	CLICKED_CALLBACK(orientation,grass,Orientation,grass)
	CLICKED_CALLBACK(intersection,grass,Intersection,grass)
	CLICKED_CALLBACK(mask_flip_x,grass,MaskFlipX,grass)
	CLICKED_CALLBACK(mask_flip_y,grass,MaskFlipY,grass)
	CLICKED_CALLBACK(mask_inverse,grass,MaskInverse,grass)
	PRESSED_CALLBACK(num_textures,grass,NumTextures,grass,int,modify)
	PRESSED_CALLBACK(mask_image,grass,MaskImageName,grass,string,modify)
	PRESSED_CALLBACK(mask_mesh,grass,MaskMeshName,grass,string,modify)
	PRESSED_CALLBACK(mask_min_value,grass,MaskMinValue,grass,int,modify)
	PRESSED_CALLBACK(mask_max_value,grass,MaskMaxValue,grass,int,modify)
	LOAD_CLICKED_CALLBACK(mask_image,grass,MaskImageName,TR("Select mask image"))
	VIEW_CLICKED_CALLBACK(mask_image,grass,MaskImageName,TR("Grass mask image"))
	CLEAR_CLICKED_CALLBACK(mask_image,grass,MaskImageName,TR("Clear mask image?"),NULL,"")
	LOAD_CLICKED_CALLBACK_MESH(mask_mesh,grass,MaskMeshName,TR("Select mask mesh"))
	CLEAR_CLICKED_CALLBACK(mask_mesh,grass,MaskMeshName,TR("Clear mask mesh?"),NULL,"")
	PRESSED_CALLBACK_MASK(field_mask,grass,FieldMask,grass);
	CLICKED_CALLBACK_MASK(field_mask,grass,FieldMask,grass,TR("Field mask"),"Nodes::Object::Grass::field_mask_changed")
	PRESSED_CALLBACK_FLOAT(shadow_radius,grass,ShadowRadius,grass,modify)
	PRESSED_CALLBACK(spawn_rate,grass,SpawnRate,grass,int,modify)
	PRESSED_CALLBACK(seed,grass,Seed,grass,int,modify)
	PRESSED_CALLBACK_FLOAT(size_x,grass,SizeX,grass,modify)
	PRESSED_CALLBACK_FLOAT(size_y,grass,SizeY,grass,modify)
	PRESSED_CALLBACK_FLOAT(step,grass,Step,grass,modify)
	PRESSED_CALLBACK(subdivision,grass,Subdivision,grass,int,modify)
	PRESSED_CALLBACK_SLIDER_PREC(density,grass,Density,5,grass,modify)
	PRESSED_CALLBACK_SLIDER(threshold,grass,Threshold,grass,modify)
	PRESSED_CALLBACK_SLIDER(angle,grass,Angle,grass,modify)
	PRESSED_CALLBACK_VEC4(probability,grass,Probability,grass,modify)
	PRESSED_CALLBACK_MEAN_SPREAD_VEC4_SAME(min_height,grass,MinHeight)
	PRESSED_CALLBACK_MEAN_SPREAD_VEC4_SAME(max_height,grass,MaxHeight)
	PRESSED_CALLBACK_MEAN_SPREAD_VEC4_SAME(aspect,grass,Aspect)
	PRESSED_CALLBACK_MEAN_SPREAD_VEC4_SAME(offset,grass,Offset)
	
} /* namespace Grass */
