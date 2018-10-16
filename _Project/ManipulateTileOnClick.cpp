// ManipulateTileOnClick
//
// Manipulates tile when tile is clicked, only if the tile has been
// highlighted and the last ability used is the ManipulateTile ability
//
// Should be generalized to handle all actions pertaining to clicking on tiles
//
// @Ken

#include "ManipulateTileOnClick.h"
#include "ability\AbilityManager.h"
#include "TileInfo.h"
#include "kitten\K_GameObject.h"
#include "components\PowerTracker.h"

#include "kitten\event_system\EventManager.h"
#include "kibble\kibble.hpp"
#include "kibble\databank\databank.hpp"
#include "unit\UnitSpawn.h"
#include "unit\InitiativeTracker\InitiativeTracker.h"

ManipulateTileOnClick::ManipulateTileOnClick()
{

}

ManipulateTileOnClick::~ManipulateTileOnClick()
{

}

void ManipulateTileOnClick::onClick()
{
	TileInfo* tileInfo = m_attachedObject->getComponent<TileInfo>();
	if (ability::AbilityManager::getInstance()->lastAbilityUsed() == MANIPULATE_TILE_ABILITY)
	{
		if (tileInfo->isHighlighted() && tileInfo->getOwnerId() == "NONE")
		{
			tileInfo->setOwnerId(tileInfo->getHighlightedBy());
			kitten::EventManager::getInstance()->triggerEvent(kitten::Event::EventType::Unhighlight_Tile, nullptr);

			kitten::Event* p_data = new kitten::Event(kitten::Event::EventType::Highlight_Tile);
			p_data->putInt(MANIPULATE_TILE_KEY, 1);
			kitten::EventManager::getInstance()->triggerEvent(kitten::Event::EventType::Manipulate_Tile, p_data);
		}
		else
		{
			kitten::EventManager::getInstance()->triggerEvent(kitten::Event::EventType::Unhighlight_Tile, nullptr);
		}
	}
	else if (ability::AbilityManager::getInstance()->lastAbilityUsed() == SUMMON_UNIT)
	{
		if (tileInfo->isHighlighted()) //&& tileInfo->getOwnerId() == tileInfo->getHighlightedBy()) // Commented out for simple testing
		{
			kitten::K_GameObject* board = &m_attachedObject->getTransform().getParent()->getAttachedGameObject();

			PowerTracker* powerTracker = board->getComponent<PowerTracker>();
			unit::UnitData* unitData = kibble::getUnitFromId(2);
			if (unitData->m_Cost <= powerTracker->getCurrentPower())
			{
				kitten::K_GameObject* unit = unit::UnitSpawn::getInstance()->spawnUnitObject(unitData);
				unit->getTransform().place(tileInfo->getPosX() - 0.5f, -1.0f, tileInfo->getPosY());
				powerTracker->summonUnitCost(unitData->m_Cost);
			}
			kitten::EventManager::getInstance()->triggerEvent(kitten::Event::EventType::Unhighlight_Tile, nullptr);
		}
		else
		{
			kitten::EventManager::getInstance()->triggerEvent(kitten::Event::EventType::Unhighlight_Tile, nullptr);
		}
	}
	else
	{
		kitten::EventManager::getInstance()->triggerEvent(kitten::Event::EventType::Unhighlight_Tile, nullptr);
	}
}