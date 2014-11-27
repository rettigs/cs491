#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "glui.h"


// title of these windows:

const char *WINDOWTITLE = { "Particle System -- Joe Graphics" };
const char *GLUITITLE   = { "User Interface Window" };

// random parameters:                   

const float XMIN =  { -100.0 };
const float XMAX =  {  100.0 };
const float YMIN =  { -100.0 };
const float YMAX =  {  100.0 };
const float ZMIN =  { -100.0 };
const float ZMAX =  {  100.0 };
const float RMIN =  {  2.f };
const float RMAX =  {  6.f };
const float RADIUS = { 3.f };
const float TMIN =  { 0.0 };
const float TMAX =  { 50.0 };
const float THETAMIN =  { (float)(M_PI)/4.f };
const float THETAMAX =  { 3.f*(float)(M_PI)/4.f };
const float VELMIN =    { 10.f };
const float VELMAX =    { 40.f };

const float DT =    { 0.030f };
const float GRAVITY =   { -9.8f };


// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE      0x1b


// initial window size:

const int INIT_WINDOW_SIZE = { 600 };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// able to use the left mouse for either rotation or scaling,
// in case have only a 2-button mouse:

enum LeftButton
{
    ROTATE,
    SCALE
};


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
    ORTHO,
    PERSP
};


// which button:

enum ButtonVals
{
    GO,
    RESET,
    QUIT
};


// window background color (rgba):

const float BACKCOLOR[] = { 0., 0., 0., 0. };


// color and line width for the axes:

const GLfloat AXES_COLOR[] = { 1., .5, 0. };
const GLfloat AXES_WIDTH   = { 3. };


// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };





//
// non-constant global variables:
//

int ActiveButton;       // current button that is down
GLuint  AxesList;       // list to hold the axes
int AxesOn;         // ON or OFF
int Debug;          // ON means print debug info
int DepthCueOn;     // TRUE means to use intensity depth cueing
float   Dt;
GLUI *  Glui;           // instance of glui window
int GluiWindow;     // the glut id for the glui window
int LeftButton;     // either ROTATE or SCALE
int MainWindow;     // window id for main graphics window
int ParticleList;
GLfloat RotMatrix[4][4];    // set by glui rotation widget
float   Scale, Scale2;      // scaling factors
float   Time;           // current time     
int WhichProjection;    // ORTHO or PERSP
int Xmouse, Ymouse;     // mouse values
float   Xrot, Yrot;     // rotation angles in degrees
float   TransXYZ[3];        // set by glui translation widgets


//
// function prototypes:
//

void    Animate( void );
void    Axes( float );
void    Buttons( int );
void    Circle( float, float, float );
void    Display( void );
void    DoRasterString( float, float, float, char * );
void    DoStrokeString( float, float, float, float, char * );
float   ElapsedSeconds( void );
void    InitGlui( void );
void    InitGraphics( void );
void    InitLists( void );
void    Keyboard( unsigned char, int, int );
void    MouseButton( int, int, int, int );
void    MouseMotion( int, int );
float   Ranf( float, float );
int     Ranf( int, int );
void    Reset( void );
void    Resize( int, int );
void    Visibility( int );


//
// main program:
//

int
main( int argc, char *argv[] )
{
    glutInit( &argc, argv );
    InitGraphics();
    InitLists();
    Reset();
    InitGlui();
    glutMainLoop();
    return 0;
}

void
Animate( void )
{
    Time += Dt;
    //fprintf( stderr, "Time = %8.3f\n", Time );
    if( Time > TMAX )
    {
        Time = 0.;
    }
    


    // ****************************************
    // Here is where you advance your particles to reflect the current Time:
    // ****************************************

    



    glutSetWindow( MainWindow );
    glutPostRedisplay();
}




//
// glui buttons callback:
//

void
Buttons( int id )
{
    switch( id )
    {
        case GO:
            GLUI_Master.set_glutIdleFunc( Animate );
            break;

        case RESET:
            Reset( );
            Time = 0.;
            GLUI_Master.set_glutIdleFunc( NULL );
            Glui->sync_live();
            glutSetWindow( MainWindow );
            glutPostRedisplay();
            break;

        case QUIT:
            Glui->close( );
            glutSetWindow( MainWindow );
            glFinish( );
            glutDestroyWindow( MainWindow );
            exit( 0 );
            break;

        default:
            fprintf( stderr, "Don't know what to do with Button ID %d\n", id );
    }

}



//
// draw the complete scene:
//

void
Display( void )
{
    GLsizei vx, vy, v;      // viewport dimensions
    GLint xl, yb;       // lower-left corner of viewport
    GLfloat scale2;     // real glui scale factor

    if( Debug )
    {
        fprintf( stderr, "Display\n" );
    }


    // set which window we want to do the graphics into:

    glutSetWindow( MainWindow );


    // erase the background:

    glDrawBuffer( GL_BACK );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable( GL_DEPTH_TEST );


    // specify shading to be flat:

    glShadeModel( GL_FLAT );


    // set the viewport to a square centered in the window:

    vx = glutGet( GLUT_WINDOW_WIDTH );
    vy = glutGet( GLUT_WINDOW_HEIGHT );
    v = vx < vy ? vx : vy;          // minimum dimension
    xl = ( vx - v ) / 2;
    yb = ( vy - v ) / 2;
    glViewport( xl, yb,  v, v );


    // set the viewing volume:
    // remember that the Z clipping  values are actually
    // given as DISTANCES IN FRONT OF THE EYE
    // ONLY USE gluOrtho2D() IF YOU ARE DOING 2D !

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    if( WhichProjection == ORTHO )
        //glOrtho( XMIN, XMAX,   YMIN, YMAX, 0.1, 1000. );
        glOrtho( -300., 300.,  -300., 300., 0.1, 1000. );
    else
        gluPerspective( 90., 1.,    0.1, 1000. );


    // place the objects into the scene:

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( 0., 0., 400.,     0., 0., 0.,     0., 1., 0. );

    glTranslatef( (GLfloat)TransXYZ[0], (GLfloat)TransXYZ[1], -(GLfloat)TransXYZ[2] );
    glRotatef( (GLfloat)Yrot, 0., 1., 0. );
    glRotatef( (GLfloat)Xrot, 1., 0., 0. );
    glMultMatrixf( (const GLfloat *) RotMatrix );
    glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );
    scale2 = 1. + Scale2;       // because glui translation starts at 0.
    if( scale2 < MINSCALE )
        scale2 = MINSCALE;
    glScalef( (GLfloat)scale2, (GLfloat)scale2, (GLfloat)scale2 );

    if( DepthCueOn )
    {
        glFogi( GL_FOG_MODE, FOGMODE );
        glFogfv( GL_FOG_COLOR, FOGCOLOR );
        glFogf( GL_FOG_DENSITY, FOGDENSITY );
        glFogf( GL_FOG_START, FOGSTART );
        glFogf( GL_FOG_END, FOGEND );
        glEnable( GL_FOG );
    }
    else
    {
        glDisable( GL_FOG );
    }

    if( AxesOn )
        glCallList( AxesList );



    // ****************************************
    // Here is where you draw the current state of the particles:
    // ****************************************






    glutSwapBuffers();
    glFlush();
}



//
// use glut to display a string of characters using a raster font:
//

void
DoRasterString( float x, float y, float z, char *s )
{
    char c;         // one character to print

    glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
    for( ; ( c = *s ) != '\0'; s++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
    }
}



//
// use glut to display a string of characters using a stroke font:
//

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
    char c;         // one character to print
    float sf;       // the scale factor

    glPushMatrix();
        glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
        sf = ht / ( 119.05 + 33.33 );
        glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
        for( ; ( c = *s ) != '\0'; s++ )
        {
            glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
        }
    glPopMatrix();
}



//
// return the number of seconds since the start of the program:
//

float
ElapsedSeconds( void )
{
    // get # of milliseconds since the start of the program:

    int ms = glutGet( GLUT_ELAPSED_TIME );

    // convert it to seconds:

    return (float)ms / 1000.;
}



//
// initialize the glui window:
//

void
InitGlui( void )
{
    GLUI_Panel *panel;
    GLUI_Rotation *rot;
    GLUI_Translation *trans, *scale;

    if( Debug )
        fprintf( stderr, "InitGlui\n" );


    // setup the glui window:

    glutInitWindowPosition( INIT_WINDOW_SIZE + 50, 0 );
    Glui = GLUI_Master.create_glui( (char *) GLUITITLE );


    Glui->add_statictext( (char *) GLUITITLE );
    Glui->add_separator();

    Glui->add_checkbox( "Axes", &AxesOn );

    Glui->add_checkbox( "Perspective", &WhichProjection );

    //Glui->add_checkbox( "Intensity Depth Cue", &DepthCueOn );

    panel = Glui->add_panel( "Object Transformation" );

        rot = Glui->add_rotation_to_panel( panel, "Rotation", (float *) RotMatrix );

        // allow the object to be spun via the glui rotation widget:

        rot->set_spin( 1.0 );


        Glui->add_column_to_panel( panel, GLUIFALSE );
        scale = Glui->add_translation_to_panel( panel, "Scale",  GLUI_TRANSLATION_Y , &Scale2 );
        scale->set_speed( 0.01f );

        Glui->add_column_to_panel( panel, FALSE );
        trans = Glui->add_translation_to_panel( panel, "Trans XY", GLUI_TRANSLATION_XY, &TransXYZ[0] );
        trans->set_speed( 1.1f );

        Glui->add_column_to_panel( panel, FALSE );
        trans = Glui->add_translation_to_panel( panel, "Trans Z",  GLUI_TRANSLATION_Z , &TransXYZ[2] );
        trans->set_speed( 1.1f );

    Glui->add_checkbox( "Debug", &Debug );


    panel = Glui->add_panel( "", FALSE );

    Glui->add_button_to_panel( panel, "Reset", RESET, (GLUI_Update_CB) Buttons );

    Glui->add_column_to_panel( panel, FALSE );

    Glui->add_button_to_panel( panel, "Go !", GO, (GLUI_Update_CB) Buttons );

    Glui->add_column_to_panel( panel, FALSE );

    Glui->add_button_to_panel( panel, "Quit", QUIT, (GLUI_Update_CB) Buttons );


    // tell glui what graphics window it needs to post a redisplay to:

    Glui->set_main_gfx_window( MainWindow );


    // set the graphics window's idle function:

    GLUI_Master.set_glutIdleFunc( NULL );
}



//
// initialize the glut and OpenGL libraries:
//  also setup display lists and callback functions
//

void
InitGraphics( void )
{
    if( Debug )
        fprintf( stderr, "InitGraphics\n" );


    // setup the display mode:
    // ( *must* be done before call to glutCreateWindow() )
    // ask for color, double-buffering, and z-buffering:

    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );


    // set the initial window configuration:

    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );


    // open the window and set its title:

    MainWindow = glutCreateWindow( WINDOWTITLE );
    glutSetWindowTitle( WINDOWTITLE );


    // setup the clear values:

    glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );


    // setup the callback routines:


    // DisplayFunc -- redraw the window
    // ReshapeFunc -- handle the user resizing the window
    // KeyboardFunc -- handle a keyboard input
    // MouseFunc -- handle the mouse button going down or up
    // MotionFunc -- handle the mouse moving with a button down
    // PassiveMotionFunc -- handle the mouse moving with a button up
    // VisibilityFunc -- handle a change in window visibility
    // EntryFunc    -- handle the cursor entering or leaving the window
    // SpecialFunc -- handle special keys on the keyboard
    // SpaceballMotionFunc -- handle spaceball translation
    // SpaceballRotateFunc -- handle spaceball rotation
    // SpaceballButtonFunc -- handle spaceball button hits
    // ButtonBoxFunc -- handle button box hits
    // DialsFunc -- handle dial rotations
    // TabletMotionFunc -- handle digitizing tablet motion
    // TabletButtonFunc -- handle digitizing tablet button hits
    // MenuStateFunc -- declare when a pop-up menu is in use
    // TimerFunc -- trigger something to happen a certain time from now
    // IdleFunc -- what to do when nothing else is going on

    glutSetWindow( MainWindow );
    glutDisplayFunc( Display );
    glutReshapeFunc( Resize );
    glutKeyboardFunc( Keyboard );
    glutMouseFunc( MouseButton );
    glutMotionFunc( MouseMotion );
    glutPassiveMotionFunc( NULL );
    glutVisibilityFunc( Visibility );
    glutEntryFunc( NULL );
    glutSpecialFunc( NULL );
    glutSpaceballMotionFunc( NULL );
    glutSpaceballRotateFunc( NULL );
    glutSpaceballButtonFunc( NULL );
    glutButtonBoxFunc( NULL );
    glutDialsFunc( NULL );
    glutTabletMotionFunc( NULL );
    glutTabletButtonFunc( NULL );
    glutMenuStateFunc( NULL );
    glutTimerFunc( 0, NULL, 0 );

    // DO NOT SET THE GLUT IDLE FUNCTION HERE !!
    // glutIdleFunc( NULL );
    // let glui take care of it in InitGlui()


    // ****************************************
    // Here is where you setup your particle system data structures just once:
    // ****************************************






}




//
// initialize the display lists that will not change:
//

void
InitLists( void )
{
    if( Debug )
        fprintf( stderr, "InitLists\n" );

    ParticleList = glGenLists( 1 );
    glNewList( ParticleList, GL_COMPILE );
        glutSolidSphere( 1., 8, 8 );
    glEndList();

    AxesList = glGenLists( 1 );
    glNewList( AxesList, GL_COMPILE );
        glColor3fv( AXES_COLOR );
        glLineWidth( AXES_WIDTH );
            Axes( 100. );
        glLineWidth( 1. );
    glEndList();
}



//
// the keyboard callback:
//

void
Keyboard( unsigned char c, int x, int y )
{
    if( Debug )
        fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

    switch( c )
    {
        case 'o':
        case 'O':
            WhichProjection = ORTHO;
            break;

        case 'p':
        case 'P':
            WhichProjection = PERSP;
            break;

        case 'q':
        case 'Q':
        case ESCAPE:
            Buttons( QUIT );    // will not return here
            break;          // happy compiler

        case 'r':
        case 'R':
            LeftButton = ROTATE;
            break;

        case 's':
        case 'S':
            LeftButton = SCALE;
            break;

        case '-':
            Dt /= 1.10f;
            break;

        case '+':
            Dt *= 1.10f;
            break;

        default:
            fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
    }
    Glui->sync_live();
    glutSetWindow( MainWindow );
    glutPostRedisplay();
}



//
// called when the mouse button transitions down or up:
//

void
MouseButton( int button, int state, int x, int y )
{
    int b;          // LEFT, MIDDLE, or RIGHT

    if( Debug )
        fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

    
    // get the proper button bit mask:

    switch( button )
    {
        case GLUT_LEFT_BUTTON:
            b = LEFT;       break;

        case GLUT_MIDDLE_BUTTON:
            b = MIDDLE;     break;

        case GLUT_RIGHT_BUTTON:
            b = RIGHT;      break;

        default:
            b = 0;
            fprintf( stderr, "Unknown mouse button: %d\n", button );
    }


    // button down sets the bit, up clears the bit:

    if( state == GLUT_DOWN )
    {
        Xmouse = x;
        Ymouse = y;
        ActiveButton |= b;      // set the proper bit
    }
    else
    {
        ActiveButton &= ~b;     // clear the proper bit
    }
}



//
// called when the mouse moves while a button is down:
//

void
MouseMotion( int x, int y )
{
    int dx, dy;     // change in mouse coordinates

    if( Debug )
        fprintf( stderr, "MouseMotion: %d, %d\n", x, y );


    dx = x - Xmouse;        // change in mouse coords
    dy = y - Ymouse;

    if( ActiveButton & LEFT )
    {
        switch( LeftButton )
        {
            case ROTATE:
                Xrot += ( ANGFACT*dy );
                Yrot += ( ANGFACT*dx );
                break;

            case SCALE:
                Scale += SCLFACT * (float) ( dx - dy );
                if( Scale < MINSCALE )
                    Scale = MINSCALE;
                break;
        }
    }


    if( ActiveButton & MIDDLE )
    {
        Scale += SCLFACT * (float) ( dx - dy );

        // keep object from turning inside-out or disappearing:

        if( Scale < MINSCALE )
            Scale = MINSCALE;
    }

    Xmouse = x;         // new current position
    Ymouse = y;

    glutSetWindow( MainWindow );
    glutPostRedisplay();
}



//
// reset the transformations and the colors:
//
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene
//

void
Reset( void )
{
    ActiveButton = 0;
    AxesOn = GLUITRUE;
    Debug = GLUIFALSE;
    DepthCueOn = FALSE;
    Dt = DT;
    LeftButton = ROTATE;
    Scale  = 1.0;
    Scale2 = 0.0;       // because add 1. to it in Display()
    WhichProjection = PERSP;
    Xrot = Yrot = 0.;
    TransXYZ[0] = TransXYZ[1] = TransXYZ[2] = 0.;

                      RotMatrix[0][1] = RotMatrix[0][2] = RotMatrix[0][3] = 0.;
    RotMatrix[1][0]                   = RotMatrix[1][2] = RotMatrix[1][3] = 0.;
    RotMatrix[2][0] = RotMatrix[2][1]                   = RotMatrix[2][3] = 0.;
    RotMatrix[3][0] = RotMatrix[3][1] = RotMatrix[3][3]                   = 0.;
    RotMatrix[0][0] = RotMatrix[1][1] = RotMatrix[2][2] = RotMatrix[3][3] = 1.;


    // ****************************************
    // Here is where you reset your particle system data structures to the initial configuration:
    // ****************************************




    Time = TMIN;
}



void
Resize( int width, int height )
{
    if( Debug )
        fprintf( stderr, "ReSize: %d, %d\n", width, height );
    glutSetWindow( MainWindow );
    glutPostRedisplay();
}



void
Visibility ( int state )
{
    if( Debug )
        fprintf( stderr, "Visibility: %d\n", state );

    if( state == GLUT_VISIBLE )
    {
        glutSetWindow( MainWindow );
        glutPostRedisplay();
    }
    else
    {
        // could optimize by keeping track of the fact
        // that the window is not visible and avoid
        // animating or redrawing it ...
    }
}







// the stroke characters 'X' 'Y' 'Z' :

static float xx[] = {
        0., 1., 0., 1.
          };

static float xy[] = {
        -.5, .5, .5, -.5
          };

static int xorder[] = {
        1, 2, -3, 4
        };


static float yx[] = {
        0., 0., -.5, .5
          };

static float yy[] = {
        0., .6f, 1., 1.
          };

static int yorder[] = {
        1, 2, 3, -2, 4
        };


static float zx[] = {
        1., 0., 1., 0., .25, .75
          };

static float zy[] = {
        .5, .5, -.5, -.5, 0., 0.
          };

static int zorder[] = {
        1, 2, 3, 4, -5, 6
        };


// fraction of the length to use as height of the characters:

#define LENFRAC     0.10


// fraction of length to use as start location of the characters:

#define BASEFRAC    1.10


//
//  Draw a set of 3D axes:
//  (length is the axis length in world coordinates)
//

void
Axes( float length )
{
    int i, j;           // counters
    float fact;         // character scale factor
    float base;         // character start location


    glBegin( GL_LINE_STRIP );
        glVertex3f( length, 0., 0. );
        glVertex3f( 0., 0., 0. );
        glVertex3f( 0., length, 0. );
    glEnd();
    glBegin( GL_LINE_STRIP );
        glVertex3f( 0., 0., 0. );
        glVertex3f( 0., 0., length );
    glEnd();

    fact = LENFRAC * length;
    base = BASEFRAC * length;

    glBegin( GL_LINE_STRIP );
        for( i = 0; i < 4; i++ )
        {
            j = xorder[i];
            if( j < 0 )
            {
                
                glEnd();
                glBegin( GL_LINE_STRIP );
                j = -j;
            }
            j--;
            glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
        }
    glEnd();

    glBegin( GL_LINE_STRIP );
        for( i = 0; i < 5; i++ )
        {
            j = yorder[i];
            if( j < 0 )
            {
                
                glEnd();
                glBegin( GL_LINE_STRIP );
                j = -j;
            }
            j--;
            glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
        }
    glEnd();

    glBegin( GL_LINE_STRIP );
        for( i = 0; i < 6; i++ )
        {
            j = zorder[i];
            if( j < 0 )
            {
                
                glEnd();
                glBegin( GL_LINE_STRIP );
                j = -j;
            }
            j--;
            glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
        }
    glEnd();

}





void
Quit()
{
    exit( 0 );
}




#define TOP 2147483647.     // 2^31 - 1 

float
Ranf( float low, float high )
{
    long random();      // returns integer 0 - TOP
    float r;        // random number    

    r = (float)rand();

    return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int
Ranf( int ilow, int ihigh )
{
    float low = (float)ilow;
    float high = ceil( (float)ihigh );

return (int) Ranf( low, high );
}



void
Circle( float xc, float yc, float radius )
{
    glBegin( GL_TRIANGLE_FAN );
        glVertex2f( 0., 0. );
        int numsegs = 20;
        float dang = 2.*M_PI/(float)numsegs;
        float ang;
        int i;
        for( i = 0, ang = 0.; i <= numsegs; i++, ang += dang )
        {
            glVertex2f( RADIUS*cos(ang), RADIUS*sin(ang) );
        }
    glEnd( );
}

