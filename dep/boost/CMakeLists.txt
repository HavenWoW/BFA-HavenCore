# 2026 BFA-HavenCore
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

if(WIN32)
  set(BOOST_DEBUG ON)
  if(DEFINED ENV{BOOST_ROOT})
    set(BOOST_ROOT $ENV{BOOST_ROOT})
  endif()

  set(Boost_USE_STATIC_LIBS        ON)
  set(Boost_USE_MULTITHREADED      ON)
  set(Boost_USE_STATIC_RUNTIME     OFF)
endif()

set(BOOST_REQUIRED_VERSION 1.74)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 OLD)
endif()

find_package(Boost ${BOOST_REQUIRED_VERSION} REQUIRED system filesystem program_options regex)

add_library(boost INTERFACE)

target_link_libraries(boost
  INTERFACE
    ${Boost_LIBRARIES})

target_include_directories(boost
  INTERFACE
    ${Boost_INCLUDE_DIRS})

target_compile_definitions(boost
  INTERFACE
    BOOST_ALL_NO_LIB
    BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
    BOOST_ASIO_NO_DEPRECATED
    BOOST_BIND_NO_PLACEHOLDERS
    BOOST_SYSTEM_USE_UTF8)

if (WITH_BOOST_STACKTRACE AND NOT WIN32)
  message("*** libbacktrace will be linked")

  if (BOOST_STACKTRACE_BACKTRACE_INCLUDE_FILE)
    CHECK_INCLUDE_FILE(${BOOST_STACKTRACE_BACKTRACE_INCLUDE_FILE} HAS_BACKTRACE)
  else()
    CHECK_INCLUDE_FILE("backtrace.h" HAS_BACKTRACE)
  endif()

  if (NOT HAS_BACKTRACE)
    message(FATAL_ERROR "Required header 'backtrace.h' not found. If building with a compiler other than GCC, please specify the full path in the CMake option BOOST_STACKTRACE_BACKTRACE_INCLUDE_FILE.")
  endif()

  target_compile_definitions(boost
    INTERFACE
      BOOST_STACKTRACE_USE_BACKTRACE)

  target_link_libraries(boost
    INTERFACE
      backtrace)
endif()
