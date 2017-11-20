
#include <string>
#include <iostream>
#include <fstream>
#include "Graph.hpp"
#include <ctype.h>
using namespace std;

bool openFile(ifstream& fin, const char* fileName);
void loadFromFile(const char* fileName, const char delimiter, Graph<int>* myGraph);
int getNodeIdFromLine(string line);
void addEdgesAtCurrentVertex(string line,int currentVertexId ,Graph<int>* myGraph);
int getNeighborIdFromLine(string line,int& currentPositionInLine);
int getNeighorWeightFromLine(string line,int& currentPositionInLine);

int main() {
  const char* fileName = "graphin.txt";
	Graph<int>* myGraph = new Graph<int>();
	loadFromFile(fileName, ':', myGraph);
  
  
  myGraph->outputMst();

	delete myGraph;
	myGraph = nullptr;
	return 0;
}

bool openFile(ifstream & fin, const char* fileName)
{
	fin.open(fileName);
	return fin.is_open();
}

void loadFromFile(const char* fileName, const char delimiter, Graph<int>* myGraph)
{
	int currentVertexId = 0;
	int adjVertex = 0;
	string line;
	ifstream fin;
	int i = 0;
	if (!openFile(fin, fileName)) 
  {
		cerr << "Error: Unable to open file." << endl;
		exit(1);
	}

	while (getline(fin,line))
	{
		currentVertexId = getNodeIdFromLine(line);
		(*myGraph).addVertex(currentVertexId);
	}

	fin.clear();
	fin.seekg(0, fin.beg);
	while (getline(fin,line))
	{
		  //Add the Edges that are Adjacent to each previously Created Vertex
		  currentVertexId = getNodeIdFromLine(line); 
      addEdgesAtCurrentVertex(line,currentVertexId,myGraph); //Create the Edges that are adjacent to recently added Vertex.
  }
  
}

//Used when preforming DFS and Dijkstra/Prim's algorithm's to construct the vertexes.
int getNodeIdFromLine(string line)
{
	return stoi(line.substr(0));
}

//Function used when preforming dfs on directed unweighted graphs
//Returns a list of all the neighborids from the current line.

void addEdgesAtCurrentVertex(string line,int currentVertexId,Graph<int>* myGraph)
{
int i = 1;
int&  currentPositionInLine = i;
int neighborId;
int edgeWeight;
  while(i < line.size()-1)
  {
    neighborId = getNeighborIdFromLine(line,++currentPositionInLine);
    edgeWeight = getNeighorWeightFromLine(line,++currentPositionInLine);
   (*myGraph).addNeighbor(currentVertexId,edgeWeight,neighborId);
  }

}

int getNeighborIdFromLine(string line,int& currentPositionInLine)
{

  for(currentPositionInLine; currentPositionInLine < line.size(); ++currentPositionInLine)
  {
    if(isdigit(line.at(currentPositionInLine)))
    {
      return stoi(line.substr(currentPositionInLine));
    }
    
  }

}

int getNeighorWeightFromLine(string line,int& currentPositionInLine)
{
  for(currentPositionInLine; currentPositionInLine < line.size(); ++currentPositionInLine)
  {
      if(isdigit(line.at(currentPositionInLine)))
      {
        return stoi(line.substr(currentPositionInLine));
      }
  }
}

