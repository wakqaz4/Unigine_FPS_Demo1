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


#ifndef __UNIGINE_SKINNER_MESH_H__
#define __UNIGINE_SKINNER_MESH_H__

/*
 */
namespace Unigine::Skinner {
	
	/*
	 */
	class SkinnerMesh {
		
		using Unigine::Schemer;
		
		Skinner skinner;
		
		ObjectMeshSkinned mesh;
		
		SchemerScript script;
		int update_id;
		int render_id;
		
		/*
		 */
		SkinnerMesh(Skinner s,ObjectMeshSkinned m) {
			
			skinner = s;
			mesh = m;
		}
		
		~SkinnerMesh() {
			
			clear();
		}
		
		/*
		 */
		Skinner getSkinner() {
			return skinner;
		}
		
		/*
		 */
		void setMesh(ObjectMeshSkinned m) {
			mesh = m;
		}
		
		ObjectMeshSkinned getMesh() {
			return mesh;
		}
		
		/*
		 */
		void clear() {
			
			// shutdown script
			if(script != NULL) {
				script.shutdown();
			}
			
			delete script;
		}
		
		/**********************************************************************\
		*
		* Load script
		*
		\**********************************************************************/
		
		/*
		 */
		int loadScript(string name) {
			
			// load script
			SchemerScript script = skinner.loadScript(name);
			if(script == NULL) {
				log.error("Unigine::Skinner::SkinnerMesh::loadScript(): can't load \"%s\" script\n",name);
				return 0;
			}
			
			// set script
			return setScript(script);
		}
		
		/**********************************************************************\
		*
		* Scripts
		*
		\**********************************************************************/
		
		/*
		 */
		int setScript(SchemerScript s) {
			
			clear();
			
			// clone script
			script = s.clone();
			
			// common source
			string source = "float ifps;\n";
			source += "ObjectMeshSkinned mesh;\n";
			script.setCommonSource(source);
			
			// init source
			source = "\tmesh.setNumLayers(1);\n";
			script.setInitSource(source);
			
			// compile script
			if(script.compile() == 0) {
				log.error("Unigine::Skinner::SkinnerMesh::setScript(): can't compile script\n");
				delete script;
				return 0;
			}
			
			// set variables
			script.setVariable("ifps",0.0f);
			script.setVariable("mesh",mesh);
			
			// get entries
			update_id = script.getEntryID("update");
			render_id = script.getEntryID("render");
			if(update_id == -1 && render_id == -1) {
				log.error("Unigine::Skinner::SkinnerMesh::setScript(): can't find entry\n");
				delete script;
				return 0;
			}
			
			// initialize script
			script.init();
			
			return 1;
		}
		
		/*
		 */
		SchemerScript getScript() {
			return script;
		}
		
		/**********************************************************************\
		*
		* Parameters
		*
		\**********************************************************************/
		
		/*
		 */
		int getInputID(string name) {
			return script.getInputID(name);
		}
		
		void setInput(int id,int value) {
			script.setInput(id,value);
		}
		
		/*
		 */
		int getOutputID(string name) {
			return script.getOutputID(name);
		}
		
		int getOutput(int id) {
			return script.getOutput(id);
		}
		
		/*
		 */
		int getEventID(string name) {
			return script.getEntryID(name);
		}
		
		void event(int id) {
			script.update(id);
		}
		
		/*
		 */
		void update(float ifps) {
			script.setVariable("ifps",ifps);
			if(update_id != -1) script.update(update_id);
		}
		
		void render(float ifps) {
			script.setVariable("ifps",ifps);
			if(render_id != -1) script.update(render_id);
		}
	};
	
} /* namespace Unigine::Skinner */

#endif /* __UNIGINE_SKINNER_MESH_H__ */
