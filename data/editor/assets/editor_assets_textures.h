/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_assets_textures.h
 * Desc:    Unigine editor
 * Version: 1.00
 * Author:  Maxim Belobryukhov <maxi@unigine.com>
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

#ifndef __EDITOR_ASSETS_TEXTURES_H__
#define __EDITOR_ASSETS_TEXTURES_H__

namespace Assets::Textures {
	
	/* mipmap types
	 */
	enum {
		MIPMAP_TYPE_BOX = 0,
		MIPMAP_TYPE_POINT,
		MIPMAP_TYPE_COMBINE,
	};
	
	/* map: string -> MIPMAP_TYPE_ enum
	 */
	int name_to_mipmap_type[] = (
		"combine"	: MIPMAP_TYPE_COMBINE,
		"point"		: MIPMAP_TYPE_POINT,
		"box"		: MIPMAP_TYPE_BOX,
	);
	
	/* map: MIPMAP_TYPE_ enum -> string
	 */
	int mipmap_type_to_name[] = (
		MIPMAP_TYPE_COMBINE	: "combine",
		MIPMAP_TYPE_POINT	: "point",
		MIPMAP_TYPE_BOX		: "box",
	);
	
	/* map: string -> IMAGE_FORMAT_ enum
	 */
	int name_to_image_format[] = (
		"rgba8"	: IMAGE_FORMAT_RGBA8,
		"rgb8"	: IMAGE_FORMAT_RGB8,
		"dxt1"	: IMAGE_FORMAT_DXT1,
		"dxt3"	: IMAGE_FORMAT_DXT3,
		"dxt5"	: IMAGE_FORMAT_DXT5,
		"rg16"	: IMAGE_FORMAT_RG16,
		"rg8"	: IMAGE_FORMAT_RG8,
		"r8"	: IMAGE_FORMAT_R8,
		"ati1"	: IMAGE_FORMAT_ATI1,
		"ati2"	: IMAGE_FORMAT_ATI2,
	);
	
	/* map: IMAGE_FORMAT_ enum -> string
	 */
	int image_format_to_name[] = (
		IMAGE_FORMAT_RGBA8	: "rgba8",
		IMAGE_FORMAT_RGB8	: "rgb8",
		IMAGE_FORMAT_DXT1	: "dxt1",
		IMAGE_FORMAT_DXT3	: "dxt3",
		IMAGE_FORMAT_DXT5	: "dxt5",
		IMAGE_FORMAT_RG16	: "rg16",
		IMAGE_FORMAT_RG8	: "rg8",
		IMAGE_FORMAT_R8		: "r8",
		IMAGE_FORMAT_ATI1	: "ati1",
		IMAGE_FORMAT_ATI2	: "ati2",
	);
	
	/* image format info
	 */
	class ImageInfo {
		
		int mipmap_type;
		float gamma;
		int image_format;
		int invert_g;
		
		ImageInfo() {}
		
		ImageInfo(int mipmap_type_,float gamma_,int image_format_,int invert_g_) {
			mipmap_type = mipmap_type_;
			gamma = gamma_;
			image_format = image_format_;
			invert_g = invert_g_;
		}
		
		void clone(ImageInfo instance) {
			instance.mipmap_type = mipmap_type;
			instance.gamma = gamma;
			instance.image_format = image_format;
			instance.invert_g = invert_g;
		}
	};
	
	/* postfix to ImageInfo mapping
	 */
	int postfix_to_info[] = (
		"d"		: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT1,	0),
		"dm"	: new ImageInfo(MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_DXT1,	0),
		"dr"	: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_RGBA8,	0),
		"d1"	: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT1,	0),
		"d3"	: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT3,	0),
		"d5"	: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT5,	0),
		"s"		: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT1,	0),
		"sm"	: new ImageInfo(MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_DXT1,	0),
		"sr"	: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_RGBA8,	0),
		"s1"	: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT1,	0),
		"s3"	: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT3,	0),
		"s5"	: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT5,	0),
		"n"		: new ImageInfo(MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_ATI2,	0),
		"nm"	: new ImageInfo(MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_ATI2,	0),
		"nr"	: new ImageInfo(MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_RG8,	0),
		"p"		: new ImageInfo(MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_ATI1,	0),
		"pm"	: new ImageInfo(MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_ATI1,	0),
		"pr"	: new ImageInfo(MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_R8,	0),
		"h"		: new ImageInfo(MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_ATI1,	0),
		"hm"	: new ImageInfo(MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_ATI1,	0),
		"hr"	: new ImageInfo(MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_R8,	0),
		"t"		: new ImageInfo(MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_ATI1,	0),
		"tm"	: new ImageInfo(MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_ATI1,	0),
		"tr"	: new ImageInfo(MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_R8,	0),
		"a"		: new ImageInfo(MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_ATI1,	0),
		"am"	: new ImageInfo(MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_ATI1,	0),
		"ar"	: new ImageInfo(MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_R8,	0),
		"e"		: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT1,	0),
		"em"	: new ImageInfo(MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_DXT1,	0),
		"er"	: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_RGBA8,	0),
		"l"		: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT1,	0),
		"lm"	: new ImageInfo(MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_DXT1,	0),
		"lr"	: new ImageInfo(MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_RGBA8,	0),
		"m"		: new ImageInfo(MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_DXT1,	0),
		"mm"	: new ImageInfo(MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_DXT1,	0),
		"mr"	: new ImageInfo(MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_RGBA8,	0),
	);
	
	int save_image(Image image,string path,ImageInfo info) {
		
		int pre_image_format = info.image_format;
		
		if(pre_image_format == IMAGE_FORMAT_DXT3 || pre_image_format == IMAGE_FORMAT_DXT5)	pre_image_format = IMAGE_FORMAT_RGBA8;
		else if(pre_image_format == IMAGE_FORMAT_DXT1)										pre_image_format = IMAGE_FORMAT_RGB8;
		else if(pre_image_format == IMAGE_FORMAT_ATI2)										pre_image_format = IMAGE_FORMAT_RG8;
		else if(pre_image_format  == IMAGE_FORMAT_ATI1)										pre_image_format = IMAGE_FORMAT_R8;
		
		if(!image.convertToFormat(pre_image_format)) {
			log.warning("Unable to perform mipmap pre-conditioning\n");
			return 0;
		}
		
		// do invert y channel value for normal maps (optional step)
		if(info.invert_g != 0) {
			Image dummy = new Image();
			dummy.copy(image);
			
			dummy.set(0,0);
			image.set(1,255);
			
			image.blend(dummy,0,0,0,0,image.getWidth(),image.getHeight(),-1.0f);
			delete dummy;
		}
		
		// mipmap creation and combining
		if(info.mipmap_type == MIPMAP_TYPE_COMBINE) {
			if(!image.combineMipmaps()) {
				log.warning("Unable to combine mipmaps\n");
				return 0;
			}
		} else {
			int filter_type = (info.mipmap_type == MIPMAP_TYPE_POINT) ? IMAGE_FILTER_POINT : IMAGE_FILTER_LINEAR;
			if(!image.createMipmaps(filter_type,info.gamma)) {
				log.warning("Unable to create mipmaps\n");
				return 0;
			}
		}
		
		if(info.image_format != pre_image_format) {
			if(!image.compress(info.image_format)) {
				log.warning("Compression failed\n");
				return 0;
			}
		}
		
		if(!image.save(path)) {
			log.warning("Unable to save image\n");
			return 0;
		}
		
		return 1;
	}
	
	ImageInfo get_info_from_postfix(string name,Image image = NULL) {
		string res[0];
		string postfix = "";
		
		re_search(name,".+/.+_([a-z,0-9]+)\\..+$",res);
		
		if(res.size() == 1 && postfix_to_info.check(res[0])) {
			postfix = res[0];
		}
		
		ImageInfo info = postfix_to_info.check(postfix,NULL);
		if(info == NULL) return NULL;
		
		ImageInfo ret_info = new ImageInfo();
		info.clone(ret_info);
		
		if(postfix != "d" && postfix != "s") return ret_info;
		
		if(image == NULL) {
			image = class_manage(new Image());
			if(image.info(name) == 0) {
				if(image.load(name) == 0) return ret_info;
			}
		}
		
		if(image.getNumChannels() == 4) ret_info.image_format = IMAGE_FORMAT_DXT5;
		
		return ret_info;
	}
	
	/*
	 */
	int process(Asset asset,AssetRuntime runtime,string source,string destination) {
		
		Image image = new Image(source);
		if(!image.isLoaded()) return 0;
		
		int ret;
		if(asset.parameters.size()) {
			ImageInfo info = new ImageInfo(
				name_to_mipmap_type[asset.getImportParameterString("mipmap_type","box")],
				asset.getImportParameterFloat("gamma",1.0f),
				name_to_image_format[asset.getImportParameterString("image_format",IMAGE_FORMAT_DXT5)],
				asset.getImportParameterInt("invert_g",0)
			);
			
			ret = save_image(image,destination,info);
			delete info;
		} else {
			ret = image.save(destination);
		}
		
		delete image;
		return ret;
	}
	
	/*
	 */
	int import(Asset asset,string source) {
		if(!is_file(source)) return 0;
		
		// parameters
		asset.clearImportParameters();
		
		ImageInfo info = get_info_from_postfix(source);
		if(info != NULL) {
			asset.addImportParameter("mipmap_type","string",mipmap_type_to_name[info.mipmap_type]);
			asset.addImportParameter("gamma","float",info.gamma);
			asset.addImportParameter("image_format","string",image_format_to_name[info.image_format]);
			asset.addImportParameter("invert_g","int",info.invert_g);
			
			delete info;
		}
		
		// runtimes
		string name = extension(basename(source),"dds");
		asset.addRuntime(name,"Assets::Textures::process");
		
		return 1;
	}
}

#endif /* __EDITOR_ASSETS_TEXTURES_H__ */
