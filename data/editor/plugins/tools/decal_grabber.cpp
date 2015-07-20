/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    decal_grabber.cpp
 * Desc:    Decal grabber plugin
 * Version: 1.00
 * Author:  Andrey Viktorov <unclebob@unigine.com>
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

#include <core/unigine.h>

/******************************************************************************\
*
* Plugin
*
\******************************************************************************/

/*
 */
string getName() {
	return "DecalGrabber";
}

/*
 */
void init(PluginMeta m) {
	DecalGrabber::init(m);
}

void shutdown() {
	DecalGrabber::shutdown();
}

void update(int need_reload) {
	DecalGrabber::update(need_reload);
}

void save() {
}

void nodeUpdate() {
	DecalGrabber::update(true);
}

/******************************************************************************\
*
* DecalGrabber
*
\******************************************************************************/

/*
 */
namespace DecalGrabber {
	
	/*
	 */
	using Unigine::Widgets;
	
	/*
	 */
	UserInterface interface;
	
	VBox main_vb;
	ComboBox texture_size_cb;
	ComboBox texture_upscale_cb;
	ComboBox grid_size_cb;
	EditLine padding_el;
	EditLine path_el;
	EditLine height_el;
	EditLine material_el;
	EditLine min_visible_el;
	EditLine min_fade_el;
	EditLine max_visible_el;
	EditLine max_fade_el;
	
	Node old_node;
	
	/*
	 */
	int disable_types[] = (
		NODE_OBJECT_SKY,
		NODE_OBJECT_WATER,
		NODE_OBJECT_WATER_MESH,
		NODE_OBJECT_PARTICLES,
		NODE_LIGHT_OMNI,
		NODE_LIGHT_PROB,
		NODE_LIGHT_PROJ,
		NODE_LIGHT_SPOT,
		NODE_LIGHT_WORLD,
	);
	
	Node disabled_nodes[0];
	Node decal_parent;
	
	float grass_min_distance[];
	float grass_max_distance[];
	
	float decal_max_distance[];
	float decal_min_distance[];
	
	float procedural_visible_distance[];
	
	float render_budget;
	float world_budget;
	float physics_budget;
	
	vec4 background_color;
	
	PluginMeta meta;
	
	/*
	 */
	void init(PluginMeta m) {
		
		meta = m;
		
		interface = new UserInterface(replace(meta.source,".cpp",".ui"),getName() + "::");
		hotkeysAddUiToolTips(interface);
		
		texture_size_cb.setCurrentItem(configGet("decal_grabber_texture_size",0));
		texture_upscale_cb.setCurrentItem(configGet("decal_grabber_texture_upscale",0));
		
		grid_size_cb.setCurrentItem(configGet("decal_grabber_grid_size",0));
		
		height_el.setText(editorFormat(configGet("decal_grabber_height",0.0f)));
		
		min_visible_el.setText(editorFormat(configGet("decal_grabber_min_visible",-INFINITY)));
		max_visible_el.setText(editorFormat(configGet("decal_grabber_max_visible",INFINITY)));
		
		min_fade_el.setText(editorFormat(configGet("decal_grabber_min_fade",0.0f)));
		max_fade_el.setText(editorFormat(configGet("decal_grabber_max_fade",0.0f)));
		
		material_el.setText(configGet("decal_grabber_material","decal_deferred_base"));
		
		padding_el.setText(string(configGet("decal_grabber_padding",4)));
		path_el.setText(configGet("decal_grabber_path",""));
	}
	
	/*
	 */
	void shutdown() {
		save();
		
		hotkeysRemoveUiToolTips(interface);
		delete interface;
	}
	
	void save() {
		configSet("decal_grabber_texture_size",texture_size_cb.getCurrentItem());
		configSet("decal_grabber_texture_upscale",texture_upscale_cb.getCurrentItem());
		configSet("decal_grabber_grid_size",grid_size_cb.getCurrentItem());
		configSet("decal_grabber_height",float(height_el.getText()));
		configSet("decal_grabber_min_visible",float(min_visible_el.getText()));
		configSet("decal_grabber_max_visible",float(max_visible_el.getText()));
		configSet("decal_grabber_min_fade",float(min_fade_el.getText()));
		configSet("decal_grabber_max_fade",float(max_fade_el.getText()));
		configSet("decal_grabber_material",material_el.getText());
		configSet("decal_grabber_padding",int(padding_el.getText()));
		configSet("decal_grabber_path",path_el.getText());
	}
	
	/*
	 */
	void update(int need_reload) {
		Node node = nodesGetNode();
		if(need_reload == 0 && node == old_node) return;
		
		old_node = node;
		if(node == NULL) return;
		
		save();
		
		if(engine.editor.isNode(node) == 0) return;
		if(node.getType() != NODE_OBJECT_TERRAIN) return;
		
		::Nodes::parameters_tb.addTab(TR("Decals"));
		::Nodes::parameters_tb.addChild(main_vb.widget,GUI_ALIGN_EXPAND);
	}
	
	/*
	 */
	void grabber_height_check() {
		float h = float(height_el.getText());
		if (h < 0.0f) {
			height_el.setCallbackEnabled(GUI_CHANGED,0);
			height_el.setText(editorFormat(0.0f));
			height_el.setCallbackEnabled(GUI_CHANGED,1);
		}
	}
	
	/*
	 */
	void grabber_padding_check() {
		padding_el.setCallbackEnabled(GUI_CHANGED,0);
		int cursor = padding_el.getCursor();
		int val = int(padding_el.getText());
		if (val < 0 || val > 100) padding_el.setText(substr(padding_el.getText(),0,2));
		else padding_el.setText(string(val));
		padding_el.setCursor(cursor);
		padding_el.setCallbackEnabled(GUI_CHANGED,1);
	}
	
	/*
	 */
	void material_load_clicked() {
		string material_name = material_el.getText();
		if(dialogMaterial(TR("Select material"),material_name)) {
			material_el.setText(material_name);
		}
	}
	
	/*
	 */
	void grab_clicked() {
		save();
		
		// check terrain
		ObjectTerrain terrain = node_cast(nodesGetNode());
		if(terrain == NULL) return;
		
		// check material
		string material = material_el.getText();
		if(engine.materials.findMaterial(material) == NULL) {
			dialogMessageOk(TR("Error"),format(TR("Decal material \"%s\" is not found"),material));
			return;
		}
		
		prepare_hierarchy(terrain);
		prepare_render();
		prepare_nodes();
		
		Image diffuse_images[0];
		Image alpha_images[0];
		BoundBox bounds[0];
		
		int result = create_bounds(terrain,bounds);
		if(result) result = create_textures(terrain,bounds,diffuse_images,alpha_images);
		if(result) result = create_decals(terrain,bounds,diffuse_images,alpha_images);
		
		diffuse_images.delete();
		alpha_images.delete();
		bounds.delete();
		
		restore_nodes();
		restore_render();
		nodesSelect(terrain,0);
	}
	
	/*
	 */
	void prepare_hierarchy(ObjectTerrain terrain) {
		decal_parent = NULL;
		
		// search for existing parent
		forloop(int i = 0; terrain.getNumChilds()) {
			Node node = node_cast(terrain.getChild(i));
			if(node.getType() != NODE_DUMMY) continue;
			if(node.getName() != "decal_lod") continue;
			
			decal_parent = node;
			break;
		}
		
		// create dummy node if parent was not found
		if(decal_parent == NULL) {
			decal_parent = node_remove(new NodeDummy());
			decal_parent.setName("decal_lod");
			
			engine.editor.addNode(decal_parent);
			terrain.addChild(decal_parent);
		}
		
		// clear decals & textures
		for(int i = decal_parent.getNumChilds() - 1; i >= 0; i--) {
			Node child = node_cast(decal_parent.getChild(i));
			if(child.isDecal() == 0) continue;
			
			string data = child.getData();
			if(strlen(data) == 0) continue;
			if(data[0] != '<') continue;
			
			Xml xml = class_manage(new Xml());
			if(xml.parse(data) == 0) continue;
			if(xml.getName() != "decal_grabber") continue;
			
			string filename = xml.getChildData("texture_path","");
			if(strlen(filename) == 0) continue;
			
			engine.editor.removeNode(child);
			remove(savePath(filename));
		}
	}
	
	void prepare_nodes() {
		forloop(int i = 0; engine.editor.getNumNodes()) {
			Node node = node_cast(engine.editor.getNode(i));
			if(node.isEnabled() == 0) continue;
			
			int type = node.getType();
			
			forloop(int j = 0; disable_types.size()) {
				if(type != disable_types[j]) continue;
				
				node.setEnabled(0);
				disabled_nodes.append(node);
				break;
			}
			
			if(node.isEnabled() == 0) continue;
			
			int is_grass = (type == NODE_OBJECT_GRASS);
			int is_cluster = (type == NODE_OBJECT_MESH_CLUSTER) || (type == NODE_WORLD_CLUSTER);
			int is_clutter = (type == NODE_OBJECT_MESH_CLUTTER) || (type == NODE_WORLD_CLUTTER);
			
			if(node.isDecal()) {
				decal_min_distance.append(node,node.call("getMinVisibleDistance"));
				decal_max_distance.append(node,node.call("getMaxVisibleDistance"));
				
				node.call("setMinVisibleDistance",-INFINITY);
				node.call("setMaxVisibleDistance",INFINITY);
			}
			
			if(is_grass) {
				grass_min_distance.append(node,node.call("getMinVisibleDistance",0));
				grass_max_distance.append(node,node.call("getMaxVisibleDistance",0));
				
				node.call("setMinVisibleDistance",-INFINITY,0);
				node.call("setMaxVisibleDistance",INFINITY,0);
			}
			
			if(is_clutter || is_cluster) {
				procedural_visible_distance.append(node,node.call("getVisibleDistance"));
				node.call("setVisibleDistance",INFINITY);
			}
		}
		
		engine.world.updateSpatial();
	}
	
	void prepare_render() {
		world_budget = engine.world.getBudget();
		physics_budget = engine.physics.getBudget();
		
		render_budget = engine.render.getBudget();
		background_color = engine.render.getBackgroundColor();
		
		engine.world.setBudget(INFINITY);
		engine.physics.setBudget(INFINITY);
		
		engine.render.setBudget(INFINITY);
		engine.render.setBackgroundColor(vec4_zero);
		
		if(ambientIsEnabled() == 0) ambientToggle();
	}
	
	/*
	 */
	void restore_nodes() {
		forloop(int i = 0; disabled_nodes.size()) {
			disabled_nodes[i].setEnabled(1);
		}
		
		foreachkey(Node node; grass_max_distance) {
			node.call("setMinVisibleDistance",grass_min_distance[node],0);
			node.call("setMaxVisibleDistance",grass_max_distance[node],0);
		}
		
		foreachkey(Node node; procedural_visible_distance) {
			node.call("setVisibleDistance",procedural_visible_distance[node]);
		}
		
		foreachkey(Node node; decal_max_distance) {
			node.call("setMinVisibleDistance",decal_min_distance[node]);
			node.call("setMaxVisibleDistance",decal_max_distance[node]);
		}
		
		disabled_nodes.clear();
		
		grass_min_distance.clear();
		grass_max_distance.clear();
		
		decal_min_distance.clear();
		decal_max_distance.clear();
		
		procedural_visible_distance.clear();
	}
	
	void restore_render() {
		engine.world.setBudget(world_budget);
		engine.physics.setBudget(physics_budget);
		
		engine.render.setBudget(render_budget);
		engine.render.setBackgroundColor(background_color);
		
		if(ambientIsEnabled()) ambientToggle();
	}
	
	/*
	 */
	int create_bounds(ObjectTerrain terrain,BoundBox bounds[]) {
		
		// grab parameters
		Vec3 terrain_min = terrain.getBoundBox().getMin();
		Vec3 terrain_max = terrain.getBoundBox().getMax();
		
		int grid_size = int(grid_size_cb.getCurrentItemText());
		
		float step_x = (terrain_max.x - terrain_min.x) / grid_size;
		float step_y = (terrain_max.y - terrain_min.x) / grid_size;
		
		float istep = 1.0f / terrain.getStep();
		
		dialogProgressBegin(TR("Finding decal bounds..."));
		
		forloop(int y = 0; grid_size) {
			forloop(int x = 0; grid_size) {
				vec3 bound_min = vec3(step_x * x,step_y * y,INFINITY);
				vec3 bound_max = vec3(step_x * (x + 1),step_y * (y + 1),-INFINITY);
				
				int x0 = int(floor(bound_min.x * istep));
				int y0 = int(floor(bound_min.y * istep));
				int x1 = int(ceil(bound_max.x * istep));
				int y1 = int(ceil(bound_max.y * istep));
				
				forloop(int w = x0; x1) {
					forloop(int h = y0; y1) {
						float height = terrain.getHeight(w,h);
						bound_min.z = min(bound_min.z,height);
						bound_max.z = max(bound_max.z,height);
					}
				}
				
				bounds.append(new BoundBox(bound_min,bound_max));
				
				int progress = (grid_size * y + x + 1) * 100 / (grid_size * grid_size);
				if(dialogProgressUpdate(progress) == 0) {
					dialogProgressEnd();
					return 0;
				}
			}
		}
		
		dialogProgressEnd();
		return 1;
	}
	
	int create_textures(ObjectTerrain terrain,BoundBox bounds[],Image diffuse_images[],Image alpha_images[]) {
		Material terrain_material = terrain.getMaterial(0);
		
		int terrain_material_state = terrain_material.getState("ambient");
		int id = terrain_material.findParameter("specular_scale");
		float terrain_specular_scale = 0.0f;
		
		if(id != -1) {
			terrain_specular_scale = terrain_material.getParameterSlider(id);
			terrain_material.setParameterSlider(id,0.0f);
		}
		
		int result = grab(terrain,bounds,diffuse_images);
		
		terrain_material.setState("ambient",0);
		
		if(result) result = grab(terrain,bounds,alpha_images);
		
		if(id != -1) terrain_material.setParameterSlider(id,terrain_specular_scale);
		terrain_material.setState("ambient",terrain_material_state);
		
		return result;
	}
	
	int create_decals(ObjectTerrain terrain,BoundBox bounds[],Image diffuse_images[],Image alpha_images[]) {
		Async async = asyncGet();
		
		// decal parameters
		int texture_size = int(texture_size_cb.getCurrentItemText());
		int padding = int(padding_el.getText());
		int use_srgb = engine.console.getInt("render_use_srgb");
		float gamma = (use_srgb) ? 2.2f : 1.0f;
		
		string material = material_el.getText();
		string path = savePath(path_el.getText());
		mkdir(path,1);
		
		float min_visible = float(min_visible_el.getText());
		float max_visible = float(max_visible_el.getText());
		
		float min_fade = float(min_fade_el.getText());
		float max_fade = float(max_fade_el.getText());
		
		// grab parameters
		int grid_size = int(grid_size_cb.getCurrentItemText());
		
		int index = 0;
		
		dialogProgressBegin(TR("Creating decals..."));
		
		forloop(int y = 0; grid_size) {
			forloop(int x = 0; grid_size) {
				
				BoundBox bound_box = bounds[index];
				Image alpha = alpha_images[index];
				Image diffuse = diffuse_images[index];
				index++;
				
				vec3 min = bound_box.getMin();
				vec3 max = bound_box.getMax();
				
				float step_x = max.x - min.x;
				float step_y = max.y - min.y;
				
				float X = step_x * x + step_x * 0.5f;
				float Y = step_y * y + step_y * 0.5f;
				
				string filename = pathname(path + "/") + terrain.getName() + format("_%02d_%02d_decal.dds",x,y);
				
				// compose images
				if(use_srgb) {
					applyGamma(diffuse);
				}
				
				async.run(diffuse,"copy",alpha,3,3);
				if(padding > 0) {
					async.run(diffuse,"extrude",padding);
					async.run(diffuse,"copy",alpha,3,3);
				}
				async.run(diffuse,"resize",texture_size,texture_size,IMAGE_FILTER_LINEAR);
				async.run(diffuse,"createMipmaps",IMAGE_FILTER_LINEAR,gamma);
				async.run(diffuse,"compress",IMAGE_FORMAT_DXT5);
				async.run(diffuse,"save",filename);
				
				while(async.isRunning()) wait;
				
				// create decal
				DecalDeferredOrtho decal = new DecalDeferredOrtho(max.z - min.z,step_x,step_y,material);
				engine.editor.addNode(node_remove(decal));
				
				decal_parent.addChild(decal);
				decal.setPosition(Vec3(X,Y,max.z));
				
				// decal material
				Material decal_material = decal.getMaterialInherit();
				int texture = decal_material.findTexture("diffuse");
				
				decal_material.setImageTextureName(texture,editorPath(filename));
				
				// decal data
				Xml xml = class_manage(new Xml("decal_grabber"));
				xml.setChildData("texture_path",editorPath(filename));
				
				decal.setData(xml.getSubTree());
				
				// decal parameters
				decal.setMinVisibleDistance(min_visible);
				decal.setMaxVisibleDistance(max_visible);
				
				decal.setMinFadeDistance(min_fade);
				decal.setMaxFadeDistance(max_fade);
				decal.setName(format("%02d_%02d",x,y));
				
				// update progress dialog
				int progress = (grid_size * y + x + 1) * 100 / (grid_size * grid_size);
				if(dialogProgressUpdate(progress) == 0) {
					dialogProgressEnd();
					return 0;
				}
			}
		}
		
		dialogProgressEnd();
		return 1;
	}
	
	/*
	 */
	int grab(ObjectTerrain terrain,BoundBox bounds[],Image images[]) {
		
		// grab parameters
		int texture_size = int(texture_size_cb.getCurrentItemText()) * int(texture_upscale_cb.getCurrentItemText());
		int grid_size = int(grid_size_cb.getCurrentItemText());
		float height = float(height_el.getText());
		
		Image surface = new Image();
		
		dialogProgressBegin(TR("Grabbing textures..."));
		
		int index = 0;
		
		forloop(int y = 0; grid_size) {
			forloop(int x = 0; grid_size) {
				
				// bounds
				BoundBox bound_box = bounds[index];
				index++;
				
				vec3 bound_min = bound_box.getMin();
				vec3 bound_max = bound_box.getMax();
				
				float step_x = bound_max.x - bound_min.x;
				float step_y = bound_max.y - bound_min.y;
				
				float X = step_x * x + step_x * 0.5f;
				float Y = step_y * y + step_y * 0.5f;
				
				int frames = 4;
				
				mat4 projection = ortho(-step_x * 0.5f,step_x * 0.5f,-step_y * 0.5f,step_y * 0.5f,0.0f,GRABBER_PROJECTION_ZFAR);
				Mat4 modelview = translate(-X,-Y,-height) * terrain.getIWorldTransform();
				engine.render.renderImage2D(projection,modelview,surface,texture_size,texture_size,NULL,0);
				
				// additional render frames
				while(frames-- > 0 || engine.filesystem.getNumQueuedResources() || engine.world.getNumQueuedResources()) {
					wait;
					engine.render.renderImage2D(projection,modelview,surface,texture_size,texture_size,NULL,0);
				}
				
				// save image
				images.append(new Image(surface));
				
				// update progress dialog
				int progress = (grid_size * y + x + 1) * 100 / (grid_size * grid_size);
				if(dialogProgressUpdate(progress) == 0) {
					delete surface;
					dialogProgressEnd();
					return 0;
				}
			}
		}
		
		delete surface;
		dialogProgressEnd();
		return 1;
	}
}
