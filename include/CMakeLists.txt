add_library(workflow_tester INTERFACE)
add_library(workflow_tester::workflow_tester ALIAS workflow_tester)

target_sources(workflow_tester
    INTERFACE
        FILE_SET HEADERS
        FILES
            math/math.h
)

target_compile_features(workflow_tester INTERFACE cxx_std_23)
