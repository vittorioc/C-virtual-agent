
    This is a network simulator, where some agents communicate each others using numeric tuples.
    
    Not every agent see the others, so some links are up and some others are down, all this is specified in the adjacency_matrix.txt (http://en.wikipedia.org/wiki/Adjacency_matrix)
    
    All is implemented using C and multithreading.
    
    Ubuntu prerequisites:
    
    $ sudo apt-get install libcunit1-doc libcunit1-ncurses libcunit1-ncurses-dev libgsl0-* libplplot-* libgraphviz-dev libblas* gsl-* multitail
    
    Run:
    
    $ make all

	$ sudo -i

	# ./test_lab