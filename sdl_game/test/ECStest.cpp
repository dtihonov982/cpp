#include "ECS.h"
#include <gtest/gtest.h>

TEST(ECS, groups) {
    Manager manager;
    Entity& e = manager.addEntity(EntityGroup::players);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
