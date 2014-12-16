#include <iostream>
#include <vector>

using namespace std;

struct node {
	int x;
	int y;
	int dist = 10000;
	node previous = NULL;
	int index;
};

node* smallestDist(const &vector<node> v) {
	unsigned sz = vector.size();
	node* smallestNode = v.at(0);

	for(unsigned i = 0; i < sz; i++) {
		if(smallestNode.dist > v.at(i).dist)
			smallestNode = v.at(i);
		}
	}
	return smallestNode;
}
		
void Dijkstra(int a, int b, int[][] graph, node source) {
	vector<node> unvisited;
	source.dist = 0;
	int cnt = 0;
	for(int i = 0; i < a; i++) {
		for(int j = 0; j < b; j++) {
			unvisited.push_back(graph[i][j]);
			unvisited.at(cnt).index = cnt;
			cnt++;
		}
	}

	while(!unvisited.empty()) {
		node* curNode = smallestDist(&unvisited); 
		//unvisited.remove(unvisited.begin() + curNode->index);	
		unvisited.at(unvisited.begin() + curNode->index) = NULL;

		
		int alt = 0;
		// up
		alt = curNode->dist + 1;
		if(alt < unvisited.at(
		
}

int main()
{


	return 0;
}
