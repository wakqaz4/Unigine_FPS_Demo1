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


#ifndef __UNIGINE_SCHEMER_ANCHOR_H__
#define __UNIGINE_SCHEMER_ANCHOR_H__

/*
 */
namespace Unigine::Schemer {
	
	/*
	 */
	class SchemerAnchor {
		
		int id;						// identifier
		
		string name;				// name
		string label;				// label
		string align;				// align
		string value;				// value
		
		int mask;					// joint mask
		int edit;					// editable flag
		
		/*
		 */
		SchemerAnchor(int i,string n,string a = "") {
			
			id = i;
			name = n;
			label = n;
			align = a;
			
			value = "";
			mask = 0x01;
			edit = 0;
		}
		
		/*
		 */
		int getID() {
			return id;
		}
		
		/*
		 */
		string getName() {
			return name;
		}
		
		string getLabel() {
			return label;
		}
		
		string getAlign() {
			return align;
		}
		
		string getValue() {
			return value;
		}
		
		/*
		 */
		int getMask() {
			return mask;
		}
		
		int getEdit() {
			return edit;
		}
		
		/*
		 */
		int load(Xml xml) {
			
			// anchor label
			if(xml.isArg("label")) label = xml.getArg("label");
			
			// anchor align
			if(xml.isArg("align")) align = xml.getArg("align");
			
			// default value
			if(xml.isArg("value")) value = xml.getArg("value");
			
			// joint mask
			if(xml.isArg("mask")) mask = xml.getIntArg("mask");
			
			// editable flag
			if(xml.isArg("edit")) edit = xml.getBoolArg("edit");
			
			return 1;
		}
	};
	
} /* namespace Unigine::Schemer */

#endif /* __UNIGINE_SCHEMER_ANCHOR_H__ */
