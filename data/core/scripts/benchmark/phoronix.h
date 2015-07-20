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


#ifndef __UNIGINE_PHORONIX_H__
#define __UNIGINE_PHORONIX_H__

/*
 */
namespace Unigine {
	
	/*
	 */
	class Phoronix {
		
		int activity;				// benchmark status
		int first_frame;			// first frame status
		
		float time;					// total time
		int frames;					// total frames
		
		/*
		 */
		void run() {
			
			assert(activity == 0);
			activity = 1;
			
			log.message("Phoronix running\n");
			
			time = 0.0f;
			frames = 0;
		}
		
		void done() {
			
			assert(activity == 1);
			activity = 0;
			
			log.message("Phoronix finished\n");
			log.message("Time:\t%g\nFrames:\t%d\nFPS:\t%g\n",time,frames,frames / time);
		}
		
		int getActivity() {
			return activity;
		}
		
		/*
		 */
		void update() {
			
			if(activity == 0) return;
			
			// check the first frame flag
			if(engine.render.isFirstFrame()) {
				first_frame = 1;
				return;
			} else if(first_frame > 0) {
				first_frame--;
				return;
			}
			
			time += engine.app.getFTime();
			frames++;
			
			// hide mouse
			if(engine.system.call("systemGetActivity") == 0) {
				engine.gui.setMouseCursor(GUI_CURSOR_NONE);
			}
		}
	};
	
} /* namespace Unigine */

#endif /* __UNIGINE_PHORONIX_H__ */
