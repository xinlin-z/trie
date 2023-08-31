

.RECIPEPREFIX = >
.PHONY: all testcpp testpy


all: testcpp testpy


testcpp: trie.cpp test_trie.cpp
> g++ -Wall -Wextra -O3 $^ -o $@
> ./$@


testpy: test_trie.py
> python $<

