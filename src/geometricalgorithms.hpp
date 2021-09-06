#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
namespace GGL{
    struct Point
    {
        int x_;
        int y_;
        int x()const {return x_;}
        int y()const {return y_;}
        friend auto& operator<<(auto& os,const Point& p){
            os<<"{ "<<p.x()<<", "<<p.y()<<"} ";
            return os;
        }
        friend Point operator -(const Point& f, const Point& s){
            return Point{f.x()-s.x(),f.y()-s.y()};
        }
    };
    
    int ccw(const auto& f, const auto& s, const auto& t){
        using Vec =decltype(f-s);
        Vec fs = s-f;
        Vec ft = t-f;
        auto area = fs.x()*ft.y() -fs.y()*ft.x();
        return area;
    }
    float polar(const auto& a , const auto& b){
        using Vec =decltype(a-b);
        Vec ab=b-a;
        if(ab.y()==0) return 0;
        if(ab.x()==0) return 3.14/2;
        auto v= atan2(ab.y(),ab.x());
        return v;
    }
    template<typename Point>
    auto convex_hull( std::vector<Point>  points){
        if(points.size()<=2) return points;
        std::vector<Point> res;
        std::sort(begin(points),end(points),[](auto a , auto b){ return a.y() < b.y();});
        auto start=points[0];
        std::sort(begin(points),end(points),[&](auto a , auto b){ return polar(start,a)<polar(start,b);});
        res.push_back(start);
        res.push_back(points[1]);
        for(int i=2; i<points.size();i++){
            auto top = res.back();res.pop_back();
            while(ccw(res.back(),top,points[i])<=0){
                top = res.back(); res.pop_back();
            }
            res.push_back(top);
            res.push_back(points[i]);
        }
        return res;
    }

}
