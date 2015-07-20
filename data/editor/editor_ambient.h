/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_ambient.cpp
 * Desc:    Unigine editor
 * Version: 1.01
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Alexey Zagniy <yingaz@unigine.com>
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
namespace Ambient {
	
	/*
	 */
	int enabled;
	
	int passes[] = (
		"skip_deferred"				: 0,
		"skip_occlusion"			: 1,
		"skip_auxiliary"			: 1,
		"skip_reflection"			: 1,
		"skip_refraction"			: 1,
		"skip_scattering"			: 1,
		"skip_opacity_ambient"		: 0,
		"skip_opacity_light"		: 1,
		"skip_transparent_ambient"	: 0,
		"skip_transparent_light"	: 1,
		"skip_decal_ambient"		: 0,
		"skip_decal_light"			: 1,
		"skip_post_materials"		: 1,
		"force_no_shadows"			: 1,
	);
	
	vec4 old_color;
	float old_exposure;
	int old_passes[passes.size()];
	
	/*
	 */
	void enable() {
		if(enabled == 0) {
			old_color = engine.render.getAmbientColor();
			old_exposure = engine.render.getEnvironmentExposure();
			
			foreachkey(string pass, i = 0; passes; i++) {
				old_passes[i] = engine.console.getInt("render_" + pass);
				engine.console.setInt("render_" + pass,passes[pass]);
			}
		}
		
		engine.render.setEnvironmentExposure(0.0f);
		engine.render.setAmbientColor(vec4_one);
		setEnabled(0,Render::Common::ambient_color_s);
	}
	
	void disable() {
		if(enabled) {
			engine.render.setAmbientColor(old_color);
			engine.render.setEnvironmentExposure(old_exposure);
			foreachkey(string pass, i = 0; passes; i++) {
				engine.console.setInt("render_" + pass,old_passes[i]);
			}
		}
		setEnabled(1,Render::Common::ambient_color_s);
	}
	
	/*
	 */
	void init() {
		
		enabled = configGet("viewport_ambient_enabled",0);
		
		if(enabled) {
			enabled = 0;
			enable();
			enabled = 1;
		}
	}
	
	void shutdown() {
		
		configSet("viewport_ambient_enabled",enabled);
		
		if(enabled) {
			disable();
		}
	}
	
	/*
	 */
	void update(int need_reload) {
		if(need_reload && enabled) {
			enable();
		}
	}
	
	/*
	 */
	void toggle() {
		if(enabled) {
			disable();
			enabled = 0;
		} else {
			enable();
			enabled = 1;
		}
	}
	
	/*
	 */
	int isEnabled() {
		return enabled;
	}
	
	/*
	 */
	void clear() {
		if(enabled) {
			disable();
			enabled = 0;
		}
	}
	
	/*
	 */
	void save_world() {
		if(enabled) {
			engine.render.setAmbientColor(old_color);
			engine.console.run("world_save");
			wait;
			engine.render.setAmbientColor(vec4_one);
		} else {
			engine.console.run("world_save");
		}
	}
	
}	/* namespace Ambient */
	

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void ambientInit() {
	Ambient::init();
}

void ambientShutdown() {
	Ambient::shutdown();
}

/*
 */
void ambientUpdate(int need_reload) {
	Ambient::update(need_reload);
}

/*
 */
void ambientClear() {
	Ambient::clear();
}

/*
 */
void ambientToggle() {
	Ambient::toggle();
}

/*
 */
int ambientIsEnabled() {
	return Ambient::isEnabled();
}

/*
 */
void ambientSave() {
	thread(functionid(ambientRestore),Ambient::enabled);
	ambientClear();
}

void ambientRestore(int enable) {
	if(enable) {
		wait;
		ambientToggle();
	}
}
