#ifndef MODERATION_SYSTEM_H
#define MODERATION_SYSTEM_H

#include <string>
#include <vector>
#include <iostream>

struct Report {
    std::string reporterName;
    std::string reportedUser;
    std::string reason;
    long long timestamp;
    bool resolved;
};

class ModerationSystem {
public:
    ModerationSystem();

    // Automated Chat Bot
    std::string processChatMessage(const std::string& user, const std::string& message);

    // Reporting
    void submitReport(const std::string& reporter, const std::string& target, const std::string& reason);
    std::vector<Report> getPendingReports();

    // Support Line
    void contactSupport(const std::string& user, const std::string& issue);

    // AI Moderation (Mock)
    bool checkContent(const std::string& content);

    // Live AI Watcher
    void monitorChatStream(const std::string& user, const std::string& message);
    void intervene(const std::string& user);
    std::string simulatedModeratorChat();

private:
    std::vector<Report> reports;
    std::vector<std::string> bannedWords;

    // Mock connecting to moderator line
    void connectToModeratorLine(const std::string& user);
};

#endif // MODERATION_SYSTEM_H
