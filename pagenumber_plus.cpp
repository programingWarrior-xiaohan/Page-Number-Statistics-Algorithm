//一个关于页码统计的算法
//时间复杂度/空间复杂度均为常数
#include<iostream>
#include<valarray>
#include<chrono>
int length (int number);
const std::valarray<int> calNumberCount(int number);
void print(const std::valarray<int> arr);
int main()
{
    using namespace std;
    while(1)
    {
        int page;                                          //页数
        std::valarray<int> page_number;                    //数字0~9的个数
        std::cin>>page;
        auto beforeTime = std::chrono::steady_clock::now();//获取当前时间
        page_number=calNumberCount(page);
        for(int i=0;i<length(page);++i)
            page_number[0]-=pow(10,i);
        print(page_number);
        auto afterTime = std::chrono::steady_clock::now();
        double duration_millsecond = std::chrono::duration<double, std::milli>(afterTime - beforeTime).count();
        cout<<"("<<duration_millsecond<<"ms)"<<endl;
    }
}

/**
 * @brief 
 * 设页码数n,对应最高位h(n),对应的长度l(n),n的去头剩余部分p(n)
 * 递归公式:calNumberCount(n) =[0~h(n)-1]{10^(l(n)-1)} + [EACH]{ h(n)*[(l(n)-1)*10^(l(n)-2)] } + [0]{(l(n)-l(p(n))-1)*(p(n)+1)} + [h(n)]{p(n)+1} +calNumberCount(p(n))
 * [注]:calNumberCount的结果是一个数组,[EACH]{x}表示数组所用内容全部加x.[0]{x}表示数组下标为0的值加x,也就是数字0的个数.'|'表示'或','x~y'表示x到y所有值
 * 该递归公式多包含0,多余0的个数为:[0]{10^(0+1+...+l(n)-1)}
 * 最后将多余的0减掉即可
 * 
 * 涉及公式：
 * 对于000.. —— 999.. 设长度为n, 每个数字出现的频率相同,所以得出公式:每个数字的个数都相等 = n*10^n/10 = n*10^(n-1)
 * 大体思路：
 * 对于1024:可看成0(000~999)<完整组>|10(00~24)<非完整组>对于前面部分结果可以直接算出，
 * 而后面10(00~24),1和0也可以直接算出,最后只需单独求出00~24即可,也就是求24页码对应的数字个数,递归由此得来
 * 详细过程：
 * 对于1024代入公式有:calNumberCount(1024)=[0]{1000}+[EACH]{300}+[0]{24}+[1]{24}+calNumber(24)
 * 1024可一次分解为0(000~999)<完整组>+1(000~024)
 * 1.[0~h(n)-1]{10^(l(n)-1)}:计算完整组外部增量.对于1024而言,1024只有一个完整组0(000~999),其中外部是0,个数为1000
 * 2.[EACH]{ h(n)*[(l(n)-1)*10^(l(n)-2)] }:计算完整组内部增量,h(n)为完整组个数,[(l(n)-1)*10^(l(n)-2)]是增量.
 * 如1024最高位是1所以只有一个完整组0(000~999),内部为(000~999),包含0~9各300个
 * 3.[0]{(l(n)-l(p(n))-1)*(p(n)+1)}和 [h(n)]{p(n)+1}:计算非完整组外部增量.对于非完整组有一点特殊：它的外部可能有后缀0，如果最高位后面有0的话
 * //对于1024其非完整外部增量为10(00~24),外部有两个数字组成10,因为1024最高位1后面有0.所以1024的外部增量为[1]{24}和[0]{24}
 * 4.calNumberCount(p(n)):计算非完整内部增量.对于1024,即10(00~24)内部增量为calNumberCount(24)
 * 5.最后去除多包含的0.注意多余零只出现在1~999..中,对于1024,多余0只出现在1~999,因为对于1024而言,我们递归算法计算的是000~999,但实际上我们的页码是1~999.
 * @param number 
 * @return const std::valarray<int> 
 */

//页码递归求解算法：给出一个页码n，计算出1~n中各个数字(0~9)出现的个数
const std::valarray<int> calNumberCount(int number)
{
    std::valarray<int> page_number(10);
    int len = length(number);
    if(len == 1)                                  //递归出口
    {
        for(int i=0;i<=number;i++)
            page_number[i]++;
        return page_number;
    }
    int high = number/(int)pow(10,len-1);         //取得最高位
    
    page_number+=high*(len-1)*pow(10,len-2);      //计算整体部分
    int i;
    for(i=0;i<high;i++)
        page_number[i]+=pow(10,len-1);
    int surplus = number%(int)pow(10,len-1);
    page_number[0]+=(len-length(surplus)-1)*(surplus+1);
    page_number[i]+= surplus + 1;
    return page_number+calNumberCount(surplus);
}
//计算数字长度
int length (int number)
{
    int i = 1;
    while(number/=10)
        i++;
    return i;
}
//打印0~9数字个数
void print(const std::valarray<int> arr)
{
    for(int i=0;i<arr.size();i++)
        std::cout<<i<<":"<<arr[i]<<std::endl;
}

