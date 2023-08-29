import random
from trie import Trie


words = ['a','ab','abc','abcd','abcde','abcde12345','abc']
t = Trie()
for w in words:
    t.insert(w)
    assert t.query(w)
assert t.query('bbb') is False
assert t.query('aaa') is False
assert t.query('123') is False
assert t.query('') is False
assert t.lcp() == 'a'
assert t.node_size == 10
assert t.word_size == 6

t.insert('cs.pynote.net')
words.append('cs.pynote.net')
t.insert('123456')
words.append('123456')
t.insert('happy coding')
words.append('happy coding')
assert len(t.lcp()) == 0
assert t.word_size == 9


words.remove('abc')
assert sorted(t.startswith('')) == sorted(words)
words2 = words[:]
random.shuffle(words2)
for w in words2:
    t.remove(w)
    words.remove(w)
    assert sorted(t.startswith('')) == sorted(words)
assert t.node_size == 0
assert t.word_size == 0


print('Test OK!')

