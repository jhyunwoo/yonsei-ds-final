# Makefile for Data Structures Final Project
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

# 테스트 프로그램
all: test_sorting test_trees

# 정렬 알고리즘 테스트
test_sorting: test_sorting_algorithms.cpp
	$(CXX) $(CXXFLAGS) -o test_sorting test_sorting_algorithms.cpp

# 탐색 트리 테스트
test_trees: test_search_trees.cpp
	$(CXX) $(CXXFLAGS) -o test_trees test_search_trees.cpp

# 개별 정렬 알고리즘 컴파일
insertion_sort: sort/insertion_sort.cpp
	$(CXX) $(CXXFLAGS) -o insertion_sort sort/insertion_sort.cpp

heap_sort: sort/heap_sort.cpp
	$(CXX) $(CXXFLAGS) -o heap_sort sort/heap_sort.cpp

merge_sort: sort/merge_sort_fixed.cpp
	$(CXX) $(CXXFLAGS) -o merge_sort sort/merge_sort_fixed.cpp

quick_sort: sort/quick_sort_fixed.cpp
	$(CXX) $(CXXFLAGS) -o quick_sort sort/quick_sort_fixed.cpp

shell_sort: sort/shell_sort_fixed.cpp
	$(CXX) $(CXXFLAGS) -o shell_sort sort/shell_sort_fixed.cpp

# 개별 트리 알고리즘 컴파일
bst: search_tree/binary_search_tree_fixed.cpp
	$(CXX) $(CXXFLAGS) -o bst search_tree/binary_search_tree_fixed.cpp

avl: search_tree/avl_search_tree.cpp
	$(CXX) $(CXXFLAGS) -o avl search_tree/avl_search_tree.cpp

rbtree: search_tree/red-black_search_tree.cpp
	$(CXX) $(CXXFLAGS) -o rbtree search_tree/red-black_search_tree.cpp

two_four_tree: search_tree/2-4_search_tree.cpp
	$(CXX) $(CXXFLAGS) -o two_four_tree search_tree/2-4_search_tree.cpp

m_ary_tree: search_tree/m-ary_search_tree.cpp
	$(CXX) $(CXXFLAGS) -o m_ary_tree search_tree/m-ary_search_tree.cpp

# 모든 개별 프로그램 컴파일
individual: insertion_sort heap_sort merge_sort quick_sort shell_sort bst avl rbtree two_four_tree m_ary_tree

# 실행
run_sorting: test_sorting
	./test_sorting

run_trees: test_trees
	./test_trees

run_all: test_sorting test_trees
	@echo "=== 정렬 알고리즘 테스트 ==="
	./test_sorting
	@echo "\n\n=== 탐색 트리 테스트 ==="
	./test_trees

# 청소
clean:
	rm -f test_sorting test_trees
	rm -f insertion_sort heap_sort merge_sort quick_sort shell_sort
	rm -f bst avl rbtree two_four_tree m_ary_tree
	rm -f *.o

.PHONY: all clean run_sorting run_trees run_all individual
