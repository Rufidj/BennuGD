cmake_minimum_required(VERSION 3.0)
project(draw)

# Archivos fuente
set(SOURCES
    libdraw.c
)

# Incluyendo directorios
include_directories(
    ${BGD_PATH}/include
    ${BGD_PATH}/core/bgdrtm/include
    /home/rufidj/BennuGD/core/bgdrtm/include
    /home/rufidj/BennuGD/core/include
    .
    ../libgrbase/
    ../libblit/
)

# Definiciones
add_definitions(${COMMON_CFLAGS})

# Creando la biblioteca
add_library(draw SHARED ${SOURCES})

# Especificando las banderas de enlace y las bibliotecas
target_link_libraries(draw ${COMMON_LDFLAGS} -L../libblit/.libs -lblit -L../libgrbase/.libs -lgrbase)
set_target_properties(draw PROPERTIES
    OUTPUT_NAME "draw"
)

