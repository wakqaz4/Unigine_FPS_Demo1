/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_tools.h
 * Desc:    Unigine editor
 * Version: 1.51
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Anna Ishina <anet@unigine.com>
 *          Andrey Viktorov <unclebob@unigine.com>
 *          Alexey Egorov <evilsprit@unigine.com>
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

#ifdef EDITOR_LUT
	#include <core/systems/widgets/widget.h>
	#include <core/systems/widgets/widget_line_color.h>
#endif

/*
 */
namespace Tools {
	
	/*
	 */
	DECLARE_WINDOW
	
	WidgetTabBox tabbox;						// tools tabbox
	
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
		WidgetCheckBox system_mouse_cb;				// system mouse toggle
		WidgetCheckBox always_update_cb;			// always update toggle
		WidgetCheckBox node_reparent_cb;			// node reparent toggle
		WidgetCheckBox node_name_cb;				// node name toggle
		WidgetCheckBox clone_confirm_cb;			// clone node confirmation toggle
		WidgetCheckBox keep_backups_cb;				// keep baackups toggle
		WidgetCheckBox node_materials_cb;			// node materials toggle
		WidgetCheckBox selection_cb;				// selection toggle
		WidgetCheckBox game_camera_cb;				// game camera toggle
		
		WidgetComboBox rotation_order_cb;			// rotation order
		WidgetEditLine size_el;						// handle size
		
		int size;
		
		/*
		 */
		void init() {
			
			system_mouse_cb.setChecked(configGet("tools_system_mouse",1));
			always_update_cb.setChecked(configGet("tools_always_update",0));
			node_reparent_cb.setChecked(configGet("tools_node_reparent",0));
			node_name_cb.setChecked(configGet("tools_node_name",1));
			clone_confirm_cb.setChecked(configGet("tools_clone_confirm",1));
			keep_backups_cb.setChecked(configGet("tools_keep_backups",0));
			node_materials_cb.setChecked(configGet("tools_node_materials",1));
			selection_cb.setChecked(configGet("tools_selection",0));
			game_camera_cb.setChecked(configGet("tools_game_camera",0));
			rotation_order_cb.setCurrentItem(configGet("tools_rotation_order",0));
			size = configGet("tools_size",100);
			size_el.setText(string(size));
			
			engine.visualizer.setSize(size);
			engine.visualizer.setTextureName("editor/gui/nodes_icons_big.png");
		}
		
		void shutdown() {
			
			configSet("tools_system_mouse",system_mouse_cb.isChecked());
			configSet("tools_always_update",always_update_cb.isChecked());
			configSet("tools_node_reparent",node_reparent_cb.isChecked());
			configSet("tools_node_name",node_name_cb.isChecked());
			configSet("tools_clone_confirm",clone_confirm_cb.isChecked());
			configSet("tools_keep_backups",keep_backups_cb.isChecked());
			configSet("tools_node_materials",node_materials_cb.isChecked());
			configSet("tools_selection",selection_cb.isChecked());
			configSet("tools_game_camera",game_camera_cb.isChecked());
			configSet("tools_rotation_order",rotation_order_cb.getCurrentItem());
			configSet("tools_size",size);
		}
		
		/*
		 */
		void size_pressed() {
			size = clamp(int(size_el.getText()),10,400);
			size_el.setText(string(size));
			engine.visualizer.setSize(size);
		}
		
	} /* namespace Common */
	
	/**************************************************************************\
	*
	* Light
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Light {
		
		/*
		 */
		WidgetCheckBox enabled_cb;		// light toggle
		WidgetSprite color_s[2];		// field of view
		WidgetEditLine multiplier_el;	// multiplier
		WidgetEditLine attenuation_el;	// attenuation
		WidgetEditLine radius_el;		// radius
		WidgetEditLine fov_el;			// fov
		
		LightProj light;
		
		/*
		 */
		void init() {
			
			light = new LightProj(vec4_one,1.0f,45.0f,"core/textures/light_base_proj.dds");
			light.setShadow(0);
			
			light.setEnabled(configGet("tools_light_enabled",0));
			light.setColor(configGet("tools_light_color",vec4_one));
			light.setMultiplier(configGet("tools_light_multiplier",1.0f));
			light.setAttenuation(configGet("tools_light_attenuation",1.0f));
			light.setRadius(configGet("tools_light_radius",1.0f));
			light.setFov(configGet("tools_light_fov",45.0f));
			
			enabled_cb.setChecked(light.isEnabled());
			multiplier_el.setText(editorFormat(light.getMultiplier()));
			attenuation_el.setText(editorFormat(light.getAttenuation()));
			radius_el.setText(editorFormat(light.getRadius()));
			fov_el.setText(editorFormat(light.getFov()));
			
			light.setZNear(light.getRadius() / 1000.0f);
		}
		
		void shutdown() {
			
			configSet("tools_light_enabled",light.isEnabled());
			configSet("tools_light_color",light.getColor());
			configSet("tools_light_multiplier",light.getMultiplier());
			configSet("tools_light_attenuation",light.getAttenuation());
			configSet("tools_light_radius",light.getRadius());
			configSet("tools_light_fov",light.getFov());
			
			delete light;
		}
		
		/*
		 */
		void toggle_light() {
			light.setEnabled(!enabled_cb.isChecked());
			enabled_cb.setChecked(!enabled_cb.isChecked());
		}
		
		/*
		 */
		void enabled_clicked() {
			light.setEnabled(enabled_cb.isChecked());
			enabled_cb.setChecked(light.isEnabled());
		}
		
		void color_changed(WidgetDialogColor dialog) {
			light.setColor(dialog.getColor());
			setSpriteColor(color_s,light.getColor());
		}
		
		void color_clicked() {
			dialogColor(TR("Select light color"),functionid(color_changed),light.getColor());
		}
		
		void multiplier_pressed() {
			light.setMultiplier(float(multiplier_el.getText()));
			multiplier_el.setText(editorFormat(light.getMultiplier()));
		}
		
		void attenuation_pressed() {
			light.setAttenuation(float(attenuation_el.getText()));
			attenuation_el.setText(editorFormat(light.getAttenuation()));
		}
		
		void radius_pressed() {
			light.setRadius(float(radius_el.getText()));
			radius_el.setText(editorFormat(light.getRadius()));
			light.setZNear(light.getRadius() / 1000.0f);
		}
		
		void fov_pressed() {
			light.setFov(float(fov_el.getText()));
			fov_el.setText(editorFormat(light.getFov()));
		}
		
	} /* namespace Light */
	
	/**************************************************************************\
	*
	* Grabber
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Grabber {
		
		/*
		 */
		WidgetComboBox mode_cb;				// current mode
		
		WidgetEditLine fov_el;				// field of view
		WidgetEditLine znear_el;			// near clip plane
		WidgetEditLine zfar_el;				// far clip plane
		
		WidgetCheckBox hdr_cb;				// hdr toggle
		WidgetCheckBox mipmaps_cb;			// mipmaps toggle
		WidgetCheckBox compress_cb;			// compress toggle
		WidgetCheckBox normals_cb;			// normals toggle
		WidgetCheckBox noise_cb;			// noise toggle
		
		WidgetComboBox size_cb;				// image size
		WidgetComboBox extrude_cb;			// image extrude
		WidgetComboBox supersampling_cb;	// image supersampling
		WidgetComboBox blur_cb;				// image blur
		WidgetComboBox phi_cb;				// image phi angles
		WidgetComboBox theta_cb;			// image theta angles
		WidgetComboBox aspect_cb;			// image aspect ratio
		WidgetComboBox border_cb;			// image border area
		
		enum {
			MODE_2D = 0,
			MODE_CUBE,
			MODE_LIGHTMAP,
			MODE_IMPOSTOR,
		};
		
		string image_2d_name;
		string image_cube_name;
		string image_lightmap_name;
		string image_impostor_name;
		
		float fov = 60.0f;
		float znear = 0.01f;
		float zfar = 1000.0f;
		
		int sizes[] = ( 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192 );
		int scales[] = ( 1, 2, 4, 8 );
		
		int phis[] = ( 4, 8, 16, 32, 64 );
		int thetas[] = ( 1, 2, 4, 8, 16 );
		
		int numerators[] = ( 4, 2, 1, 1, 1 );
		int denominators[] = ( 1, 1, 1, 2, 4 );
		
		int borders[] = ( 1, 2, 4, 8, 16, 32 );
		
		/*
		 */
		void init() {
			
			mode_cb.setCurrentItem(configGet("tools_grabber_mode",0));
			mode_changed();
			
			image_2d_name = configGet("tools_grabber_2d_name","");
			image_cube_name = configGet("tools_grabber_cube_name","");
			image_lightmap_name = configGet("tools_grabber_lightmap_name","");
			image_impostor_name = configGet("tools_grabber_impostor_name","");
			
			fov = configGet("tools_grabber_fov",fov);
			znear = configGet("tools_grabber_znear",znear);
			zfar = configGet("tools_grabber_zfar",zfar);
			
			hdr_cb.setChecked(configGet("tools_grabber_hdr",0));
			mipmaps_cb.setChecked(configGet("tools_grabber_mipmaps",0));
			compress_cb.setChecked(configGet("tools_grabber_compress",0));
			normals_cb.setChecked(configGet("tools_grabber_normals",0));
			noise_cb.setChecked(configGet("tools_grabber_noise",0));
			
			size_cb.setCurrentItem(configGet("tools_grabber_size",3));
			extrude_cb.setCurrentItem(configGet("tools_grabber_extrude",0));
			supersampling_cb.setCurrentItem(configGet("tools_grabber_supersampling",0));
			blur_cb.setCurrentItem(configGet("tools_grabber_blur",0));
			phi_cb.setCurrentItem(configGet("tools_grabber_phi",0));
			theta_cb.setCurrentItem(configGet("tools_grabber_theta",0));
			aspect_cb.setCurrentItem(configGet("tools_grabber_aspect",2));
			border_cb.setCurrentItem(configGet("tools_grabber_border",2));
			
			fov_el.setText(editorFormat(fov));
			znear_el.setText(editorFormat(znear));
			zfar_el.setText(editorFormat(zfar));
		}
		
		void shutdown() {
			
			configSet("tools_grabber_mode",mode_cb.getCurrentItem());
			
			configSet("tools_grabber_2d_name",image_2d_name);
			configSet("tools_grabber_cube_name",image_cube_name);
			configSet("tools_grabber_lightmap_name",image_lightmap_name);
			configSet("tools_grabber_impostor_name",image_impostor_name);
			
			configSet("tools_grabber_fov",fov);
			configSet("tools_grabber_znear",znear);
			configSet("tools_grabber_zfar",zfar);
			
			configSet("tools_grabber_hdr",hdr_cb.isChecked());
			configSet("tools_grabber_mipmaps",mipmaps_cb.isChecked());
			configSet("tools_grabber_compress",compress_cb.isChecked());
			configSet("tools_grabber_normals",normals_cb.isChecked());
			configSet("tools_grabber_noise",noise_cb.isChecked());
			
			configSet("tools_grabber_size",size_cb.getCurrentItem());
			configSet("tools_grabber_extrude",extrude_cb.getCurrentItem());
			configSet("tools_grabber_supersampling",supersampling_cb.getCurrentItem());
			configSet("tools_grabber_blur",blur_cb.getCurrentItem());
			configSet("tools_grabber_phi",phi_cb.getCurrentItem());
			configSet("tools_grabber_theta",theta_cb.getCurrentItem());
			configSet("tools_grabber_aspect",aspect_cb.getCurrentItem());
			configSet("tools_grabber_border",border_cb.getCurrentItem());
		}
		
		/*
		 */
		void mode_changed() {
			
			int mode = mode_cb.getCurrentItem();
			
			// 2d mode
			if(mode == MODE_2D) {
				setEnabled(0,( normals_cb, noise_cb, extrude_cb, phi_cb, theta_cb, aspect_cb, border_cb, ));
				setEnabled(1,( fov_el, znear_el, zfar_el, hdr_cb, mipmaps_cb, compress_cb, size_cb, supersampling_cb, blur_cb, ));
			}
			// cube mode
			else if(mode == MODE_CUBE) {
				setEnabled(0,( fov_el, normals_cb, noise_cb, extrude_cb, phi_cb, theta_cb, aspect_cb, border_cb, ));
				setEnabled(1,( znear_el, zfar_el, hdr_cb, mipmaps_cb, compress_cb, size_cb, supersampling_cb, blur_cb, ));
			}
			// lightmap mode
			else if(mode == MODE_LIGHTMAP) {
				setEnabled(0,( fov_el, znear_el, zfar_el, extrude_cb, compress_cb, normals_cb, noise_cb, phi_cb, theta_cb, aspect_cb, border_cb, ));
				setEnabled(1,( hdr_cb, mipmaps_cb, size_cb, supersampling_cb, blur_cb, ));
			}
			// impostor mode
			else if(mode == MODE_IMPOSTOR) {
				setEnabled(0,( fov_el, znear_el, zfar_el, hdr_cb, extrude_cb, blur_cb, ));
				setEnabled(1,( mipmaps_cb, compress_cb, normals_cb, noise_cb, size_cb, supersampling_cb, phi_cb, theta_cb, aspect_cb, border_cb, ));
			}
		}
		
		/*
		 */
		void fov_pressed() {
			fov = clamp(float(fov_el.getText()),1.0f,179.0f);
			fov_el.setText(editorFormat(fov));
		}
		
		void znear_pressed() {
			znear = max(float(znear_el.getText()),0.0f);
			znear_el.setText(editorFormat(znear));
		}
		
		void zfar_pressed() {
			zfar = max(float(zfar_el.getText()),0.0f);
			zfar_el.setText(editorFormat(zfar));
		}
		
		/*
		 */
		void size_changed() {
			supersampling_cb.setCallbackEnabled(GUI_CHANGED,0);
			while(sizes[size_cb.getCurrentItem()] * scales[supersampling_cb.getCurrentItem()] > 8192) {
				supersampling_cb.setCurrentItem(supersampling_cb.getCurrentItem() - 1);
			}
			supersampling_cb.setCallbackEnabled(GUI_CHANGED,1);
		}
		
		void supersampling_changed() {
			size_cb.setCallbackEnabled(GUI_CHANGED,0);
			while(sizes[size_cb.getCurrentItem()] * scales[supersampling_cb.getCurrentItem()] > 8192) {
				size_cb.setCurrentItem(size_cb.getCurrentItem() - 1);
			}
			size_cb.setCallbackEnabled(GUI_CHANGED,1);
		}
		
		/*
		 */
		int get_render_size() {
			return sizes[size_cb.getCurrentItem()] * scales[supersampling_cb.getCurrentItem()];
		}
		
		int get_real_size() {
			return sizes[size_cb.getCurrentItem()];
		}
		
		/*
		 */
		void grab_clicked() {
			
			int mode = mode_cb.getCurrentItem();
			
			// 2d mode
			if(mode == MODE_2D) {
				
				Player player = Unigine::getPlayer();
				if(player == NULL) return;
				
				if(dialogFileImageSave(TR("Select texture to save"),image_2d_name)) {
					
					Image image = new Image();
					
					int size = get_render_size();
					
					engine.render.renderImage2D(perspective(fov,1.0f,znear,zfar),player.getIWorldTransform(),image,size,size,NULL,hdr_cb.isChecked());
					if(supersampling_cb.getCurrentItem()) image.resize(get_real_size(),get_real_size());
					if(blur_cb.getCurrentItem()) image.blur(int(blur_cb.getCurrentItemText()));
					
					if(lower(extension(image_2d_name)) == "dds") {
						if(mipmaps_cb.isChecked()) image.createMipmaps();
						if(compress_cb.isChecked()) image.compress();
					}
					
					if(image.save(image_2d_name) == 0) dialogMessageOk(TR("Error"));
					else log.message("Save %s %s into the \"%s\" file\n",image.getTypeName(),image.getFormatName(),image_2d_name);
					
					delete image;
				}
			}
			// cube mode
			else if(mode == MODE_CUBE) {
				
				Player player = Unigine::getPlayer();
				if(player == NULL) return;
				
				if(dialogFileImageSave(TR("Select texture to save"),image_cube_name)) {
					
					Image image = new Image();
					
					int size = get_render_size();
					
					vec3 position = player.getPosition();
					mat4 projection = frustum(-znear,znear,-znear,znear,znear,zfar);
					
					engine.render.renderImageCube(projection,position,image,size,NULL,hdr_cb.isChecked());
					if(supersampling_cb.getCurrentItem()) image.resize(get_real_size(),get_real_size());
					if(blur_cb.getCurrentItem()) image.blur(int(blur_cb.getCurrentItemText()));
					
					if(lower(extension(image_cube_name)) == "dds") {
						if(mipmaps_cb.isChecked()) image.createMipmaps();
						if(compress_cb.isChecked()) image.compress();
					} else {
						image.convertToType(IMAGE_2D);
					}
					
					if(image.save(image_cube_name) == 0) dialogMessageOk(TR("Error"));
					else log.message("Save %s %s into the \"%s\" file\n",image.getTypeName(),image.getFormatName(),image_cube_name);
					
					delete image;
				}
			}
			// lightmap mode
			else if(mode == MODE_LIGHTMAP) {
				
				int surfaces[0];
				Node node = nodesGetNode(surfaces);
				
				if(node == NULL || node.isObject() == 0) {
					dialogMessageOk(TR("Error"),TR("You must select an object"));
					return;
				}
				
				if(dialogFileImageSave(TR("Select texture to save"),image_lightmap_name)) {
					
					Image image = new Image();
					
					float begin = clock();
					int size = get_render_size();
					int blur = int(blur_cb.getCurrentItemText());
					engine.editor.renderLightMapDirect(node_cast(node),surfaces,blur,size,size,image);
					log.message("Creating direct lightmap done in %f seconds\n",clock() - begin);
					
					if(supersampling_cb.getCurrentItem()) image.resize(get_real_size(),get_real_size());
					
					if(mipmaps_cb.isChecked()) image.createMipmaps();
					
					if(image.save(image_lightmap_name) == 0) dialogMessageOk(TR("Error"));
					else log.message("Save %s %s into the \"%s\" file\n",image.getTypeName(),image.getFormatName(),image_lightmap_name);
					
					delete image;
				}
			}
			// impostor mode
			else if(mode == MODE_IMPOSTOR) {
				
				int surfaces[0];
				Node node = nodesGetNode(surfaces);
				
				if(node == NULL) {
					dialogMessageOk(TR("Error"),TR("You must select a node"));
					return;
				}
				
				WorldBoundSphere bs = node.getWorldBoundSphere();
				vec3 center = bs.getCenter();
				float min_radius = bs.getRadius() / 2.0f;
				float max_radius = bs.getRadius() * 2.0f;
				
				if(dialogFileImageSave(TR("Select texture to save"),image_impostor_name)) {
					
					// parameters
					int phi = phis[phi_cb.getCurrentItem()];
					int theta = thetas[theta_cb.getCurrentItem()];
					
					int numerator = numerators[aspect_cb.getCurrentItem()];
					int denominator = denominators[aspect_cb.getCurrentItem()];
					
					int border = borders[border_cb.getCurrentItem()];
					
					// create sprite
					int width = 100;
					int height = width * numerator / denominator;
					WidgetSpriteNode sprite = new WidgetSpriteNode(engine.getGui(),width,height);
					sprite.setMaterials("post_deferred_color");
					sprite.setShadows(0);
					sprite.setNode(node);
					
					// create image
					Image image = new Image();
					
					// disable animation
					int is_enabled = engine.game.isEnabled();
					engine.game.setEnabled(0);
					
					// check borders
					int check_border(Image image) {
						forloop(int x = 0; image.getWidth()) {
							forloop(int y = 0; border) {
								vec4 p0 = image.get2D(x,y);
								vec4 p1 = image.get2D(y,x);
								if(p0.w > 0.0f || p1.w > 0.0f) return 0;
							}
							forloop(int y = image.getHeight() - border; image.getHeight()) {
								vec4 p0 = image.get2D(x,y);
								vec4 p1 = image.get2D(y,x);
								if(p0.w > 0.0f || p1.w > 0.0f) return 0;
							}
						}
						return 1;
					}
					
					// adjust image
					int border_ok = 1;
					float step = (max_radius - min_radius) * 0.01f;
					for(float radius = min_radius; radius < max_radius; radius += step) {
						sprite.setProjection(ortho(-radius,radius,-radius,radius,-max_radius * 2.0f,max_radius * 2.0f));
						border_ok = 1;
						forloop(int p = 0; phi) {
							forloop(int t = 0; theta) {
								sprite.setModelview(rotateX(180.0f) * inverse(translate(center) * rotateZ(-360.0f * p / phi) * rotateX(90.0f * t / theta - 90.0f) * translate(0.0f,0.0f,-max_radius)));
								sprite.renderImage(image);
								image.flipY();
								border_ok = check_border(image);
								if(border_ok == 0) break;
							}
							if(border_ok == 0) break;
						}
						if(border_ok) break;
					}
					
					// check borders
					if(border_ok == 0 && dialogMessageYesNo(TR("Warning"),TR("Can't adjust borders. Continue?")) == 0) {
						engine.game.setEnabled(is_enabled);
						delete sprite;
						delete image;
						return;
					}
					
					// create images
					Image alpha = new Image();
					Image color = new Image();
					Image normal = new Image();
					width = get_real_size() / phi;
					height = width * numerator / denominator;
					alpha.create2D(width * phi,height * theta,IMAGE_FORMAT_RGBA8);
					color.create2D(width * phi,height * theta,IMAGE_FORMAT_RGBA8);
					normal.create2D(width * phi,height * theta,IMAGE_FORMAT_RGBA8);
					
					// resize sprite
					sprite.setTextureWidth(get_render_size() / phi);
					sprite.setTextureHeight(get_render_size() * numerator / denominator / phi);
					
					// begin progress dialog
					dialogProgressBegin(TR("Grabbing impostors..."));
					
					// render images
					forloop(int p = 0, i = 0; phi) {
						forloop(int t = 0; theta) {
							
							sprite.setModelview(rotateX(180.0f) * inverse(translate(center) * rotateZ(-360.0f * p / phi) * rotateX(90.0f * t / theta - 90.0f) * translate(0.0f,0.0f,-max_radius)));
							
							// alpha image
							sprite.setMaterials("post_deferred_color");
							sprite.renderImage(image);
							image.flipY();
							if(supersampling_cb.getCurrentItem()) image.resize(width,height);
							alpha.copy(image,width * p,height * t,0,0,width,height);
							
							// color image
							sprite.setMaterials("post_deferred_color");
							sprite.renderImage(image);
							image.flipY();
							image.extrude(max(image.getWidth(),image.getHeight()) / 2);
							if(supersampling_cb.getCurrentItem()) image.resize(width,height);
							color.copy(image,width * p,height * t,0,0,width,height);
							
							// normal image
							if(normals_cb.isChecked()) {
								sprite.setMaterials("post_deferred_normal");
								sprite.renderImage(image);
								image.flipY();
								forloop(int y = 0; image.getHeight()) {
									forloop(int x = 0; image.getWidth()) {
										vec4 p = image.get2D(x,y);
										if(p.w == 0.0f) image.set2D(x,y,vec4(0.5f,0.5f,1.0f,1.0f));
									}
								}
								if(supersampling_cb.getCurrentItem()) image.resize(width,height);
								normal.copy(image,width * p,height * t,0,0,width,height);
							}
							
							int progress = ++i * 100 / (phi * theta);
							if(dialogProgressUpdate(progress) == 0) {
								engine.game.setEnabled(is_enabled);
								dialogProgressEnd();
								delete alpha;
								delete color;
								delete normal;
								delete sprite;
								delete image;
								return;
							}
						}
					}
					
					// end progress dialog
					dialogProgressEnd();
					
					// restore animation
					engine.game.setEnabled(is_enabled);
					
					// copy alpha channel
					int noise = noise_cb.isChecked();
					forloop(int y = 0; alpha.getHeight()) {
						forloop(int x = 0; alpha.getWidth()) {
							vec4 a = alpha.get2D(x,y);
							vec4 c = color.get2D(x,y);
							c.w = a.w;
							if(noise) {
								if(c.w > 0.55f) c.w = engine.game.getRandom(0.55f,1.0f);
							}
							color.set2D(x,y,c);
						}
					}
					
					// update color image
					if(lower(extension(image_impostor_name)) == "dds") {
						if(mipmaps_cb.isChecked()) color.createMipmaps();
						if(compress_cb.isChecked()) color.compress();
					}
					
					// save color image
					if(color.save(image_impostor_name) == 0) dialogMessageOk(TR("Error"));
					else log.message("Save %s %s into the \"%s\" file\n",color.getTypeName(),color.getFormatName(),image_impostor_name);
					
					// normal image
					if(normals_cb.isChecked()) {
						
						// normal image name
						string normal_name = extension(image_impostor_name,NULL);
						if(strstr(normal_name,"_d") == strlen(normal_name) - 2) normal_name = replace(normal_name,"_d","");
						normal_name += "_n.dds";
						
						normal.convertToFormat(IMAGE_FORMAT_RG8);
						
						// update normal image
						if(lower(extension(normal_name)) == "dds") {
							if(mipmaps_cb.isChecked()) normal.createMipmaps();
							if(compress_cb.isChecked()) normal.compress();
						}
						
						// save normal image
						if(normal.save(normal_name) == 0) dialogMessageOk(TR("Error"));
						else log.message("Save %s %s into the \"%s\" file\n",normal.getTypeName(),normal.getFormatName(),normal_name);
					}
					
					delete alpha;
					delete color;
					delete normal;
					delete sprite;
					delete image;
				}
			}
		}
		
	} /* namespace Grabber */
	
	/**************************************************************************\
	*
	* Illumination
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Illumination {
		
		#include <editor/editor_tools_illumination.h>
		
	} /* Illumination */
	
	/**************************************************************************\
	*
	* Lut
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Lut {
		
		WidgetSprite texture_s;				// texture sprite
		WidgetComboBox width_cb;			// texture width
		WidgetComboBox height_cb;			// texture height
		WidgetVBox diffuse_vb;				// diffuse container
		WidgetEditLine diffuse_scale_el;	// diffuse scale editline
		WidgetVBox specular_vb;				// specular container
		WidgetEditLine specular_scale_el;	// specular scale editline
		WidgetEditLine specular_power_el;	// specular power editline
		WidgetEditLine specular_shift_el;	// specular shift editline
		WidgetEditLine specular_angle_el;	// specular angle editline
		WidgetButton update_b;				// update button
		
		texture_s = texture_s;
		width_cb = width_cb;
		height_cb = height_cb;
		diffuse_vb = diffuse_vb;
		diffuse_scale_el = diffuse_scale_el;
		specular_vb = specular_vb;
		specular_scale_el = specular_scale_el;
		specular_power_el = specular_power_el;
		specular_shift_el = specular_shift_el;
		specular_angle_el = specular_angle_el;
		update_b = update_b;
		
		#ifdef EDITOR_LUT
			#include <editor/editor_tools_lut.h>
		#endif
		
	} /* Lut */
	
	/**************************************************************************\
	*
	* Show
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Show {
		
		/*
		 */
		WidgetCheckBox render_show_mipmaps_cb;
		WidgetCheckBox render_show_normals_cb;
		WidgetCheckBox render_show_triangles_cb;
		WidgetCheckBox render_show_decals_cb;
		WidgetCheckBox render_show_textures_cb;
		WidgetCheckBox render_show_scissors_cb;
		WidgetCheckBox render_show_queries_cb;
		WidgetCheckBox render_show_occluder_cb;
		WidgetCheckBox render_show_splits_cb;
		WidgetCheckBox render_show_lights_cb;
		WidgetCheckBox render_show_penumbra_cb;
		WidgetCheckBox render_show_occlusion_cb;
		
		WidgetCheckBox show_names[];
		
		WidgetCheckBox render_reload[0];
		
		/*
		 */
		void init() {
			
			show_names.append("render_show_mipmaps",render_show_mipmaps_cb);
			show_names.append("render_show_normals",render_show_normals_cb);
			show_names.append("render_show_triangles",render_show_triangles_cb);
			show_names.append("render_show_decals",render_show_decals_cb);
			show_names.append("render_show_textures",render_show_textures_cb);
			show_names.append("render_show_scissors",render_show_scissors_cb);
			show_names.append("render_show_queries",render_show_queries_cb);
			show_names.append("render_show_occluder",render_show_occluder_cb);
			show_names.append("render_show_splits",render_show_splits_cb);
			show_names.append("render_show_lights",render_show_lights_cb);
			show_names.append("render_show_penumbra",render_show_penumbra_cb);
			show_names.append("render_show_occlusion",render_show_occlusion_cb);
			
			render_reload = (
				render_show_splits_cb,
				render_show_lights_cb,
				render_show_penumbra_cb,
				render_show_occlusion_cb,
			);
			
			setCallbackEnabled(GUI_CLICKED,0,render_reload);
			foreachkey(string name; show_names) {
				show_names[name].setChecked(engine.console.getInt(name));
			}
			setCallbackEnabled(GUI_CLICKED,1,render_reload);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update_dynamic() {
			
			if(editorIsChild(window) && window.isChild(gui.getFocus())) {
				setCallbackEnabled(GUI_CLICKED,0,render_reload);
				foreachkey(string name; show_names) {
					if(show_names[name].isFocused() == 0) show_names[name].setChecked(engine.console.getInt(name));
				}
				setCallbackEnabled(GUI_CLICKED,1,render_reload);
			}
		}
		
		/*
		 */
		void clicked(string name) {
			engine.console.setInt(name,show_names[name].isChecked());
			if(render_reload.find(show_names[name]) != -1) engine.console.run("render_reload");
		}
		
	} /* namespace Show */
	
	/**************************************************************************\
	*
	* Group operations
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Batch {
		
		/*
		 */
		WidgetCheckBox hierarchical_cb;
		WidgetEditLine light_mask_el;
		WidgetEditLine decal_mask_el;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine intersection_mask_el;
		WidgetEditLine collision_mask_el;
		WidgetEditLine znear_el;
		WidgetEditLine zfar_el;
		WidgetComboBox frames_cb;
		
		int hierarchical = 0;
		int light_mask = 1;
		int decal_mask = 1;
		int viewport_mask = 1;
		int intersection_mask = 1;
		int collision_mask = 1;
		float znear = 0.01f;
		float zfar = 1000.0f;
		int frames = 0;
		
		/*
		 */
		void init() {
			
			hierarchical = configGet("tools_batch_hierarchical",hierarchical);
			light_mask = configGet("tools_batch_light_mask",light_mask);
			decal_mask = configGet("tools_batch_decal_mask",decal_mask);
			viewport_mask = configGet("tools_batch_viewport_mask",viewport_mask);
			intersection_mask = configGet("tools_batch_intersection_mask",intersection_mask);
			collision_mask = configGet("tools_batch_collision_mask",collision_mask);
			znear = configGet("tools_batch_znear",znear);
			zfar = configGet("tools_batch_zfar",zfar);
			frames = configGet("tools_batch_frames",frames);
			
			hierarchical_cb.setChecked(hierarchical);
			light_mask_el.setText(editorFormatMask(light_mask));
			decal_mask_el.setText(editorFormatMask(decal_mask));
			viewport_mask_el.setText(editorFormatMask(viewport_mask));
			intersection_mask_el.setText(editorFormatMask(intersection_mask));
			collision_mask_el.setText(editorFormatMask(collision_mask));
			znear_el.setText(editorFormat(znear));
			zfar_el.setText(editorFormat(zfar));
			frames_cb.setCurrentItem(frames);
		}
		
		void shutdown() {
			
			configSet("tools_batch_hierarchical",hierarchical);
			configSet("tools_batch_light_mask",light_mask);
			configSet("tools_batch_decal_mask",decal_mask);
			configSet("tools_batch_viewport_mask",viewport_mask);
			configSet("tools_batch_intersection_mask",intersection_mask);
			configSet("tools_batch_collision_mask",collision_mask);
			configSet("tools_batch_znear",znear);
			configSet("tools_batch_zfar",zfar);
			configSet("tools_batch_frames",frames);
		}
		
		/*
		 */
		void get_nodes(Node node,Node nodes[]) {
			
			if(node.isEnabled() == 0) return;
			
			nodes.append(node.getID(),node);
			
			if(hierarchical) {
				forloop(int i = 0; node.getNumChilds()) {
					get_nodes(node.getChild(i),nodes);
				}
			}
		}
		
		/*
		 */
		void apply_mask(int mask,string setter) {
			
			Node selected_nodes[0];
			nodesGetNodes(selected_nodes);
			
			if(selected_nodes.size() == 0) {
				dialogMessageOk(TR("Error"),TR("You must select objects"));
				return;
			}
			
			Node unique_nodes[];
			foreach(Node node; selected_nodes) {
				get_nodes(node,unique_nodes);
			}
			
			Node nodes[0];
			foreach(Node node; unique_nodes) {
				nodes.append(node);
			}
			Nodes::NodesState_ s = nodesUnredoCreateModifyState(nodes,nodesGetNode());
			
			foreach(Node node; unique_nodes) {
				if(node.isObject()) {
					Object object = node_cast(node);
					forloop(int i = 0; object.getNumSurfaces()) {
						object.call(setter,mask,i);
					}
				}
			}
			
			nodesUnredoSaveState(s);
		}
		
		/*
		 */
		void hierarchical_clicked() {
			hierarchical = hierarchical_cb.isChecked();
			hierarchical_cb.setChecked(hierarchical);
		}
		
		/*
		 */
		#define DECLARE_MASK(NAME,FUNC) \
		void NAME ## _mask_changed(int mask) { \
			NAME ## _mask = mask; \
			NAME ## _mask_el.setText(editorFormatMask(NAME ## _mask)); \
		} \
		void NAME ## _mask_pressed() { \
			NAME ## _mask_changed(int("0x" + NAME ## _mask_el.getText())); \
		} \
		void NAME ## _mask_clicked() { \
			dialogMask(TR("Select ") + #NAME + TR(" mask"),"Tools::Batch::" #NAME "_mask_changed",NAME ## _mask); \
		} \
		void apply_ ## NAME ## _mask_clicked() { \
			apply_mask(NAME ## _mask,FUNC); \
		}
		
		DECLARE_MASK(light,"setLightMask")
		DECLARE_MASK(decal,"setDecalMask")
		DECLARE_MASK(viewport,"setViewportMask")
		DECLARE_MASK(intersection,"setIntersectionMask")
		DECLARE_MASK(collision,"setCollisionMask")
		
		#undef DECLARE_MASK
		
		/*
		 */
		void znear_pressed() {
			znear = max(float(znear_el.getText()),0.0f);
			znear_el.setText(editorFormat(znear));
		}
		
		void zfar_pressed() {
			zfar = max(float(zfar_el.getText()),0.0f);
			zfar_el.setText(editorFormat(zfar));
		}
		
		void apply_grab_light_prob_clicked() {
			
			Node selected_nodes[0];
			nodesGetNodes(selected_nodes);
			
			if(selected_nodes.size() == 0) {
				dialogMessageOk(TR("Error"),TR("You must select lights"));
				return;
			}
			
			Node unique_nodes[];
			foreach(Node node; selected_nodes) {
				get_nodes(node,unique_nodes);
			}
			
			int masks[0];
			LightProb lights[0];
			
			foreach(Node node; unique_nodes) {
				if(node.getType() == NODE_LIGHT_PROB) {
					LightProb light = node_cast(node);
					masks.append(light.getLightMask());
					light.setLightMask(0);
					lights.append(light);
				}
			}
			
			// begin progress dialog
			dialogProgressBegin(TR("Grabbing LightProb..."));
			
			// grab LightProb
			float begin = clock();
			forloop(int i = 0; lights.size()) {
				
				lights[i].renderImageTexture(znear,zfar,128);
				
				int progress = i * 100 / lights.size();
				if(dialogProgressUpdate(progress) == 0) {
					dialogProgressEnd();
					break;
				}
			}
			log.message("Grabbing LightProb done %f seconds\n",clock() - begin);
			
			// end progress dialog
			dialogProgressEnd();
			
			forloop(int i = 0; lights.size()) {
				lights[i].setLightMask(masks[i]);
			}
		}
		
		/*
		 */
		void generate_decals_clicked() {
			
			Node selected_nodes[0];
			nodesGetNodes(selected_nodes);
			
			if(selected_nodes.size() == 0) {
				dialogMessageOk(TR("Error"),TR("You must select decals"));
				return;
			}
			
			Node unique_nodes[];
			foreach(Node node; selected_nodes) {
				get_nodes(node,unique_nodes);
			}
			
			foreach(Node node; unique_nodes) {
				if(node.isDecal()) nodesUpdateDecal(node_cast(node));
			}
		}
		
		/*
		 */
		void frames_changed() {
			frames = frames_cb.getCurrentItem();
		}
		
		void get_samples(string path,string names[]) {
			
			Dir dir = new Dir();
			if(dir.open(path)) {
				forloop(int i = 0; dir.getNumFiles()) {
					string name = basename(dir.getFileName(i));
					if(re_match(name,".+\\.world$") == 0) continue;
					names.append(path + re_replace(name,".+(\\.world)$",""));
				}
				forloop(int i = 0; dir.getNumDirs()) {
					get_samples(dir.getDirName(i),names);
				}
				dir.close();
			}
			delete dir;
		}
		
		void run_samples_clicked() {
			
			if(dialogMessageYesNo(TR("Confirm"),TR("Run all scenes from the \"data/samples\" directory?")) == 0) {
				return;
			}
			
			string names[0];
			string path = engine.getDataPath();
			get_samples(path + "samples/",names);
			foreach(string name; names) {
				name = replace(name,path,"");
			}
			names.sort();
			
			// begin progress dialog
			dialogProgressBegin(TR("Running samples..."));
			
			// run samples
			forloop(int i = 0; names.size()) {
				
				engine.console.run("world_load %s",names[i]);
				wait;
				
				forloop(int j = 0; int(frames_cb.getCurrentItemText())) wait;
				
				if(engine.world.isLoaded() == 0 || strlen(engine.getError())) {
					log.error("Can't load \"%s\" world\n",names[i]);
				}
				
				if(dialogProgressUpdate(i * 100 / names.size()) == 0) {
					dialogProgressEnd();
					return;
				}
			}
			
			// end progress dialog
			dialogProgressEnd();
		}
		
	} /* namespace Batch */
	
	/**************************************************************************\
	*
	* Window
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		Render::init();
		Common::init();
		Light::init();
		Grabber::init();
		Illumination::init();
		#ifdef EDITOR_LUT
			Lut::init();
		#else
			tabbox.setTabHidden(5,1);
		#endif
		Show::init();
		Batch::init();
		
		editorWindow(window);
		DECLARE_WINDOW_LOAD(tools)
		
		tabbox.setCurrentTab(configGet("tools_tab",0));
	}
	
	void shutdown() {
		
		DECLARE_WINDOW_SAVE(tools)
		
		configSet("tools_tab",tabbox.getCurrentTab());
		
		Common::shutdown();
		Light::shutdown();
		Grabber::shutdown();
		Illumination::shutdown();
		#ifdef EDITOR_LUT
			Lut::shutdown();
		#endif
		Show::shutdown();
		Render::shutdown();
		Batch::shutdown();
	}
	
	/*
	 */
	void update(int need_reload) {
		
		int tab = tabbox.getCurrentTab();
		
		Illumination::update(need_reload);
		#ifdef EDITOR_LUT
			Lut::update();
		#endif
		
		// update dynamic
		if(tab == 7) Show::update_dynamic();
	}
	
	/*
	 */
	void screenshot() {
		thread(functionid(grab_screenshot));
	}
	
	void grab_screenshot() {
		
		int visualizer = engine.visualizer.isEnabled();
		
		int show_fps = engine.console.getInt("show_fps");
		int show_messages = engine.console.getInt("show_messages");
		
		engine.console.run("video_grab");
		
		forloop(int i = 0; 4) {
			engine.gui.setHidden(1);
			engine.visualizer.setEnabled(0);
			engine.console.setInt("show_fps",0);
			engine.console.setInt("show_messages",0);
			wait;
		}
		
		engine.gui.setHidden(0);
		
		engine.visualizer.setEnabled(visualizer);
		
		engine.console.setInt("show_fps",show_fps);
		engine.console.setInt("show_messages",show_messages);
	}
	
} /* namespace Tools */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void toolsInit() {
	Tools::init();
}

void toolsShutdown() {
	Tools::shutdown();
}

/*
 */
void toolsToggle() {
	if(Tools::window_show) toolsHide();
	else toolsShow();
}

void toolsShow() {
	DECLARE_WINDOW_SHOW(Tools)
}

void toolsHide() {
	DECLARE_WINDOW_HIDE(Tools)
}

/*
 */
void toolsUpdate(int need_reload) {
	Tools::update(need_reload);
}

/*
 */
void toolsScreenshot() {
	Tools::screenshot();
}

/*
 */
int toolsGetSystemMouse() {
	return Tools::Common::system_mouse_cb.isChecked();
}

void toolsSetAlwaysUpdate(int update) {
	Tools::Common::always_update_cb.setChecked(update);
}

int toolsGetAlwaysUpdate() {
	return Tools::Common::always_update_cb.isChecked();
}

int toolsGetNodeReparent() {
	return Tools::Common::node_reparent_cb.isChecked();
}

int toolsGetNodeName() {
	return Tools::Common::node_name_cb.isChecked();
}

int toolsGetKeepBackups() {
	return Tools::Common::keep_backups_cb.isChecked();
}

int toolsGetNodeMaterials() {
	return Tools::Common::node_materials_cb.isChecked();
}

int toolsGetSelection() {
	return Tools::Common::selection_cb.isChecked();
}

int toolsGetGameCamera() {
	return Tools::Common::game_camera_cb.isChecked();
}

int toolsGetCloneConfirm() {
	return Tools::Common::clone_confirm_cb.isChecked();
}

int toolsGetRotationOrder() {
	return Tools::Common::rotation_order_cb.getCurrentItem();
}

/* size
 */
int toolsGetSize() {
	return Tools::Common::size;
}

void toolsSetSize(int size) {
	Tools::Common::size = size;
	Tools::Common::size_el.setText(string(size));
}

void toolsIncreaseSize() {
	int new_size = clamp(Tools::Common::size + 10,10,400);
	toolsSetSize(new_size);
}

void toolsDecreaseSize() {
	int new_size = clamp(Tools::Common::size - 10,10,400);
	toolsSetSize(new_size);
}

/*
 */
void toolsToggleLight() {
	Tools::Light::toggle_light();
}

/*
 */
LightProj toolsGetLight() {
	return Tools::Light::light;
}
