#ifndef STATION_HPP
#define STATION_HPP

#include <vector>
class Girder;
class PlayerSpawn;

class Station
{
public:
	std::vector< std::vector< std::vector<Girder*> > > m_MapGirders;
	std::vector<PlayerSpawn*> m_PlayerSpawns;
	std::string m_Name;
	//
};

#endif	STATION_HPP