#include <iostream>
#include <cmath>
int main(int argc, char const *argv[])
{
  int a = 5.4;
  std::cout << typeid(static_cast<int>(a)).name() << "\n";
  return 0;
}
