#include "currentThread.h"
#include <unistd.h>
#include <sys/syscall.h>
namespace currentThread
{
    __thread int t_cacheTid = 0;

    void cacheTid()
    {
        if (t_cacheTid == 0)
        {
            // 通过linux系统调用获取当前的系统tid值
            t_cacheTid = static_cast<pid_t>(syscall(SYS_gettid));
        }
    }
}