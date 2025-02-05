MESSAGE(NOTICE "
                        ____                 ____  _  _
                       / ___|___  _ __ ___  |___ \| || |
                      | |   / _ \| '__/ _ \   __) | || |_
                      | |__| (_) | | |  __/  / __/|__   _|
                       \____\___/|_|  \___| |_____|  |_|
                        The new vision of C++ (since 2024)
")


include(${PROJECT_SOURCE_DIR}/Config.cmake)

ADD_LIBRARY(Core-24-v1 SHARED ${MetaSources})
ADD_LIBRARY(Core-24-v2 SHARED ${NativeSources})

ADD_LIBRARY(Core-24::meta ALIAS Core-24-v1)
ADD_LIBRARY(Core-24::native ALIAS Core-24-v2)

ADD_LIBRARY(Core-24 SHARED ${LibHeaders} ${LibSources})

TARGET_LINK_LIBRARIES(Core-24 Core-24::meta Core-24::native)
