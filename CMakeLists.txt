cmake_minimum_required(VERSION 3.14)
project(Proekt)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# --- 1. Настройка зависимостей ---
include(FetchContent)

# Скачиваем и собираем SFML
FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1
)
FetchContent_MakeAvailable(sfml)

# --- 2. Сборка вашего проекта ---
add_executable(Proekt
        main.cpp
        game.cpp
        game.h
)

# --- 3. Подключение библиотек ---
target_link_libraries(Proekt PRIVATE sfml-graphics sfml-window sfml-system)

# --- 4. Автоматическое копирование DLL в папку с exe (только Windows) ---
if(WIN32)
    add_custom_command(TARGET Proekt POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_RUNTIME_DLLS:Proekt>"
            $<TARGET_FILE_DIR:Proekt>
            COMMAND_EXPAND_LISTS
    )
endif()

# --- 5. Копирование ресурсов ---
add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
)

add_dependencies(Proekt copy_assets)