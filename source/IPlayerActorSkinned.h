#ifndef _IPLAYER_ACTOR_SKINNED_H_
#define _IPLAYER_ACTOR_SKINNED_H_

#include <iostream>
#include <UnigineApp.h>
#include <UnigineGame.h>
#include <UnigineEditor.h>
#include <UniginePlayerActor.h>
#include <UnigineObjectMeshStatic.h>
#include <UnigineObjectMeshSkinned.h>
#include <vector>

using namespace std;
using namespace Unigine;

#ifdef _WIN32
#define MY_EXTERN extern	
#endif

#define EPSILON 0.000001

#define INCREASE(param) (param += ifps)
#define DECREASE(param) (param -= ifps)

class IPlayerActorSkinned
{
	virtual void Enable() = 0;
	virtual void Disable() = 0;
	virtual int Enabled() = 0;
	virtual int Disabled() = 0;
	virtual void Update() = 0;
	virtual void UpdateAnim() = 0;
	virtual float Saturate(float param) = 0;
	virtual void SetFrame(ObjectMeshSkinnedPtr& m_meshPlayer, int layIndex, int animIndex, int stateIndex) = 0;
};

#endif