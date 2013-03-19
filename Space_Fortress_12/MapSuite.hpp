#ifndef MAPSUITE_HPP
#define MAPSUITE_HPP

#include <string>
#include <vector>

class Station;

class MapSuite
{
public:
	static MapSuite& GetInstance();
	static bool LoadMap(std::string a_FileName);
	static Station* GetStartingStation();
	//
private:
	MapSuite();
	std::vector<Station*> m_MapStations;
	bool LoadMapFile(std::string a_FileName);
	//
};

#endif	MAPSUITE_HPP