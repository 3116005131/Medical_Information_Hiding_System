// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

#ifndef PCH_H
#define PCH_H
#define USER "username"
#define PWD "password"
// TODO: 添加要在此处预编译的标头
const int MAXDATANUM = 100;//最大数据数量
const int QINUM = 3;//准标识符数量
//敏感属性分组
const char TopSecret[2][20] = { {"HIV"},{"Cancer"} };
const char Secret[2][20] = { {"Phthisis"},{"Hepatitis"} };
const char LessSecret[2][20] = { {"Heart disease"},{"Asthma"} };
const char NonSecret[2][20] = { {"Flu"},{"Indigestion"} };
#endif //PCH_H
