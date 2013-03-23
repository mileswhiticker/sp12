#include "AtomManager.hpp"

#include "Object.hpp"
#include "Girder.hpp"
#include "Structure.hpp"

#include "OgreHelper.hpp"

AtomManager::AtomManager()
:	cellsFlashing(false)
{
	//
}

void AtomManager::ToggleCellFlashing()
{
	cellsFlashing = !cellsFlashing;
	if(cellsFlashing)
	{
		for(auto it = m_GirdersInWorld.begin(); it != m_GirdersInWorld.end(); ++it)
		{
			(*it)->SetFlashingColour(Ogre::ColourValue(0,0,1,1));
		}
	}
	else
	{
		for(auto it = m_GirdersInWorld.begin(); it != m_GirdersInWorld.end(); ++it)
		{
			(*it)->StopFlashingColour();
		}
	}
}

void AtomManager::Update(float a_DeltaT)
{
	for(auto it = m_AtomsInWorld.begin(); it != m_AtomsInWorld.end(); ++it)
	{
		(*it)->Update(a_DeltaT);
	}
}

Atom* AtomManager::CreateAtom(int a_AtomType, Ogre::Vector3 a_Pos, Atom** a_ppAtomLocation)
{
	Atom* pOut = NULL;
	switch(a_AtomType)
	{
	case(Atom::GIRDER):
		{
			pOut = new Girder(a_Pos);
			m_GirdersInWorld.push_back((Girder*)pOut);
			m_AtomsInWorld.push_back(pOut);
			break;
		}
	case(Atom::OBJECT):
		{
			Box* pBox = new Box(a_Pos);
			m_ObjectsInWorld.push_back(pBox);
			m_AtomsInWorld.push_back(pBox);
			break;
		}
	case(Atom::GIRDER_BUILDPOINT):
		{
			pOut = new Girder(a_Pos, true);
			m_GirdersInWorld.push_back((Girder*)pOut);
			m_AtomsInWorld.push_back(pOut);
			break;
		}
	default:
		{
			std::cout << "Unknown atom type: " << a_AtomType << std::endl;
			break;
		}
	}
	if(a_ppAtomLocation)
	{
		*a_ppAtomLocation = pOut;
	}
	return pOut;
}

/*bool AtomManager::CreateObject(std::string a_TypeTag, Ogre::Vector3 a_Position)
{
	if(!a_TypeTag.compare("box"))
	{
		Box* pBox = new Box(a_Position);
		m_ObjectsInWorld.push_back(pBox);
		m_AtomsInWorld.push_back(pBox);
		return true;
	}
	else
	{
		std::cout << "WARNING: Attempted to create object with Unknown type tag '" << a_TypeTag << "'" << std::endl;
	}
	return false;
}

Cell* AtomManager::CreateCell(Ogre::SceneNode* a_pSceneNode, Ogre::Vector3 a_Pos, std::string a_SkeletonType)
{
	m_CellsInWorld.push_back(new Cell(a_pSceneNode, a_Pos, a_SkeletonType));
	m_AtomsInWorld.push_back(m_CellsInWorld.back());

	return m_CellsInWorld.back();
}*/
