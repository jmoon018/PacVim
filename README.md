<h1>Pac-Vim</h1>
======

<h2>Table of Contents</h2>

<ol>
	<li><a href="#IntroductionTag">Introduction</a> </li>
	<li><a href="#HowToPlayTag">How To Play</a> </li>
	<li><a href="#CodeOverviewTag">Code Overview</a> </li>
	<li><a href="#ToDoBugsTag">To-do/Bugs</a> </li>
</ol>


<h2><a name="IntroductionTag">Introduction</a></h2>

Vim is a great tool to write and edit code, but many 
people, including me, struggled with the steep learning curve. 
I did not find a fun, free way to learn about the vim commands
in-depth, and thus, Pac-Vim was born. Inspired by the the classic,
PacMan, <b>PacVim</b> is a game that'll give anyone a ton of
practice with the vim commands while being a ton of fun to play.

<h2><a name="HowToPlayTag">Introduction</a></h2>

This section will explain the rules of the game and the commands
that you can use.

The objetive of PacVim is very similar to PacMan. The objective is
to run over all the characters on the screen, while navigating
through the walls (yellow color), avoiding the ghosts (red 'G'),
and not stepping over the tilde character (cyan '~').

<br>
You are given three lives. You gain a life each time you beat
level 0, 3, 6, 9, etc. There are 10 levels, 0 through 9. After
beating the 9th level, the game is reset to the 0th levle, but
the ghosts move faster.
<br>

<b>Winning conditions:</b> Use vim commands to move the cursor
over the letters and highlight them. After all letters are
highlighted, you win and proceed to the next level.

<b>Losing conditions:</b> If you touch a ghost (indicated
by a red 'G') or a tilde character, you lose a life. If you
have less than 0 lives, you lose the entire game.

<h3>List of Working Commands</h3>

<ul>
	<li>h - move left</li>
	<li>j - move down</li>
	<li>k - move up</li>
	<li>l - move right</li>
	<li>w - move forward to the next word beginning</li>
	<li>W - move forward to the next WORD beginning</li>
	<li>e - move forward to the next word ending</li>
	<li>E - move forward to the next WORD ending</li>
	<li>b - move backwards to the word beginning</li>
	<li>B - move backwards to the WORD beginning</li>
	<li>$ - move to the end of the line</li>
	<li>0 - move to the beginning of the line</li>
	<li>gg/1G - move to the beginning of the first line</li>
	<li>*number* G - move to beginning of the line given by *number*</li>
	<li>G - move to the beginning of the last line</li>
	<li>^ - move to the first word at the current line</li>
	<li>& - l337 cheatz (win the level)</li>
</ul>

<h2><a name="CodeOverviewTag">Code Overview</a></h2>

<h4>avatar.cpp</h4>
Contains the avatar class, which contains information about
the player, such as his/her x position, y position, etc. It
also contains methods that allow the player to move and correspond
to the keystrokes. For example, the avatar class contains the method
called "parseWordForward(bool)" which implements the functionality
for the "w" (or "W" if true) vim command.

<h4>ghost1.cpp</h4>
Contains the Ghost1 class, derived from the avatar class. It is
just like the avatar class, but it requires an extra paremeter
upon initialization, called 'sleepTime', a double value that
determines how quickly a ghost moves. It refers to the time, in
seconds, the ghost must wait to move. A sleepTime of 0.5 means
the ghost moves 2 times a second. sleepTime = 0.33 is 3 moves per second, etc.
<br>
The Ghost1 class also contains a method called 'spawnGhost' which
creates the ghost at the location based on its initialization parameters.
The ghost will appear when READY (global bool) is true (this means the player
is ready), and it will call ghost.think() one second afterwards.
<br>
'think' is a recursive method that simply moves the ghost. It uses 
a basic greedy algorithm based on the distance of the ghost's potential
moves (up, dwon, right, left) and the player.
<br>

Each ghost contains its own thread. A global mutex, called mtx, is used (in 'think)
to ensure that resources are shared properly.

<h4>helperFns.cpp</h4>
Contains methods that allow easy changes of the screen. A few of them:

<ul>
	<li>chtype charAt(int x, int y) - returns the chtype at the (x,y) location
	<li>bool writeAt(int x, int y, chtype letter) - writes the 'letter' at location (x,y). Returns false if location is invalid</li>
	<li>void printAtBottom(string msg) - writes a message one line below the last line</li> 
</ul>


<h2><a name="ToDoBugsTag">To-dos / Bugs</a></h2>
<ul>
	<li>Make .gif on README.md</li>
	<li>More testing on '#G' and 'G' commands</li>
	<li>Change the map-making process easier</li>
	<li>Refactor code, more comments</li>
</ul>
