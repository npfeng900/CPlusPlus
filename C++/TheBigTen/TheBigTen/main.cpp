//
//  main.cpp
//  TheBigTen
//
//  Created by Niu Panfeng on 15/5/6.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#include <iostream>
#include "algorithm.h"

using namespace std;



int main(int argc, const char * argv[]) {
    
    int a[12]={9,3,2,4,12,4,3,7,0,-1,13,-3};
    int b[12];
    int size=12;
    Print(a,size);
    
    
    cout << "（一）快速排序算法：\n";
    memcpy(b, a, sizeof(int)*size);
    QuickSort(b,size);
    Print(b,size);
    
    cout << "（二）堆排序算法：\n";
    memcpy(b, a, sizeof(int)*size);
    HeapSort(b, size);
    Print(b,size);
   
    cout << "（三）归并排序算法：\n";
    memcpy(b, a, sizeof(int)*size);
    MergeSort(b, size);
    Print(b,size);
    
    cout << "（四）二分查找算法：\n";
    cout<<"有序数列：";
    Print(b,size);
    int goal=7;
    cout<<goal<<"的下标："<<BinSearch(b,size,goal)<<'\n';
    
    cout << "（五）BFPRT(线性查找算法)：\n";
    memcpy(b, a, sizeof(int)*size);
    int num=5;
    BFPRT(b, size,num);
    Print(b,num>size?size:num);

    /*
    int n=6;
    int path[6*6]={
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        1,1,0,0,1,0,
        0,0,0,0,1,1,
        0,0,1,1,0,1,
        0,0,0,1,1,0};
     */
    
    cout << "（六）DFS(深度优先搜索)：\n";
    //Print(path,n,n);
    //DFS(path, n);
    char ch[]={'A', 'B', 'D', '#', '#', 'E', '#', '#', 'C', 'F','#', '#', 'G', '#', '#'};
    Tree tree;
    int i=0;
    treeNodeConstructor(tree,ch,i);
    DFS(tree);
    cout<<"\n";
    
    cout << "（七）DFS(宽度优先搜索)：\n";
    //BFS(path, n);
    BFS(tree);
    cout<<"\n";
    
    cout << "（八）Dijkstra算法：\n";
    int w[][3]={{0,1,100},
                {0,2,30},
                {0,4,10},
                {2,1,60},
                {2,3,60},
                {3,1,10},
                {4,3,50}};
    int n=5,e=7, n0=0;
    MGraph m;
    InitMGraph(&m, n, e, w);
    int *d=new int[n];
    int *nPrev=new int[n];
    Dijkstra(&m,d,nPrev, n0);
    for (int i=0; i<n; i++)
    {
        cout<<n0<<"->"<<i<<"的长度："<<d[i]<<"; 路线：";
        ShowPath(nPrev, n0, i);
    }
    delete [] d;
    delete [] nPrev;
    
    cout << "（九）动态规划算法：\n";
    DP();
    
    cout << "（十）朴素贝叶斯分类算法：\n";
    NaiveBayes nb;
    nb.CreateVocabList();
    nb.GetTrianMat();
    //nb.Print();
    nb.trian_NB0();
    string doc1_to_classify[] = {"love","my","dalmation","null"};
    string doc2_to_classify[] = {"stupid","garbage","null"};
    cout<<"doc1 classified as : "<<nb.classify_NB( doc1_to_classify )<<endl;
    cout<<"doc2 classified as : "<<nb.classify_NB( doc2_to_classify )<<endl;
    
    
    return 0;
}
