//
//  Created by Jonas Anseeuw on 27/12/13.
//  Copyright (c) 2013 Jonas Anseeuw. All rights reserved.
//

#include <vector>
#include "Eulercircuit.h"
#include <math.h>

class DeBruijnsequence: public std::vector<char>{
public:
	DeBruijnsequence(int _m, int _n):m(_m),n(_n-1){
		std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
		
		int count = (int) pow(m, n);
		std::vector<std::string> vertices(count);
		for(int i = 0; i < count; i++){
			int x = i;
			for(int j = 0; j < n; j++){
				vertices.at(i) += alphabet.at(x % m);
				x /= m;
			}
		}
		
		GraphWithVertexAndEdgedata<DIRECTED, std::string, char> graph;
		std::vector<int> from;
		std::vector<int> to;
		std::vector<char> c;
		for(int i = 0; i < count; i++){
			graph.addVertex(vertices.at(i));
			for(int j = 0; j < count; j++){
				if(vertices.at(i).substr(1, n-1) == vertices.at(j).substr(0, n-1)){
					from.push_back(i);
					to.push_back(j);
					c.push_back(vertices.at(j).at(n-1));
				}
			}
		}
		for(int i = 0; i < from.size(); i++){
			graph.addEdge(from.at(i), to.at(i), c.at(i));
		}
		
		std::vector<int> temp;
		Eulercircuit<DIRECTED> euler(graph, temp);
		
		for(int i = temp.size()-1; i > 0; i--){
			push_back(*graph.getEdgedata(temp.at(i), temp.at(i - 1)));
		}
		push_back(*graph.getEdgedata(temp.at(0), temp.at(temp.size()-1)));
		
	};
	friend std::ostream &operator<<(std::ostream &os, const DeBruijnsequence &b);
private:
	const int n;
	const int m;
};

int main(int argc, char *argv[]) {
	DeBruijnsequence b(2, 3);
	
	for(int i=0; i<b.size(); i++){
		cout<<b[i];
	}
	
	return 0;
}
