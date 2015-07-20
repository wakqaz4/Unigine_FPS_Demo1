/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_objects_billboards.h
 * Desc:    Unigine editor
 * Version: 1.04
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
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
* Callbacks
*
\******************************************************************************/

/*
 */
#define BILLBOARDS_PRESSED_CALLBACK(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	if(num == -1) return; \
	NodesState_ s = new NodesState_(); \
	if(WIDGET ## _el.isFocused() && billboards.get ## FUNC(num) != float(WIDGET ## _el.getText())) { \
		s.init_modify(nodes,billboards); \
		billboards.set ## FUNC(num,float(WIDGET ## _el.getText())); \
	} \
	WIDGET ## _el.setText(editorFormat(billboards.get ## FUNC(num))); \
	save_state(s,engine.gui.getFocus()); \
}

#define BILLBOARDS_PRESSED_CALLBACK_VEC3(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	if(num == -1) return; \
	NodesState_ s = new NodesState_(); \
	if(billboards.get ## FUNC(num) != getEditLineVec3(WIDGET ## _el)) { \
		s.init_modify(nodes,billboards); \
		billboards.set ## FUNC(num,getEditLineVec3(WIDGET ## _el)); \
	} \
	setEditLineVec3(WIDGET ## _el,billboards.get ## FUNC(num)); \
	save_state(s); \
}

#define BILLBOARDS_PRESSED_CALLBACK_VEC4(WIDGET,FUNC) \
void WIDGET ## _pressed() { \
	if(num == -1) return; \
	NodesState_ s = new NodesState_(); \
	if(billboards.get ## FUNC(num) != getEditLineVec4(WIDGET ## _el)) { \
		s.init_modify(nodes,billboards); \
		billboards.set ## FUNC(num,getEditLineVec4(WIDGET ## _el)); \
	} \
	setEditLineVec4(WIDGET ## _el,billboards.get ## FUNC(num)); \
	save_state(s); \
}

/******************************************************************************\
*
* ObjectBillboards
*
\******************************************************************************/

/*
 */
namespace Billboards {
	
	/*
	 */
	WidgetVBox main_vb;
	WidgetVBox billboards_vb;
	WidgetVBox generator_vb;
	WidgetVBox stars_vb;
	WidgetCheckBox depth_sort_cb;
	
	ObjectBillboards billboards;
	
	/**************************************************************************\
	*
	* Billboards
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Billboards {
		
		/*
		 */
		WidgetListBox billboards_lb;	// billboards
		WidgetButton remove_b;
		WidgetEditLine position_el[3];
		WidgetEditLine texcoord_el[4];
		WidgetEditLine width_el;
		WidgetEditLine height_el;
		WidgetEditLine angle_el;
		
		/*
		 */
		int num = -1;
		WidgetManipulatorTranslator translator_m;
		
		/*
		 */
		void init() {
			
		}
		
		void shutdown() {
			if(translator_m != NULL) gui.removeChild(translator_m);
		}
		
		/*
		 */
		void update_view() {
			
			// clear view
			if(num < 0 || num >= billboards.getNumBillboards()) {
				
				setEnabled(0,(
					position_el[0],
					position_el[1],
					texcoord_el[0],
					texcoord_el[1],
					texcoord_el[2],
					texcoord_el[3],
					width_el,
					height_el,
					angle_el,
				));
				
				forloop(int i = 0; 3) {
					position_el[i].setText("");
				}
				
				forloop(int i = 0; 4) {
					texcoord_el[i].setText("");
				}
				
				width_el.setText("");
				height_el.setText("");
				angle_el.setText("");
			}
			// fill view
			else {
				
				setEnabled(1,(
					position_el[0],
					position_el[1],
					texcoord_el[0],
					texcoord_el[1],
					texcoord_el[2],
					texcoord_el[3],
					width_el,
					height_el,
					angle_el,
				));
				
				setEditLineVec3(position_el,billboards.getPosition(num));
				setEditLineVec4(texcoord_el,billboards.getTexCoord(num));
				width_el.setText(editorFormat(billboards.getWidth(num)));
				height_el.setText(editorFormat(billboards.getHeight(num)));
				angle_el.setText(editorFormat(billboards.getAngle(num)));
			}
		}
		
		/*
		 */
		void update() {
			
			num = -1;
			
			billboards_lb.clear();
			forloop(int i = 0; billboards.getNumBillboards()) {
				billboards_lb.addItem(format("billboard_%d",i));
			}
			billboards_lb.setCurrentItem(-1);
			
			if(billboards.getNumBillboards()) {
				remove_b.setEnabled(1);
				billboards_lb.setCurrentItem(0);
			} else {
				remove_b.setEnabled(0);
				update_view();
			}
		}
		
		/*
		 */
		void add_clicked() {
			
			NodesState_ s = new NodesState_();
			s.init_modify(nodes,object);
			
			billboards.addBillboard(1.0f,1.0f);
			
			save_state(s);
			
			update();
		}
		
		void remove_clicked() {
			
			if(dialogMessageYesNo(TR("Confirm"),format(TR("Remove billboard %d from the %s?"),num,node.getTypeName()))) {
				
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,object);
				
				billboards.removeBillboard(num);
				
				save_state(s);
				
				update();
			}
		}
		
		/*
		 */
		void billboard_changed() {
			num = billboards_lb.getCurrentItem();
			update_view();
		}
		
		/*
		 */
		void translator_changed() {
			if(num < 0 || num >= billboards.getNumBillboards()) return;
			if(translator_m.isFocused() == 0) return;
			NodesState_ s = new NodesState_();
			translator_m.setBasis(translator_m.getTransform());
			vec3 position = vec3((billboards.getIWorldTransform() * translator_m.getTransform()) * vec3_zero);
			if(compare(billboards.getPosition(num),position,STATE_EPSILON) == 0) s.init_modify(nodes,billboards);
			billboards.setPosition(num,position);
			setEditLineVec3(position_el,position);
			save_state(s,translator_m);
		}		
		
		/*
		 */
		BILLBOARDS_PRESSED_CALLBACK_VEC3(position,Position)
		BILLBOARDS_PRESSED_CALLBACK_VEC4(texcoord,TexCoord)
		BILLBOARDS_PRESSED_CALLBACK(width,Width)
		BILLBOARDS_PRESSED_CALLBACK(height,Height)
		BILLBOARDS_PRESSED_CALLBACK(angle,Angle)
		
		/*
		 */
		void update_dynamic() {
			
			// current billboard
			if(parameters_tb.getCurrentTab() == 3 && num >= 0 && num < billboards.getNumBillboards()) {
				
				// create manipulators
				if(translator_m == NULL) {
					translator_m = new WidgetManipulatorTranslator(gui);
					translator_m.setCallback(GUI_CHANGED,functionid(translator_changed));
					translator_m.setCallback(GUI_CLICKED,functionid(slider_clicked));
				}
				
				// translator
				if(panelPlacementGetTranslator()) {
					updateAuxManipulator(translator_m);
					if(translator_m.isFocused() == 0) {
						mat4 transform = billboards.getWorldTransform() * translate(billboards.getPosition(num));
						translator_m.setCallbackEnabled(GUI_CHANGED,0);
						translator_m.setBasis(transform);
						translator_m.setTransform(transform);
						translator_m.setCallbackEnabled(GUI_CHANGED,1);
					}
					if(editorIsChild(translator_m) == 0) {
						gui.addChild(translator_m,GUI_ALIGN_OVERLAP);
						//gui.removeChild(rotator_m);
					}
				}
				else {
					gui.removeChild(translator_m);
				}
			}
			else if(translator_m != NULL) {
				gui.removeChild(translator_m);
			}
		}
		
	} /* namespace Billboards */
	
	/**************************************************************************\
	*
	* Generator
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Generator {
		
		/*
		 */
		WidgetEditLine count_el;
		WidgetEditLine atlas_width_el;
		WidgetEditLine atlas_height_el;
		WidgetEditLine radius_mean_el;
		WidgetEditLine radius_spread_el;
		WidgetEditLine aspect_mean_el;
		WidgetEditLine aspect_spread_el;
		WidgetEditLine angle_mean_el;
		WidgetEditLine angle_spread_el;
		WidgetEditLine mesh_el;
		
		/*
		 */
		int count = 16;
		int atlas_width = 1;
		int atlas_height = 1;
		float radius_mean = 1.0f;
		float radius_spread = 0.0f;
		float aspect_mean = 1.0f;
		float aspect_spread = 0.0f;
		float angle_mean = 0.0f;
		float angle_spread = 0.0f;
		
		/*
		 */
		void init() {
			
		}
		
		void shutdown() {
			
			if(engine.world.isNode(billboards)) {
				
				Xml xml = new Xml("generator");
				
				xmlSetData(xml,"count",count);
				xmlSetData(xml,"atlas_width",atlas_width);
				xmlSetData(xml,"atlas_height",atlas_height);
				xmlSetData(xml,"radius_mean",radius_mean);
				xmlSetData(xml,"radius_spread",radius_spread);
				xmlSetData(xml,"aspect_mean",aspect_mean);
				xmlSetData(xml,"aspect_spread",aspect_spread);
				xmlSetData(xml,"angle_mean",angle_mean);
				xmlSetData(xml,"angle_spread",angle_spread);
				xmlSetData(xml,"mesh",mesh_el.getText());
				
				billboards.setData(xml.getSubTree());
				delete xml;
			}
		}
		
		/*
		 */
		void update() {
			
			Xml xml = new Xml();
			if(xml.parse(billboards.getData()) && xml.getName() == "generator") {
				count = xmlGetData(xml,"count",count);
				atlas_width = xmlGetData(xml,"atlas_width",atlas_width);
				atlas_height = xmlGetData(xml,"atlas_height",atlas_height);
				radius_mean = xmlGetData(xml,"radius_mean",radius_mean);
				radius_spread = xmlGetData(xml,"radius_spread",radius_spread);
				aspect_mean = xmlGetData(xml,"aspect_mean",aspect_mean);
				aspect_spread = xmlGetData(xml,"aspect_spread",aspect_spread);
				angle_mean = xmlGetData(xml,"angle_mean",angle_mean);
				angle_spread = xmlGetData(xml,"angle_spread",angle_spread);
				mesh_el.setText(xmlGetData(xml,"mesh",""));
			}
			delete xml;
			
			count_el.setText(string(count));
			atlas_width_el.setText(string(atlas_width));
			atlas_height_el.setText(string(atlas_height));
			radius_mean_el.setText(editorFormat(radius_mean));
			radius_spread_el.setText(editorFormat(radius_spread));
			aspect_mean_el.setText(editorFormat(aspect_mean));
			aspect_spread_el.setText(editorFormat(aspect_spread));
			angle_mean_el.setText(editorFormat(angle_mean));
			angle_spread_el.setText(editorFormat(angle_spread));
		}
		
		/*
		 */
		void count_pressed() {
			count = clamp(int(count_el.getText()),1,8192);
			count_el.setText(string(count));
		}
		
		void atlas_width_pressed() {
			atlas_width = max(int(atlas_width_el.getText()),1);
			atlas_width_el.setText(string(atlas_width));
		}
		
		void atlas_height_pressed() {
			atlas_height = max(int(atlas_height_el.getText()),1);
			atlas_height_el.setText(string(atlas_height));
		}
		
		void radius_mean_pressed() {
			radius_mean = max(float(radius_mean_el.getText()),0.0f);
			radius_mean_el.setText(editorFormat(radius_mean));
		}
		
		void radius_spread_pressed() {
			radius_spread = max(float(radius_spread_el.getText()),0.0f);
			radius_spread_el.setText(editorFormat(radius_spread));
		}
		
		void aspect_mean_pressed() {
			aspect_mean = max(float(aspect_mean_el.getText()),0.0f);
			aspect_mean_el.setText(editorFormat(aspect_mean));
		}
		
		void aspect_spread_pressed() {
			aspect_spread = max(float(aspect_spread_el.getText()),0.0f);
			aspect_spread_el.setText(editorFormat(aspect_spread));
		}
		
		void angle_mean_pressed() {
			angle_mean = float(angle_mean_el.getText());
			angle_mean_el.setText(editorFormat(angle_mean));
		}
		
		void angle_spread_pressed() {
			angle_spread = max(float(angle_spread_el.getText()),0.0f);
			angle_spread_el.setText(editorFormat(angle_spread));
		}
		
		/*
		 */
		void mesh_pressed() {
			if(strlen(mesh_el.getText())) {
				File file = new File();
				if(file.open(mesh_el.getText(),"rb") == 0) {
					dialogMessageOk(TR("Error"),format(TR("Can't open \"%s\" bounding mesh"),mesh_el.getText()));
					mesh_el.setText("");
				} else {
					file.close();
				}
				delete file;
			}
		}
			
		void mesh_load_clicked() {
			string name = mesh_el.getText();
			if(dialogFileMesh(TR("Select bounding mesh"),name,DIALOG_FILE_OPEN)) {
				mesh_el.setText(name);
			}
		}
		
		void mesh_clear_clicked() {
			if(strlen(mesh_el.getText())) {
				if(dialogMessageYesNo(TR("Confirm"),TR("Clear bounding mesh?"))) {
					mesh_el.setText("");
				}
			}
		}
		
		/*
		 */
		int get_num_intersections(ObjectMeshStatic mesh,vec3 p0,vec3 p1) {
			int num = 0;
			ObjectIntersection intersection = new ObjectIntersection();
			forloop(int i = 0; mesh.getNumSurfaces()) {
				while(num < 100) {
					if(mesh.getIntersection(p0,p1,intersection,i)) {
						p0 = intersection.getPoint() + normalize(p1 - p0) * 1e-3f;
						num++;
					} else {
						break;
					}
				}
			}
			return num;
		}
		
		/*
		 */
		void create_clicked() {
			
			if(strlen(mesh_el.getText()) == 0) {
				dialogMessageOk(TR("Error"),TR("Select mesh"));
				return;
			}
			
			ObjectMeshStatic mesh = NULL;
			mesh = new ObjectMeshStatic(mesh_el.getText());
			if(mesh == NULL) {
				dialogMessageOk(TR("Error"),format(TR("Can't load \"%s\" bounding mesh\n"),mesh_el.getText()));
				delete mesh;
				return;
			}
			
			mesh.setEnabled(0);
			if(mesh.getNumSurfaces() == 0) {
				dialogMessageOk(TR("Error"),format(TR("Can't load \"%s\" bounding mesh\n"),mesh_el.getText()));
				delete mesh;
				return;
			}
			
			// begin progress dialog
			dialogProgressBegin(TR("Creating ObjectBillboards..."));
			
			BoundBox bb = mesh.getBoundBox();
			vec3 bound_min = bb.getMin();
			vec3 bound_max = bb.getMax();
			vec3 bound_size = bound_max - bound_min;
			vec3 bound_center = (bound_min + bound_max) * 0.5f;
			
			float step = pow(bound_size.x * bound_size.y * bound_size.z / count,0.33f);
			
			int size_x = int(ceil(bound_size.x / step));
			int size_y = int(ceil(bound_size.y / step));
			int size_z = int(ceil(bound_size.z / step));
			
			billboards.clearBillboards();
			
			float iatlas_width = 1.0f / atlas_width;
			float iatlas_height = 1.0f / atlas_height;
			
			// create billboards
			for(int i = 0; i < 16; i++) {
				forloop(int z = 0; size_z + 1) {
					float Z = (z - size_z * 0.5f) * step;
					forloop(int y = 0; size_y + 1) {
						float Y = (y - size_y * 0.5f) * step;
						forloop(int x = 0; size_x + 1) {
							float X = (x - size_x * 0.5f) * step;
							
							vec3 position = bound_center + vec3(X,Y,Z) + engine.game.getRandom(-vec3(step),vec3(step)) * 0.5f;
							if(get_num_intersections(mesh,position,position - bound_size.00z) % 2 && get_num_intersections(mesh,position,position + bound_size.00z) % 2) {
								float radius = radius_mean + engine.game.getRandom(-radius_spread,radius_spread);
								float aspect = aspect_mean + engine.game.getRandom(-aspect_spread,aspect_spread);
								int num = billboards.addBillboard(radius,radius * aspect);
								int atlas_x = engine.game.getRandom(0,atlas_width);
								int atlas_y = engine.game.getRandom(0,atlas_height);
								billboards.setTexCoord(num,vec4(iatlas_width,iatlas_height,atlas_x * iatlas_width,atlas_y * iatlas_height));
								billboards.setAngle(num,angle_mean + engine.game.getRandom(-angle_spread,angle_spread));
								billboards.setPosition(num,position);
							}
							
							int progress = (billboards.getNumBillboards() * 100 / count);
							if(billboards.getNumBillboards() == count || dialogProgressUpdate(progress) == 0) {
								Billboards::update();
								dialogProgressEnd();
								delete mesh;
								return;
							}
						}
					}
				}
			}
			
			// end progress dialog
			Billboards::update();
			dialogProgressEnd();
			delete mesh;
		}
		
		void clear_clicked() {
			if(dialogMessageYesNo(TR("Confirm"),TR("Clear billboards?"))) {
				billboards.clearBillboards();
				Billboards::update();
			}
		}
		
	} /* namespace Generator */
	
	/**************************************************************************\
	*
	* Stars
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Stars {
		
		/*
		 */
		WidgetEditLine count_el;
		WidgetEditLine radius_el;
		WidgetEditLine angle_el;
		WidgetEditLine catalog_el;
		
		WidgetEditLine latitude_el;
		WidgetEditLine longitude_el;
		WidgetComboBox location_cb;
		
		/*
		 */
		int count = 8192;
		float radius = 9000.0f;
		float angle = 0.5f;
		string texture = "";
		double latitude = 0.0;
		double longitude = 0.0;
		
		/*
		 */
		string locations[] = (
			TR("San Francisco")		: dvec3( 37.766083,-122.475586,0.0),
			TR("New York")			: dvec3( 40.659528, -74.003906,0.0),
			TR("London")			: dvec3( 51.309556,  -0.043945,0.0),
			TR("Paris")				: dvec3( 48.799145,   2.548828,0.0),
			TR("Tomsk")				: dvec3( 56.480291,  85.122070,0.0),
			TR("Hong Kong")			: dvec3( 22.499701, 114.049072,0.0),
			TR("Tokyo")				: dvec3( 35.682882, 139.707642,0.0),
			TR("Sidney")			: dvec3(-33.891089, 151.215820,0.0),
		);
		
		/*
		 */
		void init() {
			
			count = configGet("billboards_stars_count",count);
			radius = configGet("billboards_stars_radius",radius);
			angle = configGet("billboards_stars_angle",angle);
			catalog_el.setText(configGet("billboards_stars_catalog",""));
			texture = configGet("billboards_stars_texture","");
			latitude = configGet("billboards_stars_latitude",latitude);
			longitude = configGet("billboards_stars_longitude",longitude);
			location_cb.setCurrentItem(configGet("billboards_stars_location",0));
		}
		
		void shutdown() {
			
			configSet("billboards_stars_count",count);
			configSet("billboards_stars_radius",radius);
			configSet("billboards_stars_angle",angle);
			configSet("billboards_stars_catalog",catalog_el.getText());
			configSet("billboards_stars_texture",texture);
			configSet("billboards_stars_latitude",latitude);
			configSet("billboards_stars_longitude",longitude);
			configSet("billboards_stars_location",location_cb.getCurrentItem());
		}
		
		/*
		 */
		void update() {
			
			count_el.setText(string(count));
			radius_el.setText(editorFormat(radius));
			angle_el.setText(editorFormat(angle));
			latitude_el.setText(location_format(latitude));
			longitude_el.setText(location_format(longitude));
		}
		
		/*
		 */
		void count_pressed() {
			count = clamp(int(count_el.getText()),1,8192);
			count_el.setText(string(count));
		}
		
		void radius_pressed() {
			radius = max(float(radius_el.getText()),0.0f);
			radius_el.setText(editorFormat(radius));
		}
		
		void angle_pressed() {
			angle = float(angle_el.getText());
			angle_el.setText(editorFormat(angle));
		}
		
		/*
		 */
		void catalog_pressed() {
			if(strlen(catalog_el.getText())) {
				File file = new File();
				if(file.open(catalog_el.getText(),"rb") == 0) {
					dialogMessageOk(TR("Error"),format(TR("Can't open \"%s\" star catalog"),catalog_el.getText()));
					catalog_el.setText("");
				} else {
					file.close();
				}
				delete file;
			}
		}
			
		void catalog_load_clicked() {
			string name = catalog_el.getText();
			if(dialogFile(TR("Select stars FK5 type catalog"),"",name,DIALOG_FILE_OPEN)) {
				catalog_el.setText(name);
			}
		}
		
		void catalog_clear_clicked() {
			if(strlen(catalog_el.getText())) {
				if(dialogMessageYesNo(TR("Confirm"),TR("Clear stars catalog?"))) {
					catalog_el.setText("");
				}
			}
		}
		
		/*
		 */
		string location_format(double value) {
			return format("%.6f",value);
		}
		
		void select_location() {
			foreachkey(string name; locations) {
				dvec3 location = locations[name];
				if(location.x != latitude) continue;
				if(location.y != longitude) continue;
				forloop(int i = 0; location_cb.getNumItems()) {
					if(location_cb.getItemText(i) == name) {
						location_cb.setCurrentItem(i);
						return;
					}
				}
			}
			location_cb.setCurrentItem(0);
		}
		
		/*
		 */
		void latitude_pressed() {
			latitude = double(latitude_el.getText());
			latitude_el.setText(location_format(latitude));
			select_location();
		}
		
		void longitude_pressed() {
			longitude = double(longitude_el.getText());
			longitude_el.setText(location_format(longitude));
			select_location();
		}
		
		void location_changed() {
			if(locations.check(location_cb.getCurrentItemText()) == 0) return;
			dvec3 location = locations[location_cb.getCurrentItemText()];
			latitude = location.x;
			longitude = location.y;
			latitude_el.setText(location_format(latitude));
			longitude_el.setText(location_format(longitude));
		}
		
		/*
		 */
		void create_clicked() {
			
			if(strlen(catalog_el.getText()) == 0) {
				dialogMessageOk(TR("Error"),TR("Select catalog"));
				return;
			}
			
			File file = new File();
			if(file.open(catalog_el.getText(),"rb") == 0) {
				dialogMessageOk(TR("Error"),format(TR("Can't load \"%s\" star catalog\n"),catalog_el.getText()));
				delete file;
				return;
			}
			
			billboards.clearBillboards();
			
			class Star {
				float phi;
				float theta;
				float mag;
				string type;
				Star(float phi_,float theta_,float mag_,string type_) {
					phi = phi_;
					theta = theta_;
					mag = mag_;
					type = type_;
				}
				int operator<(Star s0,Star s1) {
					return (s0.mag > s1.mag);
				}
			};
			
			Star stars[0];
			
			float min_mag = INFINITY;
			float max_mag = -INFINITY;
			
			while(file.eof() == 0) {
				
				string line = file.readLine();
				
				string get_bytes(int from,int to) {
					string ret = "";
					if(strlen(line) < to) return ret;
					for(int i = from; i <= to; i++) {
						ret += format("%c",line[i - 1]);
					}
					return ret;
				}
				
				float rah = float(get_bytes(6,7));
				float ram = float(get_bytes(9,10));
				float ras = float(get_bytes(12,17));
				float decd = float(get_bytes(27,29));
				float decam = float(get_bytes(31,32));
				float decas = float(get_bytes(34,38));
				float mag = float(get_bytes(124,128));
				string type = get_bytes(131,132);
				
				float theta = PI2 * (rah + (ram + ras / 60.0f) / 60.0f) / 24.0f;
				float phi = -PI * (decd + (decam + (decas / 60.0f)) / 60.0f) / 90.0f;
				
				stars.append(new Star(phi,theta,mag,type));
				
				min_mag = min(min_mag,mag);
				max_mag = max(max_mag,mag);
			}
			
			stars.sort();
			
			string types[] = ( 'O' : 0, 'B' : 1, 'A' : 2, 'F' : 3, 'G' : 4, 'K' : 5, 'M' : 6 );
			
			forloop(int i = 0; min(count,stars.size())) {
				Star star = stars[i];
				
				if(types.check(star.type[0]) == 0) continue;
				
				float r = angle * pow((star.mag - min_mag) / (max_mag - min_mag),3.0f);
				
				int atlas_x = clamp(int(16.0f * (max_mag - star.mag) / (max_mag - min_mag)),0,15);
				int atlas_y = types[star.type[0]];
				
				int num = billboards.addBillboard(r,r);
				billboards.setTexCoord(num,vec4(1.0f / 16.0f,1.0f / 8.0f,atlas_x / 16.0f,atlas_y / 8.0f));
				billboards.setAngle(num,engine.game.getRandom(0.0f,360.0f));
				
				float phi = star.phi + latitude * DEG2RAD;
				float theta = star.theta + longitude * DEG2RAD;
				billboards.setPosition(num,vec3(cos(phi) * cos(theta),-cos(phi) * sin(theta),sin(phi)) * radius);
			}
			
			stars.delete();
			
			file.close();
			delete file;
			
			Billboards::update();
		}
		
		void clear_clicked() {
			if(dialogMessageYesNo(TR("Confirm"),TR("Clear billboards?"))) {
				billboards.clearBillboards();
				Billboards::update();
			}
		}
		
		/*
		 */
		void texture_clicked() {
			
			string texture;
			if(dialogFileImage(TR("Select stars texture"),texture,DIALOG_FILE_OPEN)) {
				
				int size = 16;
				int hsize = size / 2;
				
				Image image = new Image();
				image.create2D(size * 16,size * 8,IMAGE_FORMAT_RGBA8);
				
				vec3 colors[] = (
					vec3(155,176,255), vec3(170,191,255),
					vec3(202,215,255), vec3(248,247,255),
					vec3(255,244,234), vec3(255,210,161),
					vec3(255,204,111), vec3(255,204,111),
				);
				
				forloop(int Y = 0; 8) {
					forloop(int X = 0; 16) {
						vec3 center = vec3(size * X + hsize,size * Y + hsize,0.0f);
						vec3 color = colors[Y] * saturate(lerp(1.0f,0.1f,X / 16.0f)) / 255.0f;
						forloop(int y = size * Y; size * Y + size) {
							forloop(int x = size * X; size * X + size) {
								float alpha = saturate(1.0f - length(vec3(x,y,0.0f) - center) / hsize);
								image.set2D(x,y,vec4(color,pow(alpha,0.5f)));
							}
						}
					}
				}
				
				image.createMipmaps();
				image.save(texture);
				delete image;
			}
		}
		
	} /* namespace Stars */
	
	/**************************************************************************\
	*
	* Billboards
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		Nodes::Object::init();
		
		parameters_tb.addTab(TR("Params"));
		parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(main_vb);
		
		parameters_tb.addTab(TR("Billboards"));
		parameters_tb.addChild(billboards_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(billboards_vb);
		
		parameters_tb.addTab(TR("Generator"));
		parameters_tb.addChild(generator_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(generator_vb);
		
		parameters_tb.addTab(TR("Stars"));
		parameters_tb.addChild(stars_vb,GUI_ALIGN_EXPAND);
		enableCallbacks(stars_vb);
		
		Billboards::init();
		
		Generator::init();
		
		Stars::init();
	}
	
	void shutdown() {
		
		Nodes::Object::shutdown();
		
		Billboards::shutdown();
		
		Generator::shutdown();
		
		Stars::shutdown();
	}
	
	/*
	 */
	void update() {
		
		Nodes::Object::update();
		
		billboards = node_cast(node);
		
		setCallbackEnabled(GUI_CLICKED,0,(
			depth_sort_cb,
		));
		depth_sort_cb.setChecked(billboards.getDepthSort());
		setCallbackEnabled(GUI_CLICKED,1,(
			depth_sort_cb,
		));
		
		Billboards::update();
		
		Generator::update();
		
		Stars::update();
	}
	
	/*
	 */
	void update_dynamic() {
		
		Nodes::Object::update_dynamic();
		
		Billboards::update_dynamic();
	}
	
	/*
	 */
	CLICKED_CALLBACK(depth_sort,billboards,DepthSort,billboards)
	
} /* namespace Billboards */
