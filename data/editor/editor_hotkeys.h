/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_hotkeys.h
 * Desc:    Unigine editor
 * Version: 1.14
 * Author:  Valia Vaneeva <fattie@unigine.com>
 *          Alexander Zapryagaev <frustum@unigine.com>
 *          Pavel Kurinnoy <figonet@unigine.com>
 *          Alexey Egorov <evilsprit@unigine.com>
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

/*
 */
namespace Hotkeys {
	
	/* learning modes
	 */
	enum {
		MODE_NORMAL = 0,
		MODE_LEARN,
	};
	
	enum {
		SKIP_CALLBACKS = 1 << 0,
		SKIP_UPDATES = 1 << 1,
	};
	
	enum {
		DCLICK_WAIT_TIME = 0.3f,
	};
	
	/* key modifiers
	 */
	enum {
		KEY_NONE = 0,
		KEY_SHIFT = 1 << 0,
		KEY_CTRL = 1 << 1,
		KEY_ALT = 1 << 2,
		KEY_CMD = 1 << 3,
		KEY_MOUSE_LEFT = 1 << 4,
		KEY_MOUSE_RIGHT = 1 << 5,
		KEY_MOUSE_MIDDLE = 1 << 6,
		KEY_MOUSE_AUX_0 = 1 << 7,
		KEY_MOUSE_AUX_1 = 1 << 8,
		KEY_MOUSE_AUX_2 = 1 << 9,
		KEY_MOUSE_AUX_3 = 1 << 10,
		KEY_SAME = 1 << 11,
	};
	
	/* 
	 */
	enum {
		MOUSE_WHEEL_UP = 1,
		MOUSE_WHEEL_DOWN,
		MOUSE_WHEEL_RIGHT,
		MOUSE_WHEEL_LEFT,
	};
	
	string mouse_wheel_text[] = (
		TR("none"),
		TR("Wheel Up"),
		TR("Wheel Down"),
		TR("Wheel Right"),
		TR("Wheel Left"),
	);
	
	/*
	 */
	enum {
		PRESET_UNIGINE_2 = 0,
		PRESET_UNIGINE_1,
		PRESET_3DS_MAX,
		NUM_PRESETS,
	};
	
	/*
	 */
	string preset_names[] = (
		TR("UNIGINE 2"),
		TR("UNIGINE 1"),
		TR("3ds Max"),
	);
	
	/**************************************************************************\
	*
	* variables
	*
	\**************************************************************************/
	
	DECLARE_WINDOW
	
	WidgetTabBox tabbox;							// tools tabbox
	WidgetScrollBox scroll_boxes[0];				// tab scroll boxes
	
	string tab_text[0];
	string group_text[0];
	string conflict_group_text[0];
	int plugin_tab;
	
	WidgetGridBox plugin_groups[];					// groups of hotkeys for plugins
	string plugin_hotkeys[];						// hotkeys for plugins
	Handler plugin_defaults[];						// hotkeys defaults for plugins
	
	WidgetWindow learn_w;							// learn key window
	WidgetLabel learn_lb;							// learn window text
	WidgetLabel learn_pattern_lb;					// learn window pattern
	WidgetIcon learn_cancel_i;						// learn cancel icon
	WidgetButton clear_binding_b;					// clear binding button
	WidgetButton cancel_binding_b;					// cancel binding button
	WidgetButton apply_b;							// apply button
	WidgetComboBox preset_cb;						// preset combo box
	
	Hotkey learn_hotkey;							// index of a binding to learn
	int is_key_active;
	int mode;										// learning mode
	int skip_mode;									// skip mode
	int need_cancel;								// learn cancel flag
	int need_clear;									// learn clear flag
	int mouse_time;									// learn double click delay
	int click_modifier;								// learn left click modifier
	int mouse_wheel_callback_enabled;				// mouse wheel callback
	string presets[] = (:);							// presets
	string current_preset_filename;					// current preset filename
	string new_preset_filename;						// new preset filename
	int skip_modifier;								// skip modifiers
	Widget last_focused_window;						// last focesed window
	int last_modifier;								// last modifier
	int edit_default_preset;						// allow edit default presets
	int state_save;
	int state_restore;
	int video_grab;
	int system_menu_toggle;
	string priority_hotkeys[0];
	
	int skip_widget_types[0];
	int standart_preset_loaded_count;
	
	string font_color = "<font color=#ff9900>";
	
	/**************************************************************************\
	*
	* Handlers
	*
	\**************************************************************************/
	
	/*
	 */
	class Handler {
		
		enum {
			DUMMY = 0,
			WIDGET,
			FUNCTION,
			CONSOLE,
		};
		
		int key = 0;
		int button = 0;
		int wheel = 0;
		int modifier = KEY_NONE;
		int type = DUMMY;
		
		Handler() {
			
		}
		
		Handler(Handler h) {
			key = h.key;
			button = h.button;
			wheel = h.wheel;
			modifier = h.modifier;
			type = h.type;
		}
		
		void setKey(int k) { key = k; }
		int getKey() { return key; }
		
		void setButton(int b) { button = b; }
		int getButton() { return button; }
		
		void setWheel(int w) { wheel = w; }
		int getWheel() { return wheel; }
		
		void setModifier(int m) { modifier = m; }
		int getModifier() { return modifier; }
		
		void getType() { return type; }
		
		virtual int isEnabled() { return 1; }
		
		virtual void run() { }
	};
	
	/*
	 */
	class HandlerDummy : Handler {
		
		HandlerDummy() {
			type = DUMMY;
		}
		
		HandlerDummy(int k,int b,int w,int m) {
			type = DUMMY;
			key = k;
			button = b;
			wheel = w;
			modifier = m;
		}
	};
	
	/*
	 */
	class HandlerFunction : Handler {
		
		int function;
		int args[0];
		
		HandlerFunction(int f) {
			type = FUNCTION;
			function = f;
		}
		
		HandlerFunction(int f,int a[]) {
			type = FUNCTION;
			function = f;
			args.copy(a);
		}
		
		HandlerFunction(int f,int k,int b,int w,int m) {
			type = FUNCTION;
			function = f;
			key = k;
			button = b;
			wheel = w;
			modifier = m;
		}
		
		virtual void run() {
			assert(is_function(function,args.size()));
			call(function,args);
		}
	};
	
	/*
	 */
	class HandlerFunctionThread : HandlerFunction {
		
		HandlerFunctionThread(int f) : HandlerFunction(f) { }
		HandlerFunctionThread(int f,int k,int b,int w,int m) : HandlerFunction(f,k,b,w,m) { }
		
		virtual void run() {
			assert(is_function(function));
			thread(function);
		}
	};
	
	/*
	 */
	class HandlerPluginFunction : HandlerFunction {
		
		string plugin_name;
		
		HandlerPluginFunction(string p,int f) : HandlerFunction(f) { plugin_name = p; }
		HandlerPluginFunction(string p,int f,int k,int b,int w,int m) : HandlerFunction(f,k,b,w,m) { plugin_name = p; }
		
		virtual void run() {
			pluginsRun(plugin_name,function);
		}
	};
	
	/*
	 */
	class HandlerConsole : Handler {
		
		string command;
		
		HandlerConsole(string c) {
			type = CONSOLE;
			command = c;
		}
		
		HandlerConsole(string c,int k,int b,int w,int m) {
			type = CONSOLE;
			command = c;
			key = k;
			button = b;
			wheel = w;
			modifier = m;
		}
		
		virtual void run() {
			engine.console.run(command);
		}
	};
	
	/**************************************************************************\
	*
	* Utils
	*
	\**************************************************************************/
	
	/*
	 */
	int get_modifier_state(int mouse_modifiers = 0) {
		
		int modifier = KEY_NONE;
		
		if(engine.app.getKeyState(APP_KEY_SHIFT)) modifier |= KEY_SHIFT;
		if(engine.app.getKeyState(APP_KEY_CTRL)) modifier |= KEY_CTRL;
		if(engine.app.getKeyState(APP_KEY_ALT)) modifier |= KEY_ALT;
		if(engine.app.getKeyState(APP_KEY_CMD)) modifier |= KEY_CMD;
		
		if(mouse_modifiers == 0) return modifier;
		
		if(engine.app.getMouseButtonState(APP_BUTTON_LEFT)) modifier |= KEY_MOUSE_LEFT;
		if(engine.app.getMouseButtonState(APP_BUTTON_MIDDLE)) modifier |= KEY_MOUSE_MIDDLE;
		if(engine.app.getMouseButtonState(APP_BUTTON_RIGHT)) modifier |= KEY_MOUSE_RIGHT;
		if(engine.app.getMouseButtonState(APP_BUTTON_AUX_0)) modifier |= KEY_MOUSE_AUX_0;
		if(engine.app.getMouseButtonState(APP_BUTTON_AUX_1)) modifier |= KEY_MOUSE_AUX_1;
		if(engine.app.getMouseButtonState(APP_BUTTON_AUX_2)) modifier |= KEY_MOUSE_AUX_2;
		if(engine.app.getMouseButtonState(APP_BUTTON_AUX_3)) modifier |= KEY_MOUSE_AUX_3;
		
		return modifier;
	}
	
	int skip_by_widget_focus(Widget skip_types[] = ()) {
		
		Widget focus = engine.gui.getFocus();
		Widget permanent_focus = engine.gui.getPermanentFocus();
		
		if(focus == NULL && permanent_focus == NULL) return 0;
		
		if(focus != NULL) {
			int type = focus.getType();
			if(skip_types.find(type) != -1) return 1;
			if(skip_widget_types.find(type) != -1) return 1;
		}
		
		if(permanent_focus != NULL) {
			int type = permanent_focus.getType();
			if(skip_types.find(type) != -1) return 1;
			if(skip_widget_types.find(type) != -1) return 1;
		}
		
		return 0;
	}
	
	/**************************************************************************\
	*
	* class Hotkey
	*
	\**************************************************************************/
	
	/*
	 */
	class Hotkey {
		
		enum {
			ANY = 0,
			KEYBOARD,
			MOUSE,
			MOUSE_WHEEL,
			MODIFIER,
		};
		
		int skip_widget_types[0] = ();
		
		int type = ANY;						// type
		string name;						// identifier
		string text;						// description
		string tooltip;						// tooltip
		int tab = 0;						// tab
		int group = 0;						// group
		int conflict_group = -1;			// conflict group
		string no_conflict[0] = ();			// no conflict hotkeys
		Handler handler;					// handler
		
		WidgetButton button;				// binding button
		WidgetLabel label;					// description label
		
		int new_key = 0;					// new key to assign
		int new_button = 0;					// new button to assign
		int new_wheel = 0;					// new mouse wheel to assign
		int new_modifier = KEY_SAME;		// new modifier to assign
		
		~Hotkey() {
			delete button;
			delete label;
			delete handler;
		}
		
		//
		void setKey(int k) {
			if(new_modifier == KEY_SAME) new_modifier = KEY_NONE;
			if(k != 0) {
				new_button = 0;
				new_wheel = 0;
			}
			new_key = k;
			WidgetsManager::updateWidgets(this);
		}
		
		int getKey() {
			assert(handler != NULL);
			if(new_modifier == KEY_SAME) return handler.getKey();
			return new_key;
		}
		
		//
		void setButton(int b) {
			if(new_modifier == KEY_SAME) new_modifier = KEY_NONE;
			if(b != 0) {
				new_key = 0;
				new_wheel = 0;
			}
			new_button = b;
			WidgetsManager::updateWidgets(this);
		}
		
		int getButton() {
			assert(handler != NULL);
			if(new_modifier == KEY_SAME) return handler.getButton();
			return new_button;
		}
		
		//
		void setWheel(int w) {
			if(new_modifier == KEY_SAME) new_modifier = KEY_NONE;
			if(w != 0) {
				new_key = 0;
				new_button = 0;
			}
			new_wheel = w;
			WidgetsManager::updateWidgets(this);
		}
		
		int getWheel() {
			assert(handler != NULL);
			if(new_modifier == KEY_SAME) return handler.getWheel();
			return new_wheel;
		}
		
		//
		void setModifier(int m) {
			new_modifier = m;
			WidgetsManager::updateWidgets(this);
		}
		
		int getModifier() {
			assert(handler != NULL);
			if(new_modifier == KEY_SAME) return handler.getModifier();
			return new_modifier;
		}
		
		//
		void getType() {
			return type;
		}
		
		//
		void setSkipWidgetTypes(int widget_types[]) {
			skip_widget_types.clear();
			skip_widget_types.copy(widget_types);
		}
		
		void addSkipWidgetType(int type) {
			if(skip_widget_types.find(type,-1) != -1) return;
			skip_widget_types.append(type);
		}
		
		void removeSkipWidgetType(int type) {
			int id = skip_widget_types.find(type,-1);
			if(id == -1) return;
			skip_widget_types.remove(id);
		}
		
		//
		int isEnabled() {
			assert(handler != NULL);
			return handler.isEnabled();
		}
		
		//
		void run() {
			assert(handler != NULL);
			handler.run();
		}
		
		string getMouseButtonName(int mouse) {
			switch(mouse) {
				case APP_BUTTON_LEFT:		return "Left Click";
				case APP_BUTTON_MIDDLE:		return "Middle Click";
				case APP_BUTTON_RIGHT:		return "Right Click";
				case APP_BUTTON_AUX_0:		return "Aux_0 Click";
				case APP_BUTTON_AUX_1:		return "Aux_1 Click";
				case APP_BUTTON_AUX_2:		return "Aux_2 Click";
				case APP_BUTTON_AUX_3:		return "Aux_3 Click";
				case APP_BUTTON_DCLICK:		return "Double Click";
			}
			return engine.app.getMouseButtonName(mouse);
		}
		
		string get_text(int key,int button,int wheel,int modifier) {
			string modifier_text = "";
			if(modifier & KEY_SHIFT) modifier_text = "Shift+" + modifier_text;
			if(modifier & KEY_ALT) modifier_text = "Alt+" + modifier_text;
			if(modifier & KEY_CTRL) modifier_text = "Ctrl+" + modifier_text;
			if(modifier & KEY_CMD) modifier_text = "Cmd+" + modifier_text;
			if(modifier & KEY_MOUSE_LEFT) modifier_text = "Left click+" + modifier_text;
			if(modifier & KEY_MOUSE_MIDDLE) modifier_text = "Middle click+" + modifier_text;
			if(modifier & KEY_MOUSE_RIGHT) modifier_text = "Right click+" + modifier_text;
			if(modifier & KEY_MOUSE_AUX_0) modifier_text = "Aux_0+" + modifier_text;
			if(modifier & KEY_MOUSE_AUX_1) modifier_text = "Aux_1+" + modifier_text;
			if(modifier & KEY_MOUSE_AUX_2) modifier_text = "Aux_2+" + modifier_text;
			if(modifier & KEY_MOUSE_AUX_3) modifier_text = "Aux_3+" + modifier_text;
			
			if(key) return modifier_text + to_capital(engine.app.getKeyName(key));
			if(button) return modifier_text + getMouseButtonName(button);
			if(wheel) return modifier_text + mouse_wheel_text[wheel];
			if(modifier) return trim(modifier_text,"+");
			
			return "no hotkey";
		}
		
		string to_capital(string str) {
			if(strlen(str) > 0) {
				str = lower(str);
				str[0] = upperSymbol(str[0]);
			}
			return str;
		}
		
		string getText() {
			return get_text(getKey(),getButton(),getWheel(),getModifier());
		}
		
		int isEqual(Hotkey other) {
			return getKey() == other.getKey() && 
				getButton() == other.getButton() && 
				getWheel() == other.getWheel() && 
				getModifier() == other.getModifier();
		}
		
		void isChanged() {
			assert(handler != NULL);
			if(new_modifier == KEY_SAME) return 0;
			if(handler.getKey() != new_key) return 1;
			if(handler.getModifier() != new_modifier) return 1;
			if(handler.getWheel() != new_wheel) return 1;
			if(handler.getButton() != new_button) return 1;
			return 0;
		}
		
		void apply() {
			assert(handler != NULL);
			if(new_modifier == KEY_SAME) return;
			
			handler.setKey(new_key);
			handler.setButton(new_button);
			handler.setWheel(new_wheel);
			handler.setModifier(new_modifier);
			
			WidgetsManager::updateWidgets(this);
		}
		
		void cancel() {
			new_key = 0;
			new_button = 0;
			new_wheel = 0;
			new_modifier = KEY_SAME;
			
			WidgetsManager::updateWidgets(this);
		}
		
		int skip() {
			if(skip_mode & SKIP_UPDATES) return 1;
			if(engine.console.getActivity()) return 1;
			if(Hotkeys::mode == Hotkeys::MODE_LEARN) return 1;
			if(skip_by_widget_focus(skip_widget_types)) return 1;
			return 0;
		}
		
		int getKeyState() {
			if(skip()) return false;
			return (getKey() != 0 && engine.app.getKeyState(getKey()));
		}
		
		int getButtonState() {
			if(skip()) return false;
			
			if(engine.gui.getMouseButton() == APP_BUTTON_DCLICK) return (getButton() != 0 && engine.gui.getMouseButton() == getButton());
			
			return (getButton() != 0 && engine.app.getMouseButton() == getButton()
			 && engine.gui.getMouseButton() == getButton());
		}
		
		int getWheelState() {
			if(skip()) return false;
			int wheel = 0;
			int axis_x = engine.app.getMouseAxis(APP_AXIS_X);
			int axis_y = engine.app.getMouseAxis(APP_AXIS_Y);
			if(axis_x > 0) wheel = MOUSE_WHEEL_RIGHT;
			if(axis_x < 0) wheel = MOUSE_WHEEL_LEFT;
			if(axis_y > 0) wheel = MOUSE_WHEEL_UP;
			if(axis_y < 0) wheel = MOUSE_WHEEL_DOWN;
			return (getWheel() != 0 && getWheel() == wheel);
		}
		
		int getModifierState() {
			if(skip()) return false;
			int modifier = get_modifier_state(type == KEYBOARD);
			if(type == MODIFIER) return modifier & getModifier();
			return (modifier == getModifier() || modifier == (getModifier() | skip_modifier));
		}
		
		int clearKeyState() {
			if(skip()) return false;
			return (getKey() != 0 && engine.app.clearKeyState(getKey()));
		}
		
		int clearButtonState() {
			if(skip()) return false;
			if(getButton() != 0 && engine.app.getMouseButton() == getButton()) {
				engine.app.clearMouseButtonState(getButton());
				return true;
			}
			return false;
		}
		
		int clearWheelState() {
			if(skip()) return false;
			int wheel = 0;
			int axis_x = engine.app.clearMouseAxis(APP_AXIS_X);
			int axis_y = engine.app.clearMouseAxis(APP_AXIS_Y);
			if(axis_x > 0) wheel = MOUSE_WHEEL_RIGHT;
			if(axis_x < 0) wheel = MOUSE_WHEEL_LEFT;
			if(axis_y > 0) wheel = MOUSE_WHEEL_UP;
			if(axis_y < 0) wheel = MOUSE_WHEEL_DOWN;
			return (getWheel() != 0 && getWheel() == wheel);
		}
		
		int clearModifierState() {
			if(skip()) return false;
			int modifier = get_modifier_state(type == KEYBOARD);
			if(getModifier() & Hotkeys::KEY_SHIFT) engine.app.clearKeyState(APP_KEY_SHIFT);
			if(getModifier() & Hotkeys::KEY_CTRL) engine.app.clearKeyState(APP_KEY_CTRL);
			if(getModifier() & Hotkeys::KEY_ALT) engine.app.clearKeyState(APP_KEY_ALT);
			if(getModifier() & Hotkeys::KEY_CMD) engine.app.clearKeyState(APP_KEY_CMD);
			if(type == MODIFIER) return modifier & getModifier();
			return (modifier == getModifier() || modifier == (getModifier() | skip_modifier));
		}
		
		int getState(int modifier_require = 1) {
			int modifier_state = getModifierState();
			if(modifier_require && !modifier_state) return false;
			else if(getModifier() != 0 && !modifier_state) return false;
			return (getKeyState() || getButtonState() || getWheelState());
		}
		
		int clearState(int modifier_require = 1) {
			int modifier_state = getModifierState();
			if(modifier_require && !modifier_state) return false;
			else if(getModifier() != 0 && !modifier_state) return false;
			return (clearKeyState() || clearButtonState() || clearWheelState());
		}
	};
	
	/**************************************************************************\
	*
	* Widgets Manager
	*
	\**************************************************************************/
	
	namespace WidgetsManager {
		
		/* widget contains hotkeys and updates himself if hotkeys has changed
		 */
		class WidgetWithHotkeys {
			
			Widget widget;
			string original_tooltip;
			Hotkey hotkeys[];			// key is a placeholder name for hotkey in original_tooltip
			Hotkey hotkeys_order[0];	// order is equivalent to order in tooltip
			int hotkeys_offsets[0];		// pos offset for hotkeys in hotkeys_vector
			int hotkeys_text_size[0];	// text size of each current hotkey
			
			/*
			 */
			WidgetWithHotkeys(Widget w) {
				widget = w;
				original_tooltip = w.getToolTip();
				
				parse_tooltip();
				update();
			}
			
			/* if(hotkey_changed == NULL) - update all
			 */
			void update(Hotkey hotkey_changed = NULL) {
				
				forloop(int i = 0; hotkeys_order.size()) {
					Hotkey hotkey = hotkeys_order[i];
					
					if(hotkey_changed == NULL || hotkey_changed == hotkey) {
						
						string new_text = "(<b>" + hotkey.getText() + "</b>)";
						int text_size = strlen(new_text);
						
						string cur_tooltip = widget.getToolTip();
						string new_tooltip = substr(cur_tooltip,0,hotkeys_offsets[i]);
						new_tooltip += new_text;
						new_tooltip += substr(cur_tooltip,hotkeys_offsets[i] + hotkeys_text_size[i]);
						
						int size_diff = text_size - hotkeys_text_size[i];
						update_offsets(i + 1,size_diff);
						
						hotkeys_text_size[i] = text_size;
						
						widget.setToolTip(new_tooltip);
					}
				}
			}
			
			/*
			 */
			int isContainingHotkey(Hotkey hotkey) {
				return hotkeys.check(hotkey.name);
			}
			
			/*
			 */
			void parse_tooltip() {
				
				int length = strlen(original_tooltip);
				
				for(int i = 0; i < (length - 3); i++) {
					if(original_tooltip[i] != '$') continue;
					if(original_tooltip[i+1] != '(') continue;
					
					string hotkey_string = "";
					
					for(int j = (i + 2); j < length; j++) {
						if(original_tooltip[j] == ')') {
							
							Hotkey hotkey = get_hotkey_by_name(hotkey_string);
							if(hotkey != NULL) {
								if(hotkeys.check(hotkey.name) == 0) hotkeys.append(hotkey.name,hotkey);
								
								hotkeys_order.append(hotkey);
								hotkeys_offsets.append(i);
								hotkeys_text_size.append(strlen(hotkey_string) + 3);
							}
							
							i = j;
							break;
						}
						hotkey_string += format("%c",original_tooltip[j]);
						
						if(j == (length - 1)) return;
					}
				}
			}
			
			/*
			*/
			void update_offsets(int pos, int size_diff) {
				
				for(int i = pos; i < hotkeys_offsets.size(); i++) {
					hotkeys_offsets[i] += size_diff;
				}
			}
		};
		
		WidgetWithHotkeys hotkeys_widgets[]; // key is widget
		
		/*
		 */
		void addToolTipWidget(Widget w) {
			
			if(hotkeys_widgets.check(w) == 1) return;
			if(!hasTemplateTooltip(w)) return;
			
			WidgetWithHotkeys hotkeys_widget = new WidgetWithHotkeys(w);
			hotkeys_widgets.append(w,hotkeys_widget);
		}
		
		/*
		 */
		void removeToolTipWidget(Widget w) {
			
			if(hotkeys_widgets.check(w) != 0) {
				WidgetWithHotkeys hotkeys_widget = hotkeys_widgets[w];
				delete hotkeys_widget;
				
				hotkeys_widgets.remove(w);
			}
		}
		
		/*
		 */
		void addUiToolTips(UserInterface ui) {
			if(ui == NULL) return;
			if(is_user_class(ui)) ui = Unigine::Widgets::UserInterface(ui).ui;
			
			forloop(int i = 0; ui.getNumWidgets()) addToolTipWidget(ui.getWidget(i));
		}
		
		/*
		 */
		void removeUiToolTips(UserInterface ui) {
			if(ui == NULL) return;
			if(is_user_class(ui)) ui = Unigine::Widgets::UserInterface(ui).ui;
			
			forloop(int i = 0; ui.getNumWidgets()) removeToolTipWidget(ui.getWidget(i));
		}
		
		/*
		 */
		void updateWidgets(Hotkey hotkey_changed = NULL) {
			if(hotkey_changed == NULL) {
				hotkeys_widgets.call("update");
			} else {
				
				foreach(WidgetWithHotkeys w; hotkeys_widgets) {
					if(w.isContainingHotkey(hotkey_changed)) w.update(hotkey_changed);
				}
			}
		}
		
		/*
		 */
		void addHotkeyNameToWidget(Widget w,string hotkey_to_add) {
			Hotkey hotkey = get_hotkey_by_name(hotkey_to_add);
			if(hotkey == NULL) return;
			
			string tooltip = w.getToolTip();
			tooltip +=  " " + "$(" + hotkey.name + ")";
			w.setToolTip(tooltip);
		}
		
		/*
		 */
		int hasTemplateTooltip(Widget w) {
			string str = w.getToolTip();
			return (strstr(str,"$(") != -1);
		}
	}
	
	/**************************************************************************\
	*
	* Hotkeys
	*
	\**************************************************************************/
	
	Hotkey hotkeys_layout[0] = ();
	Hotkey hotkeys[] = (:);
	
	/*
	 */
	Hotkey add_hotkey(int type,string name,string text,string tooltip,int tab,int group,int conflict_group = -1) {
		
		Hotkey hotkey = get_hotkey_by_name(name);
		
		if(hotkey == NULL) {
			hotkey = new Hotkey();
			hotkey.type = type;
			hotkey.name = name;
			hotkey.text = text;
			hotkey.tooltip = tooltip;
			hotkey.handler = new Handler();
			hotkey.tab = tab;
			hotkey.group = group;
			hotkey.conflict_group = conflict_group;
			hotkeys.append(name,hotkey);
		}
		
		return hotkey;
	}
	
	Hotkey set_hotkey_handler(string name,Handler handler) {
		Hotkey hotkey = get_hotkey_by_name(name);
		if(hotkey == NULL) return NULL;
		delete hotkey.handler;
		hotkey.handler = handler;
		hotkey.setKey(hotkey.handler.getKey());
		hotkey.setButton(hotkey.handler.getButton());
		hotkey.setModifier(hotkey.handler.getModifier());
		return hotkey;
	}
	
	/*
	 */
	int is_hotkeys_changed() {
		foreach(Hotkey hotkey; hotkeys) {
			if(hotkey.isChanged()) return 1;
		}
		return 0;
	}
	
	/*
	 */
	int is_preset_changed() {
		return (current_preset_filename != new_preset_filename);
	}
	
	/*
	 */
	void apply_preset() {
		current_preset_filename = new_preset_filename;
	}
	
	/*
	 */
	void cancel_preset() {
		if(new_preset_filename != current_preset_filename) {
			new_preset_filename = current_preset_filename;
			set_preset(presets[current_preset_filename]);
			load_preset(current_preset_filename);
		}
	}
	
	/*
	 */
	void apply_hotkeys() { hotkeys.call("apply"); }
	
	/*
	 */
	void cancel_hotkeys() { hotkeys.call("cancel"); }
	
	/*
	 */
	Hotkey get_hotkey_by_key(int key,int modifier) {
		foreach(Hotkey hotkey; hotkeys) {
			if(hotkey.getKey() == key && hotkey.getModifier() == modifier) return hotkey;
		}
		return NULL;
	}
	
	/*
	 */
	Hotkey get_hotkey_by_name(string name) {
		if(!hotkeys.check(name)) return NULL;
		return hotkeys[name];
	}
	
	/*
	 */
	void get_hotkeys(int key,int button,int wheel,int modifier,Hotkey result[]) {
		
		result.clear();
		
		foreach(Hotkey hotkey; hotkeys) {
			if(hotkey.getModifier() != modifier) continue;
			
			if(key && hotkey.getKey() == key) result.append(hotkey);
			else if(button && hotkey.getButton() == button) result.append(hotkey);
			else if(wheel && hotkey.getWheel() == wheel) result.append(hotkey);
			else if(modifier && hotkey.getKey() == 0 && hotkey.getButton() == 0 && hotkey.getWheel() == 0) result.append(hotkey);
		}
	}
	
	int is_key_binded(int key) {
		foreach(Hotkey hotkey; hotkeys) {
			if(hotkey.getKey() == key) return true;
		}
		return false;
	}
	
	int is_key_binded(int key,string exclude_groups[]) {
		int groups[];
		foreach(string g; exclude_groups) {
			int index = group_text.find(g);
			if(index == -1) continue;
			groups.append(index,index);
		}
		
		foreach(Hotkey hotkey; hotkeys) {
			if(groups.check(hotkey.group)) continue;
			if(hotkey.getKey() == key) return true;
		}
		groups.clear();
		return false;
	}
	
	/*
	 */
	int check_priority_hotkeys(Hotkey hotkey) {
		forloop(int i = 0; priority_hotkeys.size()) {
			Hotkey priority_hotkey = Hotkeys::get_hotkey_by_name(priority_hotkeys[i]);
			if(priority_hotkey.name == hotkey.name) continue;
			if(priority_hotkey.isEqual(hotkey)) return true;
		}
		return false;
	}
	
	/**************************************************************************\
	*
	* Input callbacks
	*
	\**************************************************************************/
	
	/*
	 */
	int clear_hotkey_binding(int key = 0,int button = 0,int wheel = 0,int modifier = 0) {
		
		if(learn_hotkey.getModifier() != modifier) return false;
		
		int flag = false;
		
		if(key && learn_hotkey.getKey() == key) flag = true;
		else if(button && learn_hotkey.getButton() == button) flag = true;
		else if(wheel && learn_hotkey.getWheel() == wheel) flag = true;
		else if(modifier && learn_hotkey.getKey() == 0 && learn_hotkey.getButton() == 0 && learn_hotkey.getWheel() == 0) flag = true;
		
		if(!flag) return false;
		
		learn_hotkey.setKey(0);
		learn_hotkey.setButton(0);
		learn_hotkey.setWheel(0);
		learn_hotkey.setModifier(KEY_NONE);
		mode = MODE_NORMAL;
		
		return true;
	}
	
	/*
	 */
	int check_hotkey_conflicts(int key = 0,int button = 0,int wheel = 0,int modifier = 0) {
		
		Hotkey key_hotkeys[0];
		get_hotkeys(key,button,wheel,modifier,key_hotkeys);
		
		Hotkey conflict_hotkeys[0];
		
		foreach(Hotkey hotkey; key_hotkeys) {
			if(hotkey == learn_hotkey) continue;
			
			if(learn_hotkey.type == Hotkey::ANY && hotkey.type == Hotkey::MODIFIER) continue;
			if(learn_hotkey.type == Hotkey::KEYBOARD && hotkey.type != Hotkey::KEYBOARD && hotkey.type != Hotkey::ANY) continue;
			if(learn_hotkey.type == Hotkey::MOUSE && hotkey.type != Hotkey::MOUSE && hotkey.type != Hotkey::ANY) continue;
			if(learn_hotkey.type == Hotkey::MOUSE_WHEEL && hotkey.type != Hotkey::MOUSE_WHEEL && hotkey.type != Hotkey::ANY) continue;
			if(learn_hotkey.type == Hotkey::MODIFIER && hotkey.type != Hotkey::MODIFIER) continue;
			
			if(learn_hotkey.no_conflict.find(hotkey.name) != -1) continue;
			if(hotkey.conflict_group != learn_hotkey.conflict_group) continue;
			
			conflict_hotkeys.append(hotkey);
		}
		
		if(conflict_hotkeys.size() > 0) {
			disable_callbacks();
			mouse_time = -1;
			
			string message = "";
			message += format(TR("Hotkey \"%s%s</font>\" is already assigned<br>"),font_color,conflict_hotkeys[0].getText());
			foreach(Hotkey conflict_hotkey; conflict_hotkeys) {
				message += format(TR("to the action \"%s%s</font>\" from the \"%s%s</font>\" tab<br>"),font_color,conflict_hotkey.text,font_color,TR(tab_text[conflict_hotkey.tab]));
			}
			
			message += format(TR("<br>Do you want to overwrite it?"));
			if(dialogMessageYesNoRich(TR("Hotkey Conflict"),message)) {
				foreach(Hotkey conflict_hotkey; conflict_hotkeys) {
					conflict_hotkey.setKey(0);
					conflict_hotkey.setButton(0);
					conflict_hotkey.setWheel(0);
					conflict_hotkey.setModifier(KEY_NONE);
				}
				learn_hotkey.setKey(key);
				learn_hotkey.setButton(button);
				learn_hotkey.setWheel(wheel);
				learn_hotkey.setModifier(modifier);
				learn_hotkey.clearState();
				mode = MODE_NORMAL;
				
				window.setFocus();
				#ifdef HAS_INTERFACE
					interface.setFocus();
				#endif
			}
			engine.app.clearMouseButtonState(button);
			
			enable_callbacks();
			
			return true;
		}
		
		window.setFocus();
		#ifdef HAS_INTERFACE
			interface.setFocus();
		#endif
		
		return false;
	}
	
	/*
	 */
	int learn_hotkey_binding(int key = 0,int button = 0,int wheel = 0,int modifier = 0) {
		if(!clear_hotkey_binding(key,button,wheel,modifier) && !check_hotkey_conflicts(key,button,wheel,modifier)) {
			learn_hotkey.setKey(key);
			learn_hotkey.setButton(button);
			learn_hotkey.setWheel(wheel);
			learn_hotkey.setModifier(modifier);
			learn_hotkey.clearState();
			mode = MODE_NORMAL;
		}
	}
	
	/*
	 */
	void run_hotkey(int key = 0,int button = 0,int wheel = 0,int modifier = 0) {
		
		if(is_key_active) return 0;
		if(skip_mode & SKIP_CALLBACKS) return 0;
		
		Hotkey key_hotkeys[0];
		
		get_hotkeys(key,button,wheel,modifier,key_hotkeys);
		if(key_hotkeys.size() == 0) return 0; // unknown hotkey
		
		Hotkey current_hotkey = NULL;
		
		// priority hotkeys
		foreach(Hotkey hotkey; key_hotkeys) {
			if(priority_hotkeys.find(hotkey.name) != -1) {
				current_hotkey = hotkey;
				break;
			}
		}
		
		Widget focused_window = viewportsGetFocusedWindow();
		
		// search in other hotkey types
		if(current_hotkey == NULL) {
			foreach(Hotkey hotkey; key_hotkeys) {
				if(hotkey.handler.getType() == Handler::WIDGET && focused_window != NULL) continue;
				if(hotkey.handler.getType() == Handler::DUMMY) continue;
				current_hotkey = hotkey;
				break;
			}
		}
		
		if(current_hotkey != NULL && !skip_by_widget_focus(current_hotkey.skip_widget_types) && current_hotkey.handler.getType() != Handler::DUMMY) {
			current_hotkey.handler.run();
		}
	}
	
	/*
	 */
	int is_callback_disabled() {
		if(skip_by_widget_focus()) return 1;
		if(trackerIsFocused()) return 1;
		if(engine.console.getActivity() || (gui.getPermanentFocus() != NULL && mode != MODE_LEARN)) return 1;
		return 0;
	}
	
	/*
	 */
	int key_press_callback_gui(int key) {
		
		if(skip_mode & SKIP_CALLBACKS) return 0;
		if(skip_by_widget_focus()) return 0;
		if(mode == MODE_LEARN) return 1;
		if(trackerIsFocused()) return 0;
		
		is_key_active = (engine.console.getActivity() || gui.getKeyActivity(key));
		if(is_key_active) return 0;
		
		if(key == APP_KEY_SHIFT) return 0;
		if(key == APP_KEY_CTRL) return 0;
		if(key == APP_KEY_ALT) return 0;
		if(key == APP_KEY_CMD) return 0;
		
		if(gui.getPermanentFocus() != NULL) return 0;
		
		int modifier = get_modifier_state(true);
		return (get_hotkey_by_key(key,modifier) != NULL);
	}
	
	/*
	 */
	int button_press_callback_controls(int button) {
		
		if(is_callback_disabled()) return 0;
		
		if(need_cancel) {
			mode = MODE_NORMAL;
			need_cancel = false;
			engine.app.clearMouseButtonState(button);
			return 1;
		}
		
		if(need_clear && mode == MODE_LEARN) {
			learn_hotkey.setKey(0);
			learn_hotkey.setButton(0);
			learn_hotkey.setWheel(0);
			learn_hotkey.setModifier(KEY_NONE);
			mode = MODE_NORMAL;
			need_clear = false;
			engine.app.clearMouseButtonState(button);
			return 1;
		}
		
		int modifier = get_modifier_state();
		
		if(mode == MODE_LEARN) {
			
			if(mouse_time == -1 && button == APP_BUTTON_DCLICK) return 1;
			if(learn_hotkey.type == Hotkey::MODIFIER) return 1;
			if(learn_hotkey.type == Hotkey::KEYBOARD) return 1;
			if(learn_hotkey.type == Hotkey::MOUSE_WHEEL) return 1;
			
			if(button == APP_BUTTON_LEFT && mouse_time == -1) mouse_time = clock();
			else learn_hotkey_binding(0,button,0,click_modifier);
			
			engine.app.clearMouseButtonState(button);
		}
		else {
			run_hotkey(0,button,0,modifier);
		}
		
		return 1;
	}
	
	/*
	 */
	int key_press_callback_controls(int key) {
		
		if(is_callback_disabled()) return 0;
		
		int modifier = get_modifier_state(true);
		
		if(mode == MODE_LEARN) {
			
			if(learn_hotkey.type == Hotkey::MOUSE) return 1;
			if(learn_hotkey.type == Hotkey::MOUSE_WHEEL) return 1;
			
			if(learn_hotkey.type == Hotkey::MODIFIER) {
				switch(key) {
					case APP_KEY_SHIFT: modifier = KEY_SHIFT; break;
					case APP_KEY_CTRL: modifier = KEY_CTRL; break;
					case APP_KEY_ALT: modifier = KEY_ALT; break;
					case APP_KEY_CMD: modifier = KEY_CMD; break;
				}
				learn_hotkey_binding(0,0,0,modifier);
				return 1;
			}
			
			if(key != APP_KEY_SHIFT && key != APP_KEY_CTRL && key != APP_KEY_ALT && key != APP_KEY_CMD) {
				learn_hotkey_binding(key,0,0,modifier);
			}
			return 1;
		}
		
		run_hotkey(key,0,0,modifier);
		return 1;
	}
	
	/*
	 */
	int key_release_callback_controls(int key) {
		if(is_callback_disabled()) return 0;
		
		return 1;
	}
	
	/*
	 */
	int mouse_wheel_changed() {
		
		if(is_callback_disabled()) return 0;
		if(skip_by_widget_focus((WIDGET_EDITTEXT,WIDGET_EDITLINE,WIDGET_SLIDER,WIDGET_COMBOBOX,WIDGET_TREEBOX,WIDGET_LISTBOX,WIDGET_SCROLLBOX,WIDGET_SCROLL))) return 0;
		
		int modifier = get_modifier_state();
		
		int axis_x = engine.app.getMouseAxis(APP_AXIS_X);
		int axis_y = engine.app.getMouseAxis(APP_AXIS_Y);
		
		int wheel = 0;
		if(axis_x > 0) wheel = MOUSE_WHEEL_RIGHT;
		if(axis_x < 0) wheel = MOUSE_WHEEL_LEFT;
		if(axis_y > 0) wheel = MOUSE_WHEEL_UP;
		if(axis_y < 0) wheel = MOUSE_WHEEL_DOWN;
		
		if(wheel == 0) return 0;
		
		if(mode == MODE_LEARN) learn_hotkey_binding(0,0,wheel,modifier);
		else run_hotkey(0,0,wheel,modifier);
		
		return 1;
	}
	
	/**************************************************************************\
	*
	* Preset functions
	*
	\**************************************************************************/
	
	/*
	 */
	int load_preset(string name) {
		
		Xml xml = new Xml();
		
		if(xml.load(savePath(name)) == 0) {
			dialogMessageOk(format(TR("Error loading \"%s\" file."),name));
			delete xml;
			return 0;
		}
		
		string assigned_hotkeys[] = (:);
		forloop(int i = 0; xml.getNumChilds()) {
			Xml x = xml.getChild(i);
			
			if(x.getName() != "hotkey") continue;
			if(x.isArg("name") == 0) continue;
			
			Hotkey hotkey = get_hotkey_by_name(x.getArg("name"));
			if(hotkey == NULL) continue;
			assigned_hotkeys.append(hotkey,x.getArg("name"));
			hotkey.setKey(x.getIntArg("key",0));
			hotkey.setButton(x.getIntArg("button",0));
			hotkey.setWheel(x.getIntArg("wheel",0));
			hotkey.setModifier(x.getIntArg("modifier",KEY_NONE));
		}
		
		foreach(Hotkey hotkey; hotkeys) {
			if(assigned_hotkeys.check(hotkey)) continue;
			hotkey.setKey(0);
			hotkey.setButton(0);
			hotkey.setWheel(0);
			hotkey.setModifier(KEY_NONE);
		}
		
		xml.printUnusedData(name);
		delete xml;
		
		return 1;
	}
	
	/*
	 */
	int save_preset(string name) {
		
		Xml xml = new Xml("preset");
		foreach(Hotkey hotkey; hotkeys) {
			Xml x = xml.addChild("hotkey",format("name=\"%s\"",hotkey.name));
			if(hotkey.getKey() != 0) x.setIntArg("key",hotkey.getKey());
			if(hotkey.getButton() != 0) x.setIntArg("button",hotkey.getButton());
			if(hotkey.getWheel() != 0) x.setIntArg("wheel",hotkey.getWheel());
			if(hotkey.getModifier() != KEY_NONE) x.setIntArg("modifier",hotkey.getModifier());
		}
		
		if(xml.save(savePath(name)) == 0) {
			dialogMessageOk(format(TR("Error saving \"%s\" file."),name));
		}
		
		delete xml;
	}
	
	/*
	 */
	void set_preset(string name) {
		forloop(int i = 0; preset_cb.getNumItems() - 1) {
			if(preset_cb.getItemText(i) == name) {
				preset_cb.setCurrentItem(i);
				break;
			}
		}
	}
	
	/*
	 */
	void remove_preset(int num,string filename) {
		presets.remove(filename);
		preset_cb.removeItem(num);
	}
	
	/*
	 */
	int add_preset(string filename) {
		string name = basename(filename);
		
		if(presets.check(filename)) {
			set_preset(name);
			new_preset_filename = filename;
			return false;
		}
		
		presets.append(filename,name);
		
		preset_cb.removeItem(preset_cb.getNumItems() - 1);
		preset_cb.addItem(name);
		preset_cb.addItem(TR("Load custom..."));
		
		set_preset(name);
		new_preset_filename = filename;
		
		tabbox.setFocus();
		
		return true;
	}
	
	/**************************************************************************\
	*
	* GUI callbacks
	*
	\**************************************************************************/
	
	/*
	 */
	void cancel_enter() {
		need_cancel = 1;
	}
	
	void cancel_leave() {
		need_cancel = 0;
	}
	
	/*
	 */
	void clear_enter() {
		need_clear = 1;
	}
	
	void clear_leave() {
		need_clear = 0;
	}
	
	/* binding learn button callback
	 */
	void binding_clicked(string name) {
		
		engine.console.setLock(1);
		
		learn_hotkey = get_hotkey_by_name(name);
		assert(learn_hotkey != NULL);
		
		#ifdef HAS_INTERFACE
			Gui gui = interface.getGui();
		#else
			Gui gui = engine.getGui();
		#endif
		
		string type = TR("Accepted values: ");
		switch(learn_hotkey.type) {
			case Hotkey::ANY: type += TR("Alt/Shift/Ctrl/Cmd + any keyboard key or mouse button"); break;
			case Hotkey::KEYBOARD: type += TR("Alt/Shift/Ctrl/Cmd/Mouse button + any keyboard key"); break;
			case Hotkey::MOUSE: type += TR("Alt/Shift/Ctrl/Cmd + any mouse button"); break;
			case Hotkey::MOUSE_WHEEL: type += TR("Alt/Shift/Ctrl/Cmd + mouse wheel"); break;
			case Hotkey::MODIFIER: type += TR("Alt/Shift/Ctrl/Cmd"); break;
		}
		
		learn_lb.setFontRich(1);
		learn_lb.setText(format(TR("Press a key combination for \"%s%s</font>\" action."),font_color,learn_hotkey.text));
		learn_pattern_lb.setText(TR(type));
		
		learn_cancel_i.setCallback(GUI_ENTER,functionid(cancel_enter));
		learn_cancel_i.setCallback(GUI_LEAVE,functionid(cancel_leave));
		cancel_binding_b.setCallback(GUI_ENTER,functionid(cancel_enter));
		cancel_binding_b.setCallback(GUI_LEAVE,functionid(cancel_leave));
		clear_binding_b.setCallback(GUI_ENTER,functionid(clear_enter));
		clear_binding_b.setCallback(GUI_LEAVE,functionid(clear_leave));
		
		learn_w.arrange();
		if(window.getWidth() < learn_w.getWidth()) window.setWidth(learn_w.getWidth());
		window.arrange();
		
		learn_w.setPosition((window.getWidth() - learn_w.getWidth()) * 0.5f,(window.getHeight() - learn_w.getHeight()) * 0.5f);
		
		gui.addChild(learn_w,GUI_ALIGN_OVERLAP);
		learn_w.setPermanentFocus();
		
		mode = MODE_LEARN;
		mouse_time = -1;
		click_modifier = 0;
		
		while(mode == MODE_LEARN) {
			if(mouse_time != -1 && clock() - mouse_time > DCLICK_WAIT_TIME) button_press_callback_controls(APP_BUTTON_LEFT);
			if(mouse_time == -1) click_modifier = get_modifier_state();
			wait;
		}
		
		engine.console.setLock(0);
		engine.app.clearMouseButtonState(APP_BUTTON_LEFT);
		
		learn_cancel_i.setCallback(GUI_ENTER,NULL);
		learn_cancel_i.setCallback(GUI_LEAVE,NULL);
		cancel_binding_b.setCallback(GUI_ENTER,NULL);
		cancel_binding_b.setCallback(GUI_LEAVE,NULL);
		clear_binding_b.setCallback(GUI_ENTER,NULL);
		clear_binding_b.setCallback(GUI_LEAVE,NULL);
		
		learn_w.removeFocus();
		gui.removeChild(learn_w);
		
		fix_plugin_groups_width();
		
		update_hotkeys();
	}
	
	/*
	 */
	void preset_changed() {
		
		if(preset_cb.getCurrentItem() == preset_cb.getNumItems() - 1) {
			
			string filename;
			if(dialogFilePreset(TR("Load Preset"),filename,DIALOG_FILE_OPEN) == 0) {
				set_preset(presets.check(new_preset_filename,""));
				return;
			}
			
			if(load_preset(filename) == 0) {
				set_preset(presets.check(new_preset_filename,""));
				return;
			}
			
			if(!add_preset(filename)) {
				if(presets.check(filename,NULL) != NULL) set_preset(presets.check(filename,""));
				else set_preset(presets.check(new_preset_filename,""));
			} else {
				apply_hotkeys();
			}
		}
		else {
			
			string name = preset_cb.getCurrentItemText();
			string filename = presets.find(name,"");
			
			if(strlen(filename)) {
				
				if(load_preset(filename)) {
					new_preset_filename = filename;
					apply_hotkeys();
				}
				else {
					int to_remove = preset_cb.getCurrentItem();
					set_preset(presets[new_preset_filename]);
					remove_preset(to_remove,filename);
					if(preset_names.find(name) != -1) {
						standart_preset_loaded_count--;
					}
				}
			}
		}
		
		update_hotkeys();
	}
	
	void preset_clicked() {
		if(preset_cb.getNumItems() == 1) {
			preset_changed();
		}
	}
	
	/*
	 */
	void save_as_clicked() {
		
		string filename = basename(new_preset_filename);
		filename = extension(filename,NULL) + "_copy";
		
		if(dialogFilePreset(TR("Save Preset As"),filename,DIALOG_FILE_SAVE) == 0) return;
		
		save_preset(filename);
		load_preset(filename);
		add_preset(filename);
	}
	
	/*
	 */
	int apply_clicked() {
		
		if(is_hotkeys_changed()) {
			
			int need_select = 0;
			string filename = new_preset_filename;
			
			if(preset_names.find(presets.check(filename,"")) != -1 && edit_default_preset == false) {
				if(!dialogMessageYesNoRich(TR("Warning"),TR("This is the default preset that cannot be changed.<br>Do you want to save its copy with applied modifications?"))) return false;
				filename = extension(basename(filename),NULL) + "_copy";
				need_select = 1;
			}
			else if(filename == "") {
				need_select = 1;
			}
			
			if(need_select && dialogFilePreset(TR("Save Preset"),filename,DIALOG_FILE_SAVE) == 0) return 0;
			
			apply_preset();
			apply_hotkeys();
			save_preset(filename);
			load_preset(filename);
			add_preset(filename);
		}
		
		apply_preset();
		
		return true;
	}
	
	/*
	 */
	void ok_clicked() {
		if(apply_clicked()) hotkeysHide();
	}
	
	/*
	 */
	void cancel_clicked() {
		hotkeysHide();
		update_hotkeys();
	}
	
	/**************************************************************************\
	*
	* Plugins tab support functions
	*
	\**************************************************************************/
	
	/* generate key for plugin hotkey
	 */
	string get_plugin_hotkey_name(string plugin_name,string name) {
		return format("%s_%s",lower(replace(plugin_name," ","_")),lower(replace(name," ","_")));
	}
	
	/* recalculate width for plugin groups after change his content
	 */
	void fix_plugin_groups_width() {
		
		WidgetButton buttons[] = ();
		
		foreachkey(string key; plugin_hotkeys) {
			Hotkey h = get_hotkey_by_name(key);
			if(h != NULL) {
				buttons.append(h.button);
				h.button.setWidth(0);
				h.button.arrange();
			}
		}
		
		int max_button_width = 0;
		forloop(int i = 0; buttons.size()) {
			max_button_width = max(max_button_width,buttons[i].getWidth());
		}
		
		forloop(int i = 0; buttons.size()) {
			buttons[i].setWidth(max_button_width);
		}
	}
	
	/* check conflicts
	 */
	int check_plugin_hotkey_conflicts(int key,int modifier) {
		
		Hotkey key_hotkeys[0];
		get_hotkeys(key,0,0,modifier,key_hotkeys);
		
		return (key_hotkeys.size() != 0);
	}
	
	/* add new group in Plugins tab
	 */
	void add_plugin_group(string plugin_name) {
		
		if(plugin_groups.check(plugin_name)) return;
		
		Gui gui = window.getGui();
		
		tabbox.setTabHidden(plugin_tab,0);
		
		// create group header
		WidgetGridBox group_grid_box = new WidgetGridBox(gui,2,4,2);
		scroll_boxes[plugin_tab].addChild(group_grid_box,GUI_ALIGN_TOP);
		plugin_groups.append(plugin_name,group_grid_box);
		
		// hbox
		WidgetHBox hbox = new WidgetHBox(gui,0);
		group_grid_box.addChild(hbox,GUI_ALIGN_EXPAND);
		
		// left spacer
		hbox.addChild(new WidgetSpacer(gui),GUI_ALIGN_EXPAND);
		
		// label
		WidgetLabel label = new WidgetLabel(gui,TR(plugin_name));
		hbox.addChild(label,GUI_ALIGN_LEFT);
		
		// right spacer
		hbox.addChild(new WidgetSpacer(gui),GUI_ALIGN_EXPAND);
		
		group_grid_box.addChild(new WidgetVBox(gui));
		group_grid_box.setPadding(0,0,0,8);
	}
	
	/* remove group from Plugins tab
	 */
	void remove_plugin_group(string plugin_name) {
		
		if(!plugin_groups.check(plugin_name)) return;
		
		while(plugin_hotkeys.find(plugin_name,NULL) != NULL) {
			string hotkey_name = plugin_hotkeys.find(plugin_name,NULL);
			Hotkey h = get_hotkey_by_name(hotkey_name);
			if(h != NULL) {
				configSet("hotkey_" + h.name,format("%d %d %d %d",h.getKey(),h.getButton(),h.getWheel(),h.getModifier()));
				delete h;
				hotkeys.remove(hotkey_name);
			}
			plugin_hotkeys.remove(hotkey_name);
		}
		
		delete plugin_groups[plugin_name];
		plugin_groups.remove(plugin_name);
		
		if(!plugin_groups.size()) {
			if(tabbox.getCurrentTab() == plugin_tab)
				tabbox.setCurrentTab(plugin_tab - 1);
			tabbox.setTabHidden(plugin_tab,1);
		}
		
		fix_plugin_groups_width();
	}
	
	/* add hotkey into group on a Plugins tab
	 */
	void add_plugin_hotkey(string plugin_name,string name,string text,string tooltip,Handler handler) {
		
		if(!plugin_groups.check(plugin_name)) return;
		if(plugin_hotkeys.check(get_plugin_hotkey_name(plugin_name,name))) return;
		
		WidgetGridBox group_grid_box = plugin_groups[plugin_name];
		
		// check conflicts for default hotkey
		if(check_plugin_hotkey_conflicts(handler.key,handler.modifier)) {
			handler.setKey(KEY_NONE);
			handler.setModifier(KEY_NONE);
		}
		
		// save default values
		plugin_defaults.append(get_plugin_hotkey_name(plugin_name,name),new Handler(handler));
		
		// load from config if exists
		int value[0];
		string config_name = format("%d %d %d %d",handler.getKey(),handler.getButton(),handler.getWheel(),handler.getModifier());
		sscanf(configGet("hotkey_" + get_plugin_hotkey_name(plugin_name,name),config_name),"%d %d %d %d",value);
		handler.setKey(value[0]);
		handler.setButton(value[1]);
		handler.setWheel(value[2]);
		handler.setModifier(value[3]);
		
		// check conflicts for loaded hotkey
		if(check_plugin_hotkey_conflicts(handler.key,handler.modifier)) {
			handler.setKey(KEY_NONE);
			handler.setModifier(KEY_NONE);
		}
		
		// create hotkey
		string hotkey_name = get_plugin_hotkey_name(plugin_name,name);
		Hotkey hotkey = add_hotkey(Hotkey::ANY,hotkey_name,text,tooltip,plugin_tab,0);
		hotkey = set_hotkey_handler(hotkey_name,handler);
		plugin_hotkeys.append(get_plugin_hotkey_name(plugin_name,name),plugin_name);
		
		// key binding label
		WidgetLabel label = new WidgetLabel(engine.getGui(),TR(hotkey.text));
		label.setToolTip(TR(hotkey.tooltip));
		group_grid_box.addChild(label,GUI_ALIGN_LEFT);
		
		// key binding button
		WidgetButton button = new WidgetButton(engine.getGui(),hotkey.getText());
		button.setCallback(GUI_CLICKED,functionid(binding_clicked),hotkey.name);
		button.setToolTip(TR(hotkey.tooltip));
		group_grid_box.addChild(button,GUI_ALIGN_EXPAND);
		
		// assign hotkey widgets
		hotkey.button = button;
		hotkey.label = label;
		
		// find the max label width
		int max_label_width = 0;
		foreach(Hotkey hotkey; hotkeys) {
			max_label_width = max(max_label_width,hotkey.label.getWidth());
		}
		
		// assign max label width to all widgets
		foreach(Hotkey hotkey; hotkeys) {
			hotkey.label.setWidth(max_label_width);
		}
		
		fix_plugin_groups_width();
	}
	
	/* remove hotkey from group on a Plugins tab
	 */
	void remove_plugin_hotkey(string plugin_name,string name) {
		
		if(!plugin_groups.check(plugin_name)) return;
		
		string key = get_plugin_hotkey_name(plugin_name,name);
		if(plugin_hotkeys.check(key)) {
			Hotkey h = get_hotkey_by_name(key);
			if(h != NULL) {
				string config_name = format("%d %d %d %d",h.getKey(),h.getButton(),h.getWheel(),h.getModifier());
				configSet("hotkey_" + h.name,config_name);
				delete h;
				hotkeys.remove(key);
			}
			plugin_hotkeys.remove(key);
		}
		
		if(plugin_hotkeys.find(plugin_name,NULL) == NULL) {
			remove_plugin_group(plugin_name);
		}
		
		fix_plugin_groups_width();
	}
	
	/* remove all groups from Plugin tab
	 */
	void remove_plugins_hotkeys() {
		
		foreachkey(int group; plugin_groups) {
			remove_plugin_group(group);
		}
		
		plugin_defaults.delete();
	}
	
	
	/**************************************************************************\
	*
	* Init/update/shutdown
	*
	\**************************************************************************/
	
	/*
	 */
	void init_hotkeys() {
		
		string name = "editor/resources/hotkeys.rc";
		
		Xml xml = new Xml();
		if(xml.load(name) == 0 || xml.getName() != "hotkeys" || xml.getArg("version") < "1.0") {
			delete xml;
			return 0;
		}
		
		forloop(int c = 0; xml.getNumChilds()) {
			Xml tab = xml.getChild(c);
			
			string curent_tab_text = TR(tab.getArg("text"));
			int curent_tab = tab_text.find(curent_tab_text);
			if(curent_tab == -1) {
				tab_text.append(curent_tab_text);
				curent_tab = tab_text.size() - 1;
			}
			
			forloop(int g = 0; tab.getNumChilds()) {
				Xml group = tab.getChild(g);
				string curent_group_text = TR(group.getArg("text"));
				int curent_group = group_text.find(curent_group_text);
				if(curent_group == -1) {
					group_text.append(curent_group_text);
					curent_group = group_text.size() - 1;
				}
				
				forloop(int h = 0; group.getNumChilds()) {
					Xml hotkey = group.getChild(h);
					
					int type = Hotkey::ANY;
					string hotkey_type = hotkey.getArg("type","any");
					
					if(hotkey_type == "keyboard") type = Hotkey::KEYBOARD;
					else if(hotkey_type == "mouse") type = Hotkey::MOUSE;
					else if(hotkey_type == "mouse_wheel") type = Hotkey::MOUSE_WHEEL;
					else if(hotkey_type == "modifier") type = Hotkey::MODIFIER;
					
					int conflict_group = -1;
					if(hotkey.isArg("conflict_group")) {
						string current_conflict_group_text = hotkey.getArg("conflict_group");
						conflict_group = conflict_group_text.find(current_conflict_group_text);
						if(conflict_group == -1) {
							conflict_group_text.append(current_conflict_group_text);
							conflict_group = conflict_group_text.size() - 1;
						}
					}
					
					Hotkey current_hotkey = add_hotkey(type,
						hotkey.getArg("name",""),hotkey.getArg("text",""),hotkey.getArg("tooltip",""),
						curent_tab,curent_group,conflict_group
					);
					
					hotkeys_layout.append(current_hotkey);
					
					if(hotkey.isArg("no_conflict")) {
						strsplit(hotkey.getArg("no_conflict"),",",current_hotkey.no_conflict);
					}
				}
			}
		}
		
		xml.printUnusedData(name);
		delete xml;
		
		tab_text.append(TR("Plugins"));
		plugin_tab = tab_text.size() - 1;
		
		Handler handlers[] = (
			// undo/redo
			"undo"								:	new HandlerFunction(functionid(unredoUndo)),
			"redo"								:	new HandlerFunction(functionid(unredoRedo)),
			
			// common
			"system_menu"						:	new HandlerFunction(functionid(EditorMenu::system_menu_toggle)),
			"quick_save"						:	new HandlerConsole("state_save"),
			"quick_load"						:	new HandlerConsole("state_restore"),
			"other_screenshot"					:	new HandlerFunction(functionid(toolsScreenshot)),
			"other_stop_play"					:	new HandlerFunction(functionid(playToggle)),
			"other_light"						:	new HandlerFunction(functionid(toolsToggleLight)),
			"help"								:	new HandlerFunction(functionid(systemOpen),("https://developer.unigine.com/docs/tools/editor/")),
			
			// visualizer
			"fps"								:	new HandlerConsole("toggle show_fps"),
			"messages"							:	new HandlerConsole("toggle show_messages"),
			"profiler"							:	new HandlerConsole("switch show_profiler"),
			"visualizer"						:	new HandlerConsole("toggle show_visualizer"),
			"object_wireframe"					:	new HandlerFunction(functionid(helpersToggleObjectWireframe)),
			"object_bounds"						:	new HandlerFunction(functionid(helpersToggleObjectBounds)),
			"mesh_bones"						:	new HandlerFunction(functionid(helpersToggleMeshBones)),
			"scene_wireframe"					:	new HandlerFunction(functionid(panelRenderSetMode),("wireframe")),
			"ambient_mode"						:	new HandlerFunction(functionid(panelRenderSetMode),("unlit")),
			"mipmaps"							:	new HandlerFunction(functionid(panelRenderSetMode),("mipmap")),
			"normals"							:	new HandlerFunction(functionid(panelRenderSetMode),("gray_materials")),
			"final_image"						:	new HandlerFunction(functionid(panelRenderSetMode),("final")),
			"toggle_render_mode"				:	new HandlerFunction(functionid(panelRenderToggleRenderMode)),
			"textures"							:	new HandlerConsole("toggle render_show_textures"),
			"world_objects"						:	new HandlerConsole("toggle world_show_sectors && toggle world_show_portals && toggle world_show_occluders && toggle world_show_triggers"),
			"physics_objects"					:	new HandlerConsole("toggle physics_show_shapes && toggle physics_show_joints && toggle physics_show_contacts"),
			"deferred_depth"					:	new HandlerFunction(functionid(panelRenderSetBuffer),("post_deferred_depth")),
			"deferred_color"					:	new HandlerFunction(functionid(panelRenderSetBuffer),("post_deferred_color")),
			"deferred_normal"					:	new HandlerFunction(functionid(panelRenderSetBuffer),("post_deferred_normal")),
			
			// brushes
			"brushes_apply_brush"				:	new HandlerDummy(),
			"brushes_get_brush_value"			:	new HandlerDummy(),
			"brushes_increment_brush_radius"	:	new HandlerFunction(functionid(brushesIncRadius)),
			"brushes_decrement_brush_radius"	:	new HandlerFunction(functionid(brushesDecRadius)),
			
			// windows
			"panel_nodes"						:	new HandlerFunction(functionid(nodesToggle)),
			"panel_materials"					:	new HandlerFunction(functionid(materialsToggle)),
			"panel_properties"					:	new HandlerFunction(functionid(propertiesToggle)),
			"panel_render"						:	new HandlerFunction(functionid(renderToggle)),
			"panel_sound"						:	new HandlerFunction(functionid(soundToggle)),
			"panel_physics"						:	new HandlerFunction(functionid(physicsToggle)),
			"panel_tools"						:	new HandlerFunction(functionid(toolsToggle)),
			"panel_keys"						:	new HandlerFunction(functionid(hotkeysToggle)),
			"panel_plugins"						:	new HandlerFunction(functionid(pluginsToggle)),
			
			// editor
			"reload"							:	new HandlerConsole("editor_reload"),
			"quit"								:	new HandlerFunctionThread(functionid(EditorMenu::editor_quit)),
			
			// world
			"create_world"						:	new HandlerFunctionThread(functionid(EditorMenu::create_world)),
			"open_world"						:	new HandlerFunctionThread(functionid(EditorMenu::open_world)),
			"save_world"						:	new HandlerFunctionThread(functionid(EditorMenu::save_world)),
			"reload_world"						:	new HandlerConsole("world_reload"),
			
			// selection
			"select"							:	new HandlerDummy(),
			"rectangle_select"					:	new HandlerDummy(),
			"add_selection"						:	new HandlerDummy(),
			"remove_selection"					:	new HandlerDummy(),
			"invert_selection"					:	new HandlerFunction(functionid(nodesInvertSelection)),
			"select_all"						:	new HandlerFunction(functionid(nodesSelectAll)),
			"reset_selection"					:	new HandlerFunction(functionid(nodesDeselectNode)),
			
			// navigation
			"move_forward"						:	new HandlerDummy(),
			"move_backward"						:	new HandlerDummy(),
			"move_left"							:	new HandlerDummy(),
			"move_right"						:	new HandlerDummy(),
			"move_crouch"						:	new HandlerDummy(),
			"move_jump"							:	new HandlerDummy(),
			"move_run"							:	new HandlerDummy(),
			"inclination_right"					:	new HandlerDummy(),
			"inclination_left"					:	new HandlerDummy(),
			"change_direction"					:	new HandlerDummy(),
			"dolly"								:	new HandlerDummy(),
			"track"								:	new HandlerDummy(),
			"orbit"								:	new HandlerDummy(),
			"zoom_in"							:	new HandlerDummy(),
			"zoom_out"							:	new HandlerDummy(),
			"nodes_focus"						:	new HandlerFunction(functionid(controlsShowNode)),
			"other_camera"						:	new HandlerFunction(functionid(controlsResetOffset)),
			"camera_speed_switch"				:	new HandlerFunction(functionid(panelsSwitchCameraSpeed)),
			"camera_speed_1"					:	new HandlerFunction(functionid(panelsSetCameraSpeed),(0)),
			"camera_speed_2"					:	new HandlerFunction(functionid(panelsSetCameraSpeed),(1)),
			"camera_speed_3"					:	new HandlerFunction(functionid(panelsSetCameraSpeed),(2)),
			
			// nodes
			"isolate_nodes"						:	new HandlerFunctionThread(functionid(nodesToggleIsolation)),
			"nodes_clone"						:	new HandlerFunctionThread(functionid(nodesCloneNode)),
			"nodes_delete"						:	new HandlerFunctionThread(functionid(nodesDeleteNode)),
			"nodes_info"						:	new HandlerFunctionThread(functionid(helpersToggleNodeInfo)),
			"clone_node"						:	new HandlerDummy(),
			"move_without_children"				:	new HandlerDummy(),
			"open_context_menu"					:	new HandlerDummy(),
			"group_selected"					:	new HandlerDummy(),
			"convert_into_node_reference"		:	new HandlerFunctionThread(functionid(nodesConvertIntoNodeReference)),
			
			// placement tools
			"nodes_place_here"					:	new HandlerFunctionThread(functionid(nodesPlaceHere)),
			"nodes_position_snap"				:	new HandlerFunctionThread(functionid(panelPlacementTogglePositionSnap)),
			"nodes_rotation_snap"				:	new HandlerFunctionThread(functionid(panelPlacementToggleRotationSnap)),
			"nodes_place_manually"				:	new HandlerFunctionThread(functionid(nodesPlaceManually)),
			"nodes_drop_down"					:	new HandlerFunction(functionid(panelPlacementDropDown)),
			"place_node_relative_transform"		:	new HandlerDummy(),
			"place_node_offset_reset"			:	new HandlerDummy(),
			"place_node_decrease_offset"		:	new HandlerDummy(),
			"place_node_increase_offset"		:	new HandlerDummy(),
			"place_node_success"				:	new HandlerDummy(),
			"place_node_cancel"					:	new HandlerDummy(),
			
			// manipulators
			"manipulator_selector"				:	new HandlerFunction(functionid(panelPlacementSetManipulator),("selector")),
			"manipulator_translator"			:	new HandlerFunction(functionid(panelPlacementSetManipulator),("translator")),
			"manipulator_rotator"				:	new HandlerFunction(functionid(panelPlacementSetManipulator),("rotator")),
			"manipulator_scaler"				:	new HandlerFunction(functionid(panelPlacementSetManipulator),("scaler")),
			"manipulator_pivot"					:	new HandlerFunction(functionid(panelPlacementToggleManipulatorPivot)),
			"manipulator_basis"					:	new HandlerFunction(functionid(panelPlacementToggleManipulatorBasis)),
			
			// helpers
			"helpers_nodes"						:	new HandlerFunction(functionid(panelHelpersToggleIconGizmo),("nodes")),
			"helpers_objects"					:	new HandlerFunction(functionid(panelHelpersToggleIconGizmo),("objects")),
			"helpers_effects"					:	new HandlerFunction(functionid(panelHelpersToggleIconGizmo),("effects")),
			"helpers_lights"					:	new HandlerFunction(functionid(panelHelpersToggleIconGizmo),("lights")),
			"helpers_decals"					:	new HandlerFunction(functionid(panelHelpersToggleIconGizmo),("decals")),
			"helpers_worlds"					:	new HandlerFunction(functionid(panelHelpersToggleIconGizmo),("worlds")),
			"helpers_sounds"					:	new HandlerFunction(functionid(panelHelpersToggleIconGizmo),("worlds")),
			"helpers_paths"						:	new HandlerFunction(functionid(panelHelpersToggleIconGizmo),("paths")),
			"helpers_players"					:	new HandlerFunction(functionid(panelHelpersToggleIconGizmo),("players")),
		);
		
		foreachkey(string key; handlers) {
			set_hotkey_handler(key,handlers[key]);
			if(handlers[key] is HandlerDummy) {
				Hotkey hotkey = get_hotkey_by_name(key);
				if(hotkey != NULL) hotkey.setSkipWidgetTypes((WIDGET_EDITTEXT,
					WIDGET_EDITLINE,WIDGET_SLIDER,WIDGET_TREEBOX,WIDGET_LISTBOX,
					WIDGET_SCROLLBOX,WIDGET_SCROLL,WIDGET_DIALOG,WIDGET_COMBOBOX)
				);
			}
		}
		
		get_hotkey_by_name("group_selected").removeSkipWidgetType(WIDGET_TREEBOX);
		get_hotkey_by_name("select_all").setSkipWidgetTypes((WIDGET_EDITTEXT,
					WIDGET_EDITLINE,WIDGET_LISTBOX));
		get_hotkey_by_name("reset_selection").setSkipWidgetTypes((WIDGET_EDITTEXT,
					WIDGET_EDITLINE,WIDGET_LISTBOX,));
	}
	
	/*
	 */
	void init_presets() {
		
		string path = "editor/resources/presets/";
		Xml xml = new Xml();
		
		forloop(int i = 0; preset_names.size()) {
			string filename = format("%s%s.preset",path,lower(replace(preset_names[i]," ","_")));
			if(xml.load(savePath(filename)) == 0) continue;
			standart_preset_loaded_count++;
			presets.append(filename,preset_names[i]);
			preset_cb.addItem(preset_names[i]);
		}
		
		int custom_presets_count = configGet("hotkeys_preset_custom_count",0);
		for(int i = 0; i < custom_presets_count; i++) {
			string custom_preset_filename = configGet(format("hotkeys_preset_custom_%d",i),"");
			if(xml.load(savePath(custom_preset_filename)) == 0) continue;
			presets.append(custom_preset_filename,basename(custom_preset_filename));
			preset_cb.addItem(basename(custom_preset_filename));
		}
		
		new_preset_filename = "";
		current_preset_filename = "";
		if(preset_cb.getNumItems() == 0) {
			preset_cb.addItem(TR("Load Custom..."));
			return;
		}
		
		current_preset_filename = configGet("hotkeys_preset",presets.find(preset_cb.getItemText(0),""));
		
		if(!presets.check(current_preset_filename)) {
			if(is_file(savePath(current_preset_filename))) {
				string name = basename(current_preset_filename);
				presets.append(current_preset_filename,name);
				preset_cb.addItem(name);
			} else {
				current_preset_filename = presets.find(preset_cb.getItemText(0),"");
			}
		}
		
		preset_cb.addItem(TR("Load Custom..."));
		
		load_preset(current_preset_filename);
		apply_hotkeys();
		set_preset(presets[current_preset_filename]);
		
		new_preset_filename = current_preset_filename;
		
		apply_preset();
	}
	
	/*
	 */
	void create_hotkeys_gui() {
		
		Gui gui = window.getGui();
		scroll_boxes.resize(tab_text.size());
		
		// create key binding gui elements
		forloop(int i = 0; tab_text.size()) {
			
			// create new tab
			tabbox.addTab(TR(tab_text[i]));
			
			// scroll box
			WidgetScrollBox scroll_box = new WidgetScrollBox(gui,8,8);
			scroll_box.setHScrollEnabled(0);
			scroll_box.setBorder(0);
			tabbox.addChild(scroll_box,GUI_ALIGN_EXPAND);
			scroll_boxes[i] = scroll_box;
			
			// grid box
			WidgetGridBox grid_box = new WidgetGridBox(gui,2,4,2);
			scroll_box.addChild(grid_box,GUI_ALIGN_TOP);
			int tab_hotkey_count = 0;
			
			// groups
			forloop(int j = 0; group_text.size()) {
				
				int group_created = 0;
				
				// hotkeys
				foreach(Hotkey hotkey; hotkeys_layout) {
					
					if(hotkey.tab != i) continue;
					if(hotkey.group != j) continue;
					
					// create group header if it still not created
					if(group_created == 0) {
						
						// hbox
						WidgetHBox hbox = new WidgetHBox(gui,0);
						grid_box.addChild(hbox,GUI_ALIGN_EXPAND);
						
						// left spacer
						hbox.addChild(new WidgetSpacer(gui),GUI_ALIGN_EXPAND);
						
						// label
						WidgetLabel label = new WidgetLabel(gui,TR(group_text[j]));
						hbox.addChild(label,GUI_ALIGN_LEFT);
						
						// right spacer
						hbox.addChild(new WidgetSpacer(gui),GUI_ALIGN_EXPAND);
						
						grid_box.addChild(new WidgetVBox(gui));
						group_created = 1;
					}
					
					// key binding label
					WidgetLabel label = new WidgetLabel(engine.getGui(),hotkey.text);
					label.setToolTip(hotkey.tooltip);
					grid_box.addChild(label,GUI_ALIGN_LEFT);
					
					// key binding button
					WidgetButton button = new WidgetButton(engine.getGui(),hotkey.getText());
					button.setCallback(GUI_CLICKED,functionid(binding_clicked),hotkey.name);
					button.setToolTip(hotkey.tooltip);
					grid_box.addChild(button,GUI_ALIGN_EXPAND);
					
					// assign hotkey widgets
					hotkey.button = button;
					hotkey.label = label;
					tab_hotkey_count++;
				}
				
				// if group was created add some space after it
				if(group_created) {
					grid_box.addChild(new WidgetHBox(gui,4,4),GUI_ALIGN_EXPAND);
					grid_box.addChild(new WidgetHBox(gui,4,4),GUI_ALIGN_EXPAND);
				}
			}
			if(tab_hotkey_count == 0) tabbox.setTabHidden(i,1);
		}
		
		tabbox.arrange();
		
		// find the max label width
		int max_label_width = 0;
		foreach(Hotkey hotkey; hotkeys) {
			max_label_width = max(max_label_width,hotkey.label.getWidth());
		}
		
		// assign max label width to all widgets
		foreach(Hotkey hotkey; hotkeys) {
			hotkey.label.setWidth(max_label_width);
		}
	}
	
	/*
	 */
	void enable_callbacks() {
		engine.gui.setKeyPressCallback(functionid(key_press_callback_gui));
		engine.controls.setKeyPressCallback(functionid(key_press_callback_controls));
		engine.controls.setKeyReleaseCallback(functionid(key_release_callback_controls));
		engine.controls.setButtonPressCallback(functionid(button_press_callback_controls));
		mouse_wheel_callback_enabled = true;
	}
	
	/*
	 */
	void disable_callbacks() {
		engine.gui.setKeyPressCallback(NULL);
		engine.controls.setKeyPressCallback(NULL);
		engine.controls.setKeyReleaseCallback(NULL);
		engine.controls.setButtonPressCallback(NULL);
		mouse_wheel_callback_enabled = false;
	}
	
	/*
	 */
	void enable_system_script_bindings() {
		engine.controls.setStateKey(CONTROLS_STATE_SAVE,state_save);
		engine.controls.setStateKey(CONTROLS_STATE_RESTORE,state_restore);
		engine.controls.setStateKey(CONTROLS_STATE_SCREENSHOT,video_grab);
		engine.system.call("systemSetToggle",system_menu_toggle);
	}
	
	/*
	 */
	void disable_system_script_bindings() {
		state_save = engine.controls.getStateKey(CONTROLS_STATE_SAVE);
		state_restore = engine.controls.getStateKey(CONTROLS_STATE_RESTORE);
		video_grab = engine.controls.getStateKey(CONTROLS_STATE_SCREENSHOT);
		system_menu_toggle = engine.system.call("systemGetToggle");
		engine.system.call("systemSetToggle",-1);
		engine.controls.setStateKey(CONTROLS_STATE_SAVE,0);
		engine.controls.setStateKey(CONTROLS_STATE_RESTORE,0);
		engine.controls.setStateKey(CONTROLS_STATE_SCREENSHOT,0);
		
	}
	
	void toggle_default_preset_edit_mode() {
		edit_default_preset = !edit_default_preset;
		log.message("edit_mode: %d\n",edit_default_preset);
	}
	
	/*
	 */
	void init() {
		
		#ifndef NDEBUG
			engine.console.addCommand("default_presets_edit_mode","default_presets_edit_mode","Hotkeys::toggle_default_preset_edit_mode");
		#endif
		
		disable_system_script_bindings();
		
		mode = MODE_NORMAL;
		
		init_hotkeys();
		create_hotkeys_gui();
		init_presets();
		
		apply_hotkeys();
		enable_callbacks();
		
		// window
		editorWindow(window);
		DECLARE_WINDOW_LOAD(hotkeys)
		
		int active_tab = configGet("hotkeys_tab",0);
		if(active_tab >= tabbox.getNumTabs()) active_tab = tabbox.getNumTabs() - 1;
		tabbox.setCurrentTab(active_tab);
	}
	
	void shutdown() {
		
		DECLARE_WINDOW_SAVE(hotkeys)
		
		configSet("hotkeys_tab",tabbox.getCurrentTab());
		configSet("hotkeys_preset",current_preset_filename);
		
		preset_cb.removeItem(preset_cb.getNumItems() - 1);
		
		int custom_preset_num = 0;
		for(int i = standart_preset_loaded_count; i < preset_cb.getNumItems(); i++) {
			configSet(format("hotkeys_preset_custom_%d",custom_preset_num),presets.find(preset_cb.getItemText(i)));
			custom_preset_num++;
		}
		configSet("hotkeys_preset_custom_count",custom_preset_num);
		
		remove_plugins_hotkeys();
		
		engine.gui.setKeyPressCallback(NULL);
		
		enable_system_script_bindings();
	}
	
	/*
	 */
	void update() {
		
		if(viewportsGetFocusedWindow() != last_focused_window) {
			last_focused_window = viewportsGetFocusedWindow();
			recover_modifiers();
		}
		
		last_modifier = get_modifier_state();
		
		// update mouse wheel state
		if(mouse_wheel_callback_enabled) mouse_wheel_changed();
		
		// update current tab hotkey text
		if(window_show) {
			
			foreach(Hotkey hotkey; hotkeys) {
				if(hotkey.tab != tabbox.getCurrentTab()) continue;
				hotkey.button.setText(hotkey.getText());
			}
			
			// apply button enable
			int is_changed = is_hotkeys_changed();
			if(apply_b.isEnabled() != is_changed) {
				apply_b.setEnabled(is_changed);
				tabbox.setFocus();
			}
		}
	}
	
	/*
	 */
	void update_hotkeys() {
		menuUpdateItems();
		panelBrushUpdateHotkey();
		panelRenderUpdateItemsHotkeys();
		aboutCheckKey();
	}
	
	/*
	 */
	void recover_modifiers() {
		if(last_modifier & KEY_SHIFT) engine.app.setKeyState(APP_KEY_SHIFT,1);
		if(last_modifier & KEY_CTRL) engine.app.setKeyState(APP_KEY_CTRL,1);
		if(last_modifier & KEY_ALT) engine.app.setKeyState(APP_KEY_ALT,1);
		if(last_modifier & KEY_CMD) engine.app.setKeyState(APP_KEY_CMD,1);
	}
	
} /* namespace Hotkeys */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void hotkeysInit() {
	Hotkeys::init();
}

void hotkeysShutdown() {
	Hotkeys::shutdown();
}

/*
 */
void hotkeysToggle() {
	if(Hotkeys::window_show) hotkeysHide();
	else hotkeysShow();
}

void hotkeysShow() {
	DECLARE_WINDOW_SHOW(Hotkeys)
}

void hotkeysHide() {
	Hotkeys::cancel_preset();
	Hotkeys::cancel_hotkeys();
	DECLARE_WINDOW_HIDE(Hotkeys)
}

/*
 */
void hotkeysUpdate(int need_reload) {
	Hotkeys::update();
}

/*
 */
void hotkeysSkipAll(int mode) {
	if(mode) Hotkeys::skip_mode = Hotkeys::SKIP_CALLBACKS | Hotkeys::SKIP_UPDATES;
	else Hotkeys::skip_mode = 0;
}

/*
 */
void hotkeysSkipCallbacks(int mode) {
	if(mode) Hotkeys::skip_mode |= Hotkeys::SKIP_CALLBACKS;
	else Hotkeys::skip_mode &= ~Hotkeys::SKIP_CALLBACKS;
}

/*
 */
int hotkeysIsSkipCallbacks() {
	return (Hotkeys::skip_mode & Hotkeys::SKIP_CALLBACKS);
}

/*
 */
void hotkeysSkipUpdates(int mode) {
	if(mode) Hotkeys::skip_mode |= Hotkeys::SKIP_UPDATES;
	else Hotkeys::skip_mode &= ~Hotkeys::SKIP_UPDATES;
}

/*
 */
int hotkeysIsSkipUpdates() {
	return (Hotkeys::skip_mode & Hotkeys::SKIP_UPDATES);
}

/*
 */
string hotkeysGetText(string name) {
	Hotkeys::Hotkey hotkey = Hotkeys::get_hotkey_by_name(name);
	if(hotkey == NULL) return "";
	return hotkey.getText();
}

/*
 */
int hotkeysCheckName(string name) {
	return Hotkeys::hotkeys.check(name);
}

/*
 */
int hotkeysGetState(string name,int modifier_require = 1) {
	Hotkeys::Hotkey hotkey = Hotkeys::get_hotkey_by_name(name);
	if(hotkey == NULL || Hotkeys::check_priority_hotkeys(hotkey)) return false;
	
	return hotkey.getState(modifier_require);
}

/*
 */
int hotkeysClearState(string name,int modifier_require = 1) {
	Hotkeys::Hotkey hotkey = Hotkeys::get_hotkey_by_name(name);
	if(hotkey == NULL || Hotkeys::check_priority_hotkeys(hotkey)) return false;
	return hotkey.clearState(modifier_require);
}

/*
 */
int hotkeysGetModifierState(string name) {
	Hotkeys::Hotkey hotkey = Hotkeys::get_hotkey_by_name(name);
	if(hotkey == NULL || Hotkeys::check_priority_hotkeys(hotkey)) return false;
	return hotkey.getModifierState() && hotkey.getModifier() != Hotkeys::KEY_NONE;
}

/*
 */
int hotkeysClearModifierState(string name) { 
	Hotkeys::Hotkey hotkey = Hotkeys::get_hotkey_by_name(name);
	if(hotkey == NULL || Hotkeys::check_priority_hotkeys(hotkey)) return false;
	return hotkey.clearModifierState() && hotkey.getModifier() != Hotkeys::KEY_NONE;
}

/*
 */
void hotkeysSetSkipModifier(string name = NULL) {
	Hotkeys::skip_modifier = 0;
	if(strlen(name)) {
		Hotkeys::Hotkey modifier = Hotkeys::get_hotkey_by_name(name);
		Hotkeys::skip_modifier = modifier.getModifier();
	}
}

/*
 */
void hotkeysAddPriorityHotkey(string name) {
	if(Hotkeys::priority_hotkeys.find(name) != -1) return;
	Hotkeys::priority_hotkeys.append(name);
}

/*
 */
void hotkeysRemovePriorityHotkey(string name) {
	int num = Hotkeys::priority_hotkeys.find(name);
	if(num != -1) Hotkeys::priority_hotkeys.remove(num);
}

/*
 */
void hotkeysSetPriorityHotkeys(string names[]) {
	Hotkeys::priority_hotkeys.clear();
	forloop(int i = 0; names.size()) {
		hotkeysAddPriorityHotkey(names[i]);
	}
}

/*
 */
void hotkeysResetPriorityHotkeys() {
	Hotkeys::priority_hotkeys.clear();
}

/* find in ui for widgets with templated tooltips
 */
void hotkeysAddUiToolTips(UserInterface ui) {
	Hotkeys::WidgetsManager::addUiToolTips(ui);
}

/*
 */
void hotkeysRemoveUiToolTips(UserInterface ui) {
	Hotkeys::WidgetsManager::removeUiToolTips(ui);
}

/*
 */
void hotkeysAddWidgetToolTip(string name,Widget w) {
	Hotkeys::WidgetsManager::addHotkeyNameToWidget(w,name);
	Hotkeys::WidgetsManager::addToolTipWidget(w);
}

/*
 */
void hotkeysAddWidgetToolTip(Widget w) {
	Hotkeys::WidgetsManager::addToolTipWidget(w);
}

/*
 */
void hotkeysRemoveWidgetToolTip(Widget w) {
	Hotkeys::WidgetsManager::removeToolTipWidget(w);
}

int hotkeysIsKeyBinded(int key) {
	return Hotkeys::is_key_binded(key);
}

int hotkeysIsKeyBinded(int key,string exclude_groups[]) {
	return Hotkeys::is_key_binded(key,exclude_groups);
}

/* add hotkey into group on a Plugins tab
 */
void hotkeysAddPluginHotkey(string plugin_name,string name,string text,string tooltip,Hotkeys::Handler handler) {
	Hotkeys::add_plugin_group(plugin_name);
	Hotkeys::add_plugin_hotkey(plugin_name,name,text,tooltip,handler);
}

/* remove hotkey from group on a Plugins tab
 */
void hotkeysRemovePluginHotkey(string plugin_name,string name) {
	Hotkeys::remove_plugin_hotkey(plugin_name,name);
}

/* remove group on a Plugins tab
 */
void hotkeysRemovePlugin(string plugin_name) {
	Hotkeys::remove_plugin_group(plugin_name);
}

/*
 */
void hotkeysRecoverModifiers() {
	Hotkeys::recover_modifiers();
}
