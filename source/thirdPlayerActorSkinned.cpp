#include "common.h"
#include "thirdPlayerActorSkinned.h"

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

CThirdPlayerActorSkinned::CThirdPlayerActorSkinned(PlayerActorPtr& playerActor) :
m_bSpawn(false),
m_vSpawnPos(0, 0, 0),
m_jump(0),
m_moveForward(0),
m_moveBackward(0),
m_moveLeft(0),
m_moveRight(0),
m_walk(0),
m_run(0),
m_enabled(true)
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
		animations[i] = "../source/meshes/actor_00/" + animations[i + STATE_WALK_IDLE];
		m_meshPlayer->setAnimation(0, (animations[i]).c_str());
	}
}

void CThirdPlayerActorSkinned::Update(void)
{
	//need respawn
	if (m_bSpawn)
	{
		dmat4 trans = m_meshPlayer->getWorldTransform();
		trans.setColumn3(3, m_vSpawnPos);
		m_meshPlayer->setWorldTransform(trans);
	}

	//update meshPlayer's pos, rot, scale according to camera's pos
	m_meshPlayer->setWorldTransform(m_playerActor->getWorldTransform() * rotateZ((double)90.0f));
#ifdef debug
		{
			dmat4 tempMat = m_meshPlayer->getWorldTransform();
			dvec3 tempVec = tempMat.getTranslate();
			cout <<"player's pos: "<< tempVec.x << " " << tempVec.y << " " << tempVec.z << endl;
			if (m_meshPlayer->isEnabled())
			{
				cout << "meshPlayer is Enabled " << endl;
			}
		}
#endif

	//update animations
	{
		UpdateAnim();
	}
}

float CThirdPlayerActorSkinned::Saturate(float param)
{
	return (param > 0 ? (param < 1 ? param : 1) : 0);
}

void CThirdPlayerActorSkinned::UpdateAnim(void)
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
	//run & walk
	if (m_playerActor->getState(STATE_RUN))
	{
		INCREASE(m_run);
		DECREASE(m_walk);
	}
	else
	{
		DECREASE(m_run);
		INCREASE(m_walk);
	}
	m_run = Saturate(m_run);
	m_walk = Saturate(m_walk);

	//turn
	ADJUST_PARAM(m_turnLeft, m_turnRight, STATE_TURN_LEFT, STATE_TURN_RIGHT);
	//walk straight
	ADJUST_PARAM(m_moveForward, m_moveBackward, STATE_FORWARD, STATE_BACKWARD);
	//walk side
	ADJUST_PARAM(m_moveLeft, m_moveRight, STATE_MOVE_LEFT, STATE_MOVE_RIGHT);

	//decide playerActor's state
	int jumpState = 0;
	int turnState = 0;
	int walkFBState = 0;
	int walkLRState = 0;

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

	if (m_moveForward > m_moveBackward)
	{
		walkFBState = 1;
	}
	else if (m_moveForward < m_moveBackward)
	{
		walkFBState = 2;
	}

	if (m_moveLeft > m_moveRight)
	{
		walkLRState = 1;
	}
	else if (m_moveLeft < m_moveRight)
	{
		walkLRState = 2;
	}

	//calculate state weight
	float idleWeight = 1;
	float jumpWeight = 0;
	float frontWeight = 0;
	float sideWeight = 0;

	jumpWeight = m_jump;
	frontWeight = (1.0 - jumpWeight)*((walkFBState == 1) ? m_moveForward : ((walkFBState == 2) ? m_moveBackward : 0));
	sideWeight = (1.0 - jumpWeight)*((walkLRState == 1) ? m_moveLeft : ((walkLRState == 2) ? m_moveRight : 0));
	frontWeight = Saturate(frontWeight - sideWeight);
	idleWeight = (1.0 - jumpWeight)*Saturate(idleWeight - frontWeight - sideWeight);

	//play animation on 7 layers according to the states and values
	float time = Game::get()->getTime();

	m_meshPlayer->setLayer(0, 1, m_jump);
	
	if (m_meshPlayer->getLayerWeight(0) > EPSILON)
	{
		SetFrame(m_meshPlayer, 0, STATE_WALK_JUMP, STATE_JUMP);
	}

	m_meshPlayer->setLayer(1, 1, m_walk*frontWeight);
	if (m_meshPlayer->getLayerWeight(1) > EPSILON)
	{
		if (walkFBState == 1)
		{
			SetFrame(m_meshPlayer, 1, STATE_WALK_FORWARD, STATE_FORWARD);
		}
		else if (walkFBState == 2)
		{
			SetFrame(m_meshPlayer, 1, STATE_WALK_BACKWARD, STATE_BACKWARD);
		}
	}

	m_meshPlayer->setLayer(2, 1, idleWeight);
	if (m_meshPlayer->getLayerWeight(2) > EPSILON)
	{
		m_meshPlayer->setAnimation(2, (animations[STATE_WALK_IDLE - STATE_WALK_IDLE]).c_str());
		m_meshPlayer->setFrame(2, time*fps[STATE_WALK_IDLE]);
	}

	m_meshPlayer->setLayer(3, 1, m_walk*sideWeight);
	if (m_meshPlayer->getLayerWeight(3) > EPSILON)
	{
		if (walkLRState == 1)
		{
			SetFrame(m_meshPlayer, 3, STATE_WALK_MOVE_LEFT, STATE_MOVE_LEFT);			
		}
		else if (walkLRState == 2)
		{
			SetFrame(m_meshPlayer, 3, STATE_WALK_MOVE_RIGHT, STATE_MOVE_RIGHT);			
		}
	}

	m_meshPlayer->setLayer(5, 1, m_run*frontWeight);
	if (m_meshPlayer->getLayerWeight(5) > EPSILON)
	{
		if (walkFBState == 1)
		{
			SetFrame(m_meshPlayer, 5, STATE_RUN_FORWARD, STATE_FORWARD);			
		}
		else if (walkFBState == 2)
		{
			SetFrame(m_meshPlayer, 5, STATE_RUN_BACKWARD, STATE_BACKWARD);			
		}
	}

	m_meshPlayer->setLayer(6, 1, m_run*sideWeight);
	if (m_meshPlayer->getLayerWeight(6) > EPSILON)
	{
		if (walkLRState == 1)
		{
			SetFrame(m_meshPlayer, 6, STATE_RUN_MOVE_LEFT, STATE_MOVE_LEFT);			
		}
		else if (walkLRState == 2)
		{
			SetFrame(m_meshPlayer, 6, STATE_RUN_MOVE_RIGHT, STATE_MOVE_RIGHT);			
		}
	}
}

void CThirdPlayerActorSkinned::SetFrame(ObjectMeshSkinnedPtr& m_meshPlayer, int layerIndex, int animIndex, int stateIndex)
{
	if (m_meshPlayer->getLayerWeight(layerIndex) > EPSILON)
	{
		m_meshPlayer->setAnimation(layerIndex, (animations[animIndex - STATE_WALK_IDLE]).c_str());
		m_meshPlayer->setFrame(layerIndex, m_playerActor->getStateTime(stateIndex)*fps[animIndex]);		//fpsIndex is the same as animIndex
	}
}

void CThirdPlayerActorSkinned::Enable()
{
	if (m_enabled)
	{
		return;
	}
	m_meshPlayer->setEnabled(1);
	m_enabled = 1;
}

void CThirdPlayerActorSkinned::Disable()
{
	if (m_enabled)
	{
		m_meshPlayer->setEnabled(0);
		m_enabled = 0;
	}
	return;
}

int CThirdPlayerActorSkinned::Enabled()
{	
	return m_enabled;
}

int CThirdPlayerActorSkinned::Disabled()
{
	return (!m_enabled);
}