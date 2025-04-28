
# Mini grep

This is a simple grep implemented with c++ with the basic functionalities.

## Command-Line Options

- -n to show line numbers for each match
- -v to invert the match
- -r to recursively search for matchings in sub directories
- combinations like -vn -nv -rn -rv -rvn, .. etc (ordering does not matter)
- --help for guidance
## How does it work ?

- Our input should contains three things: 
    - The pattern to search for
    - our option (e.g -n, -r, -v) and can be compined or separated.
    - our file or directory path
- If no file path provided, The default path will be the current directory path
- If a directory path is given, -r should be sent to support recusrive searching.
- The first non-option argument will be treated as our pattern.
    - For example:  ./mygrep.sh hello -rn
        - here our pattrn is "hello".
        - since the path is not provided, the default will be the current directory
        - since the default is path of the directory, -r should be provided.
    - Another example:  ./mygrep Desktop/myfolder -n.  We have two things here:
        - Desktop/myfolder will be treated as our search string because it is the first non-option string passed. It is not our path
        - Again we did not send the file or folder path to search. an exception will be raised because -r is not provided as our default path now is a directory not a regular file.
    - Final example:  ./mygrep Desktop/myfolder -nv testfile.txt
        - our search string is "Desktop/myfolder"
        - It will print the lines with their numbers that don't match this string in testfile.txt
- ./mygrep.sh --help [IGNORED]
    - anything sent after the --help will be ignored



## TODO
-  Supporting regex or -i/-c/-l options.
    - We will need to change our code a little bit to support further extensions.
    - If we are going to add these features to our current code, We will change it (Not adding to it). which can cause problems.
    -
- I didn't recognize that I should implemented it using BASH SCRIPT but it was too late :). 