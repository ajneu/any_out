#include <iostream>
#include <string>
#include "any_out.hpp"
#include <vector>

template
< typename T
  , template<typename ELEM, typename ALLOC=std::allocator<ELEM> > class Container
  >
std::ostream& operator<< (std::ostream& o, const Container<T>& container)
{
  typename Container<T>::const_iterator beg = container.begin();

  o << "["; // 1

  while(beg != container.end())
    {
      o << " " << *beg++; // 2
    }

  o << " ]"; // 3

  return o;
}

template <typename T>
void print_ref(const T& val)
{
  boost::any a = val;
  std::cout << *boost::any_cast<typename std::decay<decltype(val)>::type>(&a) << std::endl;  
}

template <typename T>
void print(const T val)
{
  boost::any a = val;
  std::cout << *boost::any_cast<T>(&a) << std::endl;  
}


int main()
{
  any_out a;
  std::cout << "nothing: " << a << std::endl;
  
  char str[] = "mystring";
  a = str;
  std::cout << a << std::endl;

  any_out b;
  b = a;
  std::cout << b << std::endl;
  std::cout << a << std::endl;
  
  a = "myconststring";
  std::cout << a << std::endl;

  a = std::string("standard string");
  std::cout << a << std::endl;

  a = std::vector<const char *>{"asdf", "qwer"};
  std::cout << a << std::endl;

  const char *x[2] = {"aaa", "bbb"};

  a = x;
  std::cout << a << std::endl;

  a = 22;
  std::cout << *boost::any_cast<int>(&(a.get())) << std::endl;
  std::cout <<  boost::any_cast<int>(a.get()) << std::endl;

  any_out c = a;
  std::cout << c << std::endl;
  
  return 0;
}
