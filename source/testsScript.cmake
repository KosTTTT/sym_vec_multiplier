set(tret 1)
#fraction test
execute_process(COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${var1}
RESULT_VARIABLE tret
)
if(tret)
    message(FATAL_ERROR "${var1} test failed.")
endif()
function(testsFunc text t1file outf strc)
	set(t1filebp ${CMAKE_CURRENT_BINARY_DIR}/${t1file})
	file(WRITE ${t1filebp} ${text})
	set(text "")
	execute_process(COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${var2} ${t1file}
	RESULT_VARIABLE tret
	)
	file(REMOVE ${t1filebp})
	set(vread "")
	file(READ ${CMAKE_CURRENT_BINARY_DIR}/${outf} vread)
	file(REMOVE ${outf})
	if(NOT ${vread} STREQUAL ${strc})
		message(FATAL_ERROR "Test failed.")
	endif()
	set(vread "")
endfunction()
#test1
set(ttest "syms a,b
ytmp = a-b
y = ytmp*(a-b)^2
expand y
print y")
testsFunc(${ttest} "t1.txt" "y.txt" "a^(3)-3*a^(2)*b+3*a*b^(2)-b^(3)")
#test2
set(ttest "syms a,b
syms t
y = a*t^2 + b*t^2 + 5t +t*a
group y t
print y")
testsFunc(${ttest} "t2.txt" "y.txt" "t^(2)*(a+b)+t*(5+a)")
#test3
set(ttest "symv x,y
f = 1/3*x*y*y*x + y*x*x*y
expand f
print f")
testsFunc(${ttest} "t3.txt" "f.txt" "4/3*_x.dot(_y)*_y.dot(_x)")
#test4
set(ttest "syms t, AB0y,VABy, AB0z, VABz
syms AC0z, VACz, AC0y, VACy
ABty = AB0y + t*VABy
ACtz = AC0z + t*VACz
ABtz = AB0z + t*VABz
ACty = AC0y + t*VACy
Nx = ABty*ACtz - ABtz*ACty
print Nx")
testsFunc(${ttest} "t4.txt" "Nx.txt" "(AB0y+t*VABy)*(AC0z+t*VACz)-(AB0z+t*VABz)*(AC0y+t*VACy)")