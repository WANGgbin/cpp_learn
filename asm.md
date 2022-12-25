描述 c/c++ 中 asm 汇编的语法规则.详细内容可以参考[c语言 asm 汇编内嵌语法](https://www.easck.com/cos/2020/0118/459112_3.shtml)

## 格式
内嵌 asm 汇编的语法为:
```c
__asm__ __volatile__("instruction list":output:input:modify);
```
- instruction list

指定汇编指令. 有多种格式,这里建议使用""包裹每一条指令. 除最后一条指令外,其他指令末尾添加'\n\t'. eg:
```c
__asm__ __volatile(
    "instruction1\n\t"
    "instruction2\n\t"
    "instruction3"
);
```
为什么指令末尾要添加 '\n\t' 呢? 如果不添加 \n\t.上述语句生成的汇编语句为:
"instuction1 instruction2 instruction3",而加上\n\t后,则结果为:
"
    instruction1
    instruction2
    instruction3
"
可以自己通过 gcc -S 查看生成的汇编内容.

汇编指令中通过"%数字"引用某个表达式. 通过"%%rax" 的方式来引用寄存器.
- output

指定汇编指令输出相关内容.语法为:
"约束"(左值表达式),""()
比如 "=a"(var) 表示,将寄存器 eax(长度由 var 长度决定) 内容输出到 var.
output 需要注意特殊的约束 "=",表示后面表达式是 write only 的. output 一定要带上这个约束.
- input

同 output,表示汇编指令的输入.
- modify

表示内存/哪些寄存器内容,在当前的汇编指令中发生变化,从而 gcc 在选择寄存器的时候,不会考虑这些寄存器.
## 约束
- 寄存器约束

表示,使用那个寄存器.比如"a"表示使用 eax 这个寄存器,寄存器具体的长度,具体情况具体判断.
- 立即数约束

"i",就是汇编指令中的立即数.()中的内容必须是个常量.
- 内存约束

"m" 表示直接通过内存存取数据.
- 0 1 2 3 ... 9

表示使用与某个表达式相同的约束(寄存器,内存等).