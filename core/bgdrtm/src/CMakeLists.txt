cmake_minimum_required(VERSION 3.0)
project(bgdrtm)

# Archivos fuente
set(SOURCES
    fmath.c
    dcbr.c
    sysprocs.c
    strings.c
    dirs.c
    copy.c
    misc.c
    instance.c
    interpreter.c
    varspace_file.c
    ../../common/debug.c
    ../../common/b_crypt.c
    ../../common/files.c
    ../../common/xctype.c
    
)

# Opciones de compilación
set(COMMON_CFLAGS "${COMMON_CFLAGS}")  # Define tus FLAGS comunes aquí
set(ZLIB_CFLAGS "${ZLIB_CFLAGS}")  # Define tus FLAGS de ZLIB aquí
set(CRYPT_LDFLAGS "${CRYPT_LDFLAGS}")  # Define tus FLAGS de crypt aquí

# Incluyendo directorios
include_directories(
    ../../include
    ../include
)

# Agregando definiciones
add_definitions(-D__BGDRTM__ -DDEBUG ${COMMON_CFLAGS} ${ZLIB_CFLAGS})

# Creando la biblioteca
add_library(bgdrtm SHARED ${SOURCES})

# Especificando las banderas de enlace y las bibliotecas
target_link_libraries(bgdrtm ${COMMON_LDFLAGS} ${ZLIB_LIBS} ${CRYPT_LDFLAGS} -lm)
set_target_properties(bgdrtm PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

