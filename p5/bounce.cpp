void
Bounce( float dt )
{
    while( dt > EPSILON )
    {
        float tmin = dt;    // minimum time to do something
        int which = NOTHING_HIT; // which reason was it for doing the something

        //printf("tmin: %f\n", tmin);

        // these four collision times are computed using your projectile motion equations:

        float tleft = (XLEFT + RADIUS - Xnow) / Vxnow;  // time to hit the left wall
        //printf("tleft: %f\n", tleft);
        if( tleft > 0.  &&  tleft < tmin )
        {
            printf("hitleft1");
            tmin = min(tmin, tleft);
            if(tleft == tmin) which = HIT_LEFT;
        }
        float tright = (XRIGHT - RADIUS - Xnow) / Vxnow;    // time to hit the right wall
        //printf("tright: %f\n", tright);
        if( tright > 0.  &&  tright < tmin )
        {
            printf("hitright1");
            tmin = min(tmin, tright);
            if(tright == tmin) which = HIT_RIGHT;
        }
//      float tfloor1 = ????;   // time to hit the floor
//      if( tfloor1 > 0.  &&  tfloor1 < tmin )
//      {
//          ?????
//      }
//      float tfloor2 = ????;   // time to hit the floor (note there are 2 answers)
//      if( tfloor2 > 0.  &&  tfloor2 < tmin )
//      {
//          ?????
//      }

        // tmin is now set to the smallest of:
        //  dt, tleft, tright, tfloor1, tfloor2
        // which are positive

        // which is set to:
        //  NOTHING_HIT, HIT_LEFT, HIT_RIGHT, HIT_FLOOR1, or HIT_FLOOR2
        // to show what was the first thing hit

        // take a time step of time length tmin, using the projectile motion equations:
        // if a bounce is going to occur, tmin takes the ball right up to the surface:

        Xnow += Vxnow * tmin;
        Ynow += Vynow * tmin;
        Vxnow = Vxnow;
        Vynow += Gravity * tmin;

        // a "bounce trick":

        if( Ynow < YBOTTOM+RADIUS )
            Ynow = YBOTTOM+RADIUS;      // floating point roundoff sometimes messes up

        if( Xnow < XLEFT+RADIUS )
            Xnow = XLEFT+RADIUS;

        if( Xnow > XRIGHT-RADIUS )
            Xnow = XRIGHT-RADIUS;


        // depending on what caused the bounce, change the proper velocity component:
        // if nothing was hit in this time step, just return:

        switch( which )
        {
            case NOTHING_HIT:
                return;

            case HIT_LEFT:
                printf("hitleft2");
                Vxnow = -CoefRest * Vxnow;
                break;

            case HIT_RIGHT:
                printf("hitright2");
                Vxnow = -CoefRest * Vxnow;
                break;

            //case HIT_FLOOR1:
            //  ?????
            //  break;

            //case HIT_FLOOR2:
            //  ?????
            //  break;
        }

        dt -= tmin; // after the bounce, we might still have some time step left
    }
}
