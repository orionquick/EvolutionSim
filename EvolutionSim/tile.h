#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>

using namespace sf;
using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.

#define TILEMAP_SIZE 80
#define TILE_SIZE 10

#define NULL_TILE -1
#define DIRT 0
#define GRASS 1
#define ROCK 2
#define WATER 3

#define LEFT 0
#define TOP 1
#define RIGHT 2
#define BOTTOM 3

#define CV rand() % 10
#define MUT_AMT rand() % 3 - 1 

class Tile
{
	private:
		int type;
		Vector2f position;
	public:
		Tile(int, Vector2f);
		static vector<vector<Tile>> tileMap;
		int getType();
		Vector2f getPosition();
		Color getColor();
		Color dirt, grass, rock, water;
		void setType(int);
		void setPosition(Vector2f);
		void draw(RenderWindow&);
};

Tile::Tile(int i = -1, Vector2f v = Vector2f(0, 0))
{
	type = i;
	position = v;
	dirt = Color(51 + CV, 25 + CV, 0 + CV);
	grass = Color(0 + CV, 51 + CV, 0 + CV);
	rock = Color(64 + CV, 64 + CV, 64 + CV);
	water = Color(0 + CV, 76 + CV, 153 + CV);
}

vector<vector<Tile>> Tile::tileMap;

int Tile::getType()
{
	return type;
}

Vector2f Tile::getPosition()
{
	return position;
}

Color Tile::getColor()
{
	if (type == DIRT)
		return dirt;
	else if (type == GRASS)
		return grass;
	else if (type == ROCK)
		return rock;
	else if (type == WATER)
		return water;
	else
		return Color();
}

void Tile::setType(int i)
{
	type = i;
}

void Tile::setPosition(Vector2f v = Vector2f(0, 0))
{
	position = v;
}

void Tile::draw(RenderWindow& w)
{
	RectangleShape tileRect(Vector2f(TILE_SIZE, TILE_SIZE));
	tileRect.setPosition(position.x * TILE_SIZE, position.y * TILE_SIZE);
	tileRect.setFillColor(this->getColor());
	w.draw(tileRect);
}

int randDir()
{
	return rand() % 4;
}