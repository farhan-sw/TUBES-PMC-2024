# Tugas Besar - Penyelesaian Masalah Komputasi 2024

## Deskripsi Proyek
Proyek ini merupakan bagian dari tugas besar dalam mata kuliah Penyelesaian Masalah Komputasi (PMC) tahun 2024. Proyek ini bertujuan untuk mengimplementasikan beberapa algoritma dalam penyelesaian masalah perjalanan dagang (Travelling Salesman Problem) menggunakan bahasa pemrograman C.

## Struktur Direktori
- `algoritma/`: Direktori yang berisi implementasi algoritma untuk penyelesaian TSP.
- `utils/`: Direktori yang berisi file utilitas, seperti pembacaan data dari file.
- `data/`: Direktori yang berisi file-file data kota dan jarak antar kota.
- `main.c`: Berkas utama yang memuat fungsi `main()` sebagai titik masuk program.

## Cara Run dan Compile
### Menggunakan Makefile
1. Pastikan Makefile tersedia di direktori utama proyek.
2. Jalankan perintah `c_main` untuk mengkompilasi berkas dan `r_main` untuk run code.
3. Setelah itu, jalankan program utama dengan perintah `./main`.

### Compile Manual dari Terminal
1. Jalankan perintah kompilasi manual:
   ```
   gcc -o main main.c utils/openFile.c algoritma/dfs.c algoritma/greedy.c algoritma/bnb.c
   ```
2. Jalankan program yang telah dikompilasi:
   ```
   ./main
   ```

## Catatan Penting
- Pastikan format pembuatan algoritma mengikuti contoh yang ada.
- Pastikan antara file header (.h) dan file program (.c) bersesuaian.

## Daftar Anggota Kelompok
1. Nurul Annisa | 13222109
2. Ahmad Ramadhan | 13222110
3. Ibrahim Hanif Mulyana | 13222111
4. Hifzhi Dinullah | 13222112
5. Chobirudin Jamil Yudaprawira | 13222113
6. Muhammad Farhan | 13222114