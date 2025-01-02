#include "functions.h"
#include "file_util.h"



void display_help(){
    print("用于比较两个文件并显示不同(逐行比较）\nFC  +data1 +data2 [/c] [/n] [/a] [/b]\n\n");
    print("匹配显示:\n*****file1\nprevious line\nline with diff\nnext line\n*****file2\nprevious line\nline with diff\nnext line\n*****\n\n");
    print("匹配模式:\n/c 不区别大小写进行比较。\n/b 执行字节比较,显示如\noffset : byte1 byte2 \n\n显示模式:\n/n 显示行数\n/a 只显示每个不同处的第一行和最后一行。\n\n");
}
//开关/b是独立的




int main(int argc, char *argv[]) {


    // for (int i = 1; i< argc; i++){
    //     print(argv[i]);
    //     print(" ");
    // }
    // print("\n");

    if(argc < 2 ){
        return 0;
    }
    if (argv[1][0] == '/' && argv[1][1] == '?')
    {
        display_help();
        return 0;
    }
    const char* file1 = NULL;
    const char *file2 = NULL;

    int c_mode = 0;   //大小写 
    int b_mode = 0;   //字节比较

    int a_option = 0;
    int n_option = 0;

    for (int i = 1; i < argc; i++) {
        char *tmp = argv[i];
        if (argv[i][0] == '/') {
            if (argv[i][1] == 'C' || argv[i][1] == 'c')
                c_mode = 1;
            if (argv[i][1] == 'B' || argv[i][1] == 'b')
                b_mode = 1;
            if (argv[i][1] == 'A' || argv[i][1] == 'a')
                a_option = 1;
            if (argv[i][1] == 'N' || argv[i][1] == 'n')
                n_option = 1;
        } else if (argv[i][0] == '+' && argv[i][1] != '\0') {
            if (file1 == NULL) {
            file1 = argv[i] + 1;
            } else if (file2 == NULL) {
            file2 = argv[i] + 1;
            }
        }
    }
    //////////////////////////////////////////////////////////////////////

    //paras check
    ////////////////////////////////////////////////////////////////////////
    if (file1 == NULL || file2 == NULL) {
        print("格式错误,请输入两个文件\n");
        return 1;
    }
    file1 = get_abs_path(file1);
    file2 = get_abs_path(file2);
    //////////////////////////////////////////////////////////////////////////////////

    // compare
    ////////////////////////////////////////////////////////////////////////////////////
    struct LINE_INFO *result;
    if(!b_mode){
        result = fc_compare(file1, file2, c_mode);
        

    }






    return 0;
}