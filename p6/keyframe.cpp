#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <ctype.h>

#ifndef WIN32
#include <sys/time.h>
#endif

#ifdef WIN32
#pragma warning(disable:4996)
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "glui.h"


// title of these windows:
const char *WINDOWTITLE = { "KeyFrame Animation -- Joe Graphics" };
const char *GLUITITLE   = { "User Interface Window" };


// what the glui package defines as true and false:
const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };

// the escape key:
#define ESCAPE      0x1b

// initial window size:
const int INIT_WINDOW_SIZE = { 670 };

// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT    = {   1.     };
const float TRANSFACT  = {  50.f    };
const float SCLFACT    = {   0.005f };

// minimum allowable scale factor:
const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):
const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:
const int ORTHO = { GLUIFALSE };
const int PERSP = { GLUITRUE  };


// which button:
#define RESET       0
#define QUIT        1

// window background color (rgba):
const float BACKCOLOR[] = { 0., 0., 0., 0. };

// color and line width for the axes:
const float AXES_COLOR[] = { 1., .5, 0. };
const float AXES_WIDTH   = 3.;

// time in milliseconds for cycle to complete:
const int MSEC = { 10*1000 };

// handy to have around:
const int OFF = { 0 };
const int ON  = { 1 };
#define FALSE   0
#define TRUE    1

// window background color (rgba): = {
const float BACKGROUND_COLOR[4] = { 0.,0.,0.,0. };

// sizes:
const float FLOOR = { -100. };
const float CEILING = { 1100. };
const float LEFT_WALL = { -100. };
const float RIGHT_WALL = { 1100. };
const float BACK_WALL = { -100. };
const float FRONT_WALL = { 1100. };
const float FLOOR_COLOR[3] = { .5f,.5f,.5f };
const float LEFT_WALL_COLOR[3] = { .2f,.2f,.2f };
const float BACK_WALL_COLOR[3] = { .3f,.3f,.3f };

const float RADIUS1 = { 50. };
const float RADIUS2 = { 80. };

// colors:
const float KEYFRAME_COLOR[4] = { 0.6f, 0.6f, 0., 1. };
const float OBJECT_COLOR[4]   = { 0.7f, 0.4f, 0., 1. };


 // non-constant global variables:

int ActiveButton;       // current button that is down
int AnimateOn;      // TRUE means we are animating
int AxesOn;         // ON or OFF
GLUI *  Glui;           // instance of glui window
int GluiWindow;     // the glut id for the glui window
int MainWindow;     // window id for graphics window
int KeyFrameOn;     // ON or OFF
int NowFrame;       // current frame #
float   RotMatrix[4][4];    // set by glui rotation widget
float   Scale, Scale2;      // scaling factors
float   ThetaX, ThetaY, ThetaZ; // current angles
int WhichProjection;    // ORTHO or PERSP
float   X, Y, Z;        // current location
int Xmouse, Ymouse;     // mouse values
float   Xrot, Yrot;     // rotation angles in degrees
float   TransXYZ[3];        // set by glui translation widgets

GLUI_Rotation   *GluiRot;




// the KeyFrames:

struct keyframe
{
    int f;              // frame #
    float x, y, z;          // x, y, and z locations
    float ax, ay, az;       // angles in degrees
    float h;            // object's hue (s=v=1.)
    float dxdf, dydf, dzdf;     // slopes
    float daxdf, daydf, dazdf;  // slopes
};

struct keyframe KeyFrames[ ] =
{
    ?????
};

const int NUMKEYFRAMES  = sizeof( KeyFrames ) / sizeof( struct keyframe );
const int MAXFRAME      = KeyFrames[ NUMKEYFRAMES-1 ].f;

// handy to have around
float   White[ ] = { 1.,1.,1.,1. };

 // function prototypes:
void    Animate( void );
float * Array3( float, float, float );
void    Axes( float length );
void    Buttons( int );
void    Display( void );
void    DisplayObj( void );
void    DoRasterString( float, float, float, char * );
void    DoStrokeString( float, float, float, float, char * );
void    InitGraphics( void );
void    InitLists( void );
void    InitGlui( void );
void    Keyboard( unsigned char, int, int );
void    MouseButton( int, int, int, int );
void    MouseMotion( int, int );
float * MulArray3( float, float [3] );
void    Reset( void );
void    Resize( int, int );
void    SetAnimate( int );
void    Sliders( int );
void    Visibility( int );


// main program:
int
main( int argc, char *argv[] )
{
    // turn on the glut package:
    glutInit( &argc, argv );

    // setup all the graphics stuff:
    InitGraphics( );

    // init all the global variables used by Display( ):
    // this will also post a redisplay
    // it is important to call this before InitGlui( ):
    // so that the variables that glui will control are correct
    // when each glui widget is created
    Reset( );

    // setup all the user interface stuff:
    InitGlui( );

    // draw the scene once and wait for some interaction:
    // (will never return)
    glutMainLoop( );

    // this is here to make the compiler happy:
    return 0;
}



 // this is where one would put code that is to be called
 // everytime the glut main loop has nothing to do
 //
 // this is typically where animation parameters are set
 //
 // do not call Display( ) from here -- let glutMainLoop( ) do it
void
Animate( void )
{
    float dx0dt, dy0dt, dz0dt;      // velocities at pt #0
    float dax0dt, day0dt, daz0dt;   // velocities at pt #0
    float dx1dt, dy1dt, dz1dt;      // velocities at pt #1
    float dax1dt, day1dt, daz1dt;   // velocities at pt #1
    float ax, bx, cx, dx;           // x coefficients
    float ay, by, cy, dy;           // y coefficients
    float az, bz, cz, dz;           // z coefficients
    float aax, bax, cax, dax;       // x angle coefficients
    float aay, bay, cay, day;       // y angle coefficients
    float aaz, baz, caz, daz;       // z angle coefficients
    float t;                        // current parameter

    // if the timer is on, figure out what frame we are on:

    // # msec into the cycle ( 0 - MSEC-1 ):
    int msec = glutGet( GLUT_ELAPSED_TIME )  %  MSEC;   // 0 - (MSEC-1)

    // turn that into the current frame number:
    float time01 = (float)msec / (float)(MSEC-1);       // 0. - 1.
    NowFrame = (float)MAXFRAME * time01;

    // fprintf( stderr, "NowFrame = %3d\n", NowFrame );

    for( int i = 0; i < NUMKEYFRAMES - 1; i++ )
    {
        if( KeyFrames[i].f <= NowFrame  &&  NowFrame <= KeyFrames[i+1].f )
        {
            dx0dt = ?????
            dy0dt = ?????
            dz0dt = ?????
            dax0dt = ?????
            day0dt = ?????
            daz0dt = ?????

            dx1dt = ?????
            dy1dt = ?????
            dz1dt = ?????
            dax1dt = ?????
            day1dt = ?????
            daz1dt = ?????

            ax = ?????
            ay = ?????
            az = ?????
            aax = ?????
            aay = ?????
            aaz = ?????

            bx = ?????
            by = ?????
            bz = ?????
            bax = ?????
            bay = ?????
            baz = ?????

            cx = ?????
            cy = ?????
            cz = ?????
            cax = ?????
            cay = ?????
            caz = ?????

            dx = ?????
            dy = ?????
            dz = ?????
            dax = ?????
            day = ?????
            daz = ?????

            t = (float)( NowFrame - KeyFrames[i].f )  /  (float)( KeyFrames[i+1].f - KeyFrames[i].f );
            X = ax + t * ( bx + t * ( cx + t * dx ) );
            Y = ay + t * ( by + t * ( cy + t * dy ) );
            Z = az + t * ( bz + t * ( cz + t * dz ) );
            ThetaX = aax + t * ( bax + t * ( cax + t * dax ) );
            ThetaY = aay + t * ( bay + t * ( cay + t * day ) );
            ThetaZ = aaz + t * ( baz + t * ( caz + t * daz ) );
        }
    }

    // force a call to Display( ) next time it is convenient:
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


 // glui buttons callback:
void
Buttons( int id )
{
    switch( id )
    {
        case RESET:
            Reset( );
            GLUI_Master.set_glutIdleFunc( NULL );
            break;

        case QUIT:
            // gracefully close the glui window:
            // gracefully close out the graphics:
            // gracefully close the graphics window:
            // gracefully exit the program:
            Glui->close( );
            glFinish( );
            glutDestroyWindow( MainWindow );
            exit( 0 );
    }

    Glui->sync_live( );
}


 // draw the complete scene:
void
Display( void )
{
    // set which window we want to do the graphics into:
    glutSetWindow( MainWindow );

    // erase the background:
    glDrawBuffer( GL_BACK );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable( GL_DEPTH_TEST );

    // specify shading to be flat:
    glShadeModel( GL_FLAT );

    // set the viewport to a square centered in the window:
    int dx = glutGet( GLUT_WINDOW_WIDTH );
    int dy = glutGet( GLUT_WINDOW_HEIGHT );
    int d = dx < dy ? dx : dy;          // minimum dimension
    int xl = ( dx - d ) / 2;
    int yb = ( dy - d ) / 2;
    glViewport( xl, yb,  d, d );

    // set the viewing volume:
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    if( WhichProjection == ORTHO )
        glOrtho( -800., 800.,     -800., 800.,     0.8, 4000. );
    else
        gluPerspective( 70., 1.,    10., 4000. );

    // place the object into the viewing volume:
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    gluLookAt( 0., 0., 2000.,   0., 0., 0., 0., 1., 0. );

    // light position:
    glLightfv( GL_LIGHT0, GL_POSITION,  Array3( 0., 1000., 0. ) );
    glLightfv( GL_LIGHT1, GL_POSITION,  Array3( 1000., 0., 0. ) );

    // material color:
    glMaterialfv( GL_FRONT, GL_AMBIENT, OBJECT_COLOR );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, OBJECT_COLOR );

    // rotate the scene:
    glRotatef( Yrot, 0., 1., 0. );
    glRotatef( Xrot, 1., 0., 0. );
    glMultMatrixf( (const GLfloat *) RotMatrix );

    // scale the scene:
    glScalef( Scale, Scale, Scale );
    float scale2 = 1. + Scale2;     // because glui translation starts at 0.
    if( scale2 < MINSCALE )
        scale2 = MINSCALE;
    glScalef( scale2, scale2, scale2 );

    // translate the object:
    glTranslatef( TRANSFACT*TransXYZ[0], TRANSFACT*TransXYZ[1], -TRANSFACT*TransXYZ[2] );
    glTranslatef( -350., -350., -500. );

    // possibly draw the axes:
    if( AxesOn )
    {
        glDisable( GL_LIGHTING );
        glDisable( GL_LIGHT0 );
        glDisable( GL_LIGHT1 );
        glColor3fv( AXES_COLOR );
        glLineWidth( AXES_WIDTH );
        Axes( 400. );
        glLineWidth( 1. );
    }

    // the key positions:
    if( KeyFrameOn == ON )
    {
        glColor3fv( KEYFRAME_COLOR );
        for( int i = 0; i < NUMKEYFRAMES; i++ )
        {
            glPushMatrix( );
                glTranslatef( KeyFrames[i].x, KeyFrames[i].y, KeyFrames[i].z );
                glRotatef( KeyFrames[i].ax,   1., 0., 0. );
                glRotatef( KeyFrames[i].ay,   0., 1., 0. );
                glRotatef( KeyFrames[i].az,   0., 0., 1. );
                glutWireTorus( RADIUS1, RADIUS2, 10, 10 );
            glPopMatrix( );
        }
    }

    // draw the walls:
    glDisable( GL_LIGHTING );
    glDisable( GL_LIGHT0 );
    glDisable( GL_LIGHT1 );
    glColor3fv( FLOOR_COLOR );
    glBegin( GL_QUADS );
        glVertex3f( LEFT_WALL, FLOOR, FRONT_WALL );
        glVertex3f( LEFT_WALL, FLOOR, BACK_WALL );
        glVertex3f( RIGHT_WALL, FLOOR, BACK_WALL );
        glVertex3f( RIGHT_WALL, FLOOR, FRONT_WALL );
    glEnd( );
    glColor3fv( LEFT_WALL_COLOR );
    glBegin( GL_QUADS );
        glVertex3f( LEFT_WALL, FLOOR, FRONT_WALL );
        glVertex3f( LEFT_WALL, FLOOR, BACK_WALL );
        glVertex3f( LEFT_WALL, CEILING, BACK_WALL );
        glVertex3f( LEFT_WALL, CEILING, FRONT_WALL );
    glEnd( );

    // draw the object:
    glPushMatrix( );
        glTranslatef( X, Y, Z );
        glRotatef( ThetaX,   1., 0., 0. );
        glRotatef( ThetaY,   0., 1., 0. );
        glRotatef( ThetaZ,   0., 0., 1. );

        glEnable( GL_LIGHTING );
        glEnable( GL_LIGHT0 );
        glEnable( GL_LIGHT1 );

        glMaterialfv( GL_BACK, GL_AMBIENT, MulArray3( .4f, White ) );
        glMaterialfv( GL_BACK, GL_DIFFUSE, MulArray3( 1., White ) );
        glMaterialfv( GL_BACK, GL_SPECULAR, Array3( 0., 0., 0. ) );
        glMaterialf ( GL_BACK, GL_SHININESS, 5.f );
        glMaterialfv( GL_BACK, GL_EMISSION, Array3( 0., 0., 0. ) );
        glMaterialfv( GL_FRONT, GL_EMISSION, Array3( 0., 0., 0. ) );

        glutSolidTorus( RADIUS1, RADIUS2, 10, 10 );

        glDisable( GL_LIGHTING );
        glDisable( GL_LIGHT0 );
        glDisable( GL_LIGHT1 );
    glPopMatrix( );

    // swap the double-buffered framebuffers:
    glutSwapBuffers( );

    // be sure the graphics buffer has been sent:
    glFlush( );
}


 // use glut to display a string of characters using a raster font:
void
DoRasterString( float x, float y, float z, char *s )
{
    char c;         // one character to print

    glRasterPos3f( x, y, z );
    for( ; ( c = *s ) != '\0'; s++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
    }
}


 // use glut to display a string of characters using a stroke font:
void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
    char c;         // one character to print
    float sf;       // the scale factor

    glPushMatrix( );
        glTranslatef( x, y, z );
        sf = ht / ( 119.05 + 33.33 );
        glScalef( sf, sf, sf );
        for( ; ( c = *s ) != '\0'; s++ )
        {
            glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
        }
    glPopMatrix( );
}

 // initialize the glut and OpenGL libraries:
 // also setup display lists and callback functions
void
InitGraphics( void )
{
    // setup the display mode:
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

    // set the initial window configuration:
    glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

    // open the window and set its title:
    MainWindow = glutCreateWindow( WINDOWTITLE );
    glutSetWindowTitle( WINDOWTITLE );

    // setup the clear values:
    glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

    // nominate the callback routines:

    // DisplayFunc -- redraw the window
    // ReshapeFunc -- handle the user resizing the window
    // KeyboardFunc -- handle a keyboard input
    // MouseFunc -- handle the mouse button going down or up
    // MotionFunc -- handle the mouse moving with a button down
    // PassiveMotionFunc -- handle the mouse moving with a button up
    // VisibilityFunc -- handle a change in window visibility
    // EntryFunc    -- handle the cursor entering or leaving the window
    // SpecialFunc -- handle special keys on the keyboard
    // IdleFunc -- what to do when nothing else is going on
    // TimerFunc -- trigger something to happen every so often

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

    // DO NOT SET THE GLUT IDLE FUNCTION HERE !!
    // glutIdleFunc( NULL );
    // let glui take care of it in InitGlui( )

    glutTimerFunc( 0, NULL, 0 );

    // init the lighting:
    glShadeModel( GL_FLAT );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, MulArray3( .2f, White ) );
    glLightfv( GL_LIGHT0, GL_AMBIENT,   Array3( 0., 0., 0. ) );
    glLightf ( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1. );
    glLightf ( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0. );
    glLightf ( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0. );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,   Array3( 1., 1., 1. )  );
    glLightfv( GL_LIGHT0, GL_SPECULAR,  Array3( 1., 1., 1. )  );
    glLightfv( GL_LIGHT1, GL_AMBIENT,   Array3( 0., 0., 0. ) );
    glLightf ( GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1. );
    glLightf ( GL_LIGHT1, GL_LINEAR_ATTENUATION, 0. );
    glLightf ( GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0. );
    glLightfv( GL_LIGHT1, GL_DIFFUSE,   Array3( .8f, .8f, .8f )  );
    glLightfv( GL_LIGHT1, GL_SPECULAR,  Array3( .8f, .8f, .8f )  );
    glEnable( GL_NORMALIZE );
    glLightModeli ( GL_LIGHT_MODEL_TWO_SIDE, TRUE );
    glMaterialf ( GL_FRONT, GL_SHININESS, 8. );
    glMaterialfv( GL_FRONT, GL_SPECULAR, MulArray3( .7f, Array3( 1., 1., 1. )  ) );

    // init the slopes:
    for( int i = 0; i < NUMKEYFRAMES; i++ )
    {
        if( i == 0 )
        {
            KeyFrames[i].dxdf  = KeyFrames[i].dydf  = KeyFrames[i].dzdf  = 0.;
            KeyFrames[i].daxdf = KeyFrames[i].daydf = KeyFrames[i].dazdf = 0.;
        }
        else
        {
            if( i == NUMKEYFRAMES - 1 )
            {
                KeyFrames[i].dxdf  = KeyFrames[i].dydf  = KeyFrames[i].dzdf  = 0.;
                KeyFrames[i].daxdf = KeyFrames[i].daydf = KeyFrames[i].dazdf = 0.;
            }
            else
            {
                KeyFrames[i].dxdf  = ?????
                KeyFrames[i].dydf  = ?????
                KeyFrames[i].dzdf  = ?????
                KeyFrames[i].daxdf = ?????
                KeyFrames[i].daydf = ?????
                KeyFrames[i].dazdf = ?????
            }
        }
    }
}


 // initialize the glui window:
void
InitGlui( void )
{
    // setup the glui window:
    glutInitWindowPosition( INIT_WINDOW_SIZE+50, 0 );
    Glui = GLUI_Master.create_glui( (char *) GLUITITLE );

    Glui->add_statictext( (char *) GLUITITLE );
    Glui->add_separator( );

    Glui->add_checkbox( "Axes", &AxesOn );
    Glui->add_checkbox( "Perspective", &WhichProjection );
    Glui->add_checkbox( "KeyFrames", &KeyFrameOn );

    GLUI_Panel *panel = Glui->add_panel( "Animation" );
        GLUI_RadioGroup *group = Glui->add_radiogroup_to_panel( panel, &AnimateOn, 0, (GLUI_Update_CB) SetAnimate );
            Glui->add_radiobutton_to_group( group, "Off" );
            Glui->add_radiobutton_to_group( group, "On" );

    panel = Glui->add_panel( "Object Transformation" );
        GluiRot = Glui->add_rotation_to_panel( panel, "Rotation", (float *) RotMatrix );
        GluiRot->set_spin( 1. );

        Glui->add_column_to_panel( panel, FALSE );
        GLUI_Translation *scale = Glui->add_translation_to_panel( panel, "Scale",  GLUI_TRANSLATION_Y , &Scale2 );
        scale->set_speed( 0.01f );

        Glui->add_column_to_panel( panel, FALSE );
        GLUI_Translation *trans = Glui->add_translation_to_panel( panel, "Trans XY", GLUI_TRANSLATION_XY, &TransXYZ[0] );
        trans->set_speed( 0.1f );

        Glui->add_column_to_panel( panel, FALSE );
        trans = Glui->add_translation_to_panel( panel, "Trans Z",  GLUI_TRANSLATION_Z , &TransXYZ[2] );
        trans->set_speed( 0.1f );

    panel = Glui->add_panel( "", FALSE );
    Glui->add_button_to_panel( panel, "Reset", RESET, (GLUI_Update_CB) Buttons );
    Glui->add_column_to_panel( panel, FALSE );
    Glui->add_button_to_panel( panel, "Quit", QUIT, (GLUI_Update_CB) Buttons );
    Glui->add_column_to_panel( panel, FALSE );

    // tell glui what graphics window it needs to post a redisplay to:
    Glui->set_main_gfx_window( MainWindow );

    // set the graphics window's idle function:
    GLUI_Master.set_glutIdleFunc( NULL );
}


 // the keyboard callback:
void
Keyboard( unsigned char c, int x, int y )
{
    switch( c )
    {
        case 'q':
        case 'Q':
        case ESCAPE:
            Buttons( QUIT );    // will not return here
            break;          // happy compiler

        case 'f':
        case 'F':
            KeyFrameOn = ! KeyFrameOn;
            break;

        case 'o':
        case 'O':
            WhichProjection = ORTHO;
            break;

        case 'p':
        case 'P':
            WhichProjection = PERSP;
            break;

        default:
            fprintf( stderr, "Don't know what to do with keyboard hit:: '%c' (0x%0x)\n", c, c );
    }

    // synchronize the GLUI display with the variables:
    Glui->sync_live( );

    // force a call to Display( ):
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


 // called when the mouse button transitions down or up:
void
MouseButton( int button, int state, int x, int y )
{
    int b;          // LEFT, MIDDLE, or RIGHT

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
        ActiveButton &= ~b;     // clear the proper bit
}



 // called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
    int dx = x - Xmouse;        // change in mouse coords
    int dy = y - Ymouse;

    if( ActiveButton & LEFT )
    {
        Xrot += ( ANGFACT*dy );
        Yrot += ( ANGFACT*dx );
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
    glutPostRedisplay( );
}

 
 // reset the transformations and the colors:
 // this only sets the global variables --
 // the glut main loop is responsible for redrawing the scene
 void
Reset( void )
{
    NowFrame = 0;
    X = KeyFrames[0].x;
    Y = KeyFrames[0].y;
    Z = KeyFrames[0].z;
    ThetaX = KeyFrames[0].ax;
    ThetaY = KeyFrames[0].ay;
    ThetaZ = KeyFrames[0].az;
    KeyFrameOn = GLUITRUE;
    AnimateOn  = GLUIFALSE;
    ActiveButton = 0;
    AxesOn = GLUITRUE;
    Scale  = 1.0;
    Scale2 = 0.0;       // because add 1. to it in Display( )
    WhichProjection = PERSP;
    Xrot = Yrot = 0.;
    TransXYZ[0] = TransXYZ[1] = TransXYZ[2] = 0.;

                      RotMatrix[0][1] = RotMatrix[0][2] = RotMatrix[0][3] = 0.;
    RotMatrix[1][0]                   = RotMatrix[1][2] = RotMatrix[1][3] = 0.;
    RotMatrix[2][0] = RotMatrix[2][1]                   = RotMatrix[2][3] = 0.;
    RotMatrix[3][0] = RotMatrix[3][1] = RotMatrix[3][3]                   = 0.;
    RotMatrix[0][0] = RotMatrix[1][1] = RotMatrix[2][2] = RotMatrix[3][3] = 1.;

    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


 // called when user resizes the window:
void
Resize( int width, int height )
{
    // don't really need to do anything since window size is
    // checked each time in Display( ):
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}

void
SetAnimate( int id )
{
    if( AnimateOn )
    {
        GLUI_Master.set_glutIdleFunc( Animate );
    }
    else
    {
        GLUI_Master.set_glutIdleFunc( NULL );
    }

    NowFrame = 0;
}


 // handle a change to the window's visibility:
void
Visibility( int state )
{
    if( state == GLUT_VISIBLE )
    {
        glutSetWindow( MainWindow );
        glutPostRedisplay( );
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

 // Draw a set of 3D axes:
 // (length is the axis length in world coordinates)
void
Axes( float length )
{
    glBegin( GL_LINE_STRIP );
        glVertex3f( length, 0., 0. );
        glVertex3f( 0., 0., 0. );
        glVertex3f( 0., length, 0. );
    glEnd( );
    glBegin( GL_LINE_STRIP );
        glVertex3f( 0., 0., 0. );
        glVertex3f( 0., 0., length );
    glEnd( );

    float fact = LENFRAC * length;
    float base = BASEFRAC * length;

    glBegin( GL_LINE_STRIP );
        for( int i = 0; i < 4; i++ )
        {
            int j = xorder[i];
            if( j < 0 )
            {
                
                glEnd( );
                glBegin( GL_LINE_STRIP );
                j = -j;
            }
            j--;
            glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
        }
    glEnd( );

    glBegin( GL_LINE_STRIP );
        for( int i = 0; i < 5; i++ )
        {
            int j = yorder[i];
            if( j < 0 )
            {
                
                glEnd( );
                glBegin( GL_LINE_STRIP );
                j = -j;
            }
            j--;
            glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
        }
    glEnd( );

    glBegin( GL_LINE_STRIP );
        for( int i = 0; i < 6; i++ )
        {
            int j = zorder[i];
            if( j < 0 )
            {
                
                glEnd( );
                glBegin( GL_LINE_STRIP );
                j = -j;
            }
            j--;
            glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
        }
    glEnd( );
}

float *
MulArray3( float factor, float array0[3] )
{
    static float array[4];

    array[0] = factor * array0[0];
    array[1] = factor * array0[1];
    array[2] = factor * array0[2];
    array[3] = 1.;
    return( array );
}

float *
Array3( float a, float b, float c )
{
    static float array[4];

    array[0] = a;
    array[1] = b;
    array[2] = c;
    array[3] = 1.;
    return array;
}
