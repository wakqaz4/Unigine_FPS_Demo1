/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_worlds.h
 * Desc:    Unigine editor
 * Version: 1.40
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
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
#define WORLD_TRANSLATOR_CHANGED_VEC3(OBJECT) \
void translator_changed(int num) { \
	if(translator_m[num].isFocused() == 0) return; \
	NodesState_ s = new NodesState_(); \
	s.init_modify(nodes,OBJECT); \
	mat4 transform = OBJECT.getWorldTransform(); \
	vec3 old_size = OBJECT.getSize(); \
	vec3 size = ((OBJECT.getIWorldTransform() * translator_m[num].getTransform()) * vec3_zero); \
	if(num == 0) { \
		OBJECT.setWorldTransform(transform * translate(OBJECT.getSize() * 0.25f + size * 0.5f)); \
		OBJECT.setSize(OBJECT.getSize() * 0.5f - size); \
	} else { \
		OBJECT.setWorldTransform(transform * translate(size * 0.5f - OBJECT.getSize() * 0.25f)); \
		OBJECT.setSize(OBJECT.getSize() * 0.5f + size); \
	} \
	update_transform(); \
	setEditLineVec3(size_el,OBJECT.getSize()); \
	if(compare(transform,OBJECT.getWorldTransform(),STATE_EPSILON) == 0 || compare(old_size,OBJECT.getSize(),STATE_EPSILON) == 0) save_state(s,translator_m[num]); \
	else delete s; \
}

#define WORLD_TRANSLATOR_CHANGED_ARRAY(OBJECT) \
void translator_changed(int num) { \
	if(translator_m[num].isFocused() == 0) return; \
	NodesState_ s = new NodesState_(); \
	s.init_modify(nodes,OBJECT); \
	mat4 transform = OBJECT.getWorldTransform(); \
	int old_size[2] = ( OBJECT.getWidth(), OBJECT.getHeight() ); \
	vec3 size = ((OBJECT.getIWorldTransform() * translator_m[num].getTransform()) * vec3_zero); \
	if(num == 0) { \
		OBJECT.setWorldTransform(transform * translate(OBJECT.getWidth() * 0.25f + size.x * 0.5f,OBJECT.getHeight() * 0.25f + size.y * 0.5f,0.0f)); \
		OBJECT.setSize(OBJECT.getWidth() * 0.5f - size.x,OBJECT.getHeight() * 0.5f - size.y); \
	} else { \
		OBJECT.setWorldTransform(transform * translate(size.x * 0.5f - OBJECT.getWidth() * 0.25f,size.y * 0.5f - OBJECT.getHeight() * 0.25f,0.0f)); \
		OBJECT.setSize(OBJECT.getWidth() * 0.5f + size.x,OBJECT.getHeight() * 0.5f + size.y); \
	} \
	update_transform(); \
	width_el.setText(editorFormat(OBJECT.getWidth())); \
	height_el.setText(editorFormat(OBJECT.getHeight())); \
	if(compare(transform,OBJECT.getWorldTransform(),STATE_EPSILON) == 0 || compare(old_size[0],OBJECT.getWidth(),STATE_EPSILON) == 0 || compare(old_size[1],OBJECT.getHeight(),STATE_EPSILON) == 0) save_state(s,translator_m[num]); \
	else delete s; \
}

/*
 */
namespace World {
	
	/**************************************************************************\
	*
	* WorldLayer
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Layer {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetCheckBox point_cb;
		WidgetEditLine size_el[3];
		WidgetEditLine offset_el[3];
		WidgetEditLine load_distance_el;
		WidgetEditLine clear_distance_el;
		
		WidgetEditLine name_el;
		WidgetEditLine group_el;
		WidgetEditLine weight_el;
		
		WidgetButton createt_b;
		
		WorldLayer layer;
		
		int id_nodes[0];
		
		int is_expand = false;
		float load_distance;
		float clear_distance;
		
		
		/*
		 */
		void init() {
			parameters_tb.addTab(TR("Layer"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
		}
		
		/*
		 */
		void update() {
			layer = node_cast(node);
			
			load();
			
			point_cb.setCallbackEnabled(GUI_CLICKED,0);
			
			point_cb.setChecked(layer.isPoint());
			setEditLineVec3(size_el,layer.getSize());
			setEditLineVec3(offset_el,layer.getOffset());
			
			if(is_expand) {
				load_distance_el.setText(editorFormat(load_distance));
				clear_distance_el.setText(editorFormat(clear_distance));
			} else {
				load_distance_el.setText(editorFormat(layer.getLoadDistance()));
				clear_distance_el.setText(editorFormat(layer.getClearDistance()));
			}
			
			name_el.setText(layer.getNodeName());
			group_el.setText(string(layer.getNodeGroup()));
			weight_el.setText(editorFormat(layer.getNodeWeight()));
			
			point_cb.setCallbackEnabled(GUI_CLICKED,1);
		}
		
		/*
		 */
		void update_dynamic() {
			layer.renderVisualizer();
			createt_b.setEnabled(is_expand);
			
			load_distance_el.setEnabled(!is_expand);
			clear_distance_el.setEnabled(!is_expand);
			size_el[0].setEnabled(!is_expand);
			size_el[1].setEnabled(!is_expand);
			size_el[2].setEnabled(!is_expand);
			offset_el[0].setEnabled(!is_expand);
			offset_el[1].setEnabled(!is_expand);
			offset_el[2].setEnabled(!is_expand);
			name_el.setEnabled(!is_expand);
			group_el.setEnabled(!is_expand);
			weight_el.setEnabled(!is_expand);
			point_cb.setEnabled(!is_expand);
		}
		
		/*
		 */
		WorldLayer resize(WorldLayer layer,string name) {
			
			void get_bound_and_transforms(Node node, WorldBoundBox &bound_box) {
				
				bound_box.expand(new WorldBoundBox(node.getBoundBox(),node.getTransform()));
				
				bound_box.expand(node.getPosition());
				forloop(int i = 0; node.getNumChilds()) {
					bound_box.expand(node.getChild(i).getPosition());
					get_bound_and_transforms(node.getChild(i),bound_box);
				}
			}
			
			Node nodes[0];
			if(engine.world.loadNodes(name,nodes)) {
				
				WorldBoundBox box = new WorldBoundBox();
				forloop(int i = 0; nodes.size()) get_bound_and_transforms(nodes[i],box);
				
				Vec3 size = abs(box.getMax() - box.getMin());
				layer.setSize(size);
				Vec3 center = box.getMin() + size * 0.5f;
				layer.setOffset(center);
				
				forloop(int i = 0; nodes.size()) {
					node_delete(node_append(nodes[i]));
				}
				nodes.clear();
			}
			
			return layer;
		}
		
		/*
		 */
		void name_pressed() {
			
			if(name_el.getText() == layer.getNodeName()) return;
			
			string path = savePath(name_el.getText());
			if(!is_file(path)) {
				dialogMessageOk(TR("Error"),format(TR("Cannot load node file \"%s\""),path));
				name_el.setText(layer.getNodeName());
				return;
			}
			
			NodesState_ state = new NodesState_();
			state.init_modify(nodes,layer);
			
			layer.setNodeName(name_el.getText());
			layer = resize(layer,name_el.getText());
			
			save_state(state);
			
			update();
		}
		
		void load_clicked() {
			string name = layer.getNodeName();
			if(dialogFileNode(TR("Select node to load"),name,DIALOG_FILE_OPEN)) {
				name_el.setText(name);
				name_pressed();
			}
		}
		
		/*
		 */
		void create_clicked() {
			string name = layer.getNodeName();
			if(!is_file(savePath(name))) return;
			
			Node selection[0];
			nodesGetNodes(selection);
			
			NodesState_ states[0];
			states.append(create_state_modify((layer),layer));
			
			Node nodes[0];
			forloop(int i = 0; layer.getNumChilds()) {
				Node node = layer.getChild(i);
				nodes.append(node);
			}
			
			log.message("Saving \"%s\" layer\n",name);
			if(engine.world.saveNodes(name,nodes) == 0) {
				dialogMessageOk(TR("Error"));
				return;
			}
			
			forloop(int i = 0; nodes.size()) {
				Node node = nodes[i];
				states.append(create_state_remove((node),selection,Nodes::node));
				Nodes::remove_node(node);
			}
			
			id_nodes.clear();
			
			is_expand = false;
			
			layer.setLoadDistance(load_distance);
			layer.setClearDistance(clear_distance);
			
			save();
			
			save_states(states);
			nodesReload();
		}
		
		void expand_clicked() {
			string name = layer.getNodeName();
			if(!is_file(savePath(name))) return;
			
			Node selection[] = ();
			nodesGetNodes(selection);
			
			NodesState_ states[0];
			states.append(create_state_modify((layer),layer));
			
			int num_childs = layer.getNumChilds();
			for(int i = 0; i < num_childs; i++) {
				Node node = layer.getChild(i);
				if(id_nodes.find(node.getID()) == -1) continue;
				
				states.append(create_state_remove((node),selection,Nodes::node));
				Nodes::remove_node(node);
				
				num_childs--;
				i--;
			}
			id_nodes.clear();
			
			Node nodes[0];
			if(engine.world.loadNodes(name,nodes)) {
				forloop(int i = 0; nodes.size()) {
					Node node = nodes[i];
					engine.editor.addNode(node);
					node.setParent(layer);
					states.append(create_state_new((node),selection,Nodes::node));
					int id = node.getID();
					id_nodes.append(id);
				}
			} else {
				dialogMessageOk(TR("Error"));
			}
			
			if(!is_expand) {
				load_distance = layer.getLoadDistance();
				clear_distance = layer.getClearDistance();
				layer.setLoadDistance(0.0f);
				layer.setClearDistance(0.0f);
				is_expand = true;
			}
			
			save();
			
			save_states(states);
			nodesReload();
		}
		
		void save() {
			if(layer == NULL || engine.editor.isNode(layer) == 0) return;
			Xml xml = new Xml("world_layer");
			
			xml.setChildData("is_expand",is_expand);
			if(is_expand) {
				xml.setChildData("load_distance",load_distance);
				xml.setChildData("clear_distance",clear_distance);
				
				if(id_nodes.size() > 0) {
					Xml xml_nodes = xml.addChild("id_nodes");
					xml_nodes.setIntArrayData(id_nodes);
				}
			}
			
			layer.setData(xml.getSubTree());
			delete xml;
		}
		
		void load() {
			string data = layer.getData();
			is_expand = false;
			
			if(data == NULL || is_string(data) == 0 || strlen(data) == 0 || data[0] != '<') return;
			
			Xml xml = new Xml();
			if(xml.parse(data) == 0 || xml.getName() != "world_layer") {
				delete xml;
				return;
			}
			
			is_expand = xml.getChildData("is_expand",false);
			if(is_expand) {
				load_distance = xml.getChildData("load_distance",0.0f);
				clear_distance = xml.getChildData("clear_distance",0.0f);
				
				id_nodes.clear();
				if(xml.isChild("id_nodes")) {
					Xml xml_nodes = xml.getChild("id_nodes");
					xml_nodes.getIntArrayData(id_nodes);
				}
			}
			
			delete xml;
		}
		
		/*
		 */
		CLICKED_CALLBACK(point,layer,Point,layer)
		PRESSED_CALLBACK_VEC3(size,layer,Size,layer,modify)
		PRESSED_CALLBACK_VEC3(offset,layer,Offset,layer,modify)
		PRESSED_CALLBACK_FLOAT(load_distance,layer,LoadDistance,layer,modify)
		PRESSED_CALLBACK_FLOAT(clear_distance,layer,ClearDistance,layer,modify)
		PRESSED_CALLBACK(group,layer,NodeGroup,layer,int,modify)
		PRESSED_CALLBACK_FLOAT(weight,layer,NodeWeight,layer,modify)
		
	} /* namespace Layer */
	
	/**************************************************************************\
	*
	* WorldSector
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Sector {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetEditLine size_el[3];
		
		WidgetManipulatorTranslator translator_m[2];
		
		WorldSector sector;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Sector"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
			if(translator_m[0] != NULL) {
				gui.removeChild(translator_m[0]);
				gui.removeChild(translator_m[1]);
			}
		}
		
		/*
		 */
		void update() {
			
			sector = node_cast(node);
			
			setEditLineVec3(size_el,sector.getSize());
		}
		
		/*
		 */
		void update_dynamic() {
			
			if(translator_m[0] == NULL) {
				translator_m[0] = new WidgetManipulatorTranslator(gui);
				translator_m[1] = new WidgetManipulatorTranslator(gui);
				translator_m[0].setCallback(GUI_CHANGED,functionid(translator_changed),0);
				translator_m[0].setCallback(GUI_CLICKED,functionid(slider_clicked));
				translator_m[1].setCallback(GUI_CHANGED,functionid(translator_changed),1);
				translator_m[1].setCallback(GUI_CLICKED,functionid(slider_clicked));
			}
			
			if(panelPlacementGetSelector() == 0) {
				updateAuxManipulator(translator_m[0]);
				updateAuxManipulator(translator_m[1]);
				mat4 transform = sector.getWorldTransform();
				if(translator_m[0].isFocused() == 0) {
					translator_m[0].setBasis(transform);
					translator_m[0].setCallbackEnabled(GUI_CHANGED,0);
					translator_m[0].setTransform(transform * translate(-sector.getSize() * 0.5f));
					translator_m[0].setCallbackEnabled(GUI_CHANGED,1);
				}
				if(translator_m[1].isFocused() == 0) {
					translator_m[1].setBasis(transform * scale(-1.0f,-1.0f,-1.0f));
					translator_m[1].setCallbackEnabled(GUI_CHANGED,0);
					translator_m[1].setTransform(transform * translate(sector.getSize() * 0.5f));
					translator_m[1].setCallbackEnabled(GUI_CHANGED,1);
				}
				if(editorIsChild(translator_m[0]) == 0) {
					gui.addChild(translator_m[0],GUI_ALIGN_OVERLAP);
					gui.addChild(translator_m[1],GUI_ALIGN_OVERLAP);
				}
			}
			else {
				gui.removeChild(translator_m[0]);
				gui.removeChild(translator_m[1]);
			}
			
			sector.renderVisualizer();
			engine.visualizer.renderWorldSectorObjectSurfaces(sector,vec4(1.0f,1.0f,1.0f,1.0f));
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(size,sector,Size,sector,modify)
		WORLD_TRANSLATOR_CHANGED_VEC3(sector)
		
	} /* namespace Sector */
	
	/**************************************************************************\
	*
	* WorldPortal
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Portal {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetEditLine width_el;
		WidgetEditLine height_el;
		
		WidgetManipulatorTranslator translator_m[2];
		
		WorldPortal portal;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Portal"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
			if(translator_m[0] != NULL) {
				gui.removeChild(translator_m[0]);
				gui.removeChild(translator_m[1]);
			}
		}
		
		/*
		 */
		void update() {
			
			portal = node_cast(node);
			
			width_el.setText(editorFormat(portal.getWidth()));
			height_el.setText(editorFormat(portal.getHeight()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			if(translator_m[0] == NULL) {
				translator_m[0] = new WidgetManipulatorTranslator(gui);
				translator_m[1] = new WidgetManipulatorTranslator(gui);
				translator_m[0].setCallback(GUI_CHANGED,functionid(translator_changed),0);
				translator_m[0].setCallback(GUI_CLICKED,functionid(slider_clicked));
				translator_m[1].setCallback(GUI_CHANGED,functionid(translator_changed),1);
				translator_m[1].setCallback(GUI_CLICKED,functionid(slider_clicked));
			}
			
			if(panelPlacementGetSelector() == 0) {
				updateAuxManipulator(translator_m[0]);
				updateAuxManipulator(translator_m[1]);
				mat4 transform = portal.getWorldTransform();
				if(translator_m[0].isFocused() == 0) {
					translator_m[0].setBasis(transform);
					translator_m[0].setCallbackEnabled(GUI_CHANGED,0);
					translator_m[0].setTransform(transform * translate(-portal.getWidth() * 0.5f,-portal.getHeight() * 0.5f,0.0f));
					translator_m[0].setCallbackEnabled(GUI_CHANGED,1);
				}
				if(translator_m[1].isFocused() == 0) {
					translator_m[1].setBasis(transform * scale(-1.0f,-1.0f,-1.0f));
					translator_m[1].setCallbackEnabled(GUI_CHANGED,0);
					translator_m[1].setTransform(transform * translate(portal.getWidth() * 0.5f,portal.getHeight() * 0.5f,0.0f));
					translator_m[1].setCallbackEnabled(GUI_CHANGED,1);
				}
				if(editorIsChild(translator_m[0]) == 0) {
					gui.addChild(translator_m[0],GUI_ALIGN_OVERLAP);
					gui.addChild(translator_m[1],GUI_ALIGN_OVERLAP);
				}
			}
			else {
				gui.removeChild(translator_m[0]);
				gui.removeChild(translator_m[1]);
			}
			
			portal.renderVisualizer();
		}
		
		/*
		 */
		PRESSED_CALLBACK_SIZE(width,height,portal,Size,Width,Height,float,editorFormat)
		WORLD_TRANSLATOR_CHANGED_ARRAY(portal)
		
	} /* namespace Portal */
	
	/**************************************************************************\
	*
	* WorldTrigger
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Trigger {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetCheckBox touch_cb;
		WidgetEditLine size_el[3];
		WidgetEditLine enter_el;
		WidgetEditLine leave_el;
		
		WidgetManipulatorTranslator translator_m[2];
		
		WorldTrigger trigger;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Trigger"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
			if(translator_m[0] != NULL) {
				gui.removeChild(translator_m[0]);
				gui.removeChild(translator_m[1]);
			}
		}
		
		/*
		 */
		void update() {
			
			trigger = node_cast(node);
			
			touch_cb.setCallbackEnabled(GUI_CLICKED,0);
			
			touch_cb.setChecked(trigger.isTouch());
			setEditLineVec3(size_el,trigger.getSize());
			enter_el.setText(trigger.getEnterCallbackName());
			leave_el.setText(trigger.getLeaveCallbackName());
			
			touch_cb.setCallbackEnabled(GUI_CLICKED,1);
		}
		
		/*
		 */
		void update_dynamic() {
			
			if(translator_m[0] == NULL) {
				translator_m[0] = new WidgetManipulatorTranslator(gui);
				translator_m[1] = new WidgetManipulatorTranslator(gui);
				translator_m[0].setCallback(GUI_CHANGED,functionid(translator_changed),0);
				translator_m[0].setCallback(GUI_CLICKED,functionid(slider_clicked));
				translator_m[1].setCallback(GUI_CHANGED,functionid(translator_changed),1);
				translator_m[1].setCallback(GUI_CLICKED,functionid(slider_clicked));
			}
			
			if(panelPlacementGetSelector() == 0) {
				updateAuxManipulator(translator_m[0]);
				updateAuxManipulator(translator_m[1]);
				mat4 transform = trigger.getWorldTransform();
				if(translator_m[0].isFocused() == 0) {
					translator_m[0].setBasis(transform);
					translator_m[0].setCallbackEnabled(GUI_CHANGED,0);
					translator_m[0].setTransform(transform * translate(-trigger.getSize() * 0.5f));
					translator_m[0].setCallbackEnabled(GUI_CHANGED,1);
				}
				if(translator_m[1].isFocused() == 0) {
					translator_m[1].setBasis(transform * scale(-1.0f,-1.0f,-1.0f));
					translator_m[1].setCallbackEnabled(GUI_CHANGED,0);
					translator_m[1].setTransform(transform * translate(trigger.getSize() * 0.5f));
					translator_m[1].setCallbackEnabled(GUI_CHANGED,1);
				}
				if(editorIsChild(translator_m[0]) == 0) {
					gui.addChild(translator_m[0],GUI_ALIGN_OVERLAP);
					gui.addChild(translator_m[1],GUI_ALIGN_OVERLAP);
				}
			}
			else {
				gui.removeChild(translator_m[0]);
				gui.removeChild(translator_m[1]);
			}
			
			trigger.renderVisualizer();
		}
		
		/*
		 */
		CLICKED_CALLBACK(touch,trigger,Touch,trigger)
		PRESSED_CALLBACK_VEC3(size,trigger,Size,trigger,modify)
		PRESSED_CALLBACK(enter,trigger,EnterCallbackName,trigger,string,modify)
		PRESSED_CALLBACK(leave,trigger,LeaveCallbackName,trigger,string,modify)
		WORLD_TRANSLATOR_CHANGED_VEC3(trigger)
		
	} /* namespace Trigger */
	
	/**************************************************************************\
	*
	* WorldCluster
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Cluster {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetVBox generator_vb;
		WidgetVBox collector_vb;
		
		WidgetLabel info_l;
		WidgetEditLine visible_distance_el;
		WidgetEditLine fade_distance_el;
		WidgetEditLine shadow_radius_el;
		
		WorldCluster cluster;
		
		/*
		 */
		namespace Generator {
			
			/*
			 */
			WidgetGridBox references_gb;
			WidgetCheckBox orientation_cb;
			WidgetEditLine mask_image_el;
			WidgetEditLine seed_el;
			WidgetEditLine size_x_el;
			WidgetEditLine size_y_el;
			WidgetEditLine step_el;
			WidgetEditLine density_el;
			WidgetEditLine threshold_el;
			WidgetEditLine angle_el;
			WidgetEditLine min_scale_mean_el;
			WidgetEditLine min_scale_spread_el;
			WidgetEditLine max_scale_mean_el;
			WidgetEditLine max_scale_spread_el;
			WidgetEditLine offset_mean_el;
			WidgetEditLine offset_spread_el;
			WidgetEditLine rotation_mean_el[3];
			WidgetEditLine rotation_spread_el[3];
			
			int orientation = 1;
			int seed = 0;
			string mask_image = "";
			float size_x = 100.0f;
			float size_y = 100.0f;
			float step = 1.0f;
			float density = 0.01f;
			float threshold = 0.5f;
			float angle = 0.0f;
			float min_scale_mean = 1.0f;
			float min_scale_spread = 0.0f;
			float max_scale_mean = 1.0f;
			float max_scale_spread = 0.0f;
			float offset_mean = 0.0f;
			float offset_spread = 0.0f;
			vec3 rotation_mean = vec3_zero;
			vec3 rotation_spread = vec3_zero;
			
			/*
			 */
			class Reference {
				
				string name;
				WidgetCheckBox remove_cb;
				WidgetSlider probability_sl;
				
				Reference(string n,float probability) {
					
					name = n;
					remove_cb = new WidgetCheckBox(gui,basename(name));
					remove_cb.setToolTip(name);
					references_gb.addChild(remove_cb,GUI_ALIGN_LEFT);
					
					probability_sl = new WidgetSlider(gui,0,1000,probability * 1000);
					probability_sl.setToolTip(TR("Probability of the reference node"));
					references_gb.addChild(probability_sl,GUI_ALIGN_EXPAND);
				}
				~Reference() {
					delete remove_cb;
					delete probability_sl;
				}
				
				int isRemoved() {
					return remove_cb.isChecked();
				}
				string getName() {
					return name;
				}
				void getProbability() {
					return probability_sl.getValue() / 1000.0f;
				}
			};
			
			Reference references[0];
			
			/*
			 */
			void init() {
				
			}
			
			void shutdown() {
				
				if(engine.world.isNode(cluster)) {
					
					Xml xml = new Xml("generator");
					
					xmlSetData(xml,"orientation",orientation);
					
					xmlSetData(xml,"mask_image",mask_image);
					
					xmlSetData(xml,"seed",seed);
					
					xmlSetData(xml,"size_x",size_x);
					xmlSetData(xml,"size_y",size_y);
					xmlSetData(xml,"step",step);
					
					xmlSetData(xml,"density",density);
					xmlSetData(xml,"threshold",threshold);
					xmlSetData(xml,"angle",angle);
					
					xmlSetData(xml,"min_scale_mean",min_scale_mean);
					xmlSetData(xml,"min_scale_spread",min_scale_spread);
					xmlSetData(xml,"max_scale_mean",max_scale_mean);
					xmlSetData(xml,"max_scale_spread",max_scale_spread);
					
					xmlSetData(xml,"offset_mean",offset_mean);
					xmlSetData(xml,"offset_spread",offset_spread);
					xmlSetData(xml,"rotation_mean",rotation_mean);
					xmlSetData(xml,"rotation_spread",rotation_spread);
					
					xmlSetData(xml,"num_references",references.size());
					forloop(int i = 0; references.size()) {
						xmlSetData(xml,format("reference_name_%d",i),references[i].getName());
						xmlSetData(xml,format("reference_probability_%d",i),references[i].getProbability());
					}
					
					cluster.setData(xml.getSubTree());
					delete xml;
				}
			}
			
			/*
			 */
			void update() {
				
				references.delete();
				
				Xml xml = new Xml();
				if(xml.parse(cluster.getData()) && xml.getName() == "generator") {
					
					orientation = xmlGetData(xml,"orientation",orientation);
					
					mask_image = xmlGetData(xml,"mask_image",mask_image);
					
					seed = xmlGetData(xml,"seed",seed);
					
					size_x = xmlGetData(xml,"size_x",size_x);
					size_y = xmlGetData(xml,"size_y",size_y);
					step = xmlGetData(xml,"step",step);
					
					density = xmlGetData(xml,"density",density);
					threshold = xmlGetData(xml,"threshold",threshold);
					angle = xmlGetData(xml,"angle",angle);
					
					min_scale_mean = xmlGetData(xml,"min_scale_mean",min_scale_mean);
					min_scale_spread = xmlGetData(xml,"min_scale_spread",min_scale_spread);
					max_scale_mean = xmlGetData(xml,"max_scale_mean",max_scale_mean);
					max_scale_spread = xmlGetData(xml,"max_scale_spread",max_scale_spread);
					
					offset_mean = xmlGetData(xml,"offset_mean",offset_mean);
					offset_spread = xmlGetData(xml,"offset_spread",offset_spread);
					rotation_mean = xmlGetData(xml,"rotation_mean",rotation_mean);
					rotation_spread = xmlGetData(xml,"rotation_spread",rotation_spread);
					
					forloop(int i = 0; xmlGetData(xml,"num_references",0)) {
						string name = xmlGetData(xml,format("reference_name_%d",i),"");
						float probability = xmlGetData(xml,format("reference_probability_%d",i),1.0f);
						if(strlen(name)) references.append(new Reference(name,probability));
					}
				}
				delete xml;
				
				orientation_cb.setChecked(orientation);
				
				mask_image_el.setText(mask_image);
				
				seed_el.setText(string(seed));
				
				size_x_el.setText(editorFormat(size_x));
				size_y_el.setText(editorFormat(size_y));
				step_el.setText(editorFormat(step));
				
				density_el.setText(editorFormat(density,5));
				threshold_el.setText(editorFormat(threshold));
				angle_el.setText(editorFormat(angle));
				
				min_scale_mean_el.setText(editorFormat(min_scale_mean));
				min_scale_spread_el.setText(editorFormat(min_scale_spread));
				max_scale_mean_el.setText(editorFormat(max_scale_mean));
				max_scale_spread_el.setText(editorFormat(max_scale_spread));
				
				offset_mean_el.setText(editorFormat(offset_mean));
				offset_spread_el.setText(editorFormat(offset_spread));
				setEditLineVec3(rotation_mean_el,rotation_mean);
				setEditLineVec3(rotation_spread_el,rotation_spread);
			}
			
			/*
			 */
			void add_clicked() {
				string name;
				if(dialogFileNode(TR("Select node reference"),name)) {
					forloop(int i = 0; references.size()) {
						if(references[i].getName() == name) {
							dialogMessageOk(TR("Error"),format(TR("\"%s\" is already exists in the list"),name));
							return;
						}
					}
					references.append(new Reference(name,1.0f));
				}
			}
			
			void remove_clicked() {
				int remove[0];
				forloop(int i = 0; references.size()) {
					if(references[i].isRemoved()) remove.append(i);
				}
				if(remove.size() == 0) return;
				if(dialogMessageYesNo(TR("Confirm"),TR("Remove nodes from the list?"))) {
					for(int i = remove.size() - 1; i >= 0; i--) {
						delete references[remove[i]];
						references.remove(remove[i]);
					}
				}
			}
			
			/*
			 */
			void orientation_clicked() {
				orientation = orientation_cb.isChecked();
			}
			
			/*
			 */
			void mask_image_pressed() {
				mask_image = mask_image_el.getText();
			}
			
			void mask_image_load_clicked() {
				if(dialogFileImage(TR("Select mask image"),mask_image)) {
					mask_image_el.setText(mask_image);
				}
			}
			
			void mask_image_view_clicked() {
				if(strlen(mask_image)) {
					dialogImage(TR("Cluster mask image"),mask_image);
				}
			}
			
			void mask_image_clear_clicked() {
				if(strlen(mask_image)) {
					if(dialogMessageYesNo(TR("Confirm"),TR("Clear mask image?"))) {
						mask_image = "";
						mask_image_el.setText(mask_image);
					}
				}
			}
			
			/*
			 */
			void randomize_clicked() {
				seed_el.setText(string(engine.game.getRandom(0,0x0fffffff)));
				seed_el.runCallback(GUI_PRESSED);
			}			
			
			void seed_pressed() {
				seed = int(seed_el.getText());
			}
			
			/*
			 */
			#define WORLD_CLUSTER_GENERATOR_PRESSED_CALLBACK(NAME,MIN,MAX) \
			void NAME ## _pressed() { \
				NAME = clamp(float(NAME ## _el.getText()),MIN,MAX); \
				NAME ## _el.setText(editorFormat(NAME)); \
			}
			
			WORLD_CLUSTER_GENERATOR_PRESSED_CALLBACK(size_x,0.0f,INFINITY)
			WORLD_CLUSTER_GENERATOR_PRESSED_CALLBACK(size_y,0.0f,INFINITY)
			WORLD_CLUSTER_GENERATOR_PRESSED_CALLBACK(step,0.0f,INFINITY)
			WORLD_CLUSTER_GENERATOR_PRESSED_CALLBACK(threshold,0.0f,1.0f)
			WORLD_CLUSTER_GENERATOR_PRESSED_CALLBACK(angle,0.0f,1.0f)
			WORLD_CLUSTER_GENERATOR_PRESSED_CALLBACK(min_scale_mean,0.0f,100.0f)
			WORLD_CLUSTER_GENERATOR_PRESSED_CALLBACK(min_scale_spread,0.0f,100.0f)
			WORLD_CLUSTER_GENERATOR_PRESSED_CALLBACK(max_scale_mean,0.0f,100.0f)
			WORLD_CLUSTER_GENERATOR_PRESSED_CALLBACK(max_scale_spread,0.0f,100.0f)
			WORLD_CLUSTER_GENERATOR_PRESSED_CALLBACK(offset_mean,-1000.0f,1000.0f)
			WORLD_CLUSTER_GENERATOR_PRESSED_CALLBACK(offset_spread,-1000.0f,1000.0f)
			
			#undef WORLD_CLUSTER_GENERATOR_PRESSED_CALLBACK
			
			/*
			 */
			void density_pressed() {
				density = max(float(density_el.getText()),0.0f);
				density_el.setText(editorFormat(density,5));
			}
			
			/*
			 */
			void rotation_mean_pressed() {
				rotation_mean = clamp(getEditLineVec3(rotation_mean_el),vec3(-180.0f),vec3(180.0f));
				setEditLineVec3(rotation_mean_el,rotation_mean);
			}
			
			void rotation_spread_pressed() {
				rotation_spread = clamp(getEditLineVec3(rotation_spread_el),vec3(0.0f),vec3(180.0f));
				setEditLineVec3(rotation_spread_el,rotation_spread);
			}
			
			/*
			 */
			float get_density(Image image,vec3 position) {
				
				if(image == NULL) return 1.0f;
				
				float x = position.x / size_x;
				float y = position.y / size_y;
				vec3 pixel = image.get2D(x,y);
				
				return pixel.x;
			}
			
			float get_density(Image image,vec3 min,vec3 max) {
				
				if(image == NULL) return 1.0f;
				
				int x0 = int((min.x / size_x) * image.getWidth());
				int y0 = int((min.y / size_y) * image.getHeight());
				int x1 = int((max.x / size_x) * image.getWidth());
				int y1 = int((max.y / size_y) * image.getHeight());
				if(x0 == x1 || y0 == y1) return 0.0f;
				
				float density = 0.0f;
				forloop(int y = y0; y1) {
					forloop(int x = x0; x1) {
						vec3 pixel = image.get2D(x,y);
						density += pixel.x;
					}
				}
				
				return density / ((x1 - x0) * (y1 - y0));
			}
			
			/*
			 */
			int generate_positions(Image image,vec4 positions[],vec3 directions[],vec3 parameters[]) {
				
				// configuration
				int grid_step = 8;
				
				// check density image
				if(image != NULL) {
					
					// check density mask type
					if(image.getType() != IMAGE_2D) {
						dialogMessageOk(TR("Error"),format(TR("Can't use %s image as density mask"),image.getTypeName()));
						return 0;
					}
					
					// check density mask format
					if(image.convertToFormat(IMAGE_FORMAT_R8) == 0) {
						dialogMessageOk(TR("Error"),format(TR("Can't use %s image as density mask"),image.getFormatName()));
						return 0;
					}
				}
				
				// object intersection
				Object object = NULL;
				float object_radius = 0.0f;
				if(cluster.getParent() != NULL && Node(cluster.getParent()).isObject()) {
					object = node_cast(cluster.getParent());
					object_radius = object.getBoundSphere().getRadius();
				}
				
				// cell parameters
				int cell_size_x = max(int(ceil(size_x / step)),1);
				int cell_size_y = max(int(ceil(size_y / step)),1);
				float cell_step_x = size_x / cell_size_x;
				float cell_step_y = size_y / cell_size_y;
				float grid_step_x = cell_step_x / grid_step;
				float grid_step_y = cell_step_y / grid_step;
				
				// cluster direction
				vec3 direction = normalize(rotation(cluster.getWorldTransform()) * vec3(0.0f,0.0f,1.0f));
				
				// begin progress dialog
				dialogProgressBegin(TR("Generating cluster nodes..."));
				
				// generate positions
				forloop(int Y = 0; cell_size_y) {
					forloop(int X = 0; cell_size_x) {
						
						// random seed
						int id = cell_size_x * Y + X;
						engine.game.setSeed(id * 413711 + seed);
						
						// cell offset
						vec3 offset = vec3(cell_step_x * X,cell_step_y * Y,0.0f);
						
						// cell density
						float cell_density = get_density(image,offset,offset + vec3(cell_step_x,cell_step_y,0.0f)) * density * cell_step_x * cell_step_y;
						
						// generate cell positions
						int num_positions = positions.size();
						for(float density = 0.0f; density < cell_density; density += grid_step * grid_step) {
							forloop(int y = 0; grid_step) {
								forloop(int x = 0; grid_step) {
									
									// random position
									vec3 position;
									position.x = grid_step_x * x + engine.game.getRandom(0.0f,grid_step_x);
									position.y = grid_step_y * y + engine.game.getRandom(0.0f,grid_step_y);
									position.z = 0.0f;
									
									// check position density
									float d = get_density(image,position);
									if(d < EPSILON || d < threshold) continue;
									
									// transform position
									position = cluster.getWorldTransform() * (offset + position);
									
									// object intersection
									if(object != NULL) {
										int normal;
										int has_intersection = 0;
										WorldIntersectionNormal intersection = new WorldIntersectionNormal();
										vec3 p0 = object.getIWorldTransform() * (position + direction * object_radius);
										position = object.getIWorldTransform() * (position - direction * object_radius);
										forloop(int i = 0; object.getNumSurfaces()) {
											if(object.isEnabled(i) == 0) continue;
											if(object.getIntersection(i) == 0) continue;
											if(object.getIntersection(p0,position,0,intersection)) {
												has_intersection = 1;
												position = intersection.getPoint();
												normal = intersection.getNormal();
											}
										}
										if(has_intersection == 0) continue;
										position = object.getWorldTransform() * position;
										normal = normalize(rotation(object.getWorldTransform()) * normal);
										if(dot(direction,normal) < angle) continue;
										positions.append(vec4(position,d));
										if(orientation) directions.append(normal);
										else directions.append(direction);
									} else {
										positions.append(vec4(position,d));
										directions.append(direction);
									}
								}
							}
						}
						
						// randomize cell positions
						forloop(int i = num_positions; positions.size(); 2) {
							int k = engine.game.getRandom(num_positions,positions.size());
							int j = engine.game.getRandom(num_positions,positions.size());
							vec3 position = positions[k];
							vec3 direction = directions[k];
							positions[k] = positions[j];
							directions[k] = directions[j];
							positions[j] = position;
							directions[j] = direction;
						}
						
						// remove odd cell positions
						for(int i = num_positions + int(cell_density); i < positions.size()) {
							positions.remove();
							directions.remove();
						}
						
						// random parameters
						forloop(int i = num_positions; positions.size()) {
							float offset = offset_mean + engine.game.getRandom(-offset_spread,offset_spread);
							float angle = rotation_mean.z + engine.game.getRandom(-rotation_spread.z,rotation_spread.z);
							parameters.append(vec3(offset,angle,0.0f));
						}
						
						// update progress dialog
						int progress = (cell_size_x * Y + X) * 100 / (cell_size_x * cell_size_y);
						if(dialogProgressUpdate(progress) == 0) {
							dialogProgressEnd();
							return 0;
						}
					}
				}
				
				// end progress dialog
				dialogProgressEnd();
				
				// randomize all positions
				forloop(int i = 0; positions.size(); 2) {
					int k = engine.game.getRandom(0,positions.size());
					int j = engine.game.getRandom(0,positions.size());
					vec3 position = positions[k];
					vec3 direction = directions[k];
					vec3 parameter = parameters[k];
					positions[k] = positions[j];
					directions[k] = directions[j];
					parameters[k] = parameters[j];
					positions[j] = position;
					directions[j] = direction;
					parameters[j] = parameter;
				}
				
				return 1;
			}
			
			/*
			 */
			void generate_clicked() {
				
				// check references
				if(references.size() == 0) {
					dialogMessageOk(TR("Error"),TR("Node references is not loaded"));
					return;
				}
				
				// load mask image
				Image image = NULL;
				if(strlen(mask_image)) {
					image = new Image();
					if(image.load(mask_image) == 0) {
						dialogMessageOk(TR("Error"));
						delete image;
						return;
					}
				}
				
				// node positions
				vec4 positions[0];
				vec3 directions[0];
				vec3 parameters[0];
				
				// generate positions
				if(generate_positions(image,positions,directions,parameters) == 0) {
					positions.delete();
					directions.delete();
					parameters.delete();
					delete image;
					return;
				}
				
				delete image;
				
				// confirm cluster update
				if(dialogMessageYesNo(TR("Confirm"),format(TR("Create cluster with %d nodes?"),positions.size())) == 0) {
					positions.delete();
					directions.delete();
					parameters.delete();
					return;
				}
				
				// normalize probability
				float probability = 0.0f;
				forloop(int i = 0; references.size()) {
					probability += references[i].getProbability();
				}
				
				// cluster direction
				vec3 direction = normalize(rotation(cluster.getWorldTransform()) * vec3(1.0f,0.0f,0.0f));
				
				// create nodes
				Node nodes[0];
				forloop(int i = 0; positions.size()) {
					float p = engine.game.getRandom(0.0f,probability);
					forloop(int j = 0; references.size()) {
						if(p <= references[j].getProbability()) {
							NodeReference reference = new NodeReference(references[j].getName());
							
							float k = saturate((positions[i].w - threshold) / max(1.0f - threshold,EPSILON));
							float scale_mean = lerp(min_scale_mean,max_scale_mean,k);
							float scale_spread = lerp(min_scale_spread,max_scale_spread,k);
							float scale = max(scale_mean + engine.game.getRandom(-scale_spread,scale_spread),0.0f);
							
							vec3 position = vec3(positions[i]) + directions[i] * parameters[i].x;
							
							mat4 rotation_x = rotateX(rotation_mean.x + engine.game.getRandom(-rotation_spread.x,rotation_spread.x));
							mat4 rotation_y = rotateY(rotation_mean.y + engine.game.getRandom(-rotation_spread.y,rotation_spread.y));
							mat4 rotation_z = rotateZ(parameters[i].y);
							
							mat4 transform = setTo(position,position - directions[i],direction) * rotation_x * rotation_y * rotation_z * ::scale(vec3(scale));
							reference.setWorldTransform(transform);
							nodes.append(reference);
							break;
						}
						p -= references[j].getProbability();
					}
				}
				// save unredo state
				save_state(create_state_modify((cluster),cluster));
				// create nodes
				cluster.createNodes(nodes);
				nodes.delete();
				
				// delete positions
				positions.delete();
				directions.delete();
				parameters.delete();
			}
			
		} /* namespace Generator */
		
		/*
		 */
		namespace Collector {
			
			/*
			 */
			WidgetVBox references_vb;
			
			/*
			 */
			class Reference {
				
				string name;
				WidgetCheckBox select_cb;
				
				Reference(string n,int num) {
					name = n;
					select_cb = new WidgetCheckBox(gui,basename(name) + format(" (%d)",num));
					select_cb.setToolTip(name);
					references_vb.addChild(select_cb,GUI_ALIGN_LEFT);
				}
				~Reference() {
					delete select_cb;
				}
				
				int isSelected() {
					return select_cb.isChecked();
				}
				string getName() {
					return name;
				}
			};
			
			Reference references[0];
			
			/*
			 */
			void init() {
				
			}
			
			void shutdown() {
				
			}
			
			/*
			 */
			void update() {
				
			}
			
			/*
			 */
			void update_clicked() {
				
				references.delete();
				
				string names[];
				
				// editor node references
				forloop(int i = 0; engine.editor.getNumNodes()) {
					Node node = engine.editor.getNode(i);
					if(node.isEnabled() && node.getType() == NODE_REFERENCE) {
						NodeReference reference = node_cast(node);
						string name = reference.getNodeName();
						if(names.check(name)) names[name]++;
						else names.append(name,1);
					}
				}
				
				// references list
				foreachkey(string name; names) {
					references.append(new Reference(name,names[name]));
				}
			}
			
			/*
			 */
			void collect_clicked() {
				
				// selected names
				string names[];
				foreach(Reference reference; references) {
					if(reference.isSelected()) {
						names.append(reference.getName());
					}
				}
				
				// get references
				Node nodes[0];
				forloop(int i = 0; engine.editor.getNumNodes()) {
					Node node = engine.editor.getNode(i);
					if(node.isEnabled() && node.getType() == NODE_REFERENCE) {
						NodeReference reference = node_cast(node);
						if(names.check(reference.getNodeName())) {
							nodes.append(node);
						}
					}
				}
				
				if(nodes.size() > 0) {
					int nums[0];
					Node parents[0];
					Node new_parents[0];
					
					foreach(Node node; nodes) {
						nums.append(get_node_position(node));
						parents.append(node.getParent());
						new_parents.append(cluster);
					}
					
					save_state(create_state_reparent(nodes,nums,parents,new_parents,(cluster),cluster));
				}
				
				// reparent nodes
				foreach(Node node; nodes) {
					Node parent = node.getParent();
					for(int i = node.getNumChilds() - 1; i >= 0; i--) {
						Node child = node.getChild(i);
						child.setWorldParent(parent);
					}
					cluster.addWorldChild(node);
				}
				
				Nodes::update_nodes();
				Nodes::reselect_nodes();
			}
			
		} /* namespace Collector */
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Cluster"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
			
			parameters_tb.addTab(TR("Generator"));
			parameters_tb.addChild(generator_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(generator_vb);
			
			parameters_tb.addTab(TR("Collector"));
			parameters_tb.addChild(collector_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(collector_vb);
			
			Generator::init();
			
			Collector::init();
		}
		
		void shutdown() {
			
			Generator::shutdown();
			
			Collector::shutdown();
		}
		
		/*
		 */
		void update() {
			
			cluster = node_cast(node);
			
			visible_distance_el.setText(editorFormat(cluster.getVisibleDistance()));
			fade_distance_el.setText(editorFormat(cluster.getFadeDistance()));
			shadow_radius_el.setText(editorFormat(cluster.getShadowRadius()));
			
			Generator::update();
			
			Collector::update();
		}
		
		/*
		 */
		void update_dynamic() {
			
			info_l.setText(format(TR("Nodes: %d"),cluster.getNumNodes()));
			
			cluster.renderVisualizer();
			
			engine.visualizer.renderNodeBoundBox(cluster,vec4(0.0f,0.75f,0.0f,1.0f));
		}
		
		/*
		 */
		void create_clicked() {
			
			Node selection[0];
			nodesGetNodes(selection);
			
			NodesState_ states[0];
			states.append(create_state_modify((cluster),cluster));
			
			Node nodes[0];
			Node temp[0];
			Node childs[0];
			
			Mat4 transform = cluster.getWorldTransform();
			
			// cluster nodes
			forloop(int i = 0; cluster.getNumNodes()) {
				Node node = node_cast(node_clone(cluster.getNodeReference(i)));
				node.setParent(cluster);
				node.setWorldTransform(transform * cluster.getNodeTransform(i));
				node.setName(cluster.getNodeName(i));
				node.setEnabled(1);
				nodes.append(node_append(node));
				temp.append(node);
			}
			
			// child nodes
			forloop(int i = 0; cluster.getNumChilds()) {
				Node node = node_cast(cluster.getChild(i));
				if(node.isEnabled() == 0) continue;
				if(node.getNumChilds() != 0) continue;
				if(engine.editor.isNode(node) == 0) continue;
				if(node.getType() != NODE_REFERENCE) continue;
				nodes.append(node);
				childs.append(node);
			}
			// create nodes
			cluster.createNodes(nodes);
			temp.delete();
			
			forloop(int i = 0; childs.size()) {
				states.append(create_state_remove((childs[i]),selection,cluster));
				engine.editor.releaseNode(childs[i]);
			}
			
			// save unredo states
			save_states(states);
		}
		
		void expand_clicked() {
			
			Node selection[0];
			nodesGetNodes(selection);
			
			NodesState_ states[0];
			states.append(create_state_modify((cluster),cluster));
			Mat4 transform = cluster.getWorldTransform();
			
			// cluster nodes
			forloop(int i = 0; cluster.getNumNodes()) {
				Node node = node_clone(cluster.getNodeReference(i));
				states.append(create_state_new((node),(),NULL));
				node.setParent(cluster);
				node.setWorldTransform(transform * cluster.getNodeTransform(i));
				node.setName(cluster.getNodeName(i));
				node.setEnabled(1);
				engine.editor.addNode(node);
			}
			// save unredo states
			save_states(states);
			// clear nodes
			cluster.clearNodes();
		}
		
		void clear_clicked() {
			if(dialogMessageYesNo(TR("Confirm"),TR("Remove all cluster nodes?"))) {
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,cluster);
				cluster.clearNodes();
				save_state(s);
			}
		}
		
		/*
		 */
		PRESSED_CALLBACK_FLOAT(visible_distance,cluster,VisibleDistance,cluster,modify)
		PRESSED_CALLBACK_FLOAT(fade_distance,cluster,FadeDistance,cluster,modify)
		PRESSED_CALLBACK_FLOAT(shadow_radius,cluster,ShadowRadius,cluster,modify)
		
	} /* namespace Cluster */
	
	/**************************************************************************\
	*
	* WorldClutter
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Clutter {
		
		/*
		 */
		WidgetVBox main_vb;
		
		WidgetGridBox references_gb;
		
		WidgetCheckBox orientation_cb;
		WidgetCheckBox intersection_cb;
		WidgetCheckBox mask_flip_x_cb;
		WidgetCheckBox mask_flip_y_cb;
		WidgetCheckBox mask_inverse_cb;
		WidgetEditLine mask_image_el;
		WidgetEditLine mask_mesh_el;
		WidgetIcon mask_edit_i;
		
		WidgetEditLine mask_min_value_el;
		WidgetEditLine mask_max_value_el;
		
		WidgetEditLine visible_distance_el;
		WidgetEditLine fade_distance_el;
		
		WidgetEditLine spawn_rate_el;
		WidgetEditLine seed_el;
		
		WidgetEditLine size_x_el;
		WidgetEditLine size_y_el;
		WidgetEditLine step_el;
		WidgetEditLine density_el;
		WidgetSlider density_sl;
		WidgetEditLine threshold_el;
		WidgetSlider threshold_sl;
		WidgetEditLine angle_el;
		WidgetSlider angle_sl;
		
		WidgetEditLine min_scale_mean_el;
		WidgetEditLine min_scale_spread_el;
		WidgetEditLine max_scale_mean_el;
		WidgetEditLine max_scale_spread_el;
		
		WidgetEditLine offset_mean_el;
		WidgetEditLine offset_spread_el;
		WidgetEditLine rotation_mean_el[3];
		WidgetEditLine rotation_spread_el[3];
		
		WorldClutter clutter;
		
		/*
		 */
		class Reference {
			
			int num;
			WidgetCheckBox remove_cb;
			WidgetLabel name_l;
			WidgetSlider probability_sl;
			
			Reference(int n) {
				
				num = n;
				
				remove_cb = new WidgetCheckBox(gui);
				remove_cb.setToolTip(TR("Check to remove reference node"));
				references_gb.addChild(remove_cb);
				
				name_l = new WidgetLabel(gui,basename(clutter.getReferenceName(num)));
				name_l.setToolTip(clutter.getReferenceName(num));
				references_gb.addChild(name_l,GUI_ALIGN_LEFT);
				
				probability_sl = new WidgetSlider(gui,0,1000,clutter.getReferenceProbability(num) * 1000);
				probability_sl.setToolTip(TR("Probability of the reference node"));
				probability_sl.setCallback(GUI_CHANGED,functionid(probability_changed_redirector),this);
				references_gb.addChild(probability_sl,GUI_ALIGN_EXPAND);
			}
			~Reference() {
				delete remove_cb;
				delete name_l;
				delete probability_sl;
			}
			
			int isRemoved() {
				return remove_cb.isChecked();
			}
			
			void probability_changed() {
				float probability = probability_sl.getValue() / 1000.0f;
				if(clutter.getReferenceProbability(num) != probability) {
					NodesState_ s = new NodesState_();
					s.init_modify(nodes,clutter);
					clutter.setReferenceProbability(num,probability);
					save_state(s);
				}
			}
			
			void probability_changed_redirector(Reference reference) {
				reference.probability_changed();
			}
		};
		
		Reference references[0];
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Clutter"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			brushesDisable();
		}
		
		/*
		 */
		void update() {
			
			clutter = node_cast(node);
			
			setCallbackEnabled(GUI_CLICKED,0,(
				orientation_cb,
				intersection_cb,
				mask_flip_x_cb,
				mask_flip_y_cb,
				mask_inverse_cb,
			));
			
			references.delete();
			forloop(int i = 0; clutter.getNumReferences()) {
				references.append(new Reference(i));
			}
			
			orientation_cb.setChecked(clutter.getOrientation());
			intersection_cb.setChecked(clutter.getIntersection());
			mask_flip_x_cb.setChecked(clutter.getMaskFlipX());
			mask_flip_y_cb.setChecked(clutter.getMaskFlipY());
			mask_inverse_cb.setChecked(clutter.getMaskInverse());
			
			mask_image_el.setText(clutter.getMaskImageName());
			mask_mesh_el.setText(clutter.getMaskMeshName());
			
			mask_min_value_el.setText(string(clutter.getMaskMinValue()));
			mask_max_value_el.setText(string(clutter.getMaskMaxValue()));
			
			visible_distance_el.setText(editorFormat(clutter.getVisibleDistance()));
			fade_distance_el.setText(editorFormat(clutter.getFadeDistance()));
			
			spawn_rate_el.setText(string(clutter.getSpawnRate()));
			seed_el.setText(string(clutter.getSeed()));
			
			size_x_el.setText(editorFormat(clutter.getSizeX()));
			size_y_el.setText(editorFormat(clutter.getSizeY()));
			step_el.setText(editorFormat(clutter.getStep()));
			density_el.setText(editorFormat(clutter.getDensity(),5));
			threshold_el.setText(editorFormat(clutter.getThreshold()));
			angle_el.setText(editorFormat(clutter.getAngle()));
			
			min_scale_mean_el.setText(editorFormat(clutter.getMinScaleMean()));
			min_scale_spread_el.setText(editorFormat(clutter.getMinScaleSpread()));
			max_scale_mean_el.setText(editorFormat(clutter.getMaxScaleMean()));
			max_scale_spread_el.setText(editorFormat(clutter.getMaxScaleSpread()));
			
			offset_mean_el.setText(editorFormat(clutter.getOffsetMean()));
			offset_spread_el.setText(editorFormat(clutter.getOffsetSpread()));
			setEditLineVec3(rotation_mean_el,clutter.getRotationMean());
			setEditLineVec3(rotation_spread_el,clutter.getRotationSpread());
			
			update_mask_icons();
			
			setCallbackEnabled(GUI_CLICKED,1,(
				orientation_cb,
				intersection_cb,
				mask_flip_x_cb,
				mask_flip_y_cb,
				mask_inverse_cb,
			));
		}
		
		/*
		 */
		void update_dynamic() {
			
			clutter.renderVisualizer();
			
			engine.visualizer.renderNodeBoundBox(clutter,vec4(0.0f,0.75f,0.0f,1.0f));
		}
		
		/*
		 */
		void update_mask_icons() {
			mask_edit_i.setEnabled(clutter.getMaskImageName() != "");
		}
		
		/*
		 */
		void add_clicked() {
			string name;
			if(dialogFileNode(TR("Select node reference"),name,DIALOG_FILE_OPEN)) {
				forloop(int i = 0; clutter.getNumReferences()) {
					if(clutter.getReferenceName(i) == name) {
						dialogMessageOk(TR("Error"),format(TR("\"%s\" is already exists in the list"),name));
						return;
					}
				}
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,clutter);
				int num = clutter.addReference(name);
				references.append(new Reference(num));
				save_state(s);
			}
		}
		
		void remove_clicked() {
			int remove[0];
			forloop(int i = 0; references.size()) {
				if(references[i].isRemoved()) remove.append(i);
			}
			if(remove.size() == 0) return;
			if(dialogMessageYesNo(TR("Confirm"),TR("Remove nodes from the list?"))) {
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,clutter);
				for(int i = remove.size() - 1; i >= 0; i--) {
					clutter.removeReference(remove[i]);
				}
				references.delete();
				forloop(int i = 0; clutter.getNumReferences()) {
					references.append(new Reference(i));
				}
				save_state(s);
			}
		}
		
		void reload_clicked() {
			forloop(int i = 0; clutter.getNumReferences()) {
				clutter.setReferenceName(i,clutter.getReferenceName(i));
			}
		}
		
		/*
		 */
		void randomize_clicked() {
			seed_el.setText(string(engine.game.getRandom(0,0x0fffffff)));
			seed_el.runCallback(GUI_PRESSED);
		}
		
		/*
		 */
		void mask_image_update_pressed() {
			mask_image_pressed();
			update_mask_icons();
		}
		
		void mask_image_load_update_clicked() {
			mask_image_load_clicked();
			update_mask_icons();
		}
		
		void mask_image_create_clicked() {
			
			int size = 0;
			if(dialogImageSize(TR("Mask size"),size) == 0) return;
			
			string name;
			if(dialogFileImageSave(TR("Select texture to save"),name) == 0) return;
			
			if(dialogFileConfirm(name) == 0) return;
			
			Image image = new Image();
			image.create2D(size,size,IMAGE_FORMAT_R8);
			
			image.set(0,0);
			
			if(image.save(name) == 0) {
				dialogMessageOk(TR("Error"));
				
				delete image;
				return;
			}
			
			log.message("Save %s %s into the \"%s\" file\n",image.getTypeName(),image.getFormatName(),name);
			
			delete image;
			
			clutter.setMaskImageName(name);
			mask_image_el.setText(name);
			
			update_mask_icons();
		}
		
		void mask_image_edit_clicked() {
			maskEnable(clutter,Brushes::Mask::CHANNEL_R,clutter.getMaskMinValue() / 255.0f,clutter.getMaskMaxValue() / 255.0f);
		}
		
		void mask_image_clear_update_clicked() {
			mask_image_clear_clicked();
			update_mask_icons();
		}
		
		/*
		 */
		CLICKED_CALLBACK(orientation,clutter,Orientation,clutter)
		CLICKED_CALLBACK(intersection,clutter,Intersection,clutter)
		CLICKED_CALLBACK(mask_flip_x,clutter,MaskFlipX,clutter)
		CLICKED_CALLBACK(mask_flip_y,clutter,MaskFlipY,clutter)
		CLICKED_CALLBACK(mask_inverse,clutter,MaskInverse,clutter)
		PRESSED_CALLBACK(mask_image,clutter,MaskImageName,clutter,string,modify)
		PRESSED_CALLBACK(mask_mesh,clutter,MaskMeshName,clutter,string,modify)
		LOAD_CLICKED_CALLBACK(mask_image,clutter,MaskImageName,TR("Select mask image"))
		VIEW_CLICKED_CALLBACK(mask_image,clutter,MaskImageName,TR("Clutter mask image"))
		CLEAR_CLICKED_CALLBACK(mask_image,clutter,MaskImageName,TR("Clear mask image?"),NULL,"")
		LOAD_CLICKED_CALLBACK_MESH(mask_mesh,clutter,MaskMeshName,TR("Select mask mesh"))
		CLEAR_CLICKED_CALLBACK(mask_mesh,clutter,MaskMeshName,TR("Clear mask mesh?"),NULL,"")
		PRESSED_CALLBACK(mask_min_value,clutter,MaskMinValue,clutter,int,modify)
		PRESSED_CALLBACK(mask_max_value,clutter,MaskMaxValue,clutter,int,modify)
		PRESSED_CALLBACK_FLOAT(visible_distance,clutter,VisibleDistance,clutter,modify)
		PRESSED_CALLBACK_FLOAT(fade_distance,clutter,FadeDistance,clutter,modify)
		PRESSED_CALLBACK(spawn_rate,clutter,SpawnRate,clutter,int,modify)
		PRESSED_CALLBACK(seed,clutter,Seed,clutter,int,modify)
		PRESSED_CALLBACK_FLOAT(size_x,clutter,SizeX,clutter,modify)
		PRESSED_CALLBACK_FLOAT(size_y,clutter,SizeY,clutter,modify)
		PRESSED_CALLBACK_FLOAT(step,clutter,Step,clutter,modify)
		PRESSED_CALLBACK_SLIDER_PREC(density,clutter,Density,5,clutter,modify)
		PRESSED_CALLBACK_SLIDER(threshold,clutter,Threshold,clutter,modify)
		PRESSED_CALLBACK_SLIDER(angle,clutter,Angle,clutter,modify)
		PRESSED_CALLBACK_MEAN_SPREAD(min_scale,clutter,MinScale)
		PRESSED_CALLBACK_MEAN_SPREAD(max_scale,clutter,MaxScale)
		PRESSED_CALLBACK_MEAN_SPREAD(offset,clutter,Offset)
		PRESSED_CALLBACK_MEAN_SPREAD_VEC3(rotation,clutter,Rotation)
		
	} /* namespace Clutter */
	
	/**************************************************************************\
	*
	* WorldSwitcher
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Switcher {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetCheckBox point_cb;
		WidgetEditLine size_el[3];
		WidgetEditLine offset_el[3];
		WidgetEditLine min_distance_el;
		WidgetEditLine max_distance_el;
		
		WorldSwitcher switcher;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Switcher"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			switcher = node_cast(node);
			
			point_cb.setCallbackEnabled(GUI_CLICKED,0);
			
			point_cb.setChecked(switcher.isPoint());
			setEditLineVec3(size_el,switcher.getSize());
			setEditLineVec3(offset_el,switcher.getOffset());
			min_distance_el.setText(editorFormat(switcher.getMinDistance()));
			max_distance_el.setText(editorFormat(switcher.getMaxDistance()));
			
			point_cb.setCallbackEnabled(GUI_CLICKED,1);
		}
		
		/*
		 */
		void update_dynamic() {
			switcher.renderVisualizer();
		}
		
		/*
		 */
		CLICKED_CALLBACK(point,switcher,Point,switcher)
		PRESSED_CALLBACK_VEC3(size,switcher,Size,switcher,modify)
		PRESSED_CALLBACK_VEC3(offset,switcher,Offset,switcher,modify)
		PRESSED_CALLBACK_FLOAT(min_distance,switcher,MinDistance,switcher,modify)
		PRESSED_CALLBACK_FLOAT(max_distance,switcher,MaxDistance,switcher,modify)
		
	} /* namespace Switcher */
	
	/**************************************************************************\
	*
	* WorldOccluder
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Occluder {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetCheckBox back_face_cb;
		WidgetEditLine size_el[3];
		WidgetEditLine distance_el;
		
		WidgetManipulatorTranslator translator_m[2];
		
		WorldOccluder occluder;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Occluder"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
			if(translator_m[0] != NULL) {
				gui.removeChild(translator_m[0]);
				gui.removeChild(translator_m[1]);
			}
		}
		
		/*
		 */
		void update() {
			
			occluder = node_cast(node);
			
			back_face_cb.setCallbackEnabled(GUI_CLICKED,0);
			
			back_face_cb.setChecked(occluder.isBackFace());
			setEditLineVec3(size_el,occluder.getSize());
			distance_el.setText(editorFormat(occluder.getDistance()));
			
			back_face_cb.setCallbackEnabled(GUI_CLICKED,1);
		}
		
		/*
		 */
		void update_dynamic() {
			
			if(translator_m[0] == NULL) {
				translator_m[0] = new WidgetManipulatorTranslator(gui);
				translator_m[1] = new WidgetManipulatorTranslator(gui);
				translator_m[0].setCallback(GUI_CHANGED,functionid(translator_changed),0);
				translator_m[0].setCallback(GUI_CLICKED,functionid(slider_clicked));
				translator_m[1].setCallback(GUI_CHANGED,functionid(translator_changed),1);
				translator_m[1].setCallback(GUI_CLICKED,functionid(slider_clicked));
			}
			
			if(panelPlacementGetSelector() == 0) {
				updateAuxManipulator(translator_m[0]);
				updateAuxManipulator(translator_m[1]);
				mat4 transform = occluder.getWorldTransform();
				if(translator_m[0].isFocused() == 0) {
					translator_m[0].setBasis(transform);
					translator_m[0].setCallbackEnabled(GUI_CHANGED,0);
					translator_m[0].setTransform(transform * translate(-occluder.getSize() * 0.5f));
					translator_m[0].setCallbackEnabled(GUI_CHANGED,1);
				}
				if(translator_m[1].isFocused() == 0) {
					translator_m[1].setBasis(transform * scale(-1.0f,-1.0f,-1.0f));
					translator_m[1].setCallbackEnabled(GUI_CHANGED,0);
					translator_m[1].setTransform(transform * translate(occluder.getSize() * 0.5f));
					translator_m[1].setCallbackEnabled(GUI_CHANGED,1);
				}
				if(editorIsChild(translator_m[0]) == 0) {
					gui.addChild(translator_m[0],GUI_ALIGN_OVERLAP);
					gui.addChild(translator_m[1],GUI_ALIGN_OVERLAP);
				}
			}
			else {
				gui.removeChild(translator_m[0]);
				gui.removeChild(translator_m[1]);
			}
			
			occluder.renderVisualizer();
		}
		
		/*
		 */
		CLICKED_CALLBACK(back_face,occluder,BackFace,occluder)
		PRESSED_CALLBACK_VEC3(size,occluder,Size,occluder,modify)
		PRESSED_CALLBACK_FLOAT(distance,occluder,Distance,occluder,modify)
		WORLD_TRANSLATOR_CHANGED_VEC3(occluder)
		
	} /* namespace Occluder */
	
	/**************************************************************************\
	*
	* WorldOccluderMesh
	*
	\**************************************************************************/
	
	/*
	 */
	namespace OccluderMesh {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetEditLine distance_el;
		
		WorldOccluderMesh occluder;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Occluder"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			occluder = node_cast(node);
			
			distance_el.setText(editorFormat(occluder.getDistance()));
		}
		
		/*
		 */
		void update_dynamic() {
			occluder.renderVisualizer();
		}
		
		/*
		 */
		void load_clicked() {
			string name;
			if(dialogFileMesh(TR("Select mesh to load"),name,DIALOG_FILE_OPEN)) {
				if(occluder.loadMesh(name) == 0) dialogMessageOk(TR("Error"));
			}
		}
		
		void save_clicked() {
			string name;
			if(dialogFileMesh(TR("Select mesh to save"),name,DIALOG_FILE_SAVE)) {
				name = savePath(name);
				
				if(occluder.saveMesh(name) == 0) dialogMessageOk(TR("Error"));
			}
		}
		
		/*
		 */
		PRESSED_CALLBACK_FLOAT(distance,occluder,Distance,occluder,modify)
		
	} /* namespace OccluderMesh */
	
	/**************************************************************************\
	*
	* WorldOccluderTerrain
	*
	\**************************************************************************/
	
	/*
	 */
	namespace OccluderTerrain {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetEditLine size_el[3];
		WidgetEditLine radius_el;
		WidgetEditLine distance_el;
		WidgetEditLine heights_image_el;
		
		WorldOccluderTerrain occluder;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("OccluderTerrain"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			occluder = node_cast(node);
			
			setEditLineVec3(size_el,occluder.getSize());
			radius_el.setText(editorFormat(occluder.getRadius()));
			distance_el.setText(editorFormat(occluder.getDistance()));
			heights_image_el.setText(occluder.getHeightsImageName());
		}
		
		/*
		 */
		void update_dynamic() {
			occluder.renderVisualizer();
		}
		
		/*
		 */
		void heights_image_pressed() {
			string name = heights_image_el.getText();
			if(occluder.getHeightsImageName() != name) {
				if(occluder.setHeightsImageName(name)) {
					if(occluder.getHeightsImage() != NULL) {
						int format = -1;
						Image image = new Image();
						if(image.info(name) || image.load(name)) {
							format = image.getFormat();
						}
						delete image;
						image = occluder.getHeightsImage();
						if(image.getFormat() != format) {
							if(dialogMessageYesNo(TR("Confirm"),::format(TR("Update \"%s\" heights image?"),name))) {
								if(image.save(name) == 0) {
									dialogMessageOk(TR("Error"));
									return;
								}
								log.message("Save occluder heights into the \"%s\" file",name);
							}
						}
					}
				} else {
					dialogMessageOk(TR("Error"));
				}
			}
		}
		
		void heights_image_load_clicked() {
			string name = occluder.getHeightsImageName();
			if(dialogFileImage(TR("Select heights image"),name)) {
				heights_image_el.setText(name);
				heights_image_pressed();
			}
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(size,occluder,Size,occluder,modify)
		PRESSED_CALLBACK_FLOAT(radius,occluder,Radius,occluder,modify)
		PRESSED_CALLBACK_FLOAT(distance,occluder,Distance,occluder,modify)
		VIEW_CLICKED_CALLBACK(heights_image,occluder,HeightsImageName,TR("Occluder heights image"))
		CLEAR_CLICKED_CALLBACK(heights_image,occluder,HeightsImageName,TR("Clear heights image?"),NULL,"")
		
	} /* namespace OccluderTerrain */
	
	/**************************************************************************\
	*
	* WorldTransformPath
	*
	\**************************************************************************/
	
	/*
	 */
	namespace TransformPath {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetEditLine path_el;
		WidgetCheckBox orientation_cb;
		WidgetCheckBox loop_cb;
		WidgetEditLine radius_el;
		WidgetEditLine time_el;
		WidgetEditLine speed_el;
		WidgetButton play_b;
		WidgetButton stop_b;
		
		WorldTransformPath transform;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Transform "));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			transform = node_cast(node);
			
			setCallbackEnabled(GUI_CLICKED,0,(
				orientation_cb,
				loop_cb,
			));
			
			path_el.setText(transform.getPathName());
			orientation_cb.setChecked(transform.getOrientation());
			loop_cb.setChecked(transform.getLoop());
			radius_el.setText(editorFormat(transform.getRadius()));
			time_el.setText(editorFormat(transform.getTime()));
			speed_el.setText(editorFormat(transform.getSpeed()));
			
			setCallbackEnabled(GUI_CLICKED,1,(
				orientation_cb,
				loop_cb,
			));
		}
		
		/*
		 */
		void update_dynamic() {
			
			if(time_el.isFocused() == 0) time_el.setText(editorFormat(transform.getTime()));
			
			play_b.setEnabled(transform.isStopped());
			stop_b.setEnabled(transform.isPlaying());
			
			transform.renderVisualizer();
		}
		
		/*
		 */
		void path_pressed() {
			transform.setPathName(path_el.getText());
		}
		
		void path_load_clicked() {
			string name = transform.getPathName();
			if(dialogFilePath(TR("Select path to load"),name,DIALOG_FILE_OPEN)) {
				Path path = new Path();
				if(path.load(name)) {
					transform.setPathName(name);
					path_el.setText(transform.getPathName());
				} else {
					dialogMessageOk(TR("Error"));
				}
				delete path;
			}
		}
		
		/*
		 */
		void play_clicked() {
			transform.play();
		}
		
		void stop_clicked() {
			transform.stop();
		}
		
		/*
		 */
		CLICKED_CALLBACK(orientation,transform,Orientation,transform)
		CLICKED_CALLBACK(loop,transform,Loop,transform)
		PRESSED_CALLBACK_FLOAT(radius,transform,Radius,transform,modify)
		PRESSED_CALLBACK_FLOAT(time,transform,Time,transform,modify)
		PRESSED_CALLBACK_FLOAT(speed,transform,Speed,transform,modify)
		
	} /* namespace TransformPath */
	
	/**************************************************************************\
	*
	* WorldTransformBone
	*
	\**************************************************************************/
	
	/*
	 */
	namespace TransformBone {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetEditLine radius_el;
		WidgetComboBox bone_cb;
		
		WorldTransformBone transform;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Transform "));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			transform = node_cast(node);
			
			radius_el.setText(editorFormat(transform.getRadius()));
			
			Node parent = transform.getParent();
			bone_cb.clear();
			
			if(parent != NULL && parent.getType() == NODE_OBJECT_MESH_SKINNED) {
				ObjectMeshSkinned skin = node_cast(parent);
				forloop(int i = 0; skin.getNumBones()) {
					bone_cb.addItem(skin.getBoneName(i));
				}
				bone_cb.setCurrentItem(skin.findBone(transform.getBoneName()));
			}
		}
		
		/*
		 */
		void update_dynamic() {
			transform.renderVisualizer();
		}
		
		/*
		 */
		void bone_changed() {
			transform.setBoneName(bone_cb.getCurrentItemText());
		}
		
		/*
		 */
		PRESSED_CALLBACK_FLOAT(radius,transform,Radius,transform,modify)
		
	} /* namespace TransformBone */
	
	/**************************************************************************\
	*
	* WorldExpression
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Expression {
		
		/*
		 */
		WidgetVBox main_vb;
		WidgetEditLine size_el[3];
		WidgetEditLine offset_el[3];
		WidgetEditLine distance_el;
		WidgetEditLine ifps_el;
		WidgetEditLine file_el;
		WidgetEditText expression_et;
		WidgetIcon file_edit_i;
		WidgetIcon file_update_i;
		
		WorldExpression expression;
		
		/*
		 */
		void init() {
			
			editorEditText(expression_et);
			
			parameters_tb.addTab(TR("Expression"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			expression = node_cast(node);
			
			setEditLineVec3(size_el,expression.getSize());
			setEditLineVec3(offset_el,expression.getOffset());
			distance_el.setText(editorFormat(expression.getDistance()));
			ifps_el.setText(editorFormat(expression.getIFps()));
			
			update_expression();
		}
		
		/*
		 */
		void update_dynamic() {
			expression.renderVisualizer();
		}
		
		/*
		 */
		void update_expression() {
			string res[0];
			if(re_search(expression.getExpression(),"{\n+#include +<(.+)>\n+}$",res)) {
				file_el.setText(res[0]);
			} else {
				file_el.setText("");
			}
			expression_et.setText(expression.getExpression());
		}
		
		/*
		 */
		void file_pressed() {
			if(replace(file_el.getText()," ","") == "") return;
			
			if(!is_file(savePath(file_el.getText()))) {
				log.error("Can't open \"%s\" file\n",file_el.getText());
				dialogMessageOk(TR("Error"));
				return;
			}
			
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,expression);
			if(expression.setExpression(format("{\n#include <%s>\n}",file_el.getText())) == 0) {
				dialogMessageOk(TR("Error"));
			}
			update_expression();
			save_state(s);
		}
		
		/*
		 */
		void file_changed() {
			int enabled = is_file(savePath(file_el.getText()));
			file_edit_i.setEnabled(enabled);
			file_update_i.setEnabled(enabled);
		}
		
		void file_load_clicked() {
			string name = file_el.getText();
			if(dialogFile(TR("Select WorldExpression source"),".h",name,DIALOG_FILE_OPEN)) {
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,expression);
				if(expression.setExpression(format("{\n#include <%s>\n}",name)) == 0) {
					dialogMessageOk(TR("Error"));
				}
				update_expression();
				save_state(s);
			}
		}
		
		void file_edit_clicked() {
			openExternalEditor(file_el.getText(),"text_editor");
		}
		
		void file_update_clicked() {
			expression.setExpression(expression.getExpression());
		}
		
		void expression_pressed() {
			if(expression.getExpression() != expression_et.getText()) {
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,expression);
				if(expression.setExpression(expression_et.getText()) == 0 && expression_et.getText() != "") {
					dialogMessageOk(TR("Error"));
				}
				update_expression();
				save_state(s);
			}
		}
		
		/*
		 */
		PRESSED_CALLBACK_VEC3(size,expression,Size,expression,modify)
		PRESSED_CALLBACK_VEC3(offset,expression,Offset,expression,modify)
		PRESSED_CALLBACK_FLOAT(distance,expression,Distance,expression,modify)
		PRESSED_CALLBACK_FLOAT(ifps,expression,IFps,expression,modify)
		
	} /* namespace Expression */
	
	/**********************************************************************\
	*
	* WorldExtern
	*
	\**********************************************************************/
	
	/*
	 */
	namespace Extern {
		
		/*
		 */
		void init() {
			
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
		}
		
		/*
		 */
		void update_dynamic() {
			
		}
		
	} /* namespace Extern */
	
} /* namespace World */

/*
 */
WorldLayer worldLayerResize(WorldLayer layer,string name) {
	return World::Layer::resize(layer,name);
}
