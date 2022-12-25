/**
 * c++ string 类型学习
 */

#include<string>
#include<iostream>
#include<exception>

/**
 * 替换文件名后缀为tmp 如果为tmp则替换为xxx
 */
int main(int argc, char** argv){
    const std::string suffix("tmp");

    for(int i = 1; i < argc; i++) {
        std::string cur(argv[i]);
        std::string::size_type pos = cur.find('.');

        if(pos == std::string::npos) {
            // cur += '.' + "tmp"; 特别注意:这里是 char + char* 会转化为两个整型
            cur = cur + '.' + "tmp";
        } else {
            std::string ext_name(cur.substr(pos+1));

            if(ext_name == suffix) {
                cur.replace(pos + 1, std::string::npos, "xxx");
            } else {
                cur.replace(pos + 1, std::string::npos, suffix);
            }
        }
        std::cout << cur << std::endl;
    }
    std::string str;
    str.resize()
    return 0;
}