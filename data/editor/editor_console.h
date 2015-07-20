/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_console.h
 * Desc:    Unigine editor
 * Version: 0.01
 * Author:  Vladimir Borovskikh <futurist@unigine.com>
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
namespace ConsoleLog {
	
	using Unigine::Widgets;
	
	int test = 0;
	
	UserInterface ui;
	Window window;
	GridBox log_gb;
	ScrollBox log_sb;
	
	int is_show;
	
	//
	void init() {
		
		ui = new UserInterface("editor/editor_console.ui");
		if(test) test_func();
		
	}
	
	//
	void test_func() {
		// test consoleLogShow without thread
		consoleLogShow("test");
		
		// test messages
		thread([]() {
			consoleLogMessage("Basic message");
			consoleLogMessageArray("Basic message with button","ButtonName",[]() {
				consoleLogMessageArray("Basic message with two buttons","YesButton",[]() {
					consoleLogMessage("Hello i'm message from YesButton");
					return 1;
				},"NoButton",[]() {
					consoleLogMessage("NoButton");
					return 1;
				});
				return 1;
			});
			consoleLogMessageBrowse("Basic browse button",[]() {
				string name;
				if(dialogFileWorld("Select world",name)) {
					consoleLogMessage(format("You select %s world",name));
					return 1;
				}
				consoleLogMessage(format("You no select world"));
				return 0;
			});
			consoleLogMessageYesNo("Close this dialog???",[]() {
				consoleLogHide();
				return 1;
			},NULL);
			consoleLogShow("Test title");
		});
	}
	
	//
	string getTimestamp() { return date("[%h:%m:%s]",time()); }
	
	//
	void show_message(string text) {
		Label timestamp_l = new Label(getTimestamp());
		Label message_l = new Label();
		
		message_l.setFontRich(1);
		message_l.setText(text);
		log_gb.addChild(timestamp_l,GUI_ALIGN_LEFT);
		log_gb.addChild(message_l,GUI_ALIGN_LEFT);
	}
	
	//
	void message(string text) {
		show_message(text);
		HBox actions_hb = new HBox();
		actions_hb.setHeight(21);
		log_gb.addChild(actions_hb,GUI_ALIGN_EXPAND);
		log_sb.arrange();
		log_sb.setVScrollValue(INFINITY);
	}
	
	//
	void message_array(string text,string btn_name,int callback,Unigine::Vector args) {
		show_message(text);
		
		HBox actions_hb = new HBox();
		
		Button btn0 = new Button(btn_name);
		
		btn0.setCallback(GUI_CLICKED,[](Button btn0,int callback,Unigine::Vector args) {
			int result;
			switch(args.size()) {
				case 0: result = call(callback); break;
				case 1: result = call(callback,args[0]); break;
				case 2: result = call(callback,args[0],args[1]); break;
				case 3: result = call(callback,args[0],args[1],args[2]); break;
				case 4: result = call(callback,args[0],args[1],args[2],args[3]); break;
				case 5: result = call(callback,args[0],args[1],args[2],args[3],args[4]); break;
				case 6: result = call(callback,args[0],args[1],args[2],args[3],args[4],args[5]); break;
				case 7: result = call(callback,args[0],args[1],args[2],args[3],args[4],args[5],args[6]); break;
				default: throw(__FUNC__ + ": To many arguments\n");
			}
			btn0.setEnabled(result != 1);
		},btn0,callback,args);
		
		actions_hb.addChild(btn0,GUI_ALIGN_RIGHT);
		log_gb.addChild(actions_hb,GUI_ALIGN_EXPAND);
		log_sb.arrange();
		log_sb.setVScrollValue(INFINITY);
	}
	
	//
	void message_array(string text,string btn_name0,int callback0,string btn_name1,int callback1,Unigine::Vector args) {
		show_message(text);
		
		HBox actions_hb = new HBox();
		
		Button btn0 = new Button(btn_name0);
		Button btn1 = new Button(btn_name1);
		
		btn0.setCallback(GUI_CLICKED,[](Button btn0,Button btn1,int callback0,Unigine::Vector args) {
			int result;
			switch(args.size()) {
				case 0: result = call(callback0); break;
				case 1: result = call(callback0,args[0]); break;
				case 2: result = call(callback0,args[0],args[1]); break;
				case 3: result = call(callback0,args[0],args[1],args[2]); break;
				case 4: result = call(callback0,args[0],args[1],args[2],args[3]); break;
				case 5: result = call(callback0,args[0],args[1],args[2],args[3],args[4]); break;
				case 6: result = call(callback0,args[0],args[1],args[2],args[3],args[4],args[5]); break;
				case 7: result = call(callback0,args[0],args[1],args[2],args[3],args[4],args[5],args[6]); break;
				default: throw(__FUNC__ + ": To many arguments\n");
			}
			btn0.setEnabled(result != 1);
			btn1.setEnabled(result != 1);
		},btn0,btn1,callback0,args);
		
		btn1.setCallback(GUI_CLICKED,[](Button btn0,Button btn1,int callback1,Unigine::Vector args) {
			if(callback1 == NULL) {
				btn0.setEnabled(0);
				btn1.setEnabled(0);
				return;
			}
			
			int result;
			switch(args.size()) {
				case 0: result = call(callback1); break;
				case 1: result = call(callback1,args[0]); break;
				case 2: result = call(callback1,args[0],args[1]); break;
				case 3: result = call(callback1,args[0],args[1],args[2]); break;
				case 4: result = call(callback1,args[0],args[1],args[2],args[3]); break;
				case 5: result = call(callback1,args[0],args[1],args[2],args[3],args[4]); break;
				case 6: result = call(callback1,args[0],args[1],args[2],args[3],args[4],args[5]); break;
				case 7: result = call(callback1,args[0],args[1],args[2],args[3],args[4],args[5],args[6]); break;
				default: throw(__FUNC__ + ": To many arguments\n");
			}
			
			btn0.setEnabled(result != 1);
			btn1.setEnabled(result != 1);
		},btn0,btn1,callback1,args);
		
		actions_hb.addChild(btn0,GUI_ALIGN_RIGHT);
		actions_hb.addChild(btn1,GUI_ALIGN_RIGHT);
		log_gb.addChild(actions_hb,GUI_ALIGN_EXPAND);
		log_sb.arrange();
		log_sb.setVScrollValue(INFINITY);
	}
	
	//
	void shutdown() {
		hide();
	}
	
	void clear() {
		for(int i = log_gb.getNumChilds() - 1; i >= 0; i--) {
			log_gb.removeChild(log_gb.getChild(i));
		}
	}
	
	//
	int show(string title) {
		if(get_thread() == -1) {
			log.error("%s: %s",__FUNC__,"This function must run in thread\n");
			log.error(get_call_stack());
			return;
		}
		
		addChild(window,ALIGN_OVERLAP | ALIGN_CENTER);
		window.setText(title);
		window.setPermanentFocus();
		is_show = 1;
		while(is_show != 0) wait;
		hide();
	}
	
	//
	int hide(int c = 1) {
		is_show = 0;
		window.removeFocus();
		
		if(c) clear();
		
		removeChild(window);
	}
}

void consoleLogInit() {
	ConsoleLog::init();
}

void consoleLogShutdown() {
	ConsoleLog::shutdown();
}

void consoleLogUpdate() {
	
}

void consoleLogMessage(string message) {
	ConsoleLog::message(message);
}

/*
 */
void consoleLogMessageArray(string message,string button_name,int on_button,int args[] = ()) {
	Unigine::Vector _args = new Unigine::Vector(args);
	ConsoleLog::message_array(message,button_name,on_button,_args);
}

/*
 */
void consoleLogMessageArray(string message,string button_name0,int on_button0,string button_name1,int on_button1,int args[] = ()) {
	Unigine::Vector _args = new Unigine::Vector(args);
	ConsoleLog::message_array(message,button_name0,on_button0,button_name1,on_button1,_args);
}

/*
 */
void consoleLogMessageYesNoArray(string message,int on_yes,int on_no,int args[]) {
	consoleLogMessageArray(message,"Yes",on_yes,"No",on_no,args);
}

void consoleLogMessageYesNo(string message,int on_yes,int on_no) {
	consoleLogMessageYesNoArray(message,on_yes,on_no,());
}

void consoleLogMessageYesNo(string message,int on_yes,int on_no,int arg0) {
	consoleLogMessageYesNoArray(message,on_yes,on_no,(arg0));
}

void consoleLogMessageYesNo(string message,int on_yes,int on_no,int arg0,int arg1) {
	consoleLogMessageYesNoArray(message,on_yes,on_no,(arg0,arg1));
}

void consoleLogMessageYesNo(string message,int on_yes,int on_no,int arg0,int arg1,int arg2) {
	consoleLogMessageYesNoArray(message,on_yes,on_no,(arg0,arg1,arg2));
}

void consoleLogMessageYesNo(string message,int on_yes,int on_no,int arg0,int arg1,int arg2,int arg3) {
	consoleLogMessageYesNoArray(message,on_yes,on_no,(arg0,arg1,arg2,arg3));
}

/*
 */
void consoleLogMessageBrowseArray(string message,int on_browse,int args[]) {
	consoleLogMessageArray(message,"Browse",on_browse,args);
}

void consoleLogMessageBrowse(string message,int on_browse) {
	consoleLogMessageBrowseArray(message,on_browse,());
}

void consoleLogMessageBrowse(string message,int on_browse,int arg0) {
	consoleLogMessageBrowseArray(message,on_browse,(arg0));
}

void consoleLogMessageBrowse(string message,int on_browse,int arg0,int arg1) {
	consoleLogMessageBrowseArray(message,on_browse,(arg0,arg1));
}

void consoleLogMessageBrowse(string message,int on_browse,int arg0,int arg1,int arg2) {
	consoleLogMessageBrowseArray(message,on_browse,(arg0,arg1,arg2));
}

void consoleLogMessageBrowse(string message,int on_browse,int arg0,int arg1,int arg2,int arg3) {
	consoleLogMessageBrowseArray(message,on_browse,(arg0,arg1,arg2,arg3));
}

void consoleLogClear() {
	ConsoleLog::clear();
}

void consoleLogShow(string title = "ConsoleLog") {
	ConsoleLog::show(title);
}
void consoleLogHide(int c = 1) {
	ConsoleLog::hide(c);
}
