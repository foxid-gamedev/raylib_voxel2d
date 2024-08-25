#include "pch.h"

#include "game.h"
#include "player.h"
#include "tilemap.h"
#include "chunk.h"
#include "memory/bump_allocator.h"
#include "memory/pointer_array.h"
#include "moving_platform.h"
#include "interface_all_actors.h"
#include "interface_all_solids.h"
#include "collision_check_manager.h"
#include "solid_collision_check.h"

class Game1 : public game::Game, game::IAllActors, game::IAllSolids
{
// construction // --------------------------------------------------------------------------------
public:
	Game1() : 
		Game(1600, 900, "Platformer"), 
		chunk(0,0),
		solidCollisions{ this },
		player{ collisionManager },
		movingPlatform{{0,0}, 150.f, this, tempAllocator},
		actorsArray{ tempAllocator, 0 },
		solidArray{ tempAllocator, 0 }
	{
		collisionManager.addCheck(&solidCollisions);
		collisionManager.addCheck(&tilemap);

	}
	~Game1() override {}

protected:
// load/set data // -------------------------------------------------------------------------------
	void init() override 
	{
		player.setPosition(player_start);
		tilemap.generateCells();
		chunk.fillAll(game::TileType::STONE);

		movingPlatform.setPosition({ 60.f * 16.f, 62.f * 16.f });
		movingPlatform.setCollider(0, 0, 32, 16);
	}

// gameplay loop // -------------------------------------------------------------------------------
	void update(float deltaTime) override
	{
		// update mouse position //
		Vector2 mousePos = getScreenToWorld(GetMousePosition());
		mousePos.x -= 8.f;
		mousePos.y -= 8.f;

		Vector2 cellPos = tilemap.getNearestCell(mousePos);
		mouseCellPos.x = cellPos.x * game::TILEMAP_CELL_SIZE;
		mouseCellPos.y = cellPos.y * game::TILEMAP_CELL_SIZE;
		
		// input //
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

		// tiles placement //
		if (selected_tile != game::TileType::NONE)
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				tilemap.setCell
				(
					static_cast<size_t>(cellPos.x), 
					static_cast<size_t>(cellPos.y), 
					selected_tile
				);
			}
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			tilemap.setCell
			(
				static_cast<size_t>(cellPos.x), 
				static_cast<size_t>(cellPos.y),
				game::TileType::NONE
			);
		}
		
		// zoom //
		float mouseWheelMove = GetMouseWheelMove();
		camera_zoom -= mouseWheelMove * 0.2f;
		camera_zoom = std::clamp(camera_zoom, 0.1f, 5.f);

		// fill temporary allocator //
		actorsArray = mem::PointerArray<game::Actor>(tempAllocator, 1);
		actorsArray.set(&player, 0);

		solidArray = mem::PointerArray<game::Solid>(tempAllocator, 1);
		solidArray.set(&movingPlatform, 0);
	
		// call update functions //
		player.update(deltaTime);
		movingPlatform.update(deltaTime);
		updateCameraFollow(player.getPosition(), 0.f, camera_zoom);
	}

// render loop // ---------------------------------------------------------------------------------
	void render() override
	{
		// render world objects //
		tilemap.render();
		chunk.render();
		player.render();
		movingPlatform.render();

		// draw tile placement rectangle //
		if (selected_tile != game::TileType::NONE)
		{
			Rectangle mouseRect = {
				mouseCellPos.x, mouseCellPos.y,
				16.f, 16.f
			};

			DrawRectangleLines
			(
				static_cast<int>(mouseRect.x), static_cast<int>(mouseRect.y), 
				16, 16, 
				YELLOW
			);
		}

		// draw world border //
		DrawRectangleLinesEx
		(
			{
				0,0,
				game::TILEMAP_WIDTH * game::TILEMAP_CELL_SIZE,
				game::TILEMAP_HEIGHT * game::TILEMAP_CELL_SIZE 
			}, 
			4.f, 
			BLACK
		);
	}

// render user interface // -----------------------------------------------------------------------
	void renderUI() override
	{
		auto fps = "FPS: " + std::to_string(GetFPS());
		DrawText(fps.c_str(), 16, 16, 20, BLACK);

		auto selected_text = "Selected Tile: " + std::to_string(static_cast<int>(selected_tile));
		DrawText(selected_text.c_str(), 16, 32, 20, BLACK);
	}
// handling interfaces // -------------------------------------------------------------------------
private:
	mem::PointerArray<game::Actor>& getAllActors() override
	{
		return actorsArray;
	}

	mem::PointerArray<game::Solid>& getAllSolids() override
	{
		return solidArray;
	}
// game data // -----------------------------------------------------------------------------------
private:
	// world data
	game::Tilemap tilemap;
	game::Chunk chunk;

	// collision handlers
	game::SolidCollisionCheck solidCollisions;
	game::CollisionCheckManager collisionManager;
	
	// game objects
	game::MovingPlatform movingPlatform;
	game::Player player;

	// util data
	Vector2 mouseCellPos{ 0.f };
	Vector2 player_start{ 64.f*16.f, 60.f*16.f };
	float camera_zoom = 2.0f;
	game::TileType selected_tile{ game::TileType::NONE };

	// temporary pointer collections
	mem::PointerArray<game::Actor> actorsArray;
	mem::PointerArray<game::Solid> solidArray;
};

// entry point // ---------------------------------------------------------------------------------
int main() {
	Game1 game;
	game.run();
}
// ------------------------------------------------------------------------------------------------