#ifndef NEWS_FEED_H
#define NEWS_FEED_H

#include <vector>
#include <string>

struct NewsItem {
    std::string title;
    std::string content;
    std::string date;
};

class NewsFeed {
public:
    static void Initialize();
    static void FetchCommunityNews();
    static void DisplayFeed();

private:
    static std::vector<NewsItem> news;
};

#endif // NEWS_FEED_H
