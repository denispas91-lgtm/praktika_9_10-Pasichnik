#include <iostream>
#include <fstream>
#include <cstring>
#include <clocale>
#include <windows.h>
using namespace std;

enum MarketplaceType {
    FOOD,
    HOME_GOODS,
    ALL_IN_ONE
};

enum BusinessType {
    MAIN_BIZ,
    AUXILIARY_BIZ,
    FRANCHISE_BIZ
};

struct Location {
    char city[50];
    char country[50];
};

struct Marketplace {
    char name[50];
    int pvz_count;
    Location location;
    double market_value;
    MarketplaceType mp_type;
    BusinessType biz_type;
};

const int MARKET_COUNT = 20;

const char* mpTypeStr(MarketplaceType t) {
    if (t == FOOD) return "Продуктовый";
    if (t == HOME_GOODS) return "Товары для дома";
    return "Все подряд";
}

const char* bizTypeStr(BusinessType t) {
    if (t == MAIN_BIZ) return "Основной";
    if (t == AUXILIARY_BIZ) return "Вспомогательный";
    return "Франшиза";
}

void printOne(const Marketplace& m) {
    cout << "Название: " << m.name
        << ", ПВЗ: " << m.pvz_count
        << ", Город: " << m.location.city
        << ", Страна: " << m.location.country
        << ", Стоимость: " << m.market_value << " млрд руб."
        << ", Тип: " << mpTypeStr(m.mp_type)
        << ", Бизнес: " << bizTypeStr(m.biz_type) << endl;
}

void printAll(const Marketplace arr[], int n) {
    cout << "\nВсе маркетплейсы\n";
    for (int i = 0; i < n; i++)
        printOne(arr[i]);
}

int filterHomeGoods(const Marketplace src[], int n, Marketplace dst[]) {
    int cnt = 0;
    for (int i = 0; i < n; i++)
        if (src[i].mp_type == HOME_GOODS)
            dst[cnt++] = src[i];
    return cnt;
}

void bubbleSortByValue(Marketplace arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j].market_value > arr[j + 1].market_value) {
                Marketplace temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
}

void printfFiltered(const Marketplace arr[], int n, const char* title) {
    cout << "\n " << title << "\n";
    if (n == 0) {
        cout << "Нет данных\n";
        return;
    }
    for (int i = 0; i < n; i++)
        printOne(arr[i]);
}

void findByName(const Marketplace arr[], int n, const char* name) {
    cout << "\n Поиск: \"" << name << "\" \n";
    bool found = false;
    for (int i = 0; i < n; i++)
        if (strcmp(arr[i].name, name) == 0) {
            printOne(arr[i]);
            found = true;
        }
    if (!found) cout << "Не найдено\n";
}

void twoFromSameCity(const Marketplace arr[], int n) {
    cout << "\n 2 маркетплейса из одного города (не Москва) \n";
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].location.city, "Москва") == 0) continue;
        for (int j = i + 1; j < n; j++) {
            if (strcmp(arr[j].location.city, "Москва") == 0) continue;
            if (strcmp(arr[i].location.city, arr[j].location.city) == 0) {
                cout << "[1]\n"; printOne(arr[i]);
                cout << "[2]\n"; printOne(arr[j]);
                return;
            }
        }
    }
    cout << "Пара не найдена\n";
}

void updateMarketplace(Marketplace arr[], int n, const char* name,
    int newPvz, double newVal) {
    cout << "\n Изменение: \"" << name << "\" \n";
    bool found = false;
    for (int i = 0; i < n; i++)
        if (strcmp(arr[i].name, name) == 0) {
            arr[i].pvz_count = newPvz;
            arr[i].market_value = newVal;
            cout << "Обновлено:\n";
            printOne(arr[i]);
            found = true;
        }
    if (!found) cout << "Не найден\n";
}

int filterMoscow(const Marketplace src[], int n, Marketplace dst[]) {
    int cnt = 0;
    for (int i = 0; i < n; i++)
        if (strcmp(src[i].location.city, "Москва") == 0)
            dst[cnt++] = src[i];
    return cnt;
}

void writeTextFile(const char* fn, const Marketplace arr[], int n) {
    ofstream fout(fn);
    if (!fout.is_open()) {
        cout << "Ошибка открытия: " << fn << "\n";
        return;
    }
    for (int i = 0; i < n; i++)
        fout << arr[i].name << " " << arr[i].pvz_count << "\n";
    fout.close();
    cout << "\nЗаписан текстовый файл: " << fn << "\n";
}

void readTextAndUpdate(const char* fn, Marketplace arr[], int n) {
    ifstream fin(fn);
    if (!fin.is_open()) {
        cout << "Ошибка открытия: " << fn << "\n";
        return;
    }
    char name[50];
    int pvz;
    while (fin >> name >> pvz) {
        for (int i = 0; i < n; i++) {
            if (strcmp(arr[i].name, name) == 0) {
                arr[i].pvz_count = pvz;
                cout << "Обновлено: " << name << " ПВЗ = " << pvz << endl;
            }
        }
    }
    fin.close();
    cout << "Считано из: " << fn << "\n";
}

void writeBinaryFile(const char* fn, const Marketplace arr[], int n) {
    ofstream fout(fn, ios::binary | ios::out);
    if (!fout.is_open()) {
        cout << "Ошибка записи: " << fn << "\n";
        return;
    }
    fout.write(reinterpret_cast<const char*>(arr), sizeof(Marketplace) * n);
    fout.close();
    cout << "Записан бинарный файл: " << fn << "\n";
}

int readBinaryFile(const char* fn, Marketplace arr[]) {
    ifstream fin(fn, ios::binary | ios::in);
    if (!fin.is_open()) {
        cout << "Ошибка чтения: " << fn << "\n";
        return 0;
    }
    int n = 0;
    while (fin.read(reinterpret_cast<char*>(&arr[n]), sizeof(Marketplace)))
        n++;
    fin.close();
    cout << "Считан бинарный файл: " << fn << " (записей: " << n << ")\n";
    return n;
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");

    Marketplace mp[MARKET_COUNT] = {
        {"Wildberries", 200000, {"Москва", "Россия"}, 1300.5, ALL_IN_ONE, MAIN_BIZ},
        {"Ozon", 50000, {"Москва", "Россия"}, 800.2, ALL_IN_ONE, MAIN_BIZ},
        {"ЯндексМаркет", 30000, {"Москва", "Россия"}, 500.0, ALL_IN_ONE, MAIN_BIZ},
        {"СберМегаМаркет", 20000, {"Москва", "Россия"}, 300.0, ALL_IN_ONE, AUXILIARY_BIZ},
        {"AliExpress", 5000, {"Москва", "Россия"}, 150.0, ALL_IN_ONE, FRANCHISE_BIZ},
        {"ВкусВилл", 1200, {"Москва", "Россия"}, 90.0, FOOD, MAIN_BIZ},
        {"Самокат", 800, {"Москва", "Россия"}, 70.5, FOOD, MAIN_BIZ},
        {"Перекресток", 2500, {"Москва", "Россия"}, 110.0, FOOD, MAIN_BIZ},
        {"Лента", 3000, {"Санкт-Петербург", "Россия"}, 200.0, FOOD, MAIN_BIZ},
        {"ОКей", 1500, {"Санкт-Петербург", "Россия"}, 80.0, FOOD, MAIN_BIZ},
        {"Hoff", 400, {"Москва", "Россия"}, 60.0, HOME_GOODS, MAIN_BIZ},
        {"LeroyMerlin", 100, {"Москва", "Россия"}, 400.0, HOME_GOODS, FRANCHISE_BIZ},
        {"IKEA", 20, {"Москва", "Россия"}, 350.0, HOME_GOODS, FRANCHISE_BIZ},
        {"Петрович", 600, {"Санкт-Петербург", "Россия"}, 45.0, HOME_GOODS, MAIN_BIZ},
        {"ВсеИнструменты", 900, {"Москва", "Россия"}, 55.0, HOME_GOODS, MAIN_BIZ},
        {"KazanExpress", 8000, {"Казань", "Россия"}, 120.0, ALL_IN_ONE, MAIN_BIZ},
        {"ЭкоМаркет", 300, {"Новосибирск", "Россия"}, 15.0, FOOD, MAIN_BIZ},
        {"МегаМаркет", 500, {"Новосибирск", "Россия"}, 25.0, ALL_IN_ONE, AUXILIARY_BIZ},
        {"ДомДа", 200, {"Санкт-Петербург", "Россия"}, 10.0, HOME_GOODS, AUXILIARY_BIZ},
        {"МагнитМаркет", 4000, {"Краснодар", "Россия"}, 180.0, FOOD, MAIN_BIZ}
    };

    printAll(mp, MARKET_COUNT);

    Marketplace homeArr[MARKET_COUNT];
    int homeCnt = filterHomeGoods(mp, MARKET_COUNT, homeArr);
    bubbleSortByValue(homeArr, homeCnt);
    printfFiltered(homeArr, homeCnt, "Маркетплейсы с товарами для дома");

    findByName(mp, MARKET_COUNT, "IKEA");
    findByName(mp, MARKET_COUNT, "Wildberries");
    twoFromSameCity(mp, MARKET_COUNT);
    updateMarketplace(mp, MARKET_COUNT, "Hoff", 500, 75.5);

    Marketplace moscowArr[MARKET_COUNT];
    int moscowCnt = filterMoscow(mp, MARKET_COUNT, moscowArr);
    printfFiltered(moscowArr, moscowCnt, "Маркетплейсы из Москвы");

    writeTextFile("input.txt", mp, MARKET_COUNT);
    readTextAndUpdate("input.txt", mp, MARKET_COUNT);

    cout << "\n Проверка обновлений \n";
    findByName(mp, MARKET_COUNT, "Wildberries");
    findByName(mp, MARKET_COUNT, "Hoff");

    writeBinaryFile("marketplace.bin", mp, MARKET_COUNT);

    Marketplace loaded[MARKET_COUNT];
    int loadedCnt = readBinaryFile("marketplace.bin", loaded);

    Marketplace check[3];
    int chkCnt = (loadedCnt >= 3) ? 3 : loadedCnt;
    for (int i = 0; i < chkCnt; i++) check[i] = loaded[i];
    printfFiltered(check, chkCnt, "Первые 3 записи из бинарного файла");

    system("pause");
    return 0;
}