/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_plugins.h
 * Desc:    Unigine editor
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


class PluginMeta {
	string name;
	string version;
	string title;
	string description;
	string source;
};

/*
 */
namespace Plugins {
	
	/*
	 */
	DECLARE_WINDOW
	
	/*
	 */
	WidgetGridBox plugins_gb;
	int first_load = 1;
	
	/*
	 */
	class Plugin {
		
		// plugin data
		string filename;
		string dependencies;
		PluginMeta meta;
		Expression expression;
		string internal_name;
		int update_id;
		
		// state
		int is_loaded;
		int is_enabled;
		
		// interface
		WidgetLabel name_l;
		WidgetLabel description_l;
		WidgetCheckBox state_c;
		
		//
		Plugin(string n) {
			filename = n;
			meta = new PluginMeta();
			load();
		}
		
		//
		~Plugin() {
			if(is_enabled) {
				if(expression.isFunction("shutdown",0)) call(expression.getFunction("shutdown",0));
				expression.setName("");
				delete expression;
			}
			
			delete name_l;
			delete description_l;
			delete state_c;
			
			delete meta;
		}
		
		//
		void build_layout() {
			if(!is_loaded) return;
			
			name_l = new WidgetLabel(engine.getGui(),meta.title);
			name_l.setToolTip(filename);
			description_l = new WidgetLabel(engine.getGui(),meta.description);
			state_c = new WidgetCheckBox(engine.getGui());
			state_c.setChecked(isEnabled());
			state_c.setCallback(GUI_CHANGED,functionid(toggle_state_callback),this);
			
			plugins_gb.addChild(name_l,GUI_ALIGN_LEFT);
			plugins_gb.addChild(description_l,GUI_ALIGN_EXPAND);
			plugins_gb.addChild(state_c,GUI_ALIGN_CENTER);
		}
		
		//
		void update_layout() {
			if(!is_loaded) return;
			
			if(name_l == NULL || description_l == NULL || state_c == NULL) build_layout();
			state_c.setCallbackEnabled(GUI_CHANGED,0);
			state_c.setChecked(is_enabled);
			state_c.setCallbackEnabled(GUI_CHANGED,1);
		}
		
		// check dependencies
		int check_dependencies() {
			if(strlen(dependencies) > 0) {
				string test = "{\n#ifdef (" + dependencies + ") \n return 1; \n #else \n return 0; \n #endif\n}";
				Expression dependencies_test = new Expression(test);
				if(dependencies_test.isCompiled()) {
					if(!dependencies_test.run()) {
						delete dependencies_test;
						return false;
					} else {
						delete dependencies_test;
						return true;
					}
				}
			}
			return true;
		}
		
		//
		int load() {
			is_loaded = false;
			Xml xml = new Xml();
			if(xml.load(filename) == 0 || xml.getName() != "plugin" || xml.getArg("version") < "1.0") {
				delete xml;
				return 0;
			}
			
			
			meta.name = xml.getArg("name");
			meta.version = xml.getArg("version");
			forloop(int i = 0; xml.getNumChilds()) {
				Xml property = xml.getChild(i);
				if(property.getName() == "text") meta.title = property.getData();
				if(property.getName() == "description") meta.description = property.getData();
				if(property.getName() == "source") meta.source = pathname(filename) + property.getData();
				if(property.getName() == "dependencies") dependencies =  replace(property.getData(),","," && ");
			}
			
			xml.printUnusedData(filename);
			delete xml;
			is_loaded = true;
		}
		
		//
		int build(int force = 0) {
			if(!is_loaded) return false;
			if(is_enabled) return true;
			is_enabled = false;
			update_layout();
			
			File file = new File();
			if(file.open(meta.source,"rb") == 0) {
				log.error("Can't open \"%s\" plugin\n",meta.source);
				delete file;
				return false;
			}
			string src = file.gets();
			file.close();
			delete file;
			
			// common
			#ifdef EDITOR_LOCALE
				string common = "\n#define TR(TEXT) engine.gui.translate(TEXT)\n";
			#else
				string common = "\n#define TR(TEXT) TEXT\n";
			#endif
			
			// compile plugin
			expression = new Expression("{" + common + src + "}",1);
			if(expression.isCompiled() == 0) {
				log.error("Can't compile \"%s\" plugin\n",meta.name);
				delete expression;
				return false;
			}
			
			// initialize plugin
			expression.run();
			
			// plugin internal name
			if(expression.isFunction("getName",0) == 0) {
				log.error("Can't find \"getName()\" function in \"%s\" plugin\n",meta.name);
				delete expression;
				return false;
			}
			
			// get internal name
			int name_id = expression.getFunction("getName",0);
			internal_name = call(name_id);
			if(is_string(internal_name) == 0) {
				log.error("Wrong return value of \"getName()\" function in \"%s\" plugin\n",meta.name);
				delete expression;
				return false;
			}
			
			// set expression name
			expression.setName(internal_name);
			
			// init plugin
			if(expression.isFunction("init",1)) {
				call(expression.getFunction("init",1),meta);
			} else if(expression.isFunction("init",0)) {
				call(expression.getFunction("init",0));
			}
			
			if(force && expression.isFunction("show",0)) {
				call(expression.getFunction("show",0));
			}
			
			// update function
			update_id = -1;
			if(expression.isFunction("update",1)) {
				update_id = expression.getFunction("update",1);
			}
			
			is_enabled = true;
			update_layout();
			
			return true;
		}
		
		//
		int run(string name) {
			if(!is_enabled) return;
			
			if(expression.isFunction(name,0)) {
				return call(expression.getFunction(name,0));
			}
		}
		
		//
		void setEnabled(int enabled,int force = 0) {
			if(enabled) {
				if(!is_enabled) build(force);
			} else {
				if(is_enabled) {
					// shutdown plugin
					if(expression.isFunction("shutdown",0)) {
						call(expression.getFunction("shutdown",0));
					}
					expression.setName("");
					delete expression;
					expression = NULL;
				}
				is_enabled = false;
			}
			
			update_layout();
		}
		int isEnabled() { return is_enabled; }
		
		int update(int need_reload) {
			if(is_enabled && update_id != -1) call(update_id,need_reload);
		}
	};
	
	Plugin plugins[0];
	
	/*
	 */
	void toggle_state_callback(Plugin plugin) {
		if(plugin.isEnabled()) {
			plugin.setEnabled(0);
		} else {
			plugin.setEnabled(1,1);
		}
		
	}
	
	/*
	 */
	void run(string name) {
		forloop(int i = 0; plugins.size()) {
			if(plugins[i] != NULL) {
				plugins[i].run(name);
			}
		}
	}
	
	/*
	 */
	void run(string plugin,string name,int size = 0) {
		
		// run plugin function
		forloop(int i = 0; plugins.size()) {
			if(plugins[i].internal_name == plugin) {
				plugins[i].run(name);
				return;
			}
		}
	}
	
	
	/**************************************************************************\
	*
	* Window
	*
	\**************************************************************************/
	
	void enable_all_callback() {
		forloop(int i = 0; plugins.size()) {
			if(plugins[i].isEnabled()) continue;
			plugins[i].setEnabled(1,1);
		}
	}
	
	void disable_all_callback() {
		forloop(int i = 0; plugins.size()) {
			if(!plugins[i].isEnabled()) continue;
			plugins[i].setEnabled(0);
		}
	}
	
	void resize_window_width_callback() {
		if(configGet("plugins_width",0) != 0 || configGet("plugins_height",0) != 0) return;
		
		if(first_load == 1) {
			int width = window.getPaddingLeft() + window.getPaddingRight() + plugins_gb.getWidth() + 44;
			
			if(width > 1168) width = 1168;
			
			window.setWidth(width);
			first_load = 0;
		}
	}
	
	void load_available_plugins() {
		wait;
		
		string plugin_names[0];
		string plugin_path = "editor/plugins/";
		
		string path = engine.getDataPath() + plugin_path;
		
		void get_plugins(string name) {
			Dir dir = new Dir();
			if(dir.open(name)) {
				forloop(int i = 0; dir.getNumFiles()) {
					string name = dir.getFileName(i);
					if(strstr(name,path) != 0) continue;
					if(re_match(name,".+\\.plugin$") == 0) continue;
					plugin_names.append(name);
				}
				forloop(int i = 0; dir.getNumDirs()) {
					string name = dir.getDirName(i);
					if(strstr(name,".svn") != -1) continue;
					get_plugins(name);
				}
				dir.close();
			}
			delete dir;
		}
		
		get_plugins(path);
		
		
		forloop(int i = 0; engine.filesystem.getNumPackages()) {
			string names[0];
			engine.filesystem.getPackageFileNames(i,names);
			foreach(string name; names) {
				if(strstr(name,plugin_path) != 0) continue;
				if(re_match(name,".+\\.plugin$") == 0) continue;
				plugin_names.append(name);
			}
		}
		
		// load plugins
		forloop(int i = 0; plugin_names.size()) {
			Plugin plugin = new Plugin(plugin_names[i]);
			if(!plugin.check_dependencies()) {
				delete plugin;
				plugin = NULL;
				continue;
			}
			forloop(int j = 0; plugins.size()) {
				if(plugin.meta.name == plugins[j].meta.name) {
					delete plugin;
					plugin = NULL;
					break;
				}
			}
			if(plugin == NULL) continue;
			plugins.append(plugin);
			int enabled = configGet("plugins_" + plugins[plugins.size() - 1].meta.name,0);
			plugins[plugins.size() - 1].setEnabled(enabled);
		}
		
		// parse command line
		forloop(int i = 0; engine.getNumArgs()) {
			if(engine.getArg(i) == "-editor_plugin" && i + 1 < engine.getNumArgs()) {
				string editor_plugins[0];
				strsplit(engine.getArg(i + 1),",",editor_plugins);
				forloop(int i = 0; editor_plugins.size()) {
					Plugin plugin = new Plugin(editor_plugins[i]);
					if(!plugin.check_dependencies()) {
						delete plugin;
						plugin = NULL;
						continue;
					}
					forloop(int j = 0; plugins.size()) {
						if(plugin.meta.name == plugins[j].meta.name) {
							delete plugins[j];
							plugins[j] = plugin;
							plugins[j].setEnabled(1);
							plugin = NULL;
							break;
						}
					}
					if(plugin == NULL) continue;
					
					plugins.append(plugin);
					if(!plugins[plugins.size() - 1].isEnabled()) plugins[plugins.size() - 1].setEnabled(1);
				}
				break;
			}
		}
		
		plugins_gb.arrange();
	}
	
	/*
	 */
	void init() {
		editorWindow(window);
		thread(functionid(load_available_plugins));
		DECLARE_WINDOW_LOAD(plugins)
	}
	
	/*
	 */
	void shutdown() {
		DECLARE_WINDOW_SAVE(plugins)
		
		configSet("tools_plugins_num",plugins.size());
		
		// shutdown plugins
		forloop(int i = 0; plugins.size()) {
			Plugin plugin = plugins[i];
			configSet("plugins_" + plugin.meta.name,plugin.isEnabled());
			delete plugins[i];
		}
	}
	
	/*
	 */
	void update(int need_reload) {
		plugins.call("update",need_reload);
	}
	
} /* namespace Plugins */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void pluginsInit() {
	Plugins::init();
}

void pluginsShutdown() {
	Plugins::shutdown();
}

/*
 */
void pluginsToggle() {
	if(Plugins::window_show) pluginsHide();
	else pluginsShow();
}

void pluginsShow() {
	DECLARE_WINDOW_SHOW(Plugins)
}

void pluginsHide() {
	DECLARE_WINDOW_HIDE(Plugins)
}

/*
 */
void pluginsUpdate(int need_reload) {
	Plugins::update(need_reload);
}

/*
 */
void pluginsRun(string name) {
	Plugins::run(name);
}

void pluginsRun(string plugin,string name) {
	Plugins::run(plugin,name);
}

void pluginsAddWindow(Unigine::Widgets::Window window,string name,string key,string key_parent = "plugins") {
	if(key_parent == "plugins") menuAddBarItem("Plugins","plugins");
	
	menuAddItem(name,key,key_parent,functionid(windowToggle),(window,key));
	windowInit(window,key);
	menuUpdateItems();
}

void pluginsRemoveWindow(Unigine::Widgets::Window window,string key) {
	windowShutdown(window,key);
	menuRemoveItem(key);
	menuRemoveItem("plugins",false);
}

void pluginsShowWindow(Unigine::Widgets::Window window,string key) {
	windowShow(window,key);
}