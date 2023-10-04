#include <iostream>
#include <conio.h>
#include <iomanip>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <math.h>

using namespace std;
namespace fs = std::filesystem;

void Read_files_name(vector<string>& array_for_files_name)
{
    string directory_name = "";
    string operating_mode = "";
    
    cout << "Введите путь до директории, в которой хранятся файлы с данными: ";
    cin >> directory_name;
    system("cls");
    for (const auto& entry : fs::directory_iterator(directory_name))
        array_for_files_name.push_back(entry.path().string());
}

void Cycle_for_read_files_name(vector<string>& array_for_files_name)
{
    string operating_mode = "";

    Read_files_name(array_for_files_name);

    while (operating_mode != "N")
    {
        operating_mode = "";
        cout << "Если вы хотите прочитать ещё одну директорию, введите Y, иначе N: ";
        cin >> operating_mode;
        if (operating_mode == "Y")
        {
            system("cls");
            Read_files_name(array_for_files_name);
        }
        else
        {
            system("cls");
        }
    }
}


void File_reading(string path_to_file, vector<string>& results_from_file)
{
    string line_with_results;
    ifstream open_file(path_to_file);
    if (open_file.is_open())
    {
        while (getline(open_file, line_with_results))
        {
            results_from_file.push_back(line_with_results);
        }
        open_file.close();
    }
    else
        cout << "Ошибка открытия файла!" << endl;
}

int Rotation_for_tah(string bufer_for_tahname)
{
    int rotation = 0;
    cout << "Введите угол поворота системы координат для данных с тахеометра в файле " << bufer_for_tahname << "  : ";
    cin >> rotation;
    system("cls");
    return rotation;
}

void File_processing(string array_for_files_name, vector<string> results_from_file, vector<string>& name, vector<string>& y, vector<string>& x, vector<string>& h, vector<string>& comment, vector<string>& type, vector<int>& rotation, int &point_count)
{
    string buffer = "",
           buffer_for_result_string = "",
           for_comment_gps = "gps",
           for_comment_tah = "tah";
    int pos_in_file = 0,
        count_for_string = 0,
        rotation_in_file = 0;

    buffer_for_result_string = results_from_file[0];
    int file_type = buffer_for_result_string.find("xml");

    if (file_type != -1)
    {
        while ((pos_in_file = buffer_for_result_string.find("lat=\"", pos_in_file)) != -1)
        {
            point_count++;
            type.push_back(for_comment_gps);
            name.push_back(to_string(point_count));
            pos_in_file = pos_in_file + 5;
            while (buffer_for_result_string[pos_in_file] != '\"')
            {
                buffer = buffer + buffer_for_result_string[pos_in_file];
                pos_in_file++;
            }
            y.push_back(buffer);
            buffer = "";

            pos_in_file = buffer_for_result_string.find("lon=\"", pos_in_file);
            pos_in_file = pos_in_file + 5;
            while (buffer_for_result_string[pos_in_file] != '\"')
            {
                buffer = buffer + buffer_for_result_string[pos_in_file];
                pos_in_file++;
            }
            x.push_back(buffer);
            buffer = "";

            pos_in_file = buffer_for_result_string.find("<ele>", pos_in_file);
            pos_in_file = pos_in_file + 5;
            while (buffer_for_result_string[pos_in_file] != '<')
            {
                buffer = buffer + buffer_for_result_string[pos_in_file];
                pos_in_file++;
            }
            h.push_back(buffer);
            buffer = "";

            pos_in_file = buffer_for_result_string.find("<name>", pos_in_file);
            pos_in_file = pos_in_file + 6;
            while (buffer_for_result_string[pos_in_file] != '<')
            {
                buffer = buffer + buffer_for_result_string[pos_in_file];
                pos_in_file++;
            }
            buffer = buffer + "   " + for_comment_gps + "   " + array_for_files_name;
            comment.push_back(buffer);
            buffer = "";

            rotation.push_back(0);
        }
    }
    else
    {
        rotation_in_file = Rotation_for_tah(array_for_files_name);

        for (count_for_string = 0; count_for_string < results_from_file.size(); count_for_string++)
        {
            buffer_for_result_string = results_from_file[count_for_string];
            if ((buffer_for_result_string.find("2111")) != -1)
            {
                point_count++;
                type.push_back(for_comment_tah);
                name.push_back(to_string(point_count));
                pos_in_file = 5;
                while (buffer_for_result_string[pos_in_file] != ';')
                {
                    buffer = buffer + buffer_for_result_string[pos_in_file];
                    pos_in_file++;
                }
                buffer = buffer + "   " + for_comment_tah + "   " + array_for_files_name;
                comment.push_back(buffer);
                buffer = "";

                pos_in_file = pos_in_file + 2;
                while (buffer_for_result_string[pos_in_file] != ';')
                {
                    buffer = buffer + buffer_for_result_string[pos_in_file];
                    pos_in_file++;
                }
                y.push_back(buffer);
                buffer = "";

                pos_in_file = pos_in_file + 1;
                while (buffer_for_result_string[pos_in_file] != ';')
                {
                    buffer = buffer + buffer_for_result_string[pos_in_file];
                    pos_in_file++;
                }
                x.push_back(buffer);
                buffer = "";

                pos_in_file = pos_in_file + 1;
                while (buffer_for_result_string[pos_in_file] != ';')
                {
                    buffer = buffer + buffer_for_result_string[pos_in_file];
                    pos_in_file++;
                }
                h.push_back(buffer);
                buffer = "";

                rotation.push_back(rotation_in_file);
            }
        }
    }
}

void File_converting_for_GPS(vector<string>& B, vector<string>& L, vector<string>& H, int point_count)
{
    double NB,
        E2,
        a = 6378.137,
        b = 6356.7523;

    double B_rad = stod(B[point_count]) * (3.14159 / 180),
        L_rad = stod(L[point_count]) * (3.14159 / 180),
        h = stod(H[point_count]) / 1000;

    E2 = (a * a - b * b) / (a * a);
    NB = a / (sqrt(1 - E2 * pow(sin(B_rad), 2)));

    double X = (NB + h) * cos(B_rad) * cos(L_rad);
    double Y = (NB + h) * cos(B_rad) * sin(L_rad);

    B[point_count] = to_string(X);
    L[point_count] = to_string(Y);
}

void File_converting_for_tah(vector<string>& Y, vector<string>& X, int point_count, vector<int>& rotation)
{
    double Y_rot = stod(Y[point_count]) * cos(rotation[point_count] * (3.14159 / 180)) - stod(X[point_count]) * sin(rotation[point_count] * (3.14159 / 180));
    double X_rot = stod(X[point_count]) * cos(rotation[point_count] * (3.14159 / 180)) + stod(Y[point_count]) * sin(rotation[point_count] * (3.14159 / 180));

    Y[point_count] = to_string(Y_rot);
    X[point_count] = to_string(X_rot);
}

void File_recording(vector<string>& name, vector<string>& y, vector<string>& x, vector<string>& h, vector<string>& comment, int& point_count)
{
    string header = "Номер  Северная широта  Восточная долгота  Высота точки       Комментарий";
    string stroke = "";
    int space = 0;

    ofstream file("Файл с результатами.txt");
    if (file.is_open())
    {
        file << header << endl;
        for (int index = 0; index < point_count; index++)
        {
            stroke = stroke + name[index];
            space = 7;
            for (int i = 0; i < space - name[index].length(); i++)
                stroke = stroke + " ";

            stroke = stroke + y[index];
            space = 17;
            for (int i = 0; i < space - y[index].length(); i++)
                stroke = stroke + " ";

            stroke = stroke + x[index];
            space = 19;
            for (int i = 0; i < space - x[index].length(); i++)
                stroke = stroke + " ";

            stroke = stroke + h[index];
            space = 19;
            for (int i = 0; i < space - h[index].length(); i++)
                stroke = stroke + " ";

            stroke = stroke + comment[index];

            file << stroke << endl;

            stroke = "";
        }

        cout << "Файл успешно создан в директории с программой и называется 'Файл с результатами.txt' !" << endl << endl;
    }
}

void Сonvertation_to_another_number_system(vector<string>& y, vector<string>& x, vector<string>& type, int& point_count)
{
    double min_X_for_GPS = 0,
           min_Y_for_GPS = 0,
           bufer_for_X = 0,
           bufer_for_Y = 0;


    for (int i = 0; i < point_count; i++)
    {
        if (type[i] == "gps")
        {
            min_X_for_GPS = stod(x[i]);
            min_Y_for_GPS = stod(y[i]);
            break;
        }
    }

    for (int i = 0; i < point_count; i++)
    {
        if (type[i] == "gps")
        {
            if (min_X_for_GPS > stod(x[i]))
            {
                min_X_for_GPS = stod(x[i]);
                min_Y_for_GPS = stod(y[i]);
            }
        }
    }

    for (int i = 0; i < point_count; i++)
    {
        if (type[i] == "gps")
        {
            bufer_for_X = stod(x[i]) - min_X_for_GPS;
            bufer_for_Y = stod(y[i]) - min_Y_for_GPS;
            x[i] = to_string(bufer_for_X);
            y[i] = to_string(bufer_for_Y);
        }
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<string> Array_for_files_name;
    vector<string> Results_from_file;

    vector<string>  Name,
                    Y,
                    X,
                    H,
                    Comment,
                    Type;
    vector<int>     Rotation;
    
    int             Point_count = 0;

    string          Bufer_for_TahName;
    

    Cycle_for_read_files_name(Array_for_files_name);

    for (int i = 0; i < Array_for_files_name.size(); i++)
    {
        File_reading(Array_for_files_name[i], Results_from_file);
        File_processing(Array_for_files_name[i], Results_from_file, Name, Y, X, H, Comment, Type, Rotation, Point_count);
        Results_from_file.clear();
    }

    for (int i = 0; i < Point_count; i++)
    {
        if (Type[i] == "gps")
            File_converting_for_GPS(Y, X, H, i);
        else
            if (Type[i] == "tah") 
                File_converting_for_tah(Y, X, i, Rotation);
    }

    Сonvertation_to_another_number_system(Y, X, Type, Point_count);

    File_recording(Name, Y, X, H, Comment, Point_count);

    system ("pause");
}