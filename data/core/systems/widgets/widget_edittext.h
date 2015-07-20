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


#ifndef __UNIGINE_WIDGET_EDITTEXT_H__
#define __UNIGINE_WIDGET_EDITTEXT_H__

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class EditText : Widget {
		
		WidgetEditText edittext;
		
		// constructor
		EditText(string str = 0) {
			if(str is WidgetEditText) {
				owner = 0;
				edittext = str;
			} else {
				edittext = new WidgetEditText(engine.getGui(),str);
			}
			widget = edittext;
		}
		
		// editable flag
		void setEditable(int editable) { edittext.setEditable(editable); }
		int isEditable() { return edittext.isEditable(); }
		
		// background flag
		void setBackground(int background) { edittext.setBackground(background); }
		int getBackground() { return edittext.getBackground(); }
		
		// token colors
		void setTokenColor(string token,vec4 color) { edittext.setTokenColor(token,color); }
		void setTokensColor(string tokens,vec4 color) { edittext.setTokensColor(tokens,color); }
		vec4 getTokenColor(string token) { return edittext.getTokenColor(token); }
		
		// cursor position
		void setCursor(int position,int line) { edittext.setCursor(position,line); }
		int getCursorPosition() { return edittext.getCursorPosition(); }
		int getCursorLine() { return edittext.getCursorLine(); }
		
		// selection position
		void setSelection(int position,int line) { edittext.setSelection(position,line); }
		int getSelectionPosition() { return edittext.getSelectionPosition(); }
		int getSelectionLine() { return edittext.getSelectionLine(); }
		
		// selection text
		string getSelectionText() { return edittext.getSelectionText(); }
		void clearSelectionText() { edittext.clearSelectionText(); }
		
		// lines
		int addLine(string str) { return edittext.addLine(str); }
		void removeLine(int line) { edittext.removeLine(line); }
		int getNumLines() { return edittext.getNumLines(); }
		void setLineText(int line,string str) { edittext.setLineText(line,str); }
		string getLineText(int line) { return edittext.getLineText(line); }
		
		// text
		void setText(string str) { edittext.setText(str); }
		string getText() { return edittext.getText(); }
	};
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_EDITTEXT_H__ */
