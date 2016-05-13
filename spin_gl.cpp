#include <cstdio>

#include <GL/gl.h>
#include <GL/glu.h>

#include "spin_gl.h"
#include "geometry.h"

#ifdef VSGL_ON_OPENGL
#include "vsgl_renaming.h"
#endif

//----------------------------------------------------------------------------
// Globals for graphics state

float light0_color[4] = {1, .5, 0, 1};
float light1_color[4] = {.5, 1, 1, 1};

float object_ambient[4] = {.1, .1, .1, 1};
float object_diffuse[4] = {.8, .8, .8, 1};
float object_specular[4] = {.5, .5, .5, 1};
float object_shininess = 50;


//----------------------------------------------------------------------------
// Globals for control

bool draw_wireframe = false;
bool flat_shade = false;
bool rotate_object = true;
bool rotate_lights = true;

float last_frame_time = 0;
float object_time = 0;
float lights_time = 0;


//----------------------------------------------------------------------------
// Actual GL functions

#define CHECK_OPENGL(l) {int _glerr ; if((_glerr = glGetError()) != GL_NO_ERROR) printf("GL Error: %04X at %d\n", _glerr, l); }

void set_lights(float lights_time)
{
    float origin[4] = {0, 0, 0, 1};

    float light0_angle_x = 100 + lights_time * 45 / 0.9;
    float light0_angle_y = 50 + lights_time * 45 / 1.1;

    float light1_angle_x = 90 + lights_time * 45 / 0.95;
    float light1_angle_y = 200 + lights_time * 45 / 1.25;

    glPushMatrix();
    CHECK_OPENGL(__LINE__);

    glRotatef(light0_angle_x, 1, 0, 0);
    glRotatef(light0_angle_y, 0, 1, 0);
    glTranslatef(10, 10, 10);
    glLightfv(GL_LIGHT0, GL_POSITION, origin);

    CHECK_OPENGL(__LINE__);
    glPopMatrix();

    glPushMatrix();

    glRotatef(light1_angle_x, 1, 0, 0);
    glRotatef(light1_angle_y, 0, 1, 0);
    glTranslatef(10, 10, 10);
    glLightfv(GL_LIGHT1, GL_POSITION, origin);

    CHECK_OPENGL(__LINE__);
    glPopMatrix();
    CHECK_OPENGL(__LINE__);
}

void draw_object(float object_time, bool draw_wireframe, bool flat_shade)
{
    if(flat_shade)
        glShadeModel(GL_FLAT);
    else
        glShadeModel(GL_SMOOTH);

    glPushMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, object_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, object_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, object_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, object_shininess);

    float object_angle_x = object_time * 45;
    float object_angle_y = object_time * 45 / 1.3;

    glRotatef(object_angle_x, 1, 0, 0);
    glRotatef(object_angle_y, 0, 1, 0);
    glScalef(0.5 / size, 0.5 / size, 0.5 / size);
    glTranslatef(-center[0], -center[1], -center[2]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(vertices[0]), (void*)&vertices[0].v[0]);
    glNormalPointer(GL_FLOAT, sizeof(vertices[0]), (void*)&vertices[0].n[0]);

    if(draw_wireframe) {
        int vertex_offset = 0;
        for(int i = 0; i < primitive_count; i++)
        {
            switch(primitive_types[i]) {
                case GL_TRIANGLE_FAN:
                    glDrawArrays(GL_LINE_LOOP, vertex_offset, primitive_vertex_counts[i]);
                    break;
                case GL_TRIANGLES:
                    for(int j = 0; i < primitive_vertex_counts[j] / 3; i++)
                        glDrawArrays(GL_LINE_LOOP, vertex_offset + j * 3, 3);
                    break;
            }
            vertex_offset += primitive_vertex_counts[i];
        }
    } else {
        int vertex_offset = 0;
        for(int i = 0; i < primitive_count; i++)
        {
            glDrawArrays(primitive_types[i], vertex_offset, primitive_vertex_counts[i]);
            vertex_offset += primitive_vertex_counts[i];
        }
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glPopMatrix();
    CHECK_OPENGL(__LINE__);
}

void draw_frame(float frame_time, bool draw_wireframe, bool flat_shade)
{
    glClear(GL_COLOR_BUFFER_BIT);
    CHECK_OPENGL(__LINE__);

    if(rotate_lights)
        lights_time += frame_time - last_frame_time;

    set_lights(lights_time);

    if(rotate_object)
        object_time += frame_time - last_frame_time;

    draw_object(object_time, draw_wireframe, flat_shade);

    last_frame_time = frame_time;
}

void init_state()
{
    glClearColor(1, 0, 0, 0);
    CHECK_OPENGL(__LINE__);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -1.5);

#ifdef __linux
    if(0)
    {
        float m[16];
        printf("modelview, translatef\n");
        glGetFloatv(GL_MODELVIEW_MATRIX, m);
        for(int i = 0; i < 4; i++)
            printf("%f %f %f %f\n", m[0 + i], m[4 + i], m[8 + i], m[12 + i]);
        CHECK_OPENGL(__LINE__);
    }
#endif

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-.1, .1, -.1, .1, .2, 100);

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_color);

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_color);

    glEnable(GL_LIGHTING);
    CHECK_OPENGL(__LINE__);

    glEnable(GL_CULL_FACE);

    glEnable(GL_NORMALIZE);

    CHECK_OPENGL(__LINE__);
}

// UI controls, reimplement depending on interface (e.g. hard buttons, etc)

bool pressed_s = false;
bool pressed_o = false;
bool pressed_w = false;
bool pressed_l = false;

bool get_rotate_object()
{
    return rotate_object;
}
void set_rotate_object(bool enabled)
{
    rotate_object = enabled;
}

bool get_rotate_lights()
{
    return rotate_lights;
}
void set_rotate_lights(bool enabled)
{
    rotate_lights = enabled;
}

bool get_draw_wireframe()
{
    return draw_wireframe;
}
void set_draw_wireframe(bool enabled)
{
    draw_wireframe = enabled;
}

bool get_flat_shade()
{
    return flat_shade;
}
void set_flat_shade(bool enabled)
{
    flat_shade = enabled;
}

void main_loop_body(float curtime)
{
    CHECK_OPENGL(__LINE__);
    draw_frame(curtime, draw_wireframe, flat_shade);
    CHECK_OPENGL(__LINE__);
}

void set_display_size(int width, int height)
{
    glViewport(0, 0, width, height);
}

