#include <iostream>

using namespace std;

struct nodeQueue
{
    int nota;
    string nama;
    nodeQueue *next;
};

struct nodeStack
{
    int nota;
    string nama;
    nodeStack *next;
};

struct nodeTree
{
    int nota;
    string nama;
    nodeTree *kiri;
    nodeTree *kanan;
};

nodeQueue *depan = nullptr;
nodeQueue *terakhir = nullptr;
nodeStack *top = nullptr;
nodeTree *root = nullptr;

// tambahkan node baru ke pohon berdasarkan nomor nota.
// nota lebih kecil masuk ke kiri, nota lebih besar masuk ke kanan.
// mengembalikan akar pohon setelah penambahan.
nodeTree *tambah_tree(nodeTree *akar, int nota, string nama)
{
    if (akar == nullptr)
    {
        return new nodeTree{nota, nama, nullptr, nullptr};
    }
    if (nota < akar->nota)
    {
        akar->kiri = tambah_tree(akar->kiri, nota, nama);
    }
    else
    {
        akar->kanan = tambah_tree(akar->kanan, nota, nama);
    }
    return akar;
}

// mencari node di pohon berdasarkan nomor nota.
// mengembalikan node yang ditemukan, atau nullptr jika tidak ada.
nodeTree *cari_tree(nodeTree *akar, int nota)
{
    if (akar == nullptr || akar->nota == nota)
    {
        return akar;
    }
    if (nota < akar->nota)
    {
        return cari_tree(akar->kiri, nota);
    }
    return cari_tree(akar->kanan, nota);
}

// menghapus node di pohon berdasarkan nomor nota.
// harus menangani 3 kasus: tanpa anak, satu anak, dan dua anak.
// mengembalikan akar pohon setelah penghapusan.
nodeTree *hapus_tree(nodeTree *akar, int nota)
{
    if (akar == nullptr)
    {
        return nullptr;
    }
    if (nota < akar->nota)
    {
        akar->kiri = hapus_tree(akar->kiri, nota);
    }
    else if (nota > akar->nota)
    {
        akar->kanan = hapus_tree(akar->kanan, nota);
    }
    else if (akar->kiri == nullptr)
    {
        nodeTree *sisa = akar->kanan;
        delete akar;
        return sisa;
    }
    else if (akar->kanan == nullptr)
    {
        nodeTree *sisa = akar->kiri;
        delete akar;
        return sisa;
    }
    else
    {
        nodeTree *pengganti = akar->kanan;
        while (pengganti->kiri != nullptr)
        {
            pengganti = pengganti->kiri;
        }
        akar->nota = pengganti->nota;
        akar->nama = pengganti->nama;
        akar->kanan = hapus_tree(akar->kanan, pengganti->nota);
    }
    return akar;
}

// menampilkan isi pohon dengan urutan: node, kiri, kanan.
// parameter tidak dipakai sebagai nomor urut tampilan.
void preorder(nodeTree *akar, int &no)
{
    if (akar == nullptr)
    {
        return;
    }
    cout << no << ". [" << akar->nota << "] " << akar->nama << endl;
    no++;
    preorder(akar->kiri, no);
    preorder(akar->kanan, no);
}

// menampilkan isi pohon dengan urutan: kiri, node, kanan.
// hasilnya akan terurut berdasarkan nomor nota.
void inorder(nodeTree *akar, int &no)
{
    if (akar == nullptr)
    {
        return;
    }
    inorder(akar->kiri, no);
    cout << no << ". [" << akar->nota << "] " << akar->nama << endl;
    no++;
    inorder(akar->kanan, no);
}

// menampilkan isi pohon dengan urutan: kiri, kanan, node.
void postorder(nodeTree *akar, int &no)
{
    if (akar == nullptr)
    {
        return;
    }
    postorder(akar->kiri, no);
    postorder(akar->kanan, no);
    cout << no << ". [" << akar->nota << "] " << akar->nama << endl;
    no++;
}

// meminta input nomor nota dan nama cucian, lalu masukkan ke antrian (posisi paling belakang).
//  error handling: nota bukan angka atau kurang dari 1, nota sudah dipakai
//  (cek di antrian dan tumpukan), dan nama kosong.
void tambah_cucian()
{
    int nota = 0;
    string nama = "";
    
    cout << "Masukkan nomor nota: ";
    cin >> nota;
    
    if (cin.fail() || nota <= 0)
    {
        cout << "Nomor nota harus berupa angka dan lebih dari 0!\n\n";
        cin.clear();
        cin.get();
        return;
    }
    
    nodeQueue *q = depan;
    
    while (q != nullptr)
    {
        if (q->nota == nota)
        {
            cout << "Nomor nota sudah digunakan!\n\n";
            return;
        }
        q = q->next;
    }
    
    nodeStack *s = top;
    
    while (s != nullptr)
    {
        if (s->nota == nota)
        {
            cout << "Nomor nota sudah digunakan!\n\n";
            return;
        }
        
        s = s->next;
    }
    
    cout << "Masukkan nama cucian: ";
    cin.ignore();
    getline(cin, nama);
    
    if (nama.empty())
    {
        cout << "Nama cucian tidak boleh kosong!\n";
        
        return;
    }
    
    nodeQueue *newNode = new nodeQueue{nota, nama, nullptr};
    if (depan == nullptr)
    {
        depan = newNode;
        terakhir = newNode;
    }
    else
    {
        terakhir->next = newNode;
        terakhir = newNode;
    }
    
    cout << "Berhasil memasukkan cucian!\n";
    return;
}

// menampilkan seluruh cucian di antrian dari depan sampai belakang.
// penanganan kesalahan jika antrian kosong.
void lihat_antrean()
{
    
    nodeQueue *q = depan;
    int i = 0;
    
    cout << "\n=== Cucian Dalam Antrean ===\n";
    while (q != nullptr)
    {
        i++;
        cout << i << ". [" << q->nota << "] " << q->nama << endl;
        
        q = q->next;
    }
    
    cout << endl;
    return;
}

// mengambil cucian paling depan dari antrian, lalu memindahkannya ke tumpukan (posisi paling atas)
// dan memasukkan ke pohon (rak). node antrian dihapus setelah dipindahkan.
// penanganan kesalahan jika antrian kosong.
void proses_cucian()
{
    if (depan == nullptr)
    {
        cout << "Antrean cucian masih kosong!!\n\n";
        return;
    }
    
    nodeQueue *hapus = depan;
    nodeStack *newNode = new nodeStack{hapus->nota, hapus->nama, nullptr};
    top = newNode;
    root = tambah_tree(root, hapus->nota, hapus->nama);
    cout << "Berhasil memproses cucian " << hapus->nama << "!" << endl
    << endl;
    depan = hapus->next;
    if (depan == nullptr)
    {
        terakhir = nullptr;
    }
    delete hapus;
}

// menampilkan seluruh cucian di tumpukan dari atas sampai bawah.
// penanganan kesalahan jika tumpukan kosong.
void lihat_riwayat()
{
    if (top == nullptr)
    {
        cout << "Belum ada cucian yang selesai!" << endl
        << endl;
        return;
    }
    cout << "=== Cucian Yang Sudah Selesai ===" << endl
    << endl;
    nodeStack *s = top;
    int no = 1;
    while (s != nullptr)
    {
        cout << no << ". [" << s->nota << "] " << s->nama << endl;
        s = s->next;
        no++;
    }
    cout << endl;
}

// meminta input nomor nota, lalu mencarinya di pohon (rak) dan menampilkan datanya.
// penanganan error jika rak kosong, input nota tidak valid, atau nota tidak ditemukan.
void cari_cucian()
{
    if (root == nullptr)
    {
        cout << "Rak masih kosong!" << endl
        << endl;
        return;
    }
    int nota;
    cout << "Masukkan nomor nota: ";
    cin >> nota;
    if (cin.fail() || nota <= 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor nota harus berupa angka lebih dari 0!" << endl
        << endl;
        return;
    }
    nodeTree *hasil = cari_tree(root, nota);
    if (hasil == nullptr)
    {
        cout << "Cucian dengan nota " << nota << " tidak ada di rak!" << endl
        << endl;
        return;
    }
    cout << "Ditemukan: [" << hasil->nota << "] " << hasil->nama << endl
    << endl;
}

// meminta pengguna memilih pre-order, in-order, atau post-order,
// lalu menampilkan isi pohon sesuai pilihan.
// penanganan kesalahan jika rak kosong atau pilihan tidak valid.
void lihat_rak()
{
    if (root == nullptr)
    {
        cout << "Rak masih kosong!" << endl
        << endl;
        return;
    }
    char pil;
    int no = 1;
    cout << "=== Isi Rak Cucian ===" << endl
    << endl;
    cout << "1. Pre-order" << endl;
    cout << "2. In-order" << endl;
    cout << "3. Post-order" << endl
    << endl;
    cout << "Masukkan pilihan anda: ";
    cin >> pil;
    cout << endl;
    switch (pil)
    {
        case '1':
        {
            preorder(root, no);
            break;
        }
        case '2':
        {
            inorder(root, no);
            break;
        }
        case '3':
        {
            postorder(root, no);
            break;
        }
        default:
        {
            cout << "Pilihan yang anda masukkan salah!" << endl;
        }
    }
    cout << endl;
}

// meminta input nomor nota cucian yang diambil pelanggan, lalu menghapusnya dari pohon (rak).
// data di tumpukan tidak ikut terhapus.
// penanganan error jika rak kosong, input nota tidak valid, atau nota tidak ditemukan.
void ambil_cucian()
{
    if (root == nullptr)
    {
        cout << "Rak masih kosong!" << endl
        << endl;
        return;
    }
    int nota;
    cout << "Masukkan nomor nota: ";
    cin >> nota;
    if (cin.fail() || nota <= 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor nota harus berupa angka lebih dari 0!" << endl
        << endl;
        return;
    }
    nodeTree *hasil = cari_tree(root, nota);
    if (hasil == nullptr)
    {
        cout << "Cucian dengan nota " << nota << " tidak ada di rak!" << endl
        << endl;
        return;
    }
    cout << "Cucian " << hasil->nama << " telah diambil pelanggan!" << endl
    << endl;
    root = hapus_tree(root, nota);
}

// menampilkan menu utama dan memanggil fungsi sesuai pilihan pengguna sampai pengguna memilih keluar.
// menu: tambah cucian (bisa diulang), lihat antrean, proses cucian, lihat riwayat,
// cari cucian, lihat isi rak, ambil cucian, dan keluar.
// penanganan kesalahan jika pilihan di luar menu.
int main()
{
    char menu = '0';
    while (menu != '8')
    {
        cout << "=== Manajemen Laundry Rani ===" << endl
        << endl;
        cout << "1. Tambah Cucian" << endl;
        cout << "2. Lihat Cucian Dalam Antrean" << endl;
        cout << "3. Proses Cucian" << endl;
        cout << "4. Lihat Cucian Yang Sudah Selesai" << endl;
        cout << "5. Cari Cucian Di Rak" << endl;
        cout << "6. Lihat Isi Rak" << endl;
        cout << "7. Ambil Cucian" << endl;
        cout << "8. Keluar" << endl
        << endl;
        cout << "Masukkan pilihan anda: ";
        cin >> menu;
        switch (menu)
        {
            case '1':
            {
                char tambahCucian = 'y';
                while (tambahCucian == 'y' || tambahCucian == 'Y')
                {
                    tambah_cucian();
                    cout << "Apakah anda ingin menambahkan cucian lagi? (y/n): ";
                    cin >> tambahCucian;
                }
                break;
            }
            case '2':
            {
                lihat_antrean();
                break;
            }
            case '3':
            {
                proses_cucian();
                break;
            }
            case '4':
            {
                lihat_riwayat();
                break;
            }
            case '5':
            {
                cari_cucian();
                break;
            }
            case '6':
            {
                lihat_rak();
                break;
            }
            case '7':
            {
                ambil_cucian();
                break;
            }
            case '8':
            {
                cout << "Program akan dikeluarkan.";
                exit(0);
            }
            default:
            {
                cout << "Pilihan yang anda masukkan salah!" << endl;
                cout << "Silahkan coba lagi." << endl
                << endl;
            }
        }
    }
}