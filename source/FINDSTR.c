#include "functions.h"
#include <stdlib.h>

struct LINE_INFO{
    char *line;
    int line_num;
    int if_in;
};

void display_help() {
    print("1-在FIND的基础上添加了多匹配模式与多文件支持\n    如 FINDSTR -pattern1 -pattern2 +path1 +path2\n2-添加了表达式的支持和当前路径的支持\n    如 FINDSTR -regexp +filename (为了防止终端转义请在给表达式加上\" \") /R\n3.添加文件搜索\n    如 FINDSTR -filename.fix /d:dir path\n");
    print("\n参数说明:\n\n文件内容搜索(支持多pattern、多文件)\n/i 不区分大小写搜索\n/v 反转匹配\n/c 仅显示匹配的行数\n/n 显示匹配行的行号\n/r 使用正则表达式匹配\n\n目录搜索  FINDSTR -pattern /d:dir [/i] [/f]\n/i 不区分大小写搜索\n/f 显示完整路径\n(目录搜索只支持一个pattern)\n\n");
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
    int v_option = 0;
    int c_option = 0;
    int n_option = 0;
    int i_option = 0;
    int r_option = 0;
    int full_path_option = 0;

    const char *patterns[argc - 1];
    int pattern_count = 0;
    char *file_paths[argc - 1];
    int file_count = 0;
    char *dir_path = NULL;

    // print(int_to_char(argc));

    for (int i = 1; i < argc ;i++){

        if (argv[i] == NULL) {
            continue;
        }
        char *tmp = argv[i];
        // print(tmp);
        // print("\n");
        if(argv[i][0] == '/'){
            if(argv[i][1] == 'V'||argv[i][1] == 'v')
                v_option = 1;
            if(argv[i][1] == 'C'||argv[i][1] == 'c')
                c_option = 1;
            if(argv[i][1] == 'N'||argv[i][1] == 'n')
                n_option = 1;
            if(argv[i][1] == 'I'||argv[i][1] == 'i')
                i_option = 1;
            if(argv[i][1] == 'R'||argv[i][1] == 'r')
                r_option = 1;
            if(argv[i][1] == 'D'||argv[i][1] == 'd'){
                if(argv[i][2] == ':'){
                    dir_path = argv[i]+3;
                }
            }
            if(argv[i][1] == 'F'||argv[i][1] == 'f')
                    full_path_option = 1;
            

        }else if(argv[i][0] == '-' && argv[i][1] != '\0'){
            patterns[pattern_count] = argv[i]+1;
            pattern_count++;
        }else if(argv[i][0] == '+' && argv[i][1] != '\0'){
            file_paths[file_count] = argv[i]+1;
            file_count++;
        }
    }
    // check
    //////////////////////////////////////////////////////////////////////////


    // for (int i = 0; i< argc; i++){
    //     print(argv[i]);
    //     print("\n");
    // }

    if (pattern_count == 0)
    {
        print("invalid input\n");
        return 0;
    }
    if((file_count == 0 && dir_path == NULL)||(file_count != 0 && dir_path != NULL)){
        print("invalid input\n");
        return 0;
    }

    if(file_count){
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            // print("Current working dir: ");
            // print(cwd);
            // print("\n");
            perror("getcwd() error");
            return 1;
        } 

        for (int i = 0; i < file_count; i++)
        {
            if(!contains(file_paths[i],"/",0)){
                char *full_path = malloc(1024);
                int l = 0;
                for (l; cwd[l];l++)
                    full_path[l] = cwd[l];
                full_path[l] = '/';
                int m = 0;
                for (m; file_paths[i][m]; m++)
                    full_path[l+m+1] = file_paths[i][m];
                full_path[l+m+1] = '\0';

                file_paths[i] = full_path;
            }
        }
    }
    if(dir_path){
        if(!contains(dir_path,"/",0))
            dir_path = get_abs_path(dir_path);
    }
    // for (int i = 0; i < file_count; i++){
    //     print(file_paths[i]);
    //     print("\n");
    // }
    // for (int i = 0; i < pattern_count; i++)
    // {
    //     print(patterns[i]);
    //     print("\n");
    // }
    if(r_option || dir_path){
        if(pattern_count != 1){
            print("请只使用一个模式\n");
            return 0;
        }
    }

    //search
    ///////////////////////////////////////////////////////////////////////////////////////
    if(file_count){
        struct LINE_INFO *result;
        struct LINE_INFO **results = (struct LINE_INFO **)malloc(file_count * MAX_FILE_LINE * sizeof(struct LINE_INFO *));

        for(int i = 0; i < file_count; i++){
            FILE *file = fopen(file_paths[i], "r");
            result =multi_search(file, patterns, pattern_count, i_option, r_option);
            fclose(file);
            results[i] = result;
        }
        
        // print(file_path);
        for(int i = 0; i < file_count; i++){
            result = results[i];
            print("---------------");
            print(file_paths[i]);

            if(v_option){
                    if(c_option){
                    int count = 0;
                    for(int i = 0; result[i].line != NULL; i++){
                        if(!result[i].if_in){
                            count++;
                        }
                    }
                    print(": ");
                    print(int_to_char(count));
                    print("\n");
                }else{
                    print("\n");
                    for(int i = 0; result[i].line != NULL; i++){
                        if(!result[i].if_in){
                            if(n_option){
                                print("[");
                                print(int_to_char(result[i].line_num));
                                print("]");
                                print(" ");
                            }
                            print(result[i].line);
                        }
                    }
                }
            }else{
                if(c_option){
                    int count = 0;
                    for(int i = 0; result[i].line != NULL; i++){
                        if(result[i].if_in){
                            count++;
                        }
                    }
                    print(": ");
                    print(int_to_char(count));
                    print("\n");
                }else{
                    print("\n");
                    for(int i = 0; result[i].line != NULL; i++){
                        if(result[i].if_in){
                            if(n_option){
                                print("[");
                                print(int_to_char(result[i].line_num));
                                print("]");
                                print(" ");
                            }
                            print(result[i].line);
                        }
                    }
                }
            }

        }
        print("\n");

    }else if(dir_path){
        print("---------------");
        print(dir_path);
        print("\n");
        struct LINE_INFO *result;
        result = search_dir(dir_path, patterns[0], i_option,full_path_option);
        for(int i = 0; result[i].line != NULL; i++){
            if(result[i].if_in){
                print(result[i].line);
                print("\n");
            }
        }
        print("\n");
    }
}

