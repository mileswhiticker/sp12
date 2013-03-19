#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>

struct Client
{
	float lastUpdate;
	std::string ip_address;
};

class Server
{
public:
	//
private:
	std::vector<Client> m_ClientList;
	//
};

#endif	SERVER_HPP