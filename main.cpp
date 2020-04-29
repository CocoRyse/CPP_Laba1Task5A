#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <random>
#include <vector>

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;

/*
    Написать программу, содержащую следующие функции:

    Функцию заполнения текстового файла N целыми случайными числами в диапазоне от -M до M. Имя файла, N и M поступают
    на вход функции, файл возвращается функцией как результат (использовать потоки ввода-вывода).

    Реализовать два варианта генерации чисел — в цикле и с использованием алгоритма std::generate.

    Функцию, получающую файл на вход, и возвращающую последовательный контейнер, заполненный числами из файла.

    Функцию modify, выполняющую преобразование полученного контейнера. На вход функции должен поступать контейнер,
    в качестве результата функция должна возвращать модифицированный контейнер.

    Перегруженную функцию modify, принимающую на вход итераторы начала и конца обрабатываемой части контейнера.

    Функцию, реализующую требуемое преобразование с использованием алгоритма std::transform вместо функции modify.

    Функцию, реализующую требуемое преобразование с использованием алгоритма std::for_each вместо функции modify.

    Функции, вычисляющие сумму и среднее арифметическое чисел, содержащихся в контейнере.

    Функцию вывода результата в файл и на экран (опционально).

    Task: Разделить все нечетные по абсолютной величине числа на среднее арифметическое всех чисел.
 */

ifstream create_file(int n, int m, const string& file_name)
{
    ofstream file(file_name);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> dist(-m, m);

    for (int i = 0; i < n; i++)
        file << dist(mt) << " ";

    file.close();
    ifstream F(file_name);
    return F;
}

std::ostream& operator<<(std::ostream& out, const vector<int>& vect)
{
    for (int n : vect)
        out << n << " ";

    out << endl;
    return out;
}

vector<int> read_file(std::istream& in)
{
    vector<int> vect;
    int i;
    while (!in.eof())
    {
        in >> i;
        vect.push_back(i);
    }
    return vect;
}

int sum(const vector<int>& vect)
{
    int sum = 0;
    for (auto a : vect)
        sum += a;

    return sum;
}

int sum(vector<int>::iterator start, vector<int>::iterator end)
{
    int sum = 0;
    for (auto a = start; a != end; a++)
        sum += *a;

    return sum;
}

double average(const vector<int>& vect)
{
    return ((double)sum(vect)) / vect.size();
}

int count(vector<int>::iterator start, vector<int>::iterator end)
{
    int count = 0;
    for (auto a = start; a != end; a++)
        count += 1;

    return count;
}

// среднее арифметическое обрабатываемой части
double average(vector<int>::iterator start, vector<int>::iterator end)
{
    return ((double)sum(start, end)) / count(start, end);
}

vector<int> modify(vector<int> vect)
{
    double avg = average(vect);

    for (int & i : vect)
        i /= i % 2 != 0 ? avg : 1;

    return vect;
}

vector<int> modify(vector<int>& vect, vector<int>::iterator start, vector<int>::iterator end)
{
    double avg = average(start, end);
    for (auto i = start; i != end; ++i)
        *i /= *i % 2 != 0 ? avg : 1;
    
    return vect;
}

vector<int> modify_transform(vector<int> vect)
{
    double avg = average(vect.begin(), vect.end());
    std::transform(vect.begin(), vect.end(), vect.begin(), [&](int x) {return (x % 2 != 0 ? x / avg : x); });
    return vect;
}

vector<int> modify_for_each(vector<int> vect)
{
    double avg = average(vect.begin(), vect.end());
    std::for_each(vect.begin(), vect.end(), [&](int& x) { x /= x % 2 != 0 ? avg : 1; });
    return vect;
}

int main()
{
    setlocale(LC_ALL, "Rus");
    string str;
    ifstream F;
    ofstream file;
    bool full = false;
    vector<int> vect;
    vector<int>::iterator start, end;
    int st, en;
    int n, m, index = -1;
    while (index != 0)
    {
        cout << "Выберите действие: " << endl;
        cout << "1. Создание файла с рандомными числами" << endl;
        cout << "2. Запись данных из файла в вектор" << endl;
        if (full)
        {
            cout << "3. modify" << endl;
            cout << "4. перегруженный modify" << endl;
            cout << "5. transform" << endl;
            cout << "6. for_each" << endl;
            cout << "7. Сумма " << endl;
            cout << "8. Среднее арифметическое" << endl;
            cout << "9. Вывод вектора" << endl;
        }
        cout << "0. Выход" << endl;
        cin >> index;
        {
            switch (index)
            {
                case 1:
                    cout << "Введите количество чисел, которое вы хотите сгенерировать" << endl;
                    cin >> n;
                    cout << "Введите имя файла" << endl;
                    cin >> str;
                    cout << "Введите предел M ([-M,M])" << endl;
                    cin >> m;
                    F = create_file(n, m, str);
                    vect = read_file(F);
                    full = true;
                    break;
                case 2:
                    cout << "Введите имя файла" << endl;
                    cin >> str;
                    F.open(str);
                    vect = read_file(F);
                    full = true;
                    break;
                case 3:
                    vect = modify(vect);
                    break;
                case 4:
                    cout << "Введите начало и конец, обрабатываемой части" << endl;
                    cin >> st;
                    cin >> en;
                    start = vect.begin();
                    for (int i = 1; i < st; i++)
                        start++;

                    end = start;
                    for (int i = st; i <= en; i++)
                        end++;

                    vect = modify(vect, start, end);
                    break;
                case 5:
                    vect = modify_transform(vect);
                    break;
                case 6:
                    vect = modify_for_each(vect);
                    break;
                case 7:
                    cout << "Сумма чисел в контейнере: " << sum(vect) << endl;
                    break;
                case 8:
                    cout << "Ср. арефметическое в контейнере: " << average(vect) << endl;
                    break;
                case 9:
                    int sw;
                    cout << "1. На экран" << endl;
                    cout << "2. В файл" << endl;
                    cin >> sw;
                    switch (sw)
                    {
                        case 1:
                            cout << vect;
                            break;
                        case 2:
                            cout << "Введите имя файла: ";
                            cin >> str;
                            file.open(str);
                            file << vect;
                            file.close();
                            break;
                        default:
                            cout << "Ввод не опознан";
                            break;
                    }
                    break;
                default:
                    cout << "Выход из программы..." << endl;
                    break;
            }
        }
    }

    return 0;
}