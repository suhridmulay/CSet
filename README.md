# CSET
Implementation of sets.
Implement sets using *hash table*, consider a *set of strings*

## Implementation of sets
### Basic details
1. is_element_of(x,S): checks whether the value x is in the set S.
2. is_empty(S): checks whether the set S is empty.
3. size(S) or cardinality(S): returns the number of elements in S.
4. enumerate(S): returns a list containing the elements of S in some arbitrary order.
5. build(x1,x2,…,xn,): creates a set structure with values x1,x2,…,xn.
### Dynamic sets
1. create(): creates a new, initially empty set structure.
2. add(S,x): adds the element x to S, if it is not present already.
3. remove(S, x): removes the element x from S, if it is present.
### Set operation
1. union(S,T): returns the union of sets S and T.
2. intersection(S,T): returns the intersection of sets S and T.
3. difference(S,T): returns the difference of sets S and T.
4. subset(S,T): a predicate that tests whether the set S is a subset of set T.4
