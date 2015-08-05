#include "common.h"
#include "firstPlayerActorSkinned.h"

CFirstPlayerActorSkinned::CFirstPlayerActorSkinned(PlayerActorPtr& ptr):
	m_shoot(false),
	m_zoom(false),
	m_enabled(false)
{
	m_playerActor = ptr;

	//set first person's arms
	if (Editor::get()->findNode("arm_0") != -1)
	{
		m_armsMesh = ObjectMeshSkinned::create(Editor::get()->getNodeByName("arm_0"));
		m_armsMesh->setMaterial("mesh_base", "*");
		m_armsMesh->setProperty("surface_base", "*");
	}
	else
	{
		m_armsMesh = ObjectMeshSkinned::create("../source/meshes/sphere_01.mesh", 0);
		m_armsMesh->setMaterial("mesh_base", "*");
		m_armsMesh->setProperty("surface_base", "*");
	}
	
	for (int i = 0; i < m_armsMesh->getNumSurfaces(); i++)
	{
		m_armsMesh->setCollision(0, i);
	}
	m_armsMesh->setNumLayers(1);
	m_armsMesh->setAnimation(0, "../source/meshes/first_person/meshes/arm.anim");

	//set first person's gun
	if (Editor::get()->findNode("gun_0") != -1)
	{
		NodePtr ptr = Editor::get()->getNodeByName("gun_0");
		m_gunMesh = ObjectMeshSkinned::create(ptr);		
	}
	else
	{
		m_gunMesh = ObjectMeshSkinned::create("../source/meshes/first_person/meshes/QBZ95_long.mesh", 0);
		m_gunMesh->setMaterial("mesh_base", "*");
		m_gunMesh->setProperty("surface_base", "*");
	}
	for (int i = 0; i < m_gunMesh->getNumSurfaces(); i++)
	{
		m_gunMesh->setCollision(0, i);
	}
	m_gunMesh->setNumLayers(1);
	m_gunMesh->setAnimation(0, "../source/meshes/first_person/meshes/QBZ95_long.anim");

	//hide at first
	m_armsMesh->setEnabled(0);
	m_gunMesh->setEnabled(0);

}

void CFirstPlayerActorSkinned::Update()
{
	//update pos & rot
	//dvec3 transl(0.0f, 0.0f, 3.0f);
	//dmat4 transMat;
	//transMat = m_playerActor->getWorldTransform();
	//transMat.setTranslate(transl);
	//m_armsMesh->setWorldTransform(transMat);	
	//m_gunMesh->setWorldTransform(transMat);


	//{
	//	dvec3 tempVec(0.01, 0.01, 0.01);
	//	dmat4 trans = m_armsMesh->getWorldTransform();
	//	//trans.setScale(tempVec);
	//	//m_armsMesh->setWorldTransform(trans);

	//	trans = m_playerActor->getWorldTransform();
	//	tempVec.z = 1.0;
	//	trans.setTranslate(tempVec);
	//	m_armsMesh->setWorldTransform(trans);
	//}
	{
		dvec3 tempVec(0.05, 0.05, 1);
		dvec3 scaleVec(0.01, 0.01, 0.01);
		m_armsMesh->setWorldTransform(m_playerActor->getWorldTransform() * rotateZ((double)270.0f) * translate(tempVec) * scale(scaleVec));
		m_gunMesh->setWorldTransform(m_playerActor->getWorldTransform() * rotateZ((double)270.0f) * translate(tempVec) * scale(scaleVec));
	
	}


#ifdef debug
	{
		dmat4 tempMat = m_armsMesh->getWorldTransform();
		dvec3 tempVec = tempMat.getTranslate();
		cout <<"arms' pos: "<< tempVec.x << " " << tempVec.y << " " << tempVec.z << endl;
		if (m_armsMesh->isEnabled())
		{
			cout << "armsMesh enabled" << endl;
		}		
	}
#endif

	//update anim
	UpdateAnim();

	//
};

void CFirstPlayerActorSkinned::UpdateAnim()
{
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

	float ifps = Game::get()->getIFps()* 4.0f;
	//float time = Game::get()->getTime();
	if (m_playerActor->getState(STATE_FIRE))
	{
		INCREASE(m_shoot);
	}
	else
	{
		DECREASE(m_shoot);
	}
	//m_shootAnimTime = m_playerActor->getStateTime(STATE_FIRE);

	//play frame
	m_armsMesh->setLayerWeight(0, m_shoot);
	if (m_armsMesh->getLayerWeight(0))
	{
		m_armsMesh->setFrame(0, m_playerActor->getStateTime(STATE_FIRE)* ifps);
	}
	m_gunMesh->setLayerWeight(0, m_shoot);
	if (m_gunMesh->getLayerWeight(0))
	{
		m_gunMesh->setFrame(0, m_playerActor->getStateTime(STATE_FIRE) * ifps);
	}
};

float CFirstPlayerActorSkinned::Saturate(float param)
{
	return (param > 0 ? (param < 1 ? param : 1) : 0);
};

void CFirstPlayerActorSkinned::SetFrame(ObjectMeshSkinnedPtr& m_meshPlayer, int layerIndex, int animIndex, int stateIndex)
{
	return;
};

void CFirstPlayerActorSkinned::Enable()
{
	if (m_enabled)
	{
		return;
	}
	m_armsMesh->setEnabled(1);
	m_gunMesh->setEnabled(1);
	m_enabled = 1;
}

void CFirstPlayerActorSkinned::Disable()
{
	if (!m_enabled)
	{
		return;
	}
	m_armsMesh->setEnabled(0);
	m_gunMesh->setEnabled(0);
	m_enabled = 0;
}

int CFirstPlayerActorSkinned::Enabled()
{
	return m_enabled;
}

int CFirstPlayerActorSkinned::Disabled()
{
	return (!m_enabled);
}