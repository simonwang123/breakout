//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(320, 240);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = 3;

    // number of points initially
    int points = 0;

    // keep playing until game over
    double yvelocity = 1.0;
    double xvelocity = 1.0;
    while (lives > 0 && bricks > 0)
    {         
        while (true)
        {   
            GEvent event = getNextEvent(MOUSE_EVENT);
            if (event != NULL)
            {
                if (getEventType(event) == MOUSE_MOVED)
                {
                    double x  = getX(event) - 25;                    
                    setLocation (paddle, x, getHeight(window) - 30);                    
                }               
            }
            GObject object = detectCollision(window, ball);
            if (object != NULL)
            {
                if (strcmp(getType(object), "GRect") == 0)   
                {   
                    if (getY(object) < 200)
                    {
                        removeGWindow(window, object);
                        bricks = bricks - 1;
                        points = points + 1;
                        updateScoreboard(window, label, points);
                    }
                    yvelocity = -yvelocity;
                }
            }
            move(ball, xvelocity, yvelocity);
            if (getX(ball) + getWidth(ball) == getWidth(window))
            {
                xvelocity = -xvelocity;    
            }
            else if (getX(ball) == 0)
            {
                xvelocity = -xvelocity;
            }          
            else if (getY(ball) == 0)
            {
                yvelocity = -yvelocity;
            } 
            if (getY(ball) + getHeight(ball) == getHeight(window))
            {
                addAt(window,ball, (getWidth(window) / 2) - 5, (getHeight(window) / 2) - 5  );
                lives = lives - 1;  
                break;                             
            }                                       
            pause(10);
        }  
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    GRect bricks = newGRect(0, 0, 30, 5); 
    int hspace = 5;
    int vspace = 5;
    
    for (int p = 0; p < 5 ; p++)
    {
        for (int i = 0; i < 10 ; i++)
        {
            GRect bricks = newGRect((30 * i) + hspace, vspace , 30, 5); 
            if (p == 0)
            {
                setColor(bricks, "RED");
            }
            if (p == 1)
            {
                setColor(bricks, "ORANGE");
            }
            if (p == 2)
            {
                setColor(bricks, "YELLOW");
            }
            if (p == 3)
            {
                setColor(bricks, "GREEN");
            }
            if (p == 4)
            {
                setColor(bricks, "BLUE");
            }
            setFilled(bricks, true);
            add(window, bricks);
            hspace = hspace + 5;
        }
    vspace = vspace + 10;
    hspace = 5;
    }
    
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // TODO
    GOval ball = newGOval((getWidth(window) / 2) - 5, (getHeight(window) / 2) - 5, 10, 10);
    setColor(ball, "BLACK");
    setFilled(ball, true);
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect((getWidth(window) / 2) - 25, getHeight(window) - 30, 50, 5);
    setColor(paddle, "BLACK");
    setFilled(paddle, true);
    add(window, paddle);
    
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-36");
    add(window,label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
