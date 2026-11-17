/*
**     handle.cpp
*
*    Copyright (C) 2006 Luiz Velho, Hélio Lopes, Esdras Medeiros.
*/

#include "mesh.h"

using namespace Mesh;

/** 
* Creates a new face.
* Adds face to  surface.
* Creates one boundary.
* Updates toplogical references.
* @param p0 Point pointer.
* @param p1 Point pointer.
* @param p2 Point pointer.
* @return Face pointer to the new face.
*/
Face* Surf::create(Point *p0, Point *p1, Point *p2)
{
  Vertex *v0 = new Vertex (p0); p0->add_vert(v0);
  Vertex *v1 = new Vertex (p1); p1->add_vert(v1);
  Vertex *v2 = new Vertex (p2); p2->add_vert(v2);
  add_vertex(v0);
  add_vertex(v1);
  add_vertex(v2);

  Edge *e0   = new Edge (v0,v1);
  Edge *e1   = new Edge (v1,v2);
  Edge *e2   = new Edge (v2,v0);
  add_edge(e0);
  add_edge(e1);
  add_edge(e2);

  Hedge *he0 = e0->hedge(0);
  Hedge *he1 = e1->hedge(0);
  Hedge *he2 = e2->hedge(0);
 
  Face *f    = new Face (this,he0,he1,he2);
  add_face(f);

  // update boundary
  add_bdry(e0);
  e0->set_bdry(e0);
  e1->set_bdry(e0);
  e2->set_bdry(e0);
  he0->set_prev_on_bdry(he2);
  he1->set_prev_on_bdry(he0);
  he2->set_prev_on_bdry(he1);
   
  // update stars
  v0->set_star(he2);
  v1->set_star(he0);
  v2->set_star(he1);

  return f;
}

/** 
* Destroys a  face.
* Delete a face to  surface.
* Destroys one boundary.
* Removes subcells .
* @param f Face pointer.
*/
void  Surf::destroy(Face *f)
{
  Hedge *he0 = f->hedge(0);
  Hedge *he1 = f->hedge(1);
  Hedge *he2 = f->hedge(2);

  Vertex *v0 = he0->org();
  Vertex *v1 = he1->org();
  Vertex *v2 = he2->org();

  Edge *e0   = he0->edge();
  Edge *e1   = he1->edge();
  Edge *e2   = he2->edge();
  Edge *eb   = he0->mate()->edge();

  del_vertex(v0);
  del_vertex(v1);
  del_vertex(v2);
  
  v0->p()->del_vert(v0);
  v1->p()->del_vert(v1);
  v2->p()->del_vert(v2);

  delete v0;
  delete v1;
  delete v2;
  
  del_bdry(eb);
  del_edge(e0);
  del_edge(e1);
  del_edge(e2);

  delete e0;
  delete e1;
  delete e2;

  del_face(f);
  delete f;
}

/** 
* Glue two boundary edges.
* This function deals and encapsulates 4 handle operators.
* @param e1 Hedge pointer.
* @param e2 Hedge pointer.
* @return Edge pointer from the first half-edge  parameter.
*/
Edge* Surf::glue(Hedge *e1, Hedge *e2)
{
  Surf* s1 = e1->face()->surf();
  Surf* s2 = e2->face()->surf();
  Vertex* v1 = e1->org();
  Vertex* v2 = e1->dst();
  Vertex* v3 = e2->org();
  Vertex* v4 = e2->dst();
  Edge* b1 = e1->get_bdry();
  Edge* b2 = e2->get_bdry();

  if (s1 != s2)
    return attach_handle_0(e1, e2);
  if (b1 != b2)
    return attach_handle_1(e1, e2);
  if (v1 == v4) {
    if (v2 != v3)
      return attach_handle_3(e1, e2);
    else
      return attach_handle_4(e1, e2);
  }
  if (v2 == v3)
    return attach_handle_3(e1, e2);
  return attach_handle_2(e1, e2);
}

/** 
* Unglue two boundary edges.
* This function deals and encapsulates 4 handle inverse operators.
* @param e1 Hedge pointer.
* @param e2 Hedge pointer.
* @return Edge pointer from the first half-edge parameter.
*/
Edge*  Surf::unglue(Hedge *e1, Hedge *e2)
{
  Vertex* v1 = e1->org();
  Vertex* v2 = e1->dst();
 
  if (!v1->is_bdry() && !v2->is_bdry())
    return detach_handle_4(e1, e2);
  if (v1->is_bdry() && !v2->is_bdry())
    return detach_handle_3(e1, e2);
  if (!v1->is_bdry() && v2->is_bdry())
    return detach_handle_3(e1, e2);
  if (v1->is_bdry() &&  v2->is_bdry()) {
    if (v2->star_first()->mate()->edge() != v1->star_first()->mate()->edge())
      return detach_handle_2(e1, e2);
    else
      return detach_handle_1(e1, e2);
  }
  return NULL;
}

/** 
* Glue two edges from different surfaces.
* Two boundaries are joined into one.
* @param e1 Hedge pointer.
* @param e2 Hedge pointer.
* @return Edge pointer from the first half-edge parameter.
*/
Edge* Surf::attach_handle_0(Hedge *he1, Hedge *he2)
{
  Face   *f2   = he2->face();
  Edge   *e1   = he1->edge();
  Edge   *e2   = he2->edge();
  Hedge  *mhe1 = he1->mate();   
  Hedge  *nhe2 = he2->next();   
  Hedge  *phe2 = he2->prev();   
  Vertex *v1   = he1->org();   
  Vertex *v2   = he1->dst();   
  Vertex *v3   = he2->org();   
  Vertex *v4   = he2->dst();   
  Edge   *eb1  = he1->get_bdry();
  Edge   *eb2  = he2->get_bdry();
  Hedge  *he;

  transfer_cells(he2->face()->surf());

  if (eb1 == e1)
  {
    del_bdry(eb1);
    he = he1->prev_on_bdry();
    eb1 = he->edge();
    do 
    {
      he->edge()->set_bdry(eb1);
      he = he->prev_on_bdry();
    }
    while(he != he1);
    add_bdry(eb1);
  }

  he = he2->prev_on_bdry();
  do 
  {
    he->edge()->set_bdry(eb1);
    he = he->prev_on_bdry();
  }
  while(he != he2);
  del_bdry(eb2);

  he = he2;
  do
  {
    he->next()->set_org(v1);
    he = he->next()->mate();
  }
  while(!(he->next()->edge()->is_bdry()));
  del_vertex(v4);
  delete v4;

  he = he2->prev_on_bdry();
  do
  {
    he->next()->set_org(v2);
    he = he->next()->mate();
  }
  while(!(he->next()->edge()->is_bdry()));
  del_vertex(v3);
  delete v3;

  mhe1->set_edge(e1);
  mhe1->set_face(f2);
  f2->set_hedge(0,mhe1);
  f2->set_hedge(1,nhe2);
  f2->set_hedge(2,phe2);

  del_edge(e2);
  delete e2;

  return e1;
}

/** 
* Glue two edges from different boundarie curves.
* Two boundary curves are joined into one.
* @param e1 Hedge pointer.
* @param e2 Hedge pointer.
* @return Edge pointer from the first half-edge parameter.
*/
Edge* Surf::attach_handle_1(Hedge *he1, Hedge *he2)
{
  Face   *f2   = he2->face();
  Edge   *e1   = he1->edge();
  Edge   *e2   = he2->edge();
  Hedge  *mhe1 = he1->mate();   
  Hedge  *nhe2 = he2->next();   
  Hedge  *phe2 = he2->prev();   
  Vertex *v1   = he1->org();   
  Vertex *v2   = he1->dst();   
  Vertex *v3   = he2->org();   
  Vertex *v4   = he2->dst();   
  Edge   *eb1  = he1->get_bdry();
  Edge   *eb2  = he2->get_bdry();
  Hedge  *he;

  if (eb1 == e1)
  {
    del_bdry(eb1);
    he = he1->prev_on_bdry();
    eb1 = he->edge();
    do 
    {
      he->edge()->set_bdry(eb1);
      he = he->prev_on_bdry();
    }
    while(he != he1);
    add_bdry(eb1);
  }

  he = he2->prev_on_bdry();
  do 
  {
    he->edge()->set_bdry(eb1);
    he = he->prev_on_bdry();
  }
  while(he != he2);
  del_bdry(eb2);

  he = he2->next();
  he->set_org(v1);
  while(!(he->edge()->is_bdry())) 
  {
    he = he->mate()->next();
    he->set_org(v1);
  }
  he->set_prev_on_bdry(v1->star_first());
  del_vertex(v4);
  v4->p()->del_vert(v4);
  delete v4;


  he = he2->prev_on_bdry();
  he->mate()->set_org(v2);
  while(!(he->next()->edge()->is_bdry())){
    he->next()->set_org(v2);
    he = he->next()->mate();
  }
  v2->set_star(v3->star_first()); 
  del_vertex(v3);
  v3->p()->del_vert(v3);
  delete v3;
  
  he = he1->next();
  while(!(he->edge()->is_bdry()))
    he = he->mate()->next();
  he->set_prev_on_bdry(v2->star_first());
  

  mhe1->set_edge(e1);
  mhe1->set_face(f2);
  f2->set_hedge(0,mhe1);
  f2->set_hedge(1,nhe2);
  f2->set_hedge(2,phe2);

  del_edge(e2);
  delete e2;

  return e1;
}

/** 
* Glue two edges from the same boundary curve but with no common vertices.
* One boundary curve is splitted into two.
* @param e1 Hedge pointer.
* @param e2 Hedge pointer.
* @return Edge pointer from the first half-edge parameter.
*/
Edge* Surf::attach_handle_2(Hedge *he1, Hedge *he2)
{
  Face   *f2   = he2->face();
  Edge   *e1   = he1->edge();
  Edge   *e2   = he2->edge();
  Hedge  *mhe1 = he1->mate();   
  Hedge  *nhe2 = he2->next();   
  Hedge  *phe2 = he2->prev();   
  Vertex *v1   = he1->org();   
  Vertex *v2   = he1->dst();   
  Vertex *v3   = he2->org();   
  Vertex *v4   = he2->dst();   
  Edge   *eb   = he1->get_bdry();
  Hedge  *he;

  del_bdry(eb);
  
  he = he2->prev_on_bdry();
  eb = he->edge();
  do 
  {
    he->edge()->set_bdry(eb);
    he = he->prev_on_bdry();
  }
  while(he != he1);
  add_bdry(eb);

  he = he1->prev_on_bdry();  
  eb = he->edge();
  do 
  {
    he->edge()->set_bdry(eb);
    he = he->prev_on_bdry();
  }
  while(he != he2);
  add_bdry(eb);

  he = he2->next();
  he->set_org(v1);
  while(!(he->edge()->is_bdry())) 
  {
    he = he->mate()->next();
    he->set_org(v1);
  }
  he->set_prev_on_bdry(v1->star_first());
  del_vertex(v4);
  v4->p()->del_vert(v4);
  delete v4;


  he = he2->prev_on_bdry();
  he->mate()->set_org(v2);
  while(!(he->next()->edge()->is_bdry())){
    he->next()->set_org(v2);
    he = he->next()->mate();
  }
  v2->set_star(v3->star_first()); 
  del_vertex(v3);
  v3->p()->del_vert(v3);
  delete v3;
  
  he = he1->next();
  while(!(he->edge()->is_bdry()))
    he = he->mate()->next();
  he->set_prev_on_bdry(v2->star_first());

  mhe1->set_edge(e1);
  mhe1->set_face(f2);
  f2->set_hedge(0,mhe1);
  f2->set_hedge(1,nhe2);
  f2->set_hedge(2,phe2);

  del_edge(e2);
  delete e2;

  return e1;
}

/** 
* Glue two edges from the same boundary curve but with one common vertix.
* @param e1 Hedge pointer.
* @param e2 Hedge pointer.
* @return Edge pointer from the first half-edge parameter.
*/
Edge* Surf::attach_handle_3(Hedge *he1, Hedge *he2)
{
  
  Face   *f2   = he2->face();
  Edge   *e1   = he1->edge();
  Edge   *e2   = he2->edge();
  Hedge  *mhe1 = he1->mate();   
  Hedge  *nhe2 = he2->next();   
  Hedge  *phe2 = he2->prev();   
  Vertex *v1   = he1->org();   
  Vertex *v2   = he1->dst();
  Vertex *v3   = he2->org();
  Vertex *v4   = he2->dst();   
  Edge   *eb   = he1->get_bdry();
  Hedge  *he;

  if (he1 == he2->prev_on_bdry()){

    if ((eb == e1) || (eb == e2))
      {
	del_bdry(eb);
	he = he1->prev_on_bdry();
	eb = he->edge();
	do 
	  {
	    he->edge()->set_bdry(eb);
	    he = he->prev_on_bdry();
	  }
	while(he != he2);
	add_bdry(eb);
      }

    he = he2->next();
    he->set_org(v1);
    while(!(he->edge()->is_bdry())) 
      {
	he = he->mate()->next();
	he->set_org(v1);
      }
    he->set_prev_on_bdry(v1->star_first());
    del_vertex(v4);
    v4->p()->del_vert(v4);
    delete v4;

  }
  else {
    if ((eb == e1) || (eb == e2))
      {
	del_bdry(eb);
	he = he1->prev_on_bdry()->prev_on_bdry();
	eb = he->edge();
	do 
	  {
	    he->edge()->set_bdry(eb);
	    he = he->prev_on_bdry();
	  }
	while(he != he1);
	add_bdry(eb);
      }
    
    v2->set_star(v3->star_first());
    v1->set_star(mhe1);
    he = he2->prev();
    he->mate()->set_org(v2);
    while(!(he->edge()->is_bdry())) 
      {
	he = he->mate()->prev();
	he->mate()->set_org(v2);
      } 
    
    del_vertex(v3);
     v3->p()->del_vert(v3);
    delete v3;

    he = he1->next();
    while(!(he->edge()->is_bdry()))
      he = he->mate()->next();
    he->set_prev_on_bdry(v2->star_first());   
  }

  mhe1->set_edge(e1);
  mhe1->set_face(f2);
  f2->set_hedge(0,mhe1);
  f2->set_hedge(1,nhe2);
  f2->set_hedge(2,phe2);

  del_edge(e2);
  delete e2;

  return e1;
}

/** 
* Glue two edges from the same boundary curve but with two common vertices.
* One boundary curve destroyed.
* @param e1 Hedge pointer.
* @param e2 Hedge pointer.
* @return Edge pointer from the first half-edge parameter.
*/
Edge* Surf::attach_handle_4(Hedge *he1, Hedge *he2)
{
  Face  *f2   = he2->face();
  Edge  *e1   = he1->edge();
  Edge  *e2   = he2->edge();
  Hedge *mhe1 = he1->mate();   
  Hedge *nhe2 = he2->next();   
  Hedge *phe2 = he2->prev();   
  Vertex *v1   = he1->org();

  del_bdry(he1->get_bdry());

  mhe1->set_edge(e1);
  mhe1->set_face(f2);
  f2->set_hedge(0,mhe1);
  f2->set_hedge(1,nhe2);
  f2->set_hedge(2,phe2);

  v1->set_star(mhe1);
  del_edge(e2);
  delete e2;

  return e1;
}

/*detach handles*/

/** 
* Unglue two internal edges.
* One boundary curve is splitted into two.
* @param e1 Hedge pointer.
* @param e2 Hedge pointer.
* @return Edge pointer from the first half-edge parameter.
*/
Edge* Surf::detach_handle_1(Hedge *he1, Hedge *he2)
{
  Hedge  *he, *he2_old;
  Face   *f2   = he2->face();
  Vertex *v1   = he1->org();
  Vertex *v2   = he1->dst();
  Edge   *eb1  = v1->star_first()->get_bdry();
  Edge   *eb2;
  Vertex *v3   = new Vertex(v2->p()); 
  Vertex *v4   = new Vertex(v1->p());
  v3->p()->add_vert(v3);
  v4->p()->add_vert(v4);
  add_vertex(v3);
  add_vertex(v4);

  Edge *ne = new Edge(v3, v4);
  he2_old = he2;
  he2 = ne->hedge(0);
  eb2 = ne;
  add_edge(ne);
  
  /*update hedge of a face*/
  if (f2->reuse(he2_old, he2)<0) return NULL;
  he2_old->set_face(NULL);

  /*update bdries*/
  ne->set_bdry(eb2);
  he1->edge()->set_bdry(eb1);

  add_bdry(eb2);

  he = v2->star_first();
  do 
    {
      he->edge()->set_bdry(eb2);
      he = he->prev_on_bdry();
    }
  while(he != v1->star_first());
  
  
  /*update prev on bdry, orgs*/
  he1->set_prev_on_bdry(v1->star_first());

  he = he1->next();
  while(!(he->edge()->is_bdry()))
    he = he->mate()->next();
  he->set_prev_on_bdry(he1);   

  he = he2->prev();
  while(!(he->edge()->is_bdry())){
    he->mate()->set_org(v3);
    he = he->mate()->prev();
  } 
  he->mate()->set_org(v3);
  he2->set_prev_on_bdry(he);
  v3->set_star(he);  

  he = he2->next();
  he->set_org(v4);
  while(!(he->edge()->is_bdry())){
    he = he->mate()->next();
    he->set_org(v4);
  }
  he->set_prev_on_bdry(he2); 
  
  /*update star*/
  v2->set_star(he1);
  v4->set_star(he2);

  return ne;
}

/** 
* Unglue two internal edges.
* Two boundary curves are joined into one.
* @param e1 Hedge pointer.
* @param e2 Hedge pointer.
* @return Edge pointer from the first half-edge parameter.
*/
Edge* Surf::detach_handle_2(Hedge *he1, Hedge *he2)
{
  Hedge  *he, *he2_old;
  Face   *f2   = he2->face();
  Vertex *v1   = he1->org();
  Vertex *v2   = he1->dst();
  Edge   *eb1  = v1->star_first()->get_bdry();
  Edge   *eb2  = v2->star_first()->get_bdry();
  Vertex *v3   = new Vertex(v2->p()); 
  Vertex *v4   = new Vertex(v1->p());
  v3->p()->add_vert(v3);
  v4->p()->add_vert(v4);
  add_vertex(v3);
  add_vertex(v4);

  Edge *ne = new Edge(v3, v4);
  he2_old = he2;
  he2 = ne->hedge(0);
  add_edge(ne);
  
  /*update hedge of a face*/
  if (f2->reuse(he2_old, he2)<0) return NULL;
  he2_old->set_face(NULL);

  /*update bdries*/
  ne->set_bdry(eb1);
  he1->edge()->set_bdry(eb1);

  del_bdry(eb2);
  he = v2->star_first();
  do 
    {
      he->edge()->set_bdry(eb1);
      he = he->prev_on_bdry();
    }
  while(he != v2->star_first());
  
  
  /*update prev on bdry, orgs*/
  he1->set_prev_on_bdry(v1->star_first());

  he = he1->next();
  while(!(he->edge()->is_bdry()))
    he = he->mate()->next();
  he->set_prev_on_bdry(he1);   

  he = he2->prev();
  while(!(he->edge()->is_bdry())){
    he->mate()->set_org(v3);
    he = he->mate()->prev();
  }
  he->mate()->set_org(v3);
  he2->set_prev_on_bdry(he);
  v3->set_star(he);  

  he = he2->next();
  he->set_org(v4);
  while(!(he->edge()->is_bdry())){
    he = he->mate()->next();
    he->set_org(v4);
  }
  he->set_prev_on_bdry(he2); 
  
  /*update star*/
  v2->set_star(he1);
  v4->set_star(he2);

  return ne;
}

/** 
* Unglue two internal edges.
* @param e1 Hedge pointer.
* @param e2 Hedge pointer.
* @return Edge pointer from the first half-edge parameter.
*/
Edge* Surf::detach_handle_3(Hedge *he1, Hedge *he2)
{  
  if(he1->org()->is_bdry()){
    Hedge *temp =  he1;
    he1 = he2;
    he2 = temp;
  }
  Hedge *he2_old, *he;
  Face   *f2   = he2->face();
  Vertex *v1   = he1->org();
  Vertex *v2   = he1->dst();
  Edge   *eb   = v2->star_first()->get_bdry();
  Vertex *v3 = new Vertex(v2->p()); 
  v3->p()->add_vert(v3);
  add_vertex(v3);

  Edge *ne = new Edge(v3, v1);
  he2_old = he2;
  he2 = ne->hedge(0);
  add_edge(ne);
  
  /*update hedge of a face*/
  if (f2->reuse(he2_old, he2)<0) return NULL;
  he2_old->set_face(NULL);

  /*update bdries, orgs*/
  ne->set_bdry(eb);
  he1->edge()->set_bdry(eb);

  he = he2->prev();
  while(!(he->edge()->is_bdry())){
    he->mate()->set_org(v3);
    he = he->mate()->prev();
  }
  he->mate()->set_org(v3);
  he2->set_prev_on_bdry(he);
  v3->set_star(he);  
  
  /*update prev on bdry*/
  he1->set_prev_on_bdry(he2);
  he = he1->next();
  while(!(he->edge()->is_bdry()))
    he = he->mate()->next();
  he->set_prev_on_bdry(he1);   

  /*update stars*/
  v1->set_star(he2);
  v2->set_star(he1);
  return ne;
}

/** 
* Unglue two internal edges.
* A new boundary curve created.
* @param e1 Hedge pointer.
* @param e2 Hedge pointer.
* @return Edge pointer from the first half-edge parameter.
*/
Edge* Surf::detach_handle_4(Hedge *he1, Hedge *he2)
{
  Hedge  *he2_old;
  Face   *f2   = he2->face();
  Vertex *v1   = he1->org();
  Vertex *v2   = he1->dst();

  Edge *ne = new Edge(v2, v1); 
  he2_old = he2;
  he2 = ne->hedge(0);
  add_edge(ne);
  add_bdry(ne);
  
  /*update hedge of a face*/
  if (f2->reuse(he2_old, he2)<0) return NULL;
  he2_old->set_face(NULL);

  /*update bdries*/
  ne->set_bdry(ne);
  he1->edge()->set_bdry(ne);
  
  /*update prev on bdry*/
  he1->set_prev_on_bdry(he2);
  he2->set_prev_on_bdry(he1);
  
  /*update star*/
  v1->set_star(he2);
  v2->set_star(he1);

  return ne;
}
