#include <cstdio>

#include <spdlog/spdlog.h>

#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char **argv) {
    SPDLOG_INFO("Some trace message with param {}", 42);
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

