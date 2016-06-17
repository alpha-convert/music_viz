#ifndef VERTEX_H_
#define VERTEX_H_ 
#include <vector>
#include "Vec3/Vec3.h"
class Vertex{
public:
	std::vector<unsigned> adj;
	Vec3 pos;
	Vertex(Vec3 pos, std::vector<unsigned> adj) : adj(adj), pos(pos){}
};


#endif

