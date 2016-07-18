#include "lcio.h"
#include <cxx_wrap.hpp>
#include <string>

std::string open() {
  return "hello";
}

JULIA_CPP_MODULE_BEGIN(registry)
  cxx_wrap::Module& lciowrap = registry.create_module("lciowrap");
// lciowrap.method("open", &open);
JULIA_CPP_MODULE_END
