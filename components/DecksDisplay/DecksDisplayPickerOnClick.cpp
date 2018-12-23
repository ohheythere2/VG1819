#include "DecksDisplayPickerOnClick.h"
#include "components/DecksDisplayFrame.h"
#include "kibble/databank/databank.hpp"
#include "components/DeckInitializingComponent.h"

void DecksDisplayPickerOnClick::onClick()
{
	DecksDisplayFrame::getActiveInstance()->pickDisplayedDeck(this->m_attachedObject);
	DeckInitializingComponent::getActiveInstance()->setDeckData(kibble::getDeckDataFromId(DecksDisplayFrame::getActiveInstance()->getCurrentPickedDeckId()));
	DeckInitializingComponent::getActiveInstance()->setPlayerId(0);
}