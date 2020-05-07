#pragma once

#include <ore/core/GameState.h>

namespace openrr {
    class OpenRRGame : public ore::GameState {
    public:
        void update() override;

        void set(ore::resources::ResourceCache *cache) override;

        void unset() override;
    };
}
