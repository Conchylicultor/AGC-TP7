//=============================================================================
//
//   Exercise code for the lecture
//   "Advanced Computer Graphics"
//
//   Adapted from Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) 2013 LGG, epfl
//
//   DO NOT REDISTRIBUTE
//=============================================================================


//== INCLUDES =================================================================

#include "Rigid_body_viewer.h"
#include <utils/gl.h>
#include <cmath>


//== IMPLEMENTATION ========================================================== 


Rigid_body_viewer::Rigid_body_viewer(const char* _title, int _width, int _height)
: Viewer_2D(_title, _width, _height)
{
    animate_             = false;
    time_step_           = 0.001;
    mass_                = 0.5;
    damping_linear_      = 0.1;
    damping_angular_     = 0.0001;
    spring_stiffness_    = 100.0;
    spring_damping_      = 5.0;

    mouse_spring_.active = false;
}


//-----------------------------------------------------------------------------


void Rigid_body_viewer::keyboard(int key, int x, int y)
{
    switch (key)
    {
    case '1':
    {
        std::vector<vec2> p;
        p.push_back( vec2(-0.6, -0.6) );
        p.push_back( vec2(-0.4, -0.6) );
        p.push_back( vec2(-0.4, -0.4) );
        p.push_back( vec2(-0.6, -0.4) );

        body_ = Rigid_body(p, mass_);
        body_.linear_velocity = vec2(5.0, 5.0);
        glutPostRedisplay();
        break;
    }
    case '2':
    {
        std::vector<vec2> p;
        p.push_back( vec2(-0.3, -0.1) );
        p.push_back( vec2(-0.1, -0.1) );
        p.push_back( vec2( 0.1, -0.1) );
        p.push_back( vec2( 0.3, -0.1) );
        p.push_back( vec2( 0.3,  0.1) );
        p.push_back( vec2( 0.1,  0.1) );
        p.push_back( vec2(-0.1,  0.1) );
        p.push_back( vec2(-0.3,  0.1) );

        body_ = Rigid_body(p, mass_);

        glutPostRedisplay();
        break;
    }
    case '3':
    {
        std::vector<vec2> p;
        p.push_back( vec2(-0.5,  0.1) );
        p.push_back( vec2(-0.5,  0.0) );
        p.push_back( vec2( 0.0,  0.0) );
        p.push_back( vec2( 0.0, -0.3) );
        p.push_back( vec2( 0.1, -0.3) );
        p.push_back( vec2( 0.1,  0.0) );
        p.push_back( vec2( 0.3,  0.0) );
        p.push_back( vec2( 0.3,  0.1) );

        body_ = Rigid_body(p, mass_);

        glutPostRedisplay();
        break;
    }
    case '4':
    {
        std::vector<vec2> p;
        p.push_back( vec2(-0.3, -0.1) );
        p.push_back( vec2(-0.1, -0.1) );
        p.push_back( vec2( 0.1, -0.1) );
        p.push_back( vec2( 0.26, -0.1) );
        p.push_back( vec2( 0.27, -0.1) );
        p.push_back( vec2( 0.28, -0.1) );
        p.push_back( vec2( 0.29,  0.1) );
        p.push_back( vec2( 0.3,  0.1) );
        p.push_back( vec2( 0.3,  0.1) );
        p.push_back( vec2( 0.3,  0.1) );
        p.push_back( vec2( 0.1,  0.1) );
        p.push_back( vec2(-0.1,  0.1) );
        p.push_back( vec2(-0.3,  0.1) );

        body_ = Rigid_body(p, mass_);

        glutPostRedisplay();
        break;
    }
    // let parent class do the work
    default:
    {
        Viewer_2D::keyboard(key, x, y);
        break;
    }
    }
}


//-----------------------------------------------------------------------------


void Rigid_body_viewer:: mouse(int _button, int _state, int _x, int _y)
{
    // need points
    if (body_.points.empty())
        return;

    // mouse button release destroys current mouse spring
    if (_state == GLUT_UP)
    {
        mouse_spring_.active = false;
        return;
    }

    // mouse button press generates new mouse spring
    else if (_state == GLUT_DOWN)
    {
        // get point under mouse cursor
        vec2 p = pick(_x, _y);

        // find closest body point
        unsigned int i, imin;
        float dmin = FLT_MAX;
        for (i=0; i<body_.points.size(); ++i)
        {
            float d = distance(p, body_.points[i]);
            if (d < dmin)
            {
                dmin = d;
                imin = i;
            }
        }

        // setup the mouse spring
        mouse_spring_.active = true;
        mouse_spring_.particle_index = imin;
        mouse_spring_.mouse_position = p;
    }

    glutPostRedisplay();
}


//-----------------------------------------------------------------------------


void Rigid_body_viewer:: motion(int _x, int _y)
{
    if (mouse_spring_.active)
    {
        // update mouse position
        mouse_spring_.mouse_position = pick(_x, _y);
        glutPostRedisplay();
    }
}


//-----------------------------------------------------------------------------


void Rigid_body_viewer:: draw()
{
    // parent's status text
    Viewer_2D::draw();

    // draw walls
    glDisable(GL_LIGHTING);
    glLineWidth(1.0);
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_LINE_STRIP);
    glVertex2f( -1.0,  1.0 );
    glVertex2f( -1.0, -1.0 );
    glVertex2f(  1.0, -1.0 );
    glVertex2f(  1.0,  1.0 );
    glVertex2f( -1.0,  1.0 );
    glEnd();

    // draw rigid body
    body_.draw();

    // draw mouse spring
    if (mouse_spring_.active)
    {
        glLineWidth(5.0);
        glColor3f(1,0,0);
        glBegin(GL_LINES);
        glVertex2fv( body_.points[ mouse_spring_.particle_index ].data() );
        glVertex2fv( mouse_spring_.mouse_position.data() );
        glEnd();
    }
}


//-----------------------------------------------------------------------------

vec2 localToWorldRotation(float theta, vec2 localVector){
    float c = std::cos(theta);
    float s = std::sin(theta);

    return vec2(c*localVector[0]-s*localVector[1],
                s*localVector[0]+c*localVector[1]);
}

void Rigid_body_viewer::compute_forces()
{ 
    /** \todo Compute all forces acting on the rigid body
     \li clear all forces
     \li add gravity
     \li add damping to linear and angular movement
     \li add the mouse spring force
     */

    // Clear forces
    body_.force = vec2(0.0f,0.0f);
    // Clear torque
    body_.torque = 0.0f;

    // /!\ Warning: Dont calculate another force here, or the torque will be wrong!!!!!

    // Add gravity
    // Force
    body_.force += vec2(0,-9.81*body_.mass);
    // Torque
    for (unsigned int i=0; i<body_.points.size(); ++i)
    {
        body_.torque += dot(perp(body_.r.at(i)), body_.force/body_.points.size());
    }
    // std::cout << body_.torque << std::endl;
    // std::cout << body_.force/body_.points.size() << std::endl;

    // Damping
    // Linear
    body_.force  += -1.0f * damping_linear_*body_.linear_velocity;
    // Angular
    body_.torque += -1.0f * damping_angular_*body_.angular_velocity;

    //Mouse
    //Force
    if (mouse_spring_.active)
    {
        //Calculate the position of in world coordinates
        vec2 pos0 = body_.position + localToWorldRotation(-body_.orientation, body_.r[ mouse_spring_.particle_index ]);
        vec2 pos1 = mouse_spring_.mouse_position;

        vec2 vel0 = body_.linear_velocity + body_.angular_velocity*localToWorldRotation(-body_.orientation, perp(body_.r[mouse_spring_.particle_index])); // NO GLOBAL COORDINATES ????
        vec2 vel1 = vec2(0,0);

        float d = norm(pos0 - pos1);

        vec2 mouseSpringForce = -1 * (spring_stiffness_*d + spring_damping_*dot(vel0-vel1, pos0-pos1)/d) * ((pos0-pos1)/d);

        //Add the force
        body_.force += mouseSpringForce;

        //Add the torque
        body_.torque += dot(localToWorldRotation(-body_.orientation, perp(body_.r.at(mouse_spring_.particle_index))), mouseSpringForce);

        //std::cout << localToWorldRotation(-body_.orientation, perp(body_.r.at(mouse_spring_.particle_index))) -
        //             perp(localToWorldRotation(-body_.orientation, body_.r.at(mouse_spring_.particle_index)))<< std::endl;
    }
}

//-----------------------------------------------------------------------------


void Rigid_body_viewer::impulse_based_collisions()
{
    /** \todo Handle collisions based on impulses
     */

    float planes[4][3] = {
            {  0.0,  1.0, 1.0 },
            {  0.0, -1.0, 1.0 },
            {  1.0,  0.0, 1.0 },
            { -1.0,  0.0, 1.0 }
        };
    float epsilon = 1.0f;

    // For each pts
    for (int p=0; p<body_.points.size(); p++)
    {
        // For each plane
        for (int i=0; i<4;i++)
        {
            //detect collision
            vec2 n(planes[i][0], planes[i][1]);
            float dist= dot(n,body_.points[p])+ planes[i][2];
            if (dist < 0.0f &&
                dot(body_.linear_velocity,n) < 0.0f)
            {
                // std::cout << dist << std::endl;


                // Compute relative velocity
                float vrel = dot(n ,body_.linear_velocity + body_.angular_velocity*localToWorldRotation(-body_.orientation, perp(body_.r[p])));
                float temp = dot(localToWorldRotation(-body_.orientation, perp(body_.r[p])), n);
                // Compute j
                float j = -(1+ epsilon)*vrel/(1/body_.mass + (temp*temp)/body_.inertia);

                // Compute J = j.n
                vec2 J = j*n;

                vec2 deltaV = J/body_.mass;
                float deltaW = dot(localToWorldRotation(-body_.orientation, perp(body_.r[p])),J)/body_.inertia;

                body_.linear_velocity += deltaV;
                body_.angular_velocity += deltaW;
             }
        }
    }
}


//-----------------------------------------------------------------------------


void Rigid_body_viewer::time_integration(float dt)
{
    // compute all forces
    compute_forces();

    /** \todo Implement explicit Euler time integration
     \li update position and orientation
     \li update linear and angular velocities
     \li call update_points() at the end to compute the new particle positions
     */
    // Euler step
    // Use old or new velocity ??

    // Update position
    body_.linear_velocity += dt*body_.force/body_.mass; // v = v + hF/M
    body_.position += dt*body_.linear_velocity; // x = x + hv
    //body_.linear_velocity += dt*body_.force/body_.mass; // v = v + hF/M

    // Update orientation
    body_.angular_velocity += dt*body_.torque/body_.inertia;
    body_.orientation += dt*body_.angular_velocity;
    //body_.angular_velocity += dt*body_.torque/body_.inertia;

    // Update particles
    body_.update_points();

    // handle collisions
    impulse_based_collisions();
}


//=============================================================================
