#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

template <typename T>
class Stack {
private:
    T* data;
    int size;

    //выделение памяти
    void allocate() {
        data = new T[size];
    }
    
    //удаление из памяти
    void deallocate() {
        delete[] data;
    }

    //инициализатор пустого стека n-размера заполненного 0
    Stack(int n) {
        for (int i = 0; i < n; i++)
            push(NULL);
    }
public:
    //инициализатор пустого стека
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

    // отчистить стек
    void clear() {
        size = 0;
        deallocate();
        data = nullptr;
    }

    //метод добавление в стек нового элемента
    void push(const T value) {
        T* n_ptr = new T[size + 1];
        memcpy(n_ptr, data, size * sizeof(T));
        *(n_ptr + size) = value;
        size += 1;
        deallocate();
        data = n_ptr;
    }

    //изъятие последнего элемента из стека
    T pop() {
        T value = *(data + size - 1);
        memmove(data, data, (size - 1) * sizeof(T));
        size--;
        return value;
    }

    //возвращает последний элемент стека
    T back() {
        return data[size - 1];
    }

    // метод для вывода всего стека слева направо
    void printAll() {
        for (int i = 0; i < size; i++) {
            cout << "[" << get(i) << "] ";
        }
    }

    //проверка пустой ли стек
    bool isEmpty() {
        return size == 0;
    }

    //получение i-го элемента через сабскрипты
    T operator[] (const int index) {
        return get(index);
    }

    //получение i-го элемента по индексу
    T get(const int index) {
        Stack temp = *this;
        const int countOfPops = temp.size - index;
        int result;
        for (int i = 0; i < countOfPops; i++)
            result = temp.pop();
        return result;
    }

    //установить новое значение на n-ю позицию
    void set(const T newValue, const int pos) {
        Stack temp = *this;
        clear(pos);
        push(newValue);
        for (int i = pos + 1; i < temp.size; ++i)
            push(temp[i]);
    }

    //получение максимального значения
    T MaxValue() {
        T max = back();
        for (int i = 0; i < size; i++) {
            T value = get(i);
            if (value > max)
                max = value;
        }
        return max;
    }

    //получнеие минимального значения
    T MinValue() {
        T min = back();
        for (int i = 0; i < size; i++) {
            T value = get(i);
            if (value < min)
                min = value;
        }
        return min;;
    }

    //получение размера стека
    int getSize() {
        return size;
    }
    
    //сортировка подсчетом
    void CountingSort(const int min, const T max) { //работает только с int
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

        int _size = size;
        clear();
        for (int i = 0; i < _size; i++)
            push(sortedStack.get(i));
    }
};

    enum Sorts {
        CountingSort,
        BubbleSort,
        HeapSort,
        QuickSort
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

        float estimateCountingSort() {
            cout << "------Counting Sort-----";
            cout << endl << "Size of container: " << container.getSize() << ", type of container: Stack,";

            const int max = container.MaxValue();
            const int min = container.MinValue();
            const clock_t begin_time = clock();
            container.CountingSort(min, max);
            const clock_t end_time = clock();
            const float time = diffclock(end_time, begin_time);
            Sorts current = CountingSort;
            int operationsCount = evaluateOperations(current, container.getSize());
            cout << " Sort time: " << time << " ms. or " << time / CLOCKS_PER_SEC << " sec. operations: " << operationsCount << endl;
            cout << endl;
            return time;
        }

        double diffclock(clock_t clock1, clock_t clock2) {
            return double(clock1 - clock2);
        }
    
        int evaluateOperations(Sorts type,int size) {
            int n = size;
            switch (type) {
                case CountingSort:
                   return 65 * n * n * n + 282 * n * n + 388 * n + 76;
                    break;
                default:
                  return 0;
            }
        }
    };


    int main(int argc, const char* argv[]) {
        srand(time(NULL));
        Estimator estimator = Estimator();
        int maxValue = 10;
        Stack<float> time = Stack<float>();
        int sizesOfStacks[] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000 };
        int countOfTestings = sizeof(sizesOfStacks) / sizeof(*sizesOfStacks);

        for (int i = 0; i < countOfTestings; i++) {
            estimator.createPositiveRandomValues(sizesOfStacks[i], maxValue);
            float k = estimator.estimateCountingSort();
            time.push(k);
        }


        cout << "final statistic:" << endl;
        for (int i = 0; i < countOfTestings;  i++) 
            cout << "size = " << sizesOfStacks[countOfTestings - i - 1] << " t = " << time.pop() / CLOCKS_PER_SEC << " sec\n";
    
        return 0;
    }