#define my_macro(type, var, value) type var = value;

my_macro((std::vector<pair<int, int>), v, ({{1, 1}, {2, 2}, {3, 3}}))