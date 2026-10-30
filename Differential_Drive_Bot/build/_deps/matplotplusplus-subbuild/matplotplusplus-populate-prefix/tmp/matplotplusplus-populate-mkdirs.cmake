# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/query/Portfolio/Differential_Drive_Bot/build/_deps/matplotplusplus-src"
  "/home/query/Portfolio/Differential_Drive_Bot/build/_deps/matplotplusplus-build"
  "/home/query/Portfolio/Differential_Drive_Bot/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix"
  "/home/query/Portfolio/Differential_Drive_Bot/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix/tmp"
  "/home/query/Portfolio/Differential_Drive_Bot/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix/src/matplotplusplus-populate-stamp"
  "/home/query/Portfolio/Differential_Drive_Bot/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix/src"
  "/home/query/Portfolio/Differential_Drive_Bot/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix/src/matplotplusplus-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/query/Portfolio/Differential_Drive_Bot/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix/src/matplotplusplus-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/query/Portfolio/Differential_Drive_Bot/build/_deps/matplotplusplus-subbuild/matplotplusplus-populate-prefix/src/matplotplusplus-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
