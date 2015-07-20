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


#ifndef __UNIGINE_SYNCKER_SLAVE_H__
#define __UNIGINE_SYNCKER_SLAVE_H__

#include <core/systems/syncker/syncker.h>
#include <core/systems/syncker/syncker_serializer.h>

/*
 */
namespace Unigine::Syncker {
	
	/**************************************************************************\
	*
	* Slave
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Slave {
		
		/*
		 */
		enum {
			
			RECV_SIZE = 65536,
			
			FRAME_TIMEOUT = 0.2f,
			CONNECT_TIMEOUT = 1.0f,
		};
		
		/*
		 */
		string name;			// slave name
		int tcp_port = -1;		// tcp port number
		
		/*
		 */
		Socket udp_socket;		// udp server socket
		Socket tcp_socket;		// tcp client socket
		
		/*
		 */
		Blob temp_blob;			// temp blob
		Blob data_blob;			// data blob
		
		/*
		 */
		int node_id[];			// node identifiers
		
		/*
		 */
		int has_frame;			// frame flag
		int has_player;			// player flag
		int has_render;			// render flag
		int has_end;			// end flag
		
		/**********************************************************************\
		*
		* Init/shutdown
		*
		\**********************************************************************/
		
		/*
		 */
		int init(string name_,int udp_port = Unigine::Syncker::UDP_PORT,int tcp_port_ = Unigine::Syncker::TCP_PORT) {
			
			name = name_;
			tcp_port = tcp_port_;
			
			log.message("\n---- Syncker::Slave ----\n");
			log.message("Name: %s\n",name);
			log.message("UDP:  %d\n",udp_port);
			log.message("TCP:  %d\n",tcp_port);
			
			// create blobs
			temp_blob = new Blob();
			data_blob = new Blob();
			
			// create udp server socket
			udp_socket = new Socket(SOCKET_DGRAM);
			if(udp_socket.open(udp_port) == 0 || udp_socket.recv(RECV_SIZE) == 0 || udp_socket.bind() == 0 || udp_socket.nonblock() == 0) {
				log.error("Unigine::Syncker::Slave::init(): can't create udp server socket\n");
				if(udp_socket.isOpened()) udp_socket.close();
				delete udp_socket;
				return 0;
			}
		}
		
		/*
		 */
		int shutdown() {
			
			// clear nodes
			clear_nodes();
			
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
		void clear_nodes() {
			
			// delete nodes
			foreach(int id; node_id) {
				if(engine.world.isNode(id) == 0) continue;
				Node node = engine.world.getNode(id);
				delete class_cast(node.getTypeName(),node);
			}
			node_id.clear();
		}
		
		/**********************************************************************\
		*
		* Update
		*
		\**********************************************************************/
		
		/*
		 */
		void update() {
			
			// flags
			has_frame = 0;
			has_player = 0;
			has_render = 0;
			has_end = 0;
			
			// statistics
			int tcp_commands = 0;
			int udp_packets = 0;
			int udp_size = 0;
			
			/////////////////////////////////
			// read tcp commands
			/////////////////////////////////
			
			while(tcp_socket != NULL) {
				
				// read command
				tcp_socket.nonblock();
				int command = tcp_socket.readChar();
				if(command == 0) break;
				tcp_socket.block();
				
				tcp_commands++;
				
				switch(command) {
					case TCP_BYE:			tcp_bye(); return;
					case TCP_NODE_CREATE:	tcp_node_create(); break;
					case TCP_NODE_DELETE:	tcp_node_delete(); break;
					case TCP_SYSTEM_SET:	tcp_system_set(); break;
					case TCP_SYSTEM_GET:	tcp_system_get(); break;
					case TCP_SYSTEM_SYNC:	tcp_system_call(1); break;
					case TCP_SYSTEM_ASYNC:	tcp_system_call(0); break;
					case TCP_SYSTEM_STREAM:	tcp_system_stream(); break;
					case TCP_WORLD_SET:		tcp_world_set(); break;
					case TCP_WORLD_GET:		tcp_world_get(); break;
					case TCP_WORLD_SYNC:	tcp_world_call(1); break;
					case TCP_WORLD_ASYNC:	tcp_world_call(0); break;
					case TCP_WORLD_STREAM:	tcp_world_stream(); break;
					case TCP_EDITOR_SET:	tcp_editor_set(); break;
					case TCP_EDITOR_GET:	tcp_editor_get(); break;
					case TCP_EDITOR_SYNC:	tcp_editor_call(1); break;
					case TCP_EDITOR_ASYNC:	tcp_editor_call(0); break;
					case TCP_EDITOR_STREAM:	tcp_editor_stream(); break;
					default:				log.warning("Unigine::Syncker::Slave::update(): unknown tcp command %d\n",command);
				}
			}
			
			/////////////////////////////////
			// read udp packets
			/////////////////////////////////
			
			int old_frame;
			int frames = 0;
			float begin = clock();
			
			while(1) {
				
				// read packet
				temp_blob.clear();
				udp_socket.readStream(temp_blob,UDP_PACKET * 2);
				if(temp_blob.getSize() == 0) {
					if(frames == 0 || clock() - begin > FRAME_TIMEOUT) break;
					continue;
				}
				
				// read command
				temp_blob.seekSet(0);
				int command = temp_blob.readChar();
				int frame = temp_blob.readShort();
				int frame_lost = frame - old_frame;
				if(frame_lost > 1 && frame_lost < 0x4fff) log.warning("Unigine::Syncker::Slave::update(): %d frames lost\n",frame_lost);
				old_frame = frame;
				
				// decompress packet
				if(data_blob.decompress(temp_blob) == 0) {
					log.error("Unigine::Syncker::Slave::update(): can't decompress udp packet %d\n",command);
					continue;
				}
				
				udp_packets++;
				udp_size += temp_blob.getSize();
				
				switch(command) {
					case UDP_HELLO:			udp_hello(); break;
					case UDP_FRAME:			udp_frame(); frames++; break;
					case UDP_PLAYER:		udp_player(); break;
					case UDP_RENDER:		udp_render(); break;
					case UDP_MATERIAL:		udp_material(); break;
					case UDP_NODE:			udp_node(); break;
					case UDP_END:			udp_end(); frames--; break;
					default:				log.warning("Unigine::Syncker::Slave::update(): unknown udp packet %d\n",command);
				}
			}
			
			/////////////////////////////////
			// statistics
			/////////////////////////////////
			
			if(engine.profiler.isEnabled()) {
				int size = int(udp_size / engine.app.getFTime());
				engine.profiler.setValue("STcp:","",tcp_commands);
				engine.profiler.setValue("SUdp:","",udp_packets);
				if(size < 1024) engine.profiler.setValue("SSize:","B/s",size);
				else if(size < 1024 * 1024) engine.profiler.setValue("SSize:","KB/s",size / 1024);
				else engine.profiler.setValue("SSize:","MB/s",size / 1024 / 1024);
			}
		}
		
		/*
		 */
		void udp_hello() {
			
			// check tcp client socket
			if(tcp_socket != NULL) return;
			
			// check version
			if(data_blob.readInt() != VERSION) {
				log.warning("Unigine::Syncker::Slave::udp_hello(): wrong version from \"%s\"\n",udp_socket.getHost());
				return;
			}
			
			// create tcp client socket
			tcp_socket = new Socket(SOCKET_STREAM);
			if(tcp_socket.open(udp_socket.getHost(),tcp_port) == 0 || tcp_socket.recv(RECV_SIZE) == 0 || tcp_socket.nodelay() == 0 || tcp_socket.block() == 0) {
				log.warning("Unigine::Syncker::Slave::udp_hello(): can't create tcp client socket to \"%s\"\n",udp_socket.getHost());
				tcp_socket.close();
				delete tcp_socket;
				return;
			}
			
			// connect to master
			float begin = clock();
			while(clock() - begin < CONNECT_TIMEOUT) {
				if(tcp_socket.connect()) {
					
					// send slave hello
					tcp_socket.writeChar(TCP_HELLO);
					tcp_socket.writeInt(VERSION);
					tcp_socket.writeString(name);
					
					log.message("Connection to \"%s\" accepted in %.2f seconds\n",udp_socket.getHost(),clock() - begin);
					
					return;
				}
			}
			
			// can't connect to master
			log.warning("Unigine::Syncker::Slave::udp_hello(): can't connect to \"%s\"\n",udp_socket.getHost());
			tcp_socket.close();
			delete tcp_socket;
		}
		
		/*
		 */
		void tcp_bye() {
			log.message("Connection to \"%s\" closed\n",tcp_socket.getHost());
			tcp_socket.writeChar(TCP_BYE);
			tcp_socket.close();
			delete tcp_socket;
			clear_nodes();
		}
		
		/**********************************************************************\
		*
		* Frame synchronization
		*
		\**********************************************************************/
		
		/*
		 */
		void udp_frame() {
			
			// check tcp client socket
			if(tcp_socket == NULL) return;
			
			Serializer::restoreFrame(data_blob);
			
			has_frame = 1;
		}
		
		void udp_end() {
			
			// check tcp client socket
			if(tcp_socket == NULL) return;
			
			has_end = 1;
		}
		
		/*
		 */
		int hasFrame() {
			int ret = has_frame;
			has_frame = 0;
			return ret;
		}
		
		int hasEnd() {
			int ret = has_end;
			has_end = 0;
			return ret;
		}
		
		/**********************************************************************\
		*
		* Player synchronization
		*
		\**********************************************************************/
		
		/*
		 */
		void udp_player() {
			
			// check tcp client socket
			if(tcp_socket == NULL) return;
			
			Player player = engine.editor.getPlayer();
			if(player == NULL) player = engine.game.getPlayer();
			if(player != NULL) Serializer::restorePlayer(player,data_blob);
			
			has_player = 1;
		}
		
		/*
		 */
		int hasPlayer() {
			int ret = has_player;
			has_player = 0;
			return ret;
		}
		
		/**********************************************************************\
		*
		* Render synchronization
		*
		\**********************************************************************/
		
		/*
		 */
		void udp_render() {
			
			// check tcp client socket
			if(tcp_socket == NULL) return;
			
			Serializer::restoreRender(data_blob);
			
			has_render = 1;
		}
		
		/*
		 */
		int hasRender() {
			int ret = has_render;
			has_render = 0;
			return ret;
		}
		
		/**********************************************************************\
		*
		* Material synchronization
		*
		\**********************************************************************/
		
		/*
		 */
		void udp_material() {
			
			// check tcp client socket
			if(tcp_socket == NULL) return;
			
			while(data_blob.eof() == 0) {
				
				// material name
				string name = data_blob.readString();
				Material material = engine.materials.findMaterial(name);
				if(material == NULL) return;
				
				Serializer::restoreMaterial(material,data_blob);
			}
		}
		
		/**********************************************************************\
		*
		* Node synchronization
		*
		\**********************************************************************/
		
		/*
		 */
		void tcp_node_create() {
			
			// read data
			if(data_blob.decompress(tcp_socket) == 0) {
				log.error("Unigine::Syncker::Slave::tcp_node_create(): can't decompress data\n");
				return;
			}
			
			// master identifier
			int id = data_blob.readInt();
			int parent_id = data_blob.readInt();
			
			// receive node
			Node node = Serializer::receiveNode(data_blob);
			
			// check node identifier
			if(node_id.check(id)) {
				log.warning("Unigine::Syncker::Slave::tcp_node_create(): bad node id 0x%08x\n",id);
				delete node;
				return;
			}
			
			// register node
			node_id.append(id,node.getID());
			
			// check parent node
			if(id == parent_id) return;
			if(node_id.check(parent_id) == 0) {
				log.warning("Unigine::Syncker::Slave::tcp_node_create(): bad parent node id 0x%08x\n",parent_id);
				return;
			}
			
			// check parent node identifier
			if(engine.world.isNode(node_id[parent_id]) == 0) {
				log.warning("Unigine::Syncker::Slave::tcp_node_create(): can't find 0x%08x parent node id\n",parent_id);
				return;
			}
			
			// reparent node
			Node parent = engine.world.getNode(node_id[parent_id]);
			parent.addWorldChild(node);
		}
		
		/*
		 */
		void tcp_node_delete() {
			
			// master identifier
			int id = tcp_socket.readInt();
			int type = tcp_socket.readInt();
			
			// check node identifier
			if(node_id.check(id) == 0) {
				log.warning("Unigine::Syncker::Slave::tcp_node_delete(): bad node id 0x%08x\n",id);
				return;
			}
			
			// check node identifier
			if(engine.world.isNode(node_id[id]) == 0) {
				log.warning("Unigine::Syncker::Slave::tcp_node_delete(): can't find 0x%08x node id\n",id);
				return;
			}
			
			// check node type
			Node node = engine.world.getNode(node_id[id]);
			if(node.getType() != type) {
				log.warning("Unigine::Syncker::Slave::tcp_node_delete(): bad node type %s\n",node.getTypeName());
				return;
			}
			
			// delete node
			node_id.remove(id);
			delete node;
		}
		
		/*
		 */
		void udp_node() {
			
			// check tcp client socket
			if(tcp_socket == NULL) return;
			
			while(data_blob.eof() == 0) {
				
				// node identifier
				int id = data_blob.readInt();
				if(node_id.check(id)) id = node_id[id];
				
				// check node identifier
				if(engine.world.isNode(id) == 0) return;
				
				// check node type
				Node node = engine.world.getNode(id);
				if(node.getType() != data_blob.readChar()) {
					log.warning("Unigine::Syncker::Slave::udp_node(): bad node type %s\n",node.getTypeName());
					return;
				}
				
				Serializer::restoreNode(node,data_blob);
			}
		}
		
		/**********************************************************************\
		*
		* Remote procedurals
		*
		\**********************************************************************/
		
		/*
		 */
		void tcp_system_set() {
			string name = tcp_socket.readString();
			engine.system.set(name,tcp_socket.readVariable());
		}
		
		void tcp_system_get() {
			string name = tcp_socket.readString();
			tcp_socket.writeVariable(engine.system.get(name));
		}
		
		void tcp_system_call(int sync) {
			int args[0] = ();
			string name = tcp_socket.readString();
			forloop(int i = 0; tcp_socket.readChar()) {
				args.append(tcp_socket.readVariable());
			}
			int ret = engine.system.callv(name,args);
			if(sync) tcp_socket.writeVariable(ret);
		}
		
		void tcp_system_stream() {
			data_blob.clear();
			string name = tcp_socket.readString();
			tcp_socket.readStream(data_blob,tcp_socket.readInt());
			data_blob.seekSet(0);
			engine.system.call(name,data_blob);
		}
		
		/*
		 */
		void tcp_world_set() {
			string name = tcp_socket.readString();
			engine.world.set(name,tcp_socket.readVariable());
		}
		
		void tcp_world_get() {
			string name = tcp_socket.readString();
			tcp_socket.writeVariable(engine.world.get(name));
		}
		
		void tcp_world_call(int sync) {
			int args[0] = ();
			string name = tcp_socket.readString();
			forloop(int i = 0; tcp_socket.readChar()) {
				args.append(tcp_socket.readVariable());
			}
			int ret = engine.world.callv(name,args);
			if(sync) tcp_socket.writeVariable(ret);
		}
		
		void tcp_world_stream() {
			data_blob.clear();
			string name = tcp_socket.readString();
			tcp_socket.readStream(data_blob,tcp_socket.readInt());
			data_blob.seekSet(0);
			engine.world.call(name,data_blob);
		}
		
		/*
		 */
		void tcp_editor_set() {
			string name = tcp_socket.readString();
			engine.editor.set(name,tcp_socket.readVariable());
		}
		
		void tcp_editor_get() {
			string name = tcp_socket.readString();
			tcp_socket.writeVariable(engine.editor.get(name));
		}
		
		void tcp_editor_call(int sync) {
			int args[0] = ();
			string name = tcp_socket.readString();
			forloop(int i = 0; tcp_socket.readChar()) {
				args.append(tcp_socket.readVariable());
			}
			int ret = engine.editor.callv(name,args);
			if(sync) tcp_socket.writeVariable(ret);
		}
		
		void tcp_editor_stream() {
			data_blob.clear();
			string name = tcp_socket.readString();
			tcp_socket.readStream(data_blob,tcp_socket.readInt());
			data_blob.seekSet(0);
			engine.editor.call(name,data_blob);
		}
	}
	
} /* namespace Unigine::Syncker */

#endif /* __UNIGINE_SYNCKER_SLAVE_H__ */
