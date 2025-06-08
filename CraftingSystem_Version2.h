#pragma once
#include <vector>
#include <string>
#include <unordered_map>

struct Ingredient {
    std::string name;
    int quantity;
};

struct Recipe {
    std::string name;
    std::vector<Ingredient> ingredients;
    std::string result;
};

class CraftingSystem {
public:
    void addRecipe(const Recipe& recipe);
    bool canCraft(const std::string& recipeName, const std::unordered_map<std::string, int>& inventory);
    void craft(const std::string& recipeName, std::unordered_map<std::string, int>& inventory);
private:
    std::vector<Recipe> recipes;
};