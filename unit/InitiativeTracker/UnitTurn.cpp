#include "UnitTurn.h"
#include "unit/InitiativeTracker/InitiativeTracker.h"

unit::UnitTurn::UnitTurn()
{
	act = false;
	move = false;
}

unit::UnitTurn::~UnitTurn()
{
}

void unit::UnitTurn::turnStart(kitten::K_GameObject* p_unitObj)
{
	m_currentUnit = p_unitObj->getComponent<unit::Unit>();
	m_currentUnit->setTurn(this);

	//TO DO:send turn start event

	//TO DO:check CT

	//if the unit's movement is greater than 0, then it can move this turn
	
	int mv = m_currentUnit->m_attributes["mv"];
	if (mv > 0)
		move = true;

	act = true;
}

void unit::UnitTurn::checkTurn()
{
	//if unit can not move or use ability, its turn automatically end
	if (!act && !move)
	{
		turnEnd();
	}
}

void unit::UnitTurn::turnEnd()
{
	m_currentUnit->setTurn(nullptr);
	m_currentUnit = nullptr;

	//TO DO:send turn end event

	//TO DO:call initiative tracker
	InitiativeTracker::getInstance()->unitTurnEnd();
}