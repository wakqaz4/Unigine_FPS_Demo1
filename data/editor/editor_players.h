/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_players.h
 * Desc:    Unigine editor
 * Version: 1.03
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Anna Ishina <anet@unigne.com>
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
* Player
*
\******************************************************************************/

/*
 */
namespace Player {
	
	/*
	 */
	WidgetEditLine viewport_mask_el;	// viewport mask
	WidgetEditLine reflection_mask_el;	// reflection mask
	WidgetEditLine source_mask_el;		// source mask
	WidgetEditLine reverb_mask_el;		// reverb mask
	WidgetEditLine fov_el;				// field of view
	WidgetEditLine znear_el;			// near clipping plane
	WidgetEditLine zfar_el;				// far clipping plane
	
	WidgetButton up_b;					// up button
	WidgetButton down_b;				// down button
	WidgetButton remove_b;				// remove button
	WidgetListBox post_materials_lb;	// post materials
	
	Player player;
	
	/*
	 */
	void update() {
		
		player = class_cast("Player",node);
		
		// mask
		viewport_mask_el.setText(editorFormatMask(player.getViewportMask()));
		reflection_mask_el.setText(editorFormatMask(player.getReflectionMask()));
		source_mask_el.setText(editorFormatMask(player.getSourceMask()));
		reverb_mask_el.setText(editorFormatMask(player.getReverbMask()));
		
		// parameters
		fov_el.setText(editorFormat(player.getFov()));
		znear_el.setText(editorFormat(player.getZNear()));
		zfar_el.setText(editorFormat(player.getZFar()));
		
		// post materials
		if(post_materials_lb != NULL) {
			string materials[0];
			strsplit(player.getPostMaterials(),",",materials);
			post_materials_lb.setCallbackEnabled(GUI_CHANGED,0);
			post_materials_lb.clear();
			forloop(int i = 0; materials.size()) {
				string material = trim(materials[i]);
				post_materials_lb.addItem(material);
			}
			post_materials_lb.setCurrentItem(-1);
			post_materials_lb.setCallbackEnabled(GUI_CHANGED,1);
			post_materials_changed();
		}
	}
	
	/*
	 */
	void update_dynamic() {
		
		engine.visualizer.renderFrustum(player.getProjection(),player.getIModelview(),vec4(0.0f,0.75f,0.0f,1.0f));
	}
	
	/*
	 */
	PRESSED_CALLBACK_MASK(viewport_mask,player,ViewportMask,player);
	CLICKED_CALLBACK_MASK(viewport_mask,player,ViewportMask,player,TR("Viewport mask"),"Nodes::Player::viewport_mask_changed")
	PRESSED_CALLBACK_MASK(reflection_mask,player,ReflectionMask,player);
	CLICKED_CALLBACK_MASK(reflection_mask,player,ReflectionMask,player,TR("Reflection mask"),"Nodes::Player::reflection_mask_changed")
	PRESSED_CALLBACK_MASK(source_mask,player,SourceMask,player);
	CLICKED_CALLBACK_MASK(source_mask,player,SourceMask,player,TR("Source mask"),"Nodes::Player::source_mask_changed")
	PRESSED_CALLBACK_MASK(reverb_mask,player,ReverbMask,player);
	CLICKED_CALLBACK_MASK(reverb_mask,player,ReverbMask,player,TR("Reverb mask"),"Nodes::Player::reverb_mask_changed")
	PRESSED_CALLBACK_FLOAT(fov,player,Fov,player,modify)
	PRESSED_CALLBACK_FLOAT(znear,player,ZNear,player,modify)
	PRESSED_CALLBACK_FLOAT(zfar,player,ZFar,player,modify)
	
	/*
	 */
	void viewport_mask_changed(int mask) {
		player.setViewportMask(mask);
		viewport_mask_el.setText(editorFormatMask(player.getViewportMask()));
	}
	
	void reflection_mask_changed(int mask) {
		player.setReflectionMask(mask);
		reflection_mask_el.setText(editorFormatMask(player.getReflectionMask()));
	}
	
	void source_mask_changed(int mask) {
		player.setSourceMask(mask);
		source_mask_el.setText(editorFormatMask(player.getSourceMask()));
	}
	
	void reverb_mask_changed(int mask) {
		player.setReverbMask(mask);
		reverb_mask_el.setText(editorFormatMask(player.getReverbMask()));
	}
	
	/*
	 */
	void create_player_state() {
		Node selection[0];
		nodesGetNodes(selection);
		save_state(create_state_modify(selection,player));
	}
	
	void up_clicked() {
		create_player_state();
		int item = post_materials_lb.getCurrentItem();
		string material = post_materials_lb.getItemText(item - 1);
		post_materials_lb.setItemText(item - 1,post_materials_lb.getItemText(item));
		post_materials_lb.setItemText(item,material);
		post_materials_lb.setCurrentItem(item - 1);
		post_materials_changed();
	}
	
	void down_clicked() {
		create_player_state();
		int item = post_materials_lb.getCurrentItem();
		string material = post_materials_lb.getItemText(item + 1);
		post_materials_lb.setItemText(item + 1,post_materials_lb.getItemText(item));
		post_materials_lb.setItemText(item,material);
		post_materials_lb.setCurrentItem(item + 1);
		post_materials_changed();
	}
	
	void add_clicked() {
		string material;
		if(dialogMaterial("Select material",material)) {
			create_player_state();
			int item = post_materials_lb.addItem(material);
			post_materials_lb.setCurrentItem(item);
			post_materials_changed();
		}
	}
	
	void remove_clicked() {
		create_player_state();
		int item = post_materials_lb.getCurrentItem();
		post_materials_lb.removeItem(item);
		if(item > 0) post_materials_lb.setCurrentItem(item - 1);
		else post_materials_lb.setCurrentItem(-1);
		post_materials_changed();
	}
	
	void post_materials_changed() {
		
		// post materials list
		string materials = "";
		forloop(int i = 0; post_materials_lb.getNumItems()) {
			if(strlen(materials)) materials += ",";
			materials += post_materials_lb.getItemText(i);
		}
		player.setPostMaterials(materials);
		
		// grab focus
		if(up_b.isFocused() || down_b.isFocused() || remove_b.isFocused()) post_materials_lb.setFocus();
		
		// remove button
		int item = post_materials_lb.getCurrentItem();
		up_b.setEnabled((item > 0 && item < post_materials_lb.getNumItems()));
		down_b.setEnabled((item >= 0 && item < post_materials_lb.getNumItems() - 1));
		remove_b.setEnabled((item >= 0 && item < post_materials_lb.getNumItems()));
	}
	
	/**************************************************************************\
	*
	* Dummy
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Dummy {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine reflection_mask_el;
		WidgetEditLine source_mask_el;
		WidgetEditLine reverb_mask_el;
		WidgetEditLine fov_el;
		WidgetEditLine znear_el;
		WidgetEditLine zfar_el;
		
		WidgetVBox post_materials_vb;
		
		WidgetButton up_b;
		WidgetButton down_b;
		WidgetButton remove_b;
		WidgetListBox post_materials_lb;
		
		WidgetGroupBox main_gb;
		PlayerSpectator spectator;
		Mat4 dummy_transform;
		
		PlayerDummy dummy;
		
		string manipulator_name;
		
		/**********************************************************************\
		*
		* Dummy
		*
		\**********************************************************************/
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Player"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			parameters_tb.addTab(TR("Materials"));
			parameters_tb.addChild(post_materials_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
			
			manipulator_name = "";
		}
		
		void shutdown() {
			
			// remove camera panel
			gui.removeChild(main_gb);
			
			// restore player
			if(spectator != NULL) {
				cancel_clicked();
			}
		}
		
		/*
		 */
		void update() {
			
			Nodes::Player::viewport_mask_el = viewport_mask_el;
			Nodes::Player::reflection_mask_el = reflection_mask_el;
			Nodes::Player::source_mask_el = source_mask_el;
			Nodes::Player::reverb_mask_el = reverb_mask_el;
			Nodes::Player::fov_el = fov_el;
			Nodes::Player::znear_el = znear_el;
			Nodes::Player::zfar_el = zfar_el;
			Nodes::Player::up_b = up_b;
			Nodes::Player::down_b = down_b;
			Nodes::Player::remove_b = remove_b;
			Nodes::Player::post_materials_lb = post_materials_lb;
			Nodes::Player::update();
			
			dummy = node_cast(node);
		}
		
		/*
		 */
		void update_dynamic() {
			
			Nodes::Player::update_dynamic();
			
			// update editor player
			if(spectator != NULL) {
				engine.editor.setPlayer(spectator);
				dummy.setWorldTransform(spectator.getWorldTransform());
			}
		}
		
		/*
		 */
		void apply_clicked() {
			
			// show nodes window
			nodesShow();
			
			// enable manipulators
			panelPlacementSetTranslatorEnabled(1);
			panelPlacementSetRotatorEnabled(1);
			panelPlacementSetScalerEnabled(1);
			if(manipulator_name != "") panelPlacementSetManipulator(manipulator_name);
			manipulator_name = "";
			
			// enable controls
			controlsSetEnabled(1);
			
			// remove camera panel
			gui.removeChild(main_gb);
			
			// delete spectator
			delete spectator;
			spectator = NULL;
		}
		
		void cancel_clicked() {
			
			// hide panel
			apply_clicked();
			
			// restore transformation
			dummy.setWorldTransform(dummy_transform);
		}
		
	} /* namespace Dummy */
	
	/**************************************************************************\
	*
	* Spectator
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Spectator {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine reflection_mask_el;
		WidgetEditLine source_mask_el;
		WidgetEditLine reverb_mask_el;
		WidgetEditLine fov_el;
		WidgetEditLine znear_el;
		WidgetEditLine zfar_el;
		
		PlayerSpectator spectator;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Player"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Player::viewport_mask_el = viewport_mask_el;
			Nodes::Player::reflection_mask_el = reflection_mask_el;
			Nodes::Player::source_mask_el = source_mask_el;
			Nodes::Player::reverb_mask_el = reverb_mask_el;
			Nodes::Player::fov_el = fov_el;
			Nodes::Player::znear_el = znear_el;
			Nodes::Player::zfar_el = zfar_el;
			Nodes::Player::post_materials_lb = NULL;
			Nodes::Player::update();
			
			spectator = node_cast(node);
		}
		
		/*
		 */
		void update_dynamic() {
			Nodes::Player::update_dynamic();
		}
		
	} /* namespace Spectator */
	
	/**************************************************************************\
	*
	* Persecutor
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Persecutor {
		
		/*
		 */
		WidgetVBox main_vb;
		
		WidgetEditLine viewport_mask_el;
		WidgetEditLine reflection_mask_el;
		WidgetEditLine source_mask_el;
		WidgetEditLine reverb_mask_el;
		WidgetEditLine fov_el;
		WidgetEditLine znear_el;
		WidgetEditLine zfar_el;
		
		WidgetVBox post_materials_vb;
		
		WidgetButton up_b;
		WidgetButton down_b;
		WidgetButton remove_b;
		WidgetListBox post_materials_lb;
		
		WidgetVBox persecutor_vb;
		
		PlayerPersecutor persecutor;
		
		WidgetCheckBox fixed_cb;
		WidgetCheckBox controlled_cb;
		WidgetCheckBox collision_cb;
		WidgetEditLine collision_mask_el;
		WidgetComboBox target_cb;
		WidgetEditLine anchor_el[3];
		WidgetEditLine min_distance_el;
		WidgetEditLine max_distance_el;
		WidgetEditLine min_theta_angle_el;
		WidgetEditLine max_theta_angle_el;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Player"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			parameters_tb.addTab(TR("Materials"));
			parameters_tb.addChild(post_materials_vb,GUI_ALIGN_EXPAND);
			
			parameters_tb.addTab(TR("Persecutor"));
			parameters_tb.addChild(persecutor_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Player::viewport_mask_el = viewport_mask_el;
			Nodes::Player::reflection_mask_el = reflection_mask_el;
			Nodes::Player::source_mask_el = source_mask_el;
			Nodes::Player::reverb_mask_el = reverb_mask_el;
			Nodes::Player::fov_el = fov_el;
			Nodes::Player::znear_el = znear_el;
			Nodes::Player::zfar_el = zfar_el;
			Nodes::Player::up_b = up_b;
			Nodes::Player::down_b = down_b;
			Nodes::Player::remove_b = remove_b;
			Nodes::Player::post_materials_lb = post_materials_lb;
			Nodes::Player::update();
			
			persecutor = node_cast(node);
			
			target_cb.clear();
			target_cb.addItem("");
			int active_item = 0;
			forloop(int i = 0; current_nodes.size()) {
				if(current_nodes[i] == persecutor) continue;
				target_cb.addItem((current_nodes[i].getName() == "") ? current_names[i] : current_nodes[i].getName(),current_nodes[i].getType());
				if(persecutor.getTarget() != NULL && current_nodes[i] == persecutor.getTarget()) active_item = target_cb.getNumItems() - 1;
				target_cb.setItemData(target_cb.getNumItems() - 1,string(i));
			}
			if(active_item == 0) persecutor.setTarget(NULL);
			
			setCallbackEnabled(GUI_CLICKED,0,(
				fixed_cb,
				controlled_cb,
				collision_cb,
				target_cb,
			));
			setCallbackEnabled(GUI_CHANGED,0,(
				target_cb,
			));
			
			fixed_cb.setChecked(persecutor.isFixed());
			controlled_cb.setChecked(persecutor.isControlled());
			collision_cb.setChecked(persecutor.getCollision());
			target_cb.setCurrentItem(active_item);
			
			setCallbackEnabled(GUI_CLICKED,1,(
				fixed_cb,
				controlled_cb,
				collision_cb,
				target_cb,
			));
			setCallbackEnabled(GUI_CHANGED,1,(
				target_cb,
			));
			
			collision_mask_el.setText(editorFormatMask(persecutor.getCollisionMask()));
			setEditLineVec3(anchor_el,persecutor.getAnchor());
			min_distance_el.setText(editorFormat(persecutor.getMinDistance()));
			max_distance_el.setText(editorFormat(persecutor.getMaxDistance()));
			min_theta_angle_el.setText(editorFormat(persecutor.getMinThetaAngle()));
			max_theta_angle_el.setText(editorFormat(persecutor.getMaxThetaAngle()));
		}
		
		/*
		 */
		void update_dynamic() {
			Nodes::Player::update_dynamic();
		}
		
		/*
		 */
		CLICKED_CALLBACK(fixed,persecutor,Fixed,persecutor)
		CLICKED_CALLBACK(controlled,persecutor,Controlled,persecutor)
		CLICKED_CALLBACK(collision,persecutor,Collision,persecutor)
		PRESSED_CALLBACK_MASK(collision_mask,persecutor,CollisionMask,persecutor);
		CLICKED_CALLBACK_MASK(collision_mask,persecutor,CollisionMask,persecutor,TR("Collision mask"),"Nodes::Player::Persecutor::collision_mask_changed")
		PRESSED_CALLBACK_VEC3(anchor,persecutor,Anchor,persecutor,modify)
		PRESSED_CALLBACK_FLOAT(min_distance,persecutor,MinDistance,persecutor,modify)
		PRESSED_CALLBACK_FLOAT(max_distance,persecutor,MaxDistance,persecutor,modify)
		PRESSED_CALLBACK_FLOAT(min_theta_angle,persecutor,MinThetaAngle,persecutor,modify)
		PRESSED_CALLBACK_FLOAT(max_theta_angle,persecutor,MaxThetaAngle,persecutor,modify)
		
		/*
		 */
		void collision_mask_changed(int mask) {
			persecutor.setCollisionMask(mask);
			collision_mask_el.setText(editorFormatMask(persecutor.getCollisionMask()));
		}
		
		void target_changed() {
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,persecutor);
			int item = target_cb.getCurrentItem();
			if(item == 0) {
				persecutor.setTarget(NULL);
			} else {
				persecutor.setTarget(current_nodes[int(target_cb.getItemData(item))]);
			}
			save_state(s);
		}
		
	} /* namespace Persecutor */
	
	/**************************************************************************\
	*
	* Actor
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Actor {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetEditLine viewport_mask_el;
		WidgetEditLine reflection_mask_el;
		WidgetEditLine source_mask_el;
		WidgetEditLine reverb_mask_el;
		WidgetEditLine fov_el;
		WidgetEditLine znear_el;
		WidgetEditLine zfar_el;
		
		PlayerActor actor;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Player"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			Nodes::Player::viewport_mask_el = viewport_mask_el;
			Nodes::Player::reflection_mask_el = reflection_mask_el;
			Nodes::Player::source_mask_el = source_mask_el;
			Nodes::Player::reverb_mask_el = reverb_mask_el;
			Nodes::Player::fov_el = fov_el;
			Nodes::Player::znear_el = znear_el;
			Nodes::Player::zfar_el = zfar_el;
			Nodes::Player::post_materials_lb = NULL;
			Nodes::Player::update();
			
			actor = node_cast(node);
		}
		
		/*
		 */
		void update_dynamic() {
			Nodes::Player::update_dynamic();
		}
		
	} /* namespace Actor */
	
} /* namespace Player */
