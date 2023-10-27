

.RECIPEPREFIX = >
.PHONY: all testcpp testpy


all: testcpp testpy


testcpp: trie.cpp test_trie.cpp
> g++ -Wall -Wextra -O3 -fsanitize=address $^ -o $@
> ./$@


testpy: test_trie.py
> python $<

