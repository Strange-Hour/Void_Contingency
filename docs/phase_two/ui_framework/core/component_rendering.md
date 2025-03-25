# Implementing Component Rendering

## Navigation

- Previous: [Styling System Tutorial](./styling_system.md)
- Next: [Component Animations Tutorial](../components/component_animations.md)

This tutorial will guide you through implementing the rendering system for UI components in "Void Contingency", focusing on efficient batch rendering and shader management.

## Overview

The component rendering system will:

- Support batch rendering for performance
- Handle different rendering modes (solid, textured, text)
- Manage shaders and materials
- Support clipping and masking
- Enable post-processing effects

## Implementation Steps

### 1. Create Renderer Interface

Create `include/ui/render/Renderer.hpp`:

```cpp
#pragma once

#include "ui/Component.hpp"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace void_contingency {
namespace ui {

struct Vertex {
    glm::vec2 position;
    glm::vec2 texCoord;
    glm::vec4 color;
};

struct RenderCommand {
    enum class Type {
        Solid,
        Textured,
        Text,
        Mask
    };

    Type type;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    glm::mat4 transform;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Shader> shader;
    glm::vec4 color;
    float alpha;
    bool clipEnabled;
    glm::vec4 clipRect;
};

class Renderer {
public:
    static Renderer& getInstance();

    // Initialization
    void initialize();
    void shutdown();

    // Rendering
    void beginFrame();
    void endFrame();
    void submit(const RenderCommand& command);
    void flush();

    // Shader management
    void setShader(const std::string& name, std::shared_ptr<Shader> shader);
    std::shared_ptr<Shader> getShader(const std::string& name) const;

    // Texture management
    void setTexture(const std::string& name, std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> getTexture(const std::string& name) const;

    // State management
    void setViewport(const glm::vec2& size);
    void setClipRect(const glm::vec4& rect);
    void clearClipRect();

private:
    Renderer() = default;
    ~Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    // Render state
    std::vector<RenderCommand> commands_;
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders_;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures_;
    glm::vec2 viewportSize_;
    glm::vec4 currentClipRect_;
    bool hasClipRect_{false};

    // Helper methods
    void sortCommands();
    void batchCommands();
    void executeCommands();
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Shader System

Create `include/ui/render/Shader.hpp`:

```cpp
#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace void_contingency {
namespace ui {

class Shader {
public:
    Shader(const std::string& vertexSource, const std::string& fragmentSource);
    ~Shader();

    void use();
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setMat4(const std::string& name, const glm::mat4& value);

private:
    uint32_t program_;
    std::unordered_map<std::string, int> uniformLocations_;
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Texture System

Create `include/ui/render/Texture.hpp`:

```cpp
#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace void_contingency {
namespace ui {

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();

    void bind(uint32_t unit = 0);
    void unbind();

    glm::vec2 getSize() const { return size_; }
    float getAspectRatio() const { return aspectRatio_; }

private:
    uint32_t id_;
    glm::vec2 size_;
    float aspectRatio_;
    int channels_;
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Renderer Implementation

Create `src/ui/render/Renderer.cpp`:

```cpp
#include "ui/render/Renderer.hpp"
#include "ui/render/Shader.hpp"
#include "ui/render/Texture.hpp"
#include <algorithm>

namespace void_contingency {
namespace ui {

Renderer& Renderer::getInstance() {
    static Renderer instance;
    return instance;
}

void Renderer::initialize() {
    // Initialize default shaders
    auto solidShader = std::make_shared<Shader>(
        R"(
            #version 330 core
            layout (location = 0) in vec2 aPos;
            layout (location = 1) in vec2 aTexCoord;
            layout (location = 2) in vec4 aColor;

            out vec2 TexCoord;
            out vec4 Color;

            uniform mat4 transform;

            void main() {
                gl_Position = transform * vec4(aPos, 0.0, 1.0);
                TexCoord = aTexCoord;
                Color = aColor;
            }
        )",
        R"(
            #version 330 core
            in vec2 TexCoord;
            in vec4 Color;
            out vec4 FragColor;

            uniform sampler2D tex;
            uniform bool useTexture;

            void main() {
                if (useTexture) {
                    FragColor = texture(tex, TexCoord) * Color;
                } else {
                    FragColor = Color;
                }
            }
        )"
    );
    setShader("solid", solidShader);

    // Initialize default textures
    auto defaultTexture = std::make_shared<Texture>("assets/textures/default.png");
    setTexture("default", defaultTexture);
}

void Renderer::submit(const RenderCommand& command) {
    commands_.push_back(command);
}

void Renderer::flush() {
    sortCommands();
    batchCommands();
    executeCommands();
    commands_.clear();
}

void Renderer::sortCommands() {
    std::sort(commands_.begin(), commands_.end(),
        [](const RenderCommand& a, const RenderCommand& b) {
            // Sort by shader first
            if (a.shader != b.shader) {
                return a.shader < b.shader;
            }
            // Then by texture
            if (a.texture != b.texture) {
                return a.texture < b.texture;
            }
            // Finally by type
            return static_cast<int>(a.type) < static_cast<int>(b.type);
        });
}

void Renderer::batchCommands() {
    // Combine commands with same shader and texture
    std::vector<RenderCommand> batchedCommands;
    RenderCommand currentBatch;

    for (const auto& command : commands_) {
        if (currentBatch.type == command.type &&
            currentBatch.shader == command.shader &&
            currentBatch.texture == command.texture) {
            // Merge vertices and indices
            size_t vertexOffset = currentBatch.vertices.size();
            currentBatch.vertices.insert(currentBatch.vertices.end(),
                command.vertices.begin(), command.vertices.end());

            for (uint32_t index : command.indices) {
                currentBatch.indices.push_back(index + vertexOffset);
            }
        } else {
            if (!currentBatch.vertices.empty()) {
                batchedCommands.push_back(currentBatch);
            }
            currentBatch = command;
        }
    }

    if (!currentBatch.vertices.empty()) {
        batchedCommands.push_back(currentBatch);
    }

    commands_ = std::move(batchedCommands);
}

void Renderer::executeCommands() {
    for (const auto& command : commands_) {
        command.shader->use();

        // Set uniforms
        command.shader->setMat4("transform", command.transform);
        command.shader->setBool("useTexture", command.texture != nullptr);

        if (command.texture) {
            command.texture->bind();
        }

        // Set vertex data
        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, command.vertices.size() * sizeof(Vertex),
                    command.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, command.indices.size() * sizeof(uint32_t),
                    command.indices.data(), GL_STATIC_DRAW);

        // Draw
        glDrawElements(GL_TRIANGLES, command.indices.size(), GL_UNSIGNED_INT, 0);
    }
}

} // namespace ui
} // namespace void_contingency
```

### 5. Create Renderer Tests

Create `tests/unit/ui/render/RendererTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/render/Renderer.hpp"
#include "ui/render/Shader.hpp"
#include "ui/render/Texture.hpp"

using namespace void_contingency::ui;

TEST(RendererTest, CommandSubmission) {
    auto& renderer = Renderer::getInstance();

    // Create test command
    RenderCommand command;
    command.type = RenderCommand::Type::Solid;
    command.vertices = {
        {{0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
        {{1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
        {{0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
    };
    command.indices = {0, 1, 2};

    // Submit command
    renderer.submit(command);

    // Verify command was added
    renderer.flush();
}

TEST(RendererTest, ShaderManagement) {
    auto& renderer = Renderer::getInstance();

    // Create test shader
    auto shader = std::make_shared<Shader>(
        "void main() { gl_Position = vec4(0.0); }",
        "void main() { FragColor = vec4(1.0); }"
    );

    // Set shader
    renderer.setShader("test", shader);

    // Verify shader was set
    EXPECT_EQ(renderer.getShader("test"), shader);
}

TEST(RendererTest, TextureManagement) {
    auto& renderer = Renderer::getInstance();

    // Create test texture
    auto texture = std::make_shared<Texture>("test.png");

    // Set texture
    renderer.setTexture("test", texture);

    // Verify texture was set
    EXPECT_EQ(renderer.getTexture("test"), texture);
}
```

## Next Steps

1. Implement component animations
2. Add post-processing effects
3. Implement clipping and masking
4. Add performance optimizations

## Common Pitfalls

1. **Batch Rendering**: Handle state changes efficiently
2. **Shader Management**: Manage shader uniforms properly
3. **Texture Management**: Handle texture loading and caching
4. **Performance**: Optimize draw calls and state changes

## Best Practices

1. Use batch rendering for performance
2. Implement proper shader management
3. Handle texture resources efficiently
4. Support clipping and masking
5. Optimize render state changes

## Additional Resources

- [Styling System Tutorial](./styling_system.md)
- [Component Animations Tutorial](../components/component_animations.md)
- [Post-Processing Effects Tutorial](../components/post_processing.md)
- [Theme System Tutorial](../themes/theme_system.md)
- [Performance Optimization Tutorial](../optimization/performance_optimization.md)
