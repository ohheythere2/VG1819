#include "Transform.h"

namespace kitten
{

	Transform::Transform(K_GameObject& p_owner) : m_forward(0,0,1), m_matTranslation(glm::translate(0,0,0)), m_matScale(glm::scale(1,1,1)), m_translation(0,0,0), m_scale(1,1,1),
		m_derivedTranslation(0,0,0), m_derivedScale(1,1,1), m_parent(nullptr), m_ignoresParent(true), m_attachedObject(p_owner)
	{

	}

	Transform::~Transform()
	{
		if (!m_children.empty())
		{
			auto end = m_children.end();
			for (auto it = m_children.begin(); it != end; ++it)
			{
				(*it)->setParent(nullptr);
			}
		}

		if (m_parent != nullptr)
		{
			m_parent->removeChild(this);
		}
	}

	K_GameObject& Transform::getAttachedGameObject()
	{
		return m_attachedObject;
	}

	const glm::mat4& Transform::getWorldTransform()
	{
		if (m_isDirty)
		{
			m_matWorldNoScale = m_matTranslation * glm::mat4_cast(m_quatRotation);
			m_matWorld = m_matWorldNoScale * m_matScale;
			
			if (!m_ignoresParent && m_parent != nullptr)
			{
				m_matWorldNoScale = m_parent->getWorldTransformNoScale() * m_matWorldNoScale;
				m_matWorld = m_matWorldNoScale * glm::scale((m_scale * m_parent->getScale()));
			}
			m_isDirty = false;
		}

		return m_matWorld;
	}

	const glm::mat4& Transform::getWorldTransformNoScale()
	{
		if (m_isDirty)
		{
			m_matWorldNoScale = m_matTranslation * glm::mat4_cast(m_quatRotation);
			m_matWorld = m_matWorldNoScale * m_matScale;
			
			if (!m_ignoresParent && m_parent != nullptr)
			{
				m_matWorldNoScale = m_parent->getWorldTransformNoScale() * m_matWorldNoScale;
				m_matWorld = m_matWorldNoScale * glm::scale((m_scale * m_parent->getScale()));
			}
			m_isDirty = false;
		}

		return m_matWorldNoScale;
	}

	void Transform::move2D(const float xUnits, const float yUnits)
	{
		m_translation[0] += xUnits;
		m_translation[1] += yUnits;
		m_matTranslation = glm::translate(m_translation);
		m_isDirty = true;

		if (!m_ignoresParent && m_parent != nullptr)
		{
			m_derivedTranslation = m_parent->getTranslation() + (m_parent->getRotation() * m_translation);
		}

		setChildrenDirty(position);
		notifyPositionListeners();
	}

	void Transform::move(const float xUnits, const float yUnits, const float zUnits)
	{
		m_translation[0] += xUnits;
		m_translation[1] += yUnits;
		m_translation[2] += zUnits;
		m_matTranslation = glm::translate(m_translation);
		m_isDirty = true;

		if (!m_ignoresParent && m_parent != nullptr)
		{
			m_derivedTranslation = m_parent->getTranslation() + (m_parent->getRotation() * m_translation);
		}

		setChildrenDirty(position);
		notifyPositionListeners();
	}

	void Transform::place2D(const float x, const float y)
	{
		m_translation[0] = x;
		m_translation[1] = y;
		m_matTranslation = glm::translate(x, y, 0.0f);
		m_isDirty = true;

		if (!m_ignoresParent && m_parent != nullptr)
		{
			m_derivedTranslation = m_parent->getTranslation() + (m_parent->getRotation() * m_translation);
		}

		setChildrenDirty(position);
		notifyPositionListeners();
	}

	void Transform::place(const float x, const float y, const float z)
	{
		m_translation[0] = x;
		m_translation[1] = y;
		m_translation[2] = z;
		m_matTranslation = glm::translate(x, y, z);
		m_isDirty = true;

		if (!m_ignoresParent && m_parent != nullptr)
		{
			m_derivedTranslation = m_parent->getTranslation() + (m_parent->getRotation() * m_translation);
		}

		setChildrenDirty(position);
		notifyPositionListeners();
	}

	void Transform::scale2D(const float xScale, const float yScale)
	{
		m_scaleAsFloat = glm::vec2(xScale, yScale);
		m_matScale = glm::scale(xScale, yScale, 1.0f);
		m_isDirty = true;

		if (!m_ignoresParent && m_parent != nullptr)
		{
			m_derivedScale = m_scale * m_parent->getScale();
		}

		setChildrenDirty(scale);
		notifyScaleListeners();
	}

	void Transform::scaleAbsolute(const float xScale, const float yScale, const float zScale)
	{
		m_scale = glm::vec3(xScale, yScale, zScale);
		m_matScale = glm::scale(m_scale);
		m_isDirty = true;

		if (!m_ignoresParent && m_parent != nullptr)
		{
			m_derivedScale = m_scale * m_parent->getScale();
		}

		setChildrenDirty(scale);
		notifyScaleListeners();
	}

	void Transform::scaleRelative(const float xScale, const float yScale, const float zScale)
	{
		m_scale += glm::vec3(xScale, yScale, zScale);
		m_matScale = glm::scale(m_scale);
		m_isDirty = true;

		if (!m_ignoresParent && m_parent != nullptr)
		{
			m_derivedScale = m_scale * m_parent->getScale();
		}

		setChildrenDirty(scale);
		notifyScaleListeners();
	}

	//NOT FUNCTIONAL ANYMORE!!
	void Transform::rotate2D(const float deg)
	{
		m_rotateDeg += deg;
		m_matRotation = glm::rotate(m_rotateDeg, glm::vec3(0.0f, 0.0f, 1.0f));
		m_isDirty = true;
	}

	void Transform::rotateRelative(const glm::vec3& rot)
	{
		m_quatRotation = glm::quat(rot * (float)DEG_TO_RAD_FACTOR) * m_quatRotation;
		m_isDirty = true;

		if (!m_ignoresParent && m_parent != nullptr)
		{
			m_derivedQuatRotation = m_quatRotation * m_parent->getRotation();
			m_forward = glm::vec3(0, 0, 1) * m_derivedQuatRotation;
			m_upVector = glm::vec3(0, 1, 0) * m_derivedQuatRotation;
		}
		else
		{
			m_forward = glm::vec3(0, 0, 1) * m_quatRotation;
			m_upVector = glm::vec3(0, 1, 0) * m_quatRotation;
		}

		setChildrenDirty(rotation);
		notifyRotationListeners();
	}

	void Transform::rotateAbsolute(const glm::vec3& rot)
	{
		m_quatRotation = glm::quat(rot * (float)DEG_TO_RAD_FACTOR);
		m_isDirty = true;

		if (!m_ignoresParent && m_parent != nullptr)
		{
			m_derivedQuatRotation = m_quatRotation * m_parent->getRotation();
			m_forward = glm::vec3(0, 0, 1) * m_derivedQuatRotation;
			m_upVector = glm::vec3(0, 1, 0) * m_derivedQuatRotation;
		}
		else
		{
			m_forward = glm::vec3(0, 0, 1) * m_quatRotation;
			m_upVector = glm::vec3(0, 1, 0) * m_quatRotation;
		}

		setChildrenDirty(rotation);
		notifyRotationListeners();
	}

	const glm::vec3& Transform::getTranslation() const
	{
		if (m_ignoresParent)
		{
			return m_translation;
		}
		else
		{
			return m_derivedTranslation;
		}
	}

	const glm::quat& Transform::getRotation() const
	{
		if (m_ignoresParent)
		{
			return m_quatRotation;
		}
		else
		{
			return m_derivedQuatRotation;
		}
	}

	const glm::vec3& Transform::getForward() const
	{
		return m_forward;
	}

	const glm::vec3& Transform::getUpVector() const
	{
		return m_upVector;
	}

	const glm::vec3& Transform::getScale() const
	{
		if (m_ignoresParent)
		{
			return m_scale;
		}
		else
		{
			return m_derivedScale;
		}
		
	}

	const glm::vec2& Transform::getScale2D() const
	{
		return m_scaleAsFloat;
	}

	bool Transform::getIgnoreParent() const
	{
		return m_ignoresParent;
	}

	void Transform::setIgnoreParent(bool p_ignores)
	{
		m_ignoresParent = p_ignores;
		if (!p_ignores)
		{
			m_isDirty = true;
			setChildrenDirty(unknown);
			//Not sure how transform attributes will change, notify
			notifyPositionListeners();
			notifyRotationListeners();
			notifyScaleListeners();
		}
	}

	void Transform::setParent(Transform* p_parent)
	{
		if (m_parent != nullptr)
		{
			m_parent->removeChild(this);
		}

		m_parent = p_parent;

		if (p_parent != nullptr)
		{
			p_parent->addChild(this);
		}

		if (!m_ignoresParent)
		{
			m_isDirty = true;
			setChildrenDirty(unknown);
			//Not sure how transform attributes will change, notify
			notifyPositionListeners();
			notifyRotationListeners();
			notifyScaleListeners();
		}
	}

	Transform* Transform::getParent()
	{
		return m_parent;
	}

	//Private method called when the child's parent is assigned to this
	void Transform::addChild(Transform* p_child)
	{
		m_children.push_back(p_child);
	}

	bool Transform::removeChild(const Transform* p_child)
	{
		auto end = m_children.end();
		for (auto it = m_children.begin(); it != end; ++it)
		{
			if (p_child == *it)
			{
				m_children.erase(it);
				return true;
			}
		}

		return false;
	}

	const std::vector<Transform*>& Transform::getChildren() const
	{
		return m_children;
	}

	void Transform::addPositionListener(TransformPositionListener* p_toAdd)
	{
		m_positionListeners.push_back(p_toAdd);
	}

	void Transform::addScaleListener(TransformScaleListener* p_toAdd)
	{
		m_scaleListeners.push_back(p_toAdd);
	}

	void Transform::addRotationListener(TransformRotationListener* p_toAdd)
	{
		m_rotationListeners.push_back(p_toAdd);
	}

	void Transform::removePositionListener(TransformPositionListener* p_toRemove)
	{
		auto end = m_positionListeners.cend();
		for (auto it = m_positionListeners.begin(); it != end; ++it)
		{
			if (*it == p_toRemove)
			{
				m_positionListeners.erase(it);
				return;
			}
		}
	}

	void Transform::removeScaleListener(TransformScaleListener* p_toRemove)
	{
		auto end = m_scaleListeners.cend();
		for (auto it = m_scaleListeners.begin(); it != end; ++it)
		{
			if (*it == p_toRemove)
			{
				m_scaleListeners.erase(it);
				return;
			}
		}
	}

	void Transform::removeRotationListener(TransformRotationListener* p_toRemove)
	{
		auto end = m_rotationListeners.cend();
		for (auto it = m_rotationListeners.begin(); it != end; ++it)
		{
			if (*it == p_toRemove)
			{
				m_rotationListeners.erase(it);
				return;
			}
		}
	}

	void Transform::notifyScaleListeners()
	{
		const glm::vec3& scale = getScale();
		auto end = m_scaleListeners.cend();
		for (auto it = m_scaleListeners.begin(); it != end; ++it)
		{
			(*it)->onScaleChanged(scale);
		}
	}

	void Transform::notifyPositionListeners()
	{
		const glm::vec3& pos = getTranslation();
		auto end = m_positionListeners.cend();
		for (auto it = m_positionListeners.begin(); it != end; ++it)
		{
			(*it)->onPosChanged(pos);
		}
	}

	void Transform::notifyRotationListeners()
	{
		const glm::quat& rot = getRotation();
		auto end = m_rotationListeners.cend();
		for (auto it = m_rotationListeners.begin(); it != end; ++it)
		{
			(*it)->onRotationChanged(rot);
		}
	}

	void Transform::setChildrenDirty(ParentDirtyType p_type)
	{
		auto end = m_children.cend();
		for (auto it = m_children.cbegin(); it != end; ++it)
		{
			if (!(*it)->m_ignoresParent)
			{
				(*it)->onParentDirty(p_type);
			}	
		}
	}

	void Transform::onParentDirty(ParentDirtyType p_type)
	{
		m_isDirty = true;

		setChildrenDirty(p_type);

		switch (p_type)
		{
		case position:
			m_derivedTranslation = m_parent->getTranslation() + (m_parent->getRotation() * m_translation);

			notifyPositionListeners();
			break;
		case rotation:
			m_derivedQuatRotation = m_quatRotation * m_parent->getRotation();
			m_forward = glm::vec3(0, 0, 1) * m_derivedQuatRotation;
			m_upVector = glm::vec3(0, 1, 0) * m_derivedQuatRotation;

			notifyRotationListeners();
			break;
		case scale:
			m_derivedScale = m_scale * m_parent->getScale();

			notifyScaleListeners();
			break;
		case unknown:
			m_derivedTranslation = m_parent->getTranslation() + (m_parent->getRotation() * m_translation);
			m_derivedQuatRotation = m_quatRotation * m_parent->getRotation();
			m_forward = glm::vec3(0, 0, 1) * m_derivedQuatRotation;
			m_upVector = glm::vec3(0, 1, 0) * m_derivedQuatRotation;
			m_derivedScale = m_scale * m_parent->getScale();

			notifyPositionListeners();
			notifyRotationListeners();
			notifyScaleListeners();
			break;
		default:
			assert(false);
		}
	}
}