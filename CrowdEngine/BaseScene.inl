#ifndef CRWD_BASE_SCENE_INL_INCLUDED
#define CRWD_BASE_SCENE_INL_INCLUDED

namespace Crowd
{

//
//	BaseScene<T, PTR>::Manipulator
//
template<class T, class PTR>
BaseScene<T, PTR>::Manipulator::Manipulator(const typename BaseScene<T, PTR>::BOctree::Iterator &it) : m_iterator(it)
{

}

template<class T, class PTR>
void BaseScene<T, PTR>::Manipulator::Remove()
{
	m_iterator.Remove();
	m_iterator = BaseScene<T, PTR>::BOctree::Iterator();
}

template<class T, class PTR>
void BaseScene<T, PTR>::Manipulator::Update()
{
	m_iterator.UpadtePositionInTree();
}

template<class T, class PTR>
typename BaseScene<T, PTR>::Manipulator &BaseScene<T, PTR>::Manipulator::operator = (const typename BaseScene<T, PTR>::BOctree::Iterator &it)
{
	m_iterator = it;
	return *this;
}


//
//	BaseScene<T, PTR>
//
template<class T, class PTR>
BaseScene<T, PTR>::BaseScene(const AABB &box) : m_octree(box)
{

}

template<class T, class PTR>
BaseScene<T, PTR>::~BaseScene()
{

}

template<class T, class PTR>
void BaseScene<T, PTR>::Add(T *entity)
{
	if(entity)
	{
		entity->Remove();
		PTR pEntity = (PTR) entity;
		pEntity->AddRef();
		BaseScene<T, PTR>::BOctree::Iterator it = m_octree.Add(pEntity);
		Manipulator::ptr manip = new Manipulator(it);
		//manip->Release();
		SetManipulatorTo(entity, (Crowd::Manipulator::ptr&) manip);
	}
}

} // namsepace Crowd

#endif // CRWD_BASE_SCENE_INL_INCLUDED