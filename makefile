CXX = clang++
CXXFLAGS = -Wall -O0 -g -std=c++1z

OBJS = main.o si-eval.o si-eval-appl.o si-eval-begin.o si-eval-env.o si-eval-if.o si-eval-lambda.o si-eval-prim.o si-eval-util.o si-global.o si-regs.o si-stack.o si-value.o si-io.o si-number.o si-driver.o

a: $(OBJS)
	$(CXX) $(CXXFLAGS) -o a $^

o: $(OBJS)

clean: 
	rm -rf a *.o 

main.o: main.cc 

si-eval.o: si-eval.cc 

si-eval-appl.o: si-eval-appl.cc 

si-eval-begin.o: si-eval-begin.cc 

si-eval-env.o: si-eval-env.cc 

si-eval-if.o: si-eval-if.cc 

si-eval-lambda.o: si-eval-lambda.cc 

si-eval-prim.o: si-eval-prim.cc 

si-eval-util.o: si-eval-util.cc 

si-regs.o: si-regs.cc 

si-stack.o: si-stack.cc 

si-value.o: si-value.cc

si-io.o: si-io.cc

si-number.o: si-number.cc

si-driver.o: si-driver.cc