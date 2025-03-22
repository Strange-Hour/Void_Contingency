#pragma once

namespace void_contingency {
namespace core {

class Game {
public:
    // Constructor and destructor for proper resource management
    Game();
    ~Game();

    // Core game loop methods
    void initialize();  // Sets up game state and resources
    void run();         // Main game loop
    void shutdown();    // Cleanup and resource release

private:
    bool is_running_;      // Controls the main game loop
    void process_input();  // Handles user input
    void update();         // Updates game state
    void render();         // Renders the current frame
};

}  // namespace core
}  // namespace void_contingency