

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include "../include/mesh.h"
#include "../include/algebra3.h"
#include "../include/rply.h"
#include <GL/glut.h> 

using namespace std;
using namespace Mesh;

/*---------------- Global Variables ----------------------*/

Surf *surf[2];
vector<Point*> pt_array[2];
float x, y, z;
int pt_index;
int focus;
int p0, p1, p2;

/*--------------------------------------------------------*/

static int vertex_cbx(p_ply_argument argument)
{
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  x=ply_get_argument_value(argument);
  return 1;
}

////////////////////////////////////////////////////////////////////////////

static int vertex_cby(p_ply_argument argument)
{
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  y=ply_get_argument_value(argument);
  return 1;
}

///////////////////////////////////////////////////////////////////////////

static int vertex_cbz(p_ply_argument argument)
{
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  z=ply_get_argument_value(argument);
  pt_array[focus].push_back(new Point(vec3(x, y, z), vec3(0, 0, 0), pt_index));
  pt_index++;
  return 1;
}

///////////////////////////////////////////////////////////////////////////

static int face_cb(p_ply_argument argument) 
{
  long length, value_index;
  
  ply_get_argument_property(argument, NULL, &length, &value_index);
  
  switch (value_index) {
  case 0: 
    p0 = (long)ply_get_argument_value(argument);
    break;
  case 1: 
    p1 = (long)ply_get_argument_value(argument);
    break;
  case 2:
    p2 = (long)ply_get_argument_value(argument);
    
    Hedge* mhe[3], *he[3]; 
    Face *f;
    bool is_manifold;
    int j;

    mhe[0] = pt_array[focus][p0]->lookup_hedge(pt_array[focus][p1]);
    mhe[1] = pt_array[focus][p1]->lookup_hedge(pt_array[focus][p2]);
    mhe[2] = pt_array[focus][p2]->lookup_hedge(pt_array[focus][p0]);
      
    is_manifold = true;
    for(j =0;j<3; j++)
      if (mhe[j]!=NULL && !mhe[j]->edge()->is_bdry())
	is_manifold = false;
    
    if(is_manifold){
      
      f = surf[focus]->create(pt_array[focus][p0], 
			      pt_array[focus][p1], 
			      pt_array[focus][p2]); 
      he[0]=f->hedge(0);
      he[1]=f->hedge(1);
      he[2]=f->hedge(2);
      
      if(mhe[0]!=NULL)
	surf[focus]->glue(mhe[0], he[0]);
      
      if(mhe[1]!=NULL)
	surf[focus]->glue(mhe[1], he[1]);
      
      if(mhe[2]!=NULL)
	surf[focus]->glue(mhe[2], he[2]);
    }
    break;
  default: 
    break;
  }
  
  return 1;
}

////////////////////////////////////////////////////////////////////////////

void read_ply(char *file_name)
{
 
  if (file_name==NULL)
    return;  
  
  p_ply ply = ply_open(file_name, NULL);
  
  if (!ply) return;
  if (!ply_read_header(ply)) return;
  
  ply_set_read_cb(ply, "vertex", "x", vertex_cbx, NULL, 0);
  ply_set_read_cb(ply, "vertex", "y", vertex_cby, NULL, 0);
  ply_set_read_cb(ply, "vertex", "z", vertex_cbz, NULL, 1);
  ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);

  pt_index=0;
  if (!ply_read(ply)) 
    return;

  ply_close(ply);  
}

///////////////////////////////////////////////////////////////////////////

void sqrt_3_subdivide(Surf* s)
{
  Vertex *v, *v0, *v1, *v2;
  Hedge *he;
  Face   *f;
  vec3 temp(0, 0, 0);
  float alpha; int n;

  FaceIter face_iter;
  VertexIter verts_iter;
  EdgeIter edge_iter;
  vector<Face*>::iterator face_iter1;
  vector<Vertex*>::iterator verts_iter1;
  vector<Edge*>::iterator edge_iter1;

  vector<Face*> old_faces;
  vector<Vertex*> old_verts;
  vector<Edge*> old_edges; 
  vector<vec3> old_points_displct;

  for(face_iter = s->faces_begin();face_iter!=s->faces_end(); face_iter++)
    old_faces.push_back(*face_iter);

  for(verts_iter = s->verts_begin();verts_iter!=s->verts_end(); verts_iter++)
    old_verts.push_back(*verts_iter);

  for(edge_iter = s->edges_begin();edge_iter!=s->edges_end(); edge_iter++)
    old_edges.push_back(*edge_iter);

  for(face_iter1 = old_faces.begin();face_iter1!=old_faces.end(); face_iter1++)
  {
	f = (*face_iter1);
	v0 = f->vertex(0);
    v1 = f->vertex(1);
    v2 = f->vertex(2);
    v = s->split(f); 
	v->p()->add((v0->p()->p() + v1->p()->p() + v2->p()->p())/3.0);
  }

  for(verts_iter1 = old_verts.begin();verts_iter1!=old_verts.end(); verts_iter1++)
  {
	v =  *verts_iter1;
	temp = vec3(0, 0, 0);
	for(n =0, he = v->star_first(); he!=NULL; he = v->star_next(he), n++)
	{
      if(!(n%2)){
		temp +=  he->org()->p()->p();}
	}
    alpha = (4 - 2*cos(4*M_PI/n))/9;
    temp = v->p()->p()*(1-alpha) + 2*alpha*temp/n;
    old_points_displct.push_back(temp);
  }

  for(n =0; n< old_verts.size(); n++)
  {
    v = old_verts[n];
    v->p()->add(old_points_displct[n] - v->p()->p()); 
  }

  for(edge_iter1 = old_edges.begin(); edge_iter1!=old_edges.end(); edge_iter1++)
  s->flip((*edge_iter1)->hedge(0));
}

/*---------------- OpenGL Functions -------------------- */

void draw_tri(double x0, double y0, double z0,
	          double x1, double y1, double z1,
	          double x2, double y2, double z2)
{
  glColor3f (0.0, 0.0, 0.0);
  glBegin(GL_LINE_LOOP);
  glVertex3d(x0, y0, z0);
  glVertex3d(x1, y1, z1);
  glVertex3d(x2, y2, z2);
  glEnd();
}

void display_mesh()
{
  vec3 p0, p1, p2;
  FaceIter f;

  glPushMatrix();
  glTranslatef(-1.5, 0.0, 0);
  for (f = surf[0]->faces_begin(); f != surf[0]->faces_end(); f++) {
	p0=(*f)->vertex(0)->p()->p();
    p1=(*f)->vertex(1)->p()->p();
    p2=(*f)->vertex(2)->p()->p();
    draw_tri(p0[0], p0[1], p0[2],
	         p1[0], p1[1], p1[2],
	         p2[0], p2[1], p2[2]);
  }
  glPopMatrix();

  glPushMatrix();
  glTranslatef(1.5, 0,0);
  for (f = surf[1]->faces_begin(); f != surf[1]->faces_end(); f++) {
	p0=(*f)->vertex(0)->p()->p();
    p1=(*f)->vertex(1)->p()->p();
    p2=(*f)->vertex(2)->p()->p();
    draw_tri(p0[0], p0[1], p0[2],
	         p1[0], p1[1], p1[2],
	         p2[0], p2[1], p2[2]);
  }
  glPopMatrix();

}

void initgl(void)
{
    glClearColor (1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3.0, 3.0, -1.5, 1.5, -3.0, 3.0);
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);
    display_mesh();
    glutSwapBuffers();
} 


void keyboard(unsigned char key, int x, int y)
{
  switch(key)  {
    case 27: exit(0);
    case 's': 
		sqrt_3_subdivide(surf[1]);
   		glutPostRedisplay();
		break;
    
    default: break;
  }
}

int main(int argc, char** argv)
{
  int wsize = 512;
  
  surf[0] = new Surf();
  surf[1] = new Surf();

  focus = 0;
  read_ply("../demo/sphere.ply");
  focus = 1;
  read_ply("../demo/sphere.ply");

  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (2*wsize, wsize);
  glutCreateWindow ("viewply");
  initgl();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
