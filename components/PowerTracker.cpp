#include "components\PowerTracker.h"
#include "kitten\K_ComponentManager.h"
#include "kitten\K_GameObjectManager.h"
#include "puppy\Text\FontTable.h"

PowerTracker::PowerTracker()
	:
	m_iMaxPower(4),
	m_iCurrentPower(4),
	m_textBox(nullptr)
{
	kitten::EventManager::getInstance()->addListener(
		kitten::Event::EventType::Manipulate_Tile,
		this,
		std::bind(&PowerTracker::increaseMaxPowerEvent, this, std::placeholders::_1, std::placeholders::_2));

	kitten::EventManager::getInstance()->addListener(
		kitten::Event::EventType::Reset_Power,
		this,
		std::bind(&PowerTracker::resetEvent, this, std::placeholders::_1, std::placeholders::_2));

}

PowerTracker::~PowerTracker()
{
	if(m_textBox != nullptr)
		kitten::K_ComponentManager::getInstance()->destroyComponentImmediate(m_textBox);
	//kitten::K_ComponentManager::getInstance()->destroyComponentImmediate(m_textBox);
	kitten::EventManager::getInstance()->removeListener(kitten::Event::EventType::Manipulate_Tile, this);
	kitten::EventManager::getInstance()->removeListener(kitten::Event::EventType::Reset_Power, this);
	m_textBox = nullptr;
}

void PowerTracker::start()
{
	kitten::K_GameObject* textBox = kitten::K_GameObjectManager::getInstance()->createNewGameObject("UI/power_tracker/power_tracker_textbox.json");
	m_textBox = textBox->getComponent<puppy::TextBox>();

	kitten::K_GameObject* powerIcon = kitten::K_GameObjectManager::getInstance()->createNewGameObject("UI/power_tracker/power_tracker_icon.json");
	updateTextBox();
}

void PowerTracker::updateTextBox()
{
	if (m_textBox != nullptr)
	{
		m_textBox->setText(std::to_string(getCurrentPower()) + "/" + std::to_string(m_iMaxPower));
	}
}

void PowerTracker::increaseMaxPower(int p_iAmount)
{
	m_iMaxPower += p_iAmount;
	updateTextBox();
}

void PowerTracker::increaseMaxPowerEvent(kitten::Event::EventType p_type, kitten::Event* p_data)
{
	int power = p_data->getInt(MANIPULATE_TILE_KEY);
	m_iMaxPower += power;
	m_iCurrentPower += power;
	updateTextBox();
}

bool PowerTracker::summonUnitCost(int p_iCost)
{
	if ((m_iCurrentPower - p_iCost) >= 0)
	{
		m_iCurrentPower -= p_iCost;
		updateTextBox();
		return true;
	}

	return false;
}

void PowerTracker::resetCurrent()
{
	m_iCurrentPower = m_iMaxPower;
	updateTextBox();
}

void PowerTracker::resetEvent(kitten::Event::EventType p_type, kitten::Event * p_data)
{
	resetCurrent();
}

int PowerTracker::getMaxPower()
{
	return m_iMaxPower;
}

int PowerTracker::getCurrentPower()
{
	return m_iCurrentPower;
}


bool PowerTracker::changeCurrentPower(int p_iAmount)
{
	if ((m_iCurrentPower + p_iAmount) >= 0)
	{
		m_iCurrentPower += p_iAmount;
		updateTextBox();
		return true;
	}

	return false;
}