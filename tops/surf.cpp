/*:
**     surf.cpp
**
**   Copyright (C) 2004 Luiz Velho, Hélio Lopes, Esdras Medeiros.
*/
#include "mesh.h"

using namespace Mesh;

/**
* Dealocates all data (vertices, faces, edges) of surface.
*/
Surf::~Surf()
{
  for (VertexIter p = vc_.begin(); p != vc_.end(); p++) 
    delete *p;
  vc_.clear();
  for (EdgeIter e = ec_.begin(); e != ec_.end(); e++)
    delete *e;
  ec_.clear();
  for (FaceIter f = fc_.begin(); f != fc_.end(); f++)
    delete *f;
  fc_.clear();
  bc_.clear();
}

/**
* Adds a new edge in the surface.
* @param v0 origin vertex pointer.
* @param v1 end vertex pointer.
* @return pointer to half-egde of the origin vertex.
*/
Hedge *Surf::add_new_edge(Vertex *v0, Vertex *v1)
{
  Edge *e = new Edge(v0, v1);
  add_edge(e);
  return e->hedge(0);
}

/**
* Adds a new vertex in the surface.
* @return pointer to new vertex.
*/
Vertex *Surf::add_new_vertex(Point *p)
{
  Vertex *v = new Vertex(p);
  add_vertex(v);
  return v;
}

/**
* Look for a hedge in surface.
* @param p0 origin point pointer.
* @param p1 end point pointer.
* @return pointer to the hedge.
*/
Hedge* Surf::lookup_hedge(Point *p0, Point *p1)
{
  for (EdgeIter e = ec_.begin(); e != ec_.end(); e++) {
    Hedge* h = (*e)->hedge(0);
    if (h->org()->p() == p0 && h->dst()->p() == p1) return h;
    if (h->org()->p() == p1 && h->dst()->p() == p0) return h->mate();
  }
  return NULL;
}

/**
* Adds topological elements from other surface.
* @param s2 surface origin.
*/
void Surf::transfer_cells(Surf *s2)
{
  for (VertexIter p = s2->verts_begin(); p != s2->verts_end(); p++) 
    add_vertex(*p);
  for (EdgeIter e = s2->edges_begin(); e != s2->edges_end(); e++)
    add_edge(*e);
  for (FaceIter f = s2->faces_begin(); f != s2->faces_end(); f++)
    add_face(*f);
  for (EdgeIter e = s2->bdries_begin(); e != s2->bdries_end(); e++)
    add_bdry(*e);
}





