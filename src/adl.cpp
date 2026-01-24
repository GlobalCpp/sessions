#include <print>
#include <string>
#include <vector>

namespace plants {
	struct Tree { std::string name; };

	template<typename PlantT>
	void print(PlantT const& p)
	{ std::println("Plant: {}", p.name); }
} // namespace plants

namespace animals {
	struct Squirrel { std::string name; };

	template<typename AnimalT>
	void print(AnimalT const& a)
	{ std::println("Animal: {}", a.name); }
} // namespace animals

namespace n1 {
	struct Foo;
}
namespace n2 {
	struct Bar {
		void operator=(n1::Foo const&){}
	};
	template<typename T, typename S>
	void swap(T, S) {}
}
namespace n1 {
	struct Foo
	{
		void operator=(n2::Bar const&) {}
	};
}

int main()
{
	plants::Tree t {"Oak"};
	animals::Squirrel s {"Sandy"};
	print(t); // t is from plants, so plants namespace is considered, animals is not
	print(s); // s is from animals, so animals namespace is considered, plants is not
	plants::print(s); // print is qualified, so plants namespace version is used

	n1::Foo f;
	n2::Bar b;
	f = b;
	b = f;
	swap(b, f);

return 0;
}

