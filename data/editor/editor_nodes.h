/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_nodes.h
 * Desc:    Unigine editor
 * Version: 1.62
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
 *          Simonov Vasiliy <vvs@unigine.com>
 *          Anna Ishina <anet@unigne.com>
 *          Andrey Viktorov <unclebob@unigine.com>
 *          Mikhail Lyubimov <alterego@unigine.com>
 *          Vladimir Borovskikh <futurist@unigine.com>
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

#ifndef __EDITOR_NODES_H__
#define __EDITOR_NODES_H__

#include <editor/widgets/editor_widget_manipulator.h>

/*
 */
namespace Nodes {
	
	/*
	 */
	float MIN_SCALE = 0.001f;
	
	/*
	 */
	using Editor::Widgets;
	
	/*
	 */
	DECLARE_WINDOW
	
	WidgetHPaned window_hp;						// nodes hpaned
	
	WidgetComboBox type_cb;						// nodes type
	WidgetTreeBox nodes_tb;						// nodes treebox
	WidgetEditLine search_el;					// search field
	int nodes_folded[];							// nodes hidden flag
	
	Node current_nodes[];						// current nodes
	string current_names[];						// current names
	int current_items[];						// current items
	int current_type;							// current nodes type
	
	Node node;									// selected node
	Node nodes[0];								// selected nodes
	mat4 transform_buffer = 0;					// transformation buffer
	
	Node last_selected_node;					// selected node before deselect
	Node last_selected_surfaces[0];				// selected surfaces before deselect
	Node last_selected_nodes[0];				// selected nodes before deselect
	
	Node place_manually_node;
	Mat4 place_manually_transform;
	
	Vec3 nodes_world_center;
	
	WidgetEditorManipulatorTranslator translator_m;
	WidgetEditorManipulatorRotator rotator_m;
	WidgetEditorManipulatorScaler scaler_m;
	
	ManipulatorTransform::Logic transform_logic;
	
	WidgetTabBox parameters_tb;					// parameters tabbox
	
	WidgetVBox nodes_vb;						// nodes tab vbox
	WidgetLabel type_l;							// node type
	WidgetLabel id_l;							// node id
	WidgetCheckBox enabled_cb;					// enabled flag
	WidgetCheckBox handled_cb;					// handled flag
	WidgetCheckBox collider_cb;					// collider flag
	WidgetCheckBox clutter_cb;					// clutter flag
	WidgetCheckBox spatial_cb;					// spatial flag
	WidgetCheckBox query_cb;					// query flag
	WidgetCheckBox latest_cb;					// latest flag
	
	WidgetEditLine name_el;						// node name
	
	WidgetEditLine property_el;					// property name
	WidgetIcon property_load_i;					// property load
	WidgetIcon property_edit_i;					// property edit
	WidgetIcon property_clear_i;				// property clear
	
	WidgetEditLine position_el[3];				// node position
	WidgetSpinBox position_sb[3];
	WidgetIcon position_clear_i;
	
	WidgetEditLine rotation_el[3];				// node rotation
	WidgetSpinBox rotation_sb[3];
	WidgetIcon rotation_clear_i;
	
	WidgetEditLine scale_el[3];					// node scale
	WidgetSpinBox scale_sb[3];
	WidgetIcon scale_clear_i;
	
	int transform_focus = 0;					// transform widget focus out happened
	int auto_focus;								// focus is set programmatically
	
	WidgetButton copy_b;						// node copy
	WidgetButton paste_b;						// node paste
	WidgetButton place_here_b;					// node place here
	WidgetButton place_manually_b;				// node place manually
	WidgetButton close_up_b;					// node close up
	WidgetButton view_b;						// node view
	
	WidgetIcon clone_i;							// node clone
	WidgetIcon remove_i;						// node remove
	WidgetIcon up_i;							// node up
	WidgetIcon down_i;							// node down
	WidgetIcon fold_i;							// fold all nodes
	WidgetIcon save_i;							// node save
	WidgetIcon export_i;						// node export
	
	int old_type = -1;							// old node type
	int old_tabs[];								// old node tabs
	
	int node_world_types[] = (					// worlds
		NODE_WORLD_LAYER,
		NODE_WORLD_SECTOR,
		NODE_WORLD_PORTAL,
		NODE_WORLD_TRIGGER,
		NODE_WORLD_CLUSTER,
		NODE_WORLD_CLUTTER,
		NODE_WORLD_SWITCHER,
		NODE_WORLD_OCCLUDER,
		NODE_WORLD_OCCLUDER_MESH,
		NODE_WORLD_OCCLUDER_TERRAIN,
		NODE_WORLD_TRANSFORM_PATH,
		NODE_WORLD_TRANSFORM_BONE,
		NODE_WORLD_EXPRESSION,
		NODE_WORLD_EXTERN,
	);
	
	int node_field_types[] = (					// fields
		NODE_FIELD_SPACER,
		NODE_FIELD_ANIMATION,
	);
	
	int node_light_types[] = (					// lights
		NODE_LIGHT_PROB,
		NODE_LIGHT_SPOT,
		NODE_LIGHT_OMNI,
		NODE_LIGHT_PROJ,
		NODE_LIGHT_WORLD,
	);
	
	int node_decal_types[] = (					// decals
		NODE_DECAL_OBJECT_OMNI,
		NODE_DECAL_OBJECT_PROJ,
		NODE_DECAL_OBJECT_ORTHO,
		NODE_DECAL_TERRAIN_PROJ,
		NODE_DECAL_TERRAIN_ORTHO,
		NODE_DECAL_DEFERRED_PROJ,
		NODE_DECAL_DEFERRED_ORTHO,
		NODE_DECAL_DEFERRED_MESH,
	);
	
	int node_object_types[] = (					// objects
		NODE_OBJECT_DUMMY,
		NODE_OBJECT_DYNAMIC,
		NODE_OBJECT_MESH_STATIC,
		NODE_OBJECT_MESH_CLUSTER,
		NODE_OBJECT_MESH_CLUTTER,
		NODE_OBJECT_MESH_SKINNED,
		NODE_OBJECT_MESH_DYNAMIC,
		NODE_OBJECT_TERRAIN,
		NODE_OBJECT_GRASS,
		NODE_OBJECT_PARTICLES,
		NODE_OBJECT_BILLBOARD,
		NODE_OBJECT_BILLBOARDS,
		NODE_OBJECT_VOLUME_BOX,
		NODE_OBJECT_VOLUME_SPHERE,
		NODE_OBJECT_VOLUME_OMNI,
		NODE_OBJECT_VOLUME_PROJ,
		NODE_OBJECT_GUI,
		NODE_OBJECT_GUI_MESH,
		NODE_OBJECT_WATER,
		NODE_OBJECT_WATER_MESH,
		NODE_OBJECT_SKY,
		NODE_OBJECT_EXTERN,
	);
	
	int node_player_types[] = (					// players
		NODE_PLAYER_DUMMY,
		NODE_PLAYER_SPECTATOR,
		NODE_PLAYER_PERSECUTOR,
		NODE_PLAYER_ACTOR,
	);
	
	int node_physical_types[] = (				// physicals
		NODE_PHYSICAL_WIND,
		NODE_PHYSICAL_FORCE,
		NODE_PHYSICAL_NOISE,
		NODE_PHYSICAL_WATER,
		NODE_PHYSICAL_TRIGGER,
	);
	
	int node_navigation_types[] = (				// navigations
		NODE_NAVIGATION_SECTOR,
		NODE_NAVIGATION_MESH,
	);
	
	int node_obstacle_types[] = (				// obstacles
		NODE_OBSTACLE_BOX,
		NODE_OBSTACLE_SPHERE,
		NODE_OBSTACLE_CAPSULE,
	);
	
	int node_sound_types[] = (					// sounds
		NODE_SOUND_SOURCE,
		NODE_SOUND_REVERB,
	);
	
	string type_prefix[] = (
		NODE_DUMMY						: "Dummy::",
		NODE_LAYER						: "Layer::",
		NODE_PIVOT						: "Pivot::",
		NODE_TRIGGER					: "Trigger::",
		NODE_REFERENCE					: "Reference::",
		NODE_EXTERN						: "Extern::",
		NODE_WORLD_LAYER				: "World::Layer::",
		NODE_WORLD_SECTOR				: "World::Sector::",
		NODE_WORLD_PORTAL				: "World::Portal::",
		NODE_WORLD_TRIGGER				: "World::Trigger::",
		NODE_WORLD_CLUSTER				: "World::Cluster::",
		NODE_WORLD_CLUTTER				: "World::Clutter::",
		NODE_WORLD_SWITCHER				: "World::Switcher::",
		NODE_WORLD_OCCLUDER				: "World::Occluder::",
		NODE_WORLD_OCCLUDER_MESH		: "World::OccluderMesh::",
		NODE_WORLD_OCCLUDER_TERRAIN		: "World::OccluderTerrain::",
		NODE_WORLD_TRANSFORM_PATH		: "World::TransformPath::",
		NODE_WORLD_TRANSFORM_BONE		: "World::TransformBone::",
		NODE_WORLD_EXPRESSION			: "World::Expression::",
		NODE_WORLD_EXTERN				: "World::Extern::",
		NODE_FIELD_SPACER				: "Field::Spacer::",
		NODE_FIELD_ANIMATION			: "Field::Animation::",
		NODE_LIGHT_PROB					: "Light::Prob::",
		NODE_LIGHT_SPOT					: "Light::Spot::",
		NODE_LIGHT_OMNI					: "Light::Omni::",
		NODE_LIGHT_PROJ					: "Light::Proj::",
		NODE_LIGHT_WORLD				: "Light::World::",
		NODE_DECAL_OBJECT_OMNI			: "Decal::ObjectOmni::",
		NODE_DECAL_OBJECT_PROJ			: "Decal::ObjectProj::",
		NODE_DECAL_OBJECT_ORTHO			: "Decal::ObjectOrtho::",
		NODE_DECAL_TERRAIN_PROJ			: "Decal::TerrainProj::",
		NODE_DECAL_TERRAIN_ORTHO		: "Decal::TerrainOrtho::",
		NODE_DECAL_DEFERRED_PROJ		: "Decal::DeferredProj::",
		NODE_DECAL_DEFERRED_ORTHO		: "Decal::DeferredOrtho::",
		NODE_DECAL_DEFERRED_MESH		: "Decal::DeferredMesh::",
		NODE_OBJECT_DUMMY				: "Object::Dummy::",
		NODE_OBJECT_DYNAMIC				: "Object::Dynamic::",
		NODE_OBJECT_MESH_STATIC			: "Object::MeshStatic::",
		NODE_OBJECT_MESH_CLUSTER		: "Object::MeshCluster::",
		NODE_OBJECT_MESH_CLUTTER		: "Object::MeshClutter::",
		NODE_OBJECT_MESH_SKINNED		: "Object::MeshSkinned::",
		NODE_OBJECT_MESH_DYNAMIC		: "Object::MeshDynamic::",
		NODE_OBJECT_TERRAIN				: "Object::Terrain::",
		NODE_OBJECT_GRASS				: "Object::Grass::",
		NODE_OBJECT_PARTICLES			: "Object::Particles::",
		NODE_OBJECT_BILLBOARD			: "Object::Billboard::",
		NODE_OBJECT_BILLBOARDS			: "Object::Billboards::",
		NODE_OBJECT_VOLUME_BOX			: "Object::VolumeBox::",
		NODE_OBJECT_VOLUME_SPHERE		: "Object::VolumeSphere::",
		NODE_OBJECT_VOLUME_OMNI			: "Object::VolumeOmni::",
		NODE_OBJECT_VOLUME_PROJ			: "Object::VolumeProj::",
		NODE_OBJECT_GUI					: "Object::Gui::",
		NODE_OBJECT_GUI_MESH			: "Object::GuiMesh::",
		NODE_OBJECT_WATER				: "Object::Water::",
		NODE_OBJECT_WATER_MESH			: "Object::WaterMesh::",
		NODE_OBJECT_SKY					: "Object::Sky::",
		NODE_OBJECT_EXTERN				: "Object::Extern::",
		NODE_PLAYER_DUMMY				: "Player::Dummy::",
		NODE_PLAYER_SPECTATOR			: "Player::Spectator::",
		NODE_PLAYER_PERSECUTOR			: "Player::Persecutor::",
		NODE_PLAYER_ACTOR				: "Player::Actor::",
		NODE_PHYSICAL_WIND				: "Physical::Wind::",
		NODE_PHYSICAL_FORCE				: "Physical::Force::",
		NODE_PHYSICAL_NOISE				: "Physical::Noise::",
		NODE_PHYSICAL_WATER				: "Physical::Water::",
		NODE_PHYSICAL_TRIGGER			: "Physical::Trigger::",
		NODE_NAVIGATION_SECTOR			: "Navigation::Sector::",
		NODE_NAVIGATION_MESH			: "Navigation::Mesh::",
		NODE_OBSTACLE_BOX				: "Obstacle::Box::",
		NODE_OBSTACLE_SPHERE			: "Obstacle::Sphere::",
		NODE_OBSTACLE_CAPSULE			: "Obstacle::Capsule::",
		NODE_SOUND_SOURCE				: "Sound::Source::",
		NODE_SOUND_REVERB				: "Sound::Reverb::",
	);
	
	/*
	 */
	int run_function(string name) {
		int ret = 0;
		int type = node.getType();
		assert(type_prefix.check(type));
		if(is_function("Nodes::" + type_prefix[type] + name)) ret = call(type_prefix[type] + name);
		else ret = call(name);
		old_type = type;
		return ret;
	}
	
	int run_function(string name,Node node) {
		int ret = 0;
		int type = node.getType();
		assert(type_prefix.check(type));
		if(is_function("Nodes::" + type_prefix[type] + name,1)) ret = call(type_prefix[type] + name,node);
		else ret = call(name,node);
		old_type = type;
		return ret;
	}
	
	int run_old_function(string name) {
		int ret = 0;
		if(old_type != -1) {
			assert(type_prefix.check(old_type)):
			if(is_function("Nodes::" + type_prefix[old_type] + name)) ret = call(type_prefix[old_type] + name);
			else ret = call(name);
		}
		old_type = -1;
		return ret;
	}
	
	/*
	 */
	#include <editor/editor_nodes_unredo.h>
	#include <editor/editor_nodes_node.h>
	#include <editor/editor_worlds.h>
	#include <editor/editor_fields.h>
	#include <editor/editor_lights.h>
	#include <editor/editor_decals.h>
	#include <editor/editor_objects.h>
	#include <editor/editor_players.h>
	#include <editor/editor_physicals.h>
	#include <editor/editor_navigations.h>
	#include <editor/editor_obstacles.h>
	#include <editor/editor_sounds.h>
	
	/**************************************************************************\
	*
	* Node transformation
	*
	\**************************************************************************/
	
	/*
	 */
	namespace ManipulatorTransform {
		
		/* interface for scenario of using manipulators
		 */
		class Logic {
			
			/* when manipulator is clicked/grabbed with mouse
			 */
			void onGrab() = 0;
			
			/* when manipulator transform is changed
			 */
			void onChange() = 0;
			
			/* when manipulator is released
			 */
			void onRelease() = 0;
		};
		
		/*
		 */
		class Default : Logic {
			
			NodesState_ s;
			
			void onGrab() {
				s = create_state_modify(nodes,node);
			}
			
			void onChange() {
				if(s != NULL) {
					save_state(s);
					s = NULL;
				}
			}
			
			void onRelease() {
				if(s != NULL) delete s;
			}
		};
		
		/*
		 */
		class Clone : Logic {
			
			NodesState_ s;
			
			void onGrab() {
				s = clone_selection();
			}
			
			void onChange() {
			}
			
			void onRelease() {
				thread(functionid(commit_clone),s);
			}
		};
		
		/*
		 */
		class WithoutHierarchy : Logic {
			
			NodesState_ s;
			Unigine::Vector detached_children[];
			
			void onGrab() {
				s = create_state_modify(nodes,node);
				
				detach_children(detached_children);
			}
			
			void onChange() {
				if(s != NULL) {
					save_state(s);
					s = NULL;
				}
			}
			
			void onRelease() {
				reattach_children(detached_children);
				
				if(s != NULL) delete s;
			}
		};
		
		/*
		 */
		class CloneWithoutHierarchy : Logic {
			
			NodesState_ s;
			
			void onGrab() {
				Unigine::Vector detached_children[];
				detach_children(detached_children);
				
				s = clone_selection();
				
				reattach_children(detached_children);
			}
			
			void onChange() {
			}
			
			void onRelease() {
				thread(functionid(commit_clone),s);
			}
			
		};
		
		/* helper functions
		 */
		NodesState_ clone_selection() {
			Node selection[0];
			selection.copy(nodes);
			Node last_selected = node;
			
			clone_node();
			
			return create_state_new(nodes,selection,last_selected);
		}
		
		void commit_clone(NodesState_ s) {
			if(ask_clone_confirmation()) save_state(s);
			else cancel_clone(s);
		}
		
		int ask_clone_confirmation() {
			if(!toolsGetCloneConfirm()) return true;
			
			string question = format(TR("Clone \"%s\" node?"),nodes_tb.getItemText(nodes_tb.getSelectedItem(0)));
			return dialogMessageYesNo(TR("Confirm"),question);
		}
		
		void cancel_clone(NodesState_ s) {
			remove_childs(nodes);
			foreach(Node n; nodes) {
				remove_node(n,1);
			}
			
			update_nodes();
			s.select_nodes();
			engine.editor.needReload();
			delete s;
		}
		
		void detach_children(Unigine::Vector detached_children[]) {
			foreach(Node n; nodes) {
				for(int i = n.getNumChilds() - 1; i >= 0; i--) {
					Node child = n.getChild(i);
					n.removeWorldChild(child);
					Unigine::Vector v;
					if(detached_children.check(n) == 0) detached_children.append(n,new Unigine::Vector());
					v = detached_children[n];
					v.append(child);
				}
			}
		}
		
		void reattach_children(Unigine::Vector detached_children[]) {
			foreachkey(Node n; detached_children) {
				Unigine::Vector v = detached_children[n];
				for(int i = v.size() - 1; i >= 0; i--) {
					n.addWorldChild(v[i]);
				}
				v.clear();
			}
			detached_children.clear();
		}
		
	} /* namespace ManipulatorTransform */
	
	
	/*
	 */
	Mat4 start_transform;
	
	/*
	 */
	void set_nodes_vbox_callbacks_enabled(int mode) {
		if(mode) enableCallbacks(nodes_vb);
		else disableCallbacks(nodes_vb);
	}
	
	/*
	 */
	void update_transform() {
		
		if(node == NULL) return;
		
		// relative transformation
		Mat4 transform = node.getTransform();
		
		Vec3 p;
		vec3 r,s;
		Widget focus = gui.getFocus();
		decomposeTransform(transform,p,r,s);
		
		setCallbackEnabled(GUI_CHANGED,0,position_sb);
		setCallbackEnabled(GUI_CHANGED,0,rotation_sb);
		setCallbackEnabled(GUI_CHANGED,0,scale_sb);
		
		if(position_el[0] != focus && position_sb[0] != focus || auto_focus) position_el[0].setText(editorFormat(p.x));
		if(position_el[1] != focus && position_sb[1] != focus || auto_focus) position_el[1].setText(editorFormat(p.y));
		if(position_el[2] != focus && position_sb[2] != focus || auto_focus) position_el[2].setText(editorFormat(p.z));
		if(rotation_el[0] != focus && rotation_sb[0] != focus || auto_focus) rotation_el[0].setText(editorFormat(r.x));
		if(rotation_el[1] != focus && rotation_sb[1] != focus || auto_focus) rotation_el[1].setText(editorFormat(r.y));
		if(rotation_el[2] != focus && rotation_sb[2] != focus || auto_focus) rotation_el[2].setText(editorFormat(r.z));
		if(scale_el[0] != focus && scale_sb[0] != focus || auto_focus) scale_el[0].setText(editorFormat(s.x));
		if(scale_el[1] != focus && scale_sb[1] != focus || auto_focus) scale_el[1].setText(editorFormat(s.y));
		if(scale_el[2] != focus && scale_sb[2] != focus || auto_focus) scale_el[2].setText(editorFormat(s.z));
		position_clear_i.setEnabled(p != Vec3_zero);
		rotation_clear_i.setEnabled(r != Vec3_zero);
		scale_clear_i.setEnabled(s != Vec3_one);
		
		setCallbackEnabled(GUI_CHANGED,1,position_sb);
		setCallbackEnabled(GUI_CHANGED,1,rotation_sb);
		setCallbackEnabled(GUI_CHANGED,1,scale_sb);
		
		// world transformation
		transform = get_nodes_world_transform();
		start_transform = transform;
		
		update_manipulators_transform(transform);
		
		if(auto_focus) auto_focus = 0;
	}
	
	/*
	 */
	void update_manipulators_transform(Mat4 transform) {
		translator_m.setCallbackEnabled(MANIPULATOR_CHANGED,0);
		rotator_m.setCallbackEnabled(MANIPULATOR_CHANGED,0);
		scaler_m.setCallbackEnabled(MANIPULATOR_CHANGED,0);
		
		translator_m.setTransform(transform);
		rotator_m.setTransform(transform);
		scaler_m.setTransform(transform);
		
		translator_m.setCallbackEnabled(MANIPULATOR_CHANGED,1);
		rotator_m.setCallbackEnabled(MANIPULATOR_CHANGED,1);
		scaler_m.setCallbackEnabled(MANIPULATOR_CHANGED,1);
	}
	
	/*
	 */
	void update_decals(Node node) {
		if(node.isDecal()) {
			Decal decal = node_cast(node);
			Decal::update_decal(decal);
		}
		forloop(int i = 0; node.getNumChilds()) {
			Node child = node.getChild(i);
			update_decals(child);
		}
	}
	
	/*
	 */
	void set_transform(mat4 transform) {
		
		mat4 relative_transform = transform * inverse(start_transform);
		
		nodes_world_center = relative_transform * nodes_world_center;
		
		mat4 transforms[0];
		foreach(Node node; nodes) {
			transforms.append(node.getWorldTransform());
		}
		
		foreach(Node n, i = 0; nodes; i++) {
			
			apply_transform(n,relative_transform * transforms[i]);
			
			if(n.isObject()) {
				Object object = node_cast(n);
				Body body = object.getBody();
				if(body != NULL) body.setFrozen(0);
			}
			
			update_decals(n);
		}
		
		update_transform();
	}
	
	/*
	 */
	void set_object_transform(mat4 transform) {
		mat4 transforms[0];
		foreach(Node node; nodes) {
			transforms.append(node.getWorldTransform());
		}
		
		foreach(Node n, i = 0; nodes; i++) {
			transform.col33 = transforms[i].col33;
			start_transform.col33 = transforms[i].col33;
			mat4 relative_transform = transform * inverse(start_transform);
			
			apply_transform(n,relative_transform * transforms[i]);
			
			if(n.isObject()) {
				Object object = node_cast(n);
				Body body = object.getBody();
				if(body != NULL) body.setFrozen(0);
			}
			
			update_decals(n);
		}
		
		update_transform();
	}
	
	/*
	 */
	void apply_transform(Node n,Mat4 transform) {
		
		Vec3 t,r,s;
		decomposeTransform(Mat4(transform),t,r,s);
		
		if(!is_scalable(n)) {
			s = n.getWorldScale();
		}
		
		s = max(s,Vec3(MIN_SCALE));
		
		transform = composeTransform(t,r,s);
		n.setWorldTransform(transform);
	}
	
	/*
	 */
	int is_scalable(Node n) {
		
		switch(n.getType()) {
			case NODE_PIVOT:
			case NODE_LAYER:
			case NODE_TRIGGER:
			case NODE_PLAYER_PERSECUTOR:
			case NODE_OBJECT_BILLBOARD:
			case NODE_OBJECT_WATER:
			case NODE_OBJECT_PARTICLES:
			case NODE_FIELD_ANIMATION:
			case NODE_LIGHT_OMNI:
			case NODE_LIGHT_PROB:
			case NODE_LIGHT_PROJ:
			case NODE_LIGHT_SPOT:
			case NODE_LIGHT_WORLD:
			case NODE_WORLD_TRANSFORM_BONE:
			case NODE_WORLD_TRANSFORM_PATH:
			case NODE_WORLD_LAYER:
			case NODE_OBJECT_VOLUME_PROJ:
				return false;
				
			case NODE_PHYSICAL_TRIGGER: {
				PhysicalTrigger trigger = node_cast(n);
				return (trigger.getShapeType() != 0); // not sphere
			} // NODE_PHYSICAL_TRIGGER
				
			case NODE_OBJECT_DUMMY: {
				ObjectDummy dummy = node_cast(n);
				Body body = dummy.getBody();
				if(body == NULL) return true;
				
				switch(body.getType()) {
					case BODY_PATH: return false;
					default: return true;
				}
			} // NODE_OBJECT_DUMMY
				
			case NODE_OBJECT_MESH_STATIC: {
				ObjectMeshStatic mesh = node_cast(n);
				Body body = mesh.getBody();
				
				if(body == NULL) return true;
				
				switch(body.getType()) {
					case BODY_RIGID: return false;
					default: return true;
				}
			} // NODE_OBJECT_MESH_STATIC
				
			case NODE_OBJECT_MESH_DYNAMIC: {
				ObjectMeshDynamic mesh = node_cast(n);
				Body body = mesh.getBody();
				if(body == NULL) return true;
				
				switch(body.getType()) {
					case BODY_RIGID:
					case BODY_FRACTURE:
					case BODY_CLOTH:
					case BODY_ROPE:
						return false;
						
					default: return true;
				}
			} // NODE_OBJECT_MESH_DYNAMIC
				
			default:
				return true;
		}
	}
	
	/*
	 */
	void update_manipulators() {
		translator_m.setEnabled(node != NULL && panelPlacementGetTranslator());
		rotator_m.setEnabled(node != NULL && panelPlacementGetRotator());
		scaler_m.setEnabled(node != NULL && panelPlacementGetScaler());
	}
	
	ManipulatorTransform::Logic create_transform_logic() {
		int without_children = hotkeysClearModifierState("move_without_children");
		int clone = hotkeysClearModifierState("clone_node");
		
		if(!without_children && !clone) return new ManipulatorTransform::Default();
		else if(without_children && !clone) return new ManipulatorTransform::WithoutHierarchy();
		else if(!without_children && clone) return new ManipulatorTransform::Clone();
		else if(without_children && clone) return new ManipulatorTransform::CloneWithoutHierarchy();
		else return NULL;
	}
	
	void manipulator_clicked() {
		
		if(node == NULL) {
			return;
		}
		
		if(transform_logic == NULL) {
			transform_logic = create_transform_logic();
			
			assert(transform_logic != NULL);
			
			transform_logic.onGrab();
		}
	}
	
	void manipulator_released() {
		
		if(node == NULL) {
			return;
		}
		
		if(transform_logic != NULL) {
			transform_logic.onRelease();
			delete transform_logic;
		}
	}
	
	void manipulator_changed(WidgetEditorManipulator manipulator) {
	
		if(node == NULL) {
			return;
		}
		
		if(transform_logic != NULL) {
			
			transform_logic.onChange();
			
			if(MANIPULATOR_CENTER != panelPlacementGetManipulatorPivot() && (manipulator == scaler_m || manipulator == rotator_m)) {
				set_object_transform(manipulator.getTransform());
			} else {
				set_transform(manipulator.getTransform());
			}
		}
	}
	
	void translator_changed() {
		manipulator_changed(translator_m);
	}
	
	void rotator_changed() {
		manipulator_changed(rotator_m);
	}
	
	void scaler_changed() {
		manipulator_changed(scaler_m);
	}
	
	/*
	 */
	Vec3 get_world_bound_max(Node node) {
		return node.getWorldBoundBox().getMax();
	}
	
	Vec3 get_world_bound_min(Node node) {
		return node.getWorldBoundBox().getMin();
	}
	
	Vec3 get_world_bound_center(Node node) {
		return node.getWorldBoundSphere().getCenter();
	}
	
	Scalar get_bound_radius(Node node) {
		if(node.getType() == NODE_OBJECT_PARTICLES) {
			ObjectParticles particles = node_cast(node);
			return particles.getRadiusMean();
		}
		Scalar radius = node.getBoundSphere().getRadius();
		return radius < EPSILON ? 1.0f : radius;
	}
	
	Vec3 get_node_world_center(Node node) {
		if(node.getWorldBoundSphere().isValid() && node.getType() != NODE_LIGHT_WORLD) {
			return get_world_bound_center(node);
		}
		if(node.getWorldBoundBox().isValid() && node.getType() != NODE_LIGHT_WORLD) {
			return (node.getWorldBoundBox().getMin() + node.getWorldBoundBox().getMax()) * 0.5f;
		}
		return node.getWorldPosition();
	}
	
	Mat4 get_nodes_world_transform() {
		Mat4 transform = node.getWorldTransform();
		
		if(MANIPULATOR_CENTER == panelPlacementGetManipulatorPivot()) {
			transform.col33 = nodes_world_center;
		}
		
		return transform;
	}
	
	Scalar get_nodes_world_radius() {
		BoundSphere bs = new BoundSphere(vec3(get_node_world_center(node)),get_bound_radius(node));
		
		forloop(int i = 0; nodes.size()) {
			bs.expand(vec3(get_node_world_center(nodes[i])));
		}
		
		return bs.getRadius();
	}
	
	/*
	 */
	void place_here() {
		
		if(node == NULL) return;
		Player player = Unigine::getPlayer();
		if(player == NULL) return;
		
		vec3 offset = player.getWorldBoundSphere().getCenter() - get_world_bound_center(node);
		offset += player.getDirection() * get_bound_radius(node) * 2.0f;
		mat4 transform = translate(offset) * node.getWorldTransform();
		if(node.getTransform() != transform) {
			NodesState_ s = create_state_modify(nodes,node);
			save_state(s);
			set_transform(transform);
		}
	}
	
	/*
	 */
	int place_manually() {
		
		if(node == NULL) return 0;
		if(place_manually_node != NULL) return 0;
		if(gui.getMouseGrab()) return 0;
		
		panelPlacementSetSnapToSurface(1);
		translator_m.clearFocus();
		rotator_m.clearFocus();
		scaler_m.clearFocus();
		
		// remove focus
		Widget focus = gui.getFocus();
		if(focus != NULL) focus.removeFocus();
		
		hotkeysSetPriorityHotkeys((
			"place_node_relative_transform",
			"place_node_offset_reset",
			"place_node_decrease_offset",
			"place_node_increase_offset",
			"place_node_success",
			"place_node_cancel",
		));
		
		place_manually_node = node;
		place_manually_transform = get_nodes_world_transform();
		
		NodesState_ s = create_state_modify(nodes,place_manually_node);
		save_state(s);
		return 1;
	}
	
	int skip_callbacks_state = -1;
	
	/*
	 */
	void update_place_manually() {
		
		if(creatorGetNode() != NULL && place_manually_node == NULL) {
			
			place_manually_node = creatorGetNode();
			place_manually_transform = place_manually_node.getWorldTransform();
			
			hotkeysSetPriorityHotkeys((
				"place_node_relative_transform",
				"place_node_offset_reset",
				"place_node_decrease_offset",
				"place_node_increase_offset",
				"place_node_success",
				"place_node_cancel",
			));
		}
		
		if(place_manually_node == NULL) return;
		
		if(skip_callbacks_state == -1) skip_callbacks_state = hotkeysIsSkipCallbacks();
		
		if(place_manually_node != node) {
			
			if(creatorGetNode() != NULL) { // sometimes selection of newly added nodes is lost, it's hard to track where it happens
				select_node(creatorGetNode(),0);
			} else { // just in case something goes terribly wrong
				creatorRemove();
				delete_new_state();
				place_manually_node = NULL;
				
				gui.setMouseGrab(0);
				
				hotkeysSkipCallbacks(skip_callbacks_state);
				hotkeysResetPriorityHotkeys();
				skip_callbacks_state = -1;
				
				return;
			}
		}
		
		hotkeysSkipCallbacks(1);
		
		int ret = place_node(place_manually_node);
		
		if(ret == 1) {
			
			if(place_manually_node == creatorGetNode()) {
				creatorClear(0);
				save_new_state();
				select_node(place_manually_node,0);
			}
			
			place_manually_node = NULL;
			panelPlacementSetSnapToSurfaceOffset(0);
			panelPlacementSetSnapToSurfaceRelativeTransform(0);
			gui.setMouseGrab(0);
			
			hotkeysSkipCallbacks(skip_callbacks_state);
			hotkeysResetPriorityHotkeys();
			skip_callbacks_state = -1;
			
		} else if(ret == -1) {
			
			if(place_manually_node == creatorGetNode()) {
				creatorRemove();
				delete_new_state();
			} else {
				set_transform(place_manually_transform);
				update_decals(place_manually_node);
			}
			
			place_manually_node = NULL;
			panelPlacementSetSnapToSurfaceOffset(0);
			panelPlacementSetSnapToSurfaceRelativeTransform(0);
			gui.setMouseGrab(0);
			
			hotkeysSkipCallbacks(skip_callbacks_state);
			hotkeysResetPriorityHotkeys();
			skip_callbacks_state = -1;
		}
	}
	
	/*
	 */
	void drop_down(int mask,int normal_mode,int offset) {
		
		if(node == NULL) return;
		
		NodesState_ s = create_state_modify(nodes,node);
		save_state(s);
		
		// get hierarchy
		Node hierarchy[0];
		Vec3 positions[0];
		void get_nodes(Node node) {
			hierarchy.append(node);
			positions.append(node.getWorldPosition());
			forloop(int i = 0; node.getNumChilds()) {
				get_nodes(node.getChild(i));
			}
		}
		
		// get exclude
		Node exclude[0];
		void get_exclude(Node node) {
			exclude.append(node);
			forloop(int i = 0; node.getNumChilds()) {
				get_exclude(node.getChild(i));
			}
			if(node.getType() == NODE_REFERENCE) {
				NodeReference reference = node_cast(node);
				if(reference.getNode() != NULL) get_exclude(reference.getNode());
			}
			if(node.getType() == NODE_WORLD_LAYER) {
				WorldLayer layer = node_cast(node);
				forloop(int i = 0; layer.getNumNodes()) {
					Node layer_node = layer.getNode(i);
					if(layer_node != NULL) get_exclude(layer_node);
				}
			}
		}
		
		// get nodes
		foreach(Node node; nodes) {
			get_nodes(node);
			get_exclude(node);
		}
		
		// process nodes
		WorldIntersectionNormal intersection = new WorldIntersectionNormal();
		
		forloop(int i = 0; hierarchy.size()) {
			
			Node node = hierarchy[i];
			Vec3 p0 = positions[i];
			Vec3 p1 = p0 + normalize(engine.physics.getGravity()) * 1000.0f;
			
			Object object = engine.world.getIntersection(p0,p1,mask,exclude,intersection);
			
			if(object != NULL) {
				
				Vec3 point = Vec3(intersection.getPoint());
				
				if(normal_mode == DROP_DOWN_IGNORE_NORMAL) {
					node.setWorldPosition(point + normalize(-engine.physics.getGravity()) * offset);
				} else if(normal_mode == DROP_DOWN_ORIENT_BY_NORMAL) {
					
					Vec3 normal = Vec3(intersection.getNormal());
					
					Mat4 transform = node.getWorldTransform();
					transform *= orient_transform(rotation(transform),normal,Vec3(0.0f,0.0f,1.0f));
					node.setWorldTransform(transform);
					
					node.setWorldPosition(point + normal * offset);
				}
			}
		}
		
		calculate_world_center();
	}
	
	/*
	 */
	Mat4 orient_transform(Mat4 rotation,Vec3 orientation,Vec3 up) {
		
		up = up * inverse(rotation);
		float angle = acos(clamp(dot(up,orientation),-1.0f,1.0f)) * RAD2DEG;
		Vec3 pivot = normalize(cross(up,orientation)) * rotation;
		
		return Mat4(quat(pivot,angle));
	}
	
	/**************************************************************************\
	*
	* Node
	*
	\**************************************************************************/
	
	/*
	 */
	void set_node(Node n) {
		
		pluginsRun("nodeShutdown");
		run_old_function("shutdown");
		
		node = n;
		nodes.clear();
		
		if(node != NULL) {
			
			Vec3 center = get_node_world_center(node);
			Vec3 min_pos = center;
			Vec3 max_pos = center;
			
			forloop(int i = 0; nodes_tb.getNumSelectedItems()) {
				
				Node selected = current_nodes[nodes_tb.getSelectedItem(i)];
				
				Vec3 center = get_node_world_center(selected);
				min_pos = min(min_pos,center);
				max_pos = max(max_pos,center);
				
				nodes.append(selected);
			}
			
			nodes_world_center = (min_pos + max_pos) / 2.0f;
			
			panelPlacementSetScalerEnabled(is_scalable(node));
			
		} else {
			nodes_world_center = Vec3_zero;
			panelPlacementSetScalerEnabled(1);
		}
	}
	
	/*
	 */
	void calculate_world_center() {
		
		if(node != NULL) {
			
			Vec3 center = get_node_world_center(node);
			Vec3 min_pos = center;
			Vec3 max_pos = center;
			
			foreach(Node n; nodes) {
				
				Vec3 center = get_node_world_center(n);
				min_pos = min(min_pos,center);
				max_pos = max(max_pos,center);
			}
			
			nodes_world_center = (min_pos + max_pos) / 2.0f;
		} else {
			nodes_world_center = Vec3_zero;
		}
	}
	
	/*
	 */
	void reselect_surfaces(Node n,int s[]) {
		if(s.size() > 0) {
			Object::surface_select(-1,0);
			if(s.size()) {
				int surface = s[s.size() - 1];
				s.remove(s.size() - 1);
				foreach(int i; s) {
					if(i == surface) continue;
					Object::surface_select(i,1);
				}
				Object::surface_select(surface,1);
			}
		}
	}
	
	void reselect_nodes() {
		Node old_node = node;
		int surfaces[0] = ();
		if(old_node != NULL && old_node.isObject()) get_node(surfaces);
		Node old_nodes[0] = ();
		foreach(Node n; nodes) {
			old_nodes.append(n);
		}
		select_node(NULL,0);
		select_nodes(old_nodes,1);
		select_node(old_node,1);
		reselect_surfaces(old_node,surfaces);
	}
	
	/*
	 */
	void update_view(Node n = NULL,int toggle = 0) {
		Object::set_wireframe_mode(1);
		Node old_node = node;
		set_node(NULL);
		
		if(current_nodes.size() && nodes_tb.getNumSelectedItems()) {
			if(n == NULL) {
				set_node(current_nodes[nodes_tb.getSelectedItem(0)]);
			} else if(toggle == -1) {
				int in_selection = -1;
				forloop(int i = 0; nodes_tb.getNumSelectedItems()) {
					if(current_nodes[nodes_tb.getSelectedItem(i)] == old_node) {
						in_selection = i;
						break;
					}
				}
				if(in_selection != -1)
					set_node(old_node);
				else
					set_node(current_nodes[nodes_tb.getSelectedItem(0)]);
			} else {
				set_node(n);
			}
		}
		
		// clear tabs
		for(int i = parameters_tb.getNumTabs() - 1; i >= 1; i--) {
			parameters_tb.removeTab(i);
		}
		
		setCallbackEnabled(GUI_CLICKED,0,(
			enabled_cb,
			handled_cb,
			collider_cb,
			clutter_cb,
			spatial_cb,
			query_cb,
			latest_cb,
		));
		
		// hide fold tree icon
		fold_i.setEnabled((current_type == 0));
		
		// clear view
		if(node == NULL) {
			
			// parameters
			parameters_tb.setEnabled(0);
			
			type_l.setText("");
			id_l.setText("");
			enabled_cb.setChecked(0);
			handled_cb.setChecked(0);
			collider_cb.setChecked(0);
			clutter_cb.setChecked(0);
			spatial_cb.setChecked(0);
			query_cb.setChecked(0);
			latest_cb.setChecked(0);
			name_el.setText("");
			property_el.setText("");
			property_load_i.setEnabled(0);
			property_edit_i.setEnabled(0);
			property_clear_i.setEnabled(0);
			forloop(int i = 0; 3) {
				position_el[i].setText("");
				rotation_el[i].setText("");
				scale_el[i].setText("");
			}
			position_clear_i.setEnabled(0);
			rotation_clear_i.setEnabled(0);
			scale_clear_i.setEnabled(0);
			copy_b.setEnabled(0);
			paste_b.setEnabled(0);
			place_here_b.setEnabled(0);
			place_manually_b.setEnabled(0);
			close_up_b.setEnabled(0);
			view_b.setEnabled(0);
			
			// buttons
			setEnabled(0,(
				clone_i,
				remove_i,
				up_i,
				down_i,
				save_i,
				export_i,
			));
		}
		// fill view
		else {
			
			// parameters
			parameters_tb.setEnabled(1);
			
			type_l.setText(node.getTypeName());
			id_l.setText(format("ID:0x%08x",node.getID()));
			enabled_cb.setChecked(node.isEnabled());
			handled_cb.setChecked(node.isHandled());
			collider_cb.setChecked(node.isCollider());
			clutter_cb.setChecked(node.isClutter());
			spatial_cb.setChecked(node.isSpatial());
			query_cb.setChecked(node.isQuery());
			latest_cb.setChecked(node.isLatest());
			name_el.setText(node.getName());
			Property property = node.getProperty();
			property_load_i.setEnabled(1);
			if(property != NULL) {
				propertiesSelect(property.getName());
				property_el.setText(property.getName());
				property_edit_i.setEnabled(1);
				property_clear_i.setEnabled(1);
			} else {
				property_el.setText("");
				property_edit_i.setEnabled(0);
				property_clear_i.setEnabled(0);
			}
			copy_b.setEnabled((node != NULL && nodes.size() == 1));
			paste_b.setEnabled(is_int(transform_buffer) == 0 && (node != NULL || nodes.size() > 0));
			float r = get_bound_radius(node);
			place_here_b.setEnabled(r < INFINITY);
			place_manually_b.setEnabled(r < INFINITY);
			close_up_b.setEnabled(1);
			view_b.setEnabled(1);
			
			// init node
			run_function("init");
			run_function("update");
			pluginsRun("nodeInit");
			pluginsRun("nodeUpdate");
			
			// restore previous tab
			parameters_tb.setCurrentTab(clamp(old_tabs.check(node.getType(),0),0,parameters_tb.getNumTabs() - 1));
			
			// buttons
			setEnabled(1,(
				clone_i,
				remove_i,
				up_i,
				down_i,
				save_i,
				export_i,
			));
			
			// hide up and down icons
			if(current_type != 0) {
				setEnabled(0,( up_i, down_i ));
			}
			
			update_transform();
		}
		
		setCallbackEnabled(GUI_CLICKED,1,(
			enabled_cb,
			handled_cb,
			collider_cb,
			clutter_cb,
			spatial_cb,
			query_cb,
			latest_cb,
		));
		
		// update manipulators
		update_manipulators();
	}
	
	/*
	 */
	int get_child_position(Node n) {
		Node p = n.getParent();
		if(p == NULL) return -1;
		forloop(int i = 0; p.getNumChilds()) {
			if(p.getChild(i) == n) return i;
		}
		return 0;
	}
	
	/*
	 */
	void enabled_clicked() {
		
		void set_enabled_color(Node node) {
			if(node.isEnabled()) nodes_tb.setItemColor(current_items[node],nodes_tb.getFontColor());
			else nodes_tb.setItemColor(current_items[node],nodes_tb.getFontColor() * Vec4(0.5f,0.5f,0.5f,1.0f));
			
			forloop(int i = 0; node.getNumChilds()) {
				Node child = node.getChild(i);
				int item = current_items.check(child,-1);
				if(item == -1) continue;
				set_enabled_color(child);
			}
		}
		
		NodesState_ s = create_state_modify(nodes,node);
		
		foreach(Node node; nodes) {
			node.setEnabled(enabled_cb.isChecked());
			enabled_cb.setChecked(node.isEnabled());
			update_decals(node);
		}
		
		foreach(Node node; nodes) set_enabled_color(node);
		
		save_state(s);
	}
	
	/*
	 */
	CLICKED_CALLBACK_NODE(handled,Handled)
	CLICKED_CALLBACK_NODE(clutter,Clutter)
	CLICKED_CALLBACK_NODE(collider,Collider)
	CLICKED_CALLBACK_NODE(spatial,Spatial)
	CLICKED_CALLBACK_NODE(query,Query)
	CLICKED_CALLBACK_NODE(latest,Latest)
	
	/*
	 */
	void name_pressed() {
		int index = 0;
		string data[0],mask = "";
		string name = name_el.getText();
		if(re_search(name,"(%[0-9]*d)",data)) {
			mask = data[0];
			index = int(substr(mask,1,strlen(mask) - 2));
		}
		NodesState_ s = new NodesState_();
		foreach(Node node, i = index; nodes; i++) {
			if(node.getName() != replace(name,mask,string(i))) {
				s.init_modify(nodes,node);
				break;
			}
		}
		if(name != "") {
			foreach(Node node, i = index; nodes; i++) {
				int item = current_items[node];
				string new_name = replace(name,mask,string(i));
				nodes_tb.setItemText(item,new_name);
				node.setName(new_name);
			}
		} else {
			foreach(Node node; nodes) {
				int item = current_items[node];
				nodes_tb.setItemText(item,current_names[item]);
				node.setName("");
			}
		}
		save_state(s);
		
		panelsUpdateNodes();
	}
	
	/*
	 */
	void property_pressed() {
		NodesState_ s = new NodesState_();
		if(property_el.getText() != "") {
			Property property = node.getProperty();
			if(property == NULL || property.getName() != property_el.getText()) {
				s.init_modify(nodes,node);
				node.setProperty(property_el.getText());
				property = node.getProperty();
			}
			if(property == NULL) {
				property_edit_i.setEnabled(0);
				property_clear_i.setEnabled(0);
				dialogMessageOk(TR("Error"));
			} else {
				property_edit_i.setEnabled(1);
				property_clear_i.setEnabled(1);
			}
		} else {
			Property property = node.getProperty();
			if(property != NULL) s.init_modify(nodes,node);
			node.setProperty(NULL);
			property_edit_i.setEnabled(0);
			property_clear_i.setEnabled(0);
		}
		save_state(s);
	}
	
	void property_load_clicked() {
		NodesState_ s = new NodesState_();
		Property property = node.getProperty();
		string name = (property != NULL) ? property.getName() : "";
		if(dialogProperty(TR("Select property"),name)) {
			if(property == NULL || property.getName() != name) {
				s.init_modify(nodes,node);
				node.setProperty(name);
				property = node.getProperty();
			}
			if(property == NULL) {
				property_el.setText("");
				property_edit_i.setEnabled(0);
				property_clear_i.setEnabled(0);
				dialogMessageOk(TR("Error"));
			} else {
				propertiesSelect(name);
				property_el.setText(name);
				property_edit_i.setEnabled(1);
				property_clear_i.setEnabled(1);
			}
		}
		save_state(s);
	}
	
	void property_edit_clicked() {
		Property property = node.getProperty();
		if(property != NULL) propertiesEditor(property);
	}
	
	void property_clear_clicked() {
		NodesState_ s = new NodesState_();
		Property property = node.getProperty();
		if(property != NULL && dialogMessageYesNo(TR("Confirm"),TR("Clear property?"))) {
			s.init_modify(nodes,node);
			node.setProperty(NULL);
			property_el.setText("");
			property_edit_i.setEnabled(0);
			property_clear_i.setEnabled(0);
		}
		save_state(s);
	}
	
	/*
	 */
	int transform_el_focused() {
		if(transform_focus) {
			transform_focus = 0;
			return 1;
		}
		if(position_el.find(gui.getFocus()) != -1) return 1;
		if(rotation_el.find(gui.getFocus()) != -1) return 1;
		if(scale_el.find(gui.getFocus()) != -1) return 1;
		return 0;
	}
	
	int transform_sb_focused() {
		if(position_sb.find(gui.getFocus()) != -1) return 1;
		if(rotation_sb.find(gui.getFocus()) != -1) return 1;
		if(scale_sb.find(gui.getFocus()) != -1) return 1;
		return 0;
	}
	
	DECLARE_SPINBOX_CALLBACKS
	
	/*
	 */
	void transform_pressed() {
		if(node == NULL) return;
		
		NodesState_ s = new NodesState_();
		Node parent = node.getParent();
		
		// adjust scale to be non-zero
		vec3 scale = getEditLineVec3(scale_el);
		scale = max(scale,vec3(MIN_SCALE));
		Mat4 transform = composeTransform(getEditLineDVec3(position_el),getEditLineVec3(rotation_el),scale);
		
		if(transform_el_focused() && transform != node.getTransform()) s.init_modify(nodes,node);
		else if(transform_sb_focused()) s.init_modify(nodes,node);
		
		if(parent != NULL) transform = parent.getWorldTransform() * transform;
		
		start_transform = node.getWorldTransform();
		set_transform(transform);
		
		if(sb_skip_state == false) save_state(s,NULL);
		else delete s;
	}
	
	void transform_focus_out() {
		transform_focus = 1;
		transform_pressed();
	}
	
	void position_clear_clicked() {
		setEditLineDVec3(position_el,Vec3_zero);
		if(!transform_el_focused() && !transform_sb_focused()) position_el[0].setFocus();
		transform_pressed();
	}
	
	void rotation_clear_clicked() {
		setEditLineVec3(rotation_el,vec3_zero);
		if(!transform_el_focused() && !transform_sb_focused()) rotation_el[0].setFocus();
		transform_pressed();
	}
	
	void scale_clear_clicked() {
		setEditLineVec3(scale_el,vec3_one);
		if(!transform_el_focused() && !transform_sb_focused()) scale_el[0].setFocus();
		transform_pressed();
	}
	
	/*
	 */
	void copy_clicked() {
		if(node == NULL) return;
		transform_buffer = node.getWorldTransform();
		update_view();
	}
	
	void paste_clicked() {
		
		if(node == NULL && nodes.size() == 0) return;
		
		NodesState_ s = create_state_modify(nodes,node);
		save_state(s);
		
		foreach(Node n; nodes) {
			n.setWorldTransform(transform_buffer);
			update_decals(n);
		}
		
		update_view();
	}
	
	/*
	 */
	void place_here_clicked() {
		place_here();
	}
	
	void place_manually_clicked() {
		nodesHide();
		place_manually();
		nodesShow();
	}
	
	void close_up_clicked() {
		controlsShowNode();
	}
	
	void view_clicked() {
		if(node.getType() == NODE_REFERENCE) {
			NodeReference reference = node_cast(node);
			if(reference.getNode() != NULL) {
				dialogNodeView(nodes_tb.getItemText(nodes_tb.getSelectedItem(0)),node_cast(reference.getNode()));
			} else {
				dialogNodeView(nodes_tb.getItemText(nodes_tb.getSelectedItem(0)),node_cast(node));
			}
		} else {
			dialogNodeView(nodes_tb.getItemText(nodes_tb.getSelectedItem(0)),node_cast(node));
		}
	}
	
	/**************************************************************************\
	*
	* Nodes list
	*
	\**************************************************************************/
	
	int type_check(int type) {
		switch(type_cb.getCurrentItem()) {
			case 1:
				return node_world_types.find(type) != -1;
			case 2:
				return node_field_types.find(type) != -1;
			case 3:
				return node_light_types.find(type) != -1;
			case 4:
				return node_decal_types.find(type) != -1;
			case 5:
				return node_object_types.find(type) != -1;
			case 6:
				return node_player_types.find(type) != -1;
			case 7:
				return node_physical_types.find(type) != -1;
			case 8:
				return node_navigation_types.find(type) != -1;
			case 9:
				return node_obstacle_types.find(type) != -1;
			case 10:
				return node_sound_types.find(type) != -1;
		}
		return 1;
	}
	
	Node excluded_nodes[0] = ();
	/*
	 */
	void update_nodes() {
		
		nodes_tb.setCallbackEnabled(GUI_CHANGED,0);
		
		// clear nodes
		nodes_tb.clear();
		current_nodes.clear();
		current_names.clear();
		current_items.clear();
		excluded_nodes.clear();
		
		current_type = type_cb.getCurrentItem();
		
		// find nodes
		int types[0];
		forloop(int i = 0; engine.editor.getNumNodes()) {
			Node node = engine.editor.getNode(i);
			if(current_type != 0) {
				switch(current_type) {
					case 1: types = node_world_types; break;
					case 2: types = node_field_types; break;
					case 3: types = node_light_types; break;
					case 4: types = node_decal_types; break;
					case 5: types = node_object_types; break;
					case 6: types = node_player_types; break;
					case 7: types = node_physical_types; break;
					case 8: types = node_navigation_types; break;
					case 9: types = node_obstacle_types; break;
					case 10: types = node_sound_types; break;
				}
				
				if(types.find(node.getType()) == -1) {
					excluded_nodes.append(node);
					continue;
				}
			}
			
			// filter objects in isolation mode
			if(isolationIsEnabled() && isolationIsNode(node)) continue;
			
			int item = nodes_tb.addItem(node.getName(),node.getType());
			current_nodes.append(item,node);
			current_names.append(item,format("%s_%d",node.getTypeName(),i));
			current_items.append(node,item);
			
			if(node.getName() == "") nodes_tb.setItemText(item,current_names[item]); 
			
			if(node.isEnabled()) nodes_tb.setItemColor(current_items[node],nodes_tb.getFontColor());
			else nodes_tb.setItemColor(current_items[node],nodes_tb.getFontColor() * Vec4(0.5f,0.5f,0.5f,1.0f));
		}
		types = ();
		
		if(current_type == 0) {
			nodes_tb.setEditable(1);
			// hierarchy
			foreach(Node node; current_nodes) {
				Node parent = node.getParent();
				if(parent == NULL || current_items.check(parent) == 0) continue;
				nodes_tb.setItemParent(current_items[node],current_items[parent]);
			}
			
			// folded flag // enabled flag
			foreach(Node node; current_nodes) {
				nodes_tb.setItemFolded(current_items[node],node.isFolded());
			}
			
		} else {
			nodes_tb.setEditable(0);
		}
		
		nodes_tb.setCallbackEnabled(GUI_CHANGED,1);
		
		pluginsRun("nodesUpdate");
		
		camerasUpdate();
		panelsUpdateNodes();
	}
	
	/*
	 */
	void select_node(Node n,int toggle) {
		
		if(node == n && toggle != -1) return;
		
		if(toggle == 0) set_node(NULL);
		
		// select node
		nodes_tb.setCallbackEnabled(GUI_CHANGED,0);
		
		foreachkey(int id; current_nodes) {
			if(current_nodes[id] == n) {
				int parent = nodes_tb.getItemParent(id);
				while(parent != -1) {
					if(nodes_tb.isItemFolded(parent)) {
						nodes_tb.setItemFolded(parent,0);
					}
					parent = nodes_tb.getItemParent(parent);
				}
				if(toggle == -1) {
					nodes_tb.setItemSelected(id,0);
				} else {
					nodes_tb.setItemSelected(id,1);
					nodes_tb.showItem(id);
				}
			} else {
				if(toggle == 0) nodes_tb.setItemSelected(id,0);
			}
		}
		
		nodes_tb.setCallbackEnabled(GUI_CHANGED,1);
		
		// update view
		update_view(n,toggle);
	}
	
	/*
	 */
	void select_nodes(Node selected[],int toggle) {
		
		nodes_tb.setCallbackEnabled(GUI_CHANGED,0);
		
		if(toggle == 0) set_node(NULL);
		
		int selected_map[] = (:);
		if(!is_map(selected)) {
			forloop(int i = 0; selected.size()) {
				selected_map.append(selected[i]);
			}
		} else {
			selected_map = selected;
		}
		
		if(current_nodes.size() == 0) return;
		
		int last_id = 0;
		foreachkey(int id; current_nodes) {
			if(selected_map.check(current_nodes[id])) {
				int parent = nodes_tb.getItemParent(id);
				while(parent != -1) {
					if(nodes_tb.isItemFolded(parent)) {
						nodes_tb.setItemFolded(parent,0);
					}
					parent = nodes_tb.getItemParent(parent);
				}
				if(toggle == -1) {
					nodes_tb.setItemSelected(id,0);
				} else {
					nodes_tb.setItemSelected(id,1);
					last_id = id;
				}
			} else {
				if(toggle == 0) nodes_tb.setItemSelected(id,0);
			}
		}
		
		nodes_tb.showItem(last_id);
		nodes_tb.setCallbackEnabled(GUI_CHANGED,1);
		
		// update view
		update_view(current_nodes[last_id],toggle);
		
		if(node != NULL && node.isObject()) Object::surface_select_all();
	}
	
	void select_nodes_all() {
		
		if(current_nodes.size() == 0) return;
		
		nodes_tb.setCallbackEnabled(GUI_CHANGED,0);
		
		int last_id = 0;
		foreachkey(int id; current_nodes) {
			int parent = nodes_tb.getItemParent(id);
			while(parent != -1) {
				if(nodes_tb.isItemFolded(parent)) {
					nodes_tb.setItemFolded(parent,0);
				}
				parent = nodes_tb.getItemParent(parent);
			}
			
			if(current_nodes[id].getType() == NODE_LIGHT_WORLD)  continue;
			
			nodes_tb.setItemSelected(id,1);
			last_id = id;
		}
		nodes_tb.showItem(last_id);
		nodes_tb.setCallbackEnabled(GUI_CHANGED,1);
		
		// update view
		update_view(current_nodes[last_id],0);
	}
	
	/*
	 */
	void invert_selection_nodes() {
		
		if(current_nodes.size() == 0) return;
		
		nodes_tb.setCallbackEnabled(GUI_CHANGED,0);
		
		int last_id = 0;
		foreachkey(int id; current_nodes) {
			int parent = nodes_tb.getItemParent(id);
			while(parent != -1) {
				if(nodes_tb.isItemFolded(parent)) {
					nodes_tb.setItemFolded(parent,0);
				}
				parent = nodes_tb.getItemParent(parent);
			}
			
			if(current_nodes[id].getType() == NODE_LIGHT_WORLD)  continue;
			
			if(nodes_tb.isItemSelected(id)) {
				nodes_tb.setItemSelected(id,0);
			}
			else {
				nodes_tb.setItemSelected(id,1);
				last_id = id;
			}
		}
		nodes_tb.showItem(last_id);
		nodes_tb.setCallbackEnabled(GUI_CHANGED,1);
		
		// update view
		update_view(current_nodes[last_id],0);
	}
	
	/*
	 */
	void select_object(Object object,int surface,int toggle) {
		
		// select surface from current node
		if(node == object && nodes.size() == 1) {
			
			Object::surface_select(surface,toggle);
			
			// deselecting node when last surface was deselected
			if(Object::current_surfaces.size() == 0) {
				int id = current_nodes.find(object);
				nodes_tb.setItemSelected(id,0);
				update_view(object,toggle);
			}
			
			return;
		}
		
		// clear node
		if(toggle == 0) set_node(NULL);
		
		// select node
		nodes_tb.setCallbackEnabled(GUI_CHANGED,0);
		foreachkey(int id; current_nodes) {
			if(current_nodes[id] == object) {
				int parent = nodes_tb.getItemParent(id);
				while(parent != -1) {
					if(nodes_tb.isItemFolded(parent)) {
						nodes_tb.setItemFolded(parent,0);
					}
					parent = nodes_tb.getItemParent(parent);
				}
				if(toggle == -1) nodes_tb.setItemSelected(id,0);
				else {
					nodes_tb.setItemSelected(id,1);
					nodes_tb.showItem(id);
				}
			} else {
				if(toggle == 0) nodes_tb.setItemSelected(id,0);
			}
		}
		nodes_tb.setCallbackEnabled(GUI_CHANGED,1);
		
		// update view
		update_view(object,toggle);
		
		// select surface
		if(node == object) {
			Object::surface_select(surface,0);
		}
	}
	
	/*
	 */
	string get_info() {
		string info = "";
		if(node != NULL) {
			info += format("%s: ",node.getTypeName());
			if(node.getName() == "") info += nodes_tb.getItemText(nodes_tb.getSelectedItem(0));
			else info += node.getName();
		}
		return info;
	}
	
	string get_info(Node node) {
		string info = "";
		if(node != NULL) {
			info += format("%s: ",node.getTypeName());
			if(node.getName() == "") {
				if(current_items.check(node)) info += current_names[current_items[node]];
				else info += TR("(non-editor node)");
			} else {
				if(current_items.check(node) == 0) info += TR("(non-editor node) ");
				info += node.getName();
			}
		}
		return info;
	}
	
	Node get_node() {
		return node;
	}
	
	Node get_node(int surfaces[]) {
		Object::get_surfaces(surfaces);
		return node;
	}
	
	/*
	 */
	void clone_node() {
		
		Node clone[0];
		clone.clear();
		
		// clone node
		Node n = node_clone(node);
		if(n.getName() != "") n.setName(getNewNodeName(n.getName()));
		if(engine.editor.isNode(n.getParent()) == 0) n.setWorldParent(NULL);
		
		// add node
		engine.editor.addNode(n);
		
		// multiselection
		remove_childs(nodes);
		
		Node parent = NULL;
		Node new_node = NULL;
		Node path[0] = ();
		
		if(nodes.find(node) == -1) {
			remove_node(n,1);
			n = NULL;
			
			path.append(get_child_position(node));
			parent = node.getParent();
			while(nodes.find(parent) == -1) {
				path.append(get_child_position(parent));
				parent = parent.getParent();
			}
		}
		
		// clone
		foreach(Node node; nodes) {
			if(node != get_node()) {
				Node n = node_clone(node);
				if(n.getName() != "") n.setName(getNewNodeName(n.getName()));
				if(engine.editor.isNode(n.getParent()) == 0) n.setWorldParent(NULL);
				engine.editor.addNode(n);
				clone.append(n);
				
				// restore main node for manipulators
				if(node == parent) {
					new_node = n;
					foreach(int num; path) {
						new_node = new_node.getChild(num);
					}
				}
			}
		}
		
		if(place_manually_node == node) {
			place_manually_node = n;
		}
		
		update_nodes();
		select_nodes(clone,1);
		if(n != NULL) select_node(n,1);
		else select_node(new_node,1);
		engine.editor.needReload();
	}
	
	/*
	 */
	void remove_node(Node node,int force = 0) {
		if(engine.editor.isNode(node)) {
			
			if(node.isPlayer()) {
				camerasRemoveCamera(node);
			}
			
			for(int i = node.getNumChilds() - 1; i >= 0; i--) {
				remove_node(node.getChild(i));
			}
			int ret = 0;
			if(force == 1) {
				node.setEnabled(0);
				node.setWorldParent(NULL);
				ret = engine.editor.releaseNode(node);
			} else {
				ret = engine.editor.releaseNode(node);
			}
			assert(ret == 1);
		}
	}
	
	void remove_node() {
		if(node == NULL) return;
		if(node.isPlayer()) {
			camerasRemoveCamera(node);
		}
		remove_childs(nodes);
		
		foreach(Node node; nodes) {
			remove_node(node);
		}
		
		update_nodes();
		select_node(NULL,0);
		engine.editor.needReload();
	}
	
	/*
	 */
	void swap_node(string dir) {
		
		if(node == NULL) return;
		
		Node n = get_swap_candidate(dir,node);
		
		if(n == NULL) return;
		
		NodesState_ s = create_state_swap(nodes,node,(dir == "up") ? "down" : "up");
		engine.editor.swapNodes(node,n);
		swap_childs(node,n);
		save_state(s);
		update_nodes();
		
		// select node again properly
		nodes_tb.setCallbackEnabled(GUI_CHANGED,0);
		foreachkey(int id; current_nodes) {
			if(current_nodes[id] == node) {
				nodes_tb.setItemSelected(id,1);
				nodes_tb.showItem(id);
			} else {
				nodes_tb.setItemSelected(id,0);
			}
		}
		nodes_tb.setCallbackEnabled(GUI_CHANGED,1);
		
		engine.editor.needReload();
	}
	
	/*
	 */
	void type_changed() {
		update_nodes();
		update_view();
	}
	
	void nodes_changed() {
		
		// old node
		Node old_node = node;
		int old_surfaces[0] = ();
		if(node != NULL && node.isObject()) {
			get_node(old_surfaces);
		}
		
		// old selection
		Node old_selection[0] = ();
		old_selection.copy(nodes);
		
		// list of old nodes and their parents
		Node old_nodes[0] = ();
		Node old_parents[0] = ();
		int old_numbers[0] = ();
		int old_children[] = (:);
		
		foreach(Node n; current_nodes) {
			Node p = n.getParent();
			if(p != NULL && current_items.check(p) == 0) continue;
			old_nodes.append(n);
			old_parents.append(p);
			if(p != NULL) {
				Unigine::Map children = old_children.check(p,NULL);
				if(children == NULL) {
					children = new Unigine::Map();
					int children_data[] = children.data;
					forloop(int i = 0; p.getNumChilds()) {
						children_data.append(p.getChild(i),i);
					}
					old_children.append(p,children);
				}
				old_numbers.append(children[n]);
			} else {
				old_numbers.append(-1);
			}
		}
		old_children.delete();
		
		nodes_tb.setCallbackEnabled(GUI_CHANGED,0);
		
		if(current_type == 0) {
			int reparent = 0;
			Node root = referenceGetRoot();
			forloop(int i = 0; nodes_tb.getNumSelectedItems()) {
				int item = nodes_tb.getSelectedItem(i);
				int parent = nodes_tb.getItemParent(item);
				Node node = current_nodes[item];
				Node old_parent = node.getParent();
				if(parent == -1 || parent == item) {
					if(root != NULL || !isolationIsEnabled() || !isolationRestoreParentNode(node)) {
						node.setWorldParent(root);
						if(old_parent != root) reparent = 1;
					}
				} else if(old_parent != current_nodes[parent]) {
					if(current_nodes[parent] != NULL || !isolationIsEnabled() || !isolationRestoreParentNode(node)) {
						node.setWorldParent(current_nodes[parent]);
						reparent = 1;
					}
				}
			}
			
			foreach(Node node; current_nodes) {
				node.setFolded(nodes_tb.isItemFolded(current_items[node]));
				if(node.isEnabled()) nodes_tb.setItemColor(current_items[node],nodes_tb.getFontColor());
				else nodes_tb.setItemColor(current_items[node],nodes_tb.getFontColor() * Vec4(0.5f,0.5f,0.5f,1.0f));
			}
			
			if(reparent) {
				update_nodes();
				reselect_nodes();
				engine.editor.needReload();
			}
		}
		
		nodes_tb.setCallbackEnabled(GUI_CHANGED,1);
		
		update_view();
		
		// fix for undo/redo if tree sorted by type
		if(current_type != 0) return;
		
		// save selection for undo
		NodesState_ s = new NodesState_();
		if(nodes.size() != old_selection.size()) {
			s.init_select(old_selection,old_node,old_surfaces);
		} else {
			forloop(int i = 0; nodes.size()) {
				if(nodes[i] != old_selection[i]) {
					s.init_select(old_selection,old_node,old_surfaces);
					break;
				}
			}
		}
		save_state(s);
		
		// filter nodes which parents have changed
		for(int i = old_nodes.size() - 1; i >= 0; i--) {
			if(old_nodes[i] == NULL || old_nodes[i].getParent() == old_parents[i] || nodes.find(old_nodes[i]) == -1 && old_nodes[i] != node) {
				old_nodes.remove(i);
				old_numbers.remove(i);
				old_parents.remove(i);
			}
		}
		
		if(old_nodes.size() == 0) {
			return;
		}
		
		// save reparenting for undo
		Node new_parents[0] = ();
		foreach(Node n; old_nodes) {
			new_parents.append(n.getParent());
		}
		s = create_state_reparent(old_nodes,old_numbers,old_parents,new_parents,nodes,node);
		save_state(s);
	}
	
	void nodes_double_clicked() {
		forloop(int i = 0; nodes_tb.getNumSelectedItems()) {
			int item = nodes_tb.getSelectedItem(i);
			nodes_tb.setItemFolded(item,!nodes_tb.isItemFolded(item));
		}
	}
	
	/**************************************************************************\
	*
	* Buttons
	*
	\**************************************************************************/
	
	/*
	 */
	void clone_clicked() {
		if(node == NULL) return;
		if(dialogMessageYesNo(TR("Confirm"),format(TR("Clone \"%s\" node?"),nodes_tb.getItemText(nodes_tb.getSelectedItem(0)))) == 0) return;
		Node selection[0];
		selection.copy(nodes);
		Node last_selected = node;
		clone_node();
		NodesState_ s = create_state_new(nodes,selection,last_selected);
		save_state(s);
	}
	
	void remove_clicked() {
		if(node == NULL) return;
		
		string message = format(TR("Remove \"%s\" node"),nodes_tb.getItemText(nodes_tb.getSelectedItem(0)));
		if(node.getNumChilds()) message += TR(" with children");
		if(dialogMessageYesNo(TR("Confirm"),message + "?")) {
			NodesState_ s = create_state_remove(nodes,nodes,node);
			save_state(s);
			remove_node();
		}
	}
	
	void save_clicked() {
		if(node == NULL) return;
		string name;
		if(dialogFileNode(TR("Select node to save"),name,DIALOG_FILE_SAVE)) {
			name = savePath(name);
			
			Vec3 position = node.getPosition();
			node.setPosition(Vec3_zero);
			if(engine.world.saveNode(name,node,toolsGetNodeMaterials()) == 0) dialogMessageOk(TR("Error"));
			node.setPosition(position);
		}
	}
	
	void export_clicked() {
		if(node == NULL) return;
		string name;
		if(dialogFileMesh(TR("Select mesh to save"),name,DIALOG_FILE_SAVE)) {
			name = savePath(name);
			
			Node objects[0];
			void get_childs(Node n) {
				if(n.isObject()) objects.append(n);
				for(int i = 0; i < n.getNumChilds(); i++) {
					get_childs(n.getChild(i));
				}
			}
			foreach(Node n; nodes) {
				if(n.isObject()) objects.append(n);
				else if(n.getType() == NODE_REFERENCE) {
					NodeReference reference = node_cast(n);
					if(reference.getNode() != NULL) {
						get_childs(reference.getNode());
					}
				}
			}
			if(strstr(name,".mesh") == -1) name += ".mesh";
			if(engine.editor.saveMesh(name,objects) == 0) dialogMessageOk(TR("Error"));
		}
	}
	
	void swap_clicked(string dir) {
		swap_node(dir);
	}
	
	/*
	 */
	void fold_clicked() {
		
		deselect_node();
		forloop(int i = 0; engine.editor.getNumNodes()) {
			Node node = engine.editor.getNode(i);
			if(node.getNumChilds()) node.setFolded(1);
		}
		update_nodes();
	}
	
	/*
	 */
	void search_changed() {
		search_el.setCallbackEnabled(GUI_CHANGED,0);
		nodes_tb.setCallbackEnabled(GUI_CHANGED,0);
		restoreTreeBoxFolded(nodes_tb,nodes_folded);
		selectTreeBoxByName(nodes_tb,search_el);
		update_view();
		search_el.setCallbackEnabled(GUI_CHANGED,1);
		nodes_tb.setCallbackEnabled(GUI_CHANGED,1);
	}
	
	/*
	 */
	void search_focus_in() {
		saveTreeBoxFolded(nodes_tb,nodes_folded);
	}
	
	void search_focus_out() {
		nodes_folded.clear();
	}
	
	/**************************************************************************\
	*
	* Hot keys
	*
	\**************************************************************************/
	
	/*
	 */
	void deselect_node() {
		if(node != NULL) {
			
			if(node.isObject()) {
				Object::get_current_surfaces(last_selected_surfaces);
			}
			last_selected_node = node;
			last_selected_nodes.copy(nodes);
			
			set_node(NULL);
			update_nodes();
			update_view();
		}
	}
	
	/*
	 */
	void isolate_clicked() {
		if(isolationIsEnabled()) isolationDisable();
		else isolationEnable();
	}
	
	/*
	 */
	void convert_into_node_reference() {
		
		string name;
		if(dialogFileNode(TR("Select node to save"),name,DIALOG_FILE_SAVE) == false) return;
		name = savePath(name);
		
		NodesStateMany_ state = new NodesStateMany_();
		
		Node group;
		if(nodes.size() > 1) {
			NodesStateMany_ group_state;
			group = create_dummy_group(group_state);
			state.append(group_state);
		} else {
			group = node;
		}
		
		if(group == NULL) return;
		
		Vec3 position = group.getWorldPosition();
		group.setPosition(Vec3_zero);
		
		if(engine.world.saveNode(name,group,toolsGetNodeMaterials()) == false) {
			dialogMessageOk(TR("Error"));
			group.setWorldPosition(position);
			return;
		}
		
		group.setWorldPosition(position);
		Node parent = group.getParent();
		
		NodeReference ref = node_remove(new NodeReference(name));
		
		if(ref == NULL) {
			group.setWorldPosition(position);
			return;
		}
		
		NodesState_ states[0];
		
		states.append(create_state_new((ref),nodes,node));
		states.append(create_state_remove((group),nodes,node));
		
		engine.editor.addNode(ref);
		move_to_position(ref,get_node_position(group));
		update_nodes();
		
		if(parent != NULL) ref.setWorldParent(parent);
		ref.setWorldPosition(position);
		
		NodesStateMany_ convert_state = create_state_many(states);
		state.append(convert_state);
		
		remove_node(group,1);
		engine.world.addUpdateNode(ref);
		
		// update node
		nodesUpdate(1);
		nodesSelectNew(ref,0);
		
		unredoAddState(state);
	}
	
	/**************************************************************************\
	*
	* Dummy grouping
	*
	\**************************************************************************/
	
	/*
	 */
	void get_descendants(Node root,Node descendants[]) {
		
		Node this_generation[0] = ( root );
		Node next_generation[0] = ();
		
		while(this_generation.size()) {
			
			foreach(Node n; this_generation) {
				forloop(int i = 0; n.getNumChilds()) {
					Node child = n.getChild(i);
					next_generation.append(child);
					descendants.append(child);
				}
			}
			
			this_generation.copy(next_generation);
			next_generation.clear();
		}
	}
	
	/*
	 */
	void filter_selected_trees(Node selected_nodes[]) {
		
		// preparations
		int selection_map[] = (:);
		foreach(Node n; selected_nodes) selection_map.append(n,1);
		
		int selected_trees[] = (:); // sort-of-memoization for better performance
		
		int tree_selected(Node n) {
			
			if(selected_trees.check(n)) return selected_trees[n];
			
			if(!selection_map.check(n)) {
				selected_trees[n] = false;
				return false;
			}
			
			forloop(int i = 0; n.getNumChilds()) {
				if(!tree_selected(n.getChild(i))) {
					selected_trees[n] = false;
					return false;
				}
			}
			
			selected_trees[n] = true;
			return true;
		}
		
		// algorithm
		int result_map[] = (:);
		forloop(int i = 0; selected_nodes.size()) result_map.append(selected_nodes[i],1);
		
		for(int i = 0; i < selected_nodes.size(); i++) {
			Node n = selected_nodes[i];
			
			if(n.getNumChilds() > 0 && tree_selected(n)) {
				
				// looking for root of selected sub-tree
				Node self = n;
				Node parent = self.getParent();
				
				while(parent != NULL && tree_selected(parent)) {
					self = parent;
					parent = self.getParent();
				}
				
				Node descendants[0];
				get_descendants(self,descendants);
				
				foreach(Node d; descendants) result_map.remove(d);
			}
		}
		
		selected_nodes.clear();
		foreachkey(Node n; result_map) selected_nodes.append(n);
		result_map.delete();
	}
	
	/*
	 */
	Node create_dummy_group(NodesStateMany_ &ss) {
		
		if(nodes.size() == 0) return NULL;
		
		// find dummy parent
		Node parent = node.getParent();
		
		while(parent != NULL) {
			int i = 0;
			
			forloop(i = 0; nodes.size()) {
				if(parent.isChild(nodes[i]) == 0) {
					parent = parent.getParent();
					break;
				}
			}
			
			if(i == nodes.size()) break;
		}
		
		// find dummy position
		Vec3 minimum = get_world_bound_min(node);
		Vec3 maximum = get_world_bound_max(node);
		int min_node_pos = get_node_position(node);
		
		foreach(Node n; nodes) {
			minimum = min(minimum,get_world_bound_min(n));
			maximum = max(maximum,get_world_bound_max(n));
			min_node_pos = min(min_node_pos,get_node_position(n));
		}
		
		// create dummy
		NodeDummy dummy = class_remove(new NodeDummy());
		dummy.setWorldTransform(translate((minimum + maximum) / 2.0f));
		engine.editor.addNode(dummy);
		dummy.setWorldParent(parent);
		dummy.setName(getNewNodeName("group"));
		move_to_position(dummy,min_node_pos);
		update_nodes();
		
		NodesState_ states[0] = ();
		
		int surfaces[0] = ();
		if(node.isObject()) {
			Object::get_current_surfaces(surfaces);
		}
		NodesState_ s = create_state_select(nodes,node,surfaces);
		states.append(s);
		
		s = create_state_new(( dummy ),nodes,node);
		states.append(s);
		
		// when whole sub-tree of nodes is selected only its root is reparented
		Node nodes_to_group[0];
		nodes_to_group.copy(nodes);
		filter_selected_trees(nodes_to_group);
		
		Node old_parents[0] = ();
		Node new_parents[0] = ();
		int old_numbers[0] = ();
		
		foreach(Node n; nodes_to_group) {
			parent = n.getParent();
			old_parents.append(parent);
			
			if(parent == NULL) old_numbers.append(get_node_position(n));
			else old_numbers.append(get_child_position(n));
			
			n.setWorldParent(dummy);
			new_parents.append(dummy);
		}
		
		update_nodes();
		reselect_nodes();
		engine.editor.needReload();
		
		s = create_state_reparent(nodes_to_group,old_numbers,old_parents,new_parents,nodes,node);
		states.append(s);
		
		ss = create_state_many(states);
		return dummy;
	}
	
	/*
	 */
	void create_nodes_group() {
		NodesStateMany_ ss;
		create_dummy_group(ss);
		unredoAddState(ss);
	}
	
	/*
	 */
	int place_node(Node node) {
		
		int mouse_grab;
		int relative = panelPlacementGetSnapToSurfaceRelativeTransform();
		int offset = panelPlacementGetSnapToSurfaceOffset();
		
		Vec3 p0,p1;
		Unigine::getPlayerMouseDirection(p0,p1);
		
		// nodes hierarchy
		Node hierarchy[0];
		forloop(int i = 0; nodes.size()) {
			hierarchy.append(nodes[i]);
		}
		
		void get_hierarchy(Node node) {
			forloop(int i = 0; node.getNumChilds()) {
				get_hierarchy(node.getChild(i));
			}
			hierarchy.append(node);
			if(node.getType() == NODE_REFERENCE) {
				NodeReference reference = node_cast(node);
				if(reference.getNode() != NULL) get_hierarchy(reference.getNode());
			}
			if(node.getType() == NODE_WORLD_LAYER) {
				WorldLayer layer = node_cast(node);
				forloop(int i = 0; layer.getNumNodes()) {
					Node layer_node = layer.getNode(i);
					if(layer_node != NULL) get_hierarchy(layer_node);
				}
			}
		}
		
		get_hierarchy(node);
		
		// world intersection
		Vec3 ret_point;
		vec3 ret_normal;
		WorldIntersectionNormal intersection = new WorldIntersectionNormal();
		
		if(engine.world.getIntersection(p0,p1,~0,hierarchy,intersection) != NULL) {
			ret_point = intersection.getPoint();
			ret_normal = intersection.getNormal();
		}
		// intersection with zero plane
		else {
			Vec3 direction = p1 - p0;
			float k = -p0.z / direction.z;
			if(k > 0.0f && k < 1.0f) ret_point = p0 + direction * k;
			else ret_point = Vec3_zero;
			ret_normal = vec3(0.0f,0.0f,1.0f);
		}
		
		// object radius
		float radius = nodesGetBoundRadius(node) / 10.0f;
		
		// node position
		Vec3 point = ret_point + ret_normal * radius * offset;
		engine.visualizer.renderVector(ret_point,point,vec4(1.0f,0.0f,0.0f,1.0f));
		
		Mat4 transform = get_nodes_world_transform();
		start_transform = transform;
		
		point = nodesSnapPoint(point);
		transform = translate(point) * rotation(place_manually_transform);
		if(relative) {
			transform *= orient_transform(rotation(place_manually_transform),Vec3(ret_normal),Vec3(0.0f,0.0f,1.0f));
		}
		
		set_transform(transform);
		
		// grab mouse
		gui.setMouseGrab(1);
		
		// relative transformation
		if(hotkeysGetState("place_node_relative_transform")) {
			if(mouse_grab == 0) {
				relative = !relative;
				panelPlacementSetSnapToSurfaceRelativeTransform(relative);
			}
			mouse_grab = 1;
		} else {
			mouse_grab = 0;
		}
		
		// offset reset
		if(hotkeysGetState("place_node_offset_reset")) {
			offset = 0;
		}
		
		// offset vector
		if(hotkeysGetState("place_node_decrease_offset")) {
			hotkeysClearState("place_node_decrease_offset");
			offset--;
		}
		if(hotkeysGetState("place_node_increase_offset")) {
			hotkeysClearState("place_node_increase_offset");
			offset++;
		}
		panelPlacementSetSnapToSurfaceOffset(offset);
		
		int result = 0;
		
		// place node
		if(hotkeysGetState("place_node_success")) {
			hotkeysClearState("place_node_success");
			panelPlacementSetSnapToSurface(0);
			result = 1;
		}
		// discard changes
		else if(hotkeysGetState("place_node_cancel")) {
			hotkeysClearState("place_node_cancel");
			panelPlacementSetSnapToSurface(0);
			result = -1;
		}
		
		return result;
	}
	
	int visualizer_funcs[] = (:);
	
	void render_node_visualizer(Node node) {
		if(helpersGetObjectBounds()) engine.visualizer.renderNodeBoundBox(node,vec4(0.0f,0.75f,0.0f,1.0f));
		if(node.isObject()) Nodes::Object::render_visualizer(node_cast(node));
		
		int type = node.getType();
		if(visualizer_funcs.check(type)) call(visualizer_funcs[type],node_cast(node));
		else node_cast(node).renderVisualizer();
	}
	
	/**************************************************************************\
	*
	* Nodes
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		// get visualizer funcs
		foreachkey(int type; type_prefix) {
			string func_name = "Nodes::" + type_prefix[type] + "render_visualizer";
			if(is_function(func_name,1)) visualizer_funcs.append(type,get_function(func_name,1));
		}
		
		// manipulators
		translator_m = new WidgetEditorManipulatorTranslator();
		rotator_m = new WidgetEditorManipulatorRotator();
		scaler_m = new WidgetEditorManipulatorScaler();
		translator_m.setCallback(MANIPULATOR_CHANGED,functionid(translator_changed));
		translator_m.setCallback(MANIPULATOR_CLICKED,functionid(manipulator_clicked));
		translator_m.setCallback(MANIPULATOR_RELEASED,functionid(manipulator_released));
		rotator_m.setCallback(MANIPULATOR_CHANGED,functionid(rotator_changed));
		rotator_m.setCallback(MANIPULATOR_CLICKED,functionid(manipulator_clicked));
		rotator_m.setCallback(MANIPULATOR_RELEASED,functionid(manipulator_released));
		scaler_m.setCallback(MANIPULATOR_CHANGED,functionid(scaler_changed));
		scaler_m.setCallback(MANIPULATOR_CLICKED,functionid(manipulator_clicked));
		scaler_m.setCallback(MANIPULATOR_RELEASED,functionid(manipulator_released));
		
		// update nodes
		update_nodes();
		update_view();
		
		// default tab
		parameters_tb.setCurrentTab(0);
		
		// configure window
		editorWindow(window);
		DECLARE_WINDOW_LOAD(nodes)
		
		int value = configGet("nodes_value",window_hp.getValue());
		window_hp.setValue(value);
	}
	
	void shutdown() {
		
		if(node != NULL) {
			pluginsRun("nodeShutdown");
			run_old_function("shutdown");
		}
		
		nodes_tb.clear();
		
		current_nodes.clear();
		current_names.clear();
		current_items.clear();
		
		node = NULL;
		nodes.clear();
		transform_buffer = 0;
		
		DECLARE_WINDOW_SAVE(nodes)
		
		configSet("nodes_value",window_hp.getValue());
	}
	
	Node skip_visualizer_render[0];
	
	/*
	 */
	void update(int need_reload) {
		
		// save node tab
		if(node != NULL) {
			old_tabs.append(node.getType(),parameters_tb.getCurrentTab());
		}
		
		// reload world or clear node
		if(need_reload) {
			pluginsRun("nodeShutdown");
			run_old_function("shutdown");
			set_node(NULL);
			update_nodes();
			update_view();
		}
		
		if(node != NULL) panelPlacementSetScalerEnabled(is_scalable(node));
	}
	
	void render() {
		Object::update_wireframe();
		// place manually
		update_place_manually();
		
		if(hotkeysGetState("group_selected")) {
			hotkeysClearState("group_selected");
			nodesCreateDummyGroup();
		}
		
		// transform
		if(translator_m.isFocused() == 0 && rotator_m.isFocused() == 0 || scaler_m.isFocused() == 0) {
			update_transform();
		}
		
		////////////////////////////////////////////////////////////////////////
		// manipulators
		////////////////////////////////////////////////////////////////////////
		
		// selector
		if(node == NULL || panelPlacementGetSelector()) {
			translator_m.setEnabled(0);
			rotator_m.setEnabled(0);
			scaler_m.setEnabled(0);
		}
		// translator
		else if(panelPlacementGetTranslator()) {
			translator_m.setEnabled(1);
			rotator_m.setEnabled(0);
			scaler_m.setEnabled(0);
		}
		// rotator
		else if(panelPlacementGetRotator()) {
			translator_m.setEnabled(0);
			rotator_m.setEnabled(1);
			scaler_m.setEnabled(0);
		}
		// scaler
		else if(panelPlacementGetScaler()) {
			translator_m.setEnabled(0);
			rotator_m.setEnabled(0);
			scaler_m.setEnabled(1);
		}
		
		// basis
		int basis = panelPlacementGetBasis();
		Node parent = NULL;
		
		if(node != NULL) {
			
			parent = node.getParent();
			
			if(basis == PANEL_BASIS_WORLD) {
				translator_m.setBasis(Mat4_identity);
				rotator_m.setBasis(Mat4_identity);
				scaler_m.setBasis(Mat4_identity);
			} else if(basis == PANEL_BASIS_PARENT) {
				if(parent != NULL) {
					translator_m.setBasis(parent.getWorldTransform());
					rotator_m.setBasis(parent.getWorldTransform());
					scaler_m.setBasis(parent.getWorldTransform());
				} else {
					translator_m.setBasis(Mat4_identity);
					rotator_m.setBasis(Mat4_identity);
					scaler_m.setBasis(Mat4_identity);
				}
			} else if(basis == PANEL_BASIS_LOCAL) {
				Mat4 transform = get_nodes_world_transform();
				
				translator_m.setBasis(transform);
				rotator_m.setBasis(transform);
				scaler_m.setBasis(transform);
			}
			
		}
		
		Player player = Unigine::getPlayer();
		
		// translator
		translator_m.setGridEnabled(panelPlacementGetPositionGrid());
		translator_m.setGridDepthTest(panelPlacementGetPositionGridDepthTest());
		translator_m.setGridShowFocused(panelPlacementGetPositionGridShowFocused());
		translator_m.setStep(panelPlacementGetPositionStep());
		translator_m.setSize(toolsGetSize());
		if(player != NULL) translator_m.setProjection(player.getProjection());
		if(player != NULL) translator_m.setModelview(player.getModelview());
		translator_m.checkCallbacks();
		
		translator_m.render();
		
		// rotator
		rotator_m.setStep(panelPlacementGetRotationStep());
		rotator_m.setSize(toolsGetSize());
		if(player != NULL) rotator_m.setProjection(player.getProjection());
		if(player != NULL) rotator_m.setModelview(player.getModelview());
		rotator_m.checkCallbacks();
		
		rotator_m.render();
		
		// scaler
		scaler_m.setSize(toolsGetSize());
		if(player != NULL) scaler_m.setProjection(player.getProjection());
		if(player != NULL) scaler_m.setModelview(player.getModelview());
		scaler_m.checkCallbacks();
		
		scaler_m.render();
		
		// update node
		if(node != NULL && engine.world.isNode(node)) {
			
			if(skip_visualizer_render.find(node) == -1) run_function("update_dynamic");
			
			forloop(int i = 0; nodes.size()) {
				if(skip_visualizer_render.find(nodes[i]) != -1) continue;
				render_node_visualizer(nodes[i]);
			}
			skip_visualizer_render.clear();
			
			// enabled
			if(parent != NULL && node.isEnabled() == 0 && parent.isEnabled() == 0) {
				parameters_tb.setEnabled(0);
			} else {
				parameters_tb.setEnabled(1);
			}
			
			// handled
			handled_cb.setEnabled(1);
		}
		
	} /* Nodes::render() */
	
	void world_save() {
		Object::world_save();
	}
	
} /* namespace Nodes */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

void nodesSetSkipVisualizerNodes(Node nodes[]) {
	Nodes::skip_visualizer_render = nodes;
}

/*
 */
void nodesInit() {
	Nodes::init();
}

void nodesShutdown() {
	Nodes::shutdown();
}

/*
 */
void nodesToggle() {
	if(Nodes::window_show) nodesHide();
	else nodesShow();
}

void nodesShow() {
	DECLARE_WINDOW_SHOW(Nodes)
}

void nodesHide() {
	DECLARE_WINDOW_HIDE(Nodes)
}

/*
 */
::Widget nodesGetWindow() {
	return Nodes::window;
}

/*
 */
void nodesUpdate(int need_reload) {
	Nodes::update(need_reload);
}

void nodesRender() {
	Nodes::render();
}

void nodesUpdateTransform() {
	Nodes::update_transform();
}

/*
 */
void nodesSelect(Node node,int toggle) {
	if(node != Nodes::node) {
		int surfaces[0] = ();
		if(Nodes::node != NULL && Nodes::node.isObject()) {
			Nodes::Object::get_current_surfaces(surfaces);
		}
		Nodes::NodesState_ s = Nodes::create_state_select(Nodes::nodes,Nodes::node,surfaces);
		Nodes::save_state(s);
	}
	Nodes::select_node(node,toggle);
	Nodes::update(0);
}

void nodesSelect(Object object,int surface,int toggle) {
	int surfaces_[0] = ();
	if(Nodes::node != NULL && Nodes::node.isObject()) {
		Nodes::Object::get_current_surfaces(surfaces_);
	}
	int last = -1;
	if(surfaces_.size() > 0) last = surfaces_[surfaces_.size() - 1];
	if(object != Nodes::node || surface != last) {
		Nodes::NodesState_ s = Nodes::create_state_select(Nodes::nodes,Nodes::node,surfaces_);
		Nodes::save_state(s);
	}
	Nodes::select_object(object,surface,toggle);
	Nodes::update(0);
}

void nodesSelectNew(Node node,int toggle) {
	Nodes::select_node(node,toggle);
	Nodes::update(0);
}

void nodesSelectNodes(Node nodes[],int toggle) {
	Nodes::NodesState_ s = Nodes::create_state_select(Nodes::nodes,Nodes::node,());
	Nodes::save_state(s);
	
	Nodes::select_nodes(nodes,toggle);
	
	Nodes::update(0);
}

/*
 */
int nodesTypeCheck(int type) {
	return Nodes::type_check(type);
}

/*
 */
void nodesReload() {
	Nodes::update_nodes();
	Nodes::reselect_nodes();
	engine.editor.needReload();
}

/*
 */
Node nodesGetNode() {
	return Nodes::get_node();
}

void nodesGetNodes(Node nodes[]) {
	nodes.clear();
	foreach(Node n; Nodes::nodes) {
		nodes.append(n);
	}
}

int nodesGetNumSelectedNodes() {
	return Nodes::nodes.size();
}

void nodesGetCurrentNodes(Node nodes[]) {
	nodes.clear();
	foreach(Node n; Nodes::current_nodes) {
		nodes.append(n);
	}
}

void nodesGetExcludedNodes(Node nodes[]) {
	nodes.clear();
	foreach(Node n; Nodes::excluded_nodes) {
		nodes.append(n);
	}
}

void selectCurrentNodes() {
	
	Node nodes[0];
	nodesGetCurrentNodes(nodes);
	Nodes::select_nodes(nodes,true);
}

void nodesGetCurrentRootNodes(Node nodes[]) {
	nodes.clear();
	forloop(int i = 0; Nodes::nodes_tb.getNumItems()) {
		int item = Nodes::nodes_tb.getItem(i);
		int parent = Nodes::nodes_tb.getItemParent(item);
		if(parent == -1) {
			nodes.append(Nodes::current_nodes[item]);
		}
	}
}

Node nodesGetNode(int surfaces[]) {
	return Nodes::get_node(surfaces);
}

string nodesGetInfo(Node node) {
	return Nodes::get_info(node);
}

/*
 */
void nodesUpdateObjectMaterialName() {
	Nodes::Object::update_material_name();
}

void nodesUpdateObjectPropertyName() {
	Nodes::Object::update_property_name();
}

/*
 */
void nodesUpdateDecalMaterialName() {
	Nodes::Decal::update_material_name();
}

void nodesUpdateDecal(Decal decal) {
	Nodes::Decal::update_decal(decal);
}

/*
 */
Vec3 nodesGetWorldBoundCenter(Node node) {
	return Nodes::get_node_world_center(node);
}

Scalar nodesGetBoundRadius(Node node) {
	return Nodes::get_bound_radius(node);
}

/*
 */
Vec3 nodesGetNodesWorldBoundCenter() {
	return Nodes::nodes_world_center;
}

Scalar nodesGetNodesWorldBoundRadius() {
	return Nodes::get_nodes_world_radius();
}

/*
 */
void nodesSelectAll() {
	if(brushesIsEnabled()) return;
	
	int surfaces[0] = ();
	if(Nodes::node != NULL && Nodes::node.isObject()) {
		Nodes::Object::get_current_surfaces(surfaces);
	}
	Nodes::NodesState_ s = Nodes::create_state_select(Nodes::nodes,Nodes::node,surfaces);
	Nodes::save_state(s);
	
	Nodes::select_nodes_all();
	
	Nodes::update(0);
}

void nodesDeselectNode() {
	
	if(brushesIsEnabled()) return;
	if(Nodes::nodes.size() == 0) return;
	int surfaces[0] = ();
	if(Nodes::node != NULL && Nodes::node.isObject()) {
		Nodes::Object::get_current_surfaces(surfaces);
	}
	Nodes::NodesState_ s = Nodes::create_state_select(Nodes::nodes,Nodes::node,surfaces);
	Nodes::save_state(s);
	
	Nodes::deselect_node();
	
	Nodes::last_selected_node = NULL;
	Nodes::last_selected_surfaces.clear();
	Nodes::last_selected_nodes.clear();
	
	Nodes::update(0);
}

/*
 */
void nodesInvertSelection() {
	if(brushesIsEnabled()) return;
	
	int surfaces[0] = ();
	if(Nodes::node != NULL && Nodes::node.isObject()) {
		Nodes::Object::get_current_surfaces(surfaces);
	}
	Nodes::NodesState_ s = Nodes::create_state_select(Nodes::nodes,Nodes::node,surfaces);
	Nodes::save_state(s);
	
	Nodes::invert_selection_nodes();
	
	Nodes::update(0);
}

void nodesCloneNode() {
	Nodes::clone_clicked();
}

void nodesDeleteNode() {
	Nodes::remove_clicked();
}

Vec3 nodesSnapPoint(Vec3 point) {
	return Nodes::translator_m.snapPoint(point);
}

void nodesSetManipulatorMask(int mask) {
	Nodes::translator_m.setMask(mask);
	Nodes::rotator_m.setMask(mask);
	Nodes::scaler_m.setMask(mask);
}

void nodesPlaceHere() {
	Nodes::place_here();
}

int nodesPlaceManually() {
	return Nodes::place_manually();
}

void nodesDropDown(int mask,int normal_mode,int offset) {
	Nodes::drop_down(mask,normal_mode,offset);
}

void nodesToggleIsolation() {
	if(Nodes::node == NULL) return;
	Nodes::isolate_clicked();
}

Nodes::NodesState_ nodesUnredoCreateModifyState(Node nodes[],Node node) {
	return Nodes::create_state_modify(nodes,node);
}

void nodesUnredoSaveState(Nodes::NodesState_ s) {
	Nodes::save_state(s);
}

void nodesIsManipulatorFocused() {
	return Nodes::translator_m.isFocused() || Nodes::rotator_m.isFocused() || Nodes::scaler_m.isFocused();
}

void nodesRenderVisualizer(Node node) {
	Nodes::render_node_visualizer(node);
}

void nodesCreateDummyGroup() {
	Nodes::create_nodes_group();
}

void nodesConvertIntoNodeReference() {
	Nodes::convert_into_node_reference();
}

/*
 */
void nodesWorldSave() {
	Nodes::world_save();
}

/*
 */
void nodesClearTypeFilter() {
	Nodes::type_cb.setCurrentItem(0); // all nodes
	Nodes::type_changed();
}

/*
 */
string nodesGetObjectDynamicInfo(Object object,int surface) {
	return Nodes::Object::get_dynamic_info(object,surface);
}

/*
 */
int nodesNodeIsScalable(Node n) {
	return Nodes::is_scalable(n);
}

#endif /* __EDITOR_NODES_H__ */
