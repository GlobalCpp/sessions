#include <list>
#include <memory>
#include <print>
#include <vector>

struct ContainerA
{
	size_t size() const noexcept { return data.size(); }

	std::vector<int> data;
};

struct ContainerB
{
	size_t size() const noexcept { return data.size(); }

	std::list<float> data;
};

struct TypeErased
{
public:
	template<typename T>
	TypeErased(T&& t)
		: _data(new Model<T>{t})
	{}

private:
	struct Concept
	{
		virtual ~Concept() {}
		
		virtual size_t size() const noexcept = 0;
	};
	template<typename T>
	struct Model : Concept
	{
		Model(T const& t) : actualThing(t) {}
		size_t size() const noexcept { return actualThing.size(); }
		T actualThing;
	};

private:
	std::unique_ptr<Concept> _data;
}; 


int main(int argc, char** argv)
{
	TypeErased f{ContainerA()};
	//f = ContainerB();

return 0;
}

