/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    openflight.cpp
 * Desc:    OpenFlight plugin
 * Version: 1.02
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
	return "OpenFlight";
}

/*
 */
#ifndef HAS_OPEN_FLIGHT_IMPORT

/*
 */
void init() {
	thread(functionid(dialogMessageOk,2),(TR("Error"),TR("OpenFlight plugin library is not loaded.")));
}

#else

/*
 */
void init(PluginMeta m) {
	OpenFlight::init(m);
}

void shutdown() {
	OpenFlight::shutdown();
}

void update(int need_reload) {
	OpenFlight::update(need_reload);
}

void updateLanguage() {
	OpenFlight::updateLanguage();
}

void show() {
	OpenFlight::show();
}
/******************************************************************************\
*
* OpenFlight
*
\******************************************************************************/

/*
 */
namespace OpenFlight {
	
	/*
	 */
	using Unigine::Widgets;
	
	/*
	 */
	UserInterface interface;
	
	Window window;
	Button load_b;
	Button info_b;
	Button create_b;
	
	CheckBox merge_object_cb;
	CheckBox merge_group_cb;
	CheckBox merge_lod_cb;
	CheckBox mesh_dynamic_cb;
	CheckBox vertex_color_cb;
	CheckBox vertex_cache_cb;
	CheckBox import_lights_cb;
	CheckBox import_materials_cb;
	CheckBox instance_materials_cb;
	CheckBox import_textures_cb;
	CheckBox compress_textures_cb;
	CheckBox verbose_operations_cb;
	
	EditLine meshes_prefix_el;
	EditLine materials_prefix_el;
	EditLine textures_prefix_el;
	
	EditLine scale_el;
	
	Label info_l;
	
	string name = "";
	float scale = 1.0f;
	string library_name = "";
	
	Node nodes[];
	OpenFlightImport import;
	
	/*
	 */
	PluginMeta meta;
	
	/*
	 */
	void init(PluginMeta m) {
		
		meta = m;
		
		interface = new UserInterface(replace(meta.source,".cpp",".ui"),getName() + "::");
		hotkeysAddUiToolTips(interface);
		
		name = configGet("plugin_openflight_name",name);
		
		merge_object_cb.setChecked(configGet("plugin_openflight_merge_object",0));
		merge_group_cb.setChecked(configGet("plugin_openflight_merge_group",0));
		merge_lod_cb.setChecked(configGet("plugin_openflight_merge_lod",0));
		mesh_dynamic_cb.setChecked(configGet("plugin_openflight_mesh_dynamic",1));
		vertex_color_cb.setChecked(configGet("plugin_openflight_vertex_color",1));
		vertex_cache_cb.setChecked(configGet("plugin_openflight_vertex_cache",1));
		import_lights_cb.setChecked(configGet("plugin_openflight_import_lights",1));
		import_materials_cb.setChecked(configGet("plugin_openflight_import_materials",1));
		instance_materials_cb.setChecked(configGet("plugin_openflight_instance_materials",1));
		import_textures_cb.setChecked(configGet("plugin_openflight_import_textures",1));
		compress_textures_cb.setChecked(configGet("plugin_openflight_compress_textures",1));
		verbose_operations_cb.setChecked(configGet("plugin_openflight_verbose_operations",0));
		
		meshes_prefix_el.setText(configGet("plugin_openflight_meshes_prefix",""));
		materials_prefix_el.setText(configGet("plugin_openflight_materials_prefix",""));
		textures_prefix_el.setText(configGet("plugin_openflight_textures_prefix",""));
		
		scale = configGet("plugin_openflight_scale",scale);
		
		scale_el.setText(editorFormat(scale));
		
		update_enabled();
		
		window.arrange();
		
		pluginsAddWindow(window,meta.title,meta.name);
	}
	
	/*
	 */
	void shutdown() {
		
		configSet("plugin_openflight_name",name);
		
		configSet("plugin_openflight_merge_object",merge_object_cb.isChecked());
		configSet("plugin_openflight_merge_group",merge_group_cb.isChecked());
		configSet("plugin_openflight_merge_lod",merge_lod_cb.isChecked());
		configSet("plugin_openflight_mesh_dynamic",mesh_dynamic_cb.isChecked());
		configSet("plugin_openflight_vertex_color",vertex_color_cb.isChecked());
		configSet("plugin_openflight_vertex_cache",vertex_cache_cb.isChecked());
		configSet("plugin_openflight_import_lights",import_lights_cb.isChecked());
		configSet("plugin_openflight_import_materials",import_materials_cb.isChecked());
		configSet("plugin_openflight_instance_materials",instance_materials_cb.isChecked());
		configSet("plugin_openflight_import_textures",import_textures_cb.isChecked());
		configSet("plugin_openflight_compress_textures",compress_textures_cb.isChecked());
		configSet("plugin_openflight_verbose_operations",verbose_operations_cb.isChecked());
		
		configSet("plugin_openflight_meshes_prefix",meshes_prefix_el.getText());
		configSet("plugin_openflight_materials_prefix",materials_prefix_el.getText());
		configSet("plugin_openflight_textures_prefix",textures_prefix_el.getText());
		
		configSet("plugin_openflight_scale",scale);
		
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
			
			info_b.setEnabled(1);
			create_b.setEnabled(1);
			
			merge_object_cb.setEnabled(1);
			merge_group_cb.setEnabled(1);
			merge_lod_cb.setEnabled(1);
			mesh_dynamic_cb.setEnabled(1);
			vertex_color_cb.setEnabled(1);
			vertex_cache_cb.setEnabled(mesh_dynamic_cb.isChecked() == 0);
			import_lights_cb.setEnabled(1);
			import_materials_cb.setEnabled(1);
			instance_materials_cb.setEnabled(import_materials_cb.isChecked());
			import_textures_cb.setEnabled(import_materials_cb.isChecked());
			compress_textures_cb.setEnabled(import_materials_cb.isChecked() && import_textures_cb.isChecked());
			verbose_operations_cb.setEnabled(1);
			
			meshes_prefix_el.setEnabled(mesh_dynamic_cb.isChecked() == 0);
			materials_prefix_el.setEnabled(import_materials_cb.isChecked() && instance_materials_cb.isChecked() == 0);
			textures_prefix_el.setEnabled(import_materials_cb.isChecked() && instance_materials_cb.isChecked() == 0 && import_textures_cb.isChecked());
			
			scale_el.setEnabled(1);
		}
		else {
			
			info_b.setEnabled(0);
			create_b.setEnabled(0);
			
			merge_object_cb.setEnabled(0);
			merge_group_cb.setEnabled(0);
			merge_lod_cb.setEnabled(0);
			mesh_dynamic_cb.setEnabled(0);
			vertex_color_cb.setEnabled(0);
			vertex_cache_cb.setEnabled(0);
			import_lights_cb.setEnabled(0);
			import_materials_cb.setEnabled(0);
			instance_materials_cb.setEnabled(0);
			import_textures_cb.setEnabled(0);
			compress_textures_cb.setEnabled(0);
			verbose_operations_cb.setEnabled(0);
			
			meshes_prefix_el.setEnabled(0);
			materials_prefix_el.setEnabled(0);
			textures_prefix_el.setEnabled(0);
			
			scale_el.setEnabled(0);
		}
	}
	
	/*
	 */
	void load_clicked() {
		
		if(dialogFile(TR("Select FLT scene file"),".flt",name) == 0) return;
		
		engine.console.run("filesystem_reload");
		
		nodes.clear();
		delete import;
		
		import = new OpenFlightImport();
		if(import.load(name) == 0) {
			dialogMessageOk(TR("Error"));
			delete import;
		}
		
		update_enabled();
	}
	
	void info_clicked() {
		
		string units[] = (
			OPEN_FLIGHT_UNIT_METERS : TR("Meters"),
			OPEN_FLIGHT_UNIT_KILOMETERS : TR("Kilometers"),
			OPEN_FLIGHT_UNIT_FEET : TR("Feet"),
			OPEN_FLIGHT_UNIT_INCHES : TR("Inches"),
			OPEN_FLIGHT_UNIT_NAUTICAL_MILES : TR("Nautical miles"),
		);
		string projections[] = (
			OPEN_FLIGHT_PROJECTION_FLAT_EARTH : TR("Flat Earth"),
			OPEN_FLIGHT_PROJECTION_TRAPEZOIDAL : TR("Trapezoidal"),
			OPEN_FLIGHT_PROJECTION_ROUND_EARTH : TR("Round Earth"),
			OPEN_FLIGHT_PROJECTION_LAMBERT : TR("Lambert"),
			OPEN_FLIGHT_PROJECTION_UTM : TR("UTM"),
			OPEN_FLIGHT_PROJECTION_GEODETIC : TR("Geodetic"),
			OPEN_FLIGHT_PROJECTION_GEOCENTRIC : TR("Geocentric"),
		);
		string origins[] = (
			OPEN_FLIGHT_ORIGIN_OPEN_FLIGHT : TR("OpenFlight"),
			OPEN_FLIGHT_ORIGIN_DIG12 : TR("DIG I/DIG II"),
			OPEN_FLIGHT_ORIGIN_CTA5A6 : TR("Evans and Sutherland CT5A/CT6"),
			OPEN_FLIGHT_ORIGIN_PSP_DIG : TR("PSP DIG"),
			OPEN_FLIGHT_ORIGIN_CIV : TR("General Electric CIV/CV/PT2000"),
			OPEN_FLIGHT_ORIGIN_GDF : TR("Evans and Sutherland GDF"),
		);
		string models[] = (
			OPEN_FLIGHT_MODEL_USER : TR("User defined"),
			OPEN_FLIGHT_MODEL_WGS84 : TR("WGS 1984"),
			OPEN_FLIGHT_MODEL_WGS72 : TR("WGS 1972"),
			OPEN_FLIGHT_MODEL_BESSEL : TR("Bessel"),
			OPEN_FLIGHT_MODEL_CLARKE : TR("Clarke"),
			OPEN_FLIGHT_MODEL_NAD27 : TR("NAD 1927"),
		);
		
		string left = "<tr><td><p align=right>";
		string middle = "</p></td><td><p align=left><font color=#ffffff>";
		string right = "</font></p></td></tr>";
		
		string info = "<table space=3>";
		
		info += left + TR("File:") + middle + basename(name) + right;
		info += left + TR("Format revision:") + middle + string(import.getFormatRevision()) + right;
		info += left + TR("Edit revision/date:") + middle + string(import.getEditRevision()) + "/" + replace(import.getEditDate(),"\n","") + right;
		info += left + TR("Coordinate units:") + middle + units.check(import.getCoordinateUnits(),TR("Unknown")) + right;
		info += left + TR("Projection type:") + middle + projections.check(import.getProjectionType(),TR("Unknown")) + right;
		info += left + TR("Database origin:") + middle + origins.check(import.getDatabaseOrigin(),TR("Unknown")) + right;
		info += left + TR("Earth model:") + middle + models.check(import.getEarthModel(),TR("Unknown")) + right;
		info += left + TR("UTM zone:") + middle + string(import.getUTMZone()) + right;
		info += left + TR("Southwest X/Y:") + middle + string(import.getSouthwestX()) + "/" + string(import.getSouthwestY()) + right;
		info += left + TR("Delta X/Y/Z:") + middle + string(import.getDeltaX()) + "/" + string(import.getDeltaY()) + "/" + string(import.getDeltaZ()) + right;
		info += left + TR("Southwest lat/long:") + middle + string(import.getSouthwestLatitude()) + "/" + string(import.getSouthwestLongitude()) + right;
		info += left + TR("Northeast lat/long:") + middle + string(import.getNortheastLatitude()) + "/" + string(import.getNortheastLongitude()) + right;
		info += left + TR("Origin lat/long:") + middle + string(import.getOriginLatitude()) + "/" + string(import.getOriginLongitude()) + right;
		info += left + TR("Lambert upper lat:") + middle + string(import.getUpperLatitude()) + right;
		info += left + TR("Lambert lower lat:") + middle + string(import.getLowerLatitude()) + right;
		
		info += "</table>";
		
		delete info_l;
		info_l = new Label(info);
		info_l.setFontRich(1);
		dialogMessageOk(TR("OpenFlight info"),info_l.widget);
	}
	
	void create_clicked() {
		
		int flags = 0;
		if(merge_object_cb.isChecked()) flags |= OPEN_FLIGHT_IMPORT_MERGE_OBJECT;
		if(merge_group_cb.isChecked()) flags |= OPEN_FLIGHT_IMPORT_MERGE_GROUP;
		if(merge_lod_cb.isChecked()) flags |= OPEN_FLIGHT_IMPORT_MERGE_LOD;
		
		if(import.import(flags) == 0) {
			dialogMessageOk(TR("Error"));
			return;
		}
		
		create_scene();
	}
	
	/*
	 */
	void scale_pressed() {
		scale = max(float(scale_el.getText()),0.0f);
		scale_el.setText(editorFormat(scale));
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
		vec4 color = import.getLightDiffuseColor(num);
		color.w = 1.0f;
		
		// inifinite light
		if(type == OPEN_FLIGHT_IMPORT_LIGHT_INFINITE) {
			LightWorld light = new LightWorld(color);
			light.setShadow(0);
			return light;
		}
		
		// local light
		if(type == OPEN_FLIGHT_IMPORT_LIGHT_LOCAL) {
			float radius = get_light_radius(num);
			LightOmni light = new LightOmni(color,vec3(radius));
			light.setShadow(0);
			return light;
		}
		
		// spot light
		if(type == OPEN_FLIGHT_IMPORT_LIGHT_SPOT) {
			float radius = get_light_radius(num);
			float fov = import.getLightFalloffAngle(num);
			LightProj light = new LightProj(color,radius,fov);
			light.setShadow(0);
			return light;
		}
		
		log.warning("OpenFlight::import_light(): unknown light type %d\n",type);
		return NULL;
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
		if(type == OPEN_FLIGHT_IMPORT_PARAMETER_EMISSIVE || type == OPEN_FLIGHT_IMPORT_PARAMETER_AMBIENT || type == OPEN_FLIGHT_IMPORT_PARAMETER_DIFFUSE || type == OPEN_FLIGHT_IMPORT_PARAMETER_SPECULAR || type == OPEN_FLIGHT_IMPORT_PARAMETER_REFLECTION) {
			if(image.getFormat() != IMAGE_FORMAT_RGBA8) ret = image.convertToFormat(IMAGE_FORMAT_RGB8);
			if(ret) ret = image.createMipmaps(IMAGE_FILTER_LINEAR,2.2f);
			if(ret && compress_textures_cb.isChecked()) ret = image.compress();
		}
		// normal texture
		else if(type == OPEN_FLIGHT_IMPORT_PARAMETER_NORMALMAP || type == OPEN_FLIGHT_IMPORT_PARAMETER_BUMPMAP) {
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
			log.warning("OpenFlightImport::import_texture(): unknown texture type %d\n",type);
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
		int set_texture(string texture,int type) {
			
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
			
			// select file
			if(is_file == 0) {
				if(dialogMessageYesNo(TR("Error"),format(TR("Can't open \"%s\" texture.\nSelect a %s texture manually?"),name,texture)) == 0) return 0;
				if(dialogFileImage(format(TR("Select %s texture"),texture),name) == 0) return 0;
			}
			
			// import texture
			name = import_texture(type,name,path,textures);
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
		
		// emission
		int ret = set_texture("emission",OPEN_FLIGHT_IMPORT_PARAMETER_EMISSIVE);
		set_parameter("emission_color",import.getMaterialColor(num,OPEN_FLIGHT_IMPORT_PARAMETER_EMISSIVE));
		if(ret) set_state("emission",1);
		
		// ambient
		ret = set_texture("lightmap",OPEN_FLIGHT_IMPORT_PARAMETER_AMBIENT);
		if(ret) set_state("ambient",3);
		
		// diffuse
		set_texture("diffuse",OPEN_FLIGHT_IMPORT_PARAMETER_DIFFUSE);
		set_parameter("diffuse_color",import.getMaterialColor(num,OPEN_FLIGHT_IMPORT_PARAMETER_DIFFUSE));
		
		// detail diffuse
		set_texture("detail_diffuse",OPEN_FLIGHT_IMPORT_PARAMETER_DETAIL);
		
		// specular
		set_texture("specular",OPEN_FLIGHT_IMPORT_PARAMETER_SPECULAR);
		set_parameter("specular_color",import.getMaterialColor(num,OPEN_FLIGHT_IMPORT_PARAMETER_SPECULAR));
		set_parameter("specular_power",vec4(import.getMaterialValue(num,OPEN_FLIGHT_IMPORT_PARAMETER_SHININESS)));
		
		// normalmap
		set_texture("normal",OPEN_FLIGHT_IMPORT_PARAMETER_NORMALMAP);
		set_texture("normal",OPEN_FLIGHT_IMPORT_PARAMETER_BUMPMAP);
		
		// alpha blending
		if(import.getMaterialAlpha(num)) {
			material.setDepthMask(0);
			material.setPostScattering(1);
			material.setBlendFunc(MATERIAL_BLEND_SRC_ALPHA,MATERIAL_BLEND_ONE_MINUS_SRC_ALPHA);
		}
		
		// polygon offset
		if(import.getMaterialOffset(num)) {
			material.setOffset(import.getMaterialOffset(num));
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
		
		// OpenFlight material library
		if(import_materials_cb.isChecked() && instance_materials_cb.isChecked() == 0 && import.getNumMaterials() && strlen(library_name) == 0) {
			string name;
			if(dialogFileMaterial(TR("Select OpenFlight material library"),name)) {
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
		string names[];
		
		// root node
		Node root = NULL;
		
		// update materials
		int update_materials = 0;
		
		// create nodes
		forloop(int i = 0; import.getNumNodes()) {
			
			// node type
			int light = import.getNodeLight(i);
			int pivot = import.getNodePivot(i);
			int mesh = import.getNodeMesh(i);
			
			Node node = NULL;
			
			// light node
			if(light != -1) {
				if(import_lights_cb.isChecked()) node = import_light(light);
			}
			
			// pivot node
			else if(pivot != -1) {
				
				node = new NodePivot();
				NodePivot(node).setLocalTransform(import.getPivotLocalTransform(pivot));
				NodePivot(node).setPivotTransform(import.getPivotPivotTransform(pivot));
			}
			
			// gemetry node
			else if(mesh != -1) {
				
				// get mesh
				Mesh m = new Mesh();
				if(import.getMesh(mesh,m,scale) == 0) {
					dialogMessageOk(TR("Error"));
					nodes.delete();
					delete m;
					return;
				}
				
				// remove vertex color
				if(vertex_color_cb.isChecked() == 0) {
					forloop(int i = 0; m.getNumSurfaces()) {
						m.setNumColors(0,i);
					}
				}
				
				// dynamic mesh
				if(mesh_dynamic_cb.isChecked()) {
					
					// create mesh
					ObjectMeshDynamic object = new ObjectMeshDynamic(m);
					node = object;
				}
				
				// static mesh
				else {
					
					// save mesh
					if(meshes.check(mesh) == 0) {
						
						string name = replace(import.getMeshName(mesh),"/","_");
						while(strlen(name) == 0 || names.check(name)) name = import.getMeshName(mesh) + format("%0x%0x",rand(0,255),rand(0,255));
						names.append(name);
						
						string prefix = replace(meshes_prefix_el.getText(),"\\","/");
						name = path + "meshes/" + fileName(prefix + name + ".mesh");
						
						// optimize indices
						if(vertex_cache_cb.isChecked()) {
							m.optimizeIndices(MESH_VERTEX_CACHE);
						}
						
						log.message("Creating \"%s\" mesh\n",name);
						mkdir(engine.getDataPath() + path + "meshes");
						if(check_file(name) && m.save(name) == 0) {
							dialogMessageOk(TR("Error"));
							nodes.delete();
							delete m;
							return;
						}
						
						meshes.append(mesh,name);
					}
					
					// create mesh
					ObjectMeshStatic object = new ObjectMeshStatic(meshes[mesh]);
					object.setMaterial("mesh_base","*");
					node = object;
				}
				
				delete m;
				
				// object parameters
				Object object = node;
				object.setMaterial("mesh_base","*");
				object.setProperty("surface_base","*");
				forloop(int j = 0; object.getNumSurfaces()) {
					object.setEnabled(import.getMeshSurfaceEnabled(mesh,j),j);
					object.setMinVisibleDistance(import.getMeshSurfaceMinVisibleDistance(mesh,j),j);
					object.setMaxVisibleDistance(import.getMeshSurfaceMaxVisibleDistance(mesh,j),j);
					object.setMinFadeDistance(import.getMeshSurfaceMinFadeDistance(mesh,j),j);
					object.setMaxFadeDistance(import.getMeshSurfaceMaxFadeDistance(mesh,j),j);
				}
				
				// create materials
				if(import_materials_cb.isChecked()) {
					
					// check mesh
					if(object.getNumSurfaces() != import.getNumMeshSurfaces(mesh)) {
						log.warning("OpenFlight::create_scene(): different number of surfaces %d %d\n",object.getNumSurfaces(),import.getNumMeshSurfaces(mesh));
					}
					
					forloop(int j = 0; min(object.getNumSurfaces(),import.getNumMeshSurfaces(mesh))) {
						
						if(object.getSurfaceName(j) != import.getMeshSurfaceName(mesh,j)) {
							log.warning("OpenFlight::create_scene(): different surface names \"%s\" \"%s\"\n",object.getSurfaceName(j),import.getMeshSurfaceName(mesh,j));
							continue;
						}
						
						int num = import.getMeshSurfaceMaterial(mesh,j);
						if(num == -1) continue;
						
						// instance materials
						if(instance_materials_cb.isChecked()) {
							
							Material material = object.getMaterialInherit(j);
							if(material != NULL) import_material(num,material,path,textures);
						}
						
						// library materials
						else if(strlen(library_name)) {
							
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
			}
			
			// dummy node
			if(node == NULL) node = new NodeDummy();
			
			// root node
			if(root == NULL) root = node;
			
			// node name
			node.setName(import.getNodeName(i));
			
			// node transformation
			node.setWorldTransform(import.getNodeTransform(i,scale));
			
			node.setEnabled(import.getNodeEnabled(i));
			
			// node hierarchy
			int parent = import.getNodeParent(i);
			if(parent != -1 && nodes.check(parent)) nodes[parent].addWorldChild(node);
			nodes.append(i,node);
		}
		
		if(update_materials) materialsUpdate(1);
		
		// check root node
		if(root == NULL) {
			dialogMessageOk(TR("Error"),TR("Can't find root node"));
			nodes.delete();
			return;
		}
		
		node_remove(root);
		
		creatorSetNode(root,"editor/gui/object_mesh.png");
	}
}

#endif /* HAS_OPEN_FLIGHT_IMPORT */
