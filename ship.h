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
