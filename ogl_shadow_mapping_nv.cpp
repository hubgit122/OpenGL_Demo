//-----------------------------------------------------------------------------
//           Name: ogl_shadow_mapping_nv.cpp
//         Author: Kevin Harris
//  Last Modified: 06/11/05
//    Description: This sample demonstrates how to perform shadow mapping with
//                 OpenGL.
//
//                 The shadow mapping technique basically uses a depth texture
//                 or "shadow map" to figure out whether or not a pixel's Z
//                 value places it within a shadowed region or not.
//
//                 This involves two render passes. The first pass uses a
//                 p-buffer to create a depth texture from the light's point
//                 of view.
//
//                 On the second pass, we render our 3D content and perform a
//                 depth comparison between each pixel generated and the depth
//                 texture. If the pixel has a greater Z value than the depth
//                 texture, the pixel is further away from the light's position
//                 and we know it lies within a shadow. On the other had, if
//                 the pixel's Z value is less than the depth texture, we know
//                 that the current pixel is closer to the light than the
//                 object or shadow caster used to create the depth texture and
//                 couldn't possible be in a shadowed region.
//
//                 Of course, while the concept sounds easy enough, there is
//                 one sticking point that can make things less that intuitive:
//                 The pixels, which are being compared, were rendered from the
//                 eye's point-of-view and the depth map was created from the
//                 light's point of view, so for the comparison to be valid we
//                 need to do determine each pixel's XYZ position relative to
//                 the light. This sample uses eye-linear texture coordinate
//                 generation to find each pixel's light position.
//
//                 Please note that this sample uses some nVIDIA specific
//                 features and may not run properly on ATI cards.
//
//   Control Keys: Up    - Light moves up
//                 Down  - Light moves down
//                 Left  - Light moves left
//                 Right - Light moves right
//
//                 Left Mouse Button  - Spin the view
//                 Right Mouse Button - Spin the teapot
//
//                 F1 - 直接渲染深度纹理
//                 F2 - 是否显示光源指示球
//					F3 - 是否显示坐标轴
//					F4 - 坐标轴的模式切换
//					F5 - 是否启用微调(不同情境下作用不同)
//					F6 - 是否显示视景体
//					F7 - 是否启用直线抗锯齿
//					F8 - 是否启用雾
//					F11, F12 - 下一个/上一个场景
//					1 - 缩小视角
//					2 - 扩大视角
//					方向键和PageDown, PageUP - 移动光源
//                 滚动鼠标 - 拉近或拉远场景
//-----------------------------------------------------------------------------

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <cmath>
#include <gl/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "geometry.h"
#include "resource.h"

//-----------------------------------------------------------------------------
// FUNCTION POINTERS FOR OPENGL EXTENSIONS
//-----------------------------------------------------------------------------

// For convenience, this project ships with its own "glext.h" extension header
// file. If you have trouble running this sample, it may be that this "glext.h"
// file is defining something that your hardware doesn抰 actually support.
// Try recompiling the sample using your own local, vendor-specific "glext.h"
// header file. The same applies for "wglext.h".

#include "glext.h"      // Sample's header file
//#include <GL/glext.h> // Your local header file

#include "wglext.h"      // Sample's header file
//#include <GL/wglext.h> // Your local header file

// WGL_ARB_extensions_string
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = NULL;

// WGL_ARB_pbuffer
PFNWGLCREATEPBUFFERARBPROC       wglCreatePbufferARB       = NULL;
PFNWGLGETPBUFFERDCARBPROC        wglGetPbufferDCARB        = NULL;
PFNWGLRELEASEPBUFFERDCARBPROC    wglReleasePbufferDCARB    = NULL;
PFNWGLDESTROYPBUFFERARBPROC      wglDestroyPbufferARB      = NULL;

// WGL_ARB_pixel_format
PFNWGLCHOOSEPIXELFORMATARBPROC   wglChoosePixelFormatARB   = NULL;

// WGL_ARB_render_texture
PFNWGLBINDTEXIMAGEARBPROC        wglBindTexImageARB        = NULL;
PFNWGLRELEASETEXIMAGEARBPROC     wglReleaseTexImageARB     = NULL;

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
HWND   g_hWnd = NULL;
HDC	   g_hDC  = NULL;
HGLRC  g_hRC  = NULL;
GLuint g_depthTexture = -1;

float g_fSpinX_L =  0.0f;
float g_fSpinY_L = -10.0f;
float g_fSpinX_R =  0.0f;
float g_fSpinY_R =  0.0f;

float g_lightsLookAtMatrix[16];
float g_lightPosition[] = { 2.0f, 6.5f, 0.0f, 1.0f };

bool g_bRenderDepthTexture = false;
unsigned char sceneNo = 0;
bool objectCoodinate = true;
bool axis = false;
bool sphere = true;
bool adjust = false;
bool frustrum = false;
bool smooth = false;
bool fog = false;
GLfloat blue[4] = { 0.35f, 0.53f, 0.7f, 1.0f};
GLfloat gray[4] = {0.5,0.5,0.5,1.0};

const float nearZ = -0.1;
const float farZ = -100;
float z = 15.0f;
float fovy = 45;

static const long double PI = atan(1)*4.0;
float rescale = sqrt(2);
GLfloat point[8][3];
bool ini = true;
// This little struct will help to organize our p-buffer's data
struct PBUFFER
{
	HPBUFFERARB hPBuffer; // Handle to a p-buffer.
	HDC         hDC;      // Handle to a device context.
	HGLRC       hRC;      // Handle to a GL rendering context.
	int         nWidth;   // Width of the p-buffer
	int         nHeight;  // Height of the p-buffer
};

PBUFFER g_pbuffer;

const int PBUFFER_WIDTH  = 1024;//256;				//pBuffer越大阴影越精细.
const int PBUFFER_HEIGHT = 1024;//256;				//不是2的幂次也可以啊.

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void init(void);
void shutDown(void);
void initExtensions(void);
void initPbuffer(void);
void render(void);
void renderScene(void);
void createDepthTexture(void);
void displayDepthTexture(void);

int nWidth;
int nHeight;

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc( HWND   hWnd,
							 UINT   msg,
							 WPARAM wParam,
							 LPARAM lParam )
{
	static POINT ptLastMousePosit_L;
	static POINT ptCurrentMousePosit_L;
	static bool  bMousing_L;

	static POINT ptLastMousePosit_R;
	static POINT ptCurrentMousePosit_R;
	static bool  bMousing_R;

	switch( msg )
	{
		case WM_KEYDOWN:
		{
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
				case '1':
					fovy*=0.9;
					break;
				case '2':
					fovy/=0.9;
					break;
				case '3':
					break;
				case '4':
					break;

				case 33:			//PageUp
					g_lightPosition[1] += 0.1f;
					break;

				case 34:			//PageDown
					g_lightPosition[1] -= 0.1f;
					break;

				case VK_UP: 
					g_lightPosition[2] += 0.1f;
					break;

				case VK_DOWN:
					g_lightPosition[2] -= 0.1f;
					break;

				case VK_LEFT:
					g_lightPosition[0] -= 0.1f;
					break;

				case VK_RIGHT:
					g_lightPosition[0] += 0.1f;
					break;

				case VK_F1:
					g_bRenderDepthTexture = !g_bRenderDepthTexture;
					break;
				case VK_F2:
					sphere = !sphere;
					break;
				case VK_F3:
					axis = !axis;
					break;
				case VK_F4:
					objectCoodinate = !objectCoodinate;
					break;
				case VK_F5:
					adjust = !adjust;
					break;
				case VK_F6:
					frustrum = !frustrum;
					break;
				case VK_F7:
					smooth = !smooth;
					break;
				case VK_F8:
					fog =! fog;
					break;
				case VK_F9:
					break;
				case VK_F10:
					break;
				case VK_F11:
					sceneNo-=1;
					break;
				case VK_F12:
					sceneNo+=1;
					break;
				default:
					MessageBox(NULL, 
						"F1 - 直接渲染深度纹理\nF2 - 是否显示光源指示球\nF3 - 是否显示坐标轴\nF4 - 坐标轴的模式切换\nF5 - 是否启用微调(不同情境下作用不同)\nF6 - 是否显示视景体\nF7 - 是否启用直线抗锯齿\nF8 - 是否启用雾\nF11, F12 - 下一个/上一个场景\n1 - 缩小视角\n2 - 扩大视角\n方向键和PageDown, PageUP - 移动光源\n滚动鼠标 - 拉近或拉远场景",
						"请选择正确的操作", MB_OK | MB_ICONEXCLAMATION);
					break;
			}
		}
		break;

		case WM_LBUTTONDOWN:
		{
			ptLastMousePosit_L.x = ptCurrentMousePosit_L.x = LOWORD (lParam);
			ptLastMousePosit_L.y = ptCurrentMousePosit_L.y = HIWORD (lParam);
			bMousing_L = true;
		}
		break;

		case WM_LBUTTONUP:
		{
			bMousing_L = false;
		}
		break;

		case WM_RBUTTONDOWN:
		{
			ptLastMousePosit_R.x = ptCurrentMousePosit_R.x = LOWORD (lParam);
			ptLastMousePosit_R.y = ptCurrentMousePosit_R.y = HIWORD (lParam);
			bMousing_R = true;
		}
		break;

		case WM_RBUTTONUP:
		{
			bMousing_R = false;
		}
		break;

		case WM_MOUSEMOVE:
		{
			ptCurrentMousePosit_L.x = LOWORD (lParam);
			ptCurrentMousePosit_L.y = HIWORD (lParam);
			ptCurrentMousePosit_R.x = LOWORD (lParam);
			ptCurrentMousePosit_R.y = HIWORD (lParam);

			if( bMousing_L )
			{
				g_fSpinX_L -= (ptCurrentMousePosit_L.x - ptLastMousePosit_L.x);
				g_fSpinY_L -= (ptCurrentMousePosit_L.y - ptLastMousePosit_L.y);
			}

			if( bMousing_R )
			{
				g_fSpinX_R -= (ptCurrentMousePosit_R.x - ptLastMousePosit_R.x);
				g_fSpinY_R -= (ptCurrentMousePosit_R.y - ptLastMousePosit_R.y);
			}

			ptLastMousePosit_L.x = ptCurrentMousePosit_L.x;
			ptLastMousePosit_L.y = ptCurrentMousePosit_L.y;
			ptLastMousePosit_R.x = ptCurrentMousePosit_R.x;
			ptLastMousePosit_R.y = ptCurrentMousePosit_R.y;
		}
		break;

		case WM_MOUSEWHEEL:
		{
			short zDelt = (short) HIWORD(wParam);

			if( zDelt > 0 )
			{
				z -= 1;
			}
			else
			{
				z += 1;
			}
		}
		break;

		case WM_SIZE:				//改变窗口大小时一直在被调用.
		{
			nWidth  = LOWORD(lParam) / 2;
			nHeight = HIWORD(lParam) / 2;
		}
		break;

		case WM_CLOSE:
		{
			PostQuitMessage(0);
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

		default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	if ( (msg == WM_KEYDOWN ) || (msg == WM_MOUSEWHEEL ) || ((msg == WM_MOUSEMOVE) && (bMousing_L || bMousing_R)) || ((msg == WM_SIZE) && (ini == false)) )
	{
		render();
	}

	return 0;
}

void drawAxis()
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	{
		glColor3f(1,0,0);
		glVertex3f(0,0,adjust?-0.1:0); glVertex3f(100,0,adjust?-0.1:0);

		glColor3f(0,1,0);
		glVertex3f(0,0,adjust?-0.1:0); glVertex3f(0,100,adjust?-0.1:0);

		glColor3f(0,0,1);
		glVertex3f(0,0,adjust?-0.1:0); glVertex3f(0,0,100);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

//-----------------------------------------------------------------------------
// Name: renderScene()
// Desc:
//-----------------------------------------------------------------------------
void renderScene( void )
{
	switch (sceneNo)
	{
		default:
			sceneNo=0;
			break;
		case 2:
		case 0:		//茶壶和有一个球的地面
		{
			glMatrixMode( GL_MODELVIEW );

			// Render teapot...
			glPushMatrix();
			{
				// Teapot's position & orientation
				glTranslatef( 0.0f, 2.5f, 0.0f );
				glRotatef( -g_fSpinY_R, 1.0f, 0.0f, 0.0f );
				glRotatef( -g_fSpinX_R, 0.0f, 1.0f, 0.0f );

				glColor3f( 1.0f, 1.0f , 1.0f );
				renderSolidTeapot( 1.0);
			}
			glPopMatrix();

			// Render floor as a single quad...
			glPushMatrix();
			{
				glutSolidSphere(0.5, 32, 8);		//证明不仅在平面上投影是正确的.
				glBegin( GL_QUADS );
				{
					glNormal3f( 0.0f, 1.0f,  0.0f );		//指定法向才有光照效果. 就不能自动生成光照.
					glVertex3f(-5.0f, 0.0f, -5.0f );
					glVertex3f(-5.0f, 0.0f,  5.0f );
					glVertex3f( 5.0f, 0.0f,  5.0f );
					glVertex3f( 5.0f, 0.0f, -5.0f );
				}
				glEnd();
			}
			glPopMatrix();
		}
		break;

		case 1:			//一个四边形
		{
			glMatrixMode( GL_MODELVIEW );

			glPushMatrix();
			{
				glBegin( GL_QUADS );
				{
					glNormal3f( 0.0f, 1.0f,  0.0f );		//指定法向才有光照效果. 就不能自动生成光照.
					glVertex3f(-5.0f, 0.0f, -5.0f );
					glVertex3f(-5.0f, 0.0f,  5.0f );
					glVertex3f( 5.0f, 0.0f,  5.0f );
					glVertex3f( 5.0f, adjust?3.0f:0.0f, -5.0f );
				}
				glEnd();
			}
			glPopMatrix();
		}
		break;

		case 3:
		{
			glMatrixMode( GL_MODELVIEW );
			glColor3f(1,1,1);
			
			glPushMatrix();
			{
				glTranslatef( -2.5f, 0.8f, -2.5f );
				drawAxis();

				renderSolidTeapot( 1.0);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef( 2.5f, 0.8f, 2.5f );
				drawAxis();

				renderSolidTeapot( 1.0);
			}
			glPopMatrix();

			glPushMatrix();
			{
				drawAxis();

				glBegin( GL_QUADS );
				{
					glNormal3f( 0.0f, 1.0f,  0.0f );		//指定法向才有光照效果. 就不能自动生成光照.
					glVertex3f(-5.0f, 0.0f, -5.0f );
					glVertex3f(-5.0f, 0.0f,  5.0f );
					glVertex3f( 5.0f, 0.0f,  5.0f );
					glVertex3f( 5.0f, 0.0f, -5.0f );
				}
				glEnd();
			}
			glPopMatrix();

		}
		break;
	}
}

//-----------------------------------------------------------------------------
// Name: render()
// Desc:
//-----------------------------------------------------------------------------
void render( void )
{
	if (fog)
	{
		if (adjust)
		{
			glClearColor(0.5, 0.5, 0.5, 1.0);
			glFogfv (GL_FOG_COLOR, gray);
			glFogi (GL_FOG_MODE, GL_EXP);
		}
		else
		{
			glClearColor(0.35f, 0.53f, 0.7f, 1.0f);
			glFogfv(GL_FOG_COLOR, blue);
			glFogi (GL_FOG_MODE, GL_LINEAR);
		}
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}
	if (smooth)
	{
		glEnable(GL_BLEND);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_NICEST,GL_LINE_SMOOTH_HINT);
	}
	else
	{
		glDisable(GL_LINE_SMOOTH);
	}
	// Create a look-at matrix for our light and cache it for later use...
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	gluLookAt( g_lightPosition[0], g_lightPosition[1], g_lightPosition[2], // Look from the light's position
			   0.0f, 2.5f, 0.0f,   // Towards the teapot's position
			   0.0f, 1.0f, 0.0f ); // Up vector

	// Get the model-view matrix
	glGetFloatv( GL_MODELVIEW_MATRIX, g_lightsLookAtMatrix );

	createDepthTexture();

	//正式画
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glFlush();

	for (int i = 0; i < 4; ++i)
	{
		glViewport(nWidth * (i % 2 == 1), nHeight * (i >= 2), nWidth, nHeight);

		if (i == 1)			//立体图
		{
			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();
			gluPerspective( fovy, (GLdouble)nWidth / (GLdouble)nHeight, -nearZ, -farZ);
			// Place the view
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();

			for (int i=0;i<4;++i)
			{
				point[i][0] = rescale*nearZ*tan(fovy/180*PI/2)*((i==0||i==3)?-1:1);
				point[i][1] = rescale*nearZ*tan(fovy/180*PI/2)*(i&2?1:-1)*(GLdouble)nHeight/(GLdouble)nWidth;
				point[i][2] =  nearZ;

				point[i+4][0] = (adjust?0.95:1)*rescale*farZ*tan(fovy/180*PI/2)*((i==0||i==3)?-1:1);
				point[i+4][1] = (adjust?0.95:1)*rescale*farZ*tan(fovy/180*PI/2)*(i&2?1:-1)*(GLdouble)nHeight/(GLdouble)nWidth;
				point[i+4][2] =  farZ;
			}

			if (frustrum)
			{
				glDisable(GL_LIGHTING);
				glColor3f(1,0,0);
				glBegin(GL_LINES);
				{
					glColor3f(1,0,0);
					for (int i=0;i<4;++i)
					{
						glVertex3fv(point[i]);
						glVertex3fv(point[(i+1)&3]);
					}
					glColor3f(0,1,0);
					for (int i=0;i<4;++i)
					{
						glVertex3fv(point[i]);
						glVertex3fv(point[i+4]);
					}
					glColor3f(0,0,1);
					for (int i=0;i<4;++i)
					{
						glVertex3fv(point[i+4]);
						glVertex3fv(point[(i+1)&3+4]);
					}
				}
				glEnd();
			}

			glTranslatef( 0.0f, -2.0f, -z );						//视角变换, 注意灯光在视角变换之前放置. 否则translate函数使原点变化, 结果错误.
			glRotatef( -g_fSpinY_L, 1.0f, 0.0f, 0.0f );
			glRotatef( -g_fSpinX_L, 0.0f, 1.0f, 0.0f );
		}
		else
		{
			//正视图
			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();
			glOrtho(-nWidth / 50, nWidth / 50, -nHeight / 50, nHeight / 50, -40, 40);
			// Place the view
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();

			if (i == 2)		//俯视图
			{
				glRotatef(90, 1.0f, 0.0f, 0.0f );
			}
			else if (i == 3)		//侧视图
			{
				glRotatef(90, 0.0f, 1.0f, 0.0f );
			}
		}

		// Render the light's position as a sphere...
		glPushMatrix();
		{
			glLightfv( GL_LIGHT0, GL_POSITION, g_lightPosition );

			if (sphere)
			{
				glDisable( GL_LIGHTING );
				glTranslatef( g_lightPosition[0], g_lightPosition[1], g_lightPosition[2] );
				glColor3f(1.0f, 1.0f, 0.5f);
				renderSolidSphere( 0.1, 8, 8 );
			}
		}
		glPopMatrix();

		// Set up OpenGL's state machine for a depth comparison using the depth texture...
		glEnable( GL_LIGHTING );

		float x[] = { 1.0f, 0.0f, 0.0f, 0.0f };
		float y[] = { 0.0f, 1.0f, 0.0f, 0.0f };
		float z[] = { 0.0f, 0.0f, 1.0f, 0.0f };
		//float w[] = { 0.0f, 0.0f, 0.0f, 1.0f };						//Q坐标完全没有作用, 老外没仔细看文档.
		glTexGenfv( GL_S, GL_EYE_PLANE, x );
		glTexGenfv( GL_T, GL_EYE_PLANE, y );
		glTexGenfv( GL_R, GL_EYE_PLANE, z );
		//glTexGenfv( GL_Q, GL_EYE_PLANE, w );

		glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
		glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
		glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
		//glTexGeni( GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );

		glEnable( GL_TEXTURE_GEN_S );
		glEnable( GL_TEXTURE_GEN_T );
		glEnable( GL_TEXTURE_GEN_R );
		//glEnable( GL_TEXTURE_GEN_Q );

		// Set up the depth texture projection
		glMatrixMode( GL_TEXTURE );
		glLoadIdentity();
		glTranslatef( 0.5f, 0.5f, 0.5f );                     // Offset
		glScalef( 0.5f, 0.5f, 0.5f );                          // Bias
		gluPerspective( 75.0f, 640.0f / 480.0f, 0.1f, 100.0f); // light frustum
		glMultMatrixf( g_lightsLookAtMatrix );                 // Light matrix
		//注意在GL_EYE_LINEAR模式下, OpenGL内部自动乘以当前MODELVIEW_MATRIX 的逆, 所以直接变换到灯下就行了, 当然要放缩移动.

		// Bind the depth texture so we can use it as the shadow map...
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, g_depthTexture );

		// wglBindTexImageARB 是将pbuffer与纹理对象绑定的机制.
		// 纹理对象在本应用中只声明用法, 数据全在pbuffer里.
		// pbuffer也要声明格式, 才能被渲染像素, 才可以作为纹理并与纹理对象的格式配套.
		if( wglBindTexImageARB( g_pbuffer.hPBuffer, WGL_DEPTH_COMPONENT_NV ) == FALSE )		//WGL_DEPTH_COMPONENT_NV指定了被绑定在纹理对象上的PBuffer的内容: 深度缓冲区分量.
		{
			MessageBox(NULL, "Could not bind p-buffer to render texture!",
					   "ERROR", MB_OK | MB_ICONEXCLAMATION);
			exit(-1);
		}

		// 下面起作用的就是g_depthTexture的设置:
		// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_SGIX, GL_TRUE );
		// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_OPERATOR_SGIX, GL_TEXTURE_LEQUAL_R_SGIX );
		// 作用是将自动生成的纹理R坐标与纹理图片中P, Q对应的内容相比较, 如果比纹理中的值大, 就舍弃, 对应区域涂黑.
		// 涂黑等效于将对应像素的亮度乘以0, 其余乘以1.
		// 不论清除颜色为何, 阴影自动变黑.
		renderScene();

		if( wglReleaseTexImageARB( g_pbuffer.hPBuffer, WGL_DEPTH_COMPONENT_NV ) == FALSE )
		{
			MessageBox(NULL, "Could not release p-buffer from render texture!",
					   "ERROR", MB_OK | MB_ICONEXCLAMATION);
			exit(-1);
		}

		if (axis)
		{
			if (!objectCoodinate)
			{
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
			}
			drawAxis();
		}

		// Reset some of the states for the next go-around!
		glDisable( GL_TEXTURE_2D );
		glDisable( GL_TEXTURE_GEN_S );
		glDisable( GL_TEXTURE_GEN_T );
		glDisable( GL_TEXTURE_GEN_R );
	}

	if( g_bRenderDepthTexture == true )
	{
		displayDepthTexture(); // For debugging...
	}

	SwapBuffers( g_hDC );
	glFlush();
}

//-----------------------------------------------------------------------------
// Name: initExtensions()
// Desc: 初始化扩展函数
//-----------------------------------------------------------------------------
void initExtensions( void )
{
	// wglGetExtensionsStringARB
	wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	char* wgl_ext = NULL;

	if( wglGetExtensionsStringARB )
	{
		wgl_ext = (char*)wglGetExtensionsStringARB( wglGetCurrentDC() );
	}
	else
	{
		MessageBox(NULL, "Unable to get address for wglGetExtensionsStringARB!",
				   "ERROR", MB_OK | MB_ICONEXCLAMATION);
		exit(-1);
	}

	// WGL_ARB_pbuffer
	if( strstr( wgl_ext, "WGL_ARB_pbuffer" ) == NULL )
	{
		MessageBox(NULL, "WGL_ARB_pbuffer extension was not found",
				   "ERROR", MB_OK | MB_ICONEXCLAMATION);
		exit(-1);
	}
	else
	{
		wglCreatePbufferARB    = (PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
		wglGetPbufferDCARB     = (PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
		wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");
		wglDestroyPbufferARB   = (PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");

		if( !wglCreatePbufferARB || !wglGetPbufferDCARB ||
				!wglReleasePbufferDCARB || !wglDestroyPbufferARB )
		{
			MessageBox(NULL, "One or more WGL_ARB_pbuffer functions were not found",
					   "ERROR", MB_OK | MB_ICONEXCLAMATION);
			exit(-1);
		}
	}

	// WGL_ARB_pixel_format
	if( strstr( wgl_ext, "WGL_ARB_pixel_format" ) == NULL )
	{
		MessageBox(NULL, "WGL_ARB_pixel_format extension was not found",
				   "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	else
	{
		wglChoosePixelFormatARB  = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

		if( !wglCreatePbufferARB || !wglGetPbufferDCARB )
		{
			MessageBox(NULL, "One or more WGL_ARB_pixel_format functions were not found",
					   "ERROR", MB_OK | MB_ICONEXCLAMATION);
			exit(-1);
		}
	}

	// WGL_ARB_render_texture
	if( strstr( wgl_ext, "WGL_ARB_render_texture" ) == NULL )
	{
		MessageBox(NULL, "WGL_ARB_render_texture extension was not found",
				   "ERROR", MB_OK | MB_ICONEXCLAMATION);
		exit(-1);
	}
	else
	{
		wglBindTexImageARB    = (PFNWGLBINDTEXIMAGEARBPROC)wglGetProcAddress("wglBindTexImageARB");
		wglReleaseTexImageARB = (PFNWGLRELEASETEXIMAGEARBPROC)wglGetProcAddress("wglReleaseTexImageARB");

		if( !wglBindTexImageARB || !wglReleaseTexImageARB )
		{
			MessageBox(NULL, "One or more WGL_ARB_render_texture functions were not found",
					   "ERROR", MB_OK | MB_ICONEXCLAMATION);
			exit(-1);
		}
	}
}

//-----------------------------------------------------------------------------
// Name: displayDepthTexture()
// Desc: For debugging purposes
//-----------------------------------------------------------------------------
void displayDepthTexture( void )
{
	glDisable( GL_LIGHTING );

	glViewport( 0, 0, g_pbuffer.nWidth, g_pbuffer.nHeight);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( -1.0, 1.0, -1.0, 1.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// reset our texture matrix
	glMatrixMode( GL_TEXTURE );
	glLoadIdentity();

	// A depth texture can be treated as a luminance texture
	// 深度纹理被绘制时, 作为亮度纹理 来处理.
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, g_depthTexture );
	// Disable the shadow hardware
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_SGIX, GL_FALSE );

	if( wglBindTexImageARB( g_pbuffer.hPBuffer, WGL_DEPTH_COMPONENT_NV ) == FALSE )
	{
		MessageBox(NULL, "Could not bind p-buffer to render texture!",
				   "ERROR", MB_OK | MB_ICONEXCLAMATION);
		exit(-1);
	}

	glBegin( GL_QUADS );
	{
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -1.0f, -1.0f, 0.0f );

		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( -1.0f, 1.0f, 0.0f );

		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( 1.0f, 1.0f, 0.0f );

		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( 1.0f, -1.0f, 0.0f );
	}
	glEnd();

	glEnable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	// Enable the shadow mapping hardware
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_SGIX, GL_TRUE );

	if( wglReleaseTexImageARB( g_pbuffer.hPBuffer, WGL_DEPTH_COMPONENT_NV ) == FALSE )
	{
		MessageBox(NULL, "Could not release p-buffer from render texture!",
				   "ERROR", MB_OK | MB_ICONEXCLAMATION);
		exit(-1);
	}
}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int WINAPI WinMain(	HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR     lpCmdLine,
				   int       nCmdShow )
{
	WNDCLASSEX winClass;
	MSG        uMsg;

	memset(&uMsg, 0, sizeof(uMsg));

	winClass.lpszClassName = "MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
	winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_OPENGL_ICON);
	winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_OPENGL_ICON);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if( !RegisterClassEx(&winClass) )
	{
		return E_FAIL;
	}

	g_hWnd = CreateWindowEx( NULL, "MY_WINDOWS_CLASS",
		"OpenGL - Shadow Mapping",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 640, 480, NULL, NULL, hInstance, NULL );

	if( g_hWnd == NULL )
	{
		return E_FAIL;
	}

	ShowWindow( g_hWnd, nCmdShow );
	UpdateWindow( g_hWnd );

	init();

	render();

	while( uMsg.message != WM_QUIT )
	{
		if( GetMessage( &uMsg, NULL, 0, 0))		//			if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )			// 在应该重绘的地方加入重绘命令, 而不是原来的一直瞎画;
		{
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}

		//else											// 这个消息循环效率太低. 一直让CPU工作, 不好;
		//{
		//	render();
		//}
	}

	shutDown();

	UnregisterClass( "MY_WINDOWS_CLASS", winClass.hInstance );

	return uMsg.wParam;
}
//-----------------------------------------------------------------------------
// Name: initPbuffer()
// Desc: Create a p-buffer for the off-screen rendering of the depth texture
//-----------------------------------------------------------------------------
void initPbuffer( void )
{
	g_pbuffer.hPBuffer = NULL;
	g_pbuffer.nWidth   = PBUFFER_WIDTH;
	g_pbuffer.nHeight  = PBUFFER_HEIGHT;

	// Define the minimum pixel format requirements we will need for our
	// p-buffer. A p-buffer is just like a frame buffer, it can have a depth
	// buffer associated with it and it can be double buffered.
	int pf_attr[] =
	{
		WGL_SUPPORT_OPENGL_ARB, TRUE,       // P-buffer will be used with OpenGL
		WGL_DRAW_TO_PBUFFER_ARB, TRUE,      // Enable render to p-buffer				//关于pbuffer, 深度纹理的都有不同的像素格式索引. 所以要在索要索引时列出.

		WGL_BIND_TO_TEXTURE_DEPTH_NV, TRUE, // Ask for depth texture

		WGL_BIND_TO_TEXTURE_RGBA_ARB, TRUE, // P-buffer will be used as a texture
		WGL_DOUBLE_BUFFER_ARB, FALSE,       // We don't require double buffering
		0                                   // Zero terminates the list
	};

	unsigned int count = 0;
	int pixelFormat;

	//typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
	// DC, .. , .. , 最大格式数, 格式数组首地址, 共几个可用格式
	if( !wglChoosePixelFormatARB( g_hDC, pf_attr, NULL, 1, &pixelFormat, &count ) )
	{
		MessageBox(NULL, "pbuffer creation error:  wglChoosePixelFormatARB() failed.",
				   "ERROR", MB_OK | MB_ICONEXCLAMATION);
		exit(-1);
	}

	if( count <= 0 )
	{
		MessageBox(NULL, "pbuffer creation error:  Couldn't find a suitable pixel format.",
				   "ERROR", MB_OK | MB_ICONEXCLAMATION);
		exit(-1);
	}

	// Set some p-buffer attributes so that we can use this p-buffer as a
	// 2D RGBA texture target.
	int pb_attr[] =		//这个是pbuffer的属性, 上边那个是像素格式的属性, 不要搞混.
	{
		WGL_DEPTH_TEXTURE_FORMAT_NV, WGL_TEXTURE_DEPTH_COMPONENT_NV, // We need to render to a depth texture
		WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB,                // Our p-buffer will have a texture format of RGBA
		WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,                  // Of texture target will be GL_TEXTURE_2D
		0                                                            // Zero terminates the list
	};

	// Create the p-buffer...
	g_pbuffer.hPBuffer = wglCreatePbufferARB( g_hDC, pixelFormat, g_pbuffer.nWidth, g_pbuffer.nHeight, pb_attr );
	g_pbuffer.hDC      = wglGetPbufferDCARB( g_pbuffer.hPBuffer );
	g_pbuffer.hRC      = wglCreateContext( g_pbuffer.hDC );				//wgl是windows的Opengl, wgl的创建上下文就是用windows的上下文创建OpenGL的上下文.

	if( !g_pbuffer.hPBuffer )
	{
		MessageBox(NULL, "pbuffer creation error: wglCreatePbufferARB() failed!",
				   "ERROR", MB_OK | MB_ICONEXCLAMATION);
		exit(-1);
	}
}

//-----------------------------------------------------------------------------
// Name: init()
// Desc:
//-----------------------------------------------------------------------------
void init( void )
{
	MessageBox(NULL, 
		"F1 - 直接渲染深度纹理\nF2 - 是否显示光源指示球\nF3 - 是否显示坐标轴\nF4 - 坐标轴的模式切换\nF5 - 是否启用微调(不同情境下作用不同)\nF6 - 是否显示视景体\nF7 - 是否启用直线抗锯齿\nF8 - 是否启用雾\nF11, F12 - 下一个/上一个场景\n1 - 缩小视角\n2 - 扩大视角\n方向键和PageDown, PageUP - 移动光源\n滚动鼠标 - 拉近或拉远场景",
		"请操作", MB_OK | MB_ICONEXCLAMATION);
	GLuint PixelFormat;

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;						//32或16都行	//-3也行		//事实是我写几都行. 其实是在分配索引的时候找到了OpenGL支持的最接近的一个. 所以不会出现不能使用的情况.
	pfd.cDepthBits = 24;

	g_hDC = GetDC( g_hWnd );						//dc是设备上下文, 就是可以画图的地方.
	PixelFormat = ChoosePixelFormat( g_hDC, &pfd );		//得到像素格式的索引. 是最佳匹配, 而非所有的都能接受.	//OpenGL官方文档说这个函数没有扩展性, 所以才诞生了wglChoosePixelFormatARB函数. 用属性数组的指针作参数, 以0为结束标志, 可以任意扩展.
	SetPixelFormat( g_hDC, PixelFormat, &pfd);				//设置像素格式.

	// VC++ - OpenGL绘图的机制是：
	// 先用OpenGL的绘图上下文Rendering Context(简称为RC)把图画好，再把所绘结果通过SwapBuffers()函数传给Window的绘图上下文Device Context(简称为DC)。
	// 要注意的是，一个窗口有多个DC，但只有一个RC。因此当一个DC画完图后，要立即释放RC，以便其他的DC也使用。
	g_hRC = wglCreateContext( g_hDC );
	wglMakeCurrent( g_hDC, g_hRC );				// 调用OpenGL函数之前要将RC与对应的DC设置成当前.

	glClearColor( 0.35f, 0.53f, 0.7f, 1.0f );
	glEnable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );

	//初始化的时候会产生SIZE消息, 所以这一句是多余的.
	//glMatrixMode( GL_PROJECTION );
	//glLoadIdentity();
	//gluPerspective( 45.0f, 640.0f/480.0f, 0.1f, 100.0f);

	// Enable some dim, gray ambient lighting...
	GLfloat ambient_lightModel[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_lightModel );

	// Set up a point light source...
	glEnable( GL_LIGHT0 );
	GLfloat diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat linearAttenuation_light[] = { 0.0f };
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse_light );
	glLightfv( GL_LIGHT0, GL_LINEAR_ATTENUATION , linearAttenuation_light );

	// Initialize the p-buffer now that we have a valid context
	// that we can use during the p-buffer creation process.
	initExtensions();
	initPbuffer();

	// Initialize some graphics state for the p-buffer's rendering context.
	// 老外的注释是错误的, 没必要现在初始化. 因为渲染的时候也会调用makeCurrent. 其实这里只是为了挨个功能检测下是不是可以使用, 统一报错.
	if( wglMakeCurrent( g_pbuffer.hDC, g_pbuffer.hRC) == FALSE )
	{
		MessageBox(NULL, "Could not make the p-buffer's context current!",
				   "ERROR", MB_OK | MB_ICONEXCLAMATION);
		exit(-1);
	}
	else
	{
		// 注意应该给pbuffer的RC指定模型视图矩阵和视口, 所以才有了这几句.
		glViewport( 0, 0, PBUFFER_WIDTH, PBUFFER_HEIGHT );					//拉伸到正方形区域, 在渲染管线中, 视口变换在模型视图变换和投影变换的后面, 只有平级变换才需要考虑调用次序.
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( 75.0f, 640.0f / 480.0f, 0.1f, 100.0f);		//决定了CVV的内容. 因为在渲染管线中先做模型视图变换再做投影变换, 所以可以认为场景渲染完毕后再来一个线性拉伸.
	}

	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_DEPTH_TEST );

	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_lightModel );

	// Initialize some state for the window's rendering context.
	if( wglMakeCurrent( g_hDC, g_hRC ) == FALSE )
	{
		MessageBox(NULL, "Could not make the window's context current!",
				   "ERROR", MB_OK | MB_ICONEXCLAMATION);
		exit(-1);
	}

	// Create the depth texture
	glGenTextures( 1, &g_depthTexture );
	glBindTexture( GL_TEXTURE_2D, g_depthTexture );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_SGIX, GL_TRUE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_OPERATOR_SGIX, GL_TEXTURE_LEQUAL_R_SGIX );

	glLineWidth(3);

	static GLint fogMode = GL_LINEAR;

	glFogi (GL_FOG_MODE, fogMode);
	glFogfv (GL_FOG_COLOR, blue);
	glFogf (GL_FOG_DENSITY, 0.35);
	glHint (GL_FOG_HINT, GL_DONT_CARE);
	glFogf (GL_FOG_START, 1.0);
	glFogf (GL_FOG_END, 30);

	ini = false;
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc:
//-----------------------------------------------------------------------------
void shutDown( void )
{
	glDeleteTextures( 1, &g_depthTexture );

	if( g_hRC != NULL )
	{
		wglMakeCurrent( g_hDC, NULL );
		wglDeleteContext( g_hRC );
		g_hRC = NULL;
	}

	if( g_hDC != NULL )
	{
		ReleaseDC( g_hWnd, g_hDC );
		g_hDC = NULL;
	}

	// Don't forget to clean up after our p-buffer...
	if( g_pbuffer.hRC != NULL )
	{
		wglMakeCurrent( g_pbuffer.hDC, NULL );
		wglDeleteContext( g_pbuffer.hRC );
		wglReleasePbufferDCARB( g_pbuffer.hPBuffer, g_pbuffer.hDC );
		wglDestroyPbufferARB( g_pbuffer.hPBuffer );
		g_pbuffer.hRC = NULL;
	}

	if( g_pbuffer.hDC != NULL )
	{
		ReleaseDC( g_hWnd, g_pbuffer.hDC );
		g_pbuffer.hDC = NULL;
	}
}

//-----------------------------------------------------------------------------
// Name: createDepthTexture()
// Desc:
//-----------------------------------------------------------------------------
void createDepthTexture( void )
{
	// Make the pbuffer rendering context current
	//makeCurrent的意思不只是绑定DC与RC, 其实更重要的是指明当前OpenGL渲染函数的目标.
	if( wglMakeCurrent( g_pbuffer.hDC, g_pbuffer.hRC) == FALSE )
	{
		MessageBox(NULL, "Could not make the p-buffer's context current!",
				   "ERROR", MB_OK | MB_ICONEXCLAMATION);
		exit(-1);
	}

	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

	glPolygonOffset( 2.0f, 2.0f );				//这两个参数都要保持正值. 第而个参数貌似就是为了阴影设计的.
	glEnable( GL_POLYGON_OFFSET_FILL );					//这个太厉害了, 在绘制的真实图像时候不用多边形偏移, 而是在阴影处偏移一点, 防止自身阴影.

	// 将光源放在原点也就是放在了观察点.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float originPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv( GL_LIGHT0, GL_POSITION, originPosition );

	glMatrixMode( GL_MODELVIEW );
	glMultMatrixf( g_lightsLookAtMatrix);

	//设置好了pbuffer的属性, 直接渲染场景就好了. 在运行时按{F1}键, 发现得到的场景就是深度值.
	renderScene();

	glDisable( GL_POLYGON_OFFSET_FILL );

	// Make the window rendering context current
	if( wglMakeCurrent( g_hDC, g_hRC ) == FALSE )
	{
		MessageBox(NULL, "Could not make the window's context current!",
				   "ERROR", MB_OK | MB_ICONEXCLAMATION);
		exit(-1);
	}
}
