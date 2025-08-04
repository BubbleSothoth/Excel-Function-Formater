# Excel函数格式化工具
这是一个Excel函数格式化工具。具体来说，当我们在使用Excel的时候往往需要编写一系列的函数以自动处理表格中的数据，然而，当表格中的数据逻辑关系过于复杂的时候就会导致函数过度冗长，严重降低代码可读性。此程序的功能是对Excel函数进行格式化，在适当的位置插入换行符与水平制表符（这些控制符号不会影响函数在Excel中的运行）从而使代码更加清晰、易懂。

# 使用方法
1.克隆代码库
```
git clone https://github.com/BubbleSothoth/Excel-Function-Formater.git
```
2.构建、运行

3.传参、输入、输出

```
ExcelFunctionFormater.exe in-file out-file
in-file  输入文本文件，其中记录了Excel函数
out-file 输出文本文件，用于记录格式化后的 Excel函数
```

例如：
in.txt
```
=IFERROR(IF("$A2"<>"Access",SUMIFS($B:$B,$C2:$C2,A2,$D2:$D2,$F2),VLOOKUP(A2,B:E,3,FALSE)),"——")
```

运行命令行
```
ExcelFunctionFormater.exe in.txt out.txt
```

out.txt
```
=IFERROR(
	IF(
		"$A2"<>"Access",
		SUMIFS(
			$B:$B,
			$C2:$C2,
			A2,
			$D2:$D2,
			$F2
		),
		VLOOKUP(
			A2,
			B:E,
			3,
			FALSE
		)
	),
	"——"
)
```
