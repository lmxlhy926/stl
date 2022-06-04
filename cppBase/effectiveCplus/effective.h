//
// Created by 78472 on 2022/5/29.
//

#ifndef STL_EFFECTIVE_H
#define STL_EFFECTIVE_H


/*
 * class专属常量：
 *      为了将常量的作用域限制于class内，你必须让它成为class的一个成员；为了确保此常量至多只有一份实体，必须使其成为一个static成员。
 *      这里是NumTurns的声明式而非定义式。但是如果它是个class专属常量又是static且为整数类型（integral type: ints, chars, bools）
 *      则需特殊处理。只要不取它们的地址，你可以声明并使用它们而无需提供定义式。
 */
class GamePlayer{
    static const int NumTurns = 5;  //常量声明式；只要不取它们的地址，你可以声明并使用它们而无需提供定义式
    int scores[NumTurns];           //编译时必须编译器必须知道数组的大小
};


/*
 * 如果需要用到整数型常量的地址，则必须定义该变量
 * 由于class常量已在声明时获得初值，因此定义时不可以再设初值
 * 否则会报：duplicate initialization of 'GamePlayer1::NumTurns'
 */
class GamePlayer1{
public:
    static const int NumTurns = 5;
    int scores[NumTurns];

    void show(){
        std::cout << "&NumTurns: " << &NumTurns << std::endl;
    }
};

/*
 * 如果在class编译期间需要一个class常量值, 例如scores的数组声明式中(编译器坚持必须在编译期间知道数组的大小)
 * 如果编译器不允许"static整数型class常量完成"in class初值设定", 可改用所谓的"the enum hack补偿做法""
 */
class GamePlayer2{
    enum {
        NumTurns = 5
    };
    int scores[NumTurns];
};









#endif //STL_EFFECTIVE_H
