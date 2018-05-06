#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>  
#include<iostream>
using namespace std;
const int Bit_Num = sizeof(int)* 8;

//求解非零最高位  
int GF_index_of_max(int value){
    int index = 0;
    for (int i = 0; i<Bit_Num; ++i)
    if (value & (1 << i))
        index = i;
    return index;
}

//有限域GF(2^8)内的除法运算  
int GF_divide(int m, int b, int &r){//m为模数，b为要求逆元的数
    int m_MSB = GF_index_of_max(m);
    int b_MSB = GF_index_of_max(b);
    if (m_MSB<b_MSB){//除数大于被除数不能再除
        r = m;
        return 0;
    }
    int d = m_MSB - b_MSB;
    int temp = b;
    temp = temp << d;
    m = m^temp;
    return (1 << d) | GF_divide(m, b, r);//返回商，r为余数
}

//迭代求解x，y 
//扩展欧几里德迭代，d2=d0-q*d1，q=[r0/r1] 
int GF_iterate(int d0, int q, int d1){
    int value = 0;
    for (int i = 0; i<Bit_Num; ++i){
        if (q & (1 << i))
            value = value ^ ((d1 << i));//GF(2^8)的乘法，q*d1
    }
    return d0 ^ (value);//d0-q*d1
}

//扩展欧几里德求逆元
int GF_exgcd(int m, int a, int &x, int &y){
    int x0, x1, y0, y1, q, r = 0;
    x0 = 0; x1 = 1;
    y0 = 1; y1 = 0;

    while (1){
        if (a == 0)
            return m;
        if (a == 1)
            return a;
        q = GF_divide(m, a, r);//q为商，r为余数
        x = GF_iterate(x0, q, x1); x0 = x1; x1 = x;//x=x0-q*x1
        y = GF_iterate(y0, q, y1); y0 = y1; y1 = y;
        m = a; a = r;
    }
}

//仿射变换
//bi'=bi^b(i+4)mod8^b(i+5)mod8^b(i+6)mod8^b(i+7)mod8^ci
//可转换为bit矩阵相乘的方法
void exchange(int &b){
    int exarray[8] = { 0xF1, 0xE3, 0xC7, 0x8F, 0x1F, 0x3E, 0x7C, 0xF8 };//变换矩阵
    int c = 0x63;//ci
    int p = 0;
    for (int i = 0; i < 8; i++){
        int ex1 = exarray[i] & b;//行列二进制位相乘
        int bi = 0;
        for (int j = 0; j < Bit_Num; j++){
            bi = bi ^ (ex1 & 1);//相乘结果的每个二进制位进行异或运算
            ex1 = ex1 >> 1;
        }
        bi = bi << i;//得出一个bi’
        p = p | bi;//将各个bi'相加
    }
    b = p^c;
}

int main(){
    int m = 283, a, x, y;    //m(x) = x^8 + x^4 + x^3 + x + 1;
    //初始化s-box以及求逆元
    int sbox[256] = { 0, 1 };
    for (int i = 2; i < 256; i++){
        GF_exgcd(m, i, sbox[i], y);
    }
    //仿射变换
    for (int i = 0; i < 256; i++){
        exchange(sbox[i]);
    }
    //输出s-box
    for (int i = 0; i < 256; i++){
        if (i % 16 == 0)printf("\n");
        printf("%02X", sbox[i]);
        printf(" ");
    }
    printf("\n");
    system("pause");
    return 0;
}