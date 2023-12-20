cmake_minimum_required(VERSION 3.0)
project(blit)

# Archivos fuente
set(SOURCES
    libblit.c
    g_blit.c
    g_pixel.c
)

# Incluyendo directorios
include_directories(
    ${BGD_PATH}/include
    ${BGD_PATH}/core/include
    ${BGD_PATH}/bgdrtm/include
    .
    ../libgrbase/
)

# Definiciones
add_definitions(${COMMON_CFLAGS} ${SDL_CFLAGS})

# Creando la biblioteca
add_library(blit SHARED ${SOURCES})

# Especificando las banderas de enlace y las bibliotecas
target_link_libraries(blit ${COMMON_LDFLAGS} ${SDL_LIBS} -L${BGD_PATH}/bgdrtm/src/.libs -lbgdrtm -L../libgrbase/.libs -lgrbase)
set_target_properties(blit PROPERTIES
    OUTPUT_NAME "blit"
)

