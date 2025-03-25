TEST(EngineComponentTest, BasicFunctionality) {
    Ship ship("Test Ship");
    auto engine = std::make_unique<EngineComponent>();

    engine->setThrust(50.0f);
    EXPECT_EQ(engine->getThrust(), 50.0f);

    engine->update(1.0f);
    EXPECT_GT(ship.getVelocity().length(), 0.0f);
}