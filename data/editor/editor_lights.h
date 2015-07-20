/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_lights.h
 * Desc:    Unigine editor
 * Version: 1.45
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
 *          Vasiliy Simonov <vvs@unigine.com>
 *          Pavel Kurinnoy <figonet@unigine.com>
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
namespace Light {
	
	/*
	 */
	WidgetCheckBox deferred_cb;				// deferred toggle
	WidgetEditLine light_mask_el;			// light mask
	WidgetEditLine viewport_mask_el;		// viewport mask
	WidgetSprite color_s[];					// color
	WidgetEditLine multiplier_el;			// hdr multiplier
	WidgetSlider multiplier_sl;
	WidgetEditLine attenuation_el;			// attenuation
	WidgetSlider attenuation_sl;
	WidgetEditLine diffuse_scale_el;		// diffuse scale
	WidgetSlider diffuse_scale_sl;
	WidgetEditLine normal_scale_el;			// normal scale
	WidgetSlider normal_scale_sl;
	WidgetEditLine specular_scale_el;		// specular scale
	WidgetSlider specular_scale_sl;
	WidgetEditLine specular_power_el;		// specular power
	WidgetSlider specular_power_sl;
	WidgetCheckBox shadow_cb;				// shadow toggle
	WidgetCheckBox translucent_cb;			// translucent toggle
	WidgetEditLine shadow_offset_el[];		// shadow offset
	WidgetComboBox shadow_size_cb;			// shadow size
	WidgetComboBox translucent_size_cb;		// translucent size
	WidgetEditLine shadow_ambient_el;		// shadow ambient
	WidgetSlider shadow_ambient_sl;
	WidgetEditLine shadow_bias_el;			// shadow bias
	WidgetSlider shadow_bias_sl;
	WidgetEditLine shadow_slope_el;			// shadow slope
	WidgetSlider shadow_slope_sl;
	WidgetEditLine shadow_softness_el;		// shadow softness
	WidgetSlider shadow_softness_sl;
	WidgetEditLine translucent_bias_el;		// translucent bias
	WidgetSlider translucent_bias_sl;
	WidgetEditLine deferred_distance_el;	// deferred distance
	WidgetEditLine visible_distance_el;		// visible distance
	WidgetEditLine shadow_distance_el;		// shadow distance
	WidgetEditLine fade_distance_el;		// fade distance
	
	Light light;							// base light node
	
	/*
	 */
	void update() {
		
		light = class_cast("Light",node);
		
		// light
		if(deferred_cb != NULL) deferred_cb.setCallbackEnabled(GUI_CLICKED,0);
		if(deferred_cb != NULL) deferred_cb.setChecked(light.isDeferred());
		if(light_mask_el != NULL) light_mask_el.setText(editorFormatMask(light.getLightMask()));
		if(viewport_mask_el != NULL) viewport_mask_el.setText(editorFormatMask(light.getViewportMask()));
		if(color_s[0] != NULL) setSpriteColor(color_s,light.getColor());
		if(multiplier_el != NULL) multiplier_el.setText(editorFormat(light.getMultiplier()));
		if(attenuation_el != NULL) attenuation_el.setText(editorFormat(light.getAttenuation()));
		if(diffuse_scale_el != NULL) diffuse_scale_el.setText(editorFormat(light.getDiffuseScale()));
		if(normal_scale_el != NULL) normal_scale_el.setText(editorFormat(light.getNormalScale()));
		if(specular_scale_el != NULL) specular_scale_el.setText(editorFormat(light.getSpecularScale()));
		if(specular_power_el != NULL) specular_power_el.setText(editorFormat(light.getSpecularPower()));
		if(deferred_cb != NULL) deferred_cb.setCallbackEnabled(GUI_CLICKED,1);
		
		// shadow
		if(shadow_cb != NULL) shadow_cb.setCallbackEnabled(GUI_CLICKED,0);
		if(translucent_cb != NULL) translucent_cb.setCallbackEnabled(GUI_CLICKED,0);
		if(shadow_cb != NULL) shadow_cb.setChecked(light.getShadow());
		if(translucent_cb != NULL) translucent_cb.setChecked(light.getTranslucent());
		if(shadow_offset_el[0] != NULL) setEditLineVec3(shadow_offset_el,light.getShadowOffset());
		if(shadow_size_cb != NULL) shadow_size_cb.setCurrentItem(light.getShadowSize() + 1);
		if(translucent_size_cb != NULL) translucent_size_cb.setCurrentItem(light.getTranslucentSize() + 1);
		if(shadow_ambient_el != NULL) shadow_ambient_el.setText(editorFormat(light.getShadowAmbient()));
		if(shadow_bias_el != NULL) shadow_bias_el.setText(editorFormat(light.getShadowBias()));
		if(shadow_slope_el != NULL) shadow_slope_el.setText(editorFormat(light.getShadowSlope()));
		if(shadow_softness_el != NULL) shadow_softness_el.setText(editorFormat(light.getShadowSoftness()));
		if(translucent_bias_el != NULL) translucent_bias_el.setText(editorFormat(light.getTranslucentBias()));
		if(shadow_cb != NULL) shadow_cb.setCallbackEnabled(GUI_CLICKED,1);
		if(translucent_cb != NULL) translucent_cb.setCallbackEnabled(GUI_CLICKED,1);
		
		// distances
		if(deferred_distance_el != NULL) deferred_distance_el.setText(editorFormat(light.getDeferredDistance()));
		if(visible_distance_el != NULL) visible_distance_el.setText(editorFormat(light.getVisibleDistance()));
		if(shadow_distance_el != NULL) shadow_distance_el.setText(editorFormat(light.getShadowDistance()));
		if(fade_distance_el != NULL) fade_distance_el.setText(editorFormat(light.getFadeDistance()));
	}
	
	/*
	 */
	void update_dynamic() {
		
		// shadow parameters
		int light_shadow = light.getShadow();
		int light_translucent = light.getTranslucent();
		int render_translucent = engine.render.getTranslucent();
		int shadow_soft = (engine.render.getShaders() == RENDER_QUALITY_HIGH);
		
		if(translucent_cb != NULL) translucent_cb.setEnabled(light_shadow && render_translucent);
		if(shadow_offset_el[0] != NULL) setEnabled(light_shadow,shadow_offset_el);
		if(shadow_size_cb != NULL) shadow_size_cb.setEnabled(light_shadow);
		if(translucent_size_cb != NULL) translucent_size_cb.setEnabled(light_shadow && render_translucent);
		if(shadow_ambient_el != NULL) shadow_ambient_el.setEnabled(light_shadow);
		if(shadow_bias_el != NULL) shadow_bias_el.setEnabled(light_shadow);
		if(shadow_slope_el != NULL) shadow_slope_el.setEnabled(light_shadow);
		if(shadow_softness_el != NULL) shadow_softness_el.setEnabled(light_shadow && shadow_soft);
		if(translucent_bias_el != NULL) translucent_bias_el.setEnabled(light_shadow && light_translucent && render_translucent);
		if(shadow_distance_el != NULL) shadow_distance_el.setEnabled(light_shadow);
		
		// show object info
		if(helpersGetNodeInfo()) {
			string info = run_function("get_info");
			engine.visualizer.renderMessage3D(light.getWorldTransform() * vec3_zero,vec3_zero,info,vec4_one,1);
		}
	}
	
	/*
	 */
	CLICKED_CALLBACK(deferred,light,Deferred,light)
	PRESSED_CALLBACK_MASK(light_mask,light,LightMask,light);
	CLICKED_CALLBACK_MASK(light_mask,light,LightMask,light,TR("Light mask"),"Nodes::Light::light_mask_changed")
	PRESSED_CALLBACK_MASK(viewport_mask,light,ViewportMask,light);
	CLICKED_CALLBACK_MASK(viewport_mask,light,ViewportMask,light,TR("Viewport mask"),"Nodes::Light::viewport_mask_changed")
	PRESSED_CALLBACK_SLIDER(multiplier,light,Multiplier,light,modify)
	PRESSED_CALLBACK_SLIDER(diffuse_scale,light,DiffuseScale,light,modify)
	PRESSED_CALLBACK_SLIDER(normal_scale,light,NormalScale,light,modify)
	PRESSED_CALLBACK_SLIDER(specular_scale,light,SpecularScale,light,modify)
	PRESSED_CALLBACK_SLIDER(specular_power,light,SpecularPower,light,modify)
	PRESSED_CALLBACK_SLIDER(attenuation,light,Attenuation,light,modify)
	
	/*
	 */
	void light_mask_changed(int mask) {
		light.setLightMask(mask);
		light_mask_el.setText(editorFormatMask(light.getLightMask()));
	}
	
	void viewport_mask_changed(int mask) {
		light.setViewportMask(mask);
		viewport_mask_el.setText(editorFormatMask(light.getViewportMask()));
	}
	
	void color_changed(WidgetDialogColor dialog) {
		int type = light.getType();
		Node node = light;
		foreach(Node n; nodes) {
			if(n.getType() != type) continue;
			light = node_cast(n);
			light.setColor(dialog.getColor());
		}
		light = node;
		setSpriteColor(color_s,light.getColor());
	}
	
	void color_clicked() {
		NodesState_ s = new NodesState_();
		s.init_modify(nodes,light);
		vec4 color = light.getColor();
		dialogColor(format(TR("Select %s color"),light.getTypeName()),functionid(color_changed),light.getColor());
		if(compare(color,light.getColor(),STATE_EPSILON) == 0) save_state(s);
		else delete s;
	}
	
	/*
	 */
	CLICKED_CALLBACK(shadow,light,Shadow,light)
	CLICKED_CALLBACK(translucent,light,Translucent,light)
	PRESSED_CALLBACK_VEC3(shadow_offset,light,ShadowOffset,light,modify)
	PRESSED_CALLBACK_SLIDER(shadow_ambient,light,ShadowAmbient,light,modify)
	PRESSED_CALLBACK_SLIDER(shadow_bias,light,ShadowBias,light,modify)
	PRESSED_CALLBACK_SLIDER(shadow_slope,light,ShadowSlope,light,modify)
	PRESSED_CALLBACK_SLIDER(shadow_softness,light,ShadowSoftness,light,modify)
	PRESSED_CALLBACK_SLIDER(translucent_bias,light,TranslucentBias,light,modify)
	PRESSED_CALLBACK_FLOAT(deferred_distance,light,DeferredDistance,light,modify)
	PRESSED_CALLBACK_FLOAT(visible_distance,light,VisibleDistance,light,modify)
	PRESSED_CALLBACK_FLOAT(shadow_distance,light,ShadowDistance,light,modify)
	PRESSED_CALLBACK_FLOAT(fade_distance,light,FadeDistance,light,modify)
	CHANGED_CALLBACK_COMBOBOX(shadow_size,light,ShadowSize)
	CHANGED_CALLBACK_COMBOBOX(translucent_size,light,TranslucentSize)
	
	/**************************************************************************\
	*
	* Prob
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Prob {
		
		/*
		 */
		WidgetVBox main_vb;
		
		WidgetCheckBox deferred_cb;
		WidgetEditLine light_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetSprite color_s[2];
		WidgetEditLine multiplier_el;
		WidgetSlider multiplier_sl;
		WidgetEditLine attenuation_el;
		WidgetSlider attenuation_sl;
		WidgetEditLine radius_el[3];
		WidgetLabel intensity_l;
		WidgetSprite texture_s;
		
		WidgetEditLine shadow_offset_el[3];
		
		LightProb light;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Light"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update_texture() {
			
			Image image = new Image();
			light.getImageTextureImage(image);
			image.convertToType(IMAGE_2D);
			
			vec4 intensity = vec4_epsilon;
			forloop(int y = 0; image.getHeight()) {
				forloop(int x = 0; image.getWidth()) {
					intensity = max(intensity,image.get2D(x,y));
				}
			}
			
			float scale = 1.0f / max(intensity.x,max(intensity.y,intensity.z));
			forloop(int y = 0; image.getHeight()) {
				forloop(int x = 0; image.getWidth()) {
					image.set2D(x,y,vec4(vec3(image.get2D(x,y)) * scale,1.0f));
				}
			}
			
			texture_s.setImage(image);
			
			delete image;
			
			intensity_l.setText(editorFormat(1.0f / scale));
		}
		
		/*
		 */
		void update() {
			
			Nodes::Light::deferred_cb = deferred_cb;
			Nodes::Light::light_mask_el = light_mask_el;
			Nodes::Light::viewport_mask_el = viewport_mask_el;
			Nodes::Light::color_s = color_s;
			Nodes::Light::multiplier_el = multiplier_el;
			Nodes::Light::multiplier_sl = multiplier_sl;
			Nodes::Light::attenuation_el = attenuation_el;
			Nodes::Light::attenuation_sl = attenuation_sl;
			Nodes::Light::shadow_offset_el = shadow_offset_el;
			Nodes::Light::update();
			
			light = node_cast(node);
			
			setEditLineSameVec3(radius_el,light.getRadius());
			
			update_texture();
		}
		
		/*
		 */
		void update_dynamic() {
			Nodes::Light::update_dynamic();
			light.renderVisualizer();
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3_SAME(radius,light,Radius,light,modify)
		
		/*
		 */
		void radius_enter(string axis) {
			vec3 radius = light.getRadius();
			if(axis == "y" && radius_el[1].getText() == "same") radius_el[1].setText(editorFormat(radius.x));
			if(axis == "z" && radius_el[2].getText() == "same") radius_el[2].setText(editorFormat(radius.x));
		}
		
		void radius_leave(string axis) {
			vec3 radius = light.getRadius();
			if(axis == "y" && radius_el[1].isFocused() == 0 && radius.y == radius.x) radius_el[1].setText("same");
			if(axis == "z" && radius_el[2].isFocused() == 0 && radius.z == radius.x) radius_el[2].setText("same");
		}
		
		/*
		 */
		void grab_clicked() {
			Player player = Unigine::getPlayer();
			if(player == NULL) return;
			Image image = new Image();
			float znear = player.getZNear();
			float zfar = player.getZFar();
			mat4 projection = frustum(-znear,znear,-znear,znear,znear,zfar);
			engine.render.renderImageCube(projection,light.getWorldPosition(),image,128,NULL,1);
			light.setImageTextureImage(image);
			update_texture();
			delete image;
		}
		
		void load_clicked() {
			string name;
			if(dialogFileImage(TR("Select texture to load"),name,DIALOG_FILE_OPEN)) {
				Image image = new Image();
				if(image.load(name)) {
					light.setImageTextureImage(image);
					update_texture();
				} else {
					dialogMessageOk(TR("Error"));
				}
				delete image;
			}
		}
		
		void save_clicked() {
			string name;
			if(dialogFileImageSave(TR("Select texture to save"),name)) {
				Image image = new Image();
				light.getImageTextureImage(image);
				if(lower(extension(name)) != "dds") image.convertToType(IMAGE_2D);
				if(image.save(name) == 0) dialogMessageOk(TR("Error"));
				delete image;
			}
		}
		
	} /* namespace Prob */
	
	/**************************************************************************\
	*
	* Spot
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Spot {
		
		/*
		 */
		WidgetVBox main_vb;
		
		WidgetCheckBox deferred_cb;
		WidgetEditLine light_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetSprite color_s[2];
		WidgetEditLine multiplier_el;
		WidgetSlider multiplier_sl;
		WidgetEditLine attenuation_el;
		WidgetSlider attenuation_sl;
		WidgetEditLine diffuse_scale_el;
		WidgetSlider diffuse_scale_sl;
		WidgetEditLine normal_scale_el;
		WidgetSlider normal_scale_sl;
		WidgetEditLine specular_scale_el;
		WidgetSlider specular_scale_sl;
		WidgetEditLine specular_power_el;
		WidgetSlider specular_power_sl;
		WidgetEditLine radius_el;
		
		WidgetEditLine shadow_offset_el[3];
		
		WidgetEditLine deferred_distance_el;
		WidgetEditLine visible_distance_el;
		WidgetEditLine shadow_distance_el;
		WidgetEditLine fade_distance_el;
		
		LightSpot light;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Light"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Light::deferred_cb = deferred_cb;
			Nodes::Light::light_mask_el = light_mask_el;
			Nodes::Light::viewport_mask_el = viewport_mask_el;
			Nodes::Light::color_s = color_s;
			Nodes::Light::multiplier_el = multiplier_el;
			Nodes::Light::multiplier_sl = multiplier_sl;
			Nodes::Light::attenuation_el = attenuation_el;
			Nodes::Light::attenuation_sl = attenuation_sl;
			Nodes::Light::diffuse_scale_el = diffuse_scale_el;
			Nodes::Light::diffuse_scale_sl = diffuse_scale_sl;
			Nodes::Light::normal_scale_el = normal_scale_el;
			Nodes::Light::normal_scale_sl = normal_scale_sl;
			Nodes::Light::specular_scale_el = specular_scale_el;
			Nodes::Light::specular_scale_sl = specular_scale_sl;
			Nodes::Light::specular_power_el = specular_power_el;
			Nodes::Light::specular_power_sl = specular_power_sl;
			Nodes::Light::shadow_offset_el = shadow_offset_el;
			Nodes::Light::deferred_distance_el = deferred_distance_el;
			Nodes::Light::visible_distance_el = visible_distance_el;
			Nodes::Light::shadow_distance_el = shadow_distance_el;
			Nodes::Light::fade_distance_el = fade_distance_el;
			Nodes::Light::update();
			
			light = node_cast(node);
			
			radius_el.setText(editorFormat(light.getRadius()));
		}
		
		/*
		 */
		void update_dynamic() {
			Nodes::Light::update_dynamic();
			light.renderVisualizer();
		}
		
		/*
		 */
		PRESSED_CALLBACK_FLOAT(radius,light,Radius,light,modify)
		
	} /* namespace Spot */
	
	/**************************************************************************\
	*
	* Omni
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Omni {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetVBox shadow_vb;
		
		WidgetCheckBox deferred_cb;
		WidgetEditLine light_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetSprite color_s[2];
		WidgetEditLine multiplier_el;
		WidgetSlider multiplier_sl;
		WidgetEditLine attenuation_el;
		WidgetSlider attenuation_sl;
		WidgetEditLine diffuse_scale_el;
		WidgetSlider diffuse_scale_sl;
		WidgetEditLine normal_scale_el;
		WidgetSlider normal_scale_sl;
		WidgetEditLine specular_scale_el;
		WidgetSlider specular_scale_sl;
		WidgetEditLine specular_power_el;
		WidgetSlider specular_power_sl;
		WidgetEditLine radius_el[3];
		
		WidgetCheckBox shadow_cb;
		WidgetCheckBox translucent_cb;
		WidgetEditLine shadow_offset_el[3];
		WidgetComboBox shadow_size_cb;
		WidgetComboBox translucent_size_cb;
		WidgetEditLine shadow_mask_el;
		WidgetEditLine shadow_ambient_el;
		WidgetSlider shadow_ambient_sl;
		WidgetEditLine shadow_bias_el;
		WidgetSlider shadow_bias_sl;
		WidgetEditLine shadow_slope_el;
		WidgetSlider shadow_slope_sl;
		WidgetEditLine shadow_softness_el;
		WidgetSlider shadow_softness_sl;
		WidgetEditLine translucent_bias_el;
		WidgetSlider translucent_bias_sl;
		
		WidgetEditLine deferred_distance_el;
		WidgetEditLine visible_distance_el;
		WidgetEditLine shadow_distance_el;
		WidgetEditLine fade_distance_el;
		
		WidgetEditLine texture_el;
		
		LightOmni light;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Light"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			parameters_tb.addTab(TR("Shadow"));
			parameters_tb.addChild(shadow_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
			enableCallbacks(shadow_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Light::deferred_cb = deferred_cb;
			Nodes::Light::light_mask_el = light_mask_el;
			Nodes::Light::viewport_mask_el = viewport_mask_el;
			Nodes::Light::color_s = color_s;
			Nodes::Light::multiplier_el = multiplier_el;
			Nodes::Light::multiplier_sl = multiplier_sl;
			Nodes::Light::attenuation_el = attenuation_el;
			Nodes::Light::attenuation_sl = attenuation_sl;
			Nodes::Light::diffuse_scale_el = diffuse_scale_el;
			Nodes::Light::diffuse_scale_sl = diffuse_scale_sl;
			Nodes::Light::normal_scale_el = normal_scale_el;
			Nodes::Light::normal_scale_sl = normal_scale_sl;
			Nodes::Light::specular_scale_el = specular_scale_el;
			Nodes::Light::specular_scale_sl = specular_scale_sl;
			Nodes::Light::specular_power_el = specular_power_el;
			Nodes::Light::specular_power_sl = specular_power_sl;
			Nodes::Light::shadow_cb = shadow_cb;
			Nodes::Light::translucent_cb = translucent_cb;
			Nodes::Light::shadow_offset_el = shadow_offset_el;
			Nodes::Light::shadow_size_cb = shadow_size_cb;
			Nodes::Light::translucent_size_cb = translucent_size_cb;
			Nodes::Light::shadow_ambient_el = shadow_ambient_el;
			Nodes::Light::shadow_ambient_sl = shadow_ambient_sl;
			Nodes::Light::shadow_bias_el = shadow_bias_el;
			Nodes::Light::shadow_bias_sl = shadow_bias_sl;
			Nodes::Light::shadow_slope_el = shadow_slope_el;
			Nodes::Light::shadow_slope_sl = shadow_slope_sl;
			Nodes::Light::shadow_softness_el = shadow_softness_el;
			Nodes::Light::shadow_softness_sl = shadow_softness_sl;
			Nodes::Light::translucent_bias_el = translucent_bias_el;
			Nodes::Light::translucent_bias_sl = translucent_bias_sl;
			Nodes::Light::deferred_distance_el = deferred_distance_el;
			Nodes::Light::visible_distance_el = visible_distance_el;
			Nodes::Light::shadow_distance_el = shadow_distance_el;
			Nodes::Light::fade_distance_el = fade_distance_el;
			Nodes::Light::update();
			
			light = node_cast(node);
			
			setEditLineSameVec3(radius_el,light.getRadius());
			
			texture_el.setText(light.getImageTextureName());
			
			shadow_mask_el.setText(editorFormatMask(light.getShadowMask()));
		}
		
		/*
		 */
		void update_dynamic() {
			Nodes::Light::update_dynamic();
			light.renderVisualizer();
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3_SAME(radius,light,Radius,light,modify)
		PRESSED_CALLBACK(texture,light,ImageTextureName,light,string,modify)
		LOAD_CLICKED_CALLBACK(texture,light,ImageTextureName,format(TR("Select %s texture"),light.getTypeName()))
		VIEW_CLICKED_CALLBACK(texture,light,ImageTextureName,format(TR("%s texture"),light.getTypeName()))
		CLEAR_CLICKED_CALLBACK(texture,light,ImageTextureName,format(TR("Clear %s texture?"),light.getTypeName()),"","")
		PRESSED_CALLBACK_MASK(shadow_mask,light,ShadowMask,light);
		CLICKED_CALLBACK_MASK(shadow_mask,light,ShadowMask,light,TR("Shadow mask"),"Nodes::Light::Omni::shadow_mask_changed")
		
		/*
		 */
		void radius_enter(string axis) {
			vec3 radius = light.getRadius();
			if(axis == "y" && radius_el[1].getText() == "same") radius_el[1].setText(editorFormat(radius.x));
			if(axis == "z" && radius_el[2].getText() == "same") radius_el[2].setText(editorFormat(radius.x));
		}
		
		void radius_leave(string axis) {
			vec3 radius = light.getRadius();
			if(axis == "y" && radius_el[1].isFocused() == 0 && radius.y == radius.x) radius_el[1].setText("same");
			if(axis == "z" && radius_el[2].isFocused() == 0 && radius.z == radius.x) radius_el[2].setText("same");
		}
		
		/*
		 */
		void shadow_mask_changed(int mask) {
			light.setShadowMask(mask);
			shadow_mask_el.setText(editorFormatMask(light.getShadowMask()));
		}
		
	} /* namespace Omni */
	
	/**************************************************************************\
	*
	* Proj
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Proj {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetVBox shadow_vb;
		
		WidgetCheckBox deferred_cb;
		WidgetEditLine light_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetSprite color_s[2];
		WidgetEditLine multiplier_el;
		WidgetSlider multiplier_sl;
		WidgetEditLine attenuation_el;
		WidgetSlider attenuation_sl;
		WidgetEditLine diffuse_scale_el;
		WidgetSlider diffuse_scale_sl;
		WidgetEditLine normal_scale_el;
		WidgetSlider normal_scale_sl;
		WidgetEditLine specular_scale_el;
		WidgetSlider specular_scale_sl;
		WidgetEditLine specular_power_el;
		WidgetSlider specular_power_sl;
		WidgetEditLine radius_el;
		WidgetEditLine fov_el;
		WidgetSlider fov_sl;
		WidgetEditLine znear_el;
		
		WidgetCheckBox shadow_cb;
		WidgetCheckBox translucent_cb;
		WidgetEditLine shadow_offset_el[3];
		WidgetComboBox shadow_size_cb;
		WidgetComboBox translucent_size_cb;
		WidgetEditLine shadow_ambient_el;
		WidgetSlider shadow_ambient_sl;
		WidgetEditLine shadow_bias_el;
		WidgetSlider shadow_bias_sl;
		WidgetEditLine shadow_slope_el;
		WidgetSlider shadow_slope_sl;
		WidgetEditLine shadow_softness_el;
		WidgetSlider shadow_softness_sl;
		WidgetEditLine translucent_bias_el;
		WidgetSlider translucent_bias_sl;
		
		WidgetEditLine deferred_distance_el;
		WidgetEditLine visible_distance_el;
		WidgetEditLine shadow_distance_el;
		WidgetEditLine fade_distance_el;
		
		WidgetEditLine texture_el;
		
		LightProj light;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Light"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			parameters_tb.addTab(TR("Shadow"));
			parameters_tb.addChild(shadow_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
			enableCallbacks(shadow_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Light::deferred_cb = deferred_cb;
			Nodes::Light::light_mask_el = light_mask_el;
			Nodes::Light::viewport_mask_el = viewport_mask_el;
			Nodes::Light::color_s = color_s;
			Nodes::Light::multiplier_el = multiplier_el;
			Nodes::Light::multiplier_sl = multiplier_sl;
			Nodes::Light::attenuation_el = attenuation_el;
			Nodes::Light::attenuation_sl = attenuation_sl;
			Nodes::Light::diffuse_scale_el = diffuse_scale_el;
			Nodes::Light::diffuse_scale_sl = diffuse_scale_sl;
			Nodes::Light::normal_scale_el = normal_scale_el;
			Nodes::Light::normal_scale_sl = normal_scale_sl;
			Nodes::Light::specular_scale_el = specular_scale_el;
			Nodes::Light::specular_scale_sl = specular_scale_sl;
			Nodes::Light::specular_power_el = specular_power_el;
			Nodes::Light::specular_power_sl = specular_power_sl;
			Nodes::Light::shadow_cb = shadow_cb;
			Nodes::Light::translucent_cb = translucent_cb;
			Nodes::Light::shadow_offset_el = shadow_offset_el;
			Nodes::Light::shadow_size_cb = shadow_size_cb;
			Nodes::Light::translucent_size_cb = translucent_size_cb;
			Nodes::Light::shadow_ambient_el = shadow_ambient_el;
			Nodes::Light::shadow_ambient_sl = shadow_ambient_sl;
			Nodes::Light::shadow_bias_el = shadow_bias_el;
			Nodes::Light::shadow_bias_sl = shadow_bias_sl;
			Nodes::Light::shadow_slope_el = shadow_slope_el;
			Nodes::Light::shadow_slope_sl = shadow_slope_sl;
			Nodes::Light::shadow_softness_el = shadow_softness_el;
			Nodes::Light::shadow_softness_sl = shadow_softness_sl;
			Nodes::Light::translucent_bias_el = translucent_bias_el;
			Nodes::Light::translucent_bias_sl = translucent_bias_sl;
			Nodes::Light::deferred_distance_el = deferred_distance_el;
			Nodes::Light::visible_distance_el = visible_distance_el;
			Nodes::Light::shadow_distance_el = shadow_distance_el;
			Nodes::Light::fade_distance_el = fade_distance_el;
			Nodes::Light::update();
			
			light = node_cast(node);
			
			radius_el.setText(editorFormat(light.getRadius()));
			fov_el.setText(editorFormat(light.getFov()));
			znear_el.setText(editorFormat(light.getZNear()));
			
			texture_el.setText(light.getImageTextureName());
		}
		
		/*
		 */
		void update_dynamic() {
			Nodes::Light::update_dynamic();
			light.renderVisualizer();
		}
		
		/*
		 */
		PRESSED_CALLBACK_FLOAT(radius,light,Radius,light,modify)
		PRESSED_CALLBACK_SLIDER(fov,light,Fov,light,modify)
		PRESSED_CALLBACK_FLOAT(znear,light,ZNear,light,modify)
		PRESSED_CALLBACK(texture,light,ImageTextureName,light,string,modify)
		LOAD_CLICKED_CALLBACK(texture,light,ImageTextureName,format(TR("Select %s texture"),light.getTypeName()))
		VIEW_CLICKED_CALLBACK(texture,light,ImageTextureName,format(TR("%s texture"),light.getTypeName()))
		CLEAR_CLICKED_CALLBACK(texture,light,ImageTextureName,format(TR("Clear %s texture?"),light.getTypeName()),"core/textures/light_base_proj.dds","core/textures/light_base_proj.dds")
		
	} /* namespace Proj */
	
	/**************************************************************************\
	*
	* World
	*
	\**************************************************************************/
	
	/*
	 */
	namespace World {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetVBox shadow_vb;
		
		WidgetCheckBox deferred_cb;
		WidgetEditLine light_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetSprite color_s[2];
		WidgetEditLine multiplier_el;
		WidgetSlider multiplier_sl;
		WidgetEditLine diffuse_scale_el;
		WidgetSlider diffuse_scale_sl;
		WidgetEditLine normal_scale_el;
		WidgetSlider normal_scale_sl;
		WidgetEditLine specular_scale_el;
		WidgetSlider specular_scale_sl;
		WidgetEditLine specular_power_el;
		WidgetSlider specular_power_sl;
		WidgetCheckBox scattering_cb;
		
		WidgetCheckBox shadow_cb;
		WidgetCheckBox translucent_cb;
		WidgetEditLine shadow_offset_el[3];
		WidgetComboBox shadow_size_cb;
		WidgetComboBox translucent_size_cb;
		WidgetEditLine shadow_range_el;
		WidgetEditLine shadow_distribute_el;
		WidgetSlider shadow_distribute_sl;
		WidgetEditLine num_shadow_splits_el;
		WidgetSlider num_shadow_splits_sl;
		WidgetEditLine shadow_ambient_el;
		WidgetSlider shadow_ambient_sl;
		WidgetEditLine shadow_bias_el;
		WidgetSlider shadow_bias_sl;
		WidgetEditLine shadow_slope_el;
		WidgetSlider shadow_slope_sl;
		WidgetEditLine shadow_softness_el;
		WidgetSlider shadow_softness_sl;
		WidgetEditLine translucent_bias_el;
		WidgetSlider translucent_bias_sl;
		
		WidgetEditLine deferred_distance_el;
		WidgetEditLine visible_distance_el;
		WidgetEditLine shadow_distance_el;
		WidgetEditLine fade_distance_el;
		
		LightWorld light;
		
		enum {
			VISUALIZER_SIZE = 0.75f,
		};
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Light"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			parameters_tb.addTab(TR("Shadow"));
			parameters_tb.addChild(shadow_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
			enableCallbacks(shadow_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Light::deferred_cb = deferred_cb;
			Nodes::Light::light_mask_el = light_mask_el;
			Nodes::Light::viewport_mask_el = viewport_mask_el;
			Nodes::Light::color_s = color_s;
			Nodes::Light::multiplier_el = multiplier_el;
			Nodes::Light::multiplier_sl = multiplier_sl;
			Nodes::Light::diffuse_scale_el = diffuse_scale_el;
			Nodes::Light::diffuse_scale_sl = diffuse_scale_sl;
			Nodes::Light::normal_scale_el = normal_scale_el;
			Nodes::Light::normal_scale_sl = normal_scale_sl;
			Nodes::Light::specular_scale_el = specular_scale_el;
			Nodes::Light::specular_scale_sl = specular_scale_sl;
			Nodes::Light::specular_power_el = specular_power_el;
			Nodes::Light::specular_power_sl = specular_power_sl;
			Nodes::Light::shadow_cb = shadow_cb;
			Nodes::Light::translucent_cb = translucent_cb;
			Nodes::Light::shadow_offset_el = shadow_offset_el;
			Nodes::Light::shadow_size_cb = shadow_size_cb;
			Nodes::Light::translucent_size_cb = translucent_size_cb;
			Nodes::Light::shadow_ambient_el = shadow_ambient_el;
			Nodes::Light::shadow_ambient_sl = shadow_ambient_sl;
			Nodes::Light::shadow_bias_el = shadow_bias_el;
			Nodes::Light::shadow_bias_sl = shadow_bias_sl;
			Nodes::Light::shadow_slope_el = shadow_slope_el;
			Nodes::Light::shadow_slope_sl = shadow_slope_sl;
			Nodes::Light::shadow_softness_el = shadow_softness_el;
			Nodes::Light::shadow_softness_sl = shadow_softness_sl;
			Nodes::Light::translucent_bias_el = translucent_bias_el;
			Nodes::Light::translucent_bias_sl = translucent_bias_sl;
			Nodes::Light::deferred_distance_el = deferred_distance_el;
			Nodes::Light::visible_distance_el = visible_distance_el;
			Nodes::Light::shadow_distance_el = shadow_distance_el;
			Nodes::Light::fade_distance_el = fade_distance_el;
			Nodes::Light::update();
			
			light = node_cast(node);
			
			scattering_cb.setCallbackEnabled(GUI_CLICKED,0);
			scattering_cb.setChecked(light.getScattering());
			scattering_cb.setCallbackEnabled(GUI_CLICKED,1);
			
			shadow_range_el.setText(editorFormat(light.getShadowRange()));
			shadow_distribute_el.setText(editorFormat(light.getShadowDistribute()));
			num_shadow_splits_el.setText(string(light.getNumShadowSplits()));
		}
		
		/*
		 */
		void update_dynamic() {
			Nodes::Light::update_dynamic();
			render_visualizer(light);
		}
		
		void render_visualizer(Light l) {
			engine.visualizer.renderDirection(l.getWorldPosition() - l.getWorldDirection() * engine.visualizer.getScale(l.getWorldPosition()) * VISUALIZER_SIZE,l.getWorldDirection() * VISUALIZER_SIZE,l.getColor());
			engine.visualizer.renderSphere(engine.visualizer.getScale(l.getWorldPosition()) * VISUALIZER_SIZE,translate(l.getWorldPosition()) * rotation(l.getWorldRotation()),l.getColor());
		}
		
		/*
		 */
		CLICKED_CALLBACK(scattering,light,Scattering,light)
		PRESSED_CALLBACK_FLOAT(shadow_range,light,ShadowRange,light,modify)
		PRESSED_CALLBACK_SLIDER(shadow_distribute,light,ShadowDistribute,light,modify)
		PRESSED_CALLBACK_SLIDER_INT(num_shadow_splits,light,NumShadowSplits,light,modify)
		
	} /* namespace World */
	
} /* namespace Light */
