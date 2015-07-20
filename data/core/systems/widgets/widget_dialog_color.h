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


#ifndef __UNIGINE_WIDGET_DIALOG_COLOR_H__
#define __UNIGINE_WIDGET_DIALOG_COLOR_H__

#include <core/scripts/callback.h>
#include <core/systems/widgets/widget_dialog.h>

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class DialogColor : Dialog {
		
		WidgetDialogColor dialog_color;
		
		// constructor
		DialogColor(string str = 0,int flags = 0) {
			dialog_color = new WidgetDialogColor(engine.getGui(),str);
			dialog = dialog_color;
			#ifdef HAS_INTERFACE
				interface = new InterfaceWindow(flags);
			#endif
			widget = dialog;
		}
		
		// path
		void setColor(vec4 color) { dialog_color.setColor(color); }
		vec4 getColor() { return dialog_color.getColor(); }
	};
	
	/*
	 */
	int dialogColor(string str,vec4 &color,CallbackBase callback = NULL) {
		
		DialogColor dialog;
		
		// create dialog
		if(dialog == NULL) {
			dialog = new DialogColor(str);
		}
		
		// dialog color
		dialog.setText(str);
		dialog.setColor(color);
		
		// show dialog
		dialog.arrange();
		addChild(dialog,ALIGN_OVERLAP | ALIGN_CENTER);
		dialog.setPermanentFocus();
		
		// wait dialog
		while(dialog.isDone() == 0) {
			if(callback != NULL) callback.run(dialog.getColor());
			wait;
		}
		
		// dialog result
		if(dialog.isOkClicked()) color = dialog.getColor();
		if(callback != NULL) callback.run(color);
		
		// hide dialog
		removeChild(dialog);
		
		return dialog.isOkClicked();
	}
	
} /* namespace Unigine::Widgets */

#endif /* __UNIGINE_WIDGET_DIALOG_COLOR_H__ */
