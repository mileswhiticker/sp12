#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include "Atom.hpp"

class Girder;
class Turf;
class InputModule;

class Structure
:	public Atom
{
public:
	enum StructureType
	{
		UNKNOWNTYPE = 0,
		OVERLAYPLATING,
		UNDERLAYPLATING,
		LIGHTFIXTURE,
		GRAVPLATES
		//
	};
	enum StructureQuadrants
	{
		UNKNOWNQUADRANT = 0,
		//
		UPLEFT = 1,
		UPRIGHT = 2,
		DOWNLEFT = 4,
		DOWNRIGHT = 8,
		//
		ALLQUADRANT = UPLEFT | UPRIGHT | DOWNLEFT | DOWNRIGHT
	};
	static const std::vector<std::string> QuadrantNames;
	static const std::vector<std::string> LevelNames;

	Structure(Turf* a_pLocTurf, int a_Dir = 0);
	//
	virtual void InstantiateAtom();
	virtual void InstantiateStructure(bool a_IsBuildPoint)=0;
	//
	virtual void CreateFromBuildPoint()=0;
	virtual void DestroyToBuildPoint()=0;
	virtual bool IsBuildPoint();
	//
	virtual bool Interact(Mob* a_pSourceMob, Context* a_pSourceContext, int a_InteractType = 3, Atom* a_pUsedAtom = NULL);
	virtual void Select(InputModule* a_pSourceInputModule);
	virtual void DeSelect(InputModule* a_pSourceInputModule);
	//
	bool MountOnGirder(Girder* a_pMountTarget);
	bool UnmountFromGirder();
	//
	StructureType GetStructureType();
	//
	int GetStructureLevel();
	int GetStructureQuadrant();
	//
protected:
	StructureType m_MyStructureType;
	Girder* m_pMountedGirder;
	Structure* m_pMountedOnStructure;
	bool m_IsBuildPoint;
	//
	int m_StructureLevel;
	int m_FaceQuadrant;		//which of the four quadrants it occupies on the turf face in the direction it's facing
							//bitshift directions to cover multiple quadrants (NORTH == top two quadrants, NORTHEAST == top right quadrant)
	bool m_IsBlocking;		//whether other structures can overlap this structure's quadrants
	//
};

#endif	STRUCTURE_HPP