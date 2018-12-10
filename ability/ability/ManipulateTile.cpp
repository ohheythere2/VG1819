// ManipulateTile
// 
// Sprint #2 - First implementation of ManipulateTile ability
// Gets the target tile position and changes its colour
//
// @Ken

#include "ability\ability\Ability.h"
#include "kitten\event_system\EventManager.h"
#include "unit/Unit.h"
#include "board/tile/TileInfo.h"
#include "board/BoardManager.h"

namespace ability
{
	int ManipulateTile::effect(AbilityInfoPackage* p_info)
	{
		TileInfo* tileInfo = p_info->m_targetTilesGO[0]->getComponent<TileInfo>();

		tileInfo->setOwnerId(p_info->m_source->m_clientId);

		//kitten::EventManager::getInstance()->triggerEvent(kitten::Event::EventType::Unhighlight_Tile, nullptr);

		kitten::Event* e = new kitten::Event(kitten::Event::EventType::Manipulate_Tile);
		e->putInt(MANIPULATE_TILE_KEY, 1);
		kitten::EventManager::getInstance()->triggerEvent(kitten::Event::EventType::Manipulate_Tile, e);

		//highlight the tile
		kitten::Event* t = new kitten::Event(kitten::Event::EventType::Highlight_Tile);

		t->putString("mode", ABILITY_MANIPULATE_TILE);

		kitten::Event::TileList list;
		list.push_back(tileInfo->getPos());
		t->putTileList(&list);

		kitten::EventManager::getInstance()->triggerEvent(kitten::Event::EventType::Highlight_Tile, t);

		/*
		kitten::Event* p_data = new kitten::Event(kitten::Event::EventType::Highlight_Tile);
		p_data->putTileList(&p_info->m_targetTilesPos);
		p_data->putString(TILE_OWNER_KEY, p_info->m_source->m_ID);
		p_data->putString("mode", "all");
		p_data->putString("use", "ManipulateTile");//different from other 
		kitten::EventManager::getInstance()->triggerEvent(kitten::Event::EventType::Highlight_Tile, p_data);
		*/

		//delete package
		done(p_info);

		return 0;
	}

}
