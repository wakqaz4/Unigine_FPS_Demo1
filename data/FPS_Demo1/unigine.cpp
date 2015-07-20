#include <core/unigine.h>
#include <core/scripts/system/system.h>
#include <core/scripts/system/stereo.h>
#include <core/scripts/system/wall.h>

int init() {
	
	systemInit();
	stereoInit();
	wallInit();
	
	return 1;
}

int update() {
	
	systemUpdate();
	stereoUpdate();
	
	return 1;
}

int shutdown() {
	
	systemShutdown();
	stereoShutdown();
	wallShutdown();
	
	return 1;
}

int render() {
	
	stereoRender();
	wallRender();
	
	return 1;
}