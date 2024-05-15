#include <iostream>
#include <string>

using namespace std;

struct Stack
{
    string value;
    Stack* next;
};

int sizeStack(Stack* head)
{
    int count = 0;
    while (head)
    {
        count++;
        head = head->next;
    }
    return count;
}

void deleteStack(Stack* head)
{
      Stack* prev = 0;
      while (head->next)
      {
          prev = head->next;
          free(head);
          head = prev;
      }
}

void printStack(const Stack* head)
{
    while (head)
    {
        cout << head->value << " ";
        head = head->next;
    }
    cout << '\n';
    return;
}

int weight(char operation)
{
    switch (operation)
    {
    case '+':
    case '-':
        return 1;
        break;
    case '*':
    case '/':
        return 2;
        break;
    default:
        return -1;
        break;
    }
}

void transport(Stack*& op, Stack*& res)
{
    while (op->next)
    {
        Stack* curr = new Stack;
        curr->value = op->value;
        curr->next = res;
        res = curr;
        op = op->next;
    }
    Stack* curr = new Stack;
    curr->value = op->value;
    curr->next = res;
    res = curr;
    op = op->next;
}

void znak(Stack*& op, char a, Stack*& res)
{
    Stack* oper = new Stack;
    oper->value = a;
    if (op == 0)
    {
        oper->next = 0;
    }
    else
    {
        if (weight(a) < weight(op->value[0]))
        {
            transport(op, res);
            oper->next = 0;
        }
        else
        {
            oper->next = op;
        }
    }
    op = oper;
    cout << "Стек операторов: ";
    printStack(op);
}

void push(Stack*& head, string a)
{
    Stack* curr = new Stack;
    curr->value = a;
    if (head == 0)
    {
        curr->next = 0;
    }
    else
    {
        curr->next = head;
    }
    head = curr;
    cout << "Результирующий стек: ";
    printStack(head);
}

bool isOperation(char operation)
{
    if ((weight(operation) == 1) || (weight(operation) == 2))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int action(int num1, int num2, char operation)
{
    switch (operation)
    {
    case '+':
        return num1 + num2;
        break;
    case '-':
        return num1 - num2;
        break;
    case '*':
        return num1 * num2;
        break;
    case '/':
        return num1 / num2;
        break;
    default:
        break;
    }
}

Stack* prPolNot(int len, string s)
{
    string a;
    Stack* res = 0, * oper = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        if (isOperation(s[i]))
        {
            push(res, a);
            a = "";
            znak(oper, s[i], res);
        }
        else
        {
            a += s[i];
        }
    }
    push(res, a);
    transport(oper, res);
    oper = res;
    cout << "Польская нотация: ";
    printStack(res);
    return oper;
}

void obrTransport(Stack*& res)
{
    Stack* head = 0;
    while (res->next)
    {
        Stack* curr = new Stack;
        curr->value = res->value;
        if (head == 0)
        {
            curr->next = 0;
        }
        else
        {
            curr->next = head;
        }
        head = curr;
        res = res->next;
    }
    Stack* curr = new Stack;
    curr->value = res->value;
    curr->next = head;
    head = curr;
    res = head;
}

Stack* obrPolNot(int len, string s) 
{
    string a;
    Stack* res = 0, * oper = 0;
    for (int i = 0; i < len; i++)
    {
        if (isOperation(s[i]))
        {
            push(res, a);
            a = "";
            znak(oper, s[i], res);
        }
        else
        {
            a += s[i];
        }
    }
    push(res, a);
    transport(oper, res);
    obrTransport(res);
    oper = res;
    cout << "Обратная польская нотация: ";
    printStack(res);
    return oper;
}

void prCalc(Stack*& prNot)
{
    obrTransport(prNot);
    Stack* curr = 0, * head = 0;
    while (prNot)
    {
        if (!isOperation(prNot->value[0]))
        {
            if (prNot->value != "")
            {
                curr = new Stack;
                curr->next = head;
                curr->value = prNot->value;
                head = curr;
            }
        }
        else
        {
            if (head->next == 0)
            {
                cout << "Ошибка!";
                return;
            }
            else
            {
                head->next->value = to_string(action(stoi(head->value), stoi(head->next->value), prNot->value[0]));
                head = head->next;
                cout << "Операция: ";
                cout << prNot->value << '\n';
            }
        }
        prNot = prNot->next;
        cout << "Результирующий стек: ";
        printStack(head);
        cout << '\n';
    }
    cout << "Результат: ";
    cout << head->value;
}

void obrCalc(Stack*& obrNot)
{
    Stack* curr = 0, * head = 0;
    while (obrNot)
    {
        if (!isOperation(obrNot->value[0]))
        {
            if (obrNot->value != "")
            {
                curr = new Stack;
                curr->next = head;
                curr->value = obrNot->value;
                head = curr;
            }
        }
        else
        {
            if (head->next == 0)
            {
                cout << "Ошибка!";
                return;
            }
            else
            {
                head->next->value = to_string(action(stoi(head->next->value), stoi(head->value), obrNot->value[0]));
                head = head->next;
                cout << "Операция: ";
                cout << obrNot->value << '\n';
            }
        }
        cout << "Результирующий стек: ";
        printStack(head);
        obrNot = obrNot->next;
        cout << '\n';
    }
    cout << "Результат: ";
    cout << head->value;
}

void menu()
{
    bool p = true;
    while (p)
    {
        string s;
        Stack* prNot, * obrNot;
        cout << "Введите выражение(без пробелов): ";
        cin >> s;
        int len = size(s);
        prNot = prPolNot(len, s);
        cout << '\n' << '\n';
        obrNot = obrPolNot(len, s);
        cout << '\n' << '\n';
        string b, c;
        Stack* prov = obrNot;
        Stack* prova = prNot;
        while (prov)
        {
            if (!isOperation(prov->value[0]) && !isdigit(prov->value[0]) && prov->value != "")
            {
                cout << "Введите значение переменной " << prov->value << ': ';
                c = prov->value;
                cin >> prov->value;
                b = prov->value;
                while (prova->value != c)
                {
                    prova = prova->next;
                }
                prova->value = b;
            }
            prov = prov->next;
        }
        cout << "\nПольская нотация: ";
        printStack(prNot);
        cout << "\n" << "\n";
        prCalc(prNot);
        cout << '\n';
        cout << "\nОбратная польская нотация: ";
        printStack(obrNot);
        cout << "\n" << "\n";
        obrCalc(obrNot);
        cout << "\n" << "\n";
        int choise;
        cout << "1. Ввести пример еще раз\n0. Выход\n";
        cin >> choise;
        if (choise != 1)
        {
            p = false;
        }
    }
}

int main()
{
    setlocale(0, "");
    menu();
}

