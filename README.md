# Rubber Dove
Similar to a Rubber Ducky, except not. The Rubber Dove can't be run when plugged in
like the Ducky, but can instead be run manually when saved from an email, run from any
generic USB, etc. The Rubber Dove is a software-only version of the Duck.

THIS SOFTWARE IS STRICTLY FOR EDUCATIONAL PURPOSES ONLY! I AM NOT LIABLE FOR ANYTHING YOU USE MY CODE FOR

### How to use
Build Rubber Dove (or just download a binary) and copy to a USB. Create a text file and
rename it to 'input.rds', then open in your favourite text editor and let your imagination
run free! Rubber Dove uses similar language to the Ducky, so you can practically copy and
paste any payload you find on the interwebs and just run

### Example
```
REM 'input.rds' - Saves a text file to the user's desktop
WINDOWS r
DELAY 100
STRING notepad
ENTER

DELAY 500
STRING Hello, world!

CTRL s
DELAY 100
STRING %userprofile%\Desktop\test.txt
ENTER
DELAY 100
ALT F4
```

### License
The MIT License

Copyright (c) 2017 lcomstive

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

