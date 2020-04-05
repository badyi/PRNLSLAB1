#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

template <typename T>
class Stack {
private:
    T* data;
    int size;

    void allocate() {
        data = new T[size];
    }

    void deallocate() {
        delete[] data;
    }

    Stack(int n) {
        size = n;
        allocate();
        for (int i = 0; i < size; i++)
            set(NULL, i);
    }
public:
    Stack() {
        data = NULL;
        size = 0;
    }

    //конструктор копирования
    Stack(const Stack<T>& copiedStack) {
        Stack<T> tempStack = Stack<T>();
        int sizeCS = copiedStack.size;
        size = 0;
        for (int i = 0; i < sizeCS; i++)
            tempStack.push(copiedStack.data[sizeCS - i - 1]);
        for (int i = 0; i < sizeCS; i++)
            push(tempStack.pop());
    }

    //деструктор
    ~Stack() {
        size = 0;
        deallocate();
    }

    //очистить стек до n позиции
    void clear(const int pos) {
        const int n = size - pos;
        for (int i = 0; i < n; i++) {
            pop();
        }
    }

    //добавление в стек
    void push(const T value) {
        T* n_ptr = new T[size + 1];
        memcpy(n_ptr, data, size * sizeof(T));
        *(n_ptr + size) = value;
        size += 1;
        deallocate();
        data = n_ptr;
    }

    //изъятие последнего элемента
    T pop() {
        T value = *(data + size - 1);
        memmove(data, data, (size - 1) * sizeof(T));
        size--;
        return value;
    }

    //последний элемент
    T back() {
        T key = data[size - 1];
        return key;
    }

    // метод для вывода всего стека слева направо
    void printAll() {
        for (int i = 0; i < size; i++) {
            cout << "[" << data[i] << "] ";
        }
    }

    //проверка пустой ли стек
    bool isEmpty() {
        return size == 0;
    }

    T& operator[] (const int index) {
        Stack temp = *this;
        const int countOfPops = temp.size - index;
        int result;
        for (int i = 0; i < countOfPops; i++)
            result = temp.pop();
        return result;
    }

    //получить n-й элемент по индексу
    T get(const int index) {
        Stack temp = *this;
        return temp[index];
    }

    //установить новое значение на n-ю позицию
    void set(const T newValue, const int pos) {
        Stack temp = *this;
        clear(pos);
        push(newValue);
        for (int i = pos + 1; i < temp.size; ++i)
            push(temp[i]);
    }

    T MaxValue() {
        T max = back();
        for (int i = 0; i < size; i++) {
            T value = get(i);
            if (value > max)
                max = value;
        }
        return max;
    }

    T MinValue() {
        T min = back();
        for (int i = 0; i < size; i++) {
            T value = get(i);
            if (value < min)
                min = value;
        }
        return min;;
    }

    int getSize() {
        return size;
    }

    void BubbleSort() {
        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                if (get(i) > get(j)) {
                    T temp = get(j);
                    set(get(i), j);
                    set(temp, i);
                }
            }
        }
    }

    //сортировка подсчетом
    void CountingSort() { //работает только с int
        const T max = MaxValue();
        const T min = MinValue();
        const int sizeOfTempStack = max - min + 1;
        Stack<T> tempStack = Stack<T>(sizeOfTempStack);

        //считает кол-во повторений элемента
        for (int i = 0; i < size; i++) {
            int oldValue = tempStack.get(get(i) - min);
            tempStack.set(oldValue + 1, get(i) - min);
        }

        //прибавляет значение [i-1] к [i]
        for (int i = 1; i < sizeOfTempStack; i++) {
            int oldValue = tempStack.get(i);
            tempStack.set(oldValue + tempStack[i - 1], i);
        }

        Stack<T> tempStack2 = Stack<T>(sizeOfTempStack);
        //вычисляет индекс начала каждого элемента
        for (int i = 0; i < sizeOfTempStack - 1; i++) {
            tempStack2.set(tempStack.get(i), i + 1);
        }

        //выставляет элемент по своему индексу
        Stack<T> sortedStack = Stack<T>(size);
        for (int i = 0; i < size; i++) {
            const int value = get(i);
            const int pos = tempStack2.get(value - min);
            tempStack2.set(pos + 1, value - min);
            sortedStack.set(value, pos);
        }

        for (int i = 0; i < size; i++)
            set(sortedStack.get(i), i);
    }
};


class Estimator {
private:
    Stack<int> container;
public:
    void createPositiveRandomValues(const int size, int maxPosibleValue) {
        container = Stack<int>();
        for (int i = 0; i < size; i++)
            container.push(rand() % (maxPosibleValue + 1));
    }

    void estimateCountingSort() {
        cout << "------Counting Sort-----";
        cout << endl << "Size of container: " << container.getSize() << ", type of container: Stack" << endl;
        cout << "begin values: ";
        container.printAll();
        cout << endl;

        const clock_t begin_time = clock();
        container.CountingSort();
        float time = float(clock() - begin_time) / CLOCKS_PER_SEC;

        cout << "sorted values: ";
        container.printAll();
        cout << endl << "Counting Sort time: " << time << " sec." << endl;
        cout << "------------------------" << endl;
    }

    void estimateBubbleSort() {
        cout << "------Bubble Sort-----";
        cout << endl << "Size of container: " << container.getSize() << ", type of container: Stack" << endl;
        cout << "begin values: ";
        container.printAll();
        cout << endl;

        const clock_t begin_time = clock();
        container.BubbleSort();
        float time = float(clock() - begin_time) / CLOCKS_PER_SEC;

        cout << "sorted values: ";
        container.printAll();
        cout << endl << "Bubble Sort time: " << time << " sec." << endl;
        cout << "--------------------" << endl;
    }

    void test() {
        container = Stack<int>();
        container.push(1);
        container.push(4);
        container.push(1);
        container.push(2);
        container.push(7);
        container.push(5);
        container.push(2);
    }
};

int main(int argc, const char* argv[]) {
    srand(time(NULL));
    Estimator estimator = Estimator();
    int i = 0;
    while (i++ < 5) {
        estimator.createPositiveRandomValues(10, 10);
        estimator.estimateCountingSort();
    }

    while (i-- > 0) {
        estimator.createPositiveRandomValues(100, 10);
        estimator.estimateCountingSort();
    }
    return 0;
}