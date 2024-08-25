#include "pch.h"

#include "game.h"
#include "player.h"
#include "tilemap.h"
#include "chunk.h"
#include "memory/bump_allocator.h"
#include "moving_platform.h"
#include "interface_all_actors.h"
#include "interface_all_solids.h"
#include "collision_check_manager.h"
#include "solid_collision_check.h"

class Game1 : public game::Game, game::IAllActors, game::IAllSolids
{
public:
	Game1() : 
		Game(1600, 900, "Platformer"), 
		chunk(0,0),
		solidCollisions{ this },
		player{ collisionManager },
		movingPlatform{{0,0}, 150.f, this, tempAllocator}
	{
		//movingPlatform = game::MovingPlatform({ 0, 0 }, 150.f, this, tempAllocator);
		//movingPlatform{ game::MovingPlatform{ {0,0}, 150.f, this, tempAllocator } };
		collisionManager.addCheck(&solidCollisions);
		collisionManager.addCheck(&tilemap);

	std::cout << "temp_alloc in game1: " << &tempAllocator << "\n";
	}
	~Game1() override {}

protected:
	void init() override 
	{
		player.setPosition(player_start);
		tilemap.generateCells();
		chunk.fillAll(game::TileType::STONE);

		movingPlatform.setPosition({ 60.f * 16.f, 62.f * 16.f });
		movingPlatform.setCollider(0, 0, 32, 16);
	}

	void update(float deltaTime) override
	{
		actors_size = 0;
		actorsPtr = reinterpret_cast<game::Actor**>(tempAllocator.allocate(sizeof(game::Player*)));
		*actorsPtr = reinterpret_cast<game::Actor*>(& player);

		//memcpy(actorsPtr, &player, sizeof(game::Player*));
		actors_size=1;

		if (IsKeyPressed(KEY_ESCAPE))
			CloseWindow();

		if (IsKeyPressed(KEY_R))
		{
			player.setPosition(player_start);
		}

		if (IsKeyPressed(KEY_T))
		{
			tilemap.generateCells();
			player.setPosition(player_start);
		}

		// selecting tiles
		if (IsKeyPressed(KEY_KP_1)) 
		{
			selected_tile = game::TileType::GRASS;
			std::cout << "selected tile: grass\n";
		}
		if (IsKeyPressed(KEY_KP_2)) 
		{
			selected_tile = game::TileType::DIRT;
			std::cout << "selected tile: dirt\n";
		}
		if (IsKeyPressed(KEY_KP_3))
		{
			selected_tile = game::TileType::STONE;
			std::cout << "selected tile: stone\n";
		}
		if (IsKeyPressed(KEY_KP_0)) 
		{
			selected_tile = game::TileType::NONE;
			std::cout << "selected tile: [none]\n";
		}

		// moving the platform (temp)
		if (IsKeyDown(KEY_A))
		{
			movingPlatform.setMoveDirection(-1.f, 0.f);
		}
		else if (IsKeyDown(KEY_D))
		{
			movingPlatform.setMoveDirection(1.f, 0.f);
		}
		else
		{
			movingPlatform.setMoveDirection(0.f, 0.f);
		}

		// updating mouse pos
		Vector2 mousePos = getScreenToWorld(GetMousePosition());
		mousePos.x -= 8.f;
		mousePos.y -= 8.f;

		Vector2 cellPos = tilemap.getNearestCell(mousePos);
		mouseCellPos.x = cellPos.x * game::TILEMAP_CELL_SIZE;
		mouseCellPos.y = cellPos.y * game::TILEMAP_CELL_SIZE;

		// placing tiles
		if (selected_tile != game::TileType::NONE)
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				tilemap.setCell(static_cast<size_t>(cellPos.x), static_cast<size_t>(cellPos.y), selected_tile);
		}

		// removing tiles
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
			tilemap.setCell(static_cast<size_t>(cellPos.x), static_cast<size_t>(cellPos.y), game::TileType::NONE);
		
		// zooming
		float mouseWheelMove = GetMouseWheelMove();
		camera_zoom -= mouseWheelMove * 0.2f;
		camera_zoom = std::clamp(camera_zoom, 0.1f, 5.f);


		player.update(deltaTime);
		updateCameraFollow(player.getPosition(), 0.f, camera_zoom);
		// moving platform
		movingPlatform.update(deltaTime);

	}

	void render() override
	{
		tilemap.render();
		chunk.render();
		player.render();
		movingPlatform.render();

		if (selected_tile != game::TileType::NONE)
		{
			Rectangle mouseRect = {
				mouseCellPos.x,
				mouseCellPos.y,
				16.f,
				16.f
			};

			DrawRectangleLines(mouseRect.x, mouseRect.y, 16.f, 16.f, YELLOW);
		}
		DrawRectangleLinesEx({ 0, 0, game::TILEMAP_WIDTH * game::TILEMAP_CELL_SIZE, game::TILEMAP_HEIGHT * game::TILEMAP_CELL_SIZE }, 4.f, BLACK);
	}

	void renderUI() override
	{
		auto fps = "FPS: " + std::to_string(GetFPS());
		DrawText(fps.c_str(), 16, 16, 20, BLACK);

		auto selected_text = "Selected Tile: " + std::to_string(static_cast<int>(selected_tile));
		DrawText(selected_text.c_str(), 16, 32, 20, BLACK);
	}
private:
	game::Tilemap tilemap;
	game::Chunk chunk;

	game::SolidCollisionCheck solidCollisions;
	game::CollisionCheckManager collisionManager;
	
	game::MovingPlatform movingPlatform;
	game::Player player;

	Vector2 mouseCellPos{ 0.f };
	Vector2 player_start{ 64.f*16.f, 60.f*16.f };
	float camera_zoom = 2.0f;

	game::Actor** actorsPtr;
	size_t actors_size;

	game::TileType selected_tile{ game::TileType::NONE };

	// Geerbt über IAllActors
	std::tuple<game::Actor**, size_t> getAllActors() override
	{
		return { actorsPtr, actors_size };
	}

	// Geerbt über IAllSolids
	std::vector<game::Solid*> getAllSolids() override
	{
		return { &movingPlatform };
	}

};

int main() {
	Game1 game;
	game.run();
}