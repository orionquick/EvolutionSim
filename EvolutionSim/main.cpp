#include "tile.h"
#include "entity.h"

void init()
{
	
	Tile::tileMap = vector<vector<Tile>>(TILEMAP_SIZE, vector<Tile>(TILEMAP_SIZE, Tile()));;

	int numWaterSources = 4;
	int numRocks = 10;
	int numEntities = 10;

	// Grass
	for (int y = 0; y < TILEMAP_SIZE; y++)
		for (int x = 0; x < TILEMAP_SIZE; x++)
			Tile::tileMap[x][y] = Tile(GRASS, Vector2f(x, y));

	// Water
	for (int i = 0; i < numWaterSources; i++)
	{
		Vector2f source(rand() % TILEMAP_SIZE, rand() % TILEMAP_SIZE);
		int radius = rand() % 3 + 3;
		for (int y = 0; y < TILEMAP_SIZE; y++)
			for (int x = 0; x < TILEMAP_SIZE; x++)
				if (pow(pow(abs(source.x - x), 2) + pow(abs(source.y - y), 2), 0.5) - 0.25 <= radius)
					Tile::tileMap[x][y] = Tile(WATER, Vector2f(x, y));
	}

	// Rocks
	for (int i = 0; i < numRocks; i++)
	{

		Vector2f source(rand() % TILEMAP_SIZE, rand() % TILEMAP_SIZE);
		if (Tile::tileMap[source.x][source.y].getType() != GRASS)
		{
			i--;
			continue;
		}
		int radius = rand() % 2;
		for (int y = 0; y < TILEMAP_SIZE; y++)
			for (int x = 0; x < TILEMAP_SIZE; x++)
				if (pow(pow(abs(source.x - x), 2) + pow(abs(source.y - y), 2), 0.5) <= radius)
					Tile::tileMap[x][y] = Tile(ROCK, Vector2f(x, y));
	}

	// Entities
	for (int i = 0; i < numEntities; i++)
	{
		Vector2f source(rand() % TILEMAP_SIZE, rand() % TILEMAP_SIZE);
		bool valid = true;
		if (Tile::tileMap[source.x][source.y].getType() != GRASS)
			valid = false;
		for (Entity e : Entity::entities)
			if (source == e.getPosition())
				valid = false;
		if (valid)
			Entity::entities.push_back(Entity(source));
		else
			i--;
	}
}

void processEvents(RenderWindow& w)
{
	Event event;
	while (w.pollEvent(event))
	{
		// Close window : exit
		if (event.type == Event::Closed)
			w.close();
	}
}

void update()
{
	for (int i = 0; i < Entity::entities.size(); i++)
	{
		Entity::entities[i].move(randDir());
		Entity::entities[i].eat();
		Entity::entities[i].reproduce();
		if (Entity::entities[i].getEnergy() < 0)
			Entity::entities.erase(Entity::entities.begin() + i);
	}
}

void drawTileMap(RenderWindow& w)
{
	for (int y = 0; y < TILEMAP_SIZE; y++)
		for (int x = 0; x < TILEMAP_SIZE; x++)
			Tile::tileMap[x][y].draw(w);
}

void drawEntities(RenderWindow& w)
{
	for (Entity e : Entity::entities)
		e.draw(w);
}

int main()
{
	srand(time(NULL));

	// initialize tilemap and entities
	init();

	RenderWindow window(VideoMode(800, 800), "Evolution Sim");
	while (window.isOpen())
	{
		// Process events
		processEvents(window);

		// Clear screen
		window.clear();

		// Update and draw
		update();
		drawTileMap(window);
		drawEntities(window);

		// Update window
		window.display();
	}
	return 0;
}