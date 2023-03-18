﻿#pragma once
#include "Game/BlockBase.h"

namespace Game::Blocks {
    using WallBlock = Block<WALL>;
    
    template <> class Block<WALL> : public BlockBase {
        friend BlockBase;

        protected:
            Block(const Maths::Vec3Int& position) : BlockBase(position) {}
        public:
            Block() : BlockBase() {}
            Block(const BlockBase& copy) : BlockBase(copy) {}
            Block(BlockBase&& copy) : BlockBase(copy) {}

            ~Block() override = default;

            inline static constexpr BlockType TypeID = WALL;
    };
}
