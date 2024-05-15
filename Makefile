# Compile Untuk Main dan Semua Algoritma pada Folder Algoritma
c_main:
	gcc -o main main.c utils/openFile.c algoritma/dfs.c algoritma/greedy.c algoritma/bnb.c

r_main:
	./main