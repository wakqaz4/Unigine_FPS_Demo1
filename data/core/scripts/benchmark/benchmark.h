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


#ifndef __UNIGINE_BENCHMARK_H__
#define __UNIGINE_BENCHMARK_H__

/*
 */
namespace Unigine {
	
	/*
	 */
	class Benchmark {
		
		int activity;				// benchmark status
		int first_frame;			// first frame status
		
		string name;				// benchmark name
		float scale;				// benchmark scale
		int flags;					// benchmark flags
		string settings[0];			// benchmark settings
		
		WidgetLabel benchmark_l;	// benchmark label
		
		WidgetGroupBox panel_gb;	// statistics panel
		WidgetLabel fps_l;			// current fps
		WidgetLabel time_l;			// current time
		WidgetLabel frames_l;		// current frames
		WidgetLabel min_fps_l;		// current minimum fps
		WidgetLabel max_fps_l;		// current maximum fps
		WidgetLabel scene_l;		// current scene
		
		WidgetDialog result_d;		// result dialog
		WidgetLabel result_l;		// result label
		
		WidgetDialogFile result_df;	// result dialog file
		
		float time;					// total time
		int frames;					// total frames
		
		float fps_time;				// fps time
		int fps_frames;				// fps frames
		
		float min_fps;				// minimum fps
		float max_fps;				// maximum fps
		
		enum {
			
			RENDER_TRANSFORMING	= 0x000001,
			RENDER_TESSELLATION	= 0x000002,
			RENDER_SHADERS		= 0x000004,
			RENDER_TEXTURES		= 0x000008,
			RENDER_FILTER		= 0x000010,
			RENDER_ANISOTROPY	= 0x000020,
			RENDER_REFLECTION	= 0x000040,
			RENDER_TRANSLUCENT	= 0x000080,
			RENDER_PARALLAX		= 0x000100,
			RENDER_OCCLUSION	= 0x000200,
			RENDER_REFRACTION	= 0x000400,
			RENDER_MOTION_BLUR	= 0x000800,
			RENDER_SCATTERING	= 0x001000,
			RENDER_VOLUMETRIC	= 0x002000,
			RENDER_GLOW			= 0x004000,
			RENDER_DOF			= 0x008000,
			RENDER_HDR			= 0x010000,
			
			INFO_ALL = 0xffffffff,
		};
		
		string variable_names[] = (
			
			"show_profiler",
			"show_visualizer",
			
			"render_show_queries",
			"render_show_triangles",
			"render_show_decals",
			"render_show_scissors",
			"render_show_mipmaps",
			"render_show_normals",
			"render_show_textures",
			"render_show_occluder",
			"render_show_splits",
			"render_show_lights",
			"render_show_penumbra",
			"render_show_occlusion",
			
			"world_show_spatial",
		);
		int variables[variable_names.size()];
		
		int controls_states[] = (
			CONTROLS_STATE_SAVE,
			CONTROLS_STATE_RESTORE,
			CONTROLS_STATE_SCREENSHOT,
		);
		int controls_keys[controls_states.size()];
		
		/*
		 */
		void save_states() {
			foreach(string name, i = 0; variable_names; i++) {
				variables[i] = engine.console.getInt(name);
				engine.console.setInt(name,0);
			}
			foreach(int state, i = 0; controls_states; i++) {
				controls_keys[i] = engine.controls.getStateKey(state);
				engine.controls.setStateKey(state,0);
			}
		}
		
		void restore_states() {
			foreach(string name, i = 0; variable_names; i++) {
				engine.console.setInt(name,variables[i]);
			}
			foreach(int state, i = 0; controls_states; i++) {
				engine.controls.setStateKey(state,controls_keys[i]);
			}
		}
		
		/*
		 */
		string get_system(string c[]) {
			
			string result = "";
			
			result += c[0] + "Platform:" + c[1] + engine.console.getString("system_info") + c[2];
			result += c[0] + "CPU model:" + c[1] + trim(re_replace(engine.console.getString("cpu_info"),".+( [0-9]+MHz .+$)","")) + format(" (%dMHz) x%d",engine.console.getInt("cpu_frequency"),engine.console.getInt("cpu_count")) + c[2];
			result += c[0] + "GPU model:" + c[1] + trim(engine.console.getString("gpu_info")) + format(" (%dMB) x%d",engine.console.getInt("gpu_memory"),engine.console.getInt("gpu_count")) + c[2];
			
			return result;
		}
		
		string get_settings(string c[]) {
			
			string result = "";
			
			// gpu information
			string video_app = engine.console.getString("video_app");
			if(video_app == "opengl") video_app = "OpenGL";
			if(video_app == "direct3d9") video_app = "Direct3D9";
			if(video_app == "direct3d11") video_app = "Direct3D11";
			result += c[0] + "Render:" + c[1] + video_app + c[2];
			
			// video mode
			string multisample[] = (""," 2xAA"," 4xAA"," 8xAA");
			result += c[0] + "Mode:" + c[1] + format("%dx%d",engine.app.getWidth(),engine.app.getHeight());
			result += multisample[engine.console.getInt("video_multisample")];
			result += (engine.console.getInt("video_fullscreen")) ? " fullscreen" : " windowed";
			result += c[2];
			
			// qualities
			string quality[] = (c[3] + "low" + c[4],c[1] + "medium" + c[2],c[1] + "high" + c[2]);
			if(flags & RENDER_SHADERS) result += c[0] + "Shaders:" + quality[engine.render.getShaders()];
			if(flags & RENDER_TEXTURES) result += c[0] + "Textures:" + quality[engine.render.getTextures()];
			
			// texture filter
			string filter[] = (c[3] + "bilinear" + c[4],c[1] + "trilinear" + c[2]);
			if(flags & RENDER_FILTER) result += c[0] + "Filter:" + filter[engine.console.getInt("render_filter")];
			
			// texture anisotropy
			string anisotropy[] = (c[3] + "disabled" + c[4],c[1] + "2x" + c[2],c[1] + "4x" + c[2],c[1] + "8x" + c[2],c[1] + "16x" + c[2]);
			if(flags & RENDER_ANISOTROPY) result += c[0] + "Anisotropy:" + anisotropy[engine.console.getInt("render_anisotropy")];
			
			// render settings
			string state[] = (c[3] + "disabled" + c[4],c[1] + "enabled" + c[2]);
			if(flags & RENDER_TESSELLATION) result += c[0] + "Tessellation:" + state[(engine.render.getTessellation() != 0)];
			if(flags & RENDER_REFLECTION) result += c[0] + "Reflection:" + state[(engine.render.getReflection() != 0 && engine.console.getInt("render_skip_reflection") == 0)];
			if(flags & RENDER_TRANSLUCENT) result += c[0] + "Translucence:" + state[(engine.render.getTranslucent() != 0 && engine.console.getInt("render_skip_translucent") == 0)];
			if(flags & RENDER_PARALLAX) result += c[0] + "Parallax:" + state[(engine.render.getParallax() != 0 && engine.console.getInt("render_skip_deferred") == 0)];
			if(flags & RENDER_OCCLUSION) result += c[0] + "Occlusion:" + state[(engine.render.getOcclusion() != 0 && engine.console.getInt("render_skip_occlusion") == 0)];
			if(flags & RENDER_REFRACTION) result += c[0] + "Refraction:" + state[(engine.render.getRefraction() != 0 && engine.console.getInt("render_skip_refraction") == 0)];
			if(flags & RENDER_MOTION_BLUR) result += c[0] + "Motion blur:" + state[(engine.render.getMotionBlur() != 0 && engine.console.getInt("render_skip_motion_blur") == 0)];
			if(flags & RENDER_SCATTERING) result += c[0] + "Scattering:" + state[(engine.render.getScattering() != 0 && engine.console.getInt("render_skip_scattering") == 0)];
			if(flags & RENDER_VOLUMETRIC) result += c[0] + "Volumetric:" + state[(engine.render.getVolumetric() != 0 && engine.console.getInt("render_skip_scattering") == 0)];
			if(flags & RENDER_GLOW) result += c[0] + "Glow:" + state[(engine.render.getGlow() != 0 && engine.console.getInt("render_skip_post_materials") == 0)];
			if(flags & RENDER_DOF) result += c[0] + "DOF:" + state[(engine.render.getDOF() != 0 && engine.console.getInt("render_skip_post_materials") == 0)];
			if(flags & RENDER_HDR) result += c[0] + "HDR:" + state[(engine.render.getHDR() != 0 && engine.console.getInt("render_skip_post_materials") == 0)];
			
			// extern settings
			forloop(int i = 0; settings.size(); 3) {
				result += c[0] + settings[i];
				if(strlen(settings[i + 1])) result += c[3] + settings[i + 1] + c[4];
				else result += c[1] + settings[i + 2] + c[2];
			}
			
			return result;
		}
		
		/*
		 */
		void save_result(string target) {
			
			string result = "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n";
			result += "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n";
			result += "<html><head>\n";
			result += "<title>Unigine benchmark results</title>\n";
			result += "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n";
			result += "<style type=\"text/css\">\n";
			result += "body { background: #292929; color: #bbbbbb; margin-right: 20px; margin-left: 20px; font-size: 14px; font-family: Arial, sans-serif, sans; }\n";
			result += "a { }\n";
			result += "a:link { color: #00aeef; }\n";
			result += "a:hover { color: #00aeef; }\n";
			result += "a:active { color: #00aeef; }\n";
			result += "a:visited { color: #00aeef; }\n";
			result += "h1 { color: #ff9c00; font-size: 36px; text-align: center; }\n";
			result += "h2 { font-size: 24px; text-align: center; }\n";
			result += "div.result { color: #00aeef; }\n";
			result += "div.highlight { color: #ffffff; }\n";
			result += "div.engine { margin-top: 50px; text-align: center; }\n";
			result += "div.copyright { margin: 10px; text-align: center; }\n";
			result += "table.result { border: 0px; margin-left: auto; margin-right: auto; }\n";
			result += "table.result td { border: 0px; padding: 3px; font-size: 200%; }\n";
			result += "table.detail { border: 1px solid #6a6a6a; border-collapse: collapse; margin-left: auto; margin-right: auto; }\n";
			result += "table.detail td { border: 1px solid #6a6a6a; padding: 3px; }\n";
			result += ".right { text-align: right; }\n";
			result += "</style></head><body>\n";
			
			result += format("<h1>%s</h1>\n",name);
			result += "<table class=\"result\">\n";
			result += format("<tr><td class=\"right\">FPS:</td><td><div class=\"result\"><strong>%.1f</strong></div></td></tr>\n",getFps());
			result += format("<tr><td class=\"right\">Score:</td><td><div class=\"result\"><strong>%.0f</strong></div></td></tr>\n",getScore());
			result += format("<tr><td class=\"right\">Min FPS:</td><td><div class=\"result\"><strong>%.1f</strong></div></td></tr>\n",min_fps);
			result += format("<tr><td class=\"right\">Max FPS:</td><td><div class=\"result\"><strong>%.1f</strong></div></td></tr>\n",max_fps);
			result += "</table>\n";
			
			result += "<h2>System</h2>\n";
			result += "<table class=\"detail\">\n";
			result += get_system(("<tr><td class=\"right\">","</td><td><div class=\"highlight\">","</div></td></tr>\n"));
			result += "</table>\n";
			
			result += "<h2>Settings</h2>\n";
			result += "<table class=\"detail\">\n";
			result += get_settings(("<tr><td class=\"right\">","</td><td><div class=\"highlight\">","</div></td></tr>\n","</td><td>","</td></tr>\n"));
			result += "</table>\n";
			
			result += "<div class=\"engine\">Powered by <a href=\"http://unigine.com/products/unigine/\">UNIGINE Engine</a></div>\n";
			result += "<div class=\"copyright\"><a href=\"http://unigine.com/\">Unigine Corp.</a> &copy; 2005-2014</div>\n";
			result += "</body></html>\n";
			
			File file = new File();
			if(file.open(target,"wb")) {
				file.puts(result);
				file.close();
			} else {
				log.error("Unigine::Benchmark::save_result(): can't create \"%s\" file\n",target);
			}
			delete file;
		}
		
		/*
		 */
		void run() {
			
			assert(activity == 0);
			activity = 1;
			
			save_states();
			
			engine.gui.addChild(benchmark_l,GUI_ALIGN_OVERLAP);
			engine.gui.addChild(panel_gb,GUI_ALIGN_OVERLAP);
			benchmark_l.setPermanentFocus();
			
			time = 0.0f;
			frames = 0;
			
			fps_time = 0.0f;
			fps_frames = 0;
			
			min_fps = 1000.0f;
			max_fps = 0.0f;
			
			log.message("Benchmark running\n");
		}
		
		void stop() {
			
			assert(activity == 1);
			activity = 0;
			
			restore_states();
			
			engine.gui.removeChild(benchmark_l);
			engine.gui.removeChild(panel_gb);
			
			log.message("Benchmark stopped\n");
		}
		
		void done() {
			
			assert(activity == 1);
			activity = 0;
			
			restore_states();
			
			engine.gui.removeChild(benchmark_l);
			engine.gui.removeChild(panel_gb);
			
			string result = format("<center><font face=\"core/gui/font.ttf\"><font face=\"core/gui/fontb.ttf\" size=%%200 color=#ff9900>%s</font><br/><br/><font size=%%200><table>",name);
			result += format("<tr><td><p align=right>FPS: </p></td><td><p align=left><font face=\"core/gui/fontb.ttf\" color=#ff9900>%.1f</font></p></td></tr>",getFps());
			result += format("<tr><td><p align=right>Score: </p></td><td><p align=left><font face=\"core/gui/fontb.ttf\" color=#ff9900>%.0f</font></p></td></tr>",getScore());
			result += "</table></font><br/>";
			
			result += "<font size=%150><table>";
			result += format("<tr><td><p align=right>Min FPS: </p></td><td><p align=left><font face=\"core/gui/fontb.ttf\" color=#ff9900>%.1f</font></p></td></tr>",min_fps);
			result += format("<tr><td><p align=right>Max FPS: </p></td><td><p align=left><font face=\"core/gui/fontb.ttf\" color=#ff9900>%.1f</font></p></td></tr>",max_fps);
			result += "</table></font><br/>";
			
			result += "<font face=\"core/gui/fontb.ttf\" size=%150 color=#ffffff>System</font><br/><br/><table color=#888888 space=3>";
			result += get_system(("<tr><td><p align=right>","</p></td><td><p align=left><font color=#ffffff>","</font></p></td></tr>"));
			result += "</table><br/>";
			
			result += "<font face=\"core/gui/fontb.ttf\" size=%150 color=#ffffff>Settings</font><br/><br/><table color=#888888 space=3>";
			result += get_settings(("<tr><td><p align=right>","</p></td><td><p align=left><font color=#ffffff>","</font></p></td></tr>","</p></td><td><p align=left><font color=#888888>","</font></p></td></tr>"));
			result += "</table></font></center> ";
			
			result_l.setText(result);
			
			engine.gui.addChild(result_d,GUI_ALIGN_OVERLAP | GUI_ALIGN_CENTER);
			result_d.setPermanentFocus();
			
			log.message("Benchmark results:\n");
			log.message("Time:\t%g\nFrames:\t%d\nFPS:\t%g\nMin FPS:\t%g\nMax FPS:\t%g\nScore:\t%g\n",time,frames,getFps(),min_fps,max_fps,getScore());
		}
		
		int getActivity() {
			return activity;
		}
		
		float getFps() {
			return frames / time;
		}
		
		float getScore() {
			return frames / time * scale;
		}
		
		/*
		 */
		void update(string scene) {
			
			if(activity == 0) return;
			
			// stop benchmark
			if(engine.app.clearKeyState(APP_KEY_ESC)) {
				stop();
				return;
			}
			
			// update panel position
			int width = engine.app.getWidth();
			int height = engine.app.getHeight();
			benchmark_l.setFontSize(height / 24);
			benchmark_l.setPosition((width - benchmark_l.getWidth()) / 2,benchmark_l.getHeight() * 2);
			panel_gb.setPosition(width - panel_gb.getWidth(),height - panel_gb.getHeight());
			
			// update panel information
			time_l.setText(format("%.1fs",time));
			frames_l.setText(format("%d",frames));
			if(time > 0.0f && min_fps < max_fps) {
				fps_l.setText(format("%.1f",frames / time));
				min_fps_l.setText(format("%.1f",min_fps));
				max_fps_l.setText(format("%.1f",max_fps));
			} else {
				fps_l.setText("N/A");
				min_fps_l.setText("N/A");
				max_fps_l.setText("N/A");
			}
			scene_l.setText(scene);
			
			// check the first frame flag
			if(engine.render.isFirstFrame()) {
				first_frame = 2;
				return;
			} else if(first_frame > 0) {
				first_frame--;
				return;
			}
			
			// frame time
			float ftime = engine.app.getFTime();
			time += ftime;
			frames++;
			
			fps_frames++;
			fps_time += ftime;
			if(fps_frames > 16) {
				float fps = fps_frames / fps_time;
				min_fps = min(min_fps,fps);
				max_fps = max(max_fps,fps);
				fps_time = 0.0f;
				fps_frames = 0;
			}
			
			// hide mouse
			engine.gui.setMouseCursor(GUI_CURSOR_NONE);
		}
		
		/*
		 */
		void save_clicked() {
			engine.gui.addChild(result_df,GUI_ALIGN_OVERLAP | GUI_ALIGN_CENTER);
			result_df.setFile(format("%s_%s.html",replace(name," ","_"),date("%Y%02M%02D_%02h%02m")));
			result_df.setPermanentFocus();
		}
		
		void close_clicked() {
			engine.gui.removeChild(result_d);
		}
		
		/*
		 */
		void ok_clicked() {
			engine.gui.removeChild(result_df);
			save_result(result_df.getFile());
		}
		
		void cancel_clicked() {
			engine.gui.removeChild(result_df);
		}
		
		/*
		 */
		void result_redirector(Benchmark benchmark,string name) {
			benchmark.call(name);
		}
		
		/*
		 */
		void init() {
			
			Gui gui = engine.getGui();
			
			benchmark_l = new WidgetLabel(gui,"Benchmarking...");
			benchmark_l.setFontColor(gui.getDefaultColor());
			benchmark_l.setFont("core/gui/font.ttf");
			
			panel_gb = new WidgetGroupBox(gui);
			panel_gb.setBackground(1);
			panel_gb.setSpace(4,4);
			
			WidgetGridBox gridbox = new WidgetGridBox(gui,2,4,4);
			panel_gb.addChild(gridbox,GUI_ALIGN_EXPAND);
			
			fps_l = new WidgetLabel(gui);
			gridbox.addChild(new WidgetLabel(gui,"FPS:"),GUI_ALIGN_RIGHT);
			gridbox.addChild(fps_l,GUI_ALIGN_LEFT);
			
			time_l = new WidgetLabel(gui);
			gridbox.addChild(new WidgetLabel(gui,"Time:"),GUI_ALIGN_RIGHT);
			gridbox.addChild(time_l,GUI_ALIGN_LEFT);
			
			frames_l = new WidgetLabel(gui);
			gridbox.addChild(new WidgetLabel(gui,"Frames:"),GUI_ALIGN_RIGHT);
			gridbox.addChild(frames_l,GUI_ALIGN_LEFT);
			
			min_fps_l = new WidgetLabel(gui);
			gridbox.addChild(new WidgetLabel(gui,"Min FPS:"),GUI_ALIGN_RIGHT);
			gridbox.addChild(min_fps_l,GUI_ALIGN_LEFT);
			
			max_fps_l = new WidgetLabel(gui);
			gridbox.addChild(new WidgetLabel(gui,"Max FPS:"),GUI_ALIGN_RIGHT);
			gridbox.addChild(max_fps_l,GUI_ALIGN_LEFT);
			
			scene_l = new WidgetLabel(gui);
			gridbox.addChild(new WidgetLabel(gui,"Scene:"),GUI_ALIGN_RIGHT);
			gridbox.addChild(scene_l,GUI_ALIGN_LEFT);
			
			gridbox.addChild(new WidgetLabel(gui,"Press ESC to Cancel"),GUI_ALIGN_EXPAND);
			gridbox.addChild(new WidgetVBox(gui));
			
			result_d = new WidgetDialog(gui);
			result_d.setOkText("Save");
			result_d.setCancelText("Close");
			WidgetButton ok_b = result_d.getOkButton();
			WidgetButton cancel_b = result_d.getCancelButton();
			ok_b.setCallback(GUI_CLICKED,"Unigine::Benchmark::result_redirector",this,"save_clicked");
			cancel_b.setCallback(GUI_CLICKED,"Unigine::Benchmark::result_redirector",this,"close_clicked");
			
			result_l = new WidgetLabel(gui);
			result_d.addChild(result_l);
			result_l.setFontRich(1);
			
			result_df = new WidgetDialogFile(gui,"Select file");
			if(engine.config.isExist("benchmark_tabs")) result_df.setTabs(engine.config.getString("benchmark_tabs"));
			ok_b = result_df.getOkButton();
			cancel_b = result_df.getCancelButton();
			ok_b.setCallback(GUI_CLICKED,"Unigine::Benchmark::result_redirector",this,"ok_clicked");
			cancel_b.setCallback(GUI_CLICKED,"Unigine::Benchmark::result_redirector",this,"cancel_clicked");
			#ifdef _WIN32
				result_df.setPath(getenv("USERPROFILE") + "/");
			#else
				result_df.setPath(getenv("HOME") + "/");
			#endif
		}
		
		void shutdown() {
			
			engine.config.setString("benchmark_tabs",result_df.getTabs());
			WidgetButton ok_b = result_d.getOkButton();
			WidgetButton cancel_b = result_d.getCancelButton();
			ok_b.setCallback(GUI_CLICKED,NULL);
			cancel_b.setCallback(GUI_CLICKED,NULL);
		}
		
		/*
		 */
		Benchmark(string name_,float scale_,int flags_ = Unigine::Benchmark::INFO_ALL) {
			name = name_;
			scale = scale_;
			flags = flags_;
			init();
		}
		
		~Benchmark() {
			shutdown();
		}
		
		/*
		 */
		void setFlags(int flags_) {
			flags = flags_;
		}
		
		void setSettings(string settings_[]) {
			settings = settings_;
		}
		
		/*
		 */
		void __save__() {
			
		}
		
		void __restore__() {
			init();
			activity = 0;
		}
	};
	
} /* namespace Unigine */

#endif /* __UNIGINE_BENCHMARK_H__ */
