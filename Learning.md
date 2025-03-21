# 阶段-1

下载两个代理工具

`apt-get install ncat`

`apt instal connect-proxy`

前期远程连接github均未成功 -->原因:Linux权限过高# 阶段-1

# 阶段0
## Make 工具的核心功能
是自动化构建和管理项目中的编译、链接、测试、安装等任务。它通过读取` Makefile` 文件中的规则，确定文件之间的依赖关系，并仅对需要更新的部分进行重新构建，从而提高效率

1.**依赖管理**

- **功能**: Make 工具的核心是管理文件之间的依赖关系。

- **原理**:

通过 `Makefile` 文件定义目标（target）、依赖（prerequisites）和命令（recipe）。

2.**增量构建**

- **功能:** 仅构建需要更新的部分,避免重复编译

- **原理:** 

通过比较目标文件和依赖文件的时间戳来判断是否需要重构建

3.**自动化构建**

通过 `makefile` 规则,自动化编译,链接等任务

4.**其他特性**

*并行构建* *跨平台* *多目标*

## C语言编译流程

1. **预处理:** 处理预处理指令, 生成 `.i`
2. **编译:** 转化为汇编代码,  生成`.s`
3. **汇编:** 汇编代码转换为机器码, 生成 `.o` 
4. **链接:** 目标文件和库文件合并, 生成可执行文件。

## C语言多文件开发

1. 程序按功能拆分为 `.c` 文件, 实现具体功能
2. 公共声明和定义放在 `.h` 文件, 声明, 宏, 结构体等
3. 一个 **主文件** `main.c`
# 阶段1
vim 缩进修改
1. `cd ~/.vimrc`
2. 复制下列内容
```txt
set tabstop=4 #设置 Tab 建宽度为4格
set shiftwidth=4 #设置每级缩进为4格
set expandtab #原有Tab转化为相应数量空格

set smartindent #智能缩进
set cindent #专为C设计 

```
## 任务2
**?** 什么时候 `char*` 指针有时需要指定大小

-->限定字符串操作范围, 避免缓冲区溢出

此例作为参数传给 `fgets()` 提供操作安全保障



**perror()函数**

能提供比`printf()` 更多报错信息

## 任务3

**去行末换行符:**

`line[strcspn(line,"\n")]='\0';`

去掉后不干扰其他字符串函数判断逻辑

**?** 把文件打开与读取也集成在文件处理函数中应该不合适--> <u>打开文件不灵活</u>

目前单文件处理没问题, 若有多文件时应该考虑修改


# 阶段2:解析器开发

## 任务1:规则解析, 存储与检查

**`Makefile` 文件优先级:**

1. `Makefile` 不带后缀
2. `makefile` 不带后缀
3. 自定义 `.mk` 文件 注: 应带上 `-f  ` 或` --file` 参数

**strtok函数特性:** 

- 可一次使用多个分隔符
- 直接忽略开头和结尾的分隔符
- 改变原字符串
- 跳过连续的分隔符
- 记住上一次字符串分割位置,若传入 `NULL` 则从上一次字符串分隔点开始 
- `strtok_r` 时多线程安全版

<u>工作流程:</u>

1. `strtok` 会扫描字符串，找到第一个不在 `delimiters` 中的字符作为单词的起始位置。
2. 然后继续扫描，直到找到第一个在 `delimiters` 中的字符，将其替换为 `\0`（字符串结束符），从而截取出一个单词。
3. 下一次调用时，`strtok` 会从上次的分割点继续扫描，跳过所有连续的分隔符，直到找到下一个单词。

**realloc函数特性:**

- 灵活调整内存大小
- 自动化: 自动分配新内存块, 复制旧数据, 释放原内存(注: 原内存不再有效, 不再使用)
- 兼容性: 仅适用 `malloc` , `calloc` , `realloc` 分配的内存
- 错误处理: 若分配失败返回 `NULL` ,则原内存不被释放, 这时应避免覆盖原指针

**方法思路:**

理解:假定静态语法没有错误

1. **解析Makefile文件:**
1. 去掉语句多余部分
2. 判断目标行目标块
3. 拆分目标, 依赖, 命令
2. **结构体存储文件内容**
3. **按规则检查并储存报错**
4. **打印报错**

**报错信息的处理:**

1. 会检测到多种错误

--> 错误类型打包在 `ErrorType` 枚举类型统一管理

2. 需要具体记录报错相关信息

--> 报错相关信息打包 `Error` , 封装报错所有关键信息

**存在问题:**

- targets组内存在主函数分配, 而errors组内存在功能函数 ` add_error` 中分配
- 函数参数过多

## 任务2:进行第一次编译

**`cleanup` 标签特点:**

- 统一集中释放资源
- 无论程序成功失败都会执行

**`system()` 特点:**

- 执行机制: 通过创建子进程调用操作系统 `shell` (如/bin/sh)
- 成功执行返回 0
- 可能有安全风险

**fprintf()报错:**

- 在 `stderr` 流输出 ,(e.g.fprintf(stderr,""))

- 实现输出流分离: stdout(正常输出), stderr(错误信息输出)

|              | fprintf      | perror                   |
| ------------ | ------------ | ------------------------ |
| 错误描述处理 | 可自定义格式 | 自动读取`errno` 对应错误 |
| 输出流       | 可自定义     | stderr                   |

**exit(EXIT_FAILURE)使用场景:**

一般用于遇到不可恢复的错误中

- 命令行参数输入错误
- 文件操作失败
- 动态内存分配失败
- 多进程中的严重错误



**思路:**

1. 识别命令行输入
2. 解析 `Makefile` 文件
3. 运行命令

**strdup函数:**

- 创建字符串副本(duplicate), 返回指针
- 用 `malloc` 创建的, 需要释放

*以上程序中  `*dependencies` 所指内存不确定, 所以要用 `strdup()` 动态分配*

**缩进问题:**

.`vimrc` 文件中设置 `set expandtab` 会把Tab键改为空格键, 导致 `Makefile` Tab键失效

# 阶段3:依赖关系排序

## 任务1:依赖图构建

**安装igraph**

```bash
sudo apt update
sudo apt install build-essential cmake libgmp-dev libxml2-dev
#从windows中传输源码
mv /mnt/c/Users/[用户名]/文件位置 ~
tar -xvf igraph-0.10.15.tar.gz
cd igraph-0.10.15
# 创建构建目录
mkdir build && cd build

# 配置编译选项（默认安装到 /usr/local）
cmake ..

# 编译
make

# 安装到系统（需 root 权限）
sudo make install

# 更新动态链接库缓存
sudo ldconfig

#验证安装
pkg-config --modversion igraph
```

安装后头文件位置在 `/usr/local/include/igraph/igraph.h`

使用时 `#include <igraph/igraph.h>  // 匹配实际路径中的子目录`

**`igraph.so` 链接库未安装处理:**

```bash
sudo apt install libigraph-dev
```

**面对对象:**

创建对象:

​	初始化

​	方法

**面对过程:**

构建数据结构

操作数据结构:

1. ​	初始化函数
2. ​	方法函数 
3. ​	主函数

- 数据与操作分离
- 先定义总体目标-->主函数, 再分解问题为子任务-->子函数

**方法思路:**

1. 解析 `Makefile` 文件
2. 构建图结构
1. 目标
2. 图
3. 改进: 增加顶点哈希表结构便于获取
3. 初始化, 输入参数, 打印



**图结构:**

由顶点(vertex)和边(edge)组成

分类:

- 有向图
- 无向图
- 带权图
- 联通图
- 稀疏/稠密图

储存方式:

- 邻接矩阵: 二维数组表示顶点连接 (位置(i,j)的值表示 `i点到j点的边数` ), 适合稠密图 
- 邻接表:每个顶点维护一个链表, 存储相邻顶点 , 适合稀疏图

遍历方法:

- DFS: 深度优先搜索 
- BFS: 广度优先搜索 



**igraph:**

- igraph_empty(): 创建空有向图
				  - igraph_t: 图数据结构
							  - igraph_vector_int_t: 边数据结构
									   - igraph_vector_int_init(): 初始化边列表容器
																   - igraph_vector_int_push_back(): 连接边, 函数成对
																									- igraph_destroy(): 销毁图
																														- igraph_add_edges(): 添加边到列表
																																			  - igraph_add_vertex():添加顶点
																																									- igraph_degree():  计算图中所有顶点的度



																																														**igraph_degree():**

																																															计算图中所有顶点的度

																																															```c
																																															igraph_error_t igraph_degree(
																																																	const igraph_t *graph,       // 图的指针
																																																	igraph_vector_int_t *res,    // 存储结果的整型动态数组
																																																	igraph_vs_t vids,            // 要计算的顶点集合（如全部顶点:igraph_vss_all）
																																																	igraph_neimode_t mode,       // 度的计算模式（入度:IGRAPH_IN、出度:IGRAPH_OUT、全部:IGRAPG_ALL）
																																																	igraph_bool_t loops          // 是否包含自环边（True:IGRAPH_LOOPS_YES/False）
																																																	);
																																															```



																																															**igraph_vector_int_init():**

																																																初始化整型动态数组 (`igraph_vector_int_t`)

	- 需要销毁 `igraph_vector_int_destroy(&edges)`
	- 需要初始化容量(提高性能), 但可以自动扩容



	**igraph_vector_int_push_back():**

		向边列表(整形动态数组 `igraph_vector_int_t ` )添加整数

		- 要成对: 表示起点到终点



		**igraph_add_edges():**

			像图中批量添加多条边

			- 添加后应释放 `edges` 内存



			**igraph_es_t &igraph_eit_t:**

			数据结构:边选择器 (edge selector) `es` 和边迭代器(edge generator) `it`

			操作:

			1. 选择要操作的边 , 如 `igraph_es_all(&es, IGRAPH_EDGEORDER_ID) #选择所有边(按id顺序)  `
			2. 初始化迭代器 `igraph_eit_create(&graph,es,&it)`
			3. 迭代
			4. 最后销毁 `igraph_es_destroy(&es)` `graph_eit_destroy(&it)`

			| 迭代时函数                |                                       |
			| ------------------------- | ------------------------------------- |
			| `igraph_edge()`           | 获取图中某条边的两个端点              |
			| `IGRAPH_EIT_NEXT(it)`     | 移动到下一条边 (无返回值,循环调用)    |
			| `IGRAPH_EIT_END(it)`      | 检查是否遍历结束（返回 `1` 表示结束） |
			| `IGRAPH_EIT_GET(it)`      | 获取当前边的索引（`eid`）             |
			| `igraph_eit_destroy(&it)` | 销毁迭代器，释放内存。                |


