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
set(SHORT_NAME vitaSnake)
set(VITA_TITLEID  "GRZB00002")
set(VITA_VERSION  "00.99")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

include_directories(
	../common
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(${VITA_APP_NAME}
  src/main.cpp
  src/game.hpp
  src/game.cpp
  src/menu.hpp
  src/menu.cpp
  src/player.hpp
  src/player.cpp
  src/collectable.hpp
  src/collectable.cpp
  src/buttons.hpp
  src/buttons.cpp
  src/texture.hpp
  src/texture.cpp
  src/particle.hpp
  src/particle.cpp
  src/options.hpp
  src/options.cpp
  src/global.hpp
  src/global.cpp
)

target_link_libraries(${VITA_APP_NAME}
  SceLibKernel_stub
  SceDisplay_stub
  SceTouch_stub
  SceAudio_stub
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
  soloud
  pthread
  m
)

vita_create_self(${SHORT_NAME}.self ${SHORT_NAME})
vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} ${SHORT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png 									sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png 				sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png 		sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml 	sce_sys/livearea/contents/template.xml
  
  FILE img/playerDefault.png		img/playerDefault.png
  FILE img/playerClassic.png		img/playerClassic.png
  
  FILE img/apple.png 					img/apple.png
  FILE img/sparkle.png 				img/sparkle.png
  
  FILE img/menuButton.png		img/menuButton.png
  FILE img/cursor.png 				img/cursor.png
  FILE img/button_cross.png		img/button_cross.png
  FILE img/button_circle.png		img/button_circle.png
  
  FILE img/bg.png 						img/bg.png
  
  FILE sound/menu_move.wav 	sound/menu_move.wav
  FILE sound/menu_select.wav 	sound/menu_select.wav
  FILE sound/bite.wav 				sound/bite.wav
  FILE sound/snake_death.ogg 	sound/snake_death.ogg
  
  FILE font/font_main.ttf				font/font_main.ttf
)

