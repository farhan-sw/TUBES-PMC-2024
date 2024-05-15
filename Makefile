# # ======= UNTUK DFS ========
# dfs: c_dfs r_dfs

# c_dfs:
# 	gcc -o dfs dfs.c ../utils/openFile.c

# r_dfs:
# 	./dfs

# # ======== UNTUK GREEDY ========
# greedy: c_greedy r_greedy

# c_greedy:
# 	gcc -o greedy greedy.c ../utils/openFile.c

# r_greedy:
# 	./greedy

# # ======== UNTUK BRANCH AND BOUND ========

# bnb: c_bnb r_bnb

# c_bnb:
# 	gcc -o bnb bnb.c ../utils/openFile.c

# r_bnb:
# 	./bnb

# Compile Untuk Main dan Semua Algoritma pada Folder Algoritma

main: c_main r_main

c_main:
	gcc -o main main.c utils/openFile.c algoritma/dfs.c algoritma/greedy.c algoritma/bnb.c

r_main:
	./main