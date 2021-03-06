#include <cpp_wrapper.hpp>
#include <array.hpp>

#include <algorithm>
#include <sstream>

// C function for performance comparison
extern "C" double half_c(const double d)
{
  return 0.5*d;
}

namespace functions
{

double half_function(const double d)
{
	return 0.5*d;
}

template<typename T>
T half_template (const T x)
{
	return x / static_cast<T>(2);
}

void init_half_module(cpp_wrapper::Module& mod)
{
	// register a standard C++ function
	mod.method("half_d", &half_function);

	// register some template instantiations
	mod.method("half_i", &half_template<int>);
	mod.method("half_u", &half_template<unsigned int>);

	// Register a lambda
  mod.method("half_lambda", [](const double a) {return a*0.5;});

  // Looping function
  mod.method("half_loop_cpp!",
  [](cpp_wrapper::ArrayRef<double> in, cpp_wrapper::ArrayRef<double> out)
  {
    std::transform(in.begin(), in.end(), out.begin(), [](const double d) { return 0.5*d; });
  });
}

// Test for string conversion. Pointer to this function is passed to Julia as-is.
std::string concatenate_numbers(int i, double d)
{
  std::stringstream stream;
  stream << i << d;
  return stream.str();
}

std::string concatenate_strings(const int n, std::string s, const std::string& s2)
{
  std::string result;
  for(int i = 0; i != n; ++i)
  {
    result += s + s2;
  }
  return result;
}

void init_test_module(cpp_wrapper::Module& mod)
{
  mod.method("concatenate_numbers", &concatenate_numbers);
  mod.method("concatenate_strings", &concatenate_strings);
}

}

JULIA_CPP_MODULE_BEGIN(registry)
  functions::init_half_module(registry.create_module("CppHalfFunctions"));
  functions::init_test_module(registry.create_module("CppTestFunctions"));
JULIA_CPP_MODULE_END
