#include <memory>
#include <print>
#include <vector>

// Consider a platform which wishes to store and retrieve data via json config files
// Check out boost's property tree for a simple means to load json files into a tree structure

namespace cfg
{
	template<typename T>
	struct KVP
	{
		std::string key; // Unique string for id of value in config
		T& val; // We want config to mutate original value (not demo'd)
	};

	// Type-Erasure structure that can store any type of KVP
	// See GlobalCpp session 2026/01/17 for refresher
	struct TEParam {
		template<typename T>
		explicit TEParam(T&& t) : c(new Model{t}) {}
		// copy-ctor needed to clone data
		TEParam(TEParam const& that) : c(that.c->clone()) {}

		void print() const { c->print();}

		struct Concept {
			virtual ~Concept() {}
			virtual void print() const = 0;
			virtual std::unique_ptr<Concept> clone() = 0;
		};
		template<typename T>
		struct Model : Concept {
			T v;

			Model(T const& t) : v(t) {}

			std::unique_ptr<Concept> clone() override
			{ return std::unique_ptr<Concept> { new Model{v} }; }

			void print() const override
			{ std::println("{}: {}", v.key, v.val); }
		};
		std::unique_ptr<Concept> c;
	};

	struct Params {
		std::vector<TEParam> vals;
		template<typename... ParamTs>
		Params(ParamTs&&... params)
			: vals{TEParam{std::forward<ParamTs>(params)}...}
		{}
		void print() const {
			for (auto const& v : vals) v.print();
		}
	};
}

namespace myApp
{
	struct Config1 {int i; double d;};
	struct Config2 {std::string s;};

	cfg::Params config(Config1& c) 
	{
		using namespace cfg;
		return {
			KVP {"int", c.i}, // Created using CTAD
			KVP {"double", c.d} // Created using CTAD
		};
	}
	cfg::Params config(Config2& c) 
	{
		using namespace cfg;
		return {
			KVP {"str", c.s} // Created using CTAD
		};
	}
} // namespace myApp

int main() {
	myApp::Config1 c1{10, 3.14};
	myApp::Config2 c2{"Hello World"};

	// config() overload is resolved via ADL
	config(c1).print();
	config(c2).print();

	c1.i = 25;
	config(c1).print();

	return 0;
}

