# Palamedes

## Description 

Palamedes is a utility  useful to native Greek speakers and Greek scholars
(Hellenists) who wish to type traditional Greek (often referred to as
multi-accented or polytonic) under the Microsoft Windows Operating system.
All versions, post Windows XP, are supported.  

This utility  allows the typing of multi-accented (polytonic) Greek text in
the manner of writing by hand. Accents and breathings are typed after the
vowels in any order without dead keys. In other words, first we type the
letter and then we "decorate" it with the required accents. Typing of
multi-accented Greek is more intuitive this way because this method is
similar to the way we write by hand.  Only a few accent keys are used
(hence it is easier to memorize). Palamedes also facilitates the typing of
punctuation and other symbols used int the Greek script. 

## The main accent keys are as follows:

| KEY          |     ACCENT          |
|:------------:|:------------------- |
|/             | Oxia (*acute*)
|\             | Varia (*grave*)
|~ or =        | Perispomeni (*circumflex*)
|]             | Psili  (*lenis*)
|[             | Dasia (*asper*)
|"             | Dialytika (*diaeresis*)
|\|            | Ypogegrammeni (*Iota subscript*)

For example to produce the charcter **ᾄ** we type 'a' followed by 
any combination of the keys '/', ']' and '|'.

The function of the dead key (;:) as used in the monotonic Greek 
has been retained. So the character **ά** can be entered with the keys
';a' or 'a/'.

For detailed usage information and a complete table of the keys 
see the documentation included in the package.

Palamedes is written in C and it uses the Windows API hook mechanism to trap
and modify the key press events when Greek is typed. Typing in other
languages is not affected and Palamedes ignores keys he does not know
about.


## Compiling

To compile Palamedes you need **GNU make** and **MinGW GCC**. 
*Tex Live* is also needed to build the documentation and the cheat sheet.

    git clone https://github.com/proteusx/palamedes
    cd palamedes/src
    make

This will build the executable `palamedes.exe`, its hooking library
`hooker.dll`, and the documentation. 

With some modifications to the source files Palamedes can also be built with
Microsoft Visual Studio. The required changes are shown in the  comments
within the source files.

## Running Palamedes

Palamedes has been tested with Windows XP, Vista, 7 and 8, is extremely
fast and unintrusive.  To use it, place the executable and its dll in the
same directory and double click on `palamedes.exe`. If all is in order an
icon will appear in the notification area (systray).  Activate Palamedes
from its systray icon, switch your keyboard to Greek, and you can start
typing polytonic Greek. Note that you need at least one Unicode font that
includes the glyphs (characters) in the Greek (0370-03FF) *and* the 
Extended Greek range (1F00-1FFF). Examples of such fonts are Times New
Roman, Palatino Linotype, Consolas and the DejaVu family.

For more complete usage information see the documentation and cheat sheet.

## To Do

The documentation is in Greek only. 
Translate documentation into English.
