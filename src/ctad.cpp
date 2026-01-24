#include <print>

int main() {
	std::tuple t(5, 10.0, std::string("Hi"));
	static_assert(
		std::is_same_v<
			decltype(t),
	        std::tuple<int, double, std::string>>);

	std::println("Template parameters can be inferred from the constructor");

return 0;
}

