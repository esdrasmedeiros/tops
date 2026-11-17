/*:
**     face.cpp - face
**
**  Copyright (C) 2004 Luiz Velho, Hélio Lopes, Esdras Medeiros.
*/

#include "mesh.h"

/* indices of 3-ring and n-ring */
#define NEXT3(k)        ((k < 2)? (k+1) : 0) ///< Returns next half-edge index.
#define PREV3(k)        ((k > 0)? (k-1) : 2) ///< Returns previous half-edge index.


using namespace Mesh;
/**
* Face costructor.
* @param s surface pointer.
* @param e0 First half-edge.
* @param e1 Second half-edge.
* @param e2 Third half-edge.
*/
Face::Face(Surf* s, Hedge *e0, Hedge *e1, Hedge *e2)
{
  s_ = s;
  reuse(e0, e1, e2);
}
/**
* Updates the half-edges of the face. 
* Make half-edge links to build a face loop.
* Links half-edges to face. 
* @param e0 First half-edge.
* @param e1 Second half-edge.
* @param e2 Third half-edge.
* @return face pointer.
*/
Face* Face::reuse(Hedge *e0, Hedge *e1, Hedge *e2)
{
  e_ = e0;
  e0->set_next(e1); e1->set_next(e2); e2->set_next(e0);
  e0->set_face(this); e1->set_face(this); e2->set_face(this);
  return this;
}
/**
* substitutes two half-edges.
* @param e old half-edge.
* @param ne new half-edge.
* @return  i if e=ei for i=1,2,3 or -1 otherwise.
* 
*/
int  Face::reuse(Hedge *e, Hedge *ne)
{
  Hedge *e0, *e1, *e2;
  e0 = e_;
  e1 = e_->next();
  e2 = e_->next()->next();
  
  if (e0 == e){
    reuse(ne, e1, e2);
    return 0;
  }
  if (e1 == e){
    reuse(e0, ne, e2); 
    return 1;
  }
  if (e2 == e){
    reuse(e0, e1, ne);
    return 2;
  }

  return -1;
}

/**
* Returns the half-edge specified by the index k.
* @param k half-edge index. Valid values of k=[0,1,2].
* @return half-edge pointer.
*/
Hedge* Face::hedge(int k)
{
  switch (k) {
  case 0: return e_;
  case 1: return e_->next();
  case 2: return e_->next()->next();
  }
  throw Error("hedge index");
}
/**
* Returns the opposite vertex of the half-edge specified by index k.
* @param k half-edge index. Valid values of k=[0,1,2].
* @return vertex pointer. 
*/
Vertex* Face::vertex(int k)
{
  return hedge(PREV3(k))->org();
}
/**
* Updates the half-edge of index k.
* @param k half-edge index, Valid values of k=[0,1,2].
* @param h Specify a new half-edge.
*/
void  Face::set_hedge(int k, Hedge* h)
{
  Hedge *n = hedge(NEXT3(k));
  Hedge *p = hedge(PREV3(k));
  h->set_next(n);
  p->set_next(h);
  if (k == 0) e_ = h;
}
/**
* Updates the half-edges refereces to the face
*/
void Face::link_edges()
{
  for (int k = 0; k < 3; k++)
    hedge(k)->set_face(this);
}

