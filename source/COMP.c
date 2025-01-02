#include "functions.h"
#include "file_util.h"
#define MAX_FILE_LINE 1024
#define MAX_LINE_LEN 1024



void display_help(){
    print("用于比较两个文件是否相同\nCOMP  +data1 +data2 [/n] [/c] [/l] [/d] [/a]\n\n");
    print("一个匹配结果如:\noffset / line \nfile1 = diff \nfile2 = diff\n(如果没指定/n则文件大小必须相同，如果找到10个不同项则停止匹配)\n\n");
    print("匹配模式\n /n=num 如果文件大小不匹配，启用n来指定匹配的行数, 并启用 /l\n /c 匹配的ASCII字符不区别大小\n\n");
    print("显示模式\n /l 显示行号 \n /d 用十进制显示dff\n /a 用ASCII显示diff\n\n ");
}





int main(int argc, char *argv[]) {
    if(argc < 2 ){
        return 0;
    }
    if (argv[1][0] == '/' && argv[1][1] == '?')
    {
        display_help();
        return 0;
    }
    // int 

    // const char *patterns[argc - 1];
    // int pattern_count = 0;
    // char *file_paths[argc - 1];
    // int file_count = 0;
    // char *dir_path = NULL;

    // // print(int_to_char(argc));

    // for (int i = 1; i < argc ;i++){

    //     if (argv[i] == NULL) {
    //         continue;
    //     }
    //     char *tmp = argv[i];
    //     // print(tmp);
    //     // print("\n");
    //     if(argv[i][0] == '/'){
    //         if(argv[i][1] == 'V'||argv[i][1] == 'v')
    //             v_option = 1;
    //         if(argv[i][1] == 'C'||argv[i][1] == 'c')
    //             c_option = 1;
    //         if(argv[i][1] == 'N'||argv[i][1] == 'n')
    //             n_option = 1;
    //         if(argv[i][1] == 'I'||argv[i][1] == 'i')
    //             i_option = 1;
    //         if(argv[i][1] == 'R'||argv[i][1] == 'r')
    //             r_option = 1;
    //         if(argv[i][1] == 'D'||argv[i][1] == 'd'){
    //             if(argv[i][2] == ':'){
    //                 dir_path = argv[i]+3;
    //             }
    //         }
    //         if(argv[i][1] == 'F'||argv[i][1] == 'f')
    //                 full_path_option = 1;
            

    //     }else if(argv[i][0] == '-' && argv[i][1] != '\0'){
    //         patterns[pattern_count] = argv[i]+1;
    //         pattern_count++;
    //     }else if(argv[i][0] == '+' && argv[i][1] != '\0'){
    //         file_paths[file_count] = argv[i]+1;
    //         file_count++;
    //     }
    // }
}