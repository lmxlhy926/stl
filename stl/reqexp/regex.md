# regex

##  match

**Match：将整个输入拿来比对某个正则表达式**

- 如果匹配成功

​		因为是完全匹配，前缀和后缀为空

​		"匹配合格之整体string"或"第n个匹配合格之substring"的内容、长度、位置

​		smatch的大小为 1 + 捕获分组的数量	

```c++
Class std::match_results<>是个template，必须依据其所处理的字符的iterator类型而实例化。
typedef match_results<string::const_iterator>  smatch;   针对string类型
```

**match_results对象内含：**
		**`prefix()`**：sub_match对象
		**`suffix()`**：sub_match对象
		**`sub_match对象m[0]`**：表现"匹配合格之所有字符"
		**`sub_match对象m[n]`**：对于任何**capture group**，你可以访问一个对应的sub_match对象m[n]
		**`size()`**：可取得sub_match对象的个数，不包括prefix(),suffix()

**match_results对象作为一个整体来看，提供了如下方法**
		**`size()`**：sub_match对象的个数
		**`str()`**：“匹配合格之整体string”或“第n个匹配合格之substring”
		**`length()`**：“匹配合格之整体string的长度”或“第n个匹配合格之substring的长度”
		**`position()`**：“匹配合格之整体string的开始位置”或“第n个匹配合格之substring的开始位置”
		**`begin()`、`cbegin()`、`end()`、`cend()`**：可用来迭代sub_match对象，从m[0]到m[n]

**sub_match对象：派生自pair<>**
		**`str()`**：以string形式取得字符
		**`length()`**：可取得字符数量
		**`first`**成员是第一字符的迭代器，**`second`**成员是最末字符的下一个位置
		**`operator <<`** ：将字符写至某个stream

