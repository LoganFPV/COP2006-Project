/** --------------------------------------------------------
 *    File: Pong Final.cpp
 *  Author: Logan Devore
 *   Class: COP 2006, Spring 2022
 * Purpose: Adding AI and Paddle collision
 * -------------------------------------------------------- */

#include "pong_defs.h"

//Function Declaration (Prototypes)
Direction processInput() ;
void setup(Ball &ball, Borders &theWalls, Moving_Block &player, Moving_Block &ai_player, paddle paddle_prop);
bool update(Direction &input, Ball &ball, Borders walls, Moving_Block &player,Moving_Block &ai_player, paddle paddle_prop,bool &started,bool &game_over, float delta);
void render(sf::RenderWindow &window, Ball ball, Borders walls, Moving_Block &player, Moving_Block &ai_player, paddle &paddle_prop, float delta);
bool collisionChecking(Ball ball, Block walls);
bool paddleCollision(Moving_Block player_checking, Block wallCheck);
bool doCollisionChecking(Ball &ball, Borders &Wall, Moving_Block &player, Moving_Block &ai_player, bool game_over);

/*
 * The main application
 * @return OS stats message (0=Success)
 */
int main() {
    // create a 2d graphics window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pong");
    window.clear(WINDOW_COLOR);

    //declare a ball variable and set it to the middle of the window
    Direction input;
    Ball theBall;
    Borders theBlock;
    Moving_Block player;
    Moving_Block ai_player;
    paddle paddle_prop;
    setup(theBall, theBlock, player,ai_player, paddle_prop);

    // timing variables for the main game loop
    sf::Clock clock;
    sf::Time startTime = clock.getElapsedTime();
    sf::Time stopTime = startTime;
    float delta = 0.0;

    bool started = false;
    bool gameOver = false;
    bool game_over = false;
    while (!gameOver)
    {

        // calculate frame time
        stopTime = clock.getElapsedTime();
        delta += (stopTime.asMilliseconds() - startTime.asMilliseconds());
        startTime = stopTime;
        // process events
        sf::Event event;
        while (!gameOver && window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                gameOver = true;
        }

        // Process user input
        Direction userInput = processInput();
        if (userInput == Exit)
            gameOver = true;

        // Process Updates
        if (delta >= FRAME_RATE && !gameOver) {    // if we have made it at least a full frame time
                    gameOver = update(userInput, theBall, theBlock, player, ai_player, paddle_prop, started, game_over, delta);
            // subtract the frame-rate from the current frame-time
            // for each full frame covered by this updated
            while (delta >= FRAME_RATE)
                delta -= FRAME_RATE;
        }

        // Render the window
        render(window, theBall, theBlock, player, ai_player, paddle_prop, delta);
    } // end main game loop

    // game ending, close the graphics window
    window.close();
    return 0;   // return success to the OS
} // end main


/**
 * Initialising code for Balls, walls, paddles
 * @param ball
 * @param borders
 * @param player
 */
void setup(Ball &ball, Borders &theWalls, Moving_Block &player, Moving_Block &ai_player, paddle paddle_prop){

//Placing the borders on the screen
    theWalls.leftWall.left = 0.0;
    theWalls.leftWall.top = 0.0;
    theWalls.leftWall.width = WALL_THICKNESS;
    theWalls.leftWall.height = WINDOW_HEIGHT;
    theWalls.leftWall.rectangle.setSize(sf::Vector2(theWalls.leftWall.width, theWalls.leftWall.height));
    theWalls.leftWall.rectangle.setPosition(theWalls.leftWall.left, theWalls.leftWall.top);
    theWalls.leftWall.rectangle.setFillColor(WALL_COLOR);

    theWalls.topWall.left = 0.0;
    theWalls.topWall.top = 0.0;
    theWalls.topWall.width = WINDOW_WIDTH;
    theWalls.topWall.height = WALL_THICKNESS;
    theWalls.topWall.rectangle.setSize(sf::Vector2(theWalls.topWall.width, theWalls.topWall.height));
    theWalls.topWall.rectangle.setPosition(theWalls.topWall.left, theWalls.topWall.top);
    theWalls.topWall.rectangle.setFillColor(WALL_COLOR);

    theWalls.rightWall.left = WINDOW_WIDTH - WALL_THICKNESS;
    theWalls.rightWall.top = 0.0;
    theWalls.rightWall.width = WALL_THICKNESS;
    theWalls.rightWall.height = WINDOW_HEIGHT;
    theWalls.rightWall.rectangle.setSize(sf::Vector2(theWalls.rightWall.width, theWalls.rightWall.height));
    theWalls.rightWall.rectangle.setPosition(theWalls.rightWall.left, theWalls.rightWall.top);
    theWalls.rightWall.rectangle.setFillColor(WALL_COLOR);

    theWalls.bottomWall.left = 0.0;
    theWalls.bottomWall.top = WINDOW_HEIGHT - WALL_THICKNESS;
    theWalls.bottomWall.width = WINDOW_WIDTH;
    theWalls.bottomWall.height = WALL_THICKNESS;
    theWalls.bottomWall.rectangle.setSize(sf::Vector2(theWalls.bottomWall.width, theWalls.bottomWall.height));
    theWalls.bottomWall.rectangle.setPosition(theWalls.bottomWall.left, theWalls.bottomWall.top);
    theWalls.bottomWall.rectangle.setFillColor(WALL_COLOR);

    //Players Paddle
    player.paddle_Player.left = paddle_prop.PADDLE_THICKNESS;
    player.paddle_Player.top = (WINDOW_HEIGHT - paddle_prop.PADDLE_HEIGHT) / 2.0;
    player.paddle_Player.width = paddle_prop.PADDLE_THICKNESS;
    player.paddle_Player.height = paddle_prop.PADDLE_HEIGHT;
    player.paddle_Player.color = paddle_prop.PADDLE_COLOR;
    player.paddle_Player.rectangle.setSize(sf::Vector2(player.paddle_Player.width, player.paddle_Player.height));
    player.paddle_Player.rectangle.setFillColor(paddle_prop.PADDLE_COLOR);
    player.velocityX = 0.0f;
    player.velocityY = 0.0f;
    
    //AI Paddle
    ai_player.paddle_Player.left = theWalls.rightWall.left - 2 * paddle_prop.PADDLE_THICKNESS;
    ai_player.paddle_Player.top = (WINDOW_HEIGHT - paddle_prop.PADDLE_HEIGHT) / 2.0;
    ai_player.paddle_Player.width = paddle_prop.PADDLE_THICKNESS;
    ai_player.paddle_Player.height = paddle_prop.PADDLE_HEIGHT;
    ai_player.paddle_Player.color = paddle_prop.PADDLE_COLOR;
    ai_player.paddle_Player.rectangle.setSize(sf::Vector2(ai_player.paddle_Player.width, ai_player.paddle_Player.height));
    ai_player.paddle_Player.rectangle.setFillColor(paddle_prop.PADDLE_COLOR);
    ai_player.velocityX = 0.0f;
    ai_player.velocityY = 0.0f;

    // declare a ball variable and populate it in the center of the window
    ball.radius = BALL_RADIUS;
    //ball.coordinateX = WINDOW_WIDTH / 2.0;
    //ball.coordinateY = WINDOW_HEIGHT / 2.0;
    ball.coordinateX = player.paddle_Player.left + paddle_prop.PADDLE_THICKNESS + BALL_RADIUS + 1.0f;
    ball.coordinateY = player.paddle_Player.top + (paddle_prop.PADDLE_HEIGHT / 2.0f);
    ball.velocityX = 0.0;
    ball.velocityY = 0.0;
    ball.color = BALL_COLOR;
} //end setup


/**
 * convert user keyboard input into recognized integer values
 * for left=1/up=2/right=3/down=4
 * @return int - user input (default no-input=0, quit=-1)
 */
Direction processInput() {
    Direction input = None;  // no input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        input = Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        input = Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        input = Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        input = Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        input = Exit;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        input = start;
    }

    //For arrow Key input
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        input = Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        input = Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        input = Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        input = Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        input = Exit;
    }
    return input;
} //End getUserInput


/**
 * update the state of game objects
 * @param input - user keyboard input
 * @param ball  - update ball position and speed
 * @param player - player block
 * @param ai_player - ai player block
 * @param started - start the game
 * @param game_over - end the game
 * @param delta - current frame time
 */
bool update(Direction &input, Ball &ball, Borders Wall, Moving_Block &player, Moving_Block &ai_player, paddle paddle_prop,bool &started, bool &game_over, float delta) {

    game_over = false;
    // adjust velocity directions for user input
    if (input) {
        switch (input) {
            case Left: // Left
            //player.velocityX -= paddle_prop.PADDLE_SPEED;
            //ball.velocityX -= BALL_SPEED_X; //<-- if not commented makes paddle move everywhere on screen
                break;
            case Up: // Up
            player.velocityY -= paddle_prop.PADDLE_SPEED;
            //ball.velocityY -= BALL_SPEED_Y; //<-- if not commented makes paddle move everywhere on screen
                break;
            case Right: // Right
            //player.velocityX += paddle_prop.PADDLE_SPEED;
            //ball.velocityX += BALL_SPEED_X;  //<-- if not commented makes paddle move everywhere on screen
                break;
            case Down: // Down
            player.velocityY += paddle_prop.PADDLE_SPEED;
            //ball.velocityY += BALL_SPEED_Y; //<-- if not commented makes paddle move everywhere on screen
                break;
            case start: //Start
            if(!started){
                ball.velocityX = BALL_SPEED_X;
                ball.velocityY = BALL_SPEED_Y;
                if((int(delta * 10) & 1) % 2) {
                    ball.velocityY *= -1; //ball goes down if odd
                }
                started = true;
            }
                break;
        }
        // clear input
        input = None;
    }

    // player
    //player.paddle_Player.left += player.velocityX * delta;  //<-- if not commented makes paddle move everywhere on screen
    player.paddle_Player.top += player.velocityY * delta;     //<-- comment this if you want the paddle to move anywhere on screen
    ai_player.paddle_Player.top += ai_player.velocityY * delta;
    //starts ball movement
    if (started) {
        ball.coordinateX += ball.velocityX * delta;
        ball.coordinateY += ball.velocityY * delta;
        if(ball.coordinateY < ai_player.paddle_Player.top + ai_player.paddle_Player.height / 2.0f){
            ai_player.velocityY -= paddle_prop.PADDLE_SPEED;
        }
        else if(ball.coordinateY > ai_player.paddle_Player.top + ai_player.paddle_Player.height / 2.0f){
            ai_player.velocityY += paddle_prop.PADDLE_SPEED;
        }
    }
    else{ //Adjust the location of the ball for speed * time
        ball.coordinateX = player.paddle_Player.left + paddle_prop.PADDLE_THICKNESS + BALL_RADIUS + 1.0;
        ball.coordinateY = player.paddle_Player.top + (paddle_prop.PADDLE_HEIGHT / 2.0);
    }
    //starts ball movement
    doCollisionChecking(ball, Wall, player, ai_player, game_over);
} //End update


/**
 * draw the ball on the graphics window and render graphics
 * @param window - handle to open graphics window
 * @param ball   - structure variable with properties for the ball
 * @param wall   -structure for walls
 * @param player - Player paddle movement
 * @param ai_player - ai player paddle movement
 * @param delta  - amount of frame time plus lag (in ms)
 */
void render(sf::RenderWindow &window,Ball ball, Borders wall,Moving_Block &player, Moving_Block &ai_player, paddle &paddle_prop, float delta) {
    // clear the window with the background color
    window.clear(WINDOW_COLOR);
    // draw the ball
    sf::CircleShape circle;
    circle.setFillColor(BALL_COLOR);
    circle.setRadius(ball.radius);

    // set screen coordinates relative to the center of the circle
    circle.setOrigin( ball.radius, ball.radius);

    // calculate current drawing location relative to speed and frame-time
    float xPosition = ball.coordinateX + ball.velocityX * delta;
    float yPosition = ball.coordinateY + ball.velocityY * delta;
    circle.setPosition(xPosition, yPosition);
    window.draw(circle);

    //Draw the paddle
    float xPaddlePosition = player.paddle_Player.left + player.velocityX * delta + 10;
    float yPaddlePosition = player.paddle_Player.top + player.velocityY * delta + 10;
    player.paddle_Player.rectangle.setPosition(xPaddlePosition, yPaddlePosition);
    window.draw(player.paddle_Player.rectangle);

    //Draw the AI paddle
    float x_paddle_PlayerPosition = ai_player.paddle_Player.left + ai_player.velocityX * delta + 0.0f;
    float y_paddle_PlayerPosition = ai_player.paddle_Player.top + ai_player.velocityY * delta + 0.0f;
    ai_player.paddle_Player.rectangle.setPosition(x_paddle_PlayerPosition, y_paddle_PlayerPosition);
    window.draw(ai_player.paddle_Player.rectangle);

    // Draw Walls
    window.draw(wall.leftWall.rectangle);
    window.draw(wall.topWall.rectangle);
    window.draw(wall.rightWall.rectangle);
    window.draw(wall.bottomWall.rectangle);
    window.display();
} //End render


bool collisionChecking(Ball ball, Block walls){
    bool collisions = false;

    //ball collision properties
    float ball_left = ball.coordinateX - ball.radius;
    float ball_top = ball.coordinateY - ball.radius;
    float ball_right = ball.coordinateX + ball.radius;
    float ball_bottom = ball.coordinateY + ball.radius;

    //creating the "rectangle" around the ball
    float left_box = walls.left;
    float top_box = walls.top;
    float right_box = walls.left + walls.width;
    float bottom_box = walls.top + walls.height;

    //if statement for collision checking
    if (ball_top < bottom_box &&
        ball_right > left_box &&
        ball_left < right_box &&
        ball_bottom > top_box){
        collisions = true;
    }
    return collisions;
}//End collisionChecking

//collision checking for paddle and walls
bool paddleCollision(Moving_Block player_checking, Block wallCheck){
    bool collision = false;
    if(player_checking.paddle_Player.left < wallCheck.left + wallCheck.width &&
    player_checking.paddle_Player.left + player_checking.paddle_Player.width > wallCheck.left &&
    player_checking.paddle_Player.top < wallCheck.top + wallCheck.height &&
    player_checking.paddle_Player.top + player_checking.paddle_Player.height > wallCheck.top){
        collision = true;
    }
    return collision;
}//End paddleCollision

//doing collision checking for ball, wall, and paddles
bool doCollisionChecking(Ball &ball, Borders &Wall, Moving_Block &player, Moving_Block &ai_player, bool game_over){
    game_over = false;
    //collision for paddle and Ball
    if(collisionChecking(ball, player.paddle_Player)){
        //ball.velocityX *= -1;
        //ball.coordinateX = player.paddle_Player.left + player.paddle_Player.width + ball.radius + 1;
        if(ball.coordinateY > player.paddle_Player.top + player.paddle_Player.height * 0.2){
            ball.velocityX *= -1.0;
            ball.velocityY *= +1.2;
        }
        else if(ball.coordinateY == player.paddle_Player.top + player.paddle_Player.height / 1.5 && ball.coordinateY == player.paddle_Player.top + player.paddle_Player.height / 2.5){
            ball.velocityX *= -2.0;
            ball.velocityY *= -1.0;
        }
        else if(ball.coordinateY < ai_player.paddle_Player.top + ai_player.paddle_Player.height * 0.8){
            ball.velocityX *= -1.0;
            ball.velocityY *= +1.2;
        }

    }

    //Collision for ai_paddle and bal
    else if(collisionChecking(ball, ai_player.paddle_Player)){
        ball.velocityX *= -1.1;
        //ball.coordinateX + ai_player.paddle_Player.left + ai_player.paddle_Player.width + ball.radius + 1;
    }

    //ball and walls collision checking
    else if(collisionChecking(ball, Wall.leftWall)){
        ball.velocityX *= -1;
        ball.coordinateX = Wall.leftWall.left + Wall.leftWall.width + ball.radius + 1;
        game_over = true;
    }
    else if(collisionChecking(ball, Wall.rightWall)){
        ball.velocityX *= -1;
        ball.coordinateX = Wall.rightWall.left - ball.radius - 1;
        game_over = true;
    }
    else if (collisionChecking(ball, Wall.topWall)) {
        ball.velocityY *= - 1;
        ball.coordinateY = Wall.topWall.top + Wall.topWall.height + ball.radius + 1;
    }
    else if(collisionChecking(ball, Wall.bottomWall)){
        ball.velocityY *= - 1;
        ball.coordinateY = Wall.bottomWall.top - ball.radius - 1;
    }

    //Paddle and wall collision checking
    if (paddleCollision(player, Wall.topWall)){
        player.velocityY = 0.0f;
        player.paddle_Player.top = Wall.topWall.top + Wall.topWall.height;
    }
    else if (paddleCollision(player, Wall.bottomWall)){
        player.velocityY = 0.0f;
        player.paddle_Player.top  = Wall.bottomWall.top - player.paddle_Player.height;
    }

    //ai paddle collision with walls
    if (paddleCollision(ai_player, Wall.topWall)){
        ai_player.velocityY = 0.0f;
        ai_player.paddle_Player.top = Wall.topWall.top + Wall.topWall.height;
    }
    else if (paddleCollision(ai_player, Wall.bottomWall)){
        ai_player.velocityY = 0.0f;
        ai_player.paddle_Player.top  = Wall.bottomWall.top - ai_player.paddle_Player.height;
    }

    return game_over;
}//End doCollisionChecking
