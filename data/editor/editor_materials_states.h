/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_materials_states.h
 * Desc:    Unigine editor
 * Version: 1.09
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
MState states[0];

/*
 */
enum {
	STATE_GROUP_DEFAULT = 0,
	STATE_GROUP_PASSES,
	STATE_GROUP_OPTIONS,
	STATE_GROUP_REFLECTION,
};

string state_titles[] = (
	STATE_GROUP_PASSES				: TR("Passes"),
	STATE_GROUP_OPTIONS				: TR("Options"),
	STATE_GROUP_REFLECTION			: TR("Reflection"),
);

Unigine::Triple state_names[] = (
	
	"deferred"						: new Unigine::Triple(TR("Deferred"),					STATE_GROUP_PASSES,			TR("Deferred rendering pass")		),
	"auxiliary"						: new Unigine::Triple(TR(" Auxiliary"),					STATE_GROUP_PASSES,			TR("Auxiliary rendering pass")		),
	"refraction"					: new Unigine::Triple(TR(" Refraction"),				STATE_GROUP_PASSES,			TR("Refraction rendering pass")		),
	"velocity"						: new Unigine::Triple(TR(" Velocity"),					STATE_GROUP_PASSES,			TR("Velocity rendering pass")		),
	"ambient"						: new Unigine::Triple(TR(" Ambient"),					STATE_GROUP_PASSES,			TR("Ambient rendering pass")		),
	"light_prob"					: new Unigine::Triple(TR("Light prob"),					STATE_GROUP_PASSES,			TR("Light prob rendering pass")		),
	"light_spot"					: new Unigine::Triple(TR("Light spot"),					STATE_GROUP_PASSES,			TR("Light spot rendering pass")		),
	"light_omni"					: new Unigine::Triple(TR("Light omni"),					STATE_GROUP_PASSES,			TR("Light omni rendering pass")		),
	"light_proj"					: new Unigine::Triple(TR("Light proj"),					STATE_GROUP_PASSES,			TR("Light proj rendering pass")		),
	"light_world"					: new Unigine::Triple(TR("Light world"),				STATE_GROUP_PASSES,			TR("Light world rendering pass")	),
	
	"detail"						: new Unigine::Triple(TR("   Detail"),					STATE_GROUP_OPTIONS,		TR("Detail texturing mode")							),
	"parallax"						: new Unigine::Triple(TR("Parallax mapping"),			STATE_GROUP_OPTIONS,		TR("Parallax occlusion mapping")					),
	"emission"						: new Unigine::Triple(TR("Ambient emission"),			STATE_GROUP_OPTIONS,		TR("Ambient emission and post processing glow")		),
	"volumetric"					: new Unigine::Triple(TR("Shadow shafts"),				STATE_GROUP_OPTIONS,		TR("Volumetric shadow shafts on light scattering")	),
	"multisample"					: new Unigine::Triple(TR("Multisample"),				STATE_GROUP_OPTIONS,		TR("Multisample shading")							),
	"volume"						: new Unigine::Triple(TR("Soft interaction"),			STATE_GROUP_OPTIONS,		TR("Soft depth interaction")						),
	"shadow"						: new Unigine::Triple(TR("Receive shadows"),			STATE_GROUP_OPTIONS,		TR("Receive light shadows")							),
	"alpha"							: new Unigine::Triple(TR("Alpha fade"),					STATE_GROUP_OPTIONS,		TR("Fade alpha component only")						),
	"fleck"							: new Unigine::Triple(TR("Fleck noise"),				STATE_GROUP_OPTIONS,		TR("Fleck spatial noise")							),
	"global"						: new Unigine::Triple(TR("Global"),						STATE_GROUP_OPTIONS,		TR("Global texture coordinates")					),
	"substitute"					: new Unigine::Triple(TR("Substitute"),					STATE_GROUP_OPTIONS,		TR("Substitute base normals")						),
	"separable"						: new Unigine::Triple(TR("Separable"),					STATE_GROUP_OPTIONS,		TR("Separable shading")								),
	"mask"							: new Unigine::Triple(TR("Mask"),						STATE_GROUP_OPTIONS,		TR("Mask mode")										),
	
	"field_spacer"					: new Unigine::Triple(TR("Spacer interaction"),				STATE_GROUP_OPTIONS,		TR("FieldSpacer interaction")						),
	
	"reflection_dynamic"			: new Unigine::Triple(TR("Dynamical"),						STATE_GROUP_REFLECTION,		TR("Reflection is dynamical")						),
	"reflection_size"				: new Unigine::Triple(TR("Map size"),						STATE_GROUP_REFLECTION,		TR("Size of reflection in pixel")					),
	"reflection_update"				: new Unigine::Triple(TR("Update"),							STATE_GROUP_REFLECTION,		TR("Cubemap update interval")						),
	"reflection_filter"				: new Unigine::Triple(TR("Filter"),							STATE_GROUP_REFLECTION,		TR("Post processing filter")						),
	"reflection_shared"				: new Unigine::Triple(TR("Shared"),							STATE_GROUP_REFLECTION,		TR("Reflection is shared across nodes hierarchy")	),
	"reflection_shadow"				: new Unigine::Triple(TR("Shadow"),							STATE_GROUP_REFLECTION,		TR("Render shadows in the reflection")				),
	"reflection_mipmaps"			: new Unigine::Triple(TR("Mipmaps"),						STATE_GROUP_REFLECTION,		TR("Create reflection mipmaps")						),
	"reflection_two_sided"			: new Unigine::Triple(TR("TwoSided"),						STATE_GROUP_REFLECTION,		TR("Two sided reflection mode")						),
	
	"ambient_light"					: new Unigine::Triple(TR("Ambient light"),					STATE_GROUP_OPTIONS,		TR("Ambient pass world light")						),
	
	"decal_base_fade"				: new Unigine::Triple(TR("Distance fade"),					STATE_GROUP_OPTIONS,		TR("Distance fade")									),
	"decal_deferred_base_fade"		: new Unigine::Triple(TR("Distance fade"),					STATE_GROUP_OPTIONS,		TR("Distance fade")									),
	"decal_deferred_base_angle"		: new Unigine::Triple(TR("Angle threshold"),				STATE_GROUP_OPTIONS,		TR("Angle threshold")								),
	
	"decal_terrain_base_detail_0"	: new Unigine::Triple(TR("Detail 0"),						STATE_GROUP_OPTIONS,		TR("Detail 0 texturing mode")						),
	"decal_terrain_base_detail_1"	: new Unigine::Triple(TR("Detail 1"),						STATE_GROUP_OPTIONS,		TR("Detail 1 texturing mode")						),
	"decal_terrain_base_detail_2"	: new Unigine::Triple(TR("Detail 2"),						STATE_GROUP_OPTIONS,		TR("Detail 2 texturing mode")						),
	"decal_terrain_base_detail_3"	: new Unigine::Triple(TR("Detail 3"),						STATE_GROUP_OPTIONS,		TR("Detail 3 texturing mode")						),
	
	"mesh_stem_base_animation"		: new Unigine::Triple(TR("Animation"),						STATE_GROUP_OPTIONS,		TR("Procedural animation of wind")					),
	"mesh_leaf_base_animation"		: new Unigine::Triple(TR("Animation"),						STATE_GROUP_OPTIONS,		TR("Procedural animation of wind")					),
	"mesh_leaf_base_angle_fade"		: new Unigine::Triple(TR("Angle fade"),						STATE_GROUP_OPTIONS,		TR("Fading of non-parallel to the camera faces")	),
	"mesh_leaf_base_leafs"			: new Unigine::Triple(TR("Leaves"),							STATE_GROUP_OPTIONS,		TR("Leaves rendering mode")							),
	"mesh_leaf_base_phong"			: new Unigine::Triple(TR("Phong shading"),					STATE_GROUP_OPTIONS,		TR("Phong shading")									),
	"mesh_leaf_base_noise_2d"		: new Unigine::Triple(TR("2D noise"),						STATE_GROUP_OPTIONS,		TR("Spatial color noise based on 2D texture")		),
	"mesh_leaf_base_noise_3d"		: new Unigine::Triple(TR("3D noise"),						STATE_GROUP_OPTIONS,		TR("Spatial color noise based on 3D texture")		),
	
	"mesh_terrain_base_detail_0"	: new Unigine::Triple(TR(" Detail 0"),						STATE_GROUP_OPTIONS,		TR("Detail 0 texturing mode")						),
	"mesh_terrain_base_detail_1"	: new Unigine::Triple(TR(" Detail 1"),						STATE_GROUP_OPTIONS,		TR("Detail 1 texturing mode")						),
	"mesh_terrain_base_detail_2"	: new Unigine::Triple(TR(" Detail 2"),						STATE_GROUP_OPTIONS,		TR("Detail 2 texturing mode")						),
	"mesh_terrain_base_detail_3"	: new Unigine::Triple(TR(" Detail 3"),						STATE_GROUP_OPTIONS,		TR("Detail 3 texturing mode")						),
	
	"billboards_base_axis_x"		: new Unigine::Triple(TR("Lock axis X"),					STATE_GROUP_OPTIONS,		TR("Lock axis X")									),
	"billboards_base_axis_z"		: new Unigine::Triple(TR("Lock axis Z"),					STATE_GROUP_OPTIONS,		TR("Lock axis Z")									),
	"billboards_base_screen"		: new Unigine::Triple(TR("Screen aligned"),					STATE_GROUP_OPTIONS,		TR("Screen aligned")								),
	"billboards_base_angular"		: new Unigine::Triple(TR("Angular size"),					STATE_GROUP_OPTIONS,		TR("Billboards are always of the same size regardless to the camera location")),
	
	"grass_base_shape"				: new Unigine::Triple(TR("Shape"),							STATE_GROUP_OPTIONS,		TR("Grass shape rendering type")					),
	"grass_base_animation"			: new Unigine::Triple(TR("Animation"),						STATE_GROUP_OPTIONS,		TR("Procedural animation of wind")					),
	"grass_base_noise"				: new Unigine::Triple(TR("Color noise"),					STATE_GROUP_OPTIONS,		TR("Spatial color noise based on 2D texture")		),
	
	"grass_impostor_base_animation"	: new Unigine::Triple(TR("Animation"),						STATE_GROUP_OPTIONS,		TR("Procedural animation of wind")					),
	"grass_impostor_base_noise"		: new Unigine::Triple(TR("Color noise"),					STATE_GROUP_OPTIONS,		TR("Spatial color noise based on 2D texture")		),
	
	"particles_base_animation"		: new Unigine::Triple(TR("Animation texture"),				STATE_GROUP_OPTIONS,		TR("Uses texture with atlas animation")				),
	
	"volume_fog_base_noise"			: new Unigine::Triple(TR("Noise"),							STATE_GROUP_OPTIONS,		TR("Screen space noise")							),
	"volume_fog_base_height"		: new Unigine::Triple(TR("Falloff"),						STATE_GROUP_OPTIONS,		TR("Height falloff")								),
	
	"volume_cloud_base_samples"		: new Unigine::Triple(TR("Samples"),						STATE_GROUP_OPTIONS,		TR("Number of ray marching samples")				),
	"volume_cloud_base_attenuation"	: new Unigine::Triple(TR("Attenuation"),					STATE_GROUP_OPTIONS,		TR("Attenuation texture")							),
	"volume_cloud_base_volume"		: new Unigine::Triple(TR("Volume"),							STATE_GROUP_OPTIONS,		TR("Uses 3D density texture as cloud data")			),
	
	"volume_shaft_base_samples"		: new Unigine::Triple(TR("Samples"),						STATE_GROUP_OPTIONS,		TR("Number of ray marching samples")				),
	
	"gui_base_mode"					: new Unigine::Triple(TR("Mode"),							STATE_GROUP_OPTIONS,		TR("Rendering mode")								),
	
	"water_base_subsurface"			: new Unigine::Triple(TR("Subsurface"),						STATE_GROUP_OPTIONS,		TR("Water light scattering")						),
	"water_base_foam"				: new Unigine::Triple(TR("Foam"),							STATE_GROUP_OPTIONS,		TR("Procedural foam")								),
	"water_base_caps"				: new Unigine::Triple(TR("  Caps"),							STATE_GROUP_OPTIONS,		TR("Procedural caps")								),
	"water_base_caustic"			: new Unigine::Triple(TR("Caustics"),						STATE_GROUP_OPTIONS,		TR("Procedural caustics")							),
	
	"sky_base_background"			: new Unigine::Triple(TR("Background"),						STATE_GROUP_OPTIONS,		TR("Uses cubemap as sky background")				),
	"sky_base_clouds"				: new Unigine::Triple(TR(" Clouds"),						STATE_GROUP_OPTIONS,		TR("Procedural clouds")								),
	
	"sky_base_background"			: new Unigine::Triple(TR("Background"),						STATE_GROUP_OPTIONS,		TR("Uses cubemap as sky background")),
	"sky_base_clouds"				: new Unigine::Triple(TR(" Clouds"),						STATE_GROUP_OPTIONS,		TR("Procedural clouds")),
	
	"opacity_map"					: new Unigine::Triple(TR("Opacity map"),					STATE_GROUP_OPTIONS,		TR("Opacity map")),
	"additional_effect"				: new Unigine::Triple(TR("Additional effect"),				STATE_GROUP_OPTIONS,		TR("Additional effect")),
	"ao_map"						: new Unigine::Triple(TR("AO map"),							STATE_GROUP_OPTIONS,		TR("AO map")),
	"glass"							: new Unigine::Triple(TR("Glass"),							STATE_GROUP_OPTIONS,		TR("Glass")),
	
	"reflection_type"				: new Unigine::Triple(TR("Type"),							STATE_GROUP_REFLECTION,		TR("Reflection type")),
	
	"workflow"						: new Unigine::Triple(TR("Workflow"),						STATE_GROUP_DEFAULT,		TR("Workflow")),
);

string state_items[] = (
	
	"none"							: TR("Skip"),
	"default"						: TR("Default"),
	"opacity"						: TR("Opacity"),
	"transparent"					: TR("Transparent"),
	"lightmap"						: TR("LightMap"),
	"phong"							: TR("Phong"),
	"phong-rim"						: TR("Phong rim"),
	"anisotropy"					: TR("Anisotropy"),
	"overlay_0"						: TR("Overlay first"),
	"overlay_1"						: TR("Overlay second"),
	"overlay"						: TR(" Overlay"),
	"multiply_0"					: TR("Multiply first"),
	"multiply_1"					: TR("Multiply second"),
	"multiply"						: TR("Multiply"),
	"threshold"						: TR("Threshold"),
	
	"animation"						: TR("Default"),
	"field_animation"				: TR("Field"),
	
	"discard"						: TR("All faces"),
	"1/6"							: TR("One face"),
	"2/6"							: TR("Two faces"),
	"3/6"							: TR("Three faces"),
	"4/6"							: TR("Four faces"),
	"5/6"							: TR("Five faces"),
	"6/6"							: TR("Six faces"),
	
	"disabled"						: TR("Skip"),
	"1x"							: TR("One pass"),
	"2x"							: TR("Two passes"),
	"3x"							: TR("Three passes"),
	"4x"							: TR("Four passes"),
	"5x"							: TR("Five passes"),
	"6x"							: TR("Six passes"),
	
	"decal_terrain_base_none"		: TR("None"),
	"decal_terrain_base_overlay"	: TR(" Overlay"),
	"decal_terrain_base_overlap"	: TR(" Overlap"),
	
	"mesh_leaf_base_none"			: TR("None"),
	"mesh_leaf_base_geometry"		: TR("Geometry"),
	"mesh_leaf_base_billboard"		: TR("Billboard"),
	
	"mesh_terrain_base_none"		: TR("None"),
	"mesh_terrain_base_overlay"		: TR(" Overlay"),
	"mesh_terrain_base_overlap"		: TR(" Overlap"),
	
	"grass_base_billboard"			: TR("Billboard"),
	"grass_base_clutter"			: TR("Clutter"),
	
	"gui_base_default"				: TR("Default"),
	"gui_base_flash"				: TR("Flash"),
	"gui_base_yuv"					: TR("YUV"),
	
	"sky_base_none"					: TR("None"),
	"sky_base_single"				: TR("One texture"),
	"sky_base_double"				: TR("Two textures"),
	
	"first_uv"						: TR("First UV"),
	"second_uv"						: TR("Second UV"),
	
	"microfiber"					: TR("Microfiber"),
	"parallax"						: TR("Parallax"),
	
	"opacity_main"					: TR("Main"),
	"opacity_normal"				: TR("Normal"),
	
	"static"						: TR("Static"),
	"dynamic"						: TR("Dynamic"),
	"dynamic_2d"					: TR("Dynamic 2D"),
	
	"metalness"						: TR("Metalness"),
	"specular"						: TR("Specular"),
	
	"opacity_parallax"				: TR("Opacity Parallax"),
);

/*
 */
string get_state_name(string name) {
	Material m = material;
	while(m != NULL) {
		if(state_names.check(m.getName() + "_" + name)) return TR(state_names[m.getName() + "_" + name].first);
		m = m.getParent();
	}
	if(state_names.check(name)) return TR(state_names[name].first);
	return name;
}

int get_state_group(string name) {
	Material m = material;
	while(m != NULL) {
		if(state_names.check(m.getName() + "_" + name)) return state_names[m.getName() + "_" + name].second;
		m = m.getParent();
	}
	if(state_names.check(name)) return state_names[name].second;
	return STATE_GROUP_DEFAULT;
}

string get_state_tooltip(string name) {
	Material m = material;
	while(m != NULL) {
		if(state_names.check(m.getName() + "_" + name)) return TR(state_names[m.getName() + "_" + name].third);
		m = m.getParent();
	}
	if(state_names.check(name)) return TR(state_names[name].third);
	return "";
}

string get_state_title(int group) {
	if(state_titles.check(group)) return TR(state_titles[group]);
	return "";
}

string get_state_item(string name) {
	Material m = material;
	while(m != NULL) {
		if(state_items.check(m.getName() + "_" + name)) return TR(state_items[m.getName() + "_" + name]);
		m = m.getParent();
	}
	if(state_items.check(name)) return TR(state_items[name]);
	return name;
}

/*
 */
class MState {
	
	int num;					// state number
	string name;				// state name
	int type;					// state type
	int hidden;					// state hidden
	int group;					// state group
	
	WidgetGroupBox group_gb;	// state group
	WidgetGridBox grid_gb;		// state grid
	
	WidgetLabel name_l;			// name label
	WidgetVBox main_hb;			// main hbox
	
	WidgetIcon clear_i;			// state clear
	
	WidgetCheckBox toggle_cb;	// toggle checkbox
	WidgetComboBox switch_cb;	// switch combobox
	
	// update
	void update() {
		int override = (parent != NULL && parent.findState(name) != -1 && material.getState(num) != parent.getState(parent.findState(name)));
		vec4 color = override ? color_green : vec4_zero;
		name_l.setFontColor(color);
		clear_i.setEnabled(override);
		if(toggle_cb != NULL) toggle_cb.setFontColor(color);
	}
	
	// constructor
	MState(int n) {
		
		num = n;
		name = material.getStateName(num);
		type = material.getStateType(num);
		hidden = material.isStateHidden(num);
		group = get_state_group(name);
		
		if(hidden) return;
		
		string tooltip = get_state_tooltip(name);
		
		main_hb = new WidgetHBox(gui);
		
		clear_i = new WidgetIcon(gui,"editor/gui/action_clear.png");
		clear_i.setWidth(24);
		clear_i.setHeight(24);
		clear_i.setToolTip(TR("Reset"));
		
		clear_i.setCallback(GUI_CLICKED,functionid(state_clear_clicked),num);
		
		// create group
		MState parent = NULL;
		for(int i = num - 1; i >= 0; i--) {
			if(states[i].hidden) continue;
			if(states[i].group_gb == NULL) continue;
			parent = states[i];
			break;
		}
		if(parent == NULL || parent.group != group) {
			group_gb = new WidgetGroupBox(gui,get_state_title(group),2,2);
			states_vb.addChild(group_gb,GUI_ALIGN_EXPAND);
			grid_gb = new WidgetGridBox(gui,3,2,2);
			group_gb.addChild(grid_gb,GUI_ALIGN_EXPAND);
		} else {
			group_gb = parent.group_gb;
			grid_gb = parent.grid_gb;
		}
		
		// toggle state
		if(type == MATERIAL_STATE_TOGGLE) {
			
			name_l = new WidgetLabel(gui);
			name_l.setToolTip(tooltip);
			
			toggle_cb = new WidgetCheckBox(gui,get_state_name(name));
			toggle_cb.setChecked(material.getState(num));
			toggle_cb.setToolTip(tooltip);
			main_hb.addChild(toggle_cb,GUI_ALIGN_LEFT);
			
			toggle_cb.setCallback(GUI_CLICKED,functionid(state_toggle_clicked),num);
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		// switch state
		else if(type == MATERIAL_STATE_SWITCH) {
			
			name_l = new WidgetLabel(gui,get_state_name(name) + ":");
			name_l.setToolTip(tooltip);
			
			switch_cb = new WidgetComboBox(gui);
			forloop(int i = 0; material.getStateSwitchNumItems(num)) {
				switch_cb.addItem(get_state_item(material.getStateSwitchItem(num,i)));
			}
			switch_cb.setCurrentItem(material.getState(num));
			switch_cb.setToolTip(tooltip);
			main_hb.addChild(switch_cb,GUI_ALIGN_EXPAND);
			
			switch_cb.setCallback(GUI_CHANGED,functionid(state_switch_changed),num);
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		else {
			throw("Materials::MState::MState(): unknown state type\n");
		}
		
		update();
	}
	
	~MState() {
		
		delete group_gb;
		delete grid_gb;
		
		delete name_l;
		delete main_hb;
		
		delete clear_i;
		
		delete toggle_cb;
		delete switch_cb;
	}
};

/*
 */
void state_toggle_clicked(int num) {
	save_state(create_state(material,Unredo::STATE_MODIFY));
	MState state = states[num];
	state.toggle_cb.setCallbackEnabled(GUI_CLICKED,0);
	material.setState(num,state.toggle_cb.isChecked());
	state.toggle_cb.setChecked(material.getState(num));
	state.toggle_cb.setCallbackEnabled(GUI_CLICKED,1);
	state.update();
	update_textures();
	update_parameters();
}

void state_switch_changed(int num) {
	save_state(create_state(material,Unredo::STATE_MODIFY));
	MState state = states[num];
	state.switch_cb.setCallbackEnabled(GUI_CHANGED,0);
	material.setState(num,state.switch_cb.getCurrentItem());
	state.switch_cb.setCurrentItem(material.getState(num));
	state.switch_cb.setCallbackEnabled(GUI_CHANGED,1);
	state.update();
	update_textures();
	update_parameters();
}

/*
 */
void state_clear_clicked(int num) {
	MState state = states[num];
	int type = state.type;
	if(dialogMessageYesNo(TR("Confirm"),format(TR("Reset the %s state?"),state.name))) {
		if(parent != NULL && parent.findState(state.name) != -1) {
			save_state(create_state(material,Unredo::STATE_MODIFY));
			if(type == MATERIAL_STATE_TOGGLE) {
				WidgetCheckBox toggle_cb = state.toggle_cb;
				int value = parent.getState(parent.findState(state.name));
				material.setState(num,value);
				toggle_cb.setCallbackEnabled(GUI_CLICKED,0);
				toggle_cb.setChecked(value);
				toggle_cb.setCallbackEnabled(GUI_CLICKED,1);
			}
			else if(type == MATERIAL_STATE_SWITCH) {
				WidgetComboBox switch_cb = state.switch_cb;
				int value = parent.getState(parent.findState(state.name));
				material.setState(num,value);
				switch_cb.setCallbackEnabled(GUI_CHANGED,0);
				switch_cb.setCurrentItem(value);
				switch_cb.setCallbackEnabled(GUI_CHANGED,1);
			}
		}
	}
	state.update();
	update_textures();
	update_parameters();
}

/*
 */
void update_states() {
	
	// delete states
	states.delete();
	
	// create states
	forloop(int i = 0; material.getNumStates()) {
		states.append(new MState(i));
	}
	
	// attach states
	MState last_state = NULL;
	for(int i = states.size() - 1; i >= 0; i--) {
		if(states[i].grid_gb == NULL) continue;
		last_state = states[i];
		break;
	}
	foreach(MState s; states) {
		if(s.grid_gb == NULL) continue;
		if(s.grid_gb == last_state.grid_gb) continue;
		last_state.grid_gb.addAttach(s.grid_gb);
	}
}
