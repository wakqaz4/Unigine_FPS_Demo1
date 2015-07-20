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


#ifndef __UNIGINE_AUTOMATION_H__
#define __UNIGINE_AUTOMATION_H__

/*
 */
namespace Unigine {
	
	/*
	 */
	class Automation {
		
		int activity;				// automation status
		int first_frame;			// first frame status
		
		string name;				// automation name
		float scale;				// automation scale
		
		int num_scenes;				// number of scenes
		string set_scene;			// set scene callback
		string get_scene;			// get scene callback
		
		int iterations;				// number of iterations
		int scenes[0];				// selected scenes
		int scene;					// current scene
		
		File result_file;			// result file
		File scores_file;			// scores file
		File frames_file;			// frames file
		File temperature_file;		// temperature file
		File frequency_file;		// frequency file
		
		string scores_format;		// scores format
		string frames[0];			// frames data
		string temperature[0];		// temperature data
		string frequency[0];		// frequency data
		
		float log_step;				// log step
		float log_time;				// log time
		int log_frames;				// log frames
		
		float scene_time;			// scene time
		int scene_frames;			// scene frames
		float scene_min_fps;		// minimal fps
		float scene_max_fps;		// maximal fps
		
		float total_time;			// total time
		int total_frames;			// total frames
		float total_min_fps;		// minimal fps
		float total_max_fps;		// maximal fps
		
		/*
		 */
		int getActivity() {
			return activity;
		}
		
		/*
		 */
		void print(string str) {
			log.message("%s",str);
			if(result_file != NULL) result_file.puts(str);
		}
		
		/*
		 */
		void print_title() {
			
			// frames
			if(frames_file != NULL) frames.append("Scene\tStepTime\tRealTime\tFPS\n");
			
			// temperature
			#ifdef HAS_GPU_MONITOR
				if(temperature_file != NULL) {
					string data = "Scene\tStepTime\tRealTime\tMaxTemp";
					forloop(int i = 0; engine.gpumonitor.getNumMonitors()) {
						GPUMonitor monitor = engine.gpumonitor.getMonitor(i);
						forloop(int j = 0; monitor.getNumAdapters()) {
							string name = replace(monitor.getAdapterName(j)," ","_");
							data += format("\t%s_Temp",name);
						}
					}
					temperature.append(data + "\n");
				}
			#endif
			
			// frequency
			#ifdef HAS_GPU_MONITOR
				if(frequency_file != NULL) {
					string data = "Scene\tStepTime\tRealTime";
					forloop(int i = 0; engine.gpumonitor.getNumMonitors()) {
						GPUMonitor monitor = engine.gpumonitor.getMonitor(i);
						forloop(int j = 0; monitor.getNumAdapters()) {
							string name = replace(monitor.getAdapterName(j)," ","_");
							if(monitor.getCoreClock(j) > 0.0f) data += format("\t%s_Core",name);
							if(monitor.getMemoryClock(j) > 0.0f) data += format("\t%s_Memory",name);
							if(monitor.getShaderClock(j) > 0.0f) data += format("\t%s_Shader",name);
						}
					}
					frequency.append(data + "\n");
				}
			#endif
		}
		
		/*
		 */
		void print_log() {
			
			// step time
			float step_time = total_time;
			if(log_step > EPSILON) step_time = floor(step_time / log_step) * log_step;
			
			// frames
			if(frames_file != NULL) frames.append(format("%d\t%f\t%f\t%f\n",scenes[scene],step_time,total_time,log_frames / log_time));
			
			// temperature
			#ifdef HAS_GPU_MONITOR
				if(temperature_file != NULL) {
					string data = format("%d\t%f\t%f\t%f",scenes[scene],step_time,total_time,engine.gpumonitor.getMaxTemperature());
					forloop(int i = 0; engine.gpumonitor.getNumMonitors()) {
						GPUMonitor monitor = engine.gpumonitor.getMonitor(i);
						forloop(int j = 0; monitor.getNumAdapters()) {
							data += format("\t%f",monitor.getTemperature(j));
						}
					}
					temperature.append(data + "\n");
				}
			#endif
			
			// frequency
			#ifdef HAS_GPU_MONITOR
				if(frequency_file != NULL) {
					string data = format("%d\t%f\t%f",scenes[scene],step_time,total_time);
					forloop(int i = 0; engine.gpumonitor.getNumMonitors()) {
						GPUMonitor monitor = engine.gpumonitor.getMonitor(i);
						forloop(int j = 0; monitor.getNumAdapters()) {
							float core = monitor.getCoreClock(j);
							float memory = monitor.getMemoryClock(j);
							float shader = monitor.getShaderClock(j);
							if(core > 0.0f) data += format("\t%f",core);
							if(memory > 0.0f) data += format("\t%f",memory);
							if(shader > 0.0f) data += format("\t%f",shader);
						}
					}
					frequency.append(data + "\n");
				}
			#endif
		}
		
		/*
		 */
		void run() {
			
			activity = 1;
			
			iterations = 1;
			
			log_step = 0.0f;
			
			scene_min_fps = INFINITY;
			total_min_fps = INFINITY;
			
			// parse command line arguments
			forloop(int i = 0; engine.getNumArgs() - 1) {
				string arg = engine.getArg(i);
				if(arg == "-iterations") {
					iterations = max(int(engine.getArg(++i)),1);
					log.message("Number of iterations: %d\n",iterations);
				}
				else if(arg == "-scenes") {
					strsplit(engine.getArg(++i),",",scenes);
					if(scenes.size()) {
						string buffer;
						for(int j = 0; j < scenes.size(); j++) {
							scenes[j] = int(scenes[j]);
							if(scenes[j] >= 0 && scenes[j] < num_scenes) {
								if(strlen(buffer)) buffer += ",";
								buffer += string(scenes[j]);
							} else {
								scenes.remove(j--);
							}
						}
						log.message("Selected scenes: %s\n",buffer);
					}
				}
				else if(arg == "-result") {
					result_file = new File();
					if(result_file.open(engine.getArg(++i),"w")) {
						log.message("Result file: %s\n",result_file.getName());
					} else {
						delete result_file;
					}
				}
				else if(arg == "-scores") {
					scores_file = new File();
					if(scores_file.open(engine.getArg(++i),"w")) {
						log.message("Scores file: %s\n",scores_file.getName());
					} else {
						delete scores_file;
					}
				}
				else if(arg == "-format") {
					scores_format = engine.getArg(++i);
					log.message("Scores format: %s\n",scores_format);
				}
				else if(arg == "-step") {
					log_step = max(float(engine.getArg(++i)),EPSILON);
					log.message("Log step: %g\n",log_step);
				}
				else if(arg == "-frames") {
					frames_file = new File();
					if(frames_file.open(engine.getArg(++i),"w")) {
						log.message("Frames file: %s\n",frames_file.getName());
					} else {
						delete frames_file;
					}
				}
				else if(arg == "-temperature") {
					#ifdef HAS_GPU_MONITOR
						temperature_file = new File();
						if(temperature_file.open(engine.getArg(++i),"w")) {
							log.message("Temperature file: %s\n",temperature_file.getName());
						} else {
							delete temperature_file;
						}
					#else
						log.error("GPUMontor plugin is not loaded\n");
						i++;
					#endif
				}
				else if(arg == "-frequency") {
					#ifdef HAS_GPU_MONITOR
						frequency_file = new File();
						if(frequency_file.open(engine.getArg(++i),"w")) {
							log.message("Frequency file: %s\n",frequency_file.getName());
						} else {
							delete frequency_file;
						}
					#else
						log.error("GPUMontor plugin is not loaded\n");
						i++;
					#endif
				}
			}
			
			// all scenes
			if(scenes.size() == 0) {
				forloop(int i = 0; num_scenes) {
					scenes.append(i);
				}
			}
			
			// automation name
			print(format("\n%s\n\n",name));
			
			// system info
			print("Platform:  " + engine.console.getString("system_info") + "\n");
			print("CPU model: " + re_replace(engine.console.getString("cpu_info"),".+( [0-9]+MHz .+$)","") + format(" (%dMHz) x%d\n",engine.console.getInt("cpu_frequency"),engine.console.getInt("cpu_count")));
			print("GPU model: " + engine.console.getString("gpu_info") + format(" (%dMB) x%d\n",engine.console.getInt("gpu_memory"),engine.console.getInt("gpu_count")));
			
			// video app
			print("\nRender:        " + engine.console.getString("video_app") + "\n");
			
			// video mode
			string multisample[] = (""," 2xAA"," 4xAA"," 8xAA");
			string fullscreen[] = ("windowed", "fullscreen");
			print("Mode:          " + format("%dx%d%s %s\n",engine.app.getWidth(),engine.app.getHeight(),multisample[engine.console.getInt("video_multisample")],fullscreen[engine.console.getInt("video_fullscreen")]));
			
			// texture filter
			string filter[] = ("bilinear","trilinear");
			print("Filter:        " + filter[engine.console.getInt("render_filter")] + "\n");
			
			// texture anisotropy
			string anisotropy[] = ("disabled","2x","4x","8x","16x");
			print("Anisotropy:    " + anisotropy[engine.console.getInt("render_anisotropy")] + "\n");
			
			// render qualities
			string quality[] = ("low","medium","high");
			print("Shaders:       " + quality[engine.render.getShaders()] + "\n");
			print("Textures:      " + quality[engine.render.getTextures()] + "\n");
			
			// render states
			string state[] = ("disabled","enabled","mode 2","mode 3");
			print("Tessellation:  " + state[engine.render.getTessellation()] + "\n");
			print("GeomeryShader: " + state[engine.render.getGeometryShader()] + "\n");
			print("Stereo:        " + state[engine.render.getStereo()] + "\n");
			print("AlphaFade:     " + state[engine.render.getAlphaFade()] + "\n");
			print("Reflection:    " + state[engine.render.getReflection() * (engine.console.getInt("render_skip_reflection") == 0)] + "\n");
			print("Translucence:  " + state[engine.render.getTranslucent() * (engine.console.getInt("render_skip_translucent") == 0)] + "\n");
			print("Deferred:      " + state[engine.render.getDeferred() * (engine.console.getInt("render_skip_deferred") == 0)] + "\n");
			print("Parallax:      " + state[engine.render.getParallax() * (engine.console.getInt("render_skip_deferred") == 0)] + "\n");
			print("Occlusion:     " + state[engine.render.getOcclusion() * (engine.console.getInt("render_skip_occlusion") == 0)] + "\n");
			print("Auxiliary:     " + state[engine.render.getAuxiliary() * (engine.console.getInt("render_skip_auxiliary") == 0)] + "\n");
			print("Refraction:    " + state[engine.render.getRefraction() * (engine.console.getInt("render_skip_refraction") == 0)] + "\n");
			print("MotionBlur:    " + state[engine.render.getMotionBlur() * (engine.console.getInt("render_skip_motion_blur") == 0)] + "\n");
			print("Scattering:    " + state[engine.render.getScattering() * (engine.console.getInt("render_skip_scattering") == 0)] + "\n");
			print("Volumetric:    " + state[engine.render.getVolumetric() * (engine.console.getInt("render_skip_scattering") == 0)] + "\n");
			print("Glow:          " + state[engine.render.getGlow() * (engine.console.getInt("render_skip_post_materials") == 0)] + "\n");
			print("DOF:           " + state[engine.render.getDOF() * (engine.console.getInt("render_skip_post_materials") == 0)] + "\n");
			print("HDR:           " + state[engine.render.getHDR() * (engine.console.getInt("render_skip_post_materials") == 0)] + "\n");
			
			// print title
			print_title();
			
			// set first scene
			print("\nAutomation started\n\n");
			print(format("Scene %d started\n",scenes[scene]));
			call(set_scene,scenes[scene]);
		}
		
		/*
		 */
		void update() {
			
			if(activity == 0) return;
			
			// scene is changed
			if(call(get_scene) != scenes[scene]) {
				
				// scene result
				print(format("Scene time:   %g\n",scene_time));
				print(format("Scene frames: %d\n",scene_frames));
				print(format("Scene FPS:    %g / %g / %g\n",scene_frames / scene_time,scene_min_fps,scene_max_fps));
				print(format("Scene scores: %g\n\n",scene_frames / scene_time * scale));
				
				// last scene
				if(++scene == scenes.size()) {
					
					// iteration result
					print(format("Total time:   %g\n",total_time));
					print(format("Total frames: %d\n",total_frames));
					print(format("Total FPS:    %g / %g / %g\n",total_frames / total_time,total_min_fps,total_max_fps));
					print(format("Total scores: %g\n\n",total_frames / total_time * scale));
					
					// result file
					if(result_file != NULL) {
						engine.app.stopFps();
						result_file.flush();
						engine.app.startFps();
					}
					// scores file
					if(scores_file != NULL) {
						engine.app.stopFps();
						string quality[] = ( "low", "medium", "high" );
						string anisotropy[] = ( "1x", "2x", "4x", "8x", "16x" );
						string multisample[] = ( "0xAA", "2xAA", "4xAA", "8xAA" );
						forloop(int i = 0; strlen(scores_format)) {
							int c = scores_format[i];
							if(c == '$' && i + 1 < strlen(scores_format)) {
								c = scores_format[++i];
								if(c == 'v') scores_file.printf("%dx%d",engine.app.getWidth(),engine.app.getHeight());
								else if(c == 'm') scores_file.printf("%s",multisample[engine.console.getInt("video_multisample")]);
								else if(c == 'a') scores_file.printf("%s",anisotropy[engine.console.getInt("render_anisotropy")]);
								else if(c == 's') scores_file.printf("%s",quality[engine.console.getInt("render_shaders")]);
								else if(c == 't') scores_file.printf("%s",quality[engine.console.getInt("render_textures")]);
								else if(c == 'c') scores_file.printf("%s",engine.console.getString("cpu_info"));
								else if(c == 'g') scores_file.printf("%s %dMB",engine.console.getString("gpu_info"),engine.console.getInt("gpu_memory"));
								else if(c == 'S') scores_file.printf("%f",total_frames / total_time * scale);
								else if(c == 'F') scores_file.printf("%f",total_frames / total_time);
								else if(c == 'z') scores_file.printf("%f",total_min_fps);
								else if(c == 'x') scores_file.printf("%f",total_max_fps);
								else if(c == 'A') scores_file.printf("%s",engine.console.getString("video_app"));
								else if(c == '$') scores_file.printf("$");
								else log.warning("Unknown scores format '%c'\n",c);
							} else if(c == '\\' && i + 1 < strlen(scores_format)) {
								c = scores_format[++i];
								if(c == 'n') scores_file.printf("\n");
								else if(c == 'r') scores_file.printf("\r");
								else if(c == 't') scores_file.printf("\t");
								else log.warning("Unknown scores format '\\%c'\n",c);
							} else {
								scores_file.printf("%c",c);
							}
						}
						engine.app.startFps();
					}
					// frames file
					if(frames_file != NULL) {
						engine.app.stopFps();
						forloop(int i = 0; frames.size()) {
							frames_file.puts(frames[i]);
						}
						frames.clear();
						frames_file.flush();
						engine.app.startFps();
					}
					// temperature file
					if(temperature_file != NULL) {
						engine.app.stopFps();
						forloop(int i = 0; temperature.size()) {
							temperature_file.puts(temperature[i]);
						}
						temperature.clear();
						temperature_file.flush();
						engine.app.startFps();
					}
					// frequency file
					if(frequency_file != NULL) {
						engine.app.stopFps();
						forloop(int i = 0; frequency.size()) {
							frequency_file.puts(frequency[i]);
						}
						frequency.clear();
						frequency_file.flush();
						engine.app.startFps();
					}
					
					// last iteration
					if(--iterations == 0) {
						print("Automation finished\n");
						engine.console.run("quit");
						activity = 0;
						return;
					}
					// next iteration
					else {
						
						scene = 0;
						print(format("Scene %d started\n",scenes[scene]));
						call(set_scene,scenes[scene]);
						
						log_time = 0.0f;
						log_frames = 0;
						
						scene_time = 0.0f;
						scene_frames = 0;
						scene_min_fps = INFINITY;
						scene_max_fps = 0.0f;
						
						total_time = 0.0f;
						total_frames = 0;
						total_min_fps = INFINITY;
						total_max_fps = 0.0f;
					}
				}
				// next scene
				else {
					
					print(format("Scene %d started\n",scenes[scene]));
					call(set_scene,scenes[scene]);
					
					log_time = 0.0f;
					log_frames = 0;
					
					scene_time = 0.0f;
					scene_frames = 0;
					scene_min_fps = INFINITY;
					scene_max_fps = 0.0f;
				}
			}
			
			// check the first frame flag
			if(engine.render.isFirstFrame()) {
				first_frame = 1;
				return;
			} else if(first_frame > 0) {
				first_frame--;
				return;
			}
			
			// frame time
			float ftime = engine.app.getFTime();
			float fps = 1.0f / ftime;
			
			// per frame log
			if(scene_frames != 0 && (frames_file != NULL || temperature_file != NULL || frequency_file != NULL)) {
				log_time += ftime;
				log_frames++;
				if(log_time > log_step) {
					print_log();
					log_time = 0.0f;
					log_frames = 0;
				}
			}
			
			// scene
			scene_frames++;
			scene_time += ftime;
			scene_min_fps = min(scene_min_fps,fps);
			scene_max_fps = max(scene_max_fps,fps);
			
			// total
			total_frames++;
			total_time += ftime;
			total_min_fps = min(total_min_fps,fps);
			total_max_fps = max(total_max_fps,fps);
			
			// hide mouse
			if(engine.system.call("systemGetActivity") == 0) {
				engine.gui.setMouseCursor(GUI_CURSOR_NONE);
			}
		}
		
		/*
		 */
		Automation(string name_,float scale_,int num_scenes_,string set_scene_,string get_scene_) {
			
			name = name_;
			scale = scale_;
			num_scenes = num_scenes_;
			set_scene = set_scene_;
			get_scene = get_scene_;
		}
		
		~Automation() {
			
			// result file
			if(result_file != NULL) {
				result_file.close();
				delete result_file;
			}
			
			// scores file
			if(scores_file != NULL) {
				scores_file.close();
				delete scores_file;
			}
			
			// frames file
			if(frames_file != NULL) {
				frames_file.close();
				delete frames_file;
			}
			
			// temperature file
			if(temperature_file != NULL) {
				temperature_file.close();
				delete temperature_file;
			}
			
			// frequency file
			if(frequency_file != NULL) {
				frequency_file.close();
				delete frequency_file;
			}
		}
	};
	
} /* namespace Unigine */

#endif /* __UNIGINE_AUTOMATION_H__ */
