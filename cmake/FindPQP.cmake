include(FindPackageHandleStandardArgs)
include(GNUInstallDirs)
include(SelectLibraryConfigurations)

foreach(PATH ${CMAKE_PREFIX_PATH})
	file(
		GLOB
		HINTS
		${PATH}/${CMAKE_INSTALL_INCLUDEDIR}
		${PATH}/PQP*/${CMAKE_INSTALL_INCLUDEDIR}
	)
	list(APPEND PQP_INCLUDE_HINTS ${HINTS})
endforeach()

list(
	APPEND
	PQP_INCLUDE_HINTS
	$ENV{PQP_DIR}/${CMAKE_INSTALL_INCLUDEDIR}
)

foreach(PATH $ENV{CMAKE_PREFIX_PATH})
	file(
		GLOB
		HINTS
		${PATH}/${CMAKE_INSTALL_INCLUDEDIR}
		${PATH}/PQP*/${CMAKE_INSTALL_INCLUDEDIR}
	)
	list(APPEND PQP_INCLUDE_HINTS ${HINTS})
endforeach()

foreach(PATH $ENV{PATH})
	file(
		GLOB
		HINTS
		${PATH}/../${CMAKE_INSTALL_INCLUDEDIR}
	)
	list(APPEND PQP_INCLUDE_HINTS ${HINTS})
endforeach()

file(
	GLOB
	PQP_INCLUDE_PATHS
	$ENV{HOME}/include
	/usr/local/include
	/opt/local/include/PQP
	/opt/local/include
	/usr/include
)

find_path(
	PQP_INCLUDE_DIRS
	NAMES
	PQP.h
	HINTS
	${PQP_INCLUDE_HINTS}
	PATHS
	${PQP_INCLUDE_PATHS}
)

mark_as_advanced(PQP_INCLUDE_DIRS)

foreach(PATH ${CMAKE_PREFIX_PATH})
	file(
		GLOB
		HINTS
		${PATH}/${CMAKE_INSTALL_LIBDIR}
		${PATH}/PQP*/${CMAKE_INSTALL_LIBDIR}
	)
	list(APPEND PQP_LIBRARY_HINTS ${HINTS})
endforeach()

list(
	APPEND
	PQP_LIBRARY_HINTS
	$ENV{PQP_DIR}/${CMAKE_INSTALL_LIBDIR}
)

foreach(PATH $ENV{CMAKE_PREFIX_PATH})
	file(
		GLOB
		HINTS
		${PATH}/${CMAKE_INSTALL_LIBDIR}
		${PATH}/PQP*/${CMAKE_INSTALL_LIBDIR}
	)
	list(APPEND PQP_LIBRARY_HINTS ${HINTS})
endforeach()

foreach(PATH $ENV{PATH})
	file(
		GLOB
		HINTS
		${PATH}/../${CMAKE_INSTALL_LIBDIR}
	)
	list(APPEND PQP_LIBRARY_HINTS ${HINTS})
endforeach()

file(
	GLOB
	PQP_LIBRARY_PATHS
	$ENV{HOME}/lib
	/usr/local/lib
	/opt/local/lib
	/usr/lib
)

find_library(
	PQP_LIBRARY_DEBUG
	NAMES
	PQPd
	HINTS
	${PQP_LIBRARY_HINTS}
	PATHS
	${PQP_LIBRARY_PATHS}
)

find_library(
	PQP_LIBRARY_RELEASE
	NAMES
	PQP
	HINTS
	${PQP_LIBRARY_HINTS}
	PATHS
	${PQP_LIBRARY_PATHS}
)

select_library_configurations(PQP)

find_package_handle_standard_args(
	PQP
	FOUND_VAR PQP_FOUND
	REQUIRED_VARS PQP_INCLUDE_DIRS PQP_LIBRARIES
)

if(PQP_FOUND AND NOT TARGET PQP::PQP)
	add_library(PQP::PQP UNKNOWN IMPORTED)
	
	if(PQP_LIBRARY_RELEASE)
		set_property(TARGET PQP::PQP APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
		set_target_properties(PQP::PQP PROPERTIES IMPORTED_LOCATION_RELEASE "${PQP_LIBRARY_RELEASE}")
	endif()
	
	if(PQP_LIBRARY_DEBUG)
		set_property(TARGET PQP::PQP APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
		set_target_properties(PQP::PQP PROPERTIES IMPORTED_LOCATION_DEBUG "${PQP_LIBRARY_DEBUG}")
	endif()
	
	set_target_properties(
		PQP::PQP PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${PQP_INCLUDE_DIRS}"
	)
endif()
