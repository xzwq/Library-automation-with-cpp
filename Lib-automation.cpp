



#include <iostream> // giriş ve çıkış işlemleri için gerekli kütüphane
#include <fstream> // dosya işlemleri için gerekli kütüphane
#include <string> // string veri tipi için gerekli kütüphane
#include <limits> // numeric_limits kullanımı için gerekli kütüphane

using namespace std; // std isim alanını kullanarak kodu daha okunabilir hale getirir

const int MAX_KITAP = 100; // maksimum kitap sayısını belirten sabit

// kitap bilgilerini saklamak için yapı tanımı
struct Kitap {
    string ad; // kitap adı
    string yazar; // yazar adı
    int yayinYili; // yayın yılı
    string isbn; // isbn numarası
};

Kitap kitaplar[MAX_KITAP]; // kitap bilgilerini saklamak için dizi
int kitapSayisi = 0; // mevcut kitap sayısını tutan değişken

const string DOSYA_ADI = "kitaplar.txt"; // kitap bilgilerini saklamak için kullanılacak dosyanın adı

// fonksiyon prototipleri
void kitapEkle(); // kitap ekleme fonksiyonu
void kitapListele(); // kitap listeleme fonksiyonu
void kitapAra(); // kitap arama fonksiyonu
void kitapSil(); // kitap silme fonksiyonu
void kitapGuncelle(); // kitap güncelleme fonksiyonu
void dosyadanOku(); // dosyadan kitap bilgilerini okuma fonksiyonu
void dosyayaYaz(); // dosyaya kitap bilgilerini yazma fonksiyonu
bool isValidISBN(const string& isbn); // isbn numarasının geçerliliğini kontrol eden fonksiyon
bool dosyaVarMi(const string& dosyaAdi); // dosyanın var olup olmadığını kontrol eden fonksiyon

int main() {
    dosyadanOku(); // program başlarken dosyadan kitap bilgilerini okur
    
    while (true) { // sonsuz döngü, kullanıcı çıkana kadar devam eder
        cout << "\nKütüphane Yönetim Sistemi" << endl;
        cout << "1. Kitap Ekle" << endl;
        cout << "2. Kitap Listele" << endl;
        cout << "3. Kitap Ara" << endl;
        cout << "4. Kitap Sil" << endl;
        cout << "5. Kitap Güncelle" << endl;
        cout << "6. Çıkış" << endl;
        cout << "Seçiminizi yapın: ";
        
        int secim; // kullanıcıdan seçim almak için değişken
        cin >> secim; // kullanıcıdan seçim alır
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // giriş tamponunu temizler
        
        switch (secim) { // kullanıcı seçimine göre işlem yapar
            case 1:
                kitapEkle(); // kitap ekleme işlemi
                break;
            case 2:
                kitapListele(); // kitap listeleme işlemi
                break;
            case 3:
                kitapAra(); // kitap arama işlemi
                break;
            case 4:
                kitapSil(); // kitap silme işlemi
                break;
            case 5:
                kitapGuncelle(); // kitap güncelleme işlemi
                break;
            case 6:
                dosyayaYaz(); // programdan çıkmadan önce kitap bilgilerini dosyaya yazar
                cout << "Programdan çıkılıyor" << endl;
                return 0; // programı sonlandırır
            default:
                cout << "Geçersiz seçim, tekrar deneyin" << endl; // geçersiz seçim uyarısı
        }
    }
}

void dosyadanOku() {
    if (!dosyaVarMi(DOSYA_ADI)) { // dosya yoksa
        cout << "Kitap dosyası bulunamadı. Yeni dosya oluşturulacak." << endl;
        ofstream dosya(DOSYA_ADI); // yeni dosya oluşturur
        dosya.close(); // dosyayı kapatır
        return;
    }

    ifstream dosya(DOSYA_ADI); // dosyayı okuma modunda açar
    while (dosya >> ws && getline(dosya, kitaplar[kitapSayisi].ad)) { // dosyadan kitap bilgilerini okur
        getline(dosya, kitaplar[kitapSayisi].yazar); // yazar adını okur
        dosya >> kitaplar[kitapSayisi].yayinYili; // yayın yılını okur
        dosya.ignore(); // giriş tamponunu temizler
        getline(dosya, kitaplar[kitapSayisi].isbn); // isbn numarasını okur
        kitapSayisi++; // kitap sayısını artırır
    }
    
    dosya.close(); // dosyayı kapatır
}

void dosyayaYaz() {
    ofstream dosya(DOSYA_ADI); // dosyayı yazma modunda açar
    if (!dosya) { // dosya açılamazsa
        cout << "Dosya oluşturulamadı." << endl;
        return;
    }
    for (int i = 0; i < kitapSayisi; i++) { // tüm kitap bilgilerini dosyaya yazar
        dosya << kitaplar[i].ad << endl;
        dosya << kitaplar[i].yazar << endl;
        dosya << kitaplar[i].yayinYili << endl;
        dosya << kitaplar[i].isbn << endl;
    }
    dosya.close(); // dosyayı kapatır
    cout << "Kitap bilgileri dosyaya yazıldı." << endl;
}

void kitapEkle() {
    if (kitapSayisi >= MAX_KITAP) { // maksimum kitap sayısına ulaşıldıysa
        cout << "Maksimum kitap sayısına ulaştınız" << endl;
        return;
    }
    
    cout << "Kitap adı: ";
    getline(cin, kitaplar[kitapSayisi].ad); // kitap adını alır
    
    cout << "Yazar adı: ";
    getline(cin, kitaplar[kitapSayisi].yazar); // yazar adını alır
    
    cout << "Yayın yılı: ";
    while (!(cin >> kitaplar[kitapSayisi].yayinYili)) { // yayın yılını alır ve geçerli bir sayı olup olmadığını kontrol eder
        cout << "Geçersiz giriş. Lütfen bir sayı girin: ";
        cin.clear(); // giriş hatasını temizler
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // giriş tamponunu temizler
    }
    cin.ignore(); // giriş tamponunu temizler
   
    cout << "ISBN numarası (11 haneli): ";
    while (true) {
        getline(cin, kitaplar[kitapSayisi].isbn); // isbn numarasını alır
        if (isValidISBN(kitaplar[kitapSayisi].isbn)) { // isbn numarasının geçerli olup olmadığını kontrol eder
            break;
        } else {
            cout << "Geçersiz ISBN numarası. Lütfen 11 haneli bir sayı girin: ";
        }
    }
    
    kitapSayisi++; // kitap sayısını artırır
    dosyayaYaz(); // kitap bilgilerini dosyaya yazar
    cout << "Kitap eklendi." << endl;
}

void kitapListele() {
    if (kitapSayisi == 0) { // kitap yoksa
        cout << "Sistemde hiç kitap yok." << endl;
        return;
    }
    
    cout << "\nKayıtlı kitaplar:" << endl;
    for (int i = 0; i < kitapSayisi; i++) { // tüm kitap bilgilerini ekrana yazar
        cout << "Kitap adı: " << kitaplar[i].ad << endl;
        cout << "Yazar adı: " << kitaplar[i].yazar << endl;
        cout << "Yayın yılı: " << kitaplar[i].yayinYili << endl;
        cout << "ISBN numarası: " << kitaplar[i].isbn << endl;
        cout << "----------------------" << endl;
    }
}

void kitapAra() {
    cout << "Aramak istediğiniz kitabın adı veya yazar adı: ";
    string arama;
    getline(cin, arama); // arama terimini alır
    
    bool bulundu = false; // kitabın bulunup bulunmadığını kontrol eder
    for (int i = 0; i < kitapSayisi; i++) {
        if (kitaplar[i].ad == arama || kitaplar[i].yazar == arama) { // kitap adı veya yazar adı eşleşirse
            cout << "Kitap bulundu:" << endl;
            cout << "Kitap adı: " << kitaplar[i].ad << endl;
            cout << "Yazar adı: " << kitaplar[i].yazar << endl;
            cout << "Yayın yılı: " << kitaplar[i].yayinYili << endl;
            cout << "ISBN numarası: " << kitaplar[i].isbn << endl;
            bulundu = true; // kitap bulundu olarak işaretler
        }
    }
    
    if (!bulundu) { // kitap bulunamadıysa
        cout << "Aradığınız kitap bulunamadı." << endl;
    }
}

void kitapSil() {
    cout << "Silmek istediğiniz kitap adı veya ISBN numarası: ";
    string arama;
    getline(cin, arama); // silinecek kitabın adını veya isbn numarasını alır
    
    for (int i = 0; i < kitapSayisi; i++) {
        if (kitaplar[i].ad == arama || kitaplar[i].isbn == arama) { // kitap adı veya isbn numarası eşleşirse
            for (int s = i; s < kitapSayisi - 1; s++) { // kitabı siler ve diziyi kaydırır
                kitaplar[s] = kitaplar[s + 1];
            }
            kitapSayisi--; // kitap sayısını azaltır
            dosyayaYaz(); // kitap bilgilerini dosyaya yazar
            cout << "Kitap başarıyla silindi." << endl;
            return;
        }
    }
    
    cout << "Silinecek kitap bulunamadı." << endl;
}

void kitapGuncelle() {
    cout << "Güncellemek istediğiniz kitabın adı veya ISBN numarası: ";
    string arama;
    getline(cin, arama); // güncellenecek kitabın adını veya isbn numarasını alır
    
    for (int i = 0; i < kitapSayisi; i++) {
        if (kitaplar[i].ad == arama || kitaplar[i].isbn == arama) { // kitap adı veya isbn numarası eşleşirse
            cout << "Yeni kitap adı (önceki: " << kitaplar[i].ad << "): ";
            getline(cin, kitaplar[i].ad); // yeni kitap adını alır
            
            cout << "Yeni yazar adı (önceki: " << kitaplar[i].yazar << "): ";
            getline(cin, kitaplar[i].yazar); // yeni yazar adını alır
            
            cout << "Yeni yayın yılı (önceki: " << kitaplar[i].yayinYili << "): ";
            while (!(cin >> kitaplar[i].yayinYili)) { // yeni yayın yılını alır ve geçerli bir sayı olup olmadığını kontrol eder
                cout << "Geçersiz giriş. Lütfen bir sayı girin: ";
                cin.clear(); // giriş hatasını temizler
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // giriş tamponunu temizler
            }
            cin.ignore(); // giriş tamponunu temizler
            
            cout << "Yeni ISBN numarası (önceki: " << kitaplar[i].isbn << "): ";
            while (true) {
                getline(cin, kitaplar[i].isbn); // yeni isbn numarasını alır
                if (isValidISBN(kitaplar[i].isbn)) { // isbn numarasının geçerli olup olmadığını kontrol eder
                    break;
                } else {
                    cout << "Geçersiz ISBN numarası. Lütfen 11 haneli bir sayı girin: ";
                }
            }
            
            dosyayaYaz(); // kitap bilgilerini dosyaya yazar
            cout << "Kitap bilgileri güncellendi." << endl;
            return;
        }
    }
    
    cout << "Güncellenecek kitap bulunamadı." << endl;
}

bool isValidISBN(const string& isbn) {
    if (isbn.length() != 11) { // isbn numarasının uzunluğunu kontrol eder
        return false;
    }
    for (char c : isbn) { // isbn numarasının sadece rakamlardan oluşup oluşmadığını kontrol eder
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool dosyaVarMi(const string& dosyaAdi) {
    ifstream dosya(dosyaAdi); // dosyayı okuma modunda açar
    return dosya.good(); // dosyanın var olup olmadığını kontrol eder
}