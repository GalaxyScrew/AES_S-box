#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>  
#include<iostream>
using namespace std;
const int Bit_Num = sizeof(int)* 8;

//���������λ  
int GF_index_of_max(int value){
    int index = 0;
    for (int i = 0; i<Bit_Num; ++i)
    if (value & (1 << i))
        index = i;
    return index;
}

//������GF(2^8)�ڵĳ�������  
int GF_divide(int m, int b, int &r){//mΪģ����bΪҪ����Ԫ����
    int m_MSB = GF_index_of_max(m);
    int b_MSB = GF_index_of_max(b);
    if (m_MSB<b_MSB){//�������ڱ����������ٳ�
        r = m;
        return 0;
    }
    int d = m_MSB - b_MSB;
    int temp = b;
    temp = temp << d;
    m = m^temp;
    return (1 << d) | GF_divide(m, b, r);//�����̣�rΪ����
}

//�������x��y 
//��չŷ����µ�����d2=d0-q*d1��q=[r0/r1] 
int GF_iterate(int d0, int q, int d1){
    int value = 0;
    for (int i = 0; i<Bit_Num; ++i){
        if (q & (1 << i))
            value = value ^ ((d1 << i));//GF(2^8)�ĳ˷���q*d1
    }
    return d0 ^ (value);//d0-q*d1
}

//��չŷ���������Ԫ
int GF_exgcd(int m, int a, int &x, int &y){
    int x0, x1, y0, y1, q, r = 0;
    x0 = 0; x1 = 1;
    y0 = 1; y1 = 0;

    while (1){
        if (a == 0)
            return m;
        if (a == 1)
            return a;
        q = GF_divide(m, a, r);//qΪ�̣�rΪ����
        x = GF_iterate(x0, q, x1); x0 = x1; x1 = x;//x=x0-q*x1
        y = GF_iterate(y0, q, y1); y0 = y1; y1 = y;
        m = a; a = r;
    }
}

//����任
//bi'=bi^b(i+4)mod8^b(i+5)mod8^b(i+6)mod8^b(i+7)mod8^ci
//��ת��Ϊbit������˵ķ���
void exchange(int &b){
    int exarray[8] = { 0xF1, 0xE3, 0xC7, 0x8F, 0x1F, 0x3E, 0x7C, 0xF8 };//�任����
    int c = 0x63;//ci
    int p = 0;
    for (int i = 0; i < 8; i++){
        int ex1 = exarray[i] & b;//���ж�����λ���
        int bi = 0;
        for (int j = 0; j < Bit_Num; j++){
            bi = bi ^ (ex1 & 1);//��˽����ÿ��������λ�����������
            ex1 = ex1 >> 1;
        }
        bi = bi << i;//�ó�һ��bi��
        p = p | bi;//������bi'���
    }
    b = p^c;
}

int main(){
    int m = 283, a, x, y;    //m(x) = x^8 + x^4 + x^3 + x + 1;
    //��ʼ��s-box�Լ�����Ԫ
    int sbox[256] = { 0, 1 };
    for (int i = 2; i < 256; i++){
        GF_exgcd(m, i, sbox[i], y);
    }
    //����任
    for (int i = 0; i < 256; i++){
        exchange(sbox[i]);
    }
    //���s-box
    for (int i = 0; i < 256; i++){
        if (i % 16 == 0)printf("\n");
        printf("%02X", sbox[i]);
        printf(" ");
    }
    printf("\n");
    system("pause");
    return 0;
}