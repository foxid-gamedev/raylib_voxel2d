#pragma once

#include <string>
#include <raylib.h>

namespace game 
{
	class Game 
	{
	public:
		explicit Game(size_t w, size_t h, const char* title);
		virtual ~Game();

		void run();
		void updateCameraFollow(Vector2 target, float rotation, float zoom);
		Vector2 getScreenToWorld(const Vector2& position) const;
	protected:
		virtual void init() = 0;
		virtual void update(float deltaTime) = 0;
		virtual void render() = 0;
		virtual void renderUI() = 0;
	private:
		Camera2D camera;
		size_t width;
		size_t height;
		std::string title;
	};
}
