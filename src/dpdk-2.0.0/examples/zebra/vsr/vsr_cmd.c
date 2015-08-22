/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/mnt/hgfs/code/rose/src/dpdk-2.0.0/examples/zebra/vsr\vsr_cmd.c
#       @date         :2015/08/21 22:52
#       @algorithm    :
=============================================================================*/

/*
 * vsr module cmdline register and init 
 *
 * */
void cmdline_vsr_init(void)
{

    install_element(CONFIG_NODE, &vsr_add_cmd);

    return ;
}
