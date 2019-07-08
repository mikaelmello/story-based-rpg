#include "IFSM.hpp"
#include "Antagonist.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "Player.hpp"

IFSM::IFSM(GameObject& object) : object(object), pop_requested(false) {}

IFSM::~IFSM() {}

bool IFSM::UpdatePosition(float dt) {
  auto antagonist_cpt = object.GetComponent(AntagonistType);
  if (!antagonist_cpt) {
    throw std::runtime_error("objeto sem antagonista em IFSM::UpdatePosition");
  }

  auto ant = std::dynamic_pointer_cast<Antagonist>(antagonist_cpt);

  unsigned& k = ant->paths.top().first;
  std::vector<Vec2>& path = ant->paths.top().second;

  bool update = timer.Get() <= 0.1;

  if (k < path.size() && update) {
    ant->position = path[k++];
  }

  if (!update) {
    timer.Restart();
  }

  timer.Update(dt);
  return k >= path.size();
}

IFSM::Walkable IFSM::GetWalkable(GameObject& pivot) {
  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();
  auto tilemap = state.GetCurrentTileMap();

  auto antagonist_cpt = object.GetComponent(AntagonistType);
  if (!antagonist_cpt) {
    throw std::runtime_error("objeto sem antagonista em IFSM::GetWalkable");
  }

  auto player_cpt = GameData::PlayerGameObject->GetComponent(PlayerType);
  if (!player_cpt) {
    throw std::runtime_error("player sem player em IFSM::GetWalkable");
  }

  auto player_col_cpt = GameData::PlayerGameObject->GetComponent(ColliderType);
  if (!player_col_cpt) {
    throw std::runtime_error("player sem collider em IFSM::GetWalkable");
  }

  auto ant = std::dynamic_pointer_cast<Antagonist>(antagonist_cpt);
  auto player = std::dynamic_pointer_cast<Player>(player_cpt);
  auto player_collider = std::dynamic_pointer_cast<Collider>(player_col_cpt);

  int tile_dim = tilemap->GetLogicalTileDimension();

  int cells_width = round(player_collider->box.w / tile_dim);
  int cells_height = round(player_collider->box.h / tile_dim);
  if (cells_width % 2 == 1) cells_width--;

  int x = player->position.x - cells_width / 2;
  int y = player->position.y - cells_height;

  bool can_walk;
  Vec2 position = player->position;

  can_walk = Helpers::CanWalk(object, position);
  if (can_walk) {
    return {can_walk, position};
  }

  position = player_collider->box.Center();
  can_walk = Helpers::CanWalk(object, position);
  if (can_walk) {
    return {can_walk, position};
  }

  position = Vec2(x, y);
  can_walk = Helpers::CanWalk(object, position);
  if (can_walk) {
    return {can_walk, position};
  }

  position = Vec2(x, y + cells_height);
  can_walk = Helpers::CanWalk(object, position);
  if (can_walk) {
    return {can_walk, position};
  }

  position = Vec2(x + cells_width, y);
  can_walk = Helpers::CanWalk(object, position);
  if (can_walk) {
    return {can_walk, position};
  }

  position = Vec2(x + cells_width, y + cells_height);
  can_walk = Helpers::CanWalk(object, position);
  if (can_walk) {
    return {can_walk, position};
  }

  auto obj = object.GetComponent(Types::AntagonistType);
  if (!obj) {
    throw std::invalid_argument("sem antagonista em IFSM::GetWalkable");
  }

  return {false, ant->position};
}
