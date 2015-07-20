#include <core/unigine.h>

/*
 */
int init() {
	Player player = new PlayerSpectator();
	player.setDirection(Vec3(0.755f,-1.0f,0.25f));
	engine.game.setPlayer(player);
	return 1;
}

/*
 */
int shutdown() {
	return 1;
}

/*
 */
int update() {
	return 1;
}
