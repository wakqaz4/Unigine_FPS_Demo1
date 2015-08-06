#ifndef _THIRD_PLAYER_ACTOR_SKINNED_H_
#define _THIRD_PLAYER_ACTOR_SKINNED_H_
#include "IPlayerActorSkinned.h"
#include <UnigineApp.h>

#define SHOOT_ANIM_LENGTH 30

extern App* app;
typedef int(*pFunc)(int);

class CFirstPlayerActorSkinned: public IPlayerActorSkinned
{
public:
	CFirstPlayerActorSkinned(){};
	~CFirstPlayerActorSkinned(){};
	CFirstPlayerActorSkinned(PlayerActorPtr& ptr);

public:
	virtual void Enable();
	virtual void Disable();
	virtual int Enabled();
	virtual int Disabled();
	virtual void Update();
	virtual void UpdateAnim();
	virtual float Saturate(float param);
	virtual void SetFrame(ObjectMeshSkinnedPtr& m_meshPlayer, int layerIndex, int animIndex, int stateIndex);

	int LButtonDownFunc(int button = Unigine::App::BUTTON_LEFT);
	int RButtonDownFunc(int button = Unigine::App::BUTTON_RIGHT);
	int LButtonReleaseFunc(int button = Unigine::App::BUTTON_LEFT);
	int RButtonReleaseFunc(int button = Unigine::App::BUTTON_RIGHT);
private:
	int(*pointer)(int);
	PlayerActorPtr		m_playerActor;
	ObjectMeshSkinnedPtr m_armsMesh;
	ObjectMeshSkinnedPtr m_gunMesh;
	float				m_shoot;
	bool				m_zoom;
	bool				m_lButton;
	bool				m_rButton;
	float				m_shootAnimTime;

	bool					m_enabled;

};

#endif