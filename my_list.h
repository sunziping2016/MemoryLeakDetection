#ifndef MY_LIST_H_INCLUDE
#define MY_LIST_H_INCLUDE

#include "memory_leak_detection.h"

#include <utility>
#include <iterator>

template<typename ValueType>
class MyListNode
{
public:
	MyListNode<ValueType> *prev, *next;
	ValueType data;
	// No copy
	MyListNode(const MyListNode &orig) = delete;
	MyListNode & operator = (const MyListNode &orig) = delete;

	MyListNode(): prev(nullptr), next(nullptr), data() {}
	MyListNode(MyListNode &&orig): prev(orig.prev), next(orig.next), data(std::move(orig.data)) {}
	MyListNode(const ValueType &orig): prev(nullptr), next(nullptr), data(orig) {}
	MyListNode(ValueType &&orig): prev(nullptr), next(nullptr), data(std::move(orig)) {}
	~MyListNode() {}
};

template<typename ValueType>
class MyList;

template<typename ValueType>
class MyListIterator : std::iterator<std::bidirectional_iterator_tag, ValueType>
{
	friend class MyList<ValueType>;
public:
	typedef ValueType value_type;
	typedef MyListNode<value_type> node_type;
	typedef MyListIterator<value_type> iterator;
	typedef const MyListIterator<value_type> const_iterator;

	MyListIterator(node_type *node): node(node) {}
	MyListIterator(const iterator &other): node(other.node) {}
	MyListIterator &operator = (const iterator &other)
	{
		node = other.node;
		return *this;
	}
	~MyListIterator() {}

	void swap(iterator &other)
	{
		node_type *temp = other.node;
		other.node = node;
		node = temp;
	}

	ValueType &operator * ()
	{
		return node->data;
	}
	ValueType &operator -> ()
	{
		return &node->data;
	}
	const ValueType &operator * () const
	{
		return node->data;
	}
	const ValueType &operator -> () const
	{
		return &node->data;
	}

	bool operator == (const iterator &other) const
	{
		return node == other.node;
	}
	bool operator != (const iterator &other) const
	{
		return !(*this == other);
	}

	iterator &operator ++ ()
	{
		node = node->next;
		return *this;
	}
	const iterator &operator ++ (int)
	{
		const iterator temp(*this);
		node = node->next;
		return temp;
	}
	iterator &operator -- ()
	{
		node = node->prev;
		return *this;
	}
	const iterator &operator -- (int)
	{
		const iterator temp(*this);
		node = node->prev;
		return temp;
	}
private:
	MyListNode<ValueType> *node;
};

#include <iostream>

template<typename ValueType>
class MyList
{
public:
	typedef ValueType value_type;
	typedef MyListNode<value_type> node_type;
	typedef MyListIterator<value_type> iterator;
	typedef const MyListIterator<value_type> const_iterator;


	MyList(const MyList &orig) = delete;
	MyList & operator = (const MyList &orig) = delete;

	MyList()
	{
		root = new node_type();
		root->next = root->prev = root;
	}
	~MyList()
	{
		clear();
		delete root;
	}
	void clear()
	{
		while(root->next != root)
			remove(root->next);
	}
	void remove(iterator iter)
	{
		remove(iter.node);
	}
	void push_back(const value_type &value)
	{
		insert(root, new node_type(value));
	}
	void insert(iterator iter, const value_type &value)
	{
		insert(iter->node,  new node_type(value));
	}
	iterator begin()
	{
		return iterator(root->next);
	}
	iterator end()
	{
		return iterator(root);
	}
	const_iterator begin() const
	{
		return iterator(root->next);
	}
	const_iterator end() const
	{
		return iterator(root);
	}
	template<typename Func = std::less<value_type> >
	void sort(Func func = Func())
	{
		node_type *temp;
		for (node_type *outer = root; outer->prev != root->next; outer = outer->prev)
			for (node_type *inner = root->next; inner->next != outer; inner = inner->next) {
				if (!func(inner->data, inner->next->data)) {
					temp = inner->next;
					inner->prev->next = temp;
					temp->next->prev = inner;
					inner->next = temp->next;
					temp->prev = inner->prev;
					inner->prev = temp;
					temp->next = inner;
					inner = temp;
				}
			}
	}
protected:
	void remove(node_type *node)
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
		delete node;
	}
	void insert(node_type *node, node_type *newnode)
	{
		node->prev->next = newnode;
		newnode->prev = node->prev;
		node->prev = newnode;
		newnode->next = node;
	}
private:
	node_type *root;
};
#endif
