#include <SFML/Graphics.hpp>
#include<iostream>
#include<SFML/Audio.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<cstdlib>
#include<math.h>
#include<vector>
#include<sstream>
#include<fstream>
#include<algorithm>
#include<ctime>
#include<cstdlib>
using namespace sf;
using namespace std;


int u = 0;
int x = 0, y;
int GetHighScore(int score1) {
	ofstream outfile;
	outfile.open("Data/temp.txt");

	ifstream infile;

	infile.open("Data/scores.txt");
	infile >> x;

	if (x >= score1) {
		outfile << x << endl;
		y = x;
	}
	else {
		outfile << score1 << endl;
		y = score1;
	}
	outfile.close();
	infile.close();

	remove("Data/scores.txt");
	rename("Data/temp.txt", "Data/scores.txt");


	return y;
}



class rasengan {
public:
	CircleShape chidori;
	Vector2f currentvel;
	float maxspeed;

	rasengan(float radius = 5.f)
		: currentvel(0.f, 0.f), maxspeed(25.f)
	{
		this->chidori.setRadius(radius);
		this->chidori.setFillColor(Color::Blue);
	}
};


enum where { developerscreen, help, play, hscore, home, pause, gameover, options };
where location;

int main()
{
	srand(time(NULL));
	RenderWindow window(VideoMode(1800, 950), "GAME", Style::Default);
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);


	//sound

	sf::SoundBuffer buf_sample;
	if (buf_sample.loadFromFile("Data/background.ogg") == false) {
		return -1;

	}
	sf::Sound backgroundsound;
	backgroundsound.setBuffer(buf_sample);
	backgroundsound.setVolume(10);




	sf::SoundBuffer buf_sample1;
	if (buf_sample1.loadFromFile("Data/pistol.wav") == false) {
		return -1;

	}

	sf::Sound shootingsound;
	shootingsound.setBuffer(buf_sample1);
	shootingsound.setVolume(150);



	sf::SoundBuffer buf_sample2;
	if (buf_sample2.loadFromFile("Data/mouseclick.wav") == false) {
		return -1;

	}

	sf::Sound onclick;
	onclick.setBuffer(buf_sample2);
	onclick.setVolume(200);


	sf::Texture ptexture;
	if (!ptexture.loadFromFile("Data/background.png")) { //.png  file only
		//handle error
	}
	sf::Texture ptexture2;
	if (!ptexture2.loadFromFile("Data/developers.png")) { //.png  file only
		//handle error
	}
	sf::Texture ptexture3;
	if (!ptexture3.loadFromFile("Data/background1.png")) { //.png  file only
		//handle error
	}

	Sprite psprite;
	psprite.setTexture(ptexture);

	Sprite devscreem;
	devscreem.setTexture(ptexture2);

	Sprite gameback;
	gameback.setTexture(ptexture3);
	//variables
	//bool glowplay = false;
	bool leftclick = false;
	bool playbuttonpressed = false;
	bool escapekeypressed = false;

	bool playbuttonglow = false;
	bool optionsbuttonglow = false;
	bool signinbuttonglow = false;
	bool helpbuttonglow = false;
	bool exitbuttonglow = false;
	bool resumebuttonglow = false;
	bool homebuttonglow = false;
	bool backbuttonglow = false;
	bool gameoverbool = false;
	bool clicksoundplay = false;


	//interger variables

	int enemySpawnTimer = 0;
	int bombtimer = 0;
	int playerScore = 0;
	int life = 10;
	int highscore;
	int numberofclicks = 0;
	float accuracy = 0;
	int targethits = 0;
	int heartspawntimer = 0;


	//Vectors
	Vector2f playercenter;
	Vector2f mouseposition;
	Vector2f aimdirection;
	Vector2f aimnorm;
	double aimnormx;
	double aimnormy;


	//textures initilizaion
	Texture tplay;
	Texture toptions;
	Texture thelp;
	Texture texit;
	Texture thome;
	sf::Texture  tresume;
	Texture tback;
	Texture tplayer;
	Texture tenemy;
	Texture tbomb;
	Texture tbullet;
	Texture thighscore;
	Texture tcontinue;
	Texture theart;

	if (tplay.loadFromFile("Data/play.png") == false) {
		return -1;
	}

	/*if (toptions.loadFromFile("Data/high.png") == false) {
		return -1;
	}*/
	if (texit.loadFromFile("Data/exit.png") == false) {
		return -1;
	}
	if (thelp.loadFromFile("Data/help.png") == false) {
		return -1;
	}
	if (tresume.loadFromFile("Data/resume.png") == false) {
		return -1;
	}
	if (thome.loadFromFile("Data/home.png") == false) {
		return -1;
	}
	if (tback.loadFromFile("Data/Back.png") == false) {
		return -1;
	}
	if (tplayer.loadFromFile("Data/player.png") == false) {
		return -1;
	}
	if (tenemy.loadFromFile("Data/enemy.png") == false) {
		return -1;
	}
	if (tbomb.loadFromFile("Data/bomb.png") == false) {
		return -1;
	}
	if (tbullet.loadFromFile("Data/bullet.png") == false) {
		return -1;
	}
	if (thighscore.loadFromFile("Data/high.png") == false) {
		return -1;
	}
	if (theart.loadFromFile("Data/theart.png") == false) {
		return -1;
	}

	//font
	sf::Font font1;
	font1.loadFromFile("Data/chargen.ttf");

	sf::Font font2;
	font2.loadFromFile("Data/arial.ttf");

	sf::Text score;
	score.setFont(font1);
	score.setCharacterSize(30);
	score.setFillColor(sf::Color::Green);

	score.setString("0");


	sf::Text high;
	high.setFont(font1);
	high.setCharacterSize(40);
	high.setFillColor(sf::Color::Green);
	high.setPosition((window.getSize().x / 2) - 350, (window.getSize().y / 2) - 100);
	high.setString("0");

	sf::Text lifeline;
	lifeline.setFont(font1);
	lifeline.setCharacterSize(40);
	lifeline.setFillColor(sf::Color::Green);
	lifeline.setPosition(1500, 10);
	lifeline.setString("0");

	sf::Text score2;
	score2.setFont(font1);
	score2.setCharacterSize(40);
	score2.setFillColor(sf::Color::Green);
	score2.setPosition((window.getSize().x / 2) - 350, (window.getSize().y / 2) - 150);
	score2.setString("0");

	sf::Text accuracy1;
	accuracy1.setFont(font1);
	accuracy1.setCharacterSize(40);
	accuracy1.setFillColor(sf::Color::Green);
	accuracy1.setPosition((window.getSize().x / 2) - 350, (window.getSize().y / 2) - 250);
	accuracy1.setString("0");

	sf::Text highscoremessage;
	highscoremessage.setFont(font1);
	highscoremessage.setCharacterSize(80);
	highscoremessage.setFillColor(sf::Color::Green);
	highscoremessage.setPosition((window.getSize().x / 2) - 350, (window.getSize().y / 2) - 250);
	highscoremessage.setString("0");



	//Rasengan
	rasengan r1;
	vector<rasengan> rasengans;
	rasengans.push_back(rasengan(r1));


	//projectiles
	CircleShape projectile;
	projectile.setTexture(&tbullet);
	projectile.setRadius(40.f);

	RectangleShape enemy;
	enemy.setTexture(&tenemy);
	enemy.setSize(Vector2f(70.f,70.f));
	std::vector<RectangleShape> enemies;
	enemies.push_back(RectangleShape(enemy));



	//bomb
	sf::CircleShape bomb(30.f);
	bomb.setTexture(&tbomb);
	std::vector<CircleShape> bombs;
	bombs.push_back(CircleShape(bomb));

	//hearts
	sf::CircleShape heart(30.f);
	heart.setTexture(&theart);
	std::vector<CircleShape> hearts;
	hearts.push_back(CircleShape(heart));

	//PlayerShape

	sf::RectangleShape playersprite(sf::Vector2f(110, 90));
	playersprite.setTexture(&tplayer);


	//draw shapes
	sf::RectangleShape playbutton;
	playbutton.setSize(sf::Vector2f(200, 80));
	playbutton.setPosition(400, 200);
	playbutton.setTexture(&tplay);
	playbutton.setFillColor(sf::Color::White);

	sf::RectangleShape optionsbutton;
	optionsbutton.setSize(sf::Vector2f(200, 80));
	optionsbutton.setPosition(400, 400);
	optionsbutton.setTexture(&thighscore);
	optionsbutton.setFillColor(sf::Color::White);


	sf::RectangleShape exitnbutton;
	exitnbutton.setSize(sf::Vector2f(200, 80));
	exitnbutton.setPosition(400, 600);
	exitnbutton.setTexture(&texit);
	exitnbutton.setFillColor(sf::Color::White);

	sf::CircleShape helpbutton(60.f);
	helpbutton.setPosition(sf::Vector2f(900, 100));
	helpbutton.setTexture(&thelp);



	sf::RectangleShape resumebutton;
	resumebutton.setSize(sf::Vector2f(200, 80));
	resumebutton.setPosition(400, 200);
	resumebutton.setTexture(&tresume);
	resumebutton.setFillColor(sf::Color::White);

	sf::RectangleShape homebutton;
	homebutton.setSize(sf::Vector2f(200, 80));
	homebutton.setPosition(400, 400);
	homebutton.setTexture(&thome);
	homebutton.setFillColor(sf::Color::White);



	sf::RectangleShape backbutton;
	backbutton.setSize(sf::Vector2f(200, 80));

	backbutton.setTexture(&tback);
	backbutton.setFillColor(sf::Color::White);

	sf::RectangleShape gameoverarea;
	gameoverarea.setSize(sf::Vector2f(600, 800));
	gameoverarea.setPosition((window.getSize().x / 2) - 400, (window.getSize().y / 2) - 300);
	gameoverarea.setFillColor(sf::Color::Blue);



	//help menu info 
	sf::Text text1("HOW TO PLAY", font1, 50);
	text1.setPosition(470, 20);
	text1.setFillColor(sf::Color::Red);


	sf::Text text2("--Press A w s d for the movement keys. \n --Press Left Mouse Button to Fire. \n -- PLay for the high score. \n Enjoy !!", font1, 30);
	text2.setPosition(100, 140);
	text2.setFillColor(sf::Color::Green);

	sf::Text text3("'FROM DELTA DEVELOPERS' ", font2, 40);
	text3.setPosition(490, 20);
	text3.setFillColor(sf::Color::Red);


	sf::Text text4("HOOPSHOOT", font1, 80);
	text4.setPosition(window.getSize().x - 1230, 140);
	text4.setFillColor(sf::Color::White);

	sf::Text text5(" <<  Press Enter key or CLick Left mouse to Proceed  >>", font2, 20);
	text5.setPosition(window.getSize().x-1270,window.getSize().y-30);
	text5.setFillColor(sf::Color::White);

	sf::Text congrats("Congratulations!!", font1, 80);
	congrats.setPosition(500, 10);
	congrats.setFillColor(sf::Color::Green);







	sf::Event event;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (location == developerscreen) {
				if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) || (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)) {
					onclick.play();
					location = home;
				}

			}

			if (location == play) {
				if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {

					leftclick = true;
				}
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
					backgroundsound.stop();
					onclick.play();
					location = pause;
				}
			}
			if (location == hscore) {
				if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) ) {
					onclick.play();
					location = home;
				}

			}


			//mouseclicks

			if (location == gameover) {
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
					if (sf::Mouse::getPosition(window).x > backbutton.getGlobalBounds().left
						&& sf::Mouse::getPosition(window).x < (backbutton.getGlobalBounds().left + backbutton.getGlobalBounds().width)
						&& sf::Mouse::getPosition(window).y > backbutton.getGlobalBounds().top
						&& sf::Mouse::getPosition(window).y < (backbutton.getGlobalBounds().top + backbutton.getGlobalBounds().height)
						) {
						onclick.play();
						location = home;
					}
				}
			}

			if (location == home) {
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
					if (sf::Mouse::getPosition(window).x > playbutton.getGlobalBounds().left
						&& sf::Mouse::getPosition(window).x < (playbutton.getGlobalBounds().left + playbutton.getGlobalBounds().width)
						&& sf::Mouse::getPosition(window).y > playbutton.getGlobalBounds().top
						&& sf::Mouse::getPosition(window).y < (playbutton.getGlobalBounds().top + playbutton.getGlobalBounds().height)
						) {
						window.clear();
						onclick.play();
						backgroundsound.play();
						location = play;


					}
				}


				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
					if (sf::Mouse::getPosition(window).x > optionsbutton.getGlobalBounds().left
						&& sf::Mouse::getPosition(window).x < (optionsbutton.getGlobalBounds().left + optionsbutton.getGlobalBounds().width)
						&& sf::Mouse::getPosition(window).y > optionsbutton.getGlobalBounds().top
						&& sf::Mouse::getPosition(window).y < (optionsbutton.getGlobalBounds().top + optionsbutton.getGlobalBounds().height)
						) {
						onclick.play();
						location = hscore;
					}
				}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
					if (sf::Mouse::getPosition(window).x > helpbutton.getGlobalBounds().left
						&& sf::Mouse::getPosition(window).x < (helpbutton.getGlobalBounds().left + helpbutton.getGlobalBounds().width)
						&& sf::Mouse::getPosition(window).y > helpbutton.getGlobalBounds().top
						&& sf::Mouse::getPosition(window).y < (helpbutton.getGlobalBounds().top + helpbutton.getGlobalBounds().height)
						) {
						onclick.play();
						location = help;
					}
				}

				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
					if (sf::Mouse::getPosition(window).x > exitnbutton.getGlobalBounds().left
						&& sf::Mouse::getPosition(window).x < (exitnbutton.getGlobalBounds().left + exitnbutton.getGlobalBounds().width)
						&& sf::Mouse::getPosition(window).y > exitnbutton.getGlobalBounds().top
						&& sf::Mouse::getPosition(window).y < (exitnbutton.getGlobalBounds().top + exitnbutton.getGlobalBounds().height)
						) {
						onclick.play();
						window.close();
					}
				}

			}


			if (location == help) {
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
					if (sf::Mouse::getPosition(window).x > backbutton.getGlobalBounds().left
						&& sf::Mouse::getPosition(window).x < (backbutton.getGlobalBounds().left + backbutton.getGlobalBounds().width)
						&& sf::Mouse::getPosition(window).y > backbutton.getGlobalBounds().top
						&& sf::Mouse::getPosition(window).y < (backbutton.getGlobalBounds().top + backbutton.getGlobalBounds().height)
						) {
						onclick.play();
						location = home;
					}
				}
			}






			//mouse glow events

			if ((event.type == sf::Event::MouseMoved)) {
				if (sf::Mouse::getPosition(window).x > playbutton.getGlobalBounds().left
					&& sf::Mouse::getPosition(window).x < (playbutton.getGlobalBounds().left + playbutton.getGlobalBounds().width)
					&& sf::Mouse::getPosition(window).y > playbutton.getGlobalBounds().top
					&& sf::Mouse::getPosition(window).y < (playbutton.getGlobalBounds().top + playbutton.getGlobalBounds().height)
					) {

					playbuttonglow = true;
				}

				else {
					playbuttonglow = false;

				}

			}

			if (sf::Mouse::getPosition(window).x > optionsbutton.getGlobalBounds().left
				&& sf::Mouse::getPosition(window).x < (optionsbutton.getGlobalBounds().left + optionsbutton.getGlobalBounds().width)
				&& sf::Mouse::getPosition(window).y > optionsbutton.getGlobalBounds().top
				&& sf::Mouse::getPosition(window).y < (optionsbutton.getGlobalBounds().top + optionsbutton.getGlobalBounds().height)
				) {

				optionsbuttonglow = true;

				//ontouch.stop();
			}
			else {
				optionsbuttonglow = false;

			}

			if (sf::Mouse::getPosition(window).x > exitnbutton.getGlobalBounds().left
				&& sf::Mouse::getPosition(window).x < (exitnbutton.getGlobalBounds().left + exitnbutton.getGlobalBounds().width)
				&& sf::Mouse::getPosition(window).y > exitnbutton.getGlobalBounds().top
				&& sf::Mouse::getPosition(window).y < (exitnbutton.getGlobalBounds().top + exitnbutton.getGlobalBounds().height)
				) {

				exitbuttonglow = true;


			}
			else {
				exitbuttonglow = false;


			}


			if (sf::Mouse::getPosition(window).x > helpbutton.getGlobalBounds().left
				&& sf::Mouse::getPosition(window).x < (helpbutton.getGlobalBounds().left + helpbutton.getGlobalBounds().width)
				&& sf::Mouse::getPosition(window).y > helpbutton.getGlobalBounds().top
				&& sf::Mouse::getPosition(window).y < (helpbutton.getGlobalBounds().top + helpbutton.getGlobalBounds().height)
				) {

				helpbuttonglow = true;


			}
			else {
				helpbuttonglow = false;


			}
			if (sf::Mouse::getPosition(window).x > backbutton.getGlobalBounds().left
				&& sf::Mouse::getPosition(window).x < (backbutton.getGlobalBounds().left + backbutton.getGlobalBounds().width)
				&& sf::Mouse::getPosition(window).y > backbutton.getGlobalBounds().top
				&& sf::Mouse::getPosition(window).y < (backbutton.getGlobalBounds().top + backbutton.getGlobalBounds().height)
				) {

				backbuttonglow = true;


			}
			else {
				backbuttonglow = false;

			}



			if (location == help) {
				if (sf::Mouse::getPosition(window).x > backbutton.getGlobalBounds().left
					&& sf::Mouse::getPosition(window).x < (backbutton.getGlobalBounds().left + backbutton.getGlobalBounds().width)
					&& sf::Mouse::getPosition(window).y > backbutton.getGlobalBounds().top
					&& sf::Mouse::getPosition(window).y < (backbutton.getGlobalBounds().top + backbutton.getGlobalBounds().height)
					) {

					backbuttonglow = true;


				}
				else {
					backbuttonglow = false;

				}


				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
					if (sf::Mouse::getPosition(window).x > backbutton.getGlobalBounds().left
						&& sf::Mouse::getPosition(window).x < (backbutton.getGlobalBounds().left + backbutton.getGlobalBounds().width)
						&& sf::Mouse::getPosition(window).y > backbutton.getGlobalBounds().top
						&& sf::Mouse::getPosition(window).y < (backbutton.getGlobalBounds().top + backbutton.getGlobalBounds().height)
						) {
						onclick.play();
						location = home;

					}
				}
			}

			if (location == gameover) {

				if ((event.type == sf::Event::MouseMoved)) {
					if (sf::Mouse::getPosition(window).x > backbutton.getGlobalBounds().left
						&& sf::Mouse::getPosition(window).x < (backbutton.getGlobalBounds().left + backbutton.getGlobalBounds().width)
						&& sf::Mouse::getPosition(window).y > backbutton.getGlobalBounds().top
						&& sf::Mouse::getPosition(window).y < (backbutton.getGlobalBounds().top + backbutton.getGlobalBounds().height)
						) {


						backbuttonglow = true;


					}

					else {
						backbuttonglow = false;
					}

				}
			}


			if (location == pause) {
				if ((event.type == sf::Event::MouseMoved)) {
					if (sf::Mouse::getPosition(window).x > resumebutton.getGlobalBounds().left
						&& sf::Mouse::getPosition(window).x < (resumebutton.getGlobalBounds().left + resumebutton.getGlobalBounds().width)
						&& sf::Mouse::getPosition(window).y > resumebutton.getGlobalBounds().top
						&& sf::Mouse::getPosition(window).y < (resumebutton.getGlobalBounds().top + resumebutton.getGlobalBounds().height)
						) {

						resumebuttonglow = true;
					}

					else {
						resumebuttonglow = false;
					}

				}

				if ((event.type == sf::Event::MouseMoved)) {
					if (sf::Mouse::getPosition(window).x > homebutton.getGlobalBounds().left
						&& sf::Mouse::getPosition(window).x < (homebutton.getGlobalBounds().left + homebutton.getGlobalBounds().width)
						&& sf::Mouse::getPosition(window).y > homebutton.getGlobalBounds().top
						&& sf::Mouse::getPosition(window).y < (homebutton.getGlobalBounds().top + homebutton.getGlobalBounds().height)
						) {

						homebuttonglow = true;

					}

					else {
						homebuttonglow = false;
					}

				}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
					if (sf::Mouse::getPosition(window).x > homebutton.getGlobalBounds().left
						&& sf::Mouse::getPosition(window).x < (homebutton.getGlobalBounds().left + homebutton.getGlobalBounds().width)
						&& sf::Mouse::getPosition(window).y > homebutton.getGlobalBounds().top
						&& sf::Mouse::getPosition(window).y < (homebutton.getGlobalBounds().top + homebutton.getGlobalBounds().height)
						) {
						onclick.play();
						life = 10;
						playerScore = 0;
						enemySpawnTimer = 0;
						bombtimer = 0;
						playersprite.setPosition(0, 0);
						for (int i = 0; i < 50; i++) {
							enemies.clear();
							bombs.clear();
					}
						location = home;
					}
				}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
					if (sf::Mouse::getPosition(window).x > resumebutton.getGlobalBounds().left
						&& sf::Mouse::getPosition(window).x < (resumebutton.getGlobalBounds().left + resumebutton.getGlobalBounds().width)
						&& sf::Mouse::getPosition(window).y > resumebutton.getGlobalBounds().top
						&& sf::Mouse::getPosition(window).y < (resumebutton.getGlobalBounds().top + resumebutton.getGlobalBounds().height)
						) {
						onclick.play();
						backgroundsound.play();
						location = play;


					}
				}
			}
		}



		//Game Logics

		//buttonglow logics
		if (playbuttonglow == true) {
			playbutton.setFillColor(sf::Color::Blue);



		}

		if (playbuttonglow == false) {
			playbutton.setFillColor(sf::Color::White);

		}
		if (optionsbuttonglow == true) {
			optionsbutton.setFillColor(sf::Color::Blue);

		}
		if (optionsbuttonglow == false) {
			optionsbutton.setFillColor(sf::Color::White);


		}


		if (exitbuttonglow == true) {
			exitnbutton.setFillColor(sf::Color::Blue);

		}
		if (exitbuttonglow == false) {
			exitnbutton.setFillColor(sf::Color::White);

		}
		if (helpbuttonglow == true) {
			helpbutton.setFillColor(sf::Color::Blue);

		}
		if (helpbuttonglow == false) {
			helpbutton.setFillColor(sf::Color::White);

		}
		if (resumebuttonglow == true) {
			resumebutton.setFillColor(sf::Color::Blue);

		}
		if (resumebuttonglow == false) {
			resumebutton.setFillColor(sf::Color::White);

		}
		if (homebuttonglow == true) {
			homebutton.setFillColor(sf::Color::Blue);

		}
		if (homebuttonglow == false) {
			homebutton.setFillColor(sf::Color::White);
			if (backbuttonglow == true) {
				backbutton.setFillColor(sf::Color::Blue);

			}
			if (backbuttonglow == false) {
				backbutton.setFillColor(sf::Color::White);
			}
		}

		//sound play logics


		//update
		if (location == play) {
			window.clear();
			playercenter = Vector2f(playersprite.getPosition().x, playersprite.getPosition().y);
			mouseposition = Vector2f(Mouse::getPosition(window));
			aimdirection = mouseposition - playercenter;
			aimnormx = aimdirection.x / sqrt(pow(aimdirection.x, 2) + pow(aimdirection.y, 2));
			aimnormy = aimdirection.y / sqrt(pow(aimdirection.x, 2) + pow(aimdirection.y, 2));
			aimnorm = Vector2f(aimnormx, aimnormy);

			//Playermove
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				if (playersprite.getPosition().y < 0)

					playersprite.move(0.f, 0.f);

				else
					playersprite.move(0.f, -9.5);
			}
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				if (playersprite.getPosition().y > window.getSize().y - 90)

					playersprite.move(0.f, 0.f);

				else
					playersprite.move(0.f, 9.5f);
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				if (playersprite.getPosition().x > window.getSize().x - 110)
					playersprite.move(0.f, 0.f);
				else
					playersprite.move(9.5, 0.f);
			}
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				if (playersprite.getPosition().x < 0)
					playersprite.move(0.f, 0.f);

				else
					playersprite.move(-9.5, 0.f);
			}
		}


		//Shooting
		if (leftclick == true) {
			numberofclicks++;
			shootingsound.play();
			r1.chidori.setPosition(playercenter);
			r1.currentvel = aimnorm * r1.maxspeed;

			rasengans.push_back(rasengan(r1));
			leftclick = false;
		}
		else
			shootingsound.stop();

		for (size_t i = 0; i < rasengans.size(); i++) {
			rasengans[i].chidori.move(rasengans[i].currentvel);
		}

		//ENEMIES
		if (playerScore < 11) {
			if (enemySpawnTimer < 80)
				enemySpawnTimer++;

			if (enemySpawnTimer >= 80) {
				enemy.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), (rand() % int(window.getSize().y - enemy.getSize().y)));
				enemies.push_back(RectangleShape(enemy));

				enemySpawnTimer = 0;
			}
			//bombs
			if (bombtimer < 80)
				bombtimer++;

			if (bombtimer >= 80) {
				bomb.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), 0);
				bombs.push_back(CircleShape(bomb));

				bombtimer = 0;
			}
		}
		else if (playerScore >= 11 && playerScore < 21) {
			if (enemySpawnTimer < 65)
				enemySpawnTimer++;

			if (enemySpawnTimer >= 65) {
				enemy.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), (rand() % int(window.getSize().y - enemy.getSize().y)));
				enemies.push_back(RectangleShape(enemy));

				enemySpawnTimer = 0;
			}
			if (bombtimer < 65)
				bombtimer++;

			if (bombtimer >= 65) {
				bomb.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), 0);
				bombs.push_back(CircleShape(bomb));

				bombtimer = 0;
			}
		}
		else if (playerScore >= 21 && playerScore < 31) {
			if (enemySpawnTimer < 64)
				enemySpawnTimer++;

			if (enemySpawnTimer >= 64) {
				enemy.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), (rand() % int(window.getSize().y - enemy.getSize().y)));
				enemies.push_back(RectangleShape(enemy));

				enemySpawnTimer = 0;
			}

			if (bombtimer < 60)
				bombtimer++;

			if (bombtimer >= 60) {
				bomb.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), 0);
				bombs.push_back(CircleShape(bomb));

				bombtimer = 0;
			}
		}
		else if (playerScore >= 31 && playerScore < 51) {
			if (enemySpawnTimer < 60)
				enemySpawnTimer++;

			if (enemySpawnTimer >= 60) {
				enemy.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), (rand() % int(window.getSize().y - enemy.getSize().y)));
				enemies.push_back(RectangleShape(enemy));

				enemySpawnTimer = 0;
			}
			if (bombtimer < 50)
				bombtimer++;

			if (bombtimer >= 050) {
				bomb.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), 0);
				bombs.push_back(CircleShape(bomb));

				bombtimer = 0;
			}
		}
		else if (playerScore >= 51 && playerScore < 115) {
			if (enemySpawnTimer < 50)
				enemySpawnTimer++;

			if (enemySpawnTimer >= 50) {
				enemy.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), (rand() % int(window.getSize().y - enemy.getSize().y)));
				enemies.push_back(RectangleShape(enemy));

				enemySpawnTimer = 0;
			}
			if (bombtimer < 40)
				bombtimer++;

			if (bombtimer >= 40) {
				bomb.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), 0);
				bombs.push_back(CircleShape(bomb));

				bombtimer = 0;
			}
		}
		if (playerScore == 115 || playerScore > 115) {
			if (enemySpawnTimer < 40)
				enemySpawnTimer++;

			if (enemySpawnTimer >= 40) {
				enemy.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), (rand() % int(window.getSize().y - enemy.getSize().y)));
				enemies.push_back(RectangleShape(enemy));

				enemySpawnTimer = 0;
			}
			if (bombtimer < 30)
				bombtimer++;

			if (bombtimer >= 30) {
				bomb.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), 0);
				bombs.push_back(CircleShape(bomb));

				bombtimer = 0;
			}
		}

		//spawning of hearts

		if ( heartspawntimer< 220)
			heartspawntimer++;

		if (heartspawntimer >= 220){
			heart.setPosition((rand() % int(window.getSize().x - heart.getRadius())), window.getSize().y - heart.getRadius());
			hearts.push_back(CircleShape(heart));

			heartspawntimer = 0;
		}



		//movement of hearts

		for (size_t i = 0; i < hearts.size(); i++)
		{
			hearts[i].move(0, -1);

			if (hearts[i].getPosition().y > window.getSize().y)
				hearts.erase(hearts.begin() + i);
		}

		//movement of bombs
		for (size_t i = 0; i < bombs.size(); i++)
		{
			bombs[i].move(0, 1);

			if (bombs[i].getPosition().y > window.getSize().y)
				bombs.erase(bombs.begin() + i);
		}



		//Collision of projectile with enemy
		if (!enemies.empty() && !rasengans.empty())
		{
			for (size_t i = 0; i < rasengans.size(); i++)
			{
				for (size_t k = 0; k < enemies.size(); k++)
				{
					if (rasengans[i].chidori.getGlobalBounds().intersects(enemies[k].getGlobalBounds())) {
						rasengans.erase(rasengans.begin() + i);
						enemies.erase(enemies.begin() + k);
						playerScore++;
						targethits++;
						break;
					}
				}
			}
		}
		//colllision of the heart and the bomb

		for (size_t i = 0; i < hearts.size(); i++) {

			if (playersprite.getGlobalBounds().intersects(hearts[i].getGlobalBounds())) {
				hearts.erase(hearts.begin() + i);
				if (life < 10) {
					life = life + 1;
				}
				
			}
		}


		//collision of player and the bombs and enemies


		

		for (size_t k = 0; k < enemies.size(); k++) {
			if (playersprite.getGlobalBounds().intersects(enemies[k].getGlobalBounds())) {
				enemies.erase(enemies.begin() + k);
				life = life - 1;
			}

		}
		for (size_t k = 0; k < bombs.size(); k++) {
			if (playersprite.getGlobalBounds().intersects(bombs[k].getGlobalBounds())) {
				bombs.erase(bombs.begin() + k);
				life = life - 1;
			}

		}
		//condition for game over

		if (life < 0) {
			window.clear();
			life = 10;
		



			for (int i = 0; i < 50; i++) {
				enemies.clear();
				bombs.clear();
			}



			enemySpawnTimer = 100;
			bombtimer = 100;
			playersprite.setPosition(0, 0);

			backgroundsound.stop();
			srand(time(NULL));
			location = gameover;


		}
		// drawing events
		if (location == developerscreen) {
			window.clear();
			window.draw(devscreem);
			window.draw(text3);
			window.draw(text4);
			window.draw(text5);

		}


		if (location == play) {




			window.clear();
			window.draw(gameback);
			window.draw(playersprite);
			
			for (size_t i = 0; i < rasengans.size(); i++) {
				window.draw(rasengans[i].chidori);
			}
			for (size_t i = 0; i < enemies.size(); i++)
			{
				window.draw(enemies[i]);
			}
			for (size_t i = 0; i < bombs.size(); i++)
			{
				window.draw(bombs[i]);
			}
			for (size_t i = 0; i < hearts.size(); i++)
			{
				window.draw(hearts[i]);
			}


			//score
			std::stringstream text;
			text << "Your Score : " << playerScore;
			score.setString(text.str());
			score.setPosition(380, 10);
			window.draw(score);

			std::stringstream text1;
			text1 << "Lifes : " << life;
			lifeline.setString(text1.str());
			window.draw(lifeline);






		}
		if (location == home) {

			window.clear();
			window.draw(psprite);
			window.draw(playbutton);
			window.draw(optionsbutton);
			window.draw(exitnbutton);
			window.draw(helpbutton);

		}
		if (location == pause) {


			window.clear();
			window.draw(resumebutton);
			window.draw(homebutton);
		}
		if (location == help) {
			window.clear();
			;
			backbutton.setPosition(50, 50);
			window.draw(backbutton);

		}

		if (location == gameover) {

			window.clear();
			window.draw(gameoverarea);

			backbutton.setPosition(500, 700);
			window.draw(backbutton);
			int u = GetHighScore(0);
			highscore = GetHighScore(playerScore);

			if (playerScore>u)
			{
				window.draw(congrats);
				std::stringstream te;
				te << "You set a new high score of: " << highscore;
				high.setString(te.str());
				window.draw(highscoremessage);
				playerScore = 0;



			
			}
			else
			{
				std::stringstream text2;
				text2 << "HIGHSCORE : " << highscore;
				high.setString(text2.str());
				window.draw(high);
				playerScore = 0;
				
			

			
			}

			score.setPosition((window.getSize().x / 2) - 350, (window.getSize().y / 2) - 150);
			window.draw(score);

			accuracy = ((targethits / numberofclicks) * 100);
			std::stringstream text4;
			text4 << "ACCURACY : " << accuracy;
			accuracy1.setString(text4.str());
			window.draw(accuracy1);

		}
		if (location == hscore) {
			window.clear();

			highscore = GetHighScore(playerScore);
			std::stringstream msg;
			msg << "HIGHSCORE : " << highscore;
			high.setString(msg.str());
			window.draw(high);
		}




		window.display();


	}
	return 0;
}