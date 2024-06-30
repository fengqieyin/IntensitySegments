#ifndef _INTENSITY_SEGMENT_H_
#define  _INTENSITY_SEGMENT_H_

#include <map>
#include <list>
#include <string>

class IntensitySegments {
public:
    /**
    * @ 将[from, to] 区间增加 amount
    */
    void add(int from, int to, int amount);
    /**
    * @ 将[from, to] 区间的内容设置为 amount
    */
    void set(int from, int to, int amount);
    /**
    * @ 将内容输出到终端并返回强度区间列表
    */
    std::string toString();

private:
    // 抽离封装函数使用
    void empty_insert(int from, int to, int amount);
    void insert_to(int from, int to, int amount);

    // 记录数据在链表中的所在位置，用于实现快速查找
    std::map<int, std::list<std::pair<int, int>>::iterator> m_location_map;
    // 存储区间信息
    std::list<std::pair<int, int>> m_segments_list;
};


#endif