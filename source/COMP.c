#include "functions.h"
#include "file_util.h"
#define MAX_FILE_LINE 1024
#define MAX_LINE_LEN 1024



void display_help(){
    print("用于比较两个文件是否相同\nCOMP  +data1 +data2 [/n] [/c] [/l] [/d] [/a]\n\n");
    print("一个匹配结果如:\noffset / line \nfile1 = diff \nfile2 = diff\n(如果没指定/n则文件大小必须相同，如果找到10个不同项则停止匹配)\n\n");
    print("匹配模式\n /n=num 如果文件大小不匹配，启用n来指定匹配的行数, 并启用 /l\n /c 匹配的ASCII字符不区别大小\n\n");
    print("显示模式\n /l 显示行号 \n /d 用十进制显示dff(默认16进制)\n /a 用ASCII显示diff\n\n ");
}

struct COMP_INFO
{
    int offset;
    int line;
    char file1;
    char file2;
};


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

    // get paras
    ///////////////////////////////////////////////////////////////////////////////
    const char* file1 = NULL;
    const char *file2 = NULL;
    int n_mode = 0;
    int c_mode = 0;
    int l_option = 0;
    int d_option = 0;
    int a_option = 0;
    int n_lines = 0;

    for (int i = 1; i < argc; i++) {

        if (argv[i] == NULL) {
            continue;
        }
        char *tmp = argv[i];

        if (argv[i][0] == '/') {
            if (argv[i][1] == 'N' || argv[i][1] == 'n'){
                n_mode = 1;
                if(argv[i][2] == '='){
                    char * line_num = argv[i]+3;
                    if(is_digit(line_num)){
                        n_lines = char_to_int(line_num);
                    }else{
                        print("请确保\"/n\"的格式正确\n");
                        return 0;
                    }
                }else{
                    print("请确保\"/n\"的格式正确\n");
                    return 0;
                }
            }
            if (argv[i][1] == 'C' || argv[i][1] == 'c')
                c_mode = 1;
            if (argv[i][1] == 'L' || argv[i][1] == 'l')
                l_option = 1;
            if (argv[i][1] == 'D' || argv[i][1] == 'd')
                d_option = 1;
            if (argv[i][1] == 'A' || argv[i][1] == 'a')
                a_option = 1;
        } else if (argv[i][0] == '+' && argv[i][1] != '\0') {
            if (file1 == NULL) {
                file1 = argv[i] + 1;
            } else if (file2 == NULL) {
                file2 = argv[i] + 1;
            }
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////
    
    //paras check
    ////////////////////////////////////////////////////////////////////////////////////
    if (file1 == NULL || file2 == NULL) {
        print("格式错误,请输入两个文件\n");
        return 1;
    }
    file1 = get_abs_path(file1);
    file2 = get_abs_path(file2);
    if(!n_mode){
        if(!comp_file_size(file1, file2)){
            print("请确保文件大小匹配或者使用\"/n\"开关\n");
            return 1;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////

    
    // compare
    ////////////////////////////////////////////////////////////////////////////////////
    struct COMP_INFO *result;
    result = compare(file1, file2, n_lines, c_mode);


    print("---------------\n");
    int errs = 0;
    for (int i = 0; result[i].line; i++){
        if(n_mode || l_option){
            print("在line: ");
            print(int_to_char(result[i].line));
            print(" 比较错误");
            print("\n");
        }else{
            print("在offset: ");
            print(int_to_char(result[i].offset));
            print(" 比较错误");
            print("\n");
        }
        errs++;

        char *diff1 = malloc(10);
        char *diff2 = malloc(10);
        // 十进制显示
        if (d_option){ 
            int diff1_num = (int)result[i].file1;
            int diff2_num = (int)result[i].file2;

            sprintf(diff1, "%d", diff1_num);
            sprintf(diff2, "%d", diff2_num);
        }
        // ASCII显示
        else if (a_option){
            diff1[0] = result[i].file1;
            diff2[0] = result[i].file2;
            diff1[1] = '\0';
            diff2[1] = '\0';
        }
        // 默认16进制显示
        else{
            int diff1_num = (int)result[i].file1;
            int diff2_num = (int)result[i].file2;

            sprintf(diff1, "%02x", diff1_num);
            sprintf(diff2, "%02x", diff2_num);
  
        }
        print("file1 = ");
        print(diff1);
        print("\n");
        print("file2 = ");
        print(diff2);
        print("\n");
    }   

    if(errs == 0)
        print("文件相同\n");
    if(errs > 9){
        print("已找到10个不同项,停止匹配\n");
    }

    return 0;
}



