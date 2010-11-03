#include <stdio.h>
using namespace std;

template <class V> class FibonacciHeap {
public:
	struct node {
	private:
		node* prev;
		node* next;
		node* child;
		node* parent;
		V value;
		int degree;
		bool marked;
	public:
		friend class FibonacciHeap;
	};

private:
	node* heap;
public:

	FibonacciHeap() {
		heap=_empty();
	}
	virtual ~FibonacciHeap() {
		if(heap) {
			_deleteAll(heap);
		}
	}
	node* insert(V value) {
		node* ret=_singleton(value);
		heap=_merge(heap,ret);
		return ret;
	}
	void merge(FibonacciHeap& other) {
		heap=_merge(heap,other.heap);
		other.heap=_empty();
	}
	void dump() {
		printf("digraph G {\n");
		if(heap==NULL) {
			printf("empty;\n}\n");
			return;
		}
		printf("minimum -> \"%p\" [constraint=false];\n",heap);
		node* c=heap;
		do {
			_dumpChildren(c);
			c=c->next;
		} while(c!=heap);
		printf("}\n");
	}

	bool isEmpty() {
		return heap==NULL;
	}

	V getMinimum() {
		return heap->value;
	}

	V removeMinimum() {
		node* old=heap;
		heap=_removeMinimum(heap);
		V ret=old->value;
		delete old;
		return ret;
	}

	void decreaseKey(node* n,V value) {
		heap=_decreaseKey(heap,n,value);
	}

	node* find(V value) {
		return _find(heap,value);
	}
private:
	node* _empty() {
		return NULL;
	}

	node* _singleton(V value) {
		node* n=new node;
		n->value=value;
		n->prev=n->next=n;
		n->degree=0;
		n->marked=false;
		n->child=NULL;
		n->parent=NULL;
		return n;
	}

	node* _merge(node* a,node* b) {
		if(a==NULL)return b;
		if(b==NULL)return a;
		if(a->value>b->value) {
			node* temp=a;
			a=b;
			b=temp;
		}
		node* an=a->next;
		node* bp=b->prev;
		a->next=b;
		b->prev=a;
		an->prev=bp;
		bp->next=an;
		return a;
	}

	void _dumpChildren(node* n) {
		printf("\"%p\" -> \"%p\" [constraint=false,arrowhead=lnormal];\n",n,n->next);
		printf("\"%p\" -> \"%p\" [constraint=false,arrowhead=ornormal];\n",n,n->prev);
		if(n->marked)printf("\"%p\" [style=filled,fillcolor=grey];\n",n);
		printf("\"%p\" [label=%d];\n",n,n->value);
		if(n->child) {
			node* c=n->child;
			do {
				printf("\"%p\" -> \"%p\";\n",n,c);
				_dumpChildren(c);
				c=c->next;
			} while(c!=n->child);
		}
		if(n->parent) {
			printf("\"%p\" -> \"%p\" [constraint=false,arrowhead=onormal];\n",n,n->parent);
		}
	}

	void _deleteAll(node* n) {
		if(n!=NULL) {
			node* c=n;
			do {
				node* d=c;
				c=c->next;
				_deleteAll(d->child);
				delete d;
			} while(c!=n);
		}
	}
	
	void _addChild(node* parent,node* child) {
		child->prev=child->next=child;
		child->parent=parent;
		parent->degree++;
		parent->child=_merge(parent->child,child);
	}

	void _unMarkAndUnParentAll(node* n) {
		if(n==NULL)return;
		node* c=n;
		do {
			c->marked=false;
			c->parent=NULL;
			c=c->next;
		}while(c!=n);
	}

	node* _removeMinimum(node* n) {
		_unMarkAndUnParentAll(n->child);
		if(n->next==n) {
			n=n->child;
		} else {
			n->next->prev=n->prev;
			n->prev->next=n->next;
			n=_merge(n->next,n->child);
		}
		if(n==NULL)return n;
		node* trees[64]={NULL};
		
		while(true) {
			if(trees[n->degree]!=NULL) {
				node* t=trees[n->degree];
				if(t==n)break;
				trees[n->degree]=NULL;
				if(n->value<t->value) {
					t->prev->next=t->next;
					t->next->prev=t->prev;
					_addChild(n,t);
				} else {
					t->prev->next=t->next;
					t->next->prev=t->prev;
					if(n->next==n) {
						t->next=t->prev=t;
						_addChild(t,n);
						n=t;
					} else {
						n->prev->next=t;
						n->next->prev=t;
						t->next=n->next;
						t->prev=n->prev;
						_addChild(t,n);
						n=t;
					}
				}
				continue;
			} else {
				trees[n->degree]=n;
			}
			n=n->next;
		}
		node* min=n;
		do {
			if(n->value<min->value)min=n;
			n=n->next;
		} while(n!=n);
		return min;
	}

	node* _cut(node* heap,node* n) {
		if(n->next==n) {
			n->parent->child=NULL;
		} else {
			n->next->prev=n->prev;
			n->prev->next=n->next;
			n->parent->child=n->next;
		}
		n->next=n->prev=n;
		n->marked=false;
		return _merge(heap,n);
	}

	node* _decreaseKey(node* heap,node* n,V value) {
		if(n->value<value)return heap;
		n->value=value;
		if(n->value<n->parent->value) {
			heap=_cut(heap,n);
			node* parent=n->parent;
			n->parent=NULL;
			while(parent!=NULL && parent->marked) {
				heap=_cut(heap,parent);
				n=parent;
				parent=n->parent;
				n->parent=NULL;
			}
			if(parent!=NULL && parent->parent!=NULL)parent->marked=true;
		}
		return heap;
	}

	node* _find(node* heap,V value) {
		node* n=heap;
		if(n==NULL)return NULL;
		do {
			if(n->value==value)return n;
			node* ret=_find(n->child,value);
			if(ret)return ret;
			n=n->next;
		}while(n!=heap);
		return NULL;
	}
};


void test() {
	FibonacciHeap<int> h;
	h.insert(2);
	h.insert(3);
	h.insert(1);
	h.insert(4);
	h.removeMinimum();
	h.removeMinimum();
	h.insert(5);
	h.insert(7);
	h.removeMinimum();
	h.insert(2);
	FibonacciHeap<int>::node* nine=h.insert(90);
	h.removeMinimum();
	h.removeMinimum();
	h.removeMinimum();
	for(int i=0;i<20;i+=2)h.insert(30-i);
	for(int i=0;i<4;i++)h.removeMinimum();
	for(int i=0;i<20;i+=2)h.insert(30-i);
	h.insert(23);
	for(int i=0;i<7;i++)h.removeMinimum();
	h.decreaseKey(nine,1);
	h.decreaseKey(h.find(28),2);
	h.decreaseKey(h.find(23),3);

	h.dump();
}

int main() {
	test();
}
