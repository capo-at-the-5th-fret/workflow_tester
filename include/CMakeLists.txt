add_library(workflow_tester INTERFACE)
add_library(workflow_tester::workflow_tester ALIAS workflow_tester)

target_sources(workflow_tester
    INTERFACE
        FILE_SET HEADERS
        FILES
            mylib/test/dummy.h
            mylib/test/type_list.h

            mylib/concepts.h
            mylib/math.h
            mylib/tuple.h
            mylib/type_list.h
            mylib/type_traits.h
)

target_compile_features(workflow_tester INTERFACE cxx_std_23)
