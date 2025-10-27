cmake_minimum_required (VERSION 3.9)
project (asteroids)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-g -Wall -Wextra -Wpedantic -Wl,--stack,16777216)

# --- NEU: SDL2 und SDL2_Mixer finden ---
# Sucht nach den SDL2-Bibliotheken (Header und Libs)
find_package(SDL2 REQUIRED)
# Sucht nach den SDL2_mixer-Bibliotheken
find_package(SDL2_mixer REQUIRED)

# --- Dein Hauptprogramm ---
add_executable(main_game game.cc math.cc geometry.cc sdl2_renderer.cc sound.cc main_game.cc physics.cc sdl2_game_controller.cc timer.cc)

# NEU: Die "alten" Include-Pfade für SDL2_mixer hinzufügen
target_include_directories(main_game PRIVATE ${SDL2_MIXER_INCLUDE_DIRS})

# NEU: Die Bibliotheken verlinken (SDL2 modern, SDL2_mixer alt)
target_link_libraries(main_game PRIVATE
    SDL2::SDL2
    ${SDL2_MIXER_LIBRARIES}
)


# Tests ---
enable_testing()
add_executable(math_test math_test.cc math.cc)
target_link_libraries(math_test PRIVATE gtest gtest_main)

add_executable(geometry_test geometry_test.cc geometry.cc math.cc)
target_link_libraries(geometry_test PRIVATE gtest gtest_main)

add_executable(physics_test physics_test.cc physics.cc geometry.cc math.cc timer.cc)
# physics_test benötigt jetzt auch das moderne SDL2-Ziel
target_link_libraries(physics_test PRIVATE gtest gtest_main SDL2::SDL2)

add_executable(game_test game_test.cc game.cc physics.cc geometry.cc math.cc timer.cc)
# game_test benötigt jetzt auch das moderne SDL2-Ziel
target_link_libraries(game_test PRIVATE gtest gtest_main SDL2::SDL2)

