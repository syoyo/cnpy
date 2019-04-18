#include <complex>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include "cnpy.h"

// TODO(syoyo): Support windows
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

const int Nx = 128;
const int Ny = 64;
const int Nz = 32;

int main(void ) {

  // Assume generated file from example1.cpp
  // appended version. so (64, 64, 128) expected.
  int fd = open("arr1.npy", O_RDONLY);
  if (fd == -1) {
    perror("open");
    return EXIT_FAILURE;
  }

  struct stat sb;
  if (fstat(fd, &sb) == -1) { /* To obtain file size */
    perror("fstat");
    return EXIT_FAILURE;
  }

  size_t length = sb.st_size;

  void *addr = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, /* offset */0);
  if (addr == MAP_FAILED) {
    perror("mmap");
    return EXIT_FAILURE;
  }

  // load from memory
  cnpy::NpyArray arr = cnpy::npy_load(reinterpret_cast<const unsigned char *>(addr), length);
  std::complex<double>* loaded_data = arr.data<std::complex<double>>();

  std::cout << "arr.shape.size() = " << arr.shape.size() << std::endl;
  std::cout << "arr.shape[0] = " << arr.shape[0] << std::endl;
  std::cout << "arr.shape[1] = " << arr.shape[1] << std::endl;
  std::cout << "arr.shape[2] = " << arr.shape[2] << std::endl;

  // make sure the loaded data matches the saved data
  assert(arr.word_size == sizeof(std::complex<double>));
  assert(arr.shape.size() == 3 && arr.shape[0] == (Nz + Nz) && arr.shape[1] == Ny &&
         arr.shape[2] == Nx);

  return EXIT_SUCCESS;
}
