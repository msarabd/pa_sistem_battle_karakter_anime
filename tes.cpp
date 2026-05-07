#include <iostream>
#include <thread> // Untuk simulasi delay
#include <chrono>

int main() {
    std::cout << "Baris 1: Menunggu..." << std::endl;
    std::cout << "Baris 2: Menunggu..." << std::endl;

    // Tunggu 2 detik
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Pindahkan kursor naik 2 baris
    // \033[A = naik satu baris, \r = kembali ke awal baris
    std::cout << "\033[A\rBaris 1" << std::endl;
    std::cout << "\rBaris 2: Berhasil diubah!" << std::endl;

    return 0;
}