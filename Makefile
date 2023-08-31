

.PHONY: testcpp testpy


.RECIPEPREFIX = >
testcpp: trie.cpp test_trie.cpp
> g++ -Wall -Wextra -O3 $^ -o $@
> ./$@


testpy: test_trie.py
> python $<

