#pragma once

#include <ore/core/GameState.h>

namespace openrr {
    class OpenRRGame : public ore::GameState {
    public:
        void update() override;

        void set(ore::GameWorld* world) override;

        void unset() override;
    };
}
