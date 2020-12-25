#include <vector>

// Clear vector
void clear (std::vector<char> &vec) {
   std::vector<char> empty;
   std::swap(vec, empty);
};
