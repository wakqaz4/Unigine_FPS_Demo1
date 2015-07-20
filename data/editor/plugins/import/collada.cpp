/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    collada.cpp
 * Desc:    Collada plugin
 * Version: 1.05
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
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

/******************************************************************************\
*
* Plugin
*
\******************************************************************************/

/*
 */
string getName() {
	return "Collada";
}

/*
 */
#ifndef HAS_COLLADA_IMPORT

/*
 */
void init() {
	thread(functionid(dialogMessageOk,2),(TR("Error"),TR("Collada plugin library is not loaded.")));
}

#else

/*
 */
void init(PluginMeta m) {
	Collada::init(m);
}

void shutdown() {
	Collada::shutdown();
}

void update(int need_reload) {
	Collada::update(need_reload);
}

void updateLanguage() {
	Collada::updateLanguage();
}

void show() {
	Collada::show();
}
/******************************************************************************\
*
* Collada
*
\******************************************************************************/

/*
 */
namespace Collada {
	
	/*
	 */
	using Unigine::Widgets;
	
	/*
	 */
	UserInterface interface;
	
	Window window;
	Button load_b;
	Button animation_b;
	Button info_b;
	Button create_b;
	
	CheckBox vertex_cache_cb;
	CheckBox import_joints_cb;
	CheckBox import_lights_cb;
	CheckBox import_cameras_cb;
	CheckBox import_materials_cb;
	CheckBox import_textures_cb;
	CheckBox compress_textures_cb;
	CheckBox import_animation_cb;
	CheckBox verbose_operations_cb;
	
	EditLine meshes_prefix_el;
	EditLine materials_prefix_el;
	EditLine textures_prefix_el;
	
	EditLine scale_el;
	EditLine fps_el;
	EditLine time_el;
	Slider time_sl;
	
	Label info_l;
	
	string name = "";
	float scale = 1.0f;
	float fps = 25.0f;
	string library_name = "";
	
	Node nodes[];
	ColladaImport import;
	
	/*
	 */
	PluginMeta meta;
	
	/*
	 */
	void init(PluginMeta m) {
		
		meta = m;
		
		interface = new UserInterface(replace(meta.source,".cpp",".ui"),getName() + "::");
		hotkeysAddUiToolTips(interface);
		
		name = configGet("plugin_collada_name",name);
		
		vertex_cache_cb.setChecked(configGet("plugin_collada_vertex_cache",1));
		import_joints_cb.setChecked(configGet("plugin_collada_import_joints",1));
		import_lights_cb.setChecked(configGet("plugin_collada_import_lights",1));
		import_cameras_cb.setChecked(configGet("plugin_collada_import_cameras",1));
		import_materials_cb.setChecked(configGet("plugin_collada_import_materials",1));
		import_textures_cb.setChecked(configGet("plugin_collada_import_textures",1));
		compress_textures_cb.setChecked(configGet("plugin_collada_compress_textures",1));
		import_animation_cb.setChecked(configGet("plugin_collada_import_animation",1));
		verbose_operations_cb.setChecked(configGet("plugin_collada_verbose_operations",0));
		
		meshes_prefix_el.setText(configGet("plugin_collada_meshes_prefix",""));
		materials_prefix_el.setText(configGet("plugin_collada_materials_prefix",""));
		textures_prefix_el.setText(configGet("plugin_collada_textures_prefix",""));
		
		scale = configGet("plugin_collada_scale",scale);
		fps = configGet("plugin_collada_fps",fps);
		
		scale_el.setText(editorFormat(scale));
		fps_el.setText(editorFormat(fps));
		time_el.setText(editorFormat(0.0f));
		
		update_enabled();
		
		window.arrange();
		
		pluginsAddWindow(window,meta.title,meta.name);
	}
	
	/*
	 */
	void shutdown() {
		
		configSet("plugin_collada_name",name);
		
		configSet("plugin_collada_vertex_cache",vertex_cache_cb.isChecked());
		configSet("plugin_collada_import_joints",import_joints_cb.isChecked());
		configSet("plugin_collada_import_lights",import_lights_cb.isChecked());
		configSet("plugin_collada_import_cameras",import_cameras_cb.isChecked());
		configSet("plugin_collada_import_materials",import_materials_cb.isChecked());
		configSet("plugin_collada_import_textures",import_textures_cb.isChecked());
		configSet("plugin_collada_compress_textures",compress_textures_cb.isChecked());
		configSet("plugin_collada_import_animation",import_animation_cb.isChecked());
		configSet("plugin_collada_verbose_operations",verbose_operations_cb.isChecked());
		
		configSet("plugin_collada_meshes_prefix",meshes_prefix_el.getText());
		configSet("plugin_collada_materials_prefix",materials_prefix_el.getText());
		configSet("plugin_collada_textures_prefix",textures_prefix_el.getText());
		
		configSet("plugin_collada_scale",scale);
		configSet("plugin_collada_fps",fps);
		
		pluginsRemoveWindow(window,meta.name);
		
		hotkeysRemoveUiToolTips(interface);
		delete interface;
		
		delete info_l;
		
		delete import;
	}
	
	void show() {
		pluginsShowWindow(window,meta.name);
	}
	
	/*
	 */
	void update(int need_reload) {
		
		if(need_reload) {
			library_name = "";
			update_enabled();
		}
	}
	
	/*
	 */
	void updateLanguage() {
		
		interface.updateWidgets();
	}
	
	/**************************************************************************\
	*
	* Callbacks
	*
	\**************************************************************************/
	
	/*
	 */
	void window_close_clicked() {
		windowHide(window,meta.name);
	}
	
	/*
	 */
	void update_enabled() {
		
		load_b.setEnabled(engine.world.isLoaded());
		
		if(import != NULL && engine.world.isLoaded()) {
			
			animation_b.setEnabled(1);
			info_b.setEnabled(1);
			create_b.setEnabled(1);
			
			vertex_cache_cb.setEnabled(1);
			import_joints_cb.setEnabled(1);
			import_lights_cb.setEnabled(1);
			import_cameras_cb.setEnabled(1);
			import_materials_cb.setEnabled(1);
			import_textures_cb.setEnabled(import_materials_cb.isChecked());
			compress_textures_cb.setEnabled(import_materials_cb.isChecked() && import_textures_cb.isChecked());
			import_animation_cb.setEnabled(1);
			verbose_operations_cb.setEnabled(1);
			
			meshes_prefix_el.setEnabled(1);
			materials_prefix_el.setEnabled(import_materials_cb.isChecked());
			textures_prefix_el.setEnabled(import_materials_cb.isChecked() && import_textures_cb.isChecked());
			
			scale_el.setEnabled(1);
			
			if(import.getMinTime() < import.getMaxTime()) {
				fps_el.setEnabled(import_animation_cb.isChecked());
				time_el.setEnabled(1);
				time_sl.setEnabled(1);
			} else {
				fps_el.setEnabled(0);
				time_el.setEnabled(0);
				time_sl.setEnabled(0);
			}
		}
		else {
			
			animation_b.setEnabled(0);
			info_b.setEnabled(0);
			create_b.setEnabled(0);
			
			vertex_cache_cb.setEnabled(0);
			import_joints_cb.setEnabled(0);
			import_lights_cb.setEnabled(0);
			import_cameras_cb.setEnabled(0);
			import_materials_cb.setEnabled(0);
			import_textures_cb.setEnabled(0);
			compress_textures_cb.setEnabled(0);
			import_animation_cb.setEnabled(0);
			verbose_operations_cb.setEnabled(0);
			
			meshes_prefix_el.setEnabled(0);
			materials_prefix_el.setEnabled(0);
			textures_prefix_el.setEnabled(0);
			
			scale_el.setEnabled(0);
			fps_el.setEnabled(0);
			time_el.setEnabled(0);
			time_sl.setEnabled(0);
		}
	}
	
	/*
	 */
	void load_clicked() {
		
		if(dialogFile(TR("Select DAE scene file"),".dae",name) == 0) return;
		
		nodes.clear();
		delete import;
		
		import = new ColladaImport();
		if(import.load(name) == 0) {
			dialogMessageOk(TR("Error"));
			delete import;
		}
		
		time_el.setText(editorFormat(0.0f));
		time_sl.setCallbackEnabled(CHANGED,0);
		time_sl.setValue(0);
		time_sl.setCallbackEnabled(CHANGED,1);
		
		update_enabled();
	}
	
	void animation_clicked() {
		
		if(dialogFile(TR("Select DAE animation file"),".dae",name) == 0) return;
		
		if(import.loadAnimation(name) == 0) {
			dialogMessageOk(TR("Error"));
		}
		
		time_el.setText(editorFormat(0.0f));
		time_sl.setCallbackEnabled(CHANGED,0);
		time_sl.setValue(0);
		time_sl.setCallbackEnabled(CHANGED,1);
		
		update_enabled();
	}
	
	void info_clicked() {
		
		string axes[] = (
			COLLADA_IMPORT_AXIS_X : "X",
			COLLADA_IMPORT_AXIS_Y : "Y",
			COLLADA_IMPORT_AXIS_Z : "Z",
		);
		
		string left = "<tr><td><p align=right>";
		string middle = "</p></td><td><p align=left><font color=#ffffff>";
		string right = "</font></p></td></tr>";
		
		string info = "<table space=3>";
		
		info += left + TR("File:") + middle + basename(name) + right;
		
		if(strlen(import.getAuthor())) info += left + TR("Author:") + middle + import.getAuthor() + right;
		if(strlen(import.getTool())) info += left + TR("Tool:") + middle + import.getTool() + right;
		
		info += left + TR("Meter:") + middle + editorFormat(import.getMeter()) + right;
		info += left + TR("Up Axis:") + middle + axes.check(import.getUpAxis(),TR("Unknown")) + right;
		
		if(import.getMinTime() < import.getMaxTime()) info += left + TR("Time:") + middle + editorFormat(import.getMinTime()) + "/" + editorFormat(import.getMaxTime()) + right;
		
		info += "</table>";
		
		delete info_l;
		info_l = new Label(info);
		info_l.setFontRich(1);
		dialogMessageOk(TR("Collada info"),info_l.widget);
	}
	
	void create_clicked() {
		create_scene();
	}
	
	/*
	 */
	void scale_pressed() {
		scale = max(float(scale_el.getText()),0.0f);
		scale_el.setText(editorFormat(scale));
	}
	
	void fps_pressed() {
		fps = clamp(float(fps_el.getText()),1.0f,100.0f);
		fps_el.setText(editorFormat(fps));
	}
	
	void time_pressed() {
		float time = clamp(float(time_el.getText()),import.getMinTime(),import.getMaxTime());
		time_el.setText(editorFormat(time));
		time_sl.setCallbackEnabled(CHANGED,0);
		time_sl.setValue(int(1000.0f * (time - import.getMinTime()) / (import.getMaxTime() - import.getMinTime())));
		time_sl.setCallbackEnabled(CHANGED,1);
		set_time(time);
	}
	
	/*
	 */
	void time_changed() {
		float time = import.getMinTime() + (import.getMaxTime() - import.getMinTime()) * time_sl.getValue() / 1000.0f;
		time_el.setText(editorFormat(time));
		set_time(time);
	}
	
	/**************************************************************************\
	*
	* Import
	*
	\**************************************************************************/
	
	/*
	 */
	int check_file(string name) {
		if(verbose_operations_cb.isChecked() == 0) return 1;
		return dialogFileConfirm(name);
	}
	
	int check_material(string name) {
		if(verbose_operations_cb.isChecked() == 0) return 1;
		if(engine.materials.isMaterial(name) == 0) return 1;
		return dialogMessageYesNo(TR("Warning"),format(TR("Override \"%s\" material?"),name));
	}
	
	/*
	 */
	float get_light_radius(int num) {
		float radius = 10000.0f;
		float attenuation = 1000.0f;
		float linear = import.getLightLinearAttenuation(num);
		float quadratic = import.getLightQuadraticAttenuation(num);
		if(linear > 0.0f) radius = min(radius,attenuation / linear);
		if(quadratic > 0.0f) radius = min(radius,sqrt(attenuation / quadratic));
		return radius * scale;
	}
	
	/*
	 */
	Node import_light(int num) {
		
		// light parameters
		int type = import.getLightType(num);
		vec3 color = import.getLightColor(num);
		
		// ambinet light
		if(type == COLLADA_IMPORT_LIGHT_AMBIENT) {
			engine.render.setAmbientColor(vec4(color,1.0f));
			return NULL;
		}
		
		// directional light
		if(type == COLLADA_IMPORT_LIGHT_DIRECTIONAL) {
			LightWorld light = new LightWorld(vec4(color,1.0f));
			light.setShadow(0);
			return light;
		}
		
		// point light
		if(type == COLLADA_IMPORT_LIGHT_POINT) {
			float radius = get_light_radius(num);
			LightOmni light = new LightOmni(vec4(color,1.0f),vec3(radius));
			light.setShadow(0);
			return light;
		}
		
		// spot light
		if(type == COLLADA_IMPORT_LIGHT_SPOT) {
			float radius = get_light_radius(num);
			float fov = import.getLightFalloffAngle(num);
			LightProj light = new LightProj(vec4(color,1.0f),radius,fov);
			light.setShadow(0);
			return light;
		}
		
		log.warning("Collada::import_light(): unknown light type %d\n",type);
		return NULL;
	}
	
	/*
	 */
	Node import_camera(int num) {
		
		// camera parameters
		int type = import.getCameraType(num);
		float xmagfov = import.getCameraXMagFov(num);
		float ymagfov = import.getCameraYMagFov(num);
		float znear = import.getCameraZNear(num);
		float zfar = import.getCameraZFar(num);
		
		// create player
		Player player = new PlayerDummy();
		player.setZNear(znear);
		player.setZFar(zfar);
		
		// player projection
		if(type == COLLADA_IMPORT_CAMERA_PERSPECTIVE) player.setFov(ymagfov);
		else if(type == COLLADA_IMPORT_CAMERA_ORTHOGRAPHIC) player.setProjection(ortho(-xmagfov,xmagfov,-ymagfov,ymagfov,znear,zfar));
		else log.warning("Collada::import_camera(): unknown camera type %d\n",type);
		
		return player;
	}
	
	/*
	 */
	string import_texture(string type,string name,string path,string textures[]) {
		
		if(import_textures_cb.isChecked() == 0) return name;
		if(textures.check(name)) return textures[name];
		
		string prefix = replace(textures_prefix_el.getText(),"\\","/");
		string image_name = path + "textures/" + fileName(prefix + extension(basename(name),"dds"));
		if(check_file(image_name) == 0) return name;
		
		// load image
		Image image = new Image();
		if(image.load(name) == 0) {
			delete image;
			return NULL;
		}
		
		int ret = 1;
		log.message("Creating \"%s\" texture\n",image_name);
		
		// color texture
		if(type == "ambient" || type == "diffuse" || type == "specular") {
			if(image.getFormat() != IMAGE_FORMAT_RGBA8) ret = image.convertToFormat(IMAGE_FORMAT_RGB8);
			if(ret) ret = image.createMipmaps(IMAGE_FILTER_LINEAR,2.2f);
			if(ret && compress_textures_cb.isChecked()) ret = image.compress();
		}
		// normal texture
		else if(type == "bump") {
			if(image.isUShortFormat()) {
				ret = image.convertToFormat(IMAGE_FORMAT_RG16);
				if(ret) ret = image.createMipmaps();
			} else {
				ret = image.convertToFormat(IMAGE_FORMAT_RG8);
				if(ret) ret = image.createMipmaps();
				if(ret && compress_textures_cb.isChecked()) ret = image.compress();
			}
		}
		// unknown texture
		else {
			log.warning("Collada::import_texture(): unknown texture type %s\n",type);
			textures.append(name,name);
			delete image;
			return name;
		}
		
		// check texture
		if(ret == 0) {
			textures.append(name,name);
			delete image;
			return name;
		}
		
		// save texture
		mkdir(engine.getDataPath() + path + "textures");
		if(image.save(image_name) == 0)  {
			textures.append(name,name);
			delete image;
			return name;
		}
		
		textures.append(name,image_name);
		delete image;
		return image_name;
	}
	
	/*
	 */
	void import_material(int num,Material material,string path,string textures[]) {
		
		forloop(int i = 0; import.getNumMaterialParameters(num)) {
			string name = import.getMaterialParameterName(num,i);
			string image = import.getMaterialParameterImage(num,i);
			
			// texture parameter
			if(strlen(image)) {
				
				// check file
				int is_file = 0;
				File file = new File();
				if(file.open(image,"rb")) {
					is_file = 1;
					file.close();
				} else {
					image = basename(image);
					if(file.open(image,"rb")) {
						is_file = 1;
						file.close();
					}
				}
				delete file;
				
				// select file
				if(is_file == 0) {
					if(dialogMessageYesNo(TR("Error"),format(TR("Can't open \"%s\" texture.\nSelect a %s texture manually?"),image,name)) == 0) continue;
					if(dialogFileImage(format(TR("Select %s texture"),name),image) == 0) continue;
				}
				
				// assign texture
				if(name == "ambient") {
					int id = material.findState("ambient");
					if(id != -1) material.setState(id,3);
					id = material.findTexture("lightmap");
					if(id == -1) continue;
					image = import_texture(name,image,path,textures);
					if(strlen(image)) material.setImageTextureName(id,image);
				}
				else if(name == "diffuse") {
					int id = material.findTexture("diffuse");
					if(id == -1) continue;
					image = import_texture(name,image,path,textures);
					if(strlen(image)) material.setImageTextureName(id,image);
				}
				else if(name == "specular") {
					int id = material.findTexture("specular");
					if(id == -1) continue;
					image = import_texture(name,image,path,textures);
					if(strlen(image)) material.setImageTextureName(id,image);
				}
				else if(name == "bump") {
					int id = material.findTexture("normal");
					if(id == -1) continue;
					image = import_texture(name,image,path,textures);
					if(strlen(image)) material.setImageTextureName(id,image);
				}
			}
			
			// constant parameter
			else {
				
				vec4 value = import.getMaterialParameterValue(num,i);
				
				if(name == "diffuse") {
					int id = material.findParameter("diffuse_color");
					if(id != -1) material.setParameter(id,value);
				}
				else if(name == "specular") {
					int id = material.findParameter("specular_color");
					if(id != -1) material.setParameter(id,value.xyz1);
				}
				else if(name == "shininess") {
					int id = material.findParameter("specular_power");
					if(id != -1) material.setParameter(id,max(value,vec4(4.0f)));
				}
			}
		}
	}
	
	/*
	 */
	void create_scene() {
		
		// remove nodes
		if(nodes.size() && dialogMessageYesNo(TR("Confirm"),TR("Remove old nodes?"))) {
			foreach(Node node; nodes) {
				if(engine.editor.isNode(node)) engine.editor.removeNode(node);
			}
		}
		nodes.clear();
		
		// Collada material library
		if(import_materials_cb.isChecked() && import.getNumMaterials() && strlen(library_name) == 0) {
			string name;
			if(dialogFileMaterial(TR("Select Collada material library"),name)) {
				if(engine.materials.addWorldLibrary(name)) library_name = name;
			}
		}
		
		// import path
		string path = pathname(name);
		
		// base material name
		string mesh_base_name = "";
		
		// created objects
		string materials[];
		string textures[];
		string meshes[];
		
		// root node
		Node root = NULL;
		
		// update materials
		int update_materials = 0;
		
		// create nodes
		forloop(int i = 0; import.getNumNodes()) {
			
			// node type
			int light = import.getNodeLight(i);
			int camera = import.getNodeCamera(i);
			int mesh = import.getNodeMesh(i);
			int skin = import.getNodeSkin(i);
			int morph = import.getNodeMorph(i);
			
			Node node = NULL;
			
			// joint node
			if(import_joints_cb.isChecked() == 0) {
				int is_joint = 0;
				int parent = i;
				while(parent != -1) {
					if(import.getNodeJoint(parent)) is_joint = 1;
					parent = import.getNodeParent(parent);
				}
				if(is_joint) continue;
			}
			
			// light node
			if(light != -1) {
				if(import_lights_cb.isChecked()) node = import_light(light);
			}
			
			// camera node
			else if(camera != -1) {
				if(import_cameras_cb.isChecked()) node = import_camera(camera);
			}
			
			// gemetry node
			else if(mesh != -1) {
				
				// save mesh
				if(meshes.check(mesh) == 0) {
					
					string prefix = replace(meshes_prefix_el.getText(),"\\","/");
					string name = path + "meshes/" + fileName(prefix + import.getMeshID(mesh) + ".mesh");
					
					log.message("Creating \"%s\" mesh\n",name);
					mkdir(engine.getDataPath() + path + "meshes");
					if(check_file(name)) {
						Mesh m = new Mesh();
						if(import.getMesh(mesh,skin,morph,m,scale) == 0) {
							dialogMessageOk(TR("Error"));
							nodes.delete();
							delete m;
							return;
						}
						if(vertex_cache_cb.isChecked()) {
							m.optimizeIndices(MESH_VERTEX_CACHE);
						}
						if(m.save(name) == 0) {
							dialogMessageOk(TR("Error"));
							nodes.delete();
							delete m;
							return;
						}
					}
					
					meshes.append(mesh,name);
				}
				
				// check file
				Mesh m = new Mesh();
				if(m.info(meshes[mesh]) == 0) {
					dialogMessageOk(TR("Error"));
					nodes.delete();
					delete m;
					return;
				}
				int num_bones = m.getNumBones();
				delete m;
				
				// create mesh
				if(num_bones) {
					ObjectMeshSkinned object = new ObjectMeshSkinned(meshes[mesh]);
					object.setSpeed(fps);
					object.setTime(0.0f);
					object.setFrame(0,0.0f);
					node = object;
				} else {
					ObjectMeshStatic object = new ObjectMeshStatic(meshes[mesh]);
					node = object;
				}
				
				// object parameters
				Object object = node;
				object.setMaterial("mesh_base","*");
				
				// create materials
				if(import_materials_cb.isChecked() && strlen(library_name)) {
					
					forloop(int j = 0; object.getNumSurfaces()) {
						int num = import.getNodeMaterial(i,object.getSurfaceName(j));
						if(num == -1) continue;
						
						// base material
						if(strlen(mesh_base_name) == 0) {
							mesh_base_name = materials_prefix_el.getText() + import.getNodeName(0) + "_base";
							if(check_material(mesh_base_name) && engine.materials.isMaterial(mesh_base_name) == 0) {
								engine.materials.inheritMaterial("mesh_base",library_name,mesh_base_name);
								update_materials = 1;
							}
						}
						
						// material name
						string name = import.getMaterialName(num);
						if(strlen(name) == 0) name = import.getMaterialID(num);
						name = materials_prefix_el.getText() + name;
						
						// create material
						if(materials.check(num) == 0) {
							if(check_material(name)) {
								if(engine.materials.isMaterial(name) == 0) engine.materials.inheritMaterial(mesh_base_name,library_name,name);
								Material material = engine.materials.findMaterial(name);
								if(material != NULL) import_material(num,material,path,textures);
								update_materials = 1;
							}
							materials.append(num);
						}
						
						object.setMaterial(name,j);
					}
				}
			}
			
			// dummy node
			if(node == NULL) node = new NodeDummy();
			
			// root node
			if(root == NULL) root = node;
			
			// node name
			string name = import.getNodeName(i);
			log.message("node name (%d): %s\n",i,import.getNodeName(i));
			if(strlen(name) == 0) name = import.getNodeID(i);
			node.setName(name);
			
			// node transformation
			node.setWorldTransform(import.getNodeTransform(i,scale));
			
			// node hierarchy
			int parent = import.getNodeParent(i);
			if(parent != -1 && nodes.check(parent)) nodes[parent].addWorldChild(node);
			nodes.append(i,node);
		}
		
		if(update_materials) materialsUpdate(1);
		
		// animations
		if(import_animation_cb.isChecked()) {
			foreachkey(int i; nodes) {
				Node node = nodes[i];
				if(node.getType() == NODE_OBJECT_MESH_SKINNED) {
					int skin = import.getNodeSkin(i);
					ObjectMeshSkinned object = node;
					string name = replace(object.getMeshName(),".mesh",".anim");
					log.message("Creating \"%s\" animation\n",name);
					if(check_file(name)) {
						Mesh m = new Mesh();
						if(import.getAnimation(skin,m,scale,fps) && m.save(name)) {
							object.setAnimation(0,name);
							object.setAnimName(name);
						}
					}
				}
			}
		}
		
		// check root node
		if(root == NULL) {
			dialogMessageOk(TR("Error"),TR("Can't find root node"));
			nodes.delete();
			return;
		}
		
		node_remove(root);
		
		creatorSetNode(root,"editor/gui/object_mesh.png");
	}
	
	/*
	 */
	void set_time(float time) {
		
		// set time
		import.setTime(time);
		
		// update transformations
		foreachkey(int i; nodes) {
			Node node = nodes[i];
			if(engine.world.isNode(node) == 0) continue;
			if(node.getType() == NODE_OBJECT_MESH_SKINNED) {
				ObjectMeshSkinned mesh = node;
				float frame = (time - import.getMinTime()) * fps;
				frame = clamp(frame,0.0f,mesh.getNumFrames(0) - 1.0f);
				mesh.setFrame(0,frame);
			}
			node.setWorldTransform(import.getNodeTransform(i,scale));
		}
	}
}

#endif /* HAS_COLLADA_IMPORT */
