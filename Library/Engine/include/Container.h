#pragma once
#include "Node.h"

namespace Engine
{
	template<typename T>
	class Container
	{
	public:
		int num;
		Node<T>* head;
		Node<T>* end;

		//Constructor
		Container()
		{
			head = nullptr;
			end = head;
			num = 0;
		}

		//Destructors
		~Container()
		{
			while (end)
			{
				Node<T>* preNode = end->pre;
				delete end;
				end = preNode;
			}
		}

		//add a node
		void addNode(Node<T>* newNode)
		{

			if (head == nullptr)
			{
				head = newNode;
				end = head;
				num++;
			}
			else
			{
				newNode->pre = end;
				end->next = newNode;
				end = newNode;
				num++;
			}
		}

		//delete a node
		Node<T>* deleteNode(Node<T>* deleteNode)
		{
			Node<T>* temp = head;
			Node<T>* retNode = head;
			if (!temp)
				return nullptr;
			for (int i = 0; i < num; i++)
			{
				if (temp == deleteNode)
				{
					if (temp->next == nullptr)
					{
						if (temp->pre != nullptr)
						{
							//update end pointer
							temp->pre->next = nullptr;
							end = temp->pre;
						}
						else
						{
							head = nullptr;
							end = nullptr;
						}
					}
					else
					{
						if (temp->pre == nullptr)
						{
							//update head pointer
							temp->next->pre = nullptr;
							head = temp->next;
						}
						else
						{
							temp->next->pre = temp->pre;
							temp->pre->next = temp->next;
						}
					}
					retNode = deleteNode->next;
					delete deleteNode;
					break;
				}
				temp = temp->next;
			}
			num--;
			return retNode;
		}
	};
}
