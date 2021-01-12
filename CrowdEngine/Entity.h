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

/// Utilis� pour manipuler les objets de type Entity une fois qu'ils ont �t� plac�s dans une sc�ne (GeometryScene, LightScene...).
class Manipulator : public Interface
{
	public:
	typedef SmartPtr<Manipulator, IPol> ptr;

	/// Retire le Manipulator de la sc�ne dans laquelle il se trouve
	virtual void Remove() = 0;

	/// Met � jour la sc�ne apr�s que l'objet Entity a �t� modifi� (changement de position, d'orientation...).
	virtual void Update() = 0;
};

class Entity : public Resource
{
	public:
	typedef SmartPtr<Entity, IPol> ptr;

	friend class PScene;

	virtual const AABB &GetAABB() const = 0;

	/// Fonction de mise � jour de l'objet en fontion du temps. Cette mise � jour se fait sans consid�ration
	/// de la physique et de l'intelligence artificielle. Elle permet g�n�ralement de mettre � jour un d�placement
	/// (arbres oscillant, ondes sur l'eau, fum�e qui se d�gage...).
	/// \param ms Temps qui s'est �coul� depuis la derni�re mise � jour, en millisecondes.
	/// Ne fait rien par d�faut (de nombreux objets n'ont pas besoin d'�tre mis � jour).
	virtual void Update(unsigned int ms) {}

	/// Retire l'objet de la sc�ne dans laquelle il se trouve.
	CRWD_EXPORT virtual void Remove();

	/// Met � jour la sc�ne dans laquelle l'objet se trouve.
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

	/// Fonction � appeler au d�but de AddTo(...).
	CRWD_EXPORT void SetScene(Scene *scene);

	/// Fonction appel�e par la classe Scene. Permet � l'objet d'ajouter des entit�s aux sc�nes de diff�rents types, en utilisant les fonctions du type scene->Get***Scene() et
	/// les fonctions du type ***Scene->Add(...), utilis�es pour composer la sc�ne totale. L'objet peut ajouter plusieurs entit�s � chaque sc�ne.
	/// \param scene : L'instance de la classe Scene dont la fonction Add a �t� appel�e. L'objet conserve un pointeur sur cet objet.
	virtual void AddTo(Scene *scene) = 0;

	/// Permet de retirer l'objet de la sc�ne dans laquelle il est.
	virtual void Remove() = 0;

	/// Permet de r�cup�rer la sc�ne � laquelle appartient l'objet.
	CRWD_EXPORT virtual Scene *GetScene() const;

	protected:
	String			m_name;
	Scene*			m_scene;
};

} // namespace Crowd

#endif // CRWD_ENTITY_H_INCLUDED