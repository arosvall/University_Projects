/* 
For my print sentence prediccate i decided to call another predicate within it to actually take care of printing out the sentence. This allowed me 
to be able to transform the list into a sentence containing quotation marks at the beginning and the end without compromising how i needed to build the predicate
and allowing me to bypass any extra checks which would have needed to find out if the word was at the beginning of the sentence  (list). This also came about from 
originally having the one method which would incorrectly print the lists as "this "is "a "list " for example"
*/  


printSentence(X):- write('"'), prints(X), write('"').
prints(([])).
prints([X|Rs]):- X==qm, write(?), prints(Rs).
prints([X|[]]):- write(X).
prints([X|Rs]):- write(X), write(' '), prints(Rs).

pSTest1():- printSentence([this, is, example, one]).
pSTest2():- printSentence([today, i, feel, happy]).

/* For my answer i just run the predicates of match, checking if there is a point in transforming, and then transform. If there is not the word for merging i decided to throw 
a message of this is not a valid AI shrink question in order to bypass issues from this, and also to restrict the program into running only valid AI shrink inputs that we were
given in the suggested part of the assignment. I also cut after this to stop the recursion from printing out something i did not want it to. */

answer(([]), _).
answer(X,Y):- match(X), transform(X,Y), !.
answer(_, _):- write("This was not an applicable AI shrink question, please try again"), !.


/*For my tests for my answer predicate, i checked that there were correct matches when the lists need to be transformed. i did this to make sure that they would evaluate to 
true and that i would be able to take advantage of unification to check that the result was what we expected it to be. 
*/

aTest1():-answer([how, do, you, feel, today], [let, us, not, talk, about, me]).
aTest2():-answer([i, feel, like, going, for, a, run], [why, do, you, feel, like, going, for, a, run, qm]).
aTest3():-answer([i, had, a, nightmare], [do, nightmares, frighten, you, qm]).
aTest4():-answer([i, feel, like, my, brother, is, annoying], [why, do, you, feel, like, your, brother, is, annoying, qm]).
aTest5():-answer([i, fantasised, i, was, a, dragon], [have, you, fantasised, you, were, a, dragon, before, qm]).


/*Instead of the suggested match2 given in the assingment handout i decided to go with a match one which would just return whether or not the item we are looking for is
in the list. This was due to my own confusion over what it was implied that the match2 would need to do, and how to set up the correct predicates for it. I may have fixed this
confusion for myself by adding in the append and replace methods at the end, but this is how i decided to structure the assignment. The biggest issue i came across with the 
match method was when it would not correctly decide if the word was in the list if it was added on later i.e. would not pick up i feel in a list of [today, i, feel], but i 
managed to fix the issue by actually recursing on the rest of the list.  
*/
match([X | _]):- X==you.
match([X ,Y | _]):- X==i, Y==feel.
match([X ,Y | _]):- X==i, Y==fantasised.
match([X | _]):- X==nightmare.
match([X]):- X==qm.
match([_|Rs]):- match(Rs).

/*
For my match tests I just chose to write up a few that show the test will evaluate true or false correctly based on the criteria of meeting the keywords which indicate the 
Ai shrink has been asked a correct question. I did not see the point in writing massive numbers of tests as i proved that they worked and evaluated correctly, and extensive
testing happened when there was the main use of the method within the answer questions.
*/

mTest1():- match([once, upon, a, time]).
mTest2():- match([i, feel, sad]).
mTest3():- match([today, i, fantasised, about, being, a, robot]).
mTest4():- match([i, had, a, nightmare]).
mTest5():- match([how, are, you]).

/* For my transform predicate i followed the same sort of pattern as i used during match, which may extend the amount of time taken for the entire query to run but
was the easiest and most logical way i could see to construct the results. I call the replace method to take care of swapping pronouns, and ended up adding in the 
append method in order to remove printing [words] like that and also to be able to smoothly add qm to the end without getting any issues with returning the correct list
of words. However that does also increase the time taken and add some redundancy in (i.e. would be faster to just add qm to the end of the list), but when i tried this it
threw up incorrect transforms or in many cases just ignored the qm all together. 
*/
transform([],_):- !.
transform([A], Y):- A==you, Y = [let, us, not, talk, about, me], !.
transform([A| _], Y):- A==you, Y = [let, us, not, talk, about, me], !.
transform([A, B | Rs], Y):- A==i, B==feel, replace(Rs, Rs1), append([why, do, you, feel], Rs1, Y2), append(Y2, [qm], Y). 
transform([A, B | Rs], Y):- A==i, B==fantasised,!, replace(Rs, Rs1), append([have, you, fantasised], Rs1, Y2), append(Y2, [before, qm], Y).
transform([A], Y):- A==qm, Y= [why, do, you, ask, qm].
transform([A | _], Y):- A==nightmare, Y=[do, nightmares, frighten, you, qm].
transform([_|Rs], Y):- transform(Rs, Y).


/* I only wrote two skeleton tests for test1 as i tested it more extensively during the answer tests and proved it worked there
*/
tTest1:- transform([how, are, you], [let, us, not, talk, about, me]).
tTest2:- transform([the, sky, is, blue], [the, sky, is, blue]).


/* At first when i had a transform predicate which did not also make use of the append i would print out the reply and it would print the list contained
within the list as a list i.e. i feel [sad]. By adding in the append predicate which is heavily influenced by the one which was shown in the lecture slides.
By using append i managed to avoid the messiness of needing to extract the list from within the list, and allowed me to keep my structure as before with 
the transform predicate itself
I did not write any tests for this as it was based heavily on what the lecture slides have already proven to work. 
*/

append([ ], Y, Y).
append([X | Y], Z, [X | YL]) :- append(Y, Z, YL).


/*  For my replace predicate i considered the different cases where pronouns would need to be changed. i used the guide in the assignment handout
to guide the replacements i would need to make. I also needed to consider the case where the word needing to be changed was at the end of the sentence.
In this case, i did not need to add the same facts for replacement that i did when something was in the sentence, instead i looked at the logic of how 
most sentences would be structured and decided that mine was a word which could easily end a sentence, while me, I, my and am all seemed like illogical
choices to end the sentence. it would be easy to extend predicates with other options, but i could not think of any more and wanted to ensure that i had
most of the bases from the assignment covered.
*/
replace([], _).
replace([X|[]], X).
replace([X|[]], Y):- X == mine, Y = yours.
replace([X|Rs], [Y|Z]):- X == my, Y = your, replace(Rs, Z).
replace([X|Rs], [Y|Z]):- X == i, Y = you, replace(Rs, Z).
replace([X|Rs], [Y|Z]):- X == mine, Y = yours, replace(Rs, Z).
replace([X|Rs], [Y|Z]):- X == am, Y = are, replace(Rs, Z).
replace([X|Rs], [Y|Z]):- X == was, Y = were, replace(Rs, Z).
replace([X|Rs], [X|Z]):- replace(Rs, Z).

/*I wrote small, simple tests to show that the replace predicate works. The test calls the replace predicate defined above, printing out what the transformed
sentence should be. It is just a very basic set of predicates, all evaluating to true, to get the idea across. Better tests show how it works above */
replaceTest1():- replace([i, feel, sad], [you, feel, sad]). 
replaceTest2():- replace([my, brother], [your, brother]).
replaceTest3():- replace([nothing, to, replace, here], [nothing, to, replace, here]). 

/*
For print reply i make use of the other predicates defined in the program. I use the check that X\=Y in order to make sure that there will not be an ("") printed at the end of the 
sentence if there is no point in concluding i.e. answer already gave us the written message, as this was the best way i could find to make it work. apart from that i used the print
method as this was the quickest, easiest way to print out both the question and then following that the reply, the logic of which meant that i could also make the code oblivious and 
explicit. 
*/
printReply([]).
printReply(X):- answer(X,Y), X\= Y, !,printSentence(X), writeln(' '), printSentence(Y).


/* For my print reply tests i wanted to show that each of the four suggestions in the assignment handout has two applicable situations minimum where the tests would show the methods worked correctly.
I also have very similar tests in the answer section above where i showed that the correct output will be created. The tests merely take a predefined list and print out the results, showing
that they correctly use the printSentence predicate as well. prtest 10 is merely to show that incorrect inputs (i.e. inputs not covered by the definitions i have given in this program), will exit without
printing anything incorrectly. 
I have noticed that there is an error somewhere in my code and that the execution does not end after a single result, but i do not know where the error is coming from and so manually stopped the backtracking
by ending the tests with . after my first reply*/

pRTest1():- printReply([i, feel, sad]).
pRTest2():- printReply([there, are, times, when, i, feel, happy]).
pRTest3():- printReply([how, are, you]).
pRTest4():- printReply([what, do, you, think, about, puppies]).
pRTest5():- printReply([what, is, love, qm]).
pRTest6():- printReply([will, i, win, the, lottery, qm]).
pRTest7():- printReply([i, had, a, nightmare, about, springtrap]).
pRTest8():- printReply([i, fantasised, my, puppy, is, a, goldfish]).
pRTest9():- printReply([last, night, i, fantasised, about, turning, into, a, zombie]).
pRTest10():- printReply([dental, plan, lisa, needs, braces]).