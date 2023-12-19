cmake_minimum_required(VERSION 3.0)
project(bgdi)

# Archivos fuente
set(SOURCE_FILES main.c)

# Headers
set(HEADERS
    ../include/bgdi.h
    ../../bgdrtm/include/bgdrtm.h
    ../../include/files.h
    ../../include/xctype.h
    ../../include/offsets.h
    ../../include/pslang.h
)

# Si se usa windres (recurso de Windows)
if (HAVE_WINDRES)
    set(SOURCES ${SOURCE_FILES} bgdi.rc)
else()
    set(SOURCES ${SOURCE_FILES})
endif()

# Incluyendo directorios
include_directories(
    ../../include
    ../include
    ../../bgdrtm/include
)

# Definiciones
add_definitions(-DDEBUG -D__BGDI__ ${COMMON_CFLAGS} ${EXTRA_STATIC_CFLAGS})

# Creando el ejecutable
add_executable(bgdi ${SOURCES} ${HEADERS})

# Especificando las banderas de enlace y las bibliotecas
target_link_libraries(bgdi ${COMMON_LDFLAGS} -L../../bgdrtm/src/.libs -lbgdrtm ${CRYPT_LDFLAGS} ${EXTRA_STATIC_LDFLAGS})
set_target_properties(bgdi PROPERTIES
    OUTPUT_NAME "bgdi"
)

