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


#ifdef _ANDROID || _IOS
	#define MOUSE_SOFT
#endif

#include <core/unigine.h>
#include <core/scripts/system/system.h>
#include <core/scripts/system/stereo.h>
#include <core/scripts/system/wall.h>

/******************************************************************************\
*
* init/shutdown
*
\******************************************************************************/

/*
 */
int init() {
	
	systemInit();
	stereoInit();
	wallInit();
	
	return 1;
}

/*
 */
int shutdown() {
	
	systemShutdown();
	stereoShutdown();
	wallShutdown();
	
	return 1;
}

/******************************************************************************\
*
* update/render
*
\******************************************************************************/

/*
 */
int update() {
	
	systemUpdate();
	stereoUpdate();
	
	return 1;
}

/*
 */
int render() {
	
	stereoRender();
	wallRender();
	
	return 1;
}
