#include <iostream>

static bool debug_enabled{true};

void EnableDebug(void) { debug_enabled = true; }

void DisableDebug(void) { debug_enabled = false; }

bool IsDebugEnabled() { return debug_enabled; }

int main() {
  std::cout << "Debug (initial): " << debug_enabled << std::endl;

  DisableDebug();
  std::cout << "Debug (call disable): " << debug_enabled << std::endl;

  EnableDebug();
  std::cout << "Debug (call enable): " << debug_enabled << std::endl;

  return 0;
}
