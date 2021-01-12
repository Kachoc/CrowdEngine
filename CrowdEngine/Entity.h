#ifndef CRWD_ENTITY_H_INCLUDED
#define CRWD_ENTITY_H_INCLUDED

#include "Resources.h"
#include "CRWDMath.h"
#include "D3DEntry.h"
#include "Camera.h"

namespace Crowd
{

class RenderScene;
class PhysicsScene;
class AIScene;
class UpdateScene;
class Scene;

struct RenderInfo;

/// Utilisé pour manipuler les objets de type Entity une fois qu'ils ont été placés dans une scène (GeometryScene, LightScene...).
class Manipulator : public Interface
{
	public:
	typedef SmartPtr<Manipulator, IPol> ptr;

	/// Retire le Manipulator de la scène dans laquelle il se trouve
	virtual void Remove() = 0;

	/// Met à jour la scène après que l'objet Entity a été modifié (changement de position, d'orientation...).
	virtual void Update() = 0;
};

class Entity : public Resource
{
	public:
	typedef SmartPtr<Entity, IPol> ptr;

	friend class PScene;

	virtual const AABB &GetAABB() const = 0;

	/// Fonction de mise à jour de l'objet en fontion du temps. Cette mise à jour se fait sans considération
	/// de la physique et de l'intelligence artificielle. Elle permet généralement de mettre à jour un déplacement
	/// (arbres oscillant, ondes sur l'eau, fumée qui se dégage...).
	/// \param ms Temps qui s'est écoulé depuis la dernière mise à jour, en millisecondes.
	/// Ne fait rien par défaut (de nombreux objets n'ont pas besoin d'être mis à jour).
	virtual void Update(unsigned int ms) {}

	/// Retire l'objet de la scène dans laquelle il se trouve.
	CRWD_EXPORT virtual void Remove();

	/// Met à jour la scène dans laquelle l'objet se trouve.
	CRWD_EXPORT virtual void UpdateInScene();

	protected:
	Entity() {}
	virtual ~Entity() {}

	protected:
	Manipulator::ptr	m_manipulator;
};

class REntity : public Entity // Render entity
{
	public:
	typedef SmartPtr<REntity, IPol> ptr;

	virtual void Draw(const RenderInfo &info, const Camera &camera) = 0;
	virtual const AABB &GetAABB() const = 0;
	
	protected:
	REntity() {}
	virtual ~REntity() {}
};

class Object : public Interface
{
	public:
	typedef SmartPtr<Object, IPol> ptr;

	friend class Scene;

	CRWD_EXPORT virtual void SetName(const String &name);
	CRWD_EXPORT virtual const char *GetName() const;

	protected:
	CRWD_EXPORT Object();
	CRWD_EXPORT Object(const String &name);
	CRWD_EXPORT virtual ~Object();

	/// Fonction à appeler au début de AddTo(...).
	CRWD_EXPORT void SetScene(Scene *scene);

	/// Fonction appelée par la classe Scene. Permet à l'objet d'ajouter des entités aux scènes de différents types, en utilisant les fonctions du type scene->Get***Scene() et
	/// les fonctions du type ***Scene->Add(...), utilisées pour composer la scène totale. L'objet peut ajouter plusieurs entités à chaque scène.
	/// \param scene : L'instance de la classe Scene dont la fonction Add a été appelée. L'objet conserve un pointeur sur cet objet.
	virtual void AddTo(Scene *scene) = 0;

	/// Permet de retirer l'objet de la scène dans laquelle il est.
	virtual void Remove() = 0;

	/// Permet de récupérer la scène à laquelle appartient l'objet.
	CRWD_EXPORT virtual Scene *GetScene() const;

	protected:
	String			m_name;
	Scene*			m_scene;
};

} // namespace Crowd

#endif // CRWD_ENTITY_H_INCLUDED