#include "DodoSpaceGame.h"
#include "SpaceSystem.h"
#include "Portals.h"

DodoSpaceGame::DodoSpaceGame(EngineCore& engine) : m_engine(engine) {}

void DodoSpaceGame::load() {
    m_engine.space()->setupGalaxy();
    m_engine.portals()->registerSpacePortals();
    // Load entities, ships, planets, etc.
}

void DodoSpaceGame::update() {
    m_engine.space()->update();
    m_engine.portals()->update();
    // Player/ship logic
}

void DodoSpaceGame::render() {
    m_engine.space()->render(*m_engine.graphics());
    m_engine.portals()->render(*m_engine.graphics());
    // HUD, UI, etc.
}