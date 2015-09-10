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
  char str[] = "mystring";
  any_out a = str;
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
  std::cout << *boost::any_cast<int>(&(a.get_any())) << std::endl;
  std::cout <<  boost::any_cast<int>(a.get_any()) << std::endl;


  
  return 0;
}
