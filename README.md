# Artistic-Drawing
The aim of this project is to generate artistic human-like drawn lines/curves.

While there is no precise metric yet discovered to differentiate between hand-drawn and computer-generated line drawings, humans can typically distinguish with ease the difference by a shear glance. The goal of this project is to produce lines that resemble human-drawn lines and may be used to replace traditional computer drawn lines.

Human drawn lines/curves seem more familiar and easy to understand than what computer drawn sharp and crisp line do. An example to illustrate the above is given below -

Normal Lines| Line generated by application developed in this project
|------------ | ------------- 
![Normal Triangle](SVG/Results/Triangle_Normal.png) | ![SVG Triangle](SVG/Results/Triangle_SVG.png)

###Application Usage
The output/results of the application can be produced using OpenGl or SVG.
#####SVG (Requirements : Google Chrome Web Browser and g++)

● Go to the "SVG" Folder.
● On terminal type the command : $ make
● If the command is successful, it will produce an executable named "ArtisticLines".
● Create a file in the same folder for giving the input points to the program or you can use
the existing "points.txt" file.
● For running the executable, type this command in terminal : $ make run
● Now follow the instructions on the command file.
● After the program has finished, it will produce a file "outputSVG.svg" which can be
viewed in the Chrome Browser.
