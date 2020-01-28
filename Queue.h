#pragma once
#include "pch.h"
#include <iostream>
#include <stdexcept>

template<class TypeName>
class Queue
{
private:

	struct Node
	{
		TypeName info;
		Node* next;

		Node(TypeName info, Node* next)
		{
			this->info = info;
			this->next = next;
		}

		~Node()
		{
			this->next = nullptr;
		}
	};

	Node* first;
	Node* last;
	size_t size;

public:

	//creates an empty queue
	Queue()
	{
		first = nullptr;
		last = nullptr;
		size = 0;
	}

	//creates queue with size 'size' filled with 'info'
	Queue(size_t size, TypeName info)
	{
		this->first = nullptr;
		this->last = nullptr;

		this->size = 0;

		for (size_t i = 0; i < size; ++i)
			enqueue(info);
	}

	//clears the queue
	~Queue()
	{
		if (size > 1)
		{
			Node* current = first;

			while (first != last)
			{
				first = current->next;
				delete current;
				current = first;
				size--;
			}
		}

		if(size > 0)
			delete first;

		last = nullptr;
		first = nullptr;
		size = 0;
	}

	//returns size of the Queue
	inline size_t getSize()
	{
		return size;
	}

	//returns the first element of the Queue
	inline TypeName peek()
	{
		if (size != 0)
			return first->info;
		else
			throw std::out_of_range("Queue was empty!");
	}

	//inserts an element in the Queue
	void enqueue(TypeName info)
	{
		//instantly inserts element as the last
		if (size > 0)
		{
			Node* current = new Node(info, nullptr);
			last->next = current;
			last = current;
		}
		else
		{
			first = new Node(info, nullptr);
			last = first;
		}
		size++;
	}

	//deletes an element and returns its info
	TypeName dequeue()
	{
		if (size > 0)
		{
			//instantly deletes first element 
			TypeName info = first->info;

			if (size > 1)
			{
				Node* current = first->next;

				delete first;
				first = current;
				size--;
			}
			else
			{
				delete first;
				last = nullptr;
				first = nullptr;
				size = 0;
			}

			return info;
		}
		else
			throw std::out_of_range("Queue was empty!");
	}
};

