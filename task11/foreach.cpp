#include <cstddef>
#include <iostream>

int main()
{
    int values[] = {10, 20, 30, 40};
    const std::size_t count = sizeof(values) / sizeof(values[0]);

    std::cout << "C-style loop:\n";
    for (std::size_t i = 0; i < count; ++i) {
        std::cout << values[i] << '\n';
    }

    std::cout << "range-based for:\n";
    for (const auto value : values) {
        std::cout << value << '\n';
    }
    return 0;
}
