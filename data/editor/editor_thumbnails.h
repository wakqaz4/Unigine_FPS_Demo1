/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_thumbnails.h
 * Desc:    Unigine thimbnails
 * Version: 1.02
 * Author:  Andrey Viktorov <unclebob@unigine.com>
 *          Vladimir Borovskikh <futurist@unigine.com>
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

#ifndef __EDITOR_THUMBNAILS_H__
#define __EDITOR_THUMBNAILS_H__

/*
 */
namespace Thumbnails {
	
	/**************************************************************************\
	*
	* Cache
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Cache {
		
		/*
		 */
		long memory_limit;
		long memory_usage;
		
		Image images[];
		int frames[];
		int mtimes[];
		int invalid[];
		
		/*
		 */
		void init(string path,long memory) {
			memory_limit = memory;
			memory_usage = 0;
			
			Dir dir = new Dir(path);
			if(dir.isOpened()) {
				forloop(int i = 0; dir.getNumFiles()) {
					string file_name = dir.getFileName(i);
					string hash = getHashName(file_name);
					
					images[hash] = NULL;
					frames[hash] = engine.game.getFrame();
					mtimes[hash] = dir.getFileMTime(i);
				}
				
				dir.close();
			}
			
			delete dir;
		}
		
		/*
		 */
		void shutdown() {
			clear();
			
			memory_limit = 0;
		}
		
		/*
		 */
		void clear() {
			images.delete();
			frames.clear();
			mtimes.clear();
			invalid.clear();
			
			memory_usage = 0;
		}
		
		/*
		 */
		void update() {
			if(memory_usage < memory_limit) return;
			
			int min_frame = engine.game.getFrame();
			string min_hash = "";
			
			foreachkey(string hash; frames) {
				if(!images.check(hash)) continue;
				if(images[hash] == NULL) continue;
				if(frames[hash] > min_frame) continue;
				
				min_frame = frames[hash];
				min_hash = hash;
			}
			
			if(min_hash != "") {
				setImage(min_hash,NULL);
				setFrame(min_hash,-1);
			}
		}
		
		/*
		 */
		int needUpdate(string hash,int mtime) {
			if(mtimes.check(hash) == 0) return 1;
			return mtimes[hash] != mtime;
		}
		
		/*
		 */
		void set(string hash,Image image,int frame,int mtime) {
			if(image != NULL) setImage(hash,image);
			setFrame(hash,frame);
			setMTime(hash,mtime);
		}
		
		/*
		 */
		void setImage(string hash,Image image) {
			Image cache = images.check(hash,NULL);
			if(cache != NULL) {
				memory_usage -= cache.getSize();
				delete cache;
			}
			
			images[hash] = image;
			if(image != NULL) memory_usage += image.getSize();
		}
		
		Image getImage(string hash) {
			return images.check(hash,NULL);
		}
		
		/*
		 */
		void setFrame(string hash,int frame) {
			frames[hash] = frame;
		}
		
		int getFrame(string hash) {
			return frames.check(hash,-1);
		}
		
		/*
		 */
		void setMTime(string hash,int mtime) {
			mtimes[hash] = mtime;
		}
		
		int getMTime(string hash) {
			return mtimes.check(hash,-1);
		}
		
		/*
		 */
		void setInvalid(string hash,int value) {
			if(!invalid.check(hash)) invalid.append(hash,value);
			else invalid[hash] = value;
		}
		
		int isInvalid(string hash) {
			if(!invalid.check(hash)) return 0;
			return invalid[hash];
		}
		
		/*
		 */
		string getHashName(string path) {
			string hash = basename(path);
			hash = replace(hash,"." + extension(hash),NULL);
			
			return hash;
		}
		
		/*
		 */
		void dump() {
			log.message("Cache:\n");
			log.message("\tfiles: %d\n",images.size());
			log.message("\tmemory: %s / %s \n",memory(memory_usage),memory(memory_limit));
		}
	}
	
	/**************************************************************************\
	*
	* NodeRenderer
	*
	\**************************************************************************/
	
	/*
	 */
	namespace NodeRenderer {
		
		/*
		 */
		enum {
			LIGHT_MASK = 1 << 31,
			LIGHT_ANGLE = 15.0f,
			CAMERA_FOV = 60.0f,
			CAMERA_NEAR = 0.1f,
			RENDER_TRIES = 4,
			RENDER_MAX_FRAMES = 4,
			RENDER_MAX_BORDER = 6,
			RENDER_STEP = 2,
		};
		
		/*
		 */
		LightWorld node_light;
		WidgetSpriteNode node_renderer;
		float camera_factor;
		Vec3 camera_direction;
		
		/*
		 */
		void init() {
			int render_size = Thumbnails::getThumbnailSize() * 4;
			
			node_renderer = new WidgetSpriteNode(engine.getGui(),render_size,render_size);
			
			camera_factor = 1.0f / tan(CAMERA_FOV * DEG2RAD * 0.5f);
			camera_direction = Vec3(1,-1,1);
			
			Mat4 transform = rotateZ(LIGHT_ANGLE) * setTo(camera_direction,Vec3_zero,Vec3(0,0,1));
			
			node_light = new LightWorld(vec4_one);
			node_light.setWorldTransform(transform);
			node_light.setLightMask(LIGHT_MASK);
			node_light.setEnabled(0);
		}
		
		/*
		 */
		void shutdown() {
			delete node_renderer;
			delete node_light;
		}
		
		/*
		 */
		void prepare(Node node) {
			
			// light mask
			node_traversal(node,functionid(update_masks));
			node.setEnabled(0);
		}
		
		/*
		 */
		Image render(Node node) {
			
			// auxiliary scene on
			node_renderer.setNode(node);
			
			set_enabled(1);
			node.setEnabled(1);
			
			// bounds
			engine.world.updateSpatial();
			
			WorldBoundSphere bounds = node.getWorldBoundSphere();
			
			Vec3 bound_center = bounds.getCenter();
			Scalar bound_radius = bounds.getRadius();
			
			// render
			Scalar min_radius = bound_radius / 2;
			Scalar max_radius = bound_radius * 2;
			
			Image image = new Image();
			
			forloop(int i = 0; RENDER_TRIES) {
				Scalar radius = lerp(min_radius,max_radius,Scalar(i) / RENDER_TRIES);
				
				Scalar camera_distance = radius * camera_factor;
				float camera_far = float(camera_distance + max_radius);
				
				Mat4 modelview = lookAt(bound_center + camera_direction * camera_distance,bound_center,Vec3(0,0,1));
				mat4 projection = perspective(CAMERA_FOV,1.0f,CAMERA_NEAR,camera_far);
				
				node_renderer.setModelview(modelview);
				node_renderer.setProjection(projection);
				
				node_renderer.renderImage(image);
				
				if(check_borders(image)) break;
			}
			
			// auxiliary scene off
			node_renderer.setNode(NULL);
			
			set_enabled(0);
			node.setEnabled(0);
			
			return image;
		}
		
		/*
		 */
		void update_masks(Node node) {
			if(node.isObject() == 0) return;
			Object obj = node_cast(node);
			
			forloop(int i = 0; obj.getNumSurfaces()) {
				obj.setLightMask(LIGHT_MASK,i);
			}
		}
		
		int check_borders(Image image) {
			int width = image.getWidth();
			int height = image.getHeight();
			
			forloop(int x = 0; width; RENDER_STEP) {
				forloop(int y = 0; RENDER_MAX_BORDER; RENDER_STEP) {
					vec4 p0 = image.get2D(x,y);
					vec4 p1 = image.get2D(y,x);
					if(p0.w > 0.0f || p1.w > 0.0f) return 0;
				}
				
				forloop(int y = height - RENDER_MAX_BORDER; height; RENDER_STEP) {
					vec4 p0 = image.get2D(x,y);
					vec4 p1 = image.get2D(y,x);
					if(p0.w > 0.0f || p1.w > 0.0f) return 0;
				}
			}
			
			return 1;
		}
		
		/*
		 */
		void set_enabled(int enabled) {
			node_light.setEnabled(enabled);
			if(enabled) engine.world.updateSpatial();
		}
	}
	
	/**************************************************************************\
	*
	* Handlers
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Handlers {
		
		/*
		 */
		int MAX_IMAGE_SIZE = 16384;
		int MAX_NODE_SIZE = 1024 * 1024 * 5;	// 5 MB
		int MAX_MESH_SIZE = 1024 * 1024 * 50;	// 50 MB
		
		/*
		 */
		class HandlerBase {
				
			private:
				
				enum {
					MAX_RENDER_QUEUE_DEPTH = 1000,
				};
				
				//
				int files[0];
				int render_files[0];
				string extensions[0];
				
				//
				int load_file(string path) = 0;
				int check_file(string path) = 0;
				int render_file(string path) = 0;
				int grab_file(string path) = 0;
				void release_file(string path) = 0;
				
			public:
				
				//
				HandlerBase(string ext) {
					strsplit(ext,".",extensions);
				}
				
				~HandlerBase() {
					clear();
					extensions.clear();
				}
				
				//
				int canHandle(string ext) {
					forloop(int i = 0; extensions.size()) {
						if(extensions[i] == ext) return 1;
					}
					
					return 0;
				}
				
				void flush() {
					forloop(int i = 0; render_files.size()) {
						if(Cache::getMTime(render_files[i]) == -1) continue;
						Cache::setMTime(render_files[i],-1);
					}
					render_files.clear();
				}
				
				void force(string path) {
					if(render_files.size() <= 5) return;
					
					int id = render_files.find(path);
					if(id == 0) return;
					
					if(id != -1) {
						string tmp = render_files[id];
						render_files.remove(id);
						render_files.append(1,tmp);
						return;
					}
				}
				
				//
				void append(string path) {
					if(render_files.find(path) != -1) return;
					if(load_file(path) == 0) return;
					if(files.find(path) != -1) return;
					
					files.append(path);
				}
				
				//
				void update() {
					
					// check
					for(int i = 0; i < files.size(); i++) {
						string path = files[i];
						if(render_files.size() > MAX_RENDER_QUEUE_DEPTH) break;
						if(check_file(path) == 0) continue;
						
						files.remove(i--);
						render_files.append(path);
					}
					
					// render
					if(render_files.size() == 0) return;
					string path = render_files[0];
					
					if(grab_file(path) == 0) return;
					if(render_file(path) == 0) return;
					
					release_file(path);
					render_files.remove(0);
				}
				
				//
				void clear() {
					forloop(int i = 0; files.size()) {
						release_file(files[i]);
					}
					
					forloop(int i = 0; render_files.size()) {
						release_file(render_files[i]);
					}
					
					files.clear();
					render_files.clear();
				}
				
				//
				void dump() {
					log.message("Handler:\n");
					
					log.message("\textensions: ");
					forloop(int i = 0; extensions.size()) {
						log.message("*.%s ",extensions[i]);
					}
					
					log.message("\n\tfiles in loading queue: %d\n",files.size());
					
					forloop(int i = 0; files.size()) {
						log.message("\t\t(%d) file: \"%s\"\n",i,files[i]);
					}
					
					log.message("\tfiles in rendering queue: %d\n",render_files.size());
					
					forloop(int i = 0; render_files.size()) {
						log.message("\t\t(%d) file: \"%s\"\n",i,render_files[i]);
					}
				}
		};
		
		/*
		 */
		class HandlerImage : HandlerBase {
				
			private:
				
				//
				Image image;
				
				//
				int validate_file(string path) {
					
					if(!is_file(absname(engine.getDataPath(),path))) return 0;
					
					Image image = new Image();
					string to_load[] = (
						"hdr",
						"psd",
						"rgb",
						"rgba",
						"ppm",
						"pgm",
						"sgi",
					);
					int valid = 0;
					if(to_load.find(lower(extension(path))) != -1) valid = 1;
					else valid = image.info(path);
					
					int width = 0;
					int height = 0;
					
					if(valid) {
						width = image.getWidth();
						height = image.getHeight();
					}
					
					delete image;
					
					if(!valid) {
						set_invalid_file(path);
						return 0;
					}
					
					return valid && (width <= MAX_IMAGE_SIZE) && (height <= MAX_IMAGE_SIZE);
				}
				
				int load_file(string path) {
					if(validate_file(path) == 0) return 0;
					
					return 1;
				}
				
				int check_file(string path) {
					if(engine.filesystem.checkImage(path) == 0) {
						engine.filesystem.loadImage(path);
						return 0;
					}
					return 1;
				}
				
				int render_file(string path) {
					
					// get
					if(image == NULL) return 1;
					
					int convert_to[] = (
						IMAGE_FORMAT_R16F:		IMAGE_FORMAT_RGBA16F,
						IMAGE_FORMAT_RG16F:		IMAGE_FORMAT_RGBA16F,
						IMAGE_FORMAT_RGB16F:	IMAGE_FORMAT_RGBA16F,
						IMAGE_FORMAT_RGBA16F:	IMAGE_FORMAT_RGBA16F,
						IMAGE_FORMAT_R32F:		IMAGE_FORMAT_RGBA32F,
						IMAGE_FORMAT_RG32F:		IMAGE_FORMAT_RGBA32F,
						IMAGE_FORMAT_RGB32F:	IMAGE_FORMAT_RGBA32F,
						IMAGE_FORMAT_RGBA32F:	IMAGE_FORMAT_RGBA32F,
					);
					
					if(!image.isLoaded()) {
						set_invalid_file(path);
						return 1;
					}
					
					// convert
					if(image.convertToType(IMAGE_2D) == 0) return 1;
					int format = convert_to.check(image.getFormat(),IMAGE_FORMAT_RGBA8);
					if(format != IMAGE_FORMAT_RGBA8) format = convert_to[format];
					if(image.convertToFormat(format) == 0) return 1;
					
					// decompress
					if(image.isZCompressedFormat()) image.decompress();
					if(image.isCompressedFormat()) image.decompress();
					if(image.isCombinedFormat()) image.decombine();
					
					// resize
					int width = image.getWidth();
					int height = image.getHeight();
					
					int size = Thumbnails::getThumbnailSize();
					
					if(width > size || height > size) {
						float scale = float(size) / max(width,height);
						
						width = min(int(width * scale),size);
						height = min(int(height * scale),size);
						
						image.resize(width,height);
					}
					
					// save
					save_image(path,image);
					
					return 1;
				}
				
				int grab_file(string path) {
					if(image != NULL) return 1;
					
					image = engine.filesystem.getImage(path);
					
					if(image != NULL) {
						image = class_append(image);
						return 1;
					}
					
					if(engine.filesystem.checkImage(path) == 0) {
						engine.filesystem.loadImage(path);
					}
					
					return 0;
				}
				
				void release_file(string path) {
					delete image;
					image = NULL;
				}
				
			public:
				
				//
				HandlerImage(string ext) : HandlerBase(ext) {
					image = NULL;
				}
				
				~HandlerImage() {
					if(image != NULL) {
						delete image;
						image = NULL;
					}
				}
		};
		
		/*
		 */
		class HandlerNode : HandlerBase {
				
			private:
				
				int node_ids[];
				int frames;
				Node node;
				
				//
				int validate_file(string path) {
					
					if(!is_file(absname(engine.getDataPath(),path))) return 0;
					
					// nodes that not under data_path are skipped
					string full_path = pathname(absname(engine.getDataPath(),path));
					if(strstr(full_path,engine.getDataPath()) != 0) {
						set_invalid_file(path);
						return 0;
					}
					
					int valid = check_node(path);
					
					if(!valid) set_invalid_file(path);
					return valid;
				}
				
				int get_file_size(string path) {
					
					File file = new File(path,"r");
					if(file.isOpened() == 0) {
						delete file;
						return -1;
					}
					
					int size = file.getSize();
					file.close();
					
					delete file;
					
					return size;
				}
				
				int check_element(Xml element,int &mesh_size_quota,string references[]) {
					
					string type = element.getArg("type","");
					
					// mesh
					if(type == "ObjectMeshStatic") {
						
						string path = element.getChildData("mesh_name","");
						if(strlen(path)) {
							
							int mesh_size = get_file_size(path);
							if(mesh_size == -1) return false;
							
							mesh_size_quota -= mesh_size;
							if(mesh_size_quota < 0) return false;
						}
					}
					
					// noderef check
					if(type == "NodeReference") {
						string path = element.getChildData("reference","");
						if(strlen(path)) references.append(path);
					}
					
					// expression check
					if(type == "WorldExpression") return false;
					if(type == "ObjectSky") return false;
					if(type == "ObjectTerrain") return false;
					
					forloop(int i = 0; element.getNumChilds()) {
						if(!check_element(element.getChild(i),mesh_size_quota,references)) return false;
					}
					
					return true;
				}
				
				int check_xml(Xml root,int &mesh_size_quota,string references[]) {
					
					// empty node file
					if(root.getNumChilds() == 0) return false;
					
					references.clear();
					return check_element(root,mesh_size_quota,references);
				}
				
				int check_node(string node_path) {
					
					string node_files[0] = ( node_path );
					int node_size_quota = MAX_NODE_SIZE;
					int mesh_size_quota = MAX_MESH_SIZE;
					
					Xml xml = new Xml();
					
					int quit_(int result) {
						delete xml;
						return result;
					}
					
					while(node_files.size()) {
						
						string next_generation[0] = ();
						
						foreach(string node_file; node_files) {
							
							// checking size
							int node_size = get_file_size(node_file);
							if(node_size == -1 || node_size > MAX_NODE_SIZE) return quit_(false);
							
							node_size_quota -= node_size;
							if(node_size_quota < 0) return quit_(false);
							
							// loading and validating xml
							string references[0] = ();
							if(!xml.load(node_file) || !check_xml(xml,mesh_size_quota,references)) return quit_(false);
							
							if(mesh_size_quota < 0) return quit_(false);
							
							// ok, this node passed
							next_generation.merge(references);
						}
						
						node_files.copy(next_generation);
					}
					
					return quit_(true);
				}
				
				int load_file(string path) {
					if(validate_file(path) == 0) return 0;
					
					node_ids.append(path,engine.world.loadAsyncNode(path));
					return 1;
				}
				
				int check_file(string path) {
					if(node_ids.check(path) == 0) return 0;
					
					return engine.world.checkAsyncNode(node_ids[path]);
				}
				
				Node create_node(string path) {
					if(node_ids.check(path) == 0) return NULL;
					
					Node async_node = engine.world.getAsyncNode(node_ids[path]);
					if(async_node != NULL) async_node = node_append(async_node);
					
					return async_node;
				}
				
				int render_file(string path) {
					
					// render
					Image image = NodeRenderer::render(node);
					
					// check
					int filesystem_queue = engine.filesystem.getNumQueuedResources();
					int world_queue = engine.world.getNumQueuedResources();
					
					if(frames-- > 0 || filesystem_queue || world_queue) {
						delete image;
						return 0;
					}
					
					// resize
					int size = Thumbnails::getThumbnailSize();
					image.resize(size,size);
					
					// post-process
					image.flipY();
					image.set(3,255);
					
					// save
					save_image(path,image);
					
					// clear
					delete image;
					
					return 1;
				}
				
				int grab_file(string path) {
					
					if(node != NULL) return 1;
					
					node = create_node(path);
					if(node == NULL) return 0;
					
					NodeRenderer::prepare(node);
					frames = NodeRenderer::RENDER_MAX_FRAMES;
					
					return 1;
				}
				
				void release_file(string path) {
					engine.world.removeAsyncNode(node_ids[path]);
					node_ids.remove(path);
					
					delete_node();
				}
				
				void delete_node() {
					if(node == NULL) return;
					
					node_delete(node);
					node = NULL;
				}
				
			public:
				
				//
				HandlerNode(string ext) : HandlerBase(ext) {
					frames = 0;
				}
		};
		
		/*
		 */
		class HandlerMesh : HandlerNode {
				
			private:
				
				Async mesh_loader = new Async();
				int job_id = -1;
				
				//
				int validate_file(string path) {
					
					if(!is_file(absname(engine.getDataPath(),path))) return 0;
					
					Mesh mesh = new Mesh();
					int valid = mesh.info(path);
					
					if(!valid) {
						set_invalid_file(path);
						return 0;
					}
					
					int check_size(string path) {
						
						File file = new File(path,"r");
						if(file.isOpened() == 0) {
							delete file;
							return 0;
						}
						
						int size = file.getSize();
						file.close();
						
						delete file;
						
						return size < MAX_MESH_SIZE;
					}
					
					delete mesh;
					
					if(check_size(path) == 0) {
						set_invalid_file(path);
						return 0;
					}
					
					return valid;
				}
				
				int load_file(string path) {
					if(validate_file(path) == 0) return 0;
					return 1;
				}
				
				int check_file(string path) {
					if(engine.filesystem.checkMesh(path) == 0) {
						engine.filesystem.loadMesh(path);
						return 0;
					}
					return 1;
				}
				
				Node create_node(string path) {
					
					if(node != NULL) return node;
					
					if(job_id == -1) {
						job_id = mesh_loader.run(functionid(create_node_async),path);
						return NULL;
					} else if(mesh_loader.isRunning()) {
						return NULL;
					} else {
						Node n = mesh_loader.getResult(job_id);
						job_id = -1;
						return n;
					}
				}
				
				Node create_node_async(string path) {
					
					Mesh mesh = new Mesh();
					mesh.load(path);
					
					ObjectMeshStatic n = new ObjectMeshStatic(mesh);
					delete mesh;
					
					n.setMaterial("mesh_base","*");
					n.setProperty("surface_base","*");
					n.updateSurfaceBounds();
					
					return n;
				}
				
				void release_file(string path) {
					engine.filesystem.removeMesh(path);
					delete_node();
				}
				
			public:
				
				//
				HandlerMesh(string ext) : HandlerNode(ext) { }
		};
		
		/*
		 */
		HandlerBase handlers[0];
		
		/*
		 */
		void init() {
			handlers.append(new HandlerImage(".tif.tga.jpg.png.dds.psd.ppm.pgm.hdr.sgi.rgb.rgba"));
			handlers.append(new HandlerNode(".node"));
			handlers.append(new HandlerMesh(".mesh"));
		}
		
		/*
		 */
		void shutdown() {
			handlers.delete();
		}
		
		/*
		 */
		void update() { handlers.call("update"); }
		
		/*
		 */
		void flush() { handlers.call("flush"); }
		
		/*
		 */
		void clear() { handlers.call("clear"); }
		
		/*
		 */
		void dump() { handlers.call("dump"); }
		
		/*
		 */
		HandlerBase get(string path) {
			string ext = lower(extension(path));
			
			forloop(int i = 0; handlers.size()) {
				HandlerBase handler = handlers[i];
				if(handler.canHandle(ext)) return handler;
			}
			
			return NULL;
		}
	}
	
	/**************************************************************************\
	*
	* Thumbnails
	*
	\**************************************************************************/
	
	/*
	 */
	int thumbnail_size;
	string thumbnail_path;
	
	/*
	 */
	void init(string path,long memory,int size) {
		
		thumbnail_size = size;
		thumbnail_path = pathname(path + "/.thumbnails/");
		
		if(engine.console.isCommand("editor_preview_clear") == 0) engine.console.addCommand("editor_preview_clear","clear preview cache","Thumbnails::clear");
		if(engine.console.isCommand("editor_preview_info") == 0) engine.console.addCommand("editor_preview_info","print currently processing files","Thumbnails::dump");
		
		Cache::init(thumbnail_path,memory);
		Handlers::init();
		NodeRenderer::init();
	}
	
	void shutdown() {
		
		thumbnail_size = 0;
		thumbnail_path = "";
		
		if(engine.console.isCommand("editor_preview_clear")) engine.console.removeCommand("editor_preview_clear");
		if(engine.console.isCommand("editor_preview_info")) engine.console.removeCommand("editor_preview_info");
		
		Cache::shutdown();
		Handlers::shutdown();
		NodeRenderer::shutdown();
	}
	
	void update(int need_reload) {
		
		Cache::update();
		Handlers::update();
	}
	
	/*
	 */
	void clear() {
		
		Cache::clear();
		Handlers::clear();
		
		if(is_dir(thumbnail_path)) {
			rmdir(thumbnail_path,1);
		}
	}
	
	/*
	 */
	void flush() {
		Handlers::flush();
	}
	
	/*
	 */
	void dump() {
		Cache::dump();
		Handlers::dump();
	}
	
	/*
	 */
	string get_hash(string path) {
		return format("%08x",crc32(path));
	}
	
	/*
	 */
	void set_invalid_file(string path) {
		string hash = get_hash(path);
		Cache::setInvalid(hash,1);
		
		Image image = Cache::getImage(hash);
		
		if(image != NULL) {
			string path = thumbnail_path + hash;
			
			if(image.getFormat() == IMAGE_FORMAT_RGBA16F || image.getFormat() == IMAGE_FORMAT_RGBA32F) path += ".dds";
			else path += ".png";
			
			remove(path);
		}
		
		Cache::setImage(hash,NULL);
	}
	
	/*
	 */
	int save_image(string file,Image image) {
		
		mkdir(thumbnail_path,1);
		set_hidden(thumbnail_path,1);
		if(is_dir(thumbnail_path) == 0) return 0;
		
		string hash = get_hash(file);
		string path = thumbnail_path + hash;
		
		if(image.getFormat() == IMAGE_FORMAT_RGBA16F || image.getFormat() == IMAGE_FORMAT_RGBA32F) path += ".dds";
		else path += ".png";
		
		if(image.save(path) == 0) {
			return 0;
		}
		
		Cache::setImage(hash,NULL);
		set_mtime(path,Cache::getMTime(hash));
		
		grab_image(hash);
		
		return 1;
	}
	
	Image load_image(string hash) {
		
		string path = thumbnail_path + hash;
		
		if(is_file(path + ".png") == 0 && is_file(path + ".dds") == 0) return NULL;
		
		if(is_file(path + ".png") != 0) path += ".png";
		else path += ".dds";
		
		Image image = new Image();
		if(image.load(path) == 0) {
			delete image;
			return NULL;
		}
		
		Cache::setImage(hash,image);
		return image;
	}
	
	Image grab_image(string hash) {
		Image image = Cache::getImage(hash);
		if(image == NULL) image = load_image(hash);
		Cache::setFrame(hash,engine.game.getFrame());
		return image;
	}
	
	/*
	 */
	int isAvailable(string path) {
		string local_path = relname(engine.getDataPath(),path);
		if(engine.filesystem.isKnownFile(local_path) == 0) return 0;
		return Handlers::get(local_path) != NULL;
	}
	
	string getName(string path) {
		string local_path = relname(engine.getDataPath(),path);
		return thumbnail_path + get_hash(local_path) + ".png";
	}
	
	Image getImage(string path) {
		
		// thumbnail check
		if(strstr(path,thumbnail_path) != -1) {
			string hash = Cache::getHashName(path);
			return grab_image(hash);
		}
		
		// name
		string local_path = relname(engine.getDataPath(),path);
		
		if(is_dir(local_path)) return NULL;
		
		// handler
		Handlers::HandlerBase handler = Handlers::get(local_path);
		if(handler == NULL) return NULL;
		
		// cache
		string hash = get_hash(local_path);
		int file_mtime = get_mtime(path);
		
		if(grab_image(hash) == NULL && !Cache::isInvalid(hash)) {
			Cache::set(hash,NULL,-1,file_mtime);
			handler.append(local_path);
			handler.force(local_path);
			return NULL;
		} else {
			if(Cache::needUpdate(hash,file_mtime)) {
				Cache::setInvalid(hash,0);
				Cache::setMTime(hash,file_mtime);
				handler.append(local_path);
				return grab_image(hash);
			}
		}
		
		if(Cache::isInvalid(hash)) return NULL;
		
		return grab_image(hash);
	}
	
	/*
	 */
	int getThumbnailSize() {
		return thumbnail_size;
	}
	
} /* namespace Properties */

/******************************************************************************\
*
* Interface
*
\******************************************************************************/

/*
 */
void thumbnailsInit(string path = engine.getDataPath(),long memory = 4 * 1024 * 1024,int size = 128) {
	Thumbnails::init(path,memory,size);
}

void thumbnailsShutdown() {
	Thumbnails::shutdown();
}

void thumbnailsUpdate(int need_reload) {
	Thumbnails::update(need_reload);
}

/*
 */
int thumbnailsIsAvailable(string path) {
	return Thumbnails::isAvailable(path);
}

string thumbnailsGetName(string path) {
	return Thumbnails::getName(path);
}

Image thumbnailsGetImage(string path) {
	return Thumbnails::getImage(path);
}

Image thumbnailsFlush() {
	return Thumbnails::flush();
}

/*
 */
int thumbnailsGetThumbnailSize() {
	return Thumbnails::getThumbnailSize();
}

#endif /* __EDITOR_THUMBNAILS_H__ */
