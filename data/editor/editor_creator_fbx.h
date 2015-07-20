/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_creator_fbx.h
 * Desc:    Unigine editor
 * Version: 1.01
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Andrey Viktorov <unclebob@unigine.com>
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
#define STRING_PRESSED_CALLBACK(NAME,VAR) \
void NAME ## _pressed() { \
	string cur_string = VAR.getText(); \
	string new_string = fileName(cur_string); \
	if(cur_string != new_string) VAR.setText(new_string); \
}

/*
 */
#define FILL_NAMES(TYPE,UPPER,UPPER_MULTIPLE) \
void fill_ ## TYPE ## _names() { \
	int names_map[] = (:); \
	forloop(int i = 0; import.getNum ## UPPER_MULTIPLE()) { \
		string name = import.get ## UPPER ## Name(i); \
		if(names_map.check(name)) { \
			name = getNewName( \
				name + "_0", \
				[](string name) { return names_map.check(name); } \
			); \
		} \
		names_map.append(name); \
		import_ ## TYPE ## _names.append(name); \
	} \
} \

/*
 */
namespace Creator::FBX {
	
	enum {
		WORKFLOW_METALNESS,
		WORKFLOW_SPECULAR,
	};
	
	using Unigine::Widgets;
	
	/*
	 */
	UserInterface ui;
	
	Dialog dialog;
	Button info_b;
	
	//workflow
	Dialog workflow_d;
	CheckBox specular_cb;
	CheckBox metalness_cb;
	Button workflow_b;
	
	CheckBox merge_meshes_cb;
	CheckBox vertex_cache_cb;
	CheckBox import_joints_cb;
	CheckBox import_lights_cb;
	CheckBox import_cameras_cb;
	CheckBox import_materials_cb;
	CheckBox import_textures_cb;
	CheckBox compress_textures_cb;
	CheckBox import_animation_cb;
	CheckBox import_tangent_space_cb;
	CheckBox verbose_operations_cb;
	
	EditLine meshes_prefix_el;
	EditLine materials_prefix_el;
	EditLine textures_prefix_el;
	
	EditLine scale_el;
	EditLine fps_el;
	
	Label info_l;
	
	string name = "";
	float scale = 1.0f;
	float fps = 30.0f;
	int workflow;
	
	Node nodes[];
	AutodeskImport import;
	
	string library_name;
	string import_path;
	int update_materials;
	string root_name;
	string mesh_base_name;
	int materials[];
	string textures[];
	string meshes[];
	
	string import_mesh_names[0];
	string import_material_names[0];
	
	STRING_PRESSED_CALLBACK(mesh,meshes_prefix_el)
	STRING_PRESSED_CALLBACK(material,materials_prefix_el)
	STRING_PRESSED_CALLBACK(texture,textures_prefix_el)
	FILL_NAMES(mesh,Mesh,Meshes)
	FILL_NAMES(material,Material,Materials)
	
	/*
	 */
	void init() {
		ui = new UserInterface("editor/editor_creator_fbx.ui");
		hotkeysAddUiToolTips(ui);
	}
	
	void shutdown() {
		hotkeysRemoveUiToolTips(ui);
		delete ui;
	}
	
	/*
	 */
	void load_config() {
		vertex_cache_cb.setChecked(configGet("fbx_vertex_cache",1));
		merge_meshes_cb.setChecked(configGet("fbx_merge_meshes",0));
		import_joints_cb.setChecked(configGet("fbx_import_joints",1));
		import_lights_cb.setChecked(configGet("fbx_import_lights",1));
		import_cameras_cb.setChecked(configGet("fbx_import_cameras",1));
		import_materials_cb.setChecked(configGet("fbx_import_materials",1));
		import_textures_cb.setChecked(configGet("fbx_import_textures",1));
		compress_textures_cb.setChecked(configGet("fbx_compress_textures",1));
		import_animation_cb.setChecked(configGet("fbx_import_animation",1));
		verbose_operations_cb.setChecked(configGet("fbx_verbose_operations",0));
		
		meshes_prefix_el.setText(configGet("fbx_meshes_prefix",""));
		materials_prefix_el.setText(configGet("fbx_materials_prefix",""));
		textures_prefix_el.setText(configGet("fbx_textures_prefix",""));
		
		specular_cb.setChecked(configGet("fbx_specular",1));
		metalness_cb.setChecked(configGet("fbx_metalness",0));
		
		scale = configGet("fbx_scale",scale);
		fps = configGet("fbx_fps",fps);
		
		scale_el.setText(editorFormat(scale));
		fps_el.setText(editorFormat(fps));
		
		workflow = (specular_cb.isChecked()) ? WORKFLOW_SPECULAR : WORKFLOW_METALNESS;
	}
	
	void save_config() {
		configSet("fbx_vertex_cache",vertex_cache_cb.isChecked());
		configSet("fbx_merge_meshes",merge_meshes_cb.isChecked());
		configSet("fbx_import_joints",import_joints_cb.isChecked());
		configSet("fbx_import_lights",import_lights_cb.isChecked());
		configSet("fbx_import_cameras",import_cameras_cb.isChecked());
		configSet("fbx_import_materials",import_materials_cb.isChecked());
		configSet("fbx_import_textures",import_textures_cb.isChecked());
		configSet("fbx_compress_textures",compress_textures_cb.isChecked());
		configSet("fbx_import_animation",import_animation_cb.isChecked());
		configSet("fbx_verbose_operations",verbose_operations_cb.isChecked());
		
		configSet("fbx_meshes_prefix",meshes_prefix_el.getText());
		configSet("fbx_materials_prefix",materials_prefix_el.getText());
		configSet("fbx_textures_prefix",textures_prefix_el.getText());
		
		configSet("fbx_scale",scale);
		configSet("fbx_fps",fps);
		
		configSet("fbx_specular",specular_cb.isChecked());
		configSet("fbx_metalness",metalness_cb.isChecked());
	}
	
	/*
	 */
	int show_dialog() {
		
		load_config();
		addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
		dialog.setPermanentFocus();
		
		while(dialog.isDone() == 0) {
			update_dialog();
			wait 1;
		}
		
		removeChild(dialog);
		save_config();
		return dialog.isOkClicked();
	}
	
	void update_dialog() {
		if(import != NULL && engine.world.isLoaded()) {
			info_b.setEnabled(1);
			
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
			fps_el.setEnabled(import_animation_cb.isChecked());
			
			workflow_b.setEnabled(import_materials_cb.isChecked());
		}
		else {
			
			info_b.setEnabled(0);
			
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
			
			workflow_b.setEnabled(0);
		}
	}
	
	Node create(string fbx_name) {
		nodes.clear();
		delete import;
		import = new AutodeskImport();
		
		if(import.load(fbx_name) == 0) {
			dialogMessageOk(TR("Error"));
			delete import;
		}
		
		name = fbx_name;
		if(!show_dialog()) return NULL;
		
		return create_scene();
	}
	
	/*
	 */
	void info_clicked() {
		
		string axes[] = (
			AUTODESK_IMPORT_AXIS_PX : "+X", AUTODESK_IMPORT_AXIS_NX : "-X",
			AUTODESK_IMPORT_AXIS_PY : "+Y", AUTODESK_IMPORT_AXIS_NY : "-Y",
			AUTODESK_IMPORT_AXIS_PZ : "+Z", AUTODESK_IMPORT_AXIS_NZ : "-Z",
		);
		string orientations[] = (
			AUTODESK_IMPORT_ORIENTATION_RIGHT : TR("Right handed"),
			AUTODESK_IMPORT_ORIENTATION_LEFT : TR("Left handed"),
		);
		
		string left = "<tr><td><p align=right>";
		string middle = "</p></td><td><p align=left><font color=#ffffff>";
		string right = "</font></p></td></tr>";
		
		string info = "<table space=3>";
		
		info += left + TR("File:") + middle + basename(name) + right;
		info += left + TR("Version:") + middle + string(import.getVersion()) + right;
		
		if(strlen(import.getCreator())) info += left + TR("Creator:") + middle + import.getCreator() + right;
		if(strlen(import.getTitle())) info += left + TR("Title:") + middle + import.getTitle() + right;
		if(strlen(import.getSubject())) info += left + TR("Subject:") + middle + import.getSubject() + right;
		if(strlen(import.getAuthor())) info += left + TR("Author:") + middle + import.getAuthor() + right;
		if(strlen(import.getKeywords())) info += left + TR("Keywords:") + middle + import.getKeywords() + right;
		if(strlen(import.getRevision())) info += left + TR("Revision:") + middle + import.getRevision() + right;
		if(strlen(import.getComment())) info += left + TR("Comment:") + middle + import.getComment() + right;
		
		info += left + TR("Meter:") + middle + editorFormat(import.getMeter()) + right;
		info += left + TR("Up Axis:") + middle + axes.check(import.getUpAxis(),TR("Unknown")) + right;
		info += left + TR("Front Axis:") + middle + axes.check(import.getFrontAxis(),TR("Unknown")) + right;
		info += left + TR("Orientation:") + middle + orientations.check(import.getOrientation(),TR("Unknown")) + right;
		
		info += "</table>";
		
		delete info_l;
		info_l = new Label(info);
		info_l.setFontRich(1);
		dialogMessageOk(TR("Autodesk FBX info"),info_l.widget);
	}
	
	/*
	 */
	void workflow_clicked() {
		
		addChild(workflow_d,ALIGN_OVERLAP | ALIGN_CENTER);
		workflow_d.setPermanentFocus();
		
		while(workflow_d.isDone() == 0) {
			wait 1;
		}
		
		removeChild(workflow_d);
	}
	
	/*
	 */
	void specular_clicked() {
		metalness_cb.setCallbackEnabled(CLICKED,0);
		specular_cb.setCallbackEnabled(CLICKED,0);
		
		if(!specular_cb.isChecked()) specular_cb.setChecked(1);
		
		metalness_cb.setChecked(0);
		workflow = WORKFLOW_SPECULAR;
		
		metalness_cb.setCallbackEnabled(CLICKED,1);
		specular_cb.setCallbackEnabled(CLICKED,1);
	}
	
	/*
	 */
	void metalness_clicked() {
		metalness_cb.setCallbackEnabled(CLICKED,0);
		specular_cb.setCallbackEnabled(CLICKED,0);
		
		if(!metalness_cb.isChecked()) metalness_cb.setChecked(1);
		
		specular_cb.setChecked(0);
		workflow = WORKFLOW_METALNESS;
		
		metalness_cb.setCallbackEnabled(CLICKED,1);
		specular_cb.setCallbackEnabled(CLICKED,1);
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
		if(import.getLightFarAttenuation(num)) {
			return import.getLightFarAttenuationEnd(num);
		}
		return 10000.0f;
	}
	
	/*
	 */
	Node import_light(int num) {
		
		// light parameters
		int type = import.getLightType(num);
		vec3 color = import.getLightColor(num);
		color *= import.getLightIntensity(num) / 100.0f;
		
		// point light
		if(type == AUTODESK_IMPORT_LIGHT_POINT) {
			float radius = get_light_radius(num);
			LightOmni light = new LightOmni(vec4(color,1.0f),vec3(radius));
			light.setShadow(import.getLightCastShadow(num));
			return light;
		}
		
		// directional light
		if(type == AUTODESK_IMPORT_LIGHT_DIRECTIONAL) {
			LightWorld light = new LightWorld(vec4(color,1.0f));
			light.setShadow(import.getLightCastShadow(num));
			return light;
		}
		
		// spot light
		if(type == AUTODESK_IMPORT_LIGHT_SPOT) {
			float radius = get_light_radius(num);
			float fov = import.getLightOuterAngle(num);
			LightProj light = new LightProj(vec4(color,1.0f),radius,fov);
			light.setShadow(import.getLightCastShadow(num));
			return light;
		}
		
		log.warning("Creator::FBX::import_light(): unknown light type %d\n",type);
		return NULL;
	}
	
	/*
	 */
	Node import_camera(int num) {
		
		// camera parameters
		int type = import.getCameraType(num);
		float fov = import.getCameraFov(num);
		float znear = import.getCameraZNear(num);
		float zfar = import.getCameraZFar(num);
		
		// create player
		Player player = new PlayerDummy();
		player.setZNear(znear);
		player.setZFar(zfar);
		
		// player projection
		if(type == AUTODESK_IMPORT_CAMERA_ORTHOGONAL) player.setProjection(ortho(-1.0f,1.0f,-1.0f,1.0f,znear,zfar));
		else if(type == AUTODESK_IMPORT_CAMERA_PERSPECTIVE) player.setFov(fov);
		else log.warning("Creator::FBX::import_camera(): unknown camera type %d\n",type);
		
		return player;
	}
	
	/*
	 */
	string import_texture(int type,string name,string path,string textures[]) {
		
		if(import_textures_cb.isChecked() == 0) return name;
		if(textures.check(name)) return textures[name];
		
		string prefix = replace(textures_prefix_el.getText(),"\\","/");
		string image_name = path + "textures/" + prefix + extension(basename(name),"dds");
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
		if(type == AUTODESK_IMPORT_PARAMETER_EMISSIVE || type == AUTODESK_IMPORT_PARAMETER_AMBIENT || type == AUTODESK_IMPORT_PARAMETER_DIFFUSE || type == AUTODESK_IMPORT_PARAMETER_SPECULAR || type == AUTODESK_IMPORT_PARAMETER_REFLECTION) {
			if(image.getFormat() != IMAGE_FORMAT_RGBA8) ret = image.convertToFormat(IMAGE_FORMAT_RGB8);
			if(ret) ret = image.createMipmaps(IMAGE_FILTER_LINEAR,2.2f);
			if(ret && compress_textures_cb.isChecked()) ret = image.compress();
		}
		// normal texture
		else if(type == AUTODESK_IMPORT_PARAMETER_NORMALMAP || type == AUTODESK_IMPORT_PARAMETER_BUMPMAP) {
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
			log.warning("Creator::FBX::import_texture(): unknown texture type %d\n",type);
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
		string dir_path = (isAbsolute(path)) ? (path) : (engine.getDataPath() + path);
		dir_path += "textures";
		mkdir(dir_path);
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
		
		// set state
		int set_state(string state,int value) {
			
			// find state
			int id = material.findState(state);
			if(id == -1) return 0;
			
			// set state
			material.setState(id,value);
			
			return 1;
		}
		
		// set texture
		int set_texture(string texture,string default_texture,int type) {
			
			// check image
			string name = import.getMaterialImage(num,type);
			if(strlen(name) == 0) return 0;
			
			// find texture
			int id = material.findTexture(texture);
			if(id == -1) return 0;
			
			// check file
			int is_file = 0;
			File file = new File();
			if(file.open(name,"rb")) {
				is_file = 1;
				file.close();
			} else {
				name = basename(name);
				if(file.open(name,"rb")) {
					is_file = 1;
					file.close();
				}
			}
			delete file;
			
			// import texture
			name = (is_file) ? import_texture(type,name,path,textures) : default_texture;
			if(strlen(name)) material.setImageTextureName(id,name);
			
			return 1;
		}
		
		// set parameter
		int set_parameter(string parameter,vec4 value) {
			
			// find parameter
			int id = material.findParameter(parameter);
			if(id == -1) return 0;
			
			// set parameter
			material.setParameter(id,value);
			
			return 1;
		}
		
		// workflow
		set_state("workflow",workflow);
		
		// emission
		int ret = set_texture("emission","core/textures/common/white.dds",AUTODESK_IMPORT_PARAMETER_EMISSIVE);
		set_parameter("emission_color",vec4(import.getMaterialColor(num,AUTODESK_IMPORT_PARAMETER_EMISSIVE)));
		set_parameter("emission_scale",vec4(import.getMaterialValue(num,AUTODESK_IMPORT_PARAMETER_EMISSIVE)));
		if(ret) set_state("emission",1);
		
		// ambient
		ret = set_texture("lightmap","core/textures/mesh_base_lightmap.dds",AUTODESK_IMPORT_PARAMETER_AMBIENT);
		set_parameter("lightmap_scale",vec4(import.getMaterialValue(num,AUTODESK_IMPORT_PARAMETER_AMBIENT)));
		if(ret) set_state("ambient",3);
		
		if(workflow == WORKFLOW_SPECULAR) {
			
			// diffuse
			set_texture("diffuse","core/textures/common/white.dds",AUTODESK_IMPORT_PARAMETER_DIFFUSE);
			set_parameter("diffuse_color",vec4(import.getMaterialColor(num,AUTODESK_IMPORT_PARAMETER_DIFFUSE)));
			set_parameter("diffuse_scale",vec4(import.getMaterialValue(num,AUTODESK_IMPORT_PARAMETER_DIFFUSE)));
			
			// specular
			set_texture("specular","core/textures/common/white.dds",AUTODESK_IMPORT_PARAMETER_SPECULAR);
			
		} else if(workflow == WORKFLOW_METALNESS) {
			
			// albedo
			set_texture("albedo","core/textures/common/white.dds",AUTODESK_IMPORT_PARAMETER_DIFFUSE);
			set_parameter("albedo_color",vec4(import.getMaterialColor(num,AUTODESK_IMPORT_PARAMETER_DIFFUSE)));
			set_parameter("albedo_scale",vec4(import.getMaterialValue(num,AUTODESK_IMPORT_PARAMETER_DIFFUSE)));
		}
		
		// normalmap
		set_texture("normal","core/textures/mesh_base_normal.dds",AUTODESK_IMPORT_PARAMETER_NORMALMAP);
		set_texture("normal","core/textures/mesh_base_normal.dds",AUTODESK_IMPORT_PARAMETER_BUMPMAP);
	}
	
	/*
	 */
	void create_scene() {
		
		// Autodesk material library
		library_name = "";
		if(import_materials_cb.isChecked() && import.getNumMaterials()) {
			string name;
			if(dialogFileMaterial(TR("Select Autodesk material library"),name)) {
				if(engine.materials.addWorldLibrary(name)) library_name = name;
			}
		}
		
		import_path = pathname(name);
		root_name = extension(basename(name),NULL);
		mesh_base_name = "";
		materials = (:);
		textures = (:);
		meshes = (:);
		import_material_names = ();
		import_mesh_names = ();
		
		fill_mesh_names();
		fill_material_names();
		
		//create nodes
		forloop(int i = 0; import.getNumNodes()) {
			Node node = NULL;
			
			// light node
			if(import_lights_cb.isChecked()) {
				int index = import.getNodeLight(i);
				if(index != -1) node = import_light(index);
			}
			// camera node
			if(import_cameras_cb.isChecked()) {
				int index = import.getNodeCamera(i);
				if(index != -1) node = import_camera(index);
			}
			// joint node
			if(import_joints_cb.isChecked()) {
				int parent = i;
				while(parent != -1) {
					if(import.getNodeJoint(parent)) {
						node = new NodeDummy();
						break;
					}
					parent = import.getNodeParent(parent);
				}
			}
			// mesh node
			int mesh = import.getNodeMesh(i);
			if(mesh != -1) {
				
				// save
				if(!save_mesh(mesh)) {
					dialogMessageOk(TR("Error"));
					nodes.delete();
					return;
				}
				node = create_mesh(i,mesh,meshes[mesh]);
				if(node == NULL) {
					dialogMessageOk(TR("Error"));
					nodes.delete();
					return;
				}
			} else if(node != NULL) node.setTransform(import.getNodeTransform(i,scale));
			
			if(node != NULL) {
				node.setName(import.getNodeName(i));
				nodes.append(i,node);
			}
		}
		
		// create dummies
		Node root_node = NULL;
		Node dummies[] = (:);
		
		foreachkey(int i; nodes) {
			int parent = import.getNodeParent(i);
			while(parent != -1) {
				if(!nodes.check(parent) && !dummies.check(parent)) {
					Node dummy = new NodeDummy();
					dummy.setName(import.getNodeName(parent));
					dummies.append(parent,dummy);
				}
				parent = import.getNodeParent(parent);
			}
		}
		foreachkey(int i; dummies) nodes.append(i,dummies[i]);
		
		// parent nodes map[child index, parent index]
		int parents[] = (:);
		
		// fill parents hierarchy
		foreachkey(int i; nodes) {
			int parent = import.getNodeParent(i);
			if(parent == -1) root_node = nodes[i];
			
			parents.append(i,parent);
		}
		
		//merging meshes
		if(merge_meshes_cb.isChecked()) {
			int parent_indexes[] = (:);
			int nodes_to_remove[] = (:);
			int nodes_to_append[] = (:);
			
			// create merging groups at same level (for nodes that have the same parent)
			forloop(int i = 0; nodes.size()) {
				int nodes_to_merge[] = ();
				int parent = import.getNodeParent(nodes.key(i));
				if(parent == -1 || parent_indexes.check(parent)) continue;
				
				foreachkey(int key; nodes) {
					if(import.getNodeParent(key) == parent && (nodes[key] is ObjectMeshStatic)) nodes_to_merge.append(key);
				}
				
				if(nodes_to_merge.size() > 1) {
					Node merging_node = create_merging_node(nodes_to_merge,parent);
					if(merging_node == NULL) {
						dialogMessageOk(TR("Error"));
						nodes.delete();
						return;
					}
					
					nodes_to_append.append(merging_node,parent);
					parent_indexes.append(parent);
					foreach(int index; nodes_to_merge) nodes_to_remove.append(index);
				}
			}
			
			// delete merged nodes
			foreachkey(int index; nodes_to_remove) {
				delete nodes[index];
				nodes.remove(index);
			}
			
			// add nodes
			foreachkey(Node n; nodes_to_append) {
				int index = nodes.size();
				nodes.append(index,n);
				parents.append(index,nodes_to_append[n]);
			}
		}
		
		if(root_node == NULL) root_node = new NodeDummy();
		root_node.setName(root_name);
		
		// add to world
		foreachkey(int key; parents) {
			int parent = parents[key];
			if(nodes.check(key) && nodes.check(parent)) {
				nodes[parent].addWorldChild(nodes[key]);
			}
		}
		
		if(update_materials) materialsUpdate(1);
		return root_node;
	}
	
	/*
	 */
	Node create_merging_node(int indexes[],int parent) {
		
		Mesh merged_mesh = new Mesh();
		string materials[0] = ();
		
		foreach(int i; indexes) {
			Node node = nodes[i];
			ObjectMeshStatic mesh = node_cast(node);
			
			if(mesh.getNumSurfaces() != 0) {
				Mesh m = new Mesh();
				if(mesh.getMesh(m)) {
					forloop(int j = 0; mesh.getNumSurfaces()) {
						
						string material_name = mesh.getMaterialName(j);
						string surface_name = node.getName() + "_" + material_name;
						materials.append(material_name);
						
						merged_mesh.addMeshSurface(surface_name,m,j);
						merged_mesh.setSurfaceTransform(mat4(node.getWorldTransform()),merged_mesh.getNumSurfaces() - 1);
					}
				}
				delete m;
			}
		}
		
		merged_mesh.createBounds();
		
		// save mesh
		string mesh_prefix = replace(meshes_prefix_el.getText(),"\\","/");
		if(merged_mesh.save(import_path + "meshes/" + fileName(mesh_prefix + root_name + ".mesh")) == 0) {
			delete merged_mesh;
			return NULL;
		}
		
		// create object
		ObjectMeshStatic merged_mesh_static = new ObjectMeshStatic(merged_mesh);
		forloop(int i = 0; merged_mesh_static.getNumSurfaces()) merged_mesh_static.setMaterial(materials[i],i);
		
		Node node = merged_mesh_static;
		string parent_name = nodes[parent].getName();
		node.setName((parent_name == "RootNode") ? root_name : parent_name);
		
		return node;
	}
	
	/*
	 */
	void create_materials(int i,Node node) {
		
		Object object = node;
		forloop(int j = 0; object.getNumSurfaces()) {
			string name;
			int num = import.getNodeMaterial(i,object.getSurfaceName(j));
			if(num == -1) {
				object.setMaterial("mesh_pbr_base",j);
				continue;
			}
			
			name = materials_prefix_el.getText() + import_material_names[num];
			
			// import
			if(import_materials_cb.isChecked() && strlen(library_name)) {
				
				// base material
				if(strlen(mesh_base_name) == 0) {
					mesh_base_name = (strlen(root_name) > 0) ? (root_name + "_base") : (materials_prefix_el.getText() + import.getNodeName(0) + "_base");
					
					if(check_material(mesh_base_name) && engine.materials.isMaterial(mesh_base_name) == 0) {
						engine.materials.inheritMaterial("mesh_pbr_base",library_name,mesh_base_name);
						update_materials = 1;
					}
				}
				
				// create material
				if(materials.check(num) == 0) {
					
					if(check_material(name)) {
						if(engine.materials.isMaterial(name) == 0) engine.materials.inheritMaterial(mesh_base_name,library_name,name);
						Material material = engine.materials.findMaterial(name);
						if(material != NULL) import_material(num,material,import_path,textures);
						update_materials = 1;
					}
					materials.append(num);
				}
			} 
			//using existing material
			else if(!engine.materials.isMaterial(name)) name = "mesh_pbr_base";
			
			object.setMaterial(name,j);
		}
	}
	
	/*
	 */
	int save_mesh(int mesh) {
		
		if(meshes.check(mesh)) return 1;
		
		string mesh_prefix = replace(meshes_prefix_el.getText(),"\\","/");
		string mesh_name = import_path + "meshes/" + fileName(mesh_prefix + import_mesh_names[mesh] + ".mesh");
		
		log.message("Creating \"%s\" mesh\n",mesh_name);
		
		string dir_path = (isAbsolute(import_path)) ? (import_path) : (engine.getDataPath() + import_path);
		dir_path += "meshes";
		mkdir(dir_path);
		
		meshes.append(mesh,mesh_name);
		
		if(!check_file(mesh_name)) return 1;
		
		Mesh m = new Mesh();
		if(import.getMesh(mesh,m,scale,import_tangent_space_cb.isChecked()) == 0) {
			delete m;
			return 0;
		}
		
		if(vertex_cache_cb.isChecked()) m.optimizeIndices(MESH_VERTEX_CACHE);
		
		if(!m.save(mesh_name)) {
			delete m;
			return 0;
		}
		
		delete m;
		return 1;
	}
	
	/*
	 */
	Node create_mesh(int i,int mesh,string name) {
		
		Node node = NULL;
		
		// check file
		Mesh m = new Mesh();
		if(m.info(name) == 0) {
			delete m;
			return node;
		}
		int num_bones = m.getNumBones();
		delete m;
		
		if(num_bones) {
			ObjectMeshSkinned object = new ObjectMeshSkinned(name);
			object.setSpeed(fps);
			object.setTime(0.0f);
			object.setFrame(0,0.0f);
			node = object;
			
			// create animation
			if(import_animation_cb.isChecked()) {
				string animation_name = replace(object.getMeshName(),".mesh",".anim");
				if(check_file(animation_name)) {
					Mesh m = new Mesh();
					if(import.getAnimation(mesh,m,scale,-1,fps) && m.save(animation_name)) {
						object.setAnimation(0,animation_name);
						object.setAnimName(animation_name);
						object.setFrame(0,0);
					} else {
						Mat4 transform = import.getNodeTransform(i,scale);
						node.setTransform(transform);
					}
				}
			}
		} else {
			ObjectMeshStatic object = new ObjectMeshStatic(name);
			node = object;
			
			Mat4 transform = import.getNodeTransform(i,scale);
			node.setTransform(transform);
		}
		
		set_object_parameters(node,mesh);
		create_materials(i,node);
		
		return node;
	}
	
	/*
	 */
	void set_object_parameters(Node node,int import_index) {
		Object object = node;
		object.setMaterial("mesh_pbr_base","*");
		forloop(int i = 0; object.getNumSurfaces()) {
			object.setCastShadow(import.getMeshCastShadow(import_index),i);
			object.setCastWorldShadow(import.getMeshCastShadow(import_index),i);
			object.setReceiveShadow(import.getMeshReceiveShadow(import_index),i);
			object.setReceiveWorldShadow(import.getMeshReceiveShadow(import_index),i);
		}
	}
}
