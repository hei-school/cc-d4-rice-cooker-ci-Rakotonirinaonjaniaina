TEST_CASE("Rice Cooker Functionalities") {
    RiceCooker riceCooker;

    SECTION("Display Functionalities") {
        std::ostringstream output;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
        riceCooker.showFunctionalities();
        std::cout.rdbuf(oldCoutBuffer);

        std::string expectedOutput = "Rice Cooker Functionalities:\n1. Cooking rice\n2. Keep warm\n3. Cooking schedule\n4. Quantity guide\n";
        REQUIRE(output.str() == expectedOutput);
    }

    SECTION("Choose Option") {
        // REQUIRE(riceCooker.chooseOption() == 1);
    }

    SECTION("Stop Maintain Warm") {
        riceCooker.stopMaintainWarm();
        REQUIRE(riceCooker.isMaintainStopped());
    }

    SECTION("Guide Quantities") {
        // std::ostringstream output;
        // std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
        // riceCooker.guideQuantities();
        // std::cout.rdbuf(oldCoutBuffer);
        // std::string expectedOutput = "For 2 person(s), you will need:\n- Quantity of rice: 200.00 g\n- Quantity of water: 400.00 ml\n";
        // REQUIRE(output.str() == expectedOutput);
    }

    SECTION("Keep Warm") {
        // std::thread keepWarmThread(&RiceCooker::keepWarm, &riceCooker);
        // std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait for a few seconds
        // riceCooker.stopMaintainWarm();
        // keepWarmThread.join();
        // REQUIRE(/* Add a condition here to check if rice was kept warm */);
    }

    SECTION("Keep Rice Warm") {
        // std::thread keepRiceWarmThread(&RiceCooker::keepRiceWarm, &riceCooker);
        // std::this_thread::sleep_for(std::chrono::seconds(5)); // Wait for a few seconds
        // riceCooker.stopMaintainWarm();
        // keepRiceWarmThread.join();
        // REQUIRE(/* Add a condition here to check if rice was kept warm for the specified duration */);
    }

    SECTION("Cook Rice") {
        // std::thread cookRiceThread(&RiceCooker::cookRice, &riceCooker);
        // std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait for a few seconds
        // riceCooker.stopMaintainWarm();
        // cookRiceThread.join();
        // REQUIRE(/* Add a condition here to check the successful rice cooking */);
    }

    SECTION("Schedule Cooking") {
        // std::ostringstream output;
        // std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
        // riceCooker.scheduleCooking();
        // std::cout.rdbuf(oldCoutBuffer);
        // REQUIRE(/* Add a condition here to verify if cooking was scheduled correctly */);
    }

}
