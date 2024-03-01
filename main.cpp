#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
using namespace sf;

struct point //plat position
{
	int x, y;
};

void show(); //extra window showing time
void end(); //close window
int answer(int whichQ, int num); //which answer is correct
string intconvert(int point); //convert int to string

const int W = 400;//window width
const int H = 533;//window length
const int board = 30;//the board from up, left, right
const int L2 = 200, L3 = 500;//the point you need to reach different level

RenderWindow app(VideoMode(W, H), "Professor Jump");//window initialize and name
Sound jump, correct, wrong, shooo;//sound declare

int main()
{
	app.setFramerateLimit(60); // game size
	srand(time(0)); //create random positoin  
	
	Text gamescore, deduct, textSkel, bestscore;
	Font scorefont;
	scorefont.loadFromFile("./fonts/Capsmall.ttf"); //initialize text show int the game
	
	Texture tstart;
	tstart.loadFromFile("./images/component/start menu.png"); // intro menu display
	Sprite sstartground(tstart);
	
	int bestpoint = 0; //the highest point you reach
	
	while (app.isOpen())
	{
		end();
		app.draw(sstartground);
		app.display();
		//enter game decide
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			break;
		}
	}
	//starting game and restart gamr loop
	while (app.isOpen())
	{
		end();
		int gamepoint = 0; //the point get during game //it has to be 0 after a game is over
		//picture has to load in the game
		Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13;
		t1.loadFromFile("./images/background/background1.png");
		t2.loadFromFile("./images/component/platform.png");
		t3.loadFromFile("./images/component/kung.png");
		t4.loadFromFile("./images/component/ice platform.png");
		t5.loadFromFile("./images/component/ghost.png");
		t6.loadFromFile("./images/component/skeleton.png");
		t7.loadFromFile("./images/component/diamond.png");
		t8.loadFromFile("./images/component/sawteeth.png");
		t9.loadFromFile("./images/component/gift.png");
		t10.loadFromFile("./images/component/frame.png");
		t11.loadFromFile("./images/component/bullet.png");
		t12.loadFromFile("./images/component/level up.png");
		t13.loadFromFile("./images/component/startPlat.png");
		
		Sprite sBackground(t1), sPlat(t2), sPers(t3), sIcePlat(t4),
			sGhost(t5), sSkel(t6), sDia(t7), steeth(t8), sGift(t9),
			sFrame(t10), sBullet(t11), slevel(t12), sSP(t13);
		//question photo load
		Texture questions[15];
		for (int i = 0; i < 12; i++) {
			stringstream s;
			s << i + 1;
			string filename = "./images/question/Q" + s.str() + ".png";
			questions[i].loadFromFile(filename);
		}
		//sound effect loading
		SoundBuffer Buffer1, Buffer2, Buffer3, Buffer4;
		Buffer1.loadFromFile("./sounds/jump.wav");
		Buffer2.loadFromFile("./sounds/correct.wav");
		Buffer3.loadFromFile("./sounds/wrong.wav");
		Buffer4.loadFromFile("./sounds/shooo.wav");
		jump.setBuffer(Buffer1);
		correct.setBuffer(Buffer2);
		wrong.setBuffer(Buffer3);
		shooo.setBuffer(Buffer4);
		//game boarder
		FloatRect rect1, rect2, rect3;
		rect1 = sPers.getGlobalBounds();
		rect2 = sPlat.getGlobalBounds();
		rect3 = steeth.getGlobalBounds();
		//down boarder setting
		steeth.setPosition(0, 533 - rect3.height);
		//plat position
		const int platn = 12; //we decide 12 because the plat will seperate equally
		point plat[platn];
		for (int i = 0; i < platn; i++) // create plat position
		{
			plat[i].x = rand() % W;
			plat[i].y = rand() % H;
		}
		//text setting
		gamescore.setFont(scorefont);
		gamescore.setCharacterSize(30);
		gamescore.setFillColor(sf::Color::Black);
		gamescore.setStyle(sf::Text::Bold);
		gamescore.setPosition(200, 0);
		
		bestscore.setFont(scorefont);
		bestscore.setCharacterSize(40);
		bestscore.setFillColor(sf::Color::Black);
		bestscore.setStyle(sf::Text::Bold);
		bestscore.setPosition(160, 350);
		
		deduct.setString("Game Score\n       -10");
		deduct.setFont(scorefont);
		deduct.setCharacterSize(20);
		deduct.setFillColor(sf::Color::Red);
		deduct.setStyle(sf::Text::Bold);

		textSkel.setString("Touch Skeleton\nYou are dead.");
		textSkel.setFont(scorefont);
		textSkel.setCharacterSize(30);
		textSkel.setFillColor(sf::Color::Blue);
		textSkel.setStyle(sf::Text::Bold);
		//bool or int for deciding the object to be show or not
		bool start = true;
		bool gameover = false;
		bool shoot = false;
		bool ghostshow = false;
		bool skeletonshow = false;
		bool flagDia[platn] = {0};
		int flagGift[platn] = {0};
		int flagIce[platn] = {0};
		int level2 = 0;
		int level3 = 0;
		//time counting for showing the text
		int scoreDeduct = 50;//touch ghost
		int touchSkeleton = 70;//touch skeleton
		//position setting
		int x = 100, y = 100; //character position
		int h = 200;
		float dx = 0, dy = 0; //each while loop move
		int K = rand() * 10001 % 7050; //ghost and skeleton show time gap
		int ghostx = rand() * 10001 % 330, skex = rand() * 10000 % 330; //ghost and skeleton position
		int ghosty = rand() * 100, skey = rand() * 100; 
		int whichQ = 0; //question number
		int bulletx = 100, bullety = 100; //bullet position
		
		//game start
		while (app.isOpen())
		{
			end();
			//set ghost position
			if (K % 100 == 0 && ghosty > 533) {
				ghostshow = true;
				ghosty = board;
				ghostx = rand() * 10001 % 300 + board;
			}
			//set skeleton position
			if (K % 1250 == 0 && skey > 533) {
				skeletonshow = true;
				skey = board;
				skex = rand() * 10000 % 300 + board;
			}
			//game control
			if (Keyboard::isKeyPressed(Keyboard::Right))
				x += 3;
			if (Keyboard::isKeyPressed(Keyboard::Left))
				x -= 3;
			if (Keyboard::isKeyPressed(Keyboard::Space))
			{
				shooo.play();//shoot sound
				bulletx = x + 10;
				bullety = y + 5;
				shoot = true;
			}
			//keep player in the game window
			if (x < 0)
				x += 3;
			if ((x + rect1.width) > 400)
				x -= 3;
			//drop without landing on board //gameover	
			if (y + rect1.height / 2 >= 533 - rect3.height) 
			{
				gameover = true;
				break;
			}
			//touch start platform
			if (start && (y > 400)) 
			{
				start = false;
				dy = -10;
				gamepoint += 5;
				jump.play();
			}
			//player move up
			dy += 0.2;
			y += dy;
			if (y > H-board)
				dy = -10;
			//a jump finish //new moving posiotion for plat
			if (y < h)
			{
				for (int i = 0; i < platn; i++)
				{
					y = h;
					plat[i].y = plat[i].y - dy;
					//plat move out the window
					//initialize all the data on that plat
					if (plat[i].y > 533 - rect3.height)
					{
						plat[i].y = 35;
						plat[i].x = rand() % 400;
						flagDia[i] = 0;
						flagGift[i] = 0;
						flagIce[i] = 0;
					}
				}
			}
			//set moving and changing position
			sPers.setPosition(x, y);
			sBullet.setPosition(bulletx, bullety);
			sGhost.setPosition(ghostx, ghosty);
			sSkel.setPosition(skex, skey);
			gamescore.setString("Score: "+ intconvert(gamepoint));
			//draw and show the objects
			app.draw(sBackground);
			app.draw(steeth);
			if (shoot)
				app.draw(sBullet);
			app.draw(sPers);
			app.draw(gamescore);
			if (ghostshow)
				app.draw(sGhost);
			if (skeletonshow)
				app.draw(sSkel);
			if (start) 
			{
				sSP.setPosition(20, 440);
				app.draw(sSP);
			} 
			//platform position renew and diamond gift showing or not
			for (int i = 0; i < platn; i++)
			{
				sPlat.setPosition(plat[i].x, plat[i].y);
				sIcePlat.setPosition(plat[i % 2].x, plat[i % 2].y);
				sDia.setPosition(plat[i].x + 20, plat[i].y - 25);
				sGift.setPosition(plat[i].x + 10, plat[i].y - 60);
				if (plat[i].x != plat[i % 2].x || plat[i].y != plat[i % 2].y) 
				{
					app.draw(sPlat);
				}
				else if (flagIce[i]!=2)
				{
					app.draw(sIcePlat);
					flagIce[i] = 1;  //1 means this is ice platform, 2 means the ice platform has been stepped onto
			    }
				if (i % 3 == 0 && flagDia[i] == 0 && plat[i].y > 60)
					app.draw(sDia);
				else if (i % 8 == 0 && flagGift[i] !=2 && plat[i].y > 80 && plat[i].x < 400 && plat[i].x > 40 && flagIce[i] == 0){
					app.draw(sGift);
					flagGift[i] = 1;
				}
			}
			//touch ske
			if ((x + 50 > skex) && (x + 20 < skex + 68) && (y + 70 > skey) && (y + 70 < skey + 14) && skeletonshow)
			{
				touchSkeleton = 0;
				skeletonshow = false;
				textSkel.setPosition(100, 300);
			}
			if (touchSkeleton < 70) {
				app.draw(textSkel);
				touchSkeleton++;
				if (touchSkeleton == 70) {
					gameover = true;
					break;
				}
			}
			//touch ghost
			if ((x + 50 > ghostx) && (x + 20 < ghostx + 68) && (y + 70 > ghosty) && (y + 70 < ghosty + 14) && ghostshow)
			{
				scoreDeduct = 0;
				ghostshow = false;
				if (gamepoint > 10)
					gamepoint -= 10;
				if (ghosty + 20 < 200)
					deduct.setPosition(ghostx + 50, ghosty + 20);
				else
					deduct.setPosition(ghostx - 50, ghosty + 20);
			}
			if (scoreDeduct < 50) {
				app.draw(deduct);
				scoreDeduct++;
			}
			//bullet touch skeleton
			if ((bulletx + 10 > skex) && (bulletx + 10 < skex + 68) && (bullety + 20 > skey) && (bullety + 20 < skey + 14) && shoot && skeletonshow)
			{
				shoot = false;
				skeletonshow = false;
			}
			//bullet touch ghost
			if ((bulletx + 10 > ghostx) && (bulletx + 10 < ghostx + 68) && (bullety + 20 > ghosty) && (bullety + 20 < ghosty + 14) && shoot && ghostshow)
			{
				shoot = false;
				ghostshow = false;
			}
			//touch platform
			for (int i = 0; i < platn; i++)
			{
				if ((x + 50 > plat[i].x) && (x + 20 < plat[i].x + 68) && (y + 70 > plat[i].y) && (y + 70 < plat[i].y + 14) && (dy > 0))
				{
					//not on the first platform anymore
					start = false;
					dy = -10;
					//score rule
					gamepoint += 5;
					jump.play();
					//touch ice platfrom
					if (flagIce[i] == 1) 
					{
						flagIce[i] = 2;
					}
					//touch diamond
					if (flagDia[i] == 0) 
					{
						gamepoint += 1;
						flagDia[i] = 1;
					}
					//touch gift
					if (i % 8 == 0 && flagGift[i] != 2 && plat[i].y > 80 && plat[i].x < 400 && plat[i].x > 40 && flagIce[i] == 0) {
						flagGift[i] = 2;
						//start zuvio question
						Sprite sQues(questions[whichQ % 12]);
						app.draw(sQues);
						app.display();

						while (1) 
						{
							if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
							{
								sf::Vector2i position = sf::Mouse::getPosition(app);
								//see what option does the player choose
								if (position.y < 273) {
									sFrame.setPosition(40, 200);
									//
									gamepoint += answer(whichQ, 1);
								}
								else if (position.y < 352) {
									sFrame.setPosition(40, 284);
									gamepoint += answer(whichQ, 2);
								}
								else if (position.y < 432) {
									sFrame.setPosition(40, 365);
									gamepoint += answer(whichQ, 3);
								}
								else {
									sFrame.setPosition(40, 440);
									gamepoint += answer(whichQ, 4);
								}
								app.draw(sQues);
								app.draw(sFrame);
								app.display();
								
								show();
								whichQ++;
								break;
							}
						}
					}
				}
			}
			//level up
			if (gamepoint >= L2 && level2 == 0) 
			{
				app.draw(slevel);
				app.display();
				show();
						
				level2 += 1;
				t1.loadFromFile("./images/background/background2.png");
				t3.loadFromFile("./images/component/chung.png");
			}
			if (gamepoint >= L3 && level3 == 0) {
				app.draw(slevel);
				app.display();
				show();
					
				level3 += 1;
				t1.loadFromFile("./images/background/background3.png");
				t3.loadFromFile("./images/component/cheng.png");
			}
			app.display();
			//moving y
			bullety -= 8;
			ghosty += 3;
			skey += 3;
			K++;
		}
		
		//end scene
		Texture tend;
		if (gamepoint >= bestpoint)
		{
			bestpoint = gamepoint;
			tend.loadFromFile("./images/component/bestscore.png");
		}
		else
		{
			tend.loadFromFile("./images/component/end.png");
		}
		Sprite sendground(tend);
		//game over
		while (app.isOpen())
		{
			end();
			
			gamescore.setString(intconvert(gamepoint));
			bestscore.setString(intconvert(bestpoint));

			app.draw(sendground);
			if (gamepoint >= bestpoint)
			{
				gamescore.setCharacterSize(50);
				gamescore.setPosition(150, 290);
				app.draw(gamescore);
			}
			else
			{
				gamescore.setCharacterSize(42);
				gamescore.setPosition(160, 185);
				app.draw(gamescore);
				app.draw(bestscore);
			}
			app.display();
			//renew the game
			if (Keyboard::isKeyPressed(Keyboard::Return))
			{
				break;
			}
		}
	}
	return 0;			
}

//the extra display show time;
void show()
{
	int timeElapsed = 0;
	while (1) {
		timeElapsed++;
		if (timeElapsed > 500000000)
			return;
	}
}
//closing the window
void end()
{
	Event e;
	while (app.pollEvent(e))
	{
		if (e.type == Event::Closed)
			app.close();
	}
}
//answewr question
int answer(int whichQ, int num)
{
	int point = 0;
	if ((whichQ + 1) % 4 == num % 4) {
		correct.play(); // sound
		point = 20; //reward
	}
	else {
		wrong.play();
		point = -20; //punishment
	}
	return point;
}
//gamepoint change to string for showing
string intconvert(int point)
{
	stringstream s;
	s << point;
	string str = s.str();
	return str;
}

