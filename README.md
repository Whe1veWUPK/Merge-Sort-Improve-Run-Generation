# 东南大学数据结构与算法实践 Project 3
## 项目结构
* header 中存放着所有的.hpp文件
* source 中存放着所有的.cpp文件
## 附加说明
* 所有的.hpp都是对应同名的.cpp的头文件 声明在.hpp中，定义在.cpp中
* Buffer.cpp 定义了 Buffer数据结构，包含判空、判满等函数
* FileOperator.cpp 封装了Buffer 与文件之间交互 以及 其它文件操作有关的函数
* Info.cpp 自定义败者树中存放的数据结构，包含value(值),key(下标),isValid(当前数据在树中是否有效（尽量延长Run 的Size 所需）)
* LoserTree.cpp 败者树数据结构 含基础的败者树相关操作，最终胜者（最小的值存放在 loserArray[0]（Info）中）
* Timer.cpp 封装的Chrono计时器，记录程序运行时间
* FinalTest.cpp 程序入口，输入BufferSize 和 LoserTree's Size开始测试
## 使用说明
* 首先运行 $DataProducer.py$ 随机产生一定量的数据
* 然后运行 $FinalTest.cpp$  ，输入BufferSize 和 LoserTree's Size就可以开始测试了
## 补充
* 本次实验的核心点在于 败者树数据结构的构建、多线程同步编程以及最终最优Merge Sequence的选取，所以最终找到Best Merge Sequence 后 并未进行Merge(懒)
* 输入数据（待排序）在Input.txt中，最终生成的初始Run数据（未Merge）在Output.txt中
