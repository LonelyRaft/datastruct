
#include <stdio.h>
#include <stdlib.h>

// data source
// cmp
// swap

void sort_bubble(int *_vector, const unsigned int _size)
{
    if (0 == _vector ||
        0 == _size)
        return;
    for (int i = 0; i < _size; i++)
    {
        int reverse = 0;
        for (int j = 0; j < _size - i; j++)
        {
            if (_vector[j] > _vector[j + 1])
            {
                int tmp = _vector[j];
                _vector[j] = _vector[j + 1];
                _vector[j + 1] = tmp;
                reverse = 1;
            }
        }
        if (!reverse)
            break;
    }
}

void sort_insert(int *_vector, const unsigned int _size)
{
    if (0 == _vector || 0 == _size)
        return;
    for (int i = 1; i < _size; i++)
    {
        int tmp = _vector[i];
        int j = i - 1;
        while (j > -1)
        {
            if (tmp > _vector[j])
                break;
            _vector[j + 1] = _vector[j];
            j--;
        }
        _vector[j + 1] = tmp;
    }
}

void sort_select(int *_vector, const unsigned int _size)
{
    if (0 == _vector || 0 == _size)
        return;
    for (int i = 0; i < _size; i++)
    {
        int min = i;
        for (int j = i + 1; j < _size; j++)
        {
            if (_vector[min] > _vector[j])
                min = j;
        }
        if (min != i)
        {
            int tmp = _vector[i];
            _vector[i] = _vector[min];
            _vector[min] = tmp;
        }
    }
}

void sort_shell(int *_vector, const unsigned int _size)
{
    if (0 == _vector || 0 == _size)
        return;
    int gap = _size;
    while (gap > 1)
    {
        gap >>= 1;
        for (int i = gap; i < _size ; i++)
        {
            int tmp = _vector[i];
            int j = i - gap;
            while (j > -1)
            {
                if (tmp > _vector[j])
                    break;
                _vector[j + gap] = _vector[j];
                j -= gap;
            }
            _vector[j + gap] = tmp;
        }
    }
}



int main(int argc, char *argv[])
{

    int vector[] = {1, 3, 5, 7, 9, 2, 4, 6, 8, 0};
    sort_shell(vector, sizeof(vector) / sizeof(int));
    return 0;
}
