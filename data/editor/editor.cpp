/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor.cpp
 * Desc:    Unigine editor
 * Version: 1.36
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Andrey Viktorov <unclebob@unigine.com>
 *          Mikhail Lyubimov <alterego@unigine.com>
 *          Alexey Zagniy <yingaz@unigine.com>
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

//#undef HAS_INTERFACE

#define EDITOR_LOCALE
//#define EDITOR_MINIMAL
//#define EXPORT_LOCALE

#include <core/unigine.h>
#include <core/scripts/tuple.h>
#include <core/scripts/array.h>
#include <core/scripts/utils.h>
#include <core/scripts/euler.h>
#include <editor/editor_version.h>

#define EDITOR_UNDO_DEPTH	32

#define EDITOR_LUT
#define EDITOR_TRACKER
#define EDITOR_WIDGETS

//#define EDITOR_MESSAGE
//#define EDITOR_OBJECT_GUI

/*
 */
Gui gui;
UserInterface user_interfaces[0];

/*
 */
vec4 color_green = engine.gui.parseColor("#9fff39");
vec4 color_orange = engine.gui.parseColor("#ff7000");
vec4 color_red = engine.gui.parseColor("#ff0000");

/*
 */
#ifdef EDITOR_LOCALE && !EXPORT_LOCALE
	#define TR(TEXT)		engine.gui.translate(TEXT)
	engine.gui.clearDictionaries();
#else
	#define TR(TEXT)		TEXT
#endif

/*
 */
#ifdef EDITOR_WIDGETS
	#include <core/systems/widgets/widget_interface.h>
	#include <core/systems/widgets/widget_dialog_message.h>
	#include <core/systems/widgets/widget_dialog_file.h>
	#include <core/systems/widgets/widget_dialog_color.h>
	#include <core/systems/widgets/widget_dialog_image.h>
	#include <core/systems/widgets/widget_dialog_material.h>
	#include <core/systems/widgets/widget_dialog_property.h>
	#include <core/systems/widgets/widget_dialog_node.h>
	#include <core/systems/widgets/widget_line_color.h>
	#include <core/systems/widgets/widget_line_environment.h>
	#include <core/systems/widgets/widget_line_toggle.h>
	#include <core/systems/widgets/widget_line_switch.h>
	#include <core/systems/widgets/widget_line_file.h>
	#include <core/systems/widgets/widget_line_material.h>
	#include <core/systems/widgets/widget_line_property.h>
	#include <core/systems/widgets/widget_line_node.h>
#endif

/*
 */
#include <editor/editor_constants.h>
#include <editor/editor_data.h>

#include <editor/widgets/editor_widget_manipulator.h>
#include <editor/widgets/editor_widget_viewport_layout.h>
#include <editor/widgets/editor_widgets.h>
#include <editor/widgets/editor_widget_panel.h>
#include <editor/widgets/editor_widget_menu.h>

#include <editor/assets/editor_assets.h>

#include <editor/editor_about.h>
#include <editor/editor_async.h>
#include <editor/editor_config.h>
#include <editor/editor_interface.h>
#include <editor/editor_utils.h>
#include <editor/editor_menu.h>
#include <editor/editor_textures.h>
#include <editor/editor_dialogs.h>
#include <editor/editor_tools.h>
#include <editor/editor_plugins.h>
#include <editor/editor_unredo.h>
#include <editor/editor_materials.h>
#include <editor/editor_properties.h>
#include <editor/editor_isolation.h>
#include <editor/editor_reference.h>
#include <editor/editor_brushes.h>
#include <editor/editor_terrain.h>
#include <editor/editor_mask.h>
#include <editor/editor_nodes.h>
#include <editor/editor_render.h>
#include <editor/editor_sound.h>
#include <editor/editor_physics.h>
#include <editor/editor_tracker.h>
#include <editor/editor_hotkeys.h>
#include <editor/editor_creator_primitive.h>
#include <editor/editor_creator_terrain.h>

#ifdef HAS_AUTODESK_IMPORT
	#include <editor/editor_creator_fbx.h>
#endif

#include <editor/editor_creator.h>
#include <editor/editor_ambient.h>
#include <editor/editor_controls.h>
#include <editor/editor_validator.h>
#include <editor/editor_thumbnails.h>
#include <editor/editor_cameras.h>
#include <editor/editor_viewports.h>
#include <editor/editor_panels.h>
#include <editor/editor_tooltip.h>
#include <editor/editor_console.h>

/******************************************************************************\
*
* Profiler
*
\******************************************************************************/

/*
 */
void profiler_begin(string name) {
	engine.profiler.begin(name);
	#ifdef HAS_MEMORY
		engine.memory.getNumFrameAllocations();
	#endif
}

void profiler_end(string name) {
	float time = engine.profiler.end();
	#ifdef HAS_MEMORY
		int allocations = engine.memory.getNumFrameAllocations();
		log.message("%s:\t%fms\t%d\n",name,time,allocations);
	#else
		log.message("%s:\t%fms\n",name,time);
	#endif
}

/******************************************************************************\
*
* Play
*
\******************************************************************************/

void playToggle() {
	engine.game.setEnabled(!engine.game.isEnabled());
}

/******************************************************************************\
*
* Messages
*
\******************************************************************************/

/*
 */
#ifdef EDITOR_MESSAGE

/*
 */
void editor_server() {
	
	class ServerSocket {
		Socket socket;
		ServerSocket() {
			socket = new Socket(SOCKET_STREAM,0xbeef);
			socket.bind();
			socket.listen(10);
			if(socket.isOpened()) socket.nonblock();
		}
		~ServerSocket() {
			if(socket.isOpened()) socket.close();
			delete socket;
		}
		int isOpened() {
			return socket.isOpened();
		}
		int accept(Socket s) {
			return socket.accept(s);
		}
	};
	
	ServerSocket socket = new ServerSocket();
	if(socket.isOpened() == 0) return;
	
	Socket s = NULL;
	
	while(1) {
		if(s == NULL) {
			s = new Socket(SOCKET_STREAM);
		}
		if(socket.accept(s)) {
			s.block();
			string message = s.readString();
			log.message("Message from %s\n%s\n",s.getHost(),message);
			dialogMessageOk(TR("Engine"),message);
			s.close();
			delete s;
			s = NULL;
		}
		wait;
	}
}

/*
 */
void editor_message(string address,string message) {
	Socket socket = new Socket(SOCKET_STREAM,address,0xbeef);
	if(socket.isOpened() == 0) {
		log.error("Can't create socket to \"%s\"\n",address);
		delete socket;
		return;
	}
	socket.nonblock();
	float time = clock();
	while(clock() - time < 1.0f) {
		if(socket.connect()) {
			socket.writeString(message);
			socket.close();
			delete socket;
			return;
		}
	}
	log.error("Can't connect to \"%s\"\n",address);
	socket.close();
	delete socket;
}

#endif /* EDITOR_MESSAGE */

/******************************************************************************\
*
* Locale
*
\******************************************************************************/

/*
 */
#ifdef EXPORT_LOCALE

/*
 */
void export_locale(string path,string name,string mask) {
	
	string dictionary[];
	
	void parse_file(string name) {
		File file = new File();
		if(file.open(name,"rb")) {
			while(file.eof() == 0) {
				if(file.getc() == 'T' && file.getc() == 'R' && file.getc() == '(' && file.getc() == '"') {
					file.seekCur(-1);
					string token = file.readToken();
					if(file.getc() != ')') log.error("parse_file(): can't parse \"%s\" file\n%s\n",name,token);
					else if(strlen(token)) dictionary.append(token,replace(name,path,""));
				}
			}
			file.close();
		}
		delete file;
	}
	
	void parse_directory(string path) {
		Dir dir = new Dir();
		if(dir.open(path)) {
			forloop(int i = 0; dir.getNumDirs()) {
				string name = dir.getDirName(i);
				if(strstr(name,".svn") == -1) parse_directory(name);
			}
			forloop(int i = 0; dir.getNumFiles()) {
				string name = dir.getFileName(i);
				if(strstr(name,mask) == -1) continue;
				if(re_match(name,".+\\.h$")) parse_file(name);
				if(re_match(name,".+\\.cpp$")) parse_file(name);
			}
			dir.close();
		}
		delete dir;
	}
	
	parse_directory(path);
	
	log.message("Saving \"%s\" dictionary\n",name);
	
	Xml xml = new Xml("dictionary","version=\"1.01\"");
	foreachkey(string token; dictionary) {
		Xml x = xml.addChild("msg",format("file=\"%s\"",dictionary[token]));
		Xml(x.addChild("src")).setXmlData(token);
		Xml(x.addChild("en")).setXmlData(token);
	}
	xml.save(name);
	delete xml;
}

#elif EDITOR_LOCALE

/*
 */
void update_language() {
	string languages[] = ( "en", "ru" );
	int language = engine.system.call("systemGetLanguage");
	if(languages.check(language)) {
		language = languages[language];
		engine.gui.clearDictionaries();
		engine.gui.addDictionary("editor/locale/editor.locale",language);
		engine.gui.addDictionary("editor/locale/source.locale",language);
		engine.gui.addDictionary("editor/locale/plugins.locale",language);
		engine.gui.addDictionary("editor/locale/systems.locale",language);
		if(user_interfaces.size()) {
			foreach(UserInterface ui; user_interfaces) {
				ui.updateWidgets();
			}
			materialsUpdateView();
			propertiesUpdateView();
			pluginsRun("updateLanguage");
		}
		menuUpdateItems();
	}
}

#endif

/*
 */
void update_title() {
	string title = "";
	if(engine.world.isLoaded()) {
		title = engine.world.getName() + ".world";
		if(unredoIsWorldDirty()) title += " *";
		title += " - ";
	}
	title += "UnigineEditor " + EDITOR_VERSION;
	
	#ifdef _WIN32
		string api = "";
		switch(engine.render.getAPI()) {
			case API_DIRECT3D11: api = "DX11"; break;
			case API_OPENGL: api = "OpenGL"; break;
		}
		
		title += format(" (%s)",api);
	#endif
	
	engine.app.setTitle(title);
}

/*
 */
void confirm_world_save() {
	#ifndef _LINUX
		if(unredoIsWorldDirty() && engine.app.dialogMessage("Question","Do you want to save changes in the current world?","Yn") == 2) {
			engine.console.run("world_save");
			engine.console.flush();
		}
	#endif
}

/*
 * current world name
 */
string current_world;

/******************************************************************************\
*
*
*
\******************************************************************************/

/*
 */
int init() {
	
	current_world = "";
	
	log.message("\n---- Editor ----\n");
	log.message("Version: "EDITOR_VERSION"\n\n");
	
	#ifdef EDITOR_MESSAGE
		thread(functionid(editor_server));
		engine.console.addCommand("editor_message","send a network message to another editor","editor_message");
	#endif
	
	#ifdef EDITOR_OBJECT_GUI
		ObjectGui object = new ObjectGui(80.0f,60.0f,"core/gui/");
		object.setScreenSize(1280,1024);
		object.setMaterial("gui_base","*");
		object.setTransform(translate(0.0f,30.0f,60.0f) * rotate(1.0f,0.0f,0.0f,90.0f));
		object.setControlDistance(100.0f);
		gui = object.getGui();
	#else
		gui = engine.getGui();
	#endif
	
	configInit();
	
	int position_x = configGet("position_x",engine.app.getPositionX());
	int position_y = configGet("position_y",engine.app.getPositionY());
	
	#ifdef HAS_INTERFACE
		position_x = engine.interface.getVisiblePositionX(position_x,32);
		position_y = engine.interface.getVisiblePositionY(position_y,32);
	#endif
	
	engine.app.setPosition(position_x,position_y);
	
	#ifdef EXPORT_LOCALE
		engine.gui.clearDictionaries();
		engine.gui.saveDictionary("editor/locale/editor.locale-new","en");
		export_locale(engine.getDataPath() + "editor/","editor/locale/source.locale-new","editor/editor_");
		export_locale(engine.getDataPath() + "editor/plugins/","editor/locale/plugins.locale-new","editor/plugins/");
		export_locale(engine.getDataPath() + "core/systems/","editor/locale/systems.locale-new","core/systems/");
	#elif EDITOR_LOCALE
		engine.system.call("systemSetLanguageEditorCallback","update_language");
		update_language();
	#endif
	
	string interfaces[] = (
		"editor/editor_dialogs.ui",
		"editor/editor_materials.ui",
		"editor/editor_properties.ui",
		"editor/editor_render.ui",
		"editor/editor_sound.ui",
		"editor/editor_physics.ui",
		"editor/editor_tools.ui",
		"editor/editor_plugins.ui",
		"editor/editor_tracker.ui",
		"editor/editor_hotkeys.ui",
		"editor/editor_nodes.ui",
		"editor/editor_nodes_node.ui",
		"editor/editor_worlds.ui",
		"editor/editor_fields.ui",
		"editor/editor_lights.ui",
		"editor/editor_decals.ui",
		"editor/editor_shapes.ui",
		"editor/editor_joints.ui",
		"editor/editor_bodies.ui",
		"editor/editor_objects.ui",
		"editor/editor_players.ui",
		"editor/editor_physicals.ui",
		"editor/editor_navigations.ui",
		"editor/editor_obstacles.ui",
		"editor/editor_sounds.ui",
		"editor/editor_objects_meshes.ui",
		"editor/editor_objects_terrain.ui",
		"editor/editor_objects_grass.ui",
		"editor/editor_objects_particles.ui",
		"editor/editor_objects_billboard.ui",
		"editor/editor_objects_billboards.ui",
		"editor/editor_objects_volumes.ui",
		"editor/editor_objects_water.ui",
		"editor/editor_objects_sky.ui",
		"editor/editor_objects_gui.ui",
		"editor/editor_isolation.ui",
		"editor/editor_reference.ui",
		"editor/editor_validator.ui",
	);
	
	foreach(string name; interfaces) {
		UserInterface ui = new UserInterface(gui,name);
		user_interfaces.append(ui);
		hotkeysAddUiToolTips(ui);
	}
	
	engine.console.setInt("show_visualizer",1);
	
	#ifdef HAS_INTERFACE
		updateWindows();
	#endif
	
	dataInit();
	asyncInit();
	ambientInit();
	assetsInit();
	dialogsInit();
	texturesInit();
	toolsInit();
	pluginsInit();
	nodesInit();
	materialsInit();
	propertiesInit();
	soundInit();
	physicsInit();
	trackerInit();
	hotkeysInit();
	aboutInit();
	creatorInit();
	controlsInit();
	camerasInit();
	unredoInit();
	isolationInit();
	referenceInit();
	validatorInit();
	menuInit();
	viewportsInit();
	panelsInit();
	tooltipInit();
	thumbnailsInit();
	consoleLogInit();
	
	update_title();
	
	return 1;
}

/*
 */
int shutdown() {
	
	#ifndef HAS_QT_EDITOR
		confirm_world_save();
	#endif
	
	#ifdef EXPORT_LOCALE
		engine.system.call("systemSetLanguageEditorCallback",NULL);
		engine.gui.clearDictionaries();
	#endif
	
	configSet("position_x",engine.app.getPositionX());
	configSet("position_y",engine.app.getPositionY());
	
	brushesDisable(1);
	
	world_shutdown();
	
	assetsShutdown();
	aboutShutdown();
	dialogsShutdown();
	texturesShutdown();
	toolsShutdown();
	pluginsShutdown();
	nodesShutdown();
	materialsShutdown();
	propertiesShutdown();
	renderShutdown();
	soundShutdown();
	physicsShutdown();
	trackerShutdown();
	hotkeysShutdown();
	creatorShutdown();
	ambientShutdown();
	unredoShutdown();
	isolationShutdown();
	referenceShutdown();
	validatorShutdown();
	brushesShutdown();
	thumbnailsShutdown();
	menuShutdown();
	camerasShutdown();
	controlsShutdown();
	panelsShutdown();
	tooltipShutdown();
	viewportsShutdown();
	configShutdown();
	asyncShutdown();
	dataShutdown();
	consoleLogShutdown();
	
	user_interfaces.delete();
	
	engine.app.setTitle("Unigine engine http://unigine.com");
	
	current_world = "";
	
	return 1;
}

/*
 */
int update() {
	
	#ifdef HAS_INTERFACE
		updateWindows();
	#endif
	
	int need_reload = engine.editor.needReload();
	
	engine.app.setUpdate(toolsGetAlwaysUpdate());
	
	aboutUpdate();
	assetsUpdate(need_reload);
	texturesUpdate(need_reload);
	trackerUpdate(need_reload);
	toolsUpdate(need_reload);
	pluginsUpdate(need_reload);
	creatorUpdate(need_reload);
	controlsUpdate(need_reload);
	nodesUpdate(need_reload);
	referenceUpdate(need_reload);
	materialsUpdate(need_reload);
	propertiesUpdate(need_reload);
	renderUpdate(need_reload);
	soundUpdate(need_reload);
	physicsUpdate(need_reload);
	viewportsUpdate(need_reload);
	hotkeysUpdate(need_reload);
	ambientUpdate(need_reload);
	brushesUpdate(need_reload);
	unredoUpdate(need_reload);
	thumbnailsUpdate(need_reload);
	menuUpdate(need_reload);
	panelsUpdate(need_reload);
	tooltipUpdate(need_reload);
	consoleLogUpdate();
	
	asyncsUpdate();
	
	if(need_reload) engine.console.setInt("show_visualizer",1);
	check_world();
	
	return 1;
}

/*
 */
int render() {
	controlsRender();
	nodesRender();
	panelsRender();
	return 1;
}

void check_world() {
	if(engine.world.isLoaded() == false) {
		current_world = "";
		return;
	}
	string world = getCurrentWorldName();
	if(current_world == world) return;
	current_world = world;
	world_init();
}

/*
 */
int world_shutdown() {
	
	nodesShutdown();
	ambientClear();
	unredoShutdown();
	isolationShutdown();
	referenceShutdown();
	brushesShutdown();
	unredoResetState();
	camerasWorldShutdown();
	dataShutdown();
	
	#ifdef HAS_INTERFACE
		clearWindows();
	#endif
	
	current_world = "";
	
	return 1;
}

int world_save() {
	
	ambientSave();
	isolationSave();
	referenceSave();
	camerasWorldSave();
	pluginsRun("save");
	unredoResetState();
	dataWorldSave();
	nodesWorldSave();
	
	return 1;
}

void world_init() {
	dataInit();
	menuWorldInit();
	panelWorldInit();
	camerasWorldInit();
	controlsWorldInit();
	
	update_title();
}
