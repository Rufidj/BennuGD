cmake_minimum_required(VERSION 3.0)
project(grbase)

# Archivos fuente
set(SOURCES
    g_bitmap.c
    g_clear.c
    g_grlib.c
    g_pal.c
    g_blendop.c
    g_conversion.c
    libgrbase.c
)

# Incluyendo directorios
include_directories(
    ${BGD_PATH}/include
    ${BGD_PATH}/core/include
    /home/rufidj/BennuGD/core/include
    .
)

# Definiciones
add_definitions(${COMMON_CFLAGS})

# Creando la biblioteca
add_library(grbase SHARED ${SOURCES})

# Especificando las banderas de enlace y las bibliotecas
target_link_libraries(grbase ${COMMON_LDFLAGS})
set_target_properties(grbase PROPERTIES
    OUTPUT_NAME "grbase"
)

