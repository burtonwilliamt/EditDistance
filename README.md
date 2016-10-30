# Edit Distance
I was inspired to do this project after a particularly interesting Algorithms lecture. This project is a demonstration of how edit distance can be calculated. This is useful when determining the similarity of words.

## Example
Each cell represents the alignment of the sub-strings up to those coordinates.
In this example. Position 3,2 is selected, which is the first 3 letters of the string at the top, and the first 2 letters of the string on the lefthand side.
```
.     s  u  n  n  y
  |--|--|--|--|--|--|
  |00|01|02|03|04|05|
  |--|--|--|--|--|--|
s |01|00|01|02|03|04|
  |--|--|--*--*--|--|
n |02|01|01|01|02|03|
  |--|--|--*--*--|--|
o |03|02|02|02|02|03|
  |--|--|--|--|--|--|
w |04|03|03|03|03|03|
  |--|--|--|--|--|--|
y |05|04|04|04|04|03|
  |--|--|--|--|--|--|

Number of edits shown by value at position on board

Alignments:

sun
s_n
```

The value in each cell represents the fewest edits necessary to match the two sub-strings. There are three edits possible, and each one can be represented as a movement along the grid. They are as follows:
* Moving right adds only the next character to the top string, we will consider an edit, as it is like adding a space to the lefthand string.
* Moving down adds the next character to the lefthand string which will also cause an increase the number of edits, as it is like adding a space to the top string..
* The third movement is diagonal down and right, which adds both the respective characters. If they match, then there is no increase in edits, because that is aligned. If they do not match, then there is another edit increase because that would be a "swap" edit.

To get the example solution:

1) Start at 0,0 where the strings are both empty.

2) Then move diagonally to 1,1, adding both the next letters
```
s
s
```
This would be still 0 edits, because they are aligned, which is why the value at 1,1 is 0.

3) Next, add the next character to the top string, and add a space to the lefthand string.
```
su
s_
```
This would be a movement right on the board to 2,1 and would be an increase in edits, due to the addition of a space to the final solution.

4) Finally, add the next two characters to both screens
```
sun
s_n
```
This is a movement diagonally to 3,2 and would not be an increase in edits, because both strings have an n next. Thus the value at 3,2 is 1 because that is the number of total edits to get there.
