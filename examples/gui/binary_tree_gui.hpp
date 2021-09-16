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
#include "geometricalgorithms.hpp"

#include "rasterwindow.h"
#include <random>
#include <cmath>
#include "tree_sorts.hpp"
using namespace GGL;
class BinTree : public RasterWindow
{

public:
    BinTree();

protected:
    void timerEvent(QTimerEvent *) override;
    void render(QPainter *p) override;

private:
    void intialiseGraph();

    int m_timerId;
    BST<int , std::string> bst;
    std::random_device r;
    bool pause{false};
    void mousePressEvent(QMouseEvent *);
};

void BinTree::intialiseGraph()
{




}
BinTree::BinTree()
{
    setTitle("BST");
    resize(700, 700);

    intialiseGraph();
    m_timerId = startTimer(250);

}
void BinTree::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId){
         renderLater();
         if(pause) return;
         if(bst.size()>32){
             bst.clear();
         }
         std::uniform_int_distribution<int> random_d(0,100);
         auto e=std::default_random_engine{r()};
         bst.insert(random_d(e)," ");


    }
}


void BinTree::render(QPainter *p)
{
    QColor vertexcolor(127, 0, 127);
    QColor edgecoulor(127, 0, 0);

    auto drawText=[&](auto rect, auto text,auto size,auto color){
        p->save();
        p->setPen(QPen(color));
        p->setFont(QFont("Times New Roman",size));
        p->drawText(rect,text,QTextOption(Qt::AlignCenter));
        p->restore();
    };
    auto draw_vertex=[&](auto v,auto pen){
        p->save();
        p->setPen(pen);
        p->setBrush(pen.color());
        p->drawEllipse(v,15,15);
        p->restore();
        return  v;
    };
    auto draw_line=[&](auto pt1,auto pt2,auto color){

        p->save();
        p->setPen(color);
        p->setBrush(color);
        p->drawLine(pt1,pt2);
        p->restore();
    };

    int vpadding=100;
    int hpadding=1;
    int bottomwidth=3;
    auto drawNode=[&](auto recur,auto point, auto level,auto node)->QPoint{
        if(level ){
            draw_vertex(point,QPen(node->color?Qt::red:Qt::black));
            if(node->left())
                draw_line(point,recur(recur,point+QPoint(-pow(2,level+bottomwidth),vpadding),--level,node->left()),edgecoulor);
            if(node->right())
                draw_line(point,recur(recur,point+QPoint(pow(2,level+bottomwidth),vpadding),level,node->right()),edgecoulor);
            QSize rsize(20,20);
            drawText(QRect(point-QPoint((rsize/2).width(),(rsize/2).height()),rsize),std::to_string(node->key).c_str(),10,Qt::white);
        }
        return point;
    };
    drawText(QRect(0,25,width(),25),"Red-Black BST",20,Qt::black);
    int maxlevel =bst.height();
     QPoint rootOffset (width()/2,50);
     rootOffset -= QPoint(pow(2,maxlevel+bottomwidth),0);
    drawNode(drawNode,rootOffset+QPoint(pow(2,maxlevel+bottomwidth),50),maxlevel,bst.getRoot());

}
void BinTree::mousePressEvent(QMouseEvent *evt)
{

    pause=!pause;
}
