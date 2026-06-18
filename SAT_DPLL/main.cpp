#include <iostream>
#include <string>
#include <cstring>
#include <stack>
#include <ostream>
#include "NodeBoolTree.h"
#include "boolinterval.h"
#include "boolequation.h"
#include "BBV.h"

#include <list>
#include <fstream>
#include <algorithm>
#define WIN32_LEAN_AND_MEAN  // убирает много лишнего из windows.h
#define NOGDI                 // убирает GDI
#include <windows.h>

int askStrategy(){
    std::string variant_strategy;
    do{
        std::cout<<"Enter variant of strategy\n"; //qDebug()<<"Enter variant of strategy";
        std::cout<<"1 - FirstFreeBranchingStrategy\n";//qDebug()<<"1 - FirstFreeBranchingStrategy";
        std::cout<<"2 - LastFreeBranchingStrategy\n";//qDebug()<<"2 - LastFreeBranchingStrategy";
        std::cout<<"3 - MostContraintBranchingStrategy\n : ";//qDebug()<<"3 - MostContraintBranchingStrategy";
        std::cin>>variant_strategy;
    }while(variant_strategy != "1" && variant_strategy != "2" && variant_strategy != "3");

    return std::stoi(variant_strategy);
}

std::string getExecutablePath() {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    return std::string(path);
}

//#include <filesystem>     // вспомогательная библиотека для вывода текущей директории исполняемого файла
int main(int argc, char *argv[]) {
    std::list < std::string > full_file_list;
    std::string filepath;

    // ..\SDPO_Lab2\SAT_DPLL\SatExamples
    // (UserRelease)\SAT_DPLL\SatExamples\Sat_ex11_3.pla
    //do{

    //std::cout<<"Current folder: "<<filesystem::current_path()<<'\n';
    std::cout<<"Enter path to example.pla: ";
    std::cin>>filepath;  //filepath = "Sat_ex30_3.pla";

    //считываем весь файл
    std::ifstream file(filepath);
    if (file.is_open()) {
        std::cout<<"Opened file\n";
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            // std::remove() возвращает указатель (итератор) на конец новой строки, по
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            std::cout<<"'"<<line<<"'"<<'\n';
            full_file_list.push_back(line);
        }
        int cnfSize = full_file_list.size();
        std::cout<<"CNF size: "<<cnfSize<<'\n';
        BoolInterval **CNF = new BoolInterval*[cnfSize];
        int rangInterval = -1; // error

        if (cnfSize) {
            rangInterval = full_file_list.front().size();
        }

        int i = 0;
        for (const auto &strv : full_file_list) { // Заполняем массив
            CNF[i] = new BoolInterval(strv.c_str());
            ++i;
        }

        std::string rootvec = ""; //QString rootvec = "";
        std::string rootdnc = ""; //QString rootdnc = "";

        //Строим интервал в которм все компоненты принимают значение '-',
        //который представляет собой корень уравнения, пока пустой.
        //В процессе поиска корня, компоненты интервала буду заменены на конкретные значения.

        for (int i = 0; i < rangInterval; i++) {
            rootvec += "0";
            rootdnc += "1";
        }

        std::string v = rootvec; //QByteArray v = rootvec.toUtf8();
        BBV vec(v.data());
        std::string d = rootdnc; //QByteArray d = rootdnc.toUtf8();
        BBV dnc(d.data());

        // Создаем пустой корень уравнения;
        BoolInterval *root = new BoolInterval(vec, dnc);

        BoolEquation *boolequation = new BoolEquation(CNF, root, cnfSize, cnfSize, vec);

        std::shared_ptr<FirstFreeBranchingStrategy> St1 = std::make_shared<FirstFreeBranchingStrategy>();
        std::shared_ptr<LastFreeBranchingStrategy> St2 = std::make_shared<LastFreeBranchingStrategy>();
        std::shared_ptr<MostContraintBranchingStrategy> St3 = std::make_shared<MostContraintBranchingStrategy>();

        int Entered_strategy = askStrategy();

        switch (Entered_strategy) {
        case 1:  boolequation->SetBranchingStrategy(St1); break;
        case 2:  boolequation->SetBranchingStrategy(St2); break;
        case 3:  boolequation->SetBranchingStrategy(St3); break;
        default: std::cout<<"Unresolved variant. Setting 'FirstFreeBranchingStrategy'"; boolequation->SetBranchingStrategy(St1); break;
        }


        // Алгоритм поиска корня. Работаем всегда с верхушкой стека.
        // Шаг 1. Правила выполняются? Нет - Ветвление Шаг 5. Да - Упрощаем Шаг 2.
        // Шаг 2. Строки закончились? Нет - Шаг1, Да - Корень найден? Да - Успех КОНЕЦ, Нет - Шаг 3.
        // Шаг 3. Кол-во узлов в стеке > 1? Нет - Корня нет КОНЕЦ, Да - Шаг 4.
        // Шаг 4. Текущий узел выталкиваем из стека, попадаем в новый узел. У нового узла lt rt отличны от NULL? Нет - Шаг 1. Да - Шаг 3.
        // Шаг 5. Выбор компоненты ветвления, создание двух новых узлов, добавление их в стек сначала с 1 потом с 0. Шаг 1.

        // Алгоритм CheckRules.
        // Цикл по строкам КНФ.
        // 1. Проверка правила 2. Выполнилось? Да - Корня нет, Нет - Идем дальше.
        // 2. Проверка правила 1. Выполнилось? Да - Упрощаем, Нет - Идем дальше.

        // Создаем стек под узлы булева дерева
        // QStack<NodeBoolTree> BoolTree;

        bool rootIsFinded = false;
        stack<NodeBoolTree *> BoolTree;
        NodeBoolTree *startNode = new NodeBoolTree(boolequation);
        BoolTree.push(startNode);
        try{
            do {
                NodeBoolTree *currentNode(BoolTree.top());

                if (currentNode->lt == nullptr &&
                    currentNode->rt == nullptr) { // Если вернулись в обработанный узел
                    BoolEquation *currentEquation = currentNode->eq;
                    bool flag = true;

                    // Цикл для упрощения по правилам.
                    while (flag) {
                        int a = currentEquation->CheckRules(); // Проверка выполнения правил

                        switch (a) {
                        case 0: { // Корня нет.
                            BoolTree.pop();
                            flag = false;
                            break;
                        }

                        case 1: { // Правило выполнилось, корень найден или продолжаем упрощать.
                            if (currentEquation->count == 0 ||
                                currentEquation->mask.getWeight() ==
                                    currentEquation->mask.getSize()) { // Если кончились строки или столбцы, корень найден.
                                flag = false;
                                rootIsFinded =
                                    true; // Полагаем, что корень найден, выполняем проверку корня

                                for (int i = 0; i < cnfSize; i++) {

                                    if (!CNF[i]->isEqualComponent(*currentEquation->root)) {
                                        rootIsFinded = false;//Корень не найден. Продолжаем искать дальше.
                                        BoolTree.pop();
                                        break;
                                    }
                                }
                            }

                            break;
                        }

                        case 2: { // Правила не выполнились, ветвление.
                            // Ветвление, создание новых узлов.
                            int indexBranching = currentEquation->ChooseColForBranching();
                            if (indexBranching < 0) {
                                BoolTree.pop();
                                flag = false;
                                break;
                            }

                            BoolEquation *Equation0 = new BoolEquation(*currentEquation);
                            BoolEquation *Equation1 = new BoolEquation(*currentEquation);

                            Equation0->Simplify(indexBranching, '0');
                            Equation1->Simplify(indexBranching, '1');

                            NodeBoolTree *Node0 = new NodeBoolTree(Equation0);
                            NodeBoolTree *Node1 = new NodeBoolTree(Equation1);

                            currentNode->lt = Node0;
                            currentNode->rt = Node1;

                            BoolTree.push(Node1);
                            BoolTree.push(Node0);

                            flag = false;
                            break;
                        }
                        }
                    }
                } else {
                    BoolTree.pop();
                }

            } while (BoolTree.size() > 1 && !rootIsFinded);

            if (rootIsFinded) {
                std::cout << "Root is:\n ";
                BoolInterval *finded_root = BoolTree.top()->eq->root;
                std::cout << string(*finded_root);
            } else {
                std::cout << "Root is not exists!";
            }
        }catch(int arg){
            std::cout<<"Error code: "<<arg<<'\n';
        }
    } else {
        std::cout << "File does not exists.\n";
    }

    return 0;

}
