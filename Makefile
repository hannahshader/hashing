CXX      = clang++
CXXFLAGS = -g3 -O2 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 

treeTraversal: DirNode.o FSTree.o FSTreeTraversal.o
	$(CXX) $(CXXFLAGS) -o treeTraversal DirNode.o FSTree.o FSTreeTraversal.o

gerp: main.o stringProcessing.o mapClass.o DirNode.o FSTree.o 
	$(CXX) $(CXXFLAGS) -o gerp main.o stringProcessing.o mapClass.o DirNode.o FSTree.o 

main.o: main.cpp FSTree.o DirNode.o mapClass.o
	$(CXX) $(CXXFLAGS) -c main.cpp

stringProcessing.o: stringProcessing.h
	$(CXX) $(CXXFLAGS) -c stringProcessing.cpp

mapClass.o: mapClass.h stringProcessing.h FSTree.h DirNode.h wordNodeStruct.h
	$(CXX) $(CXXFLAGS) -c mapClass.cpp
	
FSTreeTraversal.o: FSTreeTraversal.cpp FSTree.o DirNode.o
	$(CXX) $(CXXFLAGS) -c FSTreeTraversal.cpp 

unit_test: unit_test_driver.o FSTreeTraversal.o
	$(CXX) $(CXXFLAGS) $^

clean: 
	rm FSTreeTraversal.o mapClass.o stringProcessing.o main.o *~ *#