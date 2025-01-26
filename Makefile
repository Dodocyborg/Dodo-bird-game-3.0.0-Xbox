TARGET = DodoBirdGame # Your executable name
SOURCES = Main.cpp game_logic.cpp Multiplayer.cpp DirectXinuit.cpp Lobby.cpp Login.cpp Matchmaking.cpp Network.cpp ProceduralTerrain.cpp Server.cpp USER.cpp utils.cpp Windows.cpp # List ALL your .cpp files here
OBJECTS = $(SOURCES:.cpp=.o)
CXX = g++ # Or clang++ if you prefer
CXXFLAGS = -std=c++17 -Wall -Wextra # Compiler flags (add more as needed)
LDFLAGS = -ld3d11 -ldxgi -ld3dcompiler -lwinmm -lcomdlg32 # Linker flags

# Rule to link the object files into the final executable
$(TARGET): $(OBJECTS)
        $(CXX) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to compile .cpp files into .o object files
%.o: %.cpp
        $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
        rm -f $(OBJECTS) $(TARGET)

.PHONY: clean # Declare clean as a phony target 
all: main

CXX = clang++
override CXXFLAGS += -g -Wall -Werror

SRCS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.cpp' -print | sed -e 's/ /\\ /g')
HEADERS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.h' -print)

main: $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o "$@"

main-debug: $(SRCS) $(HEADERS)
	NIX_HARDENING_ENABLE= $(CXX) $(CXXFLAGS) -O0  $(SRCS) -o "$@"

clean:
	rm -f main main-debug