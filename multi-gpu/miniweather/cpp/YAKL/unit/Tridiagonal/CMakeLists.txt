

set(MYSRC Tridiagonal.cpp)

add_executable(Tridiagonal ${MYSRC})
include(../../yakl_utils.cmake)
yakl_process_target(Tridiagonal)

separate_arguments(REAL_MPI_COMMAND UNIX_COMMAND ${MPI_COMMAND})

add_test(NAME Tridiagonal_test COMMAND ${REAL_MPI_COMMAND} ./Tridiagonal)

