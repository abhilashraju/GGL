# GGL-
Generic Graph Library , A library of generic graph data structures and algorithms

![Traversals](https://github.com/abhilashraju/GGL-/blob/main/demo/Graph%202021-07-20%2022-41-01.gif) ![MST](https://github.com/abhilashraju/GGL-/blob/main/demo/MST%202021-07-20%2022-44-32.gif)
![dapp](https://github.com/abhilashraju/GGL-/blob/main/demo/Graph%202021-07-21%2011-17-34.gif) ![shrtp](https://github.com/abhilashraju/GGL-/blob/main/demo/shrt_path.gif)

Generic and reusable components to solve common graph problems. The implementation follows STL philosophy that separates data structure and alogorithms and connect them together through well defined concepts. So the library is 
1) A collection of reusable generic alogorthms that can work with any data type that are compatible with the concepts the algorithm expects
2) A collection of generic graph containers that reresents different graph models which can be parameterised based on user defined vertex type

This seperation of graph model from its algorithm enables us to achieve multiplicative reusability. Which means you can apply several graph algorithms to a single graph model and vice versa

Examples
https://gist.github.com/abhilashraju/ff9097c7f9fa2788b20803a160c20bbe

