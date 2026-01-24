#include <print>
#include <tuple>
#include <string>

template<typename... Ts>
struct Foo
{
	Foo(Ts&&... ts) {}
};
template<typename... Ts>
Foo(Ts&&...) -> Foo<Ts...>;

int main() {
	std::tuple t(5, 10.0, std::string("Hi"));
	static_assert(
		std::is_same_v<
			decltype(t),
	        std::tuple<int, double, std::string>>);

	Foo f((const int)5);
	//Foo<const int> other; other = f;
	static_assert(
		std::is_same_v<
			decltype(f),
	        Foo<int>>);

	std::println("Template parameters can be inferred from the constructor");

return 0;
}

