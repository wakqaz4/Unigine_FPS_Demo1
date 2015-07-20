/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_materials_parameters.h
 * Desc:    Unigine editor
 * Version: 1.16
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
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
MParameter parameters[0];

/*
 */
enum {
	PARAMETER_EXPRESSION_UNKNOWN = -1,
	PARAMETER_EXPRESSION_FLOAT,
	PARAMETER_EXPRESSION_VEC3,
	PARAMETER_EXPRESSION_VEC4,
	PARAMETER_EXPRESSION_BASE,
	PARAMETER_EXPRESSION_DETAIL,
	PARAMETER_EXPRESSION_MASK,
};

enum {
	PARAMETER_GROUP_DEFAULT = 0,
	PARAMETER_GROUP_TESSELLATION,
	PARAMETER_GROUP_TRANSFORM,
	PARAMETER_GROUP_AUXILIARY,
	PARAMETER_GROUP_DETAIL,
	PARAMETER_GROUP_DETAIL_0,
	PARAMETER_GROUP_DETAIL_1,
	PARAMETER_GROUP_DETAIL_2,
	PARAMETER_GROUP_DETAIL_3,
	PARAMETER_GROUP_LIGHTMAP,
	PARAMETER_GROUP_AMBIENT,
	PARAMETER_GROUP_PARALLAX,
	PARAMETER_GROUP_EMISSION,
	PARAMETER_GROUP_REFLECTION,
	PARAMETER_GROUP_REFRACTION,
	PARAMETER_GROUP_SHADING,
	PARAMETER_GROUP_OVERLAP,
	PARAMETER_GROUP_FIRST,
	PARAMETER_GROUP_SECOND,
	PARAMETER_GROUP_COLOR,
	PARAMETER_GROUP_BASE,
	PARAMETER_GROUP_ANIMATION,
	PARAMETER_GROUP_MASK,
	PARAMETER_GROUP_ANGLE,
	PARAMETER_GROUP_NOISE,
	PARAMETER_GROUP_FADE,
	PARAMETER_GROUP_SOFT,
	PARAMETER_GROUP_CLOUDS,
	PARAMETER_GROUP_SPHERE,
	PARAMETER_GROUP_SHADOW,
	PARAMETER_GROUP_DENSITY,
	PARAMETER_GROUP_SURFACE,
	PARAMETER_GROUP_VOLUME,
	PARAMETER_GROUP_SUBSURFACE,
	PARAMETER_GROUP_FOAM,
	PARAMETER_GROUP_CAPS,
	PARAMETER_GROUP_CAUSTIC,
	PARAMETER_GROUP_FLECK,
	PARAMETER_GROUP_FIELD,
	PARAMETER_GROUP_NORMAL,
};

string parameter_titles[] = (
	PARAMETER_GROUP_TESSELLATION		: TR("Tessellation"),
	PARAMETER_GROUP_TRANSFORM			: TR(" Transform"),
	PARAMETER_GROUP_AUXILIARY			: TR("Auxiliary"),
	PARAMETER_GROUP_DETAIL				: TR("Detail"),
	PARAMETER_GROUP_DETAIL_0			: TR(" Detail 0"),
	PARAMETER_GROUP_DETAIL_1			: TR(" Detail 1"),
	PARAMETER_GROUP_DETAIL_2			: TR(" Detail 2"),
	PARAMETER_GROUP_DETAIL_3			: TR(" Detail 3"),
	PARAMETER_GROUP_LIGHTMAP			: TR("Lightmap"),
	PARAMETER_GROUP_AMBIENT				: TR("Ambient"),
	PARAMETER_GROUP_PARALLAX			: TR("Parallax"),
	PARAMETER_GROUP_EMISSION			: TR("Emission"),
	PARAMETER_GROUP_REFLECTION			: TR(" Reflection"),
	PARAMETER_GROUP_REFRACTION			: TR("Refraction"),
	PARAMETER_GROUP_SHADING				: TR("Shading"),
	PARAMETER_GROUP_OVERLAP				: TR("Overlap"),
	PARAMETER_GROUP_FIRST				: TR("First"),
	PARAMETER_GROUP_SECOND				: TR("Second"),
	PARAMETER_GROUP_COLOR				: TR("Color"),
	PARAMETER_GROUP_BASE				: TR("Base"),
	PARAMETER_GROUP_ANIMATION			: TR("  Animation"),
	PARAMETER_GROUP_MASK				: TR("Mask"),
	PARAMETER_GROUP_ANGLE				: TR("Angle"),
	PARAMETER_GROUP_NOISE				: TR("Noise"),
	PARAMETER_GROUP_FADE				: TR("Fade"),
	PARAMETER_GROUP_SOFT				: TR("Soft Interaction"),
	PARAMETER_GROUP_CLOUDS				: TR("Clouds"),
	PARAMETER_GROUP_SPHERE				: TR("Sphere"),
	PARAMETER_GROUP_SHADOW				: TR("Shadow"),
	PARAMETER_GROUP_DENSITY				: TR("Density"),
	PARAMETER_GROUP_SURFACE				: TR("Surface"),
	PARAMETER_GROUP_VOLUME				: TR(" Volume"),
	PARAMETER_GROUP_SUBSURFACE			: TR("Subsurface"),
	PARAMETER_GROUP_FOAM				: TR("Foam"),
	PARAMETER_GROUP_CAPS				: TR("Caps"),
	PARAMETER_GROUP_CAUSTIC				: TR("Caustics"),
	PARAMETER_GROUP_FLECK				: TR(" Fleck"),
	PARAMETER_GROUP_FIELD				: TR("Field"),
	PARAMETER_GROUP_NORMAL				: TR("Normal"),
);

Unigine::Quadruple parameter_names[] = (
	
	"tessellation_scale"				: new Unigine::Quadruple(TR(" Scale"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_TESSELLATION,	TR("Tessellation height map offset scale in units")									),
	"tessellation_power"				: new Unigine::Quadruple(TR("Power"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_TESSELLATION,	TR("Distance height map offset falloff power")										),
	"tessellation_factor"				: new Unigine::Quadruple(TR("Factor"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_TESSELLATION,	TR("Number of tessellation subdivisions")											),
	"tessellation_length"				: new Unigine::Quadruple(TR("Length"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_TESSELLATION,	TR("Minimal length of subdivided triangle edge")									),
	"tessellation_distance"				: new Unigine::Quadruple(TR("Distance"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_TESSELLATION,	TR("Distance with constant tessellation factor and offset")							),
	"tessellation_threshold"			: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_TESSELLATION,	TR("Distance of tessellation subdivision falloff")									),
	"tessellation_attenuation"			: new Unigine::Quadruple(TR(" Attenuation"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_TESSELLATION,	TR("Distance tessellation subdivision falloff power")								),
	
	"base_transform"					: new Unigine::Quadruple(TR("Base"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Base texture coordinates transformation, X,Y - scale, Z,W - offset")			),
	"mask_transform"					: new Unigine::Quadruple(TR(" Mask"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Mask texture coordinates transformation, X,Y - scale, Z,W - offset")			),
	"base_0_transform"					: new Unigine::Quadruple(TR("Base X"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("X base texture coordinates transformation, X,Y - scale, Z,W - offset")			),
	"base_1_transform"					: new Unigine::Quadruple(TR("Base Y"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Y base texture coordinates transformation, X,Y - scale, Z,W - offset")			),
	"base_2_transform"					: new Unigine::Quadruple(TR("Base Z"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Z base texture coordinates transformation, X,Y - scale, Z,W - offset")			),
	"layer_0_transform"					: new Unigine::Quadruple(TR("Layer 0"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Layer first texture coordinates transformation, X,Y - scale, Z,W - offset")		),
	"layer_1_transform"					: new Unigine::Quadruple(TR("Layer 1"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Layer second texture coordinates transformation, X,Y - scale, Z,W - offset")	),
	"noise_0_transform"					: new Unigine::Quadruple(TR("Noise 0"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Noise first texture coordinates transformation, X,Y - scale, Z,W - offset")		),
	"noise_1_transform"					: new Unigine::Quadruple(TR("Noise 1"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Noise second texture coordinates transformation, X,Y - scale, Z,W - offset")	),
	"overlay_transform"					: new Unigine::Quadruple(TR("Overlay"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Overlay texture coordinates transformation, X,Y - scale, Z,W - offset")			),
	"overlap_transform"					: new Unigine::Quadruple(TR("Overlap"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Overlap texture coordinates transformation, X,Y - scale, Z,W - offset")			),
	
	"detail_transform"					: new Unigine::Quadruple(TR("  Detail"),			PARAMETER_EXPRESSION_DETAIL,	PARAMETER_GROUP_TRANSFORM,		TR("Detail texture coordinates transformation, X,Y - scale, Z,W - offset")			),
	"detail_0_transform"				: new Unigine::Quadruple(TR("Detail 0"),			PARAMETER_EXPRESSION_DETAIL,	PARAMETER_GROUP_TRANSFORM,		TR("Terrain detail texture coordinates transformation, X,Y - scale, Z,W - offset")	),
	"detail_1_transform"				: new Unigine::Quadruple(TR("Detail 1"),			PARAMETER_EXPRESSION_DETAIL,	PARAMETER_GROUP_TRANSFORM,		TR("Terrain detail texture coordinates transformation, X,Y - scale, Z,W - offset")	),
	"detail_2_transform"				: new Unigine::Quadruple(TR("Detail 2"),			PARAMETER_EXPRESSION_DETAIL,	PARAMETER_GROUP_TRANSFORM,		TR("Terrain detail texture coordinates transformation, X,Y - scale, Z,W - offset")	),
	"detail_3_transform"				: new Unigine::Quadruple(TR("Detail 3"),			PARAMETER_EXPRESSION_DETAIL,	PARAMETER_GROUP_TRANSFORM,		TR("Terrain detail texture coordinates transformation, X,Y - scale, Z,W - offset")	),
	
	"auxiliary_color"					: new Unigine::Quadruple(TR("Color"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_AUXILIARY,		TR("Auxiliary rendering pass constant color")			),
	
	"detail_diffuse"					: new Unigine::Quadruple(TR(" Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Detail diffuse texture scale")						),
	"detail_normal"						: new Unigine::Quadruple(TR("Normal"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Detail normal texture scale")						),
	"detail_specular"					: new Unigine::Quadruple(TR("Specular"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Detail specular texture scale")						),
	"detail_0_value"					: new Unigine::Quadruple(TR("Value"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_0,		TR("Detail overlap value")								),
	"detail_0_threshold"				: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_0,		TR("Detail overlap threshold")							),
	"detail_0_diffuse"					: new Unigine::Quadruple(TR(" Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_0,		TR("Detail diffuse texture scale")						),
	"detail_0_normal"					: new Unigine::Quadruple(TR("Normal"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_0,		TR("Detail normal texture scale")						),
	"detail_1_value"					: new Unigine::Quadruple(TR("Value"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_1,		TR("Detail overlap value")								),
	"detail_1_threshold"				: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_1,		TR("Detail overlap threshold")							),
	"detail_1_diffuse"					: new Unigine::Quadruple(TR(" Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_1,		TR("Detail diffuse texture scale")						),
	"detail_1_normal"					: new Unigine::Quadruple(TR("Normal"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_1,		TR("Detail normal texture scale")						),
	"detail_2_value"					: new Unigine::Quadruple(TR("Value"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_2,		TR("Detail overlap value")								),
	"detail_2_threshold"				: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_2,		TR("Detail overlap threshold")							),
	"detail_2_diffuse"					: new Unigine::Quadruple(TR(" Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_2,		TR("Detail diffuse texture scale")						),
	"detail_2_normal"					: new Unigine::Quadruple(TR("Normal"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_2,		TR("Detail normal texture scale")						),
	"detail_3_value"					: new Unigine::Quadruple(TR("Value"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_3,		TR("Detail overlap value")								),
	"detail_3_threshold"				: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_3,		TR("Detail overlap threshold")							),
	"detail_3_diffuse"					: new Unigine::Quadruple(TR(" Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_3,		TR("Detail diffuse texture scale")						),
	"detail_3_normal"					: new Unigine::Quadruple(TR("Normal"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL_3,		TR("Detail normal texture scale")						),
	
	"lightmap_scale"					: new Unigine::Quadruple(TR("Scale"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_LIGHTMAP,		TR("Lightmap texture scale")							),
	
	"environment_scale"					: new Unigine::Quadruple(TR("Environment"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_AMBIENT,		TR("Environment lighting scale")						),
	
	"parallax_scale"					: new Unigine::Quadruple(TR("Height"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_PARALLAX,		TR("Parallax height map offset scale")					),
	"parallax_size"						: new Unigine::Quadruple(TR("Size"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_PARALLAX,		TR("Geometry size for correct shadow computation")		),
	
	"emission_color"					: new Unigine::Quadruple(TR("Color"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_EMISSION,		TR("Ambient emission color")							),
	"emission_scale"					: new Unigine::Quadruple(TR("Scale"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_EMISSION,		TR("Ambient emission texture scale")					),
	"glow_scale"						: new Unigine::Quadruple(TR("Glow"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_EMISSION,		TR("Glow post processing scale")						),
	
	"refraction_scale"					: new Unigine::Quadruple(TR("Scale"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFRACTION,		TR("Refraction post processing scale")					),
	"refraction_power"					: new Unigine::Quadruple(TR("Power"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFRACTION,		TR("Refraction post processing power")					),
	
	"refraction_color"					: new Unigine::Quadruple(TR("Color"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFRACTION,		TR("Refraction color of transparent reflection")		),
	
	"refraction_normal"					: new Unigine::Quadruple(TR("Normal"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFRACTION,		TR("Refraction normal map power scale")					),
	
	"reflection_normal"					: new Unigine::Quadruple(TR("Normal"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Reflection normal map power scale")					),
	"reflection_bias"					: new Unigine::Quadruple(TR("Bias"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Reflection mipmaps bias scale")						),
	"reflection_mask"					: new Unigine::Quadruple(TR("Mask"),				PARAMETER_EXPRESSION_MASK,		PARAMETER_GROUP_REFLECTION,		TR("Reflection viewport mask")							),
	"reflection_plane"					: new Unigine::Quadruple(TR("Plane"),				PARAMETER_EXPRESSION_VEC4,		PARAMETER_GROUP_REFLECTION,		TR("Equation of reflection plane")						),
	"reflection_offset"					: new Unigine::Quadruple(TR("Offset"),				PARAMETER_EXPRESSION_VEC3,		PARAMETER_GROUP_REFLECTION,		TR("Offset of reflection cubemap center in units")		),
	"reflection_distance"				: new Unigine::Quadruple(TR("Distance"),			PARAMETER_EXPRESSION_FLOAT,		PARAMETER_GROUP_REFLECTION,		TR("Reflection rendering distance in units")			),
	"reflection_adjust"					: new Unigine::Quadruple(TR("Adjust"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Reflection cubemap spatial adjust")					),
	"reflection_color"					: new Unigine::Quadruple(TR("Color"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Reflection color multiplier")						),
	"reflection_0_color"				: new Unigine::Quadruple(TR("Near color"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Reflection near color multiplier")					),
	"reflection_1_color"				: new Unigine::Quadruple(TR("Far color"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Reflection far color multiplier")					),
	"reflection_scale"					: new Unigine::Quadruple(TR("Scale"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Reflection color scale")							),
	"reflection_power"					: new Unigine::Quadruple(TR("Power"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Reflection power")									),
	"reflection_angle"					: new Unigine::Quadruple(TR("Angle"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Reflection angle")									),
	"reflection_step"					: new Unigine::Quadruple(TR("Step"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Reflection step")									),
	
	"diffuse_color"						: new Unigine::Quadruple(TR("  Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Diffuse color")										),
	"diffuse_scale"						: new Unigine::Quadruple(TR(" Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Scale of diffuse texture")							),
	"normal_scale"						: new Unigine::Quadruple(TR("Normal"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Scale of normal texture")							),
	"specular_color"					: new Unigine::Quadruple(TR("  Specular"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Specular color")									),
	"specular_scale"					: new Unigine::Quadruple(TR("Specular"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Scale of specular texture")							),
	"specular_power"					: new Unigine::Quadruple(TR("Glossiness"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Specular term power")								),
	"phong_rim_width"					: new Unigine::Quadruple(TR("Phong rim width"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Width of Phong rim shading") 						),
	"phong_rim_scale"					: new Unigine::Quadruple(TR("Phong rim scale"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Scale of Phong rim shading")						),
	"anisotropy_angle"					: new Unigine::Quadruple(TR("Anisotropy angle"),	PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Anisotropy shading angle")							),
	"anisotropy_scale"					: new Unigine::Quadruple(TR("Anisotropy scale"),	PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Anisotropy shading scale")							),
	"fresnel_bias"						: new Unigine::Quadruple(TR("Fresnel bias"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Fresnel term level bias")							),
	"fresnel_power"						: new Unigine::Quadruple(TR("Fresnel power"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Fresnel term power")								),
	
	"animation_stem_noise"				: new Unigine::Quadruple(TR("Stem noise"),			PARAMETER_EXPRESSION_FLOAT,		PARAMETER_GROUP_ANIMATION,		TR("Spatial noise scale")								),
	"animation_stem_offset"				: new Unigine::Quadruple(TR("Stem offset"),			PARAMETER_EXPRESSION_FLOAT,		PARAMETER_GROUP_ANIMATION,		TR("Animation offset amplitude height scale")			),
	"animation_stem_radius"				: new Unigine::Quadruple(TR("Stem radius"),			PARAMETER_EXPRESSION_FLOAT,		PARAMETER_GROUP_ANIMATION,		TR("Animation rotation amplitude scale") 				),
	"animation_stem_scale"				: new Unigine::Quadruple(TR("Stem scale"),			PARAMETER_EXPRESSION_FLOAT,		PARAMETER_GROUP_ANIMATION,		TR("Animation time scale")								),
	"animation_leaf_noise"				: new Unigine::Quadruple(TR("Leaf noise"),			PARAMETER_EXPRESSION_FLOAT,		PARAMETER_GROUP_ANIMATION,		TR("Spatial noise scale")								),
	"animation_leaf_offset"				: new Unigine::Quadruple(TR("Leaf offset"),			PARAMETER_EXPRESSION_FLOAT,		PARAMETER_GROUP_ANIMATION,		TR("Animation rotation amplitude scale")				),
	"animation_leaf_scale"				: new Unigine::Quadruple(TR("Leaf scale"),			PARAMETER_EXPRESSION_FLOAT,		PARAMETER_GROUP_ANIMATION,		TR("Animation time scale")								),
	
	"shadow_offset"						: new Unigine::Quadruple(TR("Shadow"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_BASE,			TR("Shadow offset")										),
	"slope_scale"						: new Unigine::Quadruple(TR("Slope"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_BASE,			TR("Inclination of grass blades")						),
	"alpha_scale"						: new Unigine::Quadruple(TR("Alpha"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_BASE,			TR("Grass alpha channel scale")							),
	"angle_power"						: new Unigine::Quadruple(TR("Angle"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_BASE,			TR("Angle fade power")									),
	
	"impostor_transform"				: new Unigine::Quadruple(TR("Impostor"),			PARAMETER_EXPRESSION_VEC4,		PARAMETER_GROUP_TRANSFORM,		TR("Impostor coordinates, X,Y - atlas size")			),
	
	"triplanar_power"					: new Unigine::Quadruple(TR("Power"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Direction power")									),
	
	"angle_value"						: new Unigine::Quadruple(TR("Value"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_ANGLE,			TR("Angle value")										),
	"angle_threshold"					: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_ANGLE,			TR("Angle threshold")									),
	
	"mesh_overlap_base_mask_value"		: new Unigine::Quadruple(TR("Value"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_MASK,			TR("Mask value")										),
	"mesh_overlap_base_mask_threshold"	: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_MASK,			TR("Mask threshold")									),
	
	"base_diffuse_color"				: new Unigine::Quadruple(TR("  Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Diffuse color")										),
	"base_diffuse_scale"				: new Unigine::Quadruple(TR(" Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Scale of diffuse texture")							),
	"base_normal_scale"					: new Unigine::Quadruple(TR("Normal"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Scale of normal texture")							),
	"base_specular_color"				: new Unigine::Quadruple(TR("  Specular"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Specular color")									),
	"base_specular_scale"				: new Unigine::Quadruple(TR("Specular"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Scale of specular texture")							),
	"base_specular_power"				: new Unigine::Quadruple(TR("Glossiness"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Specular term power")								),
	"base_phong_rim_width"				: new Unigine::Quadruple(TR("Phong rim width"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Width of Phong rim shading")						),
	"base_phong_rim_scale"				: new Unigine::Quadruple(TR("Phong rim scale"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Scale of Phong rim shading")						),
	"base_fresnel_bias"					: new Unigine::Quadruple(TR("Fresnel bias"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Fresnel term level bias")							),
	"base_fresnel_power"				: new Unigine::Quadruple(TR("Fresnel power"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Fresnel term power")								),
	
	"overlap_diffuse_color"				: new Unigine::Quadruple(TR("  Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_OVERLAP,		TR("Diffuse color")										),
	"overlap_diffuse_scale"				: new Unigine::Quadruple(TR(" Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_OVERLAP,		TR("Scale of diffuse texture")							),
	"overlap_normal_scale"				: new Unigine::Quadruple(TR("Normal"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_OVERLAP,		TR("Scale of normal texture")							),
	"overlap_specular_color"			: new Unigine::Quadruple(TR("  Specular"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_OVERLAP,		TR("Specular color")									),
	"overlap_specular_scale"			: new Unigine::Quadruple(TR("Specular"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_OVERLAP,		TR("Scale of specular texture")							),
	"overlap_specular_power"			: new Unigine::Quadruple(TR("Glossiness"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_OVERLAP,		TR("Specular term power")								),
	"overlap_phong_rim_width"			: new Unigine::Quadruple(TR("Phong rim width"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_OVERLAP,		TR("Width of Phong rim shading")						),
	"overlap_phong_rim_scale"			: new Unigine::Quadruple(TR("Phong rim scale"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_OVERLAP,		TR("Scale of Phong rim shading")						),
	"overlap_fresnel_bias"				: new Unigine::Quadruple(TR("Fresnel bias"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_OVERLAP,		TR("Fresnel term level bias")							),
	"overlap_fresnel_power"				: new Unigine::Quadruple(TR("Fresnel power"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_OVERLAP,		TR("Fresnel term power")								),
	
	"mesh_layer_base_mask_value"		: new Unigine::Quadruple(TR("Value"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_MASK,			TR("Mask value")										),
	"mesh_layer_base_mask_threshold"	: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_MASK,			TR("Mask threshold")									),
	
	"mesh_layer_base_diffuse_0_color"	: new Unigine::Quadruple(TR("  Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FIRST,			TR("Diffuse color")										),
	"mesh_layer_base_diffuse_0_scale"	: new Unigine::Quadruple(TR(" Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FIRST,			TR("Scale of diffuse texture")							),
	"mesh_layer_base_normal_0_scale"	: new Unigine::Quadruple(TR("Normal"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FIRST,			TR("Scale of normal texture")							),
	"mesh_layer_base_specular_0_color"	: new Unigine::Quadruple(TR("  Specular"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FIRST,			TR("Specular color")									),
	"mesh_layer_base_specular_0_scale"	: new Unigine::Quadruple(TR("Specular"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FIRST,			TR("Scale of specular texture")							),
	"mesh_layer_base_specular_0_power"	: new Unigine::Quadruple(TR("Glossiness"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FIRST,			TR("Specular term power")								),
	"mesh_layer_base_phong_rim_0_width"	: new Unigine::Quadruple(TR("Phong rim width"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FIRST,			TR("Width of Phong rim shading") 						),
	"mesh_layer_base_phong_rim_0_scale"	: new Unigine::Quadruple(TR("Phong rim scale"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FIRST,			TR("Scale of Phong rim shading")						),
	"mesh_layer_base_fresnel_0_bias"	: new Unigine::Quadruple(TR("Fresnel bias"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FIRST,			TR("Fresnel term level bias")							),
	"mesh_layer_base_fresnel_0_power"	: new Unigine::Quadruple(TR("Fresnel power"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FIRST,			TR("Fresnel term power")								),
	
	"mesh_layer_base_diffuse_1_color"	: new Unigine::Quadruple(TR("  Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SECOND,			TR("Diffuse color")										),
	"mesh_layer_base_diffuse_1_scale"	: new Unigine::Quadruple(TR(" Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SECOND,			TR("Scale of diffuse texture")							),
	"mesh_layer_base_normal_1_scale"	: new Unigine::Quadruple(TR("Normal"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SECOND,			TR("Scale of normal texture")							),
	"mesh_layer_base_specular_1_color"	: new Unigine::Quadruple(TR("  Specular"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SECOND,			TR("Specular color")									),
	"mesh_layer_base_specular_1_scale"	: new Unigine::Quadruple(TR("Specular"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SECOND,			TR("Scale of specular texture")							),
	"mesh_layer_base_specular_1_power"	: new Unigine::Quadruple(TR("Glossiness"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SECOND,			TR("Specular term power")								),
	"mesh_layer_base_phong_rim_1_width"	: new Unigine::Quadruple(TR("Phong rim width"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SECOND,			TR("Width of Phong rim shading") 						),
	"mesh_layer_base_phong_rim_1_scale"	: new Unigine::Quadruple(TR("Phong rim scale"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SECOND,			TR("Scale of Phong rim shading")						),
	"mesh_layer_base_fresnel_1_bias"	: new Unigine::Quadruple(TR("Fresnel bias"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SECOND,			TR("Fresnel term level bias")							),
	"mesh_layer_base_fresnel_1_power"	: new Unigine::Quadruple(TR("Fresnel power"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SECOND,			TR("Fresnel term power")								),
	
	"mesh_noise_base_mask_scale"		: new Unigine::Quadruple(TR("Scale"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_MASK,			TR("Mask scale")										),
	"mesh_noise_base_mask_threshold"	: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_MASK,			TR("Mask threshold")									),
	"mesh_noise_base_noise_scale"		: new Unigine::Quadruple(TR("Scale"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_NOISE,			TR("Noise scale")										),
	"mesh_noise_base_noise_threshold"	: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_NOISE,			TR("Noise threshold")									),
	
	"noise_transform"					: new Unigine::Quadruple(TR("  Transform"),			PARAMETER_EXPRESSION_VEC3,		PARAMETER_GROUP_NOISE,			TR("Spatial noise coordinates transformation")			),
	"noise_2d_transform"				: new Unigine::Quadruple(TR("2D transform"),		PARAMETER_EXPRESSION_VEC3,		PARAMETER_GROUP_NOISE,			TR("Spatial noise coordinates transformation")			),
	"noise_3d_transform"				: new Unigine::Quadruple(TR("3D transform"),		PARAMETER_EXPRESSION_VEC3,		PARAMETER_GROUP_NOISE,			TR("Spatial noise coordinates transformation")			),
	
	"noise_2d_scale"					: new Unigine::Quadruple(TR("2D scale"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_NOISE,			TR("Spatial noise color transformation")				),
	"noise_3d_scale"					: new Unigine::Quadruple(TR("3D scale"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_NOISE,			TR("Spatial noise color transformation")				),
	"noise_scale"						: new Unigine::Quadruple(TR("Scale"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_NOISE,			TR("Spatial noise color transformation")				),
	
	"translucent_scale"					: new Unigine::Quadruple(TR("Back"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Back translucence lighting term")					),
	"translucent_offset"				: new Unigine::Quadruple(TR("Side"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Side translucence lighting term")					),
	
	"diffuse_0_power"					: new Unigine::Quadruple(TR("Glossiness 0"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("First diffuse power")								),
	"diffuse_1_power"					: new Unigine::Quadruple(TR("Glossiness 1"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Second diffuse power")								),
	"diffuse_2_power"					: new Unigine::Quadruple(TR("Glossiness 2"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Third diffuse power")								),
	"diffuse_0_color"					: new Unigine::Quadruple(TR("Diffuse 0"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("First diffuse color")								),
	"diffuse_1_color"					: new Unigine::Quadruple(TR("Diffuse 1"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Second diffuse color")								),
	"diffuse_2_color"					: new Unigine::Quadruple(TR("Diffuse 2"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Third diffuse color")								),
	
	"fleck_size"						: new Unigine::Quadruple(TR("Size"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FLECK,			TR("Fleck spatial size")								),
	"fleck_power"						: new Unigine::Quadruple(TR("Power"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FLECK,			TR("Fleck noise power")									),
	"fleck_radius"						: new Unigine::Quadruple(TR("Radius"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FLECK,			TR("Fleck angle radius")								),
	"fleck_color"						: new Unigine::Quadruple(TR("Color"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FLECK,			TR("Fleck color")										),
	
	"animation_transform"				: new Unigine::Quadruple(TR("   Animation"),			PARAMETER_EXPRESSION_VEC4,		PARAMETER_GROUP_TRANSFORM,		TR("Animation coordinates, X,Y - atlas size, Z - time scale, W - Y coordinate of attenuation")	),
	
	"deferred_threshold"				: new Unigine::Quadruple(TR("Deferred threshold"),	PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_BASE,			TR("Deferred rendering pass alpha threshold")			),
	"auxiliary_threshold"				: new Unigine::Quadruple(TR("Auxiliary threshold"),	PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_BASE,			TR("Auxiliary rendering pass alpha threshold")			),
	
	"volume_scale"						: new Unigine::Quadruple(TR("Interaction"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SOFT,			TR("Soft interaction scale")							),
	
	"attenuation_transform"				: new Unigine::Quadruple(TR("Attenuation"),			PARAMETER_EXPRESSION_FLOAT,		PARAMETER_GROUP_TRANSFORM,		TR("Attenuation texture Y coordinate transformation")	),
	
	"volume_fog_base_noise_transform"	: new Unigine::Quadruple(TR("  Transform"),			PARAMETER_EXPRESSION_FLOAT,		PARAMETER_GROUP_NOISE,			TR("Spatial noise coordinates transformation")			),
	
	"volume_cloud_base_base_scale"		: new Unigine::Quadruple(TR("Scale"),				PARAMETER_EXPRESSION_VEC3,		PARAMETER_GROUP_TRANSFORM,		TR("Density texture coordinates scale")					),
	"volume_cloud_base_base_offset"		: new Unigine::Quadruple(TR("Offset"),				PARAMETER_EXPRESSION_VEC3,		PARAMETER_GROUP_TRANSFORM,		TR("Density texture coordinates offset")				),
	
	"volume_power"						: new Unigine::Quadruple(TR("Power"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DENSITY,		TR("Distance power")									),
	"volume_density"					: new Unigine::Quadruple(TR("Multiplier"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DENSITY,		TR("Volume density")									),
	"height_falloff"					: new Unigine::Quadruple(TR("Falloff"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DENSITY,		TR("Height falloff")									),
	"volume_radius"						: new Unigine::Quadruple(TR("Radius"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DENSITY,		TR("Radius of volume")									),
	
	"gui_base_diffuse_color"			: new Unigine::Quadruple(TR("  Diffuse"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_BASE,			TR("Gui diffuse color")									),
	"gui_base_specular_color"			: new Unigine::Quadruple(TR("Specular"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_BASE,			TR("Gui specular color")								),
	
	"normal_0_transform"				: new Unigine::Quadruple(TR("Normal 0"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("First normal texture coordinates transformation")	),
	"normal_1_transform"				: new Unigine::Quadruple(TR("Normal 1"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Second normal texture coordinates transformation")	),
	"normal_2_transform"				: new Unigine::Quadruple(TR("Normal 2"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Third normal texture coordinates transformation")	),
	"normal_3_transform"				: new Unigine::Quadruple(TR("Normal 3"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Fourth normal texture coordinates transformation")	),
	"foam_0_transform"					: new Unigine::Quadruple(TR("Foam 0"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Foam texture coordinates transformation")			),
	"foam_1_transform"					: new Unigine::Quadruple(TR("Foam 1"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Foam texture coordinates transformation")			),
	"caps_0_transform"					: new Unigine::Quadruple(TR("Caps 0"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Caps texture coordinates transformation")			),
	"caps_1_transform"					: new Unigine::Quadruple(TR("Caps 1"),				PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Caps texture coordinates transformation")			),
	"caustic_0_transform"				: new Unigine::Quadruple(TR("Caustic 0"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("First caustic texture coordinates transformation")	),
	"caustic_1_transform"				: new Unigine::Quadruple(TR("Caustic 1"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Second caustic texture coordinates transformation")	),
	
	"water_base_waves_radius"			: new Unigine::Quadruple(TR("Waves radius"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_ANIMATION,		TR("Radius of geometrical waves in units")				),
	
	"water_base_surface_color"			: new Unigine::Quadruple(TR("Color"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SURFACE,		TR("Surface color")										),
	"water_base_surface_density"		: new Unigine::Quadruple(TR("Density"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SURFACE,		TR("Surface density")									),
	
	"water_base_volume_color"			: new Unigine::Quadruple(TR("Color"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_VOLUME,			TR("Volume color")										),
	"water_base_volume_power"			: new Unigine::Quadruple(TR("Power"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_VOLUME,			TR("Volume distance power")								),
	"water_base_volume_density"			: new Unigine::Quadruple(TR("Density"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_VOLUME,			TR("Volume density")									),
	
	"water_base_subsurface_color"		: new Unigine::Quadruple(TR("Color"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SUBSURFACE,		TR("Subsurface color")									),
	"water_base_subsurface_power"		: new Unigine::Quadruple(TR("Power"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SUBSURFACE,		TR("Subsurface distance power")							),
	"water_base_subsurface_density"		: new Unigine::Quadruple(TR("Density"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SUBSURFACE,		TR("Subsurface density")								),
	"water_base_subsurface_falloff"		: new Unigine::Quadruple(TR("Falloff"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SUBSURFACE,		TR("Subsurface falloff")								),
	
	"water_base_foam_color"				: new Unigine::Quadruple(TR("Color"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FOAM,			TR("Foam color")										),
	"water_base_foam_scale"				: new Unigine::Quadruple(TR("Multiplier"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FOAM,			TR("Foam color multiplier")								),
	"water_base_foam_threshold"			: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FOAM,			TR("Foam texture threshold")							),
	"water_base_foam_falloff"			: new Unigine::Quadruple(TR("Falloff"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_FOAM,			TR("Foam falloff")										),
	
	"water_base_caps_color"				: new Unigine::Quadruple(TR("Color"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CAPS,			TR("Caps color")										),
	"water_base_caps_scale"				: new Unigine::Quadruple(TR("Multiplier"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CAPS,			TR("Caps color multiplier")								),
	"water_base_caps_threshold"			: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CAPS,			TR("Caps texture threshold")							),
	
	"water_base_caustic_color"			: new Unigine::Quadruple(TR("Color"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CAUSTIC,		TR("Caustic color")										),
	"water_base_caustic_power"			: new Unigine::Quadruple(TR("Power"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CAUSTIC,		TR("Caustic power")										),
	"water_base_caustic_scale"			: new Unigine::Quadruple(TR("Multiplier"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CAUSTIC,		TR("Caustic scale")										),
	"water_base_caustic_threshold"		: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CAUSTIC,		TR("Caustic normal threshold")							),
	"water_base_caustic_density"		: new Unigine::Quadruple(TR("Density"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CAUSTIC,		TR("Caustic density")									),
	"water_base_caustic_falloff"		: new Unigine::Quadruple(TR("Falloff"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CAUSTIC,		TR("Caustic falloff")									),
	
	"sky_base_sphere_transform"			: new Unigine::Quadruple(TR("  Sphere"),			PARAMETER_EXPRESSION_VEC4,		PARAMETER_GROUP_TRANSFORM,		TR("Sky sphere transformation, X,Y,Z - scale, W - ground offset")	),
	"sky_base_volume_transform"			: new Unigine::Quadruple(TR("  Volume"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Volumetric clouds texture coordinates transformation")			),
	"sky_base_noise_transform"			: new Unigine::Quadruple(TR("  Noise"),				PARAMETER_EXPRESSION_VEC3,		PARAMETER_GROUP_TRANSFORM,		TR("Volumetric noise texture coordinates transformation")			),
	"sky_base_mask_0_transform"			: new Unigine::Quadruple(TR("  Mask 0"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("First clouds mask texture coordinates transformation")			),
	"sky_base_mask_1_transform"			: new Unigine::Quadruple(TR("  Mask 1"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Second clouds mask texture coordinates transformation")			),
	"sky_base_clouds_0_transform"		: new Unigine::Quadruple(TR("  Clouds 0"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("First clouds noise texture coordinates transformation")			),
	"sky_base_clouds_1_transform"		: new Unigine::Quadruple(TR("  Clouds 1"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Second clouds noise texture coordinates transformation")		),
	"sky_base_clouds_2_transform"		: new Unigine::Quadruple(TR("  Clouds 2"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Third clouds noise texture coordinates transformation")			),
	"sky_base_clouds_3_transform"		: new Unigine::Quadruple(TR("  Clouds 3"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_TRANSFORM,		TR("Fourth clouds noise texture coordinates transformation")		),
	
	"sky_base_volume_0_color"			: new Unigine::Quadruple(TR("Color 0"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_VOLUME,			TR("Volumetric clouds light color")									),
	"sky_base_volume_1_color"			: new Unigine::Quadruple(TR("Color 1"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_VOLUME,			TR("Volumetric clouds shadow color")								),
	"sky_base_volume_01_scale"			: new Unigine::Quadruple(TR("Multiplier"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_VOLUME,			TR("Volumetric clouds color multiplier")							),
	"sky_base_volume_scattering"		: new Unigine::Quadruple(TR("Scattering"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_VOLUME,			TR("Volumetric clouds scattering interaction")						),
	"sky_base_volume_turbulence"		: new Unigine::Quadruple(TR("Turbulence"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_VOLUME,			TR("Volumetric clouds turbulence noise")							),
	
	"sky_base_clouds_01_scale"			: new Unigine::Quadruple(TR("Clouds 01"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CLOUDS,			TR("Scale of first clouds group")									),
	"sky_base_clouds_23_scale"			: new Unigine::Quadruple(TR("Clouds 23"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CLOUDS,			TR("Scale of second clouds group")									),
	"sky_base_clouds_01_threshold"		: new Unigine::Quadruple(TR("Threshold 01"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CLOUDS,			TR("Threshold of first clouds group")								),
	"sky_base_clouds_23_threshold"		: new Unigine::Quadruple(TR("Threshold 23"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CLOUDS,			TR("Threshold of second clouds group")								),
	
	"sky_base_sphere_scattering"		: new Unigine::Quadruple(TR("Scattering"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SPHERE,			TR("Clouds scattering interaction")									),
	"sky_base_background_0_scale"		: new Unigine::Quadruple(TR("Background 0"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SPHERE,			TR("Background cubemap scale")										),
	"sky_base_background_1_scale"		: new Unigine::Quadruple(TR("Background 1"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SPHERE,			TR("Background cubemap scale")										),
	
	"sky_base_shadow_scale"				: new Unigine::Quadruple(TR("Multiplier"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADOW,			TR("Clouds shadow scale")											),
	
	"field_mask"						: new Unigine::Quadruple(TR("Mask"),				PARAMETER_EXPRESSION_MASK,		PARAMETER_GROUP_FIELD,			TR("Field mask")													),
	
	"cloud_0_color"						: new Unigine::Quadruple(TR("Color 0"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CLOUDS,			TR("Cloud color 0")													),
	"cloud_1_color"						: new Unigine::Quadruple(TR("Color 1"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CLOUDS,			TR("Cloud color 1")													),
	"cloud_01_scale"					: new Unigine::Quadruple(TR("Multiplier"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CLOUDS,			TR("Cloud color scale")												),
	"cloud_scattering"					: new Unigine::Quadruple(TR("Scattering"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CLOUDS,			TR("Cloud scattering")												),
	"cloud_distance"					: new Unigine::Quadruple(TR("Distance"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_CLOUDS,			TR("Cloud distance")												),
	
	"post_filter_wet_normal_transform"	: new Unigine::Quadruple(TR("Transform"),			PARAMETER_EXPRESSION_BASE,		PARAMETER_GROUP_NORMAL,			TR("Normal texture coordinates transformation")						),
	"post_filter_wet_normal_threshold"	: new Unigine::Quadruple(TR("Threshold"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_NORMAL,			TR("Normal threshold")												),
	
	"parallax_transform"			: new Unigine::Quadruple(TR("Parallax"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_TRANSFORM,		TR("Parallax")),
	
	"albedo_color"					: new Unigine::Quadruple(TR("Albedo"),					PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Albedo")),
	"metalness"						: new Unigine::Quadruple(TR("Metalness"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Metalness")),
	"roughness"						: new Unigine::Quadruple(TR("Roughness"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Roughness")),
	"ior"							: new Unigine::Quadruple(TR("IOR"),						PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("IOR")),
	
	"gloss"							: new Unigine::Quadruple(TR("Gloss"),					PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Gloss")),
	
	"microfiber"					: new Unigine::Quadruple(TR("Microfiber"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Microfiber")),
	"parallax_s"					: new Unigine::Quadruple(TR("Parallax height"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Parallax height")),
	"parallax_num_steps"			: new Unigine::Quadruple(TR("Parallax steps"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Parallax steps")),
	"shadow_bias"					: new Unigine::Quadruple(TR("Shadow distortion"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Shadow distortion by normal")),
	"lightmap_s"					: new Unigine::Quadruple(TR("Lightmap"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_SHADING,		TR("Lightmap")),
	
	"mipmaps_number"				: new Unigine::Quadruple(TR("Mipmap count"),			PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Mipmap count")),
	
	"s_detail_albedo"				: new Unigine::Quadruple(TR("Albedo"),					PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Albedo")),
	"s_detail_metalness"			: new Unigine::Quadruple(TR("Metalness"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Metalness")),
	"s_detail_roughness"			: new Unigine::Quadruple(TR("Roughness"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Roughness")),
	"s_detail_albedo_visible"		: new Unigine::Quadruple(TR("Albedo visibility"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Albedo visibility")),
	"s_detail_roughness_visible"	: new Unigine::Quadruple(TR("Roughness visibility"),	PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Roughness visibility")),
	"s_detail_metalness_visible"	: new Unigine::Quadruple(TR("Metalness visibility"),	PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Metalness visibility")),
	
	"s_detail_diffuse"				: new Unigine::Quadruple(TR("Diffuse"),					PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Diffuse")),
	"s_detail_specular"				: new Unigine::Quadruple(TR("Specular"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Specular")),
	"s_detail_gloss"				: new Unigine::Quadruple(TR("Gloss"),					PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Gloss")),
	"s_detail_diffuse_visible"		: new Unigine::Quadruple(TR("Diffuse visibility"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Diffuse visibility")),
	"s_detail_specular_visible"		: new Unigine::Quadruple(TR("Specular visibility"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Specular visibility")),
	"s_detail_gloss_visible"		: new Unigine::Quadruple(TR("Gloss visibility"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Gloss visibility")),
	"s_detail_normal"				: new Unigine::Quadruple(TR("Normal visibility"),		PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_DETAIL,			TR("Normal visibility")),
	
	"samples"						: new Unigine::Quadruple(TR("Samples"),					PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Samples")),
	"reflection_inc_step"			: new Unigine::Quadruple(TR("Inc step"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Inc step")),
	"vignette_scale"				: new Unigine::Quadruple(TR("Vignette"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Vignette")),
	"stretching"					: new Unigine::Quadruple(TR("Stretching"),				PARAMETER_EXPRESSION_UNKNOWN,	PARAMETER_GROUP_REFLECTION,		TR("Stretching")),
);

/*
 */
string get_parameter_name(string name) {
	Material m = material;
	
	while(m != NULL) {
		Unigine::Quadruple record = parameter_names.check(m.getName() + "_" + name,NULL);
		if(record != NULL) return trim(TR(record.first));
		m = m.getParent();
	}
	
	Unigine::Quadruple record = parameter_names.check(name,NULL);
	if(record != NULL) return trim(TR(record.first));
	
	return trim(name);
}

int get_parameter_type(string name) {
	Material m = material;
	while(m != NULL) {
		if(parameter_names.check(m.getName() + "_" + name)) return parameter_names[m.getName() + "_" + name].second;
		m = m.getParent();
	}
	if(parameter_names.check(name)) return parameter_names[name].second;
	return PARAMETER_EXPRESSION_UNKNOWN;
}

int get_parameter_group(string name) {
	Material m = material;
	while(m != NULL) {
		if(parameter_names.check(m.getName() + "_" + name)) return parameter_names[m.getName() + "_" + name].third;
		m = m.getParent();
	}
	if(parameter_names.check(name)) return parameter_names[name].third;
	return PARAMETER_GROUP_DEFAULT;
}

string get_parameter_tooltip(string name) {
	Material m = material;
	while(m != NULL) {
		if(parameter_names.check(m.getName() + "_" + name)) return TR(parameter_names[m.getName() + "_" + name].fourth);
		m = m.getParent();
	}
	if(parameter_names.check(name)) return TR(parameter_names[name].fourth);
	return "";
}

string get_parameter_title(int group) {
	if(parameter_titles.check(group)) return TR(parameter_titles[group]);
	return "";
}

/*
 */
class MParameter {
	
	int num;						// parameter number
	string name;					// parameter name
	int type;						// parameter type
	int hidden;						// parameter hidden
	int group;						// parameter group
	
	WidgetGroupBox group_gb;		// parameter group
	WidgetGridBox grid_gb;			// parameter grid
	
	WidgetLabel name_l;				// name label
	WidgetHBox main_hb;				// main hbox
	
	WidgetIcon clear_i;				// parameter clear
	
	WidgetGroupBox color_gb;		// color groupbox
	WidgetHBox color_hb;
	WidgetSprite color_s[2];		// color value
	
	WidgetEditLine slider_el;		// slider editline
	WidgetVBox slider_vb;			// slider vbox
	WidgetSlider slider_sl;			// slider
	int multiplier;					// multiplier
	
	WidgetEditLine expression_el;	// expression editline
	
	// update
	void update() {
		int override = (parent != NULL && parent.findParameter(name) != -1);
		if(override) {
			override &= (parent.getParameterType(num) != MATERIAL_PARAMETER_CONSTANT || material.getParameter(num) != parent.getParameter(num));
			override &= (parent.getParameterType(num) != MATERIAL_PARAMETER_COLOR || material.getParameter(num) != parent.getParameter(num));
			override &= (parent.getParameterType(num) != MATERIAL_PARAMETER_SLIDER || material.getParameter(num) != parent.getParameter(num));
			override &= (parent.getParameterType(num) != MATERIAL_PARAMETER_EXPRESSION || material.getParameterExpression(num) != parent.getParameterExpression(num));
		}
		name_l.setFontColor(override ? color_green : vec4_zero);
		clear_i.setEnabled(override);
	}
	
	// focus
	Widget get_focus() {
		if(type == MATERIAL_PARAMETER_COLOR) return color_s[0];
		if(type == MATERIAL_PARAMETER_SLIDER) return slider_el;
		if(type == MATERIAL_PARAMETER_EXPRESSION) return expression_el;
		return NULL;
	}
	
	int is_focused() {
		Widget focus = gui.getFocus();
		if(focus == name_l) return 1;
		if(focus == main_hb) return 1;
		if(focus == clear_i) return 1;
		if(focus == color_gb) return 1;
		if(focus == color_hb) return 1;
		if(focus == color_s[0]) return 1;
		if(focus == color_s[1]) return 1;
		if(focus == expression_el) return 1;
		if(focus == slider_el) return 1;
		if(focus == slider_vb) return 1;
		if(focus == slider_sl) return 1;
		return 0;
	}
	
	// constructor
	MParameter(int n) {
		
		num = n;
		name = material.getParameterName(num);
		type = material.getParameterType(num);
		hidden = material.isParameterHidden(num);
		group = get_parameter_group(name);
		
		if(hidden) return;
		
		// render settings
		if(strstr(name,"phong_rim_") != -1 && (engine.render.getShaders() < RENDER_QUALITY_MEDIUM || engine.console.getInt("render_use_phong_rim") == 0)) {
			return;
		}
		if(strstr(name,"anisotropy_") != -1 && engine.render.getShaders() < RENDER_QUALITY_MEDIUM) {
			return;
		}
		
		string tooltip = get_parameter_tooltip(name);
		
		name_l = new WidgetLabel(gui,get_parameter_name(name) + ":");
		name_l.setToolTip(tooltip);
		
		main_hb = new WidgetHBox(gui);
		
		clear_i = new WidgetIcon(gui,"editor/gui/action_clear.png");
		clear_i.setWidth(24);
		clear_i.setHeight(24);
		clear_i.setToolTip(TR("Reset"));
		
		clear_i.setCallback(GUI_CLICKED,functionid(parameter_clear_clicked),num);
		
		// create group
		MParameter parent = NULL;
		for(int i = num - 1; i >= 0; i--) {
			if(parameters[i].hidden) continue;
			if(parameters[i].group_gb == NULL) continue;
			parent = parameters[i];
			break;
		}
		if(parent == NULL || parent.group != group) {
			group_gb = new WidgetGroupBox(gui,get_parameter_title(group),2,2);
			parameters_vb.addChild(group_gb,GUI_ALIGN_EXPAND);
			grid_gb = new WidgetGridBox(gui,3,2,2);
			group_gb.addChild(grid_gb,GUI_ALIGN_EXPAND);
		} else {
			group_gb = parent.group_gb;
			grid_gb = parent.grid_gb;
		}
		
		// constant parameter
		if(type == MATERIAL_PARAMETER_CONSTANT) {
			
		}
		// color parameter
		else if(type == MATERIAL_PARAMETER_COLOR) {
			
			color_gb = new WidgetGroupBox(gui);
			main_hb.addChild(color_gb,GUI_ALIGN_EXPAND);
			
			color_hb = new WidgetHBox(gui);
			color_gb.addChild(color_hb,GUI_ALIGN_EXPAND);
			
			color_s[0] = new WidgetSprite(gui,"editor/gui/color_white.png");
			color_s[0].setTransform(scale(8,8,1));
			color_s[0].setToolTip(tooltip);
			color_hb.addChild(color_s[0],GUI_ALIGN_EXPAND);
			
			color_s[1] = new WidgetSprite(gui,"editor/gui/color_white.png");
			color_s[1].setTransform(scale(8,8,1));
			color_s[1].setToolTip(tooltip);
			color_hb.addChild(color_s[1],GUI_ALIGN_EXPAND);
			
			color_s[0].setCallback(GUI_CLICKED,functionid(parameter_color_clicked),num);
			color_s[1].setCallback(GUI_CLICKED,functionid(parameter_color_clicked),num);
			
			setSpriteColor(color_s,material.getParameter(num));
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		// slider parameter
		else if(type == MATERIAL_PARAMETER_SLIDER) {
			
			float value = material.getParameterSlider(num);
			
			slider_el = new WidgetEditLine(gui);
			slider_el.setToolTip(tooltip);
			main_hb.addChild(slider_el);
			
			slider_vb = new WidgetVBox(gui,2,2);
			main_hb.addChild(slider_vb);
			
			int flags = 0;
			if(material.getParameterSliderLog10(num)) flags |= GUI_ATTACH_LOG10;
			if(material.getParameterSliderMaxExpand(num)) flags |= GUI_ATTACH_MAX_EXPAND;
			if(material.getParameterSliderMinExpand(num)) flags |= GUI_ATTACH_MIN_EXPAND;
			multiplier = (abs(material.getParameterSliderMaxValue(num) - material.getParameterSliderMinValue(num)) > 1000.0f) ? 10 : 1000;
			slider_sl = new WidgetSlider(gui,material.getParameterSliderMinValue(num) * multiplier,material.getParameterSliderMaxValue(num) * multiplier,value * multiplier);
			slider_sl.addAttach(slider_el,"%.3f",multiplier,flags);
			slider_sl.setCallback(GUI_CLICKED,functionid(slider_clicked));
			slider_sl.setCallback(GUI_CHANGED,functionid(slider_changed));
			slider_sl.setToolTip(tooltip);
			main_hb.addChild(slider_sl,GUI_ALIGN_EXPAND);
			
			slider_el.setText(editorFormat(value));
			slider_el.setCallback(GUI_PRESSED,functionid(parameter_slider_pressed),num);
			slider_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_slider_pressed),num);
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		// expression parameter
		else if(type == MATERIAL_PARAMETER_EXPRESSION) {
			
			expression_el = new WidgetEditLine(gui,material.getParameterExpression(num));
			expression_el.setToolTip(tooltip);
			main_hb.addChild(expression_el,GUI_ALIGN_EXPAND);
			
			expression_el.setCallback(GUI_PRESSED,functionid(parameter_expression_pressed),num);
			expression_el.setCallback(GUI_FOCUS_OUT,functionid(parameter_expression_pressed),num);
			
			int type = get_parameter_type(name);
			if(type == PARAMETER_EXPRESSION_VEC3 || type == PARAMETER_EXPRESSION_VEC4) {
				expression_el.setCallback(GUI_DOUBLE_CLICKED,functionid(parameter_expression_double_clicked),num);
			} else if(type == PARAMETER_EXPRESSION_BASE || type == PARAMETER_EXPRESSION_DETAIL) {
				expression_el.setCallback(GUI_DOUBLE_CLICKED,functionid(parameter_texcoord_double_clicked),num);
			} else if(type == PARAMETER_EXPRESSION_MASK) {
				expression_el.setFont("core/gui/console.ttf");
				expression_el.setCallback(GUI_DOUBLE_CLICKED,functionid(parameter_mask_double_clicked),num);
			}
			
			grid_gb.addChild(name_l,GUI_ALIGN_RIGHT);
			grid_gb.addChild(main_hb,GUI_ALIGN_EXPAND);
			grid_gb.addChild(clear_i);
		}
		// array parameter
		else if(type == MATERIAL_PARAMETER_ARRAY) {
			
		}
		else {
			throw("Materials::MParameter::MParameter(): unknown parameter type\n");
		}
		
		update();
	}
	
	~MParameter() {
		
		delete group_gb;
		delete grid_gb;
		
		delete name_l;
		delete main_hb;
		
		delete clear_i;
		
		delete color_gb;
		delete color_hb;
		delete color_s[0];
		delete color_s[1];
		
		delete slider_el;
		delete slider_vb;
		delete slider_sl;
		
		delete expression_el;
	}
};

/*
 */
int parameter_num;

DECLARE_SLIDER_CALLBACKS

/*
 */
void parameter_color_changed(WidgetDialogColor dialog) {
	MParameter parameter = parameters[parameter_num];
	material.setParameter(parameter_num,dialog.getColor());
	setSpriteColor(parameter.color_s,material.getParameter(parameter_num));
}

void parameter_color_clicked(int num) {
	MaterialState_ s = create_state(material,Unredo::STATE_MODIFY);
	vec4 old_color = material.getParameter(num);
	
	parameter_num = num;
	MParameter parameter = parameters[num];
	dialogColor(format(TR("Select %s color"),parameters[num].name),functionid(parameter_color_changed),material.getParameter(num));
	
	if(material.getParameter(num) != old_color) save_state(s);
	else delete s;
	
	parameter.update();
}

/*
 */
void parameter_slider_pressed(int num) {
	MaterialState_ s = create_state(material,Unredo::STATE_MODIFY);
	
	MParameter parameter = parameters[num];
	WidgetEditLine slider_el = parameter.slider_el;
	float value = float(slider_el.getText());
	
	if(skip_state == false && material.getParameterSlider(num) != value) save_state(s);
	else delete s;
	
	material.setParameterSlider(num,value);
	slider_el.setText(editorFormat(value));
	
	if(toolsGetSelection() && slider_el.isFocused()) {
		slider_el.setCursor(0);
		slider_el.setSelection(1024);
	}
	
	parameter.update();
}

/*
 */
void parameter_expression_pressed(int num) {
	MParameter parameter = parameters[num];
	WidgetEditLine expression_el = parameter.expression_el;
	if(material.getParameterExpression(num) != expression_el.getText()) {
		save_state(create_state(material,Unredo::STATE_MODIFY));
		if(material.setParameterExpression(num,expression_el.getText()) == 0 && (gui.getMouseButton() == 0 || expression_el.isFocused() == 0)) {
			dialogMessageOk(TR("Error"));
		}
	}
	parameter.update();
}

void parameter_expression_changed(int num,string expression) {
	
	MParameter parameter = parameters[num];
	material.setParameterExpression(num,expression);
	parameter.expression_el.setText(expression);
	
	parameter.update();
}

void parameter_expression_double_clicked(int num) {
	
	int type = get_parameter_type(parameters[num].name);
	assert(type == PARAMETER_EXPRESSION_VEC3 || type == PARAMETER_EXPRESSION_VEC4);
	
	MaterialState_ s = create_state(material,Unredo::STATE_MODIFY);
	
	string old_expression = material.getParameterExpression(num);
	
	int num_args = (type == PARAMETER_EXPRESSION_VEC3) ? 3 : 4;
	dialogVec(format(TR("Value of %s"),get_parameter_name(parameters[num].name)),functionid(parameter_expression_changed),num,old_expression,num_args);
	
	if(material.getParameterExpression(num) != old_expression) save_state(s);
	else delete s;
}

/*
 */
void parameter_texcoord_changed(int num,string expression) {
	
	MParameter parameter = parameters[num];
	material.setParameterExpression(num,expression);
	parameter.expression_el.setText(expression);
	
	parameter.update();
}

void parameter_texcoord_double_clicked(int num) {
	
	int type = get_parameter_type(parameters[num].name);
	assert(type == PARAMETER_EXPRESSION_BASE || type == PARAMETER_EXPRESSION_DETAIL);
	
	MaterialState_ s = create_state(material,Unredo::STATE_MODIFY);
	
	string old_expression = material.getParameterExpression(num);
	
	vec4 texcoord = vec4(1.0f,1.0f,0.0f,0.0f);
	if(type == PARAMETER_EXPRESSION_DETAIL) {
		for(int i = num - 1; i >= 0; i--) {
			if(get_parameter_type(parameters[i].name) == PARAMETER_EXPRESSION_BASE) {
				texcoord = material.getParameter(i);
				break;
			}
		}
	}
	
	string texture = "";
	for(int i = 0; i < material.getNumTextures(); i++) {
		if(material.getTextureType(i) == MATERIAL_TEXTURE_IMAGE) {
			texture = material.getImageTextureName(i);
			break;
		}
	}
	
	dialogTexCoord(format(TR("%s texture coordinates"),get_parameter_name(parameters[num].name)),functionid(parameter_texcoord_changed),num,old_expression,texcoord,texture);
	
	if(material.getParameterExpression(num) != old_expression) save_state(s);
	else delete s;
}

/*
 */
void parameter_mask_changed(int mask) {
	
	MParameter parameter = parameters[parameter_num];
	string expr = format("0x%08x",mask);
	material.setParameterExpression(parameter_num,expr);
	parameter.expression_el.setText(expr);
	
	parameter.update();
}

void parameter_mask_double_clicked(int num) {
	
	assert(get_parameter_type(parameters[num].name) == PARAMETER_EXPRESSION_MASK);
	
	parameter_num = num;
	
	MaterialState_ s = create_state(material,Unredo::STATE_MODIFY);
	
	int old_mask[1];
	sscanf(parameters[num].expression_el.getText(),"0x%x",old_mask);
	dialogMask(format(TR("Value of %s"),get_parameter_name(parameters[num].name)),functionid(parameter_mask_changed),old_mask[0]);
	
	int mask[1];
	sscanf(material.getParameterExpression(num),"0x%x",mask);
	if(old_mask[0] != mask[0]) save_state(s);
	else delete s;
}

/*
 */
void parameter_clear_clicked(int num) {
	MParameter parameter = parameters[num];
	int type = parameter.type;
	if(dialogMessageYesNo(TR("Confirm"),format(TR("Reset the %s parameter?"),parameter.name))) {
		if(parent != NULL && parent.findParameter(parameter.name) != -1) {
			save_state(create_state(material,Unredo::STATE_MODIFY));
			if(type == MATERIAL_PARAMETER_COLOR) {
				vec4 color = parent.getParameter(parent.findParameter(parameter.name));
				material.setParameter(num,color);
				setSpriteColor(parameter.color_s,color);
			}
			else if(type == MATERIAL_PARAMETER_SLIDER) {
				WidgetEditLine slider_el = parameter.slider_el;
				float value = parent.getParameterSlider(parent.findParameter(parameter.name));
				material.setParameterSlider(num,value);
				slider_el.setText(editorFormat(value));
			}
			else if(type == MATERIAL_PARAMETER_EXPRESSION) {
				WidgetEditLine expression_el = parameter.expression_el;
				string expression = parent.getParameterExpression(parent.findParameter(parameter.name));
				material.setParameterExpression(num,expression);
				expression_el.setText(expression);
			}
		}
	}
	parameter.update();
}

/*
 */
void update_parameters() {
	
	// delete parameters
	parameters.delete();
	
	// create parameters
	forloop(int i = 0; material.getNumParameters()) {
		parameters.append(new MParameter(i));
	}
	
	// attach parameters
	MParameter last_parameter = NULL;
	for(int i = parameters.size() - 1; i >= 0; i--) {
		if(parameters[i].grid_gb == NULL) continue;
		last_parameter = parameters[i];
		break;
	}
	foreach(MParameter p; parameters) {
		if(p.grid_gb == NULL) continue;
		if(p.grid_gb == last_parameter.grid_gb) continue;
		last_parameter.grid_gb.addAttach(p.grid_gb);
	}
	
	// next focus
	Widget first = NULL;
	Widget previous = NULL;
	foreach(MParameter p; parameters) {
		if(p.hidden) continue;
		Widget widget = NULL;
		if(p.type == MATERIAL_PARAMETER_SLIDER) widget = p.slider_el;
		else if(p.type == MATERIAL_PARAMETER_EXPRESSION) widget = p.expression_el;
		if(widget != NULL) {
			if(first == NULL) first = widget;
			if(previous != NULL) previous.setNextFocus(widget);
			previous = widget;
		}
	}
	if(previous != NULL) previous.setNextFocus(first);
}
