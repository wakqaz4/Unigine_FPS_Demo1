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


#ifndef __DIALOGS_H__
#define __DIALOGS_H__

/*****************************************************************************\
 *
 * namespace DialogMessage
 *
\*****************************************************************************/

/* message dialog
 */
namespace DialogMessage {
	
	/* constants
	 */
	enum {
		OK		= 0x02,
		CANCEL	= 0x40,
		YES		= 0x01,
		NO		= 0x20,
		ABORT	= 0x04,
		RETRY	= 0x08,
		IGNORE	= 0x10,
	};
	
	string button_names[] = (
		OK		: "OK", 
		CANCEL	: "Cancel",
		YES		: "Yes",
		NO		: "No",
		ABORT	: "Abort",
		RETRY	: "Retry",
		IGNORE	: "Ignore",
	);
	
	string button_images[] = (
		OK		: "gui/dialog_ok.png", 
		CANCEL	: "gui/dialog_cancel.png",
		YES		: "gui/dialog_ok.png",
		NO		: "gui/dialog_cancel.png",
		ABORT	: "gui/dialog_cancel.png",
		RETRY	: "",
		IGNORE	: "",
	);
	
	int GUI_SPACES = 4;
	
	/* variables
	 */
	Gui gui;
	int dialog_result;
	WidgetWindow window;
	WidgetVBox vbox;
	WidgetLabel label;
	WidgetHBox hbox;
	WidgetButton buttons[0];
	
	/* init message box
	 */
	void init() {
		
		gui = engine.getGui();
		
		// generate GUI
		window = new WidgetWindow(gui);
		
		vbox = new WidgetVBox(gui,GUI_SPACES,GUI_SPACES);
		window.addChild(vbox,GUI_ALIGN_EXPAND);
		
		label = new WidgetLabel(gui);
		vbox.addChild(label,GUI_ALIGN_EXPAND);
		
		hbox = new WidgetHBox(gui,GUI_SPACES,0);
		vbox.addChild(hbox,GUI_ALIGN_EXPAND);
	}
	
	/* cleanup
	 */
	void shutdown() {
		
		buttons.delete();
		delete label;
		delete hbox;
		delete vbox;
		delete window;
	}
	
	/* show dialog modal and return result
	 */
	int show(string title,string message,int button_flags,int rich = 0) {
		
		// init gui
		init();
		window.setText(title);
		label.setText(message);
		label.setFontRich(rich);
		
		// generate buttons by given flags
		foreachkey(int b; button_names) {
			if(b & button_flags) {
				WidgetButton button = new WidgetButton(gui,button_names[b]);
				if(button_images[b] != "") button.setImage(new Image(button_images[b]));
				button.setCallback(GUI_CLICKED,"DialogMessage::button_clicked",b);
				hbox.addChild(button);
				buttons.append(button);
			}
		}
		
		// enter hotkey for first button and Esc for last
		buttons[0].setCallbackAccel(GUI_CLICKED,APP_KEY_RETURN,0,0,0);
		buttons[buttons.size() - 1].setCallbackAccel(GUI_CLICKED,APP_KEY_ESC,0,0,0);
		
		// show window
		gui.addChild(window,GUI_ALIGN_CENTER | GUI_ALIGN_OVERLAP);
		window.setPermanentFocus();
		
		// wait until user close dialog
		dialog_result = -1;
		while(dialog_result < 0) wait 1;
		
		// close window
		gui.removeChild(window);
		shutdown();
		
		return dialog_result;
	}
	
	/* on clicked callback for message dialog buttons
	 */
	void button_clicked(int button) {
		dialog_result = button;
	}
}

/*****************************************************************************\
 *
 * namespace DialogColor
 *
\*****************************************************************************/

/* color dialog
 */
namespace DialogColor {
	
	/* variables
	 */
	Gui gui;
	int dialog_result;
	WidgetDialogColor dialog;
	string config_name = "color_dialog_palette_default";
	
	/* load dialog setting from config
	 */
	void load_config() {
		if(engine.config.isExist(config_name)) {
			dialog.setPaletteColors(engine.config.getString(config_name));
		}
	}
	
	/* save dialog settins to config
	 */
	void save_config() {
		engine.config.setString(config_name,dialog.getPaletteColors());
	}
	
	/*
	 */
	void init() {
		
		gui = engine.getGui();
		dialog = new WidgetDialogColor(gui);
		
		WidgetButton ok_b = dialog.getOkButton();
		WidgetButton cancel_b = dialog.getCancelButton();
		ok_b.setCallback(GUI_CLICKED,"DialogColor::dialog_button_clicked",1);
		cancel_b.setCallback(GUI_CLICKED,"DialogColor::dialog_button_clicked",0);
		
		dialog.setMoveable(1);
		dialog.setSizeable(1);
		dialog.setTitleable(0);
		dialog.setBlendable(0);
		dialog.setFloatable(0);
		dialog.setSnapDistance(16);
		dialog.setTextAlign(GUI_ALIGN_LEFT);
		dialog.arrange();
	}
	
	/*
	 */
	void shutdown() {
		delete dialog;
	}
	
	/* callback for dialog buttons
	 */
	void dialog_button_clicked(int res) {
		dialog_result = res;
	}
	
	/* show color dialog modal.
	 * callback - function name
	 * params - additional parameters, passed to callback
	 */
	int show(string title,vec4 color,string callback = "",int params = 0) {
		
		// init
		init();
		dialog.setText(title);
		dialog.setColor(color);
		if(callback != "") dialog.setCallback(GUI_CHANGED,callback,dialog,params);
		load_config();
		
		// show
		gui.addChild(dialog,GUI_ALIGN_OVERLAP);
		dialog.setPermanentFocus();
		
		// wait for result
		dialog_result = -1;
		while(dialog_result < 0) wait 1;
		
		// hide
		if(callback != "") {
			dialog.setCallback(GUI_CHANGED,NULL);
			if(dialog_result == 0) dialog.setColor(color);
			call(callback,dialog,params);
		}
		gui.removeChild(dialog);
		save_config();
		shutdown();
		
		return dialog_result;
	}
}

/*****************************************************************************\
 *
 * namespace DialogFile
 *
\*****************************************************************************/

/* file open/save dialog
 */
namespace DialogFile {
	
	/* variables
	 */
	Gui gui;
	int dialog_result;
	string config_name = "file_dialog_tabs_default";
	WidgetDialogFile dialog;
	
	/* load dialog setting from config
	 */
	void load_config() {
		
		// if no config
		if(engine.config.isExist(config_name) == 0) return;
		
		// convert all tab path to absolete
		string config = engine.config.getString(config_name);
		string tabs[0];
		strsplit(config,";",tabs);
		config = "";
		foreach(string tab; tabs) {
			config += absname(engine.getDataPath(),tab) + ";";
		}
		
		// apply config
		dialog.setTabs(config);
	}
	
	/* save dialog settins to config
	 */
	void save_config() {
		
		string tabs[0];
		string config = "";
		strsplit(dialog.getTabs(),";",tabs);
		foreach(string tab; tabs) {
			config += relname(engine.getDataPath(),tab) + ";";
		}
		engine.config.setString(config_name,config);
	}
	
	/*
	 */
	void init() {
		
		gui = engine.getGui();
		dialog = new WidgetDialogFile(gui);
		
		WidgetButton ok_b = dialog.getOkButton();
		WidgetButton cancel_b = dialog.getCancelButton();
		ok_b.setCallback(GUI_CLICKED,"DialogFile::dialog_button_clicked",1);
		cancel_b.setCallback(GUI_CLICKED,"DialogFile::dialog_button_clicked",0);
		
		dialog.setMoveable(1);
		dialog.setSizeable(1);
		dialog.setTitleable(0);
		dialog.setBlendable(0);
		dialog.setFloatable(0);
		dialog.setSnapDistance(16);
		dialog.setTextAlign(GUI_ALIGN_LEFT);
		dialog.arrange();
	}
	
	/*
	 */
	void shutdown() {
		delete dialog;
	}
	
	/* callback for dialog buttons
	 */
	void dialog_button_clicked(int res) {
		dialog_result = res;
	}
	
	/*
	 */
	int show(string title,string &name,string filter = "") {
		
		// init
		init();
		dialog.setText(title);
		dialog.setFile(name);
		dialog.setFilter(filter);
		load_config();
		
		// show
		gui.addChild(dialog,GUI_ALIGN_OVERLAP);
		dialog.setPermanentFocus();
		
		// wait for result
		dialog_result = -1;
		while(dialog_result < 0) wait 1;
		name = relname(engine.getDataPath(),dialog.getFile());
		
		// hide
		gui.removeChild(dialog);
		save_config();
		shutdown();
		
		return dialog_result;
	}
	
}

/*****************************************************************************\
 *
 * namespace DialogImage
 *
\*****************************************************************************/

/*
 */
namespace DialogImage {
	
	/* variables
	 */
	Gui gui;
	int dialog_result;
	WidgetDialogImage dialog;
	
	/*
	 */
	void init() {
		
		gui = engine.getGui();
		dialog = new WidgetDialogImage(gui);
		
		WidgetButton ok_b = dialog.getOkButton();
		WidgetButton cancel_b = dialog.getCancelButton();
		ok_b.setCallback(GUI_CLICKED,"DialogImage::dialog_button_clicked",1);
		cancel_b.setCallback(GUI_CLICKED,"DialogImage::dialog_button_clicked",0);
		
		dialog.setSizeable(0);
		dialog.setTitleable(0);
		dialog.setBlendable(0);
		dialog.setFloatable(0);
		dialog.setSnapDistance(16);
		dialog.setTextAlign(GUI_ALIGN_LEFT);
		dialog.arrange();
	}
	
	/*
	 */
	void shutdown() {
		delete dialog;
	}
	
	/* callback for dialog buttons
	 */
	void dialog_button_clicked(int res) {
		dialog_result = res;
	}
	
	/* show dialog modal
	 */
	int show(string title,string name) {
		
		// init
		init();
		dialog.setText(title);
		dialog.setTexture(name);
		
		// show
		gui.addChild(dialog,GUI_ALIGN_OVERLAP);
		dialog.setPermanentFocus();
		
		// wait for result
		dialog_result = -1;
		while(dialog_result < 0) wait 1;
		
		// hide
		engine.gui.removeChild(dialog);
		shutdown();
		
		return dialog_result;
	}
	
}

#endif // __DIALOGS_H__
