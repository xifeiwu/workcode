#include<glib.h> 
GMainLoop* loop;

gint counter = 10;
gboolean callback(gpointer arg)
{
    g_print(".");
    if(--counter ==0){
        g_print("\n");
        //退出循环
        g_main_loop_quit(loop);
        //注销定时器
        return FALSE;
    }
    //定时器继续运行
    return TRUE;
}

int main(int argc, char* argv[])
{
    if(g_thread_supported() == 0)
        g_thread_init(NULL);
    g_print("g_main_loop_new\n");
    loop = g_main_loop_new(NULL, FALSE);
    //增加一个定时器，100毫秒运行一次callback
    g_timeout_add(100,callback,NULL);
    g_print("g_main_loop_run\n");
    g_main_loop_run(loop);
    g_print("g_main_loop_unref\n");
    g_main_loop_unref(loop);
    return 0;
}
