# GGL-
Generic Graph Library , A library of generic graph data structures and algorithms

![Traversals](https://github.com/abhilashraju/GGL-/blob/main/demo/Graph%202021-07-20%2022-41-01.gif) ![MST](https://github.com/abhilashraju/GGL-/blob/main/demo/MST%202021-07-20%2022-44-32.gif)
![dapp](https://github.com/abhilashraju/GGL-/blob/main/demo/Graph%202021-07-21%2011-17-34.gif) ![shrtp](https://github.com/abhilashraju/GGL-/blob/main/demo/shrt_path.gif)

Generic and reusable components to solve common graph problems. The implementation follows STL philosophy that separates data structure and alogorithms and connect them together through well defined concepts. So the library is 
1) A collection of reusable generic alogorthms that can work with any data type that are compatible with the concepts the algorithm expects
2) A collection of generic graph containers that reresents different graph models which can be parameterised based on user defined vertex type

This seperation of graph model from its algorithm enables us to achieve multiplicative reusability. Which means you can apply several graph algorithms to a single graph model and vice versa

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
Again it is just a parmeterised version of another generic algorithm. The only difference between DFS, and BFS is in its usage of stack and queue . Then why not parameterise it?
```
using GBFS=BFS_DFS<make_bfs_push>;
using GDFS=BFS_DFS<make_dfs_push>;
```
See the type alias declaration . The main difference is in its pushing strategy. One pushes new vertices in front while the other put it in back.

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
Once again the Vertex can be any user defined types. Generally this kind of graph comes in image processing use case ,where each vetices represents a pixel or colour. Ther are several problem in image processing that can be solved using BFS , DFS ,connected components or shortest path algorithms running on matrix graph. Colour filling , sean carving etc are examples of that. I have included  one small game application "Guess who" in the demo section, which use DFS and BFS to remove forground pixel to see the picture underneath. 
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
The noted aspect in above code is that the setPredicate call . This is an important call for the graph traversal to determine the presence of an edge. Basically this function set the relation between negihbouring vertices. In our purticular case it is saying that if the vertex content is 'a' then it  represents an edge.


