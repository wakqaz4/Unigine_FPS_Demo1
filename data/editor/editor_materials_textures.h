/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_materials_textures.h
 * Desc:    Unigine editor
 * Version: 1.10
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
 *          Maxim Belobryukhov <maxi@unigine.com>
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
MTexture textures[0];

/*
 */
enum {
	TEXTURE_GROUP_DEFAULT = 0,
	TEXTURE_GROUP_BASE,
	TEXTURE_GROUP_DETAIL,
	TEXTURE_GROUP_OVERLAP,
	TEXTURE_GROUP_LIGHTMAP,
	TEXTURE_GROUP_ADDITIONAL,
	TEXTURE_GROUP_REFLECTION,
	TEXTURE_GROUP_TESSELLATION,
	TEXTURE_GROUP_CLOUDS,
};

string texture_titles[] = (
	TEXTURE_GROUP_BASE				: TR(" Base"),
	TEXTURE_GROUP_DETAIL			: TR(" Detail"),
	TEXTURE_GROUP_OVERLAP			: TR("Overlap"),
	TEXTURE_GROUP_LIGHTMAP			: TR("  Lightmap"),
	TEXTURE_GROUP_ADDITIONAL		: TR("Additional"),
	TEXTURE_GROUP_REFLECTION		: TR("Reflection"),
	TEXTURE_GROUP_TESSELLATION		: TR(" Tessellation"),
	TEXTURE_GROUP_CLOUDS			: TR("  Clouds"),
);

Unigine::Triple texture_names[] = (
	
	"diffuse"									: new Unigine::Triple(TR("Diffuse"),			TEXTURE_GROUP_BASE,				TR("Diffuse texture, alpha channel is detail texturing modulation")				),
	"normal"									: new Unigine::Triple(TR(" Normal"),			TEXTURE_GROUP_BASE,				TR("Normal texture, two component")												),
	"specular"									: new Unigine::Triple(TR(" Specular"),			TEXTURE_GROUP_BASE,				TR("Specular texture, alpha channel is glossiness modulation")					),
	
	"shading"									: new Unigine::Triple(TR("Shading"),			TEXTURE_GROUP_BASE,				TR("Shading LUT texture, X axis is diffuse, Y axis is specular")				),
	
	"parallax"									: new Unigine::Triple(TR(" Parallax"),			TEXTURE_GROUP_ADDITIONAL,		TR("Single channel parallax height map texture")								),
	"ambient"									: new Unigine::Triple(TR("  Ambient"),			TEXTURE_GROUP_ADDITIONAL,		TR("Single channel environment modulation texture")								),
	"emission"									: new Unigine::Triple(TR(" Emission"),			TEXTURE_GROUP_ADDITIONAL,		TR("Ambient emission, alpha channel is glow post process modulation")			),
	
	"height"									: new Unigine::Triple(TR(" Height"),			TEXTURE_GROUP_TESSELLATION,		TR("Single channel tessellation height map texture")							),
	"tessellation"								: new Unigine::Triple(TR("  Tessellation"),		TEXTURE_GROUP_TESSELLATION,		TR("Single channel tessellation per vertex subdivision map texture")			),
	
	"detail_diffuse"							: new Unigine::Triple(TR("Diffuse"),			TEXTURE_GROUP_DETAIL,			TR("Detail diffuse texture, alpha channel is detail texturing modulation")		),
	"detail_normal"								: new Unigine::Triple(TR(" Normal"),			TEXTURE_GROUP_DETAIL,			TR("Detail normal texture, two component")										),
	"detail_specular"							: new Unigine::Triple(TR(" Specular"),			TEXTURE_GROUP_DETAIL,			TR("Detail specular texture")													),
	
	"mesh_triplanar_base_diffuse_0"				: new Unigine::Triple(TR("Diffuse X"),			TEXTURE_GROUP_BASE,				TR("X diffuse texture")															),
	"mesh_triplanar_base_diffuse_1"				: new Unigine::Triple(TR("Diffuse Y"),			TEXTURE_GROUP_BASE,				TR("Y diffuse texture")															),
	"mesh_triplanar_base_diffuse_2"				: new Unigine::Triple(TR("Diffuse Z"),			TEXTURE_GROUP_BASE,				TR("Z diffuse texture")															),
	"mesh_triplanar_base_normal_0"				: new Unigine::Triple(TR("Normal X"),			TEXTURE_GROUP_BASE,				TR("X normal texture")															),
	"mesh_triplanar_base_normal_1"				: new Unigine::Triple(TR("Normal Y"),			TEXTURE_GROUP_BASE,				TR("Y normal texture")															),
	"mesh_triplanar_base_normal_2"				: new Unigine::Triple(TR("Normal Z"),			TEXTURE_GROUP_BASE,				TR("Z normal texture")															),
	"mesh_triplanar_base_specular_0"			: new Unigine::Triple(TR("Specular X"),			TEXTURE_GROUP_BASE,				TR("X specular texture")														),
	"mesh_triplanar_base_specular_1"			: new Unigine::Triple(TR("Specular Y"),			TEXTURE_GROUP_BASE,				TR("Y specular texture")														),
	"mesh_triplanar_base_specular_2"			: new Unigine::Triple(TR("Specular Z"),			TEXTURE_GROUP_BASE,				TR("Z specular texture")														),
	
	"mesh_overlap_base_mask"					: new Unigine::Triple(TR("Mask"),				TEXTURE_GROUP_BASE,				TR("Noise mask texture")														),
	"overlap_diffuse"							: new Unigine::Triple(TR("Diffuse"),			TEXTURE_GROUP_OVERLAP,			TR("Overlap diffuse texture")													),
	"overlap_normal"							: new Unigine::Triple(TR("Normal"),				TEXTURE_GROUP_OVERLAP,			TR("Overlap normal texture")													),
	"overlap_specular"							: new Unigine::Triple(TR("Specular"),			TEXTURE_GROUP_OVERLAP,			TR("Overlap specular texture")													),
	
	"mesh_layer_base_mask"						: new Unigine::Triple(TR("Mask"),				TEXTURE_GROUP_BASE,				TR("Noise mask texture")														),
	"mesh_layer_base_diffuse_0"					: new Unigine::Triple(TR("Diffuse 0"),			TEXTURE_GROUP_BASE,				TR("Noise first diffuse texture")												),
	"mesh_layer_base_diffuse_1"					: new Unigine::Triple(TR("Diffuse 1"),			TEXTURE_GROUP_BASE,				TR("Noise second diffuse texture")												),
	"mesh_layer_base_normal_0"					: new Unigine::Triple(TR("Normal 0"),			TEXTURE_GROUP_BASE,				TR("Noise first normal texture")												),
	"mesh_layer_base_normal_1"					: new Unigine::Triple(TR("Normal 1"),			TEXTURE_GROUP_BASE,				TR("Noise second normal texture")												),
	"mesh_layer_base_specular_0"				: new Unigine::Triple(TR("Specular 0"),			TEXTURE_GROUP_BASE,				TR("Noise first specular texture")												),
	"mesh_layer_base_specular_1"				: new Unigine::Triple(TR("Specular 1"),			TEXTURE_GROUP_BASE,				TR("Noise second specular texture")												),
	
	"mesh_noise_base_mask"						: new Unigine::Triple(TR("Mask"),				TEXTURE_GROUP_BASE,				TR("Noise mask texture")														),
	"mesh_noise_base_diffuse_0"					: new Unigine::Triple(TR("Diffuse 0"),			TEXTURE_GROUP_BASE,				TR("Noise first diffuse texture")												),
	"mesh_noise_base_diffuse_1"					: new Unigine::Triple(TR("Diffuse 1"),			TEXTURE_GROUP_BASE,				TR("Noise second diffuse texture")												),
	"mesh_noise_base_normal_0"					: new Unigine::Triple(TR("Normal 0"),			TEXTURE_GROUP_BASE,				TR("Noise first normal texture")												),
	"mesh_noise_base_normal_1"					: new Unigine::Triple(TR("Normal 1"),			TEXTURE_GROUP_BASE,				TR("Noise second normal texture")												),
	"mesh_noise_base_specular_0"				: new Unigine::Triple(TR("Specular 0"),			TEXTURE_GROUP_BASE,				TR("Noise first specular texture")												),
	"mesh_noise_base_specular_1"				: new Unigine::Triple(TR("Specular 1"),			TEXTURE_GROUP_BASE,				TR("Noise second specular texture")												),
	
	"mesh_terrain_base_mask"					: new Unigine::Triple(TR("Mask"),				TEXTURE_GROUP_BASE,				TR("Terrain mask texture")														),
	"mesh_terrain_base_detail_0_diffuse"		: new Unigine::Triple(TR(" Diffuse 0"),			TEXTURE_GROUP_DETAIL,			TR("Terrain first detail diffuse texture")										),
	"mesh_terrain_base_detail_0_normal"			: new Unigine::Triple(TR("Normal 0"),			TEXTURE_GROUP_DETAIL,			TR("Terrain first detail normal texture")										),
	"mesh_terrain_base_detail_1_diffuse"		: new Unigine::Triple(TR(" Diffuse 1"),			TEXTURE_GROUP_DETAIL,			TR("Terrain second detail diffuse texture")										),
	"mesh_terrain_base_detail_1_normal"			: new Unigine::Triple(TR("Normal 1"),			TEXTURE_GROUP_DETAIL,			TR("Terrain second detail normal texture")										),
	"mesh_terrain_base_detail_2_diffuse"		: new Unigine::Triple(TR(" Diffuse 2"),			TEXTURE_GROUP_DETAIL,			TR("Terrain third detail diffuse texture")										),
	"mesh_terrain_base_detail_2_normal"			: new Unigine::Triple(TR("Normal 2"),			TEXTURE_GROUP_DETAIL,			TR("Terrain third detail normal texture")										),
	"mesh_terrain_base_detail_3_diffuse"		: new Unigine::Triple(TR(" Diffuse 3"),			TEXTURE_GROUP_DETAIL,			TR("Terrain fourth detail diffuse texture")										),
	"mesh_terrain_base_detail_3_normal"			: new Unigine::Triple(TR("Normal 3"),			TEXTURE_GROUP_DETAIL,			TR("Terrain fourth detail normal texture")										),
	
	"decal_terrain_base_mask"					: new Unigine::Triple(TR("Mask"),				TEXTURE_GROUP_BASE,				TR("Terrain mask texture")														),
	"decal_terrain_base_detail_0_diffuse"		: new Unigine::Triple(TR("Diffuse 0"),			TEXTURE_GROUP_DETAIL,			TR("Terrain first detail diffuse texture")										),
	"decal_terrain_base_detail_0_normal"		: new Unigine::Triple(TR("Normal 0"),			TEXTURE_GROUP_DETAIL,			TR("Terrain first detail normal texture")										),
	"decal_terrain_base_detail_1_diffuse"		: new Unigine::Triple(TR("Diffuse 1"),			TEXTURE_GROUP_DETAIL,			TR("Terrain second detail diffuse texture")										),
	"decal_terrain_base_detail_1_normal"		: new Unigine::Triple(TR("Normal 1"),			TEXTURE_GROUP_DETAIL,			TR("Terrain second detail normal texture")										),
	"decal_terrain_base_detail_2_diffuse"		: new Unigine::Triple(TR("Diffuse 2"),			TEXTURE_GROUP_DETAIL,			TR("Terrain third detail diffuse texture")										),
	"decal_terrain_base_detail_2_normal"		: new Unigine::Triple(TR("Normal 2"),			TEXTURE_GROUP_DETAIL,			TR("Terrain third detail normal texture")										),
	"decal_terrain_base_detail_3_diffuse"		: new Unigine::Triple(TR("Diffuse 3"),			TEXTURE_GROUP_DETAIL,			TR("Terrain fourth detail diffuse texture")										),
	"decal_terrain_base_detail_3_normal"		: new Unigine::Triple(TR("Normal 3"),			TEXTURE_GROUP_DETAIL,			TR("Terrain fourth detail normal texture")										),
	
	"lightmap"									: new Unigine::Triple(TR(" Lightmap"),			TEXTURE_GROUP_LIGHTMAP,			TR("Lightmap texture, uses second texture channel")								),
	
	"reflection"								: new Unigine::Triple(TR("Static"),				TEXTURE_GROUP_REFLECTION,		TR("Static reflection texture")													),
	
	"mesh_leaf_base_thickness"					: new Unigine::Triple(TR("Thickness"),			TEXTURE_GROUP_BASE,				TR("Single channel thickness texture of leafs")									),
	"mesh_leaf_base_noise_2d"					: new Unigine::Triple(TR(" 2D noise"),			TEXTURE_GROUP_ADDITIONAL,		TR("Spatial color noise texture")												),
	"mesh_leaf_base_noise_3d"					: new Unigine::Triple(TR(" 3D noise"),			TEXTURE_GROUP_ADDITIONAL,		TR("Single channel spatial color noise texture")								),
	"mesh_leaf_base_color"						: new Unigine::Triple(TR("Color gradient"),		TEXTURE_GROUP_ADDITIONAL,		TR("Gradient texture for color modulation")										),
	
	"mesh_paint_base_noise"						: new Unigine::Triple(TR(" Fleck noise"),		TEXTURE_GROUP_ADDITIONAL,		TR("Single channel spatial fleck noise texture")								),
	"mesh_tessellation_paint_base_noise"		: new Unigine::Triple(TR(" Fleck noise"),		TEXTURE_GROUP_ADDITIONAL,		TR("Single channel spatial fleck noise texture")								),
	
	"grass_base_lightmap"						: new Unigine::Triple(TR("Lightmap"),			TEXTURE_GROUP_LIGHTMAP,			TR("Lightmap texture, uses planar projection across whole grass")				),
	"grass_base_noise"							: new Unigine::Triple(TR("Spatial noise"),		TEXTURE_GROUP_ADDITIONAL,		TR("Spatial color noise texture, uses planar projection across whole grass")	),
	
	"grass_impostor_base_noise"					: new Unigine::Triple(TR("Spatial noise"),		TEXTURE_GROUP_ADDITIONAL,		TR("Spatial color noise texture, uses planar projection across whole grass")	),
	
	"particles_base_normal"						: new Unigine::Triple(TR("Refraction"),			TEXTURE_GROUP_ADDITIONAL,		TR("Refraction normal texture")													),
	
	"attenuation"								: new Unigine::Triple(TR("  Attenuation"),		TEXTURE_GROUP_BASE,				TR("Gradient texture for color modulation")										),
	
	"volume_cloud_base_density"					: new Unigine::Triple(TR(" Density"),			TEXTURE_GROUP_BASE,				TR("Volumetric clouds density texture")											),
	
	"volume_fog_base_noise"						: new Unigine::Triple(TR("Noise"),				TEXTURE_GROUP_BASE,				TR("Single channel screen space noise texture")									),
	
	"water_base_normal_01"						: new Unigine::Triple(TR("Normal 01"),			TEXTURE_GROUP_BASE,				TR("First group of water normal texture")										),
	"water_base_normal_23"						: new Unigine::Triple(TR("Normal 23"),			TEXTURE_GROUP_BASE,				TR("Second group of water normal texture")										),
	"water_base_foam"							: new Unigine::Triple(TR(" Foam"),				TEXTURE_GROUP_ADDITIONAL,		TR("Water foam texture")														),
	"water_base_caps"							: new Unigine::Triple(TR(" Caps"),				TEXTURE_GROUP_ADDITIONAL,		TR("Water caps texture")														),
	
	"sky_base_mask_0"							: new Unigine::Triple(TR("Mask 0"),				TEXTURE_GROUP_CLOUDS,			TR("First group of clouds mask texture")										),
	"sky_base_mask_1"							: new Unigine::Triple(TR("Mask 1"),				TEXTURE_GROUP_CLOUDS,			TR("Second group of clouds mask texture")										),
	"sky_base_clouds_01"						: new Unigine::Triple(TR(" Clouds 01"),			TEXTURE_GROUP_CLOUDS,			TR("First group of clouds noise texture")										),
	"sky_base_clouds_23"						: new Unigine::Triple(TR(" Clouds 23"),			TEXTURE_GROUP_CLOUDS,			TR("Second group of clouds noise texture")										),
	"sky_base_background_0"						: new Unigine::Triple(TR("Background 0"),		TEXTURE_GROUP_ADDITIONAL,		TR("Sky cubemap background texture")											),
	"sky_base_background_1"						: new Unigine::Triple(TR("Background 1"),		TEXTURE_GROUP_ADDITIONAL,		TR("Sky cubemap background texture")											),
	
	"microfiber"								: new Unigine::Triple(TR(" Microfiber"),		TEXTURE_GROUP_BASE,				TR("Microfiber")),
	"parallax_"									: new Unigine::Triple(TR("Parallax"),			TEXTURE_GROUP_BASE,				TR("Parallax texture")),
	"ambient_occlusion"							: new Unigine::Triple(TR("AO"),					TEXTURE_GROUP_BASE,				TR("Ambient occlusion texture")),
	"emission_"									: new Unigine::Triple(TR("Emission"),			TEXTURE_GROUP_BASE,				TR("Emission texture")),
	
	"albedo"									: new Unigine::Triple(TR("Albedo"),				TEXTURE_GROUP_BASE,				TR("Albedo texture")),
	"metalness"									: new Unigine::Triple(TR("Metalness"),			TEXTURE_GROUP_BASE,				TR("Metalness texture, alpha channel is roughness modulation")),
	
	"detail_albedo"								: new Unigine::Triple(TR("Albedo"),				TEXTURE_GROUP_DETAIL,			TR("Detail Albedo texture, alpha channel is detail texturing modulation")),
	"detail_metalness"							: new Unigine::Triple(TR("Metalness"),			TEXTURE_GROUP_DETAIL,			TR("Detail Metalness texture")),
	
);

/*
 */
string get_texture_name(string name) {
	Material m = material;
	while(m != NULL) {
		if(texture_names.check(m.getName() + "_" + name)) return TR(texture_names[m.getName() + "_" + name].first);
		m = m.getParent();
	}
	if(texture_names.check(name)) return TR(texture_names[name].first);
	return name;
}

int get_texture_group(string name) {
	Material m = material;
	while(m != NULL) {
		if(texture_names.check(m.getName() + "_" + name)) return texture_names[m.getName() + "_" + name].second;
		m = m.getParent();
	}
	if(texture_names.check(name)) return texture_names[name].second;
	return TEXTURE_GROUP_DEFAULT;
}

string get_texture_tooltip(string name) {
	Material m = material;
	while(m != NULL) {
		if(texture_names.check(m.getName() + "_" + name)) return TR(texture_names[m.getName() + "_" + name].third);
		m = m.getParent();
	}
	if(texture_names.check(name)) return TR(texture_names[name].third);
	return "";
}

string get_texture_title(int group) {
	if(texture_titles.check(group)) return TR(texture_titles[group]);
	return "";
}

/*
 */
class MTexture {
	
	int num;					// texture number
	string name;				// texture name
	int type;					// texture type
	int hidden;					// texture hidden
	int group;					// texture group
	int override;				// override flag
	
	WidgetGroupBox group_gb;	// texture group
	WidgetGridBox grid_gb;		// texture grid
	
	WidgetLabel name_l;			// name label
	WidgetEditLine name_el;		// name editline
	
	WidgetHBox icon_hb;			// icons hbox
	WidgetIcon load_i;			// image load
	WidgetIcon view_i;			// image view
	WidgetIcon edit_i;			// image edit
	WidgetIcon clear_i;			// image clear
	
	// update
	void update() {
		override = (parent != NULL && parent.findTexture(name) != -1 && material.getImageTextureName(num) != parent.getImageTextureName(parent.findTexture(name)));
		name_l.setFontColor(override ? color_green : vec4_zero);
		edit_i.setEnabled(true);
		clear_i.setEnabled(override);
	}
	
	// constructor
	MTexture(int n) {
		
		num = n;
		name = material.getTextureName(num);
		type = material.getTextureType(num);
		hidden = material.isTextureHidden(num);
		group = get_texture_group(name);
		
		if(name == "height") hidden = 0;
		if(name == "tessellation") hidden = 0;
		
		if(hidden) return;
		
		string tooltip = get_texture_tooltip(name);
		
		// create group
		if(type == MATERIAL_TEXTURE_IMAGE) {
			MTexture parent = NULL;
			for(int i = num - 1; i >= 0; i--) {
				if(textures[i].hidden) continue;
				if(textures[i].group_gb == NULL) continue;
				parent = textures[i];
				break;
			}
			if(parent == NULL || parent.group != group) {
				group_gb = new WidgetGroupBox(gui,get_texture_title(group),2,2);
				textures_vb.addChild(group_gb,GUI_ALIGN_EXPAND);
				grid_gb = new WidgetGridBox(gui,3,2,2);
				group_gb.addChild(grid_gb,GUI_ALIGN_EXPAND);
			} else {
				group_gb = parent.group_gb;
				grid_gb = parent.grid_gb;
			}
		}
		
		// image
		if(type == MATERIAL_TEXTURE_IMAGE) {
			
			name_l = new WidgetLabel(gui,get_texture_name(name) + ":");
			name_l.setToolTip(tooltip);
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			
			name_el = new WidgetEditLine(gui,material.getImageTextureName(num));
			name_el.setToolTip(tooltip);
			grid_gb.addChild(name_el,GUI_ALIGN_EXPAND);
			
			icon_hb = new WidgetHBox(gui);
			grid_gb.addChild(icon_hb);
			
			load_i = new WidgetIcon(gui,"editor/gui/action_load.png");
			load_i.setWidth(24);
			load_i.setHeight(24);
			load_i.setToolTip(TR("Load"));
			icon_hb.addChild(load_i);
			
			view_i = new WidgetIcon(gui,"editor/gui/action_view.png");
			view_i.setWidth(24);
			view_i.setHeight(24);
			view_i.setToolTip(TR("View"));
			icon_hb.addChild(view_i);
			
			edit_i = new WidgetIcon(gui,"editor/gui/action_edit.png");
			edit_i.setWidth(24);
			edit_i.setHeight(24);
			edit_i.setToolTip(TR("Edit"));
			icon_hb.addChild(edit_i);
			
			clear_i = new WidgetIcon(gui,"editor/gui/action_clear.png");
			clear_i.setWidth(24);
			clear_i.setHeight(24);
			clear_i.setToolTip(TR("Reset"));
			icon_hb.addChild(clear_i);
			
			name_el.setCallback(GUI_PRESSED,functionid(texture_name_pressed),num);
			name_el.setCallback(GUI_FOCUS_OUT,functionid(texture_name_pressed),num);
			load_i.setCallback(GUI_CLICKED,functionid(texture_load_clicked),num);
			view_i.setCallback(GUI_CLICKED,functionid(texture_view_clicked),num);
			edit_i.setCallback(GUI_CLICKED,functionid(texture_edit_clicked),num);
			clear_i.setCallback(GUI_CLICKED,functionid(texture_clear_clicked),num);
			
			update();
		}
	}
	
	~MTexture() {
		
		delete group_gb;
		delete grid_gb;
		
		delete name_l;
		delete name_el;
		delete icon_hb;
		delete load_i;
		delete view_i;
		delete edit_i;
		delete clear_i;
	}
};

/*
 */
void texture_name_pressed(int num) {
	MTexture texture = textures[num];
	WidgetEditLine name_el = texture.name_el;
	if(material.getImageTextureName(num) != name_el.getText()) {
		save_state(create_state(material,Unredo::STATE_MODIFY));
		material.setImageTextureName(num,name_el.getText());
		texture.name_el.setText(material.getImageTextureName(num));
	}
	texture.update();
}

void texture_load_clicked(int num) {
	MTexture texture = textures[num];
	string name = material.getImageTextureName(num);
	string old_name = name;
	if(texturesDialogImage(format(TR("Select %s texture"),texture.name),material.getTextureName(num),name)) {
		if(name != old_name) save_state(create_state(material,Unredo::STATE_MODIFY));
		material.setImageTextureName(num,name);
		texture.name_el.setText(material.getImageTextureName(num));
		texturesAdd(material.getImageTextureName(num),material.getTextureName(num));
	}
	texture.update();
}

void texture_view_clicked(int num) {
	MTexture texture = textures[num];
	dialogImage(format(TR("%s texture"),texture.name),material.getImageTextureName(num));
}

void texture_edit_clicked(int num) {
	MTexture texture = textures[num];
	dialogTexture(format(TR("%s texture"),texture.name),texture.name,material,num,texture.override);
}

void texture_clear_clicked(int num) {
	MTexture texture = textures[num];
	if(dialogMessageYesNo(TR("Confirm"),format(TR("Reset the %s texture?"),texture.name))) {
		save_state(create_state(material,Unredo::STATE_MODIFY));
		material.setImageTextureName(num,NULL);
		texture.name_el.setText(material.getImageTextureName(num));
	}
	texture.update();
}

/*
 */
void update_textures() {
	
	// delete textures
	foreach(MTexture texture; textures) {
		delete texture;
	}
	textures.clear();
	
	// create textures
	forloop(int i = 0; material.getNumTextures()) {
		textures.append(new MTexture(i));
	}
	
	// attach textures
	MTexture last_texture = NULL;
	for(int i = textures.size() - 1; i >= 0; i--) {
		if(textures[i].grid_gb == NULL) continue;
		last_texture = textures[i];
		break;
	}
	foreach(MTexture t; textures) {
		if(t.grid_gb == NULL) continue;
		if(t.grid_gb == last_texture.grid_gb) continue;
		last_texture.grid_gb.addAttach(t.grid_gb);
	}
	
	// next focus
	Widget first = NULL;
	Widget previous = NULL;
	foreach(MTexture t; textures) {
		if(t.hidden) continue;
		Widget widget = t.name_el;
		if(widget != NULL) {
			if(first == NULL) first = widget;
			if(previous != NULL) previous.setNextFocus(widget);
			previous = widget;
		}
	}
	if(previous != NULL) previous.setNextFocus(first);
}
