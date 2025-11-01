#include <functional>
#include <iostream>
#include <typeinfo>

void demonstrate_type_erasure() 
{
    int v = 3;
    int dist[3][3] {};
    int path[3][3] {-1};

    for(int k = 0;k < v;++k)
    {
        for (int i = 0;i < v;++i)
        {
            for (int j = 0; j < v;++j)
            {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    //i到j的路径的最后一段复用k到j的路径的最后一段，因为此时k作为被插入的节点
                    path[i][j] = path[k][j];
                }
            }
        }
    }
}

#if  0
int main()
{
    demonstrate_type_erasure();
}
#endif