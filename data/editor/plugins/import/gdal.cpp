/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    gdal.cpp
 * Desc:    GDAL plugin
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

/******************************************************************************\
*
* Plugin
*
\******************************************************************************/

/*
 */
string getName() {
	return "GDAL";
}

/*
 */
#ifndef HAS_GDAL_IMPORT

/*
 */
void init() {
	dialogMessageOk(TR("Error"),TR("GDAL plugin library is not loaded."));
}

#else

/*
 */
void init(PluginMeta m) {
	GDAL::init(m);
}

void shutdown() {
	GDAL::shutdown();
}

void show() {
	GDAL::show();
}
/******************************************************************************\
*
* GDAL
*
\******************************************************************************/

/*
 */
namespace GDAL {
	
	/*
	 */
	using Unigine::Widgets;
	
	/*
	 */
	UserInterface interface;
	
	Window window;
	
	string name = "";
	
	GDALImport import;
	
	/*
	 */
	PluginMeta meta;
	
	/*
	 */
	void init(PluginMeta m) {
		
		meta = m;
		
		interface = new UserInterface(replace(meta.source,".cpp",".ui"),getName() + "::");
		hotkeysAddUiToolTips(interface);
		
		name = configGet("plugin_gdal_name",name);
		
		window.arrange();
		
		pluginsAddWindow(window,meta.title,meta.name);
	}
	
	/*
	 */
	void shutdown() {
		
		configSet("plugin_gdal_name",name);
		
		pluginsRemoveWindow(window,meta.name);
		
		hotkeysRemoveUiToolTips(interface);
		delete interface;
		
		delete import;
	}
	
	void show() {
		pluginsShowWindow(window,meta.name);
	}
	
	/*
	 */
	void window_close_clicked() {
		windowHide(window,meta.name);
	}
	
	/*
	 */
	void load_clicked() {
		
		if(dialogFile(TR("Select GDAL file"),"",name) == 0) return;
		
		delete import;
		
		import = new GDALImport();
		if(import.load(name) == 0) {
			dialogMessageOk(TR("Error"));
			delete import;
			return;
		}
		
		get_info();
	}
	
	/*
	 */
	void get_info() {
		
		Reflection reflection = new Reflection(import);
		forloop(int i = 0; reflection.getNumFunctions()) {
			string name = reflection.getFunctionName(i);
			int num_args = reflection.getNumFunctionArgs(i);
			int id = reflection.getFunctionID(i);
			if(strstr(name,"get") == 0 && num_args == 0) {
				name = replace(name,"get","");
				log.message("%s: %s\n",name,typeinfo(import.call(id)));
			}
		}
		delete reflection;
		
		log.message("\n");
		Image image = new Image();
		forloop(int i = 0; import.getRasterCount()) {
			log.message("Band:  %d\n",i);
			log.message("Type:  %s\n",import.getRasterTypeName(i));
			log.message("Color: %s\n",import.getRasterColorName(i));
			if(import.getRasterImage(i,image)) {
				image.save(format("import_%d.dds",i));
			}
		}
		delete image;
		
		log.message("\n");
		forloop(int y = 0; import.getRasterSizeY(); import.getRasterSizeY() - 1) {
			forloop(int x = 0; import.getRasterSizeX(); import.getRasterSizeX() - 1) {
				double latitude = import.getLatitude(x,y);
				double longitude = import.getLongitude(x,y);
				log.message("%4dx%-4d: %f / %f\n",x,y,latitude,longitude);
			}
		}
	}
}

#endif /* HAS_GDAL_IMPORT */
