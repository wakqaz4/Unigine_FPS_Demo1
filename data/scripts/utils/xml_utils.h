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


#ifndef __XML_UTILS_H__
#define __XML_UTILS_H__

namespace Unigine {
	
	void xmlCopy(Xml dst,Xml src) {
		
		// copy arguments
		forloop(int i = 0; src.getNumArgs()) {
			string arg_name = src.getArgName(i);
			dst.setArg(arg_name,src.getArg(arg_name));
		}
		
		// copy children or data
		if(src.getNumChilds() > 0) {
			
			forloop(int i = 0; src.getNumChilds()) {
				
				Xml src_child = src.getChild(i);
				Xml dst_child = dst.addChild(src_child.getName());
				xmlCopy(dst_child,src_child);
				
			}
			
		} else {
			dst.setData(src.getData());
		}
		
	}
	
}

#endif
