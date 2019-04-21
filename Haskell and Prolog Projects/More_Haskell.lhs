
As lambda calculus is such a major part of the Haskell language i decided to implement the first method with an annonymous inner class lambda 
instead of defining a separate class which I could call during the fold. The class can be broken when the elements of the list are of a different 
size, but i decided that this was not a fix i needed to attempt to fix as it related to the way the fold takes the argument I used a left fold
for this argument to ensure that the result would be in the correct order, as attempting to implement with a foldr would give me incorrect results.

\begin{code}
convert:: (Foldable t ,Integral a) => t a -> a
convert lis = foldl (\x y -> x*10 + y) 0 lis
\end{code}

For the tests for convert i wanted to show the cases where it works (list containing a single item, list 
taking descending numbers of values [i.e. 100, 10, 1] but not [1, 10, 100])

\begin{code}
convertT1 = convert [1, 2, 3] == 123
convertT2 = convert [123] == 123
convertT3 = convert [12,10] == 1210
convertT4 = convert [1,10,100] == 110100
convertTests = [convertT1, convertT2, convertT3, convertT4]
\end{code}


In delete all i decided to implement a separate inner function instead of nesting a lambda as when i attempted to use a lambda function 
i had a variety of issues that i found it easier to fix by simply changing to the internal function. It also gave me the opportunity to
work on implementing inner functions as i did not use these often in the first assignment. The most common error i had while implementing
this method was the problem that kept being thrown up from attempting to return a list of values while my first attempts either only 
compile if i returned a single value, or if i had the last line above the rest of the code it would return an incorrect list, either empty or
all values not changed. By defining the internal class i managed to bypass the issues from the lambda (single value coming back)
\begin{code}
deleteAll :: (Eq a) => a -> [a] -> [a]
deleteAll value lis = foldr myDel [] lis
                    where 
					myDel x (y:ys)  | x == value = (y:ys)
   				                    | otherwise = (x:y:ys)
					myDel x []= [x] 
\end{code}

For the deleteAll tests i wanted to show that the lists can take different types, and that they will return an unchanged list if the value is not 
present but a changed but in the same order test if it is					

\begin{code}
deleteAllT1 = deleteAll 1 [1,1,1,0] == [0]
deleteAllT2 = deleteAll 2 [0,2,3,2,1] == [0,3,1]
deleteAllT3 = deleteAll 'q' "Hello world" == "Hello world"
deleteAllT4 = deleteAll 'q' "queen" == "ueen"

deleteAllTests = [deleteAllT1, deleteAllT2, deleteAllT3, deleteAllT4]
			
\end{code}
			
With sort i instinctively felt that the insertion sort would be far easier to implement than another of other sorts, such as quick sort or merge. 
By using the insertion sort i could easily take advantage of the way in which arguments were passed to the function from the main fold, and i prefer 
the right fold to the left (for some reason) so i decided to implement my insertion sort with a fold r instead of a fold l. The internal class taking 
care of the logic made it simpler for me to debug, and when i had it running through multiple different lists the performance of the algorithm (in that 
it has the cost of O(n2)) did not impact my program due to the simplistic way i used the method (i.e. on my lists of integers) When i attempted to 
implement this sort with a lambda it threw up a number of errors, which i slowly debugged my way through before deciding, as with the above method, 
that it was far simpler to just define a smaller inner function that i could control the behaviour of.

\begin{code}
sort :: Ord a => [a] -> [a]
sort list = foldr myInsertSort [] list
                where
                myInsertSort val []     = [val]
                myInsertSort val (x:xs) | val < x     = val:x:xs
                                | otherwise = x:(myInsertSort val xs)
\end{code}
The sort tests i used were just small little functions which checked that the list would put values of equal value next to each other in the 
sorted list, that it would sort different data types and an already sorted list would return the same as it was passed

\begin{code}
sortT1 = sort ['a','c','b'] == ['a', 'b', 'c']
sortT2 = sort [1,2,3] == [1,2,3]
sortT3 = sort [4, 3, 8, 10, 2, 1, 0, 9, 5, 5, 6, 7] == [0,1,2,3,4,5,5,6,7,8,9,10]								
sortTests = [sortT1, sortT2, sortT3]
\end{code}

For my insertOrd method i admit that i used a small work around. Instead of defining an entirely new function which would take the list and insert 
the value instead i decided to addend the value to the end of the passed list and then pass that new combined list argument to my sort as defined above.
I decided to do this as my sort method used was the insertion sort, so all the logic of it would work on the list passed to insertOrd, and the performance
of the list wouldn't be greatly changed due to all values barring one already being in order. The use of the foldr in the above method i felt met the 
criteria that this method must use a fold as well. This method would not work on unordered lists, as it would change the order of the elements, 
but without some kind of inherent ordering the insert would fail in concept as it is difficult to compare values without a natural ordering.

\begin{code}
insertOrd :: Ord a => a -> [a] -> [a]
insertOrd x [] = [x]
insertOrd x y  = sort (x:y)
\end{code}

For my insertOrd tests i just wanted to show that the data type doesn't matter, and that the list will include duplicates

\begin{code}
insertOrdT1 = insertOrd 1 [2,3,4] == [1,2,3,4]
insertOrdT2 = insertOrd 'i' ['a','e','o','u'] == ['a','e','i','o','u']
insertOrdT3 = insertOrd 6 [2,3,4,5,6,7,8] == [2,3,4,5,6,6,7,8] 
insertOrdTests =[insertOrdT1, insertOrdT2, insertOrdT3]
\end{code}


For my all positions code i decided to pair up the filter and map as this allowed me to bypass the issues i found when trying to create a lambda
that did not throw an alternative message (ie. i could not just do an if then statement without throwing an error). By zipping the list into a tuple
i managed to make my code simpler and more concise in the lambda used with the map, although it did require me creating the zipped list in another,
small helper function in order for me to bypass the errors being thrown when i attempted to implement it in line in the map call. By returning the -1
if there was not a match i also made it easy to filter the final option and gave myself a clean way of returning from the map even when the value wasn't
the one matching the passed value

\begin{code}
allPositions v l = allPositions' v (indexify l)
allPositions' v l = filter (\ x -> x >= 0) (map (\(x,i) -> if x == v then i else -1) l )
indexify lis = zip lis [0..]
\end{code}

The tests are just simple, quick tests that i knocked up to show the core idea of it working:
\begin{code}
allPosT1 = allPositions 1 [1,2,2,1,2,1,2,1,2] == [0,3,5,7]
allPosT2 = allPositions 'q' "Hello world" == []
allPosT3 = allPositions 4 [1,2,3,4] == [3]
\end{code}


For the lcm' method I decided to use the infinite list comprehensions. Most of this was to give myself experience in using the infinite lists, in 
understanding how they work and how the values would be passed back, as well as how to compare two criteria for a single value which was pulled out 
of the list. Using the infinite list was easy I could take advantage of the inherent lazy nature of the Haskell language. If i was to implement this 
in a way that the list was not accessed through lazy evaluation i would require another parameter to the function which would dictate the number of 
values which the list would process then look through to compare if it was a lcm of the two passed values. Implementing this method with an explicit 
recursion would be far more difficult in my opinion, as comparing and tracking the values would be a messy procedure. I also considered an option where 
i constructed two infinite lists, one of each multiple (i.e. 4 = [4,8,...] 6 = [6,12,...]) in a separate method which i then passed back to the main 
compare method, but that was eliminated when i chose to use the list comprehension instead. I also used head as it seemed to me that it was the simplest 
way to ensure i didn't enter an infinite loop, and that i managed to get the same value. Starting the list at one also removed any need to check that the 
value was not 0 as 0 is not considered an lcm of two numbers. it does mean that this method would not work for negative numbers and their lcms, but as this 
was not a criteria in the handout i did not implement it. This would be an easy fix though, if the values were simply starting at -1 -> -infinity

\begin{code}
lcm' :: Integral a => a -> a -> a
lcm' x y =  head([v | v <- [1..], v `mod` x == 0, v `mod` y ==0 ])
\end{code}

Below are just simple tests to prove it works
\begin{code}
lcm'T1 = lcm'  4 6 == 12
lcm'T2 = lcm' 10 20 == 20
lcm'T3 = lcm' 432 19 == 8208
lcm'Tests = [lcm'T1, lcm'T2, lcm'T3]
\end{code}

For my ascending code i decided to split up the work over a few helper functions so that i could check through them as i coded and ensure that i understood 
any errors being thrown. i decided to make the list into pairs of tuples so that i would be able to directly see what the list was doing, and also so that 
i could make comparisons without having to worry about any potential booleans returned by a fold. I used a map instead of a fold as i found it to be nice and 
straight forward, easy to compare the elements and store the booleans without any issues, and all didn't throw up any protests about how i had passed elements 
to the function when i swapped from the map to the fold. Time constraints stopped me from attempting to implement a fold as well, though i can logically 
see how what i have created could be extended to a fold

\begin{code}
ascending:: Ord a => [a] -> Bool
ascending [] = True
ascending lis = ascending' (process lis )

ascending' :: Ord a => [(a,a)] -> Bool
ascending' lis = all (==True ) (map (\(x, y) -> if x < y then True else False) lis)

process (x:y:ys)= (x,y):(process(y:ys)) 
process (x:[]) = []
process [] =[]
\end{code}

My ascending tests were just simple tests to show that it works:
\begin{code}
ascT1 = ascending [True, True] == False 
ascT2 = ascending [1,2,3,4,5] == True
ascT3 = ascending [1,2,2,4] == False
\end{code}

--permutations
My permutations code is flawed. it works well for the case of one element, two or three but when it has four elements instead of following the recursive pattern 
that i intended, it simply exits out of the loop and returns the first two elements swapped i.e. with 4 it will return [1,2,3,4] [1,2,4,3] but does not move into
execution of swapping the 2 with the other values. I attempted to fix this with the blurred out line of code but this simply returned duplicates of every single
value which i could not figure out how to remove from the 
\begin{code}
perms:: (Num a, Ord a) =>[a] -> [[a]]
perms [] = [[]]
perms lis = permy lis (length (lis)) 0 [[]]

permy:: (Num a, Eq a, Ord a) => [a] -> Int -> Int -> [[a]]  -> [[a]]
permy [x,y] _ _ _ = [[x,y], [y,x]]
permy (x:xs) n cNV permed | cNV == 0 = permy (xs ++[x]) n (cNV + 1) (perms' (x:xs) n 0)
                          | cNV < n = permy (xs ++[x]) n (cNV + 1) (permed ++ (perms' (x:xs) n 0))
                          | otherwise = permed 

perms':: (Eq a, Ord a, Num a) => [a] -> Int -> Int ->[[a]]
perms' [x,y] _  _= [[x,y] ,[y,x]]
perms' (x:xs) n cV | cV < n = map (x:) (perms' xs n (cV+1)) 
                   | otherwise = perms' (xs ++ [x]) n 0
--perms' (x: y: xs) n cV | cV < n = (map (x:) (perms' (y:xs) n (cV+1)))   ++ ( map (y:) (perms' (x:xs) n (cV+1)))
\end{code}


------------------ Part Two -------------------------------
\begin{code}
data BinTree a = Empty | Node a (BinTree a) (BinTree a) deriving (Show)

empty::BinTree a
empty = Empty
\end{code}

For my binary tree operation 'has' i followed the simple principle of if the value was lower than my current going to the l /r child etc. 
This seemed to be the quickest, most intuitive way i found to implement the method. It also ensures i will not be stuck in an infinite loop 
where i checked for children that weren't contained in the tree, and that i did not need to track a boolean operator for each child then 
compare any output to see for a single True value
\begin{code}
has:: (Ord a) => a-> BinTree a -> Bool
has x Empty = False
has x (Node y l r) | x == y  =  True 
				   | x < y   =  has x l 
				   | x > y   =  has x r
				   
\end{code}				 

For my has tests i wanted to check the basic cases -> the tree is empty, the value is contained and the value is not contained. 
The tests will falsely evaluate to false in the case that the tree is not properly ordered as in the case of t4

\begin{code}
hasTest1 = has 2 Empty == False
hasTest2 = has 2 (Node 3 (Node 2 Empty Empty) (Node 4 Empty Empty)) == True
hasTest3 = has 2 (Node 3 (Node 2 Empty Empty) (Node 4 Empty Empty)) == False				   
hasTest4 = has 2 (Node 1 (Node 2 Empty Empty) (Node 3 Empty Empty)) == True
hasTests = [hasTest1, hasTest2, hasTest3, hasTest4]
\end{code}
				   
For the delete method i decided to implement a number of smaller, helper functions that would bring back the smallest value and recursively 
move down the tree to balance the values and adjust left and right children. I decided to break this up into multiple methods to ensure that 
the correct tree would be returned if the value did not exist (i.e. it would recurse until it hit an empty branch then simply return the children 
back up to the parent), and that if the value was to be deleted i could control teh method in which it happened. There is a certain amount of recursion 
to wrap my head around when deleting elements from the binary tree but the way i implemented the methods took care of most of that in my mind. To make 
the method (arguably) more efficient there could be a list of values stored where the values for the binary tree are taken and the delete function could 
return no change by comparing its x value against that list to see if the element is contained before recursing down the tree, but i did not implement 
a list in this way so explicit recursion was the most straightforward way to check

\begin{code}
delete :: Ord a => a-> BinTree a -> BinTree a				   
delete x Empty                 = Empty
delete x (Node y l r) | x == y = deleteValue x(Node y l r) 
                      | x < y  = Node y (delete x l) r 
                      | x > y  = Node y l (delete x r)
\end{code}


I used the delete value method in order to make my code clearer and more concise to me. it allowed me to eyeball the various ways in which the 
value and nodes on the tree could be stored, as well as making it easy to know what cases need to be passed to another function
to finish deleting a value from the tree. It uses the next method of lowestEle again to make it simpler, and because i prefer to break up my methods 
into smaller helpers to ensure i can understand what my code is doing when it throws errors

\begin{code}
deleteValue ::(Ord a) => a -> BinTree a -> BinTree a
deleteValue x (Node _ Empty Empty) = Empty
deleteValue x (Node _ Empty r)     = r
deleteValue x (Node _ l Empty)     = l
deleteValue x (Node _ l r)         = (Node (lowestEle l) l (delete (lowestEle l) r))
\end{code}

I used this simple method to recursively find the lowest element on the right side of the tree. The other option i had was to take the largest 
value on the left side of the subtree, but i decided to use the right side as this was the method i had the most experience with through
other subjects such as Comp 103. It only returns the element as i then pass the element back up to another method where i later deal with 
the duplicate node (same value but different children) created as i pass things through the tree


\begin{code}
lowestEle:: BinTree a -> a
lowestEle (Node x Empty _)  = x
lowestEle (Node x l _)      = lowestEle l
\end{code}

For the equals method i used the basic concept of testing every pair of nodes to check if they were equal. I call the method on the left child 
then the right because that can cut down on the amount of recursion involved with the tree as it will return the breaking value then pass 
down the right side. I found this an easier way than creating both booleans and comparing them to return the two children as a single value ie
(equals l1 l2) == equals (r1 r2), due to my personal prefence in this particular method. I used the _ when i checked against the empty list
due to the convention for doing this in Haskell when not needing to use the parameter for any comparison in the following comparison / assignment of variables


\begin{code}
equals :: (Eq a) => BinTree a -> BinTree a -> Bool
equals Empty Empty                                             = True
equals _ Empty                                                 = False 
equals Empty _                                                 = False
equals (Node x l1 r1) (Node y l2 r2) | x /= y                  = False  
                                     | equals l1 l2 == False   = False 
									 | otherwise               = equals r1 r2  

\end{code}

\begin{code}									 
equalsT1 = equals (Node 1 Empty Empty) (Node 1 Empty Empty) == True
equalsT2 = equals (Node 10 (Node 5 (Node 4 Empty Empty ) (Node 6 Empty Empty)) (Node 15 (Node 11 Empty Empty) (Node 17 Empty Empty))) (Node 10 (Node 5 (Node 4 Empty Empty ) (Node 6 Empty Empty)) (Node 15 (Node 11 Empty Empty) (Node 17 Empty Empty))) == True
equalsT3 = equals (Node 10 (Node 5 Empty Empty) (Node 15 Empty Empty)) (Node 10 (Node 4 Empty Empty) (Node 15 Empty Empty)) == False
equalsT4 = equals (Node 1 Empty Empty) Empty == False
\end{code}


For my insert method i did not attempt to ensure that the tree was balanced, instead simply inserting at whatever place the method deemed fit. 
By using the comparison i could easily control the way that the method moved down the tree, and it also provided the most simple implementation
for this method. Insert will ensure that it can create both a root node and child nodes, building the tree by applying each element of a list 
to the insert method. If i was to implement the balanced tree i would need to use the bubble down / up method within my insert or to call another
I also checked the value against the current node to ensure i can keep the set as duplicate free as possible without needing to call any other kind of function.

\begin{code}
insert:: (Eq a, Ord a) => a -> BinTree a -> BinTree a
insert x Empty                 = Node x Empty Empty
insert x (Node y l r) | x == y = Node y l r 
                      | x > y  = Node y (insert x l) r 
					  | x < y  = Node y l (insert x r)
\end{code}
					  
With the flatten method i decided to implement the flatten from left child to current to right (Pre order?) 
as this can keep the list which is returned sorted as long as the tree is the same. To change the list to the post order would be 
incredibly easy, just flipping the location of the values within the core function call. I did not see the need to throw any kind of 
error if passed an empty tree, instead returning an empty list in this case. Any of the general tree traversal patterns would be easy to 
implement in this style (could go parent-child-child). I used the concat ++ function instead of the (h:t) constructor as it produces fewer unexpected errors when compiling.

\begin{code}
flatten:: BinTree a -> [a]
flatten Empty = []
flatten (Node x Empty Empty) = [x]
flatten (Node x l r )        = (flatten l) ++ [x] ++ (flatten r)
\end{code}

------------------------- Part Three ---------------------------------
\begin{code}
type Path a b =[(a, b, a)]
type Graph a b = (Set a, Set (a, b, a))
type Set = []
\end{code}

For the predecessors and successors method i used the basic pattern matching style as it made it easier for my code to display my 
thoughts on the matter. I decided to attach the results of the method instead of passing them through to a prime method which would 
track the list of the predecessors found so far as this made me use the explicit recursion rather than tail end with a helper function 
which is my preference. For both the successsors and predecessor code i decided to implement the explicit recursion to ensure that i had 
more practise with the style and to also add more variety into my coding The similarity between the two methods made it easy to implement 
the second once the first had been logicked, hence the very small differences between my functions

\begin{code}
predecessors :: Eq a => Graph a b -> a -> Set a
predecessors (v, []) val                        = []
predecessors (v, ((v1, e, v2):rst)) val        | v2 == val = v1:predecessors(v, rst) val
											   | otherwise = predecessors (v, rst) val

successors :: Eq a => Graph a b -> a -> Set a
successors (v, []) val                         = []
successors (v, ((v1, e, v2):rst)) val          | v1 == val = v2:successors(v, rst) val
											   | otherwise = successors (v, rst) val

\end{code}

For makeGraph i ended up creating a number of smaller functions which i could pass the information to. I ended up only using the makeGraph 
to detect any errors as this ensured i would not need to check for any other issues while i was calling the following four functions. Actually 
implementing the graph itself could be taken care of in this method but instead i just passed the information on to avoid a thousand 
(not quite) different guards and pattern matches as i could export those to the relevant functions. i also took advantage of lazy evaluation 
in Haskell to call the addVerts twice, knowing that the second call would already be evaluated and would not need to recreate the list of verts

\begin{code}
makeGraph :: (Eq a, Eq b) =>[a] -> [(a, b, a)] -> Graph a b											   
makeGraph [][]          = error "Cannot make a graph from empty lists"
makeGraph [] _          = error "Need vertices to make graph"
makeGraph _ []          = error "Need edges to make a graph"
makeGraph vertexs edges = (addVerts vertexs [], addEdges edges (addVerts vertexs []) [])
\end{code}

I created similar methods for add verts and add edges which checked their error reports and folllow the same sort of pattern. I used the 
error reports to exit out if there were any problems so that i would not create a mismatched graph, and again called a few simple function 
methods to avoid overcoding and running into a number of little errors which could add up to confusion at compilation or testing time. 
I also tested the methods on their own as well as part of makeGraph to ensure that they would be able to work on their own as well as 
completing their desired outcome before combining them for their actual use in makeGraph 

\begin{code}
addVerts :: Eq a => [a] -> [a] -> [a]
addVerts [] gV      = gV
addVerts (v:vs) gV | containVertex v gV == True = error "Vertex is already contained in the graph"
				   | otherwise                  =  addVerts vs (gV ++ [v]) 

addEdges :: (Eq a, Eq b) =>  [(a, b, a)] -> [a] -> [(a, b, a)] -> [(a, b, a)]				   
addEdges [] [] [] = []
addEdges [] gV gE = gE
addEdges ((v1, e, v2):grph) gV gE | containEdge (v1, e, v2) gE == True                            = error "Edge already contained in the graph" 
							      | containVertex v1 gV == False || containVertex v2 gV == False  = error "Unknown vertex in the passed edge" 
							      | otherwise                                                     = addEdges grph gV ((v1, e, v2):gE)
\end{code}

Again i created the twin methods of containVertex and containEdge, reusing much of the concept inside the methods, to simplify the 
rest of the code. It was the quickest and most intuitive way i found to check if the values had already been contained, and i pulled 
the methods out again to check that they could work on their own or as they were expected to within the much wider makeGraph spiderweb 
of functions. they were just there to ensure that i upheld the criteria laid out in the assignment hand out and that i did not 
accidentally mess up and duplicate values, while making sense on their own and ensuring they did not throw
potentially confusing error messages as part of the wider group of functions in part 3								  

\begin{code}
containVertex :: Eq a => a -> [a] -> Bool
containVertex v []                   = False
containVertex v (v1:gph)             | v == v1   = True 
                                     | otherwise = containVertex v gph
									 
containEdge :: (Eq a, Eq b) => (a, b, a) -> [(a, b, a)] -> Bool
containEdge _ []                               = False
containEdge (v1, e, v2) ((gv1, gve, gv2):gEs) | v1 == gv1 && gve == e && gv2 == v2  = True
											  | otherwise                           = containEdge (v1, e, v2) gEs  
\end{code}

The code i wrote for the findPath is inherently similar to the code i wrote for isConnected, changing to return a probable path instead of a boolean.
I implemented the code without many higher order functions as i found this to be an easier, if messy and more hands on, way to code the function.
This turned out to be the third version of the code that i wrote, the other two variations that threw up errors i could not process so i took the
core idea of my previous drafts and simply reworked it as i started to understand how the Maybe and Nothing | Just could be used in coding the 
function. 
\begin{code}
findPath :: (Eq a, Eq b) => Graph a b -> a -> a -> Maybe (Path a b)											  

findPath ([], _) u v = error "cannot find a path with empty vertices"
findPath (_, []) u v = error "cannot find a path with empty edges"
findPath (verts, edges) u v   | containVertex u verts == False = error "The start vertex was not contained in the graph"
                              | containVertex v verts == False = error "The end vertex was not contained in the graph"
							  | otherwise = findPath' (edges) u v [] [] []
							  
findPath' :: (Eq a, Eq b) => [(a, b, a)] -> a -> a -> [(a,b,a)] -> [(a,b,a)] -> [a] -> Maybe (Path a b)							  
findPath' ((v1, e, v2):rst) u v visitedEdges pathSoFar possibleVerts | (v1, e, v2) `elem` visitedEdges = Nothing 
																     | v1 == u && v2 == v = Just [(v1, e, v2)]
                                                                     | v1 `elem` possibleVerts && v2 == v =  Just (pathSoFar ++ [(v1, e, v2)])
																	 | v1 `elem` possibleVerts = findPath' rst u v ((v1, e, v2):visitedEdges) (pathSoFar ++ [(v1,e,v2)]) (v2: possibleVerts) 
																	 | otherwise = findPath' rst u v visitedEdges pathSoFar possibleVerts
\end{code}

For isConnected i followed a very similar core idea to the code that i wrote for findPath. I decided to now throw an error when passed a partially
empty map, instead just returning false and keeping my error for when the vertex was not contained in the graph. Apart from that i simply followed
the most intiutive way that i felt the code would be written, eschewing high order functions which i perhaps would like to work into the assignment
if i had given myself more time to work on this particular part of the assignment. I used two helper functions as i wanted to handle cases with an
empty list of vertices that would previously throw a false but in the core of the function would simply allow me to ensure i had checked there was 
a path to all the other vertexs and return true because of it

\begin{code}
isConnected:: (Eq a, Eq b) => Graph a b -> a -> Bool
isConnected ([], _) _ = False
isConnected (_, []) _ = False
isConnected (verts, edges) val | containVertex val verts == False = error "The vertex supplied is not contained in the graph"  
                               | otherwise                        = isConn (verts, edges) val

isConn:: (Eq a, Eq b) => Graph a b -> a -> Bool
isConn ([], _) val                                                      = True							   
isConn ((v:vs),edges) val     | val == v = isConn (vs, edges) val
							  | isConnected' edges val v [] [] == False = False
							  | otherwise                               = isConn (vs, edges) val

isConnected' :: (Eq a, Eq b) => [(a,b,a)] -> a -> a -> [(a,b,a)] -> [a] -> Bool
isConnected' ((v1, e, v2):rst) mainVertex toVertex visitedSoFar potentialPaths 
                                                                |  v1 == mainVertex && v2 == toVertex         = True
                                                                | (v1, e, v2) `elem` visitedSoFar             = False
																|  v1 == mainVertex                           = isConnected' rst mainVertex toVertex ((v1, e, v2):visitedSoFar) (v2:potentialPaths) 
																|  v1 `elem` potentialPaths && v2 == toVertex = True
																|  v1 `elem` potentialPaths                   = isConnected' rst mainVertex toVertex ((v1, e, v2):visitedSoFar) (v2:potentialPaths)
																|  otherwise                                  = isConnected' rst mainVertex toVertex visitedSoFar potentialPaths
\end{code}
---------------------------------------------Part Four -----------------------------
I decided to create the WFF expressions in two separate data types mostly because that was what was implemented in the lecture slides and online so i found it easiest to logic out in my mind and figure out what was supposed to go on in the data types themselves
I created the WFFOp to store the actual logic of the WFFs, and the WFFval to be evaluated more simply. Due to the different number of operands in things such as And / Not i decided to implement the OneArg / TwoArg system, ensuring i did not need to create a dummy 
variable assignment which did nothing but allowed me to evaluate not in the same way as the other WFFs. 
My data types represent the propositional WFFs as they take the symbol / common name of the WFF and translate that to the WFFOp, while the WFFval contains the calls to the eval and the evaluation of the expression itself. When i call the WFFval i decided to drop the 
character from the front of the pairing ('p', True) etc, as this was not useful in the rest of the data type and including it changed the parameters i would need to look at in the apply function (i.e. apply _ (_, x) instead of apply _ x) 

\begin{code}
data WFFOp = Not | Or | And | Implies | Equivalence

data WFFval = Var (Char, Bool) 
           | OneArg WFFOp WFFval
		   | TwoArg WFFOp WFFval WFFval 


eval:: WFFval -> Bool
eval (Var (x, y)) = y
eval (OneArg nt x) = not (eval x)
eval (TwoArg op x y) = apply op (eval x) (eval y)

apply:: WFFOp -> Bool -> Bool -> Bool
apply And x y |x == y && x == True = True | otherwise = False
apply Or x y| x == True = True | y == True = True | otherwise = False
apply Implies x y | x == True && y == False = False | otherwise = True
apply Equivalence x y | x == y = True | otherwise = False

\end{code}