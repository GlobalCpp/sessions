#include <print>

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

int main()
{
	plants::Tree t {"Oak"};
	animals::Squirrel s {"Sandy"};
	print(t); // t is from plants, so plants namespace is considered, animals is not
	print(s); // s is from animals, so animals namespace is considered, plants is not
	plants::print(s); // print is qualified, so plants namespace version is used
return 0;
}

