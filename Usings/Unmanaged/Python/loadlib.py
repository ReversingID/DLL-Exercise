# Memuat pustaka saat runtime.
#
# Memanggil unmanaged.dll yang memiliki fungsi:
#     - world (static)
#     - calculate
#
# Menggunakan ctypes untuk memuat isi pustaka dan mendapatkan alamat
# fungsi yang dibutuhkan dalam eksekusi
#
# Run:
#   $ python loadlib.py

import ctypes
import sys

# Sebelum mendapatkan alamat dari fungsi terlebih dahulu kita harus memuat pustaka
# ke memory dan memetakannya ke area yang masih tersedia.
lib = ctypes.WinDLL('unmanaged.dll')
if lib is None:
    print("Tidak dapat memuat file DLL")
    sys.exit(1)

# Cari fungsi berdasarkan nama
world = lib['world']
calculate = lib['calculate']

# Periksa apakah ada fungsi yang tak dapat ditemukan
if world is None or calculate is None:
    print("Tidak dapat menemukan fungsi")
    sys.exit(2)

# Panggil fungsi
# Memanggil fungsi static world()
world()

# Memanggil calculate()
print("Hasil dari calculate(0) adalah {}".format(calculate(0)))