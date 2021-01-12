#ifndef CRWD_BASE_SCENE_H_INCLUDED
#define CRWD_BASE_SCENE_H_INCLUDED

#include "Entity.h"

namespace Crowd
{

/// BaseScene
class PScene : public Interface
{
	public:

	protected:
	CRWD_EXPORT virtual void SetManipulatorTo(Entity *entity, Manipulator::ptr manipulator);
};


/// Base Scene
template<class T, class PTR = T::ptr>
class BaseScene : public PScene
{
	public:
	typedef SmartPtr<BaseScene, IPol> ptr;
	typedef Tree<T, 16, PTR> BOctree;

	BaseScene(const AABB &box);
	virtual ~BaseScene();

	/// Ajoute un objet à la scène.
	virtual void Add(T *entity);

	protected:
	/// L'objet Manipulator référence un objet de la GeometryScene.
	class Manipulator : public Crowd::Manipulator
	{
		public:
		typedef SmartPtr<Manipulator, IPol> ptr;

		Manipulator(const typename BOctree::Iterator &it = BOctree::Iterator());

		/// Réarrange l'octree contenu dans GeometryScene.
		/// A appeler après toute modification de l'AABB contenue dans l'objet REntity référencé, et permettant son positionnement dans l'arbre.
		virtual void Update();

		/// Retire l'objet de l'octree.
		virtual void Remove();

		Manipulator &operator = (const typename BOctree::Iterator &it);

		protected:
		typename BOctree::Iterator m_iterator;
	};

	protected:
	BOctree m_octree;
};

} // namespace Crowd

#include "BaseScene.inl"

#endif // CRWD_BASE_SCENE_H_INCLUDED