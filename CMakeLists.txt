cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(snake)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "vitaSnake")
set(VITA_TITLEID  "GRZB00002")
set(VITA_VERSION  "00.50")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

include_directories(
	../common
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(vitaSnake
  src/main.cpp
  src/game.hpp
  src/game.cpp
  src/menu.hpp
  src/menu.cpp
  src/player.hpp
  src/player.cpp
  src/collectable.hpp
  src/collectable.cpp
  src/global.hpp
)

target_link_libraries(vitaSnake
  SceLibKernel_stub
  SceDisplay_stub
  SceTouch_stub
  -lvita2d 
  -lSceDisplay_stub 
  -lSceGxm_stub
  -lSceSysmodule_stub
  -lSceCtrl_stub
  -lScePgf_stub
  -lScePvf_stub
  -lSceCommonDialog_stub
  -lfreetype
  -lpng
  -ljpeg
  -lz
  -lm
  -lc
)

vita_create_self(vitaSnake.self vitaSnake)

vita_create_vpk(vitaSnake.vpk ${VITA_TITLEID} vitaSnake.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  FILE img/head.png img/head.png
  FILE img/body.png img/body.png
  FILE img/tail.png img/tail.png
  FILE img/apple.png img/apple.png
  FILE img/menuScreen.png img/menuScreen.png
  FILE img/cursor.png img/cursor.png
  FILE img/infoScreen.png img/infoScreen.png
  FILE img/pauseScreen.png img/pauseScreen.png
  FILE img/overScreen.png img/overScreen.png
)

