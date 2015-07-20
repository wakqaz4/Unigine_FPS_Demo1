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


#ifndef __PROPERTY_UTILS_H__
#define __PROPERTY_UTILS_H__

//#define LOG_PROPERTY_TYPE_ERRORS;

namespace PropertyUtils {
	
	/*
	 */
	int getState(Property property,string state_name,int default_value = 0) {
		
		if(property == NULL) return default_value;
		
		int state_id = property.findState(state_name);
		if(state_id < 0) return default_value;
		
		return property.getState(state_id);
	}
	
	/*
	 */
	string getSwitchState(Property property,string state_name,string default_value = "") {
		
		if(property == NULL) return default_value;
		
		int state_id = property.findState(state_name);
		if(state_id < 0) return default_value;
		
		if(property.getStateType(state_id) != PROPERTY_STATE_SWITCH) {
			#ifdef LOG_PROPERTY_TYPE_ERRORS
				log.error("PropertyUtils::getSwitchState(): invalid state type: property name = %s state name = %s. Expected PROPERTY_STATE_SWITCH state type.\n",
						property.getName(),property.getStateName(state_id));
			#endif
			return default_value;
		}
		
		int value = property.getState(state_id);
		
		if(value >= property.getStateSwitchNumItems(state_id)) {
			#ifdef LOG_PROPERTY_TYPE_ERRORS
				log.error("PropertyUtils::getSwitchState(): invalid state value: property name = %s state name = %s. Switch value = %d not found.\n",
						property.getName(),property.getStateName(state_id),value);
			#endif
			return default_value;
		}
		return property.getStateSwitchItem(state_id,value);
	}
	
	/*
	 */
	string getParameterString(Property property,string param_name,string default_value = "") {
		
		if(property == NULL) return default_value;
		
		int param_id = property.findParameter(param_name);
		if(param_id < 0) return default_value;
		
		if(property.getParameterType(param_id) != PROPERTY_PARAMETER_STRING) {
			#ifdef LOG_PROPERTY_TYPE_ERRORS
				log.error("PropertyUtils::getParameterString(): invalid parameter type: property name = %s parameter name = %s. Expected PROPERTY_PARAMETER_STRING property type.\n",
						property.getName(),property.getParameterName(param_id));
			#endif
			return default_value;
		}
		
		return property.getParameterString(param_id);
	}
	
	/*
	 */
	float getParameterFloat(Property property,string param_name,float default_value = 0.0f) {
		
		if(property == NULL) return default_value;
		
		int param_id = property.findParameter(param_name);
		if(param_id < 0) return default_value;
		
		if(property.getParameterType(param_id) != PROPERTY_PARAMETER_FLOAT) {
			#ifdef LOG_PROPERTY_TYPE_ERRORS
				log.error("PropertyUtils::getParameterFloat(): invalid parameter type: property name = %s parameter name = %s. Expected PROPERTY_PARAMETER_FLOAT property type.\n",
						property.getName(),property.getParameterName(param_id));
			#endif
			return default_value;
		}
		
		return property.getParameterFloat(param_id);
	}
	
	/*
	 */
	int getParameterInt(Property property,string param_name,int default_value = 0) {
		
		if(property == NULL) return default_value;
		
		int param_id = property.findParameter(param_name);
		if(param_id < 0) return default_value;
		
		if(property.getParameterType(param_id) == PROPERTY_PARAMETER_SWITCH) {
			return property.getParameterSwitch(param_id);
		}
		
		if(property.getParameterType(param_id) == PROPERTY_PARAMETER_TOGGLE) {
			return property.getParameterToggle(param_id);
		}
		
		if(property.getParameterType(param_id) == PROPERTY_PARAMETER_INT) {
			return property.getParameterInt(param_id);
		}
		
		#ifdef LOG_PROPERTY_TYPE_ERRORS
			log.error("PropertyUtils::getParameterInt(): invalid parameter type: property name = %s parameter name = %s. Expected PROPERTY_PARAMETER_INT, PROPERTY_PARAMETER_SWITCH or PROPERTY_PARAMETER_TOGGLE property types.\n",
					property.getName(),property.getParameterName(param_id));
		#endif
		
		return default_value;
	}
	
	/*
	 */
	int getParameterMaxValue(Property property,string param_name,int default_value = 0) {
	
		if(property == NULL) return default_value;
		
		int param_id = property.findParameter(param_name);
		if(param_id < 0) return default_value;
		
		if(property.getParameterType(param_id) == PROPERTY_PARAMETER_INT) {
			return property.getParameterIntMaxValue(param_id);
		}
		
		if(property.getParameterType(param_id) == PROPERTY_PARAMETER_FLOAT) {
			return property.getParameterFloatMaxValue(param_id);
		}
		
		if(property.getParameterType(param_id) == PROPERTY_PARAMETER_DOUBLE) {
			return property.getParameterDoubleMaxValue(param_id);
		}		
		
		#ifdef LOG_PROPERTY_TYPE_ERRORS
			log.error("PropertyUtils::getParameterMaxValue(): invalid parameter type: property name = %s parameter name = %s. Expected PROPERTY_PARAMETER_INT, PROPERTY_PARAMETER_FLOAT or PROPERTY_PARAMETER_DOUBLE property types.\n",
					property.getName(),property.getParameterName(param_id));
		#endif
		
		return default_value;
	}
	
	/*
	 */
	int getParameterMinValue(Property property,string param_name,int default_value = 0) {
	
		if(property == NULL) return default_value;
		
		int param_id = property.findParameter(param_name);
		if(param_id < 0) return default_value;
		
		if(property.getParameterType(param_id) == PROPERTY_PARAMETER_INT) {
			return property.getParameterIntMinValue(param_id);
		}
		
		if(property.getParameterType(param_id) == PROPERTY_PARAMETER_FLOAT) {
			return property.getParameterFloatMinValue(param_id);
		}
		
		if(property.getParameterType(param_id) == PROPERTY_PARAMETER_DOUBLE) {
			return property.getParameterDoubleMinValue(param_id);
		}		
		
		#ifdef LOG_PROPERTY_TYPE_ERRORS
			log.error("PropertyUtils::getParameterMaxValue(): invalid parameter type: property name = %s parameter name = %s. Expected PROPERTY_PARAMETER_INT, PROPERTY_PARAMETER_FLOAT or PROPERTY_PARAMETER_DOUBLE property types.\n",
					property.getName(),property.getParameterName(param_id));
		#endif
		
		return default_value;
	}
};

#endif /* __PROPERTY_UTILS_H__ */
