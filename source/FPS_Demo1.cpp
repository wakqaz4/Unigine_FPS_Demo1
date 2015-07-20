#include <UnigineEngine.h>

/*
 */
int main(int argc,char *argv[]) {
	
	// init engine
	Unigine::EnginePtr engine(UNIGINE_VERSION,argc,argv);
	
	// main loop
	while(!engine->isDone()) {
		
		engine->update();
		engine->render();
		engine->swap();
	}
	
	return 0;
}
