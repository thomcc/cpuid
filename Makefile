

CXXFLAGS = -O3 -march=native -Wall -Wextra -xc++

cpuid-test: cpuid-test.o

cpuid-test.o: cpuid-test.cc cpuid-test.hh


