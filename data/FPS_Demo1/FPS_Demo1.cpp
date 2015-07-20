#include <core/unigine.h>

int init() {
	
	PlayerSpectator camera = new PlayerSpectator();
	camera.setPosition(Vec3(2.0f,0.0f,1.5f));
	camera.setDirection(Vec3(-1.0f,0.0f,-0.5f));
	engine.game.setPlayer(camera);
	
	return 1;
}

int shutdown() {
	return 1;
}

int update() {
	return 1;
}