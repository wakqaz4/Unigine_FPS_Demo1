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


#ifndef __PARAMETER_H__
#define __PARAMETER_H__

/*
 */
enum {
	
	PARAMETER_TYPE_VOID,
	PARAMETER_TYPE_INT,
	PARAMETER_TYPE_FLOAT,
	PARAMETER_TYPE_VEC3,
	PARAMETER_TYPE_VEC4,
	PARAMETER_TYPE_QUAT,
	PARAMETER_TYPE_STRING,
	
};

/******************************************************************************\
 *
 * namespace Unigine
 *
\******************************************************************************/

namespace Unigine {
	
	/**************************************************************************\
	 *
	 * class Parameter
	 *
	\**************************************************************************/
	
	/*
	 */
	class Parameter {
			
		public:
			
			//
			int getValue() { }
			
			//
			void setValue(int value) { }
			
			//
			int getType() { }
			
	};

	/******************************************************************************\
	 *
	 * class ParameterManager
	 *
	\******************************************************************************/

	/*
	 */
	class ParameterManager {
			
		private:
			
			string items[];
			
		public:
			
			//
			ParameterManager() {
			}
			
			//
			~ParameterManager() {
				items.clear();
			}
			
			//
			Parameter getItem(string id) {
				return items.check(id,NULL);
			}
			
			//
			int isItemExists(string id) {
				return items.check(id);
			}
			
			//
			void addItem(string id,Parameter item) {
				items.append(id,item);
			}
			
			//
			int getItemValue(string id) {
				Parameter item = items.check(id,NULL);
				if(item == NULL) throw("ParameterManager::getParameterValue(): item \"%s\" does not exist.",id);
				return item.getValue();
				
			}
			
			//
			void setItemValue(string id,int value) {
				Parameter item = items.check(id,NULL);
				if(item == NULL) throw("ValueManager::setParameterValue(): value \"%s\" does not exist.",id);
				item.setValue(value);
			}
			
			//
			int getNumItems() {
				return items.size();
			}
			
			//
			Parameter getItemByIndex(int index) {
				return items.get(index);
			}
			
	};
	
	/**************************************************************************\
	 *
	 * variables
	 *
	\**************************************************************************/
	
	//
	string parameter_type_to_str[0] = ("void","int","float","vec3","vec4","quat","string");
	
	/**************************************************************************\
	 *
	 * functions
	 *
	\**************************************************************************/
	
	//
	string typeToParameterStr(int type) {
		if(type < 0 || type >= parameter_type_to_str.size()) return parameter_type_to_str[0];
		return parameter_type_to_str[type];
	}
	
	//
	int strToParameterType(string str) {
		
		forloop(int i = 0; parameter_type_to_str.size()) {
			if(parameter_type_to_str[i] == str) {
				return i;
			}
		}
		
		return PARAMETER_TYPE_VOID;
	}
	
}

#endif /*__PARAMETER_H__*/
