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


#ifndef __UNIGINE_WIDGET_TABLE_H__
#define __UNIGINE_WIDGET_TABLE_H__

#include <core/systems/widgets/widget_scrollbox.h>

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	enum {
		
		TABLE_ROW_CHANGED = 0,			// row changed callback
		TABLE_COLUMN_CHANGED,			// column chagned callback
		TABLE_NUM_CALLBACKS,
		
		TABLE_HEIGHT = 20,				// minimal row height
		TABLE_SPACE = 2,				// minimal row space
		
		TABLE_COLUMN_WIDTH = 16,		// column width
	};
	
	/*
	 */
	vec4 table_contour_color = vec4(0.0f,0.0f,0.0f,1.0f);
	vec4 table_background_color = vec4(0.15f,0.15f,0.15f,1.0f);
	vec4 table_selected_color = vec4(0.5f,0.5f,0.5f,1.0f);
	vec4 table_default_color = vec4(0.25f,0.25f,0.25f,1.0f);
	vec4 table_enabled_color = vec4(0.5f,0.5f,0.5f,1.0f);
	vec4 table_disabled_color = vec4(0.25f,0.25f,0.25f,1.0f);
	
	/**************************************************************************\
	*
	* TableRow
	*
	\**************************************************************************/
	
	/*
	 */
	class TableRow {
		
		Table table;				// table
		
		string text;				// text
		string swizzle;				// swizzle
		
		vec4 selected_color;		// selected color
		vec4 default_color;			// default color
		
		int offset;					// offset
		
		int num_columns;			// number of columns
		int value;					// value
		int mask;					// mask
		
		int data;					// row data
		
		int row_changed;			// row changed flag
		
		int text_id;				// text identifier
		int swizzle_id;				// swizzle identifier
		int contour_id;				// contour identifier
		int polygon_id;				// polygon identifier
		int column_text_id[0];		// column text identifiers
		int column_contour_id[0];	// column contour identifiers
		int column_polygon_id[0];	// column polygon identifiers
		
		WidgetCanvas canvas;
		
		/*
		 */
		TableRow(string t,int num,int v = 0,int m = ~0) {
			
			text = t;
			
			num_columns = num;
			value = v;
			mask = m;
			
			selected_color = table_selected_color;
			default_color = table_default_color;
			
			text_id = -1;
			swizzle_id = -1;
			contour_id = -1;
			polygon_id = -1;
			column_text_id.resize(num_columns);
			column_contour_id.resize(num_columns);
			column_polygon_id.resize(num_columns);
			forloop(int i = 0; num_columns) {
				column_text_id[i] = -1;
				column_contour_id[i] = -1;
				column_polygon_id[i] = -1;
			}
		}
		
		~TableRow() {
			
			if(canvas != NULL) {
				canvas.removeText(text_id);
				canvas.removeText(swizzle_id);
				canvas.removeLine(contour_id);
				canvas.removePolygon(polygon_id);
				forloop(int i = 0; num_columns) {
					canvas.removeText(column_text_id[i]);
					canvas.removeLine(column_contour_id[i]);
					canvas.removePolygon(column_polygon_id[i]);
				}
			}
		}
		
		/*
		 */
		void setTable(Table t) {
			table = t;
		}
		
		Table getTable() {
			return table;
		}
		
		/*
		 */
		void setText(string t) {
			text = t;
			row_changed = 1;
		}
		
		string getText() {
			return text;
		}
		
		/*
		 */
		void setSwizzle(string s) {
			swizzle = s;
			row_changed = 1;
		}
		
		string getSwizzle() {
			return swizzle;
		}
		
		/*
		 */
		void setColor(vec4 color) {
			selected_color = vec4(saturate(vec3(color) + vec3(0.25f)),1.0f);
			default_color = vec4(saturate(vec3(color) * 0.25f),1.0f);
			row_changed = 1;
		}
		
		vec4 getSelectedColor() {
			return selected_color;
		}
		
		vec4 getDefaultColor() {
			return default_color;
		}
		
		/*
		 */
		void setOffset(int o) {
			offset = o;
			row_changed = 1;
		}
		
		int getOffset() {
			return offset;
		}
		
		/*
		 */
		void setValue(int num,int v) {
			if(v) value |= (1 << num);
			else value &= ~(1 << num);
			row_changed = 1;
		}
		
		int getValue(int num) {
			return ((value & (1 << num)) != 0);
		}
		
		/*
		 */
		void setData(int d) {
			data = d;
		}
		
		int getData() {
			return data;
		}
		
		/*
		 */
		void setCanvas(WidgetCanvas c) {
			if(canvas != c) {
				canvas = c;
				text_id = canvas.addText(2);
				swizzle_id = canvas.addText(2);
				contour_id = canvas.addLine(1);
				polygon_id = canvas.addPolygon(0);
				canvas.setTextOutline(text_id,1);
				canvas.setTextOutline(swizzle_id,1);
				canvas.setLineColor(contour_id,table_contour_color);
				forloop(int i = 0; num_columns) {
					column_text_id[i] = canvas.addText(5);
					column_contour_id[i] = canvas.addLine(4);
					column_polygon_id[i] = canvas.addPolygon(3);
					canvas.setTextOutline(column_text_id[i],1);
					canvas.setLineColor(column_contour_id[i],table_contour_color);
				}
			}
		}
		
		/*
		 */
		int isPolygon(int id) {
			return (polygon_id == id);
		}
		
		int getColumn(int id) {
			return column_polygon_id.find(id);
		}
		
		/*
		 */
		void create(int position_y,TableRow current_row) {
			
			int width = canvas.getWidth();
			
			vec3 p0 = vec3(offset,position_y,0.0f);
			vec3 p1 = vec3(width - TABLE_COLUMN_WIDTH * num_columns,position_y,0.0f);
			vec3 p2 = vec3(width - TABLE_COLUMN_WIDTH * num_columns,position_y + TABLE_HEIGHT,0.0f);
			vec3 p3 = vec3(offset,position_y + TABLE_HEIGHT,0.0f);
			
			// create contour
			canvas.clearLinePoints(contour_id);
			canvas.addLinePoint(contour_id,p0);
			canvas.addLinePoint(contour_id,p1);
			canvas.addLinePoint(contour_id,p2);
			canvas.addLinePoint(contour_id,p3);
			canvas.addLinePoint(contour_id,p0);
			
			// create background
			canvas.clearPolygonPoints(polygon_id);
			if(current_row == this) canvas.setPolygonColor(polygon_id,selected_color);
			else canvas.setPolygonColor(polygon_id,default_color);
			canvas.addPolygonPoint(polygon_id,p0);
			canvas.addPolygonPoint(polygon_id,p1);
			canvas.addPolygonPoint(polygon_id,p2);
			canvas.addPolygonPoint(polygon_id,p3);
			
			// create text
			canvas.setTextText(text_id,text);
			int text_offset_y = (TABLE_HEIGHT - canvas.getTextHeight(text_id)) / 2;
			canvas.setTextPosition(text_id,vec3(canvas.getFontSize() / 2 + offset,position_y + text_offset_y,0.0f));
			
			// create swizzle
			canvas.setTextText(swizzle_id,swizzle);
			int text_offset_x = width - TABLE_COLUMN_WIDTH * num_columns - canvas.getFontSize() / 2 - canvas.getTextWidth(swizzle_id);
			canvas.setTextPosition(swizzle_id,vec3(text_offset_x,position_y + text_offset_y,0.0f));
			
			// create columns
			forloop(int i = 0; num_columns) {
				
				int text_id = column_text_id[i];
				int contour_id = column_contour_id[i];
				int polygon_id = column_polygon_id[i];
				
				int position_x = width - TABLE_COLUMN_WIDTH * (num_columns - i);
				
				p0 = vec3(position_x,position_y,0.0f);
				p1 = vec3(position_x + TABLE_COLUMN_WIDTH,position_y,0.0f);
				p2 = vec3(position_x + TABLE_COLUMN_WIDTH,position_y + TABLE_HEIGHT,0.0f);
				p3 = vec3(position_x,position_y + TABLE_HEIGHT,0.0f);
				
				// create column contour
				canvas.clearLinePoints(contour_id);
				canvas.addLinePoint(contour_id,p0);
				canvas.addLinePoint(contour_id,p1);
				canvas.addLinePoint(contour_id,p2);
				canvas.addLinePoint(contour_id,p3);
				canvas.addLinePoint(contour_id,p0);
				
				if(mask & (1 << i)) {
					
					// create column text
					canvas.setTextText(text_id,table.getColumn(i));
					text_offset_x = (TABLE_COLUMN_WIDTH - canvas.getTextWidth(text_id)) / 2;
					canvas.setTextPosition(text_id,vec3(position_x + text_offset_x,position_y + text_offset_y,0.0f));
					
					// create column polygon
					canvas.clearPolygonPoints(polygon_id);
					if(value & (1 << i)) canvas.setPolygonColor(polygon_id,table_enabled_color);
					else canvas.setPolygonColor(polygon_id,table_disabled_color);
					canvas.addPolygonPoint(polygon_id,p0);
					canvas.addPolygonPoint(polygon_id,p1);
					canvas.addPolygonPoint(polygon_id,p2);
					canvas.addPolygonPoint(polygon_id,p3);
				}
				
				else {
					
					// clear column text
					canvas.setTextText(text_id,"");
					
					// create column polygon
					canvas.clearPolygonPoints(polygon_id);
					canvas.setPolygonColor(polygon_id,table_background_color);
					canvas.addPolygonPoint(polygon_id,p0);
					canvas.addPolygonPoint(polygon_id,p1);
					canvas.addPolygonPoint(polygon_id,p2);
					canvas.addPolygonPoint(polygon_id,p3);
				}
			}
		}
	};
	
	/**************************************************************************\
	*
	* Table
	*
	\**************************************************************************/
	
	/*
	 */
	class Table : Widget {
		
		int width;						// table width
		
		string columns[0];				// columns
		
		TableRow rows[0];				// rows
		
		int pressed;					// pressed flag
		
		int rows_changed;				// rows changed flag
		
		TableRow current_row;			// current row
		
		ScrollBox canvas_sb;			// canvas scrollbox
		
		string callbacks[TABLE_NUM_CALLBACKS];
		int callbacks_data_0[TABLE_NUM_CALLBACKS];
		int callbacks_data_1[TABLE_NUM_CALLBACKS];
		int callbacks_enabled[TABLE_NUM_CALLBACKS];
		
		WidgetVBox vbox;
		WidgetCanvas canvas;
		
		/*
		 */
		Table(string c[] = ()) {
			
			columns.copy(c);
			
			// create canvas
			canvas_sb = new ScrollBox();
			vbox = new WidgetVBox(engine.getGui());
			canvas = new WidgetCanvas(engine.getGui());
			vbox.addChild(canvas_sb.widget,ALIGN_EXPAND);
			canvas_sb.widget.addChild(canvas,ALIGN_EXPAND);
			
			// canvas parameters
			canvas.setWidth(TABLE_COLUMN_WIDTH * columns.size());
			canvas.setHeight(TABLE_HEIGHT + TABLE_SPACE);
			canvas.setColor(table_background_color);
			
			// set callbacks
			canvas.setCallback(PRESSED,functionid(callback_redirector),this,functionid(pressed_callback));
			canvas.setCallback(RELEASED,functionid(callback_redirector),this,functionid(released_callback));
			canvas.setCallback(DOUBLE_CLICKED,functionid(callback_redirector),this,functionid(double_clicked_callback));
			
			widget = vbox;
		}
		
		~Table() {
			
			clear();
			
			delete canvas_sb;
			delete canvas;
		}
		
		/*
		 */
		ScrollBox getScrollBox() {
			return canvas_sb;
		}
		
		/*
		 */
		int isCanvasFocused() {
			return vbox.isChild(engine.gui.getFocus());
		}
		
		void clearCanvas() {
			forloop(int i = 0; canvas.getNumTexts()) {
				canvas.setTextText(canvas.getText(i),"");
			}
			forloop(int i = 0; canvas.getNumLines()) {
				canvas.clearLinePoints(canvas.getLine(i));
			}
			forloop(int i = 0; canvas.getNumPolygons()) {
				canvas.clearPolygonPoints(canvas.getPolygon(i));
			}
		}
		
		/*
		 */
		int getNumColumns() {
			return columns.size();
		}
		
		string getColumn(int num) {
			return columns[num];
		}
		
		/*
		 */
		void clear() {
			rows.delete();
		}
		
		/**********************************************************************\
		*
		* Rows
		*
		\**********************************************************************/
		
		/*
		 */
		void addRow(TableRow row) {
			if(rows.find(row) != -1) throw("Unigine::Widgets::Table::addRow(): row is already defined\n");
			row.setTable(this);
			row.setCanvas(canvas);
			rows.append(row);
			rows_changed = 1;
		}
		
		void removeRow(TableRow row) {
			int index = rows.find(row);
			if(index == -1) throw("Unigine::Widgets::Table::removeRow(): can't find row\n");
			rows.remove(index);
			rows_changed = 1;
			delete row;
		}
		
		int isRow(TableRow row) {
			return (rows.find(row) != -1);
		}
		
		/*
		 */
		int getNumRows() {
			return rows.size();
		}
		
		TableRow getRow(int num) {
			return rows[num];
		}
		
		/*
		 */
		void setCurrentRow(TableRow row) {
			if(current_row != row) {
				current_row = row;
				runCallback(TABLE_ROW_CHANGED);
				rows_changed = 1;
			}
		}
		
		TableRow getCurrentRow() {
			return current_row;
		}
		
		/**********************************************************************\
		*
		* Update
		*
		\**********************************************************************/
		
		/*
		 */
		void update() {
			
			// check canvas
			if(canvas == NULL) return;
			
			// canvas changed
			if(width != canvas.getWidth()) {
				width = canvas.getWidth();
				rows_changed = 1;
			}
			
			// rows changed
			if(rows_changed == 0) {
				foreach(TableRow row; rows) {
					rows_changed |= row.row_changed;
				}
			}
			
			// rows changed
			if(rows_changed) {
				canvas.setHeight((TABLE_HEIGHT + TABLE_SPACE) * (rows.size() + 1));
				forloop(int i = 0; rows.size()) {
					rows[i].create((TABLE_HEIGHT + TABLE_SPACE) * i,current_row);
				}
				rows_changed = 0;
			}
		}
		
		/**********************************************************************\
		*
		* Callbacks
		*
		\**********************************************************************/
		
		/*
		 */
		void setCallback(int callback,string function,int data_0 = 0,int data_1 = 0) {
			callbacks[callback] = function;
			callbacks_data_0[callback] = data_0;
			callbacks_data_1[callback] = data_1;
			callbacks_enabled[callback] = 1;
		}
		
		void runCallback(int callback) {
			string function = callbacks[callback];
			if(function != NULL && callbacks_enabled[callback]) {
				callbacks_enabled[callback] = 0;
				if(is_function(function,0)) call(function);
				else if(is_function(function,1)) call(function,callbacks_data_0[callback]);
				else if(is_function(function,2)) call(function,callbacks_data_0[callback],callbacks_data_1[callback]);
				else log.error("Unigine::Widgets::Table::runCallback(): can't find \"%s\" callback function\n",function);
				callbacks_enabled[callback] = 1;
			}
		}
		
		void setCallbackEnabled(int callback,int enable) {
			callbacks_enabled[callback] = enable;
		}
		
		/*
		 */
		void pressed_callback() {
			
			/////////////////////////////////
			// left mouse button
			/////////////////////////////////
			
			if(getMouseButton() & APP_BUTTON_LEFT) {
				
				int mouse_x = canvas.getMouseX();
				int mouse_y = canvas.getMouseY();
				
				if(pressed == 0) {
					
					pressed = 1;
					
					// polygon intersection
					int id = canvas.getPolygonIntersection(mouse_x,mouse_y);
					if(id != -1) {
						
						// find selected row
						foreach(TableRow row; rows) {
							if(row.isPolygon(id)) {
								setCurrentRow(row);
								pressed = 2;
								break;
							}
							int num = row.getColumn(id);
							if(num != -1) {
								row.setValue(num,!row.getValue(num));
								runCallback(TABLE_COLUMN_CHANGED);
								pressed = 2;
								break;
							}
						}
					}
				}
			}
			
			/////////////////////////////////
			// middle mouse button
			/////////////////////////////////
			
			if(getMouseButton() & APP_BUTTON_MIDDLE) {
				
				if(pressed == 0) {
					
					pressed = 3;
				}
				
				// move canvas
				if(pressed == 3) {
					
					int mouse_dy = getMouseDY();
					
					// move scrollbox
					canvas_sb.setVScrollValue(canvas_sb.getVScrollValue() - mouse_dy * canvas_sb.getScrollScale());
				}
			}
		}
		
		/*
		 */
		void released_callback() {
			pressed = 0;
		}
		
		/*
		 */
		void double_clicked_callback() {
			
			int mouse_x = canvas.getMouseX();
			int mouse_y = canvas.getMouseY();
			
			// polygon intersection
			int id = canvas.getPolygonIntersection(mouse_x,mouse_y);
			if(id != -1) {
						
				// find selected row
				foreach(TableRow row; rows) {
					int num = row.getColumn(id);
					if(num != -1) {
						row.setValue(num,!row.getValue(num));
						runCallback(TABLE_COLUMN_CHANGED);
						pressed = 2;
						break;
					}
				}
			}
			
			pressed = 0;
		}
		
		/*
		 */
		void callback_redirector(Table table,string name) {
			table.call(name);
		}
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_TABLE_H__ */
