#pragma once
#include "ability/ability/Ability.h"
#include "unit/Unit.h"

//Rock

namespace ability
{
	int Curse::effect(AbilityInfoPackage* p_info)
	{
		if (checkTarget(p_info))
		{
			//check if unit has this status
			if (!p_info->m_targets[0]->getStatusContainer()->getStatus(STATUS_CURSE, ABILITY_CURSE))
			{
				//apply Status_Encourage to target
				ability::Status* se = ability::StatusManager::getInstance()->findStatus(STATUS_CURSE);

				//set properties
				addStatusInfo(se, p_info);

				int pow = p_info->m_intValue.find(UNIT_POWER)->second;

				se->addCounter(UNIT_POWER, pow);
				se->m_source = ABILITY_CURSE;

				//attach to target
				se->attach(p_info->m_targets[0]);
			}
		}

		//delete package
		done(p_info);

		return 0;
	}

}