include(ExternalProject)

list(INSERT CMAKE_PREFIX_PATH 0 ${CMAKE_INSTALL_PREFIX})

ExternalProject_Add(
	coin
	DEPENDS boost-header-libs-full cpack.d simage
	#HG_REPOSITORY https://bitbucket.org/Coin3D/coin
	#HG_TAG default
	URL https://bitbucket.org/Coin3D/coin/get/default.tar.bz2
	#URL ${CMAKE_CURRENT_SOURCE_DIR}/default.tar.bz2
	PATCH_COMMAND
	${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/../cpack.d/cpack.d-prefix/src/cpack.d <SOURCE_DIR>/cpack.d &&
	${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/../boost-header-libs-full/boost-header-libs-full-prefix/src/boost-header-libs-full <SOURCE_DIR>/include/boost
	CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_CFG_INTDIR=${CMAKE_CFG_INTDIR} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH} -DCMAKE_TRY_COMPILE_CONFIGURATION=${CMAKE_TRY_COMPILE_CONFIGURATION} -DCOIN_BUILD_DOCUMENTATION=OFF
	INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
)
