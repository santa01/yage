# Checks for C++11 features
#  CXX11_FEATURE_LIST - a list containing all supported features
#  CXX11_FLAGS - flag that activates C++11 mode of compiler
#  HAS_CXX11_AUTO               - auto keyword
#  HAS_CXX11_NULLPTR            - nullptr
#  HAS_CXX11_LAMBDA             - lambdas
#  HAS_CXX11_STATIC_ASSERT      - static_assert()
#  HAS_CXX11_RVALUE_REFERENCES  - rvalue references
#  HAS_CXX11_DECLTYPE           - decltype keyword
#  HAS_CXX11_CSTDINT_H          - cstdint header
#  HAS_CXX11_LONG_LONG          - long long signed & unsigned types
#  HAS_CXX11_VARIADIC_TEMPLATES - variadic templates
#  HAS_CXX11_CONSTEXPR          - constexpr keyword
#  HAS_CXX11_SIZEOF_MEMBER      - sizeof() non-static members
#  HAS_CXX11_FUNC               - __func__ preprocessor constant
#  HAS_CXX11_DELEGATING_CTORS   - support for delegating constructors
#
# Original script by Rolf Eike Beer
# Modifications by Andreas Weis, Kirill Bogdanenko
#
cmake_minimum_required(VERSION 2.8)

set(CHECK_CXX11_OLD_CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(CXX11_FLAGS "")
if(CMAKE_COMPILER_IS_GNUCXX)
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                    OUTPUT_VARIABLE GCC_VERSION)
    if(GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
        set(CMAKE_CXX_FLAGS "-std=c++11")
        set(CXX11_FLAGS "-std=c++11")
    elseif(GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
        set(CMAKE_CXX_FLAGS "-std=c++0x")
        set(CXX11_FLAGS "-std=c++0x")
    else()
        message(WARNING "Minimum required GCC version is 4.3")
    endif()
endif()

macro(CXX11_CHECK_FEATURE FEATURE_NAME FEATURE_NUMBER RESULT_VAR)
    if(NOT DEFINED ${RESULT_VAR})
        set(_bindir "${CMAKE_CURRENT_BINARY_DIR}/cxx11/cxx11_${FEATURE_NAME}")

        if(${FEATURE_NUMBER})
            set(_SRCFILE_BASE ${CMAKE_CURRENT_LIST_DIR}/c++11-test-${FEATURE_NAME}-N${FEATURE_NUMBER})
            set(_LOG_NAME "\"${FEATURE_NAME}\" (N${FEATURE_NUMBER})")
        else(${FEATURE_NUMBER})
            set(_SRCFILE_BASE ${CMAKE_CURRENT_LIST_DIR}/c++11-test-${FEATURE_NAME})
            set(_LOG_NAME "\"${FEATURE_NAME}\"")
        endif(${FEATURE_NUMBER})
        message(STATUS "Checking C++11 support for ${_LOG_NAME}")

        set(_SRCFILE "${_SRCFILE_BASE}.cpp")
        set(_SRCFILE_FAIL "${_SRCFILE_BASE}_fail.cpp")
        set(_SRCFILE_FAIL_COMPILE "${_SRCFILE_BASE}_fail_compile.cpp")

        if(CROSS_COMPILING)
            try_compile(${RESULT_VAR} "${_bindir}" "${_SRCFILE}")
            if(${RESULT_VAR} AND EXISTS ${_SRCFILE_FAIL})
                try_compile(${RESULT_VAR} "${_bindir}_fail" "${_SRCFILE_FAIL}")
            endif (${RESULT_VAR} AND EXISTS ${_SRCFILE_FAIL})
        else(CROSS_COMPILING)
            try_run(_RUN_RESULT_VAR _COMPILE_RESULT_VAR
                        "${_bindir}" "${_SRCFILE}")
            if(_COMPILE_RESULT_VAR AND NOT _RUN_RESULT_VAR)
                set(${RESULT_VAR} TRUE)
            else(_COMPILE_RESULT_VAR AND NOT _RUN_RESULT_VAR)
                set(${RESULT_VAR} FALSE)
            endif(_COMPILE_RESULT_VAR AND NOT _RUN_RESULT_VAR)

            if(${RESULT_VAR} AND EXISTS ${_SRCFILE_FAIL})
                try_run(_RUN_RESULT_VAR _COMPILE_RESULT_VAR
                            "${_bindir}_fail" "${_SRCFILE_FAIL}")
                if(_COMPILE_RESULT_VAR AND _RUN_RESULT_VAR)
                    set(${RESULT_VAR} TRUE)
                else(_COMPILE_RESULT_VAR AND _RUN_RESULT_VAR)
                    set(${RESULT_VAR} FALSE)
                endif(_COMPILE_RESULT_VAR AND _RUN_RESULT_VAR)
            endif(${RESULT_VAR} AND EXISTS ${_SRCFILE_FAIL})
        endif(CROSS_COMPILING)

        if(${RESULT_VAR} AND EXISTS ${_SRCFILE_FAIL_COMPILE})
            try_compile(_TMP_RESULT "${_bindir}_fail_compile" "${_SRCFILE_FAIL_COMPILE}")
            if(_TMP_RESULT)
                set(${RESULT_VAR} FALSE)
            else(_TMP_RESULT)
                set(${RESULT_VAR} TRUE)
            endif(_TMP_RESULT)
        endif(${RESULT_VAR} AND EXISTS ${_SRCFILE_FAIL_COMPILE})

        if(${RESULT_VAR})
            message(STATUS "Checking C++11 support for ${_LOG_NAME} -- works")
            list(APPEND CXX11_FEATURE_LIST ${RESULT_VAR})
        else(${RESULT_VAR})
            message(STATUS "Checking C++11 support for ${_LOG_NAME} -- not supported")
        endif(${RESULT_VAR})
        set(${RESULT_VAR} ${${RESULT_VAR}} CACHE INTERNAL "C++11 support for ${_LOG_NAME}")
    endif(NOT DEFINED ${RESULT_VAR})
endmacro(CXX11_CHECK_FEATURE)

CXX11_CHECK_FEATURE("auto"               2546 HAS_CXX11_AUTO)
CXX11_CHECK_FEATURE("nullptr"            2431 HAS_CXX11_NULLPTR)
CXX11_CHECK_FEATURE("lambda"             2927 HAS_CXX11_LAMBDA)
CXX11_CHECK_FEATURE("static_assert"      1720 HAS_CXX11_STATIC_ASSERT)
CXX11_CHECK_FEATURE("rvalue_references"  2118 HAS_CXX11_RVALUE_REFERENCES)
CXX11_CHECK_FEATURE("decltype"           2343 HAS_CXX11_DECLTYPE)
CXX11_CHECK_FEATURE("cstdint"            ""   HAS_CXX11_CSTDINT_H)
CXX11_CHECK_FEATURE("long_long"          1811 HAS_CXX11_LONG_LONG)
CXX11_CHECK_FEATURE("variadic_templates" 2555 HAS_CXX11_VARIADIC_TEMPLATES)
CXX11_CHECK_FEATURE("constexpr"          2235 HAS_CXX11_CONSTEXPR)
CXX11_CHECK_FEATURE("sizeof_member"      2253 HAS_CXX11_SIZEOF_MEMBER)
CXX11_CHECK_FEATURE("__func__"           2340 HAS_CXX11_FUNC)
CXX11_CHECK_FEATURE("delegating_ctors"   1986 HAS_CXX11_DELEGATING_CTORS)

macro(require_CXX11_feature FEATURE_NAME)
    list(FIND CXX11_FEATURE_LIST ${FEATURE_NAME} FEATURE_FOUND)
    if(${FEATURE_FOUND} EQUAL -1)
        message(FATAL_ERROR "Suppport for C++11 feature '${FEATURE_NAME}' is required")
    endif(${FEATURE_FOUND} EQUAL -1)
endmacro(require_CXX11_feature)

set(CXX11_FEATURE_LIST ${CXX11_FEATURE_LIST} CACHE STRING "C++11 feature support list")
mark_as_advanced(FORCE CXX11_FEATURE_LIST CXX11_FLAGS require_CXX11_feature)

set(CMAKE_CXX_FLAGS ${CHECK_CXX11_OLD_CMAKE_CXX_FLAGS})
unset(CHECK_CXX11_OLD_CMAKE_CXX_FLAGS)
