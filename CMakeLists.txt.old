cmake_minimum_required(VERSION 3.26.0)

project(workflow_tester
    VERSION 0.1.0
    LANGUAGES CXX
)

add_subdirectory(include)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)

    option(WORKFLOW_TESTER_BUILD_TESTS "Enable or disable the building of tests" ON)
    option(WORKFLOW_TESTER_ENABLE_INSTALL "Enable or disable the install rule" ON)

    if (WORKFLOW_TESTER_BUILD_TESTS)
        enable_testing()
        add_subdirectory(tests)
    endif()

    if (WORKFLOW_TESTER_ENABLE_INSTALL)

        include(GNUInstallDirs)
        include(CMakePackageConfigHelpers)

        # NOTE 1: The XXX_PACKAGE_TARGETS variable is referenced inside of the
        # xxxConfig.cmake.in file, so if the name of this variable changes, it
        # must be updated inside of the xxxConfig.cmake.in file.
        #
        # NOTE 2: If value of this variable changes, the xxxConfig.cmake.in file
        # name must be renamed to match the new value.
        set(WORKFLOW_TESTER_PACKAGE_NAME ${PROJECT_NAME})
        set(WORKFLOW_TESTER_PACKAGE_TARGETS ${WORKFLOW_TESTER_PACKAGE_NAME}Targets)

        set(WORKFLOW_TESTER_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR})
        set(WORKFLOW_TESTER_INSTALL_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${WORKFLOW_TESTER_PACKAGE_NAME})

        set(WORKFLOW_TESTER_CONFIG_DIR ${PROJECT_BINARY_DIR}/cmake)
        set(WORKFLOW_TESTER_CONFIG_IN cmake/${WORKFLOW_TESTER_PACKAGE_NAME}Config.cmake.in)
        set(WORKFLOW_TESTER_CONFIG ${WORKFLOW_TESTER_CONFIG_DIR}/${WORKFLOW_TESTER_PACKAGE_NAME}Config.cmake)
        set(WORKFLOW_TESTER_CONFIG_VERSION ${WORKFLOW_TESTER_CONFIG_DIR}/${WORKFLOW_TESTER_PACKAGE_NAME}ConfigVersion.cmake)

        configure_package_config_file(
            ${WORKFLOW_TESTER_CONFIG_IN}
            ${WORKFLOW_TESTER_CONFIG}
            INSTALL_DESTINATION ${WORKFLOW_TESTER_INSTALL_CMAKE_DIR}
        )

        # major.minor.patch1 is compatible with major.minor.patch2..n
        # if no VERSION is specified, the ${PROJECT_VERSION} variable will be
        # used. if the ${PROJECT_VERSION} is not set, CMake will output an
        # error
        write_basic_package_version_file(
            ${WORKFLOW_TESTER_CONFIG_VERSION}
            COMPATIBILITY SameMinorVersion
        )

        install(
            FILES
                ${WORKFLOW_TESTER_CONFIG}
                ${WORKFLOW_TESTER_CONFIG_VERSION}
            DESTINATION ${WORKFLOW_TESTER_INSTALL_CMAKE_DIR}
        )

        install(TARGETS ${WORKFLOW_TESTER_PACKAGE_NAME}
            EXPORT ${WORKFLOW_TESTER_PACKAGE_TARGETS}
            FILE_SET HEADERS
        )

        install(EXPORT ${WORKFLOW_TESTER_PACKAGE_TARGETS}
            NAMESPACE ${WORKFLOW_TESTER_PACKAGE_NAME}::
            DESTINATION ${WORKFLOW_TESTER_INSTALL_CMAKE_DIR}
        )

    endif()

endif()
