
extern "C" {

typedef unsigned int GLenum;

#define GL_LINE_LOOP				0x0002
#define GL_TRIANGLES				0x0004
#define GL_TRIANGLE_FAN				0x0006

#define GL_COLOR_MATERIAL			0x0B57
#define GL_CULL_FACE				0x0B44
#define GL_VERTEX_ARRAY				0x8074
#define GL_NORMAL_ARRAY				0x8075
#define GL_LIGHTING				0x0B50
#define GL_NORMALIZE				0x0BA1
#define GL_LIGHTING_BIT				0x00000040
#define GL_COLOR_BUFFER_BIT			0x00004000
#define GL_MODELVIEW				0x1700
#define GL_PROJECTION				0x1701
#define GL_LIGHT0				0x4000
#define GL_LIGHT1				0x4001
#define GL_LIGHT2				0x4002
#define GL_LIGHT3				0x4003
#define GL_LIGHT4				0x4004
#define GL_LIGHT5				0x4005
#define GL_LIGHT6				0x4006
#define GL_LIGHT7				0x4007
#define GL_AMBIENT				0x1200
#define GL_DIFFUSE				0x1201
#define GL_SPECULAR				0x1202
#define GL_SHININESS				0x1601
#define GL_POSITION				0x1203
#define GL_AMBIENT_AND_DIFFUSE			0x1602
#define GL_MODELVIEW_MATRIX			0x0BA6
#define GL_PROJECTION_MATRIX			0x0BA7
#define GL_INVALID_ENUM				0x0500
#define GL_STACK_OVERFLOW			0x0503
#define GL_STACK_UNDERFLOW			0x0504
#define GL_FLOAT				0x1406
#define GL_FRONT_AND_BACK 0x0408
#define GL_FLAT                           0x1D00
#define GL_SMOOTH                         0x1D01

GLenum glGetError();
void glLoadIdentity();
void glPushMatrix();
void glPopMatrix();
void glMultMatrixf(const float m[16]);
void glScalef(float x, float y, float z);
void glTranslatef(float x, float y, float z);
void glRotatef(float angle, float x, float y, float z);
void glFrustum(float left, float right, float bottom, float top, float near, float far);
void glMatrixMode(GLenum which);
void glEnableClientState(int enumerant);
void glDisableClientState(int enumerant);
void glEnable(int enumerant);
void glDisable(int enumerant);
void glLightfv(GLenum which_light, GLenum what, float *p);
void glMaterialfv(GLenum which_side, GLenum what, float *p);
void glMaterialf(GLenum which_side, GLenum what, float p);
void glShadeModel(GLenum which);
void glVertexPointer(int size, int type, int stride, void* ptr);
void glNormalPointer(int type, int stride, void* ptr);
void glDrawArrays(int what, int first, int count);
void glClear(int bits);
void glClearColor(float, float, float, float);
void glGetFloatv(GLenum which, float*p);
void glViewport(int x, int y, int width, int height);

}
