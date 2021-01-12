#include "BaseScene.h"

namespace Crowd
{

void PScene::SetManipulatorTo(Entity *entity, Manipulator::ptr manipulator)
{
	entity->m_manipulator = manipulator;
}

} // namespace Crowd