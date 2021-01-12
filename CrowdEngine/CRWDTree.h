#ifndef CRWDTREE_H_INCLUDED
#define CRWDTREE_H_INCLUDED

#include <iostream>
#include <list>

#include "CRWDMath.h"
#include "SmartPtr.h"

namespace Crowd
{

//
//	Classe Tree
//
/// Structure de données.
/// Permet par exemple de générer des objets du type Octree.
/// \param T La classe des objets contenus par l'arbre. Cette classe doit contenir une fonction de type: AABB GetAABB().
/// \param MAX_OBJECTS Le nombre d'objets contenu par chaque noeud de l'arbre avant expansion de ce noeud.
/// \param PTR Le type de pointeur à utiliser pour manipuler les instances de T. Permet par exemple d'utiliser des pointeurs intelligents.
/// \param X_DIV Le nombre de divisions en x de la boîte de type AABB à effectuer lors de l'expansion d'un noeud.
/// \param Y_DIV Le nombre de divisions en y de la boîte de type AABB à effectuer lors de l'expansion d'un noeud.
/// \param Z_DIV Le nombre de divisions en z de la boîte de type AABB à effectuer lors de l'expansion d'un noeud.
/// Les paramètres templates X_DIV, Y_DIV et Z_DIV permettent la création d'un octree si laissés à 1.
template<class T, unsigned int MAX_OBJECTS, class PTR = T::ptr, unsigned int X_DIV = 1, unsigned int Y_DIV = 1, unsigned int Z_DIV = 1>
class Tree
{
	public:
	struct Node;
	struct Iterator;

	typedef PTR ObjectPtr;

	Tree(const AABB &box);
	~Tree();

	Iterator	Add(PTR &object);
	//void		Remove(Iterator &object);

	//Iterator	FirstObject();
	//Iterator	LastObject();

	/// Déplace un objet représenté par un itérateur si la bounding box de cet objet a changé.
	/// Il est possible que l'objet ne soit plus contenu par l'arbre.
	Iterator	Move(Iterator &object);

	Node		*ParentNode() const;
	unsigned int GetNumObjects() const;

	void		Write(std::ostream &stream);

	protected:
	struct Node
	{
		//friend struct Tree::Iterator;
		//friend struct Tree::UpIterator;
		//friend struct Tree::DownIterator;
		//template<class Test> friend struct Tree::BoolIterator;
		//friend struct Tree::BoxIterator;

		struct Element;

		public:
		Node();
		Node(Node *parent);
		~Node();

		void Init(Tree *tree, Node *parent, const AABB &box, unsigned int index, unsigned int nodeLevel, unsigned int maxLevelInBranch);

		bool				Contains(const AABB &box) const;

		Iterator			Add(PTR &object);
		static void			Remove(Iterator &it);
		static Iterator		Move(Iterator &it);

		void				Add(typename Element::ptr &object);
		void				Remove(typename Element::ptr object);

		Node				*SeekContainingChild(const AABB &box) const;

		bool				Expand();
		bool				Retract();

		void				Write(std::ostream &stream);

		public:
		void				GetNumObjects(unsigned int &num, unsigned int max = -1); // nombre d'objets contenus par le node et ses enfants, on arrête de compter si ce nombre dépasse max
		void				MoveObjectsTo(Node *dest);

		struct Element
		{
			friend struct Tree::Iterator;

			public:
			typedef Element*	ptr;

			Element(ObjectPtr object, Tree *parentTree);
			~Element();

			const AABB &GetAABB() const;

			public:
			ObjectPtr						m_object;

			Tree							*m_parentTree;
			Node							*m_parent;
			typename Element::ptr			m_previous;
			typename Element::ptr			m_next;
		};

		// Attributs
		public:
		Tree							*m_tree;
		Node							*m_parent;
		Node							*m_children;
		AABB							m_box;

		typename Element::ptr			m_firstObject;
		typename Element::ptr			m_lastObject;
		unsigned int					m_numObjects;

		unsigned int					m_index;
		unsigned int					m_nodeLevel;
		unsigned int					m_maxLevelInBranch;
	};

	public:
	struct Iterator
	{
		public:
		friend class Tree;

		Iterator(typename Node::Element::ptr object = NULL);

		Iterator &operator = (typename Node::Element *);

		bool operator ! () const;
		operator bool () const;

		T &operator * ();
		operator T* ();

		operator bool ();

		void UpadtePositionInTree(); // Met à jour l'Octree si la position de l'objet référencé par l'itérateur a changé.
		void Remove();

		protected:
		typename Node::Element::ptr m_ptr;
	};

	struct UpIterator : public Iterator
	{
		friend class Tree;

		UpIterator(const Iterator &it);
		UpIterator &operator = (const Iterator &it);
		void GoToFirstObject(); // L'itérateur pointe vers le premier objet du node
		void GoToNextNode();

		UpIterator &operator ++ ();

		bool operator ! () const;
		operator bool () const;

		T &operator * ();
		operator T* ();

		protected:
		typename Node::Element::ptr m_ptr;
	};

	struct DownIterator : public Iterator
	{
		friend class Tree;

		DownIterator(const Iterator &it);
		DownIterator &operator = (const Iterator &it);
		void GoToLastObject(); // L'itérateur pointe vers le premier objet du node
		void GoToNextNode();

		DownIterator &operator ++ ();

		bool operator ! () const;
		operator bool () const;

		bool operator != (unsigned int ptr) const;
		bool operator == (unsigned int ptr) const;

		T &operator * ();
		operator T* ();

		protected:
		unsigned int					m_initialNodeLevel;
		typename Node::Element::ptr		m_ptr;
	};

	template<class Test>
	struct BoolIterator : public Iterator
	{
		friend class Tree;

		BoolIterator(Test test);
		BoolIterator(Test test, const Tree &tree);
		BoolIterator(Test test, const Iterator &it);
		Node *GoToNextNode(Node *node);

		void GoToBeginning(const Tree &tree);

		BoolIterator &operator ++ ();

		bool operator ! () const;
		operator bool () const;

		bool operator != (unsigned int ptr) const;
		bool operator == (unsigned int ptr) const;

		T &operator * ();
		operator T* ();

		protected:
		Test							m_test;
		typename Node::Element::ptr		m_ptr;
	};

	struct BoxIterator : public Iterator
	{
		friend class Tree;

		BoxIterator(const Iterator &it);

		BoxIterator &operator ++ ();

		BoxIterator &operator = (const Iterator &it);

		operator T* ();

		protected:
		typename Node::Element::ptr m_ptr;
	};


	protected:
	friend struct Node::Element;

	Node				*m_parentNode;
	unsigned int		m_numObjects;
	unsigned int		m_numNodes;
	unsigned int		m_maxLevelNodes;
};

} // namespace Crowd

#include "CRWDTree.inl"

#endif // CRWDTREE_H_INCLUDED