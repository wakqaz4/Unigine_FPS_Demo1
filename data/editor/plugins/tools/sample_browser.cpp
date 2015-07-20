/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    sample_browser.cpp
 * Desc:    Sample browser plugin
 * Version: 1.00
 * Author:  Vladimir Borovskikh <futurist@unigine.com>
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

#include <core/unigine.h>

/*
 */
class SampleBrowser {
	
	using Unigine::Widgets;
	
	Window window;
	Icon close_i;
	VBox content;
	int samples_exists = true;
	
	//
	PluginMeta meta;
	
	//
	void get_sample_names(string path,string names[]) {
		Dir directory = new Dir();
		if(path == "samples/") {
		
			if(directory.open(engine.getDataPath() + "samples/")) {
				forloop(int i = 0; directory.getNumDirs()) {
					Dir samples = new Dir();
					if(samples.open(directory.getDirName(i))) {
						string files[0];
						forloop(int j = 0; samples.getNumFiles()) {
							files.append(samples.getFileName(j));
						}
						files.sort();
						foreach(string file; files) {
							if(strrstr(file,".world") == -1) continue;
							file = relname(engine.getDataPath() + "samples/",file);
							names.append(replace(file,".world",""));
							break;
						}
						samples.close();
					}
					delete samples;
				}
				directory.close();
			} else {
				string files[0];
				forloop(int i = 0; engine.filesystem.getNumPackages()) {
					engine.filesystem.getPackageFileNames(i,files);
					foreach(string file; files) {
						if(strstr(file,"samples/") != 0) continue;
						file = substr(file,8);
						int pos = strchr(file,'/');
						if(pos == -1) continue;
						file = substr(file,0,pos);
						if(names.find(file) != -1) continue;
						names.append(file);
					}
				}
				forloop(int i = 0; engine.filesystem.getNumPackages()) {
					engine.filesystem.getPackageFileNames(i,files);
					forloop(int j = 0; names.size()) {
						string name = "samples/" + names[j] + "/";
						foreach(string file; files) {
							if(strstr(file,name) != 0) continue;
							if(strrstr(file,".world") == -1) continue;
							names[j] = replace(replace(file,"samples/",""),".world","");
							break;
						}
					}
				}
			}
		} else {
			if(directory.open(engine.getDataPath() + path)) {
				forloop(int i = 0; directory.getNumFiles()) {
					string name = basename(directory.getFileName(i));
					if(strrstr(name,".world") == -1) continue;
					names.append(replace(name,".world",""));
				}
				directory.close();
			} else {
				string files[0];
				forloop(int i = 0; engine.filesystem.getNumPackages()) {
					engine.filesystem.getPackageFileNames(i,files);
					foreach(string file; files) {
						if(strstr(file,path) != 0) continue;
						if(strrstr(file,".world") == -1) continue;
						names.append(replace(basename(file),".world",""));
					}
				}
			}
		}
		
		delete directory;
		names.sort();
	}
	
	//
	void build_layout(string path,string current_name = "") {
		string names[0];
		get_sample_names(path,names);
		
		if(path == "samples/" && names.size() == 0) {
			samples_exists = false;
		}
		
		HBox hbox = new HBox();
		GroupBox groupbox = new GroupBox();
		groupbox.setPadding(2,2,2,2);
		groupbox.setBackground(1);
		hbox.addChild(groupbox,GUI_ALIGN_EXPAND);
		groupbox.addChild(new Label("Select scene"));
		GridBox gridbox = new GridBox(3);
		gridbox.setPadding(0,0,2,0);
		groupbox.addChild(gridbox,GUI_ALIGN_EXPAND);
		
		if(path != "samples/") {
			Button button = new Button("../");
			button.setFontSize(11);
			button.setCallback(GUI_CLICKED,"SampleBrowser::SampleBrowser::clicked","world_load samples/samples");
			gridbox.addChild(button,GUI_ALIGN_EXPAND);
		}
		
		foreach(string name; names) {
			Button button = new Button(name);
			
			if(name == current_name) {
				button.setToggleable(1);
				button.setToggled(1);
			}
			
			button.setFontSize(13);
			button.setCallback(GUI_CLICKED,"SampleBrowser::SampleBrowser::clicked","world_load " + path + name);
			gridbox.addChild(button,GUI_ALIGN_EXPAND);
		}
		
		gridbox.arrange();
		
		if(content.getNumChilds()) {
			Widget child = content.getChild(0);
			content.removeChild(child);
			delete child;
		}
		content.addChild(hbox,GUI_ALIGN_EXPAND);
		content.arrange();
	}
	
	//
	void clicked(string command) {
		engine.console.run(command);
	}
	
	//
	SampleBrowser(PluginMeta m) {
		meta = m;
		
		window = new Window("SampleBrowser");
		window.setCallback(SHOW,"SampleBrowser::SampleBrowser::show_window_redirector",this);
		
		close_i = new Icon("core/gui/window_close.png");
		window.addChild(close_i,GUI_ALIGN_OVERLAP | GUI_ALIGN_TOP | GUI_ALIGN_RIGHT);
		close_i.setCallback(CLICKED,"SampleBrowser::SampleBrowser::close_redirector",this);
		close_i.setPosition(4,-24);
		
		content = new VBox();
		window.addChild(content,ALIGN_EXPAND);
		
		window.arrange();
		
		pluginsAddWindow(window,meta.title,meta.name);
	}
	
	//
	~SampleBrowser() {
		pluginsRemoveWindow(window,meta.name);
		hide();
		
		delete close_i;
		delete window;
	}
	
	//
	void close() {
		windowHide(window,meta.name);
	}
	
	//
	void close_redirector(SampleBrowser samples_browser) {
		samples_browser.close();
	}
	
	//
	void show_window() {
		if(samples_exists == false) {
			windowHide(window,meta.name);
		}
	}
	
	//
	void show_window_redirector(SampleBrowser samples_browser) {
		samples_browser.show_window();
	}
	
	//
	void show() {
		string world_name = engine.world.getName();
		string path = substr(world_name,0,strrchr(world_name,'/') + 1);
		string name = substr(world_name,strrchr(world_name,'/') + 1);
		
		if(!engine.world.isLoaded() || path == "samples/" || strstr(path,"samples/") == -1) {
			path = "samples/";
			window.setText("SampleBrowser");
		} else {
			window.setText("SampleBrowser - " + name);
		}
		
		window.arrange();
		
		build_layout(path,name);
		
		window.arrange();
		
		#ifdef HAS_INTERFACE
			InterfaceWindow interface = window.getInterface();
			if(interface != NULL) {
				interface.setWidth(window.getWidth());
				interface.setHeight(window.getHeight());
			}
		#endif
		
		if(samples_exists == false) {
			windowHide(window,meta.name);
		}
		
		if(!engine.world.isLoaded()) return;
		if(!engine.world.call("is_function","sampelsHideInterface",0)) return;
		engine.world.call("sampelsHideInterface");
	}
	
	//
	void hide() {
		removeChild(window);
		
		if(!engine.world.isLoaded()) return;
		if(!engine.world.call("is_function","sampelsShowInterface",0)) return;
		engine.world.call("sampelsShowInterface");
	}
};

/*
 */
string getName() {
	return "SampleBrowser";
}

SampleBrowser samples_browser;

/*
 */
void init(PluginMeta meta) {
	samples_browser = new SampleBrowser(meta);
	samples_browser.show();
}

/*
 */
void shutdown() {
	delete samples_browser;
}

/*
 */
void update(int need_reload) {
	if(need_reload) {
		samples_browser.show();
	}
}

/*
 */
void show() {
	samples_browser.show();
	pluginsShowWindow(samples_browser.window,samples_browser.meta.name);
}