//
// Created by mark on 2021/6/15.
//

#ifndef DATASTRUCTURE_PRIORITYQUEUE_HPP
#define DATASTRUCTURE_PRIORITYQUEUE_HPP

#include <jni.h>
#include <queue>
#include <android/log.h>


#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
/**
 * 大根堆 底层容器是数组array
 *
 */
namespace c9{
    template <class T>
    class PriorityQueue {
        int count; //数组大小，不够要扩容
        int index = 0; //当前数据的角标

        T *array = nullptr;//数据数组
    private:
        void shiftUp(int i){
            if (i > 1 && array[i] > array[i / 2]){
                std::swap(array[i],array[i/2]);
                shiftUp(i/2);
            }
        }

        void shifDown(int i) {
            while ( i*2 <= index ){ //到底的情况
                //最大指向左孩子
                int max = 2*i;
                if (max + 1 <= index && array[max +1] > array[max]) {
                    //代表有右孩子，且大于左孩子
                    max = max +1;
                }
                //最大的自己
                if (array[i] > array[max]){
                    break;
                }
                //最大往上冒
                std::swap(array[i],array[max]);
                i = max;
            }
        }


        void buildHeap(){
            for (int i = count/2; i>=1;--i){
                shifDown(i);
            }
        }

    public:
        PriorityQueue(int count ){
            this->count = count;
            array = new T[count];
        }

        PriorityQueue(int count, T *array) : count(count), array(array) {
            buildHeap();
        }

        bool isEmpty() {
            return index ==0;
        }

        void push(T e) {
            index++ ;
            array[index] = e;
            shiftUp(index);
        }

        //弹出最大值，然后把最后一个放入，再整理
        T pop() {
            T max = array[1];
            array[1] = array[index];
            index--;
            shifDown(1);
            return max;
        }

    };

    void test_priorityQueue() {
        int n = 10;
        PriorityQueue<int> pQueue(n);
        for (int i =0;i <n ;++i) {
            pQueue.push(rand() % 100);
        }
        while (!pQueue.isEmpty()){
            LOGI("%d",pQueue.pop());
        }
    }
}


#endif //DATASTRUCTURE_PRIORITYQUEUE_HPP
