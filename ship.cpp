/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

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

//The ship
class Ship
{
    private:
    //The X and Y offsets of the ship
    float x, y;

    //The velocity of the ship
    float xVel, yVel;
    
    int status;

    public:
    //Initializes the variables
    Ship();

    //Takes key presses and adjusts the ship's velocity
    void handle_input();

    //Moves the ship
    void move( Uint32 deltaTicks );

    //Shows the ship on the screen
    void show();
};

//The timer
class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

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

Ship::Ship()
{
    //Initialize the offsets
    x = 0;
    y = 0;

    //Initialize the velocity
    xVel = 0;
    yVel = 0;
}

void Ship::handle_input()
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel -= SHIP_VEL; break;
            case SDLK_DOWN: yVel += SHIP_VEL; break;
            case SDLK_LEFT: xVel -= SHIP_VEL; break;
            case SDLK_RIGHT: xVel += SHIP_VEL; break;
        }
    }
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel += SHIP_VEL; break;
            case SDLK_DOWN: yVel -= SHIP_VEL; break;
            case SDLK_LEFT: xVel += SHIP_VEL; break;
            case SDLK_RIGHT: xVel -= SHIP_VEL; break;
        }
    }
}

void Ship::move( Uint32 deltaTicks )
{
    //Move the ship left or right
    x += xVel * ( deltaTicks / 1000.f );

    //If the ship went too far to the left
    if( x < 0 )
    {
        //Move back
        x = 0;
    }
    //or the right
    else if( x + SHIP_WIDTH > SCREEN_WIDTH )
    {
        //Move back
        x = SCREEN_WIDTH - SHIP_WIDTH;
    }

    //Move the ship up or down
    y += yVel * ( deltaTicks / 1000.f );

    //If the ship went too far up
    if( y < 0 )
    {
        //Move back
        y = 0;
    }
    //or down
    else if( y + SHIP_HEIGHT > SCREEN_HEIGHT )
    {
        //Move back
        y = SCREEN_HEIGHT - SHIP_HEIGHT;
    }
    //If Foo is moving left
    if( xVel < 0.0 )
    {
        status = FOO_LEFT;
    }
    //If Foo is moving right
    else if( xVel > 0.0 )
    {
        status = FOO_RIGHT;
    }
    //If Foo standing
    else
    {
        status = FOO_CENTER;
    }
}

void Ship::show()
{
    printf("%i\n", status);
    //Show the stick figure
    if( status == FOO_RIGHT )
    {
        apply_surface( (int)x, (int)y, ship, screen, &clipRight );
    }
    else if( status == FOO_LEFT )
    {
        apply_surface( (int)x, (int)y, ship, screen, &clipLeft );
    }
    else if( status == FOO_CENTER )
    {
        apply_surface( (int)x, (int)y, ship, screen, &clipCenter );
    }
    else {
        apply_surface( (int)x, (int)y, ship, screen );
    }
    
}

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
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
