#include "StartGameOnClick.h"
#include "kitten/K_Instance.h"
#include "kitten/K_GameObjectManager.h"
#include "components/DeckInitializingComponent.h"
#include "components/DecksDisplayFrame.h"
#include "kibble/databank/databank.hpp"

void StartGameOnClick::onClick() 
{
	if (DeckInitializingComponent::getActiveInstance() == nullptr) return;
	if (DeckInitializingComponent::getActiveInstance()->getDeckData() == nullptr) return;
	kitten::K_Instance::changeScene(m_targetScene);
}