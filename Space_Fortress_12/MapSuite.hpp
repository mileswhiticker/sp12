#ifndef MAPSUITE_HPP
#define MAPSUITE_HPP

#include <string>
#include <vector>
#include <list>

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
	int CreateAdjacentGirderBuildpoints(unsigned int a_X, unsigned int a_Y, unsigned int a_Z);
	MapCell* CreateNewMapCell(unsigned int a_X, unsigned int a_Y, unsigned int a_Z);
	//
private:
	MapSuite();
	std::vector<Station*> m_MapStations;
	bool LoadMapFile(std::string a_FileName);
	//
	std::vector< std::vector< std::vector<MapCell*> > > m_MapCellGrid;
	//std::vector< std::vector< std::vector<Atom*> > > m_InstanceGrid;	//3d matrix, each containing either a girder or a build highlight
	std::list<MapCell*> m_MapCells;	//vector of all cells, this should only be used for cleanup
	std::vector<PlayerSpawn*> m_LoadedSpawns;	//list of all player spawns successfully loaded
	//
};

#endif	MAPSUITE_HPP