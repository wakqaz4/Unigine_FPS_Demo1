#ifndef _UNIGINE_PROJECT_H_
#define _UNIGINE_PROJECT_H_

#include <iostream>
#include <UnigineApp.h>
#include <UniginePlayerActor.h>
#include <UnigineObjectMeshStatic.h>
#include <UnigineObjectMeshSkinned.h>
#include <vector>

using namespace Unigine;
using namespace std;

#define EPSILON 0.000001
#define STATE_COUNT 22

#ifdef _WIN32
	#define MY_EXTERN extern	
#endif

typedef vector<ObjectMeshStaticPtr> TObjectMeshStatic;

string animations[STATE_COUNT - 1] =
{
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"idle.anim",
	"jump.anim",
	"walk_fwd.anim",
	"walk_bwd.anim",
	"walk_lt.anim",
	"walk_rt.anim",
	"walk_lt.anim",
	"walk_rt.anim",
	"run_fwd.anim",
	"walk_bwd.anim",
	"run_lt.anim",
	"run_rt.anim"
};

float fps[STATE_COUNT - 1] =
{
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	25.0f,
	2.0f,
	25.0f,
	45.0f,
	45.0f,
	45.0f,
	45.0f,
	45.0f,
	25.0f,
	55.0f,
	25.0f,
	25.0f
};

class CPlayerActorSkinned
{
#define INCREASE(param) (param += ifps)
#define DECREASE(param) (param -= ifps)

#define ADJUST_PARAM(param1, param2, state1, state2) \
	if (m_playerActor->getState(state1))\
	{\
	INCREASE(param1); \
	DECREASE(param2); \
}\
else if (m_playerActor->getState(state2))\
	{\
	INCREASE(param2); \
	DECREASE(param1); \
}\
else\
	{\
	DECREASE(param1); \
	DECREASE(param2); \
}\
	param1 = Saturate(param1); \
	param2 = Saturate(param2);

	enum 
	{
		STATE_WALK,
		STATE_CROUCH,
		STATE_RUN,
		STATE_FORWARD,
		STATE_BACKWARD,
		STATE_MOVE_LEFT,
		STATE_MOVE_RIGHT,
		STATE_TURN_LEFT,
		STATE_TURN_RIGHT,
		STATE_WALK_IDLE,
		STATE_WALK_JUMP,
		STATE_WALK_FORWARD,
		STATE_WALK_BACKWARD,
		STATE_WALK_MOVE_LEFT,
		STATE_WALK_MOVE_RIGHT,
		STATE_WALK_TURN_LEFT,
		STATE_WALK_TURN_RIGHT,
		STATE_RUN_FORWARD,
		STATE_RUN_BACKWARD,
		STATE_RUN_MOVE_LEFT,
		STATE_RUN_MOVE_RIGHT
		//STATE_COUNT
	};

public:
	CPlayerActorSkinned() {};

	~CPlayerActorSkinned()
	{

	}
	
public:
	CPlayerActorSkinned(PlayerActorPtr& playerActor):
		m_bSpawn(false),
		m_vSpawnPos(0, 0, 0),
		m_jump(0),
		m_walkForward(0),
		m_walkBackward(0),
		m_walkLeft(0),
		m_walkRight(0),
		m_runForward(0),
		m_runLeft(0),
		m_runRight(0)
	{
		m_playerActor = playerActor;
		m_meshPlayer = ObjectMeshSkinned::create("../source/meshes/actor_00.mesh", 0);
		m_meshPlayer->setMaterial("mesh_base", "*");
		m_meshPlayer->setProperty("surface_base", "*");

		dmat4 trans = m_meshPlayer->getWorldTransform();
		trans.setTranslate(m_vSpawnPos);
		m_meshPlayer->setWorldTransform(trans);		

		for (int i = 0; i < m_meshPlayer->getNumSurfaces(); i++)
		{
			m_meshPlayer->setCollision(0, i);
		}

		m_meshPlayer->setNumLayers(7);
		for (int i = 0; i < 12; i++)
		{
			animations[i] = "../source/meshes/actor_00/" + animations[i+STATE_WALK_IDLE];
			m_meshPlayer->setAnimation(0, (animations[i]).c_str());
		}
	}

	void Update(void)
	{
		//need respawn
		if (m_bSpawn)
		{
			dmat4 trans = m_meshPlayer->getWorldTransform();
			trans.setColumn3(3, m_vSpawnPos);
			m_meshPlayer->setWorldTransform(trans);
		}
		//update meshPlayer's pos according to camera's pos
		{			
			m_meshPlayer->setWorldTransform(m_playerActor->getWorldTransform() * rotateZ((double)90.0f));
		}
		//update animations
		{
			UpdateAnim();
		}

	}

	float Saturate(float param)
	{
		return (param > 0 ? (param < 1 ? param : 1) : 0);
	}

	void UpdateAnim(void)
	{
		float ifps = Game::get()->getIFps()* 4.0f;

		enum
		{
			STATE_FORWARD = 0,
			STATE_BACKWARD,
			STATE_MOVE_LEFT,
			STATE_MOVE_RIGHT,
			STATE_TURN_UP,
			STATE_TURN_DOWN,
			STATE_TURN_LEFT,
			STATE_TURN_RIGHT,
			STATE_CROUCH,
			STATE_JUMP,
			STATE_RUN,
			STATE_USE,
			STATE_FIRE,
			NUM_STATES,
		};

		//jump
		if (m_playerActor->getState(STATE_JUMP))
		{
			m_jump += ifps;
		}
		else
		{
			m_jump -= ifps;
		}
		m_jump = Saturate(m_jump);

		//turn
		if (m_playerActor->getState(STATE_TURN_LEFT))
		{
			m_turnLeft += ifps;
			m_turnRight -= ifps;
		}
		else if (m_playerActor->getState(STATE_TURN_RIGHT))
		{
			m_turnLeft -= ifps;
			m_turnRight += ifps;
		}
		else 
		{
			m_turnLeft -= ifps;
			m_turnRight -= ifps;
		}
		m_turnLeft = Saturate(m_turnLeft);
		m_turnRight = Saturate(m_turnRight);

		//walk straight
		if (m_playerActor->getState(STATE_FORWARD))
		{
			INCREASE(m_walkForward);
			DECREASE(m_walkBackward);
		}
		else if (m_playerActor->getState(STATE_BACKWARD))
		{
			INCREASE(m_walkBackward);
			DECREASE(m_walkForward);
		}
		else
		{
			DECREASE(m_walkForward);
			DECREASE(m_walkBackward);
		}
		m_walkForward = Saturate(m_walkForward);
		m_walkBackward = Saturate(m_walkBackward);

		//walk side
		if (m_playerActor->getState(STATE_MOVE_LEFT))
		{
			INCREASE(m_walkLeft);
			DECREASE(m_walkRight);
		}
		else if (m_playerActor->getState(STATE_MOVE_RIGHT))
		{
			INCREASE(m_walkRight);
			DECREASE(m_walkLeft);
		}
		else
		{
			DECREASE(m_walkRight);
			DECREASE(m_walkLeft);
		}
		m_walkLeft = Saturate(m_walkLeft);
		m_walkRight = Saturate(m_walkRight);

		//run straight
		if (m_playerActor->getState(STATE_RUN))
		{
			INCREASE(m_runForward);
		}
		else if (m_playerActor->getState(STATE_RUN))
		{
			DECREASE(m_runForward);
		}
		else
		{
			DECREASE(m_runForward);
		}
		m_runForward = Saturate(m_runForward);

		//decide playerActor's state

		int jumpState = 0;
		int turnState = 0;
		int walkFBState = 0;
		int walkLRState = 0;
		int runFBState = 0;
		int runLRState = 0;

		if (m_jump > 0)
		{
			jumpState = 1;
		}

		if (m_turnLeft > m_turnRight)
		{
			turnState = 1;
		}
		else if (m_turnLeft < m_turnRight)
		{
			turnState = 2;
		}

		if (m_walkForward > m_walkBackward)
		{
			walkFBState = 1;
		}
		else if (m_walkForward < m_walkBackward)
		{
			walkFBState = 2;
		}
		
		if (m_walkLeft > m_walkRight)
		{
			walkLRState = 1;
		}
		else if (m_walkLeft < m_walkRight)
		{
			walkLRState = 2;
		}

		if (m_runForward > 0)
		{
			runFBState = 1;
		}

		if (m_runLeft > m_runRight)
		{
			runLRState = 1;
		}
		else if (m_runLeft < m_runRight)
		{
			runLRState = 2;
		}

		//calculate state weight
		float jumpWeight = 0;
		float walkForwardWeight = 0;
		float walkBackwardWeight = 0;
		float walkLeftWeight = 0;
		float walkRightWeight = 0;
		float runForwardWeight = 0;
		float runLeftWeight = 0;
		float runRightWeight = 0;

		jumpWeight = m_jump;

		walkForwardWeight = (walkFBState == 1) ? m_walkForward : 0;
		walkBackwardWeight = (walkFBState == 2) ? m_walkBackward : 0;

		walkLeftWeight = (walkLRState == 1) ? m_walkLeft : 0;
		walkRightWeight = (walkLRState == 2) ? m_walkRight : 0;

		runForwardWeight = (runFBState == 1) ? m_runForward : 0;

		runLeftWeight = (runLRState == 1) ? m_runLeft : 0;
		runRightWeight = (runLRState == 1) ? m_runRight : 0;

		//play animation on 7 layers according to the states and values
		float time = Game::get()->getTime();

		m_meshPlayer->setLayer(0, 1, m_jump);
		if (m_meshPlayer->getLayerWeight(0) > EPSILON)
		{
			m_meshPlayer->setAnimation(0, (animations[STATE_WALK_JUMP - STATE_WALK_IDLE]).c_str());
			m_meshPlayer->setFrame(0, time*fps[STATE_WALK_JUMP]);
		}

		m_meshPlayer->setLayer(1, 1, walkForwardWeight);
		if (m_meshPlayer->getLayerWeight(1) > EPSILON)
		{
			m_meshPlayer->setAnimation(1, (animations[STATE_WALK_FORWARD - STATE_WALK_IDLE]).c_str());
			m_meshPlayer->setFrame(1, time*fps[STATE_WALK_FORWARD]);
		}

		m_meshPlayer->setLayer(2, 1, walkBackwardWeight);
		if (m_meshPlayer->getLayerWeight(2) > EPSILON)
		{
			m_meshPlayer->setAnimation(2, (animations[STATE_WALK_BACKWARD - STATE_WALK_IDLE]).c_str());
			m_meshPlayer->setFrame(2, time*fps[STATE_WALK_BACKWARD]);
		}

		m_meshPlayer->setLayer(3, 1, walkLeftWeight);
		if (m_meshPlayer->getLayerWeight(3) > EPSILON)
		{
			m_meshPlayer->setAnimation(3, (animations[STATE_WALK_MOVE_LEFT - STATE_WALK_IDLE]).c_str());
			m_meshPlayer->setFrame(3, time*fps[STATE_WALK_MOVE_LEFT]);
		}

		m_meshPlayer->setLayer(4, 1, walkRightWeight);
		if (m_meshPlayer->getLayerWeight(4) > EPSILON)
		{
			m_meshPlayer->setAnimation(4, animations[STATE_WALK_MOVE_RIGHT - STATE_WALK_IDLE].c_str());
			m_meshPlayer->setFrame(4, time*fps[STATE_WALK_MOVE_RIGHT]);
		}

	}

	//void SetSpawnPos(dvec3 pos)
	//{
	//	m_bSpawn = true;
	//	m_vSpawnPos = pos;
	//}

private:
	bool					m_bSpawn;
	dvec3					m_vSpawnPos;
	PlayerActorPtr			m_playerActor;
	ObjectMeshSkinnedPtr	m_meshPlayer;

	float					m_jump;
	float					m_turnLeft;
	float					m_turnRight;
	float					m_walkForward;
	float					m_walkBackward;
	float					m_walkLeft;
	float					m_walkRight;
	float					m_runForward;
	float					m_runLeft;
	float					m_runRight;
	

};

#endif