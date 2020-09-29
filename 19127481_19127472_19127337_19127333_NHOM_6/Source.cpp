#pragma warning(disable : 4996)
#pragma comment(lib, "winmm.lib")
#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<iostream>
#include<string>
#include<cstdlib>
#include<time.h>
#include<fstream>
#include<sstream>
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27
using namespace std;
int ForRandom[2] = { -1,1 };
std::string OPTION[4] = { "NEW GAME","LOAD GAME","ABOUT","EXIT" };
std::string carFile1 = "Assets/graphics/car1.png";
std::string carFile2 = "Assets/graphics/car2.png";
std::string bgFile = "Assets/graphics/background.png";
std::string upFile = "Assets/graphics/up.png";
std::string downFile = "Assets/graphics/down.png";
std::string leftFile = "Assets/graphics/left.png";
std::string rightFile = "Assets/graphics/right.png";
std::string themeBG = "Assets/sound/BGMS.wav";
std::string collision = "Assets/sound/crash.wav";
std::string fontFile = "Assets/font/FLATS.ttf";
std::string loseFile = "Assets/sound/lose.flac";
std::string winFile = "Assets/sound/win.wav";
std::string aboutFile = "Assets/graphics/about.png";
time_t beginT = time(NULL), endT = time(NULL);
int SPEED = 1;
int MAXSPEED = 5;
float TOP = 155;
int CARWITH = 189;
int CARHEIGHT = 99;
int SPACEROW = 20;
int MAXCAR = 2;
int MAXROW = 4;
int viewSizeX = 1024;
int viewSizeY = 768;
int playerHeight = 90;
int playerWidth = 40;
int timeBegin = 10;
int timEnd = 15;

//---------------------------------------------------------------------------------------------------------------------------------------
//																	Khai báo
struct BGTheme {
	sf::Music music;
};

struct effectSound {
	sf::SoundBuffer load;
	sf::Sound effect;
};

struct player {
	sf::Vector2f size;
	sf::Texture load;
	sf::Sprite Player;
};

struct car {
	sf::Vector2f size;
	sf::Sprite car;
	sf::Texture load;
	float OriginalPosX, OriginalPosY;
	int direction;
};

struct Game {
	sf::RenderWindow* window;
	player* _player;
	car** _car;
};

struct MENU {
	sf::Font font;
	sf::Text text[4];
	int selected;
};



//Tạo và vẽ menu
void createMenu(MENU& a);
void drawMenu(MENU& a, sf::RenderWindow& window);
void MoveUp(MENU& a);
void MoveDown(MENU& a);
void Start(Game& a, BGTheme& music);
//Âm thanh nền và hiệu ứng
void playMusic(std::string file, BGTheme& a);
void effect(std::string file, effectSound& a);

//Tạo người chơi
void createPlayer(sf::Vector2u size, player& a);
void DrawPlayer(player& a, sf::RenderWindow& window);
bool movePlayer(player& a, sf::Event& event);
sf::Sprite getShape(player& a);

//Tạo xe và vẽ xe
void createCar(float posX, float posY, car& a, int direction);
void DrawCar(car& a, sf::RenderWindow& window);
bool moveCar(car& a, sf::Vector2u size, int x);
sf::Sprite getCarShape(car& a);
//Game
void createGame(Game& a);

void loop(Game& a, BGTheme& music);
//--------------------------------------------------------------------------------------------------------------------------------------

void playMusic(std::string file, BGTheme& a)
{
	a.music.openFromFile(file);
	a.music.setVolume(50);
	a.music.play();
	a.music.setLoop(true);
}

void effect(std::string file, effectSound& a)
{
	a.load.loadFromFile(file);
	a.effect.setBuffer(a.load);
	a.effect.play();
}



//---------------------------------------------------------------------------------------------------------------------------------------
//																	Người chơi


void createPlayer(sf::Vector2u size, player& a)
{

	a.Player.setScale(sf::Vector2f(1, 1));
	a.Player.setPosition(sf::Vector2f(viewSizeX / 2, viewSizeY - playerHeight));
	a.load.loadFromFile(upFile);
	a.Player.setTexture(a.load);
}

void DrawPlayer(player& a, sf::RenderWindow& window)
{
	window.draw(a.Player);
}

bool movePlayer(player& a, sf::Event& event)
{
	if (sf::Keyboard::Key::Left == event.key.code || sf::Keyboard::Key::A == event.key.code)
	{
		if (a.Player.getPosition().x - 30 > 0)
		{
			a.Player.move(-30, 0);
			a.load.loadFromFile(leftFile);
		}
	}
	if (sf::Keyboard::Key::Right == event.key.code || sf::Keyboard::Key::D == event.key.code)
	{
		if (a.Player.getPosition().x + 30 + playerWidth < viewSizeX)
		{
			a.Player.move(30, 0);
			a.load.loadFromFile(rightFile);
		}
	}
	if (sf::Keyboard::Key::Up == event.key.code || sf::Keyboard::Key::W == event.key.code)
	{
		if (a.Player.getPosition().y - 30 > 0)
		{
			a.load.loadFromFile(upFile);
			a.Player.move(0, -30);
		}

	}
	if (sf::Keyboard::Key::Down == event.key.code || sf::Keyboard::Key::S == event.key.code)
	{
		if (a.Player.getPosition().y + 30 + playerHeight < viewSizeY)
		{
			a.load.loadFromFile(downFile);
			a.Player.move(0, 30);
		}
	}
	return true;
}
sf::Sprite getShape(player& a)
{
	return a.Player;
}
//---------------------------------------------------------------------------------------------------------------------------------------
//																Xe


void createCar(float posX, float posY, car& a, int direction)
{
	a.car.setScale(sf::Vector2f(1.0f, 1.0f));
	a.car.setPosition(sf::Vector2f(posX, posY));
	a.OriginalPosX = posX;
	a.OriginalPosY = posY;
	if (direction > 0)
		a.load.loadFromFile(carFile1);
	else
		a.load.loadFromFile(carFile2);
	a.car.setTexture(a.load);
	a.direction = direction;
}

void DrawCar(car& a, sf::RenderWindow& window)
{
	window.draw(a.car);
}

bool moveCar(car& a, sf::Vector2u size, int x)
{
	a.car.move(x * 3.f, 0.0f);
	float oriY = a.car.getPosition().y;
	if (x > 0)
	{
		if (a.car.getPosition().x > size.x + CARWITH)
			a.car.setPosition(sf::Vector2f(-a.car.getScale().x, oriY));
	}
	if (x < 0)
	{
		if (a.car.getPosition().x + CARWITH < 0)
			a.car.setPosition(sf::Vector2f(size.x + a.car.getScale().x, oriY));
	}

	return true;
}
sf::Sprite getCarShape(car& a)
{
	return a.car;
}
//-----------------------------------------------------------------------------------------------------------------------------------
//															Game

void createGame(Game& a)
{
	a.window = new sf::RenderWindow(sf::VideoMode(viewSizeX, viewSizeY), "Cross street game");
	int n = rand() % 1024;
	a._car = new car * [MAXROW];
	for (int i = 0; i < MAXROW; i++)
		a._car[i] = new car[MAXCAR];
	int positionY, positionX;
	int direction;
	for (int i = 0; i < MAXROW; i++)
	{
		positionX = rand() % 300;
		positionY = TOP + (CARHEIGHT + SPACEROW) * i;
		direction = ForRandom[rand() % 2];
		for (int j = 0; j < MAXCAR; j++)
		{
			positionX += 350;
			createCar(positionX, positionY, a._car[i][j], direction);
		}
	}
	a._player = new player;
	createPlayer(a.window->getSize(), *a._player);
};
//Lưu mỗi hàng gồm direction, posX,posY tất cả các xe, sau đó đến vị trí người chơi
void saveGame(std::string file, Game& a, ofstream& save)
{
	save.open(file);
	if (save)
	{
		for (int i = 0; i < MAXROW; i++)
		{
			for (int j = 0; j < MAXCAR; j++)
				save << a._car[i][j].direction << " " << a._car[i][j].car.getPosition().x << " " << a._car[i][j].car.getPosition().y << endl;
		}
		save << a._player->Player.getPosition().x << " " << a._player->Player.getPosition().y << endl;
		save << SPEED;

		save.close();
	}
}
void loadgame(std::string file, Game& a, ifstream& load)
{
	load.open(file);
	int temp1;
	float temp2, temp3;
	if (load)
	{
		for (int i = 0; i < MAXROW; i++)
		{
			for (int j = 0; j < MAXCAR; j++)
			{
				load >> temp1;
				load >> temp2;
				load >> temp3;
				a._car[i][j].car.setPosition(sf::Vector2f(temp2, temp3));
				a._car[i][j].direction = temp1;
				if (a._car[i][j].direction > 0)
					a._car[i][j].load.loadFromFile(carFile1);
				else
					a._car[i][j].load.loadFromFile(carFile2);
				a._car[i][j].car.setTexture(a._car[i][j].load);
			}

		}
		load >> temp2;
		load >> temp3;
		a._player->Player.setPosition(sf::Vector2f(temp2, temp3));
		load >> temp1;
		SPEED = temp1;
		load.close();
	}
	else
	{
		std::cout << "Cant load the game\n";
	}
}


void loop(Game& a, BGTheme& music)
{
	effectSound sound;
	playMusic(themeBG, music);
	sf::Clock clock;
	clock.restart();
	ofstream file;
	while (a.window->isOpen())
	{
		sf::Event event;
		while (a.window->pollEvent(event))
		{
			if (event.type == event.Closed || event.key.code == sf::Keyboard::Escape)
				a.window->close();
			else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::W ||
				event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::Up)
				movePlayer(*a._player, event);
			else if (event.key.code == sf::Keyboard::T)
			{
				saveGame("save.txt", a, file);
				music.music.stop();
				Start(a, music);
			}
		}
		//Update game
		if (clock.getElapsedTime().asSeconds() <= 7.0f)
		{
			for (int i = 0; i < MAXROW; i++)
				for (int j = 0; j < MAXCAR; j++)
					moveCar(a._car[i][j], a.window->getSize(), SPEED * a._car[i][j].direction);

		}
		if (clock.getElapsedTime().asSeconds() >= 9.0f + SPEED)
			clock.restart();
		sf::Texture load;
		load.loadFromFile(bgFile);
		sf::Sprite Image;
		sf::Text textSpeed;
		sf::Font font;
		a.window->clear();
		font.loadFromFile(fontFile);
		textSpeed.setFont(font);
		Image.setTexture(load);
		a.window->draw(Image);
		for (int i = 0; i < MAXROW; i++)
		{
			for (int j = 0; j < MAXCAR; j++)
			{
				a.window->draw(a._car[i][j].car);
			}
		}
		a.window->draw(a._player->Player);

		for (int i = 0; i < MAXROW; i++)
		{
			for (int j = 0; j < MAXCAR; j++)
			{
				if (getShape(*a._player).getGlobalBounds().intersects(getCarShape(a._car[i][j]).getGlobalBounds()))
				{

					PlaySoundA("Assets/sound/crash.wav", NULL, SND_SYNC);
					sf::Text text;
					text.setFont(font);
					text.setString("YOU LOSE");
					text.setCharacterSize(100);
					text.setPosition(sf::Vector2f(viewSizeX / 2 - 300, viewSizeY / 2 - 100));
					a.window->draw(text);
					a.window->display();
					PlaySoundA("Assets/sound/lose.wav", NULL, SND_SYNC);
					music.music.stop();
					SPEED = 1;
					a._player->Player.setPosition(sf::Vector2f(viewSizeX / 2, viewSizeY - playerHeight));
					Start(a, music);
				}
			}
		}
		if (a._player->Player.getPosition().y < 20)
		{
			a._player->Player.setPosition(sf::Vector2f(viewSizeX / 2, viewSizeY - playerHeight));
			SPEED++;
			if (SPEED == MAXSPEED)
			{
				SPEED = 1;
				font.loadFromFile(fontFile);
				sf::Text text;
				text.setFont(font);
				text.setString("YOU WIN");
				text.setCharacterSize(100);
				text.setPosition(sf::Vector2f(viewSizeX / 2 - 250, viewSizeY / 2 - 100));
				a.window->draw(text);
				a.window->display();
				PlaySoundA("Assets/sound/win.wav", NULL, SND_SYNC);
				music.music.pause();
				Start(a, music);
			}

		}
		textSpeed.setString("SPEED: " + to_string(SPEED) + "/"+to_string(MAXSPEED));
		textSpeed.setCharacterSize(30);
		/*textSpeed.setPosition(sf::Vector2f(100, 100));*/
		a.window->draw(textSpeed);
		a.window->display();
	}
}

//==================================================================================================================
//														MENU
void createMenu(MENU& a)
{
	for (int i = 0; i < 4; i++)
	{
		a.font.loadFromFile(fontFile);
		a.text[i].setFont(a.font);
		a.text[i].setString(OPTION[i]);
		a.text[i].setPosition(sf::Vector2f(viewSizeX / 2 - 200, viewSizeY / 4 + 80 * i));
		a.text[i].setColor(sf::Color::White);
		a.text[i].setCharacterSize(60);
	}
	a.text[0].setColor(sf::Color::Yellow);
	a.selected = 0;
}

void drawMenu(MENU& a, sf::RenderWindow& window)
{
	for (int i = 0; i < 4; i++)
	{
		window.draw(a.text[i]);
	}
}

void MoveUp(MENU& a)
{
	if (a.selected - 1 >= 0)
	{
		a.text[a.selected].setColor(sf::Color::White);
		a.selected--;
		a.text[a.selected].setColor(sf::Color::Yellow);
	}
}
void MoveDown(MENU& a)
{
	if (a.selected + 1 < 4)
	{
		a.text[a.selected].setColor(sf::Color::White);
		a.selected++;
		a.text[a.selected].setColor(sf::Color::Yellow);
	}
}

void about(Game& a, MENU& menu, BGTheme& music)
{
	sf::Texture load;
	load.loadFromFile(aboutFile);
	sf::Sprite about_file;
	about_file.setTexture(load);
	while (a.window->isOpen())
	{
		sf::Event event;
		while (a.window->pollEvent(event))
		{

			if (sf::Keyboard::isKeyPressed)
				Start(a, music);

		}
		a.window->clear();
		a.window->draw(about_file);
		a.window->display();
	}
}

void Start(Game& a, BGTheme& music)
{
	MENU menu;
	createMenu(menu);
	while (a.window->isOpen())
	{
		sf::Event event;
		while (a.window->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::Up)
					MoveUp(menu);
				if (event.key.code == sf::Keyboard::Down)
					MoveDown(menu);
				if (event.key.code == sf::Keyboard::Enter)
				{
					if (menu.selected == 0)
					{
						loop(a, music);
					}
					if (menu.selected == 1)
					{
						ifstream file;
						loadgame("save.txt", a, file);
						loop(a, music);
					}
					if (menu.selected == 2)
					{
						about(a, menu, music);
					}
					if (menu.selected == 3)
						a.window->close();
				}
			}
		}
		a.window->clear();
		drawMenu(menu, *a.window);
		a.window->display();
	}
}
//================================================================================================================================

//===============================================================================================================================
int main()
{
	srand(time(NULL));
	BGTheme music;
	Game a;
	createGame(a);
	Start(a, music);
	return 0;

}