namespace Crowd
{

//
//	Tree
//
template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Tree(const AABB &box)
{
	m_parentNode = new Node;
	m_parentNode->Init(this, NULL, box, 0, 0, 0);
	m_numObjects = 0;
	m_numNodes = 1;
	m_maxLevelNodes = 0;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::~Tree()
{
	delete m_parentNode;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator	Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Add(PTR &object)
{
	Iterator &it = m_parentNode->Add(object);
	if(it)
	{
		m_numObjects++;
	}
	return it;
}

/*template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Remove(Iterator &object)
{
	Node::Remove(object);
	m_numObjects--;
}*/

/*template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator <T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::FirstObject()
{

}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator <T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::LastObject()
{

}*/

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Move(Iterator &object)
{
	Iterator &it = Node::Move(object);
	if(!it) {
		m_numObjects--;
	}
	return it;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node *Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::ParentNode() const
{
	return m_parentNode;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
unsigned int Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::GetNumObjects() const
{
	return m_numObjects;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Write(std::ostream &stream)
{
	stream << "Depth: " << m_maxLevelNodes << std::endl;
	stream << "Number of nodes: " << m_numNodes << std::endl;
	stream << "Number of objects: " << m_numObjects << std::endl;
	m_parentNode->Write(stream);
}


//
// Tree::Node
//
template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Node()
{
	m_parent = NULL;
	m_children = NULL;

	m_firstObject = NULL;
	m_lastObject = NULL;
	m_numObjects = 0;

	m_index = 0;
	m_nodeLevel = 0;
	m_maxLevelInBranch = 0;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Node(Node *parent)
{
	m_parent = parent;
	m_children = NULL;

	m_firstObject = NULL;
	m_lastObject = NULL;
	m_numObjects = 0;

	m_index = 0;
	m_nodeLevel = 0;
	m_maxLevelInBranch = 0;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::~Node()
{
	if(m_children)
	{
		delete[] m_children;
		m_children = NULL;
	}

	while(m_firstObject)
	{
		Element::ptr presentObject = m_firstObject;
		Remove(m_firstObject);
		delete presentObject;
	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Init(Tree *tree, Node *parent, const AABB &box, unsigned int index, unsigned int nodeLevel, unsigned int maxLevelInBranch)
{
	m_tree = tree;
	m_parent = parent;
	m_box = box;
	m_index = index;
	m_nodeLevel = nodeLevel;
	m_maxLevelInBranch = maxLevelInBranch;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
bool Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Contains(const AABB &box) const
{
	return m_box.Contains(box);
}

//
//	Fonctions Add et Remove
//
template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Add(PTR &object)
{
	Element::ptr nElement = new Element(object, m_tree);
	if(Contains(nElement->GetAABB()))
	{
		Node *node = this;
		if(node->m_numObjects >= MAX_OBJECTS && node->m_children == NULL) node->Expand();

		Node *child = node->SeekContainingChild(nElement->GetAABB());
		while(child != NULL)
		{
			if(child->m_numObjects >= MAX_OBJECTS && child->m_children == NULL) {
				child->Expand();
			}
			node = child;
			child = child->SeekContainingChild(nElement->GetAABB());
		}

		node->Add(nElement);
		return Iterator(nElement);
	}
	else
	{
		delete nElement;
		return Iterator(NULL);
	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Remove(Iterator &it)
{
	Node *parent = it.m_ptr->m_parent;
	parent->Remove(it.m_ptr);

	delete it.m_ptr;
	it.m_ptr = NULL;
	
	if(parent->m_numObjects == 0 && parent->m_nodeLevel >= parent->m_maxLevelInBranch - 3)
	{
		if(parent->m_children)
		{
			bool retract = parent->Retract();
			parent = parent->m_parent;
			while(retract == true && parent != NULL)
			{
				retract = parent->Retract();
				parent = parent->m_parent;
			}
		}
		else
		{
			parent = parent->m_parent;
			bool retract = true;
			while(retract == true && parent != NULL)
			{
				retract = parent->Retract();
				parent = parent->m_parent;
			}
		}
	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Move(typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator &it)
{
	Node *parent = it.m_ptr->m_parent;
	T *object = it.m_ptr->m_object; // On n'utilise pas un pointeur intelligent car cela n'est pas nécessaire et est plus rapide

	if(parent->Contains(object->GetAABB())) {
		Node *node = parent;
		Node *child = node->SeekContainingChild(object->GetAABB());
		if(!child) {
			return it;
		}

		while(child != NULL)
		{
			if(child->m_numObjects >= MAX_OBJECTS && child->m_children == NULL) {
				child->Expand();
			}
			node = child;
			child = child->SeekContainingChild(object->GetAABB());
		}

		parent->Remove(it.m_ptr);
		node->Add(it.m_ptr);
		return it;
	}
	else
	{
		parent->Remove(it.m_ptr);
		Node *node = parent->m_parent;
		if(node)
		{
			if(parent->m_numObjects == 0 && parent->m_nodeLevel >= parent->m_maxLevelInBranch - 3)
			{
				if(parent->m_children)
				{
					bool retract = parent->Retract();
					parent = parent->m_parent;
					while(retract == true && parent != NULL)
					{
						retract = parent->Retract();
						node = parent;
						parent = parent->m_parent;
					}
				}
				else
				{
					parent = parent->m_parent;
					bool retract = true;
					while(retract == true && parent != NULL)
					{
						retract = parent->Retract();
						node = parent;
						parent = parent->m_parent;
					}
				}
			}
		}

		while(node != NULL && !node->Contains(object->GetAABB()))
		{
			node = node->m_parent;
		}

		if(node)
		{
			Node *child = node->SeekContainingChild(object->GetAABB());
			while(child != NULL)
			{
				if(child->m_numObjects >= MAX_OBJECTS && child->m_children == NULL) {
					child->Expand();
				}
				node = child;
				child = child->SeekContainingChild(object->GetAABB());
			}

			node->Add(it.m_ptr);
			return it;
		}
		else
		{
			delete it.m_ptr;
			it.m_ptr = NULL;
			return it;
		}
	}
}

//
//	Fonctions Add et Remove spécifiques à un Node particulier
//
template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Add(typename Element::ptr &object)
{
	if(object->m_parent)
	{
		Remove(object);
	}
	
	object->m_parent = this;
	m_numObjects++;

	if(m_lastObject)
	{
		m_lastObject->m_next = object;
		object->m_previous = m_lastObject;
		m_lastObject = object;
	}
	else
	{
		m_firstObject = object;
		m_lastObject = object;
	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Remove(typename Element::ptr object)
{
	if(object->m_previous)
	{
		object->m_previous->m_next = object->m_next;
	}
	else if(object->m_parent)
	{
		object->m_parent->m_firstObject = object->m_next;
	}

	if(object->m_next)
	{
		object->m_next->m_previous = object->m_previous;
	}
	else if(object->m_parent)
	{
		object->m_parent->m_lastObject = object->m_previous;
	}

	object->m_previous = NULL;
	object->m_next = NULL;
	if(object->m_parent)
	{
		object->m_parent->m_numObjects--;
		object->m_parent = NULL;
	}

	if(object->m_parentTree)
	{
		object->m_parentTree->m_numObjects--;
		object->m_parentTree = NULL;
	}
}

/*template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node *Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::SeekContainingChild(const AABB &box) const
{
	if(!m_children) {
		return NULL;
	}

	// On calcule les dimensions des boîtes enfants (sections)
	float divWidth = (m_box.MaxPoint.x - m_box.MinPoint.x)/(float)(X_DIV+1);
	float divHeight = (m_box.MaxPoint.y - m_box.MinPoint.y)/(float)(Y_DIV+1);
	float divDepth = (m_box.MaxPoint.z - m_box.MinPoint.z)/(float)(Z_DIV+1);

	// On cherche à quelle section appartient les points min et max du cube
	Vector3I MinPoint(  (int) ((box.MinPoint.x-m_box.MinPoint.x)/divWidth),
						(int) ((box.MinPoint.y-m_box.MinPoint.y)/divWidth),
						(int) ((box.MinPoint.z-m_box.MinPoint.z)/divWidth) );

	Vector3I MaxPoint(  (int) ((box.MaxPoint.x-m_box.MinPoint.x)/divWidth),
						(int) ((box.MaxPoint.y-m_box.MinPoint.y)/divWidth),
						(int) ((box.MaxPoint.z-m_box.MinPoint.z)/divWidth) );

	if(MinPoint == MaxPoint) {
		return &m_children[ MinPoint.x*(Y_DIV+1)*(Z_DIV+1) + MinPoint.y*(Z_DIV+1) + MinPoint.z ];
	}
	else {
		return NULL;
	}
}*/

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node *Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::SeekContainingChild(const AABB &box) const
{
	if(!m_children) {
		return NULL;
	}

	for(unsigned int i = 0; i < (X_DIV+1)*(Y_DIV+1)*(Z_DIV+1); i++)
	{
		if(m_children[i].Contains(box)) return &m_children[i];
	}
	return NULL;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
bool Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Expand()
{
	if(m_numObjects >= MAX_OBJECTS && m_children == NULL)
	{
		m_maxLevelInBranch++;
		if(m_tree->m_maxLevelNodes < m_maxLevelInBranch) m_tree->m_maxLevelNodes = m_maxLevelInBranch;
		m_tree->m_numNodes += (X_DIV+1)*(Y_DIV+1)*(Z_DIV+1);

		m_children = new Node[(X_DIV+1)*(Y_DIV+1)*(Z_DIV+1)];
		for(unsigned int x = 0; x < X_DIV+1; x++) {
			for(unsigned int y = 0; y < Y_DIV+1; y++) {
				for(unsigned int z = 0; z < Z_DIV+1; z++) {
					unsigned int index = x*(Y_DIV+1)*(Z_DIV+1) + y*(Z_DIV+1) + z;
					m_children[index].m_tree = m_tree;
					m_children[index].m_box = AABB(
										Vector3(
										m_box.MinPoint.x + (m_box.MaxPoint.x - m_box.MinPoint.x)*x/(float)(X_DIV+1),
										m_box.MinPoint.y + (m_box.MaxPoint.y - m_box.MinPoint.y)*y/(float)(Y_DIV+1),
										m_box.MinPoint.z + (m_box.MaxPoint.z - m_box.MinPoint.z)*z/(float)(Z_DIV+1)),
										Vector3(
										m_box.MinPoint.x + (m_box.MaxPoint.x - m_box.MinPoint.x)*(x+1)/(float)(X_DIV+1),
										m_box.MinPoint.y + (m_box.MaxPoint.y - m_box.MinPoint.y)*(y+1)/(float)(Y_DIV+1),
										m_box.MinPoint.z + (m_box.MaxPoint.z - m_box.MinPoint.z)*(z+1)/(float)(Z_DIV+1))
										);
					m_children[index].m_parent = this;
					m_children[index].m_index = index;
					m_children[index].m_nodeLevel = m_nodeLevel + 1;
					m_children[index].m_maxLevelInBranch = m_maxLevelInBranch;
				}
			}
		}
		Node *parent = m_parent;
		while(parent)
		{
			if(parent->m_maxLevelInBranch >= m_maxLevelInBranch) {
				break;
			}
			else {
				parent->m_maxLevelInBranch = m_maxLevelInBranch;
				parent = parent->m_parent;
			}
		}

		// On répartit les objets du node dans ses enfants
		Element::ptr pObj = m_firstObject;
		while(pObj)
		{
			Element::ptr pNext = pObj->m_next;
			for(unsigned int i = 0; i < (X_DIV+1)*(Y_DIV+1)*(Z_DIV+1); i++)
			{
				if(m_children[i].Contains(pObj->GetAABB())) {
					m_children[i].Add(pObj);
					break;
				}
			}
			pObj = pNext;
		}
		return true;
	}
	return false;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
bool Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Retract()
{
	unsigned int numObjects = 0;
	GetNumObjects(numObjects, (2*MAX_OBJECTS)/3 + 1);

	if(numObjects <= (2*MAX_OBJECTS/3))
	{
		MoveObjectsTo(this);
		delete[] m_children;
		m_children = NULL;
		m_tree->m_numNodes -= (X_DIV+1)*(Y_DIV+1)*(Z_DIV+1);

		unsigned int maxLevelInBranch = m_maxLevelInBranch;
		m_maxLevelInBranch = m_nodeLevel;

		Node *parent = m_parent;
		Node *previous = this;
		while(parent) // On modifie le niveau max dans les nodes parents
		{
			if(parent->m_maxLevelInBranch == maxLevelInBranch)
			{
				for(unsigned int i = 0; i < (X_DIV+1)*(Y_DIV+1)*(Z_DIV+1); i++)
				{
					if(parent->m_children[i].m_maxLevelInBranch == maxLevelInBranch && &m_children[i] != previous)
					{
						return true;
					}
				}
				parent->m_maxLevelInBranch = m_nodeLevel;
				previous = parent;
				parent = parent->m_parent;
			}
			else
			{
				return true;
			}
		}

		// On a atteint le niveau max
		m_tree->m_maxLevelNodes = m_tree->m_parentNode->m_maxLevelInBranch;

		return true;
	}
	return false;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::GetNumObjects(unsigned int &num, unsigned int max)
{
	num += m_numObjects;
	if(num >= max) return;
	if(m_children)
	{
		for(unsigned int i = 0; i < (X_DIV+1)*(Y_DIV+1)*(Z_DIV+1); i++)
		{
			m_children[i].GetNumObjects(num, max);
			if(num >= max) return;
		}
	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::MoveObjectsTo(Node *dest)
{
	if(dest != this)
	{
		while(m_firstObject)
		{
			Element::ptr element = m_firstObject;
			Remove(element);
			dest->Add(element);
		}
	}
	if(m_children)
	{
		for(unsigned int i = 0; i < (X_DIV+1)*(Y_DIV+1)*(Z_DIV+1); i++)
		{
			m_children[i].MoveObjectsTo(dest);
		}
	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Write(std::ostream &stream)
{
	for(unsigned int i = 0; i < m_nodeLevel; i++) {
		stream << "   ";
	}
	stream << m_numObjects << endl;

	if(m_children) {
		for(unsigned int i = 0; i < (X_DIV+1)*(Y_DIV+1)*(Z_DIV+1); i++) {
			m_children[i].Write(stream);
		}
	}
}

//
//	Tree::Node::Element
//
template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Element::Element(PTR object, Tree *parentTree)
{
	m_object = object;
	m_parentTree = parentTree;
	m_parent = NULL;

	m_previous = NULL;
	m_next = NULL;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Element::~Element()
{

}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
const AABB &Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node::Element::GetAABB() const
{
	return m_object->GetAABB();
}

//
//	Tree::Iterator
//
template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator::Iterator(typename Node::Element::ptr object)
{
	m_ptr = object;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator &Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator::operator = (typename Node::Element *ptr)
{
	m_ptr = ptr;

	return *this;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
bool Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator::operator ! () const
{
	return !m_ptr;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator::operator bool () const
{
	return m_ptr != NULL;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
T &Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator::operator * ()
{
	return *m_ptr;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator::operator T* ()
{
	return m_ptr->m_object;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator::operator bool ()
{
	return m_ptr != NULL;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator::UpadtePositionInTree()
{
	if(m_ptr)
	{
		typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator it = m_ptr->m_parent->Move(*this);
		*this = it;
	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator::Remove()
{
	Node::Remove(*this);
}

//
//	Tree::UpIterator
//
template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::UpIterator::UpIterator(const Iterator &it)
{
	m_ptr = it.m_ptr;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::UpIterator &Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::UpIterator::operator = (typename const Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator &it)
{
	m_ptr = it.m_ptr;

	return *this;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::UpIterator::GoToFirstObject()
{
	if(m_ptr) {
		if(m_ptr->m_parent) {
			m_ptr = m_parent->m_firstObject;
		}
	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::UpIterator::GoToNextNode()
{
	if(m_ptr) {
		if(m_ptr->m_parent) {
			m_ptr = m_parent->m_lastObject;
			operator++();
		}
	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::UpIterator &Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::UpIterator::operator ++ ()
{
	if(m_ptr)
	{
		// On cherche si le node a un élément après l'élément pointé actuellement
		if(m_ptr->m_next)
		{
			m_ptr = m_ptr->m_next;
		}
		// On cherche le premier élément du node précédent dans la hiérarchie
		else
		{
			Node *parent = m_ptr->m_parent->m_parent;
			while(parent)
			{
				if(!parent->m_firstObject) {
					parent = parent->m_parent;
				}
				else {
					m_ptr = parent->m_firstObject;
					return *this;
				}
			}
			m_ptr = NULL;
			return *this;
		}
	}

	return *this;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
bool Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::UpIterator::operator ! () const
{
	return !m_ptr;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::UpIterator::operator bool () const
{
	return m_ptr != NULL;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
T &Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::UpIterator::operator * ()
{
	return *m_ptr;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::UpIterator::operator T* ()
{
	return ((m_ptr != NULL) ? (m_ptr->m_object) : (NULL));
}

//
//	Tree::DownIterator
//
template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::DownIterator::DownIterator(const Iterator &it)
{
	m_ptr = it.m_ptr;
	m_initialNodeLevel = ((m_ptr != NULL) ? m_ptr->m_parent->m_nodeLevel : 0);
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::DownIterator &Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::DownIterator::operator = (typename const Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator &it)
{
	m_ptr = it.m_ptr;
	m_initialNodeLevel = ((m_ptr != NULL) ? m_ptr->m_parent->m_nodeLevel : 0);

	return *this;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::DownIterator::GoToLastObject()
{
	if(m_ptr) {
		if(m_ptr->m_parent) {
			m_ptr = m_parent->m_lastObject;
			m_initialNodeLevel = m_ptr->m_parent->m_nodeLevel;
		}
	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::DownIterator::GoToNextNode()
{
	if(m_ptr) {
		if(m_ptr->m_parent) {
			m_ptr = m_parent->m_firstObject;
			m_initialNodeLevel = m_ptr->m_parent->m_nodeLevel;
			operator++();
		}
	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::DownIterator &Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::DownIterator::operator ++ ()
{
	if(m_ptr)
	{
		// On cherche si le node a un élément après l'élément pointé actuellement
		if(m_ptr->m_previous)
		{
			m_ptr = m_ptr->m_previous;
		}
		// On cherche le dernier élément du node suivant dans la hiérarchie
		else
		{
			Node *parentNode = m_ptr->m_parent;
			Node *node = &m_ptr->m_parent->m_children[0];
			while(1)
			{
				// On descend
				while(node)
				{
					if(node->m_lastObject)
					{
						m_ptr = node->m_lastObject;
						return *this;
					}
					else
					{
						parentNode = node;
						node = &node->m_children[0];
					}
				}

				// Il n'y a plus d'enfants, on remonte
				unsigned int nodeLevel = parentNode->m_nodeLevel;
				while(nodeLevel > m_initialNodeLevel && parentNode->m_index == (X_DIV+1)*(Y_DIV+1)*(Z_DIV+1)-1)
				{
					parentNode = parentNode->m_parent;
					nodeLevel--;
				}

				if(nodeLevel == m_initialNodeLevel)
				{
					m_ptr = NULL;
					return *this;
				}
				else
				{
					node = &parentNode->m_parent->m_children[ parentNode->m_index + 1 ]; // Le node parentNode doit avoir un parent car sinon il est de niveau 0 et alors la condition précédente est forcément vraie
				}
			}
			m_ptr = node->m_lastObject;

			return *this;
		}
	}

	return *this;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
bool Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::DownIterator::operator ! () const
{
	return !m_ptr;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::DownIterator::operator bool () const
{
	return m_ptr != NULL;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
T &Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::DownIterator::operator * ()
{
	return *m_ptr;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::DownIterator::operator T* ()
{
	return ((m_ptr != NULL) ? (m_ptr->m_object) : (NULL));
}



//
//	Tree::BoolIterator
//
template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
template<class Test>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::BoolIterator<Test>::BoolIterator(Test test) : m_test(test)
{
	m_ptr = NULL;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
template<class Test>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::BoolIterator<Test>::BoolIterator(Test test, const Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV> &tree) : m_test(test)
{
	m_ptr = NULL;

	Node *node = tree.ParentNode();

	while(node)
	{
		if(node->m_firstObject)
		{
			m_ptr = node->m_firstObject;
			return;
		}
					
		node = GoToNextNode(node);
	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
template<class Test>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::BoolIterator<Test>::BoolIterator(Test test, typename const Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Iterator &it) : m_test(test)
{
	m_ptr = it.m_ptr;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
template<class Test>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node *Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::BoolIterator<Test>::GoToNextNode(typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::Node *fNode)
{
	if(!fNode)
	{
		return NULL;
	}
	else
	{
		Node *node = fNode;

		if(node->m_children) // On descend si c'est possible
		{
			for(unsigned int i = 0; i < (X_DIV+1)*(Y_DIV+1)*(Z_DIV+1); i++)
			{
				if(m_test(node->m_children[i].m_box))
				{
					return &node->m_children[i];
				}
			}
		}

		// On remonte
		if(!node->m_parent)
		{
			return NULL;
		}
		else
		{
			while(1)
			{
				while(node->m_index == (X_DIV+1)*(Y_DIV+1)*(Z_DIV+1)-1)
				{
					node = node->m_parent;
					if(!node) return NULL;
				}
			
				if(!node->m_parent) return NULL; // On est revenu au premier node, on s'arrête

				for(unsigned int i = node->m_index+1; i < (X_DIV+1)*(Y_DIV+1)*(Z_DIV+1); i++)
				{
					node = &node->m_parent->m_children[i];
					if(m_test(node->m_box))
					{
						return node;
					}
				}
			}
		}

	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
template<class Test>
void Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::BoolIterator<Test>::GoToBeginning(const Tree &tree)
{
	m_ptr = NULL;

	Node *node = tree.ParentNode();

	while(node)
	{
		if(node->m_firstObject)
		{
			m_ptr = node->m_firstObject;
			return;
		}
					
		node = GoToNextNode(node);
	}
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
template<class Test>
typename Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::BoolIterator<Test> &Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::BoolIterator<Test>::operator ++ ()
{
	if(m_ptr)
	{
		// On cherche si le node a un élément après l'élément pointé actuellement
		if(m_ptr->m_next)
		{
			m_ptr = m_ptr->m_next;
		}
		// On cherche le dernier élément du node suivant dans la hiérarchie
		else
		{
			Node *node = GoToNextNode(m_ptr->m_parent);

			while(node)
			{
				if(node->m_firstObject)
				{
					m_ptr = node->m_firstObject;
					return *this;
				}
					
				node = GoToNextNode(node);
			}
			m_ptr = NULL;

			return *this;
		}
	}

	return *this;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
template<class Test>
bool Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::BoolIterator<Test>::operator ! () const
{
	return !m_ptr;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
template<class Test>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::BoolIterator<Test>::operator bool () const
{
	return m_ptr != NULL;
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
template<class Test>
T &Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::BoolIterator<Test>::operator * ()
{
	return *(m_ptr->m_object);
}

template<class T, unsigned int MAX_OBJECTS, class PTR, unsigned int X_DIV, unsigned int Y_DIV, unsigned int Z_DIV>
template<class Test>
Tree<T, MAX_OBJECTS, PTR, X_DIV, Y_DIV, Z_DIV>::BoolIterator<Test>::operator T* ()
{
	return ((m_ptr != NULL) ? (m_ptr->m_object) : (NULL));
}

//
//	Tree::BoxIterator
//

} // namespace Crowd