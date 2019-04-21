/* Note. This is a one directional graph. The assumption is that there is only an attempt to travel north, not south. This assignment handout was vague on whether or not 
the graph needed to be bidirectional, so due to time restraints from personal issues i have only implemented this assignment travelling north. wellington-> auckland routes will always work, 
however auckland-> wellington routes will always fail */
 
road(wellington, palmerston-north, 143).

road(palmerston-north, wanganui, 74).
road(palmerston-north, napier, 178).
road(palmerston-north, taupo, 259).

road(wanganui, taupo, 231).
road(wanganui, new-plymouth, 163).
road(wanganui, napier, 252).

road(napier, taupo, 147).
road(napier, gisborne, 215).

road(new-plymouth, hamilton, 242).
road(new-plymouth, taupo, 289).

road(taupo, hamilton, 153).
road(taupo, rotorua, 82).
road(taupo, gisborne, 334).

road(gisborne, rotorua, 82).

road(rotorua, hamilton, 109).

road(hamilton, auckland, 126).

/* For my route\3 i used the basic idea of append from the lecture slides and the ancestor concept from family trees. It is a very simple method but i 
know that it works and it will continue to print off all of the possible routes from start->finish working the way up the island. route\4 is the exact same
but it adds the length as an argument so there is total distance between the start and the finish. By adding the start location to the visit list with each
call of the predicate i ensured that the list would build correctly, and backtracking gives all the possible routes in the order of which their facts appear 
in the list above, which is taken from the table given in the assignment handout 
route3test2 will fail, displaying that the predicates have not been adapted to go from north to south.
*/
route(Start, Finish, [Start|Visit]):- road(Start, Finish, _), Visit = [Finish].
route(Start, Finish, [Start|Visit]):- road(Start, Interim, _), route(Interim, Finish, Visit).

route(Start, Finish, [Start|Visit], L):- road(Start, Finish, L1), Visit = [Finish], L is L1.
route(Start, Finish, [Start|Visit], L):- road(Start, Interim, L1), route(Interim, Finish, Visit, L2), L is L1+L2.

route3test1(R):- route(wellington, auckland, R).
route3test2(R):- route(palmerston-north, wellington, R).
route3test4(R):- route(wellington, taupo, R).

route4test1(R, L):- route(wellington, auckland, R, L).
route4test1(R, L):- route(taupo, auckland, R, L).
route4test3(R, L):- route(palmerston-north, napier, R, L).

/* There is not much to say about this method. I used my previous route\4 method to correctly find the paths that could be taken after testing that it was working, 
then i chose to return the tuple of towns visited and length. As with above this is only a one way graph, due to a lack of time i had while implementing this assignment 
due to personal reasons. There are some basic tests which will not actually test the correctness of the method, due to my lack of understanding in prolog of how to do this, 
but they show the output of the method which i eyeballed and calculated to ensure that all of the expected lists would be printed. I did this in part by continuing to ask
for results from my route\4 method and checking that they were all printed out by the findall method in choice below */

choice(Start, Finish, RoutesAndDistances):- findall((Visits, Length), route(Start, Finish, Visits, Length), RoutesAndDistances).

wachoice(R):- choice(wellington, auckland, R).
wtchoice(R):- choice(wellington, taupo, R).
phchoice(R):- choice(palmerston-north, hamilton, R).

/* This method is slightly broken. It will correctly visit the towns requested of the method, but it also prints out duplicates of each option. I attempted to fix this but ran out of time, 
knowing that the method is broken through the use of backtracking in prolog and some of the patterns at the left of the predicate call matching unintentionally. I had a number of attempts to 
fix this method, by using the prolog debug and stepping through to see what was going on, but i did not manage to identify the particular error. It reached the point where that the correct lists 
were printing, no matter how many times, was the best result i could hope from this assignment. I struggled to write tests for this method due to my lack of knowledge in prolog writing tests, and 
not having results from my previous assignment to check against, and in the end i went with simple 'tests' which showed the predicates that allowed me to identify that there was an error with my
predicates.
*/
via(Start, Finish, Via, RoutesAndDistances):- findall((Visits, Length), viaroute(Start, Finish, Via, Visits, Length), RoutesAndDistances).

viaroute(Start, Finish, [], [Start|Visit], L):- road(Start, Finish, L), Visit = [Finish].
viaroute(Start, Finish, [Via], [Start|Visit], L):- Via == Start, road(Start, Finish, L1), Visit = [Finish], L is L1. 
viaroute(Start, Finish, [Via], [Start|Visit], L):- Via == Finish, road(Start, Finish, L1), Visit = [Finish], L is L1. 
viaroute(Start, Finish, [Via|Rest], [Start|Visit], L):- road(Start, Through, L1), Start == Via, viaroute(Through, Finish, Rest, Visit, L2), L is L1+L2.
viaroute(Start, Finish, [Via|Rest], [Start|Visit], L):- road(Start, Through, L1), Through == Via, viaroute(Through, Finish, Rest, Visit, L2), L is L1+L2.
viaroute(Start, Finish, Vias, [Start|Visit], L):- road(Start, Interim, L1), viaroute(Interim, Finish, Vias, Visit, L2), L is L1+L2.

viatest1(R):- via(wellington, taupo, [palmerston-north], R).
viatest2(R):- via(wellington, auckland, [taupo], R).
viatest3(R):- via(wellington, gisborne, [palmerston-north, napier], R).

/* As with the method above, this method is slightly broken. It will correctly avoid the town attempting to be avoided but it will print duplicates of the routes as it 
continues to run through the backtracking inherent in prolog. This is an issue that i think was caused by having duplicate or extremely similar patterns which would output
the same route result, but due to time constraints i was not able to accurately break down which constraint this was. As with above i attempted to fix this problem with 
using the prolog debugger but I was unable to pinpoint where the issue came from. The second method below is the one which is called by my findall method as a helper, which i 
created in order to ensure that i would be able to work my original avoid as well as possible. */

avoid(Start, Finish, Avoid, RoutesAndDistances):- findall((Visits, Length), avoidroute(Start, Finish, Avoid, Visits, Length), RoutesAndDistances).

avoidroute(Start, Finish, [Avoid|Rest], Visit, L):- Start == Avoid, avoidroute(Start, Finish, Rest, Visit, L).
avoidroute(Start, Finish, [], [Start|Visit], L):- road(Start, Finish, L), Visit = [Finish].
avoidroute(Start, Finish, [], [Start|Visit], L):- road(Start, Interim, L1), avoidroute(Interim, Finish, [], Visit, L2), L is L1+L2.
avoidroute(Start, Finish, [Avoid], [Start|Visit], L):- Avoid \== Start, road(Start, Finish, L1), Visit = [Finish], L is L1. 
avoidroute(Start, Finish, [Avoid], [Start|Visit], L):- Avoid \== Finish, road(Start, Finish, L1), Visit = [Finish], L is L1. 
avoidroute(Start, Finish, [Avoid|Rest], [Start|Visit], L):- road(Start, Through, L1), Through \== Avoid, avoidroute(Through, Finish, [Avoid|Rest], Visit, L2), L is L1+L2.
