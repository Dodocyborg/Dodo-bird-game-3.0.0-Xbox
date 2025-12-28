#include "src_ModerationSystem.h"
#include <algorithm>
#include <ctime>

ModerationSystem::ModerationSystem() {
    bannedWords = {"badword1", "scam", "cheat"};
}

std::string ModerationSystem::processChatMessage(const std::string& user, const std::string& message) {
    if (!checkContent(message)) {
        return "[SYSTEM] Message blocked by AI moderation.";
    }

    // AI Chat Bot responses
    std::string lowerMsg = message;
    std::transform(lowerMsg.begin(), lowerMsg.end(), lowerMsg.begin(), ::tolower);

    if (lowerMsg.find("help") != std::string::npos) {
        return "[AutoBot] Do you need assistance? Type /support for customer service.";
    }
    if (lowerMsg.find("report") != std::string::npos) {
        return "[AutoBot] To report a player, use /report <name> <reason>.";
    }

    return ""; // No automated response
}

bool ModerationSystem::checkContent(const std::string& content) {
    std::string lower = content;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    for (const auto& word : bannedWords) {
        if (lower.find(word) != std::string::npos) {
            return false;
        }
    }
    return true;
}

void ModerationSystem::monitorChatStream(const std::string& user, const std::string& message) {
    // "Live Watch" - Analyze every message for toxic patterns
    std::cout << "[LiveAI] Scanning message from " << user << "...\n";
    if (!checkContent(message)) {
        std::cout << "[LiveAI] Flagged toxic behavior. Intervening...\n";
        intervene(user);
    }
}

void ModerationSystem::intervene(const std::string& user) {
    std::cout << "[LiveAI] @ " << user << ": Please refrain from using inappropriate language. This is a warning.\n";
    // Could also auto-mute or temp-ban here
}

std::string ModerationSystem::simulatedModeratorChat() {
    return "[Moderator] I am an AI agent monitoring this chat for your safety.";
}

void ModerationSystem::submitReport(const std::string& reporter, const std::string& target, const std::string& reason) {
    Report r;
    r.reporterName = reporter;
    r.reportedUser = target;
    r.reason = reason;
    r.timestamp = std::time(nullptr);
    r.resolved = false;
    reports.push_back(r);
    std::cout << "[Moderation] Report received from " << reporter << " against " << target << ".\n";
}

std::vector<Report> ModerationSystem::getPendingReports() {
    return reports;
}

void ModerationSystem::contactSupport(const std::string& user, const std::string& issue) {
    std::cout << "[Support] User " << user << " requested support: " << issue << "\n";
    connectToModeratorLine(user);
}

void ModerationSystem::connectToModeratorLine(const std::string& user) {
    std::cout << "[Support] Connecting " << user << " to next available moderator...\n";
    // Mock connection delay
    std::cout << "[Support] Connected. How can we help you today?\n";
}
