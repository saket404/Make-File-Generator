all: makefile_generator

makefile_generator: main.c binary_search_tree.c generator.c str_list.c string_hash.c src_parser.c function_scanner.c 
	gcc -o makefile_generator main.c binary_search_tree.c generator.c str_list.c string_hash.c src_parser.c function_scanner.c
	
clean:
	rm -rf makefile_generator
	rm -rf *.o
