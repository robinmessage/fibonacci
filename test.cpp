/*Copyright (c) 2010, Robin Message <Robin.Message@cl.cam.ac.uk>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Univsersity of Cambridge nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY OF CAMBRIDGE OR ROBIN MESSAGE
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include "fibonacci.hpp"

//Add dotty output to our heap, which produces nice diagrams
class DotFibonacciHeap : public FibonacciHeap<int> {
public:
	void dump() {
		printf("digraph G {\n");
		if(heap==NULL) {
			printf("empty;\n}\n");
			return;
		}
		printf("minimum -> \"%p\" [constraint=false];\n",heap);
		node<int>* c=heap;
		do {
			_dumpChildren(c);
			c=c->getNext();
		} while(c!=heap);
		printf("}\n");
	}

private:

	void _dumpChildren(node<int>* n) {
		printf("\"%p\" -> \"%p\" [constraint=false,arrowhead=lnormal];\n",n,n->getNext());
		printf("\"%p\" -> \"%p\" [constraint=false,arrowhead=ornormal];\n",n,n->getPrev());
		if(n->isMarked())printf("\"%p\" [style=filled,fillcolor=grey];\n",n);
		if(n->hasParent()) {
			printf("\"%p\" -> \"%p\" [constraint=false,arrowhead=onormal];\n",n,n->getParent());
		}
		printf("\"%p\" [label=%d];\n",n,n->getValue());
		if(n->hasChildren()) {
			node<int>* c=n->getChild();
			do {
				printf("\"%p\" -> \"%p\";\n",n,c);
				_dumpChildren(c);
				c=c->getNext();
			} while(c!=n->getChild());
		}
	}
};


void test() {
	DotFibonacciHeap h;
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
	node<int>* nine=h.insert(90);
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
