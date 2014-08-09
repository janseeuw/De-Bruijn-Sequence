//
//  Eulercircuit.h
//  Algoritmen II
//
//  Created by Jonas Anseeuw on 28/10/13.
//  Copyright (c) 2013 Jonas Anseeuw. All rights reserved.
//

#ifndef Algoritmen_II_Eulercircuit_h
#define Algoritmen_II_Eulercircuit_h

#include "graph.h"
#include <vector>
#include <queue>

template<EdgeType RT>
class Eulercircuit{
public:
	Eulercircuit<RT>(Graph<RT> &g, std::vector<int> &v):g(g){
		v.clear();
		int kn=0;
		while(tour(kn)==kn && !q.empty()){
			kn=q.top();
			q.pop();
			v.push_back(kn);
		}
	};
private:
	Graph<RT> g;
	std::stack<int> q;
	int tour(int v){
		while(true){
			std::map<int,int>::iterator it = g[v].begin();
			int w = it->first;
			if(it==g[v].end()) break;
			q.push(v);
			g.removeEdge(v,w);
			v=w;
		}
		return v;
	}
};


#endif
