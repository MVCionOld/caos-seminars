//
// Created by Mikhail Tsion on 04.09.2021.
//

#include <stdio.h>

int main() {
  const int a = 2;
  ++(*(&a));
  printf("%d\n", a);
  return 0;
}