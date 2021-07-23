#include "gs_apps.cpp"
#include "dfs_bfs_gui.cpp"
#include "mst_gui.cpp"
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

//    MetrixGraphsUI graph;
//    MstGraph graph;
    GraphUi graph;
    graph.show();

    return app.exec();
}
