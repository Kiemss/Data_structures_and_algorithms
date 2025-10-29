#include <functional>
#include <iostream>
#include <typeinfo>

void demonstrate_type_erasure() 
{
    int times{};
    std::function func = [times](int a = {},int b = {})
    {
        std::cout << a*b << std::endl;
    };
    for(int i = 0;i< 10;++i){func(i,i);}
}

#if  0
int main()
{
    demonstrate_type_erasure();
}
#endif