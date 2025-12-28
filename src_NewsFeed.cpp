#include "src_NewsFeed.h"
#include <iostream>

std::vector<NewsItem> NewsFeed::news;

void NewsFeed::Initialize() {
    news.clear();
    // Simulate fetching from a remote server/Wiki API
    news.push_back({"Update 6.0 Live!", "Check out the new community features.", "2023-10-27"});
    news.push_back({"Wiki Contest", "Top contributor wins 1000 Dodo Credits.", "2023-10-26"});
    news.push_back({"Security Alert", "Platinum at Work integration is now active.", "2023-10-25"});
}

void NewsFeed::FetchCommunityNews() {
    std::cout << "[NewsFeed] Connecting to Community Wiki API...\n";
    // Mock network delay
    std::cout << "[NewsFeed] 3 new articles downloaded.\n";
}

void NewsFeed::DisplayFeed() {
    std::cout << "\n=== DODO SPACE NEWS ===\n";
    for (const auto& item : news) {
        std::cout << "[" << item.date << "] " << item.title << "\n";
        std::cout << "   " << item.content << "\n";
    }
    std::cout << "=======================\n\n";
}
