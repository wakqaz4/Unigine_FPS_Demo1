/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_render.h
 * Desc:    Unigine editor
 * Version: 1.42
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
#define RENDER_PRESSED_CALLBACK(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	if(skip_state == false && float(WIDGET ## _el.getText()) != engine.render.get ## FUNC()) save_state(create_state()); \
	engine.render.set ## FUNC(float(WIDGET ## _el.getText())); \
	WIDGET ## _el.setText(editorFormat(engine.render.get ## FUNC())); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
}

#define RENDER_PRESSED_CALLBACK_STRING(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	if(skip_state == false && float(WIDGET ## _el.getText()) != engine.render.get ## FUNC()) save_state(create_state()); \
	engine.render.set ## FUNC(float(WIDGET ## _el.getText())); \
	WIDGET ## _el.setText(string(engine.render.get ## FUNC())); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
}

#define RENDER_PRESSED_CALLBACK_VEC3(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	if(getEditLineVec3(WIDGET ## _el) != engine.render.get ## FUNC()) save_state(create_state()); \
	engine.render.set ## FUNC(getEditLineVec3(WIDGET ## _el)); \
	setEditLineVec3(WIDGET ## _el,engine.render.get ## FUNC()); \
}

#define RENDER_CALLBACK_COLOR(WIDGET,FUNC,TITLE,NAMESPACE) \
void WIDGET ## _changed(WidgetDialogColor dialog) { \
	engine.render.set ## FUNC(dialog.getColor()); \
	setSpriteColor(WIDGET ## _s,engine.render.get ## FUNC()); \
} \
void WIDGET ## _clicked() { \
	Unredo::State s = create_state(); \
	vec4 color = engine.render.get ## FUNC(); \
	dialogColor(TITLE,functionid(WIDGET ## _changed),engine.render.get ## FUNC()); \
	if(compare(color,engine.render.get ## FUNC(),STATE_EPSILON) == 0) save_state(s); \
	else delete s; \
}

#define RENDER_CALLBACK_TEXTURE(WIDGET,FUNC,TITLE_LOAD,TITLE_VIEW,TITLE_CLEAR) \
void WIDGET ## _pressed() { \
	if(WIDGET ## _el.getText() != engine.render.get ## FUNC()) save_state(create_state()); \
	if(WIDGET ## _el.getText() != "") { \
		if(engine.render.set ## FUNC(WIDGET ## _el.getText()) == 0) dialogMessageOk(TR("Error")); \
	} else { \
		engine.render.set ## FUNC(NULL); \
	} \
	WIDGET ## _el.setText(engine.render.get ## FUNC()); \
} \
void WIDGET ## _load_clicked() { \
	Unredo::State s = NULL; \
	string old_name = engine.render.get ## FUNC(); \
	string name = old_name; \
	if(dialogFileImage(TITLE_LOAD,name,DIALOG_FILE_OPEN)) { \
		if(old_name != name) s = create_state(); \
		if(engine.render.set ## FUNC(name) == 0) { \
			dialogMessageOk(TR("Error")); \
			delete s; \
			s = NULL; \
			if(old_name == "") engine.render.set ## FUNC(NULL); \
			else engine.render.set ## FUNC(old_name); \
		} \
		WIDGET ## _el.setText(engine.render.get ## FUNC()); \
	} \
	save_state(s); \
} \
void WIDGET ## _view_clicked() { \
	if(engine.render.get ## FUNC() != "") { \
		dialogImage(TITLE_VIEW,engine.render.get ## FUNC()); \
	} \
} \
void WIDGET ## _clear_clicked() { \
	if(engine.render.get ## FUNC() != "") { \
		Unredo::State s = create_state(); \
		if(dialogMessageYesNo(TR("Confirm"),TITLE_CLEAR)) { \
			engine.render.set ## FUNC(NULL); \
			WIDGET ## _el.setText(engine.render.get ## FUNC()); \
			save_state(s); \
		} else { \
			delete s; \
		} \
	} \
}

#define RENDER_PRESSED_CALLBACK_SLIDER(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	Unredo::State s = NULL; \
	if(skip_state == false && engine.render.get ## FUNC() != float(WIDGET ## _el.getText())) s = create_state(); \
	engine.render.set ## FUNC(float(WIDGET ## _el.getText())); \
	WIDGET ## _el.setText(editorFormat(engine.render.get ## FUNC())); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

#define RENDER_PRESSED_CALLBACK_SLIDER_2(WIDGET_0,FUNC_0,WIDGET_1,FUNC_1) \
void WIDGET_0 ## _pressed() { \
	Unredo::State s = NULL; \
	if(skip_state == false && engine.render.get ## FUNC_0() != float(WIDGET_0 ## _el.getText())) s = create_state(); \
	engine.render.set ## FUNC_0(float(WIDGET_0 ## _el.getText())); \
	WIDGET_0 ## _el.setText(editorFormat(engine.render.get ## FUNC_0())); \
	WIDGET_1 ## _el.setText(editorFormat(engine.render.get ## FUNC_1())); \
	if(toolsGetSelection() && WIDGET_0 ## _el.isFocused()) { \
		WIDGET_0 ## _el.setCursor(0); \
		WIDGET_0 ## _el.setSelection(1024); \
	} \
	if(toolsGetSelection() && WIDGET_1 ## _el.isFocused()) { \
		WIDGET_1 ## _el.setCursor(0); \
		WIDGET_1 ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

#define RENDER_PRESSED_CALLBACK_SLIDER_SWIZZLE(WIDGET,FUNC,SWIZZLE) \
void WIDGET ## _pressed() { \
	Unredo::State s = NULL; \
	if(skip_state == false && engine.render.get ## FUNC().SWIZZLE != float(WIDGET ## _el.getText())) s = create_state(); \
	vec4 value = engine.render.get ## FUNC(); \
	value.SWIZZLE = float(WIDGET ## _el.getText()); \
	engine.render.set ## FUNC(value); \
	WIDGET ## _el.setText(editorFormat(engine.render.get ## FUNC().SWIZZLE)); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

#define RENDER_PRESSED_CALLBACK_SLIDER_INT(WIDGET,FUNC,SCALE) \
void WIDGET ## _pressed() { \
	Unredo::State s = NULL; \
	if(skip_state == false && engine.render.get ## FUNC() != int(WIDGET ## _el.getText()) / SCALE) s = create_state(); \
	engine.render.set ## FUNC(int(WIDGET ## _el.getText()) / SCALE); \
	WIDGET ## _el.setText(string(int(engine.render.get ## FUNC() * SCALE))); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

#define RENDER_CALLBACK_MATERIAL(WIDGET,FUNC,TITLE_CLEAR) \
void WIDGET ## _pressed() { \
	if(WIDGET ## _el.getText() != engine.render.get ## FUNC()) save_state(create_state()); \
	if(WIDGET ## _el.getText() != "") engine.render.set ## FUNC(WIDGET ## _el.getText()); \
	else engine.render.set ## FUNC(NULL); \
	WIDGET ## _el.setText(engine.render.get ## FUNC()); \
} \
void WIDGET ## _clear_clicked() { \
	if(engine.render.get ## FUNC() != "") { \
		Unredo::State s = create_state(); \
		if(dialogMessageYesNo(TR("Confirm"),TITLE_CLEAR)) { \
			engine.render.set ## FUNC(NULL); \
			WIDGET ## _el.setText(engine.render.get ## FUNC()); \
			save_state(s); \
		} else { \
			delete s; \
		} \
	} \
}

#define RENDER_PRESSED_CALLBACK_MASK(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	Unredo::State s = create_state(); \
	int mask = int("0x" + WIDGET ## _el.getText()); \
	if(engine.render.get ## FUNC() != mask) { \
		save_state(create_state()); \
		engine.render.set ## FUNC(mask); \
	} \
	WIDGET ## _el.setText(editorFormatMask(engine.render.get ## FUNC())); \
	if(toolsGetSelection() && WIDGET ## _el.isFocused()) { \
		WIDGET ## _el.setCursor(0); \
		WIDGET ## _el.setSelection(1024); \
	} \
	save_state(s); \
}

#define RENDER_CLICKED_CALLBACK_MASK(WIDGET,FUNC,TITLE,FUNC_ID) \
void WIDGET ## _clicked() { \
	Unredo::State s = create_state(); \
	int old_mask = engine.render.get ## FUNC(); \
	dialogMask(TITLE,FUNC_ID,old_mask); \
	int mask = engine.render.get ## FUNC(); \
	if(old_mask != mask); save_state(create_state()); \
	save_state(s); \
}

/*
 */
#define RENDER_CLEAR_CLICKED_CALLBACK(WIDGET,FUNC,VALUE) \
void WIDGET ## _clear_clicked() { \
	if(engine.render.get ## FUNC() != VALUE) save_state(create_state()); \
	engine.render.set ## FUNC(VALUE); \
	WIDGET ## _el.setText(editorFormat(engine.render.get ## FUNC())); \
}

/*
 */
namespace Render {
	
	/*
	 */
	DECLARE_WINDOW
	
	WidgetTabBox tabbox;						// render tabbox
	
	#include <editor/editor_render_unredo.h>
	
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
		WidgetEditLine distance_scale_el;		// distance scale
		WidgetEditLine world_budget_el;			// world budget
		WidgetEditLine render_budget_el;		// render budget
		WidgetEditLine physics_budget_el;		// physics budget
		WidgetEditLine world_distance_el;		// world distance
		WidgetEditLine physics_distance_el;		// physics distance
		WidgetEditLine field_distance_el;		// field distance
		WidgetEditLine light_distance_el;		// light distance
		WidgetEditLine decal_distance_el;		// decal distance
		WidgetEditLine object_distance_el;		// object distance
		WidgetEditLine clutter_distance_el;		// clutter distance
		WidgetEditLine shadow_distance_el;		// shadow distance
		WidgetEditLine reflection_distance_el;	// reflection distance
		
		WidgetSprite ambient_color_s[2];		// ambient color
		WidgetSprite reflection_color_s[2];		// reflection color
		WidgetSprite background_color_s[2];		// background color
		WidgetSprite wireframe_color_s[2];		// wireframe color
		
		WidgetEditLine environment_texture_el;	// environment texture name
		WidgetEditLine environment_exposure_el;	// environment exposure
		
		WidgetEditLine render_materials_el;		// render materials
		WidgetEditLine composite_material_el;	// composite material
		WidgetEditLine post_materials_el;		// post materials
		
		DECLARE_SLIDER_CALLBACKS
		
		/*
		 */
		void update() {
			
			distance_scale_el.setText(editorFormat(engine.render.getDistanceScale()));
			
			world_budget_el.setText(editorFormat(engine.world.getBudget()));
			render_budget_el.setText(editorFormat(engine.render.getBudget()));
			physics_budget_el.setText(editorFormat(engine.physics.getBudget()));
			
			world_distance_el.setText(editorFormat(engine.world.getDistance()));
			physics_distance_el.setText(editorFormat(engine.physics.getDistance()));
			field_distance_el.setText(editorFormat(engine.render.getFieldDistance()));
			light_distance_el.setText(editorFormat(engine.render.getLightDistance()));
			decal_distance_el.setText(editorFormat(engine.render.getDecalDistance()));
			object_distance_el.setText(editorFormat(engine.render.getObjectDistance()));
			clutter_distance_el.setText(editorFormat(engine.render.getClutterDistance()));
			shadow_distance_el.setText(editorFormat(engine.render.getShadowDistance()));
			reflection_distance_el.setText(editorFormat(engine.render.getReflectionDistance()));
			
			setSpriteColor(ambient_color_s,engine.render.getAmbientColor());
			setSpriteColor(reflection_color_s,engine.render.getReflectionColor());
			setSpriteColor(background_color_s,engine.render.getBackgroundColor());
			setSpriteColor(wireframe_color_s,engine.render.getWireframeColor());
			
			environment_texture_el.setText(engine.render.getEnvironmentTextureName());
			environment_exposure_el.setText(editorFormat(engine.render.getEnvironmentExposure()));
			
			render_materials_el.setText(engine.render.getRenderMaterials());
			composite_material_el.setText(engine.render.getCompositeMaterial());
			post_materials_el.setText(engine.render.getPostMaterials());
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		void world_budget_pressed() {
			if(float(world_budget_el.getText()) != engine.world.getBudget()) save_state(create_state());
			engine.world.setBudget(float(world_budget_el.getText()));
			world_budget_el.setText(editorFormat(engine.world.getBudget()));
		}
		
		void physics_budget_pressed() {
			if(float(physics_budget_el.getText()) != engine.physics.getBudget()) save_state(create_state());
			engine.physics.setBudget(float(physics_budget_el.getText()));
			physics_budget_el.setText(editorFormat(engine.physics.getBudget()));
		}
		
		/*
		 */
		void world_distance_pressed() {
			if(float(world_distance_el.getText()) != engine.world.getDistance()) save_state(create_state());
			engine.world.setDistance(float(world_distance_el.getText()));
			world_distance_el.setText(editorFormat(engine.world.getDistance()));
		}
		
		void physics_distance_pressed() {
			if(float(physics_distance_el.getText()) != engine.physics.getDistance()) save_state(create_state());
			engine.physics.setDistance(float(physics_distance_el.getText()));
			physics_distance_el.setText(editorFormat(engine.physics.getDistance()));
		}
		
		/*
		 */
		RENDER_PRESSED_CALLBACK_SLIDER(distance_scale,DistanceScale)
		RENDER_PRESSED_CALLBACK(render_budget,Budget)
		RENDER_PRESSED_CALLBACK(field_distance,FieldDistance)
		RENDER_PRESSED_CALLBACK(light_distance,LightDistance)
		RENDER_PRESSED_CALLBACK(decal_distance,DecalDistance)
		RENDER_PRESSED_CALLBACK(object_distance,ObjectDistance)
		RENDER_PRESSED_CALLBACK(clutter_distance,ClutterDistance)
		RENDER_PRESSED_CALLBACK(shadow_distance,ShadowDistance)
		RENDER_PRESSED_CALLBACK(reflection_distance,ReflectionDistance)
		RENDER_CALLBACK_COLOR(ambient_color,AmbientColor,TR("Select ambient color"),"Common")
		RENDER_CALLBACK_COLOR(reflection_color,ReflectionColor,TR("Select reflection color"),"Common")
		RENDER_CALLBACK_COLOR(background_color,BackgroundColor,TR("Select background color"),"Common")
		RENDER_CALLBACK_COLOR(wireframe_color,WireframeColor,TR("Select wireframe color"),"Common")
		RENDER_CALLBACK_TEXTURE(environment_texture,EnvironmentTextureName,TR("Select render environment texture"),TR("Render environment texture"),TR("Clear render environment texture?"))
		RENDER_PRESSED_CALLBACK_SLIDER(environment_exposure,EnvironmentExposure)
		RENDER_CALLBACK_MATERIAL(render_materials,RenderMaterials,TR("Clear render materials?"))
		RENDER_CALLBACK_MATERIAL(composite_material,CompositeMaterial,TR("Clear composite material?"))
		RENDER_CALLBACK_MATERIAL(post_materials,PostMaterials,TR("Clear post materials?"))
		
	} /* namespace Common */
	
	/**************************************************************************\
	*
	* Parameters
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Parameters {
		
		/*
		 */
		WidgetEditLine tessellation_scale_el;			// tessellation scale
		WidgetEditLine tessellation_factor_el;			// tessellation factor
		WidgetEditLine tessellation_distance_el;		// tessellation distance
		
		WidgetEditLine animation_stem_el;				// animation stem
		WidgetEditLine animation_leaf_el;				// animation leaf
		WidgetEditLine animation_scale_el;				// animation scale
		WidgetEditLine animation_wind_el[3];			// animation wind
		
		WidgetEditLine light_prob_mask_el;				// light prob environment light mask
		WidgetEditLine light_prob_distance_el;			// light prob environment distance
		WidgetEditLine light_prob_attenuation_el;		// light prob environment attenuation
		WidgetEditLine light_prob_fresnel_bias_el;		// light prob environment fresnel bias
		WidgetEditLine light_prob_fresnel_power_el;		// light prob environment fresnel power
		WidgetEditLine light_prob_diffuse_scale_el;		// light prob environment diffuse scale
		WidgetEditLine light_prob_specular_scale_el;	// light prob environment specular scale
		
		DECLARE_SLIDER_CALLBACKS
		
		/*
		 */
		void update() {
			
			tessellation_scale_el.setText(editorFormat(engine.render.getTessellationScale()));
			tessellation_factor_el.setText(editorFormat(engine.render.getTessellationFactor()));
			tessellation_distance_el.setText(editorFormat(engine.render.getTessellationDistance()));
			
			animation_stem_el.setText(editorFormat(engine.render.getAnimationStem()));
			animation_leaf_el.setText(editorFormat(engine.render.getAnimationLeaf()));
			animation_scale_el.setText(editorFormat(engine.render.getAnimationScale()));
			setEditLineVec3(animation_wind_el,engine.render.getAnimationWind());
			
			light_prob_mask_el.setText(editorFormatMask(engine.render.getLightProbMask()));
			light_prob_distance_el.setText(editorFormat(engine.render.getLightProbDistance()));
			light_prob_attenuation_el.setText(editorFormat(engine.render.getLightProbAttenuation()));
			light_prob_fresnel_bias_el.setText(editorFormat(engine.render.getLightProbFresnelBias()));
			light_prob_fresnel_power_el.setText(editorFormat(engine.render.getLightProbFresnelPower()));
			light_prob_diffuse_scale_el.setText(editorFormat(engine.render.getLightProbDiffuseScale()));
			light_prob_specular_scale_el.setText(editorFormat(engine.render.getLightProbSpecularScale()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			setEnabled(engine.render.getTessellation(),(
				tessellation_scale_el,
				tessellation_factor_el,
				tessellation_distance_el,
			));
			
			setEnabled(engine.render.getLightProb(),(
				light_prob_mask_el,
				light_prob_distance_el,
				light_prob_attenuation_el,
				light_prob_fresnel_bias_el,
				light_prob_fresnel_power_el,
				light_prob_diffuse_scale_el,
				light_prob_specular_scale_el,
			));
		}
		
		/*
		 */
		
		RENDER_PRESSED_CALLBACK_SLIDER(tessellation_scale,TessellationScale)
		RENDER_PRESSED_CALLBACK_SLIDER(tessellation_factor,TessellationFactor)
		RENDER_PRESSED_CALLBACK_SLIDER(tessellation_distance,TessellationDistance)
		
		RENDER_PRESSED_CALLBACK_SLIDER(animation_stem,AnimationStem)
		RENDER_PRESSED_CALLBACK_SLIDER(animation_leaf,AnimationLeaf)
		RENDER_PRESSED_CALLBACK_SLIDER(animation_scale,AnimationScale)
		RENDER_PRESSED_CALLBACK_VEC3(animation_wind,AnimationWind)
		
		RENDER_PRESSED_CALLBACK_MASK(light_prob_mask,LightProbMask);
		RENDER_CLICKED_CALLBACK_MASK(light_prob_mask,LightProbMask,TR("LightProb light mask"),functionid(light_prob_mask_changed))
		RENDER_PRESSED_CALLBACK_SLIDER(light_prob_distance,LightProbDistance)
		RENDER_PRESSED_CALLBACK_SLIDER(light_prob_attenuation,LightProbAttenuation)
		RENDER_PRESSED_CALLBACK_SLIDER(light_prob_fresnel_bias,LightProbFresnelBias)
		RENDER_PRESSED_CALLBACK_SLIDER(light_prob_fresnel_power,LightProbFresnelPower)
		RENDER_PRESSED_CALLBACK_SLIDER(light_prob_diffuse_scale,LightProbDiffuseScale)
		RENDER_PRESSED_CALLBACK_SLIDER(light_prob_specular_scale,LightProbSpecularScale)
		
		void light_prob_mask_changed(int mask) {
			engine.render.setLightProbMask(mask);
			light_prob_mask_el.setText(editorFormatMask(engine.render.getLightProbMask()));
		}
		
	} /* namespace Parameters */
	
	/**************************************************************************\
	*
	* Occlusion
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Occlusion {
		
		/*
		 */
		WidgetEditLine ambient_screen_radius_el;	// ambient screen radius
		WidgetEditLine ambient_sample_radius_el;	// ambient sample radius
		WidgetEditLine ambient_perspective_el;		// ambient sample perspective
		WidgetEditLine ambient_attenuation_el;		// ambient sample attenuation
		WidgetEditLine ambient_threshold_el;		// ambient threshold
		WidgetEditLine ambient_emitter_el;			// ambient emitter
		WidgetEditLine ambient_receiver_el;			// ambient receiver
		WidgetEditLine ambient_scale_el;			// ambient scale
		WidgetEditLine ambient_power_el;			// ambient power
		
		WidgetEditLine light_screen_radius_el;		// light screen radius
		WidgetEditLine light_sample_radius_el;		// light sample radius
		WidgetEditLine light_perspective_el;		// light sample perspective
		WidgetEditLine light_attenuation_el;		// light sample attenuation
		WidgetEditLine light_threshold_el;			// light threshold
		WidgetEditLine light_emitter_el;			// light emitter
		WidgetEditLine light_receiver_el;			// light receiver
		WidgetEditLine light_scale_el;				// light scale
		WidgetEditLine light_power_el;				// light power
		
		DECLARE_SLIDER_CALLBACKS
		
		/*
		 */
		void update() {
			
			ambient_screen_radius_el.setText(editorFormat(engine.render.getOcclusionAmbientScreenRadius()));
			ambient_sample_radius_el.setText(editorFormat(engine.render.getOcclusionAmbientSampleRadius()));
			ambient_perspective_el.setText(editorFormat(engine.render.getOcclusionAmbientPerspective()));
			ambient_attenuation_el.setText(editorFormat(engine.render.getOcclusionAmbientAttenuation()));
			ambient_threshold_el.setText(editorFormat(engine.render.getOcclusionAmbientThreshold()));
			ambient_emitter_el.setText(editorFormat(engine.render.getOcclusionAmbientEmitter()));
			ambient_receiver_el.setText(editorFormat(engine.render.getOcclusionAmbientReceiver()));
			ambient_scale_el.setText(editorFormat(engine.render.getOcclusionAmbientScale()));
			ambient_power_el.setText(editorFormat(engine.render.getOcclusionAmbientPower()));
			
			light_screen_radius_el.setText(editorFormat(engine.render.getOcclusionLightScreenRadius()));
			light_sample_radius_el.setText(editorFormat(engine.render.getOcclusionLightSampleRadius()));
			light_perspective_el.setText(editorFormat(engine.render.getOcclusionLightPerspective()));
			light_attenuation_el.setText(editorFormat(engine.render.getOcclusionLightAttenuation()));
			light_threshold_el.setText(editorFormat(engine.render.getOcclusionLightThreshold()));
			light_emitter_el.setText(editorFormat(engine.render.getOcclusionLightEmitter()));
			light_receiver_el.setText(editorFormat(engine.render.getOcclusionLightReceiver()));
			light_scale_el.setText(editorFormat(engine.render.getOcclusionLightScale()));
			light_power_el.setText(editorFormat(engine.render.getOcclusionLightPower()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			int occlusion = engine.render.getOcclusion();
			
			setEnabled((occlusion == RENDER_OCCLUSION_AMBIENT || occlusion == RENDER_OCCLUSION_AMBIENT_LIGHT),(
				ambient_screen_radius_el,
				ambient_sample_radius_el,
				ambient_perspective_el,
				ambient_attenuation_el,
				ambient_threshold_el,
				ambient_emitter_el,
				ambient_receiver_el,
				ambient_scale_el,
				ambient_power_el,
			));
			
			setEnabled((occlusion == RENDER_OCCLUSION_LIGHT || occlusion == RENDER_OCCLUSION_AMBIENT_LIGHT),(
				light_screen_radius_el,
				light_sample_radius_el,
				light_perspective_el,
				light_attenuation_el,
				light_threshold_el,
				light_emitter_el,
				light_receiver_el,
				light_scale_el,
				light_power_el,
			));
		}
		
		/*
		 */
		RENDER_PRESSED_CALLBACK_SLIDER(ambient_screen_radius,OcclusionAmbientScreenRadius)
		RENDER_PRESSED_CALLBACK_SLIDER(ambient_sample_radius,OcclusionAmbientSampleRadius)
		RENDER_PRESSED_CALLBACK_SLIDER(ambient_perspective,OcclusionAmbientPerspective)
		RENDER_PRESSED_CALLBACK_SLIDER(ambient_attenuation,OcclusionAmbientAttenuation)
		RENDER_PRESSED_CALLBACK_SLIDER(ambient_threshold,OcclusionAmbientThreshold)
		RENDER_PRESSED_CALLBACK_SLIDER(ambient_emitter,OcclusionAmbientEmitter)
		RENDER_PRESSED_CALLBACK_SLIDER(ambient_receiver,OcclusionAmbientReceiver)
		RENDER_PRESSED_CALLBACK_SLIDER(ambient_scale,OcclusionAmbientScale)
		RENDER_PRESSED_CALLBACK_SLIDER(ambient_power,OcclusionAmbientPower)
		
		/*
		 */
		RENDER_PRESSED_CALLBACK_SLIDER(light_screen_radius,OcclusionLightScreenRadius)
		RENDER_PRESSED_CALLBACK_SLIDER(light_sample_radius,OcclusionLightSampleRadius)
		RENDER_PRESSED_CALLBACK_SLIDER(light_perspective,OcclusionLightPerspective)
		RENDER_PRESSED_CALLBACK_SLIDER(light_attenuation,OcclusionLightAttenuation)
		RENDER_PRESSED_CALLBACK_SLIDER(light_threshold,OcclusionLightThreshold)
		RENDER_PRESSED_CALLBACK_SLIDER(light_emitter,OcclusionLightEmitter)
		RENDER_PRESSED_CALLBACK_SLIDER(light_receiver,OcclusionLightReceiver)
		RENDER_PRESSED_CALLBACK_SLIDER(light_scale,OcclusionLightScale)
		RENDER_PRESSED_CALLBACK_SLIDER(light_power,OcclusionLightPower)
		
	} /* namespace Occlusion */
	
	/**************************************************************************\
	*
	* Scattering
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Scattering {
		
		/*
		 */
		WidgetSprite sun_color_s[2];		// sun color
		WidgetSprite ray_color_s[2];		// ray color
		WidgetSprite mie_color_s[2];		// mie color
		WidgetEditLine inner_angle_el;		// inner angle
		WidgetEditLine outer_angle_el;		// outer angle
		WidgetEditLine refractive_el;		// refractive
		WidgetEditLine density_el;			// density
		WidgetEditLine depolarization_el;	// depolarization
		WidgetEditLine turbidity_el;		// turbidity
		WidgetEditLine direction_el[3];		// direction
		WidgetEditLine ground_el;			// ground
		WidgetEditLine areal_el;			// areal
		WidgetEditLine ray_height_el;		// ray height
		WidgetEditLine mie_height_el;		// ray height
		WidgetEditLine angularity_el;		// angularity
		WidgetEditLine threshold_el;		// threshold
		WidgetEditLine greenstein_el;		// greenstein
		WidgetEditLine energy_el;			// energy
		WidgetEditLine power_el;			// power
		WidgetEditLine sun_el;				// sun
		WidgetEditLine ray_el;				// ray
		WidgetEditLine mie_el;				// mie
		
		DECLARE_SLIDER_CALLBACKS
		
		/*
		 */
		void update() {
			
			setSpriteColor(sun_color_s,engine.render.getScatteringSunColor());
			setSpriteColor(ray_color_s,engine.render.getScatteringRayColor());
			setSpriteColor(mie_color_s,engine.render.getScatteringMieColor());
			inner_angle_el.setText(editorFormat(engine.render.getScatteringInnerAngle()));
			outer_angle_el.setText(editorFormat(engine.render.getScatteringOuterAngle()));
			refractive_el.setText(string(engine.render.getScatteringRefractive()));
			density_el.setText(string(engine.render.getScatteringDensity()));
			depolarization_el.setText(string(engine.render.getScatteringDepolarization()));
			turbidity_el.setText(string(engine.render.getScatteringTurbidity()));
			setEditLineVec3(direction_el,engine.render.getScatteringDirection());
			ground_el.setText(string(engine.render.getScatteringGround()));
			areal_el.setText(string(engine.render.getScatteringAreal()));
			ray_height_el.setText(string(engine.render.getScatteringRayHeight()));
			mie_height_el.setText(string(engine.render.getScatteringMieHeight()));
			greenstein_el.setText(string(engine.render.getScatteringGreenstein()));
			angularity_el.setText(string(engine.render.getScatteringAngularity()));
			threshold_el.setText(string(engine.render.getScatteringThreshold()));
			energy_el.setText(editorFormat(engine.render.getScatteringEnergy()));
			power_el.setText(editorFormat(engine.render.getScatteringPower()));
			sun_el.setText(editorFormat(engine.render.getScatteringSun()));
			ray_el.setText(editorFormat(engine.render.getScatteringRay()));
			mie_el.setText(editorFormat(engine.render.getScatteringMie()));
		}
		
		/*
		 */
		void update_dynamic() {
			setEnabled(engine.render.getScattering(),(
				sun_color_s[0],
				sun_color_s[1],
				ray_color_s[0],
				ray_color_s[1],
				mie_color_s[0],
				mie_color_s[1],
				inner_angle_el,
				outer_angle_el,
				refractive_el,
				density_el,
				depolarization_el,
				turbidity_el,
				ground_el,
				areal_el,
				ray_height_el,
				mie_height_el,
				greenstein_el,
				angularity_el,
				threshold_el,
				energy_el,
				power_el,
				sun_el,
				ray_el,
				mie_el,
			));
		}
		
		/*
		 */
		RENDER_CALLBACK_COLOR(sun_color,ScatteringSunColor,TR("Select sun color"),"Scattering")
		RENDER_CALLBACK_COLOR(ray_color,ScatteringRayColor,TR("Select ray color"),"Scattering")
		RENDER_CALLBACK_COLOR(mie_color,ScatteringMieColor,TR("Select mie color"),"Scattering")
		RENDER_PRESSED_CALLBACK_STRING(inner_angle,ScatteringInnerAngle)
		RENDER_PRESSED_CALLBACK_STRING(outer_angle,ScatteringOuterAngle)
		RENDER_PRESSED_CALLBACK_STRING(refractive,ScatteringRefractive)
		RENDER_PRESSED_CALLBACK_STRING(density,ScatteringDensity)
		RENDER_PRESSED_CALLBACK_STRING(depolarization,ScatteringDepolarization)
		RENDER_PRESSED_CALLBACK_STRING(turbidity,ScatteringTurbidity)
		RENDER_PRESSED_CALLBACK_VEC3(direction,ScatteringDirection)
		RENDER_PRESSED_CALLBACK_STRING(ground,ScatteringGround)
		RENDER_PRESSED_CALLBACK_STRING(areal,ScatteringAreal)
		RENDER_PRESSED_CALLBACK_STRING(ray_height,ScatteringRayHeight)
		RENDER_PRESSED_CALLBACK_STRING(mie_height,ScatteringMieHeight)
		RENDER_PRESSED_CALLBACK_SLIDER(greenstein,ScatteringGreenstein)
		RENDER_PRESSED_CALLBACK_STRING(angularity,ScatteringAngularity)
		RENDER_PRESSED_CALLBACK_STRING(threshold,ScatteringThreshold)
		RENDER_PRESSED_CALLBACK_SLIDER(energy,ScatteringEnergy)
		RENDER_PRESSED_CALLBACK_SLIDER(power,ScatteringPower)
		RENDER_PRESSED_CALLBACK_SLIDER(sun,ScatteringSun)
		RENDER_PRESSED_CALLBACK_SLIDER(ray,ScatteringRay)
		RENDER_PRESSED_CALLBACK_SLIDER(mie,ScatteringMie)
		
		/*
		 */
		void earth_clicked() {
			
			int params[] = (
				"SunColor"			:		vec4(148.0f,100.0f,26.0f,255.0f) / 255.0f,
				"RayColor"			:		vec4(89.0f,164.0f,227.0f,255.0f) / 255.0f,
				"MieColor"			:		vec4(230.0f,167.0f,76.0f,255.0f) / 255.0f,
				"InnerAngle"		:		0.7f,
				"OuterAngle"		:		2.0f,
				"Refractive"		:		1.003f,
				"Depolarization"	:		0.035f,
				"Turbidity"			:		2.0f,
				"Direction"			:		vec3(0.0f,0.0f,1.0f),
				"Ground"			:		0.0f,
				"Areal"				:		20.0f,
				"RayHeight"			:		10000.0f,
				"MieHeight"			:		5000.0f,
				"Angularity"		:		1.0f,
				"Threshold"			:		1.0f,
			);
			
			foreachkey(string func; params) {
				if(call("engine.render.getScattering" + func) == params[func]) continue;
				save_state(create_state());
				break;
			}
			
			foreachkey(string func; params) {
				call("engine.render.setScattering" + func,params[func]);
			}
			
			update();
		}
		
	} /* namespace Scattering */
	
	/**************************************************************************\
	*
	* Postprocess
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Postprocess {
		
		/*
		 */
		WidgetEditLine refraction_dispersion_x_el;			// dispersion x
		WidgetEditLine refraction_dispersion_y_el;			// dispersion y
		WidgetEditLine refraction_dispersion_z_el;			// dispersion z
		
		WidgetEditLine motion_blur_linear_scale_el;			// linear scale
		WidgetEditLine motion_blur_angular_scale_el;		// angular scale
		WidgetEditLine motion_blur_velocity_scale_el;		// velocity scale
		WidgetEditLine motion_blur_max_velocity_el;			// velocity threshold
		
		WidgetEditLine volumetric_exposure_el;				// volumetric exposure
		WidgetEditLine volumetric_length_el;				// volumetric length
		WidgetEditLine volumetric_attenuation_el;			// volumetric attenuation
		
		WidgetEditLine glow_threshold_el;					// glow threshold
		WidgetEditLine glow_small_exposure_el;				// glow small exposure
		WidgetEditLine glow_medium_exposure_el;				// glow medium exposure
		WidgetEditLine glow_large_exposure_el;				// glow large exposure
		
		DECLARE_SLIDER_CALLBACKS
		
		/*
		 */
		void update() {
			
			refraction_dispersion_x_el.setText(editorFormat(engine.render.getRefractionDispersion().x));
			refraction_dispersion_y_el.setText(editorFormat(engine.render.getRefractionDispersion().y));
			refraction_dispersion_z_el.setText(editorFormat(engine.render.getRefractionDispersion().z));
			
			motion_blur_linear_scale_el.setText(editorFormat(engine.render.getMotionBlurLinearScale()));
			motion_blur_angular_scale_el.setText(editorFormat(engine.render.getMotionBlurAngularScale()));
			motion_blur_velocity_scale_el.setText(editorFormat(engine.render.getMotionBlurVelocityScale()));
			motion_blur_max_velocity_el.setText(editorFormat(engine.render.getMotionBlurMaxVelocity()));
			
			volumetric_exposure_el.setText(editorFormat(engine.render.getVolumetricExposure()));
			volumetric_length_el.setText(editorFormat(engine.render.getVolumetricLength()));
			volumetric_attenuation_el.setText(editorFormat(engine.render.getVolumetricAttenuation()));
			
			glow_threshold_el.setText(editorFormat(engine.render.getGlowThreshold()));
			glow_small_exposure_el.setText(editorFormat(engine.render.getGlowSmallExposure()));
			glow_medium_exposure_el.setText(editorFormat(engine.render.getGlowMediumExposure()));
			glow_large_exposure_el.setText(editorFormat(engine.render.getGlowLargeExposure()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			setEnabled(engine.render.getRefraction(),(
				refraction_dispersion_x_el,
				refraction_dispersion_y_el,
				refraction_dispersion_z_el,
			));
			
			setEnabled(engine.render.getMotionBlur(),(
				motion_blur_linear_scale_el,
				motion_blur_angular_scale_el,
				motion_blur_velocity_scale_el,
				motion_blur_max_velocity_el,
			));
			
			setEnabled(engine.render.getVolumetric(),(
				volumetric_exposure_el,
				volumetric_length_el,
				volumetric_attenuation_el,
			));
			
			setEnabled(engine.render.getGlow(),(
				glow_threshold_el,
				glow_small_exposure_el,
				glow_medium_exposure_el,
				glow_large_exposure_el,
			));
		}
		
		/*
		 */
		RENDER_PRESSED_CALLBACK_SLIDER_SWIZZLE(refraction_dispersion_x,RefractionDispersion,x)
		RENDER_PRESSED_CALLBACK_SLIDER_SWIZZLE(refraction_dispersion_y,RefractionDispersion,y)
		RENDER_PRESSED_CALLBACK_SLIDER_SWIZZLE(refraction_dispersion_z,RefractionDispersion,z)
		
		RENDER_PRESSED_CALLBACK_SLIDER(motion_blur_linear_scale,MotionBlurLinearScale)
		RENDER_PRESSED_CALLBACK_SLIDER(motion_blur_angular_scale,MotionBlurAngularScale)
		RENDER_PRESSED_CALLBACK_SLIDER(motion_blur_velocity_scale,MotionBlurVelocityScale)
		RENDER_PRESSED_CALLBACK_SLIDER(motion_blur_max_velocity,MotionBlurMaxVelocity)
		
		RENDER_PRESSED_CALLBACK_SLIDER(volumetric_exposure,VolumetricExposure)
		RENDER_PRESSED_CALLBACK_SLIDER(volumetric_length,VolumetricLength)
		RENDER_PRESSED_CALLBACK_SLIDER(volumetric_attenuation,VolumetricAttenuation)
		
		RENDER_PRESSED_CALLBACK_SLIDER(glow_threshold,GlowThreshold)
		RENDER_PRESSED_CALLBACK_SLIDER(glow_small_exposure,GlowSmallExposure)
		RENDER_PRESSED_CALLBACK_SLIDER(glow_medium_exposure,GlowMediumExposure)
		RENDER_PRESSED_CALLBACK_SLIDER(glow_large_exposure,GlowLargeExposure)
		
	} /* namespace Postprocess */
	
	/**************************************************************************\
	*
	* DOF
	*
	\**************************************************************************/
	
	/*
	 */
	namespace DOF {
		
		/*
		 */
		WidgetEditLine focal_distance_el;		// focal distance
		
		WidgetEditLine far_blur_range_el;		// far blur range
		WidgetEditLine far_blur_radius_el;		// far blur radius
		WidgetEditLine far_blur_power_el;		// far blur power
		WidgetEditLine far_focal_range_el;		// far focal range
		WidgetEditLine far_focal_scale_el;		// far focal scale
		WidgetEditLine far_focal_power_el;		// far focal power
		
		WidgetEditLine near_blur_range_el;		// near blur range
		WidgetEditLine near_blur_radius_el;		// near blur radius
		WidgetEditLine near_blur_power_el;		// near blur power
		WidgetEditLine near_focal_range_el;		// near focal range
		WidgetEditLine near_focal_scale_el;		// near focal scale
		WidgetEditLine near_focal_power_el;		// near focal power
		
		DECLARE_SLIDER_CALLBACKS
		
		/*
		 */
		void update() {
			
			focal_distance_el.setText(editorFormat(engine.render.getDOFFocalDistance()));
			
			far_blur_range_el.setText(editorFormat(engine.render.getDOFFarBlurRange()));
			far_blur_radius_el.setText(editorFormat(engine.render.getDOFFarBlurRadius()));
			far_blur_power_el.setText(editorFormat(engine.render.getDOFFarBlurPower()));
			far_focal_range_el.setText(editorFormat(engine.render.getDOFFarFocalRange()));
			far_focal_scale_el.setText(editorFormat(engine.render.getDOFFarFocalScale()));
			far_focal_power_el.setText(editorFormat(engine.render.getDOFFarFocalPower()));
			
			near_blur_range_el.setText(editorFormat(engine.render.getDOFNearBlurRange()));
			near_blur_radius_el.setText(editorFormat(engine.render.getDOFNearBlurRadius()));
			near_blur_power_el.setText(editorFormat(engine.render.getDOFNearBlurPower()));
			near_focal_range_el.setText(editorFormat(engine.render.getDOFNearFocalRange()));
			near_focal_scale_el.setText(editorFormat(engine.render.getDOFNearFocalScale()));
			near_focal_power_el.setText(editorFormat(engine.render.getDOFNearFocalPower()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			setEnabled(engine.render.getDOF(),(
				focal_distance_el,
				far_blur_range_el,
				far_blur_radius_el,
				far_blur_power_el,
				far_focal_range_el,
				far_focal_scale_el,
				far_focal_power_el,
				near_blur_range_el,
				near_blur_radius_el,
				near_blur_power_el,
				near_focal_range_el,
				near_focal_scale_el,
				near_focal_power_el,
			));
		}
		
		/*
		 */
		RENDER_PRESSED_CALLBACK_SLIDER(focal_distance,DOFFocalDistance)
		RENDER_PRESSED_CALLBACK_SLIDER(far_blur_range,DOFFarBlurRange)
		RENDER_PRESSED_CALLBACK_SLIDER(far_blur_radius,DOFFarBlurRadius)
		RENDER_PRESSED_CALLBACK_SLIDER(far_blur_power,DOFFarBlurPower)
		RENDER_PRESSED_CALLBACK_SLIDER(far_focal_range,DOFFarFocalRange)
		RENDER_PRESSED_CALLBACK_SLIDER(far_focal_scale,DOFFarFocalScale)
		RENDER_PRESSED_CALLBACK_SLIDER(far_focal_power,DOFFarFocalPower)
		RENDER_PRESSED_CALLBACK_SLIDER(near_blur_range,DOFNearBlurRange)
		RENDER_PRESSED_CALLBACK_SLIDER(near_blur_radius,DOFNearBlurRadius)
		RENDER_PRESSED_CALLBACK_SLIDER(near_blur_power,DOFNearBlurPower)
		RENDER_PRESSED_CALLBACK_SLIDER(near_focal_range,DOFNearFocalRange)
		RENDER_PRESSED_CALLBACK_SLIDER(near_focal_scale,DOFNearFocalScale)
		RENDER_PRESSED_CALLBACK_SLIDER(near_focal_power,DOFNearFocalPower)
		
	} /* namespace DOF */
	
	/**************************************************************************\
	*
	* HDR
	*
	\**************************************************************************/
	
	/*
	 */
	namespace HDR {
		
		/*
		 */
		WidgetTabBox tabbox;						// hdr tabbox
		
		WidgetEditLine exposure_el;					// exposure
		WidgetEditLine adaptation_el;				// adaptation
		WidgetEditLine min_luminance_el;			// min luminance
		WidgetEditLine max_luminance_el;			// max luminance
		WidgetEditLine threshold_el;				// threshold
		WidgetEditLine small_exposure_el;			// small exposure
		WidgetEditLine medium_exposure_el;			// medium exposure
		WidgetEditLine large_exposure_el;			// large exposure
		WidgetEditLine bright_exposure_el;			// bright exposure
		
		WidgetEditLine filmic_shoulder_scale_el;	// shoulder scale
		WidgetEditLine filmic_linear_scale_el;		// linear scale
		WidgetEditLine filmic_linear_angle_el;		// linear angle
		WidgetEditLine filmic_toe_scale_el;			// toe scale
		WidgetEditLine filmic_toe_numerator_el;		// toe numerator
		WidgetEditLine filmic_toe_denominator_el;	// toe denominator
		WidgetEditLine filmic_white_level_el;		// white level
		
		WidgetCheckBox cross_enabled_cb;			// cross enabled
		WidgetSprite cross_color_s[2];				// cross color
		WidgetEditLine cross_scale_el;				// cross color scale
		WidgetEditLine cross_shafts_el;				// cross shafts
		WidgetEditLine cross_length_el;				// cross length
		WidgetEditLine cross_angle_el;				// cross angle
		WidgetEditLine cross_threshold_el;			// cross threshold
		
		WidgetCheckBox bokeh_enabled_cb;			// bokeh enabled
		WidgetSprite bokeh_color_s[2];				// bokeh color value
		WidgetEditLine bokeh_scale_el;				// bokeh color scale
		WidgetEditLine bokeh_far_size_el;			// bokeh far size scale
		WidgetEditLine bokeh_far_power_el;			// bokeh far power scale
		WidgetEditLine bokeh_near_size_el;			// bokeh near size scale
		WidgetEditLine bokeh_near_power_el;			// bokeh near power scale
		WidgetEditLine bokeh_threshold_el;			// bokeh threshold
		WidgetEditLine bokeh_texture_el;			// bokeh texture name
		
		WidgetCheckBox shaft_enabled_cb;			// shaft enabled
		WidgetSprite shaft_color_s[2];				// shaft color value
		WidgetEditLine shaft_scale_el;				// shaft color scale
		WidgetEditLine shaft_length_el;				// shaft length
		WidgetEditLine shaft_attenuation_el;		// shaft attenuation
		WidgetEditLine shaft_threshold_el;			// shaft threshold
		
		WidgetCheckBox lens_enabled_cb;				// lens enabled
		WidgetSprite lens_color_s[2];				// lens color value
		WidgetEditLine lens_scale_el;				// lens color scale
		WidgetEditLine lens_length_el;				// lens length
		WidgetEditLine lens_radius_el;				// lens radius
		WidgetEditLine lens_threshold_el;			// lens threshold
		WidgetEditLine lens_dispersion_x_el;		// lens dispersion x
		WidgetEditLine lens_dispersion_y_el;		// lens dispersion y
		WidgetEditLine lens_dispersion_z_el;		// lens dispersion z
		WidgetEditLine lens_texture_el;				// lens texture name
		
		DECLARE_SLIDER_CALLBACKS
		
		void set_tabbox_current_tab(int tab) { tabbox.setCurrentTab(tab); }
		int get_tabbox_current_tab() { return tabbox.getCurrentTab(); }
		
		/*
		 */
		void update() {
			
			setCallbackEnabled(GUI_CLICKED,0,(
				cross_enabled_cb,
				bokeh_enabled_cb,
				shaft_enabled_cb,
				lens_enabled_cb,
			));
			
			exposure_el.setText(editorFormat(engine.render.getHDRExposure()));
			adaptation_el.setText(editorFormat(engine.render.getHDRAdaptation()));
			min_luminance_el.setText(editorFormat(engine.render.getHDRMinLuminance()));
			max_luminance_el.setText(editorFormat(engine.render.getHDRMaxLuminance()));
			threshold_el.setText(editorFormat(engine.render.getHDRThreshold()));
			small_exposure_el.setText(editorFormat(engine.render.getHDRSmallExposure()));
			medium_exposure_el.setText(editorFormat(engine.render.getHDRMediumExposure()));
			large_exposure_el.setText(editorFormat(engine.render.getHDRLargeExposure()));
			bright_exposure_el.setText(editorFormat(engine.render.getHDRBrightExposure()));
			
			filmic_shoulder_scale_el.setText(editorFormat(engine.render.getHDRFilmicShoulderScale()));
			filmic_linear_scale_el.setText(editorFormat(engine.render.getHDRFilmicLinearScale()));
			filmic_linear_angle_el.setText(editorFormat(engine.render.getHDRFilmicLinearAngle()));
			filmic_toe_scale_el.setText(editorFormat(engine.render.getHDRFilmicToeScale()));
			filmic_toe_numerator_el.setText(editorFormat(engine.render.getHDRFilmicToeNumerator()));
			filmic_toe_denominator_el.setText(editorFormat(engine.render.getHDRFilmicToeDenominator()));
			filmic_white_level_el.setText(editorFormat(engine.render.getHDRFilmicWhiteLevel()));
			
			cross_enabled_cb.setChecked(engine.render.isHDRCrossEnabled());
			setSpriteColor(cross_color_s,engine.render.getHDRCrossColor());
			cross_scale_el.setText(editorFormat(engine.render.getHDRCrossScale()));
			cross_shafts_el.setText(string(int(engine.render.getHDRCrossShafts())));
			cross_length_el.setText(editorFormat(engine.render.getHDRCrossLength()));
			cross_angle_el.setText(editorFormat(engine.render.getHDRCrossAngle()));
			cross_threshold_el.setText(editorFormat(engine.render.getHDRCrossThreshold()));
			
			bokeh_enabled_cb.setChecked(engine.render.isHDRBokehEnabled());
			setSpriteColor(bokeh_color_s,engine.render.getHDRBokehColor());
			bokeh_scale_el.setText(editorFormat(engine.render.getHDRBokehScale()));
			bokeh_far_size_el.setText(editorFormat(engine.render.getHDRBokehFarSize()));
			bokeh_far_power_el.setText(editorFormat(engine.render.getHDRBokehFarPower()));
			bokeh_near_size_el.setText(editorFormat(engine.render.getHDRBokehNearSize()));
			bokeh_near_power_el.setText(editorFormat(engine.render.getHDRBokehNearPower()));
			bokeh_threshold_el.setText(editorFormat(engine.render.getHDRBokehThreshold()));
			bokeh_texture_el.setText(engine.render.getHDRBokehTextureName());
			
			shaft_enabled_cb.setChecked(engine.render.isHDRShaftEnabled());
			setSpriteColor(shaft_color_s,engine.render.getHDRShaftColor());
			shaft_scale_el.setText(editorFormat(engine.render.getHDRShaftScale()));
			shaft_length_el.setText(editorFormat(engine.render.getHDRShaftLength()));
			shaft_attenuation_el.setText(editorFormat(engine.render.getHDRShaftAttenuation()));
			shaft_threshold_el.setText(editorFormat(engine.render.getHDRShaftThreshold()));
			
			lens_enabled_cb.setChecked(engine.render.isHDRLensEnabled());
			setSpriteColor(lens_color_s,engine.render.getHDRLensColor());
			lens_scale_el.setText(editorFormat(engine.render.getHDRLensScale()));
			lens_length_el.setText(editorFormat(engine.render.getHDRLensLength()));
			lens_radius_el.setText(editorFormat(engine.render.getHDRLensRadius()));
			lens_threshold_el.setText(editorFormat(engine.render.getHDRLensThreshold()));
			lens_dispersion_x_el.setText(editorFormat(engine.render.getHDRLensDispersion().x));
			lens_dispersion_y_el.setText(editorFormat(engine.render.getHDRLensDispersion().y));
			lens_dispersion_z_el.setText(editorFormat(engine.render.getHDRLensDispersion().z));
			lens_texture_el.setText(engine.render.getHDRLensTextureName());
			
			setCallbackEnabled(GUI_CLICKED,1,(
				cross_enabled_cb,
				bokeh_enabled_cb,
				shaft_enabled_cb,
				lens_enabled_cb,
			));
		}
		
		/*
		 */
		void update_dynamic() {
			
			setEnabled(engine.render.getHDR(),(
				exposure_el,
				adaptation_el,
				min_luminance_el,
				max_luminance_el,
				threshold_el,
				small_exposure_el,
				medium_exposure_el,
				large_exposure_el,
				bright_exposure_el,
				cross_enabled_cb,
				bokeh_enabled_cb,
				shaft_enabled_cb,
				lens_enabled_cb,
			));
			
			setEnabled(engine.render.getHDR() && engine.console.getInt("render_use_filmic"),(
				filmic_shoulder_scale_el,
				filmic_linear_scale_el,
				filmic_linear_angle_el,
				filmic_toe_scale_el,
				filmic_toe_numerator_el,
				filmic_toe_denominator_el,
				filmic_white_level_el,
			));
			
			setEnabled(engine.render.getHDR() && cross_enabled_cb.isChecked(),(
				cross_color_s[0],
				cross_color_s[1],
				cross_scale_el,
				cross_shafts_el,
				cross_length_el,
				cross_angle_el,
				cross_threshold_el,
			));
			
			setEnabled(engine.render.getHDR() && bokeh_enabled_cb.isChecked() && engine.render.getDOF(),(
				bokeh_color_s[0],
				bokeh_color_s[1],
				bokeh_scale_el,
				bokeh_far_size_el,
				bokeh_far_power_el,
				bokeh_near_size_el,
				bokeh_near_power_el,
				bokeh_threshold_el,
				bokeh_texture_el,
			));
			
			setEnabled(engine.render.getHDR() && shaft_enabled_cb.isChecked(),(
				shaft_color_s[0],
				shaft_color_s[1],
				shaft_scale_el,
				shaft_length_el,
				shaft_attenuation_el,
				shaft_threshold_el,
			));
			
			setEnabled(engine.render.getHDR() && lens_enabled_cb.isChecked(),(
				lens_color_s[0],
				lens_color_s[1],
				lens_scale_el,
				lens_length_el,
				lens_radius_el,
				lens_threshold_el,
				lens_dispersion_x_el,
				lens_dispersion_y_el,
				lens_dispersion_z_el,
				lens_texture_el,
			));
		}
		
		/*
		 */
		RENDER_PRESSED_CALLBACK_SLIDER(exposure,HDRExposure)
		RENDER_PRESSED_CALLBACK_SLIDER(adaptation,HDRAdaptation)
		RENDER_PRESSED_CALLBACK_SLIDER_2(min_luminance,HDRMinLuminance,max_luminance,HDRMaxLuminance)
		RENDER_PRESSED_CALLBACK_SLIDER_2(max_luminance,HDRMaxLuminance,min_luminance,HDRMinLuminance)
		RENDER_PRESSED_CALLBACK_SLIDER(threshold,HDRThreshold)
		RENDER_PRESSED_CALLBACK_SLIDER(small_exposure,HDRSmallExposure)
		RENDER_PRESSED_CALLBACK_SLIDER(medium_exposure,HDRMediumExposure)
		RENDER_PRESSED_CALLBACK_SLIDER(large_exposure,HDRLargeExposure)
		RENDER_PRESSED_CALLBACK_SLIDER(bright_exposure,HDRBrightExposure)
		
		RENDER_PRESSED_CALLBACK_SLIDER(filmic_shoulder_scale,HDRFilmicShoulderScale)
		RENDER_PRESSED_CALLBACK_SLIDER(filmic_linear_scale,HDRFilmicLinearScale)
		RENDER_PRESSED_CALLBACK_SLIDER(filmic_linear_angle,HDRFilmicLinearAngle)
		RENDER_PRESSED_CALLBACK_SLIDER(filmic_toe_scale,HDRFilmicToeScale)
		RENDER_PRESSED_CALLBACK_SLIDER(filmic_toe_numerator,HDRFilmicToeNumerator)
		RENDER_PRESSED_CALLBACK_SLIDER(filmic_toe_denominator,HDRFilmicToeDenominator)
		RENDER_PRESSED_CALLBACK_SLIDER(filmic_white_level,HDRFilmicWhiteLevel)
		
		RENDER_CLEAR_CLICKED_CALLBACK(filmic_shoulder_scale,HDRFilmicShoulderScale,0.2f)
		RENDER_CLEAR_CLICKED_CALLBACK(filmic_linear_scale,HDRFilmicLinearScale,0.3f)
		RENDER_CLEAR_CLICKED_CALLBACK(filmic_linear_angle,HDRFilmicLinearAngle,0.1f)
		RENDER_CLEAR_CLICKED_CALLBACK(filmic_toe_scale,HDRFilmicToeScale,0.2f)
		RENDER_CLEAR_CLICKED_CALLBACK(filmic_toe_numerator,HDRFilmicToeNumerator,0.01f)
		RENDER_CLEAR_CLICKED_CALLBACK(filmic_toe_denominator,HDRFilmicToeDenominator,0.3f)
		RENDER_CLEAR_CLICKED_CALLBACK(filmic_white_level,HDRFilmicWhiteLevel,1.0f)
		
		RENDER_CALLBACK_COLOR(cross_color,HDRCrossColor,TR("Select HDR cross color"),"HDR")
		RENDER_PRESSED_CALLBACK_SLIDER(cross_scale,HDRCrossScale)
		RENDER_PRESSED_CALLBACK_SLIDER(cross_length,HDRCrossLength)
		RENDER_PRESSED_CALLBACK_SLIDER(cross_angle,HDRCrossAngle)
		RENDER_PRESSED_CALLBACK_SLIDER(cross_threshold,HDRCrossThreshold)
		
		RENDER_CALLBACK_COLOR(bokeh_color,HDRBokehColor,TR("Select HDR bokeh color"),"HDR")
		RENDER_PRESSED_CALLBACK_SLIDER(bokeh_scale,HDRBokehScale)
		RENDER_PRESSED_CALLBACK_SLIDER(bokeh_far_size,HDRBokehFarSize)
		RENDER_PRESSED_CALLBACK_SLIDER(bokeh_far_power,HDRBokehFarPower)
		RENDER_PRESSED_CALLBACK_SLIDER(bokeh_near_size,HDRBokehNearSize)
		RENDER_PRESSED_CALLBACK_SLIDER(bokeh_near_power,HDRBokehNearPower)
		RENDER_PRESSED_CALLBACK_SLIDER(bokeh_threshold,HDRBokehThreshold)
		RENDER_CALLBACK_TEXTURE(bokeh_texture,HDRBokehTextureName,TR("Select render HDR bokeh iris texture"),TR("Render HDR bokeh iris texture"),TR("Clear render HDR bokeh iris texture to default?"))
		
		RENDER_CALLBACK_COLOR(shaft_color,HDRShaftColor,TR("Select HDR shaft color"),"HDR")
		RENDER_PRESSED_CALLBACK_SLIDER(shaft_scale,HDRShaftScale)
		RENDER_PRESSED_CALLBACK_SLIDER(shaft_length,HDRShaftLength)
		RENDER_PRESSED_CALLBACK_SLIDER(shaft_attenuation,HDRShaftAttenuation)
		RENDER_PRESSED_CALLBACK_SLIDER(shaft_threshold,HDRShaftThreshold)
		
		RENDER_CALLBACK_COLOR(lens_color,HDRLensColor,TR("Select HDR lens color"),"HDR")
		RENDER_PRESSED_CALLBACK_SLIDER(lens_scale,HDRLensScale)
		RENDER_PRESSED_CALLBACK_SLIDER(lens_length,HDRLensLength)
		RENDER_PRESSED_CALLBACK_SLIDER(lens_radius,HDRLensRadius)
		RENDER_PRESSED_CALLBACK_SLIDER(lens_threshold,HDRLensThreshold)
		RENDER_PRESSED_CALLBACK_SLIDER_SWIZZLE(lens_dispersion_x,HDRLensDispersion,x)
		RENDER_PRESSED_CALLBACK_SLIDER_SWIZZLE(lens_dispersion_y,HDRLensDispersion,y)
		RENDER_PRESSED_CALLBACK_SLIDER_SWIZZLE(lens_dispersion_z,HDRLensDispersion,z)
		RENDER_CALLBACK_TEXTURE(lens_texture,HDRLensTextureName,TR("Select render HDR lens dirt texture"),TR("Render HDR lens dirt texture"),TR("Clear render HDR lens dirt texture to default?"))
		
		/*
		 */
		void cross_enabled_clicked() {
			save_state(create_state());
			engine.render.setHDRCrossEnabled(cross_enabled_cb.isChecked());
		}
		
		void bokeh_enabled_clicked() {
			save_state(create_state());
			engine.render.setHDRBokehEnabled(bokeh_enabled_cb.isChecked());
		}
		
		void cross_shafts_pressed() {
			Unredo::State s = NULL;
			if(skip_state == false && engine.render.getHDRCrossShafts() != int(cross_shafts_el.getText())) s = create_state();
			engine.render.setHDRCrossShafts(int(cross_shafts_el.getText()));
			cross_shafts_el.setText(string(engine.render.getHDRCrossShafts()));
			save_state(s);
		}
		
		void shaft_enabled_clicked() {
			save_state(create_state());
			engine.render.setHDRShaftEnabled(shaft_enabled_cb.isChecked());
		}
		
		void lens_enabled_clicked() {
			save_state(create_state());
			engine.render.setHDRLensEnabled(lens_enabled_cb.isChecked());
		}
		
	} /* namespace HDR */
	
	/**************************************************************************\
	*
	* Color
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Color {
		
		/*
		 */
		WidgetEditLine brightness_el;		// brightness
		WidgetEditLine contrast_el;			// contrast
		WidgetEditLine saturation_el;		// saturation
		WidgetEditLine gamma_el;			// gamma
		WidgetSprite white_s[2];			// white
		
		WidgetEditLine texture_el;			// texture name
		
		DECLARE_SLIDER_CALLBACKS
		
		/*
		 */
		void update() {
			
			brightness_el.setText(string(int(floor(engine.render.getColorBrightness() * 127.0f + 0.5f))));
			contrast_el.setText(string(int(floor(engine.render.getColorContrast() * 127.0f + 0.5f))));
			saturation_el.setText(string(int(floor(engine.render.getColorSaturation() * 50.0f + 0.5f))));
			gamma_el.setText(string(int(floor(engine.render.getColorGamma() * 100.0f + 0.5f))));
			setSpriteColor(white_s,engine.render.getColorWhite());
			
			texture_el.setText(engine.render.getColorTextureName());
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		RENDER_PRESSED_CALLBACK_SLIDER_INT(brightness,ColorBrightness,127.0f)
		RENDER_PRESSED_CALLBACK_SLIDER_INT(contrast,ColorContrast,127.0f)
		RENDER_PRESSED_CALLBACK_SLIDER_INT(saturation,ColorSaturation,50.0f)
		RENDER_PRESSED_CALLBACK_SLIDER_INT(gamma,ColorGamma,100.0f)
		RENDER_CALLBACK_COLOR(white,ColorWhite,TR("Select white color"),"Color")
		
		/*
		 */
		RENDER_CALLBACK_TEXTURE(texture,ColorTextureName,TR("Select render color texture"),TR("Render color texture"),TR("Clear render color texture?"))
		
		/*
		 */
		void texture_create_clicked() {
			
			int size = 32;
			float isize = 1.0f / (size - 1);
			
			string name;
			if(dialogFileImageSave(TR("Select LUT texture"),name)) {
				name = setExtensionImage(name,"dds");
				Image image = new Image();
				image.create3D(size,size,size,IMAGE_FORMAT_RGB8);
				forloop(int z = 0; size) {
					forloop(int y = 0; size) {
						forloop(int x = 0; size) {
							image.set3D(x,y,z,vec4(vec3(x,y,z) * isize,1.0f));
						}
					}
				}
				image.convertToType(IMAGE_2D);
				image.save(name);
				delete image;
			}
		}
		
	} /* namespace Color */
	
	/**************************************************************************\
	*
	* Window
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		Common::update();
		Parameters::update();
		Occlusion::update();
		Scattering::update();
		Postprocess::update();
		DOF::update();
		HDR::update();
		Color::update();
		
		editorWindow(window);
		DECLARE_WINDOW_LOAD(render)
		
		tabbox.setCurrentTab(configGet("render_tab",0));
	}
	
	void shutdown() {
		
		DECLARE_WINDOW_SAVE(render)
		
		configSet("render_tab",tabbox.getCurrentTab());
	}
	
	/*
	 */
	void update(int need_reload) {
		
		// reload render
		if(need_reload) {
			Common::update();
			Parameters::update();
			Occlusion::update();
			Scattering::update();
			Postprocess::update();
			DOF::update();
			HDR::update();
			Color::update();
		}
		
		int tab = tabbox.getCurrentTab();
		
		// update tabs
		if(tab == 0) Common::update_dynamic();
		else if(tab == 1) Parameters::update_dynamic();
		else if(tab == 2) Occlusion::update_dynamic();
		else if(tab == 3) Scattering::update_dynamic();
		else if(tab == 4) Postprocess::update_dynamic();
		else if(tab == 5) DOF::update_dynamic();
		else if(tab == 6) HDR::update_dynamic();
		else if(tab == 7) Color::update_dynamic();
	}
	
	/*
	 */
	void load_clicked() {
		string name;
		Unredo::State s = create_state();
		if(dialogFileSettings(TR("Select rendering settings to load"),name,DIALOG_FILE_OPEN)) {
			if(engine.render.loadSettings(name) == 0) {
				dialogMessageOk(TR("Error"));
				delete s;
			} else {
				save_state(s);
				update(1);
			}
		}
	}
	
	void save_clicked() {
		string name;
		if(dialogFileSettings(TR("Select rendering settings to save"),name,DIALOG_FILE_SAVE)) {
			name = savePath(name);
			
			if(engine.render.saveSettings(name,1) == 0) {
				dialogMessageOk(TR("Error"));
			}
		}
	}
	
} /* namespace Render */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void renderInit() {
	Render::init();
}

void renderShutdown() {
	Render::shutdown();
}

/*
 */
void renderToggle() {
	if(Render::window_show) renderHide();
	else renderShow();
}

void renderShow() {
	DECLARE_WINDOW_SHOW(Render)
}

void renderHide() {
	DECLARE_WINDOW_HIDE(Render)
}

/*
 */
void renderUpdate(int need_reload) {
	Render::update(need_reload);
}
