/*
MIT License

Copyright (c) 2021 abhilashraju

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "quick_sort.hpp"
#include "merge_sort.hpp"
#include "rasterwindow.h"
#include <random>
#include <cmath>
using namespace GGL;
class SortView : public RasterWindow
{
    constexpr static int noofcols=10;
    constexpr static int noofrows=10;
public:
    SortView();

protected:
    void timerEvent(QTimerEvent *) override;
    void render(QPainter *p) override;

private:
    void intialiseGraph();
    struct SortTracker{
        std::vector<int> seq;
        int first;
        int second;
    };
    std::vector<SortTracker> sortsequences;
    std::vector<SortTracker>::iterator iter;
    int m_timerId;
};

void SortView::intialiseGraph()
{
      sortsequences.clear();
      std::vector<int> v(50);
      std::generate(v.begin(), v.end(), [n = 0] () mutable { return n++; });
      shuffle(v);
      auto tracer=[&](auto& f, auto& s){

          SortTracker currentframe;
          std::copy(begin(v),end(v),std::back_inserter(currentframe.seq));
          currentframe.first=f;
          currentframe.second=s;
          sortsequences.push_back(currentframe);

      };
      merge_sort(v,std::less<int>{},tracer);
//      qsort(v,std::less<int>{},tracer);
      iter=begin(sortsequences);
}
SortView::SortView()
{
    setTitle("Sorts");
    resize(700, 700);

    intialiseGraph();
    m_timerId = startTimer(50);

}
void SortView::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId){
        renderLater();
        iter++;
        if(iter == end(sortsequences)){
            intialiseGraph();
        }
    }
}


void SortView::render(QPainter *p)
{
    auto drawText=[&](auto rect, auto text){
        p->setFont(QFont("Times New Roman",20));
        p->drawText(rect,text,QTextOption(Qt::AlignCenter));
    };

   
    auto draw_rect=[&](auto rect,auto color){
        p->setPen(QPen(color));
        p->setBrush(color);
        p->drawRect(rect);
    };
    p->setPen(QPen(Qt::red));
    drawText(QRect(0,25,width(),50),"Merge Sort");
    if(iter != end(sortsequences)){
        auto& frame= *iter;
        int width =10;
        int heightunit =10;
        QPoint offset{50,100};
        int i=0;
        for(auto v:frame.seq){
            QColor color(v*5,v*5,0);
            if(v == frame.first || v==frame.second){
                color = Qt::red;
            }
            draw_rect(QRect(offset+QPoint(i++ * width,0),QSize(width,heightunit*v)),color);
        }

    }
    

}

