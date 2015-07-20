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


#ifndef __UNIGINE_SKINNER_EDITOR_H__
#define __UNIGINE_SKINNER_EDITOR_H__

#include <core/systems/skinner/skinner.h>
#include <core/systems/schemer/editor/schemer_editor.h>
#include <core/systems/skinner/editor/skinner_editor_parameter_bone.h>

/*
 */
namespace Unigine::Skinner {
	
	/*
	 */
	vec4 link_color = vec4(1.0f,1.0f,0.0f,1.0f);
	
	/**************************************************************************\
	*
	* SkinnerEditor
	*
	\**************************************************************************/
	
	/*
	 */
	class SkinnerEditor {
		
		using Unigine::Schemer;
		using Unigine::Widgets;
		
		Skinner skinner;
		
		SchemerEditor editor;
		
		/*
		 */
		SkinnerEditor(Skinner s,ObjectMeshSkinned mesh) {
			
			skinner = s;
			
			// create editor
			editor = new SchemerEditor(skinner.getSchemer());
			
			// set mesh
			setMesh(mesh);
			
			// set link colors
			editor.setLinkColor(128,link_color);
		}
		
		~SkinnerEditor() {
			delete editor;
		}
		
		/*
		 */
		Widget getWidget() {
			return editor.getWidget();
		}
		
		void arrange() {
			editor.arrange();
		}
		
		/*
		 */
		void setMesh(ObjectMeshSkinned mesh) {
			editor.addParameter("skinner.bone","Unigine::Skinner::SkinnerEditorParameterBone",mesh);
		}
		
		/*
		 */
		void clear() {
			editor.clear();
		}
		
		/*
		 */
		int loadScript(string name) {
			return editor.loadScript(name);
		}
		
		int saveScript(string name) {
			return editor.saveScript(name);
		}
		
		/*
		 */
		int setScript(SchemerScript script) {
			return editor.setScript(script);
		}
		
		SchemerScript getScript() {
			return editor.getScript();
		}
	};
	
} /* namespace Unigine::Skinner */

#endif /* __UNIGINE_SKINNER_EDITOR_H__ */
