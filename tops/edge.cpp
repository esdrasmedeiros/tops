/*:
**     edge.c - edge and hedge
**
**  Copyright (C) 2004 Luiz Velho, Hélio Lopes, Esdras Medeiros.
*/

#include "mesh.h"

using namespace Mesh;

/**
* Updates the vertices of the adge. Also updates the half-edge fields.
* @param p0 Pointer to origin vertex.
* @param p1 Pointer to end vertex.
*/
Edge::Edge(Vertex *p0, Vertex *p1) 
{
  h_[0].set_org(p0); h_[1].set_org(p1);
  h_[0].set_face(NULL); h_[1].set_face(NULL);;
  h_[0].set_next(NULL); h_[1].set_next(NULL);
  h_[0].set_edge(this); h_[1].set_edge(this);
}

/**
* Returns a half-edge specified by the index i. 
* i=0 half-edge of the origin vertex. 
* i=1 half-edge if the end vertex.
* @param i Index of the half-edge. 
* @return half-edge of the edge.
*/
Hedge* Edge::hedge(int i)
{
  switch (i) {
  case 0: return &h_[0];
  case 1: return &h_[1];
  }
  throw Error("edge n");
}
/**
* Returns the half-edge in the opposite direction. 
* @return opposite Half-edge.
*/
Hedge* Hedge::mate()
{ 
  return (this == e_->hedge(0))? e_->hedge(1): e_->hedge(0);
};

/**
* Updates the vertices of the half-edge.
* @param v0 Pointer to origin vertex.
* @param v1 Pointer to end vertex.
* @return half-edge.
*/
Hedge* Hedge::reuse(Vertex *v0, Vertex *v1)
{
  set_org(v0); mate()->set_org(v1);
  set_next(NULL); mate()->set_next(NULL);
  set_face(NULL); mate()->set_face(NULL);
  return this;
}



