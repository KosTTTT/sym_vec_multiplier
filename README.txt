This program opens parentheses in a symbolic expression where each symbol is a vector or a scalar. Only addition, subtraction and multiplication are supported.

To use this program create an ASCII file in the same directory as the program and specify its name as an input to a it.
The following is an example of an ASCII file.

#example 1
#define symbolic scalar variables
syms a,b
ytmp = a-b
y = ytmp*(a-b)^2
#open parentheses
expand y
#print to a text file with the name "y"
print y
#prints (a^(3)-3*a^(2)*b+3*a*b^(2)-b^(3))
#delete y and ytmp from memory
clear y,ytmp

#example 2
syms t
y = a*t^2 + b*t^2 + 5t +t*a
group y t
print y
#prints (t^(2)(a+b)+t(5+a))
#delete all variables and symbolic names
clear all

#example 3
#define symbolic vector variables
symv x,y
f = 1/3*x*y*y*x + y*x*x*y
expand f
print f
#prints 4/3*_x.dot(_y)*_y.dot(_x)


