/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_widget_dialog_file.h
 * Desc:    Unigine editor
 * Version: 1.07
 * Author:  Andrey Viktorov <unclebob@unigine.com>
 *          Vladimir Borovskikh <futurist@unigine.com>
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

#ifndef __EDITOR_WIDGET_DIALOG_FILE_H__
#define __EDITOR_WIDGET_DIALOG_FILE_H__

#include <core/systems/widgets/widget.h>
#include <core/systems/widgets/widget_dialog.h>

/*
 */
namespace Editor::Widgets {
	
	/**************************************************************************\
	*
	* EditorDialogFileTableItem
	*
	\**************************************************************************/
	
	/*
	 */
	class EditorDialogFileTableItem {
		
		int icon_polygon_id;
		int name_text_id;
		int size_text_id;
		int mtime_text_id;
		
		WidgetCanvas canvas;
		Image texture;
		
		int texture_width;
		int	texture_height;
		
		int icon;
		int size;
		
		int position_x;
		int position_y;
		
		string name_text;
		int name_width;
		int have_preview;
		
		int offsets[] = (
			205,
			85,
		);
		
		//
		EditorDialogFileTableItem(WidgetCanvas c,Image t) {
			canvas = c;
			texture = t;
			
			icon_polygon_id = canvas.addPolygon();
			name_text_id = canvas.addText();
			size_text_id = canvas.addText();
			mtime_text_id = canvas.addText();
			
			texture_width = texture.getWidth();
			texture_height = texture.getHeight();
			
			have_preview = 0;
			
			canvas.setPolygonImage(icon_polygon_id,texture);
		}
		
		~EditorDialogFileTableItem() {
			if(canvas != NULL) {
				if(icon_polygon_id != -1) canvas.removePolygon(icon_polygon_id);
				if(name_text_id != -1) canvas.removeText(name_text_id);
				if(size_text_id != -1) canvas.removeText(size_text_id);
				if(mtime_text_id != -1) canvas.removeText(mtime_text_id);
				
				icon_polygon_id = -1;
				name_text_id = -1;
				size_text_id = -1;
				mtime_text_id = -1;
			}
			
			canvas = NULL;
		}
		
		//
		void setData(int i,string n,int s,string mtime) {
			if(canvas == NULL) return;
			size = s;
			canvas.setTextText(name_text_id,n);
			if(size == -1) canvas.setTextText(size_text_id," ");
			else canvas.setTextText(size_text_id,memory(size));
			canvas.setTextText(mtime_text_id,mtime);
			
			icon = i;
			name_text = n;
			name_width = canvas.getTextWidth(name_text_id);
			have_preview = 0;
			
			int wrap_width = canvas.getWidth() - offsets[0] - texture_width * 2;
			wrap_text(name_text,name_text_id,wrap_width);
		}
		
		//
		void setNameText(string text) { 
			name_text = text;
			canvas.setTextText(name_text_id,text);
			have_preview = 0;
		}
		
		string getNameText() { return name_text; }
		int getNameTextWidth() { return name_width; }
		
		//
		void setSize(int s) {
			size = s;
			if(size == -1) setSizeText(" ");
			else setSizeText(memory(size));
		}
		int getSize() { return size; }
		
		//
		void setSizeText(string text) { canvas.setTextText(size_text_id,text); }
		string getSizeText() { return canvas.getTextText(size_text_id); }
		int getSizeTextWidth() { return canvas.getTextWidth(size_text_id); }
		
		//
		void setMTimeText(string text) { canvas.setTextText(mtime_text_id,text); }
		string getMTimeText() { return canvas.getTextText(mtime_text_id); }
		int getMTimeTextWidth() { return canvas.getTextWidth(mtime_text_id); }
		
		//
		void setPositionX(int value) { position_x = value; }
		int getPositionX() { return position_x; }
		
		void setPositionY(int value) { position_y = value; }
		int getPositionY() { return position_y; }
		
		//
		void update(string path) {
			string full_path = path + name_text;
			
			if(is_dir(full_path)) return;
			if(!is_file(full_path)) return;
			
			if(thumbnailsIsAvailable(full_path) == 0) return;
			
			Image image = thumbnailsGetImage(full_path);
			
			if(image == NULL) {
				have_preview = 0;
				canvas.setPolygonImage(icon_polygon_id,texture);
				arrange();
				
				return;
			}
			
			canvas.setPolygonImage(icon_polygon_id,image);
			have_preview = 1;
			
			arrange();
		}
		
		//
		void arrange() {
			
			// icon bounds
			vec3 p0 = vec3(position_x,position_y,0.0f);
			vec3 p1 = vec3(position_x + texture_width,position_y,0.0f);
			vec3 p2 = vec3(position_x + texture_width,position_y + texture_width,0.0f);
			vec3 p3 = vec3(position_x,position_y + texture_width,0.0f);
			
			vec3 t0 = vec3(0.0f,0.0f,0.0f);
			vec3 t1 = vec3(1.0f,0.0f,0.0f);
			vec3 t2 = vec3(1.0f,1.0f,0.0f);
			vec3 t3 = vec3(0.0f,1.0f,0.0f);
			
			if(have_preview == 0) {
				t0 = vec3(0.0f,icon * texture_width / float(texture_height),0.0f);
				t1 = vec3(1.0f,icon * texture_width / float(texture_height),0.0f);
				t2 = vec3(1.0f,(icon + 1) * texture_width / float(texture_height),0.0f);
				t3 = vec3(0.0f,(icon + 1) * texture_width / float(texture_height),0.0f);
			}
			
			// icon polygon
			canvas.clearPolygonPoints(icon_polygon_id);
			canvas.addPolygonPoint(icon_polygon_id,p0);
			canvas.setPolygonTexCoord(icon_polygon_id,t0);
			
			canvas.addPolygonPoint(icon_polygon_id,p1);
			canvas.setPolygonTexCoord(icon_polygon_id,t1);
			
			canvas.addPolygonPoint(icon_polygon_id,p2);
			canvas.setPolygonTexCoord(icon_polygon_id,t2);
			
			canvas.addPolygonPoint(icon_polygon_id,p3);
			canvas.setPolygonTexCoord(icon_polygon_id,t3);
			
			// texts
			int ids[] = (
				mtime_text_id,
				size_text_id,
			);
			
			int offset_x = texture_width + 4;
			int offset_y = -1;
			
			int wrap_width = canvas.getWidth() - offsets[0] - texture_width * 2;
			
			canvas.setTextPosition(name_text_id,vec3(position_x + offset_x,position_y + offset_y,0.0f));
			
			forloop(int i = 0; offsets.size()) {
				offset_x = canvas.getWidth() - offsets[i];
				canvas.setTextPosition(ids[i],vec3(position_x + offset_x,position_y + offset_y,0.0f));
			}
			
			wrap_text(name_text,name_text_id,wrap_width);
		}
		
		//
		void wrap_text(string source,int id,int max_width) {
			canvas.setTextText(id,source);
			name_width = canvas.getTextWidth(id);
			
			if(name_width < max_width) return;
			
			int size = utf8strlen(source);
			
			int left = 0;
			int right = size;
			
			while(abs(left - right) > 1) {
				int current = (left + right) / 2;
				
				canvas.setTextText(id,utf8substr(source,0,current) + "...");
				if(canvas.getTextWidth(id) > max_width) right = current;
				else left = current;
			}
		}
	};
	
	/**************************************************************************\
	*
	* EditorDialogFileTable
	*
	\**************************************************************************/
	
	/*
	 */
	class EditorDialogFileTable {
		
		//
		enum {
			TABLE_SPACING = 2,
		};
		
		EditorDialogFileTableItem items[0];
		WidgetCanvas canvas;
		WidgetScrollBox scroll;
		
		int selection_polygon_id;
		int selected_item_id;
		
		int icon_size;
		int canvas_width;
		
		Image texture;
		
		//
		EditorDialogFileTable(WidgetCanvas c,WidgetScrollBox s) {
			texture = new Image("core/gui/dialog_file_list.png");
			icon_size = texture.getWidth();
			
			canvas = c;
			scroll = s;
			
			canvas_width = -1;
			
			selection_polygon_id = canvas.addPolygon();
			selected_item_id = -1;
			
			vec4 selection_color = vec4(0.2f,0.2f,0.2f,1.0f);
			
			canvas.setPolygonOrder(selection_polygon_id,-1);
			canvas.setPolygonColor(selection_polygon_id,selection_color);
		}
		
		~EditorDialogFileTable() {
			clear();
			
			if(canvas != NULL) {
				if(selection_polygon_id != -1) canvas.removePolygon(selection_polygon_id);
				
				selection_polygon_id = -1;
			}
			
			canvas = NULL;
			scroll = NULL;
			
			delete texture;
		}
		
		//
		void clear() {
			items.delete();
			selected_item_id = -1;
			canvas_width = -1;
			
			canvas.clearPolygonPoints(selection_polygon_id);
		}
		
		int addItem(int icon,string name,int size,string mtime) {
			canvas.setHeight(icon_size * (items.size() + 1));
			
			EditorDialogFileTableItem item = new EditorDialogFileTableItem(canvas,texture);
			item.setData(icon,name,size,mtime);
			item.setPositionY(icon_size * items.size());
			item.setPositionX(TABLE_SPACING);
			
			items.append(item);
			return items.size() - 1;
		}
		
		void setItemData(int index,int icon,string name,int size,string mtime) {
			if(!items.check(index)) return;
			items[index].setData(icon,name,size,mtime);
		}
		
		EditorDialogFileTableItem getItem(int index) {
			if(!items.check(index)) return;
			return items[index];
		}
		
		//
		int getNumItems() { return items.size(); }
		
		int findItem(string name) {
			forloop(int i = 0; items.size()) {
				if(items[i].getNameText() == name) return i;
			}
			
			return -1;
		}
		
		//
		void setCurrentItem(int id,int scroll_to_selection = 1) {
			assert(id >= 0 && id < items.size() && "EditorDialogFileTable::setCurrentItem(): bad item index");
			selected_item_id = id;
			update_selection_polygon(scroll_to_selection);
		}
		
		int getCurrentItem() {
			return selected_item_id;
		}
		
		void resetSelection() {
			canvas.clearPolygonPoints(selection_polygon_id);
			selected_item_id = -1;
		}
		
		//
		int getItemPositionX(int id) {
			assert(id >= 0 && id < items.size() && "EditorDialogFileTable::getItemPositionX(): bad item index");
			return items[id].getPositionX();
		}
		
		int getItemPositionY(int id) {
			assert(id >= 0 && id < items.size() && "EditorDialogFileTable::getItemPositionY(): bad item index");
			return items[id].getPositionY();
		}
		
		//
		void setItemNameText(int id,string text) {
			assert(id >= 0 && id < items.size() && "EditorDialogFileTable::setItemNameText(): bad item index");
			items[id].setNameText(text);
		}
		
		string getItemNameText(int id) {
			assert(id >= 0 && id < items.size() && "EditorDialogFileTable::getItemNameText(): bad item index");
			return items[id].getNameText();
		}
		
		int getItemNameTextWidth(int id) {
			assert(id >= 0 && id < items.size() && "EditorDialogFileTable::getItemNameTextWidth(): bad item index");
			return items[id].getNameTextWidth();
		}
		
		//
		void update(string path) {
			
			int scroll_top = scroll.getVScrollValue() - icon_size * scroll.getScrollScale();
			int scroll_bottom = scroll.getVScrollValue() + scroll.getVScrollFrameSize();
			
			EditorDialogFileTableItem reverse_list[0];
			forloop(int i = 0; items.size()) {
				int adjusted_position = items[i].getPositionY() * scroll.getScrollScale();
				
				if(adjusted_position < scroll_top) continue;
				if(adjusted_position > scroll_bottom) continue;
				
				reverse_list.append(0,items[i]);
			}
			reverse_list.call("update",path);
		}
		
		//
		void arrange() {
			int new_width = canvas.getWidth();
			
			if(canvas_width == new_width) return;
			canvas_width = new_width;
			
			forloop(int i = 0; items.size()) {
				items[i].arrange();
			}
			
			update_selection_polygon(0);
		}
		
		//
		int select() {
			int y = canvas.getMouseY();
			
			if(y > (items.size() * icon_size)) return 0;
			selected_item_id = clamp(y / icon_size,0,items.size() - 1);
			
			update_selection_polygon();
			
			return 1;
		}
		
		void reset_scroll_position() {
			scroll.setVScrollValue(0);
		}
		
		//
		void update_selection_polygon(int scroll_to_selection = 1) {
			if(items.size() == 0) return;
			if(selected_item_id == -1) return;
			
			// selection bounds
			int position_x = items[selected_item_id].getPositionX();
			int position_y = items[selected_item_id].getPositionY();
			
			vec3 p0 = vec3(position_x,position_y,0.0f);
			vec3 p1 = vec3(position_x + canvas.getWidth(),position_y,0.0f);
			vec3 p2 = vec3(position_x + canvas.getWidth(),position_y + icon_size,0.0f);
			vec3 p3 = vec3(position_x,position_y + icon_size,0.0f);
			
			// selection polygon
			canvas.clearPolygonPoints(selection_polygon_id);
			canvas.addPolygonPoint(selection_polygon_id,p0);
			canvas.addPolygonPoint(selection_polygon_id,p1);
			canvas.addPolygonPoint(selection_polygon_id,p2);
			canvas.addPolygonPoint(selection_polygon_id,p3);
			
			// scroll
			if(!scroll_to_selection) return;
			int adjusted_position = position_y * scroll.getScrollScale();
			
			int scroll_frame = scroll.getVScrollFrameSize() - icon_size * scroll.getScrollScale();
			int scroll_top = scroll.getVScrollValue();
			int scroll_bottom = scroll.getVScrollValue() + scroll_frame;
			
			if(adjusted_position < scroll_top) {
				scroll.setVScrollValue(adjusted_position);
			}
			
			if(adjusted_position > scroll_bottom) {
				scroll.setVScrollValue(adjusted_position - scroll_frame);
			}
		}
	};
	
	/**************************************************************************\
	*
	* EditorDialogFile
	*
	\**************************************************************************/
	
	/*
	 */
	class EditorDialogFile : Unigine::Widgets::Window {
		
		//
		class FileData {
			int id;
			string full_name;
			string short_name;
			FileData(int i,string f,string s) {
				id = i;
				full_name = f;
				short_name = s;
			}
		};
		
		//
		enum {
			RENAME_EDITLINE_OFFSET_X = 15,
			RENAME_BOOKMARK_OFFSET_X = 18,
			
			RENAME_EDITLINE_EXPAND = 25,
			
			RENAME_EDITLINE_DELAY = 300,
			
			FIRST_ARROW_SELECTION_TIME = 300,
			OTHER_ARROW_SELECTION_TIME = 50,
			
			MENU_OFFSET_X = -16,
			MENU_OFFSET_Y = -29,
			
			MAX_FOLDERS_TO_CHECK = 1000,
			MAX_FILE_NAME = 100,
			
			UPDATE_FILES_DATA_PART = 10,
		};
		
		//
		WidgetMenuBox actions_m;
		int menu_target;
		
		WidgetButton ok_b;
		WidgetButton cancel_b;
		WidgetIcon cancel_i;
		
		WidgetComboBox drives_cb;
		
		WidgetButton path_left_b;
		WidgetButton path_right_b;
		WidgetScrollBox path_sb;
		WidgetHBox path_hb;
		WidgetButton path_b[0];
		
		WidgetButton create_b;
		WidgetButton open_b;
		
		WidgetHPaned left_vp;
		WidgetHPaned right_vp;
		
		WidgetSprite preview_s;
		WidgetLabel preview_l;
		
		WidgetScrollBox bookmarks_sb;
		WidgetVBox bookmarks_vb;
		WidgetVBox bookmarks_extern_vb;
		WidgetButton bookmarks_data_b;
		WidgetHBox bookmarks_tooltip_hb;
		
		WidgetButton bookmarks_b[0];
		
		WidgetComboBox filter_cb;
		WidgetEditLine file_el;
		
		WidgetEditLine files_rename_el;
		int rename_target;
		
		WidgetScrollBox files_sb;
		WidgetCanvas files_c;
		EditorDialogFileTable files_t;
		
		::UserInterface ui;
		string current_path;
		string filter;
		
		FileData files_info[0] = ();
		int update_thread_id;
		int mtime_thread_id;
		long current_mtime;
		float click_time;
		
		float arrow_selection_time = -1.0f;
		
		int need_rename;
		int need_reload;
		int rename_time;
		int have_preview;
		int mouse_left_pressed;
		int is_done;
		int result;
		
		Async async;
		
		int mode;
		
		int width;
		int height;
		
		int paned_left_value;
		int paned_right_value;
		
		//
		EditorDialogFile(string name = 0) {
			
			async = new Async();
			
			ui = new ::UserInterface(engine.getGui(),"editor/widgets/editor_widget_dialog_file.ui");
			
			// window
			__Window__(get_widget("editor_dialog_file"));
			window.setText(name);
			
			// menu
			actions_m = get_widget("actions_m");
			actions_m.setCallback(GUI_CLICKED,functionid(callback_redirector),this,functionid(menu_clicked));
			
			// buttons
			cancel_b = get_widget("cancel_b");
			cancel_b.arrange();
			cancel_b.setCallback(GUI_CLICKED,functionid(callback_redirector),this,functionid(cancel_clicked));
			
			cancel_i = get_widget("cancel_i");
			cancel_i.setCallback(GUI_CLICKED,functionid(callback_redirector),this,functionid(cancel_clicked));
			
			ok_b = get_widget("ok_b");
			ok_b.arrange();
			ok_b.setWidth(cancel_b.getWidth());
			ok_b.setCallback(GUI_CLICKED,functionid(callback_redirector),this,functionid(ok_clicked));
			
			// drives
			drives_cb = get_widget("drives_cb");
			
			#ifdef _WIN32
				drives_cb.setCallback(GUI_CHANGED,functionid(callback_redirector_2),this,functionid(path_root_changed),drives_cb);
				
				Dir dir = new Dir();
				dir.open("/");
				
				forloop(int i = 0; dir.getNumDirs()) {
					string drive = replace(dir.getDirName(i),"/","");
					drives_cb.addItem(drive);
				}
				
				dir.close();
				
				drives_cb.setHidden(drives_cb.getNumItems() == 0);
			#endif
			
			// path
			path_hb = get_widget("path_hb");
			path_sb = get_widget("path_sb");
			
			path_left_b = get_widget("path_left_b");
			path_left_b.setCallback(GUI_CLICKED,functionid(callback_redirector),this,functionid(path_left_clicked));
			
			path_right_b = get_widget("path_right_b");
			path_right_b.setCallback(GUI_CLICKED,functionid(callback_redirector),this,functionid(path_right_clicked));
			
			// create folder button
			create_b = get_widget("create_b");
			create_b.setCallback(GUI_CLICKED,functionid(callback_redirector),this,functionid(create_folder_clicked));
			create_b.setCallbackAccel(GUI_CLICKED,'n',1,0,0);
			
			// open in system browser button
			open_b = get_widget("open_b");
			open_b.setCallback(GUI_CLICKED,functionid(callback_redirector),this,functionid(open_clicked));
			open_b.setCallbackAccel(GUI_CLICKED,'o',1,0,0);
			
			#ifdef _WIN32
				open_b.setTexture("editor/gui/file_icon_browser_windows.png");
			#elif _LINUX
				open_b.setTexture("editor/gui/file_icon_browser_linux.png");
			#elif _MACOS
				open_b.setTexture("editor/gui/file_icon_browser_mac.png");
			#endif
			
			// paned
			left_vp = get_widget("left_vp");
			right_vp = get_widget("right_vp");
			
			paned_left_value = getPanedLeftValue();
			paned_right_value = getPanedRightValue();
			
			// bookmarks
			bookmarks_sb = get_widget("bookmarks_sb");
			bookmarks_sb.setCallback(GUI_DRAG_DROP,functionid(callback_redirector),this,functionid(bookmarks_drag_drop));
			
			bookmarks_vb = get_widget("bookmarks_vb");
			bookmarks_extern_vb = get_widget("bookmarks_extern_vb");
			bookmarks_tooltip_hb = get_widget("bookmarks_tooltip_hb");
			bookmarks_tooltip_hb.setCallback(GUI_DRAG_DROP,functionid(callback_redirector),this,functionid(bookmarks_drag_drop));
			
			bookmarks_data_b = get_widget("bookmarks_data_b");
			bookmarks_data_b.setToolTip(engine.getDataPath());
			bookmarks_data_b.setCallback(GUI_CLICKED,functionid(callback_redirector_2),this,functionid(set_path),engine.getDataPath());
			
			forloop(int i = 0; engine.getNumArgs()) {
				if(engine.getArg(i) == "-extern_path" && i + 1 < engine.getNumArgs()) {
					string extern_paths[0];
					strsplit(engine.getArg(i + 1),",",extern_paths);
					forloop(int i = 0; extern_paths.size()) {
						string path = savePath(extern_paths[i]);
						if(!is_dir(path)) continue;
						
						WidgetButton widget = new WidgetButton(engine.getGui(),format("extern_path_%d",i));
						widget.setTexture("editor/gui/file_icon_bookmark.png");
						widget.setTextAlign(GUI_ALIGN_LEFT);
						widget.setBackground(0);
						widget.setToolTip(extern_paths[i]);
						widget.setCallback(GUI_CLICKED,functionid(callback_redirector_2),this,functionid(set_path),path);
						bookmarks_extern_vb.addChild(widget,GUI_ALIGN_EXPAND);
					}
				}
			}
			
			// files
			files_rename_el = get_widget("files_rename_el");
			files_rename_el.setCallback(GUI_FOCUS_OUT,functionid(callback_redirector),this,functionid(rename_reset));
			
			files_sb = get_widget("files_sb");
			
			files_c = get_widget("files_c");
			files_c.setCallback(GUI_PRESSED,functionid(callback_redirector),this,functionid(files_pressed));
			files_c.setCallback(GUI_RELEASED,functionid(callback_redirector),this,functionid(files_released));
			files_c.setCallback(GUI_DRAG_DROP,functionid(callback_redirector),this,functionid(files_drag_drop));
			files_c.setCallback(GUI_DOUBLE_CLICKED,functionid(callback_redirector),this,functionid(files_double_clicked));
			
			files_t = new EditorDialogFileTable(files_c,files_sb);
			
			// preview
			preview_s = get_widget("preview_s");
			preview_l = get_widget("preview_l");
			preview_l.setFontRich(1);
			preview_l.setText("");
			
			// filter
			filter_cb = get_widget("filter_cb");
			filter_cb.addItem(TR("All types"));
			filter_cb.setCallback(GUI_CHANGED,functionid(callback_redirector),this,functionid(filter_changed));
			
			// file
			file_el = get_widget("file_el");
			file_el.setCallback(GUI_PRESSED,functionid(callback_redirector),this,functionid(file_pressed));
			file_el.setCallback(GUI_CHANGED,functionid(callback_redirector),this,functionid(file_changed));
			file_el.setCapacity(MAX_FILE_NAME);
			
			set_path(engine.getDataPath());
			update_thread_id = thread(functionid(thread_redirector),this,functionid(update),0);
			mtime_thread_id = thread(functionid(thread_redirector),this,functionid(update_mtime),1);
			
			is_done = 1;
		}
		
		~EditorDialogFile() {
			kill_thread(update_thread_id);
			kill_thread(mtime_thread_id);
			
			bookmarks_b.delete();
			path_b.delete();
			
			delete files_t;
			delete ui;
		}
		
		//
		EditorDialogFileTable getFileTable() {
			return files_t;
		}
		
		//
		void setText(string text) {
			window.setText(text);
		}
		
		//
		int isDone() { return is_done; }
		
		//
		void reset() {
			rename_reset();
			preview_reset();
			
			files_t.resetSelection();
			
			is_done = 0;
			result = 0;
		}
		
		//
		int isOkClicked() { return (result == 1); }
		int isCancelClicked() { return (result == -1); }
		
		//
		int setMode(int m) {
			mode = m;
		}
		
		//
		void setFileFocus() { file_el.setFocus(); }
		
		void setFileFocusSelection() { file_el.setSelection(0); }
		
		void setFile(string str) {
			
			#ifdef _WIN32
				str = replace(str,"\\","/");
			#endif
			
			// root check
			string file = basename(str);
			if(is_absolute(upper(file))) {
				str = upper(file) + "/";
				file = "";
			}
			
			// path
			if(!set_path(str)) {
				if(engine.filesystem.isKnownFile(str)) {
					set_path(engine.getDataPath());
					file = "";
					file_el.setText(file);
				} else {
					thread(functionid(dialogMessageOk,2),TR("Error"),format(TR("Directory \"%s\" doesn't exist"),str));
				}
				return;
			}
			if(is_dir(current_path + file + "/")) {
				set_path(current_path + file + "/");
				file = "";
			}
			
			// file
			file_el.setText(file);
		}
		
		string getFile() {
			string path = current_path + file_el.getText();
			
			if(filter == "") return path;
			
			int num_filters = filter_cb.getNumItems();
			int current_filter = filter_cb.getCurrentItem();
			
			if(num_filters > 1 && current_filter == 0) return path;
			
			string target_extension = trim(filter_cb.getCurrentItemText(),"*.");
			path = setExtension(path,target_extension,filter);
			
			return path;
		}
		
		//
		void setPanedLeftValue(int value) { left_vp.setValue(value); }
		int getPanedLeftValue() { return left_vp.getValue(); }
		
		void setPanedRightValue(int value) { right_vp.setValue(value); }
		int getPanedRightValue() { return right_vp.getValue(); }
		
		//
		void setBookmarks(string str) {
			string bookmarks[0];
			strsplit(str,";",bookmarks);
			
			bookmarks_b.delete();
			
			for(int i = 0; i < bookmarks.size(); i += 2) {
				add_bookmark(bookmarks[i],bookmarks[i + 1]);
			}
		}
		
		string getBookmarks() {
			if(bookmarks_b.size() == 0) return "";
			
			string bookmarks = "";
			forloop(int i = 0; bookmarks_b.size() - 1) {
				bookmarks += bookmarks_b[i].getToolTip() + ";";
				bookmarks += bookmarks_b[i].getText() + ";";
			}
			
			bookmarks += bookmarks_b[bookmarks_b.size() - 1].getToolTip() + ";";
			bookmarks += bookmarks_b[bookmarks_b.size() - 1].getText();
			
			return bookmarks;
		}
		
		//
		void setFilter(string str) {
			filter = str;
			
			string extensions[0];
			strsplit(str,".",extensions);
			
			filter_cb.setCallbackEnabled(GUI_CHANGED,0);
			filter_cb.clear();
			
			// no extensions
			if(extensions.size() == 0) {
				filter = "";
				
				filter_cb.addItem(TR("All Types") + " (*.*)");
				filter_cb.setCurrentItem(0);
			}
			
			// multiple extensions
			else if(extensions.size() > 1) {
				string all = TR("All Types") + " (*." + extensions[0];
				forloop(int i = 1; extensions.size()) {
					all += ", *." + extensions[i];
				}
				all += ")";
				
				filter_cb.addItem(all);
				filter_cb.setCurrentItem(0);
			}
			
			forloop(int i = 0; extensions.size()) {
				filter_cb.addItem("*." + extensions[i]);
			}
			
			filter_cb.setCallbackEnabled(GUI_CHANGED,1);
			
			need_reload = 1;
		}
		
		string getFilter() {
			return filter;
		}
		
		//
		void update_mtime() {
			
			if(is_done) return;
			if(files_rename_el.isFocused()) return;
			
			#ifdef _WIN32
				long new_mtime = get_mtime(trim(current_path,"/"));
			#else
				long new_mtime = get_mtime(current_path);
			#endif
			
			if(new_mtime == -1) {
				need_reload = 1;
			} else if(new_mtime != current_mtime) {
				current_mtime = new_mtime;
				need_reload = 1;
			}
		}
		
		void update() {
			if(is_done) return;
			
			if(width != window.getWidth() || height != window.getHeight()) {
				if(width != window.getWidth()) resize_path_scroll();
				width = window.getWidth();
				height = window.getHeight();
			}
			
			//
			if(paned_left_value != getPanedLeftValue() && engine.app.getMouseButtonState(APP_BUTTON_LEFT) == 0) {
				setFocus();
				paned_left_value = getPanedLeftValue();
			}
			
			if(paned_right_value != getPanedRightValue() && engine.app.getMouseButtonState(APP_BUTTON_LEFT) == 0) {
				setFocus();
				paned_right_value = getPanedRightValue();
			}
			
			update_files();
			update_files_data();
			update_scroll();
			update_rename();
			update_controls();
			update_preview();
			
			files_t.update(current_path);
			files_t.arrange();
		}
		
		void update_files() {
			if(!need_reload) return;
			
			int item = files_t.getCurrentItem();
			string text = (item != -1) ? files_t.getItemNameText(item) : file_el.getText();
			
			load_path();
			load_files();
			
			#ifdef _WIN32
				long new_mtime = get_mtime(trim(current_path,"/"));
			#else
				long new_mtime = get_mtime(current_path);
			#endif
			current_mtime = new_mtime;
			
			int id = files_t.findItem(text);
			if(id != -1) files_t.setCurrentItem(id,0);
			
			preview_set();
			
			window.arrange();
			need_reload = 0;
		}
		
		void update_scroll() {
			int hide_scroll = (path_hb.getWidth() <= path_sb.getWidth());
			int scroll_value = path_sb.getHScrollValue();
			int scroll_max = path_sb.getHScrollObjectSize() - path_sb.getHScrollFrameSize();
			
			path_left_b.setHidden(hide_scroll);
			path_left_b.setEnabled(scroll_value > 0);
			
			path_right_b.setHidden(hide_scroll);
			path_right_b.setEnabled(scroll_value < scroll_max);
		}
		
		void resize_path_scroll() {
			int scroll_max = path_sb.getHScrollObjectSize() - path_sb.getHScrollFrameSize();
			path_sb.setHScrollValue(scroll_max);
		}
		
		void update_rename() {
			if(!need_rename) return;
			if(time() - rename_time < 1) return;
			
			rename_start();
		}
		
		void update_controls() {
			if(engine.gui.getPermanentFocus() != window) return;
			
			if(files_rename_el.isFocused()) {
				if(engine.app.clearKeyState(APP_KEY_RETURN)) rename_apply();
				if(engine.app.clearKeyState(APP_KEY_ESC)) rename_reset();
				return;
			}
			
			if(engine.app.getKeyState(APP_KEY_CTRL) && engine.app.clearKeyState('t')) {
				bookmarks_add_current();
			}
			
			if(engine.app.clearKeyState(APP_KEY_ESC)) cancel_clicked();
			if(!file_el.isFocused()) {
				
				if(engine.app.clearKeyState(APP_KEY_DELETE)) {
					int item = files_t.getCurrentItem();
					if(item == -1) return;
					
					thread(functionid(delete_item),this,item);
				}
				
				if(engine.app.clearKeyState(APP_KEY_F2)) {
					int item = files_t.getCurrentItem();
					if(item == -1) return;
					
					rename_init(item);
					rename_start();
				}
				
				if(engine.app.clearKeyState(APP_KEY_BACKSPACE)) {
					set_path(current_path + "../");
				}
				
				if(engine.app.clearKeyState(APP_KEY_RETURN)) {
					int item = files_t.getCurrentItem();
					if(item == -1) return;
					
					string dir = current_path + files_t.getItemNameText(item) + "/";
					string file = current_path + files_t.getItemNameText(item);
					
					if(is_dir(dir)) set_path(dir);
					if(is_file(file)) ok_clicked();
				}
			}
			
			if(files_t.getNumItems() != 0) {
				int item = files_t.getCurrentItem();
				
				if(engine.app.getKeyState(APP_KEY_UP) || engine.app.getKeyState(APP_KEY_DOWN)) {
					if(arrow_selection_time == -1.0f) {
						if(engine.app.getKeyState(APP_KEY_UP)) item = max(item - 1,0);
						if(engine.app.getKeyState(APP_KEY_DOWN)) item = min(item + 1,files_t.getNumItems() - 1);
						arrow_selection_time = clock() + float(FIRST_ARROW_SELECTION_TIME) / 1000.0f;
					} else {
						if(clock() - arrow_selection_time >= float(OTHER_ARROW_SELECTION_TIME) / 1000.0f) {
							if(engine.app.getKeyState(APP_KEY_UP)) item = max(item - 1,0);
							if(engine.app.getKeyState(APP_KEY_DOWN)) item = min(item + 1,files_t.getNumItems() - 1);
							arrow_selection_time = clock();
						}
					}
					if(file_el.isFocused()) files_t.canvas.setFocus();
				} else {
					arrow_selection_time = -1.0f;
				}
				
				if(engine.app.clearKeyState(APP_KEY_HOME) && !file_el.isFocused()) {
					item = 0;
				}
				
				if(engine.app.clearKeyState(APP_KEY_END) && !file_el.isFocused()) {
					item = files_t.getNumItems() - 1;
				}
				
				if(item != files_t.getCurrentItem()) {
					set_item(item);
				}
			}
		}
		
		void set_item(int index) {
			if(files_t.getNumItems() == 0) return;
			
			index = min(max(0,index),files_t.getNumItems() - 1);
			files_t.setCurrentItem(index);
			preview_set();
			
			string file = current_path + files_t.getItemNameText(index);
			if(is_file(file)) setFile(file);
			
			rename_reset();
		}
		
		void update_preview() {
			
			int index = files_t.getCurrentItem();
			if(index == -1) return;
			
			string item = current_path + files_t.getItemNameText(index);
			if(thumbnailsIsAvailable(item) == 0) return;
			
			Image image = thumbnailsGetImage(item);
			if(image == NULL) return;
			
			preview_s.setImage(image);
			
			have_preview = 1;
		}
		
		//
		Widget get_widget(string name) {
			int id = ui.findWidget(name);
			if(id == -1) return;
			
			return widget_cast(ui.getWidget(id));
		}
		
		//
		void add_bookmark(string path,string name) {
			if(is_dir(path) == 0) return;
			
			if(strlen(name) > 15) {
				name = substr(name,0,12) + "...";
			}
			
			if(path == engine.getDataPath()) return;
			forloop(int i = 0; bookmarks_b.size()) {
				if(bookmarks_b[i].getToolTip() == path) return;
			}
			
			WidgetButton widget = new WidgetButton(engine.getGui(),name);
			widget.setTexture("editor/gui/file_icon_bookmark.png");
			widget.setTextAlign(GUI_ALIGN_LEFT);
			widget.setBackground(0);
			widget.setToolTip(path);
			widget.setCallback(GUI_CLICKED,functionid(callback_redirector_2),this,functionid(bookmarks_clicked),widget);
			widget.setCallback(GUI_PRESSED,functionid(callback_redirector_2),this,functionid(bookmarks_pressed),widget);
			
			bookmarks_vb.addChild(widget,GUI_ALIGN_EXPAND);
			bookmarks_b.append(widget);
			
			bookmarks_tooltip_hb.setHidden(1);
			
			rename_reset();
		}
		
		void remove_bookmark(WidgetButton bookmark) {
			int id = bookmarks_b.find(bookmark);
			if(id == -1) return;
			
			bookmarks_vb.removeChild(bookmark);
			bookmarks_b.remove(id);
			
			delete bookmark;
			
			if(bookmarks_b.size() == 0) bookmarks_tooltip_hb.setHidden(0);
		}
		
		//
		void add_path_button(string path,string name) {
			WidgetButton widget = new WidgetButton(engine.getGui(),name);
			widget.setTextAlign(GUI_ALIGN_CENTER);
			widget.setToolTip(path);
			widget.setCallback(GUI_CLICKED,functionid(callback_redirector_2),this,functionid(set_path),path);
			
			path_hb.addChild(widget);
			path_b.append(widget);
		}
		
		//
		int set_path(string str) {
			string old_path = current_path;
			
			current_path = is_absolute(upper(str)) ? str : old_path + str;
			current_path = pathname(current_path);
			
			string dirs[0];
			strsplit(current_path,"/\\",dirs);
			current_path = engine.getDataPath();
			
			#ifndef _WIN32
				dirs.append(0,"");
			#endif
			
			string new_path = "";
			forloop(int i = 0; dirs.size()) {
				new_path += dirs[i] + "/";
				if(is_dir(new_path) == 0) {
					current_path = old_path;
					return false;
				}
				
				current_path = new_path;
			}
			
			dirs.clear();
			
			#ifdef _WIN32
				current_mtime = get_mtime(trim(current_path,"/"));
			#else
				current_mtime = get_mtime(current_path);
			#endif
			
			if(current_path != old_path) {
				files_t.reset_scroll_position();
				need_reload = 1;
			}
			
			return true;
		}
		
		//
		int is_absolute(string path) {
			#ifdef _WIN32
				int isalpha(int code) {
					return ((code >= 'a' && code <= 'z') || (code >= 'A' && code <= 'Z'));
				}
				return (strlen(path) > 1) && (isalpha(path[0])) && (path[1] == ':');
			#else
				return (strlen(path) > 0) && (path[0] == '/');
			#endif
		}
		
		void load_drive(string drive) {
			drives_cb.setCallbackEnabled(GUI_CHANGED,0);
			
			forloop(int i = 0; drives_cb.getNumItems()) {
				if(drives_cb.getItemText(i) == drive) {
					drives_cb.setCurrentItem(i);
					break;
				}
			}
			
			drives_cb.setCallbackEnabled(GUI_CHANGED,1);
		}
		
		void load_path() {
			rename_reset();
			
			// path
			string path[0];
			strsplit(current_path,"/\\",path);
			
			// buttons
			path_b.delete();
			
			#ifdef _WIN32
				string drive = upper(path[0]);
				path.remove(0);
				
				load_drive(drive);
				string current_path = drive + "/";
			#else
				string current_path = "/";
			#endif
			
			add_path_button(current_path,"/");
			
			forloop(int i = 0; path.size()) {
				current_path += path[i] + "/";
				add_path_button(current_path,path[i]);
			}
			
			window.arrange();
			update_scroll();
			
			window.arrange();
			path_sb.setHScrollValue(path_sb.getHScrollObjectSize());
		}
		
		//
		void get_file_data(FileData file_data) {
			File file = new File();
			int file_size = 0;
			
			if(file.open(file_data.full_name,"rb")) {
				file_size = file.getSize();
				file.close();
			}
			
			int mtime = get_mtime(file_data.full_name);
			
			string date_year = substr(date("%04Y",mtime),2,2);
			string date_day = date("%02D.%02M.",mtime);
			string date_time = date(" %02h:%02m",mtime);
			string date_full = date_day + date_year + date_time;
			
			files_t.setItemData(file_data.id,1,file_data.short_name,file_size,date_full);
			
			delete file;
		}
		
		//
		void update_files_data() {
			if(files_info.size() == 0) return;
			
			int part_size = UPDATE_FILES_DATA_PART;
			if(part_size >= files_info.size()) part_size = files_info.size();
			
			// load data part
			forloop(int i = 0; part_size) {
				FileData info = files_info[i];
				get_file_data(info);
				delete info;
			}
			
			// remove loaded part
			files_info.remove(0,part_size);
		}
		
		//
		void load_files() {
			
			preview_reset();
			rename_reset();
			thumbnailsFlush();
			
			Dir dir = new Dir();
			
			string dirs[0];
			string files[0];
			int sizes[];
			int mtimes[];
			
			if(dir.open(current_path)) {
				forloop(int i = 0; dir.getNumDirs()) {
					dirs.append(dir.getDirName(i));
				}
				
				forloop(int i = 0; dir.getNumFiles()) {
					files.append(dir.getFileName(i));
					sizes.append(dir.getFileName(i),dir.getFileSize(i));
					mtimes.append(dir.getFileName(i),dir.getFileMTime(i));
				}
				
				dir.close();
			}
			
			delete dir;
			
			int path_len = utf8strlen(current_path);
			
			dirs.sort();
			files.sort();
			
			files_t.clear();
			
			forloop(int i = 0; dirs.size()) {
				string short_dir = utf8substr(dirs[i],path_len,utf8strlen(dirs[i]) - path_len - 1);
				
				if(is_hidden(dirs[i])) continue;
				if(short_dir == "") continue;
				if(short_dir[0] == '.') continue;
				
				files_t.addItem(0,short_dir,-1,"");
			}
			
			files_info.clear();
			
			forloop(int i = 0; files.size()) {
				string short_file = utf8substr(files[i],path_len,utf8strlen(files[i]) - path_len);
				
				if(is_hidden(files[i])) continue;
				if(short_file == "") continue;
				if(short_file[0] == '.') continue;
				
				if(filter != "") {
					string ext = "*." + lower(extension(short_file));
					
					int index = -1;
					int current_filter = filter_cb.getCurrentItem();
					
					forloop(int j = 0; filter_cb.getNumItems()) {
						if(filter_cb.getItemText(j) == ext) {
							index = j;
							break;
						}
					}
					
					if(index == -1) continue;
					if(current_filter > 0 && index != current_filter) continue;
				}
				
				string file_local = relname(engine.getDataPath(),files[i]);
				if(!engine.filesystem.isKnownFile(file_local)) engine.filesystem.addKnownFile(file_local);
				
				int file_size = sizes[files[i]];
				int mtime = mtimes[files[i]];
				
				string date_year = substr(date("%04Y",mtime),2,2);
				string date_day = date("%02D.%02M.",mtime);
				string date_time = date(" %02h:%02m",mtime);
				string date_full = date_day + date_year + date_time;
				
				files_t.addItem(1,short_file,file_size,date_full);
			}
			
			files_t.update(current_path);
			files_t.arrange();
			
			files.clear();
			dirs.clear();
			sizes.clear();
			mtimes.clear();
			
			string file = file_el.getText();
			
			string check = trim(file," ");
			int enabled = strlen(check) > 0 && check[0] != '.' && check[strlen(check) - 1] != '.' && isPathValid(getFile());
			if(mode == DIALOG_FILE_OPEN) enabled = enabled && is_file(getFile());
			
			ok_b.setEnabled(enabled);
		}
		
		//
		void preview_reset() {
			preview_s.setHidden(1);
			preview_s.setTexture(NULL);
			preview_l.setText("");
			have_preview = 0;
		}
		
		void preview_set() {
			int index = files_t.getCurrentItem();
			if(index == -1) return;
			
			string path = current_path + files_t.getItemNameText(index);
			
			preview_reset();
			
			if(is_dir(path)) {
				preview_set_dir(path);
				return;
			}
			
			if(!is_file(path)) return;
			
			if(preview_set_anim(path)) return;
			if(preview_set_node(path)) return;
			thread(functionid(preview_set_image),this,path);
			thread(functionid(preview_set_mesh),this,path);
		}
		
		int preview_set_dir(string path) {
			Dir dir = new Dir();
			if(dir.open(path + "/") == 0) {
				delete dir;
				return 0;
			}
			
			string info = "";
			int num_files = dir.getNumFiles();
			int num_dirs = dir.getNumDirs();
			int size = dir.getSize();
			
			dir.close();
			delete dir;
			
			if(num_dirs != 0) info += format(TR("<center>Dirs: %d</center>"),num_dirs);
			if(num_files != 0) info += format(TR("<center>Files: %d (%s)</center>"),num_files,memory(size));
			
			preview_l.setText(info);
			
			preview_s.setHidden(1);
			
			return 1;
		}
		
		int preview_set_image(EditorDialogFile instance,string path) {
			
			if(!is_file(path)) return;
			
			Image image = new Image();
			
			string to_load[] = (
				"hdr",
				"psd",
				"rgb",
				"rgba",
				"ppm",
				"pgm",
				"sgi",
			);
			
			if(image.info(path) == 0) {
				if(to_load.find(lower(extension(path))) != -1) {
					int id = instance.async.run(image,functionid(Image::load),path);
					while(instance.async.isRunning()) wait;
					if(instance.async.getResult(id) == 0) {
						delete image;
						return 0;
					}
				} else {
					delete image;
					return 0;
				}
			}
			
			int width = image.getWidth();
			int height = image.getHeight();
			int mipmaps = image.getNumMipmaps();
			string format_name = image.getFormatName();
			
			switch(image.getType()) {
				case IMAGE_2D:
					instance.preview_l.setText(format(TR("<center>2D %s</center><center>%dx%d %d</center>"),format_name,width,height,mipmaps));
				break;
				case IMAGE_2D_ARRAY:
					instance.preview_l.setText(format(TR("<center>2D array %s</center><center>%dx%d %d</center>"),format_name,width,height,mipmaps));
				break;
				case IMAGE_3D:
					instance.preview_l.setText(format(TR("<center>3D %s</center><center>%dx%dx%d %d</center>"),format_name,width,height,image.getDepth(),mipmaps));
				break;
				case IMAGE_CUBE:
					instance.preview_l.setText(format(TR("<center>Cube %s</center><center>%dx%d %d</center>"),format_name,width,height,mipmaps));
				break;
				case IMAGE_CUBE_ARRAY:
					instance.preview_l.setText(format(TR("<center>Cube array %s</center><center>%dx%d %d</center>"),format_name,width,height,mipmaps));
				break;
			}
			
			delete image;
			
			instance.preview_s.setHidden(0);
			
			return 1;
		}
		
		int preview_set_mesh(EditorDialogFile instance,string path) {
			
			if(lower(extension(path)) != "mesh") return 0;
			
			if(!is_file(path)) return;
			
			Mesh mesh = new Mesh();
			
			int index = instance.files_t.getCurrentItem();
			EditorDialogFileTableItem item = instance.files_t.getItem(index);
			
			int full_info = 1;
			if(item.getSize() > 1024 * 1024 * 50) full_info = 0;
			
			if(full_info) {
				int id = instance.async.run(mesh,functionid(Mesh::load),path);
				while(instance.async.isRunning()) wait;
				if(instance.async.getResult(id) == 0) {
					delete mesh;
					return 0;
				}
			} else {
				if(mesh.info(path) == 0) {
					delete mesh;
					return 0;
				}
			}
			
			int animations = mesh.getNumAnimations();
			int bones = mesh.getNumBones();
			int surfaces = mesh.getNumSurfaces();
			
			string info = "";
			if(animations == 0) info += format(TR("<center>Static mesh</center>"));
			else info += format(TR("<center>Skinned mesh</center>"));
			
			if(full_info) {
				string getShortNumber(int number) {
					if(number < 1000) return string(number);
					
					float num = float(number);
					string range = "";
					while(num / 1000.0f > 1.0f) {
						num = num / 1000.0f;
						range += "k";
					}
					
					string result = trim(format("%3.2f",num),".0");
					return format("%s%s",result,range);
				}
				
				int num_cvertex = 0;
				int num_tvertex = 0;
				int num_indices = 0;
				forloop(int surface = 0; surfaces) {
					num_cvertex += mesh.getNumCVertex(surface);
					num_tvertex += mesh.getNumTVertex(surface);
					num_indices += mesh.getNumCIndices(surface);
				}
				
				info += format(TR("<center>Vertices: %s/%s</center>"),getShortNumber(num_cvertex),getShortNumber(num_tvertex));
				info += format(TR("<center>Triangles: %s</center>"),getShortNumber(num_indices / 3));
			}
			
			if(surfaces > 0) info += format(TR("<center>Surfaces: %d</center>"),surfaces);
			if(animations > 0) info += format(TR("<center>Animations: %d</center>"),animations);
			if(bones > 0) info += format(TR("<center>Bones: %d</center>"),bones);
			
			instance.preview_l.setText(info);
			
			instance.preview_s.setHidden(0);
			
			delete mesh;
			return 1;
		}
		
		int preview_set_anim(string path) {
			if(lower(extension(path)) != "anim") return 0;
			
			Mesh mesh = new Mesh();
			
			if(mesh.info(path) == 0) {
				delete mesh;
				return 0;
			}
			
			int animations = mesh.getNumAnimations();
			int bones = mesh.getNumBones();
			
			string info = format(TR("<center>Animation</center>"));
			if(animations > 0) info += format(TR("<center>Animations: %d</center>"),animations);
			if(bones > 0) info += format(TR("<center>Bones: %d</center>"),bones);
			
			preview_l.setText(info);
			preview_s.setHidden(0);
			
			delete mesh;
			return 1;
		}
		
		
		int preview_set_node(string path) {
			if(lower(extension(path)) != "node") return 0;
			
			preview_s.setHidden(0);
			
			return 1;
		}
		
		//
		void delete_item(EditorDialogFile instance,int index) {
			string item = instance.files_t.getItemNameText(index);
			string path = instance.current_path + item;
			
			#ifdef _WIN32
				path = replace(path,"/","\\");
			#endif
			
			if(is_dir(path + "/")) {
				if(dialogMessageYesNo(TR("Confirm"),TR("Are you sure you want to delete this folder?"))) {
					if(rmdir(path + "/",1) == 0) {
						dialogMessageOk(TR("Error"),TR("Can't delete folder"));
						return;
					}
				}
			}
			else if(is_file(path)) {
				if(dialogMessageYesNo(TR("Confirm"),TR("Are you sure you want to delete this file?"))) {
					if(remove(path) == 0) {
						dialogMessageOk(TR("Error"),TR("Can't delete file"));
						return;
					}
				}
			}
			
			instance.setPermanentFocus();
			
			instance.load_files();
			instance.set_item(index);
		}
		
		//
		void rename_item(EditorDialogFile instance,string old_item,string new_item) {
			if(new_item == "" || new_item == old_item) {
				instance.rename_reset();
				return;
			}
			
			string old_file = instance.current_path + old_item;
			
			if(is_file(old_file)) {
				if(new_item[0] == '.') {
					instance.rename_reset();
					return;
				}
			}
			
			new_item = trim(new_item,". ");
			if(new_item == old_item) {
				instance.rename_reset();
				return;
			}
			
			string file = instance.current_path + new_item;
			string dir = instance.current_path + new_item + "/";
			
			if(is_dir(dir)) {
				dialogMessageOk(TR("Error"),format(TR("Can't overwrite \"%s\" folder"),new_item));
				instance.rename_reset();
				return;
			}
			
			if(is_file(file)) {
				dialogMessageOk(TR("Error"),format(TR("Can't overwrite \"%s\" file"),new_item));
				instance.rename_reset();
				return;
			}
			
			#ifdef _WIN32
				file = replace(file,"/","\\");
				old_file = replace(old_file,"/","\\");
			#endif
			
			if(rename(old_file,file) == 0) {
				dialogMessageOk(TR("Error"),format(TR("Can't rename from \"%s\" to \"%s\", access denied"),old_item,new_item));
				instance.rename_reset();
				return;
			}
			
			if(is_file(file)) instance.file_el.setText(new_item);
			instance.load_files();
			
			int id = instance.files_t.findItem(new_item);
			if(id != -1) instance.files_t.setCurrentItem(id);
		}
		
		void rename_init(int item) {
			int x = 0;
			int y = 0;
			int offset_y = -1;
			int offset_x = 0;
			int width = 0;
			string text = "";
			
			::Widget new_parent = NULL;
			rename_target = item;
			
			if(item is WidgetButton) {
				WidgetButton bookmark = item;
				
				new_parent = bookmarks_vb;
				
				x = bookmark.getPositionX();
				y = bookmark.getPositionY();
				text = bookmark.getText();
				
				offset_x = RENAME_BOOKMARK_OFFSET_X;
				
				width = bookmark.getWidth();
			}
			else {
				new_parent = files_sb;
				
				x = files_t.getItemPositionX(item);
				y = files_t.getItemPositionY(item);
				text = files_t.getItemNameText(item);
				
				offset_x = RENAME_EDITLINE_OFFSET_X;
				
				width = files_t.getItemNameTextWidth(item);
				width += RENAME_EDITLINE_EXPAND;
			}
			
			if(new_parent != NULL) new_parent.addChild(files_rename_el,GUI_ALIGN_OVERLAP);
			
			files_rename_el.setPosition(x + offset_x,y + offset_y);
			files_rename_el.setWidth(width);
			files_rename_el.setText(text);
			
			int cursor = strrchr(text,'.');
			if(cursor != -1 && is_file(current_path + text)) files_rename_el.setCursor(cursor);
			files_rename_el.setSelection(0);
			
			need_rename = 1;
			rename_time = time();
		}
		
		void rename_start() {
			files_c.removeFocus();
			create_b.removeFocus();
			open_b.removeFocus();
			actions_m.removeFocus();
			
			window.raise(files_rename_el);
			
			files_rename_el.setHidden(0);
			files_rename_el.setFocus();
			
			need_rename = 0;
		}
		
		void rename_reset() {
			files_rename_el.removeFocus();
			files_rename_el.setHidden(1);
			files_rename_el.setText("");
			
			need_rename = 0;
			rename_target = NULL;
			
			if(window.isFocused()) files_c.setFocus();
		}
		
		void rename_apply() {
			string new_name = re_replace(files_rename_el.getText(),"[<>:\"/\\|?*]","");
			
			if(rename_target is WidgetButton) {
				WidgetButton bookmark = rename_target;
				if(new_name != "") bookmark.setText(new_name);
				
				rename_reset();
			}
			else {
				thread(functionid(rename_item),this,files_t.getItemNameText(rename_target),new_name);
			}
		}
		
		//
		void menu_clicked() {
			int action = actions_m.getCurrentItem();
			
			// rename
			if(action == 0) {
				rename_init(menu_target);
				rename_start();
			}
			
			// delete
			else if(action == 1) {
				if(is_int(menu_target)) {
					thread(functionid(delete_item),this,menu_target);
				}
				else {
					remove_bookmark(menu_target);
				}
			}
			
			hide_menu();
		}
		
		void show_menu(int target) {
			window.addChild(actions_m,GUI_ALIGN_OVERLAP);
			
			int x = window.getMouseX() + MENU_OFFSET_X;
			int y = window.getMouseY() + MENU_OFFSET_Y;
			
			actions_m.setPosition(x,y);
			actions_m.setPermanentFocus();
			
			menu_target = target;
		}
		
		void hide_menu() {
			window.removeChild(actions_m);
			
			menu_target = NULL;
		}
		
		//
		void path_left_clicked() {
			int scroll_scale = path_sb.getScrollScale();
			int scroll_value = path_sb.getHScrollValue();
			int path_width = 0;
			
			forloop(int i = 0; path_b.size()) {
				int new_width = path_width + path_b[i].getWidth() * scroll_scale;
				
				if(new_width >= scroll_value) break;
				path_width = new_width;
			}
			
			path_sb.setHScrollValue(path_width);
		}
		
		void path_right_clicked() {
			int scroll_scale = path_sb.getScrollScale();
			int scroll_value = path_sb.getHScrollValue();
			int path_width = 0;
			
			forloop(int i = 0; path_b.size()) {
				path_width += path_b[i].getWidth() * scroll_scale;
				
				if(path_width > scroll_value) break;
			}
			
			path_sb.setHScrollValue(path_width);
		}
		
		//
		void create_folder_clicked() {
			string folder_name = TR("New Folder");
			int counter = 0;
			
			while(is_dir(current_path + folder_name + "/")) {
				counter++;
				folder_name = format(TR("New Folder (%d)"),counter);
				
				if(counter > MAX_FOLDERS_TO_CHECK) return;
			}
			
			thread(functionid(create_folder),this,folder_name);
		}
		
		void create_folder(EditorDialogFile instance,string folder_name) {
			if(mkdir(instance.current_path + folder_name) == 0) {
				dialogMessageOk(TR("Error"),TR("Can't create folder"));
				return;
			}
			
			instance.load_files();
			
			int folder_id = instance.files_t.findItem(folder_name);
			if(folder_id == -1) return;
			
			instance.files_t.setCurrentItem(folder_id);
			
			instance.rename_init(folder_id);
			instance.rename_start();
		}
		
		//
		void open_clicked() {
			#ifdef _LINUX
				system(format("xdg-open \"%s\"",current_path),0);
			#else
				string file = "";
				
				int id = files_t.getCurrentItem();
				if(id != -1) file = current_path + files_t.getItemNameText(id);
				
				int can_highlight = (file != "") && (is_file(file) || is_dir(file + "/"));
				
				#ifdef _WIN32
					string argument = (can_highlight) ? "/select," : "";
					string path = (can_highlight) ? file : current_path;
					string command = "explorer.exe";
					
					path = replace(path,"/","\\");
				#elif _MACOS
					string argument = (can_highlight) ? "-R " : "";
					string path = (can_highlight) ? file : current_path;
					string command = "open -n";
				#endif
				
				system(format("%s %s\"%s\"",command,argument,path),0);
			#endif
		}
		
		//
		void bookmarks_drag_drop() {
			Widget focus = widget_cast(engine.gui.getFocus());
			
			if(focus is WidgetCanvas) {
				int item = files_t.getCurrentItem();
				if(item == -1) return;
				
				string dir = files_t.getItemNameText(item);
				string path = current_path + dir + "/";
				
				if(is_dir(path)) add_bookmark(path,dir);
			}
		}
		
		void bookmarks_add_current() {
			string name = basename(trim(current_path,"/"));
			if(name == "") name = TR("Root");
			
			add_bookmark(current_path,name);
		}
		
		void bookmarks_clicked(WidgetButton bookmark) {
			string path = bookmark.getToolTip();
			
			if(is_dir(path) == 0) {
				thread(functionid(dialogMessageOk,2),TR("Error"),format(TR("Directory \"%s\" doesn't exist"),path));
				remove_bookmark(bookmark);
				return;
			}
			
			set_path(path);
		}
		
		void bookmarks_pressed(WidgetButton bookmark) {
			if(engine.app.clearMouseButtonState(APP_BUTTON_RIGHT) == 0) return;
			if(bookmark == NULL) return;
			
			show_menu(bookmark);
		}
		
		//
		void path_root_changed(WidgetComboBox root_cb) {
			int item = root_cb.getCurrentItem();
			if(item == -1) return;
			
			string dir = root_cb.getItemText(item) + "/";
			if(is_dir(dir)) set_path(dir);
		}
		
		//
		void files_pressed() {
			
			// drag
			if(engine.gui.getMouseDX() > 0 || engine.gui.getMouseDY() > 0) {
				rename_reset();
			}
			
			// files
			if(engine.gui.getMouseButton() == APP_BUTTON_LEFT && mouse_left_pressed == 0) {
				mouse_left_pressed = 1;
				
				int old_selection = files_t.getCurrentItem();
				
				if(files_t.select() == 0) return;
				preview_set();
				
				int item = files_t.getCurrentItem();
				if(item == -1) return;
				
				string file = current_path + files_t.getItemNameText(item);
				if(is_file(file)) setFile(file);
				
				if(old_selection == item && clock() - click_time >= float(RENAME_EDITLINE_DELAY) / 1000.0f) {
					rename_init(item);
				} else {
					rename_reset();
					old_selection = item;
					click_time = clock();
				}
				
				return;
			}
			
			// menu
			if(engine.app.clearMouseButtonState(APP_BUTTON_RIGHT) == 0) return;
			if(files_t.select() == 0) return;
			
			int item = files_t.getCurrentItem();
			if(item == -1) return;
			
			show_menu(item);
		}
		
		void files_released() {
			mouse_left_pressed = 0;
		}
		
		void files_double_clicked() {
			if(files_t.select() == 0) return;
			
			int item = files_t.getCurrentItem();
			if(item == -1) return;
			
			string file = current_path + files_t.getItemNameText(item);
			
			if(is_dir(file + "/")) {
				set_path(file + "/");
			} else if(is_file(file)) {
				setFile(file);
				rename_reset();
				click_time = clock();
				ok_clicked();
				mouse_left_pressed = 0;
			}
		}
		
		void files_drag_drop() {
			engine.message("drag\n");
		}
		
		//
		void file_pressed() {
			setFile(trim(file_el.getText()));
			if(file_el.getText() != "") ok_clicked();
		}
		
		void file_changed() {
			
			string file = file_el.getText();
			if(is_dir(file)) return;
			
			int cursor = file_el.getCursor();
			int selection = file_el.getSelection();
			
			file_el.setCallbackEnabled(CHANGED,0);
			file_el.setText(re_replace(file,"[<>\"|?*&%]",""));
			file_el.setCallbackEnabled(CHANGED,1);
			file_el.setCursor(cursor);
			file_el.setSelection(selection);
			
			string check = trim(file," ");
			int enabled = strlen(check) > 0 && check[0] != '.' && check[strlen(check) - 1] != '.' && isPathValid(getFile());
			if(mode == DIALOG_FILE_OPEN) enabled = enabled && is_file(getFile());
			
			ok_b.setEnabled(enabled);
		}
		
		//
		void filter_changed() {
			need_reload = 1;
		}
		
		//
		void ok_clicked() {
			if(ok_b.isEnabled() == 0) return;
			
			if(mode == DIALOG_FILE_SAVE) {
				thread(functionid(file_confirm),this);
				return;
			}
			
			reset();
			
			result = 1;
			is_done = 1;
		}
		
		void file_confirm(EditorDialogFile instance) {
			
			if(!dialogFileConfirm(instance.getFile())) return;
			
			instance.reset();
			
			instance.result = 1;
			instance.is_done = 1;
		}
		
		void cancel_clicked() {
			reset();
			
			result = -1;
			is_done = 1;
		}
	};
	
	/**************************************************************************\
	*
	* Helpers
	*
	\**************************************************************************/
	
	/*
	 */
	void callback_redirector(int instance,string method) {
		instance.call(method);
	}
	
	void callback_redirector_2(int instance,string method,int data) {
		instance.call(method,data);
	}
	
	/*
	 */
	void thread_redirector(int instance,string method,float interval) {
		while(1) {
			instance.call(method);
			wait;
			
			if(interval) sleep(interval);
		}
	}
	
} /* namespace Editor::Widgets */

#endif /* __EDITOR_WIDGET_DIALOG_FILE_H__ */
