#ifndef INPUT_H
#define INPUT_H

#include <windows.h>

class Input {
public:
    static bool IsKeyDown(int key) {
        return (GetAsyncKeyState(key) & 0x8000) != 0;
    }
};

#endif // INPUT_H
