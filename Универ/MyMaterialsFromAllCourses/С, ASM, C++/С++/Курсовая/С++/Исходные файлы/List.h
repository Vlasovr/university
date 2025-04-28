#pragma once
#include <iostream>

enum RemoveMode 
{
	All,
	First
};

template<typename T>
class List
{
private:

	template<typename T>
	struct Node
	{
		Node(T data = T(), Node<T> *ptrPrevious = nullptr, Node<T> *ptrNext = nullptr)
		{
			this->data = data;
			this->ptrPrevious = ptrPrevious;
			this->ptrNext = ptrNext;
		}

		T data;
		Node<T> *ptrPrevious;
		Node<T> *ptrNext;
	};

	Node<T> *findElement(const unsigned int index)
	{
		if (size <= (int)index)
			return nullptr;

		Node<T>* findedNode;

		if (size / 2 >= (int)index)
		{
			findedNode = head;
			for (unsigned int i = 0; i < index; ++i)
				findedNode = findedNode->ptrNext;
		}
		else
		{
			findedNode = tail;
			for (unsigned int i = size - 1; i > index; --i)
				findedNode = findedNode->ptrPrevious;
		}

		return findedNode;
	}

	void setDefaultValues();

	int size;
	Node<T> *head;
	Node<T> *tail;

public:

	List(const T*, const int);
	List();
	List(std::initializer_list<T>);
	~List();
	T &operator[](const unsigned int);

	List<T> &operator=(const List<T>& other);
	T &front() const;
	T &back() const;
	void pushFront(const T);
	void pushBack(const T);
	void popFront();
	void popBack();
	void insert(const T &, const unsigned int);
	void removeAt(const unsigned int);
	void remove(const T &, RemoveMode);
	void clear();
	bool isEmpty() const;
	int getSize() const;

	int firstIndexOf(const T &) const;
	bool contains(const T &) const;

	template<typename T>
	class Iterator {
	public:
		Iterator() : node(nullptr) {}
		Iterator(Node<T>* node) : node(node) {}
		Iterator(const Iterator &other) : node(other.node) {}

		Iterator& operator=(const Iterator &other) {
			node = other.node;
			return *this;
		}

		T &operator*() const { return node->data; }
		T* operator->() { return &(node->data); }
		bool operator==(const Iterator &other) const { return node == other.node; }
		bool operator!=(const Iterator &other) const { return node != other.node; }

		Iterator &operator++()
		{
			node = node->ptrNext;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator temp(*this);
			node = node->ptrNext;
			return temp;
		}

		Iterator &operator--()
		{
			node = node->ptrPrevious;
			return *this;
		}

		Iterator operator--(int)
		{
			Iterator temp(*this);
			node = node->ptrPrevious;
			return temp;
		}

		Iterator operator+(int n) const {
			Node<T>* newNode = node;
			for (int i = 0; i < n; ++i) 
			{
				if (newNode != nullptr)
					newNode = newNode->ptrNext;
			}
			return Iterator(newNode);
		}

		
		Iterator operator-(int n) const {
			Node<T>* newNode = node;
			for (int i = 0; i < n; ++i)
			{
				if (newNode != nullptr)
					newNode = newNode->ptrPrevious;
			}
			return Iterator(newNode);
		}

	private:
		Node<T>* node;
	};

	Iterator<T> begin() const { return Iterator<T>(head); }
	Iterator<T> end() const { return Iterator<T>(nullptr); }
	Iterator<T> rbegin() const { return Iterator<T>(tail); }
	Iterator<T> rend() const { return Iterator<T>(nullptr); }
	Iterator<T> find(const T& data) const {
		for (Iterator<T> it = begin(); it != end(); ++it) {
			if (*it == data)
				return it;
		}
		return end();
	}

	Iterator<T> findAt(const unsigned int index) const {

		if ((unsigned)size <= index)
			throw std::out_of_range("Ошибка(Iterator<T> findAt(const unsigned int): Выход за границы списка");

		Node<T>* findedNode;

		if ((unsigned)size / 2 >= index)
		{
			findedNode = head;
			for (unsigned int i = 0; i < index; ++i)
				findedNode = findedNode->ptrNext;
		}
		else
		{
			findedNode = tail;
			for (unsigned int i = size - 1; i > index; --i)
				findedNode = findedNode->ptrPrevious;
		}

		return Iterator<T>(findedNode);
	}
};

template<typename T>
List<T>::List(const T* data, const int size)
{
	for (int i = 0; i < size; ++i)
	{
		pushBack(*(data + i));
	}
}

template<typename T>
List<T>::List()
{
	setDefaultValues();
}

template<typename T>
List<T>& List<T>::operator=(const List<T>& other) {
	if (this == &other)
		return *this;
		
	clear();

	for (auto it = other.begin(); it != other.end(); it++) {
		auto data = *it;
		pushBack(*it);
	}

	return *this;
}

template<typename T>
List<T>::List(std::initializer_list<T> data)
{
	setDefaultValues();
	for (auto item : data)
		pushBack(item);
}

template<typename T>
List<T>::~List()
{
	clear();
}

template<typename T>
T& List<T>::front() const
{
	if (isEmpty())
		throw std::exception("Ошибка(T & List<T>::front()): Выход за границы списка");
	return head->data;
}

template<typename T>
T& List<T>::back() const
{
	if (isEmpty())
		throw std::out_of_range("Ошибка(T & List<T>::back()): Выход за границы списка");
	return tail->data;
}

template<typename T>
T& List<T>::operator[](const unsigned int index)
{
	if (size <= (int)index)
		throw std::out_of_range("Ошибка(T & List<T>::operator[](const int index)): Выход за границы списка");

	Node<T>* currentNode = findElement(index);

	return currentNode->data;
}

template<typename T>
void List<T>::setDefaultValues()
{
	size = 0;
	head = nullptr;
	tail = nullptr;
}

template<typename T>
void List<T>::pushFront(const T element)
{
	if (size == 0)
	{
		head = new Node<T>(element, nullptr, nullptr);
		tail = head;
	}
	else
	{
		Node<T>* newNode = new Node<T>(element, nullptr, head);
		head->ptrPrevious = newNode;
		head = newNode;
	}
	++size;
}

template<typename T>
void List<T>::pushBack(const T element)
{
	if (size <= 1)
	{
		if (size == 0)
		{
			head = new Node<T>(element, nullptr, nullptr);
			tail = head;
		}
		else
		{
			tail = new Node<T>(element, head, nullptr);
			head->ptrNext = tail;
		}
	}
	else
	{
		Node<T>* newNode = new Node<T>(element, tail, nullptr);
		tail->ptrNext = newNode;
		tail = newNode;
	}
	++size;
}

template<typename T>
void List<T>::popFront()
{
	if (size <= 1)
	{
		if (size == 0)
			throw std::exception("Ошибка: Cписок уже пуст!");
		if (size == 1)
		{
			delete head;
			setDefaultValues();
		}
		return;
	}
	Node<T>* nextNode = head->ptrNext;
	delete head;
	nextNode->ptrPrevious = nullptr;
	head = nextNode;
	--size;
}

template<typename T>
void List<T>::popBack()
{
	if (size <= 1)
	{
		if (size == 0)
			throw std::exception("Ошибка: Cписок уже пуст!");
		if (size == 1)
		{
			delete tail;
			setDefaultValues();
		}
		return;
	}
	Node<T>* previousNode = tail->ptrPrevious;
	delete tail;
	previousNode->ptrNext = nullptr;
	tail = previousNode;
	--size;

}

template<typename T>
void List<T>::insert(const T& element, const unsigned int index)
{
	if (index == 0)
		pushFront(element);
	else if (size == (int)index)
		pushBack(element);
	else if (size < (int)index)
		throw std::out_of_range("Ошибка(void List<T>::insert(const T &element, const int index)): Выход за границы списка");
	else
	{
		Node<T>* nextNode = findElement(index);
		Node<T>* newNode = new Node<T>(element, nextNode->ptrPrevious, nextNode);
		nextNode->ptrPrevious->ptrNext = newNode;
		nextNode->ptrPrevious = newNode;
		++size;
	}
}

template<typename T>
void List<T>::removeAt(const unsigned int index)
{
	if (size <= (int)index)
		throw std::exception("Ошибка(void List<T>::removeAt(const int index)): Выход за границы списка");
	else if (index == 0)
		popFront();
	else if ((int)index == size - 1)
		popBack();
	else
	{
		Node<T>* currentNode = findElement(index);
		if (currentNode->ptrNext != nullptr)
			currentNode->ptrNext->ptrPrevious = currentNode->ptrPrevious;
		if (currentNode->ptrPrevious != nullptr)
			currentNode->ptrPrevious->ptrNext = currentNode->ptrNext;
		delete currentNode;
		--size;
	}
}

template<typename T>
void List<T>::remove(const T& element, RemoveMode mode)
{
	Node<T>* currentNode = head;
	while (currentNode != nullptr)
	{
		if (currentNode->data == element)
		{
			Node<T>* tempNode = currentNode->ptrNext;

			if (currentNode->ptrNext != nullptr)
				currentNode->ptrNext->ptrPrevious = currentNode->ptrPrevious;
			else
				tail = currentNode->ptrPrevious;
			if (currentNode->ptrPrevious != nullptr)
				currentNode->ptrPrevious->ptrNext = currentNode->ptrNext;
			else
				head = currentNode->ptrNext;

			delete currentNode;
			currentNode = tempNode;
			--size;
			if (mode == All)
				continue;
			else
				break;
		}
		currentNode = currentNode->ptrNext;
	}
}

template<typename T>
int List<T>::firstIndexOf(const T& element) const
{
	Node<T>* currentNode = head;
	for (int i = 0; i < size; ++i)
	{
		if (currentNode->data == element)
			return i;
		currentNode = currentNode->ptrNext;
	}
	return -1;
}

template<typename T>
bool List<T>::contains(const T& element) const
{
	return firstIndexOf(element) != -1;
}

template<typename T>
void List<T>::clear()
{
	while (size != 0)
		popFront();
}

template<typename T>
bool List<T>::isEmpty() const
{
	return size == 0;
}

template<typename T>
int List<T>::getSize() const
{
	return size;
}