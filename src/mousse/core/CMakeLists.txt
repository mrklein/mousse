# mousse: CFD toolbox
# Copyright (C) 2016 mousse project

set(CORE_SOURCES)

add_subdirectory(algorithms)

add_library(libmousse_core SHARED ${CORE_SOURCES})
