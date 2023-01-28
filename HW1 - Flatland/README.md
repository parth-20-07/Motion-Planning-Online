**Homework 1**

**Table of Contents**

<!-- TOC -->

- [Breadth First Search](#breadth-first-search)
    - [Pseudocode](#pseudocode)
- [References](#references)

<!-- /TOC -->

# Breadth First Search

## Pseudocode

```c++
BFS (G, s) //Where G is the graph and s is the source node
    let Q be queue.
    Q.enqueue(s) //Inserting s in queue until all its neighbour vertices are marked.
    
    mark s as visited.
    while (Q is not empty)
        v = Q.dequeue( ) //Removing that vertex from queue,whose neighbour will be visited now

        for all neighbours w of v in Graph G //processing all the neighbours of v
            if w is not visited
                Q.enqueue( w )             //Stores w in Q to further visit its neighbour
                mark w as visited.
```

# References
- [Breadth First Search](https://www.hackerearth.com/practice/algorithms/graphs/breadth-first-search/tutorial/)