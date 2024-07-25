#include <math.h>
#include "Arduino.h"
#include "utils.h"

String lpad(int number, int n) {
  String s = String(abs(number));
  if (number >= 0) {
    while (s.length() < n) s = "0" + s; 
  } else {
    while (s.length() < n - 1) s = "0" + s; 
    s = '-' + s;
  }
  return s;
}
