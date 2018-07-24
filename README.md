# KCL
Kernel Context [template c++] Library - K C L. Your stl for work in linux/windows kernel !!!


Remark: not work with KCL_MEMORY_DEBUG, because debug analog std::map not finished
if you want use builtin powerfull memory leak tool, add to file kernel_memory_map.h implementation of map. For example, you can use this : https://github.com/attractivechaos/klib/blob/master/khash.h 

Built-in memory debug show you file, line, function where your memory leak, double deletion e.t.c.

But you MUST use KCL_ALLOCATE, KCL_CREATE_OBJECT, KCL_DELETE_OBJECT, KCL_DEALLOCATE, KCL_POINTER, KCL_WEAK_POINTER e.t.c. for correct work of built-in memory debug.
