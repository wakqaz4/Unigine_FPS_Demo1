/* Copyright (C) 2005-2015, UNIGINE Corp. All rights reserved.
 *
 * This file is a part of the UNIGINE 2.0-RC SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE Corp. at http://unigine.com/
 */


#include <core/unigine.h>
#include <core/systems/syncker/syncker_master.h>
#include <core/systems/syncker/syncker_master_console.h>

/******************************************************************************\
*
* Plugin wrapper
*
\******************************************************************************/

/*
 */
int is_plugin = 0;

/*
 */
string getName() {
	is_plugin = 1;
	return "Master";
}

/*
 */
void update(int need_reload) {
	render();
}

/*
 */
string get_prefix() {
	if(is_plugin) return "Master::";
	return "";
}

/******************************************************************************\
*
* Console commands
*
\******************************************************************************/

/*
 */
int master_materials = 0;
void master_materials_callback() { log.message("master_materials = %d\n",master_materials); }
void master_materials_callback(string materials) { master_materials = int(materials); }

/*
 */
float master_radius = -1.0f;
void master_radius_callback() { log.message("master_radius = %g\n",master_radius); }
void master_radius_callback(string radius) { master_radius = max(float(radius),-1.0f); }

/******************************************************************************\
*
* Initialize/shutdown
*
\******************************************************************************/

/*
 */
int init() {
	
	// disable the editor
	if(is_plugin == 0) engine.editor.setEnabled(0);
	
	// load console variables
	master_materials = engine.config.getBool("master_materials",master_materials);
	master_radius = engine.config.getFloat("master_radius",master_radius);
	
	// register console commands
	engine.console.addCommand("master_materials","master materials",get_prefix() + "master_materials_callback");
	engine.console.addCommand("master_radius","master radius",get_prefix() + "master_radius_callback");
	
	// initialize syncker console
	Unigine::Syncker::Master::Console::init(get_prefix());
	
	// initialize syncker
	Unigine::Syncker::Master::init(master_address);
	
	return 1;
}

/*
 */
int shutdown() {
	
	// save console variables
	engine.config.setBool("master_materials",master_materials);
	engine.config.setFloat("master_radius",master_radius);
	
	// remove console commands
	engine.console.removeCommand("master_materials");
	engine.console.removeCommand("master_radius");
	
	// shutdown syncker console
	Unigine::Syncker::Master::Console::shutdown();
	
	// shutdown syncker
	Unigine::Syncker::Master::shutdown();
	
	return 1;
}

/******************************************************************************\
*
* Render
*
\******************************************************************************/

/*
 */
int render() {
	
	// fixed FPS
	engine.game.setFTime(1.0f / master_fps);
	
	// sync world
	string world_name;
	if(world_name != engine.world.getName()) {
		world_name = engine.world.getName();
		if(engine.world.isLoaded()) {
			
			// load slave world
			Unigine::Syncker::Master::systemCall(~0,"engine.console.run",("world_load " + world_name));
			
			// waiting for the slave world
			int ret[0];
			float begin = clock();
			while(clock() - begin < 4.0f) {
				int is_loaded = 1;
				Unigine::Syncker::Master::systemCall(~0,"engine.world.isLoaded",(),ret);
				foreach(int i = 0; ret) is_loaded &= i;
				if(is_loaded) break;
			}
		}
		else {
			
			// quit slave world
			Unigine::Syncker::Master::systemCall(~0,"engine.console.run",("world_quit"));
			
			// waiting for the slave quit
			int ret[0];
			float begin = clock();
			while(clock() - begin < 4.0f) {
				int is_loaded = 0;
				Unigine::Syncker::Master::systemCall(~0,"engine.world.isLoaded",(),ret);
				foreach(int i = 0; ret) is_loaded |= i;
				if(is_loaded == 0) break;
			}
		}
	}
	
	// check world
	if(engine.world.isLoaded() == 0) return 1;
	
	// sync frame
	Unigine::Syncker::Master::syncFrame();
	
	// sync player
	Unigine::Syncker::Master::syncPlayer();
	
	// sync render
	Unigine::Syncker::Master::syncRender();
	
	// sync materials
	if(master_materials != 0) {
		forloop(int i = 0; engine.materials.getNumWorldLibraries()) {
			int library = engine.materials.findLibrary(engine.materials.getWorldLibraryName(i));
			if(library != -1) {
				Material materials[0];
				forloop(int j = 0; engine.materials.getNumMaterials(library)) {
					Material material = engine.materials.getMaterial(library,j);
					materials.append(material);
				}
				Unigine::Syncker::Master::syncMaterials(materials);
			}
		}
	}
	
	// sync nodes
	Player player = engine.editor.getPlayer();
	if(player == NULL) player = engine.game.getPlayer();
	if(master_radius > 0.0f && player != NULL) {
		Node nodes[0];
		engine.world.getIntersectionNodes(new WorldBoundSphere(player.getWorldPosition(),Scalar(master_radius)),-1,nodes);
		forloop(int i = 0; nodes.size()) {
			Node possessor = nodes[i].getPossessor();
			if(possessor != NULL) nodes[i] = possessor;
		}
		Unigine::Syncker::Master::syncNodes(nodes);
	}
	
	// sync end
	Unigine::Syncker::Master::syncEnd();
	
	return 1;
}
