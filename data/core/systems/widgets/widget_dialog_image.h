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


#ifndef __UNIGINE_WIDGET_DIALOG_IMAGE_H__
#define __UNIGINE_WIDGET_DIALOG_IMAGE_H__

#include <core/systems/widgets/widget_dialog.h>

/*
 */
namespace Unigine::Widgets {
	
	/*
	 */
	class DialogImage : Dialog {
		
		WidgetDialogImage dialog_image;
		
		// constructor
		DialogImage(string str = 0,int flags = 0) {
			dialog_image = new WidgetDialogImage(engine.getGui(),str);
			dialog = dialog_image;
			#ifdef HAS_INTERFACE
				interface = new InterfaceWindow(flags);
			#endif
			widget = dialog;
		}
		
		// texture
		void setImage(Image image) { dialog_image.setImage(image); }
		Image getImage() { return dialog_image.getImage(); }
		void setTexture(string name) { dialog_image.setTexture(name); }
		string getTexture() { return dialog_image.getTexture(); }
	};
	
	/*
	 */
	int dialogImage(string str,string name) {
		
		DialogImage dialog;
		
		// create dialog
		if(dialog == NULL) {
			dialog = new DialogImage(str);
			dialog.setSizeable(1);
		}
		
		// dialog texture
		dialog.setText(str);
		dialog.setTexture(name);
		
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

#endif /* __UNIGINE_WIDGET_DIALOG_IMAGE_H__ */
