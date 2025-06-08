#include "../CloudServer.h"
#include <iostream>

void testAuth() {
    AuthManager auth;
    std::string resp;
    auth.login("{\"username\":\"user\",\"password\":\"pass\"}", resp);
    std::cout << "Auth response: " << resp << std::endl;
}

void testEncryption() {
    EncryptionManager enc;
    auto encrypted = enc.encrypt("hello");
    auto decrypted = enc.decrypt(encrypted);
    std::cout << "Encryption roundtrip: " << decrypted << std::endl;
}

void testAntiCheat() {
    AntiCheatManager ac;
    bool valid = ac.validate("game_data");
    std::cout << "AntiCheat valid: " << valid << std::endl;
}

void testModSandbox() {
    ModSandbox ms;
    std::string resp;
    ms.handleModUpload("mod_payload", resp);
    std::cout << "ModSandbox upload: " << resp << std::endl;
}

void testStorage() {
    StorageServer ss("data/");
    std::string resp;
    ss.saveData("{\"data\":\"save\"}", resp);
    std::cout << "Storage save: " << resp << std::endl;
    ss.loadData("{}", resp);
    std::cout << "Storage load: " << resp << std::endl;
}

int main() {
    testAuth();
    testEncryption();
    testAntiCheat();
    testModSandbox();
    testStorage();
    return 0;
}