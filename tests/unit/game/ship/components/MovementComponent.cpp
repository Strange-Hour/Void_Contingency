TEST(MovementComponentTest, BasicMovement) {
    Ship ship("Test Ship");
    auto movement = std::make_unique<MovementComponent>();

    // Test thruster mode
    movement->setMovementMode(MovementMode::Thruster);
    movement->setThrust(Vector2f(1.0f, 0.0f));
    movement->update(1.0f);

    EXPECT_GT(ship.getVelocity().x, 0.0f);
    EXPECT_NEAR(ship.getVelocity().y, 0.0f, 0.001f);
}

TEST(MovementComponentTest, SpeedLimit) {
    Ship ship("Test Ship");
    auto movement = std::make_unique<MovementComponent>();

    movement->setMaxSpeed(100.0f);
    movement->setThrust(Vector2f(1000.0f, 0.0f));
    movement->update(5.0f);

    EXPECT_LE(ship.getVelocity().length(), 100.0f);
}