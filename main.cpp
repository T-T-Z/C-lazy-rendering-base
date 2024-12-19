#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <string>
#include <map>

static const std::vector<std::vector<int>> EdgeVertexIndices = {
        {0,1}, {1,2}, {2,3}, {3,0},
        {4,5}, {5,6}, {6,7}, {7,4},
        {0,4}, {1,5}, {2,6}, {3,7}
};

// For each MC case, a list of triangles, specified as triples of edge indices, terminated by -1
static const std::vector<std::vector<int>> TriangleTable = {
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
        {3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
        {3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
        {3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
        {9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
        {9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
        {2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
        {8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
        {9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
        {4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
        {3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
        {1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
        {4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
        {4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
        {9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
        {5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
        {2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
        {9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
        {0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
        {2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
        {10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
        {4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
        {5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
        {5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
        {9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
        {0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
        {1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
        {10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
        {8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
        {2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
        {7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
        {9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
        {2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
        {11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
        {9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
        {5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
        {11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
        {11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
        {1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
        {9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
        {5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
        {2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
        {0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
        {5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
        {6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
        {3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
        {6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
        {5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
        {1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
        {10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
        {6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
        {8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
        {7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
        {3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
        {5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
        {0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
        {9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
        {8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
        {5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
        {0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
        {6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
        {10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
        {10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
        {8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
        {1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
        {3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
        {0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
        {10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
        {3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
        {6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
        {9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
        {8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
        {3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
        {6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
        {0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
        {10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
        {10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
        {2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
        {7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
        {7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
        {2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
        {1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
        {11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
        {8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
        {0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
        {7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
        {10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
        {2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
        {6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
        {7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
        {2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
        {1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
        {10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
        {10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
        {0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
        {7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
        {6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
        {8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
        {9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
        {6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
        {4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
        {10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
        {8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
        {0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
        {1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
        {8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
        {10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
        {4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
        {10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
        {5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
        {11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
        {9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
        {6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
        {7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
        {3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
        {7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
        {9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
        {3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
        {6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
        {9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
        {1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
        {4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
        {7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
        {6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
        {3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
        {0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
        {6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
        {0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
        {11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
        {6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
        {5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
        {9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
        {1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
        {1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
        {10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
        {0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
        {5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
        {10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
        {11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
        {9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
        {7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
        {2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
        {8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
        {9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
        {9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
        {1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
        {9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
        {9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
        {5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
        {0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
        {10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
        {2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
        {0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
        {0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
        {9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
        {5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
        {3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
        {5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
        {8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
        {0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
        {9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
        {1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
        {3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
        {4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
        {9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
        {11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
        {11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
        {2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
        {9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
        {3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
        {1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
        {4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
        {4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
        {0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
        {3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
        {3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
        {0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
        {9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
        {1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};


// Shader Class
class Shader {
public:
    Shader(const std::map<GLenum, std::string>& shaderPaths);
    ~Shader();

    void use() const;
    GLuint getProgram() const { return program; }

    void setMat4(const std::string& name, const glm::mat4& matrix) const {
        GLint location = glGetUniformLocation(program, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
    }
    
    void setVec3(const std::string& name, const glm::vec3& value) const {
        GLint location = glGetUniformLocation(program, name.c_str());
        glUniform3fv(location, 1, &value[0]);
    }

private:
    GLuint compileShader(GLenum type, const std::string& source);
    GLuint createProgram(const std::vector<GLuint>& shaders);
    
    GLuint program;
    
    // Helper function to load shader source from a file
    std::string loadShaderSource(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open shader file: " << filepath << std::endl;
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
};

Shader::Shader(const std::map<GLenum, std::string>& shaderPaths) {
    std::vector<GLuint> shaders;

    for (const auto& [type, path] : shaderPaths) {
        std::string source = loadShaderSource(path);
        GLuint shader = compileShader(type, source);
        shaders.push_back(shader);
    }

    program = createProgram(shaders);

    // Clean up shaders (they're already attached to the program)
    for (GLuint shader : shaders) {
        glDeleteShader(shader);
    }
}

Shader::~Shader() {
    glDeleteProgram(program);
}

void Shader::use() const {
    glUseProgram(program);
}

GLuint Shader::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        char* log = new char[logLength];
        glGetShaderInfoLog(shader, logLength, &logLength, log);
        std::cerr << "Shader Compilation Failed (" << type << "): " << log << std::endl;
        delete[] log;
    }

    return shader;
}

GLuint Shader::createProgram(const std::vector<GLuint>& shaders) {
    GLuint program = glCreateProgram();

    // Attach all shaders
    for (GLuint shader : shaders) {
        glAttachShader(program, shader);
    }

    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        char* log = new char[logLength];
        glGetProgramInfoLog(program, logLength, &logLength, log);
        std::cerr << "Program Linking Failed: " << log << std::endl;
        delete[] log;
    }

    return program;
}

class Buffer {
public:
    enum Type {
        VERTEX_BUFFER,
        ELEMENT_BUFFER,
        STORAGE_BUFFER,
        UNIFORM_BUFFER
    };

    // Constructor for a buffer object
    Buffer(Type type, GLenum usage = GL_STATIC_DRAW)
        : type(type), usage(usage), bufferID(0) {}

    // Destructor
    ~Buffer() {
        if (bufferID != 0) {
            glDeleteBuffers(1, &bufferID);
        }
    }

    // Generate the buffer
    void generateBuffer() {
        glGenBuffers(1, &bufferID);
    }

    // Bind the buffer to a specific target (e.g., GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, etc.)
    void bind() const {
        GLenum target = getBufferTarget();
        glBindBuffer(target, bufferID);
    }

    // Unbind the buffer
    void unbind() const {
        GLenum target = getBufferTarget();
        glBindBuffer(target, 0);
    }

    // Upload data to the buffer
    void uploadData(const void* data, size_t size) {
        bind();
        glBufferData(getBufferTarget(), size, data, usage);
    }

    // Map the buffer for reading or writing
    void* mapBuffer(GLenum access = GL_READ_WRITE) {
        bind();
        return glMapBuffer(getBufferTarget(), access);
    }

    // Unmap the buffer
    void unmapBuffer() const {
        glUnmapBuffer(getBufferTarget());
    }

    // Set a buffer as an SSBO or UBO (with binding points)
    void setBufferBinding(GLuint bindingPoint) {
        bind();
        if (type == STORAGE_BUFFER) {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, bufferID);
        } else if (type == UNIFORM_BUFFER) {
            glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, bufferID);
        }
    }

    GLuint getBufferID() const { return bufferID; }

private:
    GLuint bufferID;    // OpenGL buffer ID
    Type type;          // Type of buffer (e.g., VBO, EBO, SSBO)
    GLenum usage;       // Usage hint for the buffer (e.g., GL_STATIC_DRAW, GL_DYNAMIC_DRAW)

    // Returns the appropriate OpenGL target based on the buffer type
    GLenum getBufferTarget() const {
        switch (type) {
            case VERTEX_BUFFER: return GL_ARRAY_BUFFER;
            case ELEMENT_BUFFER: return GL_ELEMENT_ARRAY_BUFFER;
            case STORAGE_BUFFER: return GL_SHADER_STORAGE_BUFFER;
            case UNIFORM_BUFFER: return GL_UNIFORM_BUFFER;
            default: return GL_ARRAY_BUFFER;  // Default to vertex buffer
        }
    }
};

class Player {
public:
    Player(glm::vec3 position, glm::vec3 direction, float speed, 
           float fov = 100.0f, float aspectRatio = 4.0f / 3.0f, 
           float nearPlane = 0.1f, float farPlane = 100.0f)
        : position(position), direction(glm::normalize(direction)), 
          speed(speed), fov(fov), aspectRatio(aspectRatio), 
          nearPlane(nearPlane), farPlane(farPlane), yaw(0.0f), pitch(0.0f) {}

    void move(const sf::Time& deltaTime, const sf::Keyboard::Key& forwardKey, 
              const sf::Keyboard::Key& backwardKey, const sf::Keyboard::Key& leftKey, 
              const sf::Keyboard::Key& rightKey, const sf::Keyboard::Key& upKey, 
              const sf::Keyboard::Key& downKey) {
        float velocity = speed * deltaTime.asSeconds();

        if (sf::Keyboard::isKeyPressed(forwardKey)) {
            position += direction * velocity;
        }
        if (sf::Keyboard::isKeyPressed(backwardKey)) {
            position -= direction * velocity;
        }
        if (sf::Keyboard::isKeyPressed(leftKey)) {
            position -= glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f))) * velocity;
        }
        if (sf::Keyboard::isKeyPressed(rightKey)) {
            position += glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f))) * velocity;
        }
        if (sf::Keyboard::isKeyPressed(upKey)) {
            position += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
        }
        if (sf::Keyboard::isKeyPressed(downKey)) {
            position -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
        }
    }

    // Mouse input for looking around
    void lookAround(const sf::Window& window, float deltaTime) {
        // Get the mouse position relative to the window
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2i center(window.getSize().x / 2, window.getSize().y / 2);

        // Calculate the mouse delta movement
        float deltaX = mousePos.x - center.x;
        float deltaY = mousePos.y - center.y;

        // Sensitivity for mouse movement
        float sensitivity = 0.1f;
        
        // Update yaw and pitch based on mouse movement
        yaw += deltaX * sensitivity;
        pitch -= deltaY * sensitivity;  // Invert the Y-axis if needed

        // Clamp the pitch to prevent flipping over (e.g., pitch between -89 and 89 degrees)
        pitch = glm::clamp(pitch, -89.0f, 89.0f);

        // Update the direction vector based on the new yaw and pitch
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction = glm::normalize(front);

        // Move the mouse back to the center of the screen
        sf::Mouse::setPosition(center, window);
    }

    glm::mat4 getTransformationMatrix() const {
        glm::mat4 model = glm::mat4(1.0f); // Identity matrix
        model = glm::translate(model, position);  // Apply translation (move the player)
        model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));  // Yaw (Y-axis)
        model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));  // Pitch (X-axis)

        return model;
    }

    glm::mat4 getViewMatrix() const {
        glm::vec3 target = position + direction;  // The point the camera is looking at
        glm::vec3 up(0.0f, 1.0f, 0.0f);  // Up direction
        return glm::lookAt(position, target, up);  // LookAt function to create a view matrix
    }

    glm::mat4 getProjectionMatrix() const {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);  // Perspective projection matrix
    }

    glm::vec3 getPosition() const {
        return position;
    }

private:
    glm::vec3 position;
    glm::vec3 direction;
    float speed;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    float yaw;    // Horizontal rotation (yaw) around the vertical axis
    float pitch;  // Vertical rotation (pitch) around the horizontal axis
};

// PointCluster Class: A class to hold all points for a single cluster
class PointCluster {
public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    std::unordered_map<std::string, bool> activePoints;
    std::vector<glm::vec3> points;
    float gridSize;

    // Add a point to the cluster
    void addPoint(const glm::vec3& point) {
        points.push_back(point);
        activePoints[pointHash(point)] = true;
    }

    // Add a point to the cluster
    void removePoint(const glm::vec3& point) {
        // TODO actually remove point from points vector
        activePoints.erase(pointHash(point));
    }


    // Get all points in the cluster
    const std::vector<glm::vec3>& getPoints() const {
        return points;
    }

    // Get all points in the cluster
    const int size() const {
        return points.size();
    }

    const int vertexCount() const {
        return vertices.size();
    }

    const int indicesCount() const {
        return indices.size();
    }

    // Check if the cluster has any points
    bool isEmpty() const {
        return points.empty();
    }

    // Helper function to explore clusters of connected points
    void marchCube(const glm::vec3& point) {
        std::string key = pointHash(point);
        if (visited.find(key) != visited.end()) return;

        static const std::vector<glm::ivec3> quadrant1 = { // right top forward
            {0.0, 0.0, 0.0},{1.0, 0.0, 0.0},{1.0, 1.0, 0.0},{0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0},{1.0, 0.0, 1.0},{1.0, 1.0, 1.0},{0.0, 1.0, 1.0}
        };

        static const std::vector<glm::ivec3> quadrant2 = { // left top forward
            {0.0, 0.0, 0.0},{-1.0, 0.0, 0.0},{-1.0, 1.0, 0.0},{0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0},{-1.0, 0.0, 1.0},{-1.0, 1.0, 1.0},{0.0, 1.0, 1.0}
        };

        static const std::vector<glm::ivec3> quadrant3 = { // right top back
            {0.0, 0.0, 0.0},{1.0, 0.0, 0.0},{1.0, 1.0, 0.0},{0.0, 1.0, 0.0},
            {0.0, 0.0, -1.0},{1.0, 0.0, -1.0},{1.0, 1.0, -1.0},{0.0, 1.0, -1.0}
        };

        static const std::vector<glm::ivec3> quadrant4 = { // left top back
            {0.0, 0.0, 0.0},{-1.0, 0.0, 0.0},{-1.0, 1.0, 0.0},{0.0, 1.0, 0.0},
            {0.0, 0.0, -1.0},{-1.0, 0.0, -1.0},{-1.0, 1.0, -1.0},{0.0, 1.0, -1.0}
        };

        static const std::vector<glm::ivec3> quadrant5 = { // right bottom forward
            {0.0, 0.0, 0.0},{1.0, 0.0, 0.0},{1.0, -1.0, 0.0},{0.0, -1.0, 0.0},
            {0.0, 0.0, 1.0},{1.0, 0.0, 1.0},{1.0, -1.0, 1.0},{0.0, -1.0, 1.0}
        };

        static const std::vector<glm::ivec3> quadrant6 = { // left bottom forward
            {0.0, 0.0, 0.0},{-1.0, 0.0, 0.0},{-1.0, -1.0, 0.0},{0.0, -1.0, 0.0},
            {0.0, 0.0, 1.0},{-1.0, 0.0, 1.0},{-1.0, -1.0, 1.0},{0.0, -1.0, 1.0}
        };

        static const std::vector<glm::ivec3> quadrant7 = { // right bottom back
            {0.0, 0.0, 0.0},{1.0, 0.0, 0.0},{1.0, -1.0, 0.0},{0.0, -1.0, 0.0},
            {0.0, 0.0, -1.0},{1.0, 0.0, -1.0},{1.0, -1.0, -1.0},{0.0, -1.0, -1.0}
        };

        static const std::vector<glm::ivec3> quadrant8 = { // left bottom back
            {0.0, 0.0, 0.0},{-1.0, 0.0, 0.0},{-1.0, -1.0, 0.0},{0.0, -1.0, 0.0},
            {0.0, 0.0, -1.0},{-1.0, 0.0, -1.0},{-1.0, -1.0, -1.0},{0.0, -1.0, -1.0}
        };

        static const std::vector<std::vector<glm::ivec3>> quadrants = {
            quadrant1, quadrant2, quadrant3, quadrant4,
            quadrant5, quadrant6, quadrant7, quadrant8
        };
        for(const auto& quadrant : quadrants){
            int cubeIndex = 0;
            int i = 0;
            for (const auto& dir : quadrant) {
                if(i == 0){
                    cubeIndex |= (1 << i++);  // corner 0 is always true so we asign it as such
                    continue;
                }
                glm::vec3 neighbor = point + glm::vec3(dir);
                if (visited.find(pointHash(neighbor)) != visited.end()) { // This quadrant was already generated
                    cubeIndex = 0;
                    break;
                }
                if(activePoints.find(pointHash(neighbor)) != activePoints.end()){
                    cubeIndex |= (1 << i);  // Set the corresponding bit if the corner is active
                }
                i++;
            }
            // If cubeIndex is 0 or 255, the cube is fully empty or fully full, no surface to generate
            if (cubeIndex == 0 || cubeIndex == 255) continue;
            for (const auto& edge : TriangleTable[cubeIndex])
            {
                if(edge < 0){break;}
                EdgeVertexIndices[edge];
                glm::vec3 vertexPoint = glm::mix(point + glm::vec3(quadrant[EdgeVertexIndices[edge][0]]) , point + glm::vec3(quadrant[EdgeVertexIndices[edge][1]]), 0.5f);
                auto vertexIndex = vertIndecies.find(pointHash(vertexPoint, gridSize/2));
                if(vertexIndex != vertIndecies.end()){
                    indices.push_back(vertexIndex->second);
                }else{
                    vertIndecies[pointHash(vertexPoint, gridSize/2)] = vertices.size();
                    indices.push_back(vertices.size());
                    vertices.push_back(vertexPoint);
                }
            }

        }
    }

    // Find all clusters in the point cloud
    void applyMarchingCubes() {
        visited.clear();
        vertIndecies.clear();

        for (const auto& point : points) {
            std::string key = pointHash(point);

            if (visited.find(key) == visited.end()) {
                marchCube(point);
                visited.insert(key);
            }
        }
    }

    std::vector<glm::vec3> getVerticies() {
        return vertices;
    }

    std::vector<unsigned int> getIndecies() {
        return indices;
    }
private:

    std::unordered_set<std::string> visited;
    std::unordered_map<std::string, int> vertIndecies;

    // Hash function for spatial partitioning (voxelization)
    std::string pointHash(const glm::vec3& point) const {
        int x = static_cast<int>(std::floor(point.x / gridSize));
        int y = static_cast<int>(std::floor(point.y / gridSize));
        int z = static_cast<int>(std::floor(point.z / gridSize));
        return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
    }

    std::string pointHash(const glm::vec3& point, float gridSize) const {
        int x = static_cast<int>(std::floor(point.x / gridSize));
        int y = static_cast<int>(std::floor(point.y / gridSize));
        int z = static_cast<int>(std::floor(point.z / gridSize));
        return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
    }
};

// ClusterCollection Class: A class to hold all PointClusters
class ClusterCollection {
public:
    std::vector<PointCluster> clusters;

    // Add a new cluster
    void addCluster(const PointCluster& cluster) {
        clusters.push_back(cluster);
    }

    // Get all clusters
    const std::vector<PointCluster>& getClusters() const {
        return clusters;
    }

    // Get all points in the cluster
    const int size() const {
        return clusters.size();
    }
};

class PointCloudMesh {
public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    // Unordered map for storing active grid points (using spatial hashing)
    std::unordered_map<std::string, glm::vec3> activePoints;
    std::unordered_set<std::string> visited;
    float gridSize;

    PointCloudMesh(const std::vector<glm::vec3>& pointCloud, float gridSize = 1.0f)
        : gridSize(gridSize) {
        snapPointsToGrid(pointCloud);
    }

    // Snap points to the grid and add unique points to activePoints
    void snapPointsToGrid(const std::vector<glm::vec3>& pointCloud) {
        for (const auto& point : pointCloud) {

            // Snap each point to the nearest grid point
            glm::vec3 snappedPoint = snapToGrid(point);
        
            std::string hash = pointHash(snappedPoint);
            if (activePoints.find(hash) == activePoints.end()) {
                activePoints[hash] = snappedPoint;
                vertices.push_back(snappedPoint);
            }
        }

        std::cout << "Snapped and merged " << vertices.size() << " unique points." << std::endl;
    }

    // Check if a neighboring voxel exists (within a 3x3x3 cube of neighbors)
    bool hasNeighbor(const glm::vec3& point) {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dz = -1; dz <= 1; ++dz) {
                    if (dx == 0 && dy == 0 && dz == 0) continue; // skip the point itself
                    glm::vec3 neighbor = point + glm::vec3(dx, dy, dz) * gridSize;
                    std::string neighborHash = pointHash(neighbor);
                    if (activePoints.find(neighborHash) != activePoints.end()) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    // Helper function to explore clusters of connected points
    void exploreCluster(const glm::vec3& point, PointCluster& cluster) {
        static const std::vector<glm::ivec3> directions = {
            {1,0,0}, { 0, 1, 0}, { 0, 0, 1},
            {-1,0,0}, { 0, -1, 0}, { 0, 0, -1},
        };

        // Mark the current point as visited
        std::string key = pointHash(point);
        if (visited.find(key) != visited.end()) return;

        visited.insert(key);
        cluster.addPoint(point);

        // Explore all 26 neighbors
        for (const auto& dir : directions) {
            glm::vec3 neighbor = point + glm::vec3(dir);
            if (activePoints.find(pointHash(neighbor)) != activePoints.end() && visited.find(pointHash(neighbor)) == visited.end()) {
                exploreCluster(neighbor, cluster);
            }
        }
    }

    // Find all clusters in the point cloud
    ClusterCollection findClumps() {
        ClusterCollection clusterCollection;

        for (const auto& entry : activePoints) {
            const glm::vec3& point = entry.second;
            std::string key = pointHash(point);

            // If the point hasn't been visited yet, it's a new cluster
            if (visited.find(key) == visited.end()) {
                PointCluster cluster;
                cluster.gridSize = gridSize;
                exploreCluster(point, cluster);
                clusterCollection.addCluster(cluster);
            }
        }

        return clusterCollection;
    }

private:
    // Hash function for spatial partitioning (voxelization)
    std::string pointHash(const glm::vec3& point) const {
        int x = static_cast<int>(std::floor(point.x / gridSize));
        int y = static_cast<int>(std::floor(point.y / gridSize));
        int z = static_cast<int>(std::floor(point.z / gridSize));
        return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
    }

    // Function to snap a point to the nearest grid point based on grid size
    glm::vec3 snapToGrid(const glm::vec3& point) {
        // Round each component to the nearest multiple of gridSize
        glm::vec3 snappedPoint;
        snappedPoint.x = std::round(point.x / gridSize) * gridSize;
        snappedPoint.y = std::round(point.y / gridSize) * gridSize;
        snappedPoint.z = std::round(point.z / gridSize) * gridSize;
        return snappedPoint;
    }
};

// Function to generate a random float in the range [min, max]
float randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

int main() {
    // Initialize SFML window and OpenGL context
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 4;
    settings.minorVersion = 3;

    sf::RenderWindow window(sf::VideoMode(1600, 1200), "Lazy Renderer", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);    // Default depth function; only objects closer than the previous depth value are rendered
    glDepthMask(GL_TRUE);    // Enable writing to the depth buffer
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//wireframe

    std::map<GLenum, std::string> renderShaderPaths = {
        {GL_VERTEX_SHADER, "vertex_shader.glsl"},
        {GL_GEOMETRY_SHADER, "geometry_shader.glsl"},
        {GL_FRAGMENT_SHADER, "fragment_shader.glsl"}
    };

    Shader shader(renderShaderPaths);

    std::map<GLenum, std::string> marchingCubesShader = {
        {GL_VERTEX_SHADER, "shaders/vertex_shader.glsl"},
        {GL_FRAGMENT_SHADER, "shaders/fragment_shader.glsl"}
    };

    Shader marchShader(marchingCubesShader);

    // Number of random points you want to generate
    int numPoints = 100000;

    // Vector to hold the random points
    std::vector<glm::vec3> points;

    //// Populate the vector with random 3D points
    for (int i = 0; i < numPoints; ++i) {
        // Generate random x, y, z values between -1.0f and 1.0f (or any desired range)
        float x = randomFloat(-3.0f, 3.0f);
        float y = randomFloat(-3.0f, 3.0f);
        float z = randomFloat(-3.0f, 3.0f);
        // Add the random point to the vector
        points.push_back(glm::vec3(x, y, z));
    }

    // Create PointCloudMesh object
    PointCloudMesh pcm(points, 0.1f);

    // Find clumps
    ClusterCollection clusters = pcm.findClumps();

    PointCluster chosenCluster = clusters.getClusters()[0];

    chosenCluster.applyMarchingCubes();

    // Output the results
    std::cout << "Number of clumps: " << clusters.size() << std::endl;
    for (size_t i = 0; i < clusters.size(); ++i) {
        if(clusters.getClusters()[i].vertexCount() > 2){
        std::cout << "Clump " << i + 1 << ":" << clusters.getClusters()[i].vertexCount() << std::endl;
        }
    }

    std::cout << "chosenCluster " << chosenCluster.vertexCount() << std::endl;

    Buffer vertexBuffer(Buffer::VERTEX_BUFFER);
    Buffer indexBuffer(Buffer::ELEMENT_BUFFER);

    vertexBuffer.generateBuffer();
    indexBuffer.generateBuffer();
    vertexBuffer.uploadData(chosenCluster.vertices.data(), chosenCluster.vertices.size() * sizeof(glm::vec3));
    indexBuffer.uploadData(chosenCluster.indices.data(), chosenCluster.indices.size() * sizeof(unsigned int));

    // Create a Vertex Array Object (VAO) for organizing vertex attributes
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Bind the VBO and set vertex attributes
    vertexBuffer.bind();
    indexBuffer.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);  // Enable the vertex attribute at location 0

    // Unbind the VAO and VBO
    glBindVertexArray(0);
    vertexBuffer.unbind();
    indexBuffer.unbind();

    Buffer pointBuffer(Buffer::VERTEX_BUFFER);

    pointBuffer.generateBuffer();
    pointBuffer.uploadData(chosenCluster.points.data(), chosenCluster.points.size() * sizeof(glm::vec3));

    // Create a Vertex Array Object (VAO) for organizing vertex attributes
    GLuint debugVao;
    glGenVertexArrays(1, &debugVao);
    glBindVertexArray(debugVao);

    // Bind the VBO and set vertex attributes
    pointBuffer.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);  // Enable the vertex attribute at location 0

    // Unbind the VAO and VBO
    glBindVertexArray(0);
    pointBuffer.unbind();

    // Create the Player object
    Player player(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 5.0f);  // Position, direction, speed

    // Time-related variables for smooth movement
    sf::Clock clock;

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        bool keyPressed = false;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            break;
        }

        // Get the elapsed time
        sf::Time deltaTime = clock.restart();

        // Update player movement
        player.move(deltaTime, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::Space, sf::Keyboard::LControl);

        // Update the player's look direction based on mouse movement
        player.lookAround(window, deltaTime.asSeconds());


        // Use the view and projection matrices in the shader
        glm::mat4 view = player.getViewMatrix();
        glm::mat4 projection = player.getProjectionMatrix();
        glm::mat4 model = player.getTransformationMatrix();  // Get the player’s transformation matrix

        // Set the matrices in the shader (example)
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setMat4("model", model);  // Pass the transformation matrix to the shader
        shader.setVec3("cameraPos", player.getPosition());


        window.clear();
        glClear(GL_DEPTH_BUFFER_BIT); // Clear the depth buffer

        glBindVertexArray(debugVao);
        //glDrawArrays(GL_POINTS, 0, pcm.vertices.size());  // Render point on each vertex
        glDrawArrays(GL_POINTS, 0, chosenCluster.points.size());  // Render point at each point
        glBindVertexArray(0);

        marchShader.use();
        marchShader.setMat4("view", view);
        marchShader.setMat4("projection", projection);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, chosenCluster.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        window.setTitle("FPS: " + std::to_string((1/deltaTime.asSeconds())));

        window.display();
    }

    return 0;
}

