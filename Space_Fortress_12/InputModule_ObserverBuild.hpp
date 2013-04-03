#ifndef	OBSERVERBUILD_HPP
#define	OBSERVERBUILD_HPP

#include "InputModule.hpp"
class Atom;

class ObserverBuild
:	public InputModule
{
public:
	ObserverBuild(Mob* a_pOwnedMob, Client* a_pOwnedClient);
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//
	int m_CellBuildRange;
	bool m_BuildExpansion;
	int m_TargetStructureTypes;
	//
	void ForceClearAtomIfSelected(Atom* a_pOtherAtom);
	//
private:
	void SelectNewAtom(Atom* a_pNewAtom);
	void ClearSelectedAtom();
	Atom* m_pCurrentlyTargettedAtom;
};

#endif	OBSERVERBUILD_HPP