//
// Created by Mikhail Tsion on 04.09.2021.
//

#include "minmax.h"

#define __max(a, b) ((a) > (b) ? (a) : (b))
#define __min(a, b) ((a) < (b) ? (a) : (b))

extern int max(int lhs, int rhs) {
  return __max(lhs, rhs);
}

extern int min(int lhs, int rhs) {
  return __min(lhs, rhs);
}