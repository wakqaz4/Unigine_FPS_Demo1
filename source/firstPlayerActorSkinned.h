#ifndef _THIRD_PLAYER_ACTOR_SKINNED_H_
#define _THIRD_PLAYER_ACTOR_SKINNED_H_
#include "IPlayerActorSkinned.h"

#define SHOOT_ANIM_LENGTH 30

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

private:
	PlayerActorPtr		m_playerActor;
	ObjectMeshSkinnedPtr m_armsMesh;
	ObjectMeshSkinnedPtr m_gunMesh;
	float				m_shoot;
	bool				m_zoom;
	bool				m_lButton;
	float				m_shootAnimTime;

	bool					m_enabled;

};

#endif