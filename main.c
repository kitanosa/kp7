// вариант 23
/*
где каждая пара обозначает комплексное число
0 1 означает 0 + 1*i
потом после того как мы считали матрицу надо представить ее элементы в виде векторов
вот и все
причем в векторы мы пишем только ненулевые элементы


в общем что надо
* у нас задание с комплекными числами. Комплексное число представляет из себя
мнимую и действительную часть. Поэтому будем вводить в матрицу
по аре элементов
первое число из пары будет давать жействительную часть
второе мнимую
5 1
5+1*i
скорее всего это сделать надо будет через структуру
1. Ввести в текстовый файл матрцицу ручками
2. Считать эту матрицу из текстового файла как то через fscanf наверн
3. Заткнуть в вектор согласно заданию ненулевые значения матрицы
4. Потом сделать что по заданию. Для вектора и для матрицы
5. Сделать вывод новой матрицы и вектора. Усё!
*/
/*
что нужно сделать
сделать структуру вектора
прописать считывание файла черерз scanf
записать в вектор значение 
*/

#include<stdio.h>
#include <stdlib.h>
typedef struct vector {
    int re;
    int im;
    int row;
    int column;
    int max_row, max_column;
    struct vector *next;
}Node;


Node *create() // создание корня дерева
{
    Node *tmp = malloc(sizeof(Node));
    tmp -> re = tmp -> im = 0;
    tmp -> row = tmp -> column = 0;
    tmp -> next = NULL;
    return tmp;
}


Node *add(Node *root) // добавление узла дерева
{
    Node *new_root = malloc(sizeof(Node));
    new_root = create();

    while(root->next) // root->brother != NULL
        root = root -> next;  // дошли до последней ненудевлой ноды
    root -> next = new_root; // забили в нулевого брата ноду new_root
    return new_root;
}

int get_row(Node *root){
    return root->max_row;
}

int get_column(Node *root){
    return root->max_column;
}


// сначала нужно проинициализировать потом запринтить через структуру

Node *init_vector(Node *root){
    int re, im;
    int row, column;
    Node *root2 = root;

    FILE *myfile;
    myfile = fopen("/home/kitanosa/labs/kp7/matrix.txt", "r");

    fscanf(myfile, "%d %d",&row, &column);
    printf("\nРазмеры матрицы %d %d\n", row, column);
    root->max_row = row;
    root->max_column = column;

    for(int i = 0;i<row;i++){
        for(int j = 0;j<column;j++){
            fscanf(myfile, "%d %d",&re, &im);
            root2 -> re = re;
            root2 -> im = im;
            root2 -> row = i;
            root2 -> column = j;
            root2 -> max_column = column;
            root2 -> max_row = row;
            root2 = add(root2);
        }
    }
    fclose(myfile);
    return root;
}


void print_matrix(Node *root, int row, int column){
    for(int i = 0;i<row;i++){
        for(int j = 0;j<column;j++){
            printf(" %d %d  ",root->re,root->im);
            root = root -> next;
        }
        printf("\n");
    }
}

void print_vector(Node *root, int row, int column){
    for(int i = 0;i<row;i++){
        printf("  |%d/%d|  ", 0, root->row);// печать нуля и номера строки
        for(int j = 0;j<column;j++){
            if(root->re || root->im) {
                printf("|%d|%d+(%d*i)| ", root->column, root->re, root->im);// печать столбца и значения
            }
            root = root -> next;// переход к следующей ноде
        }
    }
    printf("  |%d/%d|\n", 0, 0);
}


int find_nonzero_value(Node *root, int value){// ищет первое ненулевое число чтобы с чем было сравнивать в find_near_value. Не с нулем а уже с каким то ненулевым числом
    if(root->re && root->re != value)
        return root->re;
    else if(root->im && root->im != value)
        return root->im;
    else
        find_nonzero_value(root->next, value);
}


int find_near_value(Node *root, int value, int near_value){
    int now_value;
    if(root->re != 0) {
        now_value = root->re;
        if (abs(value - now_value) < abs(value - near_value) && now_value &&  value != now_value && value != near_value)
            near_value = now_value;
    }
    if(root->im != 0) {
        now_value = root->im;
        if (abs(value - now_value) < abs(value - near_value) && now_value &&  value != now_value && value != near_value)
            near_value = now_value;
    }
    if(root->next)
        return find_near_value(root->next, value, near_value);
    return near_value;
}

Node *matrix_copy(Node *root){ // root2 = treeCopy(&root) копирование дерева для вывода измененного дерева
    Node *root2;

    root2 = create();
    root2 -> re = root->re;
    root2 -> im = root->im;
    root2 -> row = root->row;
    root2 -> column = root->column;
    root2->max_column = root->max_column;
    root2->max_row = root->max_row;
    if(root->next)
        root2->next = matrix_copy(root->next);
    return root2; // возвращает корень дерева скопированного
}

int delete_row_column(Node *root2, int row, int column, int value) {// берем row column из root и удаляем в root2
    for (int i = 0; i < root2->max_row; i++) {
        for (int j = 0; j < root2->max_column; j++) {
            if (root2->row == row && root2->column == column) {
                root2->re = (root2->re) * 2;
                root2->im = (root2->im) * 2;
            }
            else if (root2->row == row || root2->column == column) {
                root2->re = (root2->re) * 2;
                root2->im = (root2->im) * 2;
            }
            root2 = root2->next;
        }
    }
}


    Node *find_node_delete(Node *root, Node *root2, int value){
        for(int i = 0;i<root->max_row;i++) {
            for (int j = 0; j < root->max_column; j++) {
                if (root->re == value)
                    delete_row_column(root2, root->row, root->column, value);
                if (root->im == value)
                    delete_row_column(root2, root->row, root->column, value);
                root = root->next;
            }
        }
        return root2;
    }

// будем обходить root и искать row column и удалять все в root 2



// сделать копию матрицы и с ней работать

    int main(int argc, char* argv[]) {
        int action;
        int row, column;
        int value, nonzero_value, near_value;
        Node *root, *root2;

        while (1) {
            printf("Меню:\n");
            printf("1) Вывести матрицу\n");
            printf("2) Вывести вектор\n");
            printf("3) Вывести перобразованный матрицу\n");
            printf("4) Вывести преобразованную вектор\n");
            printf("5) Выход\n");
            printf("Выберите действие: ");
            scanf("%d", &action);

            switch (action) {
                case 1: {
                    root = create();
                    root = init_vector(root);
                    row = get_row(root);
                    column = get_column(root);
                    print_matrix(root, row, column);

                    /*
                    while(fscanf(myfile, "%d %d",&row, &column) != EOF)
                    {
                        print_matrix(row, column)

                        if(row && column)
                            printf("\nРазмеры матрицы %d %d\n", row, column);

                    }
                    */
                    break;
                }

                case 2: {
                    print_vector(root, row, column);
                    break;
                }

                case 3: {

                    printf("Введите элемент для поиска: ");
                    scanf("%d", &value);
                    nonzero_value = find_nonzero_value(root, value);
                    near_value = find_near_value(root, value, nonzero_value);
                    printf("Ближайшее значение %d\n", near_value);
                    root2 = matrix_copy(root);
                    root2 = find_node_delete(root, root2, near_value);
                    print_matrix(root2, row, column);

                    break;
                }

                case 4:
                {
                	print_vector(root2, row, column);
                    break;
                }

                case 5:
                    break;

                default:
                {
                    printf("Ошибка. Такого пункта меню не существует\n");

                    break;
                }
            }

            if (action == 5)
                break;
        }
    }
