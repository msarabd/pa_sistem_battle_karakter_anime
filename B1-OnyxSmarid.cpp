#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <limits>
#include <ctime>
#include <thread>
#include <chrono>
#include "animasi.h"
using namespace std;

#define RESET   "\033[0m"
#define MERAH   "\033[31m"
#define HIJAU   "\033[32m"
#define KUNING  "\033[33m"
#define BIRU    "\033[34m"
#define OREN    "\033[38;5;208m"

const int max_karakter = 200;
const int max_akun = 20;

struct stat{
    double ap; // attack power
    double sp; // speed
    double du; // durability
    double iq;
};

struct karakter{
    int id;
    string nama;
    string anime;
    stat stats;
    string ket;
};

struct akun{
    string user;
    string pw;
    string status;
};

struct stack{
    string battle;
    string pemenang;
    double ovr;
};


void cls(){
    #ifdef _WIN32
        system("cls");
    #else
        cout << "\033[2J\033[1;1H";
#endif
}

void enterKembali() {
    cout << KUNING << "\n(Ketuk enter untuk kembali)";
    
    // 1. Membersihkan error state jika ada
    cin.clear(); 
    
    // 2. Membersihkan sisa buffer (seperti spasi atau karakter tak terpakai)
    // Jika buffer kosong, ini tidak akan menunggu. Jika ada isinya, ini akan menghapusnya.
    if (cin.rdbuf()->in_avail() > 0) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // 3. Benar-benar menunggu input Enter dari user
    cin.get(); 
}

void progressBar(){
    cls();
    int panjangBar = 50;
    char kosong = 177;
    char isi = 219;

    cout << "\nLoading...\n\n";
    for (int i = 0; i < panjangBar; i++)
        cout << kosong;

    cout << "\r";
    for (int i = 0; i < panjangBar; i++) {
        cout << BIRU << isi << flush << RESET;
        this_thread::sleep_for(chrono::milliseconds(50));
    }
    cout << endl;
}

void iklan(string array[], int jumlahIklan){
    cls();
    int seconds = 5;
    int index = rand() % jumlahIklan;
    cout << BIRU << array[index] << RESET;
    // while (seconds > 0) {
    //     cout << "lewati dalam: " << seconds << " detik" << endl;
    //     this_thread::sleep_for(chrono::seconds(1));
    //     seconds--;
    // }

    for (int i = 0; i <= 100; i++) {
        std::cout << BIRU << "\rProgress: " << i << "%" << std::flush << RESET;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    cout << KUNING << "\n(Ketuk enter untuk lanjut)" << RESET;
    cin.get();
}

void tampilkan(string menu, int a, int b){
    cls();
    cout << OREN << menu;
    cout << "Pilih (" << a << "-" << b << "): " << RESET; 
}

void tampilkan(string kata){
    cout << OREN << kata << RESET << endl ;
}

string cekUser(akun arr[], string tampung, int panjang_user){
    for (int i = 0; i < panjang_user; i++){
        if (tampung == arr[i].user){
            cin.ignore();
            cout << MERAH << "(Username sudah ada, ketuk enter untuk kembali)";
            cin.get();
            tampung = "0";
        }
    }
    return tampung;
}

int loginUser(akun arr[], int& sisa_login, bool& token_utama, bool& token_login, int panjang_user, string header){
    string input_user, input_pw;

    cls();
    tampilkan(header);
    cout << OREN << "Masukkan username: " << RESET;
    cin >> input_user;
    cout << OREN << "Masukkan password: " << RESET;
    cin >> input_pw;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    for (int i = 0; i < panjang_user; i++){
        if (input_user == arr[i].user){
            if (input_pw == arr[i].pw){
                token_utama = true;
                token_login = false;
                cout << HIJAU << "(Login berhasil, ketuk enter untuk lanjut)";
                cin.get();
                progressBar();
                return i;
            } else {
                sisa_login--;
                cout << MERAH << "(Password salah, sisa login Anda = " << sisa_login << ")";
                cin.get();
                break;
            }
        }

        if (i == panjang_user - 1){
            sisa_login--;
            cout << MERAH "(User tidak ditemukan, sisa login Anda = " << sisa_login << ")" << RESET;
            cin.get();
        }
    }

    if (sisa_login == 0){
        return -1;
    }

    return loginUser(arr, sisa_login, token_utama, token_login, panjang_user, header);
}

void registerUser(akun arr[], string header, int* panjang_user){
    string tampung;
    string input_user;
    string kode;

    cls();
    tampilkan(header);
    cout << OREN << "Masukkan username: " << RESET;
    cin >> tampung;
    input_user = cekUser(arr, tampung, *panjang_user);
    
    if (input_user != "0"){
        arr[*panjang_user].user = input_user;
        cout << OREN << "Masukkan password: " << RESET;
        cin >> arr[*panjang_user].pw;
        
        cout << OREN << "Masukkan kode referral: " << RESET;
        cin >> kode;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (kode == "OnyxSmarid"){
            arr[*panjang_user].status = "premium";
            cout << HIJAU << "(AKUN ANDA SEKARANG ADALAH PREMIUM!!)" << endl;
        } else {
            arr[*panjang_user].status = "biasa";
            cout << MERAH << "(MAAP KODE REFERRAL ANDA SALAH!!)" << endl;
        }
        (*panjang_user)++;
        
        cout << HIJAU << "\n(Berhasil membuat akun, silahkan login kembali)";
        cin.get();
    }
}

void tampilKarakter(karakter arr[], int panjang_karakter){
    cls();
    if (panjang_karakter == 0){
        cout << MERAH << "(List kosong, tidak ada data yang bisa dilihat)" << RESET;
        cin.get();

    } else {
        cout << KUNING << setfill('=') << setw(145) << "=" << endl;
        cout << setfill(' ');
        cout << "| " << left << setw(4)  << "No" 
            << "| " << setw(8) << "ID"
            << "| " << setw(25) << "Nama Karakter"
            << "| " << setw(18) << "Seri" 
            << "| " << setw(14)  << "Attack Power" 
            << "| " << setw(7)  << "Speed" 
            << "| " << setw(12)  << "Durability" 
            << "| " << setw(5)  << "IQ"
            << "| " << setw(32) << "Keterangan" << " |" << endl;

        cout << setfill('-') << setw(145) << "-" << endl;
        cout << setfill(' '); 

        for (int i = 0; i < panjang_karakter; i++) {
            cout << "| " << left << setw(4) << (i + 1)
                << "| " << setw(8) << arr[i].id
                << "| " << setw(25) << arr[i].nama
                << "| " << setw(18) << arr[i].anime
                << "| " << setw(14)  << arr[i].stats.ap 
                << "| " << setw(7)  << arr[i].stats.sp 
                << "| " << setw(12)  << arr[i].stats.du 
                << "| " << setw(5)  << arr[i].stats.iq
                << "| " << setw(32) << arr[i].ket << " |" << endl;
        }
        cout << setfill('=') << setw(145) << "=" << endl;
        cout << setfill(' ') << RESET;
        enterKembali();
    }
}

void tambahKarakter(karakter arr[], int* panjang_karakter){
    int jumlah_tambah;

    cls();
    if (*panjang_karakter >= max_karakter){
        cout << MERAH << "(List sudah penuh, hapus satu atau lebih karakter)";
        cin.get();

    } else {
        cout << OREN << "Mau tambah berapa karakter: " << RESET;
        cin >> jumlah_tambah;
        
        try{
            if (cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("(Input harus berupa angka, ketuk enter untuk kembali)");
            }

            if (cin.peek() != '\n' && !cin.eof()){
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("(Input tidak boleh desimal/huruf, ketuk enter untuk kembali)");
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (jumlah_tambah > 0){
                for (int i = 0; i < jumlah_tambah; i++){
                    cout << OREN  "\nKarakter ke-" << i + 1 << endl;
                    cout << "Nama karakter: " << RESET;
                    getline(cin, arr[*panjang_karakter].nama);

                    cout << OREN << "Dari anime/manga: " << RESET;
                    getline(cin, arr[*panjang_karakter].anime);
                    
                    cout << OREN << "Attack power (0-100): " << RESET;
                    cin >> arr[*panjang_karakter].stats.ap;
                    cout << OREN << "Speed (0-100): " << RESET;
                    cin >> arr[*panjang_karakter].stats.sp;
                    cout << OREN << "Durability (0-100): " << RESET;
                    cin >> arr[*panjang_karakter].stats.du;
                    cout << OREN << "IQ (0-100): " << RESET;
                    cin >> arr[*panjang_karakter].stats.iq;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << OREN << "Keterangan (isi '-' jika kosong): " << RESET;
                    getline(cin, arr[*panjang_karakter].ket);

                    arr[*panjang_karakter].id = 2500 + *panjang_karakter + 1;
                    cout << HIJAU << "(Berhasil menambahkan karakter)" << endl;
                    (*panjang_karakter)++;
                }

                enterKembali();

            } else {
                cout << MERAH << "(Maaf, input tidak valid)";
                cin.get();
            }
        }
        
        catch (exception& e){
            cout << MERAH << e.what();
            cin.get();
        }
    }
}

void perbaruiKarakter(karakter arr[], int panjang_karakter){
    int jumlah_update;
    int elemen;

    cls();
    if (panjang_karakter == 0){
        cout << MERAH << "(List kosong, tidak ada data yang bisa diperbarui)";
        cin.get();

    } else {
        cout << OREN << "Mau perbarui berapa karakter: " << RESET;
        cin >> jumlah_update;
        
        try{
            if (cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("(Input harus berupa angka, ketuk enter untuk kembali)");
            }

            if (cin.peek() != '\n' && !cin.eof()){
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("(Input tidak boleh desimal/huruf, ketuk enter untuk kembali)");
            }

            if (jumlah_update > panjang_karakter){
                cout << MERAH << "(Jumlah karakter hanya ada " << panjang_karakter << ")";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();

            } else if (jumlah_update > 0){
                for (int i = 0; i < jumlah_update; i++){
                    cout << OREN << "\nKarakter ke-" << i + 1 << endl;
                    cout << "Masukkan nomor karakter: " << RESET;
                    cin >> elemen;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (elemen > 0 && elemen <= panjang_karakter){
                        cout << OREN << "Nama karakter: " << RESET;
                        getline(cin, arr[elemen - 1].nama);

                        cout << OREN << "Dari anime/manga: " << RESET;
                        getline(cin, arr[elemen - 1].anime);
                        
                        cout << OREN << "Attack power (0-100): " << RESET;
                        cin >> arr[elemen - 1].stats.ap;
                        cout << OREN << "Speed (0-100): " << RESET;
                        cin >> arr[elemen - 1].stats.sp;
                        cout << OREN << "Durability (0-100): " << RESET;
                        cin >> arr[elemen - 1].stats.du;
                        cout << OREN << "IQ (0-100): " << RESET;
                        cin >> arr[elemen - 1].stats.iq;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        cout << OREN << "Keterangan (isi '-' jika kosong): " << RESET;
                        getline(cin, arr[elemen - 1].ket);
                        cout << HIJAU << "(Data karakter berhasil diperbarui)" << endl;
                    } else {
                        cout << MERAH << "(Maaf, nomor karakter tidak valid)";
                        cin.get();
                    }
                }

                enterKembali();
                
            } else {
                cout << MERAH << "(Maaf, input tidak valid)";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            }
        }

        catch (exception& e){
            cout << MERAH << e.what();
            cin.get();
        }
    }
}

void hapusKarakter(karakter arr[], int *panjang_karakter){
    int elemen;

    cls();
    if (*panjang_karakter == 0){
        cout << MERAH << "(List kosong, tidak ada data yang bisa dihapus)";
        cin.get();

    } else {
        cout << OREN << "Masukkan nomor karakter: " << RESET;
        cin >> elemen;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        try{
            if (cin.fail()){
                cin.clear();
                throw invalid_argument("(Input harus berupa angka, ketuk enter untuk kembali)");
            }

            if (cin.peek() != '\n' && !cin.eof()){
                throw invalid_argument("(Input tidak boleh desimal/huruf, ketuk enter untuk kembali)");
            }
            
            if (elemen > 0 && elemen <= *panjang_karakter){
                cout << HIJAU << "(" << arr[elemen - 1].nama << " berhasil dihapus, ketuk enter untuk kembali)";
                for (int i = elemen - 1; i < *panjang_karakter - 1; i++){
                    arr[i] = arr[i + 1];
                }
                (*panjang_karakter)--;
            } else {
                cout << MERAH << "(Maaf, nomor karakter tidak valid)";
            }
            
            cin.get();
        }

        catch (exception& e){
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << MERAH << e.what();
            cin.get();
        }
    }
}

void swap(karakter* a, karakter* b){
    karakter temp = *a;
    *a = *b;
    *b = temp;
}

void bubleSort(karakter arr[], string atribut, int panjang){
    cls();

    if (panjang == 0){
        cout << MERAH << "(List kosong, tidak ada data yang bisa disorting)";
        cin.get();
        return;
    }
    

    for (int i = 0; i < panjang - 1; i++){
        for (int j = 0; j < panjang - 1 - i; j++){
            if (arr[j].nama > arr[j + 1].nama){
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }

    cout << HIJAU << "(Sorting by nama karakter berhasil, silahkan lihat pada menu read)";
    cin.get();
}

int max(karakter arr[], string atribut, int batas_bawah, int panjang){
    int index_max = batas_bawah;
    for (int i = batas_bawah; i < panjang; i++){
        if (atribut == "AP"){
            if (arr[i].stats.ap > arr[index_max].stats.ap){
                index_max = i;
            }
        
        } else if (atribut == "SP"){
            if (arr[i].stats.sp > arr[index_max].stats.sp){
                index_max = i;
            }
        
        } else if (atribut == "DU"){
            if (arr[i].stats.du > arr[index_max].stats.du){
                index_max = i;
            }
        
        } else if (atribut == "IQ"){
            if (arr[i].stats.iq > arr[index_max].stats.iq){
                index_max = i;
            }
        
        }
    }

    return index_max;
}

void selectionSort(karakter arr[], string atribut, int panjang){
    int index_max;

    cls();
    if (panjang == 0){
        cout << MERAH << "(List kosong, tidak ada data yang bisa disorting)";
        cin.get();
        return;
    }
    
    for (int i = 0; i < panjang - 1; i++){
        index_max = max(arr, atribut, i, panjang);
        swap(&arr[i], &arr[index_max]);
    }
    
    cout << HIJAU << "(Sorting by " << atribut << " dari terbesar ke terkecil berhasil, silahkan lihat pada menu read)";
    cin.get();
}

void insertionSort(karakter arr[], string atribut, int panjang){
    cls();
    if (panjang == 0){
        cout << MERAH << "(List kosong, tidak ada data yang bisa disorting)" << RESET;
        cin.get();
        return;
    }

    for (int i = 1; i < panjang; i++){
        karakter key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j].anime > key.anime){
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }

    cout << HIJAU << "(Sorting by nama seri/anime berhasil, silahkan lihat pada menu read)";
    cin.get();
}

void sortCustom(karakter arr[], int panjang){
    for (int i = 0; i < panjang - 1; i++){
        for (int j = 0; j < panjang - 1 - i; j++){
            if (arr[j].id > arr[j + 1].id){
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void linearSearch(karakter* arr, int* panjang){
    string target;

    cls();
    cout << OREN << "Masukkan nama karakter yang ingin dicari: " << RESET;
    getline(cin, target);

    for (int i = 0; i < *panjang; i++){
        cout << OREN << "\n== iterasi ke-" << i + 1 << " ==" << RESET << endl;

        if (target == (arr + i)->nama){
            cout << OREN << target << " = " << (arr + i)->nama << RESET << endl;
            cout << HIJAU << "(" << target << " ditemukan pada indeks ke-" << i << ")";
            cin.get();
            return;
        } else {
            cout << OREN << target  << " != " << (arr + i)->nama << RESET << endl;
            cout << MERAH << "(tidak ditemukan)" << RESET << endl;
        }
    }
    
    cout << MERAH << "\n(" << target << " tidak ditemukan pada data karakter" << ")" << RESET;
    cin.get();
}

void fibonacciSearch(karakter* arr, int* panjang){
    int fib = 1;
    int fib1 = 1;
    int fib2 = 0;
    int offset = -1;
    int index;
    int target;
    
    cls();
    sortCustom(arr, *panjang);
    cout << OREN << "Masukkan ID karakter yang ingin dicari: " << RESET;
    cin >> target;
    
    try{
        if (cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("(Input harus berupa angka, ketuk enter untuk kembali)");
        }

        if (cin.peek() != '\n' && !cin.eof()){
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("(Input tidak boleh desimal/huruf, ketuk enter untuk kembali)");
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        while (fib < *panjang){
            fib2 = fib1;
            fib1 = fib;
            fib = fib1 + fib2;
        }

        int j = 1;
        while (fib > 1){
            cout << OREN << "\n== iterasi ke-" << j << " ==" << RESET << endl;
            index = min(offset + fib2, *panjang - 1);
            if ((arr + index)->id == target){
                cout << OREN << (arr + index)->id << " = " << target << RESET << endl;
                cout << HIJAU << "("<< target << " ditemukan pada index ke-" << index << ")" << RESET;
                cin.get();
                return;
            } else if ((arr + index)->id < target){
                cout << OREN << (arr + index)->id << " < " << target << RESET << endl;
                fib = fib1;
                fib1 = fib2;
                fib2 = fib - fib1;
                offset = index;
            } else {
                cout << OREN << (arr + index)->id << " > " << target << RESET << endl;
                fib = fib2;
                fib1 = fib1 - fib2;
                fib2 = fib - fib1;
            }

            cout << MERAH << "(tidak ditemukan)" << RESET << endl;
            j++;
        }

        cout << MERAH << "\n(ID " << target << " tidak ditemukan pada data karakter)" << RESET; 
        cin.get();
    }
    
    catch (exception& e){
        cout << MERAH << e.what() << RESET;
        cin.get();
    }
}

void push(stack riwayatBattle[], int* top, string battle, string pemenang, double ovr){
    if (*top == 99){
        cout << MERAH << "(Stack penuh)";
        return;
    }

    (*top)++;
    riwayatBattle[*top].battle = battle;
    riwayatBattle[*top].pemenang = pemenang;
    riwayatBattle[*top].ovr = ovr;
}

void pop(stack riwayatBattle[], stack* hapusRiwayat, int* top){
    cls();

    if (*top == -1){
        cout << MERAH << "(Tidak ada riwayat yang bisa dihapus, ketuk enter untuk kembali)";
        cin.get();
        return;
    }
    hapusRiwayat->battle = riwayatBattle[*top].battle;
    hapusRiwayat->pemenang = riwayatBattle[*top].pemenang;
    hapusRiwayat->ovr = riwayatBattle[*top].ovr;

    cout << KUNING;
    cout << "=================== RIWAYAT DIHAPUS ===================\n\n";

    cout << setfill('=') << setw(101) << "=" << endl;
    cout << setfill(' ');

    cout << "| " << left << setw(4) << "No"
        << "| " << setw(45) << "Battle"
        << "| " << setw(25) << "Pemenang"
        << "| " << setw(18) << "OVR"
        << "|" << endl;

    cout << setfill('-') << setw(101) << "-" << endl;
    cout << setfill(' ');

    cout << "| " << left << setw(4) << 1

        << "| " << setw(45)
        << hapusRiwayat->battle

        << "| " << setw(25)
        << hapusRiwayat->pemenang

        << "| " << setw(18)
        << hapusRiwayat->ovr

        << "|" << endl;

    cout << setfill('=') << setw(101) << "=" << endl;
    cout << setfill(' ');

    (*top)--;

    enterKembali();
}

void tampilRiwayat(stack riwayatBattle[], int top){
    cls();

    if (top == -1){
        cout << MERAH << "(Belum ada riwayat, ketuk enter untuk kembali)";
        cin.get();
        return;
    }
    cout << KUNING;
    cout << "=================== RIWAYAT BATTLE ===================\n\n";

    cout << setfill('=') << setw(101) << "=" << endl;
    cout << setfill(' ');

    cout << "| " << left << setw(4) << "No"
        << "| " << setw(45) << "Battle"
        << "| " << setw(25) << "Pemenang"
        << "| " << setw(18) << "OVR"
        << "|" << endl;

    cout << setfill('-') << setw(101) << "-" << endl;
    cout << setfill(' ');

    int nomor = 1;

    for (int i = top; i >= 0; i--){

        cout << "| " << left << setw(4)
            << nomor++

            << "| " << setw(45)
            << riwayatBattle[i].battle

            << "| " << setw(25)
            << riwayatBattle[i].pemenang

            << "| " << setw(18)
            << riwayatBattle[i].ovr

            << "|" << endl;
    }

    cout << setfill('=') << setw(101) << "=" << endl;
    cout << setfill(' ');

    enterKembali();
}

double hitungSkor(stat &s){
    return (s.ap * 0.30) + 
           (s.sp * 0.20) + 
           (s.du * 0.35) + 
           (s.iq * 0.15);
}

void efekLoading(string teks){
    cout << teks;
    for (int i = 0; i < 3; i++) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    cout << endl;
}

void duel(karakter& a, karakter& b, stack riwayatBattle[], int* top){
    srand(time(0));
    double skorA = hitungSkor(a.stats);
    double skorB = hitungSkor(b.stats);

    skorA *= (0.96 + (rand() % 5) / 100.0);
    skorB *= (0.96 + (rand() % 5) / 100.0);
    
    cout << OREN;
    cout << "\n=====================================\n";
    cout << " ARENA DUEL ANIME \n";
    cout << "=====================================\n\n";

    cout << ">> " << a.nama << " (" << a.ket << ") dari " << a.anime << endl;
    cout << "   VS\n";
    cout << ">> " << b.nama << " (" << b.ket << ") dari " << b.anime << endl;

    cout << "\n=====================================\n";
    efekLoading("Pertarungan dimulai");

    cout << "\n  Statistik Duel  \n";
    cout << a.nama << " [AP:" << a.stats.ap << " SP:" << a.stats.sp 
        << " DU:" << a.stats.du << " IQ:" << a.stats.iq << "]\n";
    cout << b.nama << " [AP:" << b.stats.ap << " SP:" << b.stats.sp 
        << " DU:" << b.stats.du << " IQ:" << b.stats.iq << "]\n";

    cout << "\n  Skor Pertarungan  \n";
    cout << a.nama << ": " << skorA << endl;
    cout << b.nama << ": " << skorB << endl;

        string battle;
        string pemenang;
        double ovr;
        battle = a.nama + " VS " + b.nama;
        if (skorA > skorB) {
            cout << " Pemenang: " << a.nama << " !!!" << endl;
            pemenang = a.nama;
            ovr = skorA;

        } else if (skorB > skorA) {
            cout << " Pemenang: " << b.nama << " !!!" << endl;
            pemenang = b.nama;
            ovr = skorB;

        } else {
            cout << " Hasil seri! Pertarungan berakhir imbang." << endl;
            pemenang = "DRAW";
        }
        push(riwayatBattle, top, battle, pemenang, ovr);
    cout << "=====================================" << endl;
}

void battleKarakter(karakter karakterAnime[], int& jumlah_karakter, stack riwayatBattle[], int* top){
    int pilihan1, pilihan2;
    cls();

    try {
        cout << OREN << "Pilih karakter pertama (nomor): " << RESET;
        if (!(cin >> pilihan1)){
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("(Input harus berupa angka, ketuk enter untuk kembali)");
        }
        
        if (cin.peek() != '\n' && !cin.eof()){
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("(Input tidak boleh desimal/huruf, ketuk enter untuk kembali)");
        }

        cout << OREN << "Pilih karakter kedua (nomor): " << RESET;
        if (!(cin >> pilihan2)){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("(Input harus berupa angka, ketuk enter untuk kembali)");
        }

        if (cin.peek() != '\n' && !cin.eof()){
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("(Input tidak boleh desimal/huruf, ketuk enter untuk kembali)");
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (pilihan1 >= 1 && pilihan1 <= jumlah_karakter &&
            pilihan2 >= 1 && pilihan2 <= jumlah_karakter &&
            pilihan1 != pilihan2) {
            duel(karakterAnime[pilihan1-1], karakterAnime[pilihan2-1], riwayatBattle, top);
            enterKembali();
        } else {
            cout << MERAH << "(Maaf, input tidak valid)";
            cin.get();
        }
    }

    catch (exception& e){
        cout << MERAH << e.what();
        cin.get();
    }
}

int main(){
    akun pengguna[max_akun] = {{"mahdi", "067", "biasa"}, {"andi", "123", "premium"}};
    stack riwayatBattle[100]; 
    stack hapusRiwayat;
    int top = -1;
    karakter karakterAnime[max_karakter] = {
        {2501, "Megumi Hayashida", "Crows", {100, 92, 100, 88}, "Rindaman / Tak Terkalahkan"},
        {2502, "Bouya Harumichi", "Crows", {99, 94, 98, 85}, "Padiko / Serigala Penyendiri"},
        {2503, "Guriko Hanaki", "Worst", {98, 98, 95, 87}, "Iblis dari Kyushu"},
        {2504, "Hana Tsukishima", "Worst", {97, 91, 100, 90}, "Legenda Umehoshi"},
        {2505, "Tatsuo Kuzugami", "Crows", {99, 90, 97, 84}, "Pria Terkuat Manji Empire"},
        {2506, "Genji Takiya", "Crows Zero", {95, 92, 97, 82}, "Pemimpin GPS"},
        {2507, "Tamao Serizawa", "Crows Zero", {96, 90, 96, 88}, "Raja Suzuran (Zero)"},
        {2508, "Bitou Tatsuya", "Crows", {93, 98, 90, 90}, "Pembunuh Elit Housen"},
        {2509, "Ryushin Kunou", "Crows", {94, 92, 94, 86}, "Kepala TFOA Gen-4"},
        {2510, "Tesshou Kawachi", "Worst", {94, 91, 94, 87}, "Kepala TFOA Gen-6"},
        {2511, "King Joe", "Worst", {94, 88, 96, 88}, "Kanayama Shoji / Bos Housen"},
        {2512, "Zetton", "Worst", {93, 92, 94, 86}, "Hidetoshi Hanazawa"},
        {2513, "Mitsumasa Tsukimoto", "Worst", {91, 93, 90, 88}, "Housen Brother / Terkuat"},
        {2514, "Kousei Takeda", "Crows", {92, 90, 93, 89}, "Kepala TFOA Gen-5"},
        {2515, "Bulldog", "Crows", {96, 79, 99, 90}, "Yasushi Furukawa"},
        {2516, "Amachi Hisashi", "Worst", {96, 87, 97, 85}, "Sang Tiran dari Rindow"},
        {2517, "Takumi Fujishiro", "Worst", {90, 94, 89, 97}, "Otak Jenius TFOA"},
        {2518, "Masa Shioda", "Worst", {88, 85, 90, 83}, "Anggota Inti TFOA"},
        {2519, "Hosei Tsukimoto", "Worst", {89, 92, 88, 86}, "Tsukimoto Brother / Petarung"},
        {2520, "Shoji Rikiya", "Crows Zero", {91, 86, 93, 82}, "Benteng Serizawa Army"},

        {2521, "Manjiro Sano", "Tokyo Revengers", {100, 100, 95, 86}, "Invincible Mikey"},
        {2522, "Izana Kurokawa", "Tokyo Revengers", {97, 100, 92, 88}, "Pemimpin Tenjiku"},
        {2523, "Terano South", "Tokyo Revengers", {100, 93, 100, 83}, "Unrivaled / Rokuhara"},
        {2524, "Ken Ryuguji", "Tokyo Revengers", {97, 91, 99, 91}, "Draken / Wakil Toman"},
        {2525, "Senju Kawaragi", "Tokyo Revengers", {93, 100, 88, 89}, "Brahman No. 1"},
        {2526, "Taiju Shiba", "Tokyo Revengers", {98, 87, 98, 85}, "Black Dragon Gen-10"},
        {2527, "Keisuke Baji", "Tokyo Revengers", {94, 93, 95, 86}, "Kapten Divisi 1 Pertama"},
        {2528, "Wakasa Imaushi", "Tokyo Revengers", {91, 100, 87, 91}, "The White Leopard"},
        {2529, "Keizo Arashi", "Tokyo Revengers", {97, 85, 99, 86}, "Benkei / Black Dragon"},
        {2530, "Kakucho", "Tokyo Revengers", {96, 97, 95, 87}, "Hitto Kakucho / Tenjiku"},
        {2531, "Yasuhiro Muto", "Tokyo Revengers", {95, 85, 96, 86}, "Mucho / Kapten Divisi 5"},
        {2532, "Shuji Hanma", "Tokyo Revengers", {94, 93, 100, 89}, "The Zombie / Reaper"},
        {2533, "Takemichi Hanagaki", "Tokyo Revengers", {75, 75, 100, 94}, "Hero / Daya Tahan Mutlak"},
        {2534, "Chifuyu Matsuno", "Tokyo Revengers", {89, 91, 90, 92}, "Wakil Divisi 1 Setia"},
        {2535, "Mitsuya Takashi", "Tokyo Revengers", {91, 92, 92, 95}, "Kapten Divisi 2 / Kreatif"},
        {2536, "Souya Kawata", "Tokyo Revengers", {97, 96, 91, 83}, "Angry / Blue Ogre Mode"},
        {2537, "Kazutora Hanemiya", "Tokyo Revengers", {91, 93, 90, 84}, "Pendiri Toman / Valhalla"},
        {2538, "Ran Haitani", "Tokyo Revengers", {89, 92, 88, 90}, "Haitani Brother (Kakak)"},
        {2539, "Rindo Haitani", "Tokyo Revengers", {88, 93, 87, 89}, "Haitani Brother (Adik)"},
        {2540, "Kokonoi Hajime", "Tokyo Revengers", {83, 89, 85, 99}, "Koko / Jenius Finansial"},

        {2541, "Kohaku", "High & Low", {99, 94, 98, 92}, "Mugen Leader / Legenda"},
        {2542, "Amamiya Masaki", "High & Low", {99, 96, 97, 89}, "Amamiya Brother / Karate"},
        {2543, "Amamiya Hiroto", "High & Low", {98, 98, 95, 90}, "Amamiya Brother / Boxing"},
        {2544, "Cobra", "High & Low", {96, 95, 97, 91}, "Sannoh Rengokai Leader"},
        {2545, "Murayama Yoshiki", "High & Low", {93, 95, 100, 88}, "Oya High Full-Time Boss"},
        {2546, "Smoky", "High & Low", {91, 100, 86, 95}, "Rude Boys Leader"},
        {2547, "Hyuga Norihisa", "High & Low", {95, 92, 98, 85}, "Daruma Ikka Leader"},
        {2548, "Rocky", "High & Low", {93, 90, 96, 93}, "White Rascals Leader"},
        {2549, "Ice", "High & Low", {97, 95, 95, 89}, "Mighty Warriors Leader"},
        {2550, "Jesse", "High & Low", {94, 100, 91, 92}, "Prison Gang / Funky Boy"},
        {2551, "Lao", "High & Low", {100, 89, 100, 87}, "Raoh / Suzuran's Strongest"},
        {2552, "Fujio Hanaoka", "High & Low", {90, 93, 96, 88}, "Oya High Part-Time Leader"},
        {2553, "Sachio Ueda", "High & Low", {95, 96, 94, 93}, "Housen Academy Leader"},
        {2554, "Todoroki Yosuke", "High & Low", {92, 94, 93, 97}, "Oya High Strategic Ace"},
        {2555, "Amagai Kohei", "High & Low", {93, 92, 93, 91}, "Senomon High / Blood Gate"},
        {2556, "Yukinojo Odajima", "High & Low", {90, 95, 89, 94}, "Housen Shitenno / Analis"},
        {2557, "Mercy", "High & Low", {95, 86, 97, 85}, "Tangan Kanan Rocky"},
        {2558, "Phoenix", "High & Low", {93, 97, 92, 90}, "Sawayama / Housen Shitenno"},
        {2559, "Tsukasa Takajo", "High & Low", {90, 91, 91, 90}, "Oya High / Partner Fujio"},
        {2560, "Jamuo", "High & Low", {70, 75, 78, 95}, "Informan Setia Oya High"},

        {2561, "Ricardo Martinez", "Hajime no Ippo", {100, 100, 100, 100}, "Dewa Tinju / Juara Dunia"},
        {2562, "Mamoru Takamura", "Hajime no Ippo", {100, 98, 100, 95}, "6-Class Conqueror"},
        {2563, "Makunouchi Ippo", "Hajime no Ippo", {99, 89, 100, 87}, "Wind God / Retired"},
        {2564, "Ichiro Miyata", "Hajime no Ippo", {91, 100, 85, 96}, "Thunderbolt / Counter King"},
        {2565, "Alexander Volg Zangief", "Hajime no Ippo", {97, 97, 95, 98}, "Juara Dunia IBF"},
        {2566, "Takeshi Sendo", "Hajime no Ippo", {99, 90, 98, 87}, "Naniwa Tiger / Smasher"},
        {2567, "Ryo Mashiba", "Hajime no Ippo", {96, 95, 92, 94}, "Grim Reaper / Flicker"},
        {2568, "Sawamura Ryuhei", "Hajime no Ippo", {97, 96, 91, 95}, "Jenius Counter / Owari"},
        {2569, "David Eagle", "Hajime no Ippo", {96, 95, 97, 98}, "The Golden Eagle"},
        {2570, "Bryan Hawk", "Hajime no Ippo", {100, 98, 94, 83}, "Wilderness Genius"},
        {2571, "Eiji Date", "Hajime no Ippo", {95, 94, 93, 96}, "Heart Break Shot / Legend"},
        {2572, "Alfredo Gonzales", "Hajime no Ippo", {96, 96, 92, 94}, "Grim Reaper / Metzli"},
        {2573, "Wally", "Hajime no Ippo", {89, 100, 86, 96}, "The Nature's Child"},
        {2574, "Itagaki Manabu", "Hajime no Ippo", {85, 100, 83, 93}, "Chronos / Speed Star"},
        {2575, "Randy Boy Jr", "Hajime no Ippo", {95, 93, 94, 91}, "Asura / Switch Hitter"},
        {2576, "Sanada Kazuki", "Hajime no Ippo", {89, 91, 88, 99}, "Medal Tinju / Dokter"},
        {2577, "Shimabukuro Iwao", "Hajime no Ippo", {94, 81, 97, 85}, "Penyelam Karang / Tank"},
        {2578, "Jason Ozuma", "Hajime no Ippo", {93, 89, 91, 83}, "Hook Spesialis / Tulus"},
        {2579, "Saeki Akira", "Hajime no Ippo", {84, 99, 82, 92}, "Speed Star Orisinil"},
        {2580, "Aoki Masaru", "Hajime no Ippo", {82, 80, 90, 91}, "Raja Trik / Frog Punch"},

        {2581, "Hajime Umesamiya", "Wind Breaker", {98, 95, 98, 96}, "Pemimpin Bofurin"},
        {2582, "Chika Takiishi", "Wind Breaker", {100, 99, 96, 89}, "The Noroshi Calamity"},
        {2583, "Endo", "Wind Breaker", {96, 98, 93, 94}, "Tangan Kanan Takiishi"},
        {2584, "Haruka Sakura", "Wind Breaker", {93, 97, 91, 89}, "Pemimpin Kelas Tahun 1"},
        {2585, "Hayato Suo", "Wind Breaker", {90, 99, 89, 98}, "Ahli Beladiri Kungfu"},
        {2586, "Kyotaro Sugishita", "Wind Breaker", {96, 91, 97, 85}, "Anjing Gila / Umesamiya"},
        {2587, "Jo Togame", "Wind Breaker", {96, 93, 96, 91}, "Mantan Shishitoren No. 2"},
        {2588, "Choji Tomiyama", "Wind Breaker", {94, 100, 89, 87}, "Pemimpin Shishitoren"},
        {2589, "Toma Hiragi", "Wind Breaker", {94, 92, 95, 94}, "Bofurin Shitenno / Katana"},
        {2590, "Tasuku Tsubakino", "Wind Breaker", {91, 98, 89, 93}, "Bofurin Shitenno / Dancer"},
        {2591, "Ren Kaji", "Wind Breaker", {92, 93, 91, 90}, "Pemimpin Tahun 2 / Headphone"},
        {2592, "Mitsuki Kiryu", "Wind Breaker", {89, 94, 89, 95}, "Ahli Game & Jarak Jauh"},
        {2593, "Kotaro Sako", "Wind Breaker", {91, 92, 92, 89}, "Furin Thn 1 / Petarung Stik"},
        {2594, "Hyoma Chigiri", "Wind Breaker", {88, 95, 87, 92}, "Furin Thn 1 / Kecepatan"},
        {2595, "Taiga Tsugeura", "Wind Breaker", {95, 80, 96, 82}, "Furin Thn 1 / Otot Besar"},
        {2596, "Akihiko Nirei", "Wind Breaker", {60, 70, 65, 99}, "Database & Analis Data"},
        {2597, "Arima", "Wind Breaker", {89, 90, 89, 88}, "Anggota Inti Geng KEEL"},
        {2598, "Kanuma", "Wind Breaker", {90, 91, 90, 87}, "Anggota Inti Geng KEEL"},
        {2599, "Anzai", "Wind Breaker", {85, 86, 84, 85}, "Anggota Pendukung Furin"},
        {2600, "Nagame", "Wind Breaker", {86, 88, 85, 87}, "Anggota Pendukung Furin"}
    };

        string iklanA = R"(==========================================================================


   /\
  /  \
  \  /                                                      _________
 ./  \.      NASI CUMI HITAM MADURA PAK KRIS              /           \
  \__/  Specialis Nasi Cumi Hitam BERTELOR Asli Madura   |   ~ ~ ~ ~   |
  /||\     instagram @cumipakkris||@nasicumi             |   ~ ~ ~ ~   |
 / || \                                                   \___________/
   ||
   ||


==========================================================================
)";

    string iklanB = R"(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
       .........
    ...         ...
   .               .
  .                 .
 .        ...        .
.        .   .        .     Kipas angin TURBO JET 2000
.       .     .       .         dengan kemampuan dingin maksimal
.        .   .        .             sensasi ditiup angin mesin jet
 .        ...        .                  mendinginkan ruangan besar dan luas
  .                 .
   .               .
    ...         ...
       .........
           .
           .
           .                SEGERA BELI SEBELUM KEHABISAN
           .                    HANYA DI BAGO GO SHOP
           .
          . .
         .   .
         .   .
        .     .
       .........

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
)";

    string iklanC = R"(

                 _______________________________
             .-''                               ''-.
          .-'                                       '-.
        .'                                             '.
       /                                                 \
      /                                                   \
     |     ___________________________________________     |
     |    /                                           \    |
     |   /                                             \   |
     |  |        ===============================        |  |    =============================================
     |  |        |                             |        |  |     | PARTAI FINAL UEFA CHAMPIONS LEAGUE 2026 |
     |  |        |          PUSKAS             |        |  |     |   BAYERN MUNCHEN VS ATLETICO MADRID     |
     |  |        |          ARENA              |        |  |     |              30 MEI 2026                |
     |  |        |                             |        |  |     |    PUSKAS ARENA BUDAPHEST HUNGARIA      |
     |  |        ===============================        |  |
     |   \                                             /   |
     |    \___________________________________________/    |
      \                                                   /
       \                                                 /
        '.                                             .'
          '-.                                       .-'
             ''-._______________________________.-'' 

)";

    string iklanD = R"(
+-------------------------------------------+
|         +=======================+         |   SEGERA HADIR
|         |    METEORS BLASTER    |         |   Brand new Plane Shooter Game
|         +=======================+         |
|                                           |
|                   XXX                     |
|                  X   X            XXX     |   - Tampilan yang catchy
|                   XXX            X   X    |   - Gameplay yang unik
|      XXX                          XXX     |   - Temukan berbagai perlengkapan unik
|     X   X                                 |   - Berbagai Upgrade dan Kreasi pesawat
|      XXX                                  |   - dan Masih banyak lagi
|                :    _    :                |
|                +   | |   +                |
|              /==#########==\              |
|             |_______________|             |   Jangan lupa tambahkan ke wishlist kalian
|                    | |                    |   :)
|                    /|\                    |
|                   /_|_\                   |
|                                           |
|/=========================================\|
||              Coming Soon                ||
+\=========================================/+
)";

    string iklanE = R"(
..........................................................
.                                                        .
.                                                        .  Bosan
.           ________________________________             .  Butuh hiburan
.          |                                |            .  Nyari yang asik
.          | xx      x         x       x    |            .  
.          |   xx  xx           xx   xx     |            .  Dengerin lagu-lagu Marshmello
.          |     xx               xxx       |            .  diberbagai kanal mendengar musik
.          |   xx  xx            xx xx      |            .  Hiburan yang asik
.          | xx      x         xx     x     |            .  Mendampingi kesibukan kalian
.          |                                |            .  Menghilangkan kebosanan kalian
.          |                                |            .  
.          |                                |            .  Karena itu jangan lupa dengerin
.          |     \                  /       |            .  "Marshmello" dengan berbagai musiknya 
.          |      \                /        |            .  yang asik dan menyenangkan
.          |       \______________/         |            .
.          |                                |            .    x   x    X   X     X   X
.          |________________________________|            .    \___/    \___/     \___/
.                                                        .
.                                                        .
..........................................................
)";

    string menu_login = R"(=====================================
|           LOGIN DULU YAA          |
=====================================
| 1) Login                          |
| 2) Register                       |
| 0) Keluar                         |
=====================================
)";

    string menu_biasa = R"(=================================
|        MAU NGAPAIN CUY?       |
=================================
| 1) Read                       |
| 2) Sorting                    |
| 3) Searching                  |
| 4) Battle                     |
| 0) Keluar                     |
=================================
)";

    string menu_premium = R"(=================================
|        MAU NGAPAIN CUY?       |
=================================
| 1) Read                       |
| 2) Create                     |
| 3) Update                     |
| 4) Delete                     |
| 5) Sorting                    |
| 6) Searching                  |
| 7) Battle                     |
| 8) Riwayat Battle             |
| 9) Hapus Riwayat              |
| 0) Keluar                     |
=================================
)";

    string menu_sorting = R"(=================================
|          MENU SORTING         |
=================================
| 1) By Nama Karakter           |
| 2) By Nama Seri/Anime         |
| 3) By Attack Power            |
| 4) By Speed                   |
| 5) By Durability              |
| 6) By IQ                      |
| 0) Kembali                    |
=================================
)";

    string menu_searching = R"(=================================
|         MENU SEARCHING        |
=================================
| 1) By Nama Karakter           |
| 2) By ID karakter             |
| 0) Kembali                    |
=================================
)";

    string header_loginUser = R"(============================
|           LOGIN          |
============================)";

    string header_register = R"(============================
|         Register         |
============================)";

    string akhir_program = R"(===================================================
 ____   _   _   _____   _____   _   _   _____   _ 
 / ___| | | | || ____||_   _|| | | || ____|| |     
 \\___ \\ | | | ||  _|    | |  | |_| ||  _|  | |   
  ___) || |_| || |___   | |  |  _  || |___ |_|     
 |____/  \\___/ |_____|  |_|  |_| |_||_____|(_)    
                                                   
            PROGRAM BERHASIL DISELESAIKAN          
===================================================)";
    
    string arrayIklan[5] = {iklanA, iklanB, iklanC, iklanD, iklanE};
    int index_pengguna;
    int panjang_karakter = 100;
    int panjang_user = 2;
    int sisa_login = 3;
    int pilihan_1, pilihan_2, pilihan_3, pilihan_4;
    bool token_login = true;
    bool token_utama = false;
    bool token_sorting = false;
    bool token_searching = false;
    srand(time(0));

    while (token_login && sisa_login > 0){
        tampilkan(menu_login, 0, 3);
        cin >> pilihan_1;
        
        try{
            if (cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("(Input harus berupa angka, ketuk enter untuk kembali)");
            }

            if (cin.peek() != '\n' && !cin.eof()){
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("(Input tidak boleh desimal/huruf, ketuk enter untuk kembali)");
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch (pilihan_1){
                case 0:
                    token_login = false;
                    break;
                
                case 1:
                    index_pengguna = loginUser(pengguna, sisa_login, token_utama, token_login, panjang_user, header_loginUser);
                    break;
                
                case 2:
                    registerUser(pengguna, header_register, &panjang_user);
                    break;
            
            default:
                throw runtime_error("(Input tidak valid, ketuk enter untuk kembali)");
            }   
        } 
        
        catch (exception& e){
            cout << MERAH << e.what();
            cin.get();
        }
    }
    
    if (pengguna[index_pengguna].status == "biasa"){
        putarAnimasi();
        while (token_utama){
            tampilkan(menu_biasa, 0, 4);
            cin >> pilihan_2;
            
            try{
                if (cin.fail()){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("(Input harus berupa angka, ketuk enter untuk kembali)");
                }
    
                if (cin.peek() != '\n' && !cin.eof()){
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("(Input tidak boleh desimal/huruf, ketuk enter untuk kembali)");
                }
                
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                switch (pilihan_2){
                    case 0:
                        iklan(arrayIklan, 5);
                        token_utama = false;
                        break;
        
                    case 1:
                        iklan(arrayIklan, 5);
                        tampilKarakter(karakterAnime, panjang_karakter);
                        break;
        
                    case 2:
                        token_sorting = true;
                        while (token_sorting){
                            tampilkan(menu_sorting, 0, 6);
                            cin >> pilihan_3;
                            
                            try{
                                if (cin.fail()){
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    throw invalid_argument("(Input harus berupa angka, ketuk enter untuk kembali)");
                                }
                                
                                if (cin.peek() != '\n' && !cin.eof()){
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    throw invalid_argument("(Input tidak boleh desimal/huruf, ketuk enter untuk kembali)");
                                };
                                
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                switch (pilihan_3){
                                    case 0:
                                        token_sorting = false;
                                        break;
                                    case 1:
                                        iklan(arrayIklan, 5);
                                        bubleSort(karakterAnime, "nama", panjang_karakter);
                                        token_sorting = false;
                                        break;
                                    case 2:
                                        iklan(arrayIklan, 5);
                                        insertionSort(karakterAnime, "anime", panjang_karakter);
                                        token_sorting = false;
                                        break;
                                    case 3:
                                        iklan(arrayIklan, 5);
                                        selectionSort(karakterAnime, "AP", panjang_karakter);
                                        token_sorting = false;
                                        break;
                                    case 4:
                                        iklan(arrayIklan, 5);
                                        selectionSort(karakterAnime, "SP", panjang_karakter);
                                        token_sorting = false;
                                        break;
                                    case 5:
                                        iklan(arrayIklan, 5);
                                        selectionSort(karakterAnime, "DU", panjang_karakter);
                                        token_sorting = false;
                                        break;
                                    case 6:
                                        iklan(arrayIklan, 5);
                                        selectionSort(karakterAnime, "IQ", panjang_karakter);
                                        token_sorting = false;
                                        break;
                                    default:
                                        throw runtime_error("(Input tidak valid, ketuk enter untuk kembali)");
                                }   
                            }
    
                            catch (exception& e){
                                cout << MERAH << e.what();
                                cin.get();
                            }
                        }
                        break;
        
                    case 3:
                        token_searching = true;
                        while (token_searching){
                            tampilkan(menu_searching, 0, 2);
                            cin >> pilihan_4;
                            
                            try{
                                if (cin.fail()){
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    throw invalid_argument("(Input harus berupa angka, ketuk enter untuk kembali)");
                                }
                                
                                if (cin.peek() != '\n' && !cin.eof()){
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    throw invalid_argument("(Input tidak boleh desimal/huruf, ketuk enter untuk kembali)");
                                }
                                
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                switch (pilihan_4){
                                    case 0:
                                        iklan(arrayIklan, 5);
                                        token_searching = false;
                                        break;
                                    case 1:
                                        iklan(arrayIklan, 5);
                                        linearSearch(karakterAnime, &panjang_karakter);
                                        token_searching = false;
                                        break;
                                    case 2:
                                        iklan(arrayIklan, 5);
                                        fibonacciSearch(karakterAnime, &panjang_karakter);
                                        token_searching = false;
                                        break;
                                    default:
                                        throw runtime_error("(Input tidak valid, ketuk enter untuk kembali)");
                                } 
                            }
    
                            catch (exception& e){
                                cout << MERAH << e.what();
                                cin.get();
                            }
                        }
                        break;
        
                case 4:
                    iklan(arrayIklan, 5);
                    battleKarakter(karakterAnime, panjang_karakter, riwayatBattle, &top);
                    break;

                default:
                    throw runtime_error("(Input tidak valid, ketuk enter untuk kembali)");
                }
            }
    
            catch (exception& e){
                cout << MERAH << e.what();
                cin.get();
            }
        }
    
    } else if (pengguna[index_pengguna].status == "premium"){
        putarAnimasi();
        while (token_utama){
            tampilkan(menu_premium, 0, 9);
            cin >> pilihan_2;

            try{
                if (cin.fail()){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("(Input harus berupa angka, ketuk enter untuk kembali)");
                }
    
                if (cin.peek() != '\n' && !cin.eof()){
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("(Input tidak boleh desimal/huruf, ketuk enter untuk kembali)");
                }
                
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                switch (pilihan_2){
                    case 0:
                        token_utama = false;
                        break;
        
                    case 1:
                        tampilKarakter(karakterAnime, panjang_karakter);
                        break;
                    
                    case 2:
                        tambahKarakter(karakterAnime, &panjang_karakter);
                        break;
                        
                    case 3:
                        perbaruiKarakter(karakterAnime, panjang_karakter);
                        break;
                        
                    case 4:
                        hapusKarakter(karakterAnime, &panjang_karakter);
                        break;
                        
                    case 5:
                        token_sorting = true;
                        while (token_sorting){
                            tampilkan(menu_sorting, 0, 6);
                            cin >> pilihan_3;
                            
                            try{
                                if (cin.fail()){
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    throw invalid_argument("(Input harus berupa angka, ketuk enter untuk kembali)");
                                }
    
                                if (cin.peek() != '\n' && !cin.eof()){
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    throw invalid_argument("(Input tidak boleh desimal/huruf, ketuk enter untuk kembali)");
                                }

                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                switch (pilihan_3){
                                    case 0:
                                        token_sorting = false;
                                        break;
                                    case 1:
                                        bubleSort(karakterAnime, "nama", panjang_karakter);
                                        token_sorting = false;
                                        break;
                                    case 2:
                                        insertionSort(karakterAnime, "anime", panjang_karakter);
                                        token_sorting = false;
                                        break;
                                    case 3:
                                        selectionSort(karakterAnime, "AP", panjang_karakter);
                                        token_sorting = false;
                                        break;
                                    case 4:
                                        selectionSort(karakterAnime, "SP", panjang_karakter);
                                        token_sorting = false;
                                        break;
                                    case 5:
                                        selectionSort(karakterAnime, "DU", panjang_karakter);
                                        token_sorting = false;
                                        break;
                                    case 6:
                                        selectionSort(karakterAnime, "IQ", panjang_karakter);
                                        token_sorting = false;
                                        break;
                                    default:
                                        throw runtime_error("(Input tidak valid, ketuk enter untuk kembali)");
                                }   
                            }
    
                            catch (exception& e){
                                cout << MERAH << e.what();
                                cin.get();
                            }
                        }
                        break;
        
                    case 6:
                        token_searching = true;
                        while (token_searching){
                            tampilkan(menu_searching, 0, 2);
                            cin >> pilihan_4;
                            
                            try{
                                if (cin.fail()){
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    throw invalid_argument("(Input harus berupa angka, ketuk enter untuk kembali)");
                                }
    
                                if (cin.peek() != '\n' && !cin.eof()){
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    throw invalid_argument("(Input tidak boleh desimal/huruf, ketuk enter untuk kembali)");
                                }
                                
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                switch (pilihan_4){
                                    case 0:
                                        token_searching = false;
                                        break;
                                    case 1:
                                        linearSearch(karakterAnime, &panjang_karakter);
                                        token_searching = false;
                                        break;
                                    case 2:
                                        fibonacciSearch(karakterAnime, &panjang_karakter);
                                        token_searching = false;
                                        break;
                                    default:
                                        throw runtime_error("(Input tidak valid, ketuk enter untuk kembali)");
                                } 
                            }
    
                            catch (exception& e){
                                cout << MERAH << e.what();
                                cin.get();
                            }
                        }
                        break;

                    case 7:
                        battleKarakter(karakterAnime, panjang_karakter, riwayatBattle, &top);                    
                        break;
                    
                    case 8:
                        tampilRiwayat(riwayatBattle, top);
                        break;

                    case 9:
                        pop(riwayatBattle, &hapusRiwayat, &top);
                        break;

                    default:
                        throw runtime_error("(Input tidak valid, ketuk enter untuk kembali)");
                }
            }
    
            catch (exception& e){
                cout << MERAH << e.what();
                cin.get();
            }
        }
    }

    cls();
    tampilkan(akhir_program);
    return 0;
}