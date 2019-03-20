#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "networking\ClientNetwork.h"
#include "networking\NetworkData.h"
#include "networking\NetworkLog.h"

#include "kitten\K_GameObject.h"
#include <map>

// Ability 
#include "unit\UnitCommon.h"

namespace networking
{
	class ClientGame
	{
	private:
		ClientNetwork* m_network;

		char m_network_data[MAX_PACKET_SIZE];

		bool m_bServerCalling = false;
		bool m_bGameTurnStart = false;

		unit::Unit* m_commander;

		static int sm_iClientId;
		static bool sm_networkValid;
		static ClientGame* sm_clientGameInstance;

		float m_timeElapsed;
		std::string m_dedicatedServerAddress = "localhost";

		NetworkLog* m_log = nullptr;

		ClientGame(const std::string &p_strAddr);
		~ClientGame();

	public:
		static void createInstance(const std::string &p_strAddr = "127.0.0.1");
		static void destroyInstance();
		static ClientGame* getInstance();
		static bool isNetworkValid() { return sm_networkValid; }

		void setupNetwork(const std::string &p_strAddr = "127.0.0.1");
		void disconnectFromNetwork(bool p_bServerShutdown = false);
		void connectToDedicatedServer();

		void update();

		void useAbility(AbilityPacket& p_packet);
		void sendAbilityPacket(const std::string & p_strAbilityName, ability::AbilityInfoPackage * p_info);
		void setCastTime(AbilityPacket& p_packet);
		void sendCastTimeAbilityPacket(unit::AbilityDescription * p_ad, ability::AbilityInfoPackage * p_info);

		// Compare units via their position
		bool checkSync(int p_x, int p_y);
		void sendDesyncedPacket();

		void sendSkipTurnPacket(unit::Unit* p_unit);

		kitten::K_GameObject* summonUnit(int p_iClientId, int p_iUnitId, int p_iPosX, int p_iPosY);
		void sendSummonUnitPacket(int p_iUnitId, int p_iPosX, int p_iPosY);

		void sendTextChatMessagePacket(const std::string& p_message);
		int sendBasicPacket(PacketTypes p_packetType);

		unit::Unit* getCommander() { return m_commander; }

		static int getClientId() { return sm_iClientId; }
		bool isServerCalling() { return m_bServerCalling; }
		void setServerCalling(bool p_value) { m_bServerCalling = p_value; }
		bool isGameTurnStarted() { return m_bGameTurnStart; }

		void setDedicatedServerAddress(const std::string& p_address) { m_dedicatedServerAddress = p_address; }
		const std::string& getDedicatedServerAddress() { return m_dedicatedServerAddress; }

		inline unit::Unit* getUnitFromPos(int p_x, int p_y)
		{
			TileInfo* tile = BoardManager::getInstance()->getTile(p_x, p_y)->getComponent<TileInfo>();
			unit::Unit* unit = tile->getUnit()->getComponent<unit::Unit>();
			return unit;
		}

		inline UnitPos getPosFromUnit(unit::Unit* p_unit)
		{
			TileInfo* tile = p_unit->getTile()->getComponent<TileInfo>();
			UnitPos unit;
			unit.posX = tile->getPosX();
			unit.posY = tile->getPosY();
			return unit;
		}
	};
}