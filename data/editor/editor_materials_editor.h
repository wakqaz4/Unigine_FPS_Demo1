/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_materials_editor.h
 * Desc:    Unigine editor
 * Version: 1.08
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
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
namespace Editor {
	
	/*
	 */
	WidgetDialog dialog;					// material dialog
	
	DECLARE_DIALOG
	
	Material material;						// current material
	Material parent;						// parent material
	
	WidgetTabBox material_tb;				// material container
	
	WidgetLabel info_library_l;				// library name
	WidgetLabel info_name_l;				// material name
	
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
	
	WidgetButton paste_b;					// paste settings
	
	WidgetGridBox states_gb;				// material states container
	WidgetGridBox textures_gb;				// material textures container
	WidgetGridBox parameters_gb;			// material parameters container
	
	/**************************************************************************\
	*
	* Textures
	*
	\**************************************************************************/
	
	/*
	 */
	MTexture textures[0];
	
	/*
	 */
	class MTexture {
		
		int num;						// texture number
		string name;					// texture name
		int type;						// texture type
		int hidden;						// texture hidden
		
		WidgetLabel name_l;				// name label
		WidgetEditLine name_el;			// name editline
		
		WidgetHBox icon_hb;				// icons hbox
		WidgetIcon load_i;				// image load
		WidgetIcon view_i;				// image view
		WidgetIcon clear_i;				// image clear
		
		// update
		void update() {
			int override = (parent.findTexture(name) != -1 && material.getImageTextureName(num) != parent.getImageTextureName(parent.findTexture(name)));
			name_l.setFontColor(override ? color_green : vec4_zero);
			clear_i.setEnabled(override);
		}
		
		// constructor
		MTexture(int n) {
			
			num = n;
			name = material.getTextureName(num);
			type = material.getTextureType(num);
			hidden = material.isTextureHidden(num);
			
			if(name == "height") hidden = 0;
			if(name == "tessellation") hidden = 0;
			
			if(hidden) return;
			
			// image
			if(type == MATERIAL_TEXTURE_IMAGE) {
				
				name_l = new WidgetLabel(gui,name + ":");
				textures_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				
				name_el = new WidgetEditLine(gui,material.getImageTextureName(num));
				textures_gb.addChild(name_el,GUI_ALIGN_EXPAND);
				
				icon_hb = new WidgetHBox(gui);
				textures_gb.addChild(icon_hb);
				
				load_i = new WidgetIcon(gui,"editor/gui/action_load.png");
				load_i.setWidth(24);
				load_i.setHeight(24);
				load_i.setToolTip(TR("Load"));
				icon_hb.addChild(load_i);
				
				view_i = new WidgetIcon(gui,"editor/gui/action_view.png");
				view_i.setWidth(24);
				view_i.setHeight(24);
				view_i.setToolTip(TR("View"));
				icon_hb.addChild(view_i);
				
				clear_i = new WidgetIcon(gui,"editor/gui/action_clear.png");
				clear_i.setWidth(24);
				clear_i.setHeight(24);
				clear_i.setToolTip(TR("Reset"));
				icon_hb.addChild(clear_i);
				
				name_el.setCallback(GUI_PRESSED,functionid(texture_name_pressed),num);
				name_el.setCallback(GUI_FOCUS_OUT,functionid(texture_name_pressed),num);
				load_i.setCallback(GUI_CLICKED,functionid(texture_load_clicked),num);
				view_i.setCallback(GUI_CLICKED,functionid(texture_view_clicked),num);
				clear_i.setCallback(GUI_CLICKED,functionid(texture_clear_clicked),num);
				
				update();
			}
		}
		
		~MTexture() {
			
			delete name_l;
			delete name_el;
			delete icon_hb;
			delete load_i;
			delete view_i;
			delete clear_i;
		}
	};
	
	/*
	 */
	void texture_name_pressed(int num) {
		MTexture texture = textures[num];
		WidgetEditLine name_el = texture.name_el;
		if(material.getImageTextureName(num) != name_el.getText()) {
			material.setImageTextureName(num,name_el.getText());
			texture.name_el.setText(material.getImageTextureName(num));
		}
		texture.update();
	}
	
	void texture_load_clicked(int num) {
		MTexture texture = textures[num];
		string name = material.getImageTextureName(num);
		if(texturesDialogImage(format(TR("Select %s texture"),texture.name),material.getTextureName(num),name)) {
			material.setImageTextureName(num,name);
			texture.name_el.setText(material.getImageTextureName(num));
			texturesAdd(material.getImageTextureName(num),material.getTextureName(num));
		}
		texture.update();
	}
	
	void texture_view_clicked(int num) {
		MTexture texture = textures[num];
		dialogImage(format(TR("%s texture"),texture.name),material.getImageTextureName(num));
	}
	
	void texture_clear_clicked(int num) {
		MTexture texture = textures[num];
		if(dialogMessageYesNo(TR("Confirm"),format(TR("Reset the %s texture?"),texture.name))) {
			material.setImageTextureName(num,NULL);
			texture.name_el.setText(material.getImageTextureName(num));
		}
		texture.update();
	}
	
	/*
	 */
	void update_textures() {
		
		// delete textures
		textures.delete();
		
		// create textures
		forloop(int i = 0; material.getNumTextures()) {
			textures.append(new MTexture(i));
		}
	}
	
	/**************************************************************************\
	*
	* Parameters
	*
	\**************************************************************************/
	
	/*
	 */
	MParameter parameters[0];
	
	/*
	 */
	class MParameter {
		
		int num;						// parameter number
		string name;					// parameter name
		int type;						// parameter type
		int hidden;						// parameter hidden
		
		WidgetLabel name_l;				// name label
		WidgetHBox main_hb;				// main hbox
		
		WidgetIcon clear_i;				// parameter clear
		
		WidgetLabel constant_l;			// constant value
		
		WidgetGroupBox color_gb;		// color groupbox
		WidgetHBox color_hb;
		WidgetSprite color_s[2];		// color value
		
		WidgetEditLine slider_el;		// slider editline
		WidgetVBox slider_vb;			// slider vbox
		WidgetSlider slider_sl;			// slider
		int multiplier;					// multiplier
		
		WidgetEditLine expression_el;	// expression editline
		
		// update
		void update() {
			int override = (parent.findParameter(name) != -1);
			if(override) {
				override &= (parent.getParameterType(num) != MATERIAL_PARAMETER_CONSTANT || material.getParameter(num) != parent.getParameter(num));
				override &= (parent.getParameterType(num) != MATERIAL_PARAMETER_COLOR || material.getParameter(num) != parent.getParameter(num));
				override &= (parent.getParameterType(num) != MATERIAL_PARAMETER_SLIDER || material.getParameter(num) != parent.getParameter(num));
				override &= (parent.getParameterType(num) != MATERIAL_PARAMETER_EXPRESSION || material.getParameterExpression(num) != parent.getParameterExpression(num));
			}
			name_l.setFontColor(override ? color_green : vec4_zero);
			clear_i.setEnabled(override);
		}
		
		// constructor
		MParameter(int n) {
			
			num = n;
			name = material.getParameterName(num);
			type = material.getParameterType(num);
			hidden = material.isParameterHidden(num);
			
			if(hidden) return;
			
			name_l = new WidgetLabel(gui,name + ":");
			main_hb = new WidgetHBox(gui);
			
			clear_i = new WidgetIcon(gui,"editor/gui/action_clear.png");
			clear_i.setWidth(24);
			clear_i.setHeight(24);
			clear_i.setToolTip(TR("Reset"));
			
			clear_i.setCallback(GUI_CLICKED,functionid(parameter_clear_clicked),num);
			
			// constant parameter
			if(type == MATERIAL_PARAMETER_CONSTANT) {
				
				constant_l = new WidgetLabel(gui,string(material.getParameter(num)));
				main_hb.addChild(constant_l,GUI_ALIGN_LEFT);
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			// color parameter
			else if(type == MATERIAL_PARAMETER_COLOR) {
				
				color_gb = new WidgetGroupBox(gui);
				main_hb.addChild(color_gb,GUI_ALIGN_EXPAND);
				
				color_hb = new WidgetHBox(gui);
				color_gb.addChild(color_hb,GUI_ALIGN_EXPAND);
				
				color_s[0] = new WidgetSprite(gui,"editor/gui/color_white.png");
				color_s[0].setTransform(scale(8,8,1));
				color_hb.addChild(color_s[0],GUI_ALIGN_EXPAND);
				
				color_s[1] = new WidgetSprite(gui,"editor/gui/color_white.png");
				color_s[1].setTransform(scale(8,8,1));
				color_hb.addChild(color_s[1],GUI_ALIGN_EXPAND);
				
				color_s[0].setCallback(GUI_CLICKED,functionid(parameter_color_clicked),num);
				color_s[1].setCallback(GUI_CLICKED,functionid(parameter_color_clicked),num);
				
				setSpriteColor(color_s,material.getParameter(num));
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			// slider parameter
			else if(type == MATERIAL_PARAMETER_SLIDER) {
				
				float value = material.getParameterSlider(num);
				
				slider_el = new WidgetEditLine(gui);
				main_hb.addChild(slider_el);
				
				slider_vb = new WidgetVBox(gui,2,2);
				main_hb.addChild(slider_vb);
				
				int flags = 0;
				if(material.getParameterSliderLog10(num)) flags |= GUI_ATTACH_LOG10;
				if(material.getParameterSliderMaxExpand(num)) flags |= GUI_ATTACH_MAX_EXPAND;
				if(material.getParameterSliderMinExpand(num)) flags |= GUI_ATTACH_MIN_EXPAND;
				multiplier = (abs(material.getParameterSliderMaxValue(num) - material.getParameterSliderMinValue(num)) > 1000.0f) ? 1 : 1000;
				slider_sl = new WidgetSlider(gui,material.getParameterSliderMinValue(num) * multiplier,material.getParameterSliderMaxValue(num) * multiplier,value * multiplier);
				slider_sl.addAttach(slider_el,"%g",multiplier,flags);
				main_hb.addChild(slider_sl,GUI_ALIGN_EXPAND);
				
				slider_el.setText(editorFormat(value));
				
				slider_el.setCallback(GUI_PRESSED,functionid(parameter_slider_pressed),num);
				slider_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_slider_pressed),num);
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			// expression parameter
			else if(type == MATERIAL_PARAMETER_EXPRESSION) {
				
				expression_el = new WidgetEditLine(gui,material.getParameterExpression(num));
				main_hb.addChild(expression_el,GUI_ALIGN_EXPAND);
				
				expression_el.setCallback(GUI_PRESSED,functionid(parameter_expression_pressed),num);
				expression_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_expression_pressed),num);
				
				parameters_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				parameters_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				parameters_gb.addChild(clear_i);
			}
			// array parameter
			else if(type == MATERIAL_PARAMETER_ARRAY) {
				
			}
			else {
				throw("Materials::Editor::MParameter::MParameter(): unknown parameter type\n");
			}
			
			update();
		}
		
		~MParameter() {
			
			delete name_l;
			delete main_hb;
			
			delete clear_i;
			
			delete constant_l;
			
			delete color_gb;
			delete color_hb;
			delete color_s[0];
			delete color_s[1];
			
			delete slider_el;
			delete slider_vb;
			delete slider_sl;
			
			delete expression_el;
		}
	};
	
	/*
	 */
	int parameter_num;
	
	/*
	 */
	void parameter_color_changed(WidgetDialogColor dialog) {
		MParameter parameter = parameters[parameter_num];
		material.setParameter(parameter_num,dialog.getColor());
		setSpriteColor(parameter.color_s,material.getParameter(parameter_num));
	}
	
	void parameter_color_clicked(int num) {
		parameter_num = num;
		MParameter parameter = parameters[num];
		dialogColor(format(TR("Select %s color"),parameters[num].name),functionid(parameter_color_changed),material.getParameter(num));
		parameter.update();
	}
	
	/*
	 */
	void parameter_slider_pressed(int num) {
		MParameter parameter = parameters[num];
		material.setParameterSlider(num,float(parameter.slider_el.getText()));
		parameter.slider_el.setText(editorFormat(material.getParameterSlider(num)));
		if(toolsGetSelection() && parameter.slider_el.isFocused()) {
			parameter.slider_el.setCursor(0);
			parameter.slider_el.setSelection(1024);
		}
		parameter.update();
	}
	
	/*
	 */
	void parameter_expression_pressed(int num) {
		MParameter parameter = parameters[num];
		if(material.setParameterExpression(num,parameter.expression_el.getText()) == 0 && (gui.getMouseButton() == 0 || parameter.expression_el.isFocused() == 0)) {
			dialogMessageOk(TR("Error"));
		}
		parameter.update();
	}
	
	/*
	 */
	void parameter_clear_clicked(int num) {
		MParameter parameter = parameters[num];
		int type = parameter.type;
		if(dialogMessageYesNo(TR("Confirm"),format(TR("Reset the %s parameter?"),parameter.name))) {
			if(type == MATERIAL_PARAMETER_COLOR) {
				vec4 color = parent.getParameter(parent.findParameter(parameter.name));
				material.setParameter(num,color);
				setSpriteColor(parameter.color_s,color);
			}
			else if(type == MATERIAL_PARAMETER_SLIDER) {
				WidgetEditLine slider_el = parameter.slider_el;
				float value = parent.getParameterSlider(parent.findParameter(parameter.name));
				material.setParameterSlider(num,value);
				slider_el.setText(editorFormat(value));
			}
			else if(type == MATERIAL_PARAMETER_EXPRESSION) {
				WidgetEditLine expression_el = parameter.expression_el;
				string expression = parent.getParameterExpression(parent.findParameter(parameter.name));
				material.setParameterExpression(num,expression);
				expression_el.setText(expression);
			}
		}
		parameter.update();
	}
	
	/*
	 */
	void update_parameters() {
		
		// delete parameters
		parameters.delete();
		
		// create parameters
		forloop(int i = 0; material.getNumParameters()) {
			parameters.append(new MParameter(i));
		}
	}
	
	/**************************************************************************\
	*
	* States
	*
	\**************************************************************************/
	
	/*
	 */
	MState states[0];
	
	/*
	 */
	class MState {
		
		int num;					// state number
		string name;				// state name
		int type;					// state type
		int hidden;					// state hidden
		
		WidgetLabel name_l;			// name label
		WidgetVBox main_hb;			// main hbox
		
		WidgetIcon clear_i;			// state clear
		
		WidgetCheckBox toggle_cb;	// toggle checkbox
		WidgetComboBox switch_cb;	// switch combobox
		
		
		// update
		void update() {
			int override = (parent.findState(name) != -1 && material.getState(num) != parent.getState(parent.findState(name)));
			vec4 color = override ? color_green : vec4_zero;
			name_l.setFontColor(color);
			clear_i.setEnabled(override);
			if(toggle_cb != NULL) toggle_cb.setFontColor(color);
		}
		
		// constructor
		MState(int n) {
			
			num = n;
			name = material.getStateName(num);
			type = material.getStateType(num);
			hidden = material.isStateHidden(num);
			
			if(hidden) return;
			
			main_hb = new WidgetHBox(gui);
			
			clear_i = new WidgetIcon(gui,"editor/gui/action_clear.png");
			clear_i.setWidth(24);
			clear_i.setHeight(24);
			clear_i.setToolTip(TR("Reset"));
			
			clear_i.setCallback(GUI_CLICKED,functionid(state_clear_clicked),num);
			
			// toggle state
			if(type == MATERIAL_STATE_TOGGLE) {
				
				name_l = new WidgetLabel(gui);
				
				toggle_cb = new WidgetCheckBox(gui,name);
				toggle_cb.setChecked(material.getState(num));
				main_hb.addChild(toggle_cb,GUI_ALIGN_LEFT);
				
				toggle_cb.setCallback(GUI_CLICKED,functionid(state_toggle_clicked),num);
				
				states_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				states_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				states_gb.addChild(clear_i);
			}
			// switch state
			else if(type == MATERIAL_STATE_SWITCH) {
				
				name_l = new WidgetLabel(gui,name + ":");
				
				switch_cb = new WidgetComboBox(gui);
				forloop(int i = 0; material.getStateSwitchNumItems(num)) {
					switch_cb.addItem(material.getStateSwitchItem(num,i));
				}
				switch_cb.setCurrentItem(material.getState(num));
				main_hb.addChild(switch_cb,GUI_ALIGN_EXPAND);
				
				switch_cb.setCallback(GUI_CHANGED,functionid(state_switch_changed),num);
				
				states_gb.addChild(name_l,GUI_ALIGN_RIGHT);
				states_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
				states_gb.addChild(clear_i);
			}
			else {
				throw("Materials::Editor::MState::MState(): unknown state type\n");
			}
			
			update();
		}
		
		~MState() {
			
			delete name_l;
			delete main_hb;
			
			delete clear_i;
			
			delete toggle_cb;
			delete switch_cb;
		}
	};
	
	/*
	 */
	void state_toggle_clicked(int num) {
		MState state = states[num];
		state.toggle_cb.setCallbackEnabled(GUI_CLICKED,0);
		material.setState(num,state.toggle_cb.isChecked());
		state.toggle_cb.setChecked(material.getState(num));
		state.toggle_cb.setCallbackEnabled(GUI_CLICKED,1);
		state.update();
		update_textures();
		update_parameters();
	}
	
	void state_switch_changed(int num) {
		MState state = states[num];
		state.switch_cb.setCallbackEnabled(GUI_CHANGED,0);
		material.setState(num,state.switch_cb.getCurrentItem());
		state.switch_cb.setCurrentItem(material.getState(num));
		state.switch_cb.setCallbackEnabled(GUI_CHANGED,1);
		state.update();
		update_textures();
		update_parameters();
	}
	
	/*
	 */
	void state_clear_clicked(int num) {
		MState state = states[num];
		int type = state.type;
		if(dialogMessageYesNo(TR("Confirm"),format(TR("Reset the %s state?"),state.name))) {
			if(type == MATERIAL_STATE_TOGGLE) {
				WidgetCheckBox toggle_cb = state.toggle_cb;
				int value = parent.getState(parent.findState(state.name));
				material.setState(num,value);
				toggle_cb.setCallbackEnabled(GUI_CLICKED,0);
				toggle_cb.setChecked(value);
				toggle_cb.setCallbackEnabled(GUI_CLICKED,1);
			}
			else if(type == MATERIAL_STATE_SWITCH) {
				WidgetComboBox switch_cb = state.switch_cb;
				int value = parent.getState(parent.findState(state.name));
				material.setState(num,value);
				switch_cb.setCallbackEnabled(GUI_CHANGED,0);
				switch_cb.setCurrentItem(value);
				switch_cb.setCallbackEnabled(GUI_CHANGED,1);
			}
		}
		state.update();
		update_parameters();
	}
	
	/*
	 */
	void update_states() {
		
		// delete states
		states.delete();
		
		// create states
		forloop(int i = 0; material.getNumStates()) {
			states.append(new MState(i));
		}
	}
	
	/**************************************************************************\
	*
	* Material
	*
	\**************************************************************************/
	
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
		
		// parent material
		parent = material.getParent();
		
		// info
		int library = engine.materials.findMaterialLibrary(material.getName());
		info_library_l.setText(replace(basename(engine.materials.getLibraryName(library)),".mat",""));
		info_name_l.setText(material.getName());
		
		// blending
		blend_src_func_cb.setCurrentItem(material.getBlendSrcFunc());
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
		paste_b.setEnabled(blob != NULL);
		
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
		material.setBlendFunc(blend_src_func_cb.getCurrentItem(),material.getBlendDestFunc());
		update_colors();
	}
	
	void blend_dest_func_changed() {
		material.setBlendFunc(material.getBlendSrcFunc(),blend_dest_func_cb.getCurrentItem());
		update_colors();
	}
	
	/*
	 */
	void downsize_changed() {
		material.setDownsize(downsize_cb.getCurrentItem());
		update_colors();
	}
	
	void group_changed() {
		material.setGroup(group_cb.getCurrentItem());
		update_colors();
	}
	
	void order_changed() {
		material.setOrder(get_order(order_cb.getCurrentItem()));
		update_colors();
	}
	
	void offset_changed() {
		material.setOffset(get_offset(offset_cb.getCurrentItem()));
		update_colors();
	}
	
	/*
	 */
	void light_mask_pressed() {
		int mask = int("0x" + light_mask_el.getText());
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
		dialogMask(TR("Material light mask"),functionid(light_mask_changed),material.getLightMask());
	}
	
	/*
	 */
	void viewport_mask_pressed() {
		int mask = int("0x" + viewport_mask_el.getText());
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
		dialogMask(TR("Material viewport mask"),functionid(viewport_mask_changed),material.getViewportMask());
	}
	
	/*
	 */
	void option_clicked(string option) {
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
		else throw("Materials::Editor::option_clicked(): unknown option %s\n",option);
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
	* Dialog
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		material_tb.setCurrentTab(0);
		
		editorDialog(dialog);
		
		DECLARE_DIALOG_INIT(Materials::Editor)
	}
	
	void shutdown() {
		
	}
	
	/*
	 */
	void run(Material m) {
		
		material = m;
		update_view();
		
		Blob blob = new Blob(0);
		material.saveState(blob,1);
		blob.seekSet(0);
		
		DECLARE_DIALOG_SHOW
		while(dialog.isDone() == 0) wait 1;
		DECLARE_DIALOG_HIDE
		
		if(dialog.isCancelClicked()) material.restoreState(blob);
		delete blob;
		
		return dialog.isOkClicked();
	}
	
} /* namespace Editor */
