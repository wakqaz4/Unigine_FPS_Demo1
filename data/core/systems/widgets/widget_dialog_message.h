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


#ifndef __UNIGINE_WIDGET_DIALOG_MESSAGE_H__
#define __UNIGINE_WIDGET_DIALOG_MESSAGE_H__

#include <core/systems/widgets/widget_dialog.h>

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class DialogMessage : Dialog {
		
		WidgetDialogMessage dialog_message;
		
		// constructor
		DialogMessage(string str = 0,int flags = 0) {
			dialog_message = new WidgetDialogMessage(engine.getGui(),str);
			dialog = dialog_message;
			#ifdef HAS_INTERFACE
				interface = new InterfaceWindow(flags);
			#endif
			widget = dialog;
		}
		
		// message hidden
		void setMessageHidden(int hidden) { dialog_message.setMessageHidden(hidden); }
		int isMessageHidden() { return dialog_message.isMessageHidden(); }
		
		// message font
		void setMessageFont(string name) { dialog_message.setMessageFont(name); }
		void setMessageFontSize(int size) { dialog_message.setMessageFontSize(size); }
		void setMessageFontColor(vec4 color) { dialog_message.setMessageFontColor(color); }
		void setMessageFontRich(int rich) { dialog_message.setMessageFontRich(rich); }
		
		// message text
		void setMessageText(string str) { dialog_message.setMessageText(str); }
		string getMessageText() { return dialog_message.getMessageText(); }
	};
	
	/*
	 */
	int dialogMessage(string str,string text) {
		
		DialogMessage dialog;
		
		// create dialog
		if(dialog == NULL) {
			dialog = new DialogMessage(str);
		}
		
		// dialog message
		dialog.setText(str);
		dialog.setMessageText(text);
		
		// show dialog
		dialog.arrange();
		addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
		dialog.setPermanentFocus();
		
		// wait dialog
		while(dialog.isDone() == 0) wait;
		
		// hide dialog
		removeChild(dialog);
		
		return dialog.isOkClicked();
	}
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_DIALOG_MESSAGE_H__ */
