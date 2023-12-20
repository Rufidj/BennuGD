cmake_minimum_required(VERSION 3.0)
project(key)

# Archivos fuente
set(SOURCES
    libkey.c
)

# Incluyendo directorios
include_directories(
    ${BGD_PATH}/include
)

# Definiciones
add_definitions(${COMMON_CFLAGS} ${SDL_CFLAGS})

# Creando la biblioteca
add_library(key SHARED ${SOURCES})

# Especificando las banderas de enlace y las bibliotecas
target_link_libraries(key ${COMMON_LDFLAGS} ${SDL_LIBS} -L${BGD_PATH}/bgdrtm/src/.libs -lbgdrtm)
set_target_properties(key PROPERTIES
    OUTPUT_NAME "key"
)

