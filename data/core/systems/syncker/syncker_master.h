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


#ifndef __UNIGINE_SYNCKER_MASTER_H__
#define __UNIGINE_SYNCKER_MASTER_H__

#include <core/systems/syncker/syncker.h>
#include <core/systems/syncker/syncker_serializer.h>

/*
 */
namespace Unigine::Syncker {
	
	/**************************************************************************\
	*
	* Master
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Master {
		
		/*
		 */
		enum {
			
			QUALITY = 1,
			
			SEND_SIZE = 65536,
			
			ACCEPT_TIMEOUT = 1.0f,
			BYE_TIMEOUT = 1.0f,
			
			HELLO_COUNT = 8,
			LISTEN_QUEUE = 32,
		};
		
		/*
		 */
		Socket udp_socket;		// udp broadcast socket
		Socket tcp_socket;		// tcp server socket
		
		/*
		 */
		Blob temp_blob;			// temp blob
		Blob data_blob;			// data blob
		
		/*
		 */
		int udp_frame;			// udp frame number
		
		/*
		 */
		class Slave {
			string name;		// slave name
			Socket tcp_socket;	// slave socket
		};
		
		Slave slaves[0];		// slaves
		
		/**********************************************************************\
		*
		* Init/shutdown
		*
		\**********************************************************************/
		
		/*
		 */
		int init(string address,int udp_port = Unigine::Syncker::UDP_PORT,int tcp_port = Unigine::Syncker::TCP_PORT) {
			
			log.message("\n---- Syncker::Master ----\n");
			log.message("Address: %s\n",address);
			log.message("UDP:     %d\n",udp_port);
			log.message("TCP:     %d\n",tcp_port);
			
			// create blobs
			temp_blob = new Blob();
			data_blob = new Blob();
			
			// create udp broadcast socket
			udp_socket = new Socket(SOCKET_DGRAM);
			if(udp_socket.open(address,udp_port) == 0 || udp_socket.send(SEND_SIZE) == 0 || udp_socket.broadcast() == 0 || udp_socket.nonblock() == 0) {
				log.error("Unigine::Syncker::Master::init(): can't create udp broadcast socket\n");
				if(udp_socket.isOpened()) udp_socket.close();
				delete udp_socket;
				return 0;
			}
			
			// create tcp server socket
			tcp_socket = new Socket(SOCKET_STREAM);
			if(tcp_socket.open(tcp_port) == 0 || tcp_socket.send(SEND_SIZE) == 0 || tcp_socket.bind() == 0 || tcp_socket.listen(LISTEN_QUEUE) == 0 || tcp_socket.nonblock() == 0) {
				log.error("Unigine::Syncker::Master::init(): can't create tcp server socket\n");
				if(tcp_socket.isOpened()) tcp_socket.close();
				udp_socket.close();
				delete udp_socket;
				delete tcp_socket;
				return 0;
			}
			
			// send master hello
			data_blob.clear();
			data_blob.writeInt(VERSION);
			forloop(int i = 0; HELLO_COUNT) {
				udp_packet(UDP_HELLO,data_blob);
			}
			
			// connect slaves
			float begin = clock();
			Socket socket = NULL;
			while(clock() - begin < ACCEPT_TIMEOUT) {
				if(socket == NULL) socket = new Socket(SOCKET_STREAM);
				if(tcp_socket.accept(socket) == 0) continue;
				
				// block socket
				if(socket.block() == 0) {
					log.warning("Unigine::Syncker::Master::init(): can't block socket\n");
					socket.close();
					delete socket;
					continue;
				}
				
				// check slave hello
				if(socket.readChar() != TCP_HELLO) {
					log.warning("Unigine::Syncker::Master::init(): wrong hello from \"%s\"\n",socket.getHost());
					socket.close();
					delete socket;
					continue;
				}
				
				// check slave version
				if(socket.readInt() != VERSION) {
					log.warning("Unigine::Syncker::Master::init(): wrong version from \"%s\"\n",socket.getHost());
					socket.close();
					delete socket;
					continue;
				}
				
				// slave name
				Slave slave = new Slave();
				slave.name = socket.readString();
				slave.tcp_socket = socket;
				
				log.message("Connection from \"%s\" \"%s\" accepted in %.2f seconds\n",socket.getHost(),slave.name,clock() - begin);
				
				slaves.append(slave);
				socket = NULL;
			}
			delete socket;
			
			return 1;
		}
		
		/*
		 */
		int shutdown() {
			
			// disconnect slaves
			foreach(Slave slave; slaves) {
				tcp_bye(slave.tcp_socket);
			}
			slaves.delete();
			
			// delete blobs
			delete temp_blob;
			delete data_blob;
			
			// delete sockets
			if(udp_socket != NULL && udp_socket.isOpened()) udp_socket.close();
			if(tcp_socket != NULL && tcp_socket.isOpened()) tcp_socket.close();
			delete udp_socket;
			delete tcp_socket;
		}
		
		/*
		 */
		void tcp_bye(Socket socket) {
			float begin = clock();
			socket.writeChar(TCP_BYE);
			socket.nonblock();
			while(clock() - begin < BYE_TIMEOUT) {
				if(socket.readChar() == TCP_BYE) {
					log.message("Connection to \"%s\" closed in %.2f seconds\n",socket.getHost(),clock() - begin);
					break;
				}
			}
			socket.close();
			delete socket;
		}
		
		/*
		 */
		void udp_packet(int command,Blob blob) {
			temp_blob.clear();
			temp_blob.writeChar(command);
			temp_blob.writeShort(udp_frame);
			blob.compress(temp_blob,QUALITY);
			temp_blob.seekSet(0);
			udp_socket.writeStream(temp_blob,temp_blob.getSize());
			if(++udp_frame >= 0x7fff) udp_frame = 0;
		}
		
		/**********************************************************************\
		*
		* Slaves
		*
		\**********************************************************************/
		
		/*
		 */
		int getNumSlaves() {
			return slaves.size();
		}
		
		string getSlaveName(int num) {
			return slaves[num].name;
		}
		
		string getSlaveAddress(int num) {
			return slaves[num].tcp_socket.getHost();
		}
		
		/**********************************************************************\
		*
		* Frame synchronization
		*
		\**********************************************************************/
		
		/*
		 */
		void syncFrame() {
			data_blob.clear();
			Serializer::saveFrame(data_blob);
			udp_packet(UDP_FRAME,data_blob);
		}
		
		/*
		 */
		void syncEnd() {
			data_blob.clear();
			udp_packet(UDP_END,data_blob);
		}
		
		/**********************************************************************\
		*
		* Player synchronization
		*
		\**********************************************************************/
		
		/*
		 */
		void syncPlayer() {
			Player player = engine.editor.getPlayer();
			if(player == NULL) player = engine.game.getPlayer();
			if(player == NULL) return;
			data_blob.clear();
			Serializer::savePlayer(player,data_blob);
			udp_packet(UDP_PLAYER,data_blob);
		}
		
		/**********************************************************************\
		*
		* Render synchronization
		*
		\**********************************************************************/
		
		/*
		 */
		void syncRender() {
			data_blob.clear();
			Serializer::saveRender(data_blob);
			udp_packet(UDP_RENDER,data_blob);
		}
		
		/**********************************************************************\
		*
		* Material synchronization
		*
		\**********************************************************************/
		
		/*
		 */
		void syncMaterial(Material material) {
			data_blob.clear();
			data_blob.writeString(material.getName());
			Serializer::saveMaterial(material,data_blob);
			udp_packet(UDP_MATERIAL,data_blob);
		}
		
		/*
		 */
		void syncMaterials(Material materials[]) {
			for(int i = 0; i < materials.size();) {
				data_blob.clear();
				for(; i < materials.size(); i++) {
					Material material = materials[i];
					int size = data_blob.getSize();
					data_blob.writeString(material.getName());
					Serializer::saveMaterial(material,data_blob);
					if(data_blob.getSize() > UDP_PACKET) {
						temp_blob.clear();
						temp_blob.writeChar(UDP_MATERIAL);
						temp_blob.writeShort(udp_frame);
						data_blob.compress(temp_blob,QUALITY);
						if(temp_blob.getSize() <= UDP_PACKET) {
							data_blob.clear();
							temp_blob.seekSet(0);
							udp_socket.writeStream(temp_blob,temp_blob.getSize());
							if(++udp_frame >= 0x7fff) udp_frame = 0;
							i++;
						} else {
							data_blob.resize(size);
							if(size == 0) materials.remove(i);
						}
						break;
					}
				}
				if(data_blob.getSize()) udp_packet(UDP_MATERIAL,data_blob);
			}
		}
		
		/**********************************************************************\
		*
		* Node synchronization
		*
		\**********************************************************************/
		
		/*
		 */
		void createNode(Node node) {
			data_blob.clear();
			data_blob.writeInt(node.getID());
			if(node.getParent() == NULL) data_blob.writeInt(node.getID());
			else data_blob.writeInt(Node(node.getParent()).getID());
			Serializer::sendNode(node,data_blob);
			temp_blob.clear();
			temp_blob.writeChar(TCP_NODE_CREATE);
			data_blob.compress(temp_blob,QUALITY);
			foreach(Slave slave; slaves) {
				temp_blob.seekSet(0);
				slave.tcp_socket.writeStream(temp_blob,temp_blob.getSize());
			}
		}
		
		/*
		 */
		void deleteNode(Node node) {
			data_blob.clear();
			data_blob.writeChar(TCP_NODE_DELETE);
			data_blob.writeInt(node.getID());
			data_blob.writeChar(node.getType());
			foreach(Slave slave; slaves) {
				data_blob.seekSet(0);
				slave.tcp_socket.writeStream(data_blob,data_blob.getSize());
			}
		}
		
		/*
		 */
		void syncNode(Node node) {
			data_blob.clear();
			data_blob.writeInt(node.getID());
			data_blob.writeChar(node.getType());
			Serializer::saveNode(node,data_blob);
			udp_packet(UDP_NODE,data_blob);
		}
		
		/*
		 */
		void syncNodes(Node nodes[]) {
			for(int i = 0; i < nodes.size();) {
				data_blob.clear();
				for(; i < nodes.size(); i++) {
					Node node = nodes[i];
					int size = data_blob.getSize();
					data_blob.writeInt(node.getID());
					data_blob.writeChar(node.getType());
					Serializer::saveNode(node,data_blob);
					if(data_blob.getSize() > UDP_PACKET) {
						temp_blob.clear();
						temp_blob.writeChar(UDP_NODE);
						temp_blob.writeShort(udp_frame);
						data_blob.compress(temp_blob,QUALITY);
						if(temp_blob.getSize() <= UDP_PACKET) {
							data_blob.clear();
							temp_blob.seekSet(0);
							udp_socket.writeStream(temp_blob,temp_blob.getSize());
							if(++udp_frame >= 0x7fff) udp_frame = 0;
							i++;
						} else {
							data_blob.resize(size);
							if(size == 0) nodes.remove(i);
						}
						break;
					}
				}
				if(data_blob.getSize()) udp_packet(UDP_NODE,data_blob);
			}
		}
		
		/**********************************************************************\
		*
		* Remote procedurals
		*
		\**********************************************************************/
		
		/*
		 */
		void systemSet(int mask,string name,int value) {
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_set(slaves[i].tcp_socket,TCP_SYSTEM_SET,name,value);
			}
		}
		
		void systemGet(int mask,string name,int ret[]) {
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_get(slaves[i].tcp_socket,TCP_SYSTEM_GET,name);
			}
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				ret.append(slaves[i].tcp_socket.readVariable());
			}
		}
		
		void systemCall(int mask,string name,int args[],int ret[]) {
			ret.clear();
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_call(slaves[i].tcp_socket,TCP_SYSTEM_SYNC,name,args);
			}
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				ret.append(slaves[i].tcp_socket.readVariable());
			}
		}
		
		void systemCall(int mask,string name,int args[]) {
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_call(slaves[i].tcp_socket,TCP_SYSTEM_ASYNC,name,args);
			}
		}
		
		void systemStream(int mask,string name,Blob blob) {
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_stream(slaves[i].tcp_socket,TCP_SYSTEM_STREAM,name,blob);
			}
		}
		
		/*
		 */
		void worldSet(int mask,string name,int value) {
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_set(slaves[i].tcp_socket,TCP_WORLD_SET,name,value);
			}
		}
		
		void worldGet(int mask,string name,int ret[]) {
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_get(slaves[i].tcp_socket,TCP_WORLD_GET,name);
			}
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				ret.append(slaves[i].tcp_socket.readVariable());
			}
		}
		
		void worldCall(int mask,string name,int args[],int ret[]) {
			ret.clear();
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_call(slaves[i].tcp_socket,TCP_WORLD_SYNC,name,args);
			}
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				ret.append(slaves[i].tcp_socket.readVariable());
			}
		}
		
		void worldCall(int mask,string name,int args[]) {
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_call(slaves[i].tcp_socket,TCP_WORLD_ASYNC,name,args);
			}
		}
		
		void worldStream(int mask,string name,Blob blob) {
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_stream(slaves[i].tcp_socket,TCP_WORLD_STREAM,name,blob);
			}
		}
		
		/*
		 */
		void editorSet(int mask,string name,int value) {
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_set(slaves[i].tcp_socket,TCP_EDITOR_SET,name,value);
			}
		}
		
		void editorGet(int mask,string name,int ret[]) {
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_get(slaves[i].tcp_socket,TCP_EDITOR_GET,name);
			}
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				ret.append(slaves[i].tcp_socket.readVariable());
			}
		}
		
		void editorCall(int mask,string name,int args[],int ret[]) {
			ret.clear();
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_call(slaves[i].tcp_socket,TCP_EDITOR_SYNC,name,args);
			}
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				ret.append(slaves[i].tcp_socket.readVariable());
			}
		}
		
		void editorCall(int mask,string name,int args[]) {
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_call(slaves[i].tcp_socket,TCP_EDITOR_ASYNC,name,args);
			}
		}
		
		void editorStream(int mask,string name,Blob blob) {
			forloop(int i = 0; slaves.size()) {
				if((mask & (1 << i)) == 0) continue;
				tcp_stream(slaves[i].tcp_socket,TCP_EDITOR_STREAM,name,blob);
			}
		}
		
		/*
		 */
		void tcp_set(Socket socket,int command,string name,int value) {
			socket.writeChar(command);
			socket.writeString(name);
			socket.writeVariable(value);
		}
		
		void tcp_get(Socket socket,int command,string name) {
			socket.writeChar(command);
			socket.writeString(name);
		}
		
		void tcp_call(Socket socket,int command,string name,int args[]) {
			socket.writeChar(command);
			socket.writeString(name);
			socket.writeChar(args.size());
			foreach(int a; args) socket.writeVariable(a);
		}
		
		void tcp_stream(Socket socket,int command,string name,Blob blob) {
			blob.seekSet(0);
			socket.writeChar(command);
			socket.writeString(name);
			socket.writeInt(blob.getSize());
			socket.writeStream(blob,blob.getSize());
		}
	}
	
} /* namespace Unigine::Syncker */

#endif /* __UNIGINE_SYNCKER_MASTER_H__ */
