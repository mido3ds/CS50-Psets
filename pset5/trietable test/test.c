#include "trietable.h"

int main(void)
{
    node* new = Generate_Node();
    Add_Word("mahmoudadasmahmoud", new);
    bool check = Search_Node("mahmoudadasmahmoud", new);
    printf("%d \n", check);
    Delete_Node(new);
    check = Search_Node("mahmoudadasmahmoud", new);
    printf("%d \n", check);
    return 0;
}