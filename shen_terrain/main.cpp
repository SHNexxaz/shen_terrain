#include <iostream>
#include <noise/noise.h>
#include "noiseutils.h"
#include <SFML/Graphics.hpp>
using namespace noise;
int tileNum[2048];
float tempArray[2048];
int tile_number; 
module::Perlin myModule;
/*int main(int argc, char** argv)
{
	module::Perlin myModule;
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(256, 256);
	heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
	heightMapBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.Render();

	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("tutorial.bmp");
	writer.WriteDestFile();

	return 0;
	




}*/

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

	bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
	{
		mapSize = width * height;

		// load the tileset texture
		if (!m_tileset.loadFromFile(tileset))
			return false;

		// resize the vertex array to fit the level size
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(width * height * 4);

		// populate the vertex array, with one quad per tile
		for (unsigned int i = 0; i < width; ++i)
			for (unsigned int j = 0; j < height; ++j)
			{
				// get the current tile number
				tile_number =  tiles[i+j*width];

				// find its position in the tileset texture
				int tu = tile_number % (m_tileset.getSize().x / tileSize.x);
				int tv = tile_number / (m_tileset.getSize().x / tileSize.x);

				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

				// define its 4 corners
				quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
				quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
				quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
				quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
			}
			

			


		return true;
	}

	void generateValue()
	{
		//int x = 0;
		
	
			for (unsigned int i = 0; i < 64; i++)
			{
				for (unsigned int n = 0; n < 32; n++)
				{
					tempArray[i+64*n] = (float)myModule.GetValue(i, n, 1.15);
					//tempArray[i+64*n] = fillTheMap(values[i][n]);
					//std::cout << tempArray[n+64*i] << std::endl;
					//x++;
					
				}
			}
			
	}

	int fillTheMap(float val)
	{
		//convert perlin value to int 
					if (val <= -0.75 && val > -1.0)
					{
						return  1;
					}
				    if (val<= -0.5 && val > -0.75)
					{
						return 2;
					}
					if (val <= -0.25 && val > -0.5)
					{
						return 4;
					}
					if (val <= 0.0 && val > -0.25)
					{
						return 8;
					}
					if (val <= 0.25 && val > 0.0)
					{
						return 10;
					}
					if (val <= 0.5 && val > 0.25)
					{
						return 11;
					}
					if (val <= 0.75 && val > 0.5)
					{
						return 13;
					}
					if (val <= 1.0 && val > 0.75)
					{
						return 15;
					}
	}

	void getTileNumber()
	{
		for (int j = 0; j < 2048; j++)
		{
			tileNum[j] = fillTheMap(tempArray[j]);
			std::cout << tileNum[j] << std::endl;
		}
	}

	void changeSeed()
	{
		myModule.SetSeed(rand());
	}


private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = &m_tileset;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	int mapSize; 
	float values [64][32];
};

int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(512, 256), "Tilemap");

	// define the level with an array of tile indices
	/*const int level[] =
	{
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
		1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
		0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
		0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
		2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
		0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
	};*/

	// create the tilemap from the level definition
	TileMap map;
	map.generateValue();
	map.getTileNumber();
	if (!map.load("tileset.png", sf::Vector2u(16, 16), tileNum, 64, 32))
		return -1;

	// run the main loop
		while (window.isOpen())
		{
			// handle events
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				// left key is pressed: move our character
				map.changeSeed();
				map.generateValue();
				map.getTileNumber();

				if (!map.load("tileset.png", sf::Vector2u(16, 16),tileNum, 64, 32))
					return -1;
			}

			// draw the map
			window.clear();
			window.draw(map);
			window.display();
		}


	return 0;
}
