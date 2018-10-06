#include "UnitTest.h"
#include "ability/node/AbilityNodeManager.h"
#include "unit/InitiativeTracker/InitiativeTracker.h"
#include <iostream>
#include "kibble/kibble.hpp"

// Includes for tile highlighting and manipulating tile
#include "_Project\UseAbilityWhenClicked.h"
#include "_Project\PrintWhenClicked.h"
#include "kitten\K_ComponentManager.h"

//Rock
//test the unit data

namespace unit
{
	UnitTest* UnitTest::m_instance = nullptr;

	UnitTest::UnitTest()
	{
	}

	UnitTest * UnitTest::getInstance()
	{
		return m_instance;
	}

	UnitTest * UnitTest::getInstanceSafe()
	{
		if (m_instance == nullptr)
			m_instance = new UnitTest();
		return m_instance;
	}

	void UnitTest::test()
	{
		//createInstance
		ability::StatusManager::createInstance();
		ability::AbilityManager::createInstance();
		ability::AbilityNodeManager::createInstance();
		kibble::initializeKibbleRelatedComponents();
		InitiativeTracker::createInstance();


		kibble::UnitDataParser* parser = kibble::getUnitDataParserInstance();

		// Testing highlighting tiles and manipulating tiles using testDummy.txt
		{
			kitten::K_ComponentManager* compMan = kitten::K_ComponentManager::getInstance();
			kitten::K_GameObject* testDummyGO = UnitSpawn::getInstance()->spawnUnitObject(parser->getUnit("testDummy.txt"));
			unit::Unit* testDummy = testDummyGO->getComponent<unit::Unit>();
			UnitMonitor::getInstanceSafe()->printUnit(testDummy);
			// Moved PrintWhenClicked and UseAbilityWhenClicked into UnitSpawn::spawnUnitObject() for spawning a Commander

		} 
		// End testing selecting spawned unit

		kitten::K_GameObject* u1 = UnitSpawn::getInstance()->spawnUnitObject(parser->getUnit("Priest.txt"));
		u1->getTransform().move(10.0f, 0.0f, 0.0f);

		//test unit 
		unit::Unit* u = u1->getComponent<unit::Unit>();
		//UnitMonitor::getInstanceSafe()->printUnit(u);

		//Test Initiative Tracker
		//kitten::K_GameObject* u2 = UnitSpawn::getInstance()->spawnUnitObject(parser->getUnit("Engineer.txt"));
		//kitten::K_GameObject* u3 = UnitSpawn::getInstance()->spawnUnitObject(parser->getUnit("Duelist.txt"));

		UnitMonitor::getInstanceSafe()->printIT();
		InitiativeTracker::getInstance()->gameTurnStart();
		//UnitMonitor::getInstanceSafe()->printIT();
		//InitiativeTracker::getInstance()->removeUnit(u2);
		//UnitMonitor::getInstanceSafe()->printIT();
	}
}

