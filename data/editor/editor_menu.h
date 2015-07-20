/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_menu.h
 * Desc:    Unigine editor
 * Version: 1.02
 * Author:  Mikhail Lyubimov <alterego@unigine.com>
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
namespace EditorMenu {
	
	using Unigine::Widgets;
	
	/**************************************************************************\
	*
	* ItemCallback
	*
	\**************************************************************************/
	
	/*
	 */
	class ItemCallback {
		int function;
		int args[0];
		
		ItemCallback(int f,int args_[] = ()) {
			function = f;
			args.copy(args_);
		}
		
		void run() {
			thread(function,args);
		}
	};
	
	#define NEW_ITEM_CALLBACK_ARGS(FUNCTION,ARGS) new ItemCallback(functionid(FUNCTION),(ARGS))
	#define NEW_ITEM_CALLBACK(FUNCTION) new ItemCallback(functionid(FUNCTION))
	#define NEW_ITEM_CALLBACK_CREATOR(ARGS) new ItemCallback(functionid(Creator::create),(ARGS))
	
	/*
	 */
	string items_callback[] = (
		"create_world"					:	NEW_ITEM_CALLBACK(create_world),
		"open_world"					:	NEW_ITEM_CALLBACK(open_world),
		"save_world"					:	NEW_ITEM_CALLBACK(save_world),
		"save_world_as"					:	NEW_ITEM_CALLBACK(save_world_as),
		"world_quit"					:	NEW_ITEM_CALLBACK(world_quit),
		"reload_world"					:	NEW_ITEM_CALLBACK(reload_world),
		"quit"							:	NEW_ITEM_CALLBACK(editor_quit),
		"exit"							:	NEW_ITEM_CALLBACK(exit),
		"system_menu"					:	NEW_ITEM_CALLBACK(system_menu_toggle),
		
		"undo"							:	NEW_ITEM_CALLBACK(unredoUndo),
		"redo"							:	NEW_ITEM_CALLBACK(unredoRedo),
		"nodes_clone"					:	NEW_ITEM_CALLBACK(nodesCloneNode),
		"nodes_delete"					:	NEW_ITEM_CALLBACK(nodesDeleteNode),
		"isolate_nodes"					:	NEW_ITEM_CALLBACK(nodesToggleIsolation),
		"group_selected"				:	NEW_ITEM_CALLBACK(nodesCreateDummyGroup),
		"convert_into_node_reference"	:	NEW_ITEM_CALLBACK(nodesConvertIntoNodeReference),
		
		"panel_plugins"					:	NEW_ITEM_CALLBACK(pluginsToggle),
		"panel_nodes"					:	NEW_ITEM_CALLBACK(nodesToggle),
		"panel_materials"				:	NEW_ITEM_CALLBACK(materialsToggle),
		"panel_properties"				:	NEW_ITEM_CALLBACK(propertiesToggle),
		"panel_render"					:	NEW_ITEM_CALLBACK(renderToggle),
		"panel_physics"					:	NEW_ITEM_CALLBACK(physicsToggle),
		"panel_sound"					:	NEW_ITEM_CALLBACK(soundToggle),
		"panel_tracker"					:	NEW_ITEM_CALLBACK(trackerToggle),
		"panel_tools"					:	NEW_ITEM_CALLBACK(toolsToggle),
		"panel_keys"					:	NEW_ITEM_CALLBACK(hotkeysToggle),
		"panel_validator"				:	NEW_ITEM_CALLBACK(validatorRun),
		"panel_viewport_1"				:	NEW_ITEM_CALLBACK_ARGS(viewportsToggleWindow,"Viewport 1"),
		"panel_viewport_2"				:	NEW_ITEM_CALLBACK_ARGS(viewportsToggleWindow,"Viewport 2"),
		"panel_viewport_3"				:	NEW_ITEM_CALLBACK_ARGS(viewportsToggleWindow,"Viewport 3"),
		"panel_viewport_4"				:	NEW_ITEM_CALLBACK_ARGS(viewportsToggleWindow,"Viewport 4"),
		
		"about"							:	NEW_ITEM_CALLBACK(aboutShow),
		"help"							:	NEW_ITEM_CALLBACK_ARGS(systemOpen,"https://developer.unigine.com/docs"),
		"forum"							:	NEW_ITEM_CALLBACK_ARGS(systemOpen,"https://developer.unigine.com/forum"),
		
		"box"							:	NEW_ITEM_CALLBACK_CREATOR(CREATE_PRIMITIVE_BOX),
		"cuboid"						:	NEW_ITEM_CALLBACK_CREATOR(CREATE_PRIMITIVE_CUBOID),
		"sphere"						:	NEW_ITEM_CALLBACK_CREATOR(CREATE_PRIMITIVE_SPHERE),
		"plane"							:	NEW_ITEM_CALLBACK_CREATOR(CREATE_PRIMITIVE_PLANE),
		"cylinder"						:	NEW_ITEM_CALLBACK_CREATOR(CREATE_PRIMITIVE_CYLINDER),
		"capsule"						:	NEW_ITEM_CALLBACK_CREATOR(CREATE_PRIMITIVE_CAPSULE),
		
		"mesh_static"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_MESH_STATIC),
		"mesh_skinned"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_MESH_SKINNED),
		"mesh_dynamic"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_MESH_DYNAMIC),
		"mesh_cluster"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_MESH_CLUSTER),
		"mesh_clutter"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_MESH_CLUTTER),
		"terrain"						:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_TERRAIN),
		"water"							:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_WATER),
		"water_mesh"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_WATER_MESH),
		"sky"							:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_SKY),
		"grass"							:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_GRASS),
		"billboard"						:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_BILLBOARD),
		"billboards"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_BILLBOARDS),
		"object_dummy"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_DUMMY),
		
		"node_dummy"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_NODE_DUMMY),
		"node_layer"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_NODE_LAYER),
		"pivot"							:	NEW_ITEM_CALLBACK_CREATOR(CREATE_NODE_PIVOT),
		"node_trigger"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_NODE_TRIGGER),
		"node"							:	NEW_ITEM_CALLBACK_CREATOR(CREATE_NODE),
		"reference"						:	NEW_ITEM_CALLBACK_CREATOR(CREATE_NODE_REFERENCE),
		"fbx"							:	NEW_ITEM_CALLBACK_CREATOR(CREATE_FBX),
		"player_dummy"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_PLAYER_DUMMY),
		"player_persecutor"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_PLAYER_PERSECUTOR),
		
		"particles"						:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_PARTICLES),
		"volume_box"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_VOLUME_BOX),
		"volume_sphere"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_VOLUME_SPHERE),
		"volume_omni"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_VOLUME_OMNI),
		"volume_proj"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_VOLUME_PROJ),
		"gui"							:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_GUI),
		"gui_mesh"						:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBJECT_GUI_MESH),
		"field_spacer"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_FIELD_SPACER),
		"field_animation"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_FIELD_ANIMATION),
		"physical_wind"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_PHYSICAL_WIND),
		"physical_force"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_PHYSICAL_FORCE),
		"physical_noise"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_PHYSICAL_NOISE),
		"physical_water"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_PHYSICAL_WATER),
		"physical_trigger"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_PHYSICAL_TRIGGER),
		
		"sound"							:	NEW_ITEM_CALLBACK_CREATOR(CREATE_SOUND_SOURCE),
		"reverb"						:	NEW_ITEM_CALLBACK_CREATOR(CREATE_SOUND_REVERB),
		
		"navigation_sector"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_NAVIGATION_SECTOR),
		"navigation_mesh"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_NAVIGATION_MESH),
		"obstacle_box"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBSTACLE_BOX),
		"obstacle_sphere"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBSTACLE_SPHERE),
		"obstacle_capsule"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_OBSTACLE_CAPSULE),
		
		"light_prob"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_LIGHT_PROB),
		"light_spot"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_LIGHT_SPOT),
		"light_omni"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_LIGHT_OMNI),
		"light_proj"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_LIGHT_PROJ),
		"light_world"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_LIGHT_WORLD),
		
		"world_layer"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_WORLD_LAYER),
		"world_sector"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_WORLD_SECTOR),
		"world_portal"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_WORLD_PORTAL),
		"world_trigger"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_WORLD_TRIGGER),
		"world_cluster"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_WORLD_CLUSTER),
		"world_clutter"					:	NEW_ITEM_CALLBACK_CREATOR(CREATE_WORLD_CLUTTER),
		"world_switcher"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_WORLD_SWITCHER),
		"world_occluder"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_WORLD_OCCLUDER),
		"world_occluder_mesh"			:	NEW_ITEM_CALLBACK_CREATOR(CREATE_WORLD_OCCLUDER_MESH),
		"world_occluder_terrain"		:	NEW_ITEM_CALLBACK_CREATOR(CREATE_WORLD_OCCLUDER_TERRAIN),
		"world_transform_bone"			:	NEW_ITEM_CALLBACK_CREATOR(CREATE_WORLD_TRANSFORM_BONE),
		"world_transform_path"			:	NEW_ITEM_CALLBACK_CREATOR(CREATE_WORLD_TRANSFORM_PATH),
		"world_expression"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_WORLD_EXPRESSION),
		
		"decal_object_omni"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_DECAL_OBJECT_OMNI),
		"decal_object_proj"				:	NEW_ITEM_CALLBACK_CREATOR(CREATE_DECAL_OBJECT_PROJ),
		"decal_object_ortho"			:	NEW_ITEM_CALLBACK_CREATOR(CREATE_DECAL_OBJECT_ORTHO),
		"decal_terrain_proj"			:	NEW_ITEM_CALLBACK_CREATOR(CREATE_DECAL_TERRAIN_PROJ),
		"decal_terrain_ortho"			:	NEW_ITEM_CALLBACK_CREATOR(CREATE_DECAL_TERRAIN_ORTHO),
		"decal_deferred_proj"			:	NEW_ITEM_CALLBACK_CREATOR(CREATE_DECAL_DEFERRED_PROJ),
		"decal_deferred_ortho"			:	NEW_ITEM_CALLBACK_CREATOR(CREATE_DECAL_DEFERRED_ORTHO),
		"decal_deferred_mesh"			:	NEW_ITEM_CALLBACK_CREATOR(CREATE_DECAL_DEFERRED_MESH),
		
		"profiler_disabled"				:	NEW_ITEM_CALLBACK_ARGS(set_profiler_mode,0),
		"profiler_generic"				:	NEW_ITEM_CALLBACK_ARGS(set_profiler_mode,1),
		"profiler_rendering"			:	NEW_ITEM_CALLBACK_ARGS(set_profiler_mode,2),
		"profiler_physics"				:	NEW_ITEM_CALLBACK_ARGS(set_profiler_mode,3),
		"profiler_world"				:	NEW_ITEM_CALLBACK_ARGS(set_profiler_mode,4),
		"profiler_pathfinding_sound"	:	NEW_ITEM_CALLBACK_ARGS(set_profiler_mode,5),
	);
	
	string items[];
	string menu_bar_items[];
	
	/*
	 */
	UserInterface ui;
	GroupBox menu_gb;
	MenuBar menu;
	
	MenuBox recent_worlds_mb;
	MenuBox plugins_mb;
	string recent_worlds[0];
	
	/*
	 */
	void init() {
		
		ui = new UserInterface("editor/editor_menu.ui","EditorMenu::");
		hotkeysAddUiToolTips(ui);
		
		addChild(menu_gb,GUI_ALIGN_OVERLAP);
		
		forloop(int i = 0; menu.getNumItems()) {
			MenuBox menu_box = menu.getItemMenu(i);
			menu_bar_items.append(menu.getItemData(i),i);
			
			add_items(menu_box);
		}
		
		#ifndef HAS_AUTODESK_IMPORT
			remove_item("fbx");
		#endif
		
		string worlds = configGet("recent_worlds","");
		string ret[0];
		int size = strsplit(worlds,",",ret);
		forloop(int i = 0; size) {
			recent_worlds.append(ret[i]);
		}
		
		update_recent_worlds();
		
		world_init();
		
		update_items();
	}
	
	void add_items(MenuBox menu_box) {
		forloop(int i = 0; menu_box.getNumItems()) {
			MenuBox child = menu_box.getItemWidget(i);
			if(child != NULL && child is MenuBox) add_items(child);
			
			string key = menu_box.getItemData(i);
			if(!items.check(key)) items.append(key,new Unigine::Triple(menu_box,i,menu_box.getItemText(i)));
		}
	}
	
	void shutdown() {
		
		string worlds = "";
		forloop(int i = 0; recent_worlds.size()) {
			worlds += recent_worlds[i] + ",";
		}
		configSet("recent_worlds",trim(worlds,","));
		
		hotkeysRemoveUiToolTips(ui);
		delete ui;
	}
	
	void new_menu_bar_item(string name,string key) {
		if(menu_bar_items.check(key)) return;
		int item = menu.addItem(name);
		menu.setItemData(item,key);
		
		MenuBox menu_box = new MenuBox();
		menu_box.setSpace(8,8);
		menu_box.setWidth(64);
		menu_box.setCallback(GUI_CLICKED,functionid(callback_clicked),menu_box.widget);
		
		menu.setItemMenu(item,menu_box);
		
		menu_bar_items.append(key,item);
	}
	
	int new_item(string name,string key,string parent_key,int f = NULL,int args[] = ()) {
		
		if(items.check(key)) return false;
		if(menu_bar_items.check(key)) return false;
		
		template add_item<WIDGET,ITEM> {
			MenuBox parent_menu_box = NULL;
			if(WIDGET is MenuBar) parent_menu_box = MenuBar(WIDGET).getItemMenu(ITEM);
			else parent_menu_box = MenuBox(WIDGET).getItemWidget(ITEM);
			
			if(parent_menu_box == NULL) {
				parent_menu_box = new MenuBox();
				parent_menu_box.setSpace(8,8);
				parent_menu_box.setWidth(64);
				parent_menu_box.setCallback(GUI_CLICKED,functionid(callback_clicked),parent_menu_box.widget);
				if(WIDGET is MenuBar) MenuBar(WIDGET).setItemMenu(ITEM,parent_menu_box);
				else MenuBox(WIDGET).setItemWidget(ITEM,parent_menu_box);
			}
			
			int item = parent_menu_box.addItem(name);
			forloop(int i = item + 1; parent_menu_box.getNumItems()) {
				string key_i = parent_menu_box.getItemData(i);
				Unigine::Triple data_i = items[key_i];
				data_i.second++;
			}
			
			parent_menu_box.setItemData(item,key);
			items.append(key,new Unigine::Triple(parent_menu_box,item,name));
			
			if(f != NULL) items_callback.append(key,new ItemCallback(f,args));
		}
		
		if(items.check(parent_key)) {
			
			Unigine::Triple data = items[parent_key];
			MenuBox menu_box = data.first;
			add_item<menu_box,data.second>;
			
		} else if(menu_bar_items.check(parent_key)) {
			add_item<menu,menu_bar_items[parent_key]>;
		}
	}
	
	int remove_item(string key,int mode = true) {
		
		template clear_item<WIDGET,ITEM> {
			MenuBox menu_box = NULL;
			if(WIDGET is MenuBar) menu_box = MenuBar(WIDGET).getItemMenu(ITEM);
			else menu_box = MenuBox(WIDGET).getItemWidget(ITEM);
			
			if(menu_box != NULL) {
				if(!mode && menu_box.getNumItems() != 0) return;
				
				forloop(int i = 0; menu_box.getNumItems()) {
					remove_item(menu_box.getItemData(i));
				}
			}
			
			WIDGET.removeItem(ITEM);
			
			if(items_callback.check(key)) items_callback.remove(key);
			if(items.check(key)) items.remove(key);
			if(menu_bar_items.check(key)) menu_bar_items.remove(key);
		}
		
		if(items.check(key)) {
		
			Unigine::Triple data = items[key];
			MenuBox parent_menu_box = data.first;
			
			int item = data.second;
			forloop(int i = item + 1; parent_menu_box.getNumItems()) {
				string key_i = parent_menu_box.getItemData(i);
				Unigine::Triple data_i = items[key_i];
				data_i.second--;
			}
			
			clear_item<parent_menu_box,item>;
		}
		
		if(menu_bar_items.check(key)) {
			int item = menu_bar_items[key];
			
			forloop(int i = item + 1; menu.getNumItems()) {
				string key_i = menu.getItemData(i);
				menu_bar_items[key_i]--;
			}
			
			clear_item<menu,item>;
		}
	}
	
	int old_world_is_loaded = -1;
	int old_brushes_is_enabled;
	int old_num_nodes;
	int old_undo_stack_size;
	int old_redo_stack_size;
	Node old_node;
	/*
	 */
	void update(int need_reload) {
		
		menu_gb.setWidth(engine.app.getWidth());
		
		int world_is_loaded = engine.world.isLoaded();
		int brushes_is_enabled = brushesIsEnabled();
		int num_nodes = nodesGetNumSelectedNodes();
		int undo_stack_size = unredoUndoStackSize();
		int redo_stack_size = unredoRedoStackSize();
		Node node = nodesGetNode();
		
		if(old_world_is_loaded == world_is_loaded &&
			old_brushes_is_enabled == brushes_is_enabled &&
			old_num_nodes == num_nodes &&
			old_undo_stack_size == undo_stack_size &&
			old_redo_stack_size == redo_stack_size &&
			old_node == node) return;
		
		old_world_is_loaded = world_is_loaded;
		old_brushes_is_enabled = brushes_is_enabled;
		old_num_nodes = num_nodes;
		old_undo_stack_size = undo_stack_size;
		old_redo_stack_size = redo_stack_size;
		old_node = node;
		
		set_item_enabled("recent_worlds",recent_worlds.size() != 0);
		
		set_item_enabled("save_world",world_is_loaded);
		set_item_enabled("save_world_as",world_is_loaded);
		set_item_enabled("reload_world",world_is_loaded);
		set_item_enabled("panel_validator",world_is_loaded);
		set_item_enabled("world_quit",world_is_loaded);
		
		set_item_enabled("panel_tracker",world_is_loaded);
		
		set_item_enabled("importer",world_is_loaded && !brushes_is_enabled);
		set_item_enabled("creator",world_is_loaded && !brushes_is_enabled);
		
		set_item_enabled("edit",world_is_loaded);
		set_item_enabled("optimize",world_is_loaded);
		
		set_item_enabled("undo",undo_stack_size != 0);
		set_item_enabled("redo",undo_stack_size != 0);
		set_item_enabled("nodes_delete",node != NULL && !brushes_is_enabled);
		set_item_enabled("nodes_clone",node != NULL && !brushes_is_enabled);
		set_item_enabled("isolate_nodes",node != NULL && !brushes_is_enabled);
		
		set_item_enabled("group_selected",num_nodes > 1);
		set_item_enabled("convert_into_node_reference",num_nodes > 0);
		
		set_item_separator("panel_plugins",plugins_mb.getNumItems() > 1);
		
		#ifdef HAS_QT_EDITOR
			update_qt_menu();
		#endif
	}
	
	void callback_run(string key) {
		if(!items_callback.check(key)) return;
		ItemCallback itemcallback = items_callback[key];
		itemcallback.run();
	}
	
	void callback_clicked(WidgetMenuBox menubox) {
		
		int item = menubox.getCurrentItem();
		if(item == -1) return;
		
		string key = menubox.getItemData(item);
		if(key == "") return;
		
		callback_run(key);
	}
	
	void callback_recent_worlds_clicked() {
		int item = recent_worlds_mb.getCurrentItem();
		if(item == -1) return;
		if(!dialog_world_save()) return;
		ambientClear();
		world_load(recent_worlds[item]);
	}
	
	void update_items() {
		foreachkey(string key; items) {
			Unigine::Triple data = items[key];
			update_item(data.first,data.second,data.third);
		}
	}
	
	void update_item(MenuBox menu_box,int item,string text) {
		
		string hotkey_name = menu_box.getItemData(item);
		string hotkey_text = hotkeysGetText(hotkey_name);
		
		if(hotkey_text != "" && hotkey_text != "no hotkey") {
			menu_box.setFontRich(1);
			text = format("%s<right>    %s",text,hotkey_text);
		}
		
		MenuBox child = menu_box.getItemWidget(item);
		if(child != NULL) {
			menu_box.setFontRich(1);
			text = format("%s<right>    <font size=\"10\">►</font>",text);
		}
		
		menu_box.setItemText(item,text);
	}
	
	void set_item_enabled(string key,int mode) {
		if(items.check(key)) {
			Unigine::Triple data = items[key];
			MenuBox menu_box = data.first;
			menu_box.setItemEnabled(data.second,mode);
		}
		
		if(!menu_bar_items.check(key)) return;
		menu.setItemEnabled(menu_bar_items[key],mode);
	}
	
	void set_item_separator(string key,int mode) {
		if(items.check(key)) {
			Unigine::Triple data = items[key];
			MenuBox menu_box = data.first;
			menu_box.setItemSeparator(data.second,mode);
		}
	}
	
	int is_item_enabled(string key) {
		if(items.check(key)) {
			Unigine::Triple data = items[key];
			MenuBox menu_box = data.first;
			return menu_box.isItemEnabled(data.second);
		}
		
		if(!menu_bar_items.check(key)) return;
		return menu.isItemEnabled(menu_bar_items[key]);
	}
	
	string world_name(string name) {
		name = replace(name,relname(engine.getAppPath(),engine.getDataPath()),"");
		name = replace(name,engine.getDataPath(),"");
		name = replace(name,engine.getAppPath(),"");
		name = replace(name,".world","");
		return name;
	}
	
	int world_short_name(string &name) {
		int chars = 40;
		int hchars = chars / 2;
		if(strlen(name) < chars) return false;
		string ret[0];
		strsplit(name,"/",ret);
		string world = ret[ret.size() - 1];
		string path = ret[0];
		
		if(strlen(world) > hchars) world = substr(world,0,hchars) + "...";
		
		string all_path = "";
		int path_len = 0;
		
		forloop(int i = 0; ret.size() - 1) {
			path_len += strlen(ret[i]);
			all_path += "/" + ret[i];
		}
		if(path_len > hchars) {
			if(strlen(path) < hchars) {
				name = path + "/.../" + world;
				return true;
			} else {
				name = "/.../" + world;
				return true;
			}
		}
		name = all_path + "/" + world;
		return true;
	}
	
	int dialog_world_save() {
		if(engine.world.isLoaded() && unredoIsWorldDirty()) {
			int ret = dialogMessageYesNoCancel(TR("Confirm"),format(TR("Save \"%s\" World?"),engine.world.getName()));
			if(ret == 1) engine.console.run("world_save");
			if(ret == 0) return 0;
		}
		unredoResetState();
		return 1;
	}
	
	void world_load(string name) {
		name = world_name(name);
		ambientClear();
		engine.console.run(format("world_load \"%s\"",name));
	}
	
	void world_init() {
		
		string name = getCurrentWorldName();
		int id = recent_worlds.find(name);
		if(id != -1) recent_worlds.remove(id);
		
		recent_worlds.append(0,name);
		
		if(recent_worlds.size() > 5) recent_worlds.remove(5);
		
		update_recent_worlds();
	}
	
	void update_recent_worlds() {
		recent_worlds_mb.clear();
		
		string worlds[0];
		getAllWorlds(worlds);
		
		for(int i = 0; i < recent_worlds.size(); i++) {
			if(worlds.find(recent_worlds[i]) == -1) {
				recent_worlds.remove(i);
				i--;
				continue;
			}
			string world_name = recent_worlds[i];
			if(world_short_name(world_name)) {
				int id = recent_worlds_mb.addItem(world_name);
				recent_worlds_mb.setItemToolTip(id,recent_worlds[i]);
			} else recent_worlds_mb.addItem(world_name);
		}
	}
	
	void system_menu_toggle() {
		int activity = engine.system.call("systemGetActivity");
		if(activity) engine.system.call("systemHide");
		else engine.system.call("systemShow");
	}
	
	void create_world() {
		
		if(!dialog_world_save()) return;
		
		string name_world = "";
		string name_cpp = "";
		
		if(dialogFileWorld(TR("Select world to create"),name_world,DIALOG_FILE_SAVE)) {
			name_world = savePath(name_world);
			name_cpp = replace(name_world,".world","") + ".cpp";
			
			if(dialogFileConfirm(name_cpp) == 0) return;
			
			if(fileCopy(engine.filesystem.getFileName("editor/resources/template.world"),name_world) == 0) return;
			if(fileCopy(engine.filesystem.getFileName("editor/resources/template.cpp"),name_cpp) == 0) return;
			
			world_load(name_world);
		}
	}
	
	void save_world_as() {
		
		if(!engine.world.isLoaded()) return;
		
		string old_name_cpp = savePath(replace(engine.world.getName(),".world","") + ".cpp");
		string name_world = savePath(engine.world.getName() + ".world");
		string name_cpp = "";
		
		if(dialogFileWorld(TR("Select world to save"),name_world,DIALOG_FILE_SAVE)) {
			
			name_world = savePath(name_world);
			name_cpp = replace(name_world,".world","") + ".cpp";
			
			if(old_name_cpp == name_cpp) {
				engine.console.run("world_save");
				return;
			}
			
			if(dialogFileConfirm(name_cpp) == 0) return;
			
			engine.console.run(format("world_save \"%s\"",world_name(name_world)));
			
			if(fileCopy(old_name_cpp,name_cpp) == 0) return;
			
			world_load(name_world);
		}
	}
	
	void open_world() {
		
		if(!dialog_world_save()) return;
		
		string name = "";
		int add = 0;
		
		if(dialogWorld(TR("Select world to load"),name,add)) {
			if(add) {
				engine.world.addWorld(name + ".world");
			}
			else {
				world_load(name);
			}
		}
	}
	
	void save_world() {
		if(!engine.world.isLoaded()) return;
		if(dialogMessageYesNo(TR("Confirm"),format(TR("Save \"%s\" world?"),engine.world.getName()))) {
			engine.console.run("world_save");
		}
	}
	
	void editor_quit() {
		nodesDeselectNode();
		if(!dialog_world_save()) return;
		engine.console.run("editor_quit");
	}
	
	void exit() {
		if(!dialog_world_save()) return;
		
		engine.console.run("quit");
	}
	
	void world_quit() {
		if(!dialog_world_save()) return;
		if(engine.world.isLoaded()) {
			engine.console.run("world_quit");
		}
	}
	
	void reload_world() {
		if(!dialog_world_save()) return;
		if(engine.world.isLoaded()) {
			engine.console.run("world_reload");
		}
	}
	
	void set_profiler_mode(int mode) {
		engine.console.run(format("show_profiler %d",mode));
	}
	
	int get_height() {
		menu_gb.arrange();
		return menu_gb.getHeight();
	}
}

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void menuInit() {
	EditorMenu::init();
}

void menuShutdown() {
	EditorMenu::shutdown();
}

void menuWorldInit() {
	EditorMenu::world_init();
}

int menuGetHeight() {
	return EditorMenu::get_height();
}

void menuAddItem(string name,string key,string parent_key,int f = NULL,int args[] = ()) {
	EditorMenu::new_item(name,key,parent_key,f,args);
}

void menuAddBarItem(string name,string key) {
	EditorMenu::new_menu_bar_item(name,key);
}

void menuRemoveItem(string key,int mode = true) {
	EditorMenu::remove_item(key,mode);
}
/*
 */
void menuUpdate(int need_reload) {
	EditorMenu::update(need_reload);
}

void menuUpdateItems() {
	EditorMenu::update_items();
}

void menuCallbackRun(string key) {
	EditorMenu::callback_run(key);
}