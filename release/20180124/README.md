说明：
------------------------------------------------------------------------\
2018-1-24 00:25
临时调试版本
1.将orl和ref模块容量扩大，变为128；
2.日志模块完善一下，分割线变为|；
3.orl和ref模块可以记录log；

-------------------------------------------------------------------------
2018-01-22 23:25
临时调试版本：
1.在此前基础上打开snet和dnet模块，这两个模块主要用来设置黑名单；

-------------------------------------------------------------------------
2018-01-19 17:47
--------------------------------------------------------------------------
临时调试版本
--------------------------------------------------------------------------
版本说明：
1.增加FCT、ACL模块，FCT模块中包含：ori、ref、mask模块，ori、ref模块通过pcre模块进行匹配，
mask模块为符合规则。
2.ori模块支持按比例抽样，抽样比例为1-100，但是实际可用比例为1-50。
3.ref模块和mask模块抽样功能暂未测试。
4.在pid_url函数中增加了一个统计，统计参数解析错误情况。

cmd：
ori url add 1 ^sports\.sina\.com\.cn mask 0x1
ref url add 1 ^www\.sina\.com\.cn mask 0x2
mask add 1 0x3 redir http://sports.163.com 10

ori url add 2 ^wen\.xingsxj\.top mask 0x4
ref url add 2 ^www\.toutiao\.com mask 0x8
mask add 2 12 redir http://sports.163.com 10

ori url add 3 ^mp\.weixin\.qq\.com\/s\\Q__biz=MzI0OTcyMjU4Mg redir http://mp.weixin.qq.com/s?__biz=MzI4NjU0Mjc5Ng==&mid=2247485054&idx=1&sn=a2af00333 10