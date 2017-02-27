#pragma once
#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include <fstream>
#include <SFML\Audio.hpp>
using namespace std;
using namespace sf;
#pragma comment(lib,"openal32.lib")

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };
int Score = 0;
int Level = 1;
int GameState = 0; // 1-game 2-level up 3-game over
struct Point
{
	int x, y;
};
Point a[4];
Point b[4];

int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5 // O

};

std::ofstream Log;
Sprite Preview;


bool check()
{
	for (int i = 0; i<4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;

		else if
			(field[a[i].y][a[i].x]) return 0;

	return 1;
};
String toString(Int32 integer)
{
	char numstr[10]; // enough to hold all numbers up to 32-bits
	sprintf_s(numstr, "%i", integer);
	return numstr;
}
void OutArrayValueInLog(int WhatIndex, int WhatIndex2, int Who)
{
	ofstream Log("c:\\Destroy.txt", std::ofstream::out);
	string field_;

	field_ += "field[i][k]= " + toString(field[WhatIndex][WhatIndex2]) + "\n"
		+ "field[i][j]= " + toString(field[Who][WhatIndex2]) + "\n"
		+ "i= " + toString(WhatIndex2) + " j= " + toString(Who) + "k= " + toString(WhatIndex);
	Log << field_;
	Log.close();
}
///////check lines and destroy //////////
void CheckLineDestroy(Text &TextScore, RenderWindow &window,Sound &soundLevel)
{
	//указывает индексы где разрушаемая линия
	int k = M - 1;
	for (int i = M - 1; i>0; i--)
	{
		int count = 0;
		for (int j = 0; j<N; j++)
		{
			if (field[i][j])count++;
			field[k][j] = field[i][j];

		}
		if (count < N)
		{
			k--;

		}
		if (count == N)
		{
			Score += 100;
			string str = toString(Score);
			TextScore.setString(str);
			TextScore.setString(toString(Score));
			TextScore.setPosition(250, 100);
			window.draw(TextScore);
			soundLevel.play();
		}
	}

}
void Move(int dx,int GameState)
{
	if (GameState == 3) exit;
	//// <- Move -> ///
	for (int i = 0; i<4; i++)
	{
		b[i] = a[i];
		a[i].x += dx;
	}
	if (!check())
		for (int i = 0; i<4; i++)
			a[i] = b[i];
}
void Rotate()
{
	Point p = a[1]; //center of rotation
	for (int i = 0; i<4; i++)
	{
		int x = a[i].y - p.y;
		int y = a[i].x - p.x;
		a[i].x = p.x - x;
		a[i].y = p.y + y;
	}
}
/* colornum изменения цвета фигур */
void CheckCollider(int &colorNum, RenderWindow &window,Sprite &SpritesFigures)
{
	int aa, bb;
	// оставляем фигуры на дне
	for (int i = 0; i<4; i++)
		field[b[i].y][b[i].x] = colorNum;

	// генерация нового цвета фигуры 
	colorNum = 1 + rand() % 7;
	int n = rand() % 7;
	for (int i = 0; i<4; i++)
	{
		aa = figures[n][i] % 2;
		bb = figures[n][i] / 2;

		a[i].x = aa;
		a[i].y = bb;

		
		//window.draw(Preview);
		

		//a[i].x = figures[n][i] % 2;
		//a[i].y = figures[n][i] / 2;
		//выберем из массива фигур случайную
	}
       
}
void PreviewNextFigure(RenderWindow &window)
{
	for (int i = 0; i<M; i++)
		for (int j = 0; j < N; j++)
		{
			Preview.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));//0, 0, 18, 18));
            Preview.setPosition(235, 180);
			window.draw(Preview);
		}
	
}
void DrawScore(Text &TextScore, RenderWindow &window, sf::Vector2f &pos, int& Score)
{
	//set text score
	TextScore.setString("Score: "+toString(Score));
	TextScore.setPosition(pos);
	window.draw(TextScore);
}
void DrawLevel(Text &TextLevel, RenderWindow &window, sf::Vector2f &pos, int& Level)
{
	//set text score
	TextLevel.setString("Level: "+toString(Level));
	TextLevel.setPosition(pos);
	window.draw(TextLevel);

}
void DrawBackground(RenderWindow* window, Sprite background)
{

	/////////draw//////////
	window->clear(Color::White);
	window->draw(background);
}
void SetRectSprites(RenderWindow &window, Sprite &SpritesFigures)
{
	for (int i = 0; i<M; i++)
		for (int j = 0; j<N; j++)
		{
			if (field[i][j] == 0) continue;
			//if (field[i][j] != 0) GameState = 3; break;
			SpritesFigures.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
			//18х18 размер клетки сетки фона
			SpritesFigures.setPosition(j * 18, i * 18);
			SpritesFigures.move(28, 31); //offset
			window.draw(SpritesFigures);
		}
}
void SetMoveFigures(RenderWindow &window, Sprite &SpritesFigures, int &colorNum)
{
	for (int i = 0; i<4; i++)
	{
		SpritesFigures.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
		//18х18 размер клетки сетки фона
		SpritesFigures.setPosition(a[i].x * 18, a[i].y * 18);
		//28,31 выбор фигуры из спрайтсписка spritesheet
		SpritesFigures.move(28, 31);//28, 31); //offset
		window.draw(SpritesFigures);
	}
}
bool CheckVertical(int NumVertical,int NumStroka)
{
	
		if (field[NumVertical][NumStroka] != 0 && (field[NumVertical + 1][NumStroka] != 0) &&
		  (field[NumVertical + 2][NumStroka] != 0)) return true;
		else return false;
}
bool CheckGameOver()
{
	//int s = 0;
	//s += field[4][0] + field[4][1];

			if (field[4][0] != 0 && (field[4][1] != 0))  return true;
			if (field[4][1] != 0 && (field[4][2] != 0))  return true;
			if (field[4][2] != 0 && (field[4][3] != 0))  return true;
			if (field[4][3] != 0 && (field[4][4] != 0))  return true;
			if (field[4][4] != 0 && (field[4][5] != 0))  return true;
			if (field[4][5] != 0 && (field[4][6] != 0))  return true;
			if (field[4][6] != 0 && (field[4][7] != 0))  return true;
			if (field[4][7] != 0 && (field[4][8] != 0))  return true;
			if (field[4][8] != 0 && (field[4][9] != 0))  return true;
			if (field[4][9] != 0 && (field[5][0] != 0))  return true;

			if (field[5][0] != 0 && (field[5][1] != 0))  return true;
			if (field[5][1] != 0 && (field[5][2] != 0))  return true;
			if (field[5][2] != 0 && (field[5][3] != 0))  return true;
			if (field[5][3] != 0 && (field[5][4] != 0))  return true;
			if (field[5][4] != 0 && (field[5][5] != 0))  return true;
			if (field[5][5] != 0 && (field[5][6] != 0))  return true;
			if (field[5][6] != 0 && (field[5][7] != 0))  return true;
			if (field[5][7] != 0 && (field[5][8] != 0))  return true;
			if (field[5][8] != 0 && (field[5][9] != 0))  return true;
			if (field[5][9] != 0 && (field[6][0] != 0))  return true;
		else return false;
}
bool CheckAllVerticals()
{
	for(int j=0;j<N-2; j++)
	for (int i = 4; i < M - 2; i++)
		if (CheckVertical(i,j)) return true;
		else return false;
}
void MoveDown()
{
	for (int i = 0; i < 4; i++)
	{
		b[i] = a[i];
		a[i].y += 1;
	}
}
void OutLog()
{
	ofstream log("c:\\Field.txt", std::ofstream::out);
	string field_;
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
			field_ += toString(field[i][j]) + "\n";
	log << field_;
	log.close();

	ofstream alog("c:\\a.txt", std::ofstream::out);
	for (int i = 0; i < 4; i++)
		field_ += toString(a[i].x) + "," + toString(a[i].y) + "\n";
	alog << field_;
	alog.close();

	ofstream blog("c:\\b.txt", std::ofstream::out);
	for (int i = 0; i < 4; i++)
		field_ += toString(b[i].x) + "," + toString(b[i].y) + "\n";
	blog << field_;
	blog.close();

}
void CheckLevel(Text &TextLevel,RenderWindow &window,Vector2f &posLevel,float &delay)
{
	switch (Score)
	{
	case  500: Level = 2; DrawLevel(TextLevel, window, posLevel, Level);
		delay -= 0.01f; break;
	case 1000: Level = 3; DrawLevel(TextLevel, window, posLevel, Level);
		delay -= 0.01f; break;
	case 2000: Level = 4; DrawLevel(TextLevel, window, posLevel, Level);
		delay -= 0.01f; break;
	case 2500: Level = 5; DrawLevel(TextLevel, window, posLevel, Level);
		delay -= 0.01f; break;
	case 4000: Level = 6; DrawLevel(TextLevel, window, posLevel, Level);
		delay -= 0.01f; break;
	}
	

	DrawLevel(TextLevel, window, posLevel, Level);
	delay -= 0.01f;
	
}
int main()
{
	int dx = 0;

	int colorNum = 1;
	/* delay задержка до появления следующей фигуры*/
	float timer = 0, delay = 15;
	Texture t1, t2, t3;
	Sprite SpritesFigures, background, frame;
	/*--- Инициализация  ---*/

	srand(time(0));

	RenderWindow window(VideoMode(320, 480), "The Game!");

	Font font_;
	Font fontLevel;
	fontLevel.loadFromFile("images/arial.ttf");
	font_.loadFromFile("images/sansation.ttf");

	t1.loadFromFile("images/tiles.png");
	t2.loadFromFile("images/background.png");
	t3.loadFromFile("images/frame.png");

	SpritesFigures.setTexture(t1);
	background.setTexture(t2);
	frame.setTexture(t3);
	Preview.setTexture(t1);

	sf::SoundBuffer bufferMove;
	sf::SoundBuffer bufferDown;
	sf::SoundBuffer bufferLevelUp;
	sf::Music BackMusic;
	sf::Sound soundDown, soundMove, soundLevel;
	if (!BackMusic.openFromFile("images/backMusic.ogg"))
	{
		printf("not found music");
	}
	if (!bufferMove.loadFromFile("images/move.ogg"))
	{
		printf("not found move");
	}
	if (!bufferDown.loadFromFile("images/down.ogg"))
	{
		printf("not found down");
	}
	if (!bufferLevelUp.loadFromFile("images/destroy.ogg"))
	{
		printf("not found down");
	}

	soundLevel.setBuffer(bufferLevelUp);
	soundDown.setBuffer(bufferDown);
	soundMove.setBuffer(bufferMove);
	BackMusic.setLoop(true);

	//OutLog();
	/*--- Инициализация  ---*/
	
	
	bool Pause = false;
	bool rotate = false;

	Text TextScore;
	Text TextStateGame;
	TextStateGame.setFont(font_);
	TextStateGame.setColor(sf::Color::Red);
	TextStateGame.setPosition(100, 100);
	
	TextScore.setFont(font_);
	TextScore.setColor(sf::Color::Blue);
	TextScore.setCharacterSize(18);
	Vector2f posScore(230, 100);

    Text TextLevel;
	TextLevel.setFont(font_);
	TextLevel.setCharacterSize(18);
	TextLevel.setColor(sf::Color::Green);
	TextLevel.setString(toString(Level));
	Vector2f posLevel(230, 30);
BackMusic.play();
		if (BackMusic.getStatus() == sf::Sound::Playing)
		{
			printf("Play...");
		}
	Clock clock;
	while (window.isOpen())
	{
		PreviewNextFigure(window);

		if (CheckGameOver())
		{
			TextStateGame.setString("Game Over");
			GameState = 3;
			window.draw(TextStateGame);

			//Pause = true;
		}
		if (CheckAllVerticals())
		{
			TextStateGame.setString("Game Over");
				GameState = 3;
				window.draw(TextStateGame);
		}

		
		CheckLevel(TextLevel, window, posLevel, delay);
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
        
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			if (e.type == Event::KeyPressed)

				if (e.key.code == Keyboard::Up) 
				{
					rotate = true;
					/*if(soundMove.getStatus() != sf::Sound::Playing)
					soundMove.play(); */
				}

				else if (e.key.code == Keyboard::Left)
				{ 
					dx = -1;
					/*if (soundMove.getStatus() != sf::Sound::Playing)
						soundMove.play();*/
				}
				else if (e.key.code == Keyboard::Right)
				{ 
					dx = 1;
					/*if (soundMove.getStatus() != sf::Sound::Playing)
						soundMove.play();*/
				}
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) 
		{
			delay = 0.05;
			if (soundDown.getStatus() != sf::Sound::Playing) 
		{
				soundDown.play();
			
		} }
		if (Keyboard::isKeyPressed(Keyboard::Escape)) exit(0);
		if (Keyboard::isKeyPressed(Keyboard::N)) // NewGame
		{
			Pause = false;
			for(int i=0;i<M;i++)
				for(int j=0;j<N; j++)
			field[i][j] = 0;
			Score = 0; Level = 1;
			GameState = 0;
			TextStateGame.setString("");
		}
		if (Keyboard::isKeyPressed(Keyboard::P))
		{
			switch (Pause)
			{
			
			case	0 : Pause = true; break;
			case	1 : Pause = false; break;
			}
		}
		
		DrawScore(TextScore, window, posScore, Score);
		DrawLevel(TextLevel, window, posLevel, Level);
		
		//if (GameState != 3)
		//{
			Move(dx,GameState);
			//////Rotate//////
			if (rotate)
			{
				Rotate();
				/* проверка выхода за правую границу стакана ?*/
				if (!check())
					for (int i = 0; i < 4; i++)
						a[i] = b[i];
			}
		//}
       if (!Pause || GameState == 3)
			{
		/* задание времени игры */
		if (timer>delay)
		{
			MoveDown();
			// проверка на дно-коллайдер?
			if (!check()) CheckCollider(colorNum,window,SpritesFigures);
			timer = 0;
		}
}
		CheckLineDestroy(TextScore,window,soundLevel);
		dx = 0; rotate = 0; delay = 0.3;
		DrawBackground(&window,background);

		SetRectSprites(window,SpritesFigures);
		SetMoveFigures(window,SpritesFigures,colorNum);
        window.draw(frame);
		window.draw(TextScore);
		window.draw(TextLevel);
		window.draw(Preview);
		if (TextStateGame.getString() == "Game Over") GameState = 3;
		window.draw(TextStateGame);
		window.display();
		
		
	}

	return 0;
}