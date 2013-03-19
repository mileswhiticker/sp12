#ifndef	GAME_MANAGER_HPP
#define	GAME_MANAGER_HPP

class GameManager
{
public:
	static GameManager& GetSingleton()
	{
		GameManager* pGameManager = new GameManager();
		return *pGameManager;
	};
	~GameManager();
	//
	void Update(float a_DeltaT);
	//
private:
	GameManager();
	//
};

#endif	GAME_MANAGER_HPP