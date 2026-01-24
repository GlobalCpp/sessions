#include <print>

namespace myNamespace
{
    struct Header 
	{
		const int version = 1;
		int len = 0;
	};
    struct Message1 
	{
		Header hdr;
		char text[256];
	};
} // namespace myNamespace

namespace myNamespace
{
    struct Message2
	{
		Header hdr;
		char text[512];
	};
} // namespace myNamespace

int main() { 
	std::println("Unlike classes, you can reopen namespaces"); 
	return 0;
}

