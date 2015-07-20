/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_dialogs.h
 * Desc:    Unigine editor
 * Version: 1.55
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
 *          Pavel Kurinnoy <figonet@unigine.com>
 *          Andrey Viktorov <unclebob@unigine.com>
 *          Alexey Egorov <evilsprit@unigine.com>
 *          Pavel Nakaznenko <crio@unigine.com>
 *          Alexey Zagniy <yingaz@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification,is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

#ifndef __EDITOR_DIALOGS_H__
#define __EDITOR_DIALOGS_H__

#include <editor/widgets/editor_widget_dialog_file.h>

/*
 */
namespace Editor::Dialogs {
	
	/**************************************************************************\
	*
	* Message dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Message {
		
		/*
		 */
		WidgetDialogMessage dialog;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			dialog = new WidgetDialogMessage(gui);
			
			editorDialog(dialog);
			dialog.setSizeable(0);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::Message)
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		int run(string title,string message,int rich = 0) {
			
			dialog.setMessageFontRich(rich);
			dialog.setText(title);
			dialog.setMessageText(message);
			
			dialog.setOkText(TR("Ok"));
			dialog.setCancelText(NULL);
			dialog.setCloseText(NULL);
			
			dialog.arrange();
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			return dialog.isOkClicked();
		}
		
		/*
		 */
		int runOk(string title,string message,int rich = 0) {
			
			dialog.setMessageFontRich(rich);
			dialog.setText(title);
			
			if(is_string(message)) {
				dialog.setMessageHidden(0);
				dialog.setMessageText(message);
			} else {
				dialog.setMessageHidden(1);
				dialog.addChild(message,GUI_ALIGN_EXPAND);
			}
			
			dialog.setOkText(TR("Ok"));
			dialog.setCancelText(NULL);
			dialog.setCloseText(NULL);
			
			dialog.arrange();
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(is_string(message) == 0) {
				dialog.setMessageHidden(0);
				dialog.removeChild(message);
			}
			
			return dialog.isOkClicked();
		}
		
		/*
		 */
		int runCancelContinue(string title,string message,int rich = 0) {
			
			dialog.setMessageFontRich(rich);
			dialog.setText(title);
			dialog.setMessageText(message);
			
			dialog.setOkText(TR("Cancel"));
			dialog.setCancelText(TR("Continue"));
			dialog.setCloseText(NULL);
			
			dialog.arrange();
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			return dialog.isOkClicked();
		}
		
		/*
		 */
		int runYesNo(string title,string message,int rich = 0) {
			
			dialog.setMessageFontRich(rich);
			dialog.setText(title);
			dialog.setMessageText(message);
			
			dialog.setOkText(TR("Yes"));
			dialog.setCancelText(TR("No"));
			dialog.setCloseText(NULL);
			
			dialog.arrange();
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			return dialog.isOkClicked();
		}
		
		/*
		 */
		int runYesNoCancel(string title,string message,int rich = 0) {
			
			dialog.setMessageFontRich(rich);
			dialog.setText(title);
			dialog.setMessageText(message);
			
			dialog.setOkText(TR("Yes"));
			dialog.setCancelText(TR("No"));
			dialog.setCloseText(TR("Cancel"));
			
			dialog.arrange();
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			return dialog.getResult();
		}
		
	} /* namespace Message */
	
	/**************************************************************************\
	*
	* Progress dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Progress {
		
		/*
		 */
		WidgetDialog dialog;
		
		DECLARE_DIALOG
		
		WidgetSprite progress_s[2];
		WidgetGroupBox progress_gb[2];
		int old_progress[2];
		
		int always_update = 0;
		float old_time = 0.0f;
		float begin_time = 0.0f;
		
		/*
		 */
		void init() {
			
			dialog = new WidgetDialog(gui);
			
			editorDialog(dialog);
			dialog.setSizeable(0);
			
			forloop(int i = 0; 2) {
				progress_s[i] = new WidgetSprite(gui);
				progress_gb[i] = new WidgetGroupBox(gui);
				progress_gb[i].addChild(progress_s[i],GUI_ALIGN_EXPAND);
				dialog.addChild(progress_gb[i],GUI_ALIGN_EXPAND);
			}
			WidgetButton(dialog.getOkButton()).setHidden(1);
			
			forloop(int i = 0; 2) {
				progress_s[i].addLayer();
				progress_s[i].setLayerTexture(0,"core/gui/gui_editor.png");
				progress_s[i].setLayerTexture(1,"core/gui/gui_editor.png");
				progress_s[i].setLayerColor(0,vec4(0.0f,0.0f,0.0f,0.0f));
				progress_s[i].setLayerColor(0,vec4(0.0f,0.0f,0.0f,0.0f));
				progress_s[i].setLayerTransform(0,scale(20.0f,1.0f,1.0f));
			}
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::Progress)
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		int begin(string title,int cancel_button = 1) {
			
			WidgetButton(dialog.getCancelButton()).setHidden(!cancel_button);
			
			dialog.setText(title);
			
			progress_gb[1].setHidden(1);
			progress_s[0].setLayerTransform(1,scale(0.0f,1.0f,1.0f));
			progress_s[1].setLayerTransform(1,scale(0.0f,1.0f,1.0f));
			
			dialog.arrange();
			DECLARE_DIALOG_SHOW
			
			always_update = toolsGetAlwaysUpdate();
			toolsSetAlwaysUpdate(1);
			
			old_progress[0] = -1;
			old_progress[1] = -1;
			
			old_time = clock();
			begin_time = clock();
			
			return 1;
		}
		
		int update(int progress) {
			
			if(old_progress[0] != progress || clock() - old_time > 0.2f) {
				progress_s[0].setLayerTransform(1,scale(progress / 5.0f,1.0f,1.0f));
				old_progress[0] = progress;
				old_time = clock();
				wait 1;
			}
			
			return (dialog.isDone() == 0);
		}
		
		int update2(int progress) {
			
			progress_gb[1].setHidden(0);
			
			if(old_progress[1] != progress || clock() - old_time > 0.2f) {
				progress_s[1].setLayerTransform(1,scale(progress / 5.0f,1.0f,1.0f));
				old_progress[1] = progress;
				old_time = clock();
				wait 1;
			}
			
			return (dialog.isDone() == 0);
		}
		
		int end() {
			
			log.message("Done %.2f seconds\n",clock() - begin_time);
			
			toolsSetAlwaysUpdate(always_update);
			
			DECLARE_DIALOG_HIDE
			
			return dialog.isOkClicked();
		}
		
	} /* namespace Progress */
	
	/**************************************************************************\
	*
	* FileConfirm dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace FileConfirm {
		
		/*
		 */
		void init() {
			
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		int run(string name) {
			::File file = new ::File();
			if(file.open(name,"rb")) {
				file.close();
				delete file;
				return dialogMessageYesNo(TR("Confirm"),format(TR("Overwrite \"%s\" file?"),name));
			}
			delete file;
			
			return 1;
		}
		
	} /* namespace FileConfirm */
	
	/**************************************************************************\
	*
	* Color dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Color {
		
		/*
		 */
		WidgetDialogColor dialog;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			dialog = new WidgetDialogColor(gui);
			
			dialog.setPaletteColors(configGet("dialog_color_palette_colors",""));
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::Color)
		}
		
		void shutdown() {
			
			configSet("dialog_color_palette_colors",dialog.getPaletteColors());
		}
		
		/*
		 */
		int run(string title,int callback,vec4 color) {
			
			dialog.setText(title);
			dialog.setColor(color);
			
			dialog.setCallback(GUI_CHANGED,callback,dialog);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			dialog.setCallback(GUI_CHANGED,NULL);
			
			if(dialog.isOkClicked()) {
				call(callback,dialog);
				return 1;
			}
			
			dialog.setColor(color);
			call(callback,dialog);
			return 0;
		}
		
	} /* namespace Color */
	
	/**************************************************************************\
	*
	* Image dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Image {
		
		/*
		 */
		WidgetDialogImage dialog;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			dialog = new WidgetDialogImage(gui);
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::Image)
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		int run(string title,string name) {
			
			dialog.setText(title);
			dialog.setTexture(name);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			return dialog.isOkClicked();
		}
		
	} /* namespace Image */
	
	/**************************************************************************\
	*
	* Vec dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Vec {
		
		/*
		 */
		WidgetDialog dialog;
		
		int num_args;
		int callback;
		int num;
		
		WidgetLabel vec_l[4];
		WidgetEditLine vec_el[4];
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::Vec)
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void set_expression(string expression) {
			
			string args[0];
			getArgs(expression,args);
			for(int i = 0; i < num_args && i < args.size(); i++) {
				vec_el[i].setText(args[i]);
			}
			if(args.size() == 1) {
				forloop(int i = 1; num_args) {
					vec_el[i].setText(args[0]);
				}
			}
			
			forloop(int i = 0; num_args) {
				vec_l[i].setHidden(0);
				vec_el[i].setHidden(0);
			}
			forloop(int i = num_args; 4) {
				vec_l[i].setHidden(1);
				vec_el[i].setHidden(1);
			}
		}
		
		string get_expression() {
			
			int ret = "vec" + string(num_args) + "(";
			
			forloop(int i = 0; num_args) {
				ret += vec_el[i].getText();
				if(i != num_args - 1) ret += ",";
			}
			
			return ret + ")";
		}
		
		/*
		 */
		void vec_pressed() {
			call(callback,num,get_expression());
		}
		
		/*
		 */
		int run(string title,int cb,int n,string expression,int args) {
			
			dialog.setText(title);
			
			num_args = args;
			callback = cb;
			num = n;
			
			set_expression(expression);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				call(callback,num,get_expression());
				return 1;
			}
			
			call(callback,num,expression);
			
			return 0;
		}
		
	} /* namespace Vec */
	
	/**************************************************************************\
	*
	* TexCoord dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace TexCoord {
		
		/*
		 */
		WidgetDialog dialog;
		
		vec4 texcoord;
		string texture;
		int callback;
		int num;
		
		int old_tab;
		WidgetTabBox texcoord_tb;
		
		WidgetEditLine native_el[4];	// native transformation
		
		WidgetEditLine scale_x_el;		// procedural animation
		WidgetEditLine scale_y_el;
		WidgetEditLine frequency_el;
		WidgetEditLine amplitude_el;
		WidgetEditLine velocity_el;
		WidgetEditLine angle_el;
		
		WidgetEditLine max_el[4];		// max transformation
		
		WidgetSprite texture_s;			// visual transformation
		vec4 texture_coords;
		int texture_grab;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::TexCoord)
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void set_expression(string expression,int tab) {
			
			string args[0];
			getArgs(expression,args);
			
			if(tab == 0) {
				for(int i = 0; i < 4 && i < args.size(); i++) {
					native_el[i].setText(args[i]);
				}
			}
			else if(tab == 1) {
				float scale_x = float(args[0]);
				float scale_y = float(args[1]);
				float frequency = 0.0f;
				float amplitude = 0.0f;
				float velocity = 0.0f;
				float angle = 0.0f;
				float values[0];
				int ret = sscanf(args[2],"sin(time*%f)*%f+time*%f*sin(%f)",values);
				if(ret > 0) frequency = values[0];
				if(ret > 1) amplitude = values[1];
				if(ret > 2) velocity = values[2];
				if(ret > 3) angle = values[3];
				scale_x_el.setText(string(scale_x));
				scale_y_el.setText(string(scale_y));
				frequency_el.setText(string(frequency));
				amplitude_el.setText(string(amplitude));
				velocity_el.setText(string(velocity));
				angle_el.setText(string(angle * RAD2DEG));
			}
			else if(tab == 2) {
				float dx = float(args[0]);
				float dy = float(args[1]);
				float dz = float(args[2]);
				float dw = float(args[3]);
				float x = dx * texcoord.x;
				float y = dy * texcoord.y;
				float z = -(dz + texcoord.z * dx - 0.5f) / (dx * texcoord.x) - 0.5f;
				float w =  (dw + texcoord.w * dy - 0.5f + dy * texcoord.y) / (dy * texcoord.y) - 0.5f;
				max_el[0].setText(string(x));
				max_el[1].setText(string(y));
				max_el[2].setText(string(z));
				max_el[3].setText(string(w));
			}
			else if(tab == 3) {
				texture_coords.x = float(args[2]);
				texture_coords.y = float(args[3]);
				texture_coords.z = texture_coords.x + float(args[0]);
				texture_coords.w = texture_coords.y + float(args[1]);
			}
			else {
				throw("Editor::Dialogs::TexCoord::set_expression(): unknown tab\n");
			}
		}
		
		string get_expression(int tab) {
			
			string ret = "vec4(";
			
			if(tab == 0) {
				forloop(int i = 0; 4) {
					ret += native_el[i].getText();
					if(i != 3) ret += ",";
				}
			}
			else if(tab == 1) {
				float scale_x = float(scale_x_el.getText());
				float scale_y = float(scale_y_el.getText());
				float frequency = float(frequency_el.getText());
				float amplitude = float(amplitude_el.getText());
				float velocity = float(velocity_el.getText());
				float angle = float(angle_el.getText());
				ret += format("%g,%g",scale_x,scale_y);
				ret += format(",sin(time*%g)*%g+time*%g*sin(%g)",frequency,amplitude,velocity,angle * DEG2RAD);
				ret += format(",cos(time*%g)*%g+time*%g*cos(%g)",frequency,amplitude,velocity,angle * DEG2RAD);
			}
			else if(tab == 2) {
				float x = float(max_el[0].getText());
				float y = float(max_el[1].getText());
				float z = float(max_el[2].getText());
				float w = float(max_el[3].getText());
				float dx = x / texcoord.x;
				float dy = y / texcoord.y;
				float dz = -texcoord.z * x / texcoord.x + 0.5f - (0.5f + z) * x;
				float dw = -texcoord.w * y / texcoord.y + 1.0f - y - (0.5f - (0.5f + w) * y);
				ret += format("%g,%g,%g,%g",dx,dy,dz,dw);
			}
			else if(tab == 3) {
				ret += format("%g,",texture_coords.z - texture_coords.x);
				ret += format("%g,",texture_coords.w - texture_coords.y);
				ret += format("%g,",texture_coords.x);
				ret += format("%g",texture_coords.y);
			}
			else {
				throw("Editor::Dialogs::TexCoord::get_expression(): unknown tab\n");
			}
			
			return ret + ")";
		}
		
		/*
		 */
		void texture_update() {
			int width = texture_s.getWidth();
			int height = texture_s.getHeight();
			float sx = (texture_coords.z - texture_coords.x) * width / 8.0f;
			float sy = (texture_coords.w - texture_coords.y) * height / 8.0f;
			float dx = texture_coords.x * width;
			float dy = texture_coords.y * height;
			texture_s.setLayerTransform(1,translate(dx,dy,0.0f) * scale(sx,sy,1.0f));
		}
		
		/*
		 */
		void texcoord_changed() {
			int tab = texcoord_tb.getCurrentTab();
			if(old_tab != tab) {
				old_tab = tab;
				set_expression(get_expression(old_tab),tab);
				call(callback,num,get_expression(texcoord_tb.getCurrentTab()));
				if(tab == 3 && texture != "") {
					texture_s.setTexture(texture);
					texture_s.arrange();
					texture_update();
				}
			}
		}
		
		/*
		 */
		void texcoord_pressed() {
			call(callback,num,get_expression(texcoord_tb.getCurrentTab()));
		}
		
		/*
		 */
		void texture_pressed() {
			float x = float(texture_s.getMouseX()) / texture_s.getWidth();
			float y = float(texture_s.getMouseY()) / texture_s.getHeight();
			if(texture_grab == 0) {
				texture_grab = 1;
				texture_coords.x = x;
				texture_coords.y = y;
			} else {
				texture_coords.z = x;
				texture_coords.w = y;
				texture_update();
			}
		}
		
		void texture_clicked() {
			texture_grab = 0;
			call(callback,num,get_expression(texcoord_tb.getCurrentTab()));
		}
		
		/*
		 */
		int run(string title,int cb,int n,string expression,vec4 tc,string tex) {
			
			dialog.setText(title);
			
			texcoord = tc;
			texture = tex;
			callback = cb;
			num = n;
			
			old_tab = 0;
			texcoord_tb.setCallbackEnabled(GUI_CHANGED,0);
			texcoord_tb.setCurrentTab(0);
			set_expression(expression,0);
			set_expression(expression,1);
			set_expression(expression,2);
			set_expression(expression,3);
			texcoord_tb.setCallbackEnabled(GUI_CHANGED,1);
			
			texture_s.setTexture(NULL);
			texture_grab = 0;
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				call(callback,num,get_expression(texcoord_tb.getCurrentTab()));
				return 1;
			}
			
			call(callback,num,expression);
			
			return 0;
		}
		
	} /* namespace TexCoord */
	
	/**************************************************************************\
	*
	* Texture dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Texture {
		
		/*
		 */
		WidgetDialog dialog;
		
		Material material;
		int num;
		
		WidgetCheckBox clamp_x_cb;		// texture clamp
		WidgetCheckBox clamp_y_cb;
		WidgetCheckBox clamp_z_cb;
		WidgetCheckBox clamp_xyz_cb;
		
		WidgetCheckBox anisotropy_cb;	// texture anisotropy
		
		WidgetComboBox image_format_cb;	// image format
		WidgetComboBox mipmap_type_cb;	// image format
		
		WidgetCheckBox invert_y_cb;		// invert y channel
		
		WidgetVBox material_vbox;		// invert y channel
		WidgetVBox image_format_vb;		// invert y channel
		WidgetVBox invert_vbox;			// invert y channel
		
		int clamp_xyz = MATERIAL_WRAP_CLAMP_X | MATERIAL_WRAP_CLAMP_Y | MATERIAL_WRAP_CLAMP_Z;
		
		Textures::MetaDescriptor info;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::Texture)
		}
		
		void shutdown() {
			
		}
		
		/* fills the image format combobox with options
		 */
		void fill_image_formats(string image_type) {
			
			image_format_cb.clear();
			string conversion_formats = Textures::getConversionFormatByImageType(image_type);
			if(conversion_formats != NULL) {
				string formats[0];
				strsplit(conversion_formats,",",formats);
				foreach(string format; formats) {
					image_format_cb.addItem(upper(format));
				}
			}
		}
		
		void set_selected_mipmap_type(int type)	{
			mipmap_type_cb.setCurrentItem(type);
		}
		
		void set_selected_image_format(string format) {
			forloop(int i = 0; image_format_cb.getNumItems()) {
				if(image_format_cb.getItemText(i) == format) {
					image_format_cb.setCurrentItem(i);
					return 0;
				}
			}
			image_format_cb.setCurrentItem(-1);
		}
		
		/* "invert Y channel" combobox selection change callback
		 */
		void invert_y_changed() {
			if(info != NULL) info.invert_g = invert_y_cb.isChecked();
		}
		
		/* image format combobox selection change callback
		 */
		void image_format_changed() {
			if(info != NULL) info.image_format = Textures::getImageFormatByName(lower(image_format_cb.getCurrentItemText()));
		}
		
		/* mipmap type combobox selection change callback
		 */
		void mipmap_type_changed() {
			if(info != NULL) info.mipmap_type = mipmap_type_cb.getCurrentItem();
		}
		
		/*
		 */
		void clamp_changed() {
			
			int flags = material.getImageTextureFlags(num);
			
			flags &= ~clamp_xyz;
			
			if(clamp_x_cb.isChecked()) flags |= MATERIAL_WRAP_CLAMP_X;
			if(clamp_y_cb.isChecked()) flags |= MATERIAL_WRAP_CLAMP_Y;
			if(clamp_z_cb.isChecked()) flags |= MATERIAL_WRAP_CLAMP_Z;
			
			clamp_xyz_cb.setCallbackEnabled(GUI_CHANGED,0);
			
			clamp_xyz_cb.setChecked((flags & clamp_xyz) == clamp_xyz);
			
			clamp_xyz_cb.setCallbackEnabled(GUI_CHANGED,1);
			
			set_texture_flags(material,num,flags);
		}
		
		/*
		 */
		void clamp_xyz_changed() {
			
			int flags = material.getImageTextureFlags(num);
			
			if(clamp_xyz_cb.isChecked()) flags |= clamp_xyz;
			else flags &= ~clamp_xyz;
			
			setCallbackEnabled(GUI_CHANGED,0,(clamp_x_cb,clamp_y_cb,clamp_z_cb));
			
			clamp_x_cb.setChecked(clamp_xyz_cb.isChecked());
			clamp_y_cb.setChecked(clamp_xyz_cb.isChecked());
			clamp_z_cb.setChecked(clamp_xyz_cb.isChecked());
			
			setCallbackEnabled(GUI_CHANGED,1,(clamp_x_cb,clamp_y_cb,clamp_z_cb));
			
			set_texture_flags(material,num,flags);
		}
		
		/*
		 */
		void anisotropy_changed() {
			
			int flags = material.getImageTextureFlags(num);
			
			if(anisotropy_cb.isChecked()) flags |= MATERIAL_ANISOTROPY;
			else flags &= ~MATERIAL_ANISOTROPY;
			
			material.setImageTextureFlags(num,flags);
		}
		
		/*
		 */
		void set_texture_flags(Material m,int num,int flags) {
			
			m.setImageTextureFlags(num,flags);
			
			forloop(int i = 0; m.getNumChilds()) {
				Material child = m.getChild(i);
				child.setImageTextureFlags(num,flags);
			}
		}
		
		/*
		 */
		int run(string title,string type,Material m,int n,int overriden) {
			
			dialog.setText(title);
			
			material = m;
			num = n;
			
			string texture_name = savePath(material.getImageTextureName(num));
			
			string uncompressed_texture_name = Textures::getUncompressedImageFilename(texture_name);
			string meta_file_path = Textures::getMetaFilename(texture_name);
			Textures::MetaDescriptor original = NULL;
			if(uncompressed_texture_name != NULL) {
				original = Textures::loadImageInfo(texture_name);
				info = Textures::loadImageInfo(texture_name);
			}
			else {
				info = NULL;
				original = NULL;
			}
			int flags = material.getImageTextureFlags(num);
			
			setCallbackEnabled(GUI_CHANGED,0,(clamp_x_cb,clamp_y_cb,clamp_z_cb,clamp_xyz_cb,anisotropy_cb,image_format_cb,mipmap_type_cb,invert_y_cb));
			
			
			material_vbox.setHidden(0);
			image_format_vb.setHidden(0);
			
			clamp_x_cb.setChecked((flags & MATERIAL_WRAP_CLAMP_X) != 0);
			clamp_x_cb.setEnabled(overriden);
			clamp_y_cb.setChecked((flags & MATERIAL_WRAP_CLAMP_Y) != 0);
			clamp_y_cb.setEnabled(overriden);
			clamp_z_cb.setChecked((flags & MATERIAL_WRAP_CLAMP_Z) != 0);
			clamp_z_cb.setEnabled(overriden);
			
			clamp_xyz_cb.setChecked((flags & clamp_xyz) == clamp_xyz);
			clamp_xyz_cb.setEnabled(overriden);
			
			anisotropy_cb.setChecked((flags & MATERIAL_ANISOTROPY) != 0);
			anisotropy_cb.setEnabled(overriden);
			
			image_format_cb.setEnabled(overriden && info != NULL);
			mipmap_type_cb.setEnabled(overriden && info != NULL);
			
			string res[0];
			string lower_type;
			re_search(lower(type),"([a-z]*)_?([0-9]*)$",res);
			if(res.size() > 0) {
				lower_type = res[0];
			} else {
				log.error("Unable to parse image type %s\n",type);
			}
			
			invert_vbox.setHidden(lower_type != "normal");
			invert_y_cb.setEnabled(overriden && info != NULL);
			
			fill_image_formats(lower_type);
			if(info != NULL) {
				if(info.image_type != lower_type) {
					if(!(lower_type == "diffuse" && info.image_type == "specular") && !(lower_type == "specular" && info.image_type == "diffuse")) {	//edge case
						dialogMessageOk(TR("Warning"),format(TR("The assigned file is a %s texture, according to its meta info.\nYou might want to use proper %s texture instead."),info.image_type,lower_type));
					}
				}
				set_selected_image_format(upper(Textures::getNameByImageFormat(info.image_format)));
				set_selected_mipmap_type(info.mipmap_type);
				invert_y_cb.setChecked(info.invert_g);
			}
			else {
				set_selected_image_format("none");
				set_selected_mipmap_type(-1);
				invert_y_cb.setChecked(false);
			}
			setCallbackEnabled(GUI_CHANGED,1,(clamp_x_cb,clamp_y_cb,clamp_z_cb,clamp_xyz_cb,anisotropy_cb,image_format_cb,mipmap_type_cb,invert_y_cb));
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				if(info != NULL && !(original.isEqual(info))) {
					Textures::saveMetaUsingInfo(info,meta_file_path);
					Image source_image = new Image();
					if(source_image.load(uncompressed_texture_name) == 0) {
						dialogMessageOk(TR("Error"),TR("Unable to open source of the image"));
						return 1;
					}
					Textures::saveImageUsingInfo(source_image,texture_name,info);
					return 1;
				}
			}
			
			return 0;
		}
		
		int run(string title,string type,string texture_name,int hide_format = 0) {
			texture_name = savePath(texture_name);
			
			string uncompressed_texture_name = Textures::getUncompressedImageFilename(texture_name);
			string meta_file_path = Textures::getMetaFilename(texture_name);
			Textures::MetaDescriptor original = NULL;
			
			if(uncompressed_texture_name == NULL) return 0;
			
			original = Textures::loadImageInfo(texture_name);
			info = Textures::loadImageInfo(texture_name);
			
			if(info == NULL) return 0;
			if(original == NULL) return 0;
			
			setCallbackEnabled(GUI_CHANGED,0,(clamp_x_cb,clamp_y_cb,clamp_z_cb,clamp_xyz_cb,anisotropy_cb,image_format_cb,mipmap_type_cb,invert_y_cb));
			
			material_vbox.setHidden(1);
			image_format_vb.setHidden(hide_format);
			
			image_format_cb.setEnabled(1);
			mipmap_type_cb.setEnabled(1);
			
			string res[0];
			string lower_type;
			re_search(lower(type),"([a-z]*)_?([0-9]*)$",res);
			if(res.size() > 0) {
				lower_type = res[0];
			} else {
				log.error("Unable to parse image type %s\n",type);
			}
			
			invert_vbox.setHidden(lower_type != "normal");
			invert_y_cb.setEnabled(1);
			
			fill_image_formats(lower_type);
			if(info != NULL) {
				if(info.image_type != lower_type) {
					if(!(lower_type == "diffuse" && info.image_type == "specular") && !(lower_type == "specular" && info.image_type == "diffuse")) {	//edge case
						dialogMessageOk(TR("Warning"),format(TR("The assigned file is a %s texture, according to its meta info.\nYou might want to use proper %s texture instead."),info.image_type,lower_type));
					}
				}
				set_selected_image_format(upper(Textures::getNameByImageFormat(info.image_format)));
				set_selected_mipmap_type(info.mipmap_type);
				invert_y_cb.setChecked(info.invert_g);
			}
			else {
				set_selected_image_format("none");
				set_selected_mipmap_type(-1);
				invert_y_cb.setChecked(false);
			}
			
			setCallbackEnabled(GUI_CHANGED,1,(clamp_x_cb,clamp_y_cb,clamp_z_cb,clamp_xyz_cb,anisotropy_cb,image_format_cb,mipmap_type_cb,invert_y_cb));
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				if(!(original.isEqual(info))) {
					Textures::saveMetaUsingInfo(info,meta_file_path);
					Image source_image = new Image();
					if(source_image.load(uncompressed_texture_name) == 0) {
						dialogMessageOk(TR("Error"),TR("Unable to open source of the image"));
						return 0;
					}
					Textures::saveImageUsingInfo(source_image,texture_name,info);
					return 1;
				}
				return 0;
			}
			
			return 0;
		}
		
	} /* namespace Texture */
	
	/**************************************************************************\
	*
	* Mask dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Mask {
		
		/*
		 */
		int cb_instance;
		int callback;
		WidgetDialog dialog;
		
		WidgetGridBox mask_gb;
		WidgetButton bits_b[32];
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			for(int i = 0; i < 32; i++) {
				bits_b[i] = new WidgetButton(gui,format("%d",i));
				bits_b[i].setToggleable(1);
				mask_gb.addChild(bits_b[i],GUI_ALIGN_EXPAND);
				bits_b[i].setCallback(GUI_CLICKED,functionid(bit_clicked),bits_b[i]);
			}
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::Mask)
		}
		
		void shutdown() {
			
			for(int i = 0; i < 32; i++) {
				delete bits_b[i];
			}
		}
		
		/*
		 */
		void set_mask(int mask) {
			for(int i = 0; i < 32; i++) {
				if(mask & (1 << i)) {
					bits_b[i].setToggled(1);
					bits_b[i].setFontColor(vec4(0.5f,0.5f,0.5f,1.0f));
				} else {
					bits_b[i].setToggled(0);
					bits_b[i].setFontColor(vec4_one);
				}
			}
		}
		
		int get_mask() {
			int mask = 0;
			for(int i = 0; i < 32; i++) {
				if(bits_b[i].isToggled()) mask |= (1 << i);
			}
			return mask;
		}
		
		void apply_mask(int mask) {
			if(cb_instance == NULL) call(callback,mask);
			else cb_instance.call(callback,mask);
		}
		
		void bit_clicked(WidgetButton button) {
			if(button.isToggled()) button.setFontColor(vec4(0.5f,0.5f,0.5f,1.0f));
			else button.setFontColor(vec4(1.0f,1.0f,1.0f,1.0f));
			apply_mask(get_mask());
		}
		
		void set_clicked() {
			set_mask(~0);
			apply_mask(get_mask());
		}
		
		void clear_clicked() {
			set_mask(0);
			apply_mask(get_mask());
		}
		
		/*
		 */
		int run(string title,int cb,int mask,int cb_instance_ = NULL) {
			
			dialog.setText(title);
			
			set_mask(mask);
			
			cb_instance = cb_instance_;
			callback = cb;
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				apply_mask(get_mask());
				return 1;
			}
			
			apply_mask(mask);
			return 0;
		}
		
	} /* namespace Mask */
	
	/**************************************************************************\
	*
	* World dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace World {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetTreeBox worlds_tb;
		WidgetCheckBox add_cb;
		
		int states[];
		string names[];
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::World)
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update_worlds() {
			
			engine.console.run("filesystem_reload");
			
			wait; wait;
			
			engine.app.stopFps();
			
			worlds_tb.clear();
			names.clear();
			
			string worlds[0];
			getAllWorlds(worlds);
			worlds.sort();
			
			int id;
			string tokens[0];
			string directories[];
			forloop(int i = 0; worlds.size()) {
				strsplit(worlds[i],"/",tokens);
				if(tokens.size() < 2) {
					id = worlds_tb.addItem(worlds[i]);
				} else {
					string parent = "";
					string current = "";
					forloop(int j = 0; tokens.size() - 1) {
						parent = current;
						current += "/" + tokens[j];
						if(directories.check(current) == 0) {
							id = worlds_tb.addItem(tokens[j]);
							directories.append(current,id);
							if(j > 0) worlds_tb.setItemParent(id,directories[parent]);
						}
					}
					parent = current;
					current += "/" + tokens[tokens.size() - 1];
					id = worlds_tb.addItem(tokens[tokens.size() - 1]);
					worlds_tb.setItemParent(id,directories[parent]);
				}
				if(worlds[i] == engine.world.getName()) worlds_tb.setCurrentItem(id);
				names.append(id,worlds[i]);
			}
			
			int remove[0];
			forloop(int i = 0; worlds_tb.getNumItems()) {
				int id = worlds_tb.getItem(i);
				if(worlds_tb.getNumItemChilds(id) == 0) {
					int parent = worlds_tb.getItemParent(id);
					if(parent != -1 && worlds_tb.getItemText(id) == worlds_tb.getItemText(parent) && worlds_tb.getNumItemChilds(parent) == 1) {
						remove.append(id);
					}
				}
			}
			
			foreach(int id; remove) {
				names[worlds_tb.getItemParent(id)] = names[id];
				worlds_tb.removeItem(id);
				names.remove(id);
			}
			
			forloop(int i = 0; worlds_tb.getNumItems()) {
				int id = worlds_tb.getItem(i);
				string path = worlds_tb.getItemText(id);
				int parent = worlds_tb.getItemParent(id);
				while(parent != -1) {
					path = worlds_tb.getItemText(parent) + "/" + path;
					parent = worlds_tb.getItemParent(parent);
				}
				if(worlds_tb.getNumItemChilds(id)) {
					if(states.check(path)) {
						worlds_tb.setItemFolded(id,states[path]);
					} else {
						states.append(path,1);
						worlds_tb.setItemFolded(id,1);
					}
				}
			}
			
			if(engine.world.isLoaded()) {
				add_cb.setEnabled(1);
			} else {
				add_cb.setEnabled(0);
				add_cb.setChecked(0);
			}
			
			engine.app.startFps();
		}
		
		/*
		 */
		void worlds_changed() {
			int id = worlds_tb.getCurrentItem();
			WidgetButton button = dialog.getOkButton();
			button.setEnabled(names.check(id));
		}
		
		void worlds_double_clicked() {
			int id = worlds_tb.getCurrentItem();
			if(names.check(id)) {
				WidgetButton button = dialog.getOkButton();
				button.runCallback(GUI_CLICKED);
			} else if(id != -1) {
				worlds_tb.setItemFolded(id,!worlds_tb.isItemFolded(id));
			}
		}
		
		/*
		 */
		int run(string title,string &name,int &add) {
			
			dialog.setText(title);
			
			update_worlds();
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			forloop(int i = 0; worlds_tb.getNumItems()) {
				int id = worlds_tb.getItem(i);
				string path = worlds_tb.getItemText(id);
				int parent = worlds_tb.getItemParent(id);
				while(parent != -1) {
					path = worlds_tb.getItemText(parent) + "/" + path;
					parent = worlds_tb.getItemParent(parent);
				}
				states.append(path,worlds_tb.isItemFolded(id));
			}
			
			if(dialog.isOkClicked()) {
				name = names[worlds_tb.getCurrentItem()];
				add = add_cb.isChecked();
				return 1;
			}
			
			return 0;
		}
		
	} /* namespace World */
	
	/**************************************************************************\
	*
	* Material dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Material {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetVBox material_vb;
		WidgetSpriteMaterialView material_smv;
		
		WidgetComboBox libraries_cb;
		WidgetEditLine name_el;
		WidgetTreeBox materials_tb;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			// material view
			material_smv = new WidgetSpriteMaterialView(gui);
			material_vb.addChild(material_smv.getWidget());
			thread(functionid(update));
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::Material)
		}
		
		void shutdown() {
			
			delete material_smv;
			material_smv = NULL;
		}
		
		/*
		 */
		void update() {
			while(material_smv != NULL) {
				material_smv.update();
				wait;
			}
		}
		
		void update_libraries() {
			
			string library = libraries_cb.getCurrentItemText();
			
			libraries_cb.clear();
			libraries_cb.addItem(TR("All libraries"));
			forloop(int i = 0; engine.materials.getNumLibraries()) {
				libraries_cb.addItem(replace(basename(engine.materials.getLibraryName(i)),".mat",""));
			}
			
			forloop(int i = 0; libraries_cb.getNumItems()) {
				if(libraries_cb.getItemText(i) == library) {
					libraries_cb.setCurrentItem(i);
					break;
				}
			}
		}
		
		void update_materials() {
			
			string materials[0];
			if(libraries_cb.getCurrentItem() == 0) {
				forloop(int i = 0; engine.materials.getNumLibraries()) {
					forloop(int j = 0; engine.materials.getNumMaterials(i)) {
						string name = engine.materials.getMaterialName(i,j);
						Material material = engine.materials.findMaterial(name);
						if(material.isHidden() == 0) materials.append(name);
					}
				}
			} else {
				int library = libraries_cb.getCurrentItem() - 1;
				forloop(int i = 0; engine.materials.getNumMaterials(library)) {
					string name = engine.materials.getMaterialName(library,i);
					Material material = engine.materials.findMaterial(name);
					if(material.isHidden() == 0) materials.append(name);
				}
			}
			
			materials.sort();
			
			string items[];
			materials_tb.setCallbackEnabled(GUI_CHANGED,0);
			materials_tb.clear();
			foreach(string name; materials) {
				items.append(name,materials_tb.addItem(name));
			}
			materials_tb.setCallbackEnabled(GUI_CHANGED,1);
			
			foreach(string name; materials) {
				Material material = engine.materials.findMaterial(name);
				Material parent = material.getParent();
				if(parent == NULL) materials_tb.setItemColor(items[name],color_green);
				else if(material.getNumChilds()) materials_tb.setItemColor(items[name],color_orange);
				if(parent == NULL || materials.find(parent.getName()) == -1) continue;
				materials_tb.setItemParent(items[name],items[parent.getName()]);
			}
		}
		
		/*
		 */
		void libraries_changed() {
			materials_tb.setCallbackEnabled(GUI_CHANGED,0);
			update_materials();
			materials_tb.setCallbackEnabled(GUI_CHANGED,1);
			name_el.runCallback(GUI_CHANGED);
		}
		
		void name_changed() {
			name_el.setCallbackEnabled(GUI_CHANGED,0);
			materials_tb.setCallbackEnabled(GUI_CHANGED,0);
			selectTreeBoxByName(materials_tb,name_el,0);
			name_el.setCallbackEnabled(GUI_CHANGED,1);
			materials_tb.setCallbackEnabled(GUI_CHANGED,1);
			material_smv.setMaterial(materials_tb.getCurrentItemText());
		}
		
		void name_pressed() {
			name_el.setCallbackEnabled(GUI_CHANGED,0);
			materials_tb.setCallbackEnabled(GUI_CHANGED,0);
			selectTreeBoxByName(materials_tb,name_el,1);
			name_el.setCallbackEnabled(GUI_CHANGED,1);
			materials_tb.setCallbackEnabled(GUI_CHANGED,1);
			material_smv.setMaterial(materials_tb.getCurrentItemText());
		}
		
		void materials_changed() {
			name_el.setCallbackEnabled(GUI_CHANGED,0);
			name_el.setText(materials_tb.getCurrentItemText());
			name_el.setCallbackEnabled(GUI_CHANGED,1);
			material_smv.setMaterial(materials_tb.getCurrentItemText());
		}
		
		/*
		 */
		void ok_clicked() {
			WidgetButton button = dialog.getOkButton();
			button.runCallback(GUI_CLICKED);
		}
		
		/*
		 */
		int run(string title,string &name) {
			
			dialog.setText(title);
			
			// libraries
			libraries_cb.setCallbackEnabled(GUI_CHANGED,0);
			update_libraries();
			libraries_cb.setCallbackEnabled(GUI_CHANGED,1);
			
			// materials
			materials_tb.setCallbackEnabled(GUI_CHANGED,0);
			update_materials();
			materials_tb.setCallbackEnabled(GUI_CHANGED,1);
			
			// current material
			name_el.setText(name);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				name = materials_tb.getCurrentItemText();
				return 1;
			}
			
			return 0;
		}
		
	} /* namespace Material */
	
	/**************************************************************************\
	*
	* MaterialView dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace MaterialView {
		
		/*
		 */
		WidgetSpriteMaterialView material_smv;
		WidgetDialog dialog;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			dialog = new WidgetDialog(gui);
			
			material_smv = new WidgetSpriteMaterialView(gui);
			dialog.addChild(material_smv.getWidget(),GUI_ALIGN_EXPAND);
			thread(functionid(update));
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::MaterialView)
		}
		
		void shutdown() {
			
			delete material_smv;
			material_smv = NULL;
		}
		
		/*
		 */
		void update() {
			while(material_smv != NULL) {
				material_smv.update();
				wait;
			}
		}
		
		/*
		 */
		int run(string title,string name) {
			
			dialog.setText(title);
			material_smv.setMaterial(name);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			return dialog.isOkClicked();
		}
		
	} /* namespace MaterialView */
	
	/**************************************************************************\
	*
	* MaterialName dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace MaterialName {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetComboBox libraries_cb;
		WidgetEditLine name_el;
		
		string libraries[0];
		string input_name;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::MaterialName)
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void name_changed() {
			string name = name_el.getText();
			WidgetButton button = dialog.getOkButton();
			button.setEnabled(libraries.size() && strlen(name) > 2 && (engine.materials.isMaterial(name) == 0 || input_name == name));
		}
		
		void name_pressed() {
			string name = name_el.getText();
			if((engine.materials.isMaterial(name) == 0 || input_name == name) && strlen(name) > 2) {
				WidgetButton button = dialog.getOkButton();
				button.runCallback(GUI_CLICKED);
			}
		}
		
		/*
		 */
		int run(string title,string &library,string &name) {
			
			dialog.setText(title);
			
			// libraries
			libraries.clear();
			libraries_cb.clear();
			forloop(int i = 0,j = 0; engine.materials.getNumLibraries()) {
				if(engine.materials.isLibraryEditable(i)) {
					libraries.append(engine.materials.getLibraryName(i));
					libraries_cb.addItem(replace(basename(engine.materials.getLibraryName(i)),".mat",""));
					if(engine.materials.getLibraryName(i) == library) libraries_cb.setCurrentItem(j);
					j++;
				}
			}
			WidgetButton button = dialog.getOkButton();
			button.setEnabled(libraries.size());
			
			// material name
			input_name = name;
			name_el.setText(name);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				library = libraries[libraries_cb.getCurrentItem()];
				name = name_el.getText();
				return 1;
			}
			
			return 0;
		}
		
	} /* namespace MaterialName */
	
	/**************************************************************************\
	*
	* MaterialLibraries dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace MaterialLibraries {
		
		/*
		 */
		WidgetDialog dialog;
		
		string libraries[0];
		WidgetListBox libraries_lb;
		
		WidgetIcon up_i;
		WidgetIcon down_i;
		WidgetIcon save_i;
		WidgetIcon remove_i;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::MaterialLibraries)
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update_libraries() {
			libraries.clear();
			libraries_lb.clear();
			forloop(int i = 0; engine.materials.getNumWorldLibraries()) {
				libraries.append(engine.materials.getWorldLibraryName(i));
				libraries_lb.addItem(engine.materials.getWorldLibraryName(i));
			}
		}
		
		void libraries_changed() {
			int item = libraries_lb.getCurrentItem();
			up_i.setEnabled(item > 0);
			down_i.setEnabled(item >= 0 && item < libraries.size() - 1);
			save_i.setEnabled(item >= 0 && item < libraries.size());
			remove_i.setEnabled(item >= 0 && item < libraries.size());
		}
		
		void up_clicked() {
			
			int item = libraries_lb.getCurrentItem();
			string library = libraries[item];
			libraries[item] = libraries[item - 1];
			libraries[item - 1] = library;
			foreach(string name; libraries) {
				engine.materials.removeWorldLibrary(name);
			}
			foreach(string name; libraries) {
				engine.materials.addWorldLibrary(name);
			}
			
			update_libraries();
			libraries_lb.setCurrentItem(item - 1);
			libraries_changed();
		}
		
		void down_clicked() {
			
			int item = libraries_lb.getCurrentItem();
			string library = libraries[item];
			libraries[item] = libraries[item + 1];
			libraries[item + 1] = library;
			foreach(string name; libraries) {
				engine.materials.removeWorldLibrary(name);
			}
			foreach(string name; libraries) {
				engine.materials.addWorldLibrary(name);
			}
			
			update_libraries();
			libraries_lb.setCurrentItem(item + 1);
			libraries_changed();
		}
		
		void add_clicked() {
			string name;
			if(dialogFileMaterial(TR("Select material library"),name)) {
				engine.materials.addWorldLibrary(name);
				update_libraries();
				libraries_changed();
			}
		}
		
		void remove_clicked() {
			if(dialogMessageYesNo(TR("Confirm"),format(TR("Remove \"%s\" material library from the world?"),libraries_lb.getCurrentItemText()))) {
				
				string lib_name = engine.materials.getWorldLibraryName(libraries_lb.getCurrentItem());
				
				if(engine.materials.removeWorldLibrary(lib_name)) {
					engine.materials.clear(lib_name);
				}
				
				update_libraries();
				libraries_changed();
			}
		}
		
		void save_clicked() {
			int item = libraries_lb.getCurrentItem();
			if(engine.materials.save(libraries[item]) == 0) {
				dialogMessageOk(TR("Error"));
			}
		}
		
		/*
		 */
		int run(string title) {
			
			dialog.setText(title);
			
			update_libraries();
			libraries_changed();
			
			// save libraries
			string old_libraries[0];
			forloop(int i = 0; engine.materials.getNumWorldLibraries()) {
				old_libraries.append(engine.materials.getWorldLibraryName(i));
			}
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				return 1;
			}
			
			// restore libraries
			foreach(string name; libraries) {
				engine.materials.removeWorldLibrary(name);
			}
			foreach(string name; old_libraries) {
				engine.materials.addWorldLibrary(name);
			}
			
			// skipped libraries
			string names = "";
			foreach(string name; libraries) {
				if(old_libraries.find(name) == -1) {
					names += "\n" + name;
				}
			}
			if(names != "") {
				dialogMessageOk(TR("Warning"),format(TR("Material libraries are not appended into the world%s"),names));
			}
			
			return 0;
		}
		
	} /* namespace MaterialLibraries */
	
	/**************************************************************************\
	*
	* Property dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Property {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetComboBox libraries_cb;
		WidgetEditLine name_el;
		WidgetTreeBox properties_tb;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::Property)
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update_libraries() {
			
			string library = libraries_cb.getCurrentItemText();
			
			libraries_cb.clear();
			libraries_cb.addItem(TR("All libraries"));
			forloop(int i = 0; engine.properties.getNumLibraries()) {
				libraries_cb.addItem(replace(basename(engine.properties.getLibraryName(i)),".prop",""));
			}
			
			forloop(int i = 0; libraries_cb.getNumItems()) {
				if(libraries_cb.getItemText(i) == library) {
					libraries_cb.setCurrentItem(i);
					break;
				}
			}
		}
		
		void update_properties() {
			
			string properties[0];
			if(libraries_cb.getCurrentItem() == 0) {
				forloop(int i = 0; engine.properties.getNumLibraries()) {
					forloop(int j = 0; engine.properties.getNumProperties(i)) {
						string name = engine.properties.getPropertyName(i,j);
						Property property = engine.properties.findProperty(name);
						if(property.isHidden() == 0) properties.append(name);
					}
				}
			} else {
				int library = libraries_cb.getCurrentItem() - 1;
				forloop(int i = 0; engine.properties.getNumProperties(library)) {
					string name = engine.properties.getPropertyName(library,i);
					Property property = engine.properties.findProperty(name);
					if(property.isHidden() == 0) properties.append(name);
				}
			}
			
			properties.sort();
			
			string items[];
			properties_tb.setCallbackEnabled(GUI_CHANGED,0);
			properties_tb.clear();
			foreach(string name; properties) {
				items.append(name,properties_tb.addItem(name));
			}
			properties_tb.setCallbackEnabled(GUI_CHANGED,1);
			
			foreach(string name; properties) {
				Property property = engine.properties.findProperty(name);
				Property parent = property.getParent();
				if(parent == NULL) properties_tb.setItemColor(items[name],color_green);
				else if(property.getNumChilds()) properties_tb.setItemColor(items[name],color_orange);
				if(parent == NULL || properties.find(parent.getName()) == -1) continue;
				properties_tb.setItemParent(items[name],items[parent.getName()]);
			}
		}
		
		/*
		 */
		void libraries_changed() {
			properties_tb.setCallbackEnabled(GUI_CHANGED,0);
			update_properties();
			properties_tb.setCallbackEnabled(GUI_CHANGED,1);
			name_el.runCallback(GUI_CHANGED);
		}
		
		void name_changed() {
			name_el.setCallbackEnabled(GUI_CHANGED,0);
			properties_tb.setCallbackEnabled(GUI_CHANGED,0);
			selectTreeBoxByName(properties_tb,name_el,0);
			name_el.setCallbackEnabled(GUI_CHANGED,1);
			properties_tb.setCallbackEnabled(GUI_CHANGED,1);
		}
		
		void name_pressed() {
			name_el.setCallbackEnabled(GUI_CHANGED,0);
			properties_tb.setCallbackEnabled(GUI_CHANGED,0);
			selectTreeBoxByName(properties_tb,name_el,1);
			name_el.setCallbackEnabled(GUI_CHANGED,1);
			properties_tb.setCallbackEnabled(GUI_CHANGED,1);
		}
		
		void properties_changed() {
			name_el.setCallbackEnabled(GUI_CHANGED,0);
			name_el.setText(properties_tb.getCurrentItemText());
			name_el.setCallbackEnabled(GUI_CHANGED,1);
		}
		
		/*
		 */
		void ok_clicked() {
			WidgetButton button = dialog.getOkButton();
			button.runCallback(GUI_CLICKED);
		}
		
		/*
		 */
		int run(string title,string &name) {
			
			dialog.setText(title);
			
			// libraries
			libraries_cb.setCallbackEnabled(GUI_CHANGED,0);
			update_libraries();
			libraries_cb.setCallbackEnabled(GUI_CHANGED,1);
			
			// properties
			properties_tb.setCallbackEnabled(GUI_CHANGED,0);
			update_properties();
			properties_tb.setCallbackEnabled(GUI_CHANGED,1);
			
			// current property
			name_el.setText(name);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				name = properties_tb.getCurrentItemText();
				return 1;
			}
			
			return 0;
		}
		
	} /* namespace Property */
	
	/**************************************************************************\
	*
	* PropertyName dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace PropertyName {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetComboBox libraries_cb;
		WidgetEditLine name_el;
		
		string libraries[0];
		string input_name;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::PropertyName)
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void name_changed() {
			string name = name_el.getText();
			WidgetButton button = dialog.getOkButton();
			button.setEnabled(libraries.size() && strlen(name) > 2 && (engine.properties.isProperty(name) == 0 || input_name == name));
		}
		
		void name_pressed() {
			string name = name_el.getText();
			if((engine.properties.isProperty(name) == 0 || input_name == name) && strlen(name) > 2) {
				WidgetButton button = dialog.getOkButton();
				button.runCallback(GUI_CLICKED);
			}
		}
		
		/*
		 */
		int run(string title,string &library,string &name) {
			
			dialog.setText(title);
			
			// libraries
			libraries.clear();
			libraries_cb.clear();
			forloop(int i = 0,j = 0; engine.properties.getNumLibraries()) {
				if(engine.properties.isLibraryEditable(i)) {
					libraries.append(engine.properties.getLibraryName(i));
					libraries_cb.addItem(replace(basename(engine.properties.getLibraryName(i)),".prop",""));
					if(engine.properties.getLibraryName(i) == library) libraries_cb.setCurrentItem(j);
					j++;
				}
			}
			WidgetButton button = dialog.getOkButton();
			button.setEnabled(libraries.size());
			
			// property name
			input_name = name;
			name_el.setText(name);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				library = libraries[libraries_cb.getCurrentItem()];
				name = name_el.getText();
				return 1;
			}
			
			return 0;
		}
		
	} /* namespace PropertyName */
	
	/**************************************************************************\
	*
	* PropertyLibraries dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace PropertyLibraries {
		
		/*
		 */
		WidgetDialog dialog;
		
		string libraries[0];
		WidgetListBox libraries_lb;
		
		WidgetIcon up_i;
		WidgetIcon down_i;
		WidgetIcon save_i;
		WidgetIcon remove_i;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::PropertyLibraries)
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update_libraries() {
			libraries.clear();
			libraries_lb.clear();
			forloop(int i = 0; engine.properties.getNumWorldLibraries()) {
				libraries.append(engine.properties.getWorldLibraryName(i));
				libraries_lb.addItem(engine.properties.getWorldLibraryName(i));
			}
		}
		
		void libraries_changed() {
			int item = libraries_lb.getCurrentItem();
			up_i.setEnabled(item > 0);
			down_i.setEnabled(item >= 0 && item < libraries.size() - 1);
			save_i.setEnabled(item >= 0 && item < libraries.size());
			remove_i.setEnabled(item >= 0 && item < libraries.size());
		}
		
		void up_clicked() {
			
			int item = libraries_lb.getCurrentItem();
			string library = libraries[item];
			libraries[item] = libraries[item - 1];
			libraries[item - 1] = library;
			foreach(string name; libraries) {
				engine.properties.removeWorldLibrary(name);
			}
			foreach(string name; libraries) {
				engine.properties.addWorldLibrary(name);
			}
			
			update_libraries();
			libraries_lb.setCurrentItem(item - 1);
			libraries_changed();
		}
		
		void down_clicked() {
			
			int item = libraries_lb.getCurrentItem();
			string library = libraries[item];
			libraries[item] = libraries[item + 1];
			libraries[item + 1] = library;
			foreach(string name; libraries) {
				engine.properties.removeWorldLibrary(name);
			}
			foreach(string name; libraries) {
				engine.properties.addWorldLibrary(name);
			}
			
			update_libraries();
			libraries_lb.setCurrentItem(item + 1);
			libraries_changed();
		}
		
		void add_clicked() {
			string name;
			if(dialogFileProperty(TR("Select property library"),name)) {
				engine.properties.addWorldLibrary(name);
				update_libraries();
				libraries_changed();
			}
		}
		
		void remove_clicked() {
			if(dialogMessageYesNo(TR("Confirm"),format(TR("Remove \"%s\" property library from the world?"),libraries_lb.getCurrentItemText()))) {
				engine.properties.removeWorldLibrary(engine.properties.getWorldLibraryName(libraries_lb.getCurrentItem()));
				update_libraries();
				libraries_changed();
			}
		}
		
		void save_clicked() {
			int item = libraries_lb.getCurrentItem();
			if(engine.properties.save(libraries[item]) == 0) {
				dialogMessageOk(TR("Error"));
			}
		}
		
		/*
		 */
		int run(string title) {
			
			dialog.setText(title);
			
			update_libraries();
			libraries_changed();
			
			// save libraries
			string old_libraries[0];
			forloop(int i = 0; engine.properties.getNumWorldLibraries()) {
				old_libraries.append(engine.properties.getWorldLibraryName(i));
			}
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				return 1;
			}
			
			// restore libraries
			foreach(string name; libraries) {
				engine.properties.removeWorldLibrary(name);
			}
			foreach(string name; old_libraries) {
				engine.properties.addWorldLibrary(name);
			}
			
			// skipped libraries
			string names = "";
			foreach(string name; libraries) {
				if(old_libraries.find(name) == -1) {
					names += "\n" + name;
				}
			}
			if(names != "") {
				dialogMessageOk(TR("Warning"),format(TR("Property libraries are not appened into the world%s"),names));
			}
			
			return 0;
		}
		
	} /* namespace PropertyLibraries */
	
	/**************************************************************************\
	*
	* NodeView dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace NodeView {
		
		/*
		 */
		WidgetSpriteNodeView node_snv;
		WidgetDialog dialog;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			dialog = new WidgetDialog(gui);
			
			node_snv = new WidgetSpriteNodeView(gui);
			dialog.addChild(node_snv.getWidget(),GUI_ALIGN_EXPAND);
			thread(functionid(update));
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::NodeView)
		}
		
		void shutdown() {
			
			delete node_snv;
			node_snv = NULL;
		}
		
		/*
		 */
		void update() {
			while(node_snv != NULL) {
				node_snv.update();
				wait;
			}
		}
		
		/*
		 */
		int run(string title,Node node) {
			
			dialog.setText(title);
			node_snv.setNode(node);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			return dialog.isOkClicked();
		}
		
	} /* namespace NodeView */
	
	/**************************************************************************\
	*
	* Body dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Body {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetTreeBox bodies_tb;
		Body bodies[];
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::Body)
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update_bodies(Body body_0,Body body,int parent) {
			
			if(body == NULL) return;
			if(body == body_0) return;
			
			int item = -1;
			Object object = body.getObject();
			if(object == NULL || object.getName() == "") {
				if(body.getName() == "") item = bodies_tb.addItem(body.getTypeName());
				else item = bodies_tb.addItem(body.getTypeName() + " - " + body.getName());
			} else {
				if(body.getName() == "") item = bodies_tb.addItem(object.getName() + " - " + body.getTypeName());
				else item = bodies_tb.addItem(object.getName() + " - " + body.getTypeName() + " - " + body.getName());
			}
			bodies.append(item,body);
			
			bodies_tb.setItemParent(item,parent);
			
			forloop(int i = 0; body.getNumChilds()) {
				update_bodies(body_0,body.getChild(i),item);
			}
		}
		
		/*
		 */
		int run(string title,Body body_0,Body &body) {
			
			dialog.setText(title);
			
			bodies_tb.clear();
			bodies.clear();
			
			forloop(int i = 0; engine.editor.getNumNodes()) {
				Node node = engine.editor.getNode(i);
				if(node.isObject() == 0) continue;
				Object object = node_cast(node);
				update_bodies(body_0,object.getBody(),-1);
			}
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) {
				if(bodies_tb.getCurrentItem() != -1) {
					Body body = bodies[bodies_tb.getCurrentItem()];
					engine.visualizer.renderObjectSurface(body.getObject(),-1,engine.render.getWireframeColor());
					body.renderVisualizer();
				}
				if(engine.app.getMouseButton() & APP_BUTTON_LEFT) {
					if(dialog.isChild(gui.getFocus()) == 0) {
						vec3 p0,p1;
						Unigine::getPlayerMouseDirection(p0,p1);
						WorldIntersection intersection = new WorldIntersection();
						Object object = engine.world.getIntersection(p0,p1,~0,intersection);
						if(object != NULL) {
							int item = bodies.find(object.getBody());
							if(item != -1) {
								bodies_tb.setCurrentItem(item);
								bodies_tb.showItem(item);
							} else {
								bodies_tb.setCurrentItem(-1);
							}
						} else {
							bodies_tb.setCurrentItem(-1);
						}
					}
				}
				wait 1;
			}
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				if(bodies_tb.getCurrentItem() != -1) {
					body = bodies[bodies_tb.getCurrentItem()];
					return 1;
				}
			}
			
			return 0;
		}
		
	} /* namespace Body */
	
	/**************************************************************************\
	*
	* RagDoll dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace RagDoll {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetEditLine mass_el;
		WidgetEditLine error_el;
		WidgetEditLine threshold_el;
		WidgetCheckBox capsule_cb;
		
		float mass = 1.0f;
		float error = 0.01f;
		float threshold = 0.2f;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			mass = configGet("dialog_ragdoll_mass",mass);
			error = configGet("dialog_ragdoll_error",error);
			threshold = configGet("dialog_ragdoll_threshold",threshold);
			capsule_cb.setChecked(configGet("dialog_ragdoll_capsule",capsule_cb.isChecked()));
			
			mass_el.setText(editorFormat(mass));
			error_el.setText(editorFormat(error));
			threshold_el.setText(editorFormat(threshold));
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::RagDoll)
		}
		
		void shutdown() {
			
			configSet("dialog_ragdoll_mass",mass);
			configSet("dialog_ragdoll_error",error);
			configSet("dialog_ragdoll_threshold",threshold);
			configSet("dialog_ragdoll_capsule",capsule_cb.isChecked());
		}
		
		/*
		 */
		void mass_pressed() {
			mass = max(float(mass_el.getText()),0.0f);
			mass_el.setText(editorFormat(mass));
		}
		
		void error_pressed() {
			error = saturate(float(error_el.getText()));
			error_el.setText(editorFormat(error));
		}
		
		void threshold_pressed() {
			threshold = saturate(float(threshold_el.getText()));
			threshold_el.setText(editorFormat(threshold));
		}
		
		/*
		 */
		int run(string title,float &m,float &e,float &t,int &c) {
			
			dialog.setText(title);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				m = mass;
				e = error;
				t = threshold;
				c = capsule_cb.isChecked();
				return 1;
			}
			
			return 0;
		}
		
	} /* namespace RagDoll */
	
	/**************************************************************************\
	*
	* Convex dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Convex {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetEditLine error_el;
		
		float error = 0.01f;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			error = configGet("dialog_convex_error",error);
			
			error_el.setText(editorFormat(error));
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::Convex)
		}
		
		void shutdown() {
			
			configSet("dialog_convex_error",error);
		}
		
		/*
		 */
		void error_pressed() {
			error = saturate(float(error_el.getText()));
			error_el.setText(editorFormat(error));
		}
		
		/*
		 */
		int run(string title,float &e) {
			
			dialog.setText(title);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				e = error;
				return 1;
			}
			
			return 0;
		}
		
	} /* namespace Convex */
	
	/**************************************************************************\
	*
	* Decompose dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Decompose {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetEditLine depth_el;
		WidgetEditLine error_el;
		WidgetEditLine threshold_el;
		
		int depth = 4;
		float error = 0.01f;
		float threshold = 0.01f;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			depth = configGet("dialog_decompose_depth",depth);
			error = configGet("dialog_decompose_error",error);
			threshold = configGet("dialog_decompose_threshold",threshold);
			
			depth_el.setText(string(depth));
			error_el.setText(editorFormat(error));
			threshold_el.setText(editorFormat(threshold));
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::Decompose)
		}
		
		void shutdown() {
			
			configSet("dialog_decompose_depth",depth);
			configSet("dialog_decompose_error",error);
			configSet("dialog_decompose_threshold",threshold);
		}
		
		/*
		 */
		void depth_pressed() {
			depth = clamp(int(depth_el.getText()),0,16);
			depth_el.setText(string(depth));
		}
		
		void error_pressed() {
			error = saturate(float(error_el.getText()));
			error_el.setText(editorFormat(error));
		}
		
		void threshold_pressed() {
			threshold = saturate(float(threshold_el.getText()));
			threshold_el.setText(editorFormat(threshold));
		}
		
		/*
		 */
		int run(string title,int &d,float &e,float &t) {
			
			dialog.setText(title);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				d = depth;
				e = error;
				t = threshold;
				return 1;
			}
			
			return 0;
		}
		
	} /* namespace Decompose */
	
	/**************************************************************************\
	*
	* ImageSize dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace ImageSize {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetComboBox size_cb;
		WidgetLabel format_l;
		WidgetComboBox format_cb;
		WidgetCheckBox flip_cb;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			size_cb.setCurrentItem(4);
			
			size_cb.setCurrentItem(configGet("dialog_image_size_size",size_cb.getCurrentItem()));
			size_cb.setCurrentItem(configGet("dialog_image_size_format",format_cb.getCurrentItem()));
			flip_cb.setChecked(configGet("dialog_image_size_flip",0));
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::ImageSize)
		}
		
		void shutdown() {
			
			configSet("dialog_image_size_size",size_cb.getCurrentItem());
			configSet("dialog_image_size_format",format_cb.getCurrentItem());
			configSet("dialog_image_size_flip",flip_cb.isChecked());
		}
		
		/*
		 */
		int run(string title,int &size) {
			
			dialog.setText(title);
			
			format_l.setHidden(1);
			format_cb.setHidden(1);
			flip_cb.setHidden(1);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				size = int(size_cb.getCurrentItemText());
				return 1;
			}
			
			return 0;
		}
		
		/*
		 */
		int runFlip(string title,int &size,int &flip) {
			
			dialog.setText(title);
			
			format_l.setHidden(1);
			format_cb.setHidden(1);
			flip_cb.setHidden(0);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				size = int(size_cb.getCurrentItemText());
				flip = flip_cb.isChecked();
				return 1;
			}
			
			return 0;
		}
		
		/*
		 */
		int runFormat(string title,int &size,int &format) {
			
			dialog.setText(title);
			
			format_l.setHidden(0);
			format_cb.setHidden(0);
			flip_cb.setHidden(1);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				size = int(size_cb.getCurrentItemText());
				string format_name = format_cb.getCurrentItemText();
				if(format_name == "R8") format = IMAGE_FORMAT_R8;
				else if(format_name == "RG8") format = IMAGE_FORMAT_RG8;
				else if(format_name == "RGB8") format = IMAGE_FORMAT_RGB8;
				else if(format_name == "RGBA8") format = IMAGE_FORMAT_RGBA8;
				else format = IMAGE_FORMAT_RGBA8;
				return 1;
			}
			
			return 0;
		}
		
	} /* namespace ImageSize */
	
	/**************************************************************************\
	*
	* TerrainImport dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace TerrainImport {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetEditLine offset_x_el;
		WidgetEditLine offset_y_el;
		
		WidgetSpinBox offset_x_sb;
		WidgetSpinBox offset_y_sb;
		
		WidgetEditLine scale_el;
		
		WidgetCheckBox flip_cb;
		
		int offset_x = 0;
		int offset_y = 0;
		
		float scale = 1.0f;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			offset_x = configGet("dialog_terrain_import_offset_x",offset_x);
			offset_y = configGet("dialog_terrain_import_offset_y",offset_y);
			scale = configGet("dialog_terrain_import_scale",scale);
			flip_cb.setChecked(configGet("dialog_terrain_import_flip",flip_cb.isChecked()));
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::TerrainImport)
		}
		
		void shutdown() {
			
			configSet("dialog_terrain_import_offset_x",offset_x);
			configSet("dialog_terrain_import_offset_y",offset_y);
			configSet("dialog_terrain_import_scale",scale);
			configSet("dialog_terrain_import_flip",flip_cb.isChecked());
		}
		
		/*
		 */
		void offset_x_pressed() {
			offset_x = int(offset_x_el.getText());
		}
		
		void offset_y_pressed() {
			offset_y = int(offset_y_el.getText());
		}
		
		void scale_pressed() {
			scale = max(float(scale_el.getText()),1.0f);
		}
		
		/*
		 */
		int run(string title,ObjectTerrain terrain,int &ox,int &oy,float &s,int &flip) {
			
			dialog.setText(title);
			
			offset_x_sb.setMaxValue(terrain.getSurfacesX() - 1);
			offset_y_sb.setMaxValue(terrain.getSurfacesY() - 1);
			
			offset_x_el.setText(string(offset_x));
			offset_y_el.setText(string(offset_y));
			
			scale_el.setText(editorFormat(scale));
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				ox = offset_x;
				oy = offset_y;
				s = scale;
				flip = flip_cb.isChecked();
				return 1;
			}
			
			return 0;
		}
		
	} /* namespace TerrainImport */
	
	/**************************************************************************\
	*
	* TerrainExport dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace TerrainExport {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetEditLine offset_x_el;
		WidgetEditLine offset_y_el;
		WidgetEditLine size_x_el;
		WidgetEditLine size_y_el;
		
		WidgetSpinBox offset_x_sb;
		WidgetSpinBox offset_y_sb;
		WidgetSpinBox size_x_sb;
		WidgetSpinBox size_y_sb;
		
		WidgetEditLine scale_el;
		
		int surfaces_x;
		int surfaces_y;
		
		int offset_x = 0;
		int offset_y = 0;
		int size_x = 256;
		int size_y = 256;
		
		float scale = 1.0f;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			offset_x = configGet("dialog_terrain_export_offset_x",offset_x);
			offset_y = configGet("dialog_terrain_export_offset_y",offset_y);
			size_x = configGet("dialog_terrain_export_size_x",size_x);
			size_y = configGet("dialog_terrain_export_size_y",size_y);
			scale = configGet("dialog_terrain_export_scale",scale);
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::TerrainExport)
		}
		
		void shutdown() {
			
			configSet("dialog_terrain_export_offset_x",offset_x);
			configSet("dialog_terrain_export_offset_y",offset_y);
			configSet("dialog_terrain_export_size_x",size_x);
			configSet("dialog_terrain_export_size_y",size_y);
			configSet("dialog_terrain_export_scale",scale);
		}
		
		/*
		 */
		void offset_x_pressed() {
			offset_x = int(offset_x_el.getText());
			size_x_sb.setMaxValue(surfaces_x - offset_x);
			size_x_el.setText(string(size_x));
		}
		
		void offset_y_pressed() {
			offset_y = int(offset_y_el.getText());
			size_y_sb.setMaxValue(surfaces_y - offset_y);
			size_y_el.setText(string(size_y));
		}
		
		void size_x_pressed() {
			size_x = int(size_x_el.getText());
		}
		
		void size_y_pressed() {
			size_y = int(size_y_el.getText());
		}
		
		void scale_pressed() {
			scale = max(float(scale_el.getText()),1.0f);
		}
		
		/*
		 */
		int run(string title,ObjectTerrain terrain,int &ox,int &oy,int &sx,int &sy,float &s) {
			
			dialog.setText(title);
			
			surfaces_x = terrain.getSurfacesX();
			surfaces_y = terrain.getSurfacesY();
			
			offset_x_sb.setMaxValue(surfaces_x - 1);
			offset_y_sb.setMaxValue(surfaces_y - 1);
			
			offset_x_el.setText(string(offset_x));
			offset_y_el.setText(string(offset_y));
			size_x_el.setText(string(size_x));
			size_y_el.setText(string(size_y));
			
			scale_el.setText(editorFormat(scale));
			
			offset_x_pressed();
			offset_y_pressed();
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				ox = offset_x;
				oy = offset_y;
				sx = size_x;
				sy = size_y;
				s = scale;
				return 1;
			}
			
			return 0;
		}
		
	} /* namespace TerrainExport */
	
	/**************************************************************************\
	*
	* TerrainImageSize dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace TerrainImageSize {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetComboBox coarse_size_cb;
		WidgetComboBox surface_size_cb;
		
		WidgetLabel color_l;
		WidgetGroupBox color_gb;
		
		WidgetSprite color_s[2];
		vec4 color;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			coarse_size_cb.setCurrentItem(4);
			surface_size_cb.setCurrentItem(4);
			
			coarse_size_cb.setCurrentItem(configGet("dialog_terrain_image_size_coarse",coarse_size_cb.getCurrentItem()));
			surface_size_cb.setCurrentItem(configGet("dialog_terrain_image_size_surface",surface_size_cb.getCurrentItem()));
			color = configGet("dialog_terrain_image_size_color",vec4_one);
			
			setSpriteColor(color_s,color);
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::TerrainImageSize)
		}
		
		void shutdown() {
			
			configSet("dialog_terrain_image_size_coarse",coarse_size_cb.getCurrentItem());
			configSet("dialog_terrain_image_size_surface",surface_size_cb.getCurrentItem());
			configSet("dialog_terrain_image_size_color",color);
		}
		
		/*
		 */
		void color_clicked() {
			dialogColor(TR("Select color"),functionid(color_changed),color);
		}
		
		/*
		 */
		void color_changed(WidgetDialogColor dialog) {
			color = dialog.getColor();
			setSpriteColor(color_s,color);
		}
		
		/*
		 */
		int run(string title,int &coarse_size,int &surface_size) {
			
			dialog.setText(title);
			
			color_gb.setHidden(1);
			color_l.setHidden(1);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				coarse_size = int(coarse_size_cb.getCurrentItemText());
				surface_size = int(surface_size_cb.getCurrentItemText());
				return 1;
			}
			
			return 0;
		}
		
		int run(string title,int &coarse_size,int &surface_size,vec4 &c) {
			
			dialog.setText(title);
			
			color_gb.setHidden(0);
			color_l.setHidden(0);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				coarse_size = int(coarse_size_cb.getCurrentItemText());
				surface_size = int(surface_size_cb.getCurrentItemText());
				c = color;
				return 1;
			}
			
			return 0;
		}
		
	} /* namespace TerrainImageSize */
	
	/**************************************************************************\
	*
	* CorePlugin dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace CorePlugin {
		
		/*
		 */
		WidgetDialog dialog;
		
		WidgetListBox plugins_lb;
		
		DECLARE_DIALOG
		
		/*
		 */
		void init() {
			
			editorDialog(dialog);
			
			DECLARE_DIALOG_INIT(Editor::Dialogs::CorePlugin)
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void plugins_double_clicked() {
			WidgetButton button = dialog.getOkButton();
			button.runCallback(GUI_CLICKED);
		}
		
		/*
		 */
		int run(string title,string &name) {
			
			dialog.setText(title);
			
			string names[0];
			plugins_lb.clear();
			string path = "editor/plugins/";
			forloop(int i = 0; engine.filesystem.getNumPackages()) {
				engine.filesystem.getPackageFileNames(i,names);
				foreach(string name; names) {
					if(strstr(name,path) != 0) continue;
					if(re_match(name,".+\\.cpp$") == 0) continue;
					plugins_lb.addItem(substr(name,strlen(path)));
				}
			}
			
			path = engine.getDataPath() + path;
			void get_plugins(string name) {
				Dir dir = new Dir();
				if(dir.open(name)) {
					forloop(int i = 0; dir.getNumFiles()) {
						string name = dir.getFileName(i);
						if(strstr(name,path) != 0) continue;
						if(re_match(name,".+\\.cpp$") == 0) continue;
						plugins_lb.addItem(substr(name,strlen(path)));
					}
					forloop(int i = 0; dir.getNumDirs()) {
						string name = dir.getDirName(i);
						if(strstr(name,".svn") != -1) continue;
						get_plugins(name);
					}
					dir.close();
				}
				delete dir;
			}
			get_plugins(path);
			
			DECLARE_DIALOG_SHOW
			while(dialog.isDone() == 0) wait 1;
			DECLARE_DIALOG_HIDE
			
			if(dialog.isOkClicked()) {
				name = path + plugins_lb.getCurrentItemText();
				return 1;
			}
			
			return 0;
		}
		
	} /* namespace CorePlugin */
	
	/**************************************************************************\
	*
	* DialogFile dialog
	*
	\**************************************************************************/
	
	/*
	 */
	namespace DialogFile {
		
		/*
		 */
		using Editor::Widgets;
		EditorDialogFile dialog;
		
		/*
		 */
		void init() {
			dialog = new EditorDialogFile();
		}
		
		void shutdown() {
			delete dialog;
		}
		
		/*
		 */
		int run(string title,string filter,string &name,int mode,string config = NULL) {
			using Unigine::Widgets;
			
			string old_gui_keypress_callback = engine.gui.getKeyPressCallback();
			string old_controls_keypress_callback = engine.controls.getKeyPressCallback();
			
			engine.gui.setKeyPressCallback([] (int key) { 
				int is_key_active = (engine.console.getActivity() || gui.getKeyActivity(key));
				if(is_key_active) return 0;
				
				return dialog.isFocused();
			});
			engine.controls.setKeyPressCallback([] (int key) { return 1; });
			
			int controls_enabled = controlsIsEnabled();
			controlsSetEnabled(0);
			
			dialog.setFilter(filter);
			dialog.setMode(mode);
			dialog.setBookmarks(configGet("dialog_file_bookmarks",""));
			dialog.setWidth(configGet("dialog_file_width",640));
			dialog.setHeight(configGet("dialog_file_height",480));
			dialog.setPanedLeftValue(configGet("dialog_file_paned_left_value",-32767));
			dialog.setPanedRightValue(configGet("dialog_file_paned_right_value",32767));
			
			int position_x = configGet("dialog_file_position_x",(engine.app.getWidth() - dialog.getWidth()) / 2);
			int position_y = configGet("dialog_file_position_y",(engine.app.getHeight() - dialog.getHeight()) / 2);
			
			dialog.setText(title);
			
			if(config == NULL) {
				if(filter != "") config = replace(filter,".","_");
			} else config = "_" + config;
			
			if(config != NULL && replace(name," ","") == "") name = configGet("dialog_file_path" + config,"");
			
			dialog.setFile(savePath(name));
			
			addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
			dialog.setPermanentFocus();
			dialog.setPosition(position_x,position_y);
			dialog.setFileFocus();
			if(name != "") dialog.setFileFocusSelection();
			dialog.reset();
			
			while(dialog.isDone() == 0) wait 1;
			
			if(dialog.isOkClicked()) {
				name = editorPath(dialog.getFile());
				if(config != NULL) configSet("dialog_file_path" + config,name);
			}
			
			configSet("dialog_file_bookmarks",dialog.getBookmarks());
			configSet("dialog_file_width",dialog.getWidth());
			configSet("dialog_file_height",dialog.getHeight());
			configSet("dialog_file_position_x",dialog.getPositionX());
			configSet("dialog_file_position_y",dialog.getPositionY());
			configSet("dialog_file_paned_left_value",dialog.getPanedLeftValue());
			configSet("dialog_file_paned_right_value",dialog.getPanedRightValue());
			
			dialog.setFocus();
			
			removeChild(dialog);
			
			engine.gui.setKeyPressCallback(old_gui_keypress_callback);
			engine.controls.setKeyPressCallback(old_controls_keypress_callback);
			
			controlsSetEnabled(controls_enabled);
			
			return dialog.isOkClicked();
		}
		
	} /* namespace DialogFile */
	
	/**************************************************************************\
	*
	* Editor::Dialogs
	*
	\**************************************************************************/
	
	/*
	 */
	void init() {
		
		Message::init();
		Progress::init();
		FileConfirm::init();
		Color::init();
		Image::init();
		Vec::init();
		TexCoord::init();
		Texture::init();
		Mask::init();
		World::init();
		Material::init();
		MaterialView::init();
		MaterialName::init();
		MaterialLibraries::init();
		Property::init();
		PropertyName::init();
		PropertyLibraries::init();
		NodeView::init();
		Body::init();
		RagDoll::init();
		Convex::init();
		Decompose::init();
		ImageSize::init();
		TerrainImport::init();
		TerrainExport::init();
		TerrainImageSize::init();
		CorePlugin::init();
		DialogFile::init();
	}
	
	void shutdown() {
		
		Message::shutdown();
		Progress::shutdown();
		FileConfirm::shutdown();
		Color::shutdown();
		Image::shutdown();
		Vec::shutdown();
		TexCoord::shutdown();
		Texture::shutdown();
		Mask::shutdown();
		World::shutdown();
		Material::shutdown();
		MaterialView::shutdown();
		MaterialName::shutdown();
		MaterialLibraries::shutdown();
		Property::shutdown();
		PropertyName::shutdown();
		PropertyLibraries::shutdown();
		NodeView::shutdown();
		Body::shutdown();
		RagDoll::shutdown();
		Convex::shutdown();
		Decompose::shutdown();
		ImageSize::shutdown();
		TerrainImport::shutdown();
		TerrainExport::shutdown();
		TerrainImageSize::shutdown();
		CorePlugin::shutdown();
		DialogFile::shutdown();
	}
	
} /* namespace Editor::Dialogs */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void dialogsInit() {
	Editor::Dialogs::init();
}

void dialogsShutdown() {
	Editor::Dialogs::shutdown();
}

/* dialog message
 */
int dialogMessage(string title,string message) {
	return Editor::Dialogs::Message::run(title,message);
}

int dialogMessageRich(string title,string message) {
	return Editor::Dialogs::Message::run(title,message,1);
}

/* dialog message ok
 */
int dialogMessageOk(string title) {
	string message = engine.getMessage();
	return Editor::Dialogs::Message::runOk(title,message);
}

int dialogMessageOkRich(string title) {
	string message = engine.getMessage();
	return Editor::Dialogs::Message::runOk(title,message,1);
}

int dialogMessageOk(string title,string message) {
	return Editor::Dialogs::Message::runOk(title,message);
}

int dialogMessageOkRich(string title,string message) {
	return Editor::Dialogs::Message::runOk(title,message,1);
}

/* dialog message ok/cancel
 */
int dialogMessageCancelContinue(string title,string message) {
	return Editor::Dialogs::Message::runCancelContinue(title,message);
}

int dialogMessageCancelContinueRich(string title,string message) {
	return Editor::Dialogs::Message::runCancelContinue(title,message,1);
}

/* dialog message yes/no
 */
int dialogMessageYesNo(string title,string message) {
	return Editor::Dialogs::Message::runYesNo(title,message);
}

int dialogMessageYesNoRich(string title,string message) {
	return Editor::Dialogs::Message::runYesNo(title,message,1);
}

/* dialog message yes/no/cancel
 */
int dialogMessageYesNoCancel(string title,string message) {
	return Editor::Dialogs::Message::runYesNoCancel(title,message);
}

int dialogMessageYesNoCancelRich(string title,string message) {
	return Editor::Dialogs::Message::runYesNoCancel(title,message,1);
}

/* dialog progress
 */
int dialogProgressBegin(string title,int cancel_button = 1) {
	return Editor::Dialogs::Progress::begin(title,cancel_button);
}

int dialogProgressUpdate(int progress) {
	return Editor::Dialogs::Progress::update(progress);
}

int dialogProgressUpdate2(int progress) {
	return Editor::Dialogs::Progress::update2(progress);
}

int dialogProgressEnd() {
	return Editor::Dialogs::Progress::end();
}

/* dialog file confirm
 */
int dialogFileConfirm(string name) {
	return Editor::Dialogs::FileConfirm::run(name);
}

/* Editor::Dialogs
 */
int dialogColor(string title,int callback,vec4 color) {
	return Editor::Dialogs::Color::run(title,callback,color);
}

int dialogImage(string title,string name) {
	return Editor::Dialogs::Image::run(title,name);
}

int dialogVec(string title,int callback,int num,string expression,int num_args) {
	return Editor::Dialogs::Vec::run(title,callback,num,expression,num_args);
}

int dialogTexCoord(string title,int callback,int num,string expression,vec4 texcoord,string texture) {
	return Editor::Dialogs::TexCoord::run(title,callback,num,expression,texcoord,texture);
}

int dialogTexture(string title,string type,Material material,int num,int overriden) {
	return Editor::Dialogs::Texture::run(title,type,material,num,overriden);
}

int dialogTexture(string title,string type,string path,int hide_format = 0) {
	return Editor::Dialogs::Texture::run(title,type,path,hide_format);
}

int dialogMask(string title,int callback,int mask,int cb_instance = NULL) {
	return Editor::Dialogs::Mask::run(title,callback,mask,cb_instance);
}

int dialogWorld(string title,string &name,int &add) {
	return Editor::Dialogs::World::run(title,name,add);
}

int dialogMaterial(string title,string &name) {
	return Editor::Dialogs::Material::run(title,name);
}

int dialogMaterialView(string title,string name) {
	return Editor::Dialogs::MaterialView::run(title,name);
}

int dialogMaterialName(string title,string &library,string &name) {
	return Editor::Dialogs::MaterialName::run(title,library,name);
}

int dialogMaterialLibraries(string title) {
	return Editor::Dialogs::MaterialLibraries::run(title);
}

int dialogProperty(string title,string &name) {
	return Editor::Dialogs::Property::run(title,name);
}

int dialogPropertyName(string title,string &library,string &name) {
	return Editor::Dialogs::PropertyName::run(title,library,name);
}

int dialogPropertyLibraries(string title) {
	return Editor::Dialogs::PropertyLibraries::run(title);
}

int dialogNodeView(string title,Node node) {
	return Editor::Dialogs::NodeView::run(title,node);
}

int dialogBody(string title,Body body_0,Body &body) {
	return Editor::Dialogs::Body::run(title,body_0,body);
}

int dialogRagDoll(string title,float &mass,float &error,float &threshold,int &capsule) {
	return Editor::Dialogs::RagDoll::run(title,mass,error,threshold,capsule);
}

int dialogConvex(string title,float &error) {
	return Editor::Dialogs::Convex::run(title,error);
}

int dialogDecompose(string title,int &depth,float &error,float &threshold) {
	return Editor::Dialogs::Decompose::run(title,depth,error,threshold);
}

int dialogImageSize(string title,int &size) {
	return Editor::Dialogs::ImageSize::run(title,size);
}

int dialogImageSizeFlip(string title,int &size,int &flip) {
	return Editor::Dialogs::ImageSize::runFlip(title,size,flip);
}

int dialogImageSizeFormat(string title,int &size,int &format) {
	return Editor::Dialogs::ImageSize::runFormat(title,size,format);
}

int dialogTerrainImport(string title,ObjectTerrain terrain,int &offset_x,int &offset_y,float &scale,int &flip) {
	return Editor::Dialogs::TerrainImport::run(title,terrain,offset_x,offset_y,scale,flip);
}

int dialogTerrainExport(string title,ObjectTerrain terrain,int &offset_x,int &offset_y,int &size_x,int &size_y,float &scale) {
	return Editor::Dialogs::TerrainExport::run(title,terrain,offset_x,offset_y,size_x,size_y,scale);
}

int dialogTerrainImageSize(string title,int &coarse_size,int &surface_size) {
	return Editor::Dialogs::TerrainImageSize::run(title,coarse_size,surface_size);
}

int dialogTerrainImageSize(string title,int &coarse_size,int &surface_size,vec4 &color) {
	return Editor::Dialogs::TerrainImageSize::run(title,coarse_size,surface_size,color);
}

/*
 */
int dialogCorePlugin(string title,string &name) {
	return Editor::Dialogs::CorePlugin::run(title,name);
}

/*
 */
int dialogFile(string title,string filter,string &name,int mode = DIALOG_DEFAULT,string config = NULL) {
	return Editor::Dialogs::DialogFile::run(title,filter,name,mode,config);
}


/*
 */
template dialog_file<NAME,FILTER,CONFIG> int dialogFile ## NAME(string title,string &name,int mode = DIALOG_DEFAULT) {
	return Editor::Dialogs::DialogFile::run(title,FILTER,name,mode,CONFIG);
}

dialog_file<Mesh,".mesh.anim","mesh">;
dialog_file<Node,".node","node">;
dialog_file<Terrain,".ter","terrain">;
dialog_file<Path,".path","path">;
dialog_file<Sound,".wav.ogg.oga","sound">;
dialog_file<Image,".tif.tga.jpg.png.dds.psd.ppm.pgm.hdr.sgi.rgb.rgba","image">;
dialog_file<ImageNoHDR,".tif.tga.jpg.png.dds.psd.ppm.pgm.sgi.rgb.rgba","image">;
dialog_file<Material,".mat","material">;
dialog_file<Property,".prop","property">;
dialog_file<Settings,".settings","settings">;
dialog_file<Plugin,".cpp","plugin">;
dialog_file<World,".world","world">;
dialog_file<Preset,".preset","preset">;
dialog_file<Track,".track","track">;
dialog_file<Fbx,".fbx","fbx">;

/*
 */
template dialog_file_save<NAME,FILTER,CONFIG> int dialogFile ## NAME ## Save(string title,string &name,int mode = DIALOG_DEFAULT) {
	return Editor::Dialogs::DialogFile::run(title,FILTER,name,DIALOG_FILE_SAVE,CONFIG);
}

dialog_file_save<Mesh,".mesh","mesh_save">;
dialog_file_save<Image,".tga.jpg.png.dds.psd","image_save">;

#endif /* __EDITOR_DIALOGS_H__ */
