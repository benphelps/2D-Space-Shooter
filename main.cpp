/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include "timer.h"
#include "ship.h"

//The screen sttributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The attributes of the ship
const int SHIP_WIDTH = 20;
const int SHIP_HEIGHT = 20;
const int SHIP_VEL = 200;

//The surfaces
SDL_Surface *ship = NULL;
SDL_Surface *screen = NULL;

const int FOO_RIGHT = 0;
const int FOO_LEFT = 1;
const int FOO_CENTER = 3;

//The areas of the sprite sheet
SDL_Rect clipRight;
SDL_Rect clipLeft;
SDL_Rect clipCenter;

//The event structure
SDL_Event event;



void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

void set_clips()
{
    //Clip the sprites
    clipRight.x = 101;
    clipRight.y = 0;
    clipRight.w = 48;
    clipRight.h = 44;

    clipLeft.x = 0;
    clipLeft.y = 0;
    clipLeft.w = 48;
    clipLeft.h = 44;

    clipCenter.x = 48;
    clipCenter.y = 0;
    clipCenter.w = 54;
    clipCenter.h = 44;
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Move the Ship", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the ship image
    ship = load_image( "ship-sprite.png" );

    //If there was a problem in loading the ship
    if( ship == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( ship );

    //Quit SDL
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    //Quit flag
    bool quit = false;

    //The ship that will be used
    Ship myShip;

    //Keeps track of time since last rendering
    Timer delta;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Start delta timer
    delta.start();

    //While the user hasn't quit
    while( quit == false )
    {
        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //Handle events for the ship
            myShip.handle_input();

            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

        //Move the ship
        myShip.move( delta.get_ticks() );

        //Restart delta timer
        delta.start();

        //Fill the screen white
        //SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

        //Show the ship on the screen
        myShip.show();

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    }

    //Clean up
    clean_up();

    return 0;
}
