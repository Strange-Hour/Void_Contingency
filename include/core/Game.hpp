#pragma once

namespace void_contingency {
namespace core {
class Game {
public:
    Game();
    ~Game();

    void initialize();
    void run();
    void shutdown();

private:
    bool is_running_;
    void process_input();
    void update();
    void render();
};

}  // namespace core
}  // namespace void_contingency