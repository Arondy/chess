find_package(Catch2 2 REQUIRED)

add_executable(tests_game_logic tests_game_logic.cpp)

include_directories(../game_state ../figures ../game_components)

target_link_libraries(tests_game_logic game_state figures game_components Catch2::Catch2)