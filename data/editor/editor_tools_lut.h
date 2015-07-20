/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_tools_lut.h
 * Desc:    Unigine editor
 * Version: 1.02
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

/*
 */
Image image;
string name;
Unigine::Widgets::Line diffuse_l;
Unigine::Widgets::Line specular_l;
Unigine::Widgets::LineCurveColor diffuse_c;
Unigine::Widgets::LineCurveColor specular_c;

/*
 */
void init() {
	
	using Unigine::Widgets;
	
	image = new Image();
	
	diffuse_l = new Line(0.0f,1.0f);
	specular_l = new Line(0.0f,1.0f);
	diffuse_l.setCallback(LINE_KEY_CREATED,functionid(update_image));
	diffuse_l.setCallback(LINE_KEY_CHANGED,functionid(update_image));
	diffuse_l.setCallback(LINE_KEY_REMOVED,functionid(update_image));
	specular_l.setCallback(LINE_KEY_CREATED,functionid(update_image));
	specular_l.setCallback(LINE_KEY_CHANGED,functionid(update_image));
	specular_l.setCallback(LINE_KEY_REMOVED,functionid(update_image));
	
	diffuse_l.canvas_sb.setHeight(0);
	specular_l.canvas_sb.setHeight(0);
	diffuse_l.canvas_sb.setVScrollEnabled(0);
	specular_l.canvas_sb.setVScrollEnabled(0);
	diffuse_vb.addChild(diffuse_l.widget,GUI_ALIGN_EXPAND);
	specular_vb.addChild(specular_l.widget,GUI_ALIGN_EXPAND);
	
	diffuse_c = new LineCurveColor();
	specular_c = new LineCurveColor();
	diffuse_l.addCurve(diffuse_c);
	specular_l.addCurve(specular_c);
	
	size_changed();
	
	clear();
}

void shutdown() {
	
	delete image;
}

/*
 */
void update() {
	
	diffuse_l.update();
	specular_l.update();
}

/*
 */
void clear() {
	
	using Unigine::Widgets;
	
	name = "";
	
	width_cb.setCurrentItem(1);
	height_cb.setCurrentItem(1);
	
	diffuse_c.clear();
	diffuse_c.addKey(new LineKeyColor(0.0f,vec4(0.0f,0.0f,0.0f,1.0f)));
	diffuse_c.addKey(new LineKeyColor(0.5f,vec4(0.0f,0.0f,0.0f,1.0f)));
	diffuse_c.addKey(new LineKeyColor(1.0f,vec4(1.0f,1.0f,1.0f,1.0f)));
	diffuse_scale_el.setText(editorFormat(1.0f));
	
	specular_c.clear();
	specular_c.addKey(new LineKeyColor(0.0f,vec4(0.0f,0.0f,0.0f,1.0f)));
	specular_c.addKey(new LineKeyColor(1.0f,vec4(1.0f,1.0f,1.0f,1.0f)));
	specular_scale_el.setText(editorFormat(1.0f));
	specular_power_el.setText(editorFormat(16.0f));
	specular_angle_el.setText(editorFormat(1.0f));
	specular_shift_el.setText(editorFormat(1.0f));
	
	update_b.setEnabled(0);
	
	update_image();
}

/*
 */
void update_image() {
	
	int width = image.getWidth();
	int height = image.getHeight();
	float diffuse_scale = float(diffuse_scale_el.getText());
	float specular_scale = float(specular_scale_el.getText());
	float specular_power = float(specular_power_el.getText());
	float specular_angle = float(specular_angle_el.getText());
	float specular_shift = float(specular_shift_el.getText());
	
	float iwidth = 1.0f / (width - 1);
	float iheight = 1.0f / (height - 1);
	
	float specular_power_0 = max(specular_power - specular_power * saturate(1.0f - specular_angle),EPSILON);
	float specular_power_1 = max(specular_power - specular_power * saturate(specular_angle - 1.0f),EPSILON);
	
	float specular_shift_0 = 1.0f - saturate(specular_shift - 1.0f);
	float specular_shift_1 = 1.0f - saturate(1.0f - specular_shift);
	
	forloop(int x = 0; width) {
		vec4 diffuse = diffuse_c.get_color(x * iwidth) * diffuse_scale;
		float specular_power = lerp(specular_power_0,specular_power_1,x * iwidth);
		float specular_shift = lerp(specular_shift_0,specular_shift_1,x * iwidth);
		forloop(int y = 0; height) {
			vec4 specular = specular_c.get_color(pow(y * iheight,specular_power)) * specular_scale * specular_shift;
			image.set2D(x,y,(specular + diffuse) * 0.25f);
		}
	}
	
	Image texture = new Image();
	texture.copy(image);
	texture.resize(256,256);
	texture_s.setImage(texture);
	delete texture;
}

/*
 */
void size_changed() {
	int width = int(width_cb.getCurrentItemText());
	int height = int(height_cb.getCurrentItemText());
	image.create2D(width,height,IMAGE_FORMAT_RGB8);
	update_image();
}

#define PRESSED_CALLBACK(NAME) \
void NAME ## _pressed() { \
	float value = float(NAME ## _el.getText()); \
	NAME ## _el.setText(editorFormat(value)); \
	update_image(); \
}

PRESSED_CALLBACK(diffuse_scale)
PRESSED_CALLBACK(specular_scale)
PRESSED_CALLBACK(specular_power)
PRESSED_CALLBACK(specular_angle)
PRESSED_CALLBACK(specular_shift)

#undef PRESSED_CALLBACK

/*
 */
void clear_clicked() {
	
	if(name == "") return;
	
	if(dialogMessageYesNo(TR("Confirm"),format(TR("Clear \"%s\" LUT texture?"),basename(name)))) {
		clear();
	}
}

void update_clicked() {
	
	using Unigine::Widgets;
	
	if(image.save(name)) {
		
		// save config
		Xml xml = new Xml("lut","version=\"1.0\"");
		
		// width
		Xml x = xml.addChild("width");
		x.setIntData(width_cb.getCurrentItem());
		
		// height
		x = xml.addChild("height");
		x.setIntData(height_cb.getCurrentItem());
		
		// diffuse
		x = xml.addChild("diffuse");
		x.setFloatArg("scale",float(diffuse_scale_el.getText()));
		forloop(int i = 0; diffuse_c.getNumKeys()) {
			LineKeyColor key = diffuse_c.getKey(i);
			Xml xml = x.addChild("key");
			xml.setFloatArg("time",key.getTime());
			xml.setVec4Data(key.getColor());
		}
		
		// specular
		x = xml.addChild("specular");
		x.setFloatArg("scale",float(specular_scale_el.getText()));
		x.setFloatArg("power",float(specular_power_el.getText()));
		x.setFloatArg("angle",float(specular_angle_el.getText()));
		x.setFloatArg("shift",float(specular_shift_el.getText()));
		forloop(int i = 0; specular_c.getNumKeys()) {
			LineKeyColor key = specular_c.getKey(i);
			Xml xml = x.addChild("key");
			xml.setFloatArg("time",key.getTime());
			xml.setVec4Data(key.getColor());
		}
		
		xml.save(extension(name,"lut"));
		delete xml;
		
		update_b.setEnabled(1);
	}
}

/*
 */
void load_clicked() {
	
	using Unigine::Widgets;
	
	if(dialogFileImage(TR("Select LUT texture to load"),name,DIALOG_FILE_OPEN)) {
		
		// load config
		Xml xml = new Xml();
		if(xml.load(extension(name,"lut"))) {
			
			// width
			if(xml.isChild("width")) {
				Xml x = xml.getChild("width");
				width_cb.setCurrentItem(x.getIntData());
			}
			
			// width
			if(xml.isChild("height")) {
				Xml x = xml.getChild("height");
				height_cb.setCurrentItem(x.getIntData());
			}
			
			// diffuse
			if(xml.isChild("diffuse")) {
				Xml x = xml.getChild("diffuse");
				if(x.isArg("scale")) diffuse_scale_el.setText(editorFormat(x.getFloatArg("scale")));
				diffuse_c.clear();
				forloop(int i = 0; x.getNumChilds()) {
					Xml xml = x.getChild(i);
					if(xml.getName() == "key" && xml.isArg("time")) {
						diffuse_c.addKey(new LineKeyColor(xml.getFloatArg("time"),xml.getVec4Data()));
					}
				}
			}
			
			// specular
			if(xml.isChild("specular")) {
				Xml x = xml.getChild("specular");
				if(x.isArg("scale")) specular_scale_el.setText(editorFormat(x.getFloatArg("scale")));
				if(x.isArg("power")) specular_power_el.setText(editorFormat(x.getFloatArg("power")));
				if(x.isArg("angle")) specular_angle_el.setText(editorFormat(x.getFloatArg("angle")));
				if(x.isArg("shift")) specular_shift_el.setText(editorFormat(x.getFloatArg("shift")));
				specular_c.clear();
				forloop(int i = 0; x.getNumChilds()) {
					Xml xml = x.getChild(i);
					if(xml.getName() == "key" && xml.isArg("time")) {
						specular_c.addKey(new LineKeyColor(xml.getFloatArg("time"),xml.getVec4Data()));
					}
				}
			}
		}
		delete xml;
		
		update_image();
		
		update_b.setEnabled(1);
	}
}

void save_clicked() {
	
	if(dialogFileImageSave(TR("Select LUT texture to save"),name)) update_clicked();
}
