#ifndef MAPSUITE_HPP
#define MAPSUITE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <OGRE\OgreVector3.h>

class Station;
class Atom;
class PlayerSpawn;
class MapCell;

class MapSuite
{
public:
	static MapSuite& GetInstance();
	static bool LoadMap(std::string a_FileName);
	static Station* GetStartingStation();
	//
	//void CreateAdjacentGirderBuildpoints(int a_X, int a_Y, int a_Z);
	int CreateAdjacentGirderBuildpoints(MapCell* a_pLocMapCell);	//unsigned int a_X, unsigned int a_Y, unsigned int a_Z
	int ClearDependantAdjacentGirderBuildpoints(MapCell* a_pLocMapCell);
	MapCell* CreateNewMapCell(int a_X, int a_Y, int a_Z);
	MapCell* CreateNewMapCell(Ogre::Vector3 a_Coords);

	MapCell* GetCellInDirOrNull(MapCell* a_pSourceMapCell, int a_Direction);
	MapCell* GetCellAtCoordsOrCreate(int a_X, int a_Y, int a_Z);
	MapCell* GetCellAtCoordsOrCreate(Ogre::Vector3 a_Coords);
	MapCell* GetCellAtCoordsOrNull(int a_X, int a_Y, int a_Z);
	MapCell* GetCellAtCoordsOrNull(Ogre::Vector3 a_Coords);
	//
private:
	MapSuite();
	std::vector<Station*> m_MapStations;
	std::unordered_map<std::string, MapCell*> m_MapCellGrid;
	bool LoadMapFile(std::string a_FileName);
	//
	//std::vector< std::vector< std::vector<Atom*> > > m_InstanceGrid;	//3d matrix, each containing either a girder or a build highlight
	std::vector<PlayerSpawn*> m_LoadedSpawns;	//list of all player spawns successfully loaded
	//
};

#endif	MAPSUITE_HPP