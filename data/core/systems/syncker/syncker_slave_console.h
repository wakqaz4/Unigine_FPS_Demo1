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
#include <core/scripts/projection.h>
#include <core/systems/syncker/syncker_slave.h>

/******************************************************************************\
*
* Console commands
*
\******************************************************************************/

/*
 */
string slave_name = "unknown";
void slave_name_callback() { log.message("slave_name = %s\n",slave_name); }
void slave_name_callback(string name) { slave_name = name; }

/*
 */
string slave_mesh = "";
void slave_mesh_callback() { log.message("slave_mesh = %s\n",slave_mesh); }
void slave_mesh_callback(string mesh) { slave_mesh = mesh; }

/*
 */
int slave_grid_x = 1;
int slave_grid_y = 1;
void slave_grid_callback() { log.message("slave_grid = %dx%d\n",slave_grid_x,slave_grid_y); }
void slave_grid_callback(string grid_x,string grid_y) { slave_grid_x = max(int(grid_x),1); slave_grid_y = max(int(grid_y),1); }

/*
 */
int slave_view_x = 0;
int slave_view_y = 0;
void slave_view_callback() { log.message("slave_view = %dx%d\n",slave_view_x,slave_view_y); }
void slave_view_callback(string view_x,string view_y) { slave_view_x = max(int(view_x),0); slave_view_y = max(int(view_y),0); }

/*
 */
float slave_bezel_x = 0.0f;
float slave_bezel_y = 0.0f;
void slave_bezel_callback() { log.message("slave_bezel = %g %g\n",slave_bezel_x,slave_bezel_y); }
void slave_bezel_callback(string bezel_x,string bezel_y) { slave_bezel_x = float(bezel_x); slave_bezel_y = float(bezel_y); }

/*
 */
float slave_angle = 0.0f;
void slave_angle_callback() { log.message("slave_angle = %g\n",slave_angle); }
void slave_angle_callback(string angle) { slave_angle = float(angle); }

/******************************************************************************\
*
* Slave::Console
*
\******************************************************************************/

/*
 */
namespace Unigine::Syncker::Slave::Console {
	
	/*
	 */
	int init(string prefix = "") {
		
		// load console variables
		slave_name = engine.config.getString("slave_name",slave_name);
		slave_mesh = engine.config.getString("slave_mesh",slave_mesh);
		slave_grid_x = engine.config.getInt("slave_grid_x",slave_grid_x);
		slave_grid_y = engine.config.getInt("slave_grid_y",slave_grid_y);
		slave_view_x = engine.config.getInt("slave_view_x",slave_view_x);
		slave_view_y = engine.config.getInt("slave_view_y",slave_view_y);
		slave_bezel_x = engine.config.getFloat("slave_bezel_x",slave_bezel_x);
		slave_bezel_y = engine.config.getFloat("slave_bezel_y",slave_bezel_y);
		slave_angle = engine.config.getFloat("slave_angle",slave_angle);
		
		// parse command line arguments
		forloop(int i = 0; engine.getNumArgs()) {
			string arg = engine.getArg(i);
			if(arg == "-slave_name" && i + 1 < engine.getNumArgs()) slave_name = engine.getArg(++i);
			if(arg == "-slave_mesh" && i + 1 < engine.getNumArgs()) slave_mesh = engine.getArg(++i);
			if(arg == "-slave_grid_x" && i + 1 < engine.getNumArgs()) slave_grid_x = int(engine.getArg(++i));
			if(arg == "-slave_grid_y" && i + 1 < engine.getNumArgs()) slave_grid_y = int(engine.getArg(++i));
			if(arg == "-slave_view_x" && i + 1 < engine.getNumArgs()) slave_view_x = int(engine.getArg(++i));
			if(arg == "-slave_view_y" && i + 1 < engine.getNumArgs()) slave_view_y = int(engine.getArg(++i));
			if(arg == "-slave_bezel_x" && i + 1 < engine.getNumArgs()) slave_bezel_x = float(engine.getArg(++i));
			if(arg == "-slave_bezel_y" && i + 1 < engine.getNumArgs()) slave_bezel_y = float(engine.getArg(++i));
			if(arg == "-slave_angle" && i + 1 < engine.getNumArgs()) slave_angle = float(engine.getArg(++i));
		}
		
		// register console commands
		engine.console.addCommand("slave_name","slave name",prefix + "slave_name_callback");
		engine.console.addCommand("slave_mesh","slave mesh",prefix + "slave_mesh_callback");
		engine.console.addCommand("slave_grid","slave grid",prefix + "slave_grid_callback");
		engine.console.addCommand("slave_view","slave view",prefix + "slave_view_callback");
		engine.console.addCommand("slave_bezel","slave bezel",prefix + "slave_bezel_callback");
		engine.console.addCommand("slave_angle","slave angle",prefix + "slave_angle_callback");
		
		return 1;
	}
	
	/*
	 */
	int shutdown() {
		
		// save console variables
		engine.config.setString("slave_name",slave_name);
		engine.config.setString("slave_mesh",slave_mesh);
		engine.config.setInt("slave_grid_x",slave_grid_x);
		engine.config.setInt("slave_grid_y",slave_grid_y);
		engine.config.setInt("slave_view_x",slave_view_x);
		engine.config.setInt("slave_view_y",slave_view_y);
		engine.config.setFloat("slave_bezel_x",slave_bezel_x);
		engine.config.setFloat("slave_bezel_y",slave_bezel_y);
		engine.config.setFloat("slave_angle",slave_angle);
		
		// remove console commands
		engine.console.removeCommand("slave_name");
		engine.console.removeCommand("slave_mesh");
		engine.console.removeCommand("slave_grid");
		engine.console.removeCommand("slave_view");
		engine.console.removeCommand("slave_bezel");
		engine.console.removeCommand("slave_angle");
		
		return 1;
	}
	
} /* namespace Unigine::Syncker::Slave::Console */
