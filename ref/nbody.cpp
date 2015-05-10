/*
 * main.cc - C++ main program for n-body simulator
 *
 * So far, this is just a skeleton.
 * Calls Xforms to put up a simple user interface and allows particles
 * to be created, moved, and deleted.
 * Doesn't yet simulate Gravity; you'll need to add that.
 * After you change this code, I suggest you put your name on it!
 * Lines marked //NOTE: mark spots that you'll want to change
 *
 * Paul Heckbert, 18 Sept. 1998
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <ctype.h>
#include <math.h>

#include <GL/glu.h>	// OpenGL utilities

#include <svl/Vec2.h>	// defines Vec2 (2-D vector class) and Real
		// if compiled with -DVL_FLOAT then Real=float, else double

// following "extern" line tells C++ compiler that these
// routines & declarations are C code, not C++
// otherwise we might get an error message from linker like:
//    Unresolved text symbol "glxf_bind_pane(Glxf_pane*,flobjs_*,int*,...

extern "C" {
    #include "glxf.h"		// code to glue Xforms and OpenGL together
    #include "ui_form.h"	// XForms user interface
}

//-------------------- View definition

struct View {			// info about the current view
    // needed for converting between screen coordinates and world coordinates.

    GLdouble model_matrix[16];	// OpenGL's current modelview matrix
    GLdouble proj_matrix[16];	// OpenGL's current projection matrix

    struct {GLint x, y, width, height;} viewport;
    				// OpenGL's current viewport

    void update();		// updates matrices and viewport stored here
	// call this when transformations are changed or window resized

    GLdouble relscale()		// scale of screen space with respect to
	{return model_matrix[0];}    // world space relative to initial view

    Vec2 screen_to_world(int sx, int sy);
	// transform screen coordinates (sx,sy) to world coordinates
};

//-------------------- Particle and World definitions

struct Color {float red, green, blue;};	/* each component in [0,1] */

struct Particle {		// a particle
    Vec2 pos;			// (x,y) position
    Vec2 vel;			// velocity

    //NOTE: feel free to add stuff here (or anywhere)

    float mass;			// mass
    float radius;		// radius (for display)
    Color color;		// color (for display)

    void draw();		// draw this particle
};

struct World {			// World is a set of particles

    int n;			// number of particles in p[] array

    const int nmax = 10000;	// maximum number of particles permitted
    Particle pt[nmax];		// array of particles
    Particle *curp;		// pointer to currently selected particle
				//  (NULL if none selected)

    float G;			// gravitational constant
    float timestep;		// time step for gravity simulation
    Bool running;		// is simulation running or stopped?

    World()			// constructor
	: thresh_screen(10), G(1)
	{n = 0; curp = NULL; timestep = 1; running = false;}

    Particle *insert_particle(Real x, Real y, Real vx, Real vy, float mass);
				// add new particle at world coordinates (x,y)
				// with velocity (vx,vy)
				// returns ptr to it if successful, else NULL

    Bool delete_particle(Particle *p);		// delete a particle
				// returns true if successful, else false

    Particle *pick_particle(int sx, int sy, View &view);
				// pick a particle whose screen coordinates
				// are close to (sx,sy).
				// Returns NULL if closest to (sx,sy) is
				// farther than thresh_screen

    const float thresh_screen;	// pick tolerance in pixels

    void step(double dt);	// take one time step

    void draw();		// redraw the world
};

//-------------------- global variables

static FD_nbody *ui;	// Xforms' data structure for the user interface

static Glxf_pane pane;	// pane is Xwindows info about the sub-window of ui
			// that OpenGL draws into

static View view;	// transformations used in this window

static World world;	// set of particles

//-------------------- View procs

void print_matrix_transposed(char *str, double m[16]) {
    printf("%s:\n", str);
    int i, j;
    for (i=0; i<4; i++) {
	printf("  ");
	for (j=0; j<4; j++)
	    printf(" %7g", m[4*j+i]);	// transposed
	printf("\n");
    }
}

void View::update() {
    /* printf("View::update\n"); */
    glGetDoublev(GL_PROJECTION_MATRIX, proj_matrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, model_matrix);
    glGetIntegerv(GL_VIEWPORT, (GLint *)&viewport);
    /* print_matrix_transposed("projection matrix", proj_matrix); */
    /* print_matrix_transposed("modelview matrix", model_matrix); */
    /* printf("viewport=(%d %d %d %d)\n", */
	/* viewport.x, viewport.y, viewport.width, viewport.height); */
}

Vec2 View::screen_to_world(int sx, int sy) {
    // (sx,sy) is Xwindows screen coordinates; convert to world coordinates

    // this code will work whether Real=float or double
    double pw[3];
    gluUnProject(sx, viewport.height-1-sy, 0.,
	model_matrix, proj_matrix, (GLint *)&viewport,
	&pw[0], &pw[1], &pw[2]);
	// we do height-1-sy, and not sy, because Xwindows has y pointing up
	// while OpenGL has screen y pointing down, and gluUnProject wants
	// OpenGL screen coords, not Xwindows screen coords
    Vec2 p;
    p[0] = pw[0];
    p[1] = pw[1];
    /* printf("View::screen_to_world (%d,%d) -> (%g,%g)\n", sx, sy, p[0], p[1]); */
    return p;
}

//-------------------- reading particle2 files

/* here's an example of the particle2 file format:
=========================
particle2
# orbit.p2
# small particle orbiting a more massive one

m 1000
p .5 .5  0 0	# initial pos. & vel. of particle 0

m 10
p .7 .5   0 20	# initial pos. & vel. of particle 1
=========================
*/

#define END_OF_FILE_TOKEN "end_of_file"
static int read_line = 1;
static char *read_filename;

/* Chomp away white space, but count the newlines */
Bool read_whitespace(FILE *fp) {
    int c;
    do {
	c = getc(fp);
	switch (c) {
	    case EOF: return false;
	    case '#': do c = getc(fp); while (c!='\n'); read_line++; break;
	    case '\n': read_line++; break;
	}
    } while (isspace(c) || c=='#');
    ungetc(c, fp);
    return true;
}

/* read the next token from the file */
char *read_token(FILE *fp) {
    static char word[80];
    if (!read_whitespace(fp) || fscanf(fp, "%s", word)!=1)
	strcpy(word, END_OF_FILE_TOKEN);
    return word;
}

/* read the next number from the file */
double read_number(FILE *fp) {
    double x;
    read_whitespace(fp);
    if (fscanf(fp, "%lf", &x) != 1) {
	fprintf(stderr, "expecting a number at line %d of %s\n",
	    read_line, read_filename);
	exit(1);
    }
    return x;
}

Bool particle2_read_stream(FILE *fp, char *filename) {
    // read particle2 file format into world
    read_filename = filename;
    char *tok = read_token(fp);
    if (strcmp(tok, "particle2")) {	// "magic number" for file
	fprintf(stderr, "%s is not a particle2 file; it starts with \"%s\"\n",
	    read_filename, tok);
	return false;
    }
    float mass = 1;
    Real px, py, vx, vy;
    while (1) {
	tok = read_token(fp);
	if (!strcmp(tok, END_OF_FILE_TOKEN)) break;
	else if (!strcmp(tok, "m"))
	    mass = read_number(fp);
	else if (!strcmp(tok, "p")) {
	    px = read_number(fp);
	    py = read_number(fp);
	    vx = read_number(fp);
	    vy = read_number(fp);
	    world.insert_particle(px, py, vx, vy, mass);
	}
	else if (!strcmp(tok, "G"))
	    world.G = read_number(fp);
	else if (!strcmp(tok, "particle2"));
	    // ignore "particle2" mid-file; so we can concatenate p2 files
	else printf("ignoring token \"%s\" on line %d of %s\n",
	    tok, read_line, read_filename);
    }
    printf("read %d particles from %s\n", world.n, read_filename);
    return true;
}

//-------------------- Particle and World procs

double frand() {
    // return a random number between 0 and 1
    return rand()/32767.;
}

void Particle::draw() {
    glColor3fv(&color.red);
    float diam = 2*radius*view.relscale();	// set diameter in screen space
    if (diam<1.5) diam = 1.5;			// don't let them disappear
    glPointSize(diam); // sets diameter in screen space units, not world space
    glBegin(GL_POINTS);
    glVertex2f(pos[0], pos[1]);			// draw point
    glEnd();
}

Color random_color() {
    // return a random color, but not too close to black
    Color c;
    do {
	c.red   = frand();
	c.green = frand();
	c.blue  = frand();
    } while (.3*c.red + .59*c.green + .11*c.blue < .3);
    // the above formula computes the luminance
    // (brightness of a color for human perception)
    return c;
}

Particle *World::insert_particle(Real x, Real y, Real vx, Real vy, float mass) {
    if (n>=nmax) return NULL;
    pt[n].pos = Vec2(x, y);
    pt[n].vel = Vec2(vx, vy);
    pt[n].mass = mass;

    // for display, we set particle's display radius proportional to
    // the square root of mass, so that area is proportional to mass
    // (this is an aesthetic decision, not a physical one)
    pt[n].radius = sqrt(mass);
    pt[n].color = random_color();
    printf("World::insert_particle %d at (%g,%g) m=%g r=%g\n",
	n, x, y, mass, pt[n].radius);
    return &pt[n++];
}

Bool World::delete_particle(Particle *p) {
    if (p==NULL) return false;
    int i = p-pt;
    if (i<0 || i>=n) {
	fprintf(stderr,
	    "delete_particle: attempt to delete a bogus particle\n");
	return false;
    }
    printf("World::delete_particle %d\n", i);
    pt[i] = pt[n-1];		// fill the hole
    // (it's ok to reorder the pt array, since order is irrelevant)
    n--;
    return true;
}

Particle *World::pick_particle(int sx, int sy, View &view) {

    Vec2 q = view.screen_to_world(sx, sy);
	// transform screen space (sx,sy) to world space q

    Real thresh_world = thresh_screen/view.viewport.width/view.relscale(); 
	// threshold distance in world space

    int i;
    Real d, min = HUGE;
    Particle *nearest = NULL;
    for (i=0; i<n; i++) {
	d = sqrlen(pt[i].pos-q);		// distance squared
	if (d<min) {
	    min = d;
	    nearest = &pt[i];
	}
    }
    if (sqrt(min) > thresh_world) nearest = NULL;
    printf("World::pick_particle %d\n", nearest==NULL ? -1 : nearest-pt);
    return nearest;
}

void World::step(double dt) {
    // advance time by dt, updating the particle positions and velocities

    //NOTE: YOU NEED TO WRITE THIS!
    printf("World::step\n");
}

void error_check(char *file, int line) {
    /* this routine checks to see if OpenGL errors have occurred recently */
    GLenum e;

    while ((e = glGetError()) != GL_NO_ERROR)
    fprintf(stderr, "Error: %s before file %s line %d\n",
	gluErrorString(e), file, line);
}

void World::draw() {
    // redisplay the world

    /* printf("World::draw\n"); */
    glColor3f(0., 0., 0.);		// set current color to black
    glClear(GL_COLOR_BUFFER_BIT);	// clear to this color (background)

    // draw a grid with gray lines
    glColor3f(.15, .15, .15);
    glBegin(GL_LINES);
    int i;
    for (i=0; i<=10; i++) {
	glVertex2f(i/10., 0.);
	glVertex2f(i/10., 1.);

	glVertex2f(0., i/10.);
	glVertex2f(1., i/10.);
    }
    glEnd();

    for (i=0; i<n; i++)			// draw all the particles
	pt[i].draw();

    // swap displayed and working buffers (double buffering)
    glXSwapBuffers(pane.display, pane.window);

    /* when linking with Mesa, it's necessary to call glFlush() whenever you
     * want to make sure that stuff is drawn now (as opposed to buffered) */
    /* glFlush(); */

    /* Calling the following routine occasionally can aid debugging.
     * It will print something only if there's been an OpenGL error
     * since it was last called. */
    error_check(__FILE__, __LINE__);
}

//-------------------- Xforms callback procedures
// Xforms calls these when you modify sliders

void mass_proc(FL_OBJECT *obj, long foo) {
    if (!world.curp) return;
    world.curp->mass = fl_get_slider_value(ui->mass_slider);
    world.curp->radius = sqrt(world.curp->mass);
    printf("mass=%g for particle %d\n", world.curp->mass, world.curp-world.pt);
    world.draw();
}

void dt_proc(FL_OBJECT *obj, long foo) {
    world.timestep = pow(10., fl_get_slider_value(ui->dt_slider));
    printf("timestep=%g\n", world.timestep);
}

//-------------------- Xwindows mouse and keyboard input

Bool is_key_pressed(Display *display, KeySym sym) {
    // determine if the key "sym" is pressed or not
    // this routine from "Introduction to the X Window System", 1989, p. 388

    KeyCode code = XKeysymToKeycode(display, sym);
    if (code == NoSymbol) return false;
    char key_vector[32];
    XQueryKeymap(display, key_vector);
    return (key_vector[code/8] >> (code&7)) & 1;
}

void handle_event(XEvent *event) {
    /* see /usr/include/X11/X.h for definitions of X Windows event types */

    // you won't want to leave these printf's in here for your finished program,
    // but they should help you get started and debug.

    static int xprev, yprev, xstart, ystart;

    switch (event->type) {
	case ButtonPress:    /* button press */
	    printf("  ButtonPress.  button %d at (%d, %d)\n",
		event->xbutton.button, event->xbutton.x, event->xbutton.y);
	    switch (event->xbutton.button) {
		case 1:
		    world.curp = world.pick_particle(event->xbutton.x,
			event->xbutton.y, view);
		    if (!world.curp) break;
		    if (is_key_pressed(pane.display, XK_Shift_L) ||
		        is_key_pressed(pane.display, XK_Shift_R)) {
			    world.delete_particle(world.curp);
			    world.curp = NULL;
			}
		    else
			fl_set_slider_value(ui->mass_slider, world.curp->mass);
			    // set slider to show the mass of this particle
		    break;
		case 2:
		    {
			Vec2 p = view.screen_to_world(event->xbutton.x,
			    event->xbutton.y);
			world.curp = world.insert_particle(p[0], p[1], 0, 0,
			    fl_get_slider_value(ui->mass_slider));
		    }
		    break;
		case 3:
		    // save these for later use when MotionNotify event comes
		    xstart = xprev = event->xbutton.x;
		    ystart = yprev = event->xbutton.y;
		    break;
	    }
	    world.draw();
	    break;

	case ButtonRelease:  /* button release */
	    printf("  ButtonRelease. button %d\n", event->xbutton.button);
	    break;

	case MotionNotify:   /* mouse pointer moved */
	    printf("  MotionNotify at (%d, %d) state=%d=(%s %s %s)\n",
		event->xmotion.x, event->xmotion.y,
		event->xmotion.state,
		event->xmotion.state&Button1Mask ? "button1" : ".",
		event->xmotion.state&Button2Mask ? "button2" : ".",
		event->xmotion.state&Button3Mask ? "button3" : ".");
	    if (world.curp && (
		event->xmotion.state & (Button1Mask|Button2Mask))) {
		    /* printf("moving particle %d\n", world.curp-world.pt); */
		    world.curp->pos = view.screen_to_world(event->xmotion.x,
			event->xmotion.y);
	    }
	    else if (event->xmotion.state & Button3Mask) {
		if (is_key_pressed(pane.display, XK_Shift_L) ||
		    is_key_pressed(pane.display, XK_Shift_R)) {
			float s = 1. + (event->xmotion.x - xprev)/100.;
			/* printf("s=%g\n", s); */
			Vec2 pos = view.screen_to_world(xstart, ystart);
			// scale view around the point where user moused down
			glTranslatef(pos[0], pos[1], 0.);
			glScalef(s, s, 1.);
			glTranslatef(-pos[0], -pos[1], 0.);
		}
		else {
		    float t = view.viewport.width*view.relscale();
		    glTranslatef((event->xmotion.x - xprev)/t,
			-(event->xmotion.y - yprev)/t, 0.);
		}
		view.update();
		xprev = event->xmotion.x;
		yprev = event->xmotion.y;
	    }
	    world.draw();
	    break;

	case EnterNotify:       /* mouse pointer entered window */
	    printf("  EnterNotify.\n");
	    break;

	case LeaveNotify:       /* mouse pointer left window */
	    printf("  LeaveNotify.\n");
	    break;

	case KeyPress:          /* key pressed */
	case KeyRelease:        /* key released */
	    {
		char buf[10];
		int n;
		KeySym keysym;

		printf(event->type==KeyPress ? "  KeyPress." : "  KeyRelease.");
		n = XLookupString(&event->xkey, buf, sizeof buf - 1,
		    &keysym, 0);
		buf[n] = 0;
		/* print key as ascii and in hexadecimal */
		/* see /usr/include/X11/keysymdef.h for key codes */
		printf(" key=(%c)=0x%x\n", keysym, keysym);
	    }
	    break;

	case Expose:            /* window just uncovered */
	    printf("  Expose.\n");
	    world.draw();
	    break;

	case VisibilityNotify:  /* window occluded or exposed */
	    printf("  VisibilityNotify.\n");
	    break;

	default:
	    printf("  unknown event type.\n");
	    break;
    }
}

//-------------------- main

int main(int argc, char **argv) {

    // initialize the world with particle2 file?
    if (argc>1) {
	char *file = argv[1];
	if (!strcmp(file, "."))
	    particle2_read_stream(stdin, "stdin");
	else {
	    FILE *fp;
	    fp = fopen(file, "r");
	    if (fp==NULL) {
		fprintf(stderr, "Can't open particle file %s\n", file);
		exit(1);
	    }
	    particle2_read_stream(fp, file);
	    fclose(fp);
	}
    }

    /* initialize Xforms */
    fl_initialize(&argc, argv, "n-body", 0, 0);
    ui = create_form_nbody();

    // draw the UI window
    fl_show_form(ui->nbody,  	/* form */
	FL_PLACE_SIZE,   /* pos & size flags */
	FL_FULLBORDER,   /* border flags */
	argv[0]          /* window name */
    );

    // set up so we can draw in the big window of "ui" using OpenGL
    // set bits-per-pixel and other attributes of window

    // attributes for RGBA, without z-buffer, single-buffered
    // int single_attrs[] =
    //	{GLX_RGBA, GLX_DEPTH_SIZE, 0, GLX_RED_SIZE, 4, None};

    // attributes for RGBA, without z-buffer, double-buffered
    int double_attrs[] =
	{GLX_RGBA, GLX_DEPTH_SIZE, 0, GLX_RED_SIZE, 4, GLX_DOUBLEBUFFER, None};

    glxf_bind_pane(&pane,
	ui->pane,		// window
	double_attrs,		// requested attributes for window
	handle_event		// event_handler for window
    );

    // set viewport for OpenGL
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 1, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    // leave matrix mode at MODELVIEW so that later transformations
    // (glScale, glTranslate) go into that matrix

    // save info about current transformations
    view.update();

    // disable z-buffer and lighting
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    dt_proc(ui->dt_slider, 0);	// initialize world.timestep

    world.draw();		// draw the world (clear the screen)

    // Xforms handles events until exit button is pushed

    // the following loop repeats continuously when the world is "running",
    // otherwise it sleeps in fl_do_forms() waiting for mouse motion or other
    // Xwindows events, so as to not eat up unneeded CPU time

    // We have callback procedures set up for the sliders, but not for
    // the run and exit buttons.
    // fl_check_forms() and fl_do_forms() return only when an event occurs
    // (button, slider, mouse motion...) for which no callback procedure has
    // been set.
    // Therefore, control returns here when the run or exit buttons is hit.

    FL_OBJECT *obj;
    do {
	if (world.running) {		// if running:
	    world.step(world.timestep);	// take a time step
	    world.draw();		// redraw
	    obj = fl_check_forms();	// do a (non-blocking) check for events
	}
	else				// if not running:
	    obj = fl_do_forms();	// sleep until next event comes

	if (obj == ui->run_button) {
	    // user just hit the run button; toggle it
	    world.running = !world.running;
	}
    } while (obj != ui->exit_button);	// keep going until exit button is hit

    return 0;
}
