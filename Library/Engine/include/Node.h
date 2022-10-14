#ifndef _NODE_H_
#define _NODE_H_

namespace Engine
{
	template<typename T>
	class Node
	{
	public:
		T* m_data;
		Node<T>* next;
		Node<T>* pre;

		Node(T* data)
		{
			this->m_data = data;
			this->next = nullptr;
			this->pre = nullptr;
		}

		~Node()
		{
			delete m_data;
		}
	};
}

#endif // !_NODE_H_

