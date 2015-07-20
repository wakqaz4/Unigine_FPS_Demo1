/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_tools_illumination.h
 * Desc:    Unigine editor
 * Version: 1.01
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

/*
 */
WidgetEditLine step_el;				// light prob step
WidgetEditLine znear_el;			// near clipping plane
WidgetEditLine zfar_el;				// far clipping plane
WidgetEditLine mesh_el;				// bounding mesh
WidgetEditLine expand_el[3];		// bounding box expand
WidgetEditLine merge_el;			// light prob coefficients merge
WidgetEditLine direct_el;			// direct term scale
WidgetEditLine indirect_el;			// indirect term scale
WidgetEditLine radius_el;			// ambient occlusion radius
WidgetComboBox samples_cb;			// ambient occlusion samples
WidgetComboBox supersampling_cb;	// lightmap supersampling
WidgetComboBox extrude_cb;			// lightmap extrude
WidgetComboBox blur_cb;				// lightmap blur

/*
 */
float step = 10.0f;
float znear = 0.01f;
float zfar = 1000.0f;
vec3 expand = vec3_one;
float merge = 0.1f;
float direct = 0.5f;
float indirect = 0.5f;
float radius = 1.0f;

int samples[] = ( 16, 32, 64, 128, 256, 512 );
int scales[] = ( 1, 2, 4, 8 );

/*
 */
class LightMap {
	
	string name;					// lightmap name
	Image image;					// lightmap image
	
	Image direct_image;				// direct lightmap image
	Image indirect_image;			// indirect lightmap image
	
	Unigine::Vector surfaces[];		// object surfaces
	
	Material materials[];			// materials
	
	LightMap(string n,Image i) {
		name = n;
		image = i;
		direct_image = new Image();
		indirect_image = new Image();
	}
	~LightMap() {
		delete image;
		delete direct_image;
		delete indirect_image;
	}
};

class ObjectSurface {
	
	Object object;					// object
	int surface;					// surface number
	
	Material material;				// material
	
	ObjectSurface(Object o,int s,Material m) {
		object = o;
		surface = s;
		material = m;
	}
};

LightMap lightmaps[];
ObjectSurface object_surfaces[0];

/*
 */
void init() {
	
	step = configGet("tools_illumination_step",step);
	znear = configGet("tools_illumination_znear",znear);
	zfar = configGet("tools_illumination_zfar",zfar);
	mesh_el.setText(configGet("tools_illumination_mesh",""));
	expand = configGet("tools_illumination_expand",expand);
	merge = configGet("tools_illumination_merge",merge);
	direct = configGet("tools_illumination_direct",direct);
	indirect = configGet("tools_illumination_indirect",indirect);
	radius = configGet("tools_illumination_radius",radius);
	samples_cb.setCurrentItem(configGet("tools_illumination_samples",2));
	supersampling_cb.setCurrentItem(configGet("tools_illumination_supersampling",0));
	extrude_cb.setCurrentItem(configGet("tools_illumination_extrude",2));
	blur_cb.setCurrentItem(configGet("tools_illumination_blur",0));
	
	step_el.setText(editorFormat(step));
	znear_el.setText(editorFormat(znear));
	zfar_el.setText(editorFormat(zfar));
	setEditLineVec3(expand_el,expand);
	merge_el.setText(editorFormat(merge));
	direct_el.setText(editorFormat(direct));
	indirect_el.setText(editorFormat(indirect));
	radius_el.setText(editorFormat(radius));
}

void shutdown() {
	
	configSet("tools_illumination_step",step);
	configSet("tools_illumination_znear",znear);
	configSet("tools_illumination_zfar",zfar);
	configSet("tools_illumination_mesh",mesh_el.getText());
	configSet("tools_illumination_expand",expand);
	configSet("tools_illumination_merge",merge);
	configSet("tools_illumination_direct",direct);
	configSet("tools_illumination_indirect",indirect);
	configSet("tools_illumination_radius",radius);
	configSet("tools_illumination_samples",samples_cb.getCurrentItem());
	configSet("tools_illumination_supersampling",supersampling_cb.getCurrentItem());
	configSet("tools_illumination_extrude",extrude_cb.getCurrentItem());
	configSet("tools_illumination_blur",blur_cb.getCurrentItem());
}

/*
 */
void update(int need_reload) {
	
	if(need_reload) {
		lightmaps.delete();
		object_surfaces.delete();
	}
}

/*
 */
void step_pressed() {
	step = max(float(step_el.getText()),EPSILON);
	step_el.setText(editorFormat(step));
}

void znear_pressed() {
	znear = max(float(znear_el.getText()),0.0f);
	znear_el.setText(editorFormat(znear));
}

void zfar_pressed() {
	zfar = max(float(zfar_el.getText()),0.0f);
	zfar_el.setText(editorFormat(zfar));
}

void mesh_pressed() {
	if(strlen(mesh_el.getText())) {
		File file = new File();
		if(file.open(mesh_el.getText(),"rb") == 0) {
			dialogMessageOk(TR("Error"),format(TR("Can't open \"%s\" bounding mesh"),mesh_el.getText()));
			mesh_el.setText("");
		} else {
			file.close();
		}
		delete file;
	}
}

void mesh_load_clicked() {
	string name = mesh_el.getText();
	if(dialogFileMesh(TR("Select bounding mesh"),name,DIALOG_FILE_OPEN)) {
		mesh_el.setText(name);
	}
}

void mesh_clear_clicked() {
	if(strlen(mesh_el.getText())) {
		if(dialogMessageYesNo(TR("Confirm"),TR("Clear bounding mesh?"))) {
			mesh_el.setText("");
		}
	}
}

void expand_pressed() {
	expand = max(getEditLineVec3(expand_el),vec3_one);
	setEditLineVec3(expand_el,expand);
}

void merge_pressed() {
	merge = saturate(float(merge_el.getText()));
	merge_el.setText(editorFormat(merge));
}

void direct_pressed() {
	direct = float(direct_el.getText());
	direct_el.setText(editorFormat(direct));
}

void indirect_pressed() {
	indirect = float(indirect_el.getText());
	indirect_el.setText(editorFormat(indirect));
}

void radius_pressed() {
	radius = max(float(radius_el.getText()),0.0f);
	radius_el.setText(editorFormat(radius));
}

/*
 */
void get_objects(Node node,Object objects[]) {
	
	if(node.isEnabled() == 0) return;
	
	if(node.getType() == NODE_OBJECT_MESH_STATIC || node.getType() == NODE_OBJECT_MESH_DYNAMIC) {
		objects.append(node.getID(),node_cast(node));
	}
	
	forloop(int i = 0; node.getNumChilds()) {
		get_objects(node.getChild(i),objects);
	}
}

void get_objects(Object objects[]) {
	
	Node selected_nodes[0];
	nodesGetNodes(selected_nodes);
	
	Node unique_objects[];
	foreach(Node node; selected_nodes) {
		get_objects(node,unique_objects);
	}
	
	foreach(Object object; unique_objects) {
		objects.append(object);
	}
}

/*
 */
void get_object_surfaces(Object objects[]) {
	
	lightmaps.delete();
	object_surfaces.delete();
	
	foreach(Object object; objects) {
		forloop(int i = 0; object.getNumSurfaces()) {
			if(object.isEnabled(i) == 0) continue;
			
			Material material = object.getMaterial(i);
			if(material == NULL) continue;
			if(material.isEditable() == 0) continue;
			if(material.getParent() == NULL) continue;
			
			// check material
			int ambient_id = material.findState("ambient");
			int lightmap_id = material.findTexture("lightmap");
			if(ambient_id == -1 || lightmap_id == -1) continue;
			if(material.getStateType(ambient_id) != MATERIAL_STATE_SWITCH) continue;
			string name = material.getImageTextureName(lightmap_id);
			if(strstr(name,"core/textures") != -1) continue;
			if(strlen(name) == 0) continue;
			
			// create lightmap
			LightMap lightmap = NULL;
			if(lightmaps.check(name)) {
				lightmap = lightmaps[name];
			}
			else {
				Image image = new Image(name);
				if(image.isLoaded() == 0 || image.convertToFormat(IMAGE_FORMAT_RGBA8) == 0) {
					log.error("Can't load \"%s\" lightmap texture\n",name);
					delete image;
					continue;
				}
				lightmap = new LightMap(name,image);
				lightmaps.append(name,lightmap);
			}
			
			// lightmap object surface
			if(lightmap.surfaces.check(object) == 0) {
				lightmap.surfaces[object] = new Unigine::Vector();
			}
			lightmap.surfaces[object].append(i);
			
			// lightmap material
			lightmap.materials.append(material,0);
			
			// create object surface
			object_surfaces.append(new ObjectSurface(object,i,material));
		}
	}
}

/*
 */
int compare_light_prob(LightProb light_0,LightProb light_1) {
	vec3 coefficients_0[9];
	vec3 coefficients_1[9];
	light_0.getCoefficients(coefficients_0);
	light_1.getCoefficients(coefficients_1);
	float threshold = 0.0f;
	forloop(int i = 0; 9) {
		vec3 c0 = coefficients_0[i];
		vec3 c1 = coefficients_1[i];
		vec3 value = max(abs(c0),abs(c1)) * merge;
		threshold = max(threshold,max(max(value.x,value.y),value.z));
	}
	forloop(int i = 0; 9) {
		vec3 c0 = coefficients_0[i];
		vec3 c1 = coefficients_1[i];
		vec3 difference = abs(c1 - c0);
		if(difference.x > threshold) return 0;
		if(difference.y > threshold) return 0;
		if(difference.z > threshold) return 0;
	}
	return 1;
}

LightProb merge_light_prob(LightProb light_0,LightProb light_1) {
	vec3 coefficients[9];
	vec3 coefficients_0[9];
	vec3 coefficients_1[9];
	light_0.getCoefficients(coefficients_0);
	light_1.getCoefficients(coefficients_1);
	forloop(int i = 0; 9) {
		coefficients[i] = (coefficients_0[i] + coefficients_1[i]) / 2.0f;
	}
	vec3 radius_0 = light_0.getRadius();
	vec3 radius_1 = light_1.getRadius();
	vec3 position_0 = light_0.getWorldPosition();
	vec3 position_1 = light_1.getWorldPosition();
	vec3 direction = position_1 - position_0;
	LightProb light = new LightProb(vec4_one,vec3_one);
	light.setRadius((radius_0 + radius_1 + abs(direction)) / 2.0f);
	light.setWorldTransform(translate((position_0 + position_1) / 2.0f));
	light.setCoefficients(coefficients);
	return light;
}

/*
 */
int get_num_intersections(ObjectMeshStatic mesh,vec3 p0,vec3 p1) {
	int num = 0;
	WorldIntersection intersection = new WorldIntersection();
	forloop(int i = 0; mesh.getNumSurfaces()) {
		while(num < 100) {
			if(mesh.getIntersection(p0,p1,i,intersection)) {
				p0 = intersection.getPoint() + normalize(p1 - p0) * 1e-3f;
				num++;
			} else {
				break;
			}
		}
	}
	return num;
}

/*
 */
int confirm_light_prob(Object objects[]) {
	
	Vec3 bound_min = Vec3_infinity;
	Vec3 bound_max = -Vec3_infinity;
	foreach(Object object; objects) {
		bound_min = min(bound_min,object.getWorldBoundBox().getMin());
		bound_max = max(bound_max,object.getWorldBoundBox().getMax());
	}
	Vec3 bound_size = (bound_max - bound_min) * expand;
	
	int size_x = int(ceil(bound_size.x / step));
	int size_y = int(ceil(bound_size.y / step));
	int size_z = int(ceil(bound_size.z / step));
	
	return dialogMessageYesNo(TR("Confirm"),format(TR("%d LightProb will be generated"),(size_x + 1) * (size_y + 1) * (size_z + 1)));
}

/*
 */
int create_light_prob(Object objects[],LightProb lights[]) {
	
	Vec3 bound_min = Vec3_infinity;
	Vec3 bound_max = -Vec3_infinity;
	foreach(Object object; objects) {
		bound_min = min(bound_min,object.getWorldBoundBox().getMin());
		bound_max = max(bound_max,object.getWorldBoundBox().getMax());
	}
	Vec3 bound_size = (bound_max - bound_min) * expand;
	Vec3 bound_center = (bound_min + bound_max) * 0.5f;
	
	int size_x = int(ceil(bound_size.x / step));
	int size_y = int(ceil(bound_size.y / step));
	int size_z = int(ceil(bound_size.z / step));
	
	// bounding mesh
	ObjectMeshStatic mesh = NULL;
	if(strlen(mesh_el.getText())) {
		mesh = new ObjectMeshStatic(mesh_el.getText());
		mesh.setEnabled(0);
		if(mesh.getNumSurfaces() == 0) {
			log.error("Can't load \"%s\" bounding mesh\n",mesh_el.getText());
			delete mesh;
		}
	}
	
	// begin progress dialog
	dialogProgressBegin(TR("Creating LightProb..."));
	
	// save render parameters
	int light_prob_mask = engine.render.getLightProbMask();
	engine.render.setLightProbMask(0);
	
	// create LightProb
	int offset = 0;
	float begin = clock();
	forloop(int z = 0; size_z + 1) {
		float Z = (z - size_z * 0.5f) * step;
		forloop(int y = 0; size_y + 1) {
			float Y = (y - size_y * 0.5f) * step;
			forloop(int x = 0; size_x + 1) {
				float X = (x - size_x * 0.5f) * step;
				
				vec3 position = bound_center + vec3(X,Y,Z);
				if(mesh == NULL || (get_num_intersections(mesh,position,position - bound_size.00z) % 2 && get_num_intersections(mesh,position,position + bound_size.00z) % 2)) {
					LightProb light = new LightProb(vec4_one,vec3(step * 1.5f));
					light.setWorldTransform(translate(position));
					light.renderImageTexture(znear,zfar,128);
					lights.append(light);
				} else {
					offset++;
				}
				
				int progress = (lights.size() + offset) * 100 / ((size_x + 1) * (size_y + 1) * (size_z + 1));
				if(dialogProgressUpdate(progress) == 0) {
					engine.render.setLightProbMask(light_prob_mask);
					dialogProgressEnd();
					lights.delete();
					delete mesh;
					return 0;
				}
			}
		}
	}
	log.message("Creating LightProb done %f seconds\n",clock() - begin);
	
	// restore render parameters
	engine.render.setLightProbMask(light_prob_mask);
	
	// end progress dialog
	dialogProgressEnd();
	
	delete mesh;
	
	if(merge > EPSILON) {
		
		// begin progress dialog
		dialogProgressBegin(TR("Merging LightProb..."));
		
		// merge LightProb
		float begin = clock();
		for(int i = 0; i < lights.size(); i++) {
			
			int index = -1;
			float distance = INFINITY;
			for(int j = i + 1; j < lights.size(); j++) {
				float d = length(lights[i].getWorldPosition() - lights[j].getWorldPosition());
				if(distance > d && compare_light_prob(lights[i],lights[j])) {
					distance = d;
					index = j;
				}
			}
			
			if(index != -1) {
				LightProb light = merge_light_prob(lights[i],lights[index]);
				delete lights[index];
				lights.remove(index);
				delete lights[i];
				lights.remove(i--);
				lights.append(light);
			}
			
			int progress = i * 100 / lights.size();
			if(dialogProgressUpdate(progress) == 0) {
				dialogProgressEnd();
				lights.delete();
				return 0;
			}
		}
		log.message("Merging LightProb done %f seconds\n",clock() - begin);
		
		// end progress dialog
		dialogProgressEnd();
	}
	
	return 1;
}

/*
 */
void create_light_prob_clicked() {
	
	Object objects[0];
	get_objects(objects);
	if(objects.size() == 0) {
		dialogMessageOk(TR("Error"),TR("You must select objects"));
		return;
	}
	
	if(confirm_light_prob(objects) == 0) return;
	
	LightProb lights[0];
	if(create_light_prob(objects,lights) == 0) return;
	if(lights.size() == 0) return;
	
	NodeDummy dummy = class_remove(new NodeDummy());
	engine.editor.addNode(dummy);
	
	forloop(int i = 0; lights.size()) {
		engine.editor.addNode(class_remove(lights[i]));
		dummy.addWorldChild(lights[i]);
	}
}

/*
 */
void copy_lightmap(Image dest,Image src) {
	forloop(int y = 0; src.getHeight()) {
		forloop(int x = 0; src.getWidth()) {
			vec4 color = src.get2D(x,y);
			if(color.w > 0.0f) dest.set2D(x,y,color);
		}
	}
}

/*
 */
void create_lightmap_clicked() {
	
	Object objects[0];
	get_objects(objects);
	get_object_surfaces(objects);
	if(lightmaps.size() == 0) {
		dialogMessageOk(TR("Error"),TR("There are no defined lightmaps"));
		return;
	}
	
	if(confirm_light_prob(objects) == 0) {
		lightmaps.delete();
		object_surfaces.delete();
		return;
	}
	
	// disable lightmaps
	disable_lightmap_clicked();
	
	// begin progress dialog
	dialogProgressBegin(TR("Creating direct lightmaps..."));
	
	// lightmap scale
	int scale = scales[supersampling_cb.getCurrentItem()];
	
	// create direct lightmaps
	float begin = clock();
	forloop(int i = 0; lightmaps.size()) {
		LightMap lightmap = lightmaps.get(i);
		
		// create lightmaps
		int width = lightmap.image.getWidth() * scale;
		int height = lightmap.image.getHeight() * scale;
		lightmap.direct_image.create2D(width,height,IMAGE_FORMAT_RGBA16F);
		lightmap.indirect_image.create2D(width,height,IMAGE_FORMAT_RGBA16F);
		
		// create direct lightmap
		Image image = new Image();
		foreachkey(Object object; lightmap.surfaces) {
			Unigine::Vector vector = lightmap.surfaces[object];
			engine.editor.renderLightMapDirect(object,vector.data,1,width,height,image);
			copy_lightmap(lightmap.direct_image,image);
		}
		delete image;
		
		// update progress dialog
		int progress = i * 100 / lightmaps.size();
		if(dialogProgressUpdate(progress) == 0) {
			dialogProgressEnd();
			lightmaps.delete();
			object_surfaces.delete();
			return;
		}
	}
	log.message("Creating direct lightmaps done %f seconds\n",clock() - begin);
	
	// end progress dialog
	dialogProgressEnd();
	
	// create light prob
	LightProb lights[0];
	if(create_light_prob(objects,lights) == 0) {
		lightmaps.delete();
		object_surfaces.delete();
		return;
	}
	
	if(lights.size()) {
		
		engine.world.updateSpatial();
		
		// begin progress dialog
		dialogProgressBegin(TR("Creating indirect lightmaps..."));
		
		// create indirect lightmaps
		float begin = clock();
		forloop(int i = 0; lightmaps.size()) {
			LightMap lightmap = lightmaps.get(i);
			
			// create indirect lightmap
			Image image = new Image();
			int width = lightmap.image.getWidth() * scale;
			int height = lightmap.image.getHeight() * scale;
			foreachkey(Object object; lightmap.surfaces) {
				Unigine::Vector vector = lightmap.surfaces[object];
				engine.editor.renderLightMapIndirect(object,vector.data,samples[samples_cb.getCurrentItem()],radius,width,height,image);
				copy_lightmap(lightmap.indirect_image,image);
			}
			delete image;
			
			// update progress dialog
			int progress = i * 100 / lightmaps.size();
			if(dialogProgressUpdate(progress) == 0) {
				dialogProgressEnd();
				lightmaps.delete();
				object_surfaces.delete();
				lights.delete();
				return;
			}
		}
		log.message("Creating indirect lightmaps done %f seconds\n",clock() - begin);
		
		lights.delete();
		
		// end progress dialog
		dialogProgressEnd();
	}
	
	// update lightmaps
	update_lightmap_clicked();
	
	// enable lightmaps
	enable_lightmap_clicked();
}

/*
 */
void update_lightmap_clicked() {
	
	if(lightmaps.size() == 0) {
		dialogMessageOk(TR("Error"),TR("Lightmaps is not created"));
		return;
	}
	
	// begin progress dialog
	dialogProgressBegin(TR("Updating lightmaps..."));
	
	// update lightmaps
	float begin = clock();
	forloop(int i = 0; lightmaps.size()) {
		LightMap lightmap = lightmaps.get(i);
		
		Image image = new Image();
		
		// lightmap scales
		vec4 direct_scale = vec4(vec3(direct),1.0f);
		vec4 indirect_scale = vec4(vec3(indirect),0.0f);
		
		// create lightmap
		Image direct_image = lightmap.direct_image;
		Image indirect_image = lightmap.indirect_image;
		image.create2D(direct_image.getWidth(),direct_image.getHeight(),IMAGE_FORMAT_RGBA8);
		forloop(int y = 0; image.getHeight()) {
			forloop(int x = 0; image.getWidth()) {
				vec4 direct_color = direct_image.get2D(x,y);
				vec4 indirect_color = indirect_image.get2D(x,y);
				image.set2D(x,y,direct_color * direct_scale + indirect_color * indirect_scale);
			}
		}
		
		// update lightmap
		int width = lightmap.image.getWidth();
		int height = lightmap.image.getHeight();
		image.extrude(extrude_cb.getCurrentItem());
		if(blur_cb.getCurrentItem()) image.blur(blur_cb.getCurrentItem());
		if(image.getWidth() != width || image.getHeight() != height) image.resize(width,height);
		
		// blend lightmap
		lightmap.image.blend(image,0,0,0,0,width,height);
		
		// create mipmaps
		lightmap.image.createMipmaps(IMAGE_FILTER_LINEAR,2.2f);
		
		// save lightmap
		lightmap.image.save(lightmap.name);
		
		// update lightmap image
		foreachkey(Material material; lightmap.materials) {
			int lightmap_id = material.findTexture("lightmap");
			if(lightmap_id != -1) material.setImageTextureImage(lightmap_id,lightmap.image);
		}
		
		delete image;
		
		// update progress dialog
		int progress = i * 100 / lightmaps.size();
		if(dialogProgressUpdate(progress) == 0) {
			dialogProgressEnd();
			return;
		}
	}
	log.message("Updating lightmaps done %f seconds\n",clock() - begin);
	
	// end progress dialog
	dialogProgressEnd();
}

/*
 */
void enable_lightmap_clicked() {
	
	int clear = 0;
	if(object_surfaces.size() == 0) {
		if(dialogMessageYesNo(TR("Confirm"),TR("Enable lightmap state on selected objects?")) == 0) return;
		Object objects[0];
		get_objects(objects);
		get_object_surfaces(objects);
		if(lightmaps.size() == 0) {
			dialogMessageOk(TR("Error"),TR("There are no defined lightmaps"));
			return;
		}
		clear = 1;
	}
	
	// enable lightmap state
	foreach(ObjectSurface surface; object_surfaces) {
		assert(engine.world.isNode(surface.object) && "Tools::Illumination::enable_lightmap_clicked(): bad object");
		Material material = surface.material;
		int ambient_id = material.findState("ambient");
		assert(ambient_id != -1 && "Tools::Illumination::enable_lightmap_clicked(): can't find ambient state");
		forloop(int i = 0; material.getStateSwitchNumItems(ambient_id)) {
			string name = material.getStateSwitchItem(ambient_id,i);
			if(name == "lightmap" && material.getState(ambient_id) != i) {
				log.message("Enable lightmap state on \"%s\" material\n",material.getName());
				material.setState(ambient_id,i);
				forloop(int j = 0; material.getNumStates()) {
					string name = material.getStateName(j);
					if(strncmp(name,"light_",6)) continue;
					if(name == "light_prob") continue;
					material.setState(j,0);
				}
				break;
			}
		}
	}
	
	if(clear) {
		lightmaps.delete();
		object_surfaces.delete();
	}
}

void disable_lightmap_clicked() {
	
	int clear = 0;
	if(object_surfaces.size() == 0) {
		if(dialogMessageYesNo(TR("Confirm"),TR("Disable lightmap state on selected objects?")) == 0) return;
		Object objects[0];
		get_objects(objects);
		get_object_surfaces(objects);
		if(lightmaps.size() == 0) {
			dialogMessageOk(TR("Error"),TR("There are no defined lightmaps"));
			return;
		}
		clear = 1;
	}
	
	// disable lightmap state
	foreach(ObjectSurface surface; object_surfaces) {
		assert(engine.world.isNode(surface.object) && "Tools::Illumination::disable_lightmap_clicked(): bad object");
		Material material = surface.material;
		int ambient_id = material.findState("ambient");
		assert(ambient_id != -1 && "Tools::Illumination::disable_lightmap_clicked(): can't find ambient state");
		forloop(int i = 0; material.getStateSwitchNumItems(ambient_id)) {
			string name = material.getStateSwitchItem(ambient_id,i);
			if(name == "opacity" && material.getState(ambient_id) != i) {
				log.message("Disable lightmap state on \"%s\" material\n",material.getName());
				material.setState(ambient_id,i);
				forloop(int j = 0; material.getNumStates()) {
					string name = material.getStateName(j);
					if(strncmp(name,"light_",6)) continue;
					if(name == "light_prob") continue;
					material.setState(j,1);
				}
				break;
			}
		}
	}
	
	if(clear) {
		lightmaps.delete();
		object_surfaces.delete();
	}
}
