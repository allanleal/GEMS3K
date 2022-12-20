#find_package(ZeroMQ REQUIRED)
#find_library(ZEROMQ_LIB zeromq)
#if(NOT ZEROMQ_LIB)
#  message(FATAL_ERROR "zeromq library not found")
#endif()


find_package(ThermoFun REQUIRED)
if(NOT ThermoFun_FOUND)
  message(FATAL_ERROR "ThermoFun library not found")
else()
  message(STATUS "Found ThermoFun v${ThermoFun_VERSION}")
endif()

find_package(ChemicalFun REQUIRED)
if(NOT ChemicalFun_FOUND)
  message(FATAL_ERROR "ChemicalFun library not found")
else()
  message(STATUS "Found ChemicalFun v${ChemicalFun_VERSION}")
endif()

find_package(spdlog REQUIRED)
if(NOT spdlog_FOUND)
  message(FATAL_ERROR "spdlog library not found")
else()
  message(STATUS "Found spdlog v${spdlog_VERSION}")
endif()

#find_package(fmt REQUIRED)
#if(NOT fmt_FOUND)
#  message(FATAL_ERROR "fmt library not found")
#else()
#  message(STATUS "Found fmt v${fmt_VERSION}")
#endif()
