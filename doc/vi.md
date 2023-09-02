#### list of motions:

---

`0` - to the beginning of the current line.
`^` - to the first non-whitespace character on the current line
`$` - to the end of the line, INCLUDING the last character
`e` - to the end of the current word, INCLUDING the last character
`w` - until the start of the next word, EXCLUDING its first character
`b` - To the beginning of the previous word

`H	J	K	L`

`gg`：move to the start of the file.
`20G`：move to the nth line
`G`：move to the bottom of the file

`v`:	`Visual selection`


#### 插入：i	s	a	A	o	O

---

`i	s	a	A`		
	insert berfore the cursor	
	delete the current character and insert	
	insert after the cursor
	append after the line

`o`：open a line below the cursor and place you in Insert mode
`O`：open up a line ABOVE the cursor and place you in Insert mode

#### 修改：r	c	R	

---

`r`：replace the character under the cursor
`c`：change / delete and place you in Insert mode.		c0	c$	ce	cw	
`R`：Replace mode 

#### 复制：y	yy

---

`y`：yank (copy) text							y	y0	y^	y$	yw	ye	yb	ygg	yG	y20G
`yy`：yank (copy) a whole line		   yy	5yy	

#### 剪切：x	d	dd

---

`x`：delete the character under the cursor			x	3x
`d`：delete											d	d0	d^	d$	dw	de	db	dgg	dG	d20	
`dd`：delete a whole line				   dd	3dd															 

#### 粘贴：p	P

---

`p`：put previously deleted text after the cursor
`P`：put previously deleted text before the cursor

#### 搜索/替换：/	substitute

---

`/	n	N`
	/string-to-search`\c`： ignore case for just one search command

`:s/old/new`							substitute new for the first old in a line
`:s/old/new/g`						substitute new for all 'old's on a line
`:#,#s/old/new/g`				  substitute phrases between two line
`:%s/old/new/g`					  substitute all occurrences in the file
`:%s/old/new/gc`					ask for confirmation each time

​	`:set ic`：  ignore upper/lower case when searching
​	`:set is`：  show partial matches for a search phrase				
​	`:set hls`：highlight all matching phrases
​	`disable`：noic	nois	nohls

#### 控制：u	ctrl-r	ctrl-o	ctrl-i

---

`u`：undo the last command executed
`ctrl+r`：redo the commands (undo the undo's)
`ctrl-o`：go back to older positions
`ctrl-i`：to new positions

#### 末行模式

---

`:q	:q!`						   (exit	/	trash all changes and exit)
`:w	:wq`						（save	/	save and exit）
`:x`				  				（保存退出）
`:!`									(followed by an external command to execute that command)
`:w FILENAME`				  To save the changes made to the text
`:r FILENAME`	  			To insert the contents of a file

