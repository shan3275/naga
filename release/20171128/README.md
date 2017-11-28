说明：
本版本功能为监控输入接口，分析报文中以下成分：
1.HTTP GET 数量及字节数；
2.HTTP 200 OK 数量及字节数；
3.HTTP 302 数量及字节数；
输出：每分钟将统计数据，也即show stat命令的报文统计输出到文件中，供其他程序分析处理，记录格式如下：
        Statistics of date and time: 2017-11-28 09:09:58
        ITF_IPKTS                               : 413293228
        ITF_IBYTS                               : 239851099725
        OUTERL3_IPV4                            : 413141682
        OUTERL3_IPV6                            : 24355
        OUTERL3_NOIP                            : 127191
        OUTERL4_TCP                             : 413120034
        OUTERL4_UDP                             : 21648
        APP_HTTP                                : 390127481
        APP_HTTP_BYTES                          : 227769589760
        APP_OTHER                               : 22992553
        APP_OTHER_BYTES                         : 12060829842
        APP_HTTP_GET                            : 337198052
        APP_HTTP_GET_BYTES                      : 179650415949
        APP_HTTP_200                            : 52328660
        APP_HTTP_200_BYTES                      : 47995397471
        APP_HTTP_302                            : 541229
        APP_HTTP_302_BYTES                      : 119760092
        APP_HTTP_OTHER                          : 59540
