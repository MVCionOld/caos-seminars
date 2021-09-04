#define my_macro(type, var, value) type var = value;
#define identity(...) __VA_ARGS__

my_macro(identity(std::vector<pair<int, int>), v, identity({{1, 1}, {2, 2}, {3, 3}}))