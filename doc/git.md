### **GIT**

`本地仓库`：`.git`文件夹就是本地仓库

`工作目录`：包含`.git`文件夹的目录为工作目录，工作目录下包含本地仓库、检出的分支的工作副本。

`文件状态`：指的是工作目录下文件副本的状态，`已跟踪`和`未跟踪`。未跟踪即`untracked`,已跟踪包括3种状态`unmodified`,`Modified`,`Staged`。

`远程分支`：远程仓库中的分支。例：master

`远程分支标签`：无法移动的本地引用，指示远程分支的状态，一旦进行了网络通信，Git就会为你移动它们以精确反映远程仓库的状态。例：origin/master

`本地跟踪分支`：本质上就是一个本地分支，但是该分支是从远程分支标签位置fork出来的，用来比对本地和远程的状态。



### **Git配置**

```shell
git config --global user.email "784723153@qq.com"
git config --global user.name "lhy"
git config --global core.editor vim

#公司仓库信息：
全名：lmxlhy926
电子邮件地址：784723153@qq.com

```



### 常用操作

**Untracked----Unmodified----Modified----Staged**

```shell
git status	#检查状态
git status -s #以简洁显示方式查看状态

#git仓库
git clone https://github.com/lmxlhy926/test.git //复制git仓库到本地
git init	#在目录下创建一个git仓库

#常用操作
git add *.c	#添加要追踪的文件	untrack->staged or modified->staged
git reset HEAD a.c	#对a.c使用git add暂存后，使用此命令，取消此次暂存	staged->modified
git checkout -- a.c	#对a.c进行修改后，丢弃该修改，退回到最后一次提交时的样子。	modified->unmodified
git rm a.c	#从staging area中移除该暂存文件（同时删除工作目录下的副本）	staging working
git rm --cached a.c	#从staging area移除该暂存文件，工作区的副本保持不变   staging		
git mv file_from file_to	#更改文件名字

#提交
git commit -m 'initial project version'	#将暂存区文件提交至本地仓库	staging->repository
git commit #使用默认编辑器来添加注释信息
git commit -a #将所有已经跟踪的文件暂存起来一并提交，跳过git add步骤
git commit --amend #覆盖上一次的提交，从提交历史上来看上一次提交不存在只有本次提交


#查看提交历史
git log #查看提交历史
```



### **GIT分支**

```shell
#分支的显示
git branch #显示当前所有分支
git branch -vv #查看设置的所有跟踪分支
git branch --merged #显示合并到当前分支的分支
git branch --no-merged #显示未合并到当前分支的分支
git branch --no-merged master #显示未合并到master的分支有哪些

#本地分支的创建、切换、合并、删除
git branch testing	#创建分支
git checkout testing #切换到testing分支
git checkout -b <newbranchname>	#创建一个新分支并切换过去
git merge hotfix #将hotfix分支合并到当前分支
git branch -d hotfix #删除hotfix分支

#本地分支与远程分支
git checkout -b <branch> <remote>/<branch>	
git checkout -b sf origin/serverfix #本地跟踪分支和要跟踪的远程分支名不同

#设置本地跟踪分支的一种捷径，本地跟踪分支名和远程分支名相同。
#添加远程跟踪分支，默认只自动给origin/master远程跟踪分支建立本地跟踪分支master
git checkout serverfix	#如果存在远程分支origin/serverfix,使用该命令后会在本地创建serverfix跟踪远程分支。

#设置已有的本地分支跟踪一个刚刚拉取下来的远程分支，或者想要修改正在跟踪的上游分支
git branch -u origin/serverfix

#删除远程分支
git push origin --delete serverfix #删除远程分支
```



### **远程仓库：**

```shell
#查看远程仓库
git remote #列出remote servers的简称
git remote -v #列出remote servers的简称以及URL
git remote show <remote> #列出remote servers的详细信息

#添加远程仓库名
git remote add <shortname> <url> #添加远程仓库
git remote rename pb paul #rename pb to paul
git remote remove pual #删除pual远程仓库

#从远程仓库拉取分支
git fetch <remote>	
git fetch origin #拉取服务器有的，本地没有的数据

#推送分支to远程仓库
git push <remote> <branch>	
git push origin serverfix #将本地的分支推送到服务器上的serverfix上

```



### **远程跟踪分支<->远程分支**


```shell

# 删除 untracked files
git clean -f
# 连 untracked 的目录也一起删掉
git clean -fd
# 连 gitignore 的untrack 文件/目录也一起删掉 （慎用，一般这个是用来删掉编译出来的 .o之类的文件用的）
git clean -xfd
 
# 在用上述 git clean 前，墙裂建议加上 -n 参数来先看看会删掉哪些文件，防止重要文件被误删
git clean -nxfd
git clean -nf
git clean -nfd
```





```
# 设置新的personal token
git remote set-url origin https://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@github.com/lmxlhy926/java.git
```



























