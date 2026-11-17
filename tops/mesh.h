/** 
* @file mesh.h
* @brief Definition of Obj, Surface, Face, Edge, Hedge, Vertex and Point classes.
*/

/* Copyright (C) 2006 Luiz Velho, Hélio Lopes and Esdras Medeiros*/

#ifndef MESH_H
#define MESH_H

#include <cstdio>
#include <iostream>
#include <list>
#include <set>
#include <iterator>
#include <utility>
#include "../algebra/algebra3.h"

using namespace std;
/**
* @namespace Mesh
* @brief 
*/
namespace Mesh {

typedef double Real;
class Point;

class Obj;
class Surf;
class Face;
class Edge;
class Hedge;
class Vertex;

class Error;

/**
* @typedef std::set<Surf*>  SurfContainer (feel free changing container type)
* @brief Use the class set to store the surfaces of mesh.
*/ 
typedef std::set<Surf*>   SurfContainer; 

/**
* @typedef std::set<Face*>  FaceContainer (feel free changing container type)
* @brief Use the class set to store the faces of mesh.
*/ 
typedef std::set<Face*>   FaceContainer;

/**
* @typedef std::set<Edge*>  EdgeContainer (feel free changing container type)
* @brief Use the class set to store the edge of mesh.
*/ 
typedef std::set<Edge*>   EdgeContainer;

/**
* @typedef std::set<Vertex*>  VertexContainer (feel free changing container type)
* @brief Use the class set to store the vertices of mesh.
*/
typedef std::set<Vertex*> VertexContainer;

/**
* @typedef SurfContainer::iterator SurfIter (feel free changing container type)
* @brief Define a iterator to access the surfaces of mesh.
*/
typedef SurfContainer::iterator SurfIter;

/**
* @typedef FaceContainer::iterator FaceIter (feel free changing container type)
* @brief Define a iterator to access the faces of mesh.
*/
typedef FaceContainer::iterator FaceIter;

/**
* @typedef EdgeContainer::iterator EdgeIter (feel free changing container type)
* @brief Define a iterator to access the edge of mesh.
*/
typedef EdgeContainer::iterator EdgeIter;

/**
* @typedef VertexContainer::iterator VertexIter (feel free changing container type)
* @brief Define a iterator to access the vertices of mesh.
*/
typedef VertexContainer::iterator VertexIter;

/**
* @class Obj
* @brief The Obj class is an object with a set of surfaces.
*/

class Obj {
 private:
  SurfContainer sc_; ///< Set of surfaces

 public:
  /** @brief The construtor specifies a Obj class.*/
  Obj() {};

  /** @brief The Obj class destructor.
  *   Don't forget to delete the surface afterwards.
  */
  ~Obj() {};

  /** @brief Returns iterator that points to the first surface of object.
    * @return surface iterator.*/
  SurfIter surfs_begin() { return sc_.begin(); };

  /** @brief Returns iterator that points to the last surface of the mesh.
    * @return surface iterator.*/
  SurfIter surfs_end() { return sc_.end(); };

  /** @brief Adds a surface.
    * @param s surface pointer.
    * @return true if add operation was completed, else return false.*/
  bool add_surf(Surf *s) { std::pair<SurfIter, bool> r = sc_.insert(s); return r.second; }

  /** @brief Removes a surface from Obj.
    * @param s surface pointer.*/
  void rm_surf(Surf *s) { sc_.erase(s); };
};

/**
* @class Surf
* @brief The class Surf defines data structures Toplogical Operators to represent a surface.
*/
class Surf {
 private:
  VertexContainer vc_; ///< Set of vertices
  EdgeContainer   ec_; ///< Set of edges
  FaceContainer   fc_; ///< Set of faces
  EdgeContainer   bc_; ///< Set of Boundary Curve Representatives
  

 public:
  
  /** @brief The construtor specifies a Surf class.*/
   Surf(){};

  /** @brief The mesh class destructor. Destroys the surface cells.*/
   ~Surf();

  /** @brief Returns iterator that points to the first vertex of the surface.
    * @return Vertex iterator.*/
  VertexIter verts_begin(){ return vc_.begin(); };

  /** @brief Returns  iterador that points to the last vertex of the surface.
    * @return Vertex iterator.*/
  VertexIter verts_end(){ return vc_.end(); };

  /** @brief Returns the number of vertices.
    * @return Number of vertices.*/
  int num_verts() { return vc_.size(); };

  /** @brief Returns iterator that points to the first edge of the surface.
    * @return Edge iterator.*/
  EdgeIter edges_begin() { return ec_.begin(); };

  /** @brief Returns iterador that points to the last edge of the surface.
    * @return Edge iterator.*/
  EdgeIter edges_end() { return ec_.end(); };

  /** @brief Returns the number of egdes.
    * @return Number of edges.*/
  int num_edges() { return ec_.size(); };

  /** @brief Returns iterator that points to the first face of the surface.
    * @return Face iterator.*/
  FaceIter faces_begin() { return fc_.begin(); };

  /** @brief Returns iterador that points to the last vertex of the surface.
    * @return Face iterator.*/
  FaceIter faces_end() { return fc_.end(); };

  /** @brief Returns the number of faces.
    * @return Number of faces.*/
  int num_faces() { return fc_.size(); };

  /** @brief Returns iterator that points to the first boundary of the surface.
    * @return Edge iterator.*/
  EdgeIter bdries_begin() { return bc_.begin(); };

  /** @brief Returns iterador that points to the last boundary of the surface.
    * @return Edge iterator.*/
  EdgeIter bdries_end() { return bc_.end(); };

  /** @brief Returns the number of boundaries.
    * @return Number of boundaries.*/
  int num_bdries() { return bc_.size(); };

  /** @brief Look for a hedge in surface.*/
  Hedge* lookup_hedge(Point *p0, Point *p1);
  
  /** @brief Divides a face.*/
  Vertex* split(Face *f);

  /** @brief Splits an edge.*/
  Vertex* split(Hedge* e);

  /** @brief Removes a vertex.
  *   Supported for boudary and internal edges.
  */
  Hedge* weld(Vertex* w);

  /** @brief Flips internal edge.*/
  Hedge* flip(Hedge *h);

  /** @brief Creates a new face.*/
  Face* create(Point *p0, Point *p1, Point *p2);

  /** @brief Destroys a face.*/
  void destroy(Face *f);

  /** @brief Glue two half edges.*/
  Edge* glue(Hedge *e1, Hedge *e2);

  /** @brief Unglue two half-edges.*/
  Edge* unglue(Hedge *e1, Hedge *e2);

 private:
  /** @brief Creates a new vertex.*/
  Vertex *add_new_vertex(Point *p);

  /** @brief Adds an existing vertex.
    * @param v Vertex pointer.
    * @return Return true if add operation was completed, else return false.*/
  bool add_vertex(Vertex *v) 
  { std::pair<VertexIter, bool> r = vc_.insert(v);return r.second;};

  /** @brief Removes a vertex from surface.
    * @param v Vertex pointer.*/
  void del_vertex(Vertex* v) { vc_.erase(v);};

  /** @brief Adds a new edge.*/
  Hedge *add_new_edge(Vertex *v0, Vertex *v1);

  /** @brief Add an existing edge.
    * @param e Edge pointer. 
    * @return true if add operation was completed, else return false.*/
  bool add_edge(Edge *e) 
  { std::pair<EdgeIter, bool> r = ec_.insert(e); return r.second;};

  /** @brief Removes an edge from surface.
    * @param e Edge pointer.*/
  void del_edge(Edge* e) { ec_.erase(e);};

  /** @brief Adds an existing face.
    * @param f Face pointer.
    * @return true if add operation was completed, else return false.*/
  bool add_face(Face* f) { std::pair<FaceIter, bool> r = fc_.insert(f); return r.second;};

  /** @brief Removes a face from surface.*/
  void del_face(Face* f) { fc_.erase(f);};

  /** @brief Add an existing boundary.
    * @param e Edge pointer. 
    * @return true if add operation was completed, else return false.*/
  bool add_bdry(Edge* e) { std::pair<EdgeIter, bool> r = bc_.insert(e); return r.second;};

  /** @brief Removes an boundary from surface.
    * @param e Edge pointer.*/
  void del_bdry(Edge* e) { bc_.erase(e);};

  /**
   * @brief Adds cells from other surface. 
   * It doesn't perform any topological change.
   * @param s2 surface origin.
   */
  void transfer_cells(Surf* s2);

  /** @brief Splits an edge in two sub-edges.*/
  Vertex* bisect(Hedge* e, Hedge** el, Hedge** er);

  /** @brief Splits a face in two sub-faces.*/
  Hedge* bisect(Face *f, Hedge *e1, Hedge *e2, Hedge *el, Hedge *er);

  /** @brief Glue two edges from different surfaces.*/
  Edge* attach_handle_0(Hedge *e1, Hedge *e2);

  /** @brief Glue two edges from different boundary curves.*/
  Edge* attach_handle_1(Hedge *e1, Hedge *e2);

  /** @brief Glue two edges from the same boundary curve but no commom vertices.*/
  Edge* attach_handle_2(Hedge *e1, Hedge *e2);

  /** @brief Glue two edges from the same boundary curve but one commom vertex.*/
  Edge* attach_handle_3(Hedge *e1, Hedge *e2);

  /** @brief Glue two edges from the same boundary curve but two commom verteces.*/
  Edge* attach_handle_4(Hedge *e1, Hedge *e2);

  /** @brief Unglue two edges and split one boundary curve into two.*/
  Edge* detach_handle_1(Hedge *e1, Hedge *e2);

  /** @brief Unglue two edges and join two boundary curves.*/
  Edge* detach_handle_2(Hedge *e1, Hedge *e2);

  /** @brief Unglue two edges in the same boundary curve.*/
  Edge* detach_handle_3(Hedge *e1, Hedge *e2);

  /** @brief Unglue two edges and creates a new boundary curve.*/
  Edge* detach_handle_4(Hedge *e1, Hedge *e2);
  
};

/**
* @class Face
* @brief The class Face implements the face topological entity of Tops.
*/
class Face {
 private:
  Hedge    *e_; ///< Points to the first half-egde of face's loop.
  Surf     *s_; ///< Points to surface it belongs.

 public:

 /** @brief The constructor creates a triangular face.
   * @param s  Surface pointer
   * @param e0 First half-edge.
   * @param e1 Second half-edge.
   * @param e2 Third half-edge.*/
  Face(Surf *s, Hedge* e0, Hedge* e1, Hedge* e2);

  /** @brief The Obj class destructor.*/
  ~Face() {};

  /** @brief Returns the surface pointer.
   * @return surface pointer.*/
  Surf* surf() { return s_;};

  /** @brief Returns the half-edge of index k.
   * @return half-edge pointer.*/
  Hedge* hedge(int k);

  /** @brief Returns the vertex of index k.
   * @return vertex pointer.*/
  Vertex* vertex(int k);

  /** @brief Updates the half-edge of index k.*/
  void set_hedge(int k, Hedge* h);

  /** @brief Updates the vertex of index k.*/
  void set_surf(Surf* s) {s_ = s; };

  /*@brief Updates the half-edges references to the face.*/
  void link_edges();

  /** @brief Redefines a face by specifing replacement half-edges.*/
  Face* reuse(Hedge *e0, Hedge *e1, Hedge *e2);

  /**substitutes two half-edges.*/
  int reuse(Hedge *he, Hedge *ne);
};



/**
* @class Hedge
* @brief The Hedge class represents Half-edge topological entity of surface.
*  
*  OBS: for boundary hedges h
*
*  h.mate.face == NULL
*
*  h.mate.edge: points to the edge representative of the boundary curve
*
*  h.mate.next: points to the previous half-edge on the boundary
*/
class Hedge {
 private:
  Vertex    *o_; ///< Origin vertex pointer.
  Hedge     *n_; ///< Next half-edge pointer.
  Face      *f_; ///< Face pointer.
  Edge      *e_; ///< Edge pointer.

 public:
  /** @brief The construtor specifies a Hedge class.*/
  Hedge() {};
  /** @brief The Hedge class destructor.*/
  ~Hedge() {};

  /** @brief Returns the half-egde's face.
    * @return face pointer.*/
  Face* face() { return f_; };
  
  /** @brief Returns the half-edge's origin vertex.
    * @return vertex pointer.*/
  Vertex* org() { return o_; };

  /** @brief Returns the end vertex of the half-edge.
    * @return vertex pointer.*/
  Vertex* dst() { return mate()->org(); };

  /** @brief Returns the half-edges's parent edge.
    * @return edge pointer.*/
  Edge*  edge() { return e_; };

  /** @brief Returns the opposite half-edge.
    * @return half-edge pointer.*/
  Hedge* mate();

  /** @brief Returns the previous half-edge.
    * @return half-edge pointer.*/
  Hedge* prev() { return (n_)? n_->next() : NULL; };

  /** @brief Returns the next half-edge.
    * @return half-edge pointer.*/
  Hedge* next() { return n_; };

  /** @brief Returns the previous half-edge in boundary.
    * @return half-edge pointer.*/
  Hedge* prev_on_bdry() { return mate()->next(); };

  /** @brief Returns the boundary it belongs.
    * @return edge pointer.*/
  Edge* get_bdry() { return (mate()->face()==NULL)? mate()->edge() : NULL; };

  /** @brief Links half-edge to face.*/
  void set_face(Face* f) { f_ = f; };

  /** @brief Links half-edge to vertex.*/
  void set_org(Vertex* v) { o_ = v; };

  /** @brief Updates next half-edge.*/
  void set_next(Hedge* h) { n_ = h; };

  /** @brief Links half-edge to edge.*/
  void set_edge(Edge* e) { e_ = e; };

  /** @brief Updates previous half-edge in boundary.*/
  void set_prev_on_bdry(Hedge *e) { if (mate()->face()==NULL) mate()->set_next(e); };

  /** @brief Redefines the vertices of the half-edge.*/
  Hedge* reuse(Vertex *v0, Vertex *v1);
};

/**
 * @class Edge
 * @brief The Edge class represents the edge topological entity of the surface.
*/
class Edge  {
 private:
  Hedge h_[2]; ///< Two half-edges. One for each face use.

 public:
  /* @brief The construtctor initializes the vertices of the edge.*/
  Edge(Vertex *p0, Vertex *p1);

  /** @brief The Edge class destructor.*/
  ~Edge() {};

  /* @brief Returns a half-edge specified by the index i.*/
  Hedge* hedge(int i);
 
  /** @brief Returns the origin vertex.*/
  Vertex* org() { return h_[0].org(); };

  /** @brief Returns the end vertex.*/
  Vertex* dst() { return h_[1].org(); };

  /** @brief Returns true if the edge belongs to mesh border, else return false.
    * @return boolean type (true or false).*/
  bool is_bdry() { return (h_[0].face() == NULL || h_[1].face() == NULL); };


  /** @brief Updates the boundary pointer if it belongs another boundary of the surface.
   *  @param b edge pointer.
  */
  void set_bdry(Edge *b) { 
    if (h_[0].face() != NULL) h_[1].set_edge(b);
    else if (h_[1].face() != NULL) h_[0].set_edge(b);
  };
};

/**
* @class Vertex
* @brief The Vertex class represents the vertex entity,
* This class stores  a pointer to a Point which contains geometry and attribute information.
*/
class Vertex  {
 private: 
  Hedge*      s_; ///< vertex incoming half-edge (star handle).
  Point*      p_; ///< geometry and attribute information.

 public:

  /** @brief The constructor initializates the class variables to null values.*/
  Vertex(Point *p) {p_ = p; s_ = NULL;};

  /** @brief The Edge class destructor.*/
  ~Vertex() {};

  /** @brief Returns the geometry and attribute information.*/
  Point*  p() { return p_; }; 

  /** @brief Initializes the vertex half-edge.
    * @param h Half-edge pointer.*/
  void set_star(Hedge* h) { s_ = h; }; 

  /** @brief Returns the half-edge of the vertex.
    * @return half-edge pointer.*/
  Hedge*  star_first() { return s_; }; 

  /** @brief Returns pointer to next half-edge in face loop.
    * @param h Half-edge pointer.
    * @return Return half-edge pointer.*/
  Hedge*  star_next(Hedge* h) {
    if (h->face() ==NULL ) return NULL; // other side of bdry
    else { Hedge *n = h->next()->mate(); return (n == s_)? NULL : n; }
  }

  /** @brief Returns true if the vertex belongs to mesh boundary, else return false
    * @return boolean type (true or false).*/
  bool    is_bdry() { return s_->edge()->is_bdry(); };
};

/**
* @class Point
* @brief The Point class contains geometry and attribute information.
*/
class Point{
private:
  vec3 _p; ///< Position vector
  vec3 _n; ///< Normal vector
  list<Vertex*> _vl; ///< Set of vertices which reference Point
  int _index; ///< Label information

 public:

  /** @brief Point Class default constructor. Nothing to do.*/
	 Point(){_p=vec3(0, 0, 0);_index = 0;; _n=vec3(0, 0, 0);};

  /** @brief This constructor initializates the class variables.*/
  Point(vec3 p, vec3 n, int index) { _p=p; _index=index; _n=n;};

  /** @brief Point Class default destructor. Nothing to do.*/
  ~Point();

  /** @brief Returns position coordinate.
  *   @param i=[0, 1, 2].
  */
  float operator [] (int i) {
    if (i < 0 || i > 2)
      return 0.0;
    return _p[i];
  };
  
  /** @brief Returns position vector.*/
  vec3 p() {return _p;};

  /** @brief Returns normal vector.*/
  vec3 n() {return _n;};

  /** @brief Returns label information.*/
  int  get_index(){return _index;};

  /** @brief Adds a vector to the position vector. 
   *  @param v vec3 object.
  */
  void add(vec3 v){_p =_p+v;};

  /** @brief Updates the normal vector. 
   *  @param nx first coordinate.
   *  @param ny second coordinate.
   *  @param nz third coordinate.
  */
  void set_normal(float nx, float ny, float nz){_n[0]=nx; _n[1]=ny; _n[2]=nz;};

  /** @brief Looks for a hedge which connects a point 
   *  @param p origin point.
   *  @return Hedge pointer if found and NULL otherwise.
  */
  
  Hedge* lookup_hedge(Point *p){
   Hedge* he = NULL;
   for (list<Vertex*>::iterator viter = _vl.begin(); viter!=_vl.end();viter++)
     if ((*viter)->star_first()->org()->p()==p){
	 he = (*viter)->star_first();
	 break;
     }
    return he;
  };

  /** @brief Returns true if the point belongs to mesh border, else return false.
    * @return boolean type (true or false).*/
  bool on_bdry() {return (_vl.size()>0)? 
	  (*_vl.begin())->star_first()->edge()->is_bdry(): false;};

  /** @brief Inserts a vertex pointer to the references list.
   *  @param v Vertex pointer.
  */
  void add_vert(Vertex *v) {_vl.push_back(v);};

  /** @brief Removes a vertex pointer to the references list.
   *  @param v Vertex pointer.
  */
  void del_vert(Vertex *v) {_vl.remove(v);};

};

/**
* @class Error
* @brief The class Error is used to handle errors of Tops.
*/
class Error {
 public:
  /** @brief The constructor allows specify a textual error.*/
  Error(char *s=""){ std::cerr<<"mesh error: "<<s<<std::endl; exit(0); };
};
}


#endif
