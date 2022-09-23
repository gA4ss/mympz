#include <iostream>
#include <cstdio>

using namespace std;

char to_char(int n)
{
  return (n < 10 && n >= 0) ? n + '0' : n - 9 + 64;
}

int main()
{
  long n = 1210, base = -10, t = 0, num = 0;
  long arr[20] = {0};

  while (n)
  {
    // std::cout << num << ":" << std::endl;
    arr[num] = n % base;
    n /= base;
    if (arr[num] < 0)
    {
      // std::cout << "r = " << arr[num] << " - " << base << " = " << arr[num] - base << std::endl;
      // std::cout << " q = q + 1 = " << n+1 << std::endl;
      
      arr[num] -= base;
      n++;
    } 
    // else 
    // {
    //   std::cout << "r = " << arr[num] << " q = " << n << std::endl;
    // }
    
    // std::cout << std::endl;
    num++;
  }

  for (t = num - 1; t >= 0; t--)
  {
    cout << to_char(arr[t]);
  }
}
