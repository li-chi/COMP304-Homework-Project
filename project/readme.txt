=========================================================================
Course:		COMP304: Operating System
Project title:	AOS
Group members:	GUO, Donghao <11807347d>
		HE, Jiqiong <11804425d>
		JU, Sida <11839382d>
		LI, Chi <11808749d>
		ZHOU, Guangyu <11816172d>
Date:		05/05/2013
=========================================================================
System:		CygWin 1.5.21 running under Windows XP
		Compilers: g++
Compile:	gcc AOS.c -o aos
Data:		
Run:		./aos x1 x2 x3...
		(for example, <./aos paul peter lucy mary>, they are users 
		in the program. The number of users should be in the range
		of 3 to 10.)
-------------------------------------------------------------------------
Description:
This program is an example of basic input/output streaming in C++.
It uses iostream.h to manage the input streams. Basic functions
used are cin for reading from stdin and cout to write to stdout.
-------------------------------------------------------------------------
User Interface:

addClass -xxx YYYY-MM-DD hh:mm n.n
addMeeting -xxx YYYY-MM-DD hh:mm n.n xxx xxx
addGathering -xxx YYYY-MM-DD hh:mm n.n xxx xxx
([-xxx] is the name of caller. 
 [xxx] at the end of addMeeting and addGathering are the names of callees.
 
 Both caller and callees' names must be the same with the names which are 
 input in the command line. For example, <./aos paul peter lucy mary> is
 input in the command line, when callling addClass/addMeeting/addGathering,
 the names input should be selected from paul, peter, lucy and mary, and 
 the upper or lower case should also be the same, that is, Paul is not 
 correct.
 
 YYYY is the date of year, and must have four figures, e.g. 2013. 
 MM is the date of month, and must have two figures, e.g. <05>. 
 DD must have two figures, e.g. <05>.
 hh must have two figures, e.g. <08>.
 mm must have two figures, e.g. <59>.
 n.n is the duration of a activity and must have 3 figures, e.g. <3.0>,
 and just <3> is not correct.
)
 
printSchd -xxx -[f/t/r] [filename]
([-xxx] is the name of caller. 
 -f is to print timetable+rejected list. -r is to print just rejected list.
 -t is to print just time table.
 [filename] saves the timetable and rejected list for [xxx], and should 
 indicate the  type of the file, e.g. <paulschd.dat>.
)

endProgram
This ends our program completely, collect all children processes and 
close all files.
=========================================================================

