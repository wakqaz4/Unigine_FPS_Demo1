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


/******************************************************************************\
*
* Console commands
*
\******************************************************************************/

/*
 */
string master_address = "192.168.0.255";
void master_address_callback() { log.message("master_address = %s\n",master_address); }
void master_address_callback(string address) { master_address = address; }

/*
 */
float master_fps = 60.0f;
void master_fps_callback() { log.message("master_fps = %g\n",master_fps); }
void master_fps_callback(string fps) { master_fps = clamp(float(fps),10.0f,120.0f); }

/*
 */
void master_mesh_callback() {
	string slave_mesh[0];
	Unigine::Syncker::Master::editorGet(~0,"slave_mesh",slave_mesh);
	forloop(int i = 0; Unigine::Syncker::Master::getNumSlaves()) {
		log.message("%s (%s): %s\n",Unigine::Syncker::Master::getSlaveName(i),Unigine::Syncker::Master::getSlaveAddress(i),slave_mesh[i]);
	}
}

void master_mesh_callback(string mesh) {
	Unigine::Syncker::Master::systemCall(~0,"engine.console.run",(format("slave_mesh %s",mesh)));
}

/*
 */
void master_grid_callback() {
	int slave_grid_x[0];
	int slave_grid_y[0];
	Unigine::Syncker::Master::editorGet(~0,"slave_grid_x",slave_grid_x);
	Unigine::Syncker::Master::editorGet(~0,"slave_grid_y",slave_grid_y);
	forloop(int i = 0; Unigine::Syncker::Master::getNumSlaves()) {
		log.message("%s (%s): %dx%d\n",Unigine::Syncker::Master::getSlaveName(i),Unigine::Syncker::Master::getSlaveAddress(i),slave_grid_x[i],slave_grid_y[i]);
	}
}

void master_grid_callback(string grid_x,string grid_y) {
	Unigine::Syncker::Master::systemCall(~0,"engine.console.run",(format("slave_grid %s %s",grid_x,grid_y)));
}

/*
 */
void master_view_callback() {
	float slave_view_x[0];
	float slave_view_y[0];
	Unigine::Syncker::Master::editorGet(~0,"slave_view_x",slave_view_x);
	Unigine::Syncker::Master::editorGet(~0,"slave_view_y",slave_view_y);
	forloop(int i = 0; Unigine::Syncker::Master::getNumSlaves()) {
		log.message("%s (%s): %dx%d\n",Unigine::Syncker::Master::getSlaveName(i),Unigine::Syncker::Master::getSlaveAddress(i),slave_view_x[i],slave_view_y[i]);
	}
}

/*
 */
void master_bezel_callback() {
	float slave_bezel_x[0];
	float slave_bezel_y[0];
	Unigine::Syncker::Master::editorGet(~0,"slave_bezel_x",slave_bezel_x);
	Unigine::Syncker::Master::editorGet(~0,"slave_bezel_y",slave_bezel_y);
	forloop(int i = 0; Unigine::Syncker::Master::getNumSlaves()) {
		log.message("%s (%s): %g %g\n",Unigine::Syncker::Master::getSlaveName(i),Unigine::Syncker::Master::getSlaveAddress(i),slave_bezel_x[i],slave_bezel_y[i]);
	}
}

void master_bezel_callback(string bezel_x,string bezel_y) {
	Unigine::Syncker::Master::systemCall(~0,"engine.console.run",(format("slave_bezel %s %s",bezel_x,bezel_y)));
}

/*
 */
void master_angle_callback() {
	float slave_angle[0];
	Unigine::Syncker::Master::editorGet(~0,"slave_angle",slave_angle);
	forloop(int i = 0; Unigine::Syncker::Master::getNumSlaves()) {
		log.message("%s (%s): %g\n",Unigine::Syncker::Master::getSlaveName(i),Unigine::Syncker::Master::getSlaveAddress(i),slave_angle[i]);
	}
}

void master_angle_callback(string angle) {
	Unigine::Syncker::Master::systemCall(~0,"engine.console.run",(format("slave_angle %s",angle)));
}

/*
 */
void master_run_callback() {
	log.message("run console command on all slaves\n");
}

void master_run_callback(string command) {
	Unigine::Syncker::Master::systemCall(~0,"engine.console.run",(command));
}

void master_run_callback(string command,string a0) {
	Unigine::Syncker::Master::systemCall(~0,"engine.console.run",(command + " " + a0));
}

void master_run_callback(string command,string a0,string a1) {
	Unigine::Syncker::Master::systemCall(~0,"engine.console.run",(command + " " + a0 + " " + a1));
}

void master_run_callback(string command,string a0,string a1,string a2) {
	Unigine::Syncker::Master::systemCall(~0,"engine.console.run",(command + " " + a0 + " " + a1 + " " + a2));
}

void master_run_callback(string command,string a0,string a1,string a2,string a3) {
	Unigine::Syncker::Master::systemCall(~0,"engine.console.run",(command + " " + a0 + " " + a1 + " " + a2 + " " + a3));
}

/******************************************************************************\
*
* Master::Console
*
\******************************************************************************/

/*
 */
namespace Unigine::Syncker::Master::Console {
	
	/*
	 */
	int init(string prefix = "") {
		
		// load console variables
		master_address = engine.config.getString("master_address",master_address);
		master_fps = engine.config.getFloat("master_fps",master_fps);
		
		// parse command line arguments
		forloop(int i = 0; engine.getNumArgs()) {
			string arg = engine.getArg(i);
			if(arg == "-master_address" && i + 1 < engine.getNumArgs()) master_address = engine.getArg(++i);
			if(arg == "-master_fps" && i + 1 < engine.getNumArgs()) master_fps = float(engine.getArg(++i));
		}
		
		// register console commands
		engine.console.addCommand("master_address","master address",prefix + "master_address_callback");
		engine.console.addCommand("master_fps","master FPS",prefix + "master_fps_callback");
		engine.console.addCommand("master_mesh","master mesh",prefix + "master_mesh_callback");
		engine.console.addCommand("master_grid","master grid",prefix + "master_grid_callback");
		engine.console.addCommand("master_view","master view",prefix + "master_view_callback");
		engine.console.addCommand("master_bezel","master bezel",prefix + "master_bezel_callback");
		engine.console.addCommand("master_angle","master angle",prefix + "master_angle_callback");
		engine.console.addCommand("master_run","master console run",prefix + "master_run_callback");
		
		return 1;
	}
	
	/*
	 */
	int shutdown() {
		
		// save console variables
		engine.config.setString("master_address",master_address);
		engine.config.setFloat("master_fps",master_fps);
		
		// remove console commands
		engine.console.removeCommand("master_address");
		engine.console.removeCommand("master_fps");
		engine.console.removeCommand("master_mesh");
		engine.console.removeCommand("master_grid");
		engine.console.removeCommand("master_view");
		engine.console.removeCommand("master_bezel");
		engine.console.removeCommand("master_angle");
		engine.console.removeCommand("master_run");
		
		return 1;
	}
	
} /* namespace Unigine::Syncker::Master::Console */
