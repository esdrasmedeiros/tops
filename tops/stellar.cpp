/*:
**     stellar.cpp - stellar operations
**
**   Copyright (C) 2004 Luiz Velho, Hélio Lopes, Esdras Medeiros.
*/

#include "mesh.h"

using namespace Mesh;

/**
* Splits a half-edge into two sub-half-edges.
* @param e Half-edge that must be splitted.
* @return new vertex pointer.
*/
Vertex* Surf::split(Hedge *e)
{
  Hedge *el, *er;
  Face *f = e->face();
  if (f == NULL) throw Error("subdiv edge");
  Face *fm = e->mate()->face();
  Hedge *ef1 = e->next();
  Hedge *ef2 = e->prev();
  Hedge *efm1 = (fm)? e->mate()->next() : NULL;
  Hedge *efm2 = (fm)? e->mate()->prev() : NULL;
  Vertex *v = bisect(e, &el, &er);
  bisect(f, ef1, ef2, e, er);
  bisect(fm, efm1, efm2, er->mate(), e->mate());
  v->set_star(e);
  return v;
}
/**
* Bisects a half-edge into two new sub half-edges. 
* The old half-edge is divided by inserting a new vertex.
* @param e Half-edge pointer.
* @param el Half-edge from vertex v0 to vertex m.
* @param er Half-edge from vertex m to vertex v1.
* @return Return a new vertex pointer (m).
*/
Vertex* Surf::bisect(Hedge* e, Hedge** el, Hedge** er)
{
  Edge *bd = NULL; Hedge *epb= NULL;
  Vertex *v0 = e->org();
  Vertex *v1 = e->dst();
  Vertex *m = add_new_vertex(new Point());
  if (e->edge()->is_bdry()) {
    bd = e->get_bdry();
    epb = e->prev_on_bdry();
  }
  *el = e->reuse(v0, m);
  *er = add_new_edge(m, v1);
  if (v1->star_first() == e)
    v1->set_star((*er));
  if (bd != NULL) {
    (*el)->edge()->set_bdry(bd);
    (*er)->edge()->set_bdry(bd);
    (*el)->set_prev_on_bdry(*er); 
    (*er)->set_prev_on_bdry(epb); 
  }
  return m;
}

/**
* Splits a face into two new faces.
* The face is splitted through vertex m.
* @param f face that must be splitted.
* @param e1 next half-edge of splitted half-edge.
* @param e2 previous half-edge of splitted half-edge.
* @param el Half-edge from vertex v0 to vertex m.
* @param er Half-edge from vertex m to vertex v1.
* @return Return a half-edge pointer that splitted the face.
*/
Hedge* Surf::bisect(Face *f, Hedge *e1, Hedge *e2, Hedge *el, Hedge *er)
{
  if (f == NULL) return 0;
  Hedge *em = add_new_edge(e2->org(), er->org());
  f->reuse(e1, em, er);
  add_face(new Face(this, e2, el, em->mate()));
  return em;
}

/**
* Removes a vertex from mesh. 
* This operation welds two half-edges into one half-edge.
* @param w Points to a vertex that must be removed from mesh.
* @return new half-edge pointer. 
*/
Hedge* Surf::weld(Vertex *w)
{
  int k, n;
  Hedge *ee, *e[6]; Face *f[6]; Vertex *v[6]; 
  for (n=0, ee=w->star_first(); ee != NULL; n++, ee=w->star_next(ee)) {
    if (n > 4) throw Error("weld");
    e[n] = ee;
    v[n] = ee->org();
    f[n] = ee->face();
  }
  if (n != 4 && n != 3) 
    { std::cerr << "can't weld " << n << "\n"; return NULL; }
  Hedge *p0 = e[0]->prev();
  Hedge *n2 = e[2]->mate()->next();
  Hedge *n0, *p2;
  if (f[2] != NULL) {
    n0 = e[0]->mate()->next();
    p2 = e[2]->prev();
  }
  Hedge *en = e[0]; e[0]->reuse(v[0], v[2]);

  if (v[2]->star_first() == e[2]->mate())
    v[2]->set_star(en);
  if (v[1]->star_first() == e[1]->mate())
    v[1]->set_star(n2);
  if (n == 4 && (v[3]->star_first() == e[3]->mate()))
    v[3]->set_star(n0);

  f[0]->reuse(en, n2, p0);
  if (f[2] != NULL)
    f[1]->reuse(en->mate(), n0, p2);
  else
    del_face(f[1]);

  for (k = 2; k < n; k++) 
    if (f[k] != NULL)
      del_face(f[k]);
  for (k = 1; k < n; k++)
    del_edge(e[k]->edge());
  del_vertex(w);
  return en;
}

/**
* Divides a face into three new faces. 
* The new vertex is inserted inside the face and new faces are built.
* @param f Points to face that must be subdivided.
* @return new vertex pointer (inside vertex).
*/
Vertex* Surf::split(Face *f)
{
  Hedge *e0 = f->hedge(0);
  Hedge *e1 = f->hedge(1);
  Hedge *e2 = f->hedge(2);
  Vertex *v0 = f->vertex(0);
  Vertex *v1 = f->vertex(1);
  Vertex *v2 = f->vertex(2);
  Vertex *vc = add_new_vertex(new Point());
  Hedge *e0c = add_new_edge(v0, vc);
  Hedge *e1c = add_new_edge(v1, vc);
  Hedge *e2c = add_new_edge(v2, vc);
  f->reuse(e0, e2c, e1c->mate());
  add_face(new Face(this, e1, e0c, e2c->mate()));
  add_face(new Face(this, e2, e1c, e0c->mate()));
  vc->set_star(e0c);
  return vc;
}

/** 
* Changes the connectivity of an edge shared by two faces.
* @param h Points to half-edge.
* @return new half-edge pointer.
*/
Hedge* Surf::flip(Hedge *h)
{
  Hedge *m = h->mate();
  Face *fl = h->face();
  Face *fr = m->face();
  if (fl == NULL || fr == NULL)
    return h;
  Hedge *hp = h->prev(); 
  Hedge *hn = h->next();
  Hedge *mp = m->prev();
  Hedge *mn = m->next();
  Vertex *v0 = h->org();
  Vertex *v1 = h->dst();
  Vertex *vl = hp->org();
  Vertex *vr = mp->org();
  if (v0->star_first() == m)
    v0->set_star(hp);
  if (v1->star_first() == h)
    v1->set_star(mp);
  Hedge *o = h->reuse(vl, vr);
  fr->reuse(o, mp, hn);
  fl->reuse(o->mate(), hp, mn);
  return o;
}

