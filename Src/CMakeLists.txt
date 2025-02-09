#[[

                       ______                     ___  __ __
                      / ____/___  ________       |__ \/ // /
                     / /   / __ \/ ___/ _ \________/ / // /_
                    / /___/ /_/ / /  /  __/_____/ __/__  __/
                    \____/\____/_/   \___/     /____/ /_/   
                                                            
]]

INCLUDE("${PROJECT_SOURCE_DIR}/Config.cmake")

CREATE_EXECUTABLE(Sample ${LIBRARY_NAME}::core ${PROJECT_SOURCE_DIR}/Src/Main.cpp)