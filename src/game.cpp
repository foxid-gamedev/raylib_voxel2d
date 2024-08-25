#include "pch.h"

#include "game.h"

game::Game::Game(size_t w, size_t h, const char* t) : 
	width(w), height(h), 
	title(t),
	tempAllocator(mem::BumpAllocator{ TEMP_ALLOCATOR_SIZE })
{
	InitWindow(width, height, title.c_str());
	SetWindowState(FLAG_VSYNC_HINT);
	SetTargetFPS(500);
	camera = {};

	std::cout << "temp_alloc in game: " << &tempAllocator << "\n";
}

game::Game::~Game() { }

void game::Game::run()
{
	init();

	while (!WindowShouldClose()) 
	{
		tempAllocator.reset();

		update(GetFrameTime());

		BeginDrawing();
		ClearBackground(Color{142,142,240});

		// world camera
		BeginMode2D(camera);
		render();
		EndMode2D();

		// ui stuff
		renderUI();
		EndDrawing();
	}
}

void game::Game::updateCameraFollow(Vector2 target, float rotation, float zoom)
{
	camera.target = target;
	camera.offset = { width / 2.f, height / 2.f };
	camera.rotation = rotation;
	camera.zoom = zoom;
}

Vector2 game::Game::getScreenToWorld(const Vector2& position) const
{
	return GetScreenToWorld2D(position, camera);
}
