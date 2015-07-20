/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_textures.h
 * Desc:    Unigine editor
 * Version: 1.06
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
 *          Pavel Nakaznenko <crio@unigine.com>
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
namespace Textures {
	
	/* texture meta info descriptor class
	 */
	class MetaDescriptor {
		
		string image_type;
		int mipmap_type;
		float gamma;
		int image_format;
		int invert_g;
		
		string getImageType() { return image_type; }
		int getMipmapType() { return mipmap_type; }
		float getGamma() { return gamma; }
		int getImageFormat() { return image_format; }
		int getInvertG() { return invert_g; }
		
		MetaDescriptor(string image_type_,int mipmap_type_,float gamma_,int image_format_,int invert_g_) {
			image_type = image_type_;
			mipmap_type = mipmap_type_;
			gamma = gamma_;
			image_format = image_format_;
			invert_g = invert_g_;
		}
		
		int isEqual(MetaDescriptor md2) {
			if(md2 == NULL) return false;
			return (image_type == md2.image_type &&
				mipmap_type == md2.mipmap_type &&
				image_format == md2.image_format &&
				invert_g == md2.invert_g &&
				gamma == md2.gamma);
		}
	};
	
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
	
	/* maps image type to pre-mipmap conditioning format
	 */
	int mipmap_precondition_formats[] = (
		"diffuse"		: IMAGE_FORMAT_RGBA8,
		"specular"		: IMAGE_FORMAT_RGBA8,
		"emission"		: IMAGE_FORMAT_RGBA8,
		"lightmap"		: IMAGE_FORMAT_RGBA8,
		"mask"			: IMAGE_FORMAT_RGBA8,
		"normal"		: IMAGE_FORMAT_RG8,
		"parallax"		: IMAGE_FORMAT_R8,
		"height"		: IMAGE_FORMAT_R8,
		"tessellation"	: IMAGE_FORMAT_R8,
		"ambient"		: IMAGE_FORMAT_R8,
	);
	
	/* maps image to a list of supported conversion formats
	 */
	string image_type_to_formats[] = (
		"diffuse"		: "rgb8,rgba8,dxt1,dxt3,dxt5",
		"specular"		: "rgb8,rgba8,dxt1,dxt3,dxt5",
		"normal"		: "rg8,ati2",
		"parallax"		: "r8,ati1",
		"height"		: "r8,ati1",
		"tessellation"	: "r8,ati1",
		"ambient"		: "r8,ati1",
		"emission"		: "rgb8,rgba8,dxt1,dxt3,dxt5",
		"lightmap"		: "rgb8,dxt1",
		"mask" 			: "r8,rg8,rgb8,rgba8,ati1,ati2,dxt1,dxt5",
		
	);
	
	/* postfix to conversion information mapping
	 * MetaDescriptors are initialized as (image type, mip type, gamma, destination format, invert y channel flag)
	 */
	int postfix_to_info[] = (
		"d"		: new MetaDescriptor("diffuse",			MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT1,	0),
		"dm"	: new MetaDescriptor("diffuse",			MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_DXT1,	0),
		"dr"	: new MetaDescriptor("diffuse",			MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_RGBA8,	0),
		"d1"	: new MetaDescriptor("diffuse",			MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT1,	0),
		"d3"	: new MetaDescriptor("diffuse",			MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT3,	0),
		"d5"	: new MetaDescriptor("diffuse",			MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT5,	0),
		"s"		: new MetaDescriptor("specular",		MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT1,	0),
		"sm"	: new MetaDescriptor("specular",		MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_DXT1,	0),
		"sr"	: new MetaDescriptor("specular",		MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_RGBA8,	0),
		"s1"	: new MetaDescriptor("specular",		MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT1,	0),
		"s3"	: new MetaDescriptor("specular",		MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT3,	0),
		"s5"	: new MetaDescriptor("specular",		MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT5,	0),
		"n"		: new MetaDescriptor("normal",			MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_ATI2,	0),
		"nm"	: new MetaDescriptor("normal",			MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_ATI2,	0),
		"nr"	: new MetaDescriptor("normal",			MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_RG8,	0),
		"p"		: new MetaDescriptor("parallax",		MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_ATI1,	0),
		"pm"	: new MetaDescriptor("parallax",		MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_ATI1,	0),
		"pr"	: new MetaDescriptor("parallax",		MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_R8,	0),
		"h"		: new MetaDescriptor("height",			MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_ATI1,	0),
		"hm"	: new MetaDescriptor("height",			MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_ATI1,	0),
		"hr"	: new MetaDescriptor("height",			MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_R8,	0),
		"t"		: new MetaDescriptor("tessellation",	MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_ATI1,	0),
		"tm"	: new MetaDescriptor("tessellation",	MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_ATI1,	0),
		"tr"	: new MetaDescriptor("tessellation",	MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_R8,	0),
		"a"		: new MetaDescriptor("ambient",			MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_ATI1,	0),
		"am"	: new MetaDescriptor("ambient",			MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_ATI1,	0),
		"ar"	: new MetaDescriptor("ambient",			MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_R8,	0),
		"e"		: new MetaDescriptor("emission",		MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT1,	0),
		"em"	: new MetaDescriptor("emission",		MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_DXT1,	0),
		"er"	: new MetaDescriptor("emission",		MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_RGBA8,	0),
		"l"		: new MetaDescriptor("lightmap",		MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_DXT1,	0),
		"lm"	: new MetaDescriptor("lightmap",		MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_DXT1,	0),
		"lr"	: new MetaDescriptor("lightmap",		MIPMAP_TYPE_BOX,		2.2f,	IMAGE_FORMAT_RGBA8,	0),
		"m"		: new MetaDescriptor("mask",			MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_DXT1,	0),
		"mm"	: new MetaDescriptor("mask",			MIPMAP_TYPE_COMBINE,	1.0f,	IMAGE_FORMAT_DXT1,	0),
		"mr"	: new MetaDescriptor("mask",			MIPMAP_TYPE_BOX,		1.0f,	IMAGE_FORMAT_RGBA8,	0),
	);
	
	
	/* Returns conversion info associated with specified postfix or -1, if none found
	 */
	int get_info_by_postfix(string postfix) {
		return postfix_to_info.check(postfix,NULL);
	}
	
	/* Returns string with name of the image format associated with specified IMAGE_FORMAT_ enum
	 * or -1, if none found
	 */
	int getNameByImageFormat(int format) {
		return image_format_to_name.check(format,-1);
	}
	
	/* Returns IMAGE_FORMAT_ enum associated with specified name
	 * or -1, if none found
	 */
	int getImageFormatByName(string name) {
		return name_to_image_format.check(name,-1);
	}
	
	/* Returns string with name of the mimap type specified as an argument
	 * or -1, if none found
	 */
	int getNameByMipmapType(int mipmap_type) {
		return mipmap_type_to_name.check(mipmap_type,-1);
	}
	
	/* Returns mipmap type associated with the specified name
	 * or -1, if none found
	 */
	int getMipmapTypeByName(string name) {
		return name_to_mipmap_type.check(name,-1);
	}
	
	/* Returns pre-mipmap conditioning format associated with the specified image type
	 * or -1, if none found
	 */
	int get_conditioning_format_by_image_type(string type) {
		return mipmap_precondition_formats.check(type,-1);
	}
	
	/* Returns string with comma separated enumeration of conversion formats available for specified image type
	 * or NULL, if none found
	 */
	string getConversionFormatByImageType(string type) {
		return image_type_to_formats.check(type,NULL);
	}
	
	/**************************************************************************\
	*
	* Images
	*
	\**************************************************************************/
	
	/* saves image using information stored in Quarduple argument
	 */
	void save_image_using_info(Async async,Image image,string name,MetaDescriptor info) {
		int ret = 1;
		
		int mipmap_type = info.mipmap_type;
		float gamma = info.gamma;
		int output_image_format = info.image_format;
		
		int pre_image_format = output_image_format;
		
		if(pre_image_format == IMAGE_FORMAT_DXT3 || pre_image_format == IMAGE_FORMAT_DXT5) {
			pre_image_format = IMAGE_FORMAT_RGBA8;
		} else if(pre_image_format == IMAGE_FORMAT_DXT1) {
			pre_image_format = IMAGE_FORMAT_RGB8;
		} else if(pre_image_format == IMAGE_FORMAT_ATI2) {
			pre_image_format = IMAGE_FORMAT_RG8;
		} else if(pre_image_format  == IMAGE_FORMAT_ATI1) {
			pre_image_format = IMAGE_FORMAT_R8;
		}
		
		ret = image.convertToFormat(pre_image_format);
		
		if(!ret) {
			dialogMessageOk(TR("Error"),TR("Unable to perform mipmap pre-conditioning"));
			delete image;
			return 0;
		}
		
		// do invert y channel value for normal maps (optional step)
		if(info.invert_g != 0) {
			// to inverse G channel we do the following:
			// copy "image" to "dummy"
			// set "image" channels as R=R, G=1
			// set "dummy" channels as R=0, G=G
			// subtract "dummy" from "image" by blending with -1.0f scale:
			// result = image - dummy = (R-0, 1-G) = (R,1-G)
			// NOTE: green channel is subtracted from 1 because normals are being stored biased
			
			Image dummy = new Image();
			dummy.copy(image);
			
			dummy.set(0,0);
			image.set(1,255);
			
			image.blend(dummy,0,0,0,0,image.getWidth(),image.getHeight(),-1.0f);
			delete dummy;
		}
		
		// mipmap creation and combining
		if(mipmap_type == MIPMAP_TYPE_COMBINE) {
			ret = image.combineMipmaps();
			if(!ret) {
				dialogMessageOk(TR("Error"),TR("Unable to combine mipmaps"));
				delete image;
				return 0;
			}
		}
		else {
			int filter_type = (mipmap_type == MIPMAP_TYPE_POINT) ? IMAGE_FILTER_POINT : IMAGE_FILTER_LINEAR;
			ret = image.createMipmaps(filter_type,gamma);
			if(!ret) {
				dialogMessageOk(TR("Error"),TR("Unable to create mipmaps"));
				delete image;
				return 0;
			}
		}
		
		if(output_image_format != pre_image_format) {
			
			if(asyncisNeedStop()) return;
			image.save(name);
			
			// compress image
			panelStatusBarInfoStart("Compressing texture " + name);
			int id = async.run(image,"compress",output_image_format);
			while(async.isRunning(id)) wait;
			ret = async.getResult(id);
			if(asyncisNeedStop() == 0) panelStatusBarInfoEnd("Compressing texture " + name);
		}
		
		if(asyncisNeedStop()) return;
		
		// save image
		if(ret) ret = image.save(name);
		if(ret) log.message("Saving \"%s\" texture\n",name);
		
		// error message
		if(ret == 0) dialogMessageOk(TR("Error"),TR("Unable to save image"));
		
		// delete image
		delete image;
	}
	
	void saveImageUsingInfo(Image image,string name,MetaDescriptor info) {
		asyncSetNeedStop("update_texture_" + name);
		panelStatusBarInfoEnd("Compressing texture " + name);
		asyncRunNamedThread(functionid(save_image_using_info),(image,name,info),"update_texture_" + name);
	}
	
	/* saves specified info into .meta file
	 */
	void saveMetaUsingInfo(MetaDescriptor info,string meta_file_path) {
		Xml meta_file_xml = new Xml("texture_meta","version=\"1.00\"");
		
		string image_type = info.image_type;
		string image_mipmap = getNameByMipmapType(info.mipmap_type);
		float image_gamma = info.gamma;
		string image_format = getNameByImageFormat(info.image_format);
		int invert_g = info.invert_g;
		
		Xml type_node = meta_file_xml.addChild("type");
		type_node.setData(image_type);
		
		Xml format_node = meta_file_xml.addChild("format");
		format_node.setData(image_format);
		
		Xml mipmap_node = meta_file_xml.addChild("mipmap");
		mipmap_node.setData(image_mipmap);
		
		Xml gamma_node = meta_file_xml.addChild("gamma");
		gamma_node.setData(image_gamma);
		
		Xml options_node = meta_file_xml.addChild("options");
		options_node.setArg("invert_g",invert_g);
		
		// save .meta
		if(meta_file_xml.save(meta_file_path) == 0) {
			log.error("Unable to save meta file %s \n",meta_file_path);
		}
		delete meta_file_xml;
	}
	
	/* gets image info using information encoded in filename postfix
	 */
	MetaDescriptor get_meta_descriptor_from_postfix(string name,Image image = NULL) {
		string res[0];
		string postfix = "";
		
		re_search(name,".+/.+_([a-z,0-9]+)\\..+$",res);
		
		if(res.size() == 1 && postfix_to_info.check(res[0])) {
			postfix = lower(res[0]);
		}
		
		MetaDescriptor info = get_info_by_postfix(postfix);
		if(info == NULL) return NULL;
		if(postfix != "d" && postfix != "s") return info;
		
		if(image == NULL) {
			image = class_manage(new Image());
			if(image.info(savePath(name)) == 0) {
				if(image.load(savePath(name)) == 0) return info;
			}
		}
		
		if(image.getNumChannels() == 4) info.image_format = IMAGE_FORMAT_DXT5;
		
		return info;
	}
	
	/* saves image using information encoded in filename postfix
	 */
	void save_image_using_postfix(Image image,string name,string meta_file_path,string type,int silent) {
		string res[0];
		string lower_type;
		re_search(lower(type),"([a-z]*)_?([0-9]*)$",res);
		
		if(res.size() > 0) {
			lower_type = res[0];
		} else {
			log.error("Unable to parse image type %s\n",type);
		}
		
		MetaDescriptor info = get_meta_descriptor_from_postfix(name,image);
		assert(info != NULL && "Textures::save_image_using_postfix(): meta is NULL");
		
		if(info.image_type != lower_type && !silent) {
			if(!(lower_type == "diffuse" && info.image_type == "specular") && !(lower_type == "specular" && info.image_type == "diffuse")) {	//edge case
				dialogMessageOk(TR("Warning"),format(TR("The assigned file is a %s texture, according to its meta info.\nYou might want to use proper %s texture instead."),info.image_type,lower_type));
			}
		}
		
		saveImageUsingInfo(image,name,info);
		saveMetaUsingInfo(info,meta_file_path);
	}
	
	/* gets image info using information encoded .meta
	 */
	MetaDescriptor get_meta_descriptor_from_meta(string meta_file_path,int silent = 0) {
		Xml meta_file_xml;
		meta_file_xml = new Xml();
		
		File file = new File();
		
		// look for .meta file
		if(file.open(meta_file_path,"rb")) {
			file.close();
			delete file;
			
			if(meta_file_xml.load(meta_file_path) == 0) {
				delete meta_file_xml;
				if(!silent) dialogMessageOk(TR("Error"),TR("Unable to parse ") + meta_file_path);
				return NULL;
			}
		}
		else {
			delete file;
			delete meta_file_xml;
			if(!silent) dialogMessageOk(TR("Error"),TR("Unable to open ") + meta_file_path);
			return NULL;
		}
		
		// .meta file was found, process it
		int mip_type = MIPMAP_TYPE_BOX;
		string image_type = "default";
		string dest_format = "default";
		string mipmap_mode = "box";
		float gamma = 1.0f;
		int invert_g = 0;
		
		image_type = lower(meta_file_xml.getChildData("type",image_type));
		dest_format = lower(meta_file_xml.getChildData("format",dest_format));
		mipmap_mode = lower(meta_file_xml.getChildData("mipmap",mipmap_mode));
		gamma = meta_file_xml.getChildData("gamma",gamma);
		Xml options_node = meta_file_xml.getChild("options");
		if(options_node != NULL) {
			invert_g = options_node.getArg("invert_g",invert_g);
		}
		
		delete meta_file_xml;
		
		mip_type = getMipmapTypeByName(mipmap_mode);
		int dest_image_format = getImageFormatByName(dest_format);
		if(dest_image_format < 0) {
			dialogMessageOk(TR("Error"),TR("Unknown destination_format: ") + dest_format);
			if(!silent) return NULL;
		}
		
		MetaDescriptor new_info = new MetaDescriptor(image_type,mip_type,gamma,dest_image_format,invert_g);
		return new_info;
	}
	
	/* saves image using information encoded in .meta
	 */
	void save_image_using_meta(Image image,string name,string meta_file_path,string type,int silent) {
		MetaDescriptor new_info = get_meta_descriptor_from_meta(meta_file_path);
		string res[0];
		string lower_type;
		re_search(lower(type),"([a-z]*)_?([0-9]*)$",res);
		
		if(res.size() > 0) {
			lower_type = res[0];
		} else {
			log.error("Unable to parse image type %s\n",type);
		}
		
		if(new_info.image_type != lower_type && !silent) {
			if(!(lower_type == "diffuse" && new_info.image_type == "specular") && !(lower_type == "specular" && new_info.image_type == "diffuse")) {	//edge case
				dialogMessageOk(TR("Warning"),format(TR("The assigned file is a %s texture, according to its meta info.\nYou might want to use proper %s texture instead."),new_info.image_type,lower_type));
			}
		}
		if(new_info != NULL) {
			saveImageUsingInfo(image,name,new_info);
		}
	}
	
	/* returns .meta filename based on passed image name
	 */
	string getMetaFilename(string name) {
		string directory_and_filename[0];
		string meta_file_name = NULL;
		
		re_search(name,"(.+)/(.+)\.[a-z,0-9]+$",directory_and_filename); // get path and filename (without ext)
		
		if(directory_and_filename.size() == 2) {
			string image_name_tokens[0];
			re_search(directory_and_filename[1],"(.+)_([a-z,0-9]+)$",image_name_tokens);
			
			if(image_name_tokens.size() == 2) {
				string image_postfix_first_letter = substr(image_name_tokens[1],0,1);
				meta_file_name = format(TR("%s/uncompressed/%s_%s.meta"),directory_and_filename[0],image_name_tokens[0],image_postfix_first_letter);
			}
		}
		return meta_file_name;
	}
	
	string getUncompressedImageFilename(string name) {
		string directory_and_filename[0];
		
		re_search(name,"(.+)/(.+)\.[a-z,0-9]+$",directory_and_filename); // get path and filename (without ext)
		if(directory_and_filename.size() == 2) {
			string uncompressed_path = directory_and_filename[0] + "/uncompressed/";
			string full_path = savePath(uncompressed_path);
			if(!is_dir(full_path)) {
				return NULL;
			}
			// source extensions
			string extensions[] = ( "png", "psd", "tga", "dds", "jpg", "tif", "ppm", "psd", "rgb", "rgba", "pgm", "hdr", "sgi" );
			
			full_path = full_path + directory_and_filename[1] + ".";
			
			// try to find source file
			foreach(string extension; extensions) {
				if(is_file(full_path + extension)) {
					return full_path + extension;
				}
			}
		}
		return NULL;
	}
	
	/* returns MetaDescriptor with image info, loaded from .meta or postfix
	 * returns NULL if failed
	 */
	MetaDescriptor loadImageInfo(string name) {
		string meta_file_path = getMetaFilename(name);
		MetaDescriptor info = get_meta_descriptor_from_meta(meta_file_path,1);
		if(info == NULL) {
			info = get_meta_descriptor_from_postfix(name);
			if(info != NULL) {
				saveMetaUsingInfo(info,meta_file_path);
			}
		}
		return info;
	}
	
	/*
	 */
	int dialog_image(string title,string type,string &name,int allow_float = true) {
		
		int dialog_result;
		
		if(allow_float) dialog_result = dialogFileImage(title,name,DIALOG_FILE_OPEN);
		else dialog_result = dialogFileImageNoHDR(title,name,DIALOG_FILE_OPEN);
		
		if(!dialog_result) return 0;
		
		name = savePath(name);
		
		Image image = new Image();
		
		if(!allow_float) {
			if(!image.info(name) && !image.load(name)) {
				dialogMessageOk(TR("Error"),format(TR("Failed to get image info: %s"),name));
				delete image;
				return 0;
			}
			
			if(image.getType() != IMAGE_2D) {
				dialogMessageOk(TR("Error"),format(TR("Unsupported texture type (%s). Only 2D textures with unsigned texture format can be used."),image.getTypeName()));
				delete image;
				return 0;
			}
			
			if(image.isFloatFormat() || image.isHalfFormat()) {
				dialogMessageOk(TR("Error"),format(TR("Unsupported texture format (%s). Only 2D textures with unsigned texture format can be used."),image.getFormatName()));
				delete image;
				return 0;
			}
		}
		
		// check file name
		string res[0];
		re_search(name,"(.+)/(.+)/(.+)\\.([a-z,0-9]+)$",res);
		if(res.size() != 4 || lower(res[1]) != "uncompressed") {
			name = editorPath(name);
			return 1;
		}
		
		string extension = lower(res[3]);
		
		// load source image
		if(!image.isLoaded() && !image.load(name)) {
			dialogMessageOk(TR("Error"),format(TR("Failed to load image: %s"),name));
			delete image;
			return 0;
		}
		
		string image_name_tokens[0];
		string meta_file_name;
		
		re_search(res[2],"(.+)_([a-z,0-9]+)$",image_name_tokens);
		
		if(image_name_tokens.size() == 2) {
			string image_postfix_first_letter = substr(image_name_tokens[1],0,1);
			meta_file_name = format("%s/%s/%s_%s.meta",res[0],res[1],image_name_tokens[0],image_postfix_first_letter);
		}
		else {
			dialogMessageOk(TR("Error"),format(TR("Incorrect name format for '%s', postfix is missing"),name));
			log.error("Textures::dialog_image(): postfix is missing for '%s'\n",name);
			
			delete image;
			return 0;
		}
		
		MetaDescriptor info = get_meta_descriptor_from_postfix(name,image);
		if(info == NULL) {
			dialogMessageOk(TR("Error"),format(TR("Incorrect postfix '%s' for '%s'"),image_name_tokens[1],name));
			log.error("Textures::dialog_image(): incorrect postfix '%s' for '%s'\n",image_name_tokens[1],name);
			
			delete image;
			return 0;
		}
		
		// compressed file name
		name = res[0] + "/" + res[2] + ".dds";
		
		// check file
		File file = new File();
		if(file.open(engine.filesystem.getFileName(name),"rb") && dialogMessageYesNo(TR("Confirm"),format(TR("File \"%s\" already exists. Override?"),name)) == 0) {
			file.close();
			delete file;
			delete image;
			return 0;
		}
		if(file.isOpened()) file.close();
		delete file;
		
		log.message("Creating \"%s\" texture\n",name);
		
		// copy image
		if(extension == "dds") {
			if(image.save(name) == 0) {
				dialogMessageOk(TR("Error"));
				delete image;
				return 0;
			}
			delete image;
		}
		
		// convert image
		else {
			
			int id;
			if(is_file(engine.filesystem.getFileName(meta_file_name)) == 0) {
				id = thread(functionid(save_image_using_postfix),image,name,meta_file_name,type,0);
			}
			else {
				id = thread(functionid(save_image_using_meta),image,name,meta_file_name,type,0);
			}
			
			//
			while(is_thread(id)) wait;
		}
		
		name = editorPath(name);
		
		return 1;
	}
	
	/*
	 */
	void check_image(string name) {
		
		// check file name
		if(is_file(engine.filesystem.getFileName(name)) == 0) return;
		
		// check loaded material libraries
		forloop(int i = 0; engine.materials.getNumLibraries()) {
			forloop(int j = 0; engine.materials.getNumMaterials(i)) {
				Material material = engine.materials.findMaterial(engine.materials.getMaterialName(i,j));
				forloop(int k = 0; material.getNumTextures()) {
					if(material.getTextureType(k) == MATERIAL_TEXTURE_IMAGE) {
						if(material.getImageTextureName(k) == name) return;
					}
				}
			}
		}
		
		// remove file
		if(dialogMessageYesNo(TR("Confirm"),format(TR("File \"%s\" is not referenced by material. Remove?"),name))) {
			remove(engine.getDataPath() + name);
		}
	}
	
	/**************************************************************************\
	*
	* Auto reloading
	*
	\**************************************************************************/
	
	/*
	 */
	class Texture {
		
		string source;			// source image
		string ext;				// source extension
		string type;			// texture type
		string name;			// texture image
		int thread_id;			// texture thread
		int mtime_source;		// texture mtime
		
		string source_path;
		string name_path;
		
		Texture(string source_,string ext_,string type_,string name_) {
			source = source_;
			ext = ext_;
			type = type_;
			name = name_;
			thread_id = -1;
			
			source_path = savePath(source);
			name_path = savePath(extension(name,"dds"));
			
			mtime_source = get_mtime(source_path);
		}
		
		~Texture() {
			if(thread_id != -1 && is_thread(thread_id)) kill_thread(thread_id);
		}
		
		int update() {
			
			if(is_file(source_path) == 0) return 0;
			if(thread_id != -1 && is_thread(thread_id)) return 1;
			
			int mtime = get_mtime(source_path);
			if(mtime == -1) return 1;
			
			if(mtime == mtime_source) return 1;
			
			Image image = class_manage(new Image());
			if(image.load(source) == 0) return 1;
			
			mtime_source = mtime;
			
			log.message("Update \"%s\" texture\n",name_path);
			
			string meta_file_name = getMetaFilename(name_path);
			if(is_file(engine.filesystem.getFileName(meta_file_name)) == 0) {
				thread_id = thread(functionid(save_image_using_postfix),image,name_path,meta_file_name,type,1);
			}
			else {
				MetaDescriptor info = get_meta_descriptor_from_meta(meta_file_name);
				if(info != NULL) {
					string res[0];
					re_search(name_path,".+/.+_([a-z,0-9]+)\\..+$",res);
					
					if(res.size() == 1 && (res[0] == "d" || res[0] == "s")) {
						int format = info.image_format;
						if(image.getNumChannels() == 4) {
							if(format == IMAGE_FORMAT_RGB8) format = IMAGE_FORMAT_RGBA8;
							if(format == IMAGE_FORMAT_DXT1) format = IMAGE_FORMAT_DXT5;
						}
						else if(image.getNumChannels() == 3) {
							if(format == IMAGE_FORMAT_RGBA8) format = IMAGE_FORMAT_RGB8;
							if(format == IMAGE_FORMAT_DXT3 || format == IMAGE_FORMAT_DXT5) format = IMAGE_FORMAT_DXT1;
						}
						
						if(info.image_format != format) {
							info.image_format = format;
							saveMetaUsingInfo(info,meta_file_name);
						}
					}
				}
				
				thread_id = thread(functionid(save_image_using_meta),image,name_path,meta_file_name,type,1);
			}
			
			return 1;
		}
		
	};
	
	/*
	 */
	class TerrainTextureArray {
		string name;
		Unigine::Vector textures_name;
		Unigine::Vector textures_mtime;
		int size;
		int old_size;
		string type;
		
		
		//
		int is_changed() {
			if(old_size != size) return 1;
			
			forloop(int i = 0; textures_name.size()) {
				string name = textures_name[i];
				if(is_file(name) == 0) continue;
				if(textures_mtime[i] != get_mtime(name)) return 1;
			}
			
			return 0;
		}
		
		//
		TerrainTextureArray(string name_, string type_) {
			textures_name = new Unigine::Vector();
			textures_mtime = new Unigine::Vector();
			size = -1;
			old_size = -1;
			name = name_;
			type = type_;
		}
		
		//
		~TerrainTextureArray() {
			asyncStopThreads("update_texture_array_" + name);
			delete textures_mtime;
			delete textures_name;
		}
		
		//
		int update() {
			if(is_file(name) == 0) return 0;
			if(textures_name.size() == 0) return 1;
			
			if(is_changed()) {
				if(asyncIsRunning("update_texture_array_" + name)) {
					asyncSetNeedStop("update_texture_array_" + name);
					return 1;
				}
				asyncRunNamedThread(functionid(create),(textures_name,size,name,type),"update_texture_array_" + name);
				update_mtime();
				old_size = size;
			}
			
			return 1;
		}
		
		//
		void update_mtime() {
			forloop(int i = 0; textures_name.size()) {
				string name  = textures_name[i];
				if(is_file(name )) textures_mtime[i] = get_mtime(name);
			}
		}
		
		int create(Async async,Unigine::Vector names,int size,string name,string type) {
			
			log.message("Update texture array: %s \n",name);
			
			name = savePath(name);
			
			if(is_file(name) == 0) return;
			panelStatusBarInfoStart("Update texture array " + name);
			
			Image array = class_manage(new Image());
			
			if(size == -1) {
				array.info(name);
				size = array.getHeight();
			}
			
			// get texture array info
			int num_materials = names.size();
			int num_channels = -1;
			int format = -1;
			int mipmaps = 1;
			
			int s = size;
			while(s > 1) {
				if(s > 1) s >>= 1;
				mipmaps++;
			}
			
			Image layer = class_manage(new Image());
			
			int id = -1;
			
			if(type == "normal") {
				format = IMAGE_FORMAT_ATI2;
				array.create2DArray(size,size,num_materials,format,mipmaps);
			} else {
				forloop(int i = 0; names.size()) {
					string image_name = names[i];
					if(strlen(image_name) == 0) continue;
					
					if(layer.info(image_name) == 0) continue;
					if(layer.getType() != IMAGE_2D) {
						log.error("getTextureArrayInfo(): wrong %s texture type %s\n",image_name,layer.getTypeName());
						continue;
					}
					
					num_channels = max(num_channels,layer.getNumChannels());
				}
				
				format = IMAGE_FORMAT_DXT5;
				if(num_channels == 1) format = IMAGE_FORMAT_ATI1;
				else if(num_channels == 2) format = IMAGE_FORMAT_ATI2;
				
				array.create2DArray(size,size,num_materials,format,mipmaps);
			}
			
			
			forloop(int i = 0; names.size()) {
				if(asyncisNeedStop()) {
					panelStatusBarInfoEnd("Update texture array " + name);
					return;
				}
				
				string image_name = savePath(names[i]);
				if(is_file(image_name) == 0) continue;
				
				//load
				id = async.run(layer,"load",image_name);
				while(async.isRunning(id)) wait;
				if(async.getResult(id) == 0) continue;
				
				if(size != layer.getHeight()) {
					// resize
					
					if(layer.isCompressedFormat() && size > layer.getHeight()) {
						id = async.run(layer,"decompress");
						while(async.isRunning(id)) wait;
						if(async.getResult(id) == 0) continue;
					}
					
					id = async.run(layer,"resize",size,size,IMAGE_FILTER_LINEAR);
					while(async.isRunning(id)) wait;
					if(async.getResult(id) == 0) {
						log.error("size: %d getHeight: %d name: %s",size,layer.getHeight(),image_name);
						continue;
					}
				}
				
				if(layer.getNumMipmaps() != mipmaps) {
					if(layer.isCompressedFormat()) {
						id = async.run(layer,"decompress");
						while(async.isRunning(id)) wait;
						if(async.getResult(id) == 0) continue;
					}
					
					int filter_type = IMAGE_FILTER_LINEAR;
					
					string meta_file_path = savePath(getMetaFilename(names[i]));
					if(is_file(meta_file_path)) {
						MetaDescriptor info = loadImageInfo(savePath(names[i]));
						if(info != NULL) {
							filter_type = (info.mipmap_type == MIPMAP_TYPE_POINT) ? IMAGE_FILTER_POINT : IMAGE_FILTER_LINEAR;
						}
					}
					
					id = async.run(layer,"createMipmaps",filter_type,1.0f);
					while(async.isRunning(id)) wait;
					if(async.getResult(id) == 0) continue;
				}
				
				// convert
				id = async.run(layer,"convertToFormat",format);
				while(async.isRunning(id)) wait;
				if(async.getResult(id) == 0) continue;
				
				// copy
				id = async.run(array,"copy",layer,i);
				while(async.isRunning(id)) wait;
			}
			
			panelStatusBarInfoEnd("Update texture array " + name);
			
			if(asyncisNeedStop()) return;
			
			return array.save(name);
		}
	};
	
	/*
	 */
	enum {
		TEXTURES_PER_UPDATE = 5,
	};
	
	Texture textures[0];
	Texture textures_map[];
	
	TerrainTextureArray texture_arrays[0];
	TerrainTextureArray texture_arrays_map[];
	
	int arrays_counter;
	int textures_counter;
	
	/*
	 */
	void add_texture(string name,string type) {
		
		name = savePath(name);
		string source = savePath(getUncompressedImageFilename(name));
		
		// check existence
		if(textures_map.check(source)) return;
		if(is_file(source) == 0) return;
		
		textures_map.append(source,new Texture(source,extension(source),type,name));
		textures.append(textures_map[source]);
	}
	
	
	template update_array<NAME,TYPE,TYPE_STRING> void update_array_ ## NAME(ObjectTerrain terrain,int size = -1) {
		
		string name_array = savePath(terrain.get ## TYPE ## TextureArrayName());
		
		if(is_file(name_array) == 0) return;
		
		if(texture_arrays_map.check(name_array) == 0) add_terrain(terrain);
		
		TerrainTextureArray texture_array = texture_arrays_map[name_array];
		
		texture_array.textures_mtime.clear();
		texture_array.textures_name.clear();
		
		forloop(int i = 0; terrain.getNumMaterials()) {
			string path = terrain.getMaterial ## TYPE ## TextureName(i);
			path = savePath(path);
			add_texture(path,TYPE_STRING);
			
			texture_array.textures_name.append(path);
			texture_array.textures_mtime.append(-1);
		}
		
		if(size != -1) texture_array.size = size;
	}
	
	update_array<diffuse,Diffuse,"diffuse">;
	update_array<normal,Normal,"normal">;
	update_array<specular,Specular,"specular">;
	
	void add_terrain(ObjectTerrain terrain) {
		
		template add_array<TYPE,NAME,TYPE_STRING> {
			{
				string array_name = savePath(terrain.get ## TYPE ## TextureArrayName());
				
				if(is_file(array_name) && !texture_arrays_map.check(array_name)) {
					TerrainTextureArray texture_array = new TerrainTextureArray(array_name,TYPE_STRING);
					
					forloop(int i = 0; terrain.getNumMaterials()) {
						string path = terrain.getMaterial ## TYPE ## TextureName(i);
						path = savePath(path);
						add_texture(path,TYPE_STRING);
						
						texture_array.textures_name.append(path);
						texture_array.textures_mtime.append(get_mtime(path));
					}
					
					texture_arrays_map.append(array_name,texture_array);
					texture_arrays.append(texture_array);
				}
			}
		}
		
		add_array<Diffuse,diffuse,"diffuse">;
		add_array<Normal,normal,"normal">;
		add_array<Specular,specular,"mask">;
		
		if(strlen(terrain.getDiffuseTextureName())) add_texture(terrain.getDiffuseTextureName(),"diffuse");
		if(strlen(terrain.getNormalTextureName())) add_texture(terrain.getNormalTextureName(),"normal");
		if(strlen(terrain.getMaskTextureName())) add_texture(terrain.getMaskTextureName(),"mask");
	}
	
	
	/*
	 */
	void find_textures() {
		
		engine.app.stopFps();
		
		// clear textures
		textures_map.delete();
		textures.delete();
		
		arrays_counter = 0;
		
		// scan loaded material libraries
		forloop(int i = 0; engine.materials.getNumLibraries()) {
			forloop(int j = 0; engine.materials.getNumMaterials(i)) {
				Material material = engine.materials.findMaterial(engine.materials.getMaterialName(i,j));
				forloop(int k = 0; material.getNumTextures()) {
					if(material.getTextureType(k) == MATERIAL_TEXTURE_IMAGE) {
						add_texture(material.getImageTextureName(k),material.getTextureName(k));
					}
				}
			}
		}
		
		// scan loaded nodes
		forloop(int i = 0; engine.editor.getNumNodes()) {
			Node node = engine.editor.getNode(i);
			if(node.getType() == NODE_OBJECT_TERRAIN) {
				ObjectTerrain terrain = node_cast(node);
				add_terrain(terrain);
			}
		}
		
		engine.app.startFps();
	}
	
	/*
	 */
	void update_textures() {
		
		if(textures.size() == 0) return;
		
		if((textures_counter) >= textures.size()) textures_counter = 0;
		
		Texture texture = textures[textures_counter];
		if(texture.update() == 0) {
			log.message("Removing \"%s\" texture\n",texture.source);
			
			textures.remove(textures_counter);
			textures_map.remove(texture.source);
			
			delete texture;
		}
		
		textures_counter++;
	}
	
	/*
	 */
	void update_arrays() {
		
		if(texture_arrays.size() == 0) return;
		
		if(arrays_counter >= texture_arrays.size()) arrays_counter = 0;
		
		TerrainTextureArray texture_array = texture_arrays[arrays_counter];
		if(texture_array.update() == 0) {
			log.message("Removing \"%s\" terrain texture\n",texture_array.name);
			
			texture_arrays.remove(arrays_counter);
			texture_arrays_map.remove(texture_array.name);
			
			delete texture_array;
		} else {
			arrays_counter++;
		}
	}
	
	/**************************************************************************\
	*
	* Textures
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		find_textures();
		
		textures_counter = 0;
		arrays_counter = 0;
	}
	
	void shutdown() {
		textures_counter = 0;
		arrays_counter = 0;
		
		textures_map.delete();
		textures.delete();
		
		texture_arrays_map.delete();
		texture_arrays.delete();
	}
	
	/*
	 */
	void update(int need_reload) {
		if(need_reload) find_textures();
		
		update_textures();
		update_arrays();
	}
	
} /* namespace Textures */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void texturesInit() {
	Textures::init();
}

void texturesShutdown() {
	Textures::shutdown();
}

/*
 */
void texturesAdd(string name,string type) {
	Textures::add_texture(name,type);
}

/*
 */
void texturesAddTerrain(ObjectTerrain terrain) {
	Textures::add_terrain(terrain);
}

void texturesUpdateTerrainDiffuseArray(ObjectTerrain terrain,int size = -1) {
	Textures::update_array_diffuse(terrain,size);
}

void texturesUpdateTerrainNormalArray(ObjectTerrain terrain,int size = -1) {
	Textures::update_array_normal(terrain,size);
}

void texturesUpdateTerrainSpecularArray(ObjectTerrain terrain,int size = -1) {
	Textures::update_array_specular(terrain,size);
}

void texturesUpdate(int need_reload) {
	Textures::update(need_reload);
}

/*
 */
int texturesDialogImage(string title,string type,string &name,int allow_float = true) {
	return Textures::dialog_image(title,type,name,allow_float);
}

int texturesCheckImage(string name) {
	return Textures::check_image(name);
}
