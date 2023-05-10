// Pegar codigo 

// I am creating a lot of auxiliar Nodes* and maybe I can just do a global one?
// I keep passing vector through the elements function, but this is not optimized
// Pass vectors by reference

// Custom Sort Function

// Using a lot of memory so we can do the O(1) insertions and deletions

// Do we create a new Set Trie every time or do we create an erase operation ? 

// Order of operations

// Design Decision: We are maintaining a few global structures to avoid memory redundancy
// We need to be able to reset these structures properly before using each one
// We could also test always going through every vertex and edge instead of getting vectors with active elements

// We could create a library for instance that has access to everything and maintains all the data structures there

// A lot of the structures that we are maintaining globally are only used in one lib, we could put it there

// I think the problem is the upperBound function returns 0 in some cases (when all vertices are ignored ?)
// Or maybe I am not rollbacking right