#pragma once

#include "tile.h"

class Entity
{
private:
	Vector2f position;
	int lastDir;
	int age;
	int energy;
	int efficiency;
	int intelligence;
public:
	Entity(Vector2f);
	static vector<Entity> entities;
	Vector2f getPosition(int);
	Entity(Entity*);
	
	void setPosition(Vector2f);
	void move(int);
	void reproduce();
	void eat();

	int getEnergy();
	int getEfficiency();
	int getIntelligence();
	Color getColor();
	void draw(RenderWindow&);
};

Entity::Entity(Vector2f v = Vector2f(0, 0))
{
	position = v;
	lastDir = -1;
	age = 0;
	energy = 200;
	efficiency = rand() % 2 ? 5 : 15;	
	intelligence = rand() % 2 ? 5 : 15;
}

vector<Entity> Entity::entities;

Vector2f Entity::getPosition(int d = -1)
{
	if (d == LEFT && position.x > 0)
		return Vector2f(position.x - 1, position.y);
	if (d == TOP && position.y > 0)
		return Vector2f(position.x, position.y - 1);
	if (d == RIGHT && position.x < TILEMAP_SIZE - 1)
		return Vector2f(position.x + 1, position.y);
	if (d == BOTTOM && position.y < TILEMAP_SIZE - 1)
		return Vector2f(position.x, position.y + 1);
	return position;
}

Entity::Entity(Entity* parent)
{
	position = parent->getPosition();
	lastDir = -1;
	age = 0;
	energy = 200;
	efficiency = parent->getEfficiency() + MUT_AMT;
	efficiency = efficiency <= 0 ? 1 : efficiency;
	intelligence = parent->getIntelligence() + MUT_AMT;
	intelligence += intelligence <= 0 ? 1 : intelligence;
}

void Entity::setPosition(Vector2f v = Vector2f(0, 0))
{
	position = v;
}

void Entity::move(int d)
{
	if (age % efficiency == 0)
	{
		bool valid = true;
		bool smart = rand() % 11 + intelligence >= 15;

		for (int i = 0; i < 4; i++)
			if (smart && Tile::tileMap[getPosition(i).x][getPosition(i).y].getType() == GRASS)
				d = i;
		
		for (Entity e : entities)
			if (getPosition(d) == e.getPosition())
				valid = false;
		
		if (Tile::tileMap[getPosition(d).x][getPosition(d).y].getType() != GRASS && Tile::tileMap[getPosition(d).x][getPosition(d).y].getType() != DIRT)
			valid = false;
		
		if (valid)
			setPosition(getPosition(d));
	}
	energy -= 20 / efficiency;
	age++;
}

void Entity::reproduce()
{
	if (age > 10 * efficiency && energy >= 150)
	{
		entities.push_back(Entity(this));
		energy -= 100;
		age = 0;
	}
}

void Entity::eat()
{
	if (energy <= 175 && Tile::tileMap[getPosition().x][getPosition().y].getType() == GRASS)
	{
		Tile::tileMap[getPosition().x][getPosition().y].setType(DIRT);
		energy += 30;
	}
}

int Entity::getEnergy()
{
	return energy;
}

int Entity::getEfficiency()
{
	return efficiency;
}

int Entity::getIntelligence()
{
	return intelligence;
}

Color Entity::getColor()
{
	return Color(efficiency * 12, 0, intelligence * 12);
}

void Entity::draw(RenderWindow& w)
{
	RectangleShape tileRect(Vector2f(TILE_SIZE, TILE_SIZE));
	tileRect.setPosition(position.x * TILE_SIZE, position.y * TILE_SIZE);
	tileRect.setFillColor(this->getColor());
	w.draw(tileRect);
}
