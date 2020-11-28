This program opens parentheses in a symbolic expression where each symbol is a vector or a scalar.

To run this program.
1) Create an input file. And specify its name as an input to a program
In the file one may write.

#example 1

#define symbolic scalar variables


syms a,b

ytmp = a+b

y = ytmp*ytmp

#open parentheses


expand y

#print to a text file with the name "y"


print y

#prints a^2 + 2*a*b + b^2


#delete y and ytmp from memory

clear y,ytmp

#example 2


syms t

y = a*t*t + b*t*t + 5t +t*a

group y t

print y

#prints t^2(a+b) + t(5+a)


#delete all variables and symbolic names

clear all



#example 3

#define symbolic vector variables

symv x,y

f = x*y*y*x + y*x*x*y

expand f

print f

#prints 2*_xdot_y*_ydot_x

