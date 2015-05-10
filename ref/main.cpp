
#include <cassert>
#include <iostream>
using namespace std;

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <boxcollision.h>
#include <cell.h>
#include <commandline.h>
#include <d2toindex.h>
#include <graphmisc.h>
#include <histogram.h>
#include <integration.h>
#include <particle.h>
#include <particledisp.h>
#include <particleranvd.h>
#include <particlesampler.h>
#include <zpr.h>


//#define DEBUG_PART


//integration<particlev0theta0> G;
//integration<particleranvd,d2toindex> G;
//integration<particlev0,d2toindex> G;
integration<particlev0spaced,d2toindex> G;



void animate()
{
  if (G.run)
    G.step();

  G.fr.update();
  G.stepcountmsg.updatevalue("stepcount: ",G.stepcount);
  glutPostRedisplay();
}


void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

  glDisable(GL_LIGHTING);
  //axes(10.0);

  G.draw();



  glPopMatrix();
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void keyboard
(
  unsigned char key, 
  int x, 
  int y
)
{
  switch (key)
  {
    case 27:
      exit(0);
      break;
    case 'h':
      G.step();
      break;
    case 'H':
      G.steplarge();
      break;
    case 's':
    case 'S':
      G.togglestartstop();
      break;

  }
}


void test01(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(600, 600);
  glutCreateWindow("Collision Detection and Dynamics: Particles in an Arena");
  glutDisplayFunc(display);
  glutIdleFunc(animate);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  G.reset(argc,argv);

  zpr zz;

  zz.update();

  commandline cmd(argc,argv);

  double angle=20.0;
  double near=1.0;
  double far=100.0;
  uint choice(2);
  cmd.mapvar(choice,"choice");
  cmd.mapvar(angle,"angle");
  cmd.mapvar(near,"near");
  cmd.mapvar(far,"far");

  double left=-1.0;
  double right=1.0;
  double bottom=-1.0;
  double top=1.0;

  cmd.mapvar(left,"left");
  cmd.mapvar(right,"right");
  cmd.mapvar(bottom,"bottom");
  cmd.mapvar(top,"top");

  if (choice!=0)
  {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
  }
 
  switch( choice )
  {
    case 0:  break;
    case 1:  
      glFrustum(left,right,bottom,top,near,far); break;
    case 2:  camera::lookat(cmd,4.8,4.7,5.6); break;
      // Can over-ride with 
      // $./main choice=2 camerax=2.0 cameray=.4 cameraz=4.0 
      // whatever numbers you want
    case 3:  gluPerspective(angle,1.0,near,far); break;
    default: break;
  }

  if (choice!=0)
  {
    glMatrixMode(GL_MODELVIEW);
  }

  double eyex(0.0);
  double eyey(0.0);
  double eyez(0.0);
  cmd.mapvar(eyex,"eyex");
  cmd.mapvar(eyey,"eyey");
  cmd.mapvar(eyez,"eyez");
  glTranslated(eyex,eyey,eyez);

  glutPostRedisplay();
 
  glutMainLoop();
}


// 
//  Measuring particle update separate to graphics 
//
//  This is going to be used to compare against other implementations.
void test02(int argc, char** argv)
{
  int const milli = 1000;
  uint N = 1000;
  commandline c(argc,argv);
  c.mapvar(N,"N");
  c.mapvar(G.uniform,"uniform");
  c.mapvar(G.numParticles,"numParticles");

  G.reset(argc,argv);

  float startTime = glutGet(GLUT_ELAPSED_TIME) / (float)milli;
  if (!G.uniform)
  {
    for (uint i=0; i<N; ++i)
      G.bruteforce();
  }
  else
  {
    for (uint i=0; i<N; ++i)
      G.uniformgridtest();
  }

  float elapsedTime = glutGet(GLUT_ELAPSED_TIME) / (float)milli - startTime;
  cout << SHOW(elapsedTime) << endl;
}

void test03(int argc, char** argv)
{
  particlev0theta0 D
  (
    0.2,  //radius
    1.0,  //vmax
    boxcollision(0.0,10.0,0.0,10.0)  //box dimensions
  );

  uintc N(8);
  particle pi[N];
  d2toindex f2(pi,D.box);
  f2.reset();
  uint W = (uint)( sqrt((double)N) )+1;
  f2.W = W;

  for (uint i=0; i<N; ++i)
  {
    D.randomposition(pi[i]);
    cout << pi[i];
    cout << endl;
    cout << "  indx=" << f2.index(i) << endl;
  }

  cout << endl << endl;

  cell C(N);
  C.eval(f2);

  uint * nb;
  uint sz;
  for (uint i=0; i<W*W; ++i)
  {
    f2.getsurroundingcells(nb,sz,i);
    cout << "i=" << i << " ";
    for (uint k=0; k<sz; ++k)
      cout << nb[k] << " ";
    cout << endl;
  }

}



int main(int argc, char** argv)
{
  commandline c(argc,argv);
  bool benchmark(false);
  c.mapvar(benchmark,"benchmark");

  if (benchmark)
    test02(argc,argv);
  else
    test01(argc,argv);



  return 0;
}
