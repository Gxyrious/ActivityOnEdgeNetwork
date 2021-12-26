#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include "LinkedList.hpp"
using namespace std;
#define DefaultSize 10

template<class T>
class MyEdge {
public:
	MyEdge() { _weight = 0; _nextEdge = 0; _secNode = 0, _tag = false; }
	MyEdge(int num, T weight) :
		_secNode(num), _weight(weight), _nextEdge(NULL), _tag(false) {}
	bool _tag;
	int _secNode;
	T _weight;
	MyEdge<T>* _nextEdge;
};

template<class T, class E>
class MyVertex {
public:
	MyVertex() { _data = 0; _firstEdge = NULL; _tag = false; Ve = -1; Vl = INT_MAX; }
	MyVertex(E& data) :_data(data), Ve(-1), Vl(INT_MAX) { _data = 0; _firstEdge = NULL; }
	bool _tag;
	int Ve, Vl;
	E _data;
	MyEdge<T>* _firstEdge;
};


template<class T, class E>
class Graph {
public:
	LinkedList<int> startNode, endNode;
	int* TopoSort;
	int getMaxVertices()const { return _maxVertices; }//获取结点最大数
	int getNumVertices()const { return _numVertices; }//获取当前结点数
	int getNumEdges()const { return _numEdges; }//获取当前边的数目
	T getDefaultValueOfEdge()const { return _defaultValue_T; }//获取默认边权值
	E getDefaultValueOfVertex()const { return _defaultValue_E; }//获取默认结点数据
	E getPosData(int i) const { return _NodeTable[i]._data; }
	Graph(T dafaultValue_T = 0, E defaultValue_E = 0, int maxVertices = DefaultSize) {
		//构造函数，要求输入两种数据类型的默认值
		if (maxVertices <= 1) { maxVertices = DefaultSize; }
		_defaultValue_T = dafaultValue_T;
		_defaultValue_E = defaultValue_E;
		_maxVertices = maxVertices;
		_numVertices = 0;
		_numEdges = 0;
		_NodeTable = new MyVertex<T, E>[_maxVertices];
		TopoSort = new int[_maxVertices];
		for (int i = 0; i < _maxVertices; i++) {
			_NodeTable[i]._firstEdge = NULL;
			TopoSort[i] = -1;
		}
	}
	void clear() {
		for (int i = 0; i < _numVertices; i++) {
			MyEdge<T>* p = _NodeTable[i]._firstEdge;
			while (p != NULL) {
				_NodeTable[i]._firstEdge = p->_nextEdge;
				delete p;
				p = _NodeTable[i]._firstEdge;
			}
		}
		for (int i = 0; i < _maxVertices; i++) {
			_NodeTable[i]._firstEdge = NULL;
			TopoSort[i] = -1;
		}
		startNode.Erase();
		endNode.Erase();
		_numVertices = 0;
		_numEdges = 0;
	}

	~Graph() {
		//析构函数
		for (int i = 0; i < _numVertices; i++) {
			MyEdge<T>* p = _NodeTable[i]._firstEdge;
			while (p != NULL) {
				_NodeTable[i]._firstEdge = p->_nextEdge;
				delete p;
				p = _NodeTable[i]._firstEdge;
			}
		}
		delete[] _NodeTable;
		delete[] TopoSort;
	}
	E getVertexValue(int i)const {
		//获取某个结点的值
		if (i >= 0 && i < _numVertices) { return _NodeTable[i]._data; }
		else { return _defaultValue_E; }
	}
	bool insertVertex(const E& vertex) {
		//插入结点，数据为vertex
		if (_numVertices == _maxVertices) { return false; }
		for (int i = 0; i < _numVertices; i++) {
			if (_NodeTable[i]._data == vertex) { return false; }
		}
		_NodeTable[_numVertices++]._data = vertex;
		return true;
	}
	T getEdgeWeight(int v1, int v2)const {
		//获取某条边的权值
		if (v1 != -1 && v2 != -1) {
			MyEdge<T>* p = _NodeTable[v1]._firstEdge;
			while (p != NULL && p->_secNode != v2) { p = p->_nextEdge; }
			if (p != NULL) { return p->_weight; }
		}
		return _defaultValue_T;
	}
	bool insertEdge(const E& n1, const E& n2, const T& weight) {
		//在v1和v2之间插入权值为weight的边，若边已存在则返回false
		int v1 = getVertexPos(n1), v2 = getVertexPos(n2);
		if (v1 >= 0 && v1 < _numVertices && v2 >= 0 && v2 < _numVertices) {
			MyEdge<T>* p = _NodeTable[v1]._firstEdge;
			while (p != NULL && p->_secNode != v2) { p = p->_nextEdge; }
			if (p != NULL) { return false; }//此边本就存在
			p = new MyEdge<T>;
			p->_secNode = v2;
			p->_weight = weight;
			p->_nextEdge = _NodeTable[v1]._firstEdge;
			_NodeTable[v1]._firstEdge = p;
			_numEdges++;
			return true;
		}
		return false;
	}
	bool removeVertex(E& n) {
		//删除值为n的结点
		int v = getVertexPos(n);
		if (_numVertices == 1 || v < 0 || v >= _numVertices) { return false; }
		MyEdge<T>* p, * s, * t;
		int i, k;
		while (_NodeTable[v]._firstEdge != NULL) {
			p = _NodeTable[v]._firstEdge;
			//删除所有与v相连的顶点指向v的边
			k = p->_secNode;
			s = _NodeTable[k]._firstEdge;
			t = NULL;
			while (s != NULL && s->_secNode != v) {
				t = s;
				s = s->_nextEdge;
			}
			if (s != NULL) {
				if (t == NULL) { _NodeTable[k]._firstEdge = s->_nextEdge; }
				else { t->_nextEdge = s->_nextEdge; }
				delete s;
			}
			_NodeTable[v]._firstEdge = p->_nextEdge;
			delete p;
			_numEdges--;
		}
		_numVertices--;
		_NodeTable[v]._data = _NodeTable[_numVertices]._data;
		p = _NodeTable[v]._firstEdge = _NodeTable[_numVertices]._firstEdge;
		//start from v,search circles
		while (p != NULL) {
			s = _NodeTable[p->_secNode]._firstEdge;
			while (s != NULL) {
				if (s->_secNode == _numVertices) { s->_secNode = v; break; }
				else { s = s->_nextEdge; }
			}
			p = p->_nextEdge;
		}
		return true;
	}
	bool removeEdge(E& n1, E& n2) {
		//删除n1和n2之间的边，若不存在这条边则返回false
		int v1 = getVertexPos(n1), v2 = getVertexPos(n2);
		if (v1 != -1 && v2 != -1) {
			MyEdge<T>* p = _NodeTable[v1]._firstEdge, * q = NULL, * s = p;
			while (p != NULL && p->_secNode != v2) {
				q = p;
				p = p->_nextEdge;
			}
			if (p != NULL) {
				if (p == s) { _NodeTable[v1]._firstEdge = p->_nextEdge; }
				else { q->_nextEdge = p->_nextEdge; }
				delete p;
			}
			else { return false; }
			return true;
		}
		return false;
	}
	int getFirstNeighbor(int i) {
		if (i != -1) {
			MyEdge<T>* p = _NodeTable[i]._firstEdge;
			if (p != NULL) {
				return p->_secNode;
			}
		}
		return -1;
	}
	int getNextNeighbor(E i, E j) {
		if (i != -1) {
			MyEdge<T>* p = _NodeTable[i]._firstEdge;
			while (p != NULL && p->_secNode != j) {
				p = p->_nextEdge;
			}
			if (p != NULL && p->_nextEdge != NULL) {
				return p->_nextEdge->_secNode;
			}
		}
		return -1;
	}
	bool TopologicalSort() {
		//若存在拓扑排序，则返回true，并在TopoSort[]中记录对应的顺序
		//若不存在拓扑排序，则返回false
		int i, w, v, counter = 0;
		int top = -1;//指向TopoSort最后元素
		int n = getNumVertices();
		int* count = new int[n];
		bool* ifConnected = new bool[n];
		for (int i = 0; i < n; i++) {
			count[i] = 0;
			ifConnected[i] = false;
		}
		/* 遍历所有边，将节点的入读记录在count数组中 */
		for (int i = 0; i < n; i++) {
			MyEdge<T>* p = _NodeTable[i]._firstEdge;
			if (p != NULL) { ifConnected[i] = true; }
			while (p != NULL) {
				ifConnected[p->_secNode] = true;
				count[p->_secNode]++;
				p = p->_nextEdge;
			}
		}
		for (int i = 0; i < n; i++) {
			//如果存在一个没被连接上，则返回false
			if (!ifConnected[i]) { 
				delete[] count;
				delete[] ifConnected;
				return false;
			}
		}

		for (i = 0; i < n; i++) {
			if (count[i] == 0) {
				count[i] = top;
				top = i;
			}
		}

		for (int i = 0; i < n; i++) {
			if (top == -1) {
				//存在回路
				delete[] count;
				delete[] ifConnected;
				return false;
			}
			else {
				v = top;
				top = count[top];
				TopoSort[counter] = v;
				counter++;
				w = getFirstNeighbor(v);
				while (w != -1) {
					if (--count[w] == 0) {
						count[w] = top;
						top = w;
					}
					w = getNextNeighbor(v, w);
				}
			}
		}
		delete[] count;
		delete[] ifConnected;
		return true;
	}
	inline void VeWrite(int i, int ve) {
		_NodeTable[i].Ve = ve;
	}
	inline void VlWrite(int i, int vl) {
		_NodeTable[i].Vl = vl;
	}
private:
	MyVertex<T, E>* _NodeTable;//各边链表的头结点
	T _defaultValue_T;
	E _defaultValue_E;
	int _maxVertices, _numEdges, _numVertices;
	inline int getVertexPos(const E& data) {
		//获取数据为vertex的结点序号
		for (int i = 0; i < _numVertices; i++) {
			if (_NodeTable[i]._data == data) { return i; }
		}
		//目标结点不存在
		return -1;
	}
};

template <class T, class E>
void CriticalPath(Graph<T, E>* G) {
	int i, j, k;
	E Ae, Al, w;
	int n = G->getNumVertices();
	E* Ve = new E[n], * Vl = new E[n];
	for (i = 0; i < n; i++) {
		//初始化
		Ve[i] = 0;
		Vl[i] = INT_MAX;
	}
	for (i = 0; i < n; i++) {
		//正序遍历求Ve
		int curNode = G->TopoSort[i];
		j = G->getFirstNeighbor(curNode);
		while (j != -1) {
			w = G->getEdgeWeight(curNode, j);
			if (Ve[curNode] + w > Ve[j]) {
				Ve[j] = Ve[curNode] + w;
			}
			j = G->getNextNeighbor(curNode, j);
		}
	}
	Vl[n - 1] = Ve[n - 1];
	for (j = n - 2; j >= 0; j--) {
		//逆序遍历求Vl
		int curNode = G->TopoSort[j];
		k = G->getFirstNeighbor(curNode);
		while (k != -1) {
			w = G->getEdgeWeight(curNode, k);
			if (Vl[k] - w < Vl[curNode]) {
				Vl[curNode] = Vl[k] - w;
			}
			k = G->getNextNeighbor(curNode, k);
		}
	}
	for (i = 0; i < n; i++) {
		G->VeWrite(i, Ve[i]);
		G->VlWrite(i, Vl[i]);
	}
	for (i = 0; i < n; i++) {
		//判断Ve == Vl
		int curNode = G->TopoSort[i];
		j = G->getFirstNeighbor(curNode);
		while (j != -1) {
			Ae = Ve[curNode];
			Al = Vl[j] - G->getEdgeWeight(curNode, j);
			if (Al == Ae) {
				G->startNode.PushBack(curNode);
				G->endNode.PushBack(j);
			}
			j = G->getNextNeighbor(curNode, j);
		}
	}
	delete[]Ve;
	delete[]Vl;
}

#endif // GRAPH_HPP
