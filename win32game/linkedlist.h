#pragma once
#include <algorithm>
using std::swap;

template <class T> struct Node {
	Node() : data(0), prev(NULL), next(NULL) {}
	Node(T data) : data(data), prev(NULL), next(NULL) {}
	T data;					// the object information
    Node *prev, *next;		// pointer to the next node element
};

template <class T> class List {
	Node<T> *pHead, *pTail;
	int count;
public:
	List() : pHead(NULL), pTail(NULL), count(0) {}
	List(const List<T> &that) : pHead(NULL), pTail(NULL), count(0) {
		Node<T> *ptr = that.pHead;
		while(ptr) { 
			this->push_back(ptr->data); 
			ptr = ptr->next;
		}
	}
	~List() { clear(); }
	void clear() {
		Node<T> * ptr = pHead;
		while(ptr != NULL) {
			pHead = pHead->next;
			delete ptr;
			ptr = pHead;
		}
		pHead = pTail = NULL;
		count = 0;
	}
	bool empty() const { return count == 0; }
	int size() const { return count; }
	T & back() const { return pTail->data; }
	T & front() const { return pHead->data; }
	void push_front(const T & x) {
		Node<T> *node = new Node<T>(x);

		if(pHead == NULL) {
			pHead = pTail = node;
		} else {
			node->next = pHead;
			pHead->prev = node;
			pHead = node;
		}
		++count;
	}

	void push_back(const T & x) {
		Node<T> *node = new Node<T>(x);

		if(pTail == NULL) {
			pHead = pTail = node;
		} else {
			node->prev = pTail;
			pTail->next = node;
			pTail = node;
		}
		++count;
	}

	void pop_front() {
		if(count == 0) return;
		Node<T> *oldhead = pHead;
		pHead = pHead->next;
		delete oldhead;
		if (pHead != NULL) {
			pHead->prev = NULL;
		}
		else {
			pHead = pTail = NULL;
		}

		count--;
	}

	void pop_back() {
		if(count == 0) return;
		Node<T> *oldtail = pTail;
		pTail = pTail->prev;
		delete oldtail;
		if (pTail != NULL) {
			pTail->next = NULL;
		}
		else {
			pHead = pTail = NULL;
		}
		count--;
	}

	void insertAt(int pos, const T & x) {
		if(pos < 0 || pos >= count) return;

		if(pos == 0) { push_front(x); return; }
		if(pos == count - 1) { push_back(x); return; }

		int i = 0;
		while(ptr && i < pos) { 
			ptr = ptr->next;
			++i;			
		}

		Node<T> *node = new Node<T>(x);
		node->next = ptr;
		node->prev = ptr->prev;
		ptr->prev = node;
		ptr->prev->next = node;
		++count;
	}

	void removeAt(int pos) {
		if(pos < 0 || pos >= count) return;
		if(pos == 0) { pop_front(); return; }
		if(pos == count - 1) { pop_back(); return; }
		printf("REMOV\n");
		int i = 0;
		Node<T> *ptr = pHead;
		while(ptr && i < pos) { 
			ptr = ptr->next;
			++i;			
		}
		Node<T> *removable = ptr;

		ptr->prev->next = ptr->next;
		ptr->next->prev = ptr->prev;
		delete removable;
		--count;
	}

	List<T> & operator=(const List<T> & that) {
		if(this != &that) {
			List<T> temp(that);
			std::swap(this->pHead, temp.pHead);
			std::swap(this->pTail, temp.pTail);
			count = that->count;
		}
		return *this;
	}

	T & operator[] (int pos) const {

		if(pos == 0) return pHead->data;
		if(pos == count - 1) return pTail->data;

		Node<T> *ptr = pHead;
		int i = 0;
		while(ptr && i < pos) {
			++i;
			ptr = ptr->next;
		}
		return ptr->data;
	}

	const Node<T>* getFirst() { return pHead; }
	const Node<T>* getLast() { return pTail; }
};