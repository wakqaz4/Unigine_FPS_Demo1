/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_objects_meshes.h
 * Desc:    Unigine editor
 * Version: 1.28
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
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
int check_mesh_file(string name,string ext) {
	
	// check format
	ext = "." + ext;
	int ext_pos = strrchr(name,'.');
	if(ext_pos == -1 || strstr(name,ext) != ext_pos) return 0;
	
	// check file
	File file =  new File();
	if(file.open(name,"rb") == 0) {
		delete file;
		return 0;
	}
	file.close();
	delete file;
	return 1;
}

/*
 */
void copy_parameters(Object dest,Object src) {
	
	// node parameters
	dest.setParent(src.getParent());
	Node children[0] = ();
	forloop(int i = 0; src.getNumChilds()) {
		children.append(src.getChild(i));
	}
	foreach(Node n; children) {
		dest.addChild(n);
	}
	
	dest.setHandled(src.isHandled());
	dest.setCollider(src.isCollider());
	dest.setClutter(src.isClutter());
	dest.setSpatial(src.isSpatial());
	dest.setQuery(src.isQuery());
	dest.setLatest(src.isLatest());
	dest.setFolded(src.isFolded());
	
	dest.setName(src.getName());
	dest.setData(src.getData());
	
	if(src.getProperty() != NULL) dest.setProperty(src.getPropertyName());
	dest.setWorldTransform(src.getWorldTransform());
	
	// surface parameters
	forloop(int i = 0; dest.getNumSurfaces()) {
		int j = src.findSurface(dest.getSurfaceName(i));
		if(j == -1) {
			dest.setMaterial("mesh_base",i);
			dest.setProperty("surface_base",i);
			continue;
		}
		
		dest.setEnabled(src.isEnabled(j),i);
		dest.setFolded(src.isFolded(j),i);
		
		dest.setDecal(src.getDecal(j),i);
		dest.setCastShadow(src.getCastShadow(j),i);
		dest.setReceiveShadow(src.getReceiveShadow(j),i);
		dest.setCastWorldShadow(src.getCastWorldShadow(j),i);
		dest.setReceiveWorldShadow(src.getReceiveWorldShadow(j),i);
		dest.setIntersection(src.getIntersection(j),i);
		dest.setCollision(src.getCollision(j),i);
		
		dest.setLightMask(src.getLightMask(j),i);
		dest.setDecalMask(src.getDecalMask(j),i);
		dest.setViewportMask(src.getViewportMask(j),i);
		dest.setIntersectionMask(src.getIntersectionMask(j),i);
		dest.setCollisionMask(src.getCollisionMask(j),i);
		
		dest.setMinParent(src.getMinParent(j),i);
		dest.setMaxParent(src.getMaxParent(j),i);
		dest.setMinVisibleDistance(src.getMinVisibleDistance(j),i);
		dest.setMaxVisibleDistance(src.getMaxVisibleDistance(j),i);
		dest.setMinFadeDistance(src.getMinFadeDistance(j),i);
		dest.setMaxFadeDistance(src.getMaxFadeDistance(j),i);
		
		if(src.getMaterial(i) != NULL) dest.setMaterial(src.getMaterialName(j),i);
		if(src.getProperty(i) != NULL) dest.setProperty(src.getPropertyName(j),i);
	}
	
	// ObjectMeshClutter
	if(src.getType() == NODE_OBJECT_MESH_CLUTTER) {
		ObjectMeshClutter d = dest;
		ObjectMeshClutter s = src;
		d.setOrientation(s.getOrientation());
		d.setIntersection(s.getIntersection());
		d.setVisibleDistance(s.getVisibleDistance());
		d.setFadeDistance(s.getFadeDistance());
		d.setShadowRadius(s.getShadowRadius());
		d.setSpawnRate(s.getSpawnRate());
		d.setSeed(s.getSeed());
		d.setSizeX(s.getSizeX());
		d.setSizeY(s.getSizeY());
		d.setStep(s.getStep());
		d.setDensity(s.getDensity());
		d.setThreshold(s.getThreshold());
		d.setAngle(s.getAngle());
		d.setMinScale(s.getMinScaleMean(),s.getMinScaleSpread());
		d.setMaxScale(s.getMaxScaleMean(),s.getMaxScaleSpread());
		d.setOffset(s.getOffsetMean(),s.getOffsetSpread());
		d.setRotation(s.getRotationMean(),s.getRotationSpread());
		d.setMaskImageName(s.getMaskImageName());
		d.setMaskFlipX(s.getMaskFlipX());
		d.setMaskFlipY(s.getMaskFlipY());
		d.setMaskMinValue(s.getMaskMinValue());
		d.setMaskMaxValue(s.getMaskMaxValue());
		d.setMaskMeshName(s.getMaskMeshName());
		d.setMaskInverse(s.getMaskInverse());
	}
	
	// ObjectMeshCluster
	if(src.getType() == NODE_OBJECT_MESH_CLUSTER) {
		ObjectMeshCluster d = dest;
		ObjectMeshCluster s = src;
		d.setVisibleDistance(s.getVisibleDistance());
		d.setFadeDistance(s.getFadeDistance());
		d.setShadowRadius(s.getShadowRadius());
		Mat4 transforms[0];
		Mat4 transform = s.getWorldTransform();
		forloop(int i = 0; s.getNumMeshes()) {
			transforms.append(transform * s.getMeshTransform(i));
		}
		d.createMeshes(transforms);
		transforms.delete();
	}
	
	// physics
	Body body = src.getBody();
	if(body != NULL) body_clone(body,dest);
}

/*
 */
void save_selection(Node selection[],int surfaces[]) {
	
	foreach(Node n; nodes) {
		if(n == node) continue;
		selection.append(n);
	}
	
	get_node(surfaces);
}

void restore_selection(Object object,Node selection[],int surfaces[]) {
	
	update_nodes();
	foreach(Node n; selection) {
		select_node(n,1);
	}
	select_node(object,1);
	Nodes::reselect_surfaces(object,surfaces);
	engine.editor.needReload();
}

/*
 */
#define MESH_CALLBACKS(TYPE,MESH,EXTENSION) \
void mesh_pressed() { \
	if(mesh_el.getText() == MESH.getMeshName()) return; \
	if(check_mesh_file(mesh_el.getText(),EXTENSION) == 0) { \
		dialogMessageOk(TR("Error"),format(TR("Cannot load mesh file \"%s\""),mesh_el.getText())); \
		mesh_el.setText(MESH.getMeshName()); \
		return; \
	} \
	Node selection[0] = (); \
	int surfaces[0] = (); \
	save_selection(selection,surfaces); \
	NodesState_ s = new NodesState_(); \
	s.init_modify(nodes,node); \
	save_state(s); \
	TYPE new_mesh = class_remove(new TYPE(mesh_el.getText())); \
	engine.editor.addNode(new_mesh); \
	copy_parameters(new_mesh,MESH); \
	swap_node(MESH,new_mesh); \
	engine.editor.swapNodes(MESH,new_mesh); \
	engine.editor.removeNode(new_mesh); \
	remove_node(new_mesh); \
	restore_selection(MESH,selection,surfaces); \
	mesh_el.setFocus(); \
} \
void mesh_load_clicked() { \
	string name = mesh_el.getText(); \
	if(dialogFileMesh(TR("Select mesh to load"),name,DIALOG_FILE_OPEN)) { \
		mesh_el.setText(name); \
		mesh_pressed(); \
	} \
}

/******************************************************************************\
*
* MeshStatic
*
\******************************************************************************/

/*
 */
namespace MeshStatic {
	
	/*
	 */
	WidgetVBox main_vb;
	WidgetEditLine mesh_el;
	
	ObjectMeshStatic mesh;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("MeshStatic "));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
		Nodes::Object::shutdown();
	}
	
	/*
	 */
	void update() {
		
		Nodes::Object::update();
		
		mesh = node_cast(node);
		
		mesh_el.setText(mesh.getMeshName());
		
		Nodes::Object::Body::init();
		Nodes::Object::Body::update();
	}
	
	/*
	 */
	string get_info(Node node) {
		ObjectMeshStatic mesh = node_cast(node);
		return Nodes::get_info(node) + format(TR("\nFile: %s"),mesh.getMeshName());
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
	}
	
	/*
	 */
	MESH_CALLBACKS(ObjectMeshStatic,mesh,"mesh")
	
} /* namespace MeshStatic */

/******************************************************************************\
*
* MeshCluster
*
\******************************************************************************/

/*
 */
namespace MeshCluster {
	
	/*
	 */
	WidgetVBox main_vb;
	WidgetEditLine mesh_el;
	
	WidgetLabel info_l;
	
	WidgetEditLine visible_distance_el;
	WidgetEditLine fade_distance_el;
	WidgetEditLine shadow_radius_el;
	
	ObjectMeshCluster cluster;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("MeshCluster "));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
		Nodes::Object::shutdown();
	}
	
	/*
	 */
	void update() {
		
		Nodes::Object::update();
		
		cluster = node_cast(node);
		
		mesh_el.setText(cluster.getMeshName());
		
		visible_distance_el.setText(editorFormat(cluster.getVisibleDistance()));
		fade_distance_el.setText(editorFormat(cluster.getFadeDistance()));
		shadow_radius_el.setText(editorFormat(cluster.getShadowRadius()));
	}
	
	/*
	 */
	string get_info(Node node) {
		ObjectMeshCluster cluster = node_cast(node);
		return Nodes::get_info(node) + format(TR("\nFile: %s"),cluster.getMeshName());
	}
	
	/*
	 */
	void update_dynamic() {
		
		info_l.setText(format(TR("Meshes: %d"),cluster.getNumMeshes()));
		
		Nodes::Object::update_dynamic();
	}
	
	/*
	 */
	int check_mesh(ObjectMeshStatic mesh) {
		if(mesh.getMeshName() != cluster.getMeshName()) return 0;
		forloop(int j = 0; cluster.getNumSurfaces()) {
			if(j < mesh.getNumSurfaces()) {
				if(mesh.getMaterialName(j) != cluster.getMaterialName(j)) {
					return 0;
				}
			}
		}
		return 1;
	}
	
	/*
	 */
	void create_clicked() {
		
		Mat4 transforms[0];
		
		Node nodes[0];
		
		Mat4 transform = cluster.getWorldTransform();
		
		// cluster meshes
		forloop(int i = 0; cluster.getNumMeshes()) {
			transforms.append(transform * cluster.getMeshTransform(i));
		}
		
		// child meshes
		forloop(int i = 0; cluster.getNumChilds()) {
			Node node = cluster.getChild(i);
			if(node.isEnabled() == 0) continue;
			if(node.getNumChilds() != 0) continue;
			if(engine.editor.isNode(node) == 0) continue;
			if(node.getType() == NODE_OBJECT_MESH_STATIC) {
				ObjectMeshStatic mesh = node_cast(node);
				if(check_mesh(mesh) == 0) continue;
				engine.editor.releaseNode(mesh);
				transforms.append(mesh.getWorldTransform());
				nodes.append(node_append(mesh));
			}
			else if(node.getType() == NODE_REFERENCE) {
				NodeReference reference = node_cast(node);
				Node node = reference.getNode();
				if(node == NULL) continue;
				if(node.getType() == NODE_OBJECT_MESH_STATIC) {
					ObjectMeshStatic mesh = node_cast(node);
					if(check_mesh(mesh) == 0) continue;
					engine.editor.releaseNode(reference);
					transforms.append(mesh.getWorldTransform());
					nodes.append(node_append(reference));
				}
			}
		}
		Nodes::NodesState_ states[0] = ();
		states.append(Nodes::create_state_modify((cluster),cluster));
		
		// create meshes
		cluster.createMeshes(transforms);
		
		states.append(Nodes::create_state_remove(nodes,(cluster),cluster));
		Nodes::save_states(states);
		
		nodes.delete();
		
		nodesReload();
	}
	
	void collect_clicked() {
		
		Mat4 transforms[0];
		
		Node nodes[0];
		
		Mat4 transform = cluster.getWorldTransform();
		
		// cluster meshes
		forloop(int i = 0; cluster.getNumMeshes()) {
			transforms.append(transform * cluster.getMeshTransform(i));
		}
		
		// editor meshes
		forloop(int i = 0; engine.editor.getNumNodes()) {
			Node node = engine.editor.getNode(i);
			if(node.isEnabled() == 0) continue;
			if(node.getNumChilds() != 0) continue;
			if(engine.editor.isNode(node) == 0) continue;
			if(node.getType() == NODE_OBJECT_MESH_STATIC) {
				ObjectMeshStatic mesh = node_cast(node);
				if(check_mesh(mesh) == 0) continue;
				transforms.append(mesh.getWorldTransform());
				nodes.append(mesh);
			}
			else if(node.getType() == NODE_REFERENCE) {
				NodeReference reference = node_cast(node);
				Node node = reference.getNode();
				if(node == NULL) continue;
				if(node.getType() == NODE_OBJECT_MESH_STATIC) {
					ObjectMeshStatic mesh = node_cast(node);
					if(check_mesh(mesh) == 0) continue;
					transforms.append(mesh.getWorldTransform());
					nodes.append(mesh);
				}
			}
		}
		
		Nodes::NodesState_ states[0] = ();
		states.append(Nodes::create_state_modify((cluster),cluster));
		
		// compaction nodes
		forloop(int i = 0; nodes.size()) {
			engine.editor.releaseNode(nodes[i]);
		}
		
		// create meshes
		cluster.createMeshes(transforms);
		
		states.append(Nodes::create_state_modify((cluster),cluster));
		states.append(Nodes::create_state_remove(nodes,(cluster),cluster));
		
		Nodes::save_states(states);
		
		nodesReload();
	}
	
	void expand_clicked() {
		Nodes::NodesState_ states[0] = ();
		states.append(Nodes::create_state_modify((cluster),cluster));
		
		Mat4 transform = cluster.getWorldTransform();
		
		Node nodes[0];
		
		forloop(int i = 0; cluster.getNumMeshes()) {
			ObjectMeshStatic mesh = new ObjectMeshStatic(cluster.getMeshName());
			forloop(int j = 0; cluster.getNumSurfaces()) {
				if(j < mesh.getNumSurfaces()) {
					mesh.setMaterial(cluster.getMaterialName(j),j);
				}
			}
			mesh.setProperty("surface_base","*");
			mesh.setParent(cluster);
			mesh.setWorldTransform(transform * cluster.getMeshTransform(i));
			engine.editor.addNode(class_remove(mesh));
			
			nodes.append(mesh);
		}
		
		// clear meshes
		cluster.clearMeshes();
		
		states.append(Nodes::create_state_new(nodes,(cluster),cluster));
		Nodes::save_states(states);
		
		nodesReload();
	}
	
	void clear_clicked() {
		if(dialogMessageYesNo(TR("Confirm"),TR("Remove all cluster meshes?"))) {
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,cluster);
			cluster.clearMeshes();
			save_state(s);
			nodesReload();
		}
	}
	
	/*
	 */
	MESH_CALLBACKS(ObjectMeshCluster,cluster,"mesh")
	PRESSED_CALLBACK_FLOAT(visible_distance,cluster,VisibleDistance,cluster,modify)
	PRESSED_CALLBACK_FLOAT(fade_distance,cluster,FadeDistance,cluster,modify)
	PRESSED_CALLBACK_FLOAT(shadow_radius,cluster,ShadowRadius,cluster,modify)
	
} /* namespace MeshCluster */

/******************************************************************************\
*
* MeshClutter
*
\******************************************************************************/

/*
 */
namespace MeshClutter {
	
	/*
	 */
	WidgetVBox main_vb;
	WidgetEditLine mesh_el;
	
	WidgetCheckBox orientation_cb;
	WidgetCheckBox intersection_cb;
	WidgetCheckBox collision_cb;
	WidgetCheckBox mask_flip_x_cb;
	WidgetCheckBox mask_flip_y_cb;
	WidgetCheckBox mask_inverse_cb;
	WidgetEditLine mask_image_el;
	WidgetEditLine mask_mesh_el;
	WidgetIcon mask_edit_i;
	
	WidgetEditLine mask_min_value_el;
	WidgetEditLine mask_max_value_el;
	
	WidgetEditLine visible_distance_el;
	WidgetEditLine fade_distance_el;
	WidgetEditLine shadow_radius_el;
	
	WidgetEditLine spawn_rate_el;
	WidgetEditLine seed_el;
	
	WidgetEditLine size_x_el;
	WidgetEditLine size_y_el;
	WidgetEditLine step_el;
	WidgetEditLine density_el;
	WidgetSlider density_sl;
	WidgetEditLine threshold_el;
	WidgetSlider threshold_sl;
	WidgetEditLine angle_el;
	WidgetSlider angle_sl;
	
	WidgetEditLine min_scale_mean_el;
	WidgetEditLine min_scale_spread_el;
	WidgetEditLine max_scale_mean_el;
	WidgetEditLine max_scale_spread_el;
	
	WidgetEditLine offset_mean_el;
	WidgetEditLine offset_spread_el;
	WidgetEditLine rotation_mean_el[3];
	WidgetEditLine rotation_spread_el[3];
	
	ObjectMeshClutter clutter;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("MeshClutter "));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
		brushesDisable();
		
		Nodes::Object::shutdown();
	}
	
	/*
	 */
	void update() {
		
		Nodes::Object::update();
		
		clutter = node_cast(node);
		
		setCallbackEnabled(GUI_CLICKED,0,(
			orientation_cb,
			intersection_cb,
			collision_cb,
			mask_flip_x_cb,
			mask_flip_y_cb,
			mask_inverse_cb,
		));
		
		mesh_el.setText(clutter.getMeshName());
		
		orientation_cb.setChecked(clutter.getOrientation());
		intersection_cb.setChecked(clutter.getIntersection());
		collision_cb.setChecked(clutter.getCollision());
		mask_flip_x_cb.setChecked(clutter.getMaskFlipX());
		mask_flip_y_cb.setChecked(clutter.getMaskFlipY());
		mask_inverse_cb.setChecked(clutter.getMaskInverse());
		
		mask_image_el.setText(clutter.getMaskImageName());
		mask_mesh_el.setText(clutter.getMaskMeshName());
		
		mask_min_value_el.setText(string(clutter.getMaskMinValue()));
		mask_max_value_el.setText(string(clutter.getMaskMaxValue()));
		
		visible_distance_el.setText(editorFormat(clutter.getVisibleDistance()));
		fade_distance_el.setText(editorFormat(clutter.getFadeDistance()));
		shadow_radius_el.setText(editorFormat(clutter.getShadowRadius()));
		
		spawn_rate_el.setText(string(clutter.getSpawnRate()));
		seed_el.setText(string(clutter.getSeed()));
		
		size_x_el.setText(editorFormat(clutter.getSizeX()));
		size_y_el.setText(editorFormat(clutter.getSizeY()));
		step_el.setText(editorFormat(clutter.getStep()));
		density_el.setText(editorFormat(clutter.getDensity(),5));
		threshold_el.setText(editorFormat(clutter.getThreshold()));
		angle_el.setText(editorFormat(clutter.getAngle()));
		
		min_scale_mean_el.setText(editorFormat(clutter.getMinScaleMean()));
		min_scale_spread_el.setText(editorFormat(clutter.getMinScaleSpread()));
		max_scale_mean_el.setText(editorFormat(clutter.getMaxScaleMean()));
		max_scale_spread_el.setText(editorFormat(clutter.getMaxScaleSpread()));
		
		offset_mean_el.setText(editorFormat(clutter.getOffsetMean()));
		offset_spread_el.setText(editorFormat(clutter.getOffsetSpread()));
		setEditLineVec3(rotation_mean_el,clutter.getRotationMean());
		setEditLineVec3(rotation_spread_el,clutter.getRotationSpread());
		
		update_mask_icons();
		
		setCallbackEnabled(GUI_CLICKED,1,(
			orientation_cb,
			intersection_cb,
			collision_cb,
			mask_flip_x_cb,
			mask_flip_y_cb,
			mask_inverse_cb,
		));
	}
	
	/*
	 */
	string get_info(Node node) {
		ObjectMeshClutter clutter = node_cast(node);
		return Nodes::get_info(node) + format(TR("\nFile: %s"),clutter.getMeshName());
	}
	
	/*
	 */
	void update_mask_icons() {
		mask_edit_i.setEnabled(clutter.getMaskImageName() != "");
	}
	
	/*
	 */
	void update_dynamic() {
		Nodes::Object::update_dynamic();
	}
	
	/*
	 */
	void mask_image_update_pressed() {
		mask_image_pressed();
		update_mask_icons();
	}
	
	void mask_image_load_update_clicked() {
		mask_image_load_clicked();
		update_mask_icons();
	}
	
	void mask_image_create_clicked() {
		
		int size = 0;
		if(dialogImageSize(TR("Mask size"),size) == 0) return;
		
		string name;
		if(dialogFileImageSave(TR("Select texture to save"),name) == 0) return;
		
		Image image = new Image();
		image.create2D(size,size,IMAGE_FORMAT_R8);
		
		image.set(0,0);
		
		if(image.save(name) == 0) {
			dialogMessageOk(TR("Error"));
			delete image;
			return;
		}
		
		log.message("Save %s %s into the \"%s\" file\n",image.getTypeName(),image.getFormatName(),name);
		
		delete image;
		
		clutter.setMaskImageName(name);
		mask_image_el.setText(name);
		
		update_mask_icons();
	}
	
	void mask_image_edit_clicked() {
		maskEnable(clutter,Brushes::Mask::CHANNEL_R,clutter.getMaskMinValue() / 255.0f,clutter.getMaskMaxValue() / 255.0f);
	}
	
	void mask_image_clear_update_clicked() {
		mask_image_clear_clicked();
		update_mask_icons();
	}
	
	/*
	 */
	void randomize_clicked() {
		seed_el.setText(string(engine.game.getRandom(0,0x0fffffff)));
		seed_el.runCallback(GUI_PRESSED);
	}
	
	/*
	 */
	MESH_CALLBACKS(ObjectMeshClutter,clutter,"mesh")
	CLICKED_CALLBACK(orientation,clutter,Orientation,clutter)
	CLICKED_CALLBACK(intersection,clutter,Intersection,clutter)
	CLICKED_CALLBACK(collision,clutter,Collision,clutter)
	CLICKED_CALLBACK(mask_flip_x,clutter,MaskFlipX,clutter)
	CLICKED_CALLBACK(mask_flip_y,clutter,MaskFlipY,clutter)
	CLICKED_CALLBACK(mask_inverse,clutter,MaskInverse,clutter)
	PRESSED_CALLBACK(mask_image,clutter,MaskImageName,clutter,string,modify)
	PRESSED_CALLBACK(mask_mesh,clutter,MaskMeshName,clutter,string,modify)
	LOAD_CLICKED_CALLBACK(mask_image,clutter,MaskImageName,TR("Select mask image"))
	VIEW_CLICKED_CALLBACK(mask_image,clutter,MaskImageName,TR("Clutter mask image"))
	CLEAR_CLICKED_CALLBACK(mask_image,clutter,MaskImageName,TR("Clear mask image?"),NULL,"")
	LOAD_CLICKED_CALLBACK_MESH(mask_mesh,clutter,MaskMeshName,TR("Select mask mesh"))
	CLEAR_CLICKED_CALLBACK(mask_mesh,clutter,MaskMeshName,TR("Clear mask mesh?"),NULL,"")
	PRESSED_CALLBACK(mask_min_value,clutter,MaskMinValue,clutter,int,modify)
	PRESSED_CALLBACK(mask_max_value,clutter,MaskMaxValue,clutter,int,modify)
	PRESSED_CALLBACK_FLOAT(visible_distance,clutter,VisibleDistance,clutter,modify)
	PRESSED_CALLBACK_FLOAT(fade_distance,clutter,FadeDistance,clutter,modify)
	PRESSED_CALLBACK_FLOAT(shadow_radius,clutter,ShadowRadius,clutter,modify)
	PRESSED_CALLBACK(spawn_rate,clutter,SpawnRate,clutter,int,modify)
	PRESSED_CALLBACK(seed,clutter,Seed,clutter,int,modify)
	PRESSED_CALLBACK_FLOAT(size_x,clutter,SizeX,clutter,modify)
	PRESSED_CALLBACK_FLOAT(size_y,clutter,SizeY,clutter,modify)
	PRESSED_CALLBACK_FLOAT(step,clutter,Step,clutter,modify)
	PRESSED_CALLBACK_SLIDER_PREC(density,clutter,Density,5,clutter,modify)
	PRESSED_CALLBACK_SLIDER(threshold,clutter,Threshold,clutter,modify)
	PRESSED_CALLBACK_SLIDER(angle,clutter,Angle,clutter,modify)
	PRESSED_CALLBACK_MEAN_SPREAD(min_scale,clutter,MinScale)
	PRESSED_CALLBACK_MEAN_SPREAD(max_scale,clutter,MaxScale)
	PRESSED_CALLBACK_MEAN_SPREAD(offset,clutter,Offset)
	PRESSED_CALLBACK_MEAN_SPREAD_VEC3(rotation,clutter,Rotation)
	
} /* namespace MeshClutter */

/******************************************************************************\
*
* MeshSkinned
*
\******************************************************************************/

/*
 */
namespace MeshSkinned {
	
	/*
	 */
	WidgetVBox main_vb;
	WidgetEditLine mesh_el;
	WidgetEditLine anim_el;
	WidgetCheckBox loop_cb;
	WidgetCheckBox controlled_cb;
	WidgetEditLine time_el;
	WidgetEditLine speed_el;
	WidgetButton play_b;
	WidgetButton stop_b;
	
	ObjectMeshSkinned skinned;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("MeshSkinned "));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
		Nodes::Object::shutdown();
	}
	
	/*
	 */
	void update() {
		
		Nodes::Object::update();
		
		skinned = node_cast(node);
		
		mesh_el.setText(skinned.getMeshName());
		anim_el.setText(skinned.getAnimName());
		loop_cb.setChecked(skinned.getLoop());
		controlled_cb.setChecked(skinned.isControlled());
		time_el.setText(editorFormat(skinned.getTime()));
		speed_el.setText(editorFormat(skinned.getSpeed()));
		
		Nodes::Object::Body::init();
		Nodes::Object::Body::update();
	}
	
	/*
	 */
	string get_info(Node node) {
		ObjectMeshSkinned skinned = node_cast(node);
		return Nodes::get_info(node) + format(TR("\nFile: %s"),skinned.getMeshName());
	}
	
	/*
	 */
	void render_visualizer(ObjectMeshSkinned node) {
		if(helpersGetMeshSkinnedBones()) node.renderVisualizer();
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
		
		if(time_el.isFocused() == 0) time_el.setText(editorFormat(skinned.getTime()));
		
		play_b.setEnabled(skinned.isStopped());
		stop_b.setEnabled(skinned.isPlaying());
	}
	
	/*
	 */
	MESH_CALLBACKS(ObjectMeshSkinned,skinned,"mesh")
	CLICKED_CALLBACK(loop,skinned,Loop,skinned)
	CLICKED_CALLBACK(controlled,skinned,Controlled,skinned)
	PRESSED_CALLBACK_FLOAT(time,skinned,Time,skinned,modify)
	PRESSED_CALLBACK_FLOAT(speed,skinned,Speed,skinned,modify)
	
	/*
	 */
	void anim_pressed() {
		if(anim_el.getText() == "") skinned.setAnimation(0,-1);
		else skinned.setAnimation(0,anim_el.getText());
		skinned.setAnimName(anim_el.getText());
	}
	
	void anim_load_clicked() {
		string name = skinned.getAnimName();
		if(dialogFileMesh(TR("Select animation to load"),name,DIALOG_FILE_OPEN)) {
			skinned.setAnimation(0,name);
			skinned.setAnimName(name);
			anim_el.setText(skinned.getAnimName());
		}
	}
	
	/*
	 */
	void play_clicked() {
		
		Node nodes[0];
		nodesGetNodes(nodes);
		
		int type = skinned.getType();
		foreach(Node n; nodes) {
			if(n.getType() != type) continue;
			ObjectMeshSkinned current_skinned = node_cast(n);
			
			current_skinned.play();
		}
	}
	
	void stop_clicked() {
		Node nodes[0];
		nodesGetNodes(nodes);
		
		int type = skinned.getType();
		foreach(Node n; nodes) {
			if(n.getType() != type) continue;
			ObjectMeshSkinned current_skinned = node_cast(n);
			
			current_skinned.stop();
		}
	}
}

/******************************************************************************\
*
* MeshDynamic
*
\******************************************************************************/

/*
 */
namespace MeshDynamic {
	
	/*
	 */
	WidgetVBox main_vb;
	
	ObjectMeshDynamic dynamic;
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("MeshDynamic "));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
	}
	
	void shutdown() {
		
		Nodes::Object::shutdown();
	}
	
	/*
	 */
	void update() {
		
		Nodes::Object::update();
		
		dynamic = node_cast(node);
		
		Nodes::Object::Body::init();
		Nodes::Object::Body::update();
	}

	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
	}
	
	/*
	 */
	void load_clicked() {
		string name;
		string materials[];
		string properties[];
		if(dialogFileMesh(TR("Select mesh to load"),name,DIALOG_FILE_OPEN)) {
			forloop(int i = 0; dynamic.getNumSurfaces()) {
				materials.append(dynamic.getSurfaceName(i),dynamic.getMaterialName(i));
				properties.append(dynamic.getSurfaceName(i),dynamic.getPropertyName(i));
			}
			Body body = dynamic.getBody();
			if(body != NULL) {
				forloop(int i = 0; body.getNumJoints()) {
					Joint joint = body.getJoint(i);
					if(joint.getType() == JOINT_PARTICLES) {
						JointParticles particles = joint_cast(joint);
						particles.clearParticles();
					}
				}
			}
			if(dynamic.loadMesh(name)) {
				foreachkey(string name; materials) {
					int surface = dynamic.findSurface(name);
					if(surface != -1) {
						if(strlen(materials[name])) dynamic.setMaterial(materials[name],surface);
						if(strlen(properties[name])) dynamic.setProperty(properties[name],surface);
					}
				}
				if(body != NULL) {
					dynamic.setBody(NULL);
					dynamic.setBody(body);
				}
			} else {
				dialogMessageOk(TR("Error"));
			}
			if(body != NULL) {
				forloop(int i = 0; body.getNumJoints()) {
					Joint joint = body.getJoint(i);
					if(joint.getType() == JOINT_PARTICLES) {
						JointParticles particles = joint_cast(joint);
						particles.setSize(particles.getSize());
					}
				}
			}
		}
	}
	
	void save_clicked() {
		string name;
		if(dialogFileMesh(TR("Select mesh to save"),name,DIALOG_FILE_SAVE)) {
			name = savePath(name);
			if(dynamic.saveMesh(name) == 0) dialogMessageOk(TR("Error"));
		}
	}
	
} /* namespace MeshDynamic */
