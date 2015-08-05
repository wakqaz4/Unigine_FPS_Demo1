#ifndef _FIRST_PLAYER_ACTOR_SKINNED_H_
#define _FIRST_PLAYER_ACTOR_SKINNED_H_

#pragma once

#include "IPlayerActorSkinned.h"

#define STATE_COUNT 22

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

typedef vector<ObjectMeshStaticPtr> TObjectMeshStatic;

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

class CThirdPlayerActorSkinned: public IPlayerActorSkinned
{


public:
	CThirdPlayerActorSkinned() {};
	CThirdPlayerActorSkinned(PlayerActorPtr& playerActor);
	~CThirdPlayerActorSkinned()
	{

	}
	
public:
	virtual void Enable();
	virtual void Disable();
	virtual int Enabled();
	virtual int Disabled();
	virtual void Update();
	virtual void UpdateAnim();
	virtual float Saturate(float param);
	virtual void SetFrame(ObjectMeshSkinnedPtr& m_meshPlayer, int layIndex, int animIndex, int stateIndex);

private:
	bool					m_bSpawn;
	dvec3					m_vSpawnPos;
	PlayerActorPtr			m_playerActor;
	ObjectMeshSkinnedPtr	m_meshPlayer;

	float					m_jump;
	float					m_turnLeft;
	float					m_turnRight;
	float					m_moveForward;
	float					m_moveBackward;
	float					m_moveLeft;
	float					m_moveRight;
	float					m_walk;
	float					m_run;

	bool					m_enabled;
		
};

#endif