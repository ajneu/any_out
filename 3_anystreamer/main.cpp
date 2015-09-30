#include <iostream>
#include <string>
#include "anystreamer.hpp"


int main()
{
  char str[] = "mystring";
  AnyStreamer a{str};
  std::cout << a << std::endl;

  a = "myconststring";
  std::cout << a << std::endl;

  a = std::string("standard string");
  std::cout << a << std::endl;


  const char *x[2] = {"aaa", "bbb"};

  a = x;
  std::cout << a << std::endl;

  return 0;
}
