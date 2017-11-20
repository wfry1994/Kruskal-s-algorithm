#pragma once
#ifndef Graph_hpp
#define Graph_hpp

#include <list>
#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <limits.h>
using namespace std;
template <typename E>
class Graph
{
private:
  int nodeId;
  int time;
  bool acyclic;
  int edgeCount;
class Node;
class Edge;
  class Node{
	public:
		E vertexData;
		char color;
		int discoveryTime;
		int finishTime;
    int rank;
    int key;
		vector<Edge*> neighbors;
		Node* parent;
		Node(const E& vertexData, int discoveryTime, int finishTime)
			:vertexData(vertexData), discoveryTime(discoveryTime), finishTime(finishTime)
		{
			color = 'w';
		}
		void addNeighbor(Edge* edge)
		{
			neighbors.push_back(edge);
		}	
	};//end Node

  class Edge
  {
    public:
     friend class Node;
     int weight;
     Node* source;
     Node* destination;
     Edge(int weight,Node* source,Node* destination)
       : weight(weight), source(source),destination(destination)  
     {

     }
   };//End Edge

     void makeSet(Node* node)
    {  
		      node->parent = node;
          node->rank = 0;  

    }
    Node* findSet(Node* node)
    {
      if(node->parent != node)
      {
         node->parent = findSet(node->parent);
      }
      return node->parent;
    }
    void link(Node* node1, Node* node2)
   		{
          if (node1->rank > node2->rank)
          {
            node2->parent = node1;
          }else{
            node1->parent = node2;
            
            //Union by size used
            if(node1->rank == node2->rank )
            {
              node2->rank = node2->rank +1;
            }
          }
      }
   void unionSet(Node* node1,Node* node2)
   {
     link(findSet(node1),findSet(node2));
   } 

	Node* createNode(const E& vertex, int discoveryTime, int finishTime)
	{
		return new Node(vertex,discoveryTime,finishTime);
	}

 Edge* createEdge(int weight,Node* source,Node* destination)
     {
       return new Edge(weight, source,destination);
     }
  void releaseGraph()
  {
    graph.clear();
    for(int i = 0; i < edges.size();i++)
    {
      delete edges[i];
      edges[i] = nullptr;
    }
  }
protected:
		map<E,Node*> graph;
    vector<Edge*> edges;
public:
	Graph()
	{
    edgeCount = 0;
		nodeId = 1;
		acyclic = true;
	}

	~Graph() 
  {
    releaseGraph();
  }
	void addVertex(E vertex)
	{
		Node* node = createNode(vertex,0, 0);
		graph[nodeId++] = node;

	}
	void addNeighbor(int nodeId,int weight,int neighborNodeId)
  {
     Edge* edge = createEdge(weight,graph[nodeId],graph[neighborNodeId]);
   	 graph[nodeId]->addNeighbor(edge);
     edges.push_back(edge);
	}

	bool isGraphAcyclic() {
		return acyclic;
	}
	
 void displayEdgesAndWeights()
 {
  //created to show the validity of my graph after i had it constructed

   for(int i = 0; i < edges.size();i++)
   {
     cout << "An Edge exists from " << edges[i]->source->vertexData << " to " << edges[i]->destination->vertexData << " With " << edges[i]->weight << " Weight" << endl; 
   }
 }

void sortEdges(){
	mergeSort(edges,0,edges.size()-1);
}

void mergeSort(vector<Edge*>& edges,int leftIndex, int rightIndex)
{
	int middle;
	if (leftIndex < rightIndex) 
	{
		middle = (leftIndex + rightIndex) / 2;
		mergeSort(edges, leftIndex , middle);
		mergeSort(edges,middle + 1 , rightIndex);
		merge(edges,leftIndex,middle,rightIndex);
	}
}

void merge(vector<Edge*>& edges, int leftIndex, int middle, int rightIndex)
{
	int i, j, k = 0;
	int leftArraySize = middle - leftIndex + 1;
	int rightArraySize = rightIndex - middle;
	vector<Edge*> leftVector;
	leftVector.resize(leftArraySize);
	vector<Edge*> rightVector;
	rightVector.resize(rightArraySize);


	for ( i = 0; i < leftArraySize; i++) {
		leftVector[i] = edges[leftIndex + i];
	}

	for ( j = 0; j < rightArraySize; j++) {
		rightVector[j] = edges[middle + 1 + j];
	}
	k = leftIndex;
	i = 0;
	j = 0;
	for (k; (i < leftArraySize) && (j < rightArraySize); k++) 
	{
		if (leftVector[i]->weight <= rightVector[j]->weight)
				{
					edges[k] = leftVector[i++];	
				}
				else
				{
				edges[k] = rightVector[j++];
				}
	}
	if (i < leftArraySize) {
	for (i; i < leftArraySize; i++) {
			edges[k++] = leftVector[i];
		}
	}

	if (j < rightArraySize) {
		for (j; j < rightArraySize; j++) {
			edges[k++] = rightVector[j];
		}
	}
}

vector<Edge*> kruskal()
{
vector<Edge*> edgesInMst;
sortEdges();
for (typename std::map<E, Node*>::iterator it = graph.begin(); it != graph.end(); it++)
{
 makeSet(it->second);

} 
for(int i = 0; i < edges.size(); i++)
{
if(findSet(edges[i]->source) != findSet(edges[i]->destination))
  {
      edgesInMst.push_back(edges[i]);
      unionSet(edges[i]->source,edges[i]->destination);
  }
}

return edgesInMst;
}//END KRUSKAL-------------------


vector<Edge*> prim()
{
vector<Node*>vertex;
Node* min;
Node* v;
vector<Edge*> edgesInPrimMst;
Node* nil = createNode(0,0,0);
for (typename std::map<E, Node*>::iterator it = graph.begin(); it != graph.end(); it++)
{ 
 
it->second->parent = nil;
it->second->key = INT_MAX;
  //Insert into heap....
vertex.push_back(it->second);

}
decreaseKey(vertex[0],0);
buildHeap(vertex,vertex.size());
while(!vertex.empty())
{
   min = extractMin(vertex);
   edgesInPrimMst.push_back(createEdge(min->key,min->parent,min));
   for(int i = 0; i < min->neighbors.size();i++)
  {
     
    v = min->neighbors[i]->destination;
    if(isInHeap(vertex,v) && min->neighbors[i]->weight < v->key)
    {
     v->parent = min;
     decreaseKey(v,min->neighbors[i]->weight);

    }

  }

    buildHeap(vertex,vertex.size());
 }

delete nil;
nil = nullptr;
return edgesInPrimMst;
}//END PRIM--------------

bool isInHeap(const vector<Node*> vertex,Node* node)
{
  for(int i = 0; i < vertex.size();i++)
  {
    if(vertex[i] == node)
    {
      return true;
    }
  }
  return false;

}
Node* extractMin(vector<Node*>& vertex)
{
  Node* temp;
  Node* min = vertex[0];
  temp = vertex[0];
  vertex[0] = vertex[vertex.size()-1];
  vertex[vertex.size()-1] = temp;
  vertex.pop_back();
  return min;
}

void decreaseKey(Node* node,int key)
{
    node->key = key;
}
void outputMst()
{
  int totalWeight = 0;
  vector<Edge*> edgesInKruskalMst  = kruskal();
  vector<Edge*> edgesInPrimMst = prim();
  ofstream fout;
  fout.open("kruskalout.txt");
  fout << "Edges In Kruskal MST:" << endl;
  for(int i = 0; i < edgesInKruskalMst.size();i++)
  {
    fout << "Edge From " << edgesInKruskalMst[i]->source->vertexData << " To " << edgesInKruskalMst[i]->destination->vertexData << " With Weight " << edgesInKruskalMst[i]->weight << endl; 
  totalWeight += edgesInKruskalMst[i]->weight;
  
  }
  fout << "Total Weight of Kruskal MST: " << totalWeight;

  fout.close();
  totalWeight = 0;

  fout.open("primout.txt");
  fout << "Edges in Prim MST:" << endl;
  for(int i = 1; i < edgesInPrimMst.size();i++)
  {
    fout << "Edge From " << edgesInPrimMst[i]->source->vertexData << " To " << edgesInPrimMst[i]->destination->vertexData << " With Weight " << edgesInPrimMst[i]->weight << endl;
  totalWeight += edgesInPrimMst[i]->weight;
  
  }
  fout << "Total Weight of Prim MST: " << totalWeight;
  fout.close();


  
}

void buildHeap(vector<Node*>& vertexHeap, int count)
{

	for (int i = count / 2; i >= 0; i--) 
	{
		minHeapify(vertexHeap, i, count);
	}
}

void minHeapify(vector<Node*>& vertexHeap, int rootIndex, int n)
{

	int min;
  int left  = 2 * rootIndex;
  int right = 2 * rootIndex + 1;
	Node* temp;
  

if(left < vertexHeap.size() && vertexHeap[left]->key < vertexHeap[rootIndex]->key)
{
  min = left;
}
else
{
  min = rootIndex;
}
if(right < vertexHeap.size() && vertexHeap[right]->key < vertexHeap[min]->key)
{
  min = right;
}

if(min != rootIndex)
{
  temp = vertexHeap[rootIndex];
	vertexHeap[rootIndex] = vertexHeap[min];
	vertexHeap[min] = temp;
  minHeapify(vertexHeap,min,n);
}

}

};//END GRAPH
#endif
