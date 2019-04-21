Note about program structure:
All the functions are implemented first, then the tests are all listed afterwards. The tests take the general format of T Question Number Question Letter then number ie HasKey = t3HK1 or positives = t1a1

\begin{code}
import Data.Char
\end{code}

basic code, checking all values are above 0 if so adding them to the return list.
\begin{code}
positives:: (Ord a, Num a) => [a] -> [a]
positives [] = []
positives (x:xs) | x > 0 = (x:positives xs) | otherwise = positives xs
\end{code}

basic oddElts idea, ditch every second element without doing anything to it, adding only the odd elements. Could easily be swapped for every second element if necessary
\begin{code}
oddElts :: [a] -> [a]
oddElts [] = []
oddElts (x:y:xs) = x:oddElts xs
oddElts x = x
\end{code}

compares two elements, returns the list easily so there's no complications. Only works for the ascending sorted list, otherwise will throw up duplicates. 
\begin{code}
union:: Ord a => [a] -> [a] -> [a]
union x [] = x
union [] y = y
union (x:xs) (y:ys) | x == y = (x:(union xs ys)) | x < y = (x:(union xs(y:ys)))| x > y = (y:(union(x:xs)ys))
\end{code}

QUESTION 1D = Couldn't figure out how to get the Occurs Once to work, personally would have implemented it as tail recursive with a helper function tracking a boolean that was passed through to see if it had occured in the past. 

Nothing much to say, basic idea of a prime number. Put in the catch for ==1 or error for negatives to avoid the complication of attempting to divide negative numbers by negative (-2/-2) etc, and having a different function for positives
\begin{code}
isPrime:: Int -> Bool
isPrime num = isPrime' num 2

isPrime':: Int -> Int -> Bool
isPrime' num val | num == 1 = True | num < 1 = error "Only allowed to use positive numbers" | num == val = True | num `mod` val == 0 = False | otherwise = isPrime' num (val+1)
\end{code}

   
I implemented my own version of length as we could not call the predefined one, naming it slightly different to avoid confusion. The length of the string was calculated in another function for simplicity    
\begin{code}
longest:: [[Char]] -> [Char]
longest x = longest' x ""

longest' :: [[Char]] -> [Char] -> [Char]
longest' [] str = str
longest' (x:xs) str | myLength(x) > myLength(str) = longest' xs x |otherwise = longest' xs str

myLength :: [Char] -> Int
myLength [] = 0
myLength (x:xs) = 1 + myLength(xs)
\end{code}

Passing the position as a parameter into the helper function so i know exactly where i am in the map, will return an empty list if it isn't contained as the position is only added when needed. 
\begin{code}
allPositions :: (Eq a, Num b) => [a] -> a -> [b]
allPositions x val = allPositions' x val 0

allPositions' :: (Eq a, Num b) => [a] -> a -> b -> [b]
allPositions' [] val pos = []
allPositions' (x:xs) val pos | x==val = (pos:(allPositions' xs val (pos+1))) | otherwise = allPositions' xs val (pos+1)
\end{code}

For the change of the int list to a digit number i used the idea of adding the value times a power of ten to ensure that the position in the number would remain the same, increasing the power as the number went on
\begin{code}
convert :: [Int] ->  Int
convert x = convert' x 1

convert' :: [Int] -> Int -> Int
convert' [] pow = 0
convert' (x:xs) pow =(x*pow) + (convert' xs (pow*10)) 
\end{code}

 
\begin{code}
type Map a b = [(a,b)]

emptyMap:: Map a b
emptyMap = []
\end{code}

Nothing much to say about this code, as always it is tail recursive with a helper to make sure it's simple and easy to work with. returns the value to match the key if it is contained
\begin{code}
getVal :: Eq a => a -> Map a b -> b
getVal key myMap  = getVal' key myMap

getVal':: Eq a => a -> Map a b -> b 
getVal' key [] = error "Key was not contained in the map"
getVal' key ((y, ys):xs)  | y == key = ys | otherwise = getVal' key xs
\end{code}

SetVal behaves as expected, if the key is not contained it will simply return the map unchanged. 
\begin{code}
setVal :: Eq a=> a -> b -> Map a b -> Map a b
setVal key val x = setVal' key val x emptyMap

setVal' :: Eq a =>  a -> b -> Map a b -> Map a b -> Map a b
setVal' key val [] myMap = myMap
setVal' key val ((y,ys):xs) myMap |  y == key  = setVal' key val xs (myMap ++[(y, val)]) | otherwise = setVal' key val xs (myMap ++ [(y, ys)])
\end{code}

Has key is a simple function, does as it should and if the 'key' is contained in the map as a value it will not throw a false positive. As with most of the functions i prefer tail recursive with a helper to clear up any confusion
\begin{code}
hasKey :: Eq a => a -> Map a b -> Bool
hasKey key x = hasKey' key x False

hasKey' :: Eq a => a -> Map a b -> Bool -> Bool
hasKey' key [] res = res
hasKey' key ((y,ys):xs)  res | y == key = True| otherwise = hasKey' key xs res
\end{code}

This function will return a new map which is duplicate of the original passed map, or throw an error. I chose to add another map as it seemed to be the most straight forwards way to implement a delete, adding every key-val pair to the new map and simply skipping over the one which needed to be deleted.
\begin{code}
delKey :: Eq a => a -> Map a b -> Map a b
delKey key myMap = delKey' key myMap emptyMap False

delKey' :: Eq a => a -> Map a b -> Map a b -> Bool -> Map a b
delKey' key [] delMap res | res == True = delMap | otherwise = error "Key was not contained in the map"
delKey' key ((y,ys):xs) delMap res |y == key = delKey' key xs delMap True | otherwise =  delKey' key xs (delMap++[(y,ys)]) res
\end{code}

The following function is the LetterFrequencies function. I split it up into a number of smaller functions (such as allLower and addKey) to make the code as simple as possible. 
Checking for the false value in haskey is first in this case as it will need to add a new pairing to the map, while the other calls the functions as arguments which were created in the previous question for simplicity

\begin{code}
letterFrequencies :: Num b => [Char] -> [(Char, b)]
letterFrequencies x = letterFrequencies' (allLower x) emptyMap

letterFrequencies' :: (Ord a, Num b) => [a] -> Map a b -> Map a b
letterFrequencies' [] myMap = myMap
letterFrequencies' (x:xs) myMap | hasKey x myMap == False = letterFrequencies' xs (addKey x myMap) | otherwise = letterFrequencies' xs (setVal x ((getVal x myMap)+1) myMap)
\end{code}

Below are three functions i created to simplify how i called the letterfrequencies function and clear up mess. They can be run individually of that function and will return the correct results if called directly.
I added the addKey to have a self-implemented way to ensure that the map would be arranged in ascending order, adding the count of 1 if there was no instance of that key in the map already. This could be changed for any number of values, including taking another argument which allowed a custom value, but for the purpose of this assignment the single count increase was enough.
\begin{code}
addKey :: (Ord a, Num b) => a -> Map a b -> Map a b
addKey key [] = [(key,1)]
addKey key ((y,ys):xs) | key < y = [(key,1)] ++ (y,ys):xs |otherwise = [(y,ys)] ++ (addKey key xs)

allLower :: [Char] -> [Char]
allLower x = allLower' x []

allLower' :: [Char] -> [Char] -> [Char]
allLower' [] lis = lis
allLower' (x:xs) lis | (isAlpha x) == True = allLower' xs (lis ++ [toLower x]) | otherwise = allLower' xs lis
\end{code}

--- end of code, begin of tests
--question one tests
These tests are copied from the assignment handout. Didn't see the need to change them for such a simple function.
\begin{code}
t1a1 = positives [] == []
t1a2 = positives [1] == [1]
t1a3 = positives [-1] == []
t1a4 = positives [1,3,-5,0,7,-16] == [1,3,7]
\end{code}

SIDE NOTE ABOUT THE NEXT TWO TESTS:
When attempting to implement these tests including the test that oddElts would throw an empty list the compiler threw up an error of ambiguous typing that i did not know how to fix. However i tested the code by hand, and calling oddElts [] will return the empty list,
while union [] [] will also result in the empty list

tests to check that oddELts throws the correct answers, that case does not matter and different types are allowed
\begin{code}
t1b1 = oddElts [1, 2, 3] == [1, 3]
t1b2 = oddElts [1] == [1]
t1b3 = oddElts [1.0, 2.5] == [1.0]
t1b4 = oddElts [1, 2, 3, -4, -5, -6, 100, 12] == [1, 3, -5, 100]
\end{code}

tests to ensure empty lists will not throw errors, that the values will be sorted correctly and that order of the lists does not matter. Sidenote: Tests only check that the lists work wtih the assumption of ascending sorted order
\begin{code}
t1c1 = union [1] [2] == [1,2]
t1c2 = union [1, 2, 3] [4, 5, 6, 7, 8] == [1, 2, 3, 4, 5, 6, 7, 8]
t1c3 = union [1, 2, 3] [2, 3, 4] == [1, 2, 3, 4]
t1c4 = union [4, 7, 9] [1, 4, 6, 7, 12] == [1, 4, 6, 7, 9, 12]
t1c5 = union [] [1, 2, 3] == [1, 2, 3]
t1c6 = union ['a', 'b', 'c'] [] == ['a','b','c']
\end{code}

Note: No tests for 1d as did not implement that function

--question one amalgamation
All question one tests gathered together
\begin{code}
test1a = [t1a1, t1a2, t1a3, t1a4]
test1b = [t1b1, t1b2, t1b3, t1b4]
test1c = [t1c1, t1c2, t1c3, t1c4, t1c5, t1c6]
\end{code}

--question two tests
tests that ensure the errors of isPrime are thrown correctly, and that the values will return the correct boolean
\begin{code}
t2a1 = isPrime 1 == True
t2a2 = isPrime 2 == True
t2a3 = isPrime 9 == False
t2a4 = isPrime 33 == False
t2a5 = isPrime 701 == True
test2aError1 = isPrime (-10)
test2aError2 = isPrime 0
test2dError = convert [-1] 
\end{code}

Tests for longest, ensuring that if all values are of the same length the first instance of that length string will be returned, and a mini test to ensure my implemented length returns correctly 
\begin{code}
t2b1 = longest [] == ""
t2b2 = longest ["This", "is", "testb2", "longest"] == "longest"
t2b3 = longest ["this", "that", "then"] == "this"
t2b4 = longest ["THAT", "that"] == "THAT"
tMyLength1 = myLength "Haskell" == 7
tMyLength2 = myLength "" == 0
\end{code}

all tests for question 2 c, checking that the empty list is thrown correctly, that the positions are correct and that when the value is not contained an empty list is returned
\begin{code}
t2c1 = allPositions [] "str" == []
t2c2 = allPositions ["Once", "upon", "a", "time"] "time" == [3]
t2c3 = allPositions ["this", "that", "this", "this", "then"] "this" == [0,2,3]
t2c4 = allPositions ['a', '0', 't', '+', ')'] 'b' == []
t2c5 = allPositions ["CAPS", "DONT", "COUNT"] "count" == []
t2c6 = allPositions [1, 2, 1, 5, 1, 6, 1, 100] 1 == [0, 2, 4, 6]
\end{code}

All tests for question 2 d, checking that the values will convert even if negative, and that an empty list will return 0 
\begin{code}
t2d1 = convert [] == 0
t2d2 = convert [12345] == 12345
t2d3 = convert [222] == 222
t2d4 = convert [1] == 1
t2d5 = convert [-730] == -730
\end{code}

--question two amalgamation
Grouping of all the question 2 tests
\begin{code}
test2a = [t2a1, t2a2, t2a3, t2a4, t2a5] 
test2b = [t2b1, t2b2, t2b3, t2b4]
testLength = [tMyLength1, tMyLength2]
test2c = [t2c1, t2c2, t2c3, t2c4, t2c5, t2c6]
test2d = [t2d1, t2d2, t2d3, t2d4, t2d5]
\end{code}

--test three map questions begin
Simple boolean tests to ensure that the haskey function returns the correct boolean and that the key being contained as a value in the map does not throw a false positive
\begin{code}
t3HK1 = hasKey 'z' [('t', 2), ('y', 4), ('a',7), ('z', 0)] == True
t3HK2 = hasKey "does" [("has", 2), ("key", 4), ("do",7), ("not", 0)] == False
t3HK3 = hasKey 'd' [('t', 'y'), ('y', 'd'), ('a','s'), ('z', 't')] == False
t3HK4 = hasKey  100 [(5, 2), (56, 4), (49,7), (100, 0)] == True
\end{code}

Tests to check that when the value is set for a key that doesn't exist, the original map will be returned unchanged. Other than that, tests to ensure the value will change. All instances where a key is matched will have the new value in this particular test block
\begin{code}
t3SV1 = setVal 'z' 15 [('t', 2), ('y', 4), ('a',7), ('z', 0)] == [('t', 2), ('y', 4), ('a',7), ('z', 15)]
t3SV2 = setVal  "when" 12 [("has", 2), ("key", 4), ("do",7), ("not", 0)] == [("has", 2), ("key", 4), ("do",7), ("not", 0)]
t3SV3 = setVal 't' 'h' [('t', 'y'), ('y', 'd'), ('a','s'), ('z', 't'), ('t', 'y')] == [('t', 'h'), ('y', 'd'), ('a','s'), ('z', 't'), ('t', 'h')]
\end{code}


Tests to check that the right value will be returned from the map based on the key. Designed to throw errors when the key passed is not present or is present where there is a value with that but a different key. The tests will remove ALL instances of the key though. also checking key is an exact match for letter case, as well as errors are thrown correctly
\begin{code}
t3GV1 = getVal 'z' [('t', 2), ('y', 4), ('a',7), ('z', 0)] == 0
t3GV2 = getVal  100 [(5, 2), (56, 4), (49,7), (100, 0)] == 0
t3GV3 = getVal "ash" [("ash", 'a'), ("flame", 'f'), ("Smoke", 's')] == 'a'
t3GVError1 = getVal "when" [("has", 2), ("key", 4), ("do",7), ("not", 0)]
t3GVError2 = getVal 'h' [('t', 'y'), ('y', 'd'), ('a','s'), ('z', 't'), ('t', 'h')]
t3GVError3 = getVal "HI" [("hi", 3), ("Hi", 7)]
\end{code}

Tests to delete key including ones which are designed to break the function and be caught by the error throwing ie trying to delete the pair based on the value not the key.  other than that just testing different types work with the mapping 
\begin{code}
t3DK1 = delKey 'z' [('t', 2), ('y', 4), ('a',7), ('z', 0)] == [('t', 2), ('y', 4), ('a',7)]
t3DK2 = delKey "do" [("has", 2), ("key", 4), ("do",7), ("not", 0)] == [("has", 2), ("key", 4), ("not", 0)]
t3DK3 = delKey 100 [(5, 2), (56, 4), (49,7), (100, 0), (10000, 3)] == [(5, 2), (56, 4), (49,7), (10000, 3)]

t3DKError1 = delKey "when" [("has", 2), ("key", 4), ("do",7), ("not", 0)]
t3DKError2 = delKey 'h' [('t', 'y'), ('y', 'd'), ('a','s'), ('z', 't'), ('t', 'h')]
\end{code}


--question three test amalgamation
Below is just a compilation of tests based on their individual functions
\begin{code}
test3Has = [t3HK1, t3HK2, t3HK3, t3HK4]
test3Set = [t3SV1, t3SV2, t3SV3]
test3Get = [t3GV1, t3GV2, t3GV3]
test3Del = [t3DK1, t3DK2, t3DK3]
\end{code}
 
--test four set starts
The below tests are an attempt to break the LF function. They ensure the correct output for an empty string, display the various ways that words and letters and symbols can be arranged but only produce the desired output, also ensuring all letters are not caps to count properly. 
The tests all pass as i would expect, order of letter input irrelevant due to the sorting function implemented in addKey
\begin{code}
t4LF1 = letterFrequencies "" == []
t4LF2 = letterFrequencies "AAAAAaaaahhhhHH" == [('a', 9), ('h', 6)]
t4LF3 = letterFrequencies "ONCE upon a TIME there __ was A PRINCESS!!??@#$" == [('a', 3), ('c', 2), ('e', 5), ('h', 1), ('i', 2), ('m', 1), ('n', 3), ('o', 2), ('p', 2), ('r', 2), ('s', 3), ('t', 2), ('u', 1), ('w', 1)]
t4LF4 = letterFrequencies "saturn, neptune, jupiter, earth" == [('a', 2), ('e', 4), ('h', 1), ('i', 1), ('j', 1), ('n', 3), ('p', 2), ('r', 3), ('s', 1), ('t', 4), ('u', 3)]
t4LF5 = letterFrequencies "COMP304 Assignment One" == [('a', 1), ('c', 1), ('e', 2), ('g', 1), ('i', 1), ('m', 2), ('n', 3), ('o', 2), ('p', 1), ('s', 2), ('t', 1)]
\end{code}

--test mini functions to help letter frequencies work
These tests run purely against the small helper functions i wrote as part of question four. They check that my map is sorted in ascending order (a-z), that the functions can run on their own and that they work independently of being called in the LF main function
\begin{code}
taK1 = addKey 'x' [] == [('x', 1)]
taK2 = addKey 'a' [('b', 2), ('c', 3)] == [('a', 1), ('b', 2), ('c', 3)]
taK3 = addKey 'f' [('a', 1), ('g', 3), ('l', 12)] == [('a', 1), ('f', 1), ('g', 3), ('l', 12)]
taK4 = addKey 'x' [('a', 1), ('h', 3), ('t', 12)] == [('a', 1), ('h', 3), ('t', 12), ('x', 1)]

taLow1 = allLower "" == ""
taLow2 = allLower "abcd 123"  == "abcd"
taLow3 = allLower "ABCD !!@@" == "abcd"
\end{code}


The below functions just run all the tests together to get a list of boolean outputs of if they passed or failed, simpler than running individual tests
\begin{code}
test4LF = [t4LF1, t4LF2, t4LF3, t4LF4, t4LF5]
test4helpers = [taLow1, taLow2, taLow3, taK1, taK2, taK3, taK4]
\end{code}

Below is just a grouping of tests based on their question, with all the tests running together in the last option
\begin{code}
testAllQ1 = [test1a, test1b, test1c]
testAllQ2 = [test2a, test2b, test2c, test2d]
testAllQ3 = [test3Has, test3Set, test3Get, test3Del]
testAllQ4 = [test4LF, test4helpers]

--all tests running consecutively
testAll = [testAllQ1, testAllQ2, testAllQ3, testAllQ4]
\end{code}