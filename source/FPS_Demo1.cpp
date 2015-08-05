#include <UnigineApp.h>
#include <UnigineEngine.h>
#include <UnigineGame.h>
#include <UnigineConsole.h>
#include <UniginePlayer.h>
#include <UniginePlayerPersecutor.h>
#include "firstPlayerActorSkinned.h"
#include "thirdPlayerActorSkinned.h"
#include "common.h"

using namespace Unigine;


MY_EXTERN void my_init(void);
MY_EXTERN void my_update(void);
MY_EXTERN void my_shutdown(void);

//should figure out sth better to manage objects/nodes/... created by C++API;
TObjectMeshStatic			gMeshStatic;
CThirdPlayerActorSkinned	gPlayerActorSkinned;
CFirstPlayerActorSkinned	gFirstPersonActorSkinned;
PlayerPersecutorPtr			gPlayerPersecutor;
PlayerActorPtr				gPlayerActor;
App*						app;
bool						gOnChangeFpsTps = false;



#ifdef _WIN32
	int wmain(int argc, wchar_t** argv)
#else
	int main(int argc, char** argv)
#endif
{
	EnginePtr engine(UNIGINE_VERSION, argc, argv);
	app = App::get();

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

	//init the true player actor
	gPlayerActor = PlayerActor::create();
	gPlayerActor->setMinThetaAngle(0.0f);
	gPlayerActor->setMaxThetaAngle(60.0f);
	gPlayerActor->setThetaAngle(20.0f);
	gPlayerActor->setJumping(2.0f);

	//init playerActorSkinned instance(3rd person)
	CThirdPlayerActorSkinned skinnedPlayer(gPlayerActor);
	gPlayerActorSkinned = skinnedPlayer;
	//dmat4 tempTrans = gPlayerActor->getIWorldTransform();
	//tempTrans.setTranslate(dvec3(0, 0, 1));
	//gPlayerActor->setWorldTransform(tempTrans);

	//init firstPlayerActorSkinned instance
	CFirstPlayerActorSkinned firstPlayerSkinned(gPlayerActor);
	gFirstPersonActorSkinned = firstPlayerSkinned;		

	//init PlayerPesecutor instance so that it's used as game camera
	gPlayerPersecutor = PlayerPersecutor::create();
	gPlayerPersecutor->setTarget(gPlayerActor->getNode());
	gPlayerPersecutor->setAnchor(vec3(0.0f, 0.0f, 1.0f));
	gPlayerPersecutor->setMinDistance(2.0f);
	gPlayerPersecutor->setMaxDistance(2.5f);
	gPlayerPersecutor->setMinThetaAngle(0.0f);
	gPlayerPersecutor->setMaxThetaAngle(60.0f);

	Game::get()->setPlayer(gPlayerPersecutor->getPlayer());

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
	gFirstPersonActorSkinned.Update();
	//update the meshPlayer's rotation with camera
	//here is a strange question: Why the following two sentences could work as well?
	gPlayerPersecutor->setPhiAngle(gPlayerActor->getPhiAngle());
	gPlayerPersecutor->setThetaAngle(gPlayerActor->getThetaAngle());


	//change between first person view & third person view
	if (app->getKeyState(app->KEY_F1) == 1)
	{
		gOnChangeFpsTps = 1;
	}
	else if (gOnChangeFpsTps == 1)
	{
		if (gFirstPersonActorSkinned.Enabled() == 1)
		{
			gFirstPersonActorSkinned.Disable();
			gPlayerActorSkinned.Enable();

			gPlayerPersecutor->setAnchor(vec3(0.0f, 0.0f, 1.0f));
			gPlayerPersecutor->setMinDistance(2.0f);
			gPlayerPersecutor->setMaxDistance(2.5f);
			gPlayerPersecutor->setMinThetaAngle(0.0f);
			gPlayerPersecutor->setMaxThetaAngle(60.0f);
		}
		else
		{
			gFirstPersonActorSkinned.Enable();
			gPlayerActorSkinned.Disable();

			gPlayerPersecutor->setAnchor(vec3(0.0f, 0.0f, 1.1f));
			gPlayerPersecutor->setMinDistance(0.5f);
			gPlayerPersecutor->setMaxDistance(0.5f);
			gPlayerPersecutor->setMinThetaAngle(0.0f);
			gPlayerPersecutor->setMaxThetaAngle(90.0f);
		}
		gOnChangeFpsTps = 0;
	}
}

void my_shutdown(void)
{

}



