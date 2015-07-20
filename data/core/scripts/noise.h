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


#ifndef __UNIGINE_NOISE_H__
#define __UNIGINE_NOISE_H__

/*
 */
namespace Unigine {
	
	/*
	 */
	class Noise {
		
		float scale;		// time scale
		float size;			// noise size
		int frequency;		// noise frequency
		float seed_1;		// float seed
		vec3 seed_3;		// vec3 seed
		vec4 seed_4;		// vec4 seed
		
		/*
		 */
		Noise(float scale,float size = 64.0f,int frequency = 64) {
			
			this.scale = scale;
			this.size = size;
			this.frequency = frequency;
			
			float seed = 100000.0f;
			seed_1 = engine.game.getRandom(-seed,seed);
			seed_3.x = engine.game.getRandom(-seed,seed);
			seed_3.y = engine.game.getRandom(-seed,seed);
			seed_3.z = engine.game.getRandom(-seed,seed);
			seed_4.x = engine.game.getRandom(-seed,seed);
			seed_4.y = engine.game.getRandom(-seed,seed);
			seed_4.z = engine.game.getRandom(-seed,seed);
			seed_4.w = engine.game.getRandom(-seed,seed);
		}
		
		/*
		 */
		float get1() {
			float pos = (engine.game.getTime() * scale) + seed_1;
			return engine.game.getNoise(pos,size,frequency);
		}
		
		vec3 get3() {
			vec3 pos = vec3(engine.game.getTime() * scale) + seed_3;
			return vec3(engine.game.getNoise(pos.x,size,frequency),
				engine.game.getNoise(pos.y,size,frequency),
				engine.game.getNoise(pos.z,size,frequency));
		}
		
		vec4 get4() {
			vec4 pos = vec4(engine.game.getTime() * scale) + seed_4;
			return vec4(engine.game.getNoise(pos.x,size,frequency),
				engine.game.getNoise(pos.y,size,frequency),
				engine.game.getNoise(pos.z,size,frequency),
				engine.game.getNoise(pos.w,size,frequency));
		}
		
		/*
		 */
		float get1(float offset) {
			float pos = (engine.game.getTime() * scale + offset) + seed_1;
			return engine.game.getNoise(pos,size,frequency);
		}
		
		vec3 get3(float offset) {
			vec3 pos = vec3(engine.game.getTime() * scale + offset) + seed_3;
			return vec3(engine.game.getNoise(pos.x,size,frequency),
				engine.game.getNoise(pos.y,size,frequency),
				engine.game.getNoise(pos.z,size,frequency));
		}
		
		vec4 get4(float offset) {
			vec4 pos = vec4(engine.game.getTime() * scale + offset) + seed_4;
			return vec4(engine.game.getNoise(pos.x,size,frequency),
				engine.game.getNoise(pos.y,size,frequency),
				engine.game.getNoise(pos.z,size,frequency),
				engine.game.getNoise(pos.w,size,frequency));
		}
	};
	
} /* namespace Unigine */

#endif /* __UNIGINE_NOISE_H__ */
