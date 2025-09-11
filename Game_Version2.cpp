void Game::update() {
    float deltaTime = renderer.getDeltaTime();
    player.update(deltaTime);

    // Procedural terrain and decoration
    glm::vec3 playerPos = player.getPosition();
    auto biome = biomeGenerator.getBiomeForPosition(playerPos.x, playerPos.z);
    std::vector<std::string> decorations;
    biomeGenerator.decorateChunk(playerPos.x, playerPos.z, decorations);

    // Place decorations in the renderer (pseudo)
    renderer.placeDecorations(decorations, biome.color);

    // Crafting, quests, etc.
    craftingSystem.update(player, inventory);
    questSystem.update(player, world);
}