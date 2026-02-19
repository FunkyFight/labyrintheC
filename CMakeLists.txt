cmake_minimum_required(VERSION 3.29)
project(labyrinthe C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GOBJECT REQUIRED gobject-2.0)
pkg_check_modules(SDL3 REQUIRED sdl3)

add_executable(labyrinthe main.c
        types.h
        events/quit_event.c
        events/quit_event.h
        events/event_manager.c
        events/event_manager.h
        draw/draw_game.c
        draw/draw_game.h
        gameobjects/LabyrinthCell/gameobject_labyrinthe_cell.c
        gameobjects/LabyrinthCell/gameobject_labyrinthe_cell.h
        gameobjects/gameobject_manager.c
        gameobjects/gameobject_manager.h
        business/nodes/labyrinthe_node.c
        business/nodes/labyrinthe_node.h
        facade/GameFacade.c
        facade/GameFacade.h
        main.h
        generation/nodeListTool.c
        generation/nodeListTool.h
        generation/RDFS/rdfsGeneration.c
        generation/RDFS/rdfsGeneration.h
        utils/node_sort.c
        utils/node_sort.h
)

target_include_directories(labyrinthe PRIVATE
        ${GOBJECT_INCLUDE_DIRS}
        ${SDL3_INCLUDE_DIRS}
)
target_compile_options(labyrinthe PRIVATE
        ${GOBJECT_CFLAGS_OTHER}
        ${SDL3_CFLAGS_OTHER}
)

target_link_directories(labyrinthe PRIVATE
        ${GOBJECT_LIBRARY_DIRS}
        ${SDL3_LIBRARY_DIRS}
)
target_link_libraries(labyrinthe PRIVATE
        ${GOBJECT_LIBRARIES}
        ${SDL3_LIBRARIES}
        rpcrt4
)
