#include <print>

namespace myProtocol::v1
{
    struct Header { const int version = 1; int len = 0; };
    struct Message { char text[256]; };
} // namespace myProtocol::v1

namespace myProtocol::v2
{
    struct Header { const int version = 2; int len = 0; };
    struct Message { char text[512]; };
} // namespace myProtocol::v2

namespace myProtocol
{
    using namespace v2; // Pull v2 names into myProtocol
} // namespace myProtocol

static_assert(sizeof(myProtocol::Message::text) == 512);

int main() { 
	std::println("Namespaces can be included, one into another"); 
	std::println("Namespaces can be defined using qualified names for simplicity"); 
	return 0;
}
