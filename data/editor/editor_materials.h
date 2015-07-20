/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_materials.h
 * Desc:    Unigine editor
 * Version: 1.52
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
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
namespace Materials {
	
	/*
	 */
	DECLARE_WINDOW
	
	WidgetHPaned window_hp;					// materials hpaned
	
	WidgetVBox material_vb;					// material vbox
	WidgetSpriteMaterialView material_smv;	// material view
	
	WidgetIcon libraries_i;					// material libraries
	WidgetIcon save_i;						// save materials
	WidgetIcon assign_i;					// assign material
	WidgetIcon clone_i;						// clone material
	WidgetIcon inherit_i;					// inherit material
	WidgetIcon move_i;						// move material
	WidgetIcon remove_i;					// remove material
	
	WidgetComboBox libraries_cb;			// libraries combobox
	WidgetEditLine name_el;					// current material name
	WidgetTreeBox materials_tb;				// materials treebox
	int materials_folded[];					// materials hidden flag
	
	Material material;						// current material
	Material parent;						// parent material
	
	WidgetTabBox material_tb;				// material container
	
	WidgetVBox common_vb;					// material common container
	
	WidgetLabel info_library_l;				// library name
	WidgetLabel info_name_l;				// material name
	WidgetLabel info_parent_l;				// parent name
	WidgetLabel info_childs_l;				// number of children
	
	WidgetComboBox blend_src_func_cb;		// blending source function
	WidgetLabel blend_src_func_l;
	WidgetComboBox blend_dest_func_cb;		// blending destination function
	WidgetLabel blend_dest_func_l;
	
	WidgetComboBox downsize_cb;				// downsize rendering
	WidgetLabel downsize_l;
	WidgetComboBox group_cb;				// rendering group
	WidgetLabel group_l;
	WidgetComboBox order_cb;				// rendering order
	WidgetLabel order_l;
	WidgetComboBox offset_cb;				// polygon offset
	WidgetLabel offset_l;
	
	WidgetEditLine light_mask_el;			// light mask
	WidgetLabel light_mask_l;
	WidgetEditLine viewport_mask_el;		// viewport mask
	WidgetLabel viewport_mask_l;
	
	WidgetCheckBox post_deferred_cb;		// material options
	WidgetCheckBox post_refraction_cb;
	WidgetCheckBox post_scattering_cb;
	WidgetCheckBox depth_mask_cb;
	WidgetCheckBox glow_mask_cb;
	WidgetCheckBox parallax_mask_cb;
	WidgetCheckBox occlusion_mask_cb;
	WidgetCheckBox alpha_test_cb;
	WidgetCheckBox two_sided_cb;
	WidgetCheckBox light_blending_cb;
	WidgetCheckBox cast_shadow_cb;
	WidgetCheckBox receive_shadow_cb;
	WidgetCheckBox cast_world_shadow_cb;
	WidgetCheckBox receive_world_shadow_cb;
	WidgetCheckBox cast_translucent_cb;
	WidgetCheckBox receive_translucent_cb;
	
	WidgetButton copy_b;					// copy settings
	WidgetButton paste_b;					// paste settings
	
	WidgetVBox states_vb;					// material states container
	WidgetVBox textures_vb;					// material textures container
	WidgetVBox parameters_vb;				// material parameters container
	
	Blob blob;
	
	#include <editor/editor_materials_unredo.h>
	#include <editor/editor_materials_textures.h>
	#include <editor/editor_materials_parameters.h>
	#include <editor/editor_materials_states.h>
	#include <editor/editor_materials_editor.h>
	
	/**************************************************************************\
	*
	* Material
	*
	\**************************************************************************/
	
	/*
	 */
	int get_order_item(int order) {
		return order + 8;
	}
	
	int get_order(int item) {
		return item - 8;
	}
	
	/*
	 */
	int get_offset_item(int offset) {
		if(offset <= 2) return offset;
		if(offset <= 4) return 3;
		if(offset <= 8) return 4;
		if(offset <= 16) return 5;
		if(offset <= 32) return 6;
		if(offset <= 64) return 7;
		if(offset <= 128) return 8;
		if(offset <= 256) return 9;
		if(offset <= 512) return 10;
		return 11;
	}
	
	int get_offset(int item) {
		if(item <= 2) return item;
		if(item == 3) return 4;
		if(item == 4) return 8;
		if(item == 5) return 16;
		if(item == 6) return 32;
		if(item == 7) return 64;
		if(item == 8) return 128;
		if(item == 9) return 256;
		if(item == 10) return 512;
		return 1024;
	}
	
	/*
	 */
	string get_material_name() {
		if(material == NULL) return "";
		return material.getName();
	}
	
	/*
	 */
	void update_view() {
		
		// disable callbacks
		setCallbackEnabled(GUI_CHANGED,0,(
			blend_src_func_cb,
			blend_dest_func_cb,
			downsize_cb,
			group_cb,
			order_cb,
			offset_cb,
		));
		
		setCallbackEnabled(GUI_CLICKED,0,(
			post_deferred_cb,
			post_refraction_cb,
			post_scattering_cb,
			depth_mask_cb,
			glow_mask_cb,
			parallax_mask_cb,
			occlusion_mask_cb,
			alpha_test_cb,
			two_sided_cb,
			light_blending_cb,
			cast_shadow_cb,
			receive_shadow_cb,
			cast_world_shadow_cb,
			receive_world_shadow_cb,
			cast_translucent_cb,
			receive_translucent_cb,
		));
		
		// delete resources
		states.delete();
		textures.delete();
		parameters.delete();
		
		// clear material
		material = NULL;
		if(materials_tb.getCurrentItem() != -1) {
			material = engine.materials.findMaterial(materials_tb.getCurrentItemText());
			if(material == NULL) throw("Materials::update_view(): can't find \"%s\" material\n",materials_tb.getCurrentItemText());
		}
		
		// parent material
		parent = (material != NULL) ? material.getParent() : NULL;
		
		// clear view
		if(material == NULL) {
			
			material_smv.setMaterial(NULL);
			setEnabled(0,(
				common_vb,
				states_vb,
				textures_vb,
				parameters_vb,
			));
			
			// info
			info_library_l.setText("");
			info_name_l.setText("");
			info_parent_l.setText("");
			info_childs_l.setText("");
			
			// blending
			blend_src_func_cb.setCurrentItem(0);
			blend_dest_func_cb.setCurrentItem(0);
			blend_src_func_l.setFontColor(vec4_zero);
			blend_dest_func_l.setFontColor(vec4_zero);
			
			// downsize rendering
			downsize_cb.setCurrentItem(0);
			downsize_l.setFontColor(vec4_zero);
			
			// rendering group
			group_cb.setCurrentItem(0);
			group_l.setFontColor(vec4_zero);
			
			// rendering order
			order_cb.setCurrentItem(8);
			order_l.setFontColor(vec4_zero);
			
			// polygon offset
			offset_cb.setCurrentItem(0);
			offset_l.setFontColor(vec4_zero);
			
			// light mask
			light_mask_el.setText("");
			light_mask_l.setFontColor(vec4_zero);
			
			// viewport mask
			viewport_mask_el.setText("");
			viewport_mask_l.setFontColor(vec4_zero);
			
			// options
			WidgetCheckBox options[] = (
				post_deferred_cb,
				post_refraction_cb,
				post_scattering_cb,
				depth_mask_cb,
				glow_mask_cb,
				parallax_mask_cb,
				occlusion_mask_cb,
				alpha_test_cb,
				two_sided_cb,
				light_blending_cb,
				cast_shadow_cb,
				receive_shadow_cb,
				cast_world_shadow_cb,
				receive_world_shadow_cb,
				cast_translucent_cb,
				receive_translucent_cb,
			);
			setChecked(0,options);
			setFontColor(vec4_zero,options);
			
			// buttons
			setEnabled(0,(
				assign_i,
				clone_i,
				inherit_i,
				move_i,
				remove_i,
				copy_b,
				paste_b,
			));
		}
		// fill view
		else {
			
			int num_childs = material.getNumChilds();
			int editable = material.isEditable();
			int library = engine.materials.findMaterialLibrary(material.getName());
			int library_editable = engine.materials.isLibraryEditable(library);
			int enabled = (editable && library_editable);
			
			material_smv.setMaterial(material.getName());
			setEnabled(enabled,(
				common_vb,
				states_vb,
				textures_vb,
				parameters_vb,
			));
			
			// info
			info_library_l.setText(replace(basename(engine.materials.getLibraryName(library)),".mat",""));
			info_name_l.setText(material.getName());
			info_parent_l.setText((parent != NULL) ? parent.getName() : "");
			info_childs_l.setText(string(num_childs));
			
			// blend src function
			blend_src_func_cb.setCurrentItem(material.getBlendSrcFunc());
			
			// blend dest function
			blend_dest_func_cb.setCurrentItem(material.getBlendDestFunc());
			
			// downsize rendering
			downsize_cb.setCurrentItem(material.getDownsize());
			
			// rendering group
			group_cb.setCurrentItem(material.getGroup());
			
			// rendering order
			order_cb.setCurrentItem(get_order_item(material.getOrder()));
			
			// polygon offset
			offset_cb.setCurrentItem(get_offset_item(material.getOffset()));
			
			// light mask
			light_mask_el.setText(editorFormatMask(material.getLightMask()));
			
			// viewport mask
			viewport_mask_el.setText(editorFormatMask(material.getViewportMask()));
			
			// options
			post_deferred_cb.setChecked(material.getPostDeferred());
			post_refraction_cb.setChecked(material.getPostRefraction());
			post_scattering_cb.setChecked(material.getPostScattering());
			depth_mask_cb.setChecked(material.getDepthMask());
			glow_mask_cb.setChecked(material.getGlowMask());
			parallax_mask_cb.setChecked(material.getParallaxMask());
			occlusion_mask_cb.setChecked(material.getOcclusionMask());
			alpha_test_cb.setChecked(material.getAlphaTest());
			two_sided_cb.setChecked(material.getTwoSided());
			light_blending_cb.setChecked(material.getLightBlending());
			cast_shadow_cb.setChecked(material.getCastShadow());
			receive_shadow_cb.setChecked(material.getReceiveShadow());
			cast_world_shadow_cb.setChecked(material.getCastWorldShadow());
			receive_world_shadow_cb.setChecked(material.getReceiveWorldShadow());
			cast_translucent_cb.setChecked(material.getCastTranslucent());
			receive_translucent_cb.setChecked(material.getReceiveTranslucent());
			
			// update
			update_colors();
			update_states();
			update_textures();
			update_parameters();
			
			// buttons
			assign_i.setEnabled(1);
			clone_i.setEnabled(editable);
			inherit_i.setEnabled(1);
			move_i.setEnabled(parent != NULL && editable && library_editable);
			remove_i.setEnabled(parent != NULL && editable && library_editable);
			copy_b.setEnabled(1);
			paste_b.setEnabled(blob != NULL && editable);
		}
		
		// enable callbacks
		setCallbackEnabled(GUI_CHANGED,1,(
			blend_src_func_cb,
			blend_dest_func_cb,
			downsize_cb,
			group_cb,
			order_cb,
			offset_cb,
		));
		
		setCallbackEnabled(GUI_CLICKED,1,(
			post_deferred_cb,
			post_refraction_cb,
			post_scattering_cb,
			depth_mask_cb,
			glow_mask_cb,
			parallax_mask_cb,
			occlusion_mask_cb,
			alpha_test_cb,
			two_sided_cb,
			light_blending_cb,
			cast_shadow_cb,
			receive_shadow_cb,
			cast_world_shadow_cb,
			receive_world_shadow_cb,
			cast_translucent_cb,
			receive_translucent_cb,
		));
	}
	
	/*
	 */
	void blend_src_func_changed() {
		if(material.getBlendSrcFunc() != blend_src_func_cb.getCurrentItem()) save_state(create_state(material,Unredo::STATE_MODIFY));
		material.setBlendFunc(blend_src_func_cb.getCurrentItem(),material.getBlendDestFunc());
		update_colors();
	}
	
	void blend_dest_func_changed() {
		if(material.getBlendDestFunc() != blend_dest_func_cb.getCurrentItem()) save_state(create_state(material,Unredo::STATE_MODIFY));
		material.setBlendFunc(material.getBlendSrcFunc(),blend_dest_func_cb.getCurrentItem());
		update_colors();
	}
	
	/*
	 */
	void downsize_changed() {
		if(material.getDownsize() != downsize_cb.getCurrentItem()) save_state(create_state(material,Unredo::STATE_MODIFY));
		material.setDownsize(downsize_cb.getCurrentItem());
		update_colors();
	}
	
	void group_changed() {
		if(material.getGroup() != group_cb.getCurrentItem()) save_state(create_state(material,Unredo::STATE_MODIFY));
		material.setGroup(group_cb.getCurrentItem());
		update_colors();
	}
	
	void order_changed() {
		if(material.getOrder() != get_order(order_cb.getCurrentItem())) save_state(create_state(material,Unredo::STATE_MODIFY));
		material.setOrder(get_order(order_cb.getCurrentItem()));
		update_colors();
	}
	
	void offset_changed() {
		if(material.getOffset() != get_offset(offset_cb.getCurrentItem())) save_state(create_state(material,Unredo::STATE_MODIFY));
		material.setOffset(get_offset(offset_cb.getCurrentItem()));
		update_colors();
	}
	
	/*
	 */
	void light_mask_pressed() {
		MaterialState_ s = create_state(material,Unredo::STATE_MODIFY);
		int mask = int("0x" + light_mask_el.getText());
		if(material.getLightMask() != mask) save_state(s);
		else delete s;
		material.setLightMask(mask);
		light_mask_el.setText(editorFormatMask(material.getLightMask()));
		update_colors();
	}
	
	void light_mask_changed(int mask) {
		material.setLightMask(mask);
		light_mask_el.setText(editorFormatMask(material.getLightMask()));
		update_colors();
	}
	
	void light_mask_clicked() {
		MaterialState_ s = create_state(material,Unredo::STATE_MODIFY);
		int mask = material.getLightMask();
		dialogMask(TR("Material light mask"),functionid(light_mask_changed),material.getLightMask());
		if(mask !=  material.getLightMask()) save_state(s);
		else delete s;
	}
	
	/*
	 */
	void viewport_mask_pressed() {
		MaterialState_ s = create_state(material,Unredo::STATE_MODIFY);
		int mask = int("0x" + viewport_mask_el.getText());
		if(material.getViewportMask() != mask) save_state(s);
		else delete s;
		material.setViewportMask(mask);
		viewport_mask_el.setText(editorFormatMask(material.getViewportMask()));
		update_colors();
	}
	
	void viewport_mask_changed(int mask) {
		material.setViewportMask(mask);
		viewport_mask_el.setText(editorFormatMask(material.getViewportMask()));
		update_colors();
	}
	
	void viewport_mask_clicked() {
		MaterialState_ s = create_state(material,Unredo::STATE_MODIFY);
		int mask = material.getViewportMask();
		dialogMask(TR("Material viewport mask"),functionid(viewport_mask_changed),material.getViewportMask());
		if(mask !=  material.getViewportMask()) save_state(s);
		else delete s;
	}
	
	/*
	 */
	void option_clicked(string option) {
		save_state(create_state(material,Unredo::STATE_MODIFY));
		if(option == "post_deferred") material.setPostDeferred(post_deferred_cb.isChecked());
		else if(option == "post_refraction") material.setPostRefraction(post_refraction_cb.isChecked());
		else if(option == "post_scattering") material.setPostScattering(post_scattering_cb.isChecked());
		else if(option == "depth_mask") material.setDepthMask(depth_mask_cb.isChecked());
		else if(option == "glow_mask") material.setGlowMask(glow_mask_cb.isChecked());
		else if(option == "parallax_mask") material.setParallaxMask(parallax_mask_cb.isChecked());
		else if(option == "occlusion_mask") material.setOcclusionMask(occlusion_mask_cb.isChecked());
		else if(option == "alpha_test") material.setAlphaTest(alpha_test_cb.isChecked());
		else if(option == "two_sided") material.setTwoSided(two_sided_cb.isChecked());
		else if(option == "light_blending") material.setLightBlending(light_blending_cb.isChecked());
		else if(option == "cast_shadow") material.setCastShadow(cast_shadow_cb.isChecked());
		else if(option == "receive_shadow") material.setReceiveShadow(receive_shadow_cb.isChecked());
		else if(option == "cast_world_shadow") material.setCastWorldShadow(cast_world_shadow_cb.isChecked());
		else if(option == "receive_world_shadow") material.setReceiveWorldShadow(receive_world_shadow_cb.isChecked());
		else if(option == "cast_translucent") material.setCastTranslucent(cast_translucent_cb.isChecked());
		else if(option == "receive_translucent") material.setReceiveTranslucent(receive_translucent_cb.isChecked());
		else throw("Materials::option_clicked(): unknown option %s\n",option);
		update_textures();
		update_parameters();
		update_states();
		update_colors();
	}
	
	/*
	 */
	void copy_clicked() {
		delete blob;
		
		blob = new Blob(0);
		material.saveState(blob,1);
		blob.seekSet(0);
		
		update_view();
	}
	
	void paste_clicked() {
		save_state(create_state(material,Unredo::STATE_MODIFY));
		material.restoreState(blob,1);
		blob.seekSet(0);
		update_view();
	}
	
	/*
	 */
	void update_colors() {
		
		parent = material.getParent();
		
		WidgetLabel data_labels[] = (
			"getBlendSrcFunc" : blend_src_func_l,
			"getBlendDestFunc" : blend_dest_func_l,
			"getDownsize" : downsize_l,
			"getGroup" : group_l,
			"getOrder" : order_l,
			"getOffset" : offset_l,
			"getLightMask" : light_mask_l,
			"getViewportMask" : viewport_mask_l,
		);
		
		foreachkey(string function; data_labels) {
			WidgetLabel label = data_labels[function];
			label.setFontColor((parent != NULL && parent.call(function) != material.call(function)) ? color_green : vec4_zero);
		}
		
		WidgetCheckBox data_checkBoxes[] = (
			"getPostDeferred" : post_deferred_cb,
			"getPostRefraction" : post_refraction_cb,
			"getPostScattering" : post_scattering_cb,
			"getDepthMask" : depth_mask_cb,
			"getGlowMask" : glow_mask_cb,
			"getParallaxMask" : parallax_mask_cb,
			"getOcclusionMask" : occlusion_mask_cb,
			"getAlphaTest" : alpha_test_cb,
			"getTwoSided" : two_sided_cb,
			"getLightBlending" : light_blending_cb,
			"getCastShadow" : cast_shadow_cb,
			"getReceiveShadow" : receive_shadow_cb,
			"getCastWorldShadow" : cast_world_shadow_cb,
			"getReceiveWorldShadow" : receive_world_shadow_cb,
			"getCastTranslucent" : cast_translucent_cb,
			"getReceiveTranslucent" : receive_translucent_cb,
		);
		
		foreachkey(string function; data_checkBoxes) {
			WidgetCheckBox check_box = data_checkBoxes[function];
			check_box.setFontColor((parent != NULL && parent.call(function) != material.call(function)) ? color_green : vec4_zero);
		}
	}
	
	/**************************************************************************\
	*
	* Materials list
	*
	\**************************************************************************/
	
	/*
	 */
	void update_libraries() {
		
		libraries_cb.clear();
		libraries_cb.addItem(TR("All libraries"));
		forloop(int i = 0; engine.materials.getNumLibraries()) {
			libraries_cb.addItem(replace(basename(engine.materials.getLibraryName(i)),".mat",""));
		}
		
		libraries_i.setEnabled(engine.world.isLoaded());
		save_i.setEnabled(engine.world.isLoaded());
	}
	
	void update_materials() {
		
		Material old_material = material;
		material = NULL;
		
		string materials[0];
		if(libraries_cb.getCurrentItem() == 0) {
			forloop(int i = 0; engine.materials.getNumLibraries()) {
				forloop(int j = 0; engine.materials.getNumMaterials(i)) {
					string name = engine.materials.getMaterialName(i,j);
					Material material = engine.materials.findMaterial(name);
					if(material.isHidden() == 0) materials.append(name);
				}
			}
		} else {
			int library = libraries_cb.getCurrentItem() - 1;
			forloop(int i = 0; engine.materials.getNumMaterials(library)) {
				string name = engine.materials.getMaterialName(library,i);
				Material material = engine.materials.findMaterial(name);
				if(material.isHidden() == 0) materials.append(name);
			}
		}
		
		materials.sort();
		
		int hidden[];
		saveTreeBoxFolded(materials_tb,hidden);
		
		string items[];
		materials_tb.setCallbackEnabled(GUI_CHANGED,0);
		materials_tb.clear();
		foreach(string name; materials) {
			items.append(name,materials_tb.addItem(name));
		}
		materials_tb.setCallbackEnabled(GUI_CHANGED,1);
		
		materials_tb.setCurrentItem(-1);
		foreach(string name; materials) {
			Material material = engine.materials.findMaterial(name);
			if(material == old_material) materials_tb.setCurrentItem(items[name]);
			
			Material parent = material.getParent();
			if(parent == NULL) materials_tb.setItemColor(items[name],color_green);
			else if(material.getNumChilds()) materials_tb.setItemColor(items[name],color_orange);
			if(parent == NULL || materials.find(parent.getName()) == -1) continue;
			
			materials_tb.setItemParent(items[name],items[parent.getName()]);
		}
		if(materials_tb.getCurrentItem() == -1) update_view();
		
		restoreTreeBoxFolded(materials_tb,hidden);
	}
	
	/*
	 */
	void libraries_changed() {
		update_materials();
		name_el.runCallback(GUI_CHANGED);
	}
	
	void name_changed() {
		name_el.setCallbackEnabled(GUI_CHANGED,0);
		materials_tb.setCallbackEnabled(GUI_CHANGED,0);
		restoreTreeBoxFolded(materials_tb,materials_folded);
		if(selectTreeBoxByName(materials_tb,name_el,0)) update_view();
		name_el.setCallbackEnabled(GUI_CHANGED,1);
		materials_tb.setCallbackEnabled(GUI_CHANGED,1);
	}
	
	void name_pressed() {
		name_el.setCallbackEnabled(GUI_CHANGED,0);
		materials_tb.setCallbackEnabled(GUI_CHANGED,0);
		restoreTreeBoxFolded(materials_tb,materials_folded);
		if(selectTreeBoxByName(materials_tb,name_el,1)) update_view();
		name_el.setCallbackEnabled(GUI_CHANGED,1);
		materials_tb.setCallbackEnabled(GUI_CHANGED,1);
	}
	
	void name_focus_in() {
		saveTreeBoxFolded(materials_tb,materials_folded);
	}
	
	void name_focus_out() {
		materials_folded.clear();
	}
	
	/*
	 */
	void materials_changed() {
		update_view();
	}
	
	void materials_double_clicked() {
		int item = materials_tb.getCurrentItem();
		materials_tb.setItemFolded(item,!materials_tb.isItemFolded(item));
	}
	
	/*
	 */
	void select_material(string name) {
		int items[0];
		materials_tb.getItems(items);
		foreach(int item; items) {
			if(materials_tb.getItemText(item) == name) {
				int parent = materials_tb.getItemParent(item);
				while(parent != -1) {
					materials_tb.setItemFolded(parent,0);
					parent = materials_tb.getItemParent(parent);
				}
				materials_tb.setCurrentItem(item);
				materials_tb.showItem(item);
				return;
			}
		}
		materials_tb.setCurrentItem(-1);
	}
	
	/**************************************************************************\
	*
	* Buttons
	*
	\**************************************************************************/
	
	/*
	 */
	void libraries_clicked() {
		dialogMaterialLibraries(TR("Preloaded material libraries"));
		update_libraries();
		update_materials();
	}
	
	/*
	 */
	void save_clicked() {
		if(libraries_cb.getCurrentItem() == 0) {
			forloop(int i = 0; engine.materials.getNumLibraries()) {
				if(engine.materials.isLibraryEditable(i)) {
					engine.materials.save(engine.materials.getLibraryName(i));
				}
			}
		} else {
			int library = libraries_cb.getCurrentItem() - 1;
			if(engine.materials.isLibraryEditable(library)) {
				engine.materials.save(engine.materials.getLibraryName(library));
			}
		}
	}
	
	/*
	 */
	void assign_clicked() {
		Node nodes[0];
		int surfaces[0];
		nodesGetNodes(nodes);
		Node node = nodesGetNode(surfaces);
		Nodes::NodesState_ s = new Nodes::NodesState_();
		s.init_modify(nodes,node);
		foreach(Node n; nodes) {
			if(n == node) continue;
			if(n.isObject()) {
				Object object = node_cast(n);
				forloop(int i = 0; object.getNumSurfaces()) {
					object.setMaterial(material.getName(),i);
				}
			} else if(n.isDecal()) {
				Decal decal = node_cast(n);
				decal.setMaterial(material.getName());
				nodesUpdateDecal(decal);
			}
		}
		if(node == NULL) return;
		if(node.isObject()) {
			Object object = node_cast(node);
			foreach(int surface; surfaces) {
				object.setMaterial(material.getName(),surface);
			}
			nodesUpdateObjectMaterialName();
		} else if(node.isDecal()) {
			Decal decal = node_cast(node);
			decal.setMaterial(material.getName());
			nodesUpdateDecalMaterialName();
			nodesUpdateDecal(decal);
		}
		Nodes::save_state(s);
	}
	
	/*
	 */
	void clone_clicked() {
		string library = engine.materials.getLibraryName(engine.materials.findMaterialLibrary(material.getName()));
		string name = getNewMaterialName(material.getName());
		if(dialogMaterialName(TR("Clone material to"),library,name)) {
			if(engine.materials.cloneMaterial(material.getName(),library,name) == 0) {
				dialogMessageOk(TR("Error"));
				return;
			}
			MaterialState_ s = create_state(engine.materials.findMaterial(name),Unredo::STATE_NEW);
			s.selected = material.getName();
			update_materials();
			select_material(name);
			save_state(s);
		}
	}
	
	void inherit_clicked() {
		string library = engine.materials.getLibraryName(engine.materials.findMaterialLibrary(material.getName()));
		string name = getNewMaterialName(material.getName());
		if(dialogMaterialName(TR("Inherit material to"),library,name)) {
			if(engine.materials.inheritMaterial(material.getName(),library,name) == 0) {
				dialogMessageOk(TR("Error"));
				return;
			}
			MaterialState_ s = create_state(engine.materials.findMaterial(name),Unredo::STATE_NEW);
			s.selected = material.getName();
			update_materials();
			select_material(name);
			save_state(s);
		}
	}
	
	void move_clicked() {
		string library = engine.materials.getLibraryName(engine.materials.findMaterialLibrary(material.getName()));
		string name = material.getName();
		if(dialogMaterialName(TR("Move material to"),library,name)) {
			MaterialState_ s = create_state(material,Unredo::STATE_MOVE);
			if(engine.materials.moveMaterial(material.getName(),library,name) == 0) {
				dialogMessageOk(TR("Error"));
				return;
			}
			update_materials();
			select_material(name);
			save_state(s);
		}
	}
	
	void remove_clicked() {
		string message = format(TR("Remove \"%s\" material"),material.getName());
		if(material.getNumChilds()) message += format(TR(" with children"));
		if(dialogMessageYesNo(TR("Confirm"),message + "?")) {
			MaterialState_ s = create_state(material,Unredo::STATE_REMOVE);
			if(engine.materials.removeMaterial(material.getName()) == 0) {
				dialogMessageOk(TR("Error"));
				return;
			}
			material = NULL;
			material_smv.setMaterial(NULL);
			update_materials();
			save_state(s);
		}
	}
	
	/**************************************************************************\
	*
	* Materials
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		// material view
		material_smv = new WidgetSpriteMaterialView(gui);
		material_vb.addChild(material_smv.getWidget());
		
		// update materials
		update_libraries();
		update_materials();
		
		// default tab
		material_tb.setCurrentTab(0);
		
		// configure window
		editorWindow(window);
		DECLARE_WINDOW_LOAD(materials)
		
		int value = configGet("materials_value",window_hp.getValue());
		window_hp.setValue(value);
		
		blob = NULL;
	}
	
	void shutdown() {
		
		//undo_states.clear();
		//redo_states.clear();
		
		if(blob != NULL) {
			delete blob;
			blob = NULL;
		}
		
		DECLARE_WINDOW_SAVE(materials)
		
		configSet("materials_value",window_hp.getValue());
	}
	
	/*
	 */
	void update(int need_reload) {
		
		// material view
		material_smv.update();
		
		// reload materials
		if(need_reload) {
			update_libraries();
			update_materials();
		}
	}
	
} /* namespace Materials */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void materialsInit() {
	Materials::init();
	Materials::Editor::init();
}

void materialsShutdown() {
	Materials::shutdown();
	Materials::Editor::shutdown();
}

/*
 */
void materialsToggle() {
	if(Materials::window_show) materialsHide();
	else materialsShow();
}

void materialsShow() {
	DECLARE_WINDOW_SHOW(Materials)
}

void materialsHide() {
	DECLARE_WINDOW_HIDE(Materials)
}

/*
 */
void materialsUpdate(int need_reload) {
	Materials::update(need_reload);
}

void materialsUpdateView() {
	Materials::update_view();
}

/*
 */
void materialsSelect(string name) {
	Materials::select_material(name);
}

/*
 */
int materialsEditor(Material material) {
	return Materials::Editor::run(material);
}
