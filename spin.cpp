#include <cstdio>
#include <cstring>
#include <ctime>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "spin_gl.h"
#include "raster.h"

//----------------------------------------------------------------------------
// Linux and GLUT scaffolding

// Called from pocketgl.cpp

void clear_color_buffer(float r, float g, float b)
{
    glClearColor(r, g, b, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

int win_width;
int win_height;

void rasterize_line(const screen_vertex* sv0, const screen_vertex* sv1)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef SUBPIXEL_COORDS
    glOrtho(0, win_width * subpixel_coordinate_scale, win_height * subpixel_coordinate_scale, 0, 0, 1.0f);
#else
    glOrtho(0, win_width, win_height, 0, 0, 1.0f);
#endif
    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_LINES);
    glColor3s(sv0->r, sv0->g, sv0->b);
    glVertex2s(sv0->x, sv0->y);
    glColor3s(sv1->r, sv1->g, sv1->b);
    glVertex2s(sv1->x, sv1->y);
    glEnd();
}

void rasterize_triangle(const screen_vertex* sv0, const screen_vertex* sv1, const screen_vertex* sv2)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef SUBPIXEL_COORDS
    glOrtho(0, win_width * subpixel_coordinate_scale, win_height * subpixel_coordinate_scale, 0, 0, 1.0f);
#else
    glOrtho(0, win_width, win_height, 0, 0, 1.0f);
#endif
    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_TRIANGLES);
    // glColor3s(sv0->r, sv0->g, sv0->b);
    glColor3ub(sv0->r >> 7, sv0->g >> 7, sv0->b >> 7);
    glVertex2s(sv0->x, sv0->y);
    // glColor3s(sv1->r, sv1->g, sv1->b);
    glColor3ub(sv1->r >> 7, sv1->g >> 7, sv1->b >> 7);
    glVertex2s(sv1->x, sv1->y);
    // glColor3s(sv2->r, sv2->g, sv2->b);
    glColor3ub(sv2->r >> 7, sv2->g >> 7, sv2->b >> 7);
    glVertex2s(sv2->x, sv2->y);
    glEnd();

    if(0) {
        glBegin(GL_TRIANGLES);
        glColor3s(16384, 32767, 16384);
        glVertex2s(10, 10);
        glColor3s(16384, 16384, 32767);
        glVertex2s(100, 10);
        glColor3s(32767, 16384, 16384);
        glVertex2s(50, 50);
        glEnd();
    }

}

// Clock management

static double clock_start;

double wallclock()
{
    timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    double nowf = (double)now.tv_sec + (double)now.tv_nsec / (double)1e9;
    return nowf;
}

double elapsed()
{
    return wallclock() - clock_start;
}

static void elapsed_init() __attribute__((constructor));
static void elapsed_init()
{
    clock_start = wallclock();
}


// GLUT

    extern void flush_deferred_primitives(void);
void redraw()
{
    main_loop_body(elapsed() / 5);
#ifdef VSGL_ON_OPENGL
    flush_deferred_primitives();
#endif
    glutSwapBuffers();
}

void keydown(unsigned char key, int x, int y)
{
    switch(key) {
        case 's':
	    set_flat_shade(!get_flat_shade());
	    break;
        case 'o':
	    set_rotate_object(!get_rotate_object());
	    break;
        case 'w':
	    set_draw_wireframe(!get_draw_wireframe());
	    break;
        case 'l':
	    set_rotate_lights(!get_rotate_lights());
	    break;
        case 'q':
            exit(EXIT_SUCCESS);
    }
}

void motion(int x, int y)
{
}

void button(int b, int state, int x, int y)
{
}

void reshape(int width, int height)
{
    set_display_size(width, height);
    win_width = width;
    win_height = height;
}


int main(int argc, char **argv)
{
    char *progname;
    bool double_buffered = true;

    glutInit(&argc, argv);

    progname = argv[0];
    argc--;
    argv++;
    while(argc > 0) {
        if(strcmp(argv[0], "-single") == 0) {
            double_buffered = false;
            argc--;
            argv++;
        } else {
            printf("usage: %s [-single]\n", progname);
        }
    }

    glutInitWindowSize(512,512);
    if(double_buffered)
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    else
        glutInitDisplayMode(GLUT_RGB ); // | GLUT_DOUBLE);

#ifdef VSGL_ON_OPENGL
    glutCreateWindow("vsgl spin");
#else
    glutCreateWindow("opengl spin");
#endif

    glutDisplayFunc(redraw);
    glutIdleFunc(redraw);
    glutKeyboardFunc(keydown);
    glutMotionFunc(motion);
    glutMouseFunc(button);
    glutReshapeFunc(reshape);

    init_state();

    glutMainLoop();
}
