#include "geometricalgorithms.hpp"
using namespace GGL;
int main(){
    using namespace std;
    vector<Point> points{
        {1,10},{7,7},{3,5},{8,1}
    };
    for(auto p: convex_hull(points)){
        cout <<p;
    }
    return 0;
}