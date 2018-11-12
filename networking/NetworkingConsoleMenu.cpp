#include "networking\NetworkingConsoleMenu.h"
#include "kitten\InputManager.h"

#include <iostream>
#include <process.h>

// Networking stuff
#include "networking\ServerGame.h"
#include "networking\ClientGame.h"
#include "networking\NetworkData.h"
#include "unit\InitiativeTracker\InitiativeTracker.h"

NetworkingConsoleMenu::NetworkingConsoleMenu()
	:
	m_bMenuOpen(false),
	m_bPrintText(false),
	m_bClientUpdate(false),
	m_bServerUpdate(false)
{
	// Uncomment for testing on a single machine, then enable client instance through menu
	/*networking::ServerGame::createInstance();
	if (networking::ServerGame::getInstance()->setupNetwork())
	{
		_beginthread(serverLoop, 0, (void*)12);
	} else
	{
		networking::ServerGame::destroyInstance();
	}*/
}

NetworkingConsoleMenu::~NetworkingConsoleMenu()
{
	if (m_bClientUpdate || networking::ClientGame::getInstance())
	{
		networking::ClientGame::destroyInstance();
	}

	if (m_bServerUpdate || networking::ServerGame::getInstance())
	{
		networking::ServerGame::destroyInstance();
	}
}

void NetworkingConsoleMenu::update()
{
	if (input::InputManager::getInstance()->keyDown(m_cEnterMenuKey) && !input::InputManager::getInstance()->keyDownLast(m_cEnterMenuKey))
	{
		m_bMenuOpen = true;
		m_bPrintText = true;
	}

	if (m_bMenuOpen)
	{
		if (m_bPrintText)
		{
			printf("\n** Networking Console Menu Opened **\n");
			printf("%c - Host game\n", m_cHostKey);
			printf("%c - Stop hosting game\n", m_cStopHostKey);
			printf("%c - Connect to host\n", m_cConnectKey);
			printf("%c - Disconnect from host\n", m_cDisconnectKey);
			printf("%c - Exit menu\n", m_cExitMenuKey);
			printf("************************************\n\n");
			m_bPrintText = false;
		}

		// Host game option
		if (input::InputManager::getInstance()->keyDown(m_cHostKey) && !input::InputManager::getInstance()->keyDownLast(m_cHostKey))
		{
			printf("Host Game option selected\n");
			
			hostGame();
			m_bMenuOpen = false;
		}

		// Stop hosting option
		if (input::InputManager::getInstance()->keyDown(m_cStopHostKey) && !input::InputManager::getInstance()->keyDownLast(m_cStopHostKey))
		{
			printf("Stop hosting option selected\n");

			stopHosting();
			m_bMenuOpen = false;
		}

		// Connect to host option
		if (input::InputManager::getInstance()->keyDown(m_cConnectKey) && !input::InputManager::getInstance()->keyDownLast(m_cConnectKey))
		{
			printf("Connect to host option selected\n");
			
			connectToHost();
			m_bMenuOpen = false;
		}

		// Disconnect from host option
		if (input::InputManager::getInstance()->keyDown(m_cDisconnectKey) && !input::InputManager::getInstance()->keyDownLast(m_cDisconnectKey))
		{
			printf("Disconnect from host option selected\n");
			
			disconnectFromHost();
			m_bMenuOpen = false;
		}

		// Exit menu option
		if (input::InputManager::getInstance()->keyDown(m_cExitMenuKey) && !input::InputManager::getInstance()->keyDownLast(m_cExitMenuKey))
		{
			printf("** Networking Console Menu Closed **\n");
			m_bMenuOpen = false;
		}
	}

	// Call updates if ClientGame/ServerGame are initialized
	if (m_bClientUpdate)
	{
		networking::ClientGame::getInstance()->update();
	}

	if (m_bServerUpdate)
	{
		networking::ServerGame::getInstance()->update();
	}
}

void NetworkingConsoleMenu::hostGame()
{
	// Check to make sure ServerGame and ClientGame are not initialized
	if (!networking::ServerGame::getInstance() && !networking::ClientGame::getInstance())
	{
		networking::ServerGame::createInstance();
		if (networking::ServerGame::getInstance()->isNetworkValid())
		{
			m_bServerUpdate = true;
			printf("Server network setup complete\n");

			// Server setup successful, now setup ClientGame
			networking::ClientGame::createInstance();
			m_bClientUpdate = checkClientNetwork();
		} else
		{
			networking::ServerGame::destroyInstance();
		}
	} else
	{
		printf("Already hosting/connected to a server\n");
	}
	
}

void NetworkingConsoleMenu::stopHosting()
{
	// Host player should be running an instance of ServerGame and ClientGame
	// First, shutdown local ClientGame
	disconnectFromHost(true);

	// Then, shutdown local server, which alerts all connected clients
	if (networking::ServerGame::getInstance())
	{
		if (networking::ServerGame::getInstance()->isNetworkValid())
		{
			printf("Shutting down server\n");
			networking::ServerGame::getInstance()->shutdownNetwork();
			networking::ServerGame::destroyInstance();
			m_bServerUpdate = false;
		} else
		{
			printf("Server was not initialized\n");
		}
	} else
	{
		printf("ServerGame is not setup\n");
	}
}

void NetworkingConsoleMenu::connectToHost()
{
	networking::ClientGame* client = networking::ClientGame::getInstance();

	// Check if we've already created an instance of ClientGame
	if (client)
	{
		// Check if we're already connected to a host
		if (!checkClientNetwork())
		{
			printf("Enter an address: ");
			std::string addr;
			std::cin >> addr;
			client->setupNetwork(addr);

			m_bClientUpdate = checkClientNetwork();
		} else
		{
			printf("[Client: %d]: already connected to host\n", client->getClientId());
		}
	} else // If not, get address and create ClientGame instance
	{
		printf("Enter an address: ");
		std::string addr;
		std::cin >> addr;

		networking::ClientGame::createInstance(addr);

		m_bClientUpdate = checkClientNetwork();
	}
}

void NetworkingConsoleMenu::disconnectFromHost(bool p_bServerShutdown)
{
	// If ServerGame exists and disconnectFromHost was not called by Server, then we should not 
	// remove ClientGame. Host should always disconnect via Stop hosting game option
	if (networking::ServerGame::getInstance() && !p_bServerShutdown)
	{
		printf("Host cannot disconnect from host!\n");
	} else if (networking::ClientGame::getInstance())
	{
		// Make sure that ClientGame is connected to something otherwise don't do anything
		if (checkClientNetwork())
		{
			printf("[Client: %d] disconnecting from server\n", networking::ClientGame::getInstance()->getClientId());

			networking::ClientGame::getInstance()->disconnectFromNetwork(p_bServerShutdown);
			networking::ClientGame::destroyInstance();

			m_bClientUpdate = false;
		}
	} else
	{
		printf("ClientGame is not setup\n");
	}
}

bool NetworkingConsoleMenu::checkClientNetwork()
{
	if (networking::ClientGame::getInstance())
	{
		if (networking::ClientGame::getInstance()->isNetworkValid())
		{
			//printf("Client network setup; connected to server\n");
			return true;
		} else
		{
			printf("Client network error; not connected to server\n");
			networking::ClientGame::destroyInstance();
			return false;
		}
	}
	return false;
}

void NetworkingConsoleMenu::setMenuKeys(
	char p_cEnterMenuKey, char p_cExitMenuKey,
	char p_cHostKey, char p_cStopHostKey,
	char p_cConnectKey, char p_cDisconnectKey)
{
	m_cEnterMenuKey = p_cEnterMenuKey;
	m_cHostKey = p_cHostKey;
	m_cStopHostKey = p_cStopHostKey;
	m_cConnectKey = p_cConnectKey;
	m_cDisconnectKey = p_cDisconnectKey;
	m_cExitMenuKey = p_cExitMenuKey;
}

void serverLoop(void* arg)
{
	while (true)
	{
		networking::ServerGame::getInstance()->update();
	}
}