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


#ifndef __CHARACTER_ATTACHMENTS_H__
#define __CHARACTER_ATTACHMENTS_H__

#include <unigine.h>

/// @note: this Attachment implementation has the following bug:
/// when locator is moved rather fast attachment can lag behind its position.
/// This is happened because Attachment is updated inside script update before engine update
/// and locator is updated inside engine update
/// Use WorldTransformBone to fix this.
namespace Unigine {

	/******************************************************************************\
	 *
	 * class Attachment
	 *
	\******************************************************************************/
	
	/*
	 */
	class Attachment {
		
		Locator locator;
		Node node;
		string node_file_name;
		
		// constructor
		Attachment(Locator locator_,Node node_ = NULL) {
			locator = locator_;
			node = node_;
		}
		
		~Attachment() {
			node_delete(node);
		}
		
		//
		void update() {
			if(locator == NULL || node == NULL) return;
			node.setWorldTransform(locator.getWorldTransform());
		}
		
		//
		void setNodeFile(string file_name) {
			if(node != NULL) node_delete(node);
			node_file_name = file_name;
			node = node_load(node_file_name);
		}
		
		//
		string getNodeFile() {
			return node_file_name;
		}
		
		//
		string getLocatorName() {
			return locator.getName();
		}
		
		Node getNode() {
			return node;
		}
		
		void setEnabled(int enabled) {
			node.setEnabled(enabled);
		}
		
		int isEnabled() {
			return node.isEnabled();
		}
	};
	
	/******************************************************************************\
	 *
	 * class AttachmentManager
	 *
	\******************************************************************************/
	
	/*
	 */
	class AttachmentManager {
		
		LocatorManager locators;
		Attachment attachments[];
		
		//
		AttachmentManager(LocatorManager locators_) {
			locators = locators_;
		}
		
		~AttachmentManager() {
			clear();
		}
		
		//
		void clear() {
			attachments.delete();
		}
		
		//
		void load(Xml xml) {
			
			clear();
			
			if(xml.isChild("attachments") == 0) return;
			Xml node = xml.getChild("attachments");
			
			forloop(int i = 0; node.getNumChilds()) {
				
				Xml child = node.getChild(i);
				if(child.getName() != "attachment") continue;
				Locator locator = NULL;
				
				if(child.isArg("locator")) {
					locator = locators.getLocator(child.getArg("locator"));
				}
				
				if(locator == NULL) throw("AttachmentManager::load(): attachment locator does not exist.");
				
				Attachment attachment = new Attachment(locator);
				attachment.setNodeFile(child.getData());
				attachments.append(child.getArg("name"),attachment);
			}
		}
		
		//
		void save(Xml xml) {
			
			if(attachments.size() == 0) return;
			Xml node = xml.addChild("attachments");
			
			foreachkey(string attachment_id; attachments) {
				
				Attachment attachment = attachments[attachment_id];
				
				Xml child = node.addChild("attachment");
				child.setArg("name",attachment_id);
				child.setArg("locator",attachment.getLocatorName());
				child.setData(attachment.getNodeFile());
			}
		}
		
		//
		Attachment getAttachment(string id) {
			return attachments[id];
		}
		
		//
		void update() {
			foreach(Attachment attachment; attachments) {
				attachment.update();
			}
		}
		
		void setEnabled(int enabled) {
			foreach(Attachment attachment; attachments) {
				attachment.setEnabled(enabled);
			}
		}
	};
}

#endif
