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


#ifndef __UNIGINE_IMAGE_ATLAS_H__
#define __UNIGINE_IMAGE_ATLAS_H__

/*
 */
namespace Unigine {
	
	/*
	 */
	class ImageAtlas {
		
		int format;				// atlas format
		int padding;			// atlas padding
		
		Tile tiles[];			// image tiles
		
		/*
		 */
		class Tile {
			
			int atlas;			// atlas flag
			int width;			// image width
			int height;			// image height
			Image image;		// image copy
			float scale_x;		// atlas scale x
			float scale_y;		// atlas scale y
			float position_x;	// atlas position x
			float position_y;	// atlas position y
			
			~Tile() {
				delete image;
			}
			
			int operator<(Tile t0,Tile t1) {
				return (t0.width + t0.height > t1.width + t1.height);
			}
		};
		
		/*
		 */
		class Node {
			
			int x0,y0;			// minimum
			int x1,y1;			// maximum
			Node left;			// left child
			Node right;			// right child
			Tile tile;			// tile
			
			~Node() {
				delete left;
				delete right;
			}
		};
		
		/**********************************************************************\
		*
		* ImageAtlas
		*
		\**********************************************************************/
		
		/*
		 */
		ImageAtlas(int f,int p) {
			format = f;
			padding = p;
		}
		
		~ImageAtlas() {
			clear();
		}
		
		/*
		 */
		void clear() {
			tiles.delete();
		}
		
		/*
		 */
		int addImage(int id,Image i) {
			
			// check image
			if(i.isLoaded() == 0) {
				log.error("Unigine::ImageAtlas::addImage(): %d image is not loaded\n",id);
				return 0;
			}
			
			// check image identifier
			if(tiles.check(id) != 0) {
				log.error("Unigine::ImageAtlas::addImage(): %d image is already defined\n",id);
				return 0;
			}
			
			// image copy
			Image image = new Image();
			image.copy(i);
			
			// update image format
			if(image.convertToFormat(format) == 0) {
				log.error("Unigine::ImageAtlas::addImage(): can't convert %d image\n",id);
				delete image;
				return 0;
			}
			
			// create tile
			Tile tile = new Tile();
			tile.width = image.getWidth() + padding;
			tile.height = image.getHeight() + padding;
			tile.image = image;
			tiles.append(id,tile);
			
			return 1;
		}
		
		/*
		 */
		Image create(int max_size) {
			
			if(tiles.size() == 0) return 0;
			
			// tiles
			Tile pointers[0];
			foreach(Tile tile; tiles) {
				pointers.append(tile);
				tile.atlas = 0;
			}
			
			// sort tiles by perimiter
			pointers.sort();
			
			// create atlas
			int step = max(pointers.size() / 8,1);
			for(int offset = 0; offset < pointers.size(); offset += step) {
				
				int area = 0;
				forloop(int i = offset; pointers.size()) {
					area += pointers[i].width * pointers[i].height;
				}
				
				int size = 1;
				while(size <= max_size) {
					if(size * size >= area) {
						Image image = create_tiles(size,pointers,offset);
						if(image != 0) return image;
					}
					if(size * size * 9 / 4 >= area) {
						Image image = create_tiles(size * 3 / 2,pointers,offset);
						if(image != 0) return image;
					}
					size *= 2;
				}
			}
			
			return 0;
		}
		
		/*
		 */
		int isImage(int id) {
			return tiles[id].atlas;
		}
		
		float getScaleX(int id) {
			return tiles[id].scale_x;
		}
		float getScaleY(int id) {
			return tiles[id].scale_y;
		}
		
		float getPositionX(int id) {
			return tiles[id].position_x;
		}
		float getPositionY(int id) {
			return tiles[id].position_y;
		}
		
		/*
		 */
		int create_node(Node root,Tile tile) {
			
			// branch node
			if(root.left != 0) {
				if(create_node(root.left,tile)) return 1;
				return create_node(root.right,tile);
			}
			
			// check node
			if(root.tile != 0) return 0;
			int width = root.x1 - root.x0;
			if(width < tile.width) return 0;
			int height = root.y1 - root.y0;
			if(height < tile.height) return 0;
			if(tile.width == width && tile.height == height) {
				root.tile = tile;
				return 1;
			}
			
			// split node
			root.left = new Node();
			root.right = new Node();
			if(width - tile.width > height - tile.height) {
				root.left.x0 = root.x0;
				root.left.y0 = root.y0;
				root.left.x1 = root.x0 + tile.width;
				root.left.y1 = root.y1;
				root.right.x0 = root.x0 + tile.width;
				root.right.y0 = root.y0;
				root.right.x1 = root.x1;
				root.right.y1 = root.y1;
			} else {
				root.left.x0 = root.x0;
				root.left.y0 = root.y0;
				root.left.x1 = root.x1;
				root.left.y1 = root.y0 + tile.height;
				root.right.x0 = root.x0;
				root.right.y0 = root.y0 + tile.height;
				root.right.x1 = root.x1;
				root.right.y1 = root.y1;
			}
			
			return create_node(root.left,tile);
		}
		
		void create_atlas(Node root,Image image) {
			
			// branch node
			if(root.left != 0) {
				create_atlas(root.left,image);
				create_atlas(root.right,image);
			}
			
			// copy image
			if(root.tile != 0) {
				Tile tile = root.tile;
				Image src = tile.image;
				float iwidth = 1.0f / image.getWidth();
				float iheight = 1.0f / image.getHeight();
				image.copy(src,root.x0 + padding / 2,root.y0 + padding / 2,0,0,src.getWidth(),src.getHeight());
				tile.scale_x = src.getWidth() * iwidth;
				tile.scale_y = src.getHeight() * iheight;
				tile.position_x = (root.x0 + padding / 2) * iwidth;
				tile.position_y = (root.y0 + padding / 2) * iheight;
				tile.atlas = 1;
			}
		}
		
		Image create_tiles(int size,Tile tiles[],int offset) {
			
			// root node
			Node root = new Node();
			root.x1 = size;
			root.y1 = size;
			
			// create primary atlas
			forloop(int i = offset; tiles.size()) {
				if(create_node(root,tiles[i]) == 0) {
					delete root;
					return 0;
				}
			}
			
			// create secondary atlas
			for(int i = offset - 1; i >= 0; i--) {
				if(create_node(root,tiles[i]) == 0) break;
			}
			
			// atlas is ready
			Image image = new Image();
			image.create2D(size,size,format);
			create_atlas(root,image);
			delete root;
			return image;
		}
	};
	
} /* namespace Unigine */

#endif /* __UNIGINE_IMAGE_ATLAS_H__ */
