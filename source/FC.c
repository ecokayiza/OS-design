#include "functions.h"
#include "file_util.h"
#include <string.h>



void display_help(){
    print("用于比较两个文件并显示不同(逐行比较）\nFC  +data1 +data2 [/c] [/n] [/a] [/b]\n\n");
    print("匹配显示:\n*****file1\nprevious line\nline with diff\nnext line\n*****file2\nprevious line\nline with diff\nnext line\n*****\n\n");
    print("匹配模式:\n/c 不区别大小写进行比较。\n/b 执行字节比较,显示如\noffset : byte1 byte2 \n\n显示模式:\n/n 显示行数\n/a 只显示每个不同处的第一行和最后一行。\n\n");
}
//开关/b是独立的

struct FC_COMP_INFO
{
    int line;
    int is_diff;
    char* file1;
    char* file2;
};
struct COMP_INFO
{
    int offset;
    int line;
    char file1;
    char file2;
};

struct FC_COMP_INFO* get_display_line(struct FC_COMP_INFO* result,int a_option,int n_option,const char* file1_name,const char* file2_name){
    //显示的行包括所以不同的行及其上下行

    int LINE_NUM = MAX_FILE_LINE + 1;
    int *display_line = (int *)malloc(LINE_NUM * sizeof(int));
    memset(display_line, 0, LINE_NUM* sizeof(int));

    if(result[0].is_diff){  //处理第一行
        display_line[0] = 1;
        display_line[1] = 1;
    }

    int i = 1;                          // 假定 n 为 result 的最后一个下标（行号n+1)
    for (i; result[i + 1].line; i++)  //填充diff行前后，从下标1开始到n-1 
    {
        if(result[i].is_diff){
            display_line[i-1] = 1;
            display_line[i] = 1;
            display_line[i+1] = 1;
        }
    }
    if(result[i].is_diff){   //处理最后一行 i=n
        display_line[i-1] = 1;
        display_line[i] = 1;
    }
    i++;
    display_line[i] = 0;   //n+1的位置封0用于判断边界

    // for (int l = 0; l < i + 1;l++){
    //     print(int_to_char(display_line[l]));
    // }

    if (!a_option){
        int j = 0;
        for (j; j < i; j++)
        {
            if (display_line[j])
            {
                print("*****"); // 一个section的开始
                print(file1_name);
                print("\n");
                if (n_option)
                {
                    print("[");
                    print(int_to_char(result[j].line));
                    print("]");
                }
                print(result[j].file1);
                int k = j;
                while (display_line[k + 1]) // 走完一个section
                {
                    if (n_option)
                    {
                        print("[");
                        print(int_to_char(result[k+1].line));
                        print("]");
                    }
                    print(result[k + 1].file1);
                    k++;
                }
                ////////////////////////////////////////////////////////
                print("*****"); // 到file2
                print(file2_name);
                print("\n");

                if (n_option)
                {
                    print("[");
                    print(int_to_char(result[j].line));
                    print("]");
                }
                print(result[j].file2);
                while (display_line[j + 1])
                { // 这里要实际改动j
                    if (n_option)
                    {
                        print("[");
                        print(int_to_char(result[j+1].line));
                        print("]");
                    }
                    print(result[j + 1].file2);
                    j++; // 最后j的位置display为0
                }
                print("\n");
            }
        }
    }else{
        int j = 0;
        for (j; j < i; j++)
        {
            if (display_line[j])
            {
                print("*****"); // 一个section的开始 j是一个secion的头部
                print(file1_name);
                print("\n");
                if (n_option)
                {
                    print("[");
                    print(int_to_char(result[j].line));
                    print("]");
                }
                print(result[j].file1);

                //中间
                int k = j;
                int len = 1;
                while (display_line[k + 1]){
                    len++;
                    k++;
                }
                if(len==3){
                    if (n_option)
                    {
                        print("[");
                        print(int_to_char(result[j+1].line));
                        print("]");
                    }
                    print(result[j + 1].file1);
                }else{
                    print("...");
                    print("\n");
                }

                // k处于section尾部
                if (n_option){
                        print("[");
                        print(int_to_char(result[k].line));
                        print("]");
                }
                print(result[k].file1);

                ////////////////////////////////////////////////////////
                print("*****"); // 到file2
                print(file2_name);
                print("\n");
                if (n_option)
                {
                    print("[");
                    print(int_to_char(result[j].line));
                    print("]");
                }
                print(result[j].file2);
                
                //中间
                if(len==3){
                    if (n_option)
                    {
                        print("[");
                        print(int_to_char(result[j+1].line));
                        print("]");
                    }
                    print(result[j + 1].file2);
                }else{
                    print("...");
                    print("\n");
                }

                //尾部
                if (n_option){
                        print("[");
                        print(int_to_char(result[k].line));
                        print("]");
                }
                print(result[k].file2);

                j=k+1;  //更新j
                print("\n");
            }
        }
    }
}





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
    print("正在比较文件");
    print(file1);
    print("和");
    print(file2);
    print("\n");
    file1 = get_abs_path(file1);
    file2 = get_abs_path(file2);
    //////////////////////////////////////////////////////////////////////////////////

    // compare
    ////////////////////////////////////////////////////////////////////////////////////
    
    if(!b_mode){
        struct FC_COMP_INFO *result;
        result = fc_compare(file1, file2, c_mode);

        for (int i = 0; result[i].line; i++){
            if(result[i].is_diff){
                get_display_line(result, a_option, n_option, file1, file2);
                return;
            }
        }
        print("两个文件相同\n");
            
    }else{
        struct COMP_INFO *result;
        result = compare(file1, file2, 0, c_mode);  //用了利用COMP

        for(int i = 0; result[i].line; i++){    
            
        }





    }
    return 0;
}