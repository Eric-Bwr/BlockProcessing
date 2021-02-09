#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "../EventSystem/Event/Event.h"
#include "../Util/Key.h"

namespace gfx {
    class Camera {
    private:
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
        std::vector<ib::Key> keys;
    protected:
        void setViewMatrix(glm::mat4 & view);
        void setProjMatrix(glm::mat4 & proj);
        std::vector<ib::Key>& getKeys();
    public:
        Camera() = default;
        Camera(glm::mat4 projection, glm::mat4 view);
        virtual ~Camera() = default;

        virtual void onEvent(es::Event& e) = 0;
        virtual void update(float deltaTime) = 0;
        void setKeys(std::vector<ib::Key> keys);

        glm::mat4& getViewMatrix();
        glm::mat4& getProjMatrix();
    };
}