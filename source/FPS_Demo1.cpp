#include <UnigineApp.h>
#include <UnigineEngine.h>
#include <UnigineGame.h>
#include <UnigineConsole.h>
#include <UniginePlayer.h>
#include <UniginePlayerPersecutor.h>
#include "FPS_Demo1.h"

using namespace Unigine;

MY_EXTERN void my_init(void);
MY_EXTERN void my_update(void);
MY_EXTERN void my_shutdown(void);

//should figure out sth better to manage objects/nodes/... created by C++API;
TObjectMeshStatic gMeshStatic;
CPlayerActorSkinned gPlayerActorSkinned;
PlayerPersecutorPtr playerPersecutor;
PlayerActorPtr playerActor;

#ifdef _WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
	EnginePtr engine(UNIGINE_VERSION, argc, argv);

	//Console *console = Console::get();
	//console->run("world_load unigine_project/unigine_project");
	//console->flush();

	my_init();

	while (0 == engine->isDone())
	{
		engine->update();
		my_update();

		engine->render();

		engine->swap();
	}

	my_shutdown();

	return 0;
}

void my_init()
{
	//init several static spheres
	for (int x = -2; x < 3; x++)
	{
		for (int y = -2; y < 3; y++)
		{
			ObjectMeshStaticPtr meshStatic = ObjectMeshStatic::create("../source/meshes/sphere_00.mesh", 1);
			meshStatic->setMaterial("mesh_base", "*");
			meshStatic->setProperty("surface_base", "*");

			dmat4 trans = meshStatic->getWorldTransform();
			trans.setTranslate(dvec3(x * 2, y * 2, 0));
			meshStatic->setTransform(trans);	
			gMeshStatic.push_back(meshStatic);
		}
	}

	//init playerActorSkinned instance(3rd person)
	playerActor = PlayerActor::create();
	playerActor->setMinThetaAngle(0.0f);
	playerActor->setMaxThetaAngle(60.0f);
	playerActor->setThetaAngle(20.0f);
	playerActor->setJumping(2.0f);

	CPlayerActorSkinned skinnedPlayer(playerActor);
	gPlayerActorSkinned = skinnedPlayer;
	dmat4 tempTrans = playerActor->getIWorldTransform();
	tempTrans.setTranslate(dvec3(0, 0, 1));
	playerActor->setWorldTransform(tempTrans);

	//init PlayerPesecutor instance so that it's used as game camera
	playerPersecutor = PlayerPersecutor::create();
	playerPersecutor->setTarget(playerActor->getNode());
	playerPersecutor->setAnchor(vec3(0.0f, 0.0f, 1.0f));
	playerPersecutor->setMinDistance(2.0f);
	playerPersecutor->setMaxDistance(2.5f);
	playerPersecutor->setMinThetaAngle(0.0f);
	playerPersecutor->setMaxThetaAngle(60.0f);

	Game::get()->setPlayer(playerPersecutor->getPlayer());

}

void my_update(void)
{
	//update objectMeshStatics' pos so that they'll be rendered
	for (TObjectMeshStatic::iterator iter = gMeshStatic.begin(); iter != gMeshStatic.end(); iter++)
	{			
		(*iter)->setWorldTransform((*iter)->getWorldTransform());
	}

	//update playerActorSkinned's pos so that it be rendered
	gPlayerActorSkinned.Update();

	//update the meshPlayer's rotation with camera
	//here is a strange question: Why the following two sentences could work as well?
	playerPersecutor->setPhiAngle(playerActor->getPhiAngle());
	playerPersecutor->setThetaAngle(playerActor->getThetaAngle());
	//playerActor->setPhiAngle(playerPersecutor->getPhiAngle());
	//playerActor->setThetaAngle(playerPersecutor->getThetaAngle());

}

void my_shutdown(void)
{

}



