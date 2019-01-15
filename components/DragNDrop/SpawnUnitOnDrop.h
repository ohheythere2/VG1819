#include "UI/DragNDrop.h"
#include "UI\CardUIO.h"
#include "_Project/LerpController.h"

class SpawnUnitOnDrop : public DragNDrop, public kitten::TransformPositionListener
{
private:
	bool m_isHovered = false; 
	userinterface::CardUIO* m_cardUIO;
public:
	SpawnUnitOnDrop();
	~SpawnUnitOnDrop();

	void start() override;
	void onClick() override;
	void onHoverStart() override;
	void onHoverEnd() override;
	void onDrop() override;
	void onPause() override;
	void onPosChanged(const glm::vec3& p_newPos) override;
};