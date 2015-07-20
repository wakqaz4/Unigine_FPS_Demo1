/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    plugin.cpp
 * Desc:    Plugin sample
 * Version: 1.01
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

/******************************************************************************\
*
* Plugin
*
\******************************************************************************/

/*
 */
string getName() {
	return "Plugin";
}

/*
 */
void init(PluginMeta meta) {
	
	// plugin initialization callback
	log.message("%s: init(%s): called\n",getName(),meta.name);
}

void shutdown() {
	
	// plugin shutdown callback
	log.message("%s: shutdown(): called\n",getName());
}

void save() {
	
	// plugin world save callback
	log.message("%s: save(): called\n",getName());
}

void update(int need_reload) {
	
	// plugin update callback
}

/******************************************************************************\
*
* Node callbacks
*
\******************************************************************************/

/*
 */
WidgetLabel plugin_l;

/*
 */
void nodeInit() {
	
	// plugin node initialization callback
	log.message("%s: nodeInit(): called\n",getName());
	
	// create plugin label
	plugin_l = new WidgetLabel(engine.getGui());
	
	// create plugin tab
	::Nodes::parameters_tb.addTab(getName());
	::Nodes::parameters_tb.addChild(plugin_l,GUI_ALIGN_TOP);
}

void nodeUpdate() {
	
	// plugin node update callback
	log.message("%s: nodeUpdate(): called\n",getName());
	
	// update plugin label
	Node node = ::Nodes::node;
	plugin_l.setText(format("%s: 0x%08x",node.getTypeName(),node.getID()));
}

void nodeShutdown() {
	
	// plugin node shutdown callback
	log.message("%s: nodeShutdown(): called\n",getName());
	
	// delete plugin label
	delete plugin_l;
}

/*
 */
void nodesUpdate() {
	
	// plugin nodes update callback
	log.message("%s: nodesUpdate(): called\n",getName());
}
