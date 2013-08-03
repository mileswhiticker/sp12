#ifndef MAPSUITE_HPP
#define MAPSUITE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <OGRE\OgreVector3.h>

class Station;
class Atom;
class PlayerSpawn;
class Turf;

class MapSuite
{
public:
	static MapSuite& GetInstance();
	static bool LoadMap(std::string a_FileName);
	static Station* GetStartingStation();
	//
	//void CreateAdjacentGirderBuildpoints(int a_X, int a_Y, int a_Z);
	int CreateAdjacentGirderBuildpoints(Turf* a_pCenterTurf);	//unsigned int a_X, unsigned int a_Y, unsigned int a_Z
	int ClearDependantAdjacentGirderBuildpoints(Turf* a_pCenterTurf);
	Turf* CreateTurf(int a_X, int a_Y, int a_Z, int a_TurfType = 1);
	Turf* CreateTurf(Ogre::Vector3 a_Coords, int a_TurfType = 1);
	//
	Turf* GetTurfInDirOrNull(Turf* a_pSourceTurf, int a_Direction);
	Turf* GetTurfInDirOrCreate(Turf* a_pSourceTurf, int a_Direction);
	Turf* GetTurfAtCoordsOrCreate(int a_X, int a_Y, int a_Z);
	Turf* GetTurfAtCoordsOrCreate(Ogre::Vector3 a_Coords);
	Turf* GetTurfAtCoordsOrNull(int a_X, int a_Y, int a_Z);
	Turf* GetTurfAtCoordsOrNull(Ogre::Vector3 a_Coords);
	//
	PlayerSpawn* GetRandomPlayerSpawn();
	//
	static bool SaveMapDefaultName();
	static bool SaveMap(std::string a_FileName);
	//
private:
	MapSuite();
	std::vector<Station*> m_MapStations;
	std::unordered_map<std::string, Turf*> m_TurfGrid;
	bool LoadMapFile(std::string a_FileName);
	//
	//std::vector< std::vector< std::vector<Atom*> > > m_InstanceGrid;	//3d matrix, each containing either a girder or a build highlight
	std::vector<PlayerSpawn*> m_LoadedSpawns;	//list of all player spawns successfully loaded
	//
};

#endif	MAPSUITE_HPP