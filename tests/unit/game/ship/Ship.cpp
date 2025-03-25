#include <gtest/gtest.h>
#include "game/ship/Ship.hpp"

using namespace void_contingency::game;

TEST(ShipTest, BasicProperties) {
    Ship ship("Test Ship");

    EXPECT_EQ(ship.getName(), "Test Ship");
    EXPECT_EQ(ship.getHealth(), 100.0f);
    EXPECT_EQ(ship.getMaxHealth(), 100.0f);
}

TEST(ShipTest, Movement) {
    Ship ship("Test Ship");
    Vector2f velocity(1.0f, 2.0f);

    ship.setVelocity(velocity);
    EXPECT_EQ(ship.getVelocity(), velocity);
}

TEST(ShipTest, DamageAndHeal) {
    Ship ship("Test Ship");

    ship.damage(30.0f);
    EXPECT_EQ(ship.getHealth(), 70.0f);

    ship.heal(20.0f);
    EXPECT_EQ(ship.getHealth(), 90.0f);

    ship.damage(100.0f);
    EXPECT_EQ(ship.getHealth(), 0.0f);

    ship.heal(100.0f);
    EXPECT_EQ(ship.getHealth(), 100.0f);
}