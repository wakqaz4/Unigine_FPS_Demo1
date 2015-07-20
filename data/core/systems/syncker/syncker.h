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


#ifndef __UNIGINE_SYNCKER_H__
#define __UNIGINE_SYNCKER_H__

/*
 */
namespace Unigine::Syncker {
	
	/*
	 */
	enum {
		
		VERSION = 0x0002,
		UDP_PORT = 8890,
		TCP_PORT = 8891,
		
		UDP_PACKET = 1024,		// udp packet size
		
		UDP_HELLO = 1,			// hello message
		UDP_FRAME,				// frame synchronization
		UDP_PLAYER,				// player synchronization
		UDP_RENDER,				// render synchronization
		UDP_MATERIAL,			// material synchronization
		UDP_NODE,				// node synchronization
		UDP_END,				// frame synchronization
		
		TCP_HELLO = 1,			// hello message
		TCP_BYE,				// bye command
		TCP_NODE_CREATE,		// create node command
		TCP_NODE_DELETE,		// delete node command
		TCP_SYSTEM_SET,			// set system remote variable
		TCP_SYSTEM_GET,			// get system remote variable
		TCP_SYSTEM_SYNC,		// call system remote function sync
		TCP_SYSTEM_ASYNC,		// call system remote function async
		TCP_SYSTEM_STREAM,		// call system remote function stream
		TCP_WORLD_SET,			// set world remote variable
		TCP_WORLD_GET,			// get world remote variable
		TCP_WORLD_SYNC,			// call world remote function sync
		TCP_WORLD_ASYNC,		// call world remote function async
		TCP_WORLD_STREAM,		// call world remote function stream
		TCP_EDITOR_SET,			// set editor remote variable
		TCP_EDITOR_GET,			// get editor remote variable
		TCP_EDITOR_SYNC,		// call editor remote function sync
		TCP_EDITOR_ASYNC,		// call editor remote function async
		TCP_EDITOR_STREAM,		// call editor remote function stream
	};
	
} /* namespace Unigine::Syncker */

#endif /* __UNIGINE_SYNCKER_H__ */
