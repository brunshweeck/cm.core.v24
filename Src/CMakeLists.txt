#[[

                       ______                     ___  __ __
                      / ____/___  ________       |__ \/ // /
                     / /   / __ \/ ___/ _ \________/ / // /_
                    / /___/ /_/ / /  /  __/_____/ __/__  __/
                    \____/\____/_/   \___/     /____/ /_/   
                                                            
]]

INCLUDE("${PROJECT_SOURCE_DIR}/Config.cmake")

ADD_EXECUTABLE(Main "${PROJECT_SOURCE_DIR}/Src/Main.cpp")

TARGET_LINK_LIBRARIES(Main Core-24)