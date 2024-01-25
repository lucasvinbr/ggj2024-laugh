#pragma once

namespace Laugh {
constexpr auto TAG_PLAYER = "player";
constexpr auto TAG_DEAD = "dead";


constexpr unsigned int SPRITELAYER_BG = 0;
constexpr unsigned int SPRITELAYER_BEHIND_MIXER = 1;
constexpr unsigned int SPRITELAYER_INSIDE_MIXER = 2;
constexpr unsigned int SPRITELAYER_MIXER = 4;

/// <summary>
/// not really a layer, it's actually "no layer selected", so it's just "ignore this"
/// </summary>
constexpr unsigned int COLLAYER_IGNORE = 0;
constexpr unsigned int COLLAYER_WORLD = 1;
constexpr unsigned int COLLAYER_PLAYER = 2;
constexpr unsigned int COLLAYER_PLAYER_BODYPARTS = 4;
/// <summary>
/// layer for things that should collide with the world, but shouldn't block shots
/// </summary>
constexpr const unsigned int COLLAYER_SMALLPROPS = 8;

enum GameState {
	GAMESTATE_ENDED,
	GAMESTATE_STARTING,
	GAMESTATE_PLAYING,
	GAMESTATE_ENDING,
	GAMESTATE_PAUSED
};

constexpr auto WALKABLE_WORLD_BOUNDS = 100.0f;

constexpr auto MAX_VIEW_DISTANCE = 150.0f;

constexpr auto PROJECT_FOLDER = "ggj2024-laugh";
}