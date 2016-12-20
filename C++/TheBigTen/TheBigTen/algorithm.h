//  algorithm.h
//  TheBigTen
//
//  Created by Niu Panfeng on 15/5/6.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#ifndef TheBigTen_algorithm_h
#define TheBigTen_algorithm_h

#include <Stack>
#include <Queue>
#include <Map>
#include <Vector>
#include <numeric>
#include <math.h>
using namespace std;

//打印数组
void Print(int* a,int size)
{
    for (int i=0; i<size; i++) {
        std::cout<<a[i]<<" ";
    }
    std::cout<<std::endl;
}
void Print(int* a,int size1,int size2)
{
    for (int i=0; i<size1; i++)
    {
        for(int j=0;j<size2;j++)
        {
            std::cout<<*(a+i*size2+j)<<" ";
        }
        std::cout<<std::endl;
    }
}

//交换
void Swap(int* a, int* b)
{
    //异或只能交换不相等的值
    if(*a==*b)
          return;
    else
    {
        *a=*a^*b;
        *b=*a^*b;
        *a=*a^*b;
    }
    
}

//////////十大算法/////////////////////////////

//快速排序算法
int iQuickSort(int* a, int left,int right)
{
    if(left>right)
        return -1;
    
    int key=a[left];
    int low=left;
    int high=right;
    
    while (low<high)
    {
        while(low<high && a[high]>=key)
        {
            high--;
        }
        a[low]=a[high];
        
        while(low<high && a[low]<=key)
        {
            low++;
        }
        a[high]=a[low];
    }
    
    a[low]=key;
    iQuickSort(a, left, low-1);
    iQuickSort(a, low+1, right);
    
    return 0;
}
int QuickSort(int* a,int size)
{
    iQuickSort(a, 0, size-1);
    return 0;
}

//堆排序算法
void MaxHeapAdjust(int* a,int size,int i)
{
    int lchild=2*i;
    int rchild=2*i+1;
    int max=i;
    
    if(i<=size/2)
    {
        if(lchild<=size && a[lchild-1]>a[max-1])
            max=lchild;
        if(rchild<=size && a[rchild-1]>a[max-1])
            max=rchild;
        if(max!=i)
        {
            Swap(a+max-1, a+i-1);
            MaxHeapAdjust(a, size, max);
        }
    }
}
void MakeMaxHeap(int* a,int size)
{
    for (int i=size/2; i>0; i--)
    {
        MaxHeapAdjust(a,size,i);
    }
}
int HeapSort(int* a,int size)
{
    MakeMaxHeap(a, size);
    
    for (int i=size-1; i>0;i-- )
    {
        Swap(a,a+i);
        MakeMaxHeap(a, i);
    }
    
    return 0;
}

//归并排序算法
void MergeArray(int* a,int first,int middle,int last,int* temp)
{
    int i=first,j=middle+1,k=0;
    
    while (i<=middle && j<=last)
    {
        if (a[i]<a[j])
            temp[k++]=a[i++];
        else
            temp[k++]=a[j++];
    }
    
    while(i<=middle) //必须为小于等于，否者序列末尾的数值加不上
    {
        temp[k++]=a[i++];
    }
    while(j<=last)
    {
        temp[k++]=a[j++];
    }
    
    for(i=0;i<k;i++)
    {
        a[first+i]=temp[i];
    }
    
}
int iMergeSort(int* a,int first,int last,int* temp)
{
    if(first<last)
    {
        int middle=(first+last)/2;
        iMergeSort(a, first, middle, temp);
        iMergeSort(a, middle+1, last, temp);
        MergeArray(a, first, middle, last, temp);
    }
    return 0;
}
int MergeSort(int* a,int size)
{
    int* temp=new int[size];
    if (temp==NULL)
        return -1;
    
    iMergeSort(a, 0, size-1, temp);
    delete[] temp;
    
    return 0;
}

//二分查找算法
int BinSearch(int* a,int size,int goal)
{
    int low=0,high=size-1;
    int middle;
    while (low<=high)
    {
        middle=(low+high)/2;
        if (a[middle]==goal)
            return middle;
        else if (a[middle]>goal)
            high=middle-1;
        else if (a[middle]<goal)
            low=middle+1;
    }
    return -1;
}

//BFPRT(线性查找算法)：
void BubbleSort(int* a,int size)
{
    for (int i=size; i>0; i--)
    {
        for (int j=0; j+1<i; j++)
        {
            if (a[j]>a[j+1])
                Swap(&a[j], &a[j+1]);
        }
    }
}
int Partition(int* a,int front,int last,int x)
{
    if(front>last)
        return -1;
    
    int low=front;
    int high=last;
    
    while (low<high)
    {
        while (low<high && a[low]<x)
        {
            low++;
        }
        
        while (low<high && a[high]>=x)
        {
            high--;
        }
        
        if(low<high)
            Swap(a+low, a+high);
        
    }
    if (low==last && a[low]<x)
        low=low+1;
    
    return low-1;
 
}
int FindMiddle(int* a,int first,int last,int k)
{
    if(last-first+1<=5)
    {
        BubbleSort(a+first, last-first+1);
        return a[first+k-1];
    }
    
    int i=0;
    //分别找出各组的中位数，再将各组的中位数与数组开头的数据按组的顺序依次交换
    for(i=0;i<(last-first+1)/5;i++)
    {
        int begin=first+5*i,end=begin+4;
        BubbleSort(a+begin, end-begin+1);
        Swap(a+first+i, a+begin+2);
    }
    //对这些各组的中位数
    //按同样的方法继续求出中位数，最后得出的整个数组的中位数 x
    int x=FindMiddle(a, first, first+(last-first+1)/5-1,(last-first+6)/10);
    i=Partition(a, first, last, x);
    int j=i-first+1;
    
    if(k<=j)
        return FindMiddle(a, first,i,k);
    else
        return FindMiddle(a, i+1, last,k-j);
}
void BFPRT(int* a,int size,int num)
{
    FindMiddle(a,0,size-1,num>size?size:num);
}

//DFS（深度优先搜索）
/*
int Visit(int* path,int* pathOpt,int nodeSize,int* nodePast,int node)
{
    std::cout<<node<<" ";
    nodePast[node]=1;
    for (int i=0; i<nodeSize; i++)
    {
        if (path[node*nodeSize+i]==1 && nodePast[i]!=1)
        {
            pathOpt[node*nodeSize+i]=1;
            Visit(path,pathOpt,nodeSize,nodePast,i);
        }
    }
    
    return 0;
}
int DFS(int* path,int nodeSize)
{
    int* nodePast=new int[nodeSize];
    int* pathOpt=new int[nodeSize*nodeSize];
    memset(nodePast, 0, sizeof(int)*nodeSize);
    memset(pathOpt,0,sizeof(int)*nodeSize*nodeSize);

    std::cout<<"遍历路径为：";
    Visit(path, pathOpt, nodeSize, nodePast, 0);
    std::cout<<"\n";
    Print(pathOpt, nodeSize,nodeSize);
    
    
    delete [] nodePast;
    delete [] pathOpt;
    
    return 0;
}*/
typedef struct Node
{
    char value;
    struct Node* lchild;
    struct Node* rchild;
}*Tree;
int treeNodeConstructor(Tree &root,char* data,int &index)
{
    char e=data[index++];
    if(e=='#')
        root=NULL;
    else
    {
        if( !(root=(Node*)malloc(sizeof(Node))) )//注意本程序没有free空间
            return -1;
        else
        {
            root->value=e;
            treeNodeConstructor(root->lchild, data,index);
            treeNodeConstructor(root->rchild, data,index);
        }
    }
    return 0;
}
void DFS(Tree root)
{
    stack<Node*> nodeStack;
    nodeStack.push(root);
    Node* node;
    while (nodeStack.empty()==false)
    {
        node=nodeStack.top();
        cout<<node->value<<" ";
        nodeStack.pop();
        if(node->lchild)
            nodeStack.push(node->lchild);
        if(node->rchild)
            nodeStack.push(node->rchild);
        
    }
    
}

//BFS（宽度优先搜索）
/*int BFS(int* path,int nodeSize)
{
    return 0;
}
*/
void BFS(Tree root)
{
    queue<Node*> nodeQueue;
    nodeQueue.push(root);
    Node* node;
    while (nodeQueue.empty()==false)
    {
        node=nodeQueue.front();
        cout<<node->value<<" ";
        nodeQueue.pop();
        if(node->lchild)
            nodeQueue.push(node->lchild);
        if(node->rchild)
            nodeQueue.push(node->rchild);
        
    }
    
}

//Dijkstra算法
typedef struct iNode
{
    int** matrix;
    int n;//顶点
    int e;//边数
}MGraph;
int InitMGraph(MGraph* g,int n,int e,int w[][3])
{
    g->n=n;
    g->e=e;
    g->matrix=new int*[n];
    for(int i=0;i<n;i++)
    {
        *(g->matrix+i)=new int[n];
        for(int j=0;j<n;j++)
        {
            if (i==j)
                *(*(g->matrix+i)+j)=0;
            else
                *(*(g->matrix+i)+j)=INT_MAX;
        }
    }
    
    for(int i=0;i<e;i++)
        *(*(g->matrix+w[i][0])+w[i][1])=w[i][2];
    return 0;
}
void Dijkstra(MGraph* g,int* nDistance,int* nPrev,int n0)
{
    bool* nVisit=new bool[g->n];
    for(int i=0;i<g->n;i++)
    {
        nVisit[i]=false;
        nDistance[i]=g->matrix[n0][i];
        if(nDistance[i]==INT_MAX)
            nPrev[i]=-1;
        else
            nPrev[i]=n0;
    }
    nDistance[n0]=0;
    nPrev[n0]=-1;
    nVisit[n0]=true;
    
    for(int k=0;k<g->n-1;k++)
    {
        int min=INT_MAX;
        int nSelect=n0;
        for (int i=0; i<g->n; i++)//选择下一个点
        {
            if(nVisit[i]==false && nDistance[i]<min)
            {
                nSelect=i;
                min=nDistance[nSelect];
            }
        }
        nVisit[nSelect]=true;
        for (int j=0; j<g->n; j++) //计算连接权值比重，以便选择最小的
        {
            if( nVisit[j]==false && g->matrix[nSelect][j]<INT_MAX)
            {
               if( nDistance[j] > (nDistance[nSelect]+g->matrix[nSelect][j]) )
               {
                   nDistance[j]=nDistance[nSelect]+g->matrix[nSelect][j];
                   nPrev[j]=nSelect;
               }
            }
            
        }
    }
    
    delete [] nVisit;
}
void ShowPath(int* nPrev,int n0,int n)
{
    stack<int> s;
    while(n!=n0)
    {
        s.push(n);
        n=nPrev[n];
    }
    s.push(n);
    while (!s.empty())
    {
        cout<<s.top()<<" ";
        s.pop();
    }
    cout<<endl;
}

//动态规划算法
//有N件物品和一个容量为V的背包。第i件物品的体积是c[i]，价值是v[i]。求解将哪些物品装入背包可使价值总和最大。
//我们把题目具体下， 有5个商品，背包的体积为10，他们的体积为 c[5] = {3,5,2,7,4};  价值为 v[5] = {2,4,1,6,5};
#define max(a,b) (a>b?a:b)
void DP()
{

    int c[6] = {0,3,5,2,7,4};//体积
    int v[6] = {0,2,4,1,6,5};//价值
    int f[6][11] = {0};
    
    for (int i=1;i<6; i++)
    {
        for (int j=1; j<11; j++)
        {
            if(c[i] > j)//如果背包的容量，放不下c[i]，则不选c[i]
                f[i][j] = f[i-1][j];
            else
            {
                f[i][j] = max(f[i-1][j], f[i-1][j - c[i]] + v[i]);//转移方程式
            }
            
        }
    }
    
    for (int i=0; i<6; i++)
    {
        for (int j=0; j<11; j++)
        {
            cout<<f[i][j]<<" ";
        }
        cout<<endl;
    }
    
    cout<<"最大值:"<<f[5][10]<<endl;
}


//朴素贝叶斯分类算法
/*
char vocbulary[1000][20];
int SplitToWord(char* text)
{
    int i=0;
    char seps[]=", .\n";
    char* subString;
    
    subString=strtok(text,seps);
    while(subString!=NULL)
    {
     //   strcpy(vocabulary[i],subString);
        subString=strtok(NULL, seps);
        i++;
        
    }
    return i;
}

int NaiveBayes(char* text)
{
    int vocabuNum;
    vocabuNum=SplitToWord(text);
    
    return 0;
}*/
string posting_list[6][10]={
    {"my","dog","has","flea","problems","help","please","null"},
    {"maybe","not","take","him","to","dog","park","stupid","null"},
    {"my","dalmation","is","so","cute","I","love","him","null"},
    {"stop","posting","stupid","worthless","garbage","null"},
    {"mr","licks","ate","my","steak","how","to","stop","him","null"},
    {"quit","buying","worthless","dog","food","stupid","null"}
};
int class_vec[6] = {0,1,0,1,0,1};   //1 is abusive ,0 not
class NaiveBayes {
private:
    vector<vector<string>> list_of_post;
    vector<int> list_classes;
    map<string,int> my_vocab_list;
    int *return_vec;
    vector<vector<int>> train_mat;
    vector<float> p0vect;
    vector<float> p1vect;
    float p_abusive;
public:
    NaiveBayes()
    {
        vector<string> vec;
        for (int i=0; i<6; i++)
        {
            vec.clear();
            for(int j=0;posting_list[i][j]!="null";j++)
                vec.push_back(posting_list[i][j]);
            list_of_post.push_back(vec);
        }
        for(int i=0;i<sizeof(class_vec)/sizeof(class_vec[0]);i++)
            list_classes.push_back(class_vec[i]);
    }
    
    void CreateVocabList()
    {
        vector<vector<string>>::iterator it=list_of_post.begin();
        int index=1;
        while(it!=list_of_post.end())
        {
            vector<string>::iterator it_tmp=it->begin();
            while(it_tmp!=it->end())
            {
                //记录单词第一次出现顺序
                if(my_vocab_list[*it_tmp]==0)
                    my_vocab_list[*it_tmp]=index++;
                it_tmp++;
            }
            
            it++;
        }
        
        map<string,int>::const_iterator itt = my_vocab_list.begin();
        while( itt!=my_vocab_list.end() )
        {
            cout<<itt->first<<" "<<itt->second<<"   ";
            itt++;
        }
        cout<<endl;
    }
    
    void SetOfWordsToVec(int idx)
    {

        int len=(int)my_vocab_list.size()+1;
        return_vec=new int[len]();
    
        for(int i=0;posting_list[idx][i]!="null";i++)
        {
            int pos=my_vocab_list[posting_list[idx][i]];
            if(pos)
                return_vec[pos]=1;
        }
    }
    
    void Print()
    {
        cout<<"print the train matrix begin : "<<endl;
        vector< vector<int> > :: iterator it = train_mat.begin();
        while(it!=train_mat.end())
        {
            vector<int> vec = *it;
            vector<int> :: iterator itt = vec.begin();
            while( itt!=vec.end())
            {
                cout<<*itt<<" ";
                itt++;
            }
            cout<<endl;
            it++;
        }
    }
    
    void GetTrianMat()
    {
        train_mat.clear();
        for(int i=0;i<6;i++)
        {
            SetOfWordsToVec(i);
            vector<int> vec(return_vec,return_vec + my_vocab_list.size()+1 );
            train_mat.push_back(vec);
            delete []return_vec;
        }
    }
    
    void trian_NB0()
    {
        int train_mat_num = (int)train_mat.size();
        cout<<"train_mat_num = "<<train_mat_num<<endl; //样本的总数目
        int words_num = (int)train_mat[0].size() - 1 ;
        cout<<"words_num = "<<words_num<<endl;         //单词数量，样本的所有特征值
        int sum=accumulate(list_classes.begin(),list_classes.end(),0);
        cout<<"sum="<<sum<<endl;                       //absusive样本的数量
        p_abusive = (float)sum/(float)train_mat_num;    //absusive样本所占的比例
        cout<<"p_abusive = "<<p_abusive<<endl;
        p0vect.resize(train_mat[0].size(),1); //各个单词出现的数量 non-absusive
        p1vect.resize(train_mat[0].size(),1); //各个单词出现的数量in abusive
        float p0Denom = 2.0; //分在no-abusive的总单词数目
        float p1Denom = 2.0; //the total number of words in abusive docs
       
        for(int i=0;i<list_classes.size();i++)
        {
            if(list_classes[i]==1)
            {
                for(int j=0;j<p1vect.size();j++)
                {
                    p1vect[j] += train_mat[i][j];
                    if(train_mat[i][j]==1)
                        p1Denom++;
                }
            }
            else
            {
                for(int j=0;j<p0vect.size();j++)
                {
                    p0vect[j] += train_mat[i][j];
                    if(train_mat[i][j]==1)
                        p0Denom++;
                }

            }
        }
        
        cout<<"p1Denom="<<p1Denom<<"   p0Denom=="<<p0Denom<<endl;
        
        for(int i=0;i<p1vect.size();i++)
        {
            p0vect[i] =log( p0vect[i]/p0Denom);
            p1vect[i] =log( p1vect[i]/p1Denom);
        }
    
    }
    
    int classify_NB(string* text)
    {
        return_vec = new int[ my_vocab_list.size()+1 ]();
       
        for(int i=0;text[i]!="null";i++)
        {
            int pos=my_vocab_list[ text[i] ];
            if( pos!=0 )
            {
                return_vec[ pos ] = 1;
            }
        }
        
        for(int i=0;i<my_vocab_list.size()+1;i++)
            cout<<return_vec[i]<<" ";
        cout<<endl;
        
        float p1=inner_product(p1vect.begin()+1, p1vect.end(), return_vec+1, 0)+log(p_abusive);
        float p0=inner_product( p0vect.begin()+1,p0vect.end(),return_vec+1,0 ) + log(1-p_abusive);
        // 因为p1和p2的分母相同，所以略去分母
        cout<<"p1 = "<<p1<<"   p0 = "<<p0<<endl;
        
        if( p1>p0 )
        {
            return 1;
        }
        else
        {
            return 0;
        }      }
};

#endif
