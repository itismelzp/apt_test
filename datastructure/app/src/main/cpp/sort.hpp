//
// Created by mark on 2021/5/23.
//

#ifndef DATASTRUCTURE_SORT_HPP
#define DATASTRUCTURE_SORT_HPP

#include <jni.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <android/log.h>

#define TAG "JNI_TAG"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

namespace c9 {
    /**
     * 冒泡排序 从小到大 优化前
     * @param arr
     * @param len
     */
    void bubbleSort(int arr[], int len);

    /**
     * 冒泡排序 从小到大 优化后
     * @param arr
     * @param len
     */
    void optimizeBubbleSort(int arr[], int len);

    /*
     * 选择排序 从小到大
     */
    void selectSort(int arr[], int len);

    /**
     * 插入排序 （适用于接近有序）
     * @param arr
     * @param len
     */
    void insertSort(int arr[], int len);

    /**
     * 希尔排序
     * @param arr
     * @param len
     */
    void shellInsertSort(int arr[], int len);


    /**
     * 归并排序
     * @tparam T 泛型
     * @param aar
     * @param len
     */
    template<class T>
    void mergeSort(T arr[], int len);

    template<class T>
    void mergeSort_(T arr[], int left, int right);

    template<class T>
    void merge_(T arr[], int left, int mid, int right);

    /**
     * 快速排序
     * @tparam T
     * @param arr
     * @param len
     */
    template<class T>
    void quickSort(T arr[], int len);

    //对数组区间[left，right]，进行快速排序
    template<class T>
    void quickSort_(T arr[], int left, int right);

    template<class T>
    int partition(T arr[], int left, int right);

    //适用于不确定场景（大量重复）
    template<class T>
    void quickSort3ways(T arr[], int len);

    template<class T>
    void quickSort3ways_(T arr[], int left, int right);


    /**
     * 算法测试方法
     */
    void sort_array(char *sortName, void (*sort)(int *, int), int *arr, int len);
}

void c9::bubbleSort(int *arr, int len) {
    for (int i = 0; i < len - 1; ++i) {
        bool swaped = false;
        for (int j = 0; j < len - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                swaped = true;
                std::swap(arr[j], arr[j + 1]);
            }
        }
        if (!swaped) {
            break;
        }
    }
}

void c9::optimizeBubbleSort(int *arr, int len) {
    // 记录上一次最后的位置
    int last = len - 1;
    int temp = 0;
    do {
        temp = 0;
        for (int i = 0; i < last; ++i) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i + 1], arr[i]);
                temp = i;
            }
        }
        last = temp;
    } while (last > 0);
}

void c9::selectSort(int *arr, int len) {
    for (int i = 0; i < len; ++i) {
        int min = i;
        for (int j = i + 1; j < len; ++j) {
            if (arr[min] > arr[j]) {
                min = j;
            }
        }
        if (min != i) {
            std::swap(arr[min], arr[i]);
        }
    }
}

void c9::insertSort(int arr[], int len) {
    //优化前
    /*for (int i = 1; i < len; i++) {
        for (int j = i; j > 0; --j) {
            if (arr[j] < arr[j - 1]) {
                std::swap(arr[j-1], arr[j]);
            } else {
                break;
            }
        }
    }*/

    int j;
    for (int i = 1; i < len; i++) {
        //用临时变量减少swap中的三次交换次数
        int temp = arr[i];
        for (j = i; j > 0 && arr[j - 1] > temp; --j) {
            arr[j] = arr[j - 1];
        }
        arr[j] = temp;
    }
}

void c9::shellInsertSort(int *arr, int len) {
    //分组 步长
    int increment = len / 2;
    int i, j, k;
    while (increment > 0) {
        //希尔排序
        for (i = 0; i < increment; ++i) {
            for (j = i + increment; j < len; j += increment) {
                int temp = arr[j];
                for (k = j; k > i && arr[k - increment] > temp; k -= increment) {
                    //std::swap(arr[k],arr[k - increment]);
                    arr[k] = arr[k - increment];
                }
                arr[k] = temp;
            }
        }
        increment /= 2;
    }

}

template<class T>
void c9::mergeSort(T *arr, int len) {
    c9::mergeSort_(arr, 0, len - 1);
}

//进行递归
template<class T>
void c9::mergeSort_(T *arr, int left, int right) {
    using namespace c9;
    //递归到底
    if (left >= right) {
        return;
    }

    int mid = (left + right) >> 1;
    mergeSort_(arr, left, mid);
    mergeSort_(arr, mid + 1, right);
    if (arr[mid] > arr[mid +1]) {
        merge_(arr, left, mid, right);
    }
}

//进行归并
template<class T>
void c9::merge_(T *arr, int left, int mid, int right) {
    //对数组进行一次拷贝
    T temp[right -left + 1];
    for (int i = left; i <= right; ++i) {
        temp[i - left] = arr[i];
    }

    int i = left;
    int j = mid + 1;
    int k = left;
    for (; k <= right; ++k) {
        if (i > mid) {
            arr[k] = temp[j - left];
            j++;
        } else if (j > right) {
            arr[k] = temp[i - left];
            i++;
        } else if (temp[i - left] < temp[j - left]) {
            arr[k] = temp[i - left];
            i++;
        } else {
            arr[k] = temp[j - left];
            j++;
        }
    }
}
template<class T>
void c9::quickSort(T *arr, int len) {
    srand(time(NULL));
    //递归到底
    c9::quickSort_(arr,0,len-1);
}
template<class T>
void c9::quickSort_(T *arr, int left, int right) {
    if (left > right) {
        return;
    }

    int pos = c9::partition(arr,left,right);
    c9::quickSort_(arr,left,pos-1);
    c9::quickSort_(arr,pos +1 ,right);
}
//分割
template<class T>
int c9::partition(T *arr, int left, int right) {
    //优化 跟随机位比较 优化降低了乱序效率，增加了近似排序的速度
    std::swap(arr[left],arr[rand() % (right -left+1 ) +left]);
    int v = arr[left];
    int pos = left;

    for (int i = left; i <=right; ++i) {
        if (v > arr[i]) {
            pos++;
            std::swap(arr[pos],arr[i]);
        }
    }
    std::swap(arr[left],arr[pos]);
    return pos;
}

template<class T>
void c9::quickSort3ways(T *arr, int len) {
    srand(time(NULL));
    //递归到底
    c9::quickSort3ways_(arr,0,len-1);
}

template<class T>
void c9::quickSort3ways_(T *arr, int left, int right) {
    if (left > right) {
        return;
    }

    //定义
    //优化 跟随机位比较 优化降低了乱序效率，增加了近似排序的速度
    std::swap(arr[left],arr[rand() % (right -left+1 ) +left]);
    int v = arr[left];
    int lessPoint = left; //[left + 1 ,lessPoint] < v
    int greaterPoint = right + 1; //[ greaterPoint , right ] > v
    int i = left +1; //[lessPoint + 1 , i] = v
    while ( greaterPoint > i ) {
        if (arr[i] > v) {
            greaterPoint--;
            std::swap(arr[i],arr[greaterPoint]);
        } else if (arr[i] < v) {
            lessPoint++;
            std::swap(arr[i],arr[lessPoint]);
            i++;
        } else {
            i++;
        }
    }
    std::swap(arr[left],arr[lessPoint]);
    //int pos = c9::partition(arr,left,right);
    c9::quickSort_(arr,left,lessPoint-1);
    c9::quickSort_(arr,greaterPoint ,right);
}



void c9::sort_array(char *sortName, void (*sort)(int *, int), int *arr, int len) {
    {
        size_t start_time = clock();
        sort(arr, len);
        size_t end_time = clock();
        double time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
        LOGD("%s的执行时间%lf", sortName, time);
        //检查数组是否排序
        for (int i = 0; i < len - 1; ++i) {
            assert(arr[i] <= arr[i + 1]);
        }
    }
}


#endif //DATASTRUCTURE_SORT_HPP
