#ifndef	RCD_INPUTMODULE_HPP
#define	RCD_INPUTMODULE_HPP

#include "InputModule.hpp"
class Atom;
class Humanoid;
class RCD;

class RCD_InputModule
:	public InputModule
{
public:
	RCD_InputModule(Mob* a_pOwnedMob, RCD* a_pOwnedRCD);
	virtual void Update(float a_DeltaT);
	//
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//
	//virtual void ToggleBuildExpansion();
	//virtual void IncrementTargetBuildOption(int a_IndexIncrement = 1);
	virtual void SetTargetStructure(int a_StructureType);
	virtual void SetTargetTurf(int a_TurfType);
	//
	virtual void ClearBuildTarget();
	virtual float GetInteractRange();
	virtual int GetInteractFilterGroup();
	virtual int GetInteractFilterMask();
	virtual int GetInteractAtomType();
	virtual int GetInteractAtomSubtype();
	//
	virtual bool TryInteract(Atom* a_pInteractAtom, bool a_DoInteract = true);
	virtual void TryInteract(std::vector<Atom*>& a_InteractAtoms);
protected:
	RCD* m_pOwnedRCD;
	Atom* m_pCurrentTargetBuildpoint;
	//
};

#endif	RCD_INPUTMODULE_HPP