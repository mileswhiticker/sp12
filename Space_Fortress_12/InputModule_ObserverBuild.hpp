#ifndef	OBSERVERBUILD_HPP
#define	OBSERVERBUILD_HPP

#include "InputModule.hpp"
class Atom;
class Human;

class ObserverBuild
:	public InputModule
{
public:
	ObserverBuild(Mob* a_pOwnedMob, Client* a_pOwnedClient);
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	//
	int m_CellBuildRange;
	bool m_BuildExpansion;
	int m_TargetStructureTypes;
	//
	void ForceClearAtomIfSelected(Atom* a_pOtherAtom);
	Human* m_pTestHuman;
	//
private:
	void SelectNewAtom(Atom* a_pNewAtom);
	void ClearSelectedAtom();
	Atom* m_pCurrentlyTargettedAtom;
};

#endif	OBSERVERBUILD_HPP