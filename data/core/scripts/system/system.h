/* Copyright (C) 2005-2015, UNIGINE Corp. All rights reserved.
 *
 * This file is a part of the UNIGINE 2.0-RC SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE Corp. at http://unigine.com/
 */


#ifndef __UNIGINE_SYSTEM_H__
#define __UNIGINE_SYSTEM_H__

//#define MENU_USER
//#define MOUSE_USER
//#define MOUSE_SOFT
//#define EXPORT_LOCALE
//#define EXPORT_EXTERN
//#define RENDER_SIMPLE

#ifndef VIDEO_FULLSCREEN
	#define VIDEO_FULLSCREEN	1
#endif
#ifndef RENDER_OCCLUSION
	#define RENDER_OCCLUSION	1
#endif
#ifndef RENDER_HDR
	#define RENDER_HDR			1
#endif
#ifndef SHOW_GPU
	#define SHOW_GPU			1
#endif

#ifdef HAS_INTERFACE
	#undef VIDEO_FULLSCREEN
	#define VIDEO_FULLSCREEN	2
#endif

/*
 */
namespace System {
	
	/*
	 */
	string name;					// interface name
	string languages[0];			// language names
	string locales[0];				// locale names
	
	string language_callback;		// language callback
	string language_world_callback;	// language world callback
	string language_editor_callback;// language editor callback
	
	UserInterface user_interface;	// user interface
	
	WidgetWindow main_w;			// main window
	WidgetTabBox main_tb;			// main tabbox
	WidgetButton apply_b;			// apply button
	
	WidgetWindow event_w;			// event window
	
	WidgetDialog message_d;			// message dialog
	WidgetLabel message_l;			// message label
	
	WidgetComboBox language_cb;		// language combobox
	
	WidgetLabel gpu_l;				// gpu monitor
	
	#ifdef HAS_INTERFACE
		InterfaceWindow main_i;		// main window interface
		InterfaceWindow event_i;	// event window interface
		InterfaceWindow message_i;	// message dialog interface
	#endif
	
	string splash_4x3_name;			// 4x3 splash
	string splash_16x9_name;		// 16x9 splash
	int splash_threshold;			// splash threshold
	vec4 splash_transform;			// splash transform
	vec4 splash_color;				// splash color
	string splash_text;				// splash text
	
	int toggle;						// menu key toggle
	int enabled;					// system enabled flag
	int clicked;					// apply clicked flag
	int language;					// current language
	int editor;						// editor status
	
	/*
	 */
	string tab_names[] = (
		"Video",
		"Render",
		"Interface",
		"Controls",
	);
	
	string tab_prefix[] = (
		"System::Video::",
		"System::Render::",
		"System::Interface::",
		"System::Controls::",
	);
	
	int run_function(string name) {
		foreach(string tab, i = 0; tab_names; i++) {
			if(tab == main_tb.getCurrentTabText() || engine.gui.translate(tab) == main_tb.getCurrentTabText()) {
				return call(tab_prefix[i] + name);
			}
		}
		return 0;
	}
	
	/*
	 */
	int message_ret;
	
	void message_ok_clicked() {
		message_ret = 1;
		#ifdef HAS_INTERFACE
			message_i.setWidget(NULL);
			message_i.setHidden(1);
		#else
			engine.gui.removeChild(message_d);
		#endif
	}
	
	void message_cancel_clicked() {
		message_ret = -1;
		#ifdef HAS_INTERFACE
			message_i.setWidget(NULL);
			message_i.setHidden(1);
		#else
			engine.gui.removeChild(message_d);
		#endif
	}
	
	int message(string title,string message) {
		
		if(message_d == NULL) {
			log.error("System::message(): message dialog is NULL\n");
			log.message("%s\n%s\n",title,message);
			return;
		}
		
		message_d.setText(engine.gui.translate(title));
		message_l.setText(engine.gui.translate(message));
		
		WidgetButton ok_b = message_d.getOkButton();
		WidgetButton cancel_b = message_d.getCancelButton();
		ok_b.setCallback(GUI_CLICKED,"System::message_ok_clicked");
		cancel_b.setCallback(GUI_CLICKED,"System::message_cancel_clicked");
		
		#ifdef HAS_INTERFACE
			if(message_i == NULL) message_i = new InterfaceWindow(0);
			message_i.setWidget(message_d);
			message_i.setCenterPosition();
			message_i.setHidden(0);
		#else
			engine.gui.addChild(message_d,GUI_ALIGN_CENTER | GUI_ALIGN_OVERLAP);
			message_d.setPermanentFocus();
		#endif
		
		message_ret = 0;
		while(message_ret == 0 && message_d.isDone() == 0) wait 1;
		
		return (message_ret == 1);
	}
	
	/*
	 */
	void need_restart() {
		message("Info","You must restart Unigine to apply this settings");
	}
	
	/*
	 */
	void tab_changed() {
		run_function("update");
	}
	
	void apply_clicked() {
		run_function("apply");
		run_function("update");
		clicked = 1;
	}
	
	void quit_clicked() {
		if(message("Confirm","Quit from application?")) {
			engine.gui.removeChild(message_d);
			engine.console.run("quit");
		}
	}
	
	/*
	 */
	void update_language() {
		language_cb.clear();
		foreach(string language; languages) {
			language_cb.addItem(engine.gui.translate(language));
		}
		language_cb.setCurrentItem(language);
	}
	
	void set_language(int l) {
		#ifndef EXPORT_LOCALE
			if(language == l) return;
			language = l;
			if(locales.check(language)) {
				string tokens[2];
				engine.gui.clearDictionaries();
				if(strsplit(locales[language],":",tokens) == 2) engine.gui.addDictionary(tokens[0],tokens[1]);
				else engine.gui.addDictionary(locales[language]);
				user_interface.updateWidgets();
				foreach(string tab; tab_prefix) {
					call(tab + "init");
					call(tab + "update");
				}
				if(language_cb != NULL) update_language();
				if(language_callback != NULL) call(language_callback);
				if(language_world_callback != NULL && engine.world.isLoaded()) engine.world.call(language_world_callback);
				if(language_editor_callback != NULL && engine.editor.isLoaded()) engine.editor.call(language_editor_callback);
			}
		#endif
	}
	
	int get_language() {
		return language;
	}
	
	/*
	 */
	void update_splash() {
		
		float aspect = float(engine.app.getWidth()) / engine.app.getHeight();
		
		// system splash
		engine.splash.setSystem((aspect < 1.5f) ? splash_4x3_name : splash_16x9_name,splash_threshold);
		engine.splash.setSystemTransform(splash_transform);
		engine.splash.setSystemBackground(splash_color);
		engine.splash.setSystemText(splash_text);
		
		// world splash
		engine.splash.setWorld((aspect < 1.5f) ? splash_4x3_name : splash_16x9_name,splash_threshold);
		engine.splash.setWorldTransform(splash_transform);
		engine.splash.setWorldBackground(splash_color);
		engine.splash.setWorldText(splash_text);
	}
	
	/*
	 */
	void init(string name_,string languages_[],string locales_[]) {
		
		name = name_;
		languages.copy(languages_);
		locales.copy(locales_);
		toggle = APP_KEY_ESC;
		language = -1;
		
		// splash parameters
		splash_4x3_name = "core/gui/splash.png";
		splash_16x9_name = "core/gui/splash.png";
		splash_transform = vec4(0.5f,0.33f,0.5f,0.5f);
		splash_color = engine.gui.parseColor("0c0c0c");
		splash_text = "<p align=center><font color=#606060 size=16><xy y=%100 dy=-20/>UNIGINE Corp. (c) 2005-2015</font></p>";
		splash_threshold = 16;
		
		// export locale
		#ifdef EXPORT_LOCALE
			if(locales.size()) {
				string tokens[0];
				if(strsplit(locales[0],":",tokens) == 2) engine.gui.saveDictionary(tokens[0],tokens[1]);
				else engine.gui.saveDictionary(locales[0]);
			}
		#endif
		
		// export extern
		#ifdef EXPORT_EXTERN
			File file = new File();
			if(file.open("extern_info.h","wb")) {
				file.puts(get_extern_info(EXPORT_EXTERN));
				file.close();
			}
			delete file;
		#endif
		
		// initialize window
		user_interface = new UserInterface(engine.getGui(),name);
		#ifndef HAS_INTERFACE
			engine.gui.addChild(main_w,GUI_ALIGN_CENTER | GUI_ALIGN_OVERLAP);
			engine.gui.removeChild(main_w);
			engine.gui.removeChild(main_w);
		#endif
		
		// set language
		set_language(engine.config.getInt("system_language",0));
		
		// initialize tabs
		foreach(string tab; tab_prefix) {
			call(tab + "init");
			call(tab + "update");
		}
		
		// update language
		if(language_cb != NULL) update_language();
		
		// update splash
		update_splash();
		
		// load materials
		engine.materials.load("core/materials/default/unigine_post.mat");
		engine.materials.load("core/materials/default/unigine_render.mat");
		engine.materials.load("core/materials/default/unigine_decal.mat");
		engine.materials.load("core/materials/default/unigine_decal_terrain.mat");
		engine.materials.load("core/materials/default/unigine_decal_deferred.mat");
		engine.materials.load("core/materials/default/unigine_mesh.mat");
		engine.materials.load("core/materials/default/unigine_mesh_lut.mat");
		engine.materials.load("core/materials/default/unigine_mesh_paint.mat");
		engine.materials.load("core/materials/default/unigine_mesh_tessellation.mat");
		engine.materials.load("core/materials/default/unigine_mesh_tessellation_paint.mat");
		engine.materials.load("core/materials/default/unigine_mesh_triplanar.mat");
		engine.materials.load("core/materials/default/unigine_mesh_overlap.mat");
		engine.materials.load("core/materials/default/unigine_mesh_layer.mat");
		engine.materials.load("core/materials/default/unigine_mesh_noise.mat");
		engine.materials.load("core/materials/default/unigine_mesh_stem.mat");
		engine.materials.load("core/materials/default/unigine_mesh_wire.mat");
		engine.materials.load("core/materials/default/unigine_mesh_shadow.mat");
		engine.materials.load("core/materials/default/unigine_terrain.mat");
		engine.materials.load("core/materials/default/unigine_grass.mat");
		engine.materials.load("core/materials/default/unigine_particles.mat");
		engine.materials.load("core/materials/default/unigine_billboard.mat");
		engine.materials.load("core/materials/default/unigine_billboards.mat");
		engine.materials.load("core/materials/default/unigine_volume.mat");
		engine.materials.load("core/materials/default/unigine_gui.mat");
		engine.materials.load("core/materials/default/unigine_water.mat");
		engine.materials.load("core/materials/default/unigine_sky.mat");
		
		// system properties
		engine.properties.load("core/properties/unigine.prop");
	}
	
	void shutdown() {
		
		// remove widgets
		#ifdef HAS_INTERFACE
			delete main_i;
			delete event_i;
			delete message_i;
		#else
			engine.gui.removeChild(main_w);
			engine.gui.removeChild(main_w);
		#endif
		if(gpu_l != NULL) engine.gui.removeChild(gpu_l);
		
		// save config
		engine.config.setInt("system_language",language);
	}
	
	/*
	 */
	void show() {
		if(enabled == 0) {
			enabled = 1;
			#ifdef HAS_INTERFACE
				if(main_i == NULL) {
					main_i = new InterfaceWindow(0);
					main_i.setWidget(main_w);
					main_i.setCenterPosition();
				}
				main_i.setWidget(main_w);
				main_i.setHidden(0);
			#else
				main_w.setHidden(0);
				engine.gui.addChild(main_w,GUI_ALIGN_OVERLAP);
			#endif
			main_w.setFocus();
			run_function("update");
		}
	}
	
	void hide() {
		if(enabled == 1) {
			enabled = 0;
			#ifdef HAS_INTERFACE
				if(main_i != NULL) {
					main_i.setWidget(NULL);
					main_i.setHidden(1);
				}
			#else
				engine.gui.removeChild(main_w);
			#endif
		}
		#ifdef HAS_INTERFACE
			if(message_i != NULL) {
				message_i.setWidget(NULL);
				message_i.setHidden(1);
			}
		#else
			engine.gui.removeChild(message_d);
		#endif
	}
	
	void set_toggle(int t) {
		toggle = t;
	}
	
	int get_toggle() {
		return toggle;
	}
	
	int get_activity() {
		return enabled;
	}
	
	#ifdef HAS_GPU_MONITOR
		
		/*
		 */
		void show_gpu() {
			log.message("show_gpu = %d\n",engine.config.getInt("show_gpu",SHOW_GPU));
		}
		
		void show_gpu(string show) {
			engine.config.setInt("show_gpu",int(show));
		}
		
	#endif
	
	/*
	 */
	void update() {
		
		int init;
		float time;
		int mouse_x;
		int mouse_y;
		
		if(init == 0) {
			init = 1;
			time = clock();
			mouse_x = engine.app.getWidth() / 2;
			mouse_y = engine.app.getHeight() / 2;
		}
		
		#ifdef MOUSE_USER
			
			// don't handle mouse
			
		#elif MOUSE_SOFT
			
			// check editor
			if(engine.editor.isLoaded() == 0) {
				
				// hide gui mouse
				engine.gui.setMouseCursor(GUI_CURSOR_NONE);
				
				// switch mouse cursor
				if(mouse_x == engine.app.getMouseX() && mouse_y == engine.app.getMouseY()) {
					if(clock() - time > 1.0f && engine.gui.getActivity() == 0) engine.app.setMouseShow(0);
					else engine.app.setMouseShow(1);
				} else {
					time = clock();
					mouse_x = engine.app.getMouseX();
					mouse_y = engine.app.getMouseY();
					engine.app.setMouseShow(1);
				}
				
				// engine controls
				if(engine.gui.getMouseButton() == APP_BUTTON_LEFT && engine.gui.getActivity() == 0) {
					engine.controls.setMouseEnabled(1);
				} else {
					engine.controls.setMouseEnabled(0);
				}
			}
			
		#else
			
			// check editor
			if(engine.editor.isLoaded() == 0) {
				
				// mouse status
				int mouse_status;
				if(engine.gui.getMouseButton() == APP_BUTTON_LEFT) mouse_status++;
				else mouse_status = 0;
				
				// hide system mouse
				engine.app.setMouseShow(0);
				
				// engine controls
				if(engine.controls.isMouseEnabled()) {
					if(engine.app.clearKeyState(toggle) || engine.console.getActivity()) {
						if(engine.app.getMouseGrab()) engine.app.setMouse(mouse_x,mouse_y);
						engine.controls.setMouseEnabled(0);
						engine.gui.setMouseEnabled(1);
						engine.app.setMouseGrab(0);
					}
				} else if(engine.gui.getActivity() == 0 && mouse_status > 1) {
					mouse_x = engine.app.getMouseX();
					mouse_y = engine.app.getMouseY();
					engine.controls.setMouseEnabled(1);
					engine.gui.setMouseEnabled(0);
					engine.app.setMouseGrab(1);
				}
			}
			
		#endif
		
		#ifdef MENU_USER
			
			// don't handle menu
			
		#else
			
			// show/hide main window
			if(engine.console.getActivity() == 0 && engine.gui.getMouseGrab() == 0 &&
				engine.gui.getPermanentFocus() == NULL && engine.app.clearKeyState(toggle)) {
				
				if(enabled == 0) show();
				else hide();
			}
			
		#endif
		
		// update system
		if(enabled) {
			run_function("update_dynamic");
			apply_b.setEnabled(run_function("is_changed"));
		}
		
		// apply status
		if(clicked) {
			clicked++;
			if(apply_b.isEnabled() && engine.getError() != "") {
				engine.dialogError(engine.gui.translate("Can't apply settings"));
				clicked = 0;
			}
			if(clicked == 4) {
				clicked = 0;
			}
		}
		
		// key states
		int get_key_state(int state) {
			int key = engine.controls.getStateKey(state);
			return (key && engine.app.clearKeyState(key));
		}
		
		if(get_key_state(CONTROLS_STATE_SAVE)) engine.console.run("state_save");
		if(get_key_state(CONTROLS_STATE_RESTORE)) engine.console.run("state_restore");
		if(get_key_state(CONTROLS_STATE_SCREENSHOT)) engine.console.run("video_grab");
		
		// gpu monitor
		#ifdef HAS_GPU_MONITOR
			if(gpu_l == NULL) {
				engine.console.addCommand("show_gpu","show gpu monitor","System::show_gpu");
				gpu_l = new WidgetLabel(engine.getGui());
				gpu_l.setFontPermanent(1);
				gpu_l.setFontVSpacing(3);
				gpu_l.setFontOutline(1);
				gpu_l.setFontRich(1);
			}
			if(engine.config.getInt("show_gpu",SHOW_GPU)) {
				string text = "";
				forloop(int i = 0; engine.gpumonitor.getNumMonitors()) {
					GPUMonitor monitor = engine.gpumonitor.getMonitor(i);
					forloop(int j = 0; monitor.getNumAdapters()) {
						float core = monitor.getCoreClock(j);
						float memory = monitor.getMemoryClock(j);
						float shader = monitor.getShaderClock(j);
						float temperature = monitor.getTemperature(j);
						text += format("<p align=right><font size=14>%s</font></p>",monitor.getAdapterName(j));
						if(core > 0.0f) text += format("<p align=right><font size=11>Graphics: %d MHz</font></p>",core);
						if(memory > 0.0f) text += format("<p align=right><font size=11>Memory: %d MHz</font></p>",memory);
						if(shader > 0.0f) text += format("<p align=right><font size=11>Processor: %d MHz</font></p>",shader);
						if(temperature > -1000.0f) text += format("<p align=right><font size=11>Temperature: %d ºC</font></p>",temperature);
						text += format("<p align=right><font size=1> </font></p>");
					}
				}
				gpu_l.setText(text);
				gpu_l.arrange();
				int position_x = engine.app.getWidth() - gpu_l.getWidth() - 4;
				if(engine.console.getInt("show_fps")) gpu_l.setPosition(position_x,28);
				else gpu_l.setPosition(position_x,4);
				engine.gui.addChild(gpu_l,GUI_ALIGN_OVERLAP);
			} else {
				engine.gui.removeChild(gpu_l);
			}
		#endif
		
		// editor status
		editor = engine.editor.isLoaded();
	}
	
	/*
	 */
	void update_setting(WidgetCheckBox checkbox,string name,int value) {
		if(checkbox == NULL) return;
		checkbox.setChecked(checkbox.isEnabled() && (engine.console.getInt(name) == value));
	}
	
	void update_setting(WidgetCheckBox checkbox,string name,int enable,int value) {
		if(checkbox == NULL) return;
		if(is_int(enable)) checkbox.setEnabled(enable);
		else if(is_string(enable)) checkbox.setEnabled(engine.console.getInt(enable) == 0);
		checkbox.setChecked(checkbox.isEnabled() && (engine.console.getInt(name) == value));
	}
	
	int check_setting(WidgetCheckBox checkbox,string name,int value) {
		if(checkbox == NULL || checkbox.isEnabled() == 0) return 0;
		return (engine.console.getInt(name) != checkbox.isChecked() * value);
	}
	
	void apply_setting(WidgetCheckBox checkbox,string name,int value) {
		if(checkbox == NULL || checkbox.isEnabled() == 0) return;
		engine.console.setInt(name,checkbox.isChecked() * value);
	}
	
	/**************************************************************************\
	*
	* Video
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Video {
		
		WidgetCheckBox opengl_cb;		// video app
		WidgetCheckBox direct3d11_cb;
		
		WidgetComboBox mode_cb;			// video mode number
		WidgetComboBox multisample_cb;	// multisample number
		WidgetCheckBox fullscreen_cb;	// fullscreen toggle
		WidgetCheckBox vsync_cb;		// vertical sync toggle
		
		WidgetSlider gamma_sl;			// gamma correction
		
		/*
		 */
		void init() {
			
			#ifdef HAS_OPENGL
				if(opengl_cb != NULL) opengl_cb.setEnabled(1);
			#endif
			#ifdef HAS_DIRECT3D11
				if(direct3d11_cb != NULL) direct3d11_cb.setEnabled(1);
			#endif
		}
		
		/*
		 */
		void update() {
			
			// video app
			string video_app = engine.console.getString("video_app");
			if(opengl_cb != NULL && video_app == "opengl") opengl_cb.setChecked(1);
			if(direct3d11_cb != NULL && video_app == "direct3d11") direct3d11_cb.setChecked(1);
			
			// video mode
			if(mode_cb != NULL) mode_cb.setCurrentItem(engine.console.getInt("video_mode"));
			if(multisample_cb != NULL) multisample_cb.setCurrentItem(engine.console.getInt("video_multisample"));
			update_setting(fullscreen_cb,"video_fullscreen",VIDEO_FULLSCREEN);
			update_setting(vsync_cb,"video_vsync",1);
			
			// gamma correction
			if(gamma_sl != NULL) gamma_sl.setValue(engine.console.getFloat("video_gamma") * 1000.0f);
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		int is_app_changed() {
			string app = engine.console.getString("video_app");
			if(opengl_cb != NULL && app == "opengl" && opengl_cb.isChecked() == 0) return 1;
			if(direct3d11_cb != NULL && app == "direct3d11" && direct3d11_cb.isChecked() == 0) return 1;
			return 0;
		}
		
		int is_mode_changed() {
			if(mode_cb != NULL && mode_cb.getCurrentItem() != engine.console.getInt("video_mode")) return 1;
			if(multisample_cb != NULL && multisample_cb.getCurrentItem() != engine.console.getInt("video_multisample")) return 1;
			if(check_setting(fullscreen_cb,"video_fullscreen",VIDEO_FULLSCREEN)) return 1;
			if(check_setting(vsync_cb,"video_vsync",1)) return 1;
			return 0;
		}
		
		int is_gamma_changed() {
			if(gamma_sl != NULL && gamma_sl.getValue() / 1000.0f != engine.console.getFloat("video_gamma")) return 1;
			return 0;
		}
		
		int is_changed() {
			if(is_app_changed()) return 1;
			if(is_mode_changed()) return 1;
			if(is_gamma_changed()) return 1;
			return 0;
		}
		
		/*
		 */
		void apply() {
			
			// video app
			string app = engine.console.getString("video_app");
			if(opengl_cb != NULL && opengl_cb.isChecked()) {
				if(app != "opengl") thread("System::need_restart");
				engine.console.setString("video_app","opengl");
			}
			if(direct3d11_cb != NULL && direct3d11_cb.isChecked()) {
				if(app != "direct3d11") thread("System::need_restart");
				engine.console.setString("video_app","direct3d11");
			}
			
			// video mode
			if(is_mode_changed()) {
				update_splash();
				engine.console.setInt("video_mode",mode_cb.getCurrentItem());
				engine.console.setInt("video_multisample",multisample_cb.getCurrentItem());
				apply_setting(fullscreen_cb,"video_fullscreen",VIDEO_FULLSCREEN);
				apply_setting(vsync_cb,"video_vsync",1);
				engine.console.run("video_restart mode");
			}
			
			// gamma correction
			if(is_gamma_changed()) {
				engine.console.setFloat("video_gamma",gamma_sl.getValue() / 1000.0f);
				engine.console.run("video_restart gamma");
			}
		}
	}
	
	/**************************************************************************\
	*
	* Render
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Render {
		
		WidgetComboBox shaders_cb;					// shaders quality
		WidgetCheckBox shaders_low_cb;
		WidgetCheckBox shaders_medium_cb;
		WidgetCheckBox shaders_high_cb;
		
		WidgetComboBox textures_cb;					// textures quality
		WidgetCheckBox textures_low_cb;
		WidgetCheckBox textures_medium_cb;
		WidgetCheckBox textures_high_cb;
		
		WidgetComboBox filter_cb;					// texture filter
		WidgetComboBox anisotropy_cb;				// texture anisotropy
		
		WidgetCheckBox alpha_fade_cb;				// alpha fade toggle
		WidgetCheckBox reflection_cb;				// reflection toggle
		WidgetCheckBox translucent_cb;				// translucent toggle
		WidgetCheckBox parallax_cb;					// parallax toggle
		WidgetCheckBox light_prob_cb;				// light prob toggle
		WidgetCheckBox occlusion_cb;				// occlusion toggle
		WidgetCheckBox auxiliary_cb;				// auxiliary toggle
		WidgetCheckBox refraction_cb;				// refraction toggle
		WidgetCheckBox motion_blur_cb;				// motion blur toggle
		WidgetCheckBox scattering_cb;				// scattering toggle
		WidgetCheckBox volumetric_cb;				// volumetric toggle
		WidgetCheckBox glow_cb;						// glow toggle
		WidgetCheckBox dof_cb;						// dof toggle
		WidgetCheckBox hdr_cb;						// hdr toggle
		
		#ifdef HAS_DIRECT3D11
			WidgetCheckBox d3d11_render_use_tessellation_cb;
			WidgetCheckBox d3d11_render_use_feature_level_11_cb;
			WidgetCheckBox d3d11_render_use_feature_level_101_cb;
			WidgetCheckBox d3d11_render_use_alpha_test_level_101_cb;
		#endif
		
		#ifdef HAS_OPENGL
			WidgetCheckBox gl_render_use_arb_tessellation_shader_cb;
			WidgetCheckBox gl_render_use_arb_sample_shading_cb;
		#endif
		
		/*
		 */
		void init() {
			
		}
		
		/*
		 */
		void update() {
			
			// shaders
			int shaders = engine.console.getInt("render_shaders");
			if(shaders_cb != NULL) shaders_cb.setCurrentItem(shaders);
			if(shaders_low_cb != NULL) shaders_low_cb.setChecked((shaders == RENDER_QUALITY_LOW));
			if(shaders_medium_cb != NULL) shaders_medium_cb.setChecked((shaders == RENDER_QUALITY_MEDIUM));
			if(shaders_high_cb != NULL) shaders_high_cb.setChecked((shaders == RENDER_QUALITY_HIGH));
			
			// textures
			int textures = engine.console.getInt("render_textures");
			if(textures_cb != NULL) textures_cb.setCurrentItem(textures);
			if(textures_low_cb != NULL) textures_low_cb.setChecked((textures == RENDER_QUALITY_LOW));
			if(textures_medium_cb != NULL) textures_medium_cb.setChecked((textures == RENDER_QUALITY_MEDIUM));
			if(textures_high_cb != NULL) textures_high_cb.setChecked((textures == RENDER_QUALITY_HIGH));
			if(filter_cb != NULL) filter_cb.setCurrentItem(engine.console.getInt("render_filter"));
			if(anisotropy_cb != NULL) anisotropy_cb.setCurrentItem(engine.console.getInt("render_anisotropy"));
			
			// settings
			update_setting(alpha_fade_cb,"render_alpha_fade",1);
			update_setting(reflection_cb,"render_reflection","render_skip_reflection",1);
			update_setting(translucent_cb,"render_translucent","render_skip_translucent",1);
			update_setting(parallax_cb,"render_parallax","render_skip_deferred",1);
			update_setting(light_prob_cb,"render_light_prob","render_skip_light_prob",1);
			update_setting(occlusion_cb,"render_occlusion","render_skip_occlusion",RENDER_OCCLUSION);
			update_setting(auxiliary_cb,"render_auxiliary","render_skip_auxiliary",1);
			update_setting(refraction_cb,"render_refraction","render_skip_refraction",1);
			update_setting(motion_blur_cb,"render_motion_blur","render_skip_motion_blur",1);
			update_setting(scattering_cb,"render_scattering","render_skip_scattering",1);
			update_setting(volumetric_cb,"render_volumetric","render_skip_scattering",1);
			update_setting(glow_cb,"render_glow","render_skip_post_materials",1);
			update_setting(dof_cb,"render_dof","render_skip_post_materials",1);
			update_setting(hdr_cb,"render_hdr","render_skip_post_materials",RENDER_HDR);
			
			#ifdef HAS_DIRECT3D11
				int is_direct3d11 = (engine.console.getString("video_app") == "direct3d11");
				update_setting(d3d11_render_use_tessellation_cb,"d3d11_render_use_tessellation",is_direct3d11,1);
				update_setting(d3d11_render_use_feature_level_11_cb,"d3d11_render_use_feature_level_11",is_direct3d11,1);
				update_setting(d3d11_render_use_feature_level_101_cb,"d3d11_render_use_feature_level_101",is_direct3d11,1);
				update_setting(d3d11_render_use_alpha_test_level_101_cb,"d3d11_render_use_alpha_test_level_101",is_direct3d11,1);
			#endif
			
			#ifdef HAS_OPENGL
				int is_opengl = (engine.console.getString("video_app") == "opengl");
				update_setting(gl_render_use_arb_tessellation_shader_cb,"gl_render_use_arb_tessellation_shader",is_opengl,1);
				update_setting(gl_render_use_arb_sample_shading_cb,"gl_render_use_arb_sample_shading",is_opengl,1);
			#endif
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		int is_changed() {
			
			// shaders
			int shaders = engine.console.getInt("render_shaders");
			if(shaders_cb != NULL && shaders_cb.getCurrentItem() != shaders) return 1;
			if(shaders_low_cb != NULL && shaders_low_cb.isChecked() != (shaders == RENDER_QUALITY_LOW)) return 1;
			if(shaders_medium_cb != NULL && shaders_medium_cb.isChecked() != (shaders == RENDER_QUALITY_MEDIUM)) return 1;
			if(shaders_high_cb != NULL && shaders_high_cb.isChecked() != (shaders == RENDER_QUALITY_HIGH)) return 1;
			
			// textures
			int textures = engine.console.getInt("render_textures");
			if(textures_cb != NULL && textures_cb.getCurrentItem() != textures) return 1;
			if(textures_low_cb != NULL && textures_low_cb.isChecked() != (textures == RENDER_QUALITY_LOW)) return 1;
			if(textures_medium_cb != NULL && textures_medium_cb.isChecked() != (textures == RENDER_QUALITY_MEDIUM)) return 1;
			if(textures_high_cb != NULL && textures_high_cb.isChecked() != (textures == RENDER_QUALITY_HIGH)) return 1;
			if(filter_cb != NULL && filter_cb.getCurrentItem() != engine.console.getInt("render_filter")) return 1;
			if(anisotropy_cb != NULL && anisotropy_cb.getCurrentItem() != engine.console.getInt("render_anisotropy")) return 1;
			
			// settings
			if(check_setting(alpha_fade_cb,"render_alpha_fade",1)) return 1;
			if(check_setting(reflection_cb,"render_reflection",1)) return 1;
			if(check_setting(translucent_cb,"render_translucent",1)) return 1;
			if(check_setting(parallax_cb,"render_parallax",1)) return 1;
			if(check_setting(light_prob_cb,"render_light_prob",1)) return 1;
			if(check_setting(occlusion_cb,"render_occlusion",RENDER_OCCLUSION)) return 1;
			if(check_setting(auxiliary_cb,"render_auxiliary",1)) return 1;
			if(check_setting(refraction_cb,"render_refraction",1)) return 1;
			if(check_setting(motion_blur_cb,"render_motion_blur",1)) return 1;
			if(check_setting(scattering_cb,"render_scattering",1)) return 1;
			if(check_setting(volumetric_cb,"render_volumetric",1)) return 1;
			if(check_setting(glow_cb,"render_glow",1)) return 1;
			if(check_setting(dof_cb,"render_dof",1)) return 1;
			if(check_setting(hdr_cb,"render_hdr",RENDER_HDR)) return 1;
			
			#ifdef HAS_DIRECT3D11
				if(check_setting(d3d11_render_use_tessellation_cb,"d3d11_render_use_tessellation",1)) return 1;
				if(check_setting(d3d11_render_use_feature_level_11_cb,"d3d11_render_use_feature_level_11",1)) return 1;
				if(check_setting(d3d11_render_use_feature_level_101_cb,"d3d11_render_use_feature_level_101",1)) return 1;
				if(check_setting(d3d11_render_use_alpha_test_level_101_cb,"d3d11_render_use_alpha_test_level_101",1)) return 1;
			#endif
			
			#ifdef HAS_OPENGL
				if(check_setting(gl_render_use_arb_tessellation_shader_cb,"gl_render_use_arb_tessellation_shader",1)) return 1;
				if(check_setting(gl_render_use_arb_sample_shading_cb,"gl_render_use_arb_sample_shading",1)) return 1;
			#endif
			
			return 0;
		}
		
		/*
		 */
		void apply() {
			
			// shaders
			if(shaders_cb != NULL) engine.console.setInt("render_shaders",shaders_cb.getCurrentItem());
			if(shaders_low_cb != NULL && shaders_low_cb.isChecked()) engine.console.setInt("render_shaders",RENDER_QUALITY_LOW);
			if(shaders_medium_cb != NULL && shaders_medium_cb.isChecked()) engine.console.setInt("render_shaders",RENDER_QUALITY_MEDIUM);
			if(shaders_high_cb != NULL && shaders_high_cb.isChecked()) engine.console.setInt("render_shaders",RENDER_QUALITY_HIGH);
			
			// textures
			if(textures_cb != NULL) engine.console.setInt("render_textures",textures_cb.getCurrentItem());
			if(textures_low_cb != NULL && textures_low_cb.isChecked()) engine.console.setInt("render_textures",RENDER_QUALITY_LOW);
			if(textures_medium_cb != NULL && textures_medium_cb.isChecked()) engine.console.setInt("render_textures",RENDER_QUALITY_MEDIUM);
			if(textures_high_cb != NULL && textures_high_cb.isChecked()) engine.console.setInt("render_textures",RENDER_QUALITY_HIGH);
			if(filter_cb != NULL) engine.console.setInt("render_filter",filter_cb.getCurrentItem());
			if(anisotropy_cb != NULL) engine.console.setInt("render_anisotropy",anisotropy_cb.getCurrentItem());
			
			// settings
			apply_setting(alpha_fade_cb,"render_alpha_fade",1);
			apply_setting(reflection_cb,"render_reflection",1);
			apply_setting(translucent_cb,"render_translucent",1);
			apply_setting(parallax_cb,"render_parallax",1);
			apply_setting(light_prob_cb,"render_light_prob",1);
			apply_setting(occlusion_cb,"render_occlusion",RENDER_OCCLUSION);
			apply_setting(auxiliary_cb,"render_auxiliary",1);
			apply_setting(refraction_cb,"render_refraction",1);
			apply_setting(motion_blur_cb,"render_motion_blur",1);
			apply_setting(scattering_cb,"render_scattering",1);
			apply_setting(volumetric_cb,"render_volumetric",1);
			apply_setting(glow_cb,"render_glow",1);
			apply_setting(dof_cb,"render_dof",1);
			apply_setting(hdr_cb,"render_hdr",RENDER_HDR);
			
			#ifdef HAS_DIRECT3D11
				apply_setting(d3d11_render_use_tessellation_cb,"d3d11_render_use_tessellation",1);
				apply_setting(d3d11_render_use_feature_level_11_cb,"d3d11_render_use_feature_level_11",1);
				apply_setting(d3d11_render_use_feature_level_101_cb,"d3d11_render_use_feature_level_101",1);
				apply_setting(d3d11_render_use_alpha_test_level_101_cb,"d3d11_render_use_alpha_test_level_101",1);
			#endif
			
			#ifdef HAS_OPENGL
				apply_setting(gl_render_use_arb_tessellation_shader_cb,"gl_render_use_arb_tessellation_shader",1);
				apply_setting(gl_render_use_arb_sample_shading_cb,"gl_render_use_arb_sample_shading",1);
			#endif
			
			engine.console.run("render_restart");
		}
	}
	
	/**************************************************************************\
	*
	* Interface
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Interface {
		
		WidgetCheckBox fps_cb;			// fps counter toggle
		WidgetCheckBox gpu_cb;			// gpu monitor toggle
		WidgetCheckBox messages_cb;		// messages toggle
		WidgetCheckBox profiler_cb;		// profiler toggle
		
		WidgetCheckBox triangles_cb;	// show triangles toggle
		
		WidgetComboBox language_cb;		// system language
		
		/*
		 */
		void init() {
			
			::System::language_cb = language_cb;
		}
		
		/*
		 */
		void update() {
			
			// engine
			update_setting(fps_cb,"show_fps",1);
			update_setting(messages_cb,"show_messages",1);
			update_setting(profiler_cb,"show_profiler",1);
			
			// render show
			update_setting(triangles_cb,"render_show_triangles",1);
			
			// gpu monitor
			#ifdef HAS_GPU_MONITOR
				if(gpu_cb != NULL) gpu_cb.setChecked(engine.config.getInt("show_gpu",SHOW_GPU));
			#else
				if(gpu_cb != NULL) gpu_cb.setHidden(1);
			#endif
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		int is_changed() {
			
			// engine
			if(check_setting(fps_cb,"show_fps",1)) return 1;
			if(check_setting(messages_cb,"show_messages",1)) return 1;
			if(check_setting(profiler_cb,"show_profiler",1)) return 1;
			
			// render show
			if(check_setting(triangles_cb,"render_show_triangles",1)) return 1;
			
			// language
			if(language_cb != NULL && language_cb.getCurrentItem() != language) return 1;
			
			// gpu monitor
			#ifdef HAS_GPU_MONITOR
				if(gpu_cb != NULL && gpu_cb.isChecked() != engine.config.getInt("show_gpu",SHOW_GPU)) return 1;
			#endif
			
			return 0;
		}
		
		/*
		 */
		void apply() {
			
			// engine
			apply_setting(fps_cb,"show_fps",1);
			apply_setting(messages_cb,"show_messages",1);
			apply_setting(profiler_cb,"show_profiler",1);
			
			// render show
			apply_setting(triangles_cb,"render_show_triangles",1);
			
			// language
			if(language_cb != NULL) set_language(language_cb.getCurrentItem());
			
			// gpu monitor
			#ifdef HAS_GPU_MONITOR
				if(gpu_cb != NULL) engine.config.setInt("show_gpu",gpu_cb.isChecked());
			#endif
		}
	}
	
	/**************************************************************************\
	*
	* Settings
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Settings {
		
		WidgetComboBox mode_cb;			// video mode number
		WidgetComboBox multisample_cb;	// multisample number
		WidgetCheckBox fullscreen_cb;	// fullscreen toggle
		WidgetCheckBox vsync_cb;		// vertical sync toggle
		
		WidgetSlider gamma_sl;			// gamma correction
		
		WidgetCheckBox occlusion_cb;	// occlusion toggle
		WidgetCheckBox refraction_cb;	// refraction toggle
		WidgetCheckBox motion_blur_cb;	// motion blur toggle
		WidgetCheckBox volumetric_cb;	// volumetric toggle
		
		WidgetCheckBox fps_cb;			// fps counter toggle
		WidgetCheckBox gpu_cb;			// gpu monitor toggle
		WidgetCheckBox messages_cb;		// messages toggle
		WidgetCheckBox profiler_cb;		// profiler toggle
		
		WidgetCheckBox triangles_cb;	// show triangles toggle
		
		WidgetComboBox language_cb;		// system language
		
		/*
		 */
		void init() {
			
			::System::language_cb = language_cb;
		}
		
		/*
		 */
		void update() {
			
			// video mode
			if(mode_cb != NULL) mode_cb.setCurrentItem(engine.console.getInt("video_mode"));
			if(multisample_cb != NULL) multisample_cb.setCurrentItem(engine.console.getInt("video_multisample"));
			update_setting(fullscreen_cb,"video_fullscreen",VIDEO_FULLSCREEN);
			update_setting(vsync_cb,"video_vsync",1);
			
			// gamma correction
			if(gamma_sl != NULL) gamma_sl.setValue(engine.console.getFloat("video_gamma") * 1000.0f);
			
			// render settings
			update_setting(occlusion_cb,"render_occlusion","render_skip_occlusion",RENDER_OCCLUSION);
			update_setting(refraction_cb,"render_refraction","render_skip_refraction",1);
			update_setting(motion_blur_cb,"render_motion_blur","render_skip_motion_blur",1);
			update_setting(volumetric_cb,"render_volumetric","render_skip_scattering",1);
			
			// interface settings
			update_setting(fps_cb,"show_fps",1);
			update_setting(messages_cb,"show_messages",1);
			update_setting(profiler_cb,"show_profiler",1);
			
			// render show
			update_setting(triangles_cb,"render_show_triangles",1);
			
			// gpu monitor
			#ifdef HAS_GPU_MONITOR
				if(gpu_cb != NULL) gpu_cb.setChecked(engine.config.getInt("show_gpu",SHOW_GPU));
			#else
				if(gpu_cb != NULL) gpu_cb.setHidden(1);
			#endif
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
		/*
		 */
		int is_mode_changed() {
			if(mode_cb != NULL && mode_cb.getCurrentItem() != engine.console.getInt("video_mode")) return 1;
			if(multisample_cb != NULL && multisample_cb.getCurrentItem() != engine.console.getInt("video_multisample")) return 1;
			if(check_setting(fullscreen_cb,"video_fullscreen",VIDEO_FULLSCREEN)) return 1;
			if(check_setting(vsync_cb,"video_vsync",1)) return 1;
			return 0;
		}
		
		int is_gamma_changed() {
			if(gamma_sl != NULL && gamma_sl.getValue() / 1000.0f != engine.console.getFloat("video_gamma")) return 1;
			return 0;
		}
		
		int is_render_changed() {
			if(check_setting(occlusion_cb,"render_occlusion",RENDER_OCCLUSION)) return 1;
			if(check_setting(refraction_cb,"render_refraction",1)) return 1;
			if(check_setting(motion_blur_cb,"render_motion_blur",1)) return 1;
			if(check_setting(volumetric_cb,"render_volumetric",1)) return 1;
			return 0;
		}
		
		int is_interface_changed() {
			if(check_setting(fps_cb,"show_fps",1)) return 1;
			if(check_setting(messages_cb,"show_messages",1)) return 1;
			if(check_setting(profiler_cb,"show_profiler",1)) return 1;
			if(check_setting(triangles_cb,"render_show_triangles",1)) return 1;
			if(language_cb != NULL && language_cb.getCurrentItem() != language) return 1;
			#ifdef HAS_GPU_MONITOR
				if(gpu_cb != NULL && gpu_cb.isChecked() != engine.config.getInt("show_gpu",SHOW_GPU)) return 1;
			#endif
			return 0;
		}
		
		int is_changed() {
			if(is_mode_changed()) return 1;
			if(is_gamma_changed()) return 1;
			if(is_render_changed()) return 1;
			if(is_interface_changed()) return 1;
			return 0;
		}
		
		/*
		 */
		void apply() {
			
			// video mode
			if(is_mode_changed()) {
				update_splash();
				engine.console.setInt("video_mode",mode_cb.getCurrentItem());
				engine.console.setInt("video_multisample",multisample_cb.getCurrentItem());
				apply_setting(fullscreen_cb,"video_fullscreen",VIDEO_FULLSCREEN);
				apply_setting(vsync_cb,"video_vsync",1);
				engine.console.run("video_restart mode");
			}
			
			// gamma correction
			if(is_gamma_changed()) {
				engine.console.setFloat("video_gamma",gamma_sl.getValue() / 1000.0f);
				engine.console.run("video_restart gamma");
			}
			
			// render settings
			if(is_render_changed()) {
				apply_setting(occlusion_cb,"render_occlusion",RENDER_OCCLUSION);
				apply_setting(refraction_cb,"render_refraction",1);
				apply_setting(motion_blur_cb,"render_motion_blur",1);
				apply_setting(volumetric_cb,"render_volumetric",1);
				engine.console.run("render_restart");
			}
			
			// interface settings
			if(is_interface_changed()) {
				apply_setting(fps_cb,"show_fps",1);
				apply_setting(messages_cb,"show_messages",1);
				apply_setting(profiler_cb,"show_profiler",1);
				apply_setting(triangles_cb,"render_show_triangles",1);
				if(language_cb != NULL) set_language(language_cb.getCurrentItem());
				#ifdef HAS_GPU_MONITOR
					if(gpu_cb != NULL) engine.config.setInt("show_gpu",gpu_cb.isChecked());
				#endif
			}
		}
	}
	
	/**************************************************************************\
	*
	* Controls
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Controls {
		
		WidgetCheckBox always_run_cb;
		WidgetCheckBox mouse_inverse_cb;
		WidgetSlider mouse_sensitivity_sl;
		
		WidgetButton forward_b;
		WidgetButton backward_b;
		WidgetButton move_left_b;
		WidgetButton move_right_b;
		WidgetButton turn_up_b;
		WidgetButton turn_down_b;
		WidgetButton turn_left_b;
		WidgetButton turn_right_b;
		WidgetButton crouch_b;
		WidgetButton jump_b;
		WidgetButton run_b;
		WidgetButton use_b;
		WidgetButton fire_b;
		WidgetButton save_b;
		WidgetButton restore_b;
		WidgetButton screenshot_b;
		WidgetButton aux_0_b;
		WidgetButton aux_1_b;
		WidgetButton aux_2_b;
		WidgetButton aux_3_b;
		WidgetButton aux_4_b;
		WidgetButton aux_5_b;
		WidgetButton aux_6_b;
		WidgetButton aux_7_b;
		WidgetButton aux_8_b;
		WidgetButton aux_9_b;
		WidgetButton aux_a_b;
		WidgetButton aux_b_b;
		WidgetButton aux_c_b;
		WidgetButton aux_d_b;
		WidgetButton aux_e_b;
		WidgetButton aux_f_b;
		
		/*
		 */
		void init() {
			
			// check config
			int init = 1;
			for(int i = 0; i < CONTROLS_STATE_AUX_0; i++) {
				if(engine.config.isExist(format("controls_key_%d",i)) || engine.config.isExist(format("controls_button_%d",i))) {
					init = 0;
					break;
				}
			}
			
			// setup default controls
			if(init) {
				engine.controls.setStateKey(CONTROLS_STATE_FORWARD,'w');
				engine.controls.setStateKey(CONTROLS_STATE_BACKWARD,'s');
				engine.controls.setStateKey(CONTROLS_STATE_MOVE_LEFT,'a');
				engine.controls.setStateKey(CONTROLS_STATE_MOVE_RIGHT,'d');
				engine.controls.setStateKey(CONTROLS_STATE_TURN_UP,APP_KEY_UP);
				engine.controls.setStateKey(CONTROLS_STATE_TURN_DOWN,APP_KEY_DOWN);
				engine.controls.setStateKey(CONTROLS_STATE_TURN_LEFT,APP_KEY_LEFT);
				engine.controls.setStateKey(CONTROLS_STATE_TURN_RIGHT,APP_KEY_RIGHT);
				engine.controls.setStateKey(CONTROLS_STATE_CROUCH,'q');
				engine.controls.setStateKey(CONTROLS_STATE_JUMP,'e');
				engine.controls.setStateKey(CONTROLS_STATE_RUN,APP_KEY_SHIFT);
				engine.controls.setStateKey(CONTROLS_STATE_USE,APP_KEY_RETURN);
				engine.controls.setStateKey(CONTROLS_STATE_SAVE,APP_KEY_F5);
				engine.controls.setStateKey(CONTROLS_STATE_RESTORE,APP_KEY_F6);
				engine.controls.setStateKey(CONTROLS_STATE_SCREENSHOT,APP_KEY_F12);
				engine.controls.setStateButton(CONTROLS_STATE_FIRE,APP_BUTTON_LEFT);
			}
		}
		
		/*
		 */
		void update() {
			
			if(always_run_cb != NULL) always_run_cb.setChecked(engine.controls.getAlwaysRun());
			if(mouse_inverse_cb != NULL) mouse_inverse_cb.setChecked(engine.controls.getMouseInverse());
			if(mouse_sensitivity_sl != NULL) mouse_sensitivity_sl.setValue(engine.controls.getMouseSensitivity() * 1000.0f);
			
			// states
			if(forward_b != NULL) forward_b.setText(engine.controls.getStateName(CONTROLS_STATE_FORWARD));
			if(backward_b != NULL) backward_b.setText(engine.controls.getStateName(CONTROLS_STATE_BACKWARD));
			if(move_left_b != NULL) move_left_b.setText(engine.controls.getStateName(CONTROLS_STATE_MOVE_LEFT));
			if(move_right_b != NULL) move_right_b.setText(engine.controls.getStateName(CONTROLS_STATE_MOVE_RIGHT));
			if(turn_up_b != NULL) turn_up_b.setText(engine.controls.getStateName(CONTROLS_STATE_TURN_UP));
			if(turn_down_b != NULL) turn_down_b.setText(engine.controls.getStateName(CONTROLS_STATE_TURN_DOWN));
			if(turn_left_b != NULL) turn_left_b.setText(engine.controls.getStateName(CONTROLS_STATE_TURN_LEFT));
			if(turn_right_b != NULL) turn_right_b.setText(engine.controls.getStateName(CONTROLS_STATE_TURN_RIGHT));
			if(crouch_b != NULL) crouch_b.setText(engine.controls.getStateName(CONTROLS_STATE_CROUCH));
			if(jump_b != NULL) jump_b.setText(engine.controls.getStateName(CONTROLS_STATE_JUMP));
			if(run_b != NULL) run_b.setText(engine.controls.getStateName(CONTROLS_STATE_RUN));
			if(use_b != NULL) use_b.setText(engine.controls.getStateName(CONTROLS_STATE_USE));
			if(fire_b != NULL) fire_b.setText(engine.controls.getStateName(CONTROLS_STATE_FIRE));
			if(save_b != NULL) save_b.setText(engine.controls.getStateName(CONTROLS_STATE_SAVE));
			if(restore_b != NULL) restore_b.setText(engine.controls.getStateName(CONTROLS_STATE_RESTORE));
			if(screenshot_b != NULL) screenshot_b.setText(engine.controls.getStateName(CONTROLS_STATE_SCREENSHOT));
			if(aux_0_b != NULL) aux_0_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_0));
			if(aux_1_b != NULL) aux_1_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_1));
			if(aux_2_b != NULL) aux_2_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_2));
			if(aux_3_b != NULL) aux_3_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_3));
			if(aux_4_b != NULL) aux_4_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_4));
			if(aux_5_b != NULL) aux_5_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_5));
			if(aux_6_b != NULL) aux_6_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_6));
			if(aux_7_b != NULL) aux_7_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_7));
			if(aux_8_b != NULL) aux_8_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_8));
			if(aux_9_b != NULL) aux_9_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_9));
			if(aux_a_b != NULL) aux_a_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_A));
			if(aux_b_b != NULL) aux_b_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_B));
			if(aux_c_b != NULL) aux_c_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_C));
			if(aux_d_b != NULL) aux_d_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_D));
			if(aux_e_b != NULL) aux_e_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_E));
			if(aux_f_b != NULL) aux_f_b.setText(engine.controls.getStateName(CONTROLS_STATE_AUX_F));
		}
		
		/*
		 */
		void update_dynamic() {
			if(engine.controls.isStateEvent()) {
				#ifdef HAS_INTERFACE
					event_i.setWidget(NULL);
					event_i.setHidden(1);
				#else
					engine.gui.removeChild(event_w);
				#endif
				update();
			}
		}
		
		/*
		 */
		int is_changed() {
			if(always_run_cb != NULL && always_run_cb.isChecked() != engine.controls.getAlwaysRun()) return 1;
			if(mouse_inverse_cb != NULL && mouse_inverse_cb.isChecked() != engine.controls.getMouseInverse()) return 1;
			if(mouse_sensitivity_sl != NULL && mouse_sensitivity_sl.getValue() / 1000.0f != engine.controls.getMouseSensitivity()) return 1;
			return 0;
		}
		
		/*
		 */
		void apply() {
			if(always_run_cb != NULL) engine.controls.setAlwaysRun(always_run_cb.isChecked());
			if(mouse_inverse_cb != NULL) engine.controls.setMouseInverse(mouse_inverse_cb.isChecked());
			if(mouse_sensitivity_sl != NULL) engine.controls.setMouseSensitivity(mouse_sensitivity_sl.getValue() / 1000.0f);
		}
		
		/*
		 */
		void event_clicked(string state) {
			
			if(event_w == NULL) {
				log.error("System::Controls::event_clicked(): event window is NULL\n");
				return;
			}
			
			int states[] = (
				"forward"		: CONTROLS_STATE_FORWARD,
				"backward"		: CONTROLS_STATE_BACKWARD,
				"move_left"		: CONTROLS_STATE_MOVE_LEFT,
				"move_right"	: CONTROLS_STATE_MOVE_RIGHT,
				"turn_up"		: CONTROLS_STATE_TURN_UP,
				"turn_down"		: CONTROLS_STATE_TURN_DOWN,
				"turn_left"		: CONTROLS_STATE_TURN_LEFT,
				"turn_right"	: CONTROLS_STATE_TURN_RIGHT,
				"crouch"		: CONTROLS_STATE_CROUCH,
				"jump"			: CONTROLS_STATE_JUMP,
				"run"			: CONTROLS_STATE_RUN,
				"use"			: CONTROLS_STATE_USE,
				"fire"			: CONTROLS_STATE_FIRE,
				"save"			: CONTROLS_STATE_SAVE,
				"restore"		: CONTROLS_STATE_RESTORE,
				"screenshot"	: CONTROLS_STATE_SCREENSHOT,
				
				"aux_0" : CONTROLS_STATE_AUX_0, "aux_1" : CONTROLS_STATE_AUX_1,
				"aux_2" : CONTROLS_STATE_AUX_2, "aux_3" : CONTROLS_STATE_AUX_3,
				"aux_4" : CONTROLS_STATE_AUX_4, "aux_5" : CONTROLS_STATE_AUX_5,
				"aux_6" : CONTROLS_STATE_AUX_6, "aux_7" : CONTROLS_STATE_AUX_7,
				"aux_8" : CONTROLS_STATE_AUX_8, "aux_9" : CONTROLS_STATE_AUX_9,
				"aux_a" : CONTROLS_STATE_AUX_A, "aux_b" : CONTROLS_STATE_AUX_B,
				"aux_c" : CONTROLS_STATE_AUX_C, "aux_d" : CONTROLS_STATE_AUX_D,
				"aux_e" : CONTROLS_STATE_AUX_E, "aux_f" : CONTROLS_STATE_AUX_F,
			);
			
			if(states.check(state) == 0) {
				log.error("System::Controls::event_clicked(): unknown event state \"%s\"\n",state);
				return;
			}
			
			engine.controls.getStateEvent(states[state]);
			
			#ifdef HAS_INTERFACE
				if(event_i == NULL) event_i = new InterfaceWindow(0);
				event_i.setWidget(event_w);
				event_i.setCenterPosition();
				event_i.setHidden(0);
				event_i.setPermanentFocus();
				event_w.setPermanentFocus();
			#else
				engine.gui.addChild(event_w,GUI_ALIGN_CENTER | GUI_ALIGN_OVERLAP);
				event_w.setPermanentFocus();
			#endif
		}
	}
}

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void systemInit(string name,string languages[],string locales[]) {
	System::init(name,languages,locales);
}

void systemInit(string name = "core/unigine.ui") {
	string languages[] = ( "English", "Russian" );
	string locales[] = ( "core/locale/unigine.locale:en", "core/locale/unigine.locale:ru" );
	System::init(name,languages,locales);
}

void systemShutdown() {
	System::shutdown();
}

void systemUpdate() {
	System::update();
}

void systemQuit() {
	thread("System::quit_clicked");
}

/*
 */
void systemShow() {
	System::show();
}

void systemHide() {
	System::hide();
}

void systemSetToggle(int toggle) {
	System::set_toggle(toggle);
}

int systemGetToggle() {
	return System::get_toggle();
}

int systemGetActivity() {
	return System::get_activity();
}

/*
 */
void systemSetLanguage(int language) {
	System::set_language(language);
}

int systemGetLanguage() {
	return System::get_language();
}

void systemSetLanguageCallback(string callback) {
	System::language_callback = callback;
}

string systemGetLanguageCallback() {
	return System::language_callback;
}

void systemSetLanguageWorldCallback(string callback) {
	System::language_world_callback = callback;
}

string systemGetLanguageWorldCallback() {
	return System::language_world_callback;
}

void systemSetLanguageEditorCallback(string callback) {
	System::language_editor_callback = callback;
}

string systemGetLanguageEditorCallback() {
	return System::language_editor_callback;
}

/*
 */
void systemSetSplash(string splash_4x3_name,string splash_16x9_name,int splash_threshold = 16,
	vec4 splash_transform = vec4(1.0f,1.0f,0.0f,0.0f),vec4 splash_color = vec4(0.0f,0.0f,0.0f,1.0f),string splash_text = "") {
	
	System::splash_4x3_name = splash_4x3_name;
	System::splash_16x9_name = splash_16x9_name;
	System::splash_threshold = splash_threshold;
	System::splash_transform = splash_transform;
	System::splash_color = splash_color;
	System::splash_text = splash_text;
	
	System::update_splash();
}

void systemSetSplash(string splash_4x3_name,string splash_16x9_name,int splash_threshold = 16) {
	
	System::splash_4x3_name = splash_4x3_name;
	System::splash_16x9_name = splash_16x9_name;
	System::splash_threshold = splash_threshold;
	System::splash_transform = vec4(1.0f,1.0f,0.0f,0.0f);
	System::splash_color = vec4(0.0f,0.0f,0.0f,1.0f);
	System::splash_text = "";
	
	System::update_splash();
}

/*
 */
WidgetWindow systemGetWindow() {
	return System::main_w;
}

void systemSetWindowText(string text) {
	return System::main_w.setText(text);
}

#endif /* __UNIGINE_SYSTEM_H__ */
