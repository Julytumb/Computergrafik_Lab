cmake_minimum_required(VERSION 3.9)
project(asteroids)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-g -Wall -Wextra -Wpedantic -Wl,--stack,16777216)

# --- SDL2 und SDL2_Mixer finden ---
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

# --- Dein Hauptprogramm ---
add_executable(main_game game.cc math.cc geometry.cc sdl2_renderer.cc sound.cc main_game.cc physics.cc sdl2_game_controller.cc timer.cc)

# Explizit die Include-Pfade für SDL2_mixer hinzufügen
# (SDL2::SDL2 wird seine eigenen Includes automatisch hinzufügen)
target_include_directories(main_game PRIVATE
    ${SDL2_MIXER_INCLUDE_DIRS} # Plural ist korrekt für Includes
)

# Explizit die Bibliotheken verlinken
target_link_libraries(main_game PRIVATE
    SDL2::SDL2    # Das moderne Target für SDL2 (funktioniert)
    SDL2_mixer    # Der "alte" Weg: sag dem Linker, er soll nach -lSDL2_mixer suchen
)

# --- Deine Tests ---
enable_testing()
add_executable(math_test math_test.cc math.cc)
target_link_libraries(math_test PRIVATE gtest gtest_main)

add_executable(geometry_test geometry_test.cc geometry.cc math.cc)
target_link_libraries(geometry_test PRIVATE gtest gtest_main)

# Tests, die SDL brauchen, verlinken wir mit dem modernen SDL2-Target
add_executable(physics_test physics_test.cc physics.cc geometry.cc math.cc timer.cc)
target_link_libraries(physics_test PRIVATE gtest gtest_main SDL2::SDL2)

add_executable(game_test game_test.cc game.cc physics.cc geometry.cc math.cc timer.cc)
target_link_libraries(game_test PRIVATE gtest gtest_main SDL2::SDL2)