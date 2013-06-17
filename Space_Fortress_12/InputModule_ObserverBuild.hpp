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
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//
	int m_CellBuildRange;
	bool m_BuildExpansion;
	int m_TargetAtomTypes;
	int m_TargetStructureTypes;
	int m_TargetTurfTypes;
	//
	Human* m_pTestHuman;
	//
protected:
	bool TrySelect(Atom* a_pTestAtom);
	//
};

#endif	OBSERVERBUILD_HPP