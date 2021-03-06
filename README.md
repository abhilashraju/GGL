# GGL-
Generic Graph Library , A library of generic graph data structures and algorithms

![Traversals](https://github.com/abhilashraju/GGL-/blob/main/demo/dfs_bfs.gif) ![MST](https://github.com/abhilashraju/GGL-/blob/main/demo/MST%202021-07-20%2022-44-32.gif)
![dapp](https://github.com/abhilashraju/GGL-/blob/main/demo/Graph%202021-07-21%2011-17-34.gif) ![shrtp](https://github.com/abhilashraju/GGL-/blob/main/demo/shrt_path.gif) ![ch](https://github.com/abhilashraju/GGL-/blob/main/demo/convex_hull.gif) ![qs](https://github.com/abhilashraju/GGL-/blob/main/demo/quicksort.gif) ![ms](https://github.com/abhilashraju/GGL-/blob/main/demo/merge_sort.gif) ![bst](https://github.com/abhilashraju/GGL-/blob/main/demo/red-black-bst.gif)

Generic and reusable components to solve common graph problems. The implementation follows STL philosophy that separates data structure and alogorithms and connect them together through well defined concepts. The library is 
1) A collection of reusable generic alogorthms that can work with any data type that are compatible with the concepts the algorithm expects
2) A collection of generic graph containers that reresents different graph models which can be parameterised based on user defined vertex type

By separating the algorithm from the graph structure gives us fexibility in combining them freely and hence generete an n x m solution space fron n + m resusable components.

Examples
```
using Graph=GraphImpl<int,undirected>;
using DGraph=GraphImpl<int,directed>;

DGraph g(10);
g.add_edge(0,2);
g.add_edge(0,5);
g.add_edge(2,5);
g.add_edge(1,5);
g.add_edge(5,1);
g.print(std::cout);
```
In the above example we have created a Directed graph that uses adjuscency list with integer vertex type. Library already provides reusable type aliases for most frequently used graphs. You can notice that how easy it is to change Directed Graph in to Undirected one just by changing the declaration of it. 
Some times you wanted to analyse relationships between entitities using graph algorithms. In those cases the vertex type may not be an integer . It will be an entity represented using your domain type such as Employee or Color. The following example shows how easy it is to create a graph with string vertex type.
```
using SGraph=GraphImpl<std::string,undirected>;
using SDGraph=GraphImpl<std::string,directed>;

SDGraph sg(10);
sg.add_edge("Abhilash","Abhila");
sg.add_edge("Abhilash","Pranav");
sg.add_edge("Abhila","Pranav");
sg.print(std::cout);
```
Notice the generic nature of the implementaion . With single implementation you achieved wide range of graphs with directed and undirected edges that supports any domain specific type. 

Now let's shift our focus to generic algorithm. Let's see how we can do a depth and breadth first seach using above graph structure
Lets us capture the relationship among people(captured using above graph) using a DFS
```
std::map<std::string,std::string> strparents;
travers(sg,"Abhilash",GDFS(),[&](auto p,auto c){
    strparents[c]=p;
},empty_visitor);
```
Travers is a generic function from library that accepts a graph , the start vertex , the search algorithm and two user specfic action that need to be taken whenever 
1) algorithm visit a vertex.
2) algorithm enques a child vertex for visit.

User can choose to provide any of the above two callbacks my leaving other empty depending on their requirements. In this purticular example I choose to give one that can capture the parent child relationship between vertices.

Now let's dig a bit deeper into the actual search algorithms
The only difference between DFS, and BFS is in its usage of stack and queue . And the main difference between stack and queue is in its pushing strategy. Then why not parameterise it?
So we can make a generic BFS_DFS algorithm that is prameterised on its pushing stractegy as below

```
using GBFS=BFS_DFS<make_bfs_push>;
using GDFS=BFS_DFS<make_dfs_push>;
```

Now the traverse implementation is as simple as below
```
inline void travers(auto& g,auto root,auto alg,auto child_handler,auto handler){
    alg(g,root,child_handler,handler);
}
```
If you wanted to find out number of connected components in the same graph .There is an algorithm for you . The following code demonstrate that.

```
ConnectedMap<VertexType> map;
Connected_Cmponents{}(g,[&](auto index,auto v){
    map[index].push_back(v);
});
std::cout<< "\nPrinting connected components..\n";
for(auto& l:map){
     std::cout<<l.first<< "= ";
    for(auto& e:l.second){
        std::cout<<e<<" ";
    }
    std::cout<<"\n";
}
```
The connected components is another useful algorithm that accepts a graph and a callback. The callback will be called with the vertex and the id of connected component it belong to . The id is just an integer indicating the current connected component count. So, after  completing the algorithm  you can use the last id as total number of connected component in the graph.
One good application of connected component and our people graph is that you can easily find out how many different families are there in the graph and  can cluster them based on the family they belong to.

<B> Matrix Graph </B>

Adjuscency list based graph is just one part of the story. In real life we will come across other kind of graphs that comes in matrix form. In this kind of graph each cell of the matrix represents a vertex.Basically this kind of graphs conveys a cell's relationship with it's neighbhouring cell. The edges are implicit in this graph. It depend on the iterator mechanism we chose to visit neighbouring vertices. For example we can choose to traverse the negihborhood vertices in a cross manner. Possible edges in this cross metrix is left/right and top/bottom . It looked like a cross , hence the name is chosen. Another possible traversal method is grid. Possible edges in grid metrix includes all neighbouring vertices including corner one. That looked like a grid, hence the name is chosen. Apart from difference in these iteration directions everything else in this graph is same. So a better choice of implimentation is to have a generic matrix graph parmeterised using an iterator type.
The following code snippet shows avaiable metrix graph

```
template<typename T>
using RowMatrix=MatrixGraph<T,typename MatrixIterators<T>::RowList>;
template<typename T>
using CrossMatrix=MatrixGraph<T,typename MatrixIterators<T>::CrossList>;
template<typename T>
using GridMatrix=MatrixGraph<T,typename MatrixIterators<T>::GridList>;
```
The cell contents can be any user defined types. Generally this kind of graph comes in image processing use case ,where each cell represents a pixel or colour. Ther are several problem in image processing that can be solved using BFS , DFS ,connected components or shortest path algorithms running on pixel matrix graph. Colour filling and sean carving  are good examples. I have included  one small game application ,"Guess who", in the demo section, which uses DFS and BFS to remove forground pixel to see the picture underneath. 
Below code snippet is an example usage of character-matrix graph.It demostrate how we can replace one set of conneted character vertices with another character vertices.
```
using CGVertex=typename CrossMatrix<char>::VertexType;
CrossMatrix<char> cg({
                   'a','b','a','a',
                   'b','b','b','a',
                   'a','b','a','a',
                   'b','b','a','a',
                 });
cg.setPredicate([](char c){return c=='a';});

std::map<CGVertex,CGVertex> vertexParents;
travers(cg,CGVertex{3,3},GDFS(),[&](auto p,auto c){
    vertexParents[c]=p;
    std::cout<< cg.to_string(p)<<cg.to_string(c)<<"\n";
},[&](auto v){
    cg.setValue(v,'b');
});
printparents(vertexParents,CGVertex{0,2},CGVertex{0,0},[&](auto& v){return cg.to_string(v);});
cg.printContent(std::cout);
```
The noted addition in above code is the setPredicate call . This is an important call for the graph traversal to determine the presence of an edge. Basically this function set the relation between among neighbourhood vertices. In our purticular case it is saying that two negihboring cells are connected if both have 'a' as it's content.
The traverse call looks a bit differnt in this example. Here we are doing two thing during traversal.
1) capturing the parent child relashonship among vertices.
2) replacing the content of each visited cell with 'b' . So, after the traversal all cell contents that connected to {3,3} will change its value from 'a' to 'b'

 Note the Vertex type usage. It is the index representing the cell position in the matrix. I have used the point number 2 in one of my demo app to change the pixel tansparency.You can see all GUI examples in example folder.
 
 <B> Minimum Spanning Trees </B>
 
 So far we have covered generic unweighted graph data structures and basic travesal algorithms. We have a lot more to do in order to cover all graph processing models. I will keep on adding new algorithms and data structure to the library . For now lets talk about one of the completed feature Minimum Spanning Tree( MST )
As we know in MST model we are trying to figure out the minimum cost edges that are connecting the graphs. In this model we need another kind of graph, called EdgeWeightedGraph, that can hold the weight or cost of taking that edge. In case of Unweighted graph we are only interested in vertices that are connected to a vertex. Since we need to worry about cost of the edge, we need new type Edge . The edge can be either directed or undirected. In case of EdgeWeightedGraph we  store edges instead of vertices in the adjuscency list of a vertex. As usual the vertex type can be any user defined type,depending on your buisiness needs.

Our EdgeWeightedGraph is parameterised by Vertex type and a bool, that tells if it is directed or undirected.
The following code gives a type alias for a directed EdgeWeightedGraph.
```
template <typename T>
using DEdgeWeightedGraph=EdgeWeightedGraph<T,true>;
```
Though minimum spanning tree does not need directed graph , I simply mentioned about it here just to indroduce a new EdgeWeightedGraph. The DEdgeWeightedGraph will be useful when we talk about shortestpath algorithm.
The following code demonstrate the usage of MST in the context of finding least intimate relashionship in our family tree.
```
auto run=[](auto& g){
    std::cout<<"from Kruskal\n";
    m_s_t(m_s_t_kru(),g,[](const auto& e){
       std::cout<<"edge "<<e.either()<<" "<<e.other(e.either())<<"\n";
    });
    std::cout<<"from Primes\n";
    m_s_t(m_s_t_pri(),g,[](const auto& e){
       std::cout<<"edge "<<e.either()<<" "<<e.other(e.either())<<"\n";
    });
};

EdgeWeightedGraph<std::string> sg;
using SEdge=EdgeWeightedGraph<std::string>::Edge;
sg.addEdge(SEdge("Abhilash","Abhila",10)).addEdge(SEdge("Pranav","Shreya",10)).addEdge(SEdge("Abhila","Shreya",5));
sg.addEdge(SEdge("Abhilash","Pranav",7)).addEdge(SEdge("Kamalamma","Abhilash",8)).addEdge(SEdge("Abhila","Pranav",3));
run(sg);
 ```
 In this example we are caculating MST for a family tree using two well know algorithm Prime's and Kruskal's. You can see the usage of MST algorithms in the run method. The Algorithm accepts graph and a callback . The callback will be triggered whenever algorithm picks a vertex to be added to the tree.
 
 <B>Single Source Shortest Path </B>
 
 
  The shortest path is another important algorithm that has wide range of applications, from finding the shortest route from a map to reducing size of an image by removing low entropy pixels(Sean Carving>. The graph container we uses here again is an edgeweighted graph but now we have direction to care about. The following code demostrates  how to use shortest path algorithm. 

```   
    DEdgeWeightedGraph<std::string> sg;
    using SEdge=DEdgeWeightedGraph<std::string>::value_type;
    sg.addEdge(SEdge("Abhilash","Abhila",1)).addEdge(SEdge("Pranav","Shreya",10)).addEdge(SEdge("Abhila","Shreya",5))
    sg.addEdge(SEdge("Abhilash","Pranav",7)).addEdge(SEdge("Kamalamma","Abhilash",8)).addEdge(SEdge("Abhila","Pranav",3));
    sg.addEdge(SEdge("Pranav","Abhilash",10)).addEdge(SEdge("Pranav","Kamalamma",1));
    s_s_shortest_path s_p;
    using  VertexType = std::string;
    std::map<VertexType,VertexType> paths;
    std::map<VertexType,int> distances;
    s_p(g,"Abhila",[&](const auto& v,const auto& distance, const auto& parent){
        paths[v]=parent;distances[v]=distance;
    });
    for(auto& p:paths){
        std::cout<<"Vertex "<<p.first;
        auto via=p.first;
        while(via!=paths[via]){
            via=paths[via];
            std::cout<<" via "<<via;
        }
        std::cout<<" with cost "<<distances[p.first]<<"\n";
    }
```

The example demonstrates the SP algorithm running over our family tree to find out shotest relationship one person has with the rest of the family members. 
The s_p algorithm takes a graph, a source vertex and a callback. The callback will be called once the algorithm finds out the shortest distance between the source vertex and the current processing vertex. 

