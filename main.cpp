#include <iostream>
#include <string>
#include <math.h>

using namespace std;

// Структура содержащая значения одного элемента хэш-таблицы
struct Company {
public:
    int license;
    string name;
    string direcrot;

    Company (int license, string name, string director) {
        this->license = license;
        this->name = name;
        this->direcrot = director;
    };
};

// Класс хранящий хэш-таблицу
class Hash_table {
private:
    Company** table;
    int size, hash;
    int const c = 1, d = 2;

public:
    // Конструктор для создания масива структур заданной длинны
    Hash_table(int i) {
        table = new Company * [i];
        size = i;

        for (int j = 0; j < size; ++j) {
            table[j] = nullptr;
        }
    };

    // Функция возвращающая хэш элемента в зависимости от размера хэш-таблицы
    int make_hash(unsigned int key) {
        return key % size;
    }

    // Функция возвращающая размер хэш-таблицы
    int getSize(){
        return size;
    }

    // Функция добавляющая элемент в хэш-таблицу : возвращает был ли добавлен элемент булевым значением
    bool add (Company& element) {
        hash = make_hash(element.license);
        if (!is_element(hash))
            table[hash] = &element;
        else {
            int i = 0, address;
            while (true) {
                i++;
                address = hash + c * i + d * pow(i, 2);
                if (address >= size)
                    return false;
                if (!is_element(address)){
                    table[address] = &element;
                    break;
                }
            }
        }
        return true;
    };

    // Функция поиска элемента в хэш-таблице : возвращает либо адрес, либо не возможное значание
    int find (int key) {
        hash = make_hash(key);
        int i = 0, address = hash;
        while (true) {
            if (address > size || table[address] == nullptr) {
                cout << "Не возможен такой вариант" << endl;
                return -1;
            }
            if (table[address]->license == key){
                return address;
            }
            i++;
            address = hash + c * i + d * pow(i, 2);
        }
    }

    // Функция проверяет существование элемента по адресу : возвращает булево значение
    bool is_element (int address) {
        if (address == -1) return false;
        if (table[address] != nullptr) {
            if (table[address]->license != -1)
                return true;
        }
        return false;
    }

    // Функция возвращяет ссылку на элемент в хэш-таблице
    Company& getElement (int address) {
        return *table[address];
    }

    // Функция удаляющая элемент из хэш-таблицы
    void del(int key) {
        hash = find(key);
        table[hash]->license = -1;
    }

    // Функция превращающая элемент в воспринимаемую человеком строку : возвращает строку со значениями полей элемента
    string element_to_string(int hash) {
        if (!is_element(hash))
            return "Ничего не содерится";
        return to_string(table[hash]->license) + " " + table[hash]->name + " - " + table[hash]->direcrot;
    }

    // Функция выводит содержимое хэш-таблицы в премлимом виде
    void info() {
        for (int i = 0; i < size; ++i) {
            cout << i + 1 << ": " << element_to_string(i) << endl;
        }
    }

};

// Функция рехеширования
Hash_table * ReHash_table (Hash_table *hash_table) {
    Hash_table* temp = new Hash_table(hash_table->getSize()*2);
    for (int i = 0; i < hash_table->getSize(); ++i) {
        if (hash_table->is_element(i))
            temp->add(hash_table->getElement(i));
    }
    return temp;
}

void test_hash (Hash_table *hash_table) {
    int i = 120;
    while (true) {
        Company* lk = new Company(120, "loren", "ev");
        lk->license = i++;
        if (i > 1400)
            break;
        if (!hash_table->add(*lk)) {
            hash_table = ReHash_table(hash_table);
            hash_table->add(*lk);
        }
    }
    cout << hash_table->element_to_string(hash_table->find(122)) << endl;
    hash_table->del(120);
    hash_table->info();
}

// Основная функния программы
int main() {
    setlocale(LC_ALL, "Rus");
    bool work = true;
    int choice, key = 1, license;
    string director, name;
    Company* element;

    Hash_table* hash_table = new Hash_table(5);
    // test_hash(hash_table);

    while (work) {
        cout << "Меню:\n"
                "1. Добавить элемент\n"
                "2. Найти элемент по ключу\n"
                "3. Удалить элемент\n"
                "4. Вывод содержимого Хеш-таблицы\n"
                "5. Выйти\n"
                "Выбор: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Номер лицензии: ";
                cin >> license;
                cout << "Название компании: ";
                cin >> name;
                cout << "Имя директора: ";
                cin >> director;

                element = new Company(license, name, director);

                while (!hash_table->add(*element)){
                    hash_table = ReHash_table(hash_table);
                }
                break;
            case 2:
                cout << "Номер лицензии: ";
                cin >> key;
                cout << hash_table->element_to_string(hash_table->find(key)) << endl;
                break;
            case 3:
                cout << "Номер лицензии: ";
                cin >> key;
                hash_table->del(key);
                break;
            case 4:
                hash_table->info();
                break;
            case 5:
                work = false;
                break;
        }
    }

    return 0;
}
