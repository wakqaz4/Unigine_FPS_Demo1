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


#ifndef __COLORS_H__
#define __COLORS_H__

namespace Color {
	
	// Basic
	vec4 COLOR_RED = vec4(1.0f,0.0f,0.0f,1.0f);
	
	// Greens
	vec4 COLOR_SEA_GREEN = vec4(0.18f,0.74f,0.56f,1.0f);
	
	// Browns
	vec4 COLOR_INDIAN_RED = vec4(0.8f,0.36f,0.36f,1.0f);
	
	// Oranges
	vec4 COLOR_CORAL = vec4(1.0f,0.5f,0.31f,1.0f);
	
	// Blues
	vec4 COLOR_CORNFLOWER_BLUE = vec4(0.39f,0.58f,0.93f,1.0f);
	
	// Pinks/Violets
	vec4 COLOR_MEDIUM_PURPLE = vec4(0.58f,0.44f,0.86f,1.0f);
	
	// Whites/Pastels
	vec4 COLOR_PEACH_PUFF = vec4(1.0f,0.85f,0.73f,1.0f);
	
	/// To suppress warnings.
	vec4 color;
	color = COLOR_RED;
	color = COLOR_SEA_GREEN;
	color = COLOR_INDIAN_RED;
	color = COLOR_CORAL;
	color = COLOR_CORNFLOWER_BLUE;
	color = COLOR_MEDIUM_PURPLE;
	color = COLOR_PEACH_PUFF;
};

#endif /* __COLORS_H__ */

